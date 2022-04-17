#ifndef HW_TIMER_H
#define HW_TIMER_H (1)

#include <cstdint>

#include "gpio.h"
#include "stm32f4xx_hal.h"

namespace hal
{
enum class TimerMode
{
  TIME_BASE = 0,
  INPUT_CAPTURE,
  OUTPUT_COMPARE,
  PWM_GENERATION,
  ONE_PULSE_MODE_OUTPUT,
  ENCODER_CH1_2X_MODE,
  ENCODER_CH2_2X_MODE,
  ENCODER_4X_MODE
};

class HwTimer
{
 public:
  HwTimer(const uint8_t timer_instance, const TimerMode mode, const float period_s,
          const bool ch_1_enabled = false, const bool ch_2_enabled = false,
          const bool ch_3_enabled = false, const bool ch_4_enabled = false);
  TIM_HandleTypeDef* getHandle(void);
  uint16_t getPeriod(void);

 protected:
  uint32_t getSourceFreqHz(void);
  void initPwmMode(const bool ch_1_enabled, const bool ch_2_enabled, const bool ch_3_enabled,
                   const bool ch_4_enabled);
  void initEncoderMode(const uint32_t encoder_mode);
  TIM_TypeDef* _timer_ptr;
  TIM_HandleTypeDef _timer_handle;
};

}  // namespace hal

#endif  // HW_TIMER_H (1)
