#include <stdio.h>

#include "board.h"
#include "cmsis_os.h"
#include "digital_out.h"
#include "io.h"
#include "motor_controls.h"
#include "serial_logger.h"

static void threadHeartbeat(const void* argument);

static osThreadId heartbeat_thread_handle;
static char heartbeat_thread_name[] = "HeartbeatThread";
const osThreadDef_t heartbeat_thread_def = {
  heartbeat_thread_name, threadHeartbeat, osPriorityLow, 0u, 64u, NULL, NULL
};

static osThreadId motor_controls_thread_handle;
static char motor_controls_thread_name[] = "MotorControlsThread";
const osThreadDef_t motor_controls_thread_def = {
  motor_controls_thread_name, threadMotorControls, osPriorityRealtime, 0u, 64u, NULL, NULL
};

static osThreadId serial_logger_thread_handle;
static char serial_logger_thread_name[] = "SerialLoggerThread";
const osThreadDef_t serial_logger_thread_def = {
  serial_logger_thread_name, threadSerialLogger, osPriorityNormal, 0u, 128u, NULL, NULL
};

int main(void)
{
  // Create the threads
  heartbeat_thread_handle = osThreadCreate(&heartbeat_thread_def, NULL);
  motor_controls_thread_handle = osThreadCreate(&motor_controls_thread_def, NULL);
  serial_logger_thread_handle = osThreadCreate(&serial_logger_thread_def, NULL);

  // Start the scheduler
  osKernelStart();

  return 0;
}

static void threadHeartbeat(const void* argument)
{
  static hal::DigitalOut heartbeat(HEARTBEAT_LED);

  for (;;)
  {
    heartbeat.toggle();
    osDelay(500u);
  }
}
