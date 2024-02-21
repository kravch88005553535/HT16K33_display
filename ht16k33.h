#ifndef __HT16K33__
#define __HT16K33__

#include "stm32f10x.h"
#include "gpio_stm32f103.h"
#include "i2c.h"

class HT16K33_Display
{
public:
  enum Brightness: uint8_t
  {
    Brightness_1 = 0,  Brightness_2,
    Brightness_3,      Brightness_4,
    Brightness_5,      Brightness_6,
    Brightness_7,      Brightness_8,
    Brightness_9,      Brightness_10,
    Brightness_11,     Brightness_12,
    Brightness_13,     Brightness_14,
    Brightness_15,     Brightness_16
  };
  
  enum Position: uint8_t
  {
    Position_0 = 0,
    Position_1,
    Position_2,
    Position_3,
    Position_NONE
  };

  enum Blink: uint8_t
  {
    Blink_OFF = 0x81,
    Blink_2Hz = 0x83,
    Blink_1Hz = 0x85,
    Blink_0_5Hz = 0x87
  };
  
  
  explicit HT16K33_Display(I2C& aref_i2c, const uint8_t a_i2c_address);
  ~HT16K33_Display();
  
  void TurnDisplayOn();
  void TurnDisplayOff();
  
  void SetBrightness(const Brightness a_brightness);
  Brightness GetBrightness() const;
  void IncrementBrightness();
  void DecrementBrightness();
  void Clear();
  
  void PrintNumber(uint32_t a_decimal_number, const Position a_separator_position = Position_NONE);
  void PrintFloatNumber(float a_number); 
  void PrintString(const char* a_string);
  void SetBlink(const Blink a_blink);
  
private:
  HT16K33_Display() = delete;

  void TransmitData(const Position a_position, const uint8_t a_byte1, const uint8_t a_byte2);
  void TransmitData(const Position a_position, const uint16_t a_byte12);
  void TransmitCommand(const uint8_t a_command);

  uint16_t DigitToSymbol(const uint8_t a_digit) const;
  uint16_t CharacterToSymbol(const uint8_t a_character) const;

  I2C& mref_i2c;
  Brightness m_brightness;
  Blink m_blink_frequency;
  const uint8_t m_i2c_address;
};

#endif //__HT16K33__