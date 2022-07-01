#include "analog_in.h"

#include "serial_logger.h"

namespace hal
{
AnalogIn::AnalogIn(const Pin& pin) : GPIO(pin, GpioMode::ANALOG)
{
  ADC_ChannelConfTypeDef channel_cfg = { 0 };

  switch (pin.num)
  {
    case 5:
      channel_cfg.Channel = ADC_CHANNEL_5;
      break;
    default:
      _handle.Instance = NULL;
      return;
  }

  __HAL_RCC_ADC1_CLK_ENABLE();

  _handle.Instance = ADC1;
  _handle.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
  _handle.Init.Resolution = ADC_RESOLUTION_12B;
  _handle.Init.ScanConvMode = DISABLE;
  _handle.Init.ContinuousConvMode = ENABLE;
  _handle.Init.DiscontinuousConvMode = DISABLE;
  _handle.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  _handle.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  _handle.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  _handle.Init.NbrOfConversion = 1;
  _handle.Init.DMAContinuousRequests = DISABLE;
  _handle.Init.EOCSelection = ADC_EOC_SEQ_CONV;
  HAL_ADC_Init(&_handle);

  channel_cfg.Rank = 1;
  channel_cfg.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  HAL_ADC_ConfigChannel(&_handle, &channel_cfg);
}

uint16_t AnalogIn::getValue(void)
{
  uint16_t rv = 0;

  if (NULL != _handle.Instance)
  {
    HAL_ADC_Start(&_handle);
    if (HAL_OK == HAL_ADC_PollForConversion(&_handle, 5))
    {
      rv = (uint16_t)HAL_ADC_GetValue(&_handle);
    }
    HAL_ADC_Stop(&_handle);
  }

  return rv;
}
}  // namespace hal
