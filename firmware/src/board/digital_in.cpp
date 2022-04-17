#include "digital_in.h"

namespace hal
{
DigitalIn::DigitalIn(const Pin& pin, const GpioPullType pull) : GPIO(pin, GpioMode::INPUT, pull)
{
}

DigitalIn::operator uint8_t()
{
  return (uint8_t)HAL_GPIO_ReadPin(this->_port_ptr, this->_pin_select);
}

}  // namespace hal
