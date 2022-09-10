#include "digital_out.h"

#include <cassert>

namespace hal
{
DigitalOut::DigitalOut(const Pin& pin) : GPIO(pin, GpioMode::OUTPUT_PUSH_PULL)
{
}

DigitalOut& DigitalOut::operator=(const uint8_t state)
{
  if (0u == state)
  {
    HAL_GPIO_WritePin(_port_ptr, _pin_select, GPIO_PIN_RESET);
  }
  else if (1u == state)
  {
    HAL_GPIO_WritePin(_port_ptr, _pin_select, GPIO_PIN_SET);
  }
  else
  {
    assert(0);
  }

  return *this;
}

void DigitalOut::toggle(void)
{
  if (NULL != _port_ptr)
  {
    HAL_GPIO_TogglePin(_port_ptr, _pin_select);
  }
  else
  {
    assert(0);
  }
}

}  // namespace hal