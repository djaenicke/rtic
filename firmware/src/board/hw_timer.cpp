#include "hw_timer.h"

#include <cmath>

namespace hal
{
HwTimer::HwTimer(const uint8_t timer_instance, const TimerMode mode, const float period_s,
                 const bool ch_1_enabled, const bool ch_2_enabled, const bool ch_3_enabled,
                 const bool ch_4_enabled)
{
  switch (timer_instance)
  {
    case 1:
      _timer_ptr = TIM1;
      break;
    case 2:
      _timer_ptr = TIM2;
      break;
    case 3:
      _timer_ptr = TIM3;
      break;
    case 4:
      _timer_ptr = TIM4;
      break;
    case 5:
      _timer_ptr = TIM5;
      break;
    case 6:
      _timer_ptr = TIM6;
      break;
    case 7:
      _timer_ptr = TIM7;
      break;
    case 9:
      _timer_ptr = TIM9;
      break;
    case 10:
      _timer_ptr = TIM10;
      break;
    case 11:
      _timer_ptr = TIM11;
      break;
    case 12:
      _timer_ptr = TIM12;
      break;
    case 13:
      _timer_ptr = TIM13;
      break;
    case 14:
      _timer_ptr = TIM14;
      break;
    default:
      _timer_ptr = NULL;
      // todo: assert
      return;
  }

  if (NULL == _timer_ptr)
  {
    return;
  }

  _timer_handle.Instance = _timer_ptr;
  _timer_handle.Init.Prescaler = 0;
  _timer_handle.Init.CounterMode = TIM_COUNTERMODE_UP;
  _timer_handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  _timer_handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

  TIM_ClockConfigTypeDef clk_cfg = { 0 };
  clk_cfg.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  HAL_TIM_ConfigClockSource(&_timer_handle, &clk_cfg);

  switch (mode)
  {
    case TimerMode::TIME_BASE:
    {
      break;
    }
    case TimerMode::INPUT_CAPTURE:
    {
      break;
    }
    case TimerMode::OUTPUT_COMPARE:
    {
      break;
    }
    case TimerMode::PWM_GENERATION:
    {
      uint16_t period_ticks = (uint16_t)roundf(period_s * getSourceFreqHz());
      _timer_handle.Init.Period = period_ticks;
      initPwmMode(ch_1_enabled, ch_2_enabled, ch_3_enabled, ch_4_enabled);
      break;
    }
    case TimerMode::ONE_PULSE_MODE_OUTPUT:
    {
      break;
    }
    case TimerMode::ENCODER_CH1_2X_MODE:
    {
      _timer_handle.Init.Period = 65535u;
      initEncoderMode(TIM_ENCODERMODE_TI1);
      break;
    }
    case TimerMode::ENCODER_CH2_2X_MODE:
    {
      _timer_handle.Init.Period = 65535u;
      initEncoderMode(TIM_ENCODERMODE_TI2);
      break;
    }
    case TimerMode::ENCODER_4X_MODE:
    {
      _timer_handle.Init.Period = 65535u;
      initEncoderMode(TIM_ENCODERMODE_TI12);
      break;
    }
    default:
      break;
  }
}

TIM_HandleTypeDef* HwTimer::getHandle(void)
{
  return &_timer_handle;
}

uint16_t HwTimer::getPeriod(void)
{
  return (uint16_t)_timer_handle.Init.Period;
}

uint32_t HwTimer::getSourceFreqHz(void)
{
  if (TIM9 == _timer_handle.Instance)
  {
    return HAL_RCC_GetPCLK2Freq();
  }
  else if (TIM12 == _timer_handle.Instance)
  {
    return HAL_RCC_GetPCLK1Freq();
  }

  return 0;
}

void HwTimer::initPwmMode(const bool ch_1_enabled, const bool ch_2_enabled, const bool ch_3_enabled,
                          const bool ch_4_enabled)
{
  if (TIM9 == _timer_handle.Instance)
  {
    __HAL_RCC_TIM9_CLK_ENABLE();
  }
  else if (TIM12 == _timer_handle.Instance)
  {
    __HAL_RCC_TIM12_CLK_ENABLE();
  }

  HAL_TIM_PWM_Init(&_timer_handle);

  TIM_OC_InitTypeDef oc_cfg = { 0 };
  oc_cfg.OCMode = TIM_OCMODE_PWM1;
  oc_cfg.Pulse = 0;
  oc_cfg.OCPolarity = TIM_OCPOLARITY_HIGH;
  oc_cfg.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  oc_cfg.OCFastMode = TIM_OCFAST_DISABLE;
  oc_cfg.OCIdleState = TIM_OCIDLESTATE_RESET;
  oc_cfg.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (ch_1_enabled)
  {
    HAL_TIM_PWM_ConfigChannel(&_timer_handle, &oc_cfg, TIM_CHANNEL_1);
  }
  if (ch_2_enabled)
  {
    HAL_TIM_PWM_ConfigChannel(&_timer_handle, &oc_cfg, TIM_CHANNEL_2);
  }
  if (ch_3_enabled)
  {
    HAL_TIM_PWM_ConfigChannel(&_timer_handle, &oc_cfg, TIM_CHANNEL_3);
  }
  if (ch_4_enabled)
  {
    HAL_TIM_PWM_ConfigChannel(&_timer_handle, &oc_cfg, TIM_CHANNEL_4);
  }
}

void HwTimer::initEncoderMode(const uint32_t encoder_mode)
{
  if (TIM1 == _timer_handle.Instance)
  {
    __HAL_RCC_TIM1_CLK_ENABLE();
  }
  else if (TIM3 == _timer_handle.Instance)
  {
    __HAL_RCC_TIM3_CLK_ENABLE();
  }
  else if (TIM5 == _timer_handle.Instance)
  {
    __HAL_RCC_TIM5_CLK_ENABLE();
  }
  else if (TIM8 == _timer_handle.Instance)
  {
    __HAL_RCC_TIM8_CLK_ENABLE();
  }

  TIM_Encoder_InitTypeDef encoder_cfg = { 0 };

  encoder_cfg.EncoderMode = encoder_mode;

  encoder_cfg.IC1Filter = 0;
  encoder_cfg.IC1Polarity = TIM_INPUTCHANNELPOLARITY_RISING;
  encoder_cfg.IC1Prescaler = TIM_ICPSC_DIV1;
  encoder_cfg.IC1Selection = TIM_ICSELECTION_DIRECTTI;

  encoder_cfg.IC2Filter = 0;
  encoder_cfg.IC2Polarity = TIM_INPUTCHANNELPOLARITY_RISING;
  encoder_cfg.IC2Prescaler = TIM_ICPSC_DIV1;
  encoder_cfg.IC2Selection = TIM_ICSELECTION_DIRECTTI;

  HAL_TIM_Encoder_Init(&_timer_handle, &encoder_cfg);
}

}  // namespace hal
