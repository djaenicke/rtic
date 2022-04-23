#include "tb6612.h"

namespace tb6612
{
MotorDriver::MotorDriver(const hal::PwmPin& pwm, const hal::Pin& in1, const hal::Pin& in2,
                         const MotorPolarity polarity)
    : pwm(pwm), _in1(in1), _in2(in2), _polarity(polarity)
{
}

void MotorDriver::setDirection(const Direction new_dir)
{
  Direction dir = new_dir;

  if (MotorPolarity::STANDARD != _polarity)
  {
    if (Direction::FORWARD == new_dir)
    {
      dir = Direction::REVERSE;
    }
    else if (Direction::REVERSE == new_dir)
    {
      dir = Direction::FORWARD;
    }
    else
    {
      dir = Direction::UNKNOWN_DIR;
    }
  }

  if (Direction::FORWARD == dir)
  {
    _in1 = 1u;
    _in2 = 0u;
  }
  else if (Direction::REVERSE == dir)
  {
    _in1 = 0u;
    _in2 = 1u;
  }
  else
  {
    // todo assert
  }
}

void MotorDriver::setDutyCycle(const uint8_t percent)
{
  pwm.setDutyCycle(percent * 0.01f);
}

void MotorDriver::stop(void)
{
  _in1 = 0;
  _in2 = 0;
  pwm.setDutyCycle(1.0f);
}
}  // namespace tb6612