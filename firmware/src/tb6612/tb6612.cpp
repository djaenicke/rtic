#include "tb6612.h"

namespace tb6612
{
TB6612::TB6612(const hal::PwmPin& pwm_a, const hal::PwmPin& pwm_b, const hal::Pin& ain1, const hal::Pin& ain2,
               const hal::Pin& bin1, const hal::Pin& bin2, const hal::Pin& stby, const MotorPolarity a_polarity,
               const MotorPolarity b_polarity)
    : pwm_a(pwm_a),
      pwm_b(pwm_b),
      _a_in1(ain1),
      _a_in2(ain2),
      _b_in1(bin1),
      _b_in2(bin2),
      _stby(stby),
      _a_polarity(a_polarity),
      _b_polarity(b_polarity)
{
  _stby = 1;
}

Direction TB6612::handlePolarity(const MotorPolarity polarity, const Direction new_dir)
{
  if (STANDARD == polarity)
  {
    return new_dir;
  }
  else if (FORWARD == new_dir)
  {
    return REVERSE;
  }
  else if (REVERSE == new_dir)
  {
    return FORWARD;
  }
  else
  {
    return UNKNOWN_DIR;
  }
}

void TB6612::setDirection(const MotorId motor, const Direction new_dir)
{
  Direction dir = UNKNOWN_DIR;

  if (MOTOR_A == motor)
  {
    dir = handlePolarity(_a_polarity, new_dir);
    _motor_a_dir = new_dir;
  }
  else if (MOTOR_B == motor)
  {
    dir = handlePolarity(_b_polarity, new_dir);
    _motor_b_dir = new_dir;
  }

  switch (dir)
  {
    case FORWARD:
    {
      switch (motor)
      {
        case MOTOR_A:
          _a_in1 = 1;
          _a_in2 = 0;
          break;
        case MOTOR_B:
          _b_in1 = 1;
          _b_in2 = 0;
          break;
        default:
          // todo assert
          break;
      }
      break;
    }
    case REVERSE:
    {
      switch (motor)
      {
        case MOTOR_A:
          _a_in1 = 0;
          _a_in2 = 1;
          break;
        case MOTOR_B:
          _b_in1 = 0;
          _b_in2 = 1;
          break;
        default:
          // todo assert
          break;
      }
      break;
    }
    default:
      // todo assert
      break;
  }
}

Direction TB6612::getDirection(const MotorId motor)
{
  switch (motor)
  {
    case MOTOR_A:
      return _motor_a_dir;
    case MOTOR_B:
      return _motor_b_dir;
    default:
      return UNKNOWN_DIR;
  }
}

void TB6612::setDutyCycle(const MotorId motor, const uint8_t percent)
{
  switch (motor)
  {
    case MOTOR_A:
      pwm_a.setDutyCycle(percent * 0.01f);
      break;
    case MOTOR_B:
      pwm_b.setDutyCycle(percent * 0.01f);
      break;
    default:
      // todo assert
      break;
  }
}

void TB6612::stop(const MotorId motor)
{
  switch (motor)
  {
    case MOTOR_A:
      _a_in1 = 0;
      _a_in2 = 0;
      pwm_a.setDutyCycle(1.0f);
      break;
    case MOTOR_B:
      _b_in1 = 0;
      _b_in2 = 0;
      pwm_a.setDutyCycle(1.0f);
      break;
    default:
      // todo assert
      break;
  }
}

void TB6612::standby(void)
{
  _stby = 0;
}

}  // namespace tb6612