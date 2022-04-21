#ifndef TB6612_H
#define TB6612_H (1)

#include "digital_out.h"
#include "pwm.h"

namespace tb6612
{
static const float vdrop = 0.1;

enum class Direction
{
  REVERSE = -1,
  UNKNOWN_DIR = 0,
  FORWARD = 1
};

enum class MotorPolarity
{
  REVERSED = -1,
  STANDARD = 1
};

class MotorDriver
{
 public:
  MotorDriver(const hal::PwmPin& pwm, const hal::Pin& in1, const hal::Pin& in2,
              const MotorPolarity polarity = MotorPolarity::STANDARD);
  void setDirection(const Direction new_dir);
  void setDutyCycle(const uint8_t percent);
  void stop(void);
  hal::PwmOut pwm;

 private:
  hal::DigitalOut _in1;
  hal::DigitalOut _in2;
  MotorPolarity _polarity;
};
}  // namespace tb6612

#endif  // TB6612_H (1)