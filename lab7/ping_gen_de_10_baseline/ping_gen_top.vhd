-- ping_gen_top.vhd
-- wrapping circuit for de-10 system interface

 library ieee;
 use ieee.std_logic_1164.all;
 use ieee.numeric_std.all;

entity ping_gen_top is
  generic(
    N: integer := 27
  );
  port(
    -- i / o for board
    clk  : in std_logic;
    --trig_i : in std_logic_vector(0 downto 0);
    trig_i : in std_logic;
    --ping_o : out std_logic_vector(0 downto 0);
    ping_o : out std_logic;
    -- i / o for application
    prd_i    : in unsigned(N-1 downto 0);
    fccw_i : in integer
  );
end ping_gen_top;

architecture arch of ping_gen_top is
  signal pulse_s, gate_s, trig_s: std_logic;
begin
  trig_s <= trig_i;
  ping_o <= (gate_s and pulse_s);

  fmsd_unit: entity work.fsmd
    port map(
      clk => clk,
      trig => trig_s,
      dvsr => prd_i,
      gate => gate_s
    );

  ddfs_unit: entity work.ddfs
    port map(
      clk => clk,
      reset => '0',
      fccw => std_logic_vector(to_signed(fccw_i, 30)),
      focw => (others => '0'),
      pha => (others => '0'),
      env => (others => '0'),
      pcm_out => open,
      pulse_out => pulse_s
    );

end arch;
