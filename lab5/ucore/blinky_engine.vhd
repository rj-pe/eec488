library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
entity blinky_engine is
   port(
      clk          : in  std_logic;
      ctr0, ctr1, ctr2     : in  std_logic_vector(1 downto 0);
      p0, p1, p2       : in  std_logic_vector(9 downto 0);
      bout0, bout1, bout2 : out std_logic
   );
end blinky_engine;

architecture arch of blinky_engine is
   constant CLK_PERIOD     : integer := 20; -- 20 ns (50 MHz)
   constant DVSR_1ms       : integer := 1000000 / 20;
   signal ms_reg, ms_next  : unsigned(16 downto 0);
   signal b2_reg, b2_next  : unsigned(9 downto 0);
   signal b1_reg, b1_next  : unsigned(9 downto 0);
   signal b0_reg, b0_next  : unsigned(9 downto 0);
   signal p0_half, p1_half, p2_half : unsigned(9 downto 0);
   signal ms_tick          : std_logic;
begin
   -- register
   process(clk)
   begin
      if (clk'event and clk = '1') then
         ms_reg <= ms_next;
         b2_reg <= b2_next;
         b1_reg <= b1_next;
         b0_reg <= b0_next;
      end if;
   end process;
   
   -- next-state logic
   -- 1-ms tick generator:
   ms_next <= (others => '0') when (ms_reg = DVSR_1ms - 1) else ms_reg + 1;
   ms_tick <= '1' when ms_reg = 0 else '0';

   -- counter/decoding for led #0 
   b0_next <= b0_reg when ms_tick = '0'
      else (others => '0') when ctr0(0) = '1'
      else (others => '1') when ctr0 = "00"
      else (others => '0') when b0_reg = unsigned(p0)
      else b0_reg + 1 when ctr0(1) = '1'
      else b0_reg;

   p0_half <= unsigned('0' & p0(9 downto 1));

   bout0   <= '0' when b0_reg < p0_half
                    else '1';

   -- counter/decoding for led #1 
   b1_next <= b1_reg when ms_tick = '0'
      else (others => '0') when ctr1(0) = '1'
      else (others => '1') when ctr1 = "00"
      else (others => '0') when b1_reg = unsigned(p1)
      else b1_reg + 1 when ctr1(1) = '1'
      else b1_reg;

   p1_half <= unsigned('0' & p1(9 downto 1));

   bout1   <= '0' when b1_reg < p1_half
                    else '1';

  -- counter/decoding for led #2 
   b2_next <= b2_reg when ms_tick = '0'
      else (others => '0') when ctr2(0) = '1'
      else (others => '1') when ctr2 = "00"
      else (others => '0') when b2_reg = unsigned(p2)
      else b2_reg + 1 when ctr2(1) = '1'
      else b2_reg;

   p2_half <= unsigned('0' & p2(9 downto 1));

   bout2   <= '0' when b2_reg < p2_half
                    else '1';

end arch;



