#include "uart.h"

namespace hal
{
UART::UART(const uint8_t uart_instance, const GPIO& tx, const GPIO& rx, const uint32_t baudrate)
    : _tx(tx), _rx(rx)
{
  switch (uart_instance)
  {
    case 1:
      __HAL_RCC_USART1_CLK_ENABLE();
      _handle.Instance = USART1;
      break;
    case 2:
      __HAL_RCC_USART2_CLK_ENABLE();
      _handle.Instance = USART2;
      break;
    case 3:
      __HAL_RCC_USART3_CLK_ENABLE();
      _handle.Instance = USART3;
      break;
    case 6:
      __HAL_RCC_USART6_CLK_ENABLE();
      _handle.Instance = USART6;
      break;
    default:
      _handle.Instance = NULL;
      return;
  }

  _handle.Init.BaudRate = baudrate;
  _handle.Init.WordLength = UART_WORDLENGTH_8B;
  _handle.Init.StopBits = UART_STOPBITS_1;
  _handle.Init.Parity = UART_PARITY_NONE;
  _handle.Init.Mode = UART_MODE_TX;
  _handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  _handle.Init.OverSampling = UART_OVERSAMPLING_16;

  HAL_UART_Init(&_handle);
}

void UART::transmitBlocking(const uint8_t* const buf, const uint16_t buf_len)
{
  HAL_UART_Transmit(&_handle, (uint8_t*)buf, buf_len, HAL_MAX_DELAY);
}

}  // namespace hal
