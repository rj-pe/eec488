library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity fsdm_top is
	port(
		clk: in std_logic;
		sw: in std_logic_vector(0 downto 0);
		led: out std_logic_vector(0 downto 0)
	);
end fsdm_top;

architecture arch of fsdm_top is
	constant prd : unsigned := "0000011010111100001000000000";
	begin
		fsmd_unit: entity work.fsmd
		port map(
		clk => clk,
		trig => sw(0),
		dvsr => prd,
		gate => led(0)
		);
end arch; 