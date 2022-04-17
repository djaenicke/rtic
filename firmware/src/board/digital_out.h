#ifndef DIGITAL_OUT_H
#define DIGITAL_OUT_H (1)

#include <cstdint>

#include "gpio.h"
#include "stm32f4xx_hal.h"

namespace hal
{
class DigitalOut : public GPIO
{
 public:
  DigitalOut(const Pin& pin);
  DigitalOut& operator=(const uint8_t state);
  void toggle(void);
};

}  // namespace hal

#endif  // DIGITAL_OUT_H (1)
