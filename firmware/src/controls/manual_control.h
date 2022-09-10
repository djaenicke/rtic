#ifndef MANUAL_CONTROL_H
#define MANUAL_CONTROL_H (1)

#include "velocity_control.h"

void processManualControl(bool* const manual_control_active, WheelSpeeds* const cmd);

#endif  // MANUAL_CONTROL_H