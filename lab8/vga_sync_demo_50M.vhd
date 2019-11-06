library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
entity vga_sync_demo_50M is
   generic(CD : integer := 12);    -- color depth
      port(
      clk, reset : in  std_logic;
      -- stream input
      vga_si_rgb : in  std_logic_vector(CD - 1 downto 0);
      -- to vga monitor
      hsync  : out std_logic;
      vsync  : out std_logic;
      rgb    : out std_logic_vector(CD - 1 downto 0);
      -- frame counter output
      hc, vc : out std_logic_vector(10 downto 0)
   );
end vga_sync_demo_50M;

architecture arch of vga_sync_demo_50M is
   -- vga 640-by-480 sync parameters
   constant HD : integer := 640;  -- horizontal display area
   constant HF : integer := 16;   -- h. front porch
   constant HB : integer := 48;   -- h. back porch
   constant HR : integer := 96;   -- h. retrace
   constant HT : integer := HD+HF+HB+HR; -- horizontal total (800)
   constant VD : integer := 480;  -- vertical display area
   constant VF : integer := 10;   -- v. front porch
   constant VB : integer := 33;   -- v. back porch
   constant VR : integer := 2;    -- v. retrace
   constant VT : integer := VD+VF+VB+VR; -- vertical total (525)
   -- sync counetr and signals
   signal x, y       : unsigned(10 downto 0);
   signal hcount     : std_logic_vector(10 downto 0);
   signal vcount     : std_logic_vector(10 downto 0);
   signal hsync_i    : std_logic;
   signal vsync_i    : std_logic;
   signal video_on_i : std_logic;
   --signal q_reg      : unsigned(1 downto 0); -- for 100 Mhz 
   signal q_reg      : unsigned(0 downto 0);   -- for 50 Mhz 
   signal tick_25M   : std_logic;
begin
   process(clk)
   begin
      if (clk'event and clk = '1') then
         q_reg <= q_reg + 1;
      end if;
   end process;
   -- tick_25M <= '1' when q_reg = "11" else '0';  -- 100 MHz
   tick_25M <= '1' when q_reg = "1" else '0';      -- 50 MHz
   -- instantiate frame counter
   counter_unit : entity work.frame_counter
      generic map(
         HMAX => HT,
         VMAX => VT
      )
      port map(
         clk         => clk,
         reset       => reset,
         sync_clr    => '0',
         hcount      => hcount,
         vcount      => vcount,
         inc         => tick_25M,
         frame_start => open,
         frame_end   => open
      );
   x  <= unsigned(hcount);
   y  <= unsigned(vcount);
   hc <= hcount;
   vc <= vcount;

   -- horizontal sync decoding
   hsync_i <= '0' when (x>=(HD+HF)) and (x<=(HD+HF+HR-1)) else '1';
   -- vertical sync decoding
   vsync_i <= '0' when (y>=(VD+VF)) and (y<=(VD+VF+VR-1)) else '1';
   -- display on/off
   video_on_i <= '1' when (x < HD) and (y < VD) else '0';
   -- buffered output to vga monitor
   process(clk)
   begin
      if (clk'event and clk = '1') then
         vsync <= vsync_i;
         hsync <= hsync_i;
         if (video_on_i = '1') then
            rgb <= vga_si_rgb;
         else
            rgb <= (others => '0');    -- black when display off 
         end if;
      end if;
   end process;
end arch;
