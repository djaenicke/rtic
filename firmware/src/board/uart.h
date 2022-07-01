#ifndef UART_H
#define UART_H (1)

#include <cstdint>

#include "gpio.h"
#include "stm32f4xx_hal.h"

namespace hal
{
class UART
{
 public:
  UART(const uint8_t uart_instance, const GPIO& tx, const GPIO& rx, const uint32_t baudrate);
  void transmit_blocking(const uint8_t* const buf, const uint16_t buf_len);

 private:
  hal::GPIO _tx;
  hal::GPIO _rx;
  UART_HandleTypeDef _handle;
};

}  // namespace hal

#endif  // UART_H (1)
