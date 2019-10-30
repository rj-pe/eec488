-- file chirp_gen_top_test.vhd
-- testing circuit for top level design of ping generator

 library ieee;
 use ieee.std_logic_1164.all;
 use ieee.numeric_std.all;

entity chirp_gen_top_test is
  port(
    clk : in std_logic;
    sw: in std_logic_vector(0 downto 0);
    led : out std_logic_vector(0 downto 0)
	 -- arduino_io : out std_logic_vector(7 downto 7) 
  );
end chirp_gen_top_test;

architecture arch of chirp_gen_top_test is
  constant PRD: unsigned := "100011110000110100011000000"; -- 1.5 s
  constant M : integer := 17000;
  signal pulse_s, gate_s, trig_s: std_logic;
  signal count_s: std_logic_vector(29 downto 0);

begin
  trig_s <= sw(0);
  led(0) <= (gate_s and pulse_s);
  -- arduino_io(7) <= (gate_s and pulse_s);
  -- finite state machine produces gate signal
  fmsd_unit: entity work.fsmd
    port map(
      clk =>  clk,
      trig => trig_s,
      dvsr => PRD,
      gate => gate_s
    );
  -- counter unit for chirp 
  counter_unit: entity work.counter
    --generic map( -- for testing with audio frequencies
    --  start_freq => to_unsigned(25770, 30),
    --  stop_freq  => to_unsigned(17180, 30)
    --);
    port map(
      clk =>     clk,
      count_o => count_s
    );
  -- ddfs unit
  ddfs_unit: entity work.ddfs
    port map(
      clk =>       clk,
      reset =>     '0',
      fccw =>      count_s, -- gate input
      focw =>      (others => '0'),
      pha =>       (others => '0'),
      env =>       (others => '0'),
      pcm_out =>   open, 
      pulse_out => pulse_s
    );

end arch;
