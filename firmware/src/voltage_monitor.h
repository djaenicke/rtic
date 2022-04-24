#ifndef VOLTAGE_MONITOR_H
#define VOLTAGE_MONITOR_H (1)

#include <cstdint>

#include "analog_in.h"

namespace vm
{
class VoltageMonitor
{
 public:
  VoltageMonitor(const hal::AnalogIn& adc, const float scale_factor);
  float read(void);

 private:
  hal::AnalogIn _adc;
  float _scale_factor;
};

}  // namespace vm

#endif  // VOLTAGE_MONITOR_H (1)
