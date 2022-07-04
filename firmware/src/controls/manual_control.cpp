#include "manual_control.h"

#include <cmath>

#include "rf_interface.h"

static const uint16_t MIN_10BIT_ADC_COUNTS = 0u;
static const uint16_t MAX_10BIT_ADC_COUNTS = 1023u;

// Range of joystick counts values where the joystick is considered not engaged
static const uint16_t JOYSTICK_DEADBAND_MIN_COUNTS = 475u;
static const uint16_t JOYSTICK_DEADBAND_MAX_COUNTS = 575u;

// Saturation values for the joystick counts
static const uint16_t JOYSTICK_MIN_COUNTS = 50u;
static const uint16_t JOYSTICK_MAX_COUNTS = 1000u;

static const float MIN_WHEEL_SPEED_RPS = -15.0f;
static const float MAX_WHEEL_SPEED_RPS = 15.0f;

static bool joystickInput2WheelSpeed(uint16_t counts, float* const wheel_speed_rps);

void processManualControl(bool* const manual_control_active, WheelSpeeds* const cmd)
{
  UserInputMessage ui_msg;

  if (userInputMessageBufferRead(&ui_msg))
  {
    // New user input message received
    *manual_control_active = false;

    // Process right joystick input
    if (joystickInput2WheelSpeed(ui_msg.r_joystick.y_counts, &cmd->fr_rps))
    {
      // Set the both right wheels to the same speed (tank drive)
      cmd->fr_rps *= -1.0f;  // right joystick is inverted on the controller
      cmd->rr_rps = cmd->fr_rps;
      *manual_control_active = true;
    }

    // Process left joystick input
    if (joystickInput2WheelSpeed(ui_msg.l_joystick.y_counts, &cmd->fl_rps))
    {
      // Set the both left wheels to the same speed (tank drive)
      cmd->rl_rps = cmd->fl_rps;
      *manual_control_active = true;
    }
  }
}

static bool joystickInput2WheelSpeed(uint16_t counts, float* const wheel_speed_rps)
{
  if (NULL == wheel_speed_rps)
  {
    return false;
  }

  // Is the joystick engaged?
  if ((counts >= JOYSTICK_DEADBAND_MIN_COUNTS) && (counts <= JOYSTICK_DEADBAND_MAX_COUNTS))
  {
    *wheel_speed_rps = 0.0f;
    return false;
  }

  // Saturate the joystick counts to ensure the min/max speeds can be achieved
  if (counts < JOYSTICK_MIN_COUNTS)
  {
    counts = MIN_10BIT_ADC_COUNTS;
  }
  else if (counts > JOYSTICK_MAX_COUNTS)
  {
    counts = MAX_10BIT_ADC_COUNTS;
  }

  *wheel_speed_rps =
      (counts * ((MAX_WHEEL_SPEED_RPS - MIN_WHEEL_SPEED_RPS) / MAX_10BIT_ADC_COUNTS)) -
      fabsf(MIN_WHEEL_SPEED_RPS);

  return true;
}
