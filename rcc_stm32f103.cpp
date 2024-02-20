#include <stm32f10x.h>
#include "rcc_stm32f103.h"


Rcc::Rcc(System_clock_source a_clock_source, Hse_frequency a_hse_frequency)
  : m_hse_frequency{a_hse_frequency}
{
//    /* Enable Prefetch Buffer */
//  FLASH->ACR |= FLASH_ACR_PRFTBE;
//    /* Flash 2 wait state */
//  FLASH->ACR &= (uint32_t)((uint32_t)~FLASH_ACR_LATENCY);
//  FLASH->ACR |= (uint32_t)FLASH_ACR_LATENCY_2; 
//  EnableHse();
//  DisablePll();
//  SetPllSource(Pll_clock_source_hse_oscillator);
//  SetPllMultiplier(Pll_multiplier_9);
//  EnablePll();  
//  SetApb1Prescaler(Apb1_prescaler_2);
//  SetSystemClockSource(System_clock_source_pll_clock);
}

Rcc::~Rcc()
{
  
}
uint32_t Rcc::GetAhbClock()
{
  return GetSystemClock()/ GetAhbPrescaler();
}
uint32_t Rcc::GetApb1Clock()
{
  return GetAhbClock()/GetApb1Prescaler();
} 
uint32_t Rcc::GetApb2Clock()
{
  return GetAhbClock()/GetApb2Prescaler();
}

uint32_t Rcc::GetPeripheralClock(void* ap_peripheral)
{
  switch(reinterpret_cast<uint32_t>(ap_peripheral))
  {
    case WWDG_BASE:
    case SPI2_BASE:
    case SPI3_BASE:
    case USART2_BASE:
    case USART3_BASE:
    case UART4_BASE:
    case UART5_BASE:
    case I2C1_BASE:
    case I2C2_BASE:
//    case usb_base:
    case CAN1_BASE:
    case BKP_BASE:
    case PWR_BASE:
    case DAC_BASE:
    return GetApb1Clock();
        
    case AFIO_BASE:
    case GPIOA_BASE:
    case GPIOB_BASE:
    case GPIOC_BASE:
    case GPIOD_BASE:
    case GPIOE_BASE:
    case GPIOF_BASE:
    case GPIOG_BASE:
    case ADC1_BASE:
    case ADC2_BASE:
    case SPI1_BASE:
    case USART1_BASE:
    case ADC3_BASE:
    return GetApb2Clock();
    
    
    case DMA1_BASE:
    case DMA2_BASE:
//    case fsmc_base:
    case SDIO_BASE:
    case CRC_BASE:
    case SRAM_BASE:
    return GetAhbClock();

    case TIM1_BASE:
    case TIM2_BASE:
    case TIM3_BASE:
    case TIM4_BASE:
    case TIM5_BASE:
    case TIM6_BASE:
    case TIM7_BASE:
    case TIM8_BASE:
    case TIM9_BASE:
    case TIM10_BASE:
    case TIM11_BASE:
    case TIM12_BASE:
    case TIM13_BASE:
    case TIM14_BASE:
    return GetTimerClock(reinterpret_cast<TIM_TypeDef*>(ap_peripheral));
  }
  
  while(1){}
  return 666;
  //uint32_t GetTimerClock(TIM_TypeDef* ap_timer);
}

void Rcc::SetSysClockToMax()
{
    /* Enable Prefetch Buffer */
  FLASH->ACR |= FLASH_ACR_PRFTBE;
    /* Flash 2 wait state */
  FLASH->ACR &= (uint32_t)((uint32_t)~FLASH_ACR_LATENCY);
  FLASH->ACR |= (uint32_t)FLASH_ACR_LATENCY_2; 
  EnableHse();
  SetPllSource(Pll_clock_source_hse_oscillator);
  //setpllprescaler
  SetPllMultiplier(Pll_multiplier_9);
  EnablePll();  
  SetApb1Prescaler(Apb1_prescaler_2);
  SetSystemClockSource(System_clock_source_pll_clock);
}

Rcc::System_clock_source Rcc::GetSystemClockSource()
{
  const uint32_t RCC_CFGR_SWS_Pos = 2;
  const uint32_t RCC_CFGR_SWS_Msk = RCC_CFGR_SWS;
  return static_cast<System_clock_source>((RCC->CFGR & RCC_CFGR_SWS_Msk) >> RCC_CFGR_SWS_Pos);
}
  
void Rcc::SetSystemClockSource(System_clock_source a_system_clock_source)
{
  RCC->CFGR &= ~RCC_CFGR_SW;
  
  const uint32_t RCC_CFGR_SW_Pos = 0;
  RCC->CFGR |= a_system_clock_source << RCC_CFGR_SW_Pos;
  while (GetSystemClockSource() != a_system_clock_source) __ASM("nop");
}
Rcc::Pll_clock_source Rcc::GetPllSource()
{
  return static_cast<Pll_clock_source>(bool(RCC->CFGR & RCC_CFGR_PLLSRC));
}

void Rcc::SetPllMultiplier(Pll_multiplier a_pll_multiplier)
{
  RCC->CFGR &= ~RCC_CFGR_PLLMULL;
  
  const uint32_t RCC_CFGR_PLLMULL_Pos = 18;
  RCC->CFGR |= a_pll_multiplier << RCC_CFGR_PLLMULL_Pos;
}

uint32_t Rcc::GetPllMultiplier()
{
  const uint32_t RCC_CFGR_PLLMULL_Pos = 18;
  switch ((RCC->CFGR & RCC_CFGR_PLLMULL) >> RCC_CFGR_PLLMULL_Pos)
  {
    case Pll_multiplier_2:
    return 2;
    
    case Pll_multiplier_3:
    return 3;
    
    case Pll_multiplier_4:
    return 4;
    
    case Pll_multiplier_5:
    return 5;
    
    case Pll_multiplier_6:
    return 6;
    
    case Pll_multiplier_7:
    return 7;
    
    case Pll_multiplier_8:
    return 8;
    
    case Pll_multiplier_9:
    return 9;  
    
    case Pll_multiplier_10:
    return 10;
    
    case Pll_multiplier_11:
    return 11;
    
    case Pll_multiplier_12:
    return 12;
    
    case Pll_multiplier_13:
    return 13;
    
    case Pll_multiplier_14:
    return 14;
    
    case Pll_multiplier_15:
    return 15;
    
    case Pll_multiplier_16:
    return 16;
  }
  
  while(1) __ASM("nop");
  return 666;
}

uint32_t Rcc::GetSystemClock()
{
  switch (static_cast<uint32_t>(GetSystemClockSource()))
  {
    case System_clock_source_hse_clock:
    return m_hse_frequency;
    break;
    
    case System_clock_source_hsi_clock:
    return HSI_VALUE;
    break;
    
    case System_clock_source_pll_clock:
//      RCC_CFGR_PLLXTPRE
      uint32_t system_clock{};
      if (GetPllSource() == Pll_clock_source_hsi_oscillator)
      {
        system_clock = HSI_VALUE;
      }
      else
      {
        system_clock = m_hse_frequency / (bool(RCC->CFGR & RCC_CFGR_PLLXTPRE) + 1);
      }
      system_clock *= GetPllMultiplier(); 
      return system_clock;
    break;
  }
  while(1){}
  
  return 666;
}

void Rcc::SetPllSource(Pll_clock_source a_pll_clock_soiurce)
{
  switch (static_cast<uint32_t>(a_pll_clock_soiurce))
  {
    case Pll_clock_source_hsi_oscillator:
      RCC->CFGR &= ~RCC_CFGR_PLLSRC;
    break;
    case Pll_clock_source_hse_oscillator:
      RCC->CFGR &= ~RCC_CFGR_PLLSRC;  
      RCC->CFGR |= RCC_CFGR_PLLSRC;
    break;
    case Pll_clock_source_hse_oscillator_div2:
      RCC->CFGR &= ~RCC_CFGR_PLLSRC;
      RCC->CFGR |= RCC_CFGR_PLLSRC;
//      RCC->CFGR |= RCC_CFGR_PLLXTPRE_HSE_Div2;
    break;
  }
}

bool Rcc::IsHseReady()
{
  return RCC->CR & RCC_CR_HSERDY;
}

bool Rcc::IsPllReady()
{
  return RCC->CR & RCC_CR_PLLRDY;
}

void Rcc::EnableHse()
{
  RCC->CR |= RCC_CR_HSEON; 
    while (!IsHseReady())  __ASM("nop");
}

void Rcc::DisableHse()
{
  RCC->CR &= ~RCC_CR_HSEON; 
}

void Rcc::EnablePll()
{
  RCC->CR |= RCC_CR_PLLON;
    while (!IsPllReady()) __ASM("nop"); 
}

void Rcc::DisablePll()
{
  RCC->CR &= ~RCC_CR_PLLON;
}

uint32_t Rcc::GetAhbPrescaler()
{
  const uint32_t RCC_CFGR_HPRE_Pos = 4; 
  switch ((RCC->CFGR & RCC_CFGR_HPRE) >> RCC_CFGR_HPRE_Pos)
  {
    case Ahb_prescaler_not_divided:
    return 1;
    
    case Ahb_prescaler_2:
    return 2;
    
    case Ahb_prescaler_4:
    return 4;
    
    case Ahb_prescaler_8:
    return 8;
    
    case Ahb_prescaler_16:
    return 16;
    
    case Ahb_prescaler_64:
    return 64;
    
    case Ahb_prescaler_128:
    return 128;
    
    case Ahb_prescaler_256:
    return 256;
    
    case Ahb_prescaler_512:
    return 512;    
  }
  while(1){}
  return 666;
}

uint32_t  Rcc::GetApb1Prescaler()
{
  const uint32_t RCC_CFGR_PPRE1_Pos = 8;
  switch ((RCC->CFGR & RCC_CFGR_PPRE1) >> RCC_CFGR_PPRE1_Pos)
  {
    case Apb1_prescaler_not_divided:
    return 1;
    
    case Apb1_prescaler_2:
    return 2;
    
    case Apb1_prescaler_4:
    return 4;
    
    case Apb1_prescaler_8:
    return 8;
    
    case Apb1_prescaler_16:
    return 16;
  }
  
  while(1){}
      
  return 666;
}

uint32_t  Rcc::GetApb2Prescaler()
{
  const uint32_t RCC_CFGR_PPRE2_Pos = 11;
  switch ((RCC->CFGR & RCC_CFGR_PPRE2) >> RCC_CFGR_PPRE2_Pos)
  {
    case Apb2_prescaler_not_divided:
    return 1;
    
    case Apb2_prescaler_2:
    return 2;
    
    case Apb2_prescaler_4:
    return 4;
    
    case Apb2_prescaler_8:
    return 8;
    
    case Apb2_prescaler_16:
    return 16;
  }
  
  while(1){}
      
  return 666;
}

void Rcc::SetAhbPrescaler(Ahb_prescaler a_ahb_prescaler)
{
  const uint32_t RCC_CFGR_HPRE_Pos = 4;
  RCC->CFGR &= ~RCC_CFGR_HPRE;
  RCC->CFGR |= a_ahb_prescaler << RCC_CFGR_HPRE_Pos; 
}

void Rcc::SetApb1Prescaler(Apb1_prescaler a_apb1_prescaler)
{
  const uint32_t RCC_CFGR_PPRE1_Pos = 8;
  RCC->CFGR &= ~RCC_CFGR_PPRE1;
  RCC->CFGR |= a_apb1_prescaler << RCC_CFGR_PPRE1_Pos;
}

void Rcc::SetApb2Prescaler(Apb2_prescaler a_apb2_prescaler)
{
  const uint32_t RCC_CFGR_PPRE2_Pos = 11;
  RCC->CFGR &= ~RCC_CFGR_PPRE2;
  RCC->CFGR |= a_apb2_prescaler << RCC_CFGR_PPRE2_Pos;
}

uint32_t Rcc::GetTimerPrescaler(void* ap_timer_ptr)
{
  switch (reinterpret_cast<uint32_t>(ap_timer_ptr))
  {
    case TIM2_BASE:
    case TIM3_BASE:
    case TIM4_BASE:
    case TIM5_BASE:
    case TIM6_BASE:
    case TIM7_BASE:
    case TIM12_BASE:
    case TIM13_BASE:
    case TIM14_BASE:
      return (GetApb1Prescaler() == 1) ? 1 : 2;
      
    case TIM1_BASE:
    case TIM8_BASE:
    case TIM9_BASE:
    case TIM10_BASE:
    case TIM11_BASE:
      return (GetApb2Prescaler() == 1) ? 1 : 2;
  }
  
  while(1) __ASM("nop");
  return 666;
}

uint32_t Rcc::GetTimerClock (void* ap_timer_ptr)
{
  switch (reinterpret_cast<uint32_t>(ap_timer_ptr))
  {
    case TIM2_BASE:
    case TIM3_BASE:
    case TIM4_BASE:
    case TIM5_BASE:
    case TIM6_BASE:
    case TIM7_BASE:
    case TIM12_BASE:
    case TIM13_BASE:
    case TIM14_BASE:
      return GetTimerPrescaler(ap_timer_ptr) * GetApb1Clock();
      
    case TIM1_BASE:
    case TIM8_BASE:
    case TIM9_BASE:
    case TIM10_BASE:
    case TIM11_BASE:
      return GetTimerPrescaler(ap_timer_ptr) * GetApb2Clock();  
  }
  
  while(1){}
  return 666;
}
  