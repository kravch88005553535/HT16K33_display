#ifndef __PROGRAM_TIMER_H__
#define __PROGRAM_TIMER_H__

#include <stm32f10x.h>

class Program_timer
{
public:
  enum TimerType: uint8_t
  {
    TimerType_one_pulse,
    TimerType_loop
  };
  
  enum TimerStatus: uint8_t
  {
    TimerStatus_started,
    TimerStatus_stopped
  };
  
  explicit Program_timer(TimerType a_timertype);
  explicit Program_timer(TimerType a_timertype, uint32_t a_interval_ms);
  ~Program_timer();
  
  void SetInterval_ms(uint32_t a_interval_ms);
  uint32_t GetInterval_ms();
  void SetInterval_sec(uint32_t a_interval_sec);
  void SetInterval_min(uint32_t a_interval_min);
  
  void Start();
  void Stop();
  bool Check();
  void Reload();
  bool IsStarted();
  volatile static inline uint64_t overflows_number{0};
private:
  Program_timer() = delete;
  
  TimerType                   m_timertype;
  TimerStatus                 m_status;
  volatile static inline bool  systick_is_initialized{0};  
  volatile static inline auto  timers_total_count{0};
  
  uint32_t m_interval_ms;
  uint64_t m_previous_overflow_number;
};

#endif //__PROGRAM_TIMER_H__