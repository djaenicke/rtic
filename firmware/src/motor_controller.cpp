#include "motor_controller.h"

#include <cmath>

#include "serial_logger.h"

namespace mc
{
MotorController::MotorController(const tb6612::MotorDriver& driver,
                                 const hal::QuadratureEncoder& encoder)
    : _driver(driver),
      _encoder(encoder),
      _mode(ControlMode::OPEN_LOOP),
      _ke(0.0f),
      _pulses_2_rad(0.0f)
{
  _encoder.start();
}

MotorController::MotorController(const tb6612::MotorDriver& driver, const hal::Pin& cha,
                                 const hal::Pin& chb, const hal::EncoderPolarity polarity)
    : _driver(driver),
      _encoder(cha, chb, polarity),
      _mode(ControlMode::OPEN_LOOP),
      _ke(0.0f),
      _pulses_2_rad(0.0f)
{
}

/*!
 * @brief This executes the controller.
 *
 * @param[in] sp_rps: The motor speed setpoint (rad/s)
 * @param[in] dt_s: The elapsed time in seconds between executions.
 * @param[in] max_u_volts: The maximum achievable actuation voltage.
 *
 */
void MotorController::step(const float sp_rps, const float dt_s, const float max_u_volts)
{
  // Measure the current shaft speed
  _fb_rps = (_encoder.getPulses() * _pulses_2_rad) / dt_s;

  // Compute the voltage setpoint.
  _sp_volts = sp_rps * _ke;

  if (ControlMode::OPEN_LOOP == _mode)
  {
    // Saturate the setpoint to be within the actuator voltage range.
    const int8_t sign = _sp_volts <= 0.0f ? -1 : 1;
    _u_volts = fabsf(_sp_volts) < max_u_volts ? _sp_volts : sign * max_u_volts;
  }

  // Convert the actuation voltage to a percent duty cycle
  _u_percent = static_cast<uint8_t>(fabs(_u_volts) * (100.0f / max_u_volts));

  _driver.setDirection(tb6612::Direction::FORWARD);
  _driver.setDutyCycle(_u_percent);
}

/*!
 * @brief This method starts the hardware timer that will be used to generate the driver PWM signal.
 *
 * @param[in] pwm_timer: Pointer to the HAL hardware timer object configured for PWM mode
 *
 */
void MotorController::startPWM(const hal::HwTimer* pwm_timer)
{
  _driver.pwm.start(pwm_timer);
}

/*!
 * @brief This method sets the controller mode, i.e. open loop or closed loop via PID.
 *
 * @param[in] mode: The controller mode
 *
 */
void MotorController::setMode(const ControlMode mode)
{
  _mode = mode;
}

/*!
 * @brief This method sets the speed constant for the DC motor being controlled.
 *
 * @param[in] kv: The motor speed constant (no load omega (rad/s) / input voltage (volts))
 *
 */
void MotorController::setSpeedConstant(const float kv)
{
  // Store as the back EMF constant
  _ke = 1.0f / kv;
}

/*!
 * @brief This method sets the pulses to rad conversion factor used when calculating shaft speed.
 *
 * @param[in] pulses_2_rad: The conversion factor to set.
 *
 */
void MotorController::setPulses2RadiansFactor(const float pulses_2_rad)
{
  // Store as the back EMF constant
  _pulses_2_rad = pulses_2_rad;
}

/*!
 * @brief This method returns the last measured shaft speed (rad/s).
 *
 * @return The last measured shaft speed
 *
 */
float MotorController::getSpeed(void)
{
  return _fb_rps;
}

}  // namespace mc