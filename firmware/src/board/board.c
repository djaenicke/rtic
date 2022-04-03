#include <stdint.h>
#include <string.h>
#include "board.h"

UART_HandleTypeDef debug_uart;

static void configSystemClock(void);
static void gpioInit(void);
static void serialInit(void);
static void errorHandler(void);

void initBoard(void)
{
  // Reset of all peripherals, Initializes the Flash interface and the Systick.
  HAL_Init();

  configSystemClock();

  gpioInit();
  serialInit();
}

void toggleHeartbeat(void)
{
  HAL_GPIO_TogglePin(HEARTBEAT_LED_GPIO_PORT, HEARTBEAT_LED_PIN);
    HAL_Delay(500);
}

void delayMs(const uint32_t delay_ms)
{
  HAL_Delay(delay_ms);
}

void serialTxString(const char* const string)
{
  HAL_UART_Transmit(&debug_uart, (uint8_t*)string, (uint16_t)strlen(string), HAL_MAX_DELAY);
}

static void configSystemClock(void)
{
  RCC_OscInitTypeDef rcc_osc_init = {0};
  RCC_ClkInitTypeDef rcc_clk_init = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  rcc_osc_init.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSE;
  rcc_osc_init.HSEState = RCC_HSE_ON;
  rcc_osc_init.HSIState = RCC_HSI_ON;
  rcc_osc_init.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  rcc_osc_init.PLL.PLLState = RCC_PLL_ON;
  rcc_osc_init.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  rcc_osc_init.PLL.PLLM = 10;
  rcc_osc_init.PLL.PLLN = 72;
  rcc_osc_init.PLL.PLLP = RCC_PLLP_DIV2;
  rcc_osc_init.PLL.PLLQ = 3;
  rcc_osc_init.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&rcc_osc_init) != HAL_OK)
  {
    errorHandler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  rcc_clk_init.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  rcc_clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  rcc_clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
  rcc_clk_init.APB1CLKDivider = RCC_HCLK_DIV1;
  rcc_clk_init.APB2CLKDivider = RCC_HCLK_DIV1;
  if (HAL_RCC_ClockConfig(&rcc_clk_init, FLASH_LATENCY_0) != HAL_OK)
  {
    errorHandler();
  }
}

static void gpioInit(void)
{
  GPIO_InitTypeDef gpio_init = { 0 };

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, HEARTBEAT_LED_PIN|D1_AIN1_PIN|D1_BIN1_PIN|D1_BIN2_PIN
                          |D2_BIN2_PIN|D2_BIN1_PIN, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, D1_AIN2_PIN|D2_STBY_PIN, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, D1_STBY_PIN|D2_AIN1_PIN|D2_AIN2_PIN|CAN2_RS_PIN
                          |RF_EN_PIN, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(RF_CS_GPIO_PORT, RF_CS_PIN, GPIO_PIN_RESET);

  /*Configure GPIO pin : IMU_INT_PIN */
  gpio_init.Pin = IMU_INT_PIN;
  gpio_init.Mode = GPIO_MODE_INPUT;
  gpio_init.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(IMU_INT_GPIO_PORT, &gpio_init);

  /*Configure GPIO pins : HEARTBEAT_LED_PIN D1_AIN1_PIN D1_BIN1_PIN D1_BIN2_PIN
                           D2_BIN2_PIN D2_BIN1_PIN */
  gpio_init.Pin = HEARTBEAT_LED_PIN|D1_AIN1_PIN|D1_BIN1_PIN|D1_BIN2_PIN
                          |D2_BIN2_PIN|D2_BIN1_PIN;
  gpio_init.Mode = GPIO_MODE_OUTPUT_PP;
  gpio_init.Pull = GPIO_NOPULL;
  gpio_init.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &gpio_init);

  /*Configure GPIO pins : D1_AIN2_PIN D2_STBY_PIN */
  gpio_init.Pin = D1_AIN2_PIN|D2_STBY_PIN;
  gpio_init.Mode = GPIO_MODE_OUTPUT_PP;
  gpio_init.Pull = GPIO_NOPULL;
  gpio_init.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &gpio_init);

  /*Configure GPIO pins : D1_STBY_PIN D2_AIN1_PIN D2_AIN2_PIN CAN2_RS_PIN
                           RF_EN_PIN */
  gpio_init.Pin = D1_STBY_PIN|D2_AIN1_PIN|D2_AIN2_PIN|CAN2_RS_PIN
                          |RF_EN_PIN;
  gpio_init.Mode = GPIO_MODE_OUTPUT_PP;
  gpio_init.Pull = GPIO_NOPULL;
  gpio_init.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &gpio_init);

  /*Configure GPIO pin : RF_CS_PIN */
  gpio_init.Pin = RF_CS_PIN;
  gpio_init.Mode = GPIO_MODE_OUTPUT_PP;
  gpio_init.Pull = GPIO_NOPULL;
  gpio_init.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(RF_CS_GPIO_PORT, &gpio_init);

  /*Configure GPIO pin : RF_IRQ_PIN */
  gpio_init.Pin = RF_IRQ_PIN;
  gpio_init.Mode = GPIO_MODE_INPUT;
  gpio_init.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(RF_IRQ_GPIO_PORT, &gpio_init);
}

static void serialInit(void)
{
  GPIO_InitTypeDef gpio_init = { 0 };

  __HAL_RCC_USART1_CLK_ENABLE();

  __HAL_RCC_GPIOB_CLK_ENABLE();

  gpio_init.Pin = DEBUG_TX_PIN|DEBUG_RX_PIN;
  gpio_init.Mode = GPIO_MODE_AF_PP;
  gpio_init.Pull = GPIO_NOPULL;
  gpio_init.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  gpio_init.Alternate = GPIO_AF7_USART1;
  HAL_GPIO_Init(GPIOB, &gpio_init);

  debug_uart.Instance = USART1;
  debug_uart.Init.BaudRate = 115200;
  debug_uart.Init.WordLength = UART_WORDLENGTH_8B;
  debug_uart.Init.StopBits = UART_STOPBITS_1;
  debug_uart.Init.Parity = UART_PARITY_NONE;
  debug_uart.Init.Mode = UART_MODE_TX;
  debug_uart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  debug_uart.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&debug_uart) != HAL_OK)
  {
    errorHandler();
  }
}

static void errorHandler(void)
{
  __disable_irq();
  while (1)
  {
  }
}

void SysTick_Handler(void)
{
  HAL_IncTick();
}

void NMI_Handler(void)
{
  while (1)
  {
  }
}

void HardFault_Handler(void)
{
  while (1)
  {
  }
}

void MemManage_Handler(void)
{
  while (1)
  {
  }
}

void BusFault_Handler(void)
{
  while (1)
  {
  }
}

void UsageFault_Handler(void)
{
  while (1)
  {
  }
}

void SVC_Handler(void)
{
}

void DebugMon_Handler(void)
{
}

void PendSV_Handler(void)
{
}
