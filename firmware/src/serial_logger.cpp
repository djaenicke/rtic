#include "serial_logger.h"

#include <cstring>

#include "cmsis_os.h"
#include "io.h"
#include "threads.h"
#include "uart.h"

static hal::UART serial(1u, DEBUG_TX, DEBUG_RX, 115200u);

static const uint8_t QUEUE_LEN = 20u;
static QueueHandle_t msg_queue_handle;
static StaticQueue_t msg_queue;
static uint8_t queue_storage[MAX_MSG_LEN * QUEUE_LEN];

static LogLevel logging_level = LOG_DEBUG;

void initSerialLogger(const LogLevel desired_logging_level)
{
  char buf[64];

  msg_queue_handle = xQueueCreateStatic(QUEUE_LEN, MAX_MSG_LEN, queue_storage, &msg_queue);

  if (NULL != msg_queue_handle)
  {
    snprintf(buf, 64, "\"initSerialLogger\" successfully create queue.\r\n");
  }
  else
  {
    snprintf(buf, 64, "\"initSerialLogger\" failed to create queue.\r\n");
  }

  logging_level = desired_logging_level;
  serial.transmit_blocking((uint8_t*)buf, strnlen(buf, MAX_MSG_LEN));
}

void sendMessage(const LogLevel msg_log_level, const char* const msg)
{
  if ((msg_log_level < logging_level) || (NULL == msg))
  {
    return;
  }
  (void)xQueueSendToBack(msg_queue_handle, (void*)msg, 10);
}

void threadSerialLogger(const void* argument)
{
  static uint8_t tx_msg[MAX_MSG_LEN];
  static TickType_t last_wake_time = xTaskGetTickCount();
  const TickType_t cycle_time_ticks = SERIAL_LOGGER_PERIOD_MS * portTICK_PERIOD_MS;

  for (;;)
  {
    vTaskDelayUntil(&last_wake_time, cycle_time_ticks);
    if (pdTRUE == xQueueReceive(msg_queue_handle, (void*)tx_msg, 0))
    {
      serial.transmit_blocking(tx_msg, strnlen((char*)tx_msg, MAX_MSG_LEN));
    }
  }
}
