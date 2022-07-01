#include "voltage_monitor.h"

namespace vm
{
VoltageMonitor::VoltageMonitor(const hal::AnalogIn& adc, const float scale_factor)
    : _adc(adc), _scale_factor(scale_factor)
{
}

float VoltageMonitor::read(void)
{
  return (_scale_factor * _adc.getValue());
}
}  // namespace vm
