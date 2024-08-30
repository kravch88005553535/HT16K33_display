#include <cstring>
#include "ht16k33.h"


// bit position of segments:
//
//       ------1------
//      | \    |    / |
//      |  \   |   /  |
//      6   9 10  11  2
//      |    \ | /    |
//      |     \|/     |
//       --7--   --8--
//      |     /|\     |
//      |    / | \    |
//      5  12 13  14  3
//      |  /   |   \  |
//      | /    |    \ |
//       ------4------ ****
//                     *15*
//                     ****


HT16K33_Display::HT16K33_Display(I2C& aref_i2c, const uint8_t a_i2c_address)
  : mref_i2c{aref_i2c}
  , m_brightness{Brightness::Brightness_16}
  , m_blink_frequency{Blink_Off}
  , m_i2c_address{static_cast<uint8_t>(a_i2c_address << 1)}
  , m_update_timer{* new Program_timer(Program_timer::TimerType_loop)}
  , m_digits{Digits_4}
  , m_string_buffer{nullptr}
  , m_string_ptr_offset{0}  
  , m_string_length{0}
  , m_display_buffer{new uint16_t[m_digits]}
  , m_status{Status_ok}
{
  TransmitCommand(0x21);    //turn display's oscillator on
  TurnDisplayOn();
  SetBrightness(m_brightness);
  SetBlink(Blink_Off);
  Clear();
  Update();
}

HT16K33_Display::~HT16K33_Display()
{
  delete[] m_display_buffer;
  delete  &m_update_timer;
}

void HT16K33_Display::TurnDisplayOn()
{
  TransmitCommand(0x81);
}
  
void HT16K33_Display::TurnDisplayOff()
{
  TransmitCommand(0x80);
}
  
void HT16K33_Display::SetBrightness(const Brightness a_brightness)
{
  m_brightness = a_brightness;
  TransmitCommand(0xE0 | a_brightness);
}

HT16K33_Display::Brightness HT16K33_Display::GetBrightness() const
{
  return m_brightness;
}

void HT16K33_Display::IncrementBrightness()
{
  if(m_brightness == Brightness_16)
    return;
  
  m_brightness = static_cast<Brightness>(m_brightness + 1);
  SetBrightness(m_brightness);
}

void HT16K33_Display::DecrementBrightness()
{
  if(m_brightness == Brightness_1)
    return;
  
  m_brightness = static_cast<Brightness>(m_brightness - 1);  
  SetBrightness(m_brightness);
}

void HT16K33_Display::Clear()
{
  ClearDisplayBuffer();
  Update();
}  

void HT16K33_Display::PrintNumber(uint32_t a_decimal_number, const Position a_separator_position)
{
  m_string_buffer = nullptr;
  m_status = Status_ok;
  ClearDisplayBuffer();
  
  for (volatile auto i{0}; i < m_digits; ++i)
  {
    const auto index{m_digits-1-i};
    m_display_buffer[index] = DigitToSymbol(a_decimal_number % 10);
    a_decimal_number /= 10;
    
    if(!a_decimal_number)
      break;
  }
  constexpr auto separator_bitmask{0x4000};
  if(a_separator_position != Position_NONE)
    m_display_buffer[a_separator_position] |= separator_bitmask;
  
  Update();
  m_update_timer.Reload();
  m_update_timer.Start();
}

void HT16K33_Display::PrintFloatNumber(float a_number)
{
  m_string_buffer = nullptr;
  m_status = Status_ok;
  
  if (a_number < 0 || a_number > 99.9)
    return;
  else PrintNumber(a_number*100, Position_1);
}

void HT16K33_Display::PrintString(const char* a_string)
{
  m_status = Status_on_printing_string;
  m_string_buffer = a_string;
  m_string_ptr_offset = 0;
  m_string_length = strlen(a_string);
  ClearDisplayBuffer();
  if(!m_string_length)
    return;
  //print operation
  uint8_t commas_number{0};
  for (volatile auto i{0}; i < m_digits; ++i)
  {
    m_digits[i] = CharacterToSymbol(m_string_buffer[i+m_string_ptr_offset+commas_number])
    
    if(m_digits[i+1] == '.' or m_digits[i+1] == ',')
    {
      ++commas_number;
      constexpr auto comma_bitmask{0x4000};
      m_digits[i] |= comma_bitmask;
    }
  }
  Update();
}

void HT16K33_Display::UpdateString()
{
  if(m_string_length <= m_digits)
  {
    m_string_ptr_offset = 0;
    return;
  }
  else
    ClearDisplayBuffer();
  
  auto symbols_remaining{m_string_length - m_string_ptr_offset};
  if(symbols_remaining < 0)
    m_string_ptr_offset = 0;
  
  for (volatile auto i{0}; i < m_digits; ++i)
  {
    if(m_string_buffer[i+m_string_ptr_offset] == '\n')
    {
      m_display_buffer[i] = 0;
      return;
    }

    m_display_buffer[i] |= CharacterToSymbol(m_string_buffer[i+m_string_ptr_offset]);

    if((*a_string) == '.' or (*a_string) == ',')
    {
      data |= 0x4000;
      a_string++;
    }
    m_display_buffer[i] = data;

  }
  
}
  
void HT16K33_Display::SetBlink(const Blink a_blink)
{
  m_blink_frequency = a_blink;
  TransmitCommand(a_blink);
}


void HT16K33_Display::Execute()
{
  if(m_update_timer.Check())
  {
    if(m_status == Status_on_printing_string)
    {
      ++m_string_ptr_offset;
      UpdateString();
    }
    
    Update();
    //load new values to ds
  }
}
  
void HT16K33_Display::TransmitCommand(const uint8_t a_command)
{
  mref_i2c.GenerateStartCondition();
  mref_i2c.TransmitDeviceAddress(m_i2c_address);
  mref_i2c.TransmitData(a_command);
  mref_i2c.CheckByteTransmissionFlag();
  mref_i2c.GenerateStopCondition();
}

void HT16K33_Display::TransmitData(const Position a_position, const uint8_t a_byte1, const uint8_t a_byte2)
{
  mref_i2c.GenerateStartCondition();
  mref_i2c.TransmitDeviceAddress(m_i2c_address);
  mref_i2c.TransmitData(a_position*2);
  mref_i2c.TransmitData(a_byte1);
  mref_i2c.TransmitData(a_byte2);
  mref_i2c.CheckByteTransmissionFlag();
  mref_i2c.GenerateStopCondition();
}

void HT16K33_Display::TransmitData(const Position a_position, const uint16_t a_byte12)
{
  mref_i2c.GenerateStartCondition();
  mref_i2c.TransmitDeviceAddress(m_i2c_address);
  mref_i2c.TransmitData(a_position*2);
  mref_i2c.TransmitData(a_byte12 & 0xFF);
  mref_i2c.TransmitData((a_byte12 & 0xFF00)>> 8);
  mref_i2c.CheckByteTransmissionFlag();
  mref_i2c.GenerateStopCondition();
}

uint16_t HT16K33_Display::DigitToSymbol(const uint8_t a_digit) const
{
  if(a_digit > 9) 
    while(1);
    
  switch(a_digit)
  {
    case 0:
    return 0xC3F;
    
    case 1:
    return 0x406;
    
    case 2:
    return 0xDB;
    
    case 3:
    return 0x8F;
    
    case 4:
    return 0xE6;
    
    case 5:
    return 0xED;
    
    case 6:
    return 0xFD;
    
    case 7:
    return 0x1401;
    
    case 8:
    return 0xFF;
    
    case 9:
    return 0xEF; //0xE7
  }
  return 0;
}

uint16_t HT16K33_Display::CharacterToSymbol(const uint8_t a_character) const
{
  switch(a_character)
  {
    case '0':
    return 0xC3F;
    
    case '1':
    return 0x406;
    
    case '2':
    return 0xDB;
    
    case '3':
    return 0x8F;
    
    case '4':
    return 0xE6;
    
    case '5':
    return 0xED;
    
    case '6':
    return 0xFD;
    
    case '7':
    return 0x1401;
    
    case '8':
    return 0xFF;
    
    case '9':
    return 0xEF;  //0xE7
    
    case 'a':
    case 'A':
    return 0xF7;
    
    case 'B':
    case 'b':
    return 0x128F;
    
    case 'C':
    case 'c':
    return 0x39;

    case 'D':
    case 'd':
    return 0x120F;
    
    case 'E':
    case 'e':
    return 0xF9;
    
    case 'F':
    case 'f':
    return 0xF1;
    
    case 'G':
    case 'g':
    return 0xBD;
    
    case 'H':
    case 'h':
    return 0xF6;
    
    case 'I':
    case 'i':
    return 0x1209;
    
    case 'J':
    case 'j':
    return 0x1E;
    
    case 'K':
    case 'k':
    return 0x2470;
    
    case 'L':
    case 'l':
    return 0x38;
    
    case 'M':
    case 'm':
    return 0x536;
    
    case 'N':
    case 'n':
    return 0x2136;
    
    case 'O':
    case 'o':
    return 0x3F;
    
    case 'P':
    case 'p':
    return 0xF3;
    
    case 'Q':
    case 'q':
    return 0x203F;
    
    case 'R':
    case 'r':
    return 0x20F3;
    
    case 'S':
    case 's':
    return 0x18D;
    
    case 'T':
    case 't':
    return 0x1201;
    
    case 'U':
    case 'u':
    return 0x3E;
    
    case 'V':
    case 'v':
    return 0xC30;
    
    case 'W':
    case 'w':
    return 0x2836;
    
    case 'X':
    case 'x':
    return 0x2D00;
    
    case 'Y':
    case 'y':
    return 0x1500;
    
    case 'Z':
    case 'z':
    return 0xC09;
    
    case'-':
    return 0xC0;
    
    case '/':
    return 0x0C00;
    
    case '.':
    case ',':
    return 0x4000;
    
    case ' ':
    return 0;
  }
  return 0;
}
void HT16K33_Display::ClearDisplayBuffer()
{
  for (volatile auto i{0}; i < m_digits; ++i)
    m_display_buffer[i] = 0;
}
void HT16K33_Display::Update()
{
  for(volatile auto i{0}; i < m_digits; ++i)
    TransmitData(static_cast<Position>(i), m_display_buffer[i]);
} 