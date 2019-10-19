library ieee;
use ieee.std_logic_1164.all;
use work.chu_io_map.all;

entity nios_top_de10_bsaeline is
   generic(BRIDGE_BASE : std_logic_vector(31 downto 0) := x"C0000000");
   port(
      clk     : in  std_logic;
      --reset_n : in  std_logic;
      -- switches and LEDs
      sw      : in  std_logic_vector(9 downto 0);
      led     : out std_logic_vector(9 downto 0);
      btn   : in std_logic_vector(1 downto 0);
      -- 6-digit 7-seg LED 
      hex0, hex1 : out std_logic_vector(7 downto 0);
      hex2, hex3 : out std_logic_vector(7 downto 0);
      hex4, hex5 : out std_logic_vector(7 downto 0);
      -- spi accelerator
      acl_sclk     : out   std_logic;
      acl_mosi     : out   std_logic;
      acl_miso     : in    std_logic;
      acl_ss_n       : out   std_logic;
      -- gpio header
      gpio  : inout std_logic_vector(35 downto 0);
      arduino_io: inout std_logic_vector(15 downto 0);
      -- to vga monitor  
      hsync   : out std_logic;
      vsync   : out std_logic;
      rgb     : out std_logic_vector(11 downto 0)
   );
end nios_top_de10_bsaeline;

architecture arch of nios_top_de10_bsaeline is
    component nios_128K is
        port (
            clk_clk              : in  std_logic                     := 'X';             -- clk
            fpro_bus_fp_video_cs : out std_logic;                                        -- fp_video_cs
            fpro_bus_fp_mmio_cs  : out std_logic;                                        -- fp_mmio_cs
            fpro_bus_fp_wre      : out std_logic;                                        -- fp_wre
            fpro_bus_fp_rd       : out std_logic;                                        -- fp_rd
            fpro_bus_fp_addr     : out std_logic_vector(20 downto 0);                    -- fp_addr
            fpro_bus_fp_wr_data  : out std_logic_vector(31 downto 0);                    -- fp_wr_data
            fpro_bus_fp_rd_data  : in  std_logic_vector(31 downto 0) := (others => 'X'); -- fp_rd_data
            reset_reset_n        : in  std_logic                     := 'X'              -- reset_n
        );
    end component nios_128K;
   -- clk/reset related
   signal clk_25M, clk_50M : std_logic;
   signal locked, reset_sys : std_logic;
   -- fpro bus 
   signal fp_mmio_cs    : std_logic;
   signal fp_wr         : std_logic;
   signal fp_rd         : std_logic;
   signal fp_addr       : std_logic_vector(20 downto 0);
   signal fp_wr_data    : std_logic_vector(31 downto 0);
   signal fp_rd_data    : std_logic_vector(31 downto 0);
   signal fp_video_cs       : std_logic;
   -- gpi led
   signal led_gpi: std_logic_vector(9 downto 0);
   -- uart 
   signal tx, rx        : std_logic;
   signal tx2, rx2      : std_logic;
   -- pwm 
   signal pwm: std_logic_vector(7 downto 0); 
   -- ddfs/audio pdm 
   signal pdm             : std_logic;
   signal ddfs_sq_wave    : std_logic;
   -- tft spi 
   signal tft_sclk     : std_logic;
   signal tft_mosi     : std_logic;
   signal tft_miso     : std_logic;
   signal tft_ss     : std_logic_vector(3 downto 0);
   --
   signal neopixel       : std_logic;
   -- user core generic signals
   signal to_ucore: std_logic_vector(7 downto 0);
   signal from_ucore: std_logic_vector(7 downto 0);
begin
   -- clock and reset
   reset_sys <= not locked; -- or (not btn(1));
   -- instantiate clock management unit 
   pll_unit : entity work.de10_pll_fpro
      port map(
         clk_50M_ref    => clk,
         clk_50M_sys    => clk_50M,
         clk_100M_sys   => open,
         clk_100M_sdram => open, --dram_clk,
         clk_25M        => clk_25M,
         clk_65M        => open,
         pll_locked     => locked
      );

    u0 : component nios_128K
        port map (
            clk_clk              => clk_50M,         
            reset_reset_n        => not reset_sys,       
            fpro_bus_fp_video_cs => fp_video_cs, 
            fpro_bus_fp_mmio_cs  => fp_mmio_cs,  
            fpro_bus_fp_wre      => fp_wr,       
            fpro_bus_fp_rd       => fp_rd,       
            fpro_bus_fp_addr     => fp_addr,     
            fpro_bus_fp_wr_data  => fp_wr_data,  
            fpro_bus_fp_rd_data  => fp_rd_data   
        );

   -- instantiate daisy video subsystem
   video_sys_unit : entity work.video_sys_daisy_de10
      generic map(
         CD              => 12,
         VRAM_DATA_WIDTH => 9)
      port map(
         clk_sys       => clk_50M,
         clk_25M       => clk_25M,
         reset_sys     => reset_sys,
         video_cs      => fp_video_cs,
         video_wr      => fp_wr,
         video_addr    => fp_addr,
         video_wr_data => fp_wr_data,
         vsync         => vsync,
         hsync         => hsync,
         rgb           => rgb
      );

   mmio_sys_unit : entity work.mmio_sys_ultra_de10
      port map(
         clk          => clk_50M,
         reset        => reset_sys,
         mmio_cs      => fp_mmio_cs,
         mmio_wr      => fp_wr,
         mmio_rd      => fp_rd,
         mmio_addr    => fp_addr,
         mmio_wr_data => fp_wr_data,
         mmio_rd_data => fp_rd_data,
         sw           => sw,
         led          => led_gpi,
         rx           => rx,
         tx           => tx,
         rx2          => rx2,
         tx2          => tx2,
         pwm => pwm,
         btn => not btn,
         --an => open,
         --sseg => open,
         hex0    => hex0,
         hex1    => hex1,
         hex2    => hex2,
         hex3    => hex3,
         hex4    => hex4,
         hex5    => hex5,
         scl  => arduino_io(15),
         sda  => arduino_io(14),
         scl2  => gpio(24),
         sda2  => gpio(25),      
         acl_sclk     => acl_sclk,
         acl_mosi     => acl_mosi,
         acl_miso     => acl_miso,
         acl_ss       => acl_ss_n, 
         tft_sclk     => tft_sclk,
         tft_mosi     => tft_mosi,
         tft_miso     => tft_miso,
         tft_ss       => tft_ss,
         ps2d         => open, --ps2d,
         ps2c         => open, --ps2c,
         ddfs_sq_wave => ddfs_sq_wave,
         pdm          => pdm,
         neopixel     => neopixel,
         to_ucore     => to_ucore,
         from_ucore   => from_ucore      
      );
  --==========================================
  -- discrete led 
  --led <= led_gpi;
  led <= led_gpi(9 downto 3) & from_ucore(2 downto 0);
  --==========================================
  -- arduino header connection
  rx <= arduino_io(0);
  arduino_io(1) <= tx;     
  arduino_io(2) <= ddfs_sq_wave;
  arduino_io(3) <= pdm; 
  arduino_io(4) <= neopixel;
  arduino_io(7 downto 5) <= pwm(7 downto 5);
  arduino_io(8) <= tft_ss(2);
  arduino_io(9) <= tft_ss(1);
  arduino_io(10) <= tft_ss(0);
  arduino_io(11) <= tft_mosi;
  --tft_miso <= arduino_io(12);
  arduino_io(13) <= tft_sclk;
  --==========================================
  -- de10 lite 40-pin gpio header
  rx2 <= gpio(34);
  gpio(35) <= tx2;     
  to_ucore <= gpio(7 downto 0);  
  gpio(17 downto 10) <= from_ucore;
  gpio(33 downto 26) <= pwm;
end arch;

