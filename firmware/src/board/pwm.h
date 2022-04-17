#ifndef PWM_H
#define PWM_H (1)

#include <cstdint>

#include "gpio.h"
#include "hw_timer.h"
#include "stm32f4xx_hal.h"

namespace hal
{
struct PwmPin
{
  hal::Pin pin;
  uint8_t gpio_alt_function;
  uint8_t timer_channel;
  PwmPin(const Pin& pin, const uint8_t gpio_alt_function, const uint8_t timer_channel = 1u)
      : pin(pin), gpio_alt_function(gpio_alt_function), timer_channel(timer_channel) {};
};

class PwmOut
{
 public:
  PwmOut(const hal::PwmPin& pwm_pin);
  void start(const hal::HwTimer* const timer_instance);
  void setDutyCycle(float normalized_dc);

 private:
  hal::GPIO _output;
  hal::HwTimer* _timer_ptr;
  uint32_t _tim_channel;
};

}  // namespace hal

#endif  // PWM_H (1)
