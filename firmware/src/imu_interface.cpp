#include "imu_interface.h"

#include "cmsis_os.h"
#include "i2c.h"
#include "io.h"
#include "serial_logger.h"
#include "threads.h"

static hal::I2CMaster imu(1u, IMU_SCL, IMU_SDA, 100000u);

void threadImuInterface(const void* argument)
{
  static TickType_t last_wake_time = xTaskGetTickCount();
  const TickType_t cycle_time_ticks = IMU_INTERFACE_PERIOD_MS * portTICK_PERIOD_MS;

  uint8_t data = 0;
  if (imu.read_device_memory(IMU_DEV_ADDR, 0u, &data, 1u))
  {
    logMessage(LOG_INFO, "IMU chip ID = %d.\r\n", data);
  }

  logMessage(LOG_DEBUG, "ImuInterfaceThread started.\r\n");

  for (;;)
  {
    vTaskDelayUntil(&last_wake_time, cycle_time_ticks);
  }
}
