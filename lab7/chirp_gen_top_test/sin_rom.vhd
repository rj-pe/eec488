-- file sin_rom.vhd
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
entity sin_rom is
  generic( ADDR_WIDTH: integer:= 8; DATA_WIDTH: integer:= 16);
  port(
    clk: in std_logic;
    addr_r:  in std_logic_vector(ADDR_WIDTH-1 downto 0);
    dout: out std_logic_vector(DATA_WIDTH-1 downto 0)
  );
end sin_rom;

architecture beh_arch of sin_rom is
  type rom_type is array (0 to 2**ADDR_WIDTH-1)
    of std_logic_vector(DATA_WIDTH-1 downto 0);
  -- sinusoidal LUT
  constant SIN_LUT: rom_type:=( --2^8 by 16
      x"0000", x"0324", x"0648", x"096B", x"0C8C", x"0FAB", x"12C8", 
      x"15E2", x"18F9", x"1C0C", x"1F1A", x"2224", x"2528", x"2827",
      x"2B1F", x"2E11", x"30FC", x"33DF", x"36BA", x"398D", x"3C57",
      x"3F17", x"41CE", x"447B", x"471D", x"49B4", x"4C40", x"4EC0",
      x"5134", x"539B", x"55F6", x"5843", x"5A82", x"5CB4", x"5ED7",
      x"60EC", x"62F2", x"64E9", x"66D0", x"68A7", x"6A6E", x"6C24",
      x"6DCA", x"6F5F", x"70E3", x"7255", x"73B6", x"7505", x"7642",
      x"776C", x"7885", x"798A", x"7A7D", x"7B5D", x"7C2A", x"7CE4",
      x"7D8A", x"7E1E", x"7E9D", x"7F0A", x"7F62", x"7FA7", x"7FD9",
      x"7FF6", x"7FFF",-- 90 degrees
      x"7FF6", x"7FD9", x"7FA7", x"7F62", x"7F0A",
      x"7E9D", x"7E1E", x"7D8A", x"7CE4", x"7C2A", x"7B5D", x"7A7D",
      x"798A", x"7885", x"776C", x"7642", x"7505", x"73B6", x"7255",
      x"70E3", x"6F5F", x"6DCA", x"6C24", x"6A6E", x"68A7", x"66D0",
      x"64E9", x"62F2", x"60EC", x"5ED7", x"5CB4", x"5A82", x"5843",
      x"55F6", x"539B", x"5134", x"4EC0", x"4C40", x"49B4", x"471D",
      x"447B", x"41CE", x"3F17", x"3C57", x"398D", x"36BA", x"33DF",
      x"30FC", x"2E11", x"2B1F", x"2827", x"2528", x"2224", x"1F1A",
      x"1C0C", x"18F9", x"15E2", x"12C8", x"0FAB", x"0C8C", x"096B",
      x"0648", x"0324", x"0000", -- 180 degrees
      x"FCDC", x"F9B8", x"F695", x"F374", 
      x"F055", x"ED38", x"EA1E", x"E707", x"E3F4", x"E0E6", x"DDDC",
      x"DAD8", x"D7D9", x"D4E1", x"D1EF", x"CF04", x"CC21", x"C946", 
      x"C673", x"C3A9", x"C0E9", x"BE32", x"BB85", x"B8E3", x"B64C", 
      x"B3C0", x"B140", x"AECC", x"AC65", x"AA0A", x"A7BD", x"A57E",
      x"A34C", x"A129", x"9F14", x"9D0E", x"9B17", x"9930", x"9759",
      x"9592", x"93DC", x"9236", x"90A1", x"8F1D", x"8DAB", x"8C4A",
      x"8AFB", x"89BE", x"8894", x"877B", x"8676", x"8583", x"84A3",
      x"83D6", x"831C", x"8276", x"81E2", x"8163", x"80F6", x"809E",
      x"8059", x"8027", x"800A", x"8001", -- 270 degrees
      x"800A", x"8027", x"8059",
      x"809E", x"80F6", x"8163", x"81E2", x"8276", x"831C", x"83D6",
      x"84A3", x"8583", x"8676", x"877B", x"8894", x"89BE", x"8AFB",
      x"8C4A", x"8DAB", x"8F1D", x"90A1", x"9236", x"93DC", x"9592",
      x"9759", x"9930", x"9B17", x"9D0E", x"9F14", x"A129", x"A34C",
      x"A57E", x"A7BD", x"AA0A", x"AC65", x"AECC", x"B140", x"B3C0",
      x"B64C", x"B8E3", x"BB85", x"BE32", x"C0E9", x"C3A9", x"C673",
      x"C946", x"CC21", x"CF04", x"D1EF", x"D4E1", x"D7D9", x"DAD8",
      x"DDDC", x"E0E6", x"E3F4", x"E707", x"EA1E", x"ED38", x"F055",
      x"F374", x"F695", x"F9B8", x"FCDC"); -- 360 degrees
  signal rom: rom_type := SIN_LUT;

begin
  process(clk)
  begin
    if (clk'event and clk='1') then
      dout <= rom(to_integer(unsigned(addr_r)));
    end if;
  end process;
end beh_arch;