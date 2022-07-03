#ifndef I2C_H
#define I2C_H (1)

#include <cstdint>

#include "gpio.h"
#include "stm32f4xx_hal.h"

namespace hal
{
class I2CMaster
{
 public:
  I2CMaster(const uint8_t i2c_instance, const GPIO& scl, const GPIO& sda,
            const uint32_t clk_freq_hz);

  int8_t student_ready(const uint16_t dev_addr);
  int8_t transmit_blocking(const uint16_t dev_addr, const uint8_t* const data, const uint16_t size);
  int8_t receive_blocking(const uint16_t dev_addr, uint8_t* const data, const uint16_t size);
  int8_t read_device_memory(uint8_t dev_addr, uint8_t reg_addr, uint8_t* data, uint16_t len);
  int8_t write_device_memory(uint8_t dev_addr, uint8_t reg_addr, uint8_t* data, uint16_t len);

 private:
  hal::GPIO _scl;
  hal::GPIO _sda;

  I2C_HandleTypeDef _handle;
};

}  // namespace hal

#endif  // I2C_H (1)
