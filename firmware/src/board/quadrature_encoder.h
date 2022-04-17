#ifndef QUAD_ENCODER_H
#define QUAD_ENCODER_H (1)

#include <cstdint>

#include "gpio.h"
#include "hw_timer.h"
#include "stm32f4xx_hal.h"

namespace hal
{
enum class EncoderPolarity
{
  REVERSED = -1,
  STANDARD = 1
};

class QuadratureEncoder : public HwTimer
{
 public:
  QuadratureEncoder();
  QuadratureEncoder(const Pin& cha, const Pin& chb,
                    const EncoderPolarity polarity = EncoderPolarity::STANDARD);
  QuadratureEncoder(const uint8_t timer_instance, const GPIO& cha, const GPIO& chb,
                    const EncoderPolarity polarity = EncoderPolarity::STANDARD);
  void start(void);
  void stop(void);
  int32_t getPulses(void);
  void setMaxPulsesPerCycle(const uint16_t max_pulses);
  void _pinChangeISR(void);

 private:
  EncoderPolarity _polarity;
  bool _timer_mode;
  uint16_t _prev_timer_cnt;
  volatile int32_t _encoder_pulses;
  volatile uint8_t _encoder_value;
  hal::GPIO _cha;
  hal::GPIO _chb;
  uint16_t _max_pulses_per_cycle = 10000;
};

}  // namespace hal

#endif  // QUAD_ENCODER_H (1)
