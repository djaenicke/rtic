#include "imu_interface.h"

#include "bmi160.h"
#include "cmsis_os.h"
#include "i2c.h"
#include "io.h"
#include "serial_logger.h"
#include "threads.h"

#define LOG_SENSOR_DATA 0

static bmi160::IMU imu(hal::I2CMaster(1u, IMU_SCL, IMU_SDA, 100000u), IMU_DEV_ADDR);

void threadImuInterface(const void* argument)
{
  static TickType_t last_wake_time = xTaskGetTickCount();
  const TickType_t cycle_time_ticks = IMU_INTERFACE_PERIOD_MS * portTICK_PERIOD_MS;
  static bmi160::SensorData sensor_data;

  if (imu.init(bmi160::RANGE_2G, bmi160::RANGE_500_DPS))
  {
    logMessage(LOG_DEBUG, "IMU initialized.\r\n");
  }
  else
  {
    logMessage(LOG_ERROR, "IMU initialization failed.\r\n");
  }

  if (imu.perform_foc())
  {
    logMessage(LOG_DEBUG, "IMU offset compensation completed.\r\n");
  }
  else
  {
    logMessage(LOG_ERROR, "IMU offset compensation failed.\r\n");
  }

  logMessage(LOG_DEBUG, "ImuInterfaceThread started.\r\n");

  for (;;)
  {
    if (imu.get_sensor_data(sensor_data))
    {
#if LOG_SENSOR_DATA
      logMessage(LOG_DEBUG, "lin_acc_mps2 (x, y, z) = (%f, %f, %f)\r\n", sensor_data.lin_acc_mps2.x,
                 sensor_data.lin_acc_mps2.y, sensor_data.lin_acc_mps2.z);
      logMessage(LOG_DEBUG, "ang_vel_rps (x, y, z) = (%f, %f, %f)\r\n", sensor_data.ang_vel_rps.x,
                 sensor_data.ang_vel_rps.y, sensor_data.ang_vel_rps.z);
#endif
    }
    vTaskDelayUntil(&last_wake_time, cycle_time_ticks);
  }
}
