-- 3 blinking LED core via the chu_usr_core

library ieee;
use ieee.std_logic_1164.all;

entity chu_usr_core is
   port(
      clk        : in  std_logic;
      reset      : in  std_logic;
      -- io bridge interface
      cs         : in  std_logic;
      write      : in  std_logic;
      read       : in  std_logic;       -- not used 
      addr       : in  std_logic_vector(4 downto 0);
      rd_data    : out std_logic_vector(31 downto 0);
      wr_data    : in  std_logic_vector(31 downto 0);
      -- external interface
      to_ucore   : in  std_logic_vector(7 downto 0);
      from_ucore : out std_logic_vector(7 downto 0)
   );
end chu_usr_core;

architecture blink_arch of chu_usr_core is
   signal p0_reg, p1_reg, p2_reg     : std_logic_vector(9 downto 0);
   signal cmd0_reg, cmd1_reg, cmd2_reg : std_logic_vector(1 downto 0);
   signal wr_en              : std_logic;
   signal wr_0, wr_1, wr_2        : std_logic;
begin
   -- instantiate blinking circuit
   blink_unit : entity work.blinky_engine
      port map(
         clk   => clk,
         p0    => p0_reg,
         p1    => p1_reg,
         p2    => p2_reg,
         ctr0   => cmd0_reg,
         ctr1   => cmd1_reg,
         ctr2   => cmd2_reg,
         bout0 => from_ucore(0),
         bout1 => from_ucore(1),
         bout2 => from_ucore(2)
      );
   -- i/o registers
   process(clk)
   begin
      if (clk'event and clk = '1') then
         if wr_0 = '1' then
            p0_reg   <= wr_data(9 downto 0);
            cmd0_reg <= wr_data(16 downto 15);
         end if;
         if wr_1 = '1' then
            p1_reg   <= wr_data(9 downto 0);
            cmd1_reg <= wr_data(16 downto 15);
         end if;
          if wr_2 = '1' then
            p2_reg   <= wr_data(9 downto 0);
            cmd2_reg <= wr_data(16 downto 15);
         end if;
      end if;
   end process;
   -- decoding circuit
   wr_en <= '1' when write = '1' and cs = '1' else '0';
   wr_0  <= '1' when addr = "00000" and wr_en = '1' else '0';
   wr_1  <= '1' when addr = "00001" and wr_en = '1' else '0';
   wr_2  <= '1' when addr = "00010" and wr_en = '1' else '0';

   -- unused output
   rd_data                <= (others => '0');
   from_ucore(7 downto 2) <= (others => '0');
end blink_arch;
