#include "program_timer.h"

Program_timer::Program_timer(TimerType a_timertype)
  : m_timertype{a_timertype}
  , m_status{TimerStatus_stopped}
  , m_interval_ms{0}
  , m_previous_overflow_number{0}
{
  timers_total_count++;
  
  if(!systick_is_initialized)
  {
    constexpr auto systick_1sec_tick_value{9000000};
    SysTick->LOAD = systick_1sec_tick_value / 1000;
    SysTick->CTRL |= SysTick_CTRL_TICKINT;
    NVIC_EnableIRQ(SysTick_IRQn);
    SysTick->CTRL &= ~SysTick_CTRL_CLKSOURCE;
    systick_is_initialized = true;    
  }
}

Program_timer::Program_timer(TimerType a_timertype, uint32_t a_interval_ms)
  : Program_timer(a_timertype)
{
  m_interval_ms = a_interval_ms;
  Start();
}

void Program_timer::Start()
{
  if(m_status  == TimerStatus_stopped)
  {
    ++timers_total_count;
    SysTick->CTRL |= SysTick_CTRL_ENABLE;
    m_previous_overflow_number = overflows_number;
  }
  m_status = TimerStatus_started;
}

void Program_timer::Stop()
{
  if (m_status  == TimerStatus_started)
  {
    --timers_total_count;
    if(!timers_total_count)
    {
      SysTick->CTRL &= ~SysTick_CTRL_ENABLE;
    }
  }
  m_status = TimerStatus_stopped;
}

bool Program_timer::Check()
{
  if(m_status == TimerStatus_started)
  {
    if(overflows_number > (m_previous_overflow_number + m_interval_ms))
    {
      m_previous_overflow_number = overflows_number - overflows_number % m_interval_ms;
      return true;
    }
  }
  return false;
}
void Program_timer::Reload()
{
  m_previous_overflow_number = overflows_number;
}
bool Program_timer::IsStarted()
{
  return m_status == TimerStatus_started ? true : false;
}
void Program_timer::SetInterval_ms(uint32_t a_interval_ms)
{
  m_interval_ms = a_interval_ms;
}
uint32_t Program_timer::GetInterval_ms()
{
  return m_interval_ms;
}
void Program_timer::SetInterval_sec(uint32_t a_interval_sec)
{
  m_interval_ms = a_interval_sec * 1000;
}

void Program_timer::SetInterval_min(uint32_t a_interval_min)
{
  m_interval_ms = a_interval_min * 1000 * 60;
}

Program_timer::~Program_timer()
{
}

#ifdef __cplusplus
extern "C" 
{
#endif 
    
void SysTick_Handler()
{
  Program_timer::overflows_number++;
}
  
#ifdef __cplusplus
}
#endif 