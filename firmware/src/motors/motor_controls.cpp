#include "motor_controls.h"

#include "analog_in.h"
#include "cmsis_os.h"
#include "digital_out.h"
#include "hw_timer.h"
#include "io.h"
#include "low_pass_filter.h"
#include "motor_config.h"
#include "motor_controller.h"
#include "pwm.h"
#include "quadrature_encoder.h"
#include "serial_logger.h"
#include "tb6612.h"
#include "threads.h"
#include "voltage_monitor.h"

static vm::VoltageMonitor vbatt_monitor(VBATT_ADC, VBATT_SCALE_FACTOR);

static hal::HwTimer f_pwm_timer(9u, hal::TimerMode::PWM_GENERATION, 0.0001f, true, true);
static hal::DigitalOut f_driver_standby(D1_STBY);

static hal::HwTimer r_pwm_timer(12u, hal::TimerMode::PWM_GENERATION, 0.0001f, true, true);
static hal::DigitalOut r_driver_standby(D2_STBY);

static mc::MotorController fr_controller(tb6612::MotorDriver(D1_PWMA, D1_AIN1, D1_AIN2),
                                         hal::QuadratureEncoder(5u, D1_MOTORA_CHA, D1_MOTORA_CHB));

static mc::MotorController fl_controller(tb6612::MotorDriver(D1_PWMB, D1_BIN1, D1_BIN2),
                                         hal::QuadratureEncoder(3u, D1_MOTORB_CHA, D1_MOTORB_CHB,
                                                                hal::EncoderPolarity::REVERSED));

static mc::MotorController rr_controller(tb6612::MotorDriver(D2_PWMA, D2_AIN1, D2_AIN2),
                                         D2_MOTORA_CHA, D2_MOTORA_CHB);

static mc::MotorController rl_controller(tb6612::MotorDriver(D2_PWMB, D2_BIN1, D2_BIN2),
                                         D2_MOTORB_CHA, D2_MOTORB_CHB,
                                         hal::EncoderPolarity::REVERSED);

static const float VBATT_FILT_ALPHA = 0.4f;
static const float MAX_MOTOR_VOLTAGE = 12.0f;

static void initMotorControllers(void);

void threadMotorControls(const void* argument)
{
  static TickType_t last_wake_time = xTaskGetTickCount();
  const TickType_t cycle_time_ticks = MOTOR_CONTROLS_PERIOD_MS * portTICK_PERIOD_MS;
  const float period_s = MOTOR_CONTROLS_PERIOD_MS / 1000.0f;
  static float meas_vbatt = 0.0f;
  static float max_vbatt = 0.0f;

  initMotorControllers();
  logMessage(LOG_DEBUG, "MotorControlsThread started.\r\n");

  for (;;)
  {
    vTaskDelayUntil(&last_wake_time, cycle_time_ticks);

    // Determine the max actuation voltage based on the vbatt measurement
    meas_vbatt = lpFilter(vbatt_monitor.read(), meas_vbatt, VBATT_FILT_ALPHA);
    if (meas_vbatt < MAX_MOTOR_VOLTAGE)
    {
      max_vbatt = (meas_vbatt - tb6612::vdrop);
    }
    else
    {
      max_vbatt = MAX_MOTOR_VOLTAGE - tb6612::vdrop;
    }

    fr_controller.step(0.0f, period_s, max_vbatt);
    fl_controller.step(0.0f, period_s, max_vbatt);
    rr_controller.step(0.0f, period_s, max_vbatt);
    rl_controller.step(0.0f, period_s, max_vbatt);
  }
}

static void initMotorControllers(void)
{
  fr_controller.startPWM(&f_pwm_timer);
  fr_controller.setSpeedConstant(FR_KV);
  fr_controller.setPulses2RadiansFactor(ENCODER_PULSES_TO_RADIANS);

  fl_controller.startPWM(&f_pwm_timer);
  fl_controller.setSpeedConstant(FL_KV);
  fl_controller.setPulses2RadiansFactor(ENCODER_PULSES_TO_RADIANS);

  rr_controller.startPWM(&r_pwm_timer);
  rr_controller.setSpeedConstant(RR_KV);
  rr_controller.setPulses2RadiansFactor(ENCODER_PULSES_TO_RADIANS);

  rl_controller.startPWM(&r_pwm_timer);
  rl_controller.setSpeedConstant(RL_KV);
  rl_controller.setPulses2RadiansFactor(ENCODER_PULSES_TO_RADIANS);

  f_driver_standby = 1u;
  r_driver_standby = 1u;
}
