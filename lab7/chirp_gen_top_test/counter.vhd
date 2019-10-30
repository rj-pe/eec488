-- file: counter.vhd
-- outputs a range of freq. from start_freq to stop_freq
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity counter is 
  generic(
    N : integer := 30;
    stop_freq : unsigned :=  to_unsigned(644, 30); --30Hz visual
    start_freq : unsigned := to_unsigned(2, 30) --1Hz visual
    );
  port(
    clk  : in std_logic;
    count_o : out std_logic_vector(N-1 downto 0)
  );
end counter;
architecture arch of counter is

signal count_reg, count_next : unsigned(N-1 downto 0);

begin
  --output logic
  count_o <= std_logic_vector(count_reg);
  -- register
  process(clk)
  begin
    if (clk'event and clk = '1') then
      count_reg <= count_next;
    end if;
  end process;
  -- next state logic
  count_next <= count_reg + 1 when count_reg < stop_freq else
            start_freq;
end arch; -- arch