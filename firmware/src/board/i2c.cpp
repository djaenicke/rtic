#include "i2c.h"

#include <cassert>

namespace hal
{
I2CMaster::I2CMaster(const uint8_t i2c_instance, const GPIO& scl, const GPIO& sda,
                     const uint32_t clk_freq_hz)
    : _scl(scl), _sda(sda)
{
  switch (i2c_instance)
  {
    case 1:
      __HAL_RCC_I2C1_CLK_ENABLE();
      _handle.Instance = I2C1;
      break;
    case 2:
      __HAL_RCC_I2C2_CLK_ENABLE();
      _handle.Instance = I2C2;
      break;
    case 3:
      __HAL_RCC_I2C3_CLK_ENABLE();
      _handle.Instance = I2C3;
      break;
    default:
      assert(0);
      _handle.Instance = NULL;
      return;
  }

  _handle.Init.ClockSpeed = clk_freq_hz;
  _handle.Init.DutyCycle = I2C_DUTYCYCLE_2;
  _handle.Init.OwnAddress1 = 0;
  _handle.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  _handle.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  _handle.Init.OwnAddress2 = 0;
  _handle.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  _handle.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

  HAL_I2C_Init(&_handle);
}

int8_t I2CMaster::studentReady(const uint16_t dev_addr)
{
  if (HAL_OK == HAL_I2C_IsDeviceReady(&_handle, (dev_addr << 1u), 100u, HAL_MAX_DELAY))
  {
    return 0;
  }
  else
  {
    return -1;
  }
}

int8_t I2CMaster::transmitBlocking(const uint16_t dev_addr, const uint8_t* const data,
                                   const uint16_t size)
{
  if (HAL_OK ==
      HAL_I2C_Master_Transmit(&_handle, (dev_addr << 1u), (uint8_t*)data, size, HAL_MAX_DELAY))
  {
    return 0;
  }
  else
  {
    return -1;
  }
}

int8_t I2CMaster::receiveBlocking(const uint16_t dev_addr, uint8_t* const data, const uint16_t size)
{
  if (HAL_OK ==
      HAL_I2C_Master_Receive(&_handle, (dev_addr << 1u), (uint8_t*)data, size, HAL_MAX_DELAY))
  {
    return 0;
  }
  else
  {
    return -1;
  }
}

int8_t I2CMaster::readDeviceMemory(const uint8_t dev_addr, const uint8_t reg_addr, uint8_t* data,
                                   const uint16_t len)
{
  if (HAL_OK == HAL_I2C_Mem_Read(&_handle, dev_addr, reg_addr, I2C_MEMADD_SIZE_8BIT, data, len,
                                 HAL_MAX_DELAY))
  {
    return 0;
  }
  else
  {
    return -1;
  }
}

int8_t I2CMaster::writeDeviceMemory(const uint8_t dev_addr, const uint8_t reg_addr, uint8_t* data,
                                    const uint16_t len)
{
  if (HAL_OK == HAL_I2C_Mem_Write(&_handle, dev_addr, reg_addr, I2C_MEMADD_SIZE_8BIT, data, len,
                                  HAL_MAX_DELAY))
  {
    return 0;
  }
  else
  {
    return -1;
  }
}

}  // namespace hal
