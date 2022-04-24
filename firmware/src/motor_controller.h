#ifndef MOTOR_CONTROLLER_H
#define MOTOR_CONTROLLER_H (1)

#include <cstdint>

#include "gpio.h"
#include "quadrature_encoder.h"
#include "tb6612.h"

namespace mc
{
enum class ControlMode
{
  OPEN_LOOP = 0,
  CLOSED_LOOP_PID
};

class MotorController
{
 public:
  MotorController(const tb6612::MotorDriver& driver, const hal::QuadratureEncoder& encoder);
  MotorController(const tb6612::MotorDriver& driver, const hal::Pin& cha, const hal::Pin& chb,
                  const hal::EncoderPolarity polarity = hal::EncoderPolarity::STANDARD);
  void step(const float sp_rps, const float dt_s, const float max_u_volts);
  void startPWM(const hal::HwTimer* pwm_timer);
  void setMode(const ControlMode mode);
  void setSpeedConstant(const float kv);
  void setPulses2RadiansFactor(const float pulses_2_rps);
  float getSpeed(void);

 private:
  //! @brief Motor driver object for setting the motor speed and direction
  tb6612::MotorDriver _driver;
  //! @brief Quadrature encoder object for reading the motor speed and direction
  hal::QuadratureEncoder _encoder;
  //! @brief Control mode, i.e. open loop or closed loop via PID
  ControlMode _mode;
  //! @brief DC motor back EMF constant
  float _ke;
  //! @brief Constant to convert pulses into rads
  float _pulses_2_rad;
  //! @brief Actual angular velocity
  float _fb_rps;
  //! @brief Elapsed time between loops
  float _dt_s;
  //! @brief DC motor setpoint voltage
  float _sp_volts;
  //! @brief DC motor feedback voltage
  float _fb_volts;
  //! @brief DC motor actuation voltage
  float _u_volts;
  //! @brief DC motor actuation voltage as a % of max voltage
  uint8_t _u_percent;
};
}  // namespace mc

#endif  // MOTOR_CONTROLLER_H (1)