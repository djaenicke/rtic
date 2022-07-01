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

hal::HwTimer timer_d1_pwm(9, hal::TimerMode::PWM_GENERATION, 0.0001f, true, true);
hal::HwTimer timer_d2_pwm(12, hal::TimerMode::PWM_GENERATION, 0.0001f, true, true);
tb6612::TB6612 driver_1(D1_PWMA, D1_PWMB, D1_AIN1, D1_AIN2, D1_BIN1, D1_BIN2, D1_STBY);
tb6612::TB6612 driver_2(D2_PWMA, D2_PWMB, D2_AIN1, D2_AIN2, D2_BIN1, D2_BIN2, D2_STBY);

hal::QuadratureEncoder fr_encoder(5u, D1_MOTORA_CHA, D1_MOTORA_CHB);
hal::QuadratureEncoder fl_encoder(3u, D1_MOTORB_CHA, D1_MOTORB_CHB, hal::EncoderPolarity::REVERSED);
hal::QuadratureEncoder rr_encoder(D2_MOTORA_CHA, D2_MOTORA_CHB);
hal::QuadratureEncoder rl_encoder(D2_MOTORB_CHA, D2_MOTORB_CHB, hal::EncoderPolarity::REVERSED);

int main(void)
{
  driver_1.pwm_a.start(&timer_d1_pwm);
  driver_1.pwm_b.start(&timer_d1_pwm);

  driver_2.pwm_a.start(&timer_d2_pwm);
  driver_2.pwm_b.start(&timer_d2_pwm);

  fr_encoder.start();
  fl_encoder.start();
  rr_encoder.start();
  rl_encoder.start();

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
