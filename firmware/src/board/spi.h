#ifndef SPI_H
#define SPI_H (1)

#include <cstdint>

#include "digital_out.h"
#include "gpio.h"
#include "stm32f4xx_hal.h"

namespace hal
{
class SpiMaster
{
  enum class SpiBaudratePrescalar
  {
    PRESCALER_2 = 0,
    PRESCALER_4,
    PRESCALER_8,
    PRESCALER_16,
    PRESCALER_32,
    PRESCALER_64,
    PRESCALER_128,
    PRESCALER_256
  };

 public:
  SpiMaster(const uint8_t spi_instance, const GPIO& mosi, const GPIO& miso, const GPIO& sclk,
            const Pin& cs,
            const SpiBaudratePrescalar prescalar = SpiBaudratePrescalar::PRESCALER_2);
  void csEnable(void);
  void csDisable(void);
  int8_t transfer(const uint8_t* const data_out, uint8_t* const data_in, const uint16_t len);

 private:
  hal::GPIO _mosi;
  hal::GPIO _miso;
  hal::GPIO _sclk;
  hal::DigitalOut _cs;

  SPI_HandleTypeDef _handle;
};

}  // namespace hal

#endif  // SPI_H (1)
