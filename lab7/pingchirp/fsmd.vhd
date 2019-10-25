library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity fsmd is 
	generic (n : integer := 28 );
	port (
	clk		: in std_logic;
	trig		: in std_logic;
	dvsr		: in unsigned(n - 1 downto 0);
	gate		: out std_logic
	);
end fsmd;

architecture arch of fsmd is
	type state_type is (idle, counter);
	signal state_reg, state_next: state_type;
	signal count_reg, count_next : unsigned( n - 1 downto 0);
begin 
	-- update state and count
	process(clk)
	begin
		if (clk'event and clk='1') then
			state_reg <= state_next;
			count_reg <= count_next;
		end if;
	end process;
	
	-- next-state/output logic
	process(state_reg, trig) --does state_reg need to be included in this process?
	begin
		case state_reg is
			when idle =>
				gate <= '0';
				if trig = '1' then
					state_next <= counter;
				end if;
			when counter =>
				gate <= '1';
				if count_reg = dvsr then
					--count_reg <= (others => '0');
					state_next <= idle;
				else
					count_next <= count_reg + 1;
				end if;
		end case;
	end process;
end arch;