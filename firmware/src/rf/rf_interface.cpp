#include "rf_interface.h"

#include "cmsis_os.h"
#include "digital_out.h"
#include "io.h"
#include "nrf24.h"
#include "serial_logger.h"
#include "spi.h"
#include "threads.h"

#define ADDRESS_LEN 5

typedef struct __attribute__((packed))
{
  uint8_t id;
  uint8_t version;
  uint8_t tx_cnt;
} MessageHeader;

typedef struct __attribute__((packed))
{
  uint16_t x_counts;
  uint16_t y_counts;
  uint8_t z_press_cnt;
} JoystickInput;

typedef struct __attribute__((packed))
{
  MessageHeader header;
  JoystickInput l_joystick;
  JoystickInput r_joystick;
} UserInputMessage;

static const uint8_t CONTROLLER_TX_ADDRESS[ADDRESS_LEN + 1] = "ROBOT";

hal::SpiMaster rf_spi(3u, RF_MOSI, RF_MISO, RF_SCK, RF_CS);
hal::DigitalOut rf_en(RF_EN);

void threadRfInterface(const void* argument)
{
  static TickType_t last_wake_time = xTaskGetTickCount();
  const TickType_t cycle_time_ticks = RF_INTERFACE_PERIOD_MS * portTICK_PERIOD_MS;

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

  nRF24_EnableAA(nRF24_PIPE0);
  nRF24_SetRFChannel(0x4C);
  nRF24_SetDataRate(nRF24_DR_1Mbps);
  nRF24_SetCRCScheme(nRF24_CRC_2byte);
  nRF24_SetAddrWidth(ADDRESS_LEN);
  nRF24_SetAddr(nRF24_PIPE0, CONTROLLER_TX_ADDRESS);
  nRF24_SetRXPipe(nRF24_PIPE0, nRF24_AA_ON, sizeof(UserInputMessage));
  nRF24_SetTXPower(nRF24_TXPWR_0dBm);
  nRF24_SetOperationalMode(nRF24_MODE_RX);
  nRF24_SetPowerMode(nRF24_PWR_UP);

  logMessage(LOG_DEBUG, "RfInterfaceThread started.\r\n");

  for (;;)
  {
    UserInputMessage ui;
    uint8_t payload[sizeof(UserInputMessage)];
    uint8_t len = 0;
    if (nRF24_GetStatus_RXFIFO() != nRF24_STATUS_RXFIFO_EMPTY)
    {
      const nRF24_RXResult pipe = nRF24_ReadPayload(payload, &len);
      if ((nRF24_RX_PIPE0 == pipe) && (sizeof(UserInputMessage) == len))
      {
        (void)memcpy(&ui, payload, len);
        logMessage(LOG_DEBUG, "L joystick = (%d, %d, %d).\r\n", ui.l_joystick.x_counts,
                   ui.l_joystick.y_counts, ui.l_joystick.z_press_cnt);
        logMessage(LOG_DEBUG, "R joystick = (%d, %d, %d).\r\n", ui.r_joystick.x_counts,
                   ui.r_joystick.y_counts, ui.r_joystick.z_press_cnt);
      }
      nRF24_ClearIRQFlags();
    }
    vTaskDelayUntil(&last_wake_time, cycle_time_ticks);
  }
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
