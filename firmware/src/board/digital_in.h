#ifndef DIGITAL_IN_H
#define DIGITAL_IN_H (1)

#include <cstdint>

#include "gpio.h"
#include "stm32f4xx_hal.h"

namespace hal
{
class DigitalIn : public GPIO
{
 public:
  DigitalIn(const Pin& pin, const GpioPullType pull = GpioPullType::NO_PULL);
  operator uint8_t();
};

}  // namespace hal

#endif  // DIGITAL_IN_H (1)
