-- revision from textbook
--   * slot 4: dummy user core
--   * slot 5: altera max ADC
--   * slot 7: non-multiplexed 7-seg led
--   * slot 14: neopixel 
--   * slot 15: 2nd spi for tft
--   * slot 16: jtag uart
--   * slot 17: 2nd uart
--   * slot 18: 2nd i2c


library ieee;
use ieee.std_logic_1164.all;
use work.chu_io_map.all;

entity mmio_sys_ultra_de10 is
   port(
      -- FPro bus
      clk          : in    std_logic;
      reset        : in    std_logic;
      mmio_cs      : in    std_logic;
      mmio_wr      : in    std_logic;
      mmio_rd      : in    std_logic;
      mmio_addr    : in    std_logic_vector(20 downto 0); -- only 11 LSBs used
      mmio_wr_data : in    std_logic_vector(31 downto 0);
      mmio_rd_data : out   std_logic_vector(31 downto 0);
      -- switches and LEDs
      sw           : in    std_logic_vector(9 downto 0);
      led          : out   std_logic_vector(9 downto 0);
      -- uart
      rx           : in    std_logic;
      tx           : out   std_logic;
      -- 2nd uart
      rx2          : in    std_logic;
      tx2          : out   std_logic;
      -- pwm 
      pwm          : out   std_logic_vector(7 downto 0);
      -- btn
      btn          : in    std_logic_vector(1 downto 0);
      -- time-multplex LEDs
      --an           : out   std_logic_vector(7 downto 0);
      --sseg         : out   std_logic_vector(7 downto 0);
      -- 6-digit 7-seg LED 
      hex0, hex1 : out std_logic_vector(7 downto 0);
      hex2, hex3 : out std_logic_vector(7 downto 0);
      hex4, hex5 : out std_logic_vector(7 downto 0);     
      -- spi accelerator
      acl_sclk     : out   std_logic;
      acl_mosi     : out   std_logic;
      acl_miso     : in    std_logic;
      acl_ss       : out   std_logic;
      -- tft module
      tft_sclk     : out   std_logic;
      tft_mosi     : out   std_logic;
      tft_miso     : in    std_logic;
      tft_ss     : out   std_logic_vector(3 downto 0);
      -- i2c  
      scl  : out   std_logic;
      sda  : inout std_logic;
      -- 2nd i2c  
      scl2 : out   std_logic;
      sda2 : inout std_logic;
      -- ps2
      ps2d         : inout std_logic;
      ps2c         : inout std_logic;
      --ddfs square wave output
      ddfs_sq_wave : out   std_logic;
      -- 1-bit dac 
      pdm          : out   std_logic;
      -- 
      neopixel     : out   std_logic;
      -- user core generic input/output 
      -- R.J. Edit 7 -> 29 (10-28)
      to_ucore: in std_logic_vector(29 downto 0);
      from_ucore: out std_logic_vector(7 downto 0)   
  );
end mmio_sys_ultra_de10;

architecture arch of mmio_sys_ultra_de10 is
   signal cs_array       : std_logic_vector(63 downto 0);
   signal reg_addr_array : slot_2d_reg_type;
   signal mem_rd_array   : std_logic_vector(63 downto 0);
   signal mem_wr_array   : std_logic_vector(63 downto 0);
   signal rd_data_array  : slot_2d_data_type;
   signal wr_data_array  : slot_2d_data_type;
   -- adsr to ddfs conenction
   signal adsr_env       : std_logic_vector(15 downto 0);

begin
   --******************************************************************
   --  MMIO controller instantiation  
   --******************************************************************
   ctrl_unit : entity work.chu_mmio_controller
      port map(
         -- FPro bus interface
         mmio_cs             => mmio_cs,
         mmio_wr             => mmio_wr,
         mmio_rd             => mmio_rd,
         mmio_addr           => mmio_addr,
         mmio_wr_data        => mmio_wr_data,
         mmio_rd_data        => mmio_rd_data,
         -- 64 slot interface
         slot_cs_array       => cs_array,
         slot_reg_addr_array => reg_addr_array,
         slot_mem_rd_array   => mem_rd_array,
         slot_mem_wr_array   => mem_wr_array,
         slot_rd_data_array  => rd_data_array,
         slot_wr_data_array  => wr_data_array
      );

   --******************************************************************
   -- IO slots instantiations
   --******************************************************************
   -- slot 0: system timer 
   timer_slot0 : entity work.chu_timer
      port map(
         clk     => clk,
         reset   => reset,
         cs      => cs_array(S0_SYS_TIMER),
         read    => mem_rd_array(S0_SYS_TIMER),
         write   => mem_wr_array(S0_SYS_TIMER),
         addr    => reg_addr_array(S0_SYS_TIMER),
         rd_data => rd_data_array(S0_SYS_TIMER),
         wr_data => wr_data_array(S0_SYS_TIMER)
      );

   -- slot 1: uart1     
   uart1_slot1 : entity work.chu_uart
      generic map(FIFO_DEPTH_BIT => 6)
      port map(
         clk     => clk,
         reset   => reset,
         cs      => cs_array(S1_UART1),
         read    => mem_rd_array(S1_UART1),
         write   => mem_wr_array(S1_UART1),
         addr    => reg_addr_array(S1_UART1),
         rd_data => rd_data_array(S1_UART1),
         wr_data => wr_data_array(S1_UART1),
         -- external signals
         tx      => tx,
         rx      => rx
      );

   -- slot 2: GPO for 16 LEDs
   gpo_slot2 : entity work.chu_gpo
      generic map(W => 10)
      port map(
         clk     => clk,
         reset   => reset,
         cs      => cs_array(S2_LED),
         read    => mem_rd_array(S2_LED),
         write   => mem_wr_array(S2_LED),
         addr    => reg_addr_array(S2_LED),
         rd_data => rd_data_array(S2_LED),
         wr_data => wr_data_array(S2_LED),
         -- external signal
         dout    => led
      );

   -- slot 3: input port for slide switches     
   gpi_slot3 : entity work.chu_gpi
      generic map(W => 10)
      port map(
         clk     => clk,
         reset   => reset,
         cs      => cs_array(S3_SW),
         read    => mem_rd_array(S3_SW),
         write   => mem_wr_array(S3_SW),
         addr    => reg_addr_array(S3_SW),
         rd_data => rd_data_array(S3_SW),
         wr_data => wr_data_array(S3_SW),
         -- external signal
         din     => sw
      );

   -- slot 4: place holder for user core           
   user_slot4 : entity work.chu_usr_core
      port map(
         clk      => clk,
         reset    => reset,
         cs       => cs_array(S4_USER),
         read     => mem_rd_array(S4_USER),
         write    => mem_wr_array(S4_USER),
         addr     => reg_addr_array(S4_USER),
         rd_data  => rd_data_array(S4_USER),
         wr_data  => wr_data_array(S4_USER),
         to_ucore => to_ucore,
         from_ucore => from_ucore
      );


    -- slot 5: max10 adc           
    adc_slot5 : entity work.chu_max_adc_core
      port map(clk     => clk,
               reset   => reset,
               cs      => cs_array(S5_XADC),
               read    => mem_rd_array(S5_XADC),
               write   => mem_wr_array(S5_XADC),
               addr    => reg_addr_array(S5_XADC),
               rd_data => rd_data_array(S5_XADC),
               wr_data => wr_data_array(S5_XADC)
      );

   -- slot 6: pwm 
   pwm_slot6 : entity work.chu_io_pwm_core
      generic map(
         W => 8,
         R => 10)
      port map(
         clk     => clk,
         reset   => reset,
         cs      => cs_array(S6_PWM),
         read    => mem_rd_array(S6_PWM),
         write   => mem_wr_array(S6_PWM),
         addr    => reg_addr_array(S6_PWM),
         rd_data => rd_data_array(S6_PWM),
         wr_data => wr_data_array(S6_PWM),
         -- external interface
         pwm_out => pwm
      );


   -- slot 7: push button     
   debounce_slot7 : entity work.chu_debounce_core
      generic map(
         W => 2,
         N => 20
      )
      port map(
         clk     => clk,
         reset   => reset,
         cs      => cs_array(S7_BTN),
         read    => mem_rd_array(S7_BTN),
         write   => mem_wr_array(S7_BTN),
         addr    => reg_addr_array(S7_BTN),
         rd_data => rd_data_array(S7_BTN),
         wr_data => wr_data_array(S7_BTN),
         -- external interface
         din     => btn
      );
      
      
    -- slot 8: 6-digit_led     
   sseg6_lot8 : entity work.chu_6digit_led_core
      port map(
         clk     => clk,
         reset   => reset,
         cs      => cs_array(S8_SSEG),
         read    => mem_rd_array(S8_SSEG),
         write   => mem_wr_array(S8_SSEG),
         addr    => reg_addr_array(S8_SSEG),
         rd_data => rd_data_array(S8_SSEG),
         wr_data => wr_data_array(S8_SSEG),
         -- external interface
         hex0    => hex0,
         hex1    => hex1,
         hex2    => hex2,
         hex3    => hex3,
         hex4    => hex4,
         hex5    => hex5
      );
      
--   -- slot 8: 7-seg LED  (multiplexed)  
--   sseg_led_slot8 : entity work.chu_led_mux_core
--      port map(
--         clk     => clk,
--         reset   => reset,
--         cs      => cs_array(S8_SSEG),
--         read    => mem_rd_array(S8_SSEG),
--         write   => mem_wr_array(S8_SSEG),
--         addr    => reg_addr_array(S8_SSEG),
--         rd_data => rd_data_array(S8_SSEG),
--         wr_data => wr_data_array(S8_SSEG),
--         -- external interface
--         an      => an,
--         sseg    => sseg
--      );

   -- slot 9 SPI
   spi_slot9 : entity work.chu_spi_core
      generic map (S=>1)
      port map(clk      => clk,
               reset    => reset,
               cs       => cs_array(S9_SPI),
               read     => mem_rd_array(S9_SPI),
               write    => mem_wr_array(S9_SPI),
               addr     => reg_addr_array(S9_SPI),
               rd_data  => rd_data_array(S9_SPI),
               wr_data  => wr_data_array(S9_SPI),
               spi_sclk => acl_sclk,
               spi_mosi => acl_mosi,
               spi_miso => acl_miso,
               spi_ss_n(0)  => acl_ss
      );

   -- slot 10: i2C    
   i2c_slot10 : entity work.chu_i2c_core
      port map(clk     => clk,
               reset   => reset,
               cs      => cs_array(S10_I2C),
               read    => mem_rd_array(S10_I2C),
               write   => mem_wr_array(S10_I2C),
               addr    => reg_addr_array(S10_I2C),
               rd_data => rd_data_array(S10_I2C),
               wr_data => wr_data_array(S10_I2C),
               scl     => scl,
               sda     => sda
     );

   -- slot 11: PS2  
   ps2_slot11 : entity work.chu_ps2_core
      port map(
         clk     => clk,
         reset   => reset,
         cs      => cs_array(S11_PS2),
         read    => mem_rd_array(S11_PS2),
         write   => mem_wr_array(S11_PS2),
         addr    => reg_addr_array(S11_PS2),
         rd_data => rd_data_array(S11_PS2),
         wr_data => wr_data_array(S11_PS2),
         -- external interface
         ps2d    => ps2d,
         ps2c    => ps2c
      );

   -- slot 12: ddfs  
   ddfs_slot12 : entity work.chu_ddfs_core
      port map(
         clk         => clk,
         reset       => reset,
         cs          => cs_array(S12_DDFS),
         read        => mem_rd_array(S12_DDFS),
         write       => mem_wr_array(S12_DDFS),
         addr        => reg_addr_array(S12_DDFS),
         rd_data     => rd_data_array(S12_DDFS),
         wr_data     => wr_data_array(S12_DDFS),
         -- external interface
         focw_ext    => (others => '0'),
         pha_ext     => (others => '0'),
         env_ext     => adsr_env,      
         pcm_out     => open,
         digital_out => ddfs_sq_wave,
         pdm_out     => pdm
      );

   -- slot 13: adsr  
   adsr_slot13 : entity work.chu_adsr_core
      port map(
         clk      => clk,
         reset    => reset,
         cs       => cs_array(S13_ADSR),
         read     => mem_rd_array(S13_ADSR),
         write    => mem_wr_array(S13_ADSR),
         addr     => reg_addr_array(S13_ADSR),
         rd_data  => rd_data_array(S13_ADSR),
         wr_data  => wr_data_array(S13_ADSR),
         -- external interface
         adsr_env => adsr_env
      );
      
  -- slot 14: ws2812 LED (neopixel)
  neopixel_slot14 : entity work.chu_ws2812_core
      generic map(
        CLK_RATE   => 50,   -- system clock period in MHz
        ADDR_WIDTH => 8      -- # framer buffer size = 2^ADDR_WIDTH 
      )
      port map(
         clk     => clk,
         reset   => reset,
         cs      => cs_array(14),
         read    => mem_rd_array(14),
         write   => mem_wr_array(14),
         addr    => reg_addr_array(14),
         rd_data => rd_data_array(14),
         wr_data => wr_data_array(14),
         -- external interface
         sout => neopixel
      );

   -- slot 15: 2nd spi (4 ss for tft module w/ sd card and touch)              
   user_slot15 : entity work.chu_spi_core
      generic map (S=>4)
          port map(clk      => clk,
                   reset    => reset,
                   cs       => cs_array(15),
                   read     => mem_rd_array(15),
                   write    => mem_wr_array(15),
                   addr     => reg_addr_array(15),
                   rd_data  => rd_data_array(15),
                   wr_data  => wr_data_array(15),
                   spi_sclk => tft_sclk,
                   spi_mosi => tft_mosi,
                   spi_miso => tft_miso,
                   spi_ss_n => tft_ss
                  );


   -- slot 16: altera jtag_uart            
   --   jtag_uart_slot16 : entity work.chu_jtag_uart
   --      generic map(FIFO_DEPTH_BIT => 6)
   --      port map(
   --         clk     => clk,
   --         reset   => reset,
   --         cs      => cs_array(16),
   --         read    => mem_rd_array(16),
   --         write   => mem_wr_array(16),
   --         addr    => reg_addr_array(16),
   --         rd_data => rd_data_array(16),
   --         wr_data => wr_data_array(16)  
   --      );
   rd_data_array(16)<= (others => '0');


   -- slot 17: 2nd uart          
   uart2_slot17 : entity work.chu_uart
      generic map(FIFO_DEPTH_BIT => 6)
      port map(
         clk     => clk,
         reset   => reset,
         cs      => cs_array(17),
         read    => mem_rd_array(17),
         write   => mem_wr_array(17),
         addr    => reg_addr_array(17),
         rd_data => rd_data_array(17),
         wr_data => wr_data_array(17),
         -- external signals
         tx      => tx2,
         rx      => rx2
      );

   -- slot 18: 2nd i2c    
   i2c2_slot18 : entity work.chu_i2c_core
      port map(clk     => clk,
               reset   => reset,
               cs      => cs_array(18),
               read    => mem_rd_array(18),
               write   => mem_wr_array(18),
               addr    => reg_addr_array(18),
               rd_data => rd_data_array(18),
               wr_data => wr_data_array(18),
               scl     => scl2,
               sda     => sda2
     );
   
   
   -- assign 0's to all unused slot rd_data signals 
   gen_unused_slot : for i in 19 to 63 generate
      rd_data_array(i) <= (others => '0');
   end generate gen_unused_slot;
end arch;

