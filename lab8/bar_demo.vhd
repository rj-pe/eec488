--======================================================================
-- Description: generate a 3-level test bar pattern:
--   * gray scale 
--   * 8 prime colors
--   * a continuous color spectrum
--   * it is customized for 12-bit VGA
--   * two registers form 2-clock delay line  
--======================================================================

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
entity bar_demo is
   port(
      x, y       : in  std_logic_vector(10 downto 0); --treated as x-/y-axis
      bar_rgb    : out std_logic_vector(11 downto 0)
   );
end bar_demo;

architecture arch of bar_demo is
   -- intermdeiate counter value
   signal up, down   : std_logic_vector(3 downto 0);
   -- misc signals
   signal r, g, b    : std_logic_vector(3 downto 0);
begin
   up   <= x(6 downto 3);
   down <= not x(6 downto 3);           -- "not" reverse the binary sequence 
   process(x, y, up, down)
   begin
      -- 16 shades of gray
      if unsigned(y) < 128 then
         r <= x(8 downto 5);
         g <= x(8 downto 5);
         b <= x(8 downto 5);
      -- 8 prime colcor with 50% intensity
      elsif unsigned(y) < 256 then
         r <= x(8) & x(8) & "00";
         g <= x(7) & x(7) & "00";
         b <= x(6) & x(6) & "00";
      -- a continuous color spectrum 
      -- width of up/sown can be increased to accommodate finer spectrum
      -- see Fig 23 of http://en.wikipedia.org/wiki/HSL_and_HSV
      else
         case x(9 downto 7) is
            when "000" =>
               r <= (others => '1');
               g <= up;
               b <= "0000";
            when "001" =>
               r <= down;
               g <= "1111";
               b <= "0000";
            when "010" =>
               r <= "0000";
               g <= "1111";
               b <= up;
            when "011" =>
               r <= "0000";
               g <= down;
               b <= "1111";
            when "100" =>
               r <= up;
               g <= "0000";
               b <= "1111";
            when "101" =>
               r <= "1111";
               g <= "0000";
               b <= down;
            when others =>
               r <= "1111";
               g <= "1111";
               b <= "1111";
         end case;
      end if;
   end process;
   bar_rgb <= r & g & b;
end arch;
