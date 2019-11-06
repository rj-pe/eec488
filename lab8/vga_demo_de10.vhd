library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
entity vga_demo_de10 is
   generic(
      CD : integer := 12  -- color depth
   );
   port(
      clk   : in  std_logic;
      -- switch control 
      sw    : in  std_logic_vector(13 downto 0);
      -- to vga monitor
      hsync : out std_logic;
      vsync : out std_logic;
      rgb   : out std_logic_vector(CD - 1 downto 0)
   );
end vga_demo_de10;

architecture arch of vga_demo_de10 is
   signal hc, vc      : std_logic_vector(10 downto 0);
   signal bar_rgb     : std_logic_vector(CD - 1 downto 0);
   signal back_r, back_g, back_b    : std_logic_vector(3 downto 0);
   signal back_rgb    : std_logic_vector(CD - 1 downto 0);
   signal gray_rgb    : std_logic_vector(CD - 1 downto 0);
   signal color_rgb   : std_logic_vector(CD - 1 downto 0);
   signal vga_rgb     : std_logic_vector(CD - 1 downto 0);
   signal y_start, y_stop : unsigned(8 downto 0);
   signal x_start, x_stop : unsigned(8 downto 0);
   signal sq_side : unsigned(8 downto 0);
   constant VD : integer := 480;  -- vertical display area
   constant HD : integer := 640;  -- horizontal display area
   signal bypass_bar  : std_logic;
   signal bypass_gray : std_logic;
begin
   -- select size of square based on user input
   with sw(1 downto 0) select 
      sq_side <= 
                 to_unsigned(16, 9) when "00",
                 to_unsigned(32, 9) when "01",
                 to_unsigned(64, 9) when "10",
                 to_unsigned(128, 9) when others;  

   -- compute start and stop points for colors
   --y_start <= (VD - sq_side) / 2;
   y_start <= (VD / 2) - sq_side + sq_side/ 2;
   y_stop  <= y_start + sq_side;

   x_start <= (HD /2)- sq_side + sq_side/2;
   x_stop <= x_start + sq_side ;

   -- instantiate bar generator
   bar_demo_unit : entity work.square_demo
      port map(
         x       => unsigned(hc),
         y       => unsigned(vc),
         sq_rgb  => bar_rgb,
         y_start => y_start,
         y_stop  => y_stop,
         x_start => x_start, 
         x_stop  => x_stop,
         colors => sw(4 downto 2)
         --r_color => sw(4),
         --g_color => sw(3), 
         --b_color => sw(2)
      );
   -- instantiate color-to-gray conversion circuit
   c2g_unit : entity work.rgb2gray
      port map(
         color_rgb => color_rgb,
         gray_rgb  => gray_rgb
      );
   -- instantiate video synchronization circuit
   sync_unit : entity work.vga_sync_demo_50M
      generic map(CD => CD)
      port map(
         clk        => clk,
         reset      => '0',
         vga_si_rgb => vga_rgb,
         hsync      => hsync,
         vsync      => vsync,
         rgb        => rgb,
         hc         => hc,
         vc         => vc
      );
   -- video source selection mux #1  
   color_rgb <= bar_rgb;
   -- video source selection mux #0  
   vga_rgb <= color_rgb;
end arch;
