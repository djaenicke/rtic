#include "serial_logger.h"

#include "cmsis_os.h"

void threadSerialLogger(const void* argument)
{
  for (;;)
  {
    osDelay(10u);
  }
}
