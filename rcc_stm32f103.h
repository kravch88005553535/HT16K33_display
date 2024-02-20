#ifndef __RCC_H__
#define __RCC_H__

class Rcc
{
public:
  enum Hse_frequency
  {
    Hse_frequency_none = 0,
    Hse_frequency_4Mhz = 4'000'000,
    Hse_frequency_5Mhz = 5'000'000,
    Hse_frequency_6Mhz = 6'000'000,
    Hse_frequency_8Mhz = 8'000'000,
    Hse_frequency_10Mhz = 10'000'000,
    Hse_frequency_12Mhz = 12'000'000,
    Hse_frequency_13Mhz = 13'000'000,
    Hse_frequency_15Mhz = 15'000'000,
    Hse_frequency_16Mhz = 16'000'000
  };

  enum System_clock_source
  {
    System_clock_source_hsi_clock = 0,
    System_clock_source_hse_clock = 1,
    System_clock_source_pll_clock = 2
  };
  
  Rcc() = delete;
  Rcc(System_clock_source a_clock_source, Hse_frequency a_hse_frequency = Hse_frequency_none);
  ~Rcc();

  uint32_t GetAhbClock();
  uint32_t GetApb1Clock();  
  uint32_t GetApb2Clock();
  uint32_t GetPeripheralClock(void* ap_peripheral);
  
  void SetMcuToLowPowerMode();
  void SetMcuToRunMode();

private:
  enum Pll_multiplier
  {
    Pll_multiplier_2 = 0,
    Pll_multiplier_3 = 1,
    Pll_multiplier_4 = 2,
    Pll_multiplier_5 = 3,
    Pll_multiplier_6 = 4,
    Pll_multiplier_7 = 5,
    Pll_multiplier_8 = 6,
    Pll_multiplier_9 = 7,
    Pll_multiplier_10 = 8,
    Pll_multiplier_11 = 9,
    Pll_multiplier_12 = 10,
    Pll_multiplier_13 = 11,
    Pll_multiplier_14 = 12,
    Pll_multiplier_15 = 13,
    Pll_multiplier_16 = 14
  };
  
  enum Pll_clock_source
  {
    Pll_clock_source_hsi_oscillator = 0,
    Pll_clock_source_hse_oscillator = 1,
    Pll_clock_source_hse_oscillator_div2 = 2,
  };
  
  enum Ahb_prescaler
  {
    Ahb_prescaler_not_divided = 0,
    Ahb_prescaler_2 = 8,
    Ahb_prescaler_4 = 9,
    Ahb_prescaler_8 = 10,
    Ahb_prescaler_16 = 11,
    Ahb_prescaler_64 = 12,
    Ahb_prescaler_128 = 13,
    Ahb_prescaler_256 = 14,
    Ahb_prescaler_512 = 15,
  };
  
  enum Apb1_prescaler
  {
    Apb1_prescaler_not_divided = 0,
    Apb1_prescaler_2 = 4,
    Apb1_prescaler_4 = 5,
    Apb1_prescaler_8 = 6,
    Apb1_prescaler_16 = 7
  };
  
  enum Apb2_prescaler
  {
    Apb2_prescaler_not_divided = 0,
    Apb2_prescaler_2 = 4,
    Apb2_prescaler_4 = 5,
    Apb2_prescaler_8 = 6,
    Apb2_prescaler_16 = 7
  };
  
  ////// HSE
  bool IsHseReady();
  void EnableHse();
  void DisableHse();
  
  ////// PLL
  bool IsPllReady();
  void EnablePll();
  void DisablePll();
  void SetPllSource(Pll_clock_source a_pll_clock_soiurce);
  Pll_clock_source GetPllSource();
  void SetPllMultiplier(Pll_multiplier a_pll_multiplier);
  uint32_t GetPllMultiplier();
  
  ////// SYSTEM CLOCKS
  void SetSystemClockSource(System_clock_source a_system_clock_source);
  System_clock_source GetSystemClockSource();
  uint32_t GetSystemClock();
  void SetSysClockToMax();
  void SetAhbPrescaler(Ahb_prescaler a_ahb_prescaler);
  void SetApb1Prescaler(Apb1_prescaler a_apb1_prescaler);
  void SetApb2Prescaler(Apb2_prescaler a_apb2_prescaler);
  uint32_t GetAhbPrescaler();
  uint32_t GetApb1Prescaler();
  uint32_t GetApb2Prescaler();
  
  uint32_t GetTimerPrescaler(void* ap_timer_ptr);
  uint32_t GetTimerClock (void* ap_timer_ptr);
  ////// CLOCK SECURITY SYSTEM
  //  void EnableClockSecuritySystem();
  //  void DisableClockSecuritySystem();

  Hse_frequency m_hse_frequency;
};

#endif //__RCC_H__