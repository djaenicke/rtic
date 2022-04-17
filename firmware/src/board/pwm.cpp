#include "pwm.h"

#include <cmath>

namespace hal
{
PwmOut::PwmOut(const hal::PwmPin& pwm_pin)
    : _output(pwm_pin.pin, hal::GpioMode::AF_PUSH_PULL, hal::GpioPullType::NO_PULL, pwm_pin.gpio_alt_function)
{
  switch (pwm_pin.timer_channel)
  {
    case 1:
      _tim_channel = TIM_CHANNEL_1;
      break;
    case 2:
      _tim_channel = TIM_CHANNEL_2;
      break;
    case 3:
      _tim_channel = TIM_CHANNEL_3;
      break;
    case 4:
      _tim_channel = TIM_CHANNEL_4;
      break;
    default:
      // todo: assert
      _tim_channel = TIM_CHANNEL_1;
  }
}

void PwmOut::start(const hal::HwTimer* const timer_instance)
{
  if (NULL == timer_instance)
  {
    // todo: assert
    return;
  }

  _timer_ptr = (hal::HwTimer*)timer_instance;
  HAL_TIM_PWM_Start(_timer_ptr->getHandle(), _tim_channel);
  __HAL_TIM_SET_COMPARE(_timer_ptr->getHandle(), _tim_channel, 0);
}

void PwmOut::setDutyCycle(float normalized_dc)
{
  // Saturate the
  if (normalized_dc > 1.0f)
  {
    normalized_dc = 1.0f;
  }
  else if (normalized_dc < 0.0f)
  {
    normalized_dc = 0.0f;
  }
  const uint16_t new_cmp_value = (uint16_t)roundf(normalized_dc * _timer_ptr->getPeriod());
  __HAL_TIM_SET_COMPARE(_timer_ptr->getHandle(), _tim_channel, new_cmp_value);
}

}  // namespace hal
