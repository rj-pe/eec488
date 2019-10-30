-- file ping_gen_top_test.vhd
-- testing circuit for top level design of ping generator

 library ieee;
 use ieee.std_logic_1164.all;
 use ieee.numeric_std.all;

entity ping_gen_top_test is
  --generic(
  --  N: integer := 30;
  --  SYS_FRQ: integer := 50000000;
  --  PING_FRQ: integer := 20
  --);
  port(
    clk : in std_logic;
    sw: in std_logic_vector(0 downto 0);
    --led : out std_logic_vector(2 downto 0)
    --led : out std_logic_vector(0 downto 0)
	 arduino_io : out std_logic_vector(7 downto 7) 
  );
end ping_gen_top_test;

architecture arch of ping_gen_top_test is
  constant PRD: unsigned := "100011110000110100011000000"; -- 1.5 s
  constant PING_M : integer := 17000;
  --signal PING_M : integer:= (PING_FRQ / SYS_FRQ) * (2**N);  -- frequency tuning word
  signal pulse_s, gate_s, trig_s: std_logic;
begin
  trig_s <= sw(0);
  --led(0) <= (gate_s and pulse_s);
  arduino_io(7) <= (gate_s and pulse_s);
  -- for debugging
  --led(1) <= pulse_s;
  --led(2) <= gate_s;

  fmsd_unit: entity work.fsmd
    port map(
      clk => clk,
      trig => trig_s,
      dvsr => PRD,
      gate => gate_s
    );
  -- add a counter unit here for chirp

  ddfs_unit: entity work.ddfs
    port map(
      clk => clk,
      reset => '0',
      fccw => std_logic_vector(to_unsigned(PING_M, 30)),
      focw => (others => '0'), -- attach counter output here for chirp
      pha => (others => '0'),
      env => (others => '0'),
      pcm_out => open,
      pulse_out => pulse_s
    );

end arch;
