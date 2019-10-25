-- update from text: 
--   insert an output buffer for pcm_out to improve timing 
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
entity ddfs is
   generic(PW : integer := 30);   -- width of phase accumulator
   port(
      clk       : in  std_logic;
      reset     : in  std_logic;
      fccw      : in  std_logic_vector(PW - 1 downto 0); 
      focw      : in  std_logic_vector(PW - 1 downto 0); 
      pha       : in  std_logic_vector(PW - 1 downto 0); 
      env       : in  std_logic_vector(15 downto 0); 
      pcm_out   : out std_logic_vector(15 downto 0);
      pulse_out : out std_logic
   );
end ddfs;

architecture arch of ddfs is
   signal fcw, pcw          : unsigned(PW - 1 downto 0);
   signal p_reg, p_next     : unsigned(PW - 1 downto 0);
   signal p2a_raddr         : std_logic_vector(7 downto 0);
   signal amp               : std_logic_vector(15 downto 0);
   signal modu              : signed(31 downto 0);
   signal buf_reg, buf_next : std_logic_vector(15 downto 0); 
begin
   -- instantiate sin ROM
   sin_rom_unit : entity work.sin_rom
      port map(
         clk    => clk,
         addr_r => p2a_raddr,
         dout   => amp);
   -- phase register
   process(clk, reset)
   begin
      if reset = '1' then
         p_reg <= (others => '0');
         buf_reg <= (others => '0');
      elsif (clk'event and clk = '1') then
         p_reg <= p_next;
         buf_reg <= buf_next;
      end if;
   end process;
   -- frequency modulation
   fcw       <= unsigned(fccw) + unsigned(focw);
   -- phase accumulation 
   p_next    <= p_reg + fcw;
   -- phase modulation
   pcw       <= p_reg + unsigned(pha);
   -- phase to amplitude mapping address
   p2a_raddr <= std_logic_vector(pcw(PW-1 downto PW-8));
   -- amplitude modulation 
   modu      <= signed(env) * signed(amp); 
   buf_next  <= std_logic_vector(modu(29 downto 14));
   -- modulated and square-wave ouptut 
   pcm_out   <= buf_reg;
   pulse_out <= p_reg(PW - 1);
end arch;

