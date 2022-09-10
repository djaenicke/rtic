#include "quadrature_encoder.h"

#include <cassert>

namespace hal
{
static const uint8_t NUM_INTERRUPT_INPUTS = 5u;
static const uint8_t MIN_SUPPORTED_INTERRUPT_PIN_NUM = 5u;
static const uint8_t MAX_SUPPORTED_INTERRUPT_PIN_NUM = 9u;
static const int8_t LOOKUP_TABLE[] = { 0, -1, 1, 0, 1, 0, 0, -1, -1, 0, 0, 1, 0, 1, -1, 0 };

static hal::QuadratureEncoder* interrupt_encoders[NUM_INTERRUPT_INPUTS];

QuadratureEncoder::QuadratureEncoder(const Pin& cha, const Pin& chb, const EncoderPolarity polarity)
    : HwTimer(255, TimerMode::ENCODER_4X_MODE, 0.0f),
      _polarity(polarity),
      _timer_mode(false),
      _encoder_pulses(0),
      _encoder_value(0),
      _cha(cha, GpioMode::INPUT_BOTH_INT, hal::GpioPullType::NO_PULL, 0U, GpioSpeed::VERY_HIGH),
      _chb(chb, GpioMode::INPUT_BOTH_INT, hal::GpioPullType::NO_PULL, 0U, GpioSpeed::VERY_HIGH)
{
  if ((cha.port == chb.port) && (cha.num == chb.num))
  {
    assert(0);
  }

  if ((cha.num >= MIN_SUPPORTED_INTERRUPT_PIN_NUM) && (cha.num <= MAX_SUPPORTED_INTERRUPT_PIN_NUM))
  {
    interrupt_encoders[cha.num - MIN_SUPPORTED_INTERRUPT_PIN_NUM] = this;
  }
  else
  {
    assert(0);
  }

  if ((chb.num >= MIN_SUPPORTED_INTERRUPT_PIN_NUM) && (chb.num <= MAX_SUPPORTED_INTERRUPT_PIN_NUM))
  {
    interrupt_encoders[chb.num - MIN_SUPPORTED_INTERRUPT_PIN_NUM] = this;
  }
  else
  {
    assert(0);
  }
}

QuadratureEncoder::QuadratureEncoder(const uint8_t timer_instance, const GPIO& cha, const GPIO& chb,
                                     const EncoderPolarity polarity)
    : HwTimer(timer_instance, TimerMode::ENCODER_4X_MODE, 0.0f),
      _polarity(polarity),
      _timer_mode(true),
      _encoder_pulses(0),
      _encoder_value(0),
      _cha(cha),
      _chb(chb)
{
}

void QuadratureEncoder::start(void)
{
  if (_timer_mode)
  {
    HAL_TIM_Encoder_Start(&_timer_handle, TIM_CHANNEL_ALL);
  }
  else
  {
    HAL_NVIC_SetPriority(EXTI9_5_IRQn, 14, 0);
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
  }
}

void QuadratureEncoder::stop(void)
{
  if (_timer_mode)
  {
    HAL_TIM_Encoder_Stop(&_timer_handle, TIM_CHANNEL_ALL);
  }
  else
  {
    HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);
  }
}

int32_t QuadratureEncoder::getPulses(void)
{
  int32_t temp = 0;

  if (_timer_mode)
  {
    temp = _timer_handle.Instance->CNT;
    _timer_handle.Instance->CNT = 0;
    // apply polarity based on which direction the counter is counting
    if (temp > _max_pulses_per_cycle)
    {
      temp -= UINT16_MAX;
    }
  }
  else  // interrupt mode
  {
    stop();
    temp = _encoder_pulses;
    _encoder_pulses = 0;
    start();
  }

  return (temp * static_cast<int32_t>(_polarity));
}

void QuadratureEncoder::setMaxPulsesPerCycle(const uint16_t max_pulses)
{
  _max_pulses_per_cycle = max_pulses;
}

void QuadratureEncoder::_pinChangeISR(void)
{
  _encoder_value = _encoder_value << 2;
  _encoder_value = _encoder_value | (_cha << 1) | _chb;
  _encoder_pulses += LOOKUP_TABLE[_encoder_value & 0b1111];
}

}  // namespace hal

extern "C"
{
  void EXTI9_5_IRQHandler(void)
  {
    if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_5) != RESET)
    {
      if (NULL != hal::interrupt_encoders[0])
      {
        hal::interrupt_encoders[0]->_pinChangeISR();
      }
      __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_5);
    }

    if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_6) != RESET)
    {
      if (NULL != hal::interrupt_encoders[1])
      {
        hal::interrupt_encoders[1]->_pinChangeISR();
      }
      __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_6);
    }

    if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_7) != RESET)
    {
      if (NULL != hal::interrupt_encoders[2])
      {
        hal::interrupt_encoders[2]->_pinChangeISR();
      }
      __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_7);
    }

    if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_8) != RESET)
    {
      if (NULL != hal::interrupt_encoders[3])
      {
        hal::interrupt_encoders[3]->_pinChangeISR();
      }
      __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_8);
    }

    if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_9) != RESET)
    {
      if (NULL != hal::interrupt_encoders[4])
      {
        hal::interrupt_encoders[4]->_pinChangeISR();
      }
      __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_9);
    }
  }
}