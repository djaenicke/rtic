#ifndef GPIO_H
#define GPIO_H (1)

#include <cstdint>

#include "stm32f4xx_hal.h"

namespace hal
{
const uint8_t NO_GPIO_ALT_FUNCTION = 0;

enum class Port
{
  A = 0,
  B,
  C,
  D,
  H,

  NUM_PORTS
};

enum class GpioMode
{
  INPUT = 0,
  OUTPUT_PUSH_PULL,
  OUTPUT_OPEN_DRAIN,
  AF_PUSH_PULL,
  AF_OPEN_DRAIN,
  ANALOG,
  INPUT_RISING_INT,
  INPUT_FALLING_INT,
  INPUT_BOTH_INT,

  NUM_MODES
};

struct Pin
{
  Port port;
  uint8_t num;
  Pin(const Port port, const uint8_t num) : port(port), num(num) {};
};

enum class GpioPullType
{
  NO_PULL = 0,
  PULL_UP,
  PULL_DOWN,

  NUM_PULL_TYPES
};

class GPIO
{
 public:
  GPIO(const Pin& pin, const GpioMode mode, const GpioPullType pull = GpioPullType::NO_PULL,
       const uint8_t alt_function = NO_GPIO_ALT_FUNCTION);
  operator uint8_t();

 private:
  void enableClock(void);

 protected:
  GPIO_TypeDef* _port_ptr;
  uint16_t _pin_select;
};

}  // namespace hal

#endif  // GPIO_H (1)
