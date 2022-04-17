#include "board.h"

#include <stdint.h>
#include <string.h>

#define DEBUG_TX_PIN GPIO_PIN_6
#define DEBUG_TX_GPIO_PORT GPIOB

#define DEBUG_RX_PIN GPIO_PIN_7
#define DEBUG_RX_GPIO_PORT GPIOB

UART_HandleTypeDef debug_uart;

static void configSystemClock(void);
static void serialInit(void);
static void errorHandler(void);

void initBoard(void)
{
  // Reset of all peripherals, Initializes the Flash interface and the Systick.
  HAL_Init();

  configSystemClock();
  serialInit();
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
  RCC_OscInitTypeDef rcc_osc_init = { 0 };
  RCC_ClkInitTypeDef rcc_clk_init = { 0 };

  /** Configure the main internal regulator output voltage
   */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
  rcc_osc_init.OscillatorType = RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_HSE;
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
  rcc_clk_init.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  rcc_clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  rcc_clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
  rcc_clk_init.APB1CLKDivider = RCC_HCLK_DIV1;
  rcc_clk_init.APB2CLKDivider = RCC_HCLK_DIV1;
  if (HAL_RCC_ClockConfig(&rcc_clk_init, FLASH_LATENCY_0) != HAL_OK)
  {
    errorHandler();
  }
}

static void serialInit(void)
{
  GPIO_InitTypeDef gpio_init = { 0 };

  __HAL_RCC_USART1_CLK_ENABLE();

  __HAL_RCC_GPIOB_CLK_ENABLE();

  gpio_init.Pin = DEBUG_TX_PIN | DEBUG_RX_PIN;
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
