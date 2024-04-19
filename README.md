Display address table:
![image](https://github.com/kravch88005553535/HT16K33_display/assets/118789542/c04c6d9c-0798-47ed-835e-22b76cdfea31)

Display's picture:
![image](https://github.com/kravch88005553535/HT16K33_display/assets/118789542/91b97c20-e017-4aed-a543-8935f61ec4a2)

  void TurnDisplayOn();
    -turns display on
    
  void TurnDisplayOff();
    -turns display off
    
  Brightness GetBrightness() const;
    -returns current brightness of display(1..16)
    
  void IncrementBrightness();
    -increments brightness of display
    
  void DecrementBrightness();
    -decrements brightness of display
    
  void Clear();
    -clears all data from display

  void PrintNumber(uint32_t a_decimal_number, const Position a_separator_position = Position_NONE);
    -prints integer on display. can be used for float numbers but with explicitly defined separator position
    
  void PrintFloatNumber(float a_number);
    -prints float number on display. separator position is after second digit by default.

  void PrintString(const char* a_string);
    -prints first 4 symbols (letters/digits) of ASCII containing characters string. **Dots commas and slashes are displayed!**
  
  void SetBlink(const Blink a_blink);
    -sets blink frequency of display (OFF2Hz/1Hz/0.5Hz)
