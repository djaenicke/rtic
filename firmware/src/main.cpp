#include "board.h"
#include "digital_out.h"
#include "hw_timer.h"
#include "io.h"
#include "pwm.h"
#include "tb6612.h"

hal::DigitalOut heartbeat(HEARTBEAT_LED);

hal::HwTimer timer_d1_pwm(9, hal::TimerMode::PWM_GENERATION, 0.0001f, true, true);
hal::HwTimer timer_d2_pwm(12, hal::TimerMode::PWM_GENERATION, 0.0001f, true, true);
tb6612::TB6612 driver_1(D1_PWMA, D1_PWMB, D1_AIN1, D1_AIN2, D1_BIN1, D1_BIN2, D1_STBY);
tb6612::TB6612 driver_2(D2_PWMA, D2_PWMB, D2_AIN1, D2_AIN2, D2_BIN1, D2_BIN2, D2_STBY);

int main(void)
{
  driver_1.pwm_a.start(&timer_d1_pwm);
  driver_1.pwm_b.start(&timer_d1_pwm);

  driver_2.pwm_a.start(&timer_d2_pwm);
  driver_2.pwm_b.start(&timer_d2_pwm);

  driver_1.setDirection(tb6612::MOTOR_A, tb6612::FORWARD);
  driver_1.setDirection(tb6612::MOTOR_B, tb6612::FORWARD);
  driver_1.setDutyCycle(tb6612::MOTOR_A, 25u);
  driver_1.setDutyCycle(tb6612::MOTOR_B, 25u);

  driver_2.setDirection(tb6612::MOTOR_A, tb6612::FORWARD);
  driver_2.setDirection(tb6612::MOTOR_B, tb6612::FORWARD);
  driver_2.setDutyCycle(tb6612::MOTOR_A, 25u);
  driver_2.setDutyCycle(tb6612::MOTOR_B, 25u);

  while (1)
  {
    heartbeat.toggle();
    delayMs(500u);
  }

  return 0;
}
