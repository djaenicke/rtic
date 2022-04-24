#ifndef MOTOR_CONFIG_H
#define MOTOR_CONFIG_H (1)

#include <cmath>

//!@brief The number of quadrature encoder pulses (4x mode) in one revolution
#define ENCODER_4X_PULSES_PER_REV (30 * 12 * 4)

//!@brief Conversion factor to convert encoder pulses to radians
#define ENCODER_PULSES_TO_RADIANS ((2.0f * M_PI) / ENCODER_4X_PULSES_PER_REV)

//!@brief Front right motor speed constant
#define FR_KV 2.70f

//!@brief Front left motor speed constant
#define FL_KV 2.63f

//!@brief Rear right motor speed constant
#define RR_KV 2.70f

//!@brief Rear left motor speed constant
#define RL_KV 2.70f

#endif // MOTOR_CONFIG_H
