/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdarg.h"
#include "RM3100QUAD.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
//#define CDC_USB_DEBUG
#define UART_DEBUG
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define sector_size 32
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
void myprintf(const char *fmt, ...);
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
RTC_HandleTypeDef hrtc;

SPI_HandleTypeDef hspi1;
SPI_HandleTypeDef hspi2;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

PCD_HandleTypeDef hpcd_USB_FS;

/* USER CODE BEGIN PV */
uint16_t address = 0x00;
uint8_t status_reg=0;
uint8_t READ_FLAG=0;
int tx[70];
uint16_t time_counter=0;

uint8_t DEBUG_DATA_RX_FLAG = 0;
// variables used by the filesystem
typedef struct{
	uint32_t secCount;
	uint32_t bootCount;
}app_count_t;
uint8_t data[20];


app_count_t Counter = {0};
//Command for run in differents minutes
uint8_t COMMAND1[] = {0x53,0x0e,0x0d,0x0e,0x01,0x7e}; //Command for 1 minutes
uint8_t COMMAND2[] = {0x53,0x0e,0x0d,0x0e,0x02,0x7e}; //Command for 2 minutes
uint8_t COMMAND3[] = {0x53,0x0e,0x0d,0x0e,0x03,0x7e}; //Command for 3 minutes
uint8_t COMMAND4[] = {0x53,0x0e,0x0d,0x0e,0x04,0x7e}; //command for 4 minutes
uint8_t COMMAND5[] = {0x53,0x0e,0x0d,0x0e,0x05,0x7e}; //command for 5 minutes
uint8_t OBC_HANDSHAKE_SUCESS,OBC_HANDSHAKE_RX[7]={'\0'},OBC_HANDSHAKE_TX[7]={'\0'};//,OBC_HANDSHAKE_RX[7]={'\0'};
uint8_t Read_time = {0};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_SPI2_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_RTC_Init(void);
static void MX_USB_PCD_Init(void);
/* USER CODE BEGIN PFP */
//Handshake between OBC(Onboard computer) and EPDM board and compare command for run in different minutes
uint8_t OBC_COMMAND(){
	memset(OBC_HANDSHAKE_RX, '\0', sizeof(OBC_HANDSHAKE_RX));
	memset(OBC_HANDSHAKE_TX, '\0', sizeof(OBC_HANDSHAKE_TX));
	OBC_HANDSHAKE_SUCESS = 0;
	do{
	if(HAL_UART_Receive(&huart2, OBC_HANDSHAKE_RX, sizeof(OBC_HANDSHAKE_RX),1000)==HAL_OK){
		if(HAL_UART_Transmit(&huart2, OBC_HANDSHAKE_RX, sizeof(OBC_HANDSHAKE_RX), 3000)==HAL_OK){
			if(OBC_HANDSHAKE_RX[0] == COMMAND1[0] && OBC_HANDSHAKE_RX[1] == COMMAND1[1] && OBC_HANDSHAKE_RX[2] == COMMAND1[2] && OBC_HANDSHAKE_RX[3] == COMMAND1[3] && OBC_HANDSHAKE_RX[4] == COMMAND1[4] && OBC_HANDSHAKE_RX[5] == COMMAND1[5] && OBC_HANDSHAKE_RX[6] == COMMAND1[6] )
				{
				OBC_HANDSHAKE_SUCESS = 1;
				Read_time = 1; //Sets board run for 1 minutes
				HAL_UART_Transmit(&huart1, "COMMAND SUCCESSFULLY\n", "COMMAND SUCCESSFULLY", 1000);

				}
			else if(OBC_HANDSHAKE_RX[0] == COMMAND2[0] && OBC_HANDSHAKE_RX[1] == COMMAND2[1] && OBC_HANDSHAKE_RX[2] == COMMAND2[2] && OBC_HANDSHAKE_RX[3] == COMMAND2[3] && OBC_HANDSHAKE_RX[4] == COMMAND2[4] && OBC_HANDSHAKE_RX[5] == COMMAND2[5] && OBC_HANDSHAKE_RX[6] == COMMAND2[6] )
				{
				OBC_HANDSHAKE_SUCESS = 1;
				Read_time = 2;  //Sets board run for 2 minutes
				HAL_UART_Transmit(&huart1, "COMMAND SUCCESSFULLY\n", "COMMAND SUCCESSFULLY", 1000);

				}
			else if(OBC_HANDSHAKE_RX[0] == COMMAND3[0] && OBC_HANDSHAKE_RX[1] == COMMAND3[1] && OBC_HANDSHAKE_RX[2] == COMMAND3[2] && OBC_HANDSHAKE_RX[3] == COMMAND3[3] && OBC_HANDSHAKE_RX[4] == COMMAND3[4] && OBC_HANDSHAKE_RX[5] == COMMAND3[5] && OBC_HANDSHAKE_RX[6] == COMMAND3[6] )
				{
				OBC_HANDSHAKE_SUCESS = 1;
				Read_time = 3; //Sets board run for 3 minutes
				HAL_UART_Transmit(&huart1, "COMMAND SUCCESSFULLY\n", "COMMAND SUCCESSFULLY", 1000);

				}
			else if(OBC_HANDSHAKE_RX[0] == COMMAND4[0] && OBC_HANDSHAKE_RX[1] == COMMAND4[1] && OBC_HANDSHAKE_RX[2] == COMMAND4[2] && OBC_HANDSHAKE_RX[3] == COMMAND4[3] && OBC_HANDSHAKE_RX[4] == COMMAND4[4] && OBC_HANDSHAKE_RX[5] == COMMAND4[5] && OBC_HANDSHAKE_RX[6] == COMMAND4[6] )
				{
				OBC_HANDSHAKE_SUCESS = 1;
				Read_time = 4;  //Sets board run for 4 minutes
				HAL_UART_Transmit(&huart1, "COMMAND SUCCESSFULLY\n", "COMMAND SUCCESSFULLY", 1000);

				}
			else if(OBC_HANDSHAKE_RX[0] == COMMAND5[0] && OBC_HANDSHAKE_RX[1] == COMMAND5[1] && OBC_HANDSHAKE_RX[2] == COMMAND5[2] && OBC_HANDSHAKE_RX[3] == COMMAND5[3] && OBC_HANDSHAKE_RX[4] == COMMAND5[4] && OBC_HANDSHAKE_RX[5] == COMMAND5[5] && OBC_HANDSHAKE_RX[6] == COMMAND5[6] )
				{
				OBC_HANDSHAKE_SUCESS = 1;
				Read_time = 5;  //Sets board run for 5 minutes
				HAL_UART_Transmit(&huart1, "COMMAND SUCCESSFULLY\n", "COMMAND SUCCESSFULLY", 1000);

				}
			else{
				  Read_time = 5;
				  OBC_HANDSHAKE_SUCESS = 1;

//				  HAL_UART_Transmit(&huart1, "xxxxxxCOMMAND ERRORxxxxxx", sizeof("xxxxxxCOMMAND ERRORxxxxxx"), 1000);
			}
		}
	}
	}while(!OBC_HANDSHAKE_SUCESS);
	return OBC_HANDSHAKE_SUCESS;
}
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

// Function to read data from a file in LittleFS

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_SPI2_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_RTC_Init();
  MX_USB_PCD_Init();
  /* USER CODE BEGIN 2 */
  HAL_UART_Transmit(&huart1, "*****************EPDM starting**************\n",sizeof("*****************EPDM starting**************\n"),1000);

//  OBC_HANDSHAKE();
  OBC_COMMAND();
  HAL_GPIO_WritePin(GPIOB, MSN_EN1_Pin, SET); // Set PB9 high
  HAL_GPIO_WritePin(GPIOB, MSN_EN2_Pin, SET); // Set PB8 high
  HAL_GPIO_WritePin(GPIOB, MSN_EN3_Pin, SET); // Set PA15 high
  HAL_GPIO_WritePin(GPIOB, MSN_EN4_Pin, SET); // Set PA8 high
  uint8_t data[20];

//Read ID of Flash memory
  while(1){
	  Read_ID(&hspi2, GPIOB, GPIO_PIN_12, data);
	  if(data[0] == 32){
		  break;
	  }
	  HAL_Delay(10000);
  }
 //Sets parameters of RM3100 magnetometer differents modes
    for(int i=0;i<4;i++){
    	SET_COUNT(i);
    	Continuous_Mode(i);
    	TMRC_Mode(i);
    }
    uint32_t count=0;
    __init_storage();
    uint8_t last[]={'E','P','D','M'}; //Stores before reading data and after reading data
    uint8_t eom[]={0xff,0xd9,'\0'}; //OBC stops mission if received

    HAL_UART_Transmit(&huart2, last, sizeof(last), 1000);//Before reading
    if (Read_time >= 1 && Read_time <= 5) {
    	      uint32_t startTime = HAL_GetTick();  // Record the start time
    	      uint32_t maxDuration = Read_time * 60 * 1000;  // Maximum duration in milliseconds

    	      while (HAL_GetTick() - startTime < maxDuration) {  // Run loop according to the value of Read_time
    	          for (int i = 0; i < 4; i++) {
    	              Mea_Result(i);
    	              Comb_measurement(i);
    	              count += 6;
    	          }
    	          HAL_Delay(100);  // Small delay between measurements
    	      }

    	      // Transmit the total count of bytes sent
    	      myDebug("%d",count);
    	      count = 0;  // Reset count after transmitting
    	      HAL_UART_Transmit(&huart2, last, sizeof(last), 1000); //after reading
    	      HAL_UART_Transmit(&huart2, eom, sizeof(eom), 1000); // OBC stops or turned off EPDM mission
    	      HAL_Delay(90000);  // Wait for 1 minute after sending EOM

    	  } else {
    	      // If Read_time is not in the valid range, send an error message
    	      HAL_UART_Transmit(&huart1, (uint8_t*)"xxxxxx**********ReadTimeInvalid**********xxxxxx", sizeof("xxxxxx**********ReadTimeInvalid**********xxxxxx"), 1000);
    	  }

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */


  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC|RCC_PERIPHCLK_USB;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */
  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.AsynchPrediv = RTC_AUTO_1_SECOND;
  hrtc.Init.OutPut = RTC_OUTPUTSOURCE_ALARM;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief USB Initialization Function
  * @param None
  * @retval None
  */
static void MX_USB_PCD_Init(void)
{

  /* USER CODE BEGIN USB_Init 0 */

  /* USER CODE END USB_Init 0 */

  /* USER CODE BEGIN USB_Init 1 */

  /* USER CODE END USB_Init 1 */
  hpcd_USB_FS.Instance = USB;
  hpcd_USB_FS.Init.dev_endpoints = 8;
  hpcd_USB_FS.Init.speed = PCD_SPEED_FULL;
  hpcd_USB_FS.Init.low_power_enable = DISABLE;
  hpcd_USB_FS.Init.lpm_enable = DISABLE;
  hpcd_USB_FS.Init.battery_charging_enable = DISABLE;
  if (HAL_PCD_Init(&hpcd_USB_FS) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USB_Init 2 */

  /* USER CODE END USB_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, SS1_Pin|SS2_Pin|SS3_Pin|SS4_Pin
                          |CS_FM_Pin|DRDY4_Pin|DRDY3_Pin|DRDY2_Pin
                          |DRDY1_Pin|MSN_EN3_Pin|MSN_EN1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, MSN_EN4_Pin|MSN_EN2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : SS1_Pin SS2_Pin SS3_Pin SS4_Pin
                           CS_FM_Pin DRDY4_Pin DRDY3_Pin DRDY2_Pin
                           DRDY1_Pin MSN_EN3_Pin MSN_EN1_Pin */
  GPIO_InitStruct.Pin = SS1_Pin|SS2_Pin|SS3_Pin|SS4_Pin
                          |CS_FM_Pin|DRDY4_Pin|DRDY3_Pin|DRDY2_Pin
                          |DRDY1_Pin|MSN_EN3_Pin|MSN_EN1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : MSN_EN4_Pin MSN_EN2_Pin */
  GPIO_InitStruct.Pin = MSN_EN4_Pin|MSN_EN2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
//void myprintf(const char *fmt, ...) {
//    va_list args;
//    va_start(args, fmt);
//    char buffer[100];
//    vsnprintf(buffer, sizeof(buffer), fmt, args);
//    HAL_UART_Transmit(&DEBUG_UART, (uint8_t*) buffer, strlen(buffer), HAL_MAX_DELAY);
//    va_end(args);
//}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
