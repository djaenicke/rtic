#include "gpio.h"

#include <cassert>

namespace hal
{
GPIO::GPIO(const Pin& pin, const GpioMode mode, const GpioPullType pull, const uint8_t alt_function,
           const GpioSpeed speed)
{
  _pin_select = (1u << pin.num);

  switch (pin.port)
  {
    case Port::A:
      if (0 == READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOAEN))
      {
        __HAL_RCC_GPIOA_CLK_ENABLE();
      }
      _port_ptr = GPIOA;
      break;
    case Port::B:
      if (0 == READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOBEN))
      {
        __HAL_RCC_GPIOB_CLK_ENABLE();
      }
      _port_ptr = GPIOB;
      break;
    case Port::C:
      if (0 == READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOCEN))
      {
        __HAL_RCC_GPIOC_CLK_ENABLE();
      }
      _port_ptr = GPIOC;
      break;
    case Port::D:
      if (0 == READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIODEN))
      {
        __HAL_RCC_GPIOD_CLK_ENABLE();
      }
      _port_ptr = GPIOD;
      break;
    case Port::H:
      if (0 == READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOHEN))
      {
        __HAL_RCC_GPIOH_CLK_ENABLE();
      }
      _port_ptr = GPIOH;
      break;
    default:
      assert(0);
      _port_ptr = NULL;
      return;
  }

  GPIO_InitTypeDef hal_gpio = { 0 };

  hal_gpio.Pin = _pin_select;

  switch (mode)
  {
    case GpioMode::INPUT:
      hal_gpio.Mode = GPIO_MODE_INPUT;
      break;
    case GpioMode::OUTPUT_PUSH_PULL:
      hal_gpio.Mode = GPIO_MODE_OUTPUT_PP;
      break;
    case GpioMode::OUTPUT_OPEN_DRAIN:
      hal_gpio.Mode = GPIO_MODE_OUTPUT_OD;
      break;
    case GpioMode::AF_PUSH_PULL:
      hal_gpio.Mode = GPIO_MODE_AF_PP;
      hal_gpio.Alternate = alt_function;
      break;
    case GpioMode::AF_OPEN_DRAIN:
      hal_gpio.Mode = GPIO_MODE_AF_OD;
      hal_gpio.Alternate = alt_function;
      break;
    case GpioMode::ANALOG:
      hal_gpio.Mode = GPIO_MODE_ANALOG;
      break;
    case GpioMode::INPUT_RISING_INT:
      hal_gpio.Mode = GPIO_MODE_IT_RISING;
      break;
    case GpioMode::INPUT_FALLING_INT:
      hal_gpio.Mode = GPIO_MODE_IT_FALLING;
      break;
    case GpioMode::INPUT_BOTH_INT:
      hal_gpio.Mode = GPIO_MODE_IT_RISING_FALLING;
      break;
    default:
      assert(0);
      return;
  }

  if (GpioMode::INPUT != mode)
  {
    HAL_GPIO_WritePin(_port_ptr, _pin_select, GPIO_PIN_RESET);
  }

  switch (pull)
  {
    case GpioPullType::NO_PULL:
      hal_gpio.Pull = GPIO_NOPULL;
      break;
    case GpioPullType::PULL_UP:
      hal_gpio.Pull = GPIO_PULLUP;
      break;
    case GpioPullType::PULL_DOWN:
      hal_gpio.Pull = GPIO_PULLDOWN;
      break;
    default:
      assert(0);
      return;
  }

  hal_gpio.Speed = static_cast<uint8_t>(speed);

  HAL_GPIO_Init(_port_ptr, &hal_gpio);
}

GPIO::operator uint8_t()
{
  return (uint8_t)HAL_GPIO_ReadPin(this->_port_ptr, this->_pin_select);
}

}  // namespace hal
