#ifndef ANALOG_IN_H
#define ANALOG_IN_H (1)

#include <cstdint>

#include "gpio.h"
#include "stm32f4xx_hal.h"

namespace hal
{
class AnalogIn : public GPIO
{
 public:
  AnalogIn(const Pin& pin);
  uint16_t getValue(void);

 private:
  ADC_HandleTypeDef _handle;
};

}  // namespace hal

#endif  // ANALOG_IN_H (1)
