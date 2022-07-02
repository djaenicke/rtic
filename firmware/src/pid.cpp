#include "pid.h"

#include <cmath>
#include <cstdint>

namespace pid
{
PID::PID(const float tol)
{
  kp_ = 0.0f;
  ki_ = 0.0f;
  kd_ = 0.0f;
  tol_ = tol;

  last_e_ = 0.0f;
  integral_ = 0.0f;
}

float PID::step(const float sp, const float fb, const float dt, const float max, const float min)
{
  float u, d;           /* actuator command, derivative  */
  float ti = integral_; /* temporary integral            */
  float e = sp - fb;    /* error = setpoint - feedback */
  int8_t sign;

  // Ignore the error if it's smaller than the tolerance
  if (fabs(e) < tol_)
  {
    e = 0.0f;
  }

  d = (e - last_e_) / dt;
  ti += (((e + last_e_) / 2.0f) * dt);  // Trapezoidal integration

  u = (kp_ * e) + (ki_ * integral_) + (kd_ * d);

  last_e_ = e;

  // Determine the sign of u
  sign = std::signbit(u) ? -1 : 1;

  // Limit the integral term via clamping
  if (fabs(u) < max)
  {
    integral_ = ti;
  }

  // Perform saturation on the activation signal
  if (fabs(u) < min)
  {
    u = sign * min;
  }
  else if (fabs(u) > max)
  {
    u = sign * max;
  }

  return (u);
}

void PID::reset(const Gains& gains)
{
  kp_ = gains.kp;
  ki_ = gains.ki;
  kd_ = gains.kd;
  last_e_ = 0.0f;
  integral_ = 0.0f;
}

}  // namespace pid
