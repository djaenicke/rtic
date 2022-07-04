#include "rf_interface.h"

#include "cmsis_os.h"
#include "digital_out.h"
#include "io.h"
#include "message_buffer.h"
#include "nrf24.h"
#include "serial_logger.h"
#include "spi.h"
#include "threads.h"

static const uint8_t NRF24_CHANNEL = 0x4Cu;
static const uint8_t UI_MSG_RX_PIPE = nRF24_PIPE0;
static const uint8_t ADDRESS_LEN = 5u;
static const uint8_t CONTROLLER_TX_ADDRESS[ADDRESS_LEN + 1] = "ROBOT";

static MessageBufferHandle_t ui_message_buffer_handle;
static StaticMessageBuffer_t ui_message_buffer;
static uint8_t ui_message_buffer_storage[sizeof(UserInputMessage) + sizeof(size_t) + 1u];

hal::SpiMaster rf_spi(3u, RF_MOSI, RF_MISO, RF_SCK, RF_CS);
hal::DigitalOut rf_en(RF_EN);

static void initNrf24(void);
static void userInputMessageBufferWrite(const UserInputMessage* const ui_msg);

void threadRfInterface(const void* argument)
{
  const TickType_t cycle_time_ticks = RF_INTERFACE_PERIOD_MS / portTICK_PERIOD_MS;

  initNrf24();

  vTaskDelay(RF_INTERFACE_POST_INIT_DELAY_MS / portTICK_PERIOD_MS);
  logMessage(LOG_DEBUG, "RfInterfaceThread started.\r\n");

  static TickType_t last_wake_time = xTaskGetTickCount();

  for (;;)
  {
    uint8_t payload[sizeof(UserInputMessage)];
    uint8_t len = 0;
    if (nRF24_GetStatus_RXFIFO() != nRF24_STATUS_RXFIFO_EMPTY)
    {
      const nRF24_RXResult pipe = nRF24_ReadPayload(payload, &len);
      if ((nRF24_RX_PIPE0 == pipe) && (sizeof(UserInputMessage) == len))
      {
        static uint8_t ui_msg_cnt = 0;
        UserInputMessage ui_msg;
        (void)memcpy(&ui_msg, payload, len);

        // Check the transmit counter in the message header
        if (ui_msg.header.tx_cnt == ui_msg_cnt)
        {
          ui_msg_cnt = (ui_msg_cnt == UINT8_MAX) ? 0u : (ui_msg_cnt + 1u);
        }
        else
        {
          logMessage(LOG_WARN, "Dropped UI message detected.\r\n");
          ui_msg_cnt = ui_msg.header.tx_cnt + 1u;
        }

        userInputMessageBufferWrite(&ui_msg);
      }
      nRF24_ClearIRQFlags();
    }
    vTaskDelayUntil(&last_wake_time, cycle_time_ticks);
  }
}

static void initNrf24(void)
{
  rf_en = 1u;
  nRF24_Init();

  if (0u != nRF24_Check())
  {
    logMessage(LOG_DEBUG, "NRF24 initialized.\r\n");
  }
  else
  {
    logMessage(LOG_ERROR, "NRF24 initialization failed.\r\n");
  }

  nRF24_EnableAA(UI_MSG_RX_PIPE);
  nRF24_SetRFChannel(NRF24_CHANNEL);
  nRF24_SetDataRate(nRF24_DR_1Mbps);
  nRF24_SetCRCScheme(nRF24_CRC_2byte);
  nRF24_SetAddrWidth(ADDRESS_LEN);
  nRF24_SetAddr(UI_MSG_RX_PIPE, CONTROLLER_TX_ADDRESS);
  nRF24_SetRXPipe(UI_MSG_RX_PIPE, nRF24_AA_ON, sizeof(UserInputMessage));
  nRF24_SetTXPower(nRF24_TXPWR_0dBm);
  nRF24_SetOperationalMode(nRF24_MODE_RX);
  nRF24_SetPowerMode(nRF24_PWR_UP);
}

static void userInputMessageBufferWrite(const UserInputMessage* const ui_msg)
{
  const size_t bytes_written =
      xMessageBufferSend(ui_message_buffer_handle, (void*)ui_msg, sizeof(UserInputMessage), 0);
  if (bytes_written != sizeof(UserInputMessage))
  {
    logMessage(LOG_ERROR, "Failed to write UI message into message buffer.\r\n");
  }
}

void initUiMessageBuffer(void)
{
  ui_message_buffer_handle = xMessageBufferCreateStatic(
      sizeof(ui_message_buffer_storage), ui_message_buffer_storage, &ui_message_buffer);

  if (NULL != ui_message_buffer_handle)
  {
    logMessage(LOG_DEBUG, "Successfully create UI message buffer.\r\n");
  }
  else
  {
    logMessage(LOG_ERROR, "Failed to create UI message buffer..\r\n");
  }
}

bool userInputMessageBufferRead(UserInputMessage* const ui_msg)
{
  const size_t bytes_read =
      xMessageBufferReceive(ui_message_buffer_handle, (void*)ui_msg, sizeof(UserInputMessage), 0);
  if (sizeof(UserInputMessage) == bytes_read)
  {
    return true;
  }
  return false;
}

void enableNrf24ChipSelect(void)
{
  rf_spi.csEnable();
}

void disableNrf24ChipSelect(void)
{
  rf_spi.csDisable();
}

uint8_t readWriteNrf24(uint8_t data)
{
  uint8_t data_in = 0;
  if (0 != rf_spi.transfer(&data, &data_in, 1u))
  {
    logMessage(LOG_ERROR, "NRF24 SPI transfer failed.\r\n");
  }
  return data_in;
}
