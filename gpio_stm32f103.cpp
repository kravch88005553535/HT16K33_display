#include "gpio_stm32f103.h"


Pin::Pin(GPIO_TypeDef* ap_port, uint8_t a_pin_number, Mode a_mode)
  : mp_port(ap_port)
  , m_pin_number(a_pin_number)
  , m_mode(a_mode)
{
  switch(reinterpret_cast<uint32_t>(mp_port))
  {
    case GPIOA_BASE:
    RCC->APB2ENR |=  RCC_APB2ENR_IOPAEN;
    break;
    
    case GPIOB_BASE:
    RCC->APB2ENR |=  RCC_APB2ENR_IOPBEN;
    break;
    
    case GPIOC_BASE:
    RCC->APB2ENR |=  RCC_APB2ENR_IOPCEN;
    break;
    
    
    case GPIOD_BASE:
    RCC->APB2ENR |=  RCC_APB2ENR_IOPDEN;
    break;
    
    #ifndef STM32F103C6T6
    case GPIOE_BASE:
    RCC->APB2ENR |= RCC_APB2ENR_IOPEEN;
    break;
    #endif//#ifndef STM32F103C6T6
    
#if defined (STM32F10X_HD) || defined (STM32F10X_XL)
    case GPIOF_BASE:
    RCC->APB2ENR |= RCC_APB2ENR_IOPFEN;
    break;
    
    case GPIOG_BASE:
    RCC->APB2ENR |= RCC_APB2ENR_IOPGEN;
    break;
#endif //defined (STM32F10X_HD) || defined (STM32F10X_XL)
  }

  switch(static_cast<int>(m_mode))
  {
    case mode_in_floating:  
    {
      if(m_pin_number < 8)
      {
        mp_port->CRL &= ~(GPIO_CRL_MODE0 << (m_pin_number*4));          //MODE_INPUT
        mp_port->CRL &= ~(GPIO_CRL_CNF0  << (m_pin_number*4));          //ANALOG_MODE //clear
        mp_port->CRL |= GPIO_CRL_CNF0_0  << (m_pin_number*4);            //FLOATING_INPUT
      }
      else
      {
        mp_port->CRH &= ~(GPIO_CRH_MODE8 << ((m_pin_number-8)*4));      //MODE_INPUT  
        mp_port->CRH &= ~(GPIO_CRH_CNF8 << ((m_pin_number-8)*4));        //ANALOG_MODE //clear
        mp_port->CRH |= GPIO_CRH_CNF8_0 << ((m_pin_number-8)*4);        //FLOATING_INPUT
      }
    }
    break;
    
    case mode_in_pullup:
    {
      if(m_pin_number < 8)
      {
        mp_port->CRL &= ~(GPIO_CRL_MODE0 << (m_pin_number*4));          //MODE_INPUT  
        mp_port->CRL &= ~(GPIO_CRL_CNF0  << (m_pin_number*4));          //ANALOG_MODE //clear
        mp_port->CRL |= GPIO_CRL_CNF0_1  << (m_pin_number*4);            //INPUT WITN PU_PD
      }
      else
      {
        mp_port->CRH &= ~(GPIO_CRH_MODE8 << ((m_pin_number-8)*4));      //MODE_INPUT  
        mp_port->CRH &= ~(GPIO_CRH_CNF8 << ((m_pin_number-8)*4));        //ANALOG_MODE //clear
        mp_port->CRH |= GPIO_CRH_CNF8_1 << ((m_pin_number-8)*4);          //INPUT WITN PU_PD
      }
      mp_port->ODR |= GPIO_ODR_ODR0 << m_pin_number;
    }
    break;
    
    case mode_in_pulldown:
    {
      if(m_pin_number < 8)
      {
        mp_port->CRL &= ~(GPIO_CRL_MODE0 << (m_pin_number*4));          //MODE_INPUT  
        mp_port->CRL &= ~(GPIO_CRL_CNF0  << (m_pin_number*4));          //ANALOG_MODE //clear
        mp_port->CRL |= GPIO_CRL_CNF0_1  << (m_pin_number*4);            //INPUT WITN PU_PD
      }
      else
      {
        mp_port->CRH &= ~(GPIO_CRH_MODE8 << ((m_pin_number-8)*4));      //MODE_INPUT  
        mp_port->CRH &= ~(GPIO_CRH_CNF8 << ((m_pin_number-8)*4));        //ANALOG_MODE //clear
        mp_port->CRH |= GPIO_CRH_CNF8_1 << ((m_pin_number-8)*4);          //INPUT WITN PU_PD
      }
      mp_port->ODR &= ~(GPIO_ODR_ODR0 << m_pin_number);
    }  
    break;
    
    case mode_out_pullup:
    {
      if(m_pin_number < 8)
      {
        mp_port->CRL |= GPIO_CRL_MODE0 << (m_pin_number*4);                //MODE_OUTPUT, max_speed
        mp_port->CRL &= ~(GPIO_CRL_CNF0  << (m_pin_number*4));            //OUT_PP
      }
      else
      {
        mp_port->CRH |= GPIO_CRH_MODE8 << ((m_pin_number-8)*4);            //MODE_OUTPUT, max_speed
        mp_port->CRH &= ~(GPIO_CRH_CNF8 << ((m_pin_number-8)*4));          //OUT_PP
      }
      
      mp_port->ODR |= GPIO_ODR_ODR0 << m_pin_number;                       //OUT_PU
    }
    break;
    
    case mode_out_pulldown:
    {
      if(m_pin_number < 8)
      {
        mp_port->CRL |= GPIO_CRL_MODE0 << (m_pin_number*4);                //MODE_OUTPUT, max_speed
        mp_port->CRL &= ~(GPIO_CRL_CNF0  << (m_pin_number*4));            //OUT_PP
      }
      else
      {
        mp_port->CRH |= GPIO_CRH_MODE8 << ((m_pin_number-8)*4);            //MODE_OUTPUT, max_speed
        mp_port->CRH &= ~(GPIO_CRH_CNF8 << ((m_pin_number-8)*4));          //OUT_PP
      }
      
      mp_port->ODR &= ~(GPIO_ODR_ODR0 << m_pin_number);                       //OUT_PD
    }  
    break;
        
    case mode_out_open_drain:
    {
      if(m_pin_number < 8)
      {
        mp_port->CRL |= GPIO_CRL_MODE0 << (m_pin_number*4);                //MODE_OUTPUT, max_speed
        mp_port->CRL &= ~(GPIO_CRL_CNF0  << (m_pin_number*4));            //CLEAR_CNF
        mp_port->CRL |= GPIO_CRL_CNF0_0  << (m_pin_number*4);              //OUTPUT_OD
      }
      else
      {
        mp_port->CRH |= GPIO_CRH_MODE8 << ((m_pin_number-8)*4);            //MODE_OUTPUT, max_speed
        mp_port->CRH &= ~(GPIO_CRH_CNF8 << ((m_pin_number-8)*4));          //CLEAR_CNF
        mp_port->CRH |= GPIO_CRH_CNF8_0 << ((m_pin_number-8)*4);          //OUTPUT_OD
      }
    }
    break;
    
    case mode_alternate_function_pushpull:
    {
      if(m_pin_number < 8)
      {
        mp_port->CRL |= GPIO_CRL_MODE0 << (m_pin_number*4);                //MODE_OUTPUT, max_speed
        mp_port->CRL &= ~(GPIO_CRL_CNF0  << (m_pin_number*4));            //CLEAR_CNF
        mp_port->CRL |= GPIO_CRL_CNF0_1  << (m_pin_number*4);              //OUTPUT_AF_PP
      }
      else
      {
        mp_port->CRH |= GPIO_CRH_MODE8 << ((m_pin_number-8)*4);            //MODE_OUTPUT, max_speed
        mp_port->CRH &= ~(GPIO_CRH_CNF8  << (m_pin_number-8)*4);            //CLEAR_CNF
        mp_port->CRH |= GPIO_CRH_CNF8_1 << ((m_pin_number-8)*4);          //OUTPUT_AF_PP
      }
    }
    
    break;
    
    case mode_alternate_function_open_drain:
    {
      if(m_pin_number < 8)
      {
        mp_port->CRL |= GPIO_CRL_MODE0 << (m_pin_number*4);                //MODE_OUTPUT, max_speed
        mp_port->CRL |= GPIO_CRL_CNF0  << (m_pin_number*4);                //OUTPUT_AF_OD
      }
      else
      {
        mp_port->CRH |= GPIO_CRH_MODE8 << ((m_pin_number-8)*4);            //MODE_OUTPUT, max_speed
        mp_port->CRH |= GPIO_CRH_CNF8 << ((m_pin_number-8)*4);            //OUTPUT_AF_OD
      }
    }
    break;
    
    case mode_analog:
    {
      if(m_pin_number < 8)
      {
        mp_port->CRL &= ~(GPIO_CRL_MODE0 << (m_pin_number*4));          //MODE_INPUT
        mp_port->CRL &= ~(GPIO_CRL_CNF0  << (m_pin_number*4));          //ANALOG_MODE 
      }
      else
      {
        mp_port->CRH &= ~(GPIO_CRH_MODE8 << ((m_pin_number-8)*4));      //MODE_INPUT  
        mp_port->CRH &= ~(GPIO_CRH_CNF8 << ((m_pin_number-8)*4));        //ANALOG_MODE 
      }
    }
    break;

  }
}
  
Pin::~Pin()
{
}
void Pin::Set()
{
  mp_port->BSRR = GPIO_BSRR_BS0 << m_pin_number;
}
void Pin::Reset()
{
  mp_port->BRR = GPIO_BRR_BR0 << m_pin_number;
}

bool Pin::GetState()
{
  return mp_port->IDR & GPIO_IDR_IDR0 << m_pin_number;
}
void Pin::TogglePin()
{
  mp_port->ODR ^= GPIO_ODR_ODR0 << m_pin_number;
}
void Pin::SetMode(const Mode a_mode)
{
}
Pin::Mode Pin::GetMode()
{
  return m_mode;
}

void Pin::SetAlternateFunctionNumber(AlternateFunction a_af_number)
{
//  if (m_pin_number <=7)
//  {
//    mp_port->AFR[0] &= ~(GPIO_AFRL_AFRL0 << m_pin_number*4);
//    mp_port->AFR[0] |= a_af_number << m_pin_number*4;
//  }
//  else
//    mp_port->AFR[1] &= ~(GPIO_AFRH_AFRH0 << (m_pin_number-8)*4);
//    mp_port->AFR[1] |= a_af_number << (m_pin_number-8)*4;
}

void Pin::Lock()
{
//  mp_port->LCKR |= GPIO_LCKR_LCK0 << m_pin_number;
//  do {mp_port->LCKR ^= GPIO_LCKR_LCKK; } while ((mp_port->LCKR & GPIO_LCKR_LCKK) != GPIO_LCKR_LCKK);
}