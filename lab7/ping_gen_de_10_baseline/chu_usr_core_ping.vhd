-- ping generator core via the chu_usr_core

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

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
      to_ucore   : in  std_logic_vector(29 downto 0);
      from_ucore : out std_logic_vector(7 downto 0)
   );
end chu_usr_core;

architecture ping_arch of chu_usr_core is
   signal fccw_reg : std_logic_vector(29 downto 0); -- holds fccw
   signal dvsr_reg : std_logic_vector(26 downto 0); -- holds dvsr
   signal trig_reg : std_logic;                     -- holds trig
   signal wr_en    : std_logic;                     -- enables write  
   signal wr_fccw  : std_logic;                     -- writes fccw
   signal wr_dvsr  : std_logic;                     -- writes dvsr   
   signal wr_trig  : std_logic;                     -- wirtes trig
begin
   -- instantiate ping generator circuit
   ping_unit : entity work.ping_gen_top
      port map(
         clk   => clk,
         trig_i  => trig_reg,
         ping_o  => from_ucore(0),
         prd_i   => unsigned(dvsr_reg),
         fccw_i  => to_integer(unsigned(fccw_reg))
      );
   -- i/o registers
   process(clk)
   begin
      if (clk'event and clk = '1') then
         if wr_fccw = '1' then
            fccw_reg  <= wr_data(29 downto 0);
         end if;
         if wr_dvsr = '1' then
            dvsr_reg  <= wr_data(26 downto 0);
         end if;
         if wr_trig = '1' then
            trig_reg  <= wr_data(0);
         end if;
      end if;
   end process;
   -- decoding circuit
   wr_en <= '1' when write = '1' and cs = '1' else '0';
   wr_fccw  <= '1' when addr = "00000" and wr_en = '1' else '0';
   wr_dvsr  <= '1' when addr = "00001" and wr_en = '1' else '0';
   wr_trig  <= '1' when addr = "00010" and wr_en = '1' else '0';
   -- unused output
   rd_data                <= (others => '0');
   from_ucore(7 downto 1) <= (others => '0');
end ping_arch;
