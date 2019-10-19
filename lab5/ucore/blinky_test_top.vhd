library ieee;
use ieee.std_logic_1164.all;
entity blinky_test_top is
   port(
      clk : in  std_logic;
      sw  : in  std_logic_vector(5 downto 0);
      led : out std_logic_vector(2 downto 0)
   );
end blinky_test_top;

architecture arch of blinky_test_top is
begin
   blink_unit : entity work.blinky_engine
      port map(
         clk   => clk,
         p0    => "1111101000",         -- 1000 ms
         p1    => "0111110100",         -- 500 ms
         p2    => "0001100100",         -- 100 ms
         ctr0   => sw(1 downto 0),
         ctr1   => sw(3 downto 2),
         ctr2   => sw(5 downto 4),
         bout0 => led(0),
         bout1 => led(1),
         bout2 => led(2)
      );
end arch;

