library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
entity frame_counter is
   generic(
      HMAX : integer := 640;            -- max horizontal count
      VMAX : integer := 480             -- max vertical count
   );
   port(
      clk, reset  : in  std_logic;
      inc         : in  std_logic;
      sync_clr    : in  std_logic;
      hcount      : out std_logic_vector(10 downto 0);
      vcount      : out std_logic_vector(10 downto 0);
      frame_start : out std_logic;
      frame_end   : out std_logic
   );
end frame_counter;

architecture arch of frame_counter is
   signal hc_reg, hc_next : unsigned(10 downto 0);
   signal vc_reg, vc_next : unsigned(10 downto 0);
begin
   -- horizontal and vertical pixel counters 
   -- registers
   process(clk, reset)
   begin
      if reset = '1' then
         vc_reg <= (others => '0');
         hc_reg <= (others => '0');
      elsif (clk'event and clk = '1') then
         if (sync_clr = '1') then
            vc_reg <= (others => '0');
            hc_reg <= (others => '0');
         else
            vc_reg <= vc_next;
            hc_reg <= hc_next;
         end if;
      end if;
   end process;
   -- next-state logic of horizontal counter
   process(hc_reg, inc)
   begin
      if (inc = '1') then
         if hc_reg = (HMAX - 1) then
            hc_next <= (others => '0');
         else
            hc_next <= hc_reg + 1;
         end if;
      else
         hc_next <= hc_reg;
      end if;
   end process;
   -- next-state logic of vertical counter
   process(vc_reg, hc_reg, inc)
   begin
      if (inc = '1') and hc_reg = (HMAX - 1) then
         if vc_reg = (VMAX - 1) then
            vc_next <= (others => '0');
         else
            vc_next <= vc_reg + 1;
         end if;
      else
         vc_next <= vc_reg;
      end if;
   end process;
   -- output
   hcount      <= std_logic_vector(hc_reg);
   vcount      <= std_logic_vector(vc_reg);
   frame_start <= '1' when vc_reg = 0 and hc_reg = 0 else '0';
   frame_end   <= 
     '1' when vc_reg = (VMAX - 1) and hc_reg = (HMAX - 1) else '0';
end arch;
