-- file test_top_yabl.vhd
library ieee;
use ieee.std_logic_1164.all;
entity blinky_engine_test_top is
port(
clk : in std_logic;
sw : in std_logic_vector(4 downto 0);
led : out std_logic_vector(1 downto 0)
);
end blinky_engine_test_top;
architecture arch of blinky_engine_test_top is
constant PRD0: std_logic_vector(9 downto 0) := "1111101000";
constant PRD1: std_logic_vector(9 downto 0) := "0111110100";
-- connects blinky engine to pwm
signal blink_signal0, blink_signal1 : std_logic;
signal duty_in0, duty_in1 : unsigned(2 downto 0);
-- calculate the desired brightness for each led
signal bright0 : integer : (to_integer(PRD0) / 4) * (1 + to_integer(sw(4 downto 2)));
signal bright1 : integer : (to_integer(PRD1) / 4) * (1 + to_integer(sw(4 downto 2)));
-- calculate the duty cycle for each led
duty_in0 => unsigned(bright0) / unsigned(PRD0) ;
duty_in1 => unsigned(bright1) / unsigned(PRD1) ;

begin
blink_unit : entity work.blinky_engine
port map(
clk => clk,
p0 => PRD0, -- 1000 ms
p1 => PRD1, -- 500 ms
go0 => sw(0),
go1 => sw(1),
bout0 => blink_signal0,
bout1 => blink_signal1
);

pwm_unit : entity work.pwm
port map(
clk => clk,
reset =>'0',
duty0 =>duty_in0,
duty1 =>duty_in0,
blinkyin0=> blink_signal0,
blinkyin1=> blink_signal1,
pwm_out0=>led(0),
pwm_out1=>led(1)
);
end arch;