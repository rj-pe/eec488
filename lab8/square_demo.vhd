-- square_demo.vhd
--======================================================================
-- Description: generate a square pattern: 
--   * 8 prime colors
--   * a square of 16, 32, 64, 128 pixels  
--======================================================================

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
entity square_demo is
   port(
      x, y       : in  unsigned(10 downto 0);      --treated as x-/y-axis
      y_start, y_stop   : in unsigned(8 downto 0);     -- vertical bounds
      x_start, x_stop   : in unsigned(8 downto 0);       -- horiz. bounds
      colors :  in std_logic_vector(2 downto 0);         -- desired color
      sq_rgb    : out std_logic_vector(11 downto 0)             -- output
   );
end square_demo;

architecture arch of square_demo is

   -- signals hold color channels
   signal r, g, b    : std_logic_vector(3 downto 0);
begin
   -- figure out what we need to draw
   process(x,y,colors)
   begin
      -- are we in the x and y ranges?
      if (y >= y_start and y < y_stop and x >= x_start and x < x_stop ) then
          -- draw square
          r <= colors(2) & colors(2) & colors(2) & colors(2);
          g <= colors(1) & colors(1) & colors(1) & colors(1);
          b <= colors(0) & colors(0) & colors(0) & colors(0);
      else
        -- no square
          r <= not (colors(2) & colors(2) & colors(2) & colors(2));
          g <= not (colors(1) & colors(1) & colors(1) & colors(1));
          b <= not (colors(0) & colors(0) & colors(0) & colors(0));
      end if;
   end process;
   -- output logic
   sq_rgb <= r & g & b;
end arch;
