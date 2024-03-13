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
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
//#include "oled.h"
#include "arm_math.h"
#include "arm_const_structs.h"
#include "oled.h"
#include "sys.h"
#include "stdio.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */


/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

float32_t  	testInput_f32[NPT];	//fft输入数据
float32_t  	testOutput_f32[NPT];	//fft输出数据
float32_t  	testOutput[NPT];			//输出频谱
uint16_t	 	ADC_data[NPT] ;			//adc原始数据
uint8_t 	 	ADC_flag = 0;						//adc转换完成标志位
float32_t  	Uo[Hn+1];							//输出谐波幅值
float32_t  	THD;
uint8_t			dis_buff[20];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

void arm_rfft_fast_f32_app(void) ;

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_TIM2_Init();
  MX_USART1_UART_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */
	IIC_Init();		//初始化LED	
	OLED_Init();
	HAL_TIM_Base_Start_IT(&htim1);
	HAL_TIM_Base_Start(&htim2);
	HAL_ADC_Start_DMA(&hadc1,(uint32_t *)ADC_data,1024);
	SW_Control(state_Array[0]);
//	arm_rfft_fast_f32_app();
 
	OLED_Refresh();//更新oled缓存
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while(1)
  {
//		PGout(4) = 0;
//		HAL_Delay(10);
//		PGout(4) = 1;
//		HAL_Delay(10);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		display_distort (state);
		OLED_Refresh();//更新oled缓存
		if(ADC_flag == 1)
		{
			for (int x=0;x < 1024; x++)
			{
				testInput_f32[x] = (float)(ADC_data[x]*3.3/4096);
//				printf("%f\r\n",testInput_f32[x]);
			}
			arm_rfft_fast_f32_app();
			snprintf(dis_buff,sizeof(dis_buff),"THD:%2.2f%%   ",THD*100);
			OLED_ShowString(0,0,dis_buff,16);
			ADC_flag = 0;
		}
		else if(ADC_flag == 2)
		{
			HAL_TIM_Base_Start(&htim2);
			HAL_ADC_Start_DMA(&hadc1,(uint32_t *)ADC_data,1024);
		}
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

  /** Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
int n;
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim == &htim1)
	{
		n++;
		if(n == 500)
		{
			HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_7);
			if(ADC_flag == 0)
			{
				ADC_flag = 2;
			}
			n = 0;
		}
	}
}


void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    if(hadc == &hadc1)//ADC采集中断
    {
        HAL_TIM_Base_Stop(&htim2);//启动定时器
        HAL_ADC_Stop_DMA(&hadc1);
        ADC_flag = 1;
    }
}

void arm_rfft_fast_f32_app(void) 
{ 
  uint16_t i; 
	uint16_t j; 
	uint16_t k;
	float distort_comp = 0;	//失真度算法补偿
	
  arm_rfft_fast_instance_f32 S; 

  /* 实数序列FFT长度 */ 
  uint16_t fftSize = NPT;  
  /* 正变换 */ 
  uint8_t ifftFlag = 0;  
  /* 初始化结构体S中的参数 */ 
  arm_rfft_fast_init_f32(&S, fftSize); 
  arm_rfft_fast_f32(&S, testInput_f32, testOutput_f32, ifftFlag); //实序列fft
  arm_cmplx_mag_f32(testOutput_f32, testOutput, fftSize); 
	Uo[0] = testOutput[0]/1024;
	printf("Uo%d = %f\r\n",0,Uo[0]);
	for(int y = 1;y < Hn+1;y++)
	{
		Uo[y] =  testOutput[PD*y]/512;
		Uo[y] = Uo[y]*Uo[y];
//		printf("Uo%d = %f\r\n",y ,Uo[y]); //输出谐波处的数值
	}
	THD = sqrtf((Uo[2] + Uo[3] + Uo[4] + Uo[5])/Uo[1]);
	switch(state)
	{
		case 0:		//无失真
		{
			distort_comp = 0.0003;
		}break;
		case 1:		//顶部失真
		{
			distort_comp = -0.01;
		}break;
		case 2:		//底部失真
		{
			distort_comp = -0.01;
		}break;
		case 3:		//双向失真
		{
			distort_comp = -0.02;
		}break;
		case 4:		//交越失真
		{
			distort_comp = 0.0;
		}
	}
//	THD = THD - distort_comp;
//	printf("分划线---------------------------------------\r\n");
//  for(i=0; i<fftSize/2; i++) 
//  { 
//    printf("%f\r\n", testOutput[i]); 
//  }
	printf("THD = %f\r\n",THD);

}


void display_distort (uint8_t state)
{
	switch(state)
	{
		case 0:
		{
		OLED_ShowChinese(0,16,0,16);			//无
		OLED_ShowChinese(16,16,11,16);		//失
		OLED_ShowChinese(32,16,12,16);		//真
		OLED_ShowString(48,16,"  ",16);		//
		}break;
		case 1:
		{
		OLED_ShowChinese(0,16,3,16);			//顶
		OLED_ShowChinese(16,16,4,16);			//部
		OLED_ShowChinese(32,16,11,16);		//失
		OLED_ShowChinese(48,16,12,16);		//真
		}break;
		case 2:
		{
		OLED_ShowChinese(0,16,5,16);			//底
		OLED_ShowChinese(16,16,4,16);			//部
		OLED_ShowChinese(32,16,11,16);		//失
		OLED_ShowChinese(48,16,12,16);		//真
		}break;
		case 3:
		{
		OLED_ShowChinese(0,16,7,16);			//双
		OLED_ShowChinese(16,16,8,16);			//向
		OLED_ShowChinese(32,16,11,16);		//失
		OLED_ShowChinese(48,16,12,16);		//真
		}break;
		case 4:
		{
		OLED_ShowChinese(0,16,9,16);			//交
		OLED_ShowChinese(16,16,10,16);		//越
		OLED_ShowChinese(32,16,11,16);		//失
		OLED_ShowChinese(48,16,12,16);		//真
		}
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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
