/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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
#include "adc.h"
#include "dfsdm.h"
#include "i2c.h"
#include "octospi.h"
#include "spi.h"
#include "usart.h"
#include "usb_otg.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include "../../Drivers/ES_WIFI/es_wifi_io.h"
#include "../../Drivers/ES_WIFI/wifi.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define SOCKET_NUM (0)
#define TIMEOUT (20)
#define BUF_SZ (1024)
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int __io_putchar(int ch)
{
	if (ch == '\n')
	{
		uint8_t send_data[2] = {'\r','\n'};
		HAL_UART_Transmit(&huart1, send_data, 2, 1);
	}
	else
	{
		while (HAL_OK != HAL_UART_Transmit(&huart1, (uint8_t *) & ch, 1, 30000))
		{

		}
	}
	return ch;
}

//int __io_getchar(void)
//{
//  uint8_t ch = 0;
//  while (HAL_OK != HAL_UART_Receive(&huart1, (uint8_t *)&ch, 1, 30000))
//  {
//  }
//  return ch;
//}

int _read(int32_t file, uint8_t *result, int32_t len)
{
    HAL_StatusTypeDef status;
    int retcode = 0;

    if (len != 0) {
        status = HAL_UART_Receive( &huart1, (uint8_t *) result, len, HAL_MAX_DELAY);
        if (status == HAL_OK) {
            retcode = len;
        } else {
            retcode = -1;
        }
    }
    return(retcode);
}

void Nextion_Send_String(char *Id, char *String)
{
	uint8_t Cmd_End[3] = { 0xFF, 0xFF, 0xFF};
	char buf[50];
	int len = sprintf(buf, "%s.txt=\"%s\"", Id, String);
	printf("Stringr=%s\n", buf);
	HAL_UART_Transmit(&huart4, (uint8_t *)buf, len, 1000);
	HAL_UART_Transmit(&huart4, Cmd_End, sizeof(Cmd_End), 1000);
}

void Nextion_Send_Number(char *Id, int no)
{
	uint8_t Cmd_End[3] = { 0xFF, 0xFF, 0xFF};
	char buf[50];
	int len = sprintf(buf, "%s.val=%d", Id, no);
	printf("Number=%s\n", buf);
	HAL_UART_Transmit(&huart4, (uint8_t *)buf, len, 1000);
	HAL_UART_Transmit(&huart4, Cmd_End, sizeof(Cmd_End), 1000);
}


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	char *SSID = "jmkim";
	char *PASSWD = "qaz12345";
    char req_data[BUF_SZ];
#ifdef THINGSPARK
    uint16_t sent_data_len = 0;
	float field1, field2;
#endif
	int count = 0;

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
  MX_ADC1_Init();
  MX_DFSDM1_Init();
  MX_I2C1_Init();
  MX_I2C2_Init();
  MX_OCTOSPI1_Init();
  MX_SPI1_Init();
  MX_SPI3_Init();
  MX_UART4_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  MX_USB_OTG_FS_USB_Init();
  /* USER CODE BEGIN 2 */
  printf("WIFI Init ...\n");
      WIFI_Status_t ret = WIFI_Init();
      if (ret != WIFI_STATUS_OK) {
        printf("Failed to initialize Wi-Fi [%d]\n", ret);
      }
      uint8_t  MAC_Addr[6];
      if(WIFI_GetMAC_Address(MAC_Addr) == WIFI_STATUS_OK)
      {
      	printf("WIFI MAC Address : %X:%X:%X:%X:%X:%X\n",
      	MAC_Addr[0], MAC_Addr[1], MAC_Addr[2],
      	MAC_Addr[3], MAC_Addr[4], MAC_Addr[5]);
      }
      else
      {
      	printf("> ERROR : CANNOT get MAC address\r\n");
      }

      printf("WIFI Connect to %s ...\n", SSID);
      ret = WIFI_Connect(SSID, PASSWD, WIFI_ECN_WPA2_PSK);
      if (ret != WIFI_STATUS_OK)
      {
        printf("Failed to connect AP [%d]\n", ret);
      }


      uint8_t IP_Addr[4];
      if(WIFI_GetIP_Address(IP_Addr) == WIFI_STATUS_OK)
      {
        printf("WIFI IP Address : %d.%d.%d.%d\n",
        IP_Addr[0], IP_Addr[1],	IP_Addr[2], IP_Addr[3]);
        sprintf(req_data, "WIFI IP Address : %d.%d.%d.%d",
                IP_Addr[0], IP_Addr[1],	IP_Addr[2], IP_Addr[3]);
        Nextion_Send_String("t1", req_data);
        printf("send WIFI IP Address : %s\n", req_data);
        HAL_Delay(1000);
      }
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	printf("Connect to thinspark\n");
	bzero(req_data, BUF_SZ);

  while (1)
  {
	  char sbuf[100];
//	  // USER_Button delete
//	  HAL_GPIO_TogglePin(LED2_GREEN_GPIO_Port, LED2_GREEN_Pin);
//	  HAL_Delay(1000);

// THINGSPARK
//	field1 = (float) HAL_GetTick();
	printf("count = %d\n", ++count);
	Nextion_Send_Number("n0", count);
	HAL_Delay(1000);
	sprintf(sbuf, "Try %6ld", HAL_GetTick()); //
	printf("%s\n", sbuf);
	Nextion_Send_String("t1", sbuf);
	HAL_Delay(1000);

#ifdef THINSPARK
	  uint8_t ipAddr[4];
	  ret = WIFI_GetHostAddress("api.thingspark.co.kr", ipAddr);
	  if (ret != WIFI_STATUS_OK)
	  {
		  printf("Failed to get host address [%d]\n", ret);
		  return (-1);
	  } else
	  {
		  printf("WIFI thingspark-connect\n");
	  }
	  ret = WIFI_OpenClientConnection(SOCKET_NUM, WIFI_TCP_PROTOCOL, "cloud-connect", ipAddr, 8480, 0);
	  HAL_Delay(1000);
	  if (ret != WIFI_STATUS_OK)
	  {
		  printf("Failed to connect to server [%d]\r\n", ret);
	  }

	  field1 = (float) HAL_GetTick();
	  field2 = 99.99;
	  sprintf(req_data,"GET /update?apiKey=1Goxe9xWIuw2uk84&field1=%f&field2=%f\r\n", field1, field2);

	  WIFI_SendData(SOCKET_NUM, (uint8_t *)req_data, sizeof(req_data), &sent_data_len, TIMEOUT);
	  HAL_Delay(1000);
	  WIFI_CloseClientConnection(SOCKET_NUM);
#endif

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

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE|RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 60;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_USART2
                              |RCC_PERIPHCLK_USART3|RCC_PERIPHCLK_UART4
                              |RCC_PERIPHCLK_I2C1|RCC_PERIPHCLK_I2C2
                              |RCC_PERIPHCLK_DFSDM1|RCC_PERIPHCLK_USB
                              |RCC_PERIPHCLK_ADC|RCC_PERIPHCLK_OSPI;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  PeriphClkInit.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1;
  PeriphClkInit.Uart4ClockSelection = RCC_UART4CLKSOURCE_PCLK1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
  PeriphClkInit.I2c2ClockSelection = RCC_I2C2CLKSOURCE_PCLK1;
  PeriphClkInit.AdcClockSelection = RCC_ADCCLKSOURCE_PLLSAI1;
  PeriphClkInit.Dfsdm1ClockSelection = RCC_DFSDM1CLKSOURCE_PCLK;
  PeriphClkInit.OspiClockSelection = RCC_OSPICLKSOURCE_SYSCLK;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLLSAI1;
  PeriphClkInit.PLLSAI1.PLLSAI1Source = RCC_PLLSOURCE_MSI;
  PeriphClkInit.PLLSAI1.PLLSAI1M = 1;
  PeriphClkInit.PLLSAI1.PLLSAI1N = 24;
  PeriphClkInit.PLLSAI1.PLLSAI1P = RCC_PLLP_DIV2;
  PeriphClkInit.PLLSAI1.PLLSAI1Q = RCC_PLLQ_DIV2;
  PeriphClkInit.PLLSAI1.PLLSAI1R = RCC_PLLR_DIV2;
  PeriphClkInit.PLLSAI1.PLLSAI1ClockOut = RCC_PLLSAI1_48M2CLK|RCC_PLLSAI1_ADC1CLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
  /** Enable MSI Auto calibration
  */
  HAL_RCCEx_EnableMSIPLLMode();
}

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if (GPIO_Pin == USER_BUTTON_Pin)
	{
		HAL_GPIO_TogglePin(LED2_GREEN_GPIO_Port, LED2_GREEN_Pin);
	}
}
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
