#include "gpio_stm32f103.h"
#include "rcc_stm32f103.h"
#include "i2c.h"
#include "ht16k33.h"

int main()
 {
  Rcc rcc(Rcc::System_clock_source_pll_clock, Rcc::Hse_frequency_8Mhz);

  Pin  i2c_scl_pin  (GPIOB, 6, Pin::mode_alternate_function_open_drain);  //SCL //b8 remap
  Pin  i2c_sda_pin  (GPIOB, 7, Pin::mode_alternate_function_open_drain);  //SDA  //b9 remap

  I2C i2c (I2C1, rcc.GetPeripheralClock(I2C1), I2C::Speed_100kHz, I2C::Address_7bit);
  HT16K33_Display display(i2c_scl_pin, i2c_sda_pin, i2c, 0x70);
  display.Clear();
  while(1)
  {
    for(volatile auto i{0}; i<10000; ++i)
      display.PrintNumber(i);
      
    display.Clear();
  }
  return 0;
}