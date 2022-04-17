#ifndef TB6612_H
#define TB6612_H (1)

#include "digital_out.h"
#include "pwm.h"

namespace tb6612
{
static const float vdrop = 0.1;

typedef enum
{
  REVERSE = -1,
  UNKNOWN_DIR = 0,
  FORWARD = 1
} Direction;

typedef enum
{
  MOTOR_A = 0,
  MOTOR_B
} MotorId;

typedef enum
{
  REVERSED = -1,
  STANDARD = 1
} MotorPolarity;

class TB6612
{
 public:
  TB6612(const hal::PwmPin& pwm_a, const hal::PwmPin& pwm_b, const hal::Pin& ain1,
         const hal::Pin& ain2, const hal::Pin& bin1, const hal::Pin& bin2, const hal::Pin& stby,
         const MotorPolarity a_polarity = STANDARD, const MotorPolarity b_polarity = STANDARD);

  void setDirection(const MotorId motor, const Direction new_dir);
  Direction getDirection(const MotorId motor);
  void setDutyCycle(const MotorId motor, const uint8_t percent);
  void stop(const MotorId motor);
  void standby(void);
  hal::PwmOut pwm_a;
  hal::PwmOut pwm_b;

 private:
  Direction handlePolarity(const MotorPolarity polarity, const Direction new_dir);

  hal::DigitalOut _a_in1;
  hal::DigitalOut _a_in2;
  hal::DigitalOut _b_in1;
  hal::DigitalOut _b_in2;
  hal::DigitalOut _stby;
  MotorPolarity _a_polarity;
  MotorPolarity _b_polarity;
  Direction _motor_a_dir;
  Direction _motor_b_dir;
};

}  // namespace tb6612

#endif  // TB6612_H (1)