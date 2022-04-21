#include <stdio.h>

#include "board.h"
#include "digital_out.h"
#include "hw_timer.h"
#include "io.h"
#include "pwm.h"
#include "quadrature_encoder.h"
#include "tb6612.h"

#define PRINT_BUF_LEN 256

char buffer[PRINT_BUF_LEN];

hal::DigitalOut heartbeat(HEARTBEAT_LED);

hal::HwTimer timer_d1_pwm(9u, hal::TimerMode::PWM_GENERATION, 0.0001f, true, true);
hal::HwTimer timer_d2_pwm(12u, hal::TimerMode::PWM_GENERATION, 0.0001f, true, true);

tb6612::MotorDriver fr_driver(D1_PWMA, D1_AIN1, D1_AIN2);
tb6612::MotorDriver fl_driver(D1_PWMB, D1_BIN1, D1_BIN2);
hal::DigitalOut f_driver_standby(D1_STBY);

tb6612::MotorDriver rr_driver(D2_PWMA, D2_AIN1, D2_AIN2);
tb6612::MotorDriver rl_driver(D2_PWMB, D2_BIN1, D2_BIN2);
hal::DigitalOut r_driver_standby(D2_STBY);

hal::QuadratureEncoder fr_encoder(5u, D1_MOTORA_CHA, D1_MOTORA_CHB);
hal::QuadratureEncoder fl_encoder(3u, D1_MOTORB_CHA, D1_MOTORB_CHB, hal::EncoderPolarity::REVERSED);

hal::QuadratureEncoder rr_encoder(D2_MOTORA_CHA, D2_MOTORA_CHB);
hal::QuadratureEncoder rl_encoder(D2_MOTORB_CHA, D2_MOTORB_CHB, hal::EncoderPolarity::REVERSED);

int main(void)
{
  fr_driver.pwm.start(&timer_d1_pwm);
  fl_driver.pwm.start(&timer_d1_pwm);

  rr_driver.pwm.start(&timer_d2_pwm);
  rl_driver.pwm.start(&timer_d2_pwm);

  f_driver_standby = 1u;
  r_driver_standby = 1u;

  fr_encoder.start();
  fl_encoder.start();
  rr_encoder.start();
  rl_encoder.start();

  fr_driver.setDirection(tb6612::Direction::FORWARD);
  fl_driver.setDirection(tb6612::Direction::FORWARD);
  fr_driver.setDutyCycle(25u);
  fl_driver.setDutyCycle(25u);

  rr_driver.setDirection(tb6612::Direction::FORWARD);
  rl_driver.setDirection(tb6612::Direction::FORWARD);
  rr_driver.setDutyCycle(25u);
  rl_driver.setDutyCycle(25u);

  while (1)
  {
    heartbeat.toggle();

    snprintf(buffer, PRINT_BUF_LEN, "fr encoder counts = %ld\r\n", fr_encoder.getPulses());
    serialTxString(buffer);

    snprintf(buffer, PRINT_BUF_LEN, "fl encoder counts = %ld\r\n", fl_encoder.getPulses());
    serialTxString(buffer);

    snprintf(buffer, PRINT_BUF_LEN, "rr encoder counts = %ld\r\n", rr_encoder.getPulses());
    serialTxString(buffer);

    snprintf(buffer, PRINT_BUF_LEN, "rl encoder counts = %ld\r\n", rl_encoder.getPulses());
    serialTxString(buffer);

    delayMs(500u);
  }

  return 0;
}
