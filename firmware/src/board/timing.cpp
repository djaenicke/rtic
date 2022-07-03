#include "timing.h"

#include "cmsis_os.h"

void delayMs(const uint32_t delay_ms)
{
  const TickType_t start_ticks = xTaskGetTickCount();
  const TickType_t delay_ticks = delay_ms * portTICK_PERIOD_MS;
  while ((xTaskGetTickCount() - start_ticks) < delay_ticks)
  {
  }
}
