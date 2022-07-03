#include <stdio.h>

#include "board.h"
#include "cmsis_os.h"
#include "digital_out.h"
#include "imu_interface.h"
#include "io.h"
#include "motor_controls.h"
#include "rf_interface.h"
#include "serial_logger.h"
#include "threads.h"

static void threadHeartbeat(const void* argument);

// clang-format off
static osThreadId heartbeat_thread_handle;
static char heartbeat_thread_name[] = "HeartbeatThread";
const osThreadDef_t heartbeat_thread_def = { 
  heartbeat_thread_name,
  threadHeartbeat,
  HEARTBEAT_PRIORITY,
  0u,
  HEARTBEAT_STACK_SIZE_WORDS,
  NULL,
  NULL
};

static osThreadId serial_logger_thread_handle;
static char serial_logger_thread_name[] = "SerialLoggerThread";
const osThreadDef_t serial_logger_thread_def = { 
  serial_logger_thread_name,
  threadSerialLogger,
  SERIAL_LOGGER_PRIORITY,
  0u,
  SERIAL_LOGGER_STACK_SIZE_WORDS,
  NULL,
  NULL
};

static osThreadId rf_interface_thread_handle;
static char rf_interface_thread_name[] = "RfInterfaceThread";
const osThreadDef_t rf_interface_thread_def = { 
  rf_interface_thread_name,
  threadRfInterface,
  RF_INTERFACE_PRIORITY,
  0u,
  RF_INTERFACE_STACK_SIZE_WORDS,
  NULL,
  NULL
};

static osThreadId imu_interface_thread_handle;
static char imu_interface_thread_name[] = "ImuInterfaceThread";
const osThreadDef_t imu_interface_thread_def = { 
  imu_interface_thread_name,
  threadImuInterface,
  IMU_INTERFACE_PRIORITY,
  0u,
  IMU_INTERFACE_STACK_SIZE_WORDS,
  NULL,
  NULL
};

static osThreadId motor_controls_thread_handle;
static char motor_controls_thread_name[] = "MotorControlsThread";
const osThreadDef_t motor_controls_thread_def = { 
  motor_controls_thread_name,
  threadMotorControls,
  MOTOR_CONTROLS_PRIORITY,
  0u,
  MOTOR_CONTROLS_STACK_SIZE_WORDS,
  NULL,
  NULL
};
// clang-format on

int main(void)
{
  initSerialLogger(LOG_DEBUG);

  // Create the threads
  heartbeat_thread_handle = osThreadCreate(&heartbeat_thread_def, NULL);
  serial_logger_thread_handle = osThreadCreate(&serial_logger_thread_def, NULL);
  rf_interface_thread_handle = osThreadCreate(&rf_interface_thread_def, NULL);
  imu_interface_thread_handle = osThreadCreate(&imu_interface_thread_def, NULL);
  motor_controls_thread_handle = osThreadCreate(&motor_controls_thread_def, NULL);

  // Start the scheduler
  osKernelStart();

  return 0;
}

static void threadHeartbeat(const void* argument)
{
  static hal::DigitalOut heartbeat(HEARTBEAT_LED);
  static TickType_t last_wake_time = xTaskGetTickCount();
  const TickType_t cycle_time_ticks = HEARTBEAT_PERIOD_MS * portTICK_PERIOD_MS;

  logMessage(LOG_DEBUG, "HeartbeatThread started.\r\n");

  for (;;)
  {
    vTaskDelayUntil(&last_wake_time, cycle_time_ticks);
    heartbeat.toggle();
  }
}
