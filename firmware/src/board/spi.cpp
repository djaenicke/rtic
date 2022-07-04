#include "spi.h"

#include <cassert>

namespace hal
{
SpiMaster::SpiMaster(const uint8_t spi_instance, const GPIO& mosi, const GPIO& miso,
                     const GPIO& sclk, const Pin& cs, const SpiBaudratePrescalar prescalar)
    : _mosi(mosi), _miso(miso), _sclk(sclk), _cs(cs)
{
  switch (spi_instance)
  {
    case 1:
      __HAL_RCC_SPI1_CLK_ENABLE();
      _handle.Instance = SPI1;
      break;
    case 2:
      __HAL_RCC_SPI2_CLK_ENABLE();
      _handle.Instance = SPI2;
      break;
    case 3:
      __HAL_RCC_SPI3_CLK_ENABLE();
      _handle.Instance = SPI3;
      break;
    case 4:
      __HAL_RCC_SPI4_CLK_ENABLE();
      _handle.Instance = SPI4;
      break;
    case 5:
      __HAL_RCC_SPI5_CLK_ENABLE();
      _handle.Instance = SPI5;
      break;
    default:
      assert(0);
      _handle.Instance = NULL;
      return;
  }

  switch (prescalar)
  {
    case SpiBaudratePrescalar::PRESCALER_2:
      _handle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
      break;
    case SpiBaudratePrescalar::PRESCALER_4:
      _handle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
      break;
    case SpiBaudratePrescalar::PRESCALER_8:
      _handle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
      break;
    case SpiBaudratePrescalar::PRESCALER_16:
      _handle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
      break;
    case SpiBaudratePrescalar::PRESCALER_32:
      _handle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
      break;
    case SpiBaudratePrescalar::PRESCALER_64:
      _handle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;
      break;
    case SpiBaudratePrescalar::PRESCALER_128:
      _handle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_128;
      break;
    case SpiBaudratePrescalar::PRESCALER_256:
      _handle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
      break;
    default:
      assert(0);
      return;
  }

  _handle.Init.Mode = SPI_MODE_MASTER;
  _handle.Init.Direction = SPI_DIRECTION_2LINES;
  _handle.Init.DataSize = SPI_DATASIZE_8BIT;
  _handle.Init.CLKPolarity = SPI_POLARITY_LOW;
  _handle.Init.CLKPhase = SPI_PHASE_1EDGE;
  _handle.Init.NSS = SPI_NSS_SOFT;
  _handle.Init.FirstBit = SPI_FIRSTBIT_MSB;
  _handle.Init.TIMode = SPI_TIMODE_DISABLE;
  _handle.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  _handle.Init.CRCPolynomial = 10;

  HAL_SPI_Init(&_handle);
}

void SpiMaster::csEnable(void)
{
  _cs = 0;
}

void SpiMaster::csDisable(void)
{
  _cs = 1;
}

int8_t SpiMaster::transfer(const uint8_t* const data_out, uint8_t* const data_in,
                           const uint16_t len)
{
  if (HAL_OK == HAL_SPI_TransmitReceive(&_handle, (uint8_t*)data_out, data_in, len, HAL_MAX_DELAY))
  {
    return 0;
  }
  else
  {
    return -1;
  }
}
}  // namespace hal
