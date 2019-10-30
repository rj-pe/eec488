-- file pwm.vhd
-- 2 x 2 pwm for yet another blinky engine lab
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity pwm is
  generic(
    R : integer := 8 -- PWM resolution
  );
  port(
    clk : in std_logic;
    reset : in std_logic;
    -- duty cycle for led0
    duty0 : in std_logic_vector(R downto 0);
    -- duty cycle for led1
    duty1 : in std_logic_vector(R downto 0);
    -- input from blinky engine 0
    blinkyin0 : in std_logic;
    -- input from blink engine 1
    blinkyin1 : in std_logic;
    -- output for led0
    pwm_out0 : out std_logic
    -- output for led1
    pwm_out1 : out std_logic
  );
end pwm;

end architecture arch of pwm is
  signal d_reg0, d_next0 : unsigned(R - 1 downto 0);
  signal d_reg1, d_next1 : unsigned(R - 1 downto 0);
  signal pwm_reg0, pwm_next0 : std_logic;
  signal pwm_reg1, pwm_next1 : std_logic;
begin
  -- register
  process(clk, reset)
  begin
    if reset = '1' then
      d_reg0 <= (others => '0');
      d_reg1 <= (others => '0');
      pwm_reg0 <= '0';
      pwm_reg1 <= '0';
    elsif (clk'event and clk = '1') then
      d_reg0 <= d_next0;
      d_reg1 <= d_next1;
      pwm_reg0 <= pwm_next0;
      pwm_reg1 <= pwm_next1;
    end if; 
  end process; -- register
  -- duty cycle counter
  d_next0 <= d_reg0 + 1;
  d_next1 <= d_reg1 + 1;
  --comparison circuit
  pwm_next0 <= '1' when ( (d_reg0 < unsigned(duty0)) and blinkyin0 )
    else '0';
  pwm_next1 <= '1' when ( (d_reg1 < unsigned(duty1)) and blinkyin1 )
    else '0';  
  --output
  pwm_out0 <= pwm_reg0;
  pwm_out1 <= pwm_reg1;
end architecture ; -- arch