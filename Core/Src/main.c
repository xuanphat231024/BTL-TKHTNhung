/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "LCD.h"
#include "string.h"
#include "flash.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
uint8_t enter_passcode[6] = {0};  // Array is used to compare with passcode.
uint8_t passcode[6] = {1, 2, 3, 4, 5, 6};  // Default passcode when beginning restart system.
uint8_t new_passcode[6] = {0};  // Array of values for enter new passcode.
uint8_t count = 0;    // Numerical order of passcode.
uint8_t x = 1 ;  // variable x Cursor of LCD.
uint8_t y = 9 ;  // Variable y Cursor of LCD.
uint8_t key, check, check_old;  // values in process.
uint8_t False_Passcode = 3;  // You have 3 times to enter passcode.
uint8_t Check_Mode = 0; // Check a change mode or enter mode. Check_Mode = 0 => Enter mode.
uint8_t flag_new_pass = 0;  //
uint8_t Change_flag = 0;
uint8_t Value_check ;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define Flash_save_pass 0x08007C3C
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */
void Reset_LCD();
void Read_Keypad(int key);
void Change_Passcode();
void Default_Change_Screen();
void Check_Old_Passcode();
void Successful_change_screen();
void Xu_Ly_Mat_Khau();
int  KeyPad();
void Save_pass_to_flash();
void Read_pass_from_flash();
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
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  Value_check = Flash_Read_2Byte(Flash_save_pass);
  if( Value_check != 255) // 255 is 0xFF value.
  {
	  Change_flag = 1;
  }
  Read_pass_from_flash();
  Reset_LCD();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
   KeyPad();
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_SlaveConfigTypeDef sSlaveConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 65535;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sSlaveConfig.SlaveMode = TIM_SLAVEMODE_DISABLE;
  sSlaveConfig.InputTrigger = TIM_TS_ITR2;
  if (HAL_TIM_SlaveConfigSynchro(&htim2, &sSlaveConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7
                          |GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_10
                          |GPIO_PIN_11|GPIO_PIN_3|GPIO_PIN_8|GPIO_PIN_9, GPIO_PIN_RESET);

  /*Configure GPIO pins : PA0 PA1 PA2 PA3
                           PA4 PA5 PA6 PA7
                           PA15 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7
                          |GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB0 PB1 PB2 PB10
                           PB11 PB3 PB8 PB9 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_10
                          |GPIO_PIN_11|GPIO_PIN_3|GPIO_PIN_8|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PB4 PB5 PB6 PB7 */
  GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
//------------------------------------------------
// RESTART WITH LCD.
     void Reset_LCD()
     {
     //DEFAULT SCREEN.
       LCD_Clear();
       x = 1 ;
       y = 9 ;
       LCD_Init();
       LCD_SetCursor(0,4);
	   LCD_Print("WELLCOME!");
	   HAL_Delay(50);
	   LCD_SetCursor(1,0);
	   LCD_Print("PASSCODE:");
	   LCD_SetCursor(1,9);
	   LCD_CursorBlink();
	   count = 0;
       x = 1 ;
       y = 9 ;
       Check_Mode = 0;
     }
     // WRITE DATA in ARRAY.
     void Read_Keypad(int key)
     {
         if(count < 6)
     	 {
     	  	 enter_passcode[count] = key;
     	  	 if(flag_new_pass)
     	  	 {
     	  	     new_passcode[count]=key;
     	  	     if(count==5)
     	  	     {
     	  	    	Save_pass_to_flash();
     	  	    	flag_new_pass = 0;
     	  	    	Check_Mode = 2;
     	  	     }
     	  	 }
     	  	    count++;
     	 }
     }
     void Save_pass_to_flash()
     {
         Flash_Erase(Flash_save_pass);
         for(int i = 0; i < 6; i++)
         {
        	 Flash_Write_2Byte(Flash_save_pass + i*2, new_passcode[i]);
         }
     }
     void Read_pass_from_flash()
	 {
    	 for(int i = 0; i < 6; i++)
    	 {
    	     new_passcode[i] = Flash_Read_2Byte(Flash_save_pass + i*2);
    	 }
	 }
     //--------------------------------------------
     // Change passcode if need.
     void Default_Change_Screen()
     {
         //Initial.
         Check_Mode = 1;
         count = 0;
         LCD_Clear();
         LCD_Print("Waiting...");
         LCD_DisableCursorBlink();
         HAL_Delay(1000);
     	 LCD_Clear();
     	 LCD_SetCursor(0,1);
     	 LCD_Print("Enter Old Pass");
     	 LCD_SetCursor(1,0);
     	 LCD_Print("=>");
     	 LCD_CursorBlink();
     }
     void Change_Passcode()
     {
     	 Default_Change_Screen();
         x = 1;
         y = 2;
     }
     //--------------------------------------------
     // CHECK OLD PASSCODE.
     void Check_Old_Passcode()
     {
     	 check_old = 1;
     	 if(Change_flag == 1)
     	 {
     		for(int i = 0; i < 6; i++)
     		{
     			if(new_passcode[i] != enter_passcode[i])
     			{
     				check_old  =  0;
     				break;
     			}
     			else if(count < 5 || count > 6)
     			{
     				check_old = 0;
     				break;
     			}
     			else
     			{
     				check_old = 1;
     			}
     		}
     	 }else
     	 {
     		 for(int i = 0; i < 6; i++)
     		 {
     			 if(passcode[i] != enter_passcode[i])
     			 {
     				 check_old  =  0;
     				 break;
     			 }
     			 else if(count < 5 || count > 6)
     			 {
     				 check_old = 0;
     				 break;
     			 }
     			 else
     			 {
     				 check_old = 1;
     			 }
     		 }
     	 }
     	 if(check_old != 1)
     	 {
     		 LCD_Clear();
     		 LCD_SetCursor(0,1);
     		 LCD_Print("FALSE,");
     		 LCD_SetCursor(1,1);
     		 LCD_Print("PLEASE AGAIN !");
     		 LCD_DisableCursorBlink();
     		 HAL_Delay(1500);
     		 Change_Passcode();
     	 }
     	 else
     	 {
     		 LCD_Clear();
     		 LCD_SetCursor(0,1);
     		 LCD_Print("Enter New Pass");
     		 LCD_SetCursor(1,0);
     		 LCD_Print("=>");
     		 x = 1; y = 2;
     		 count = 0;
     		 flag_new_pass = 1;
     	 }
     }
     void Successful_change_screen()
     {
    	 LCD_Clear();
    	 LCD_SetCursor(0,0);
    	 LCD_Print("Successful to");
    	 LCD_SetCursor(1,0);
    	 LCD_Print("Change Passcode!");
    	 HAL_Delay(1500);
     }
     //---------------------------------------------
     //CHECK PASSCODE to OPEN THE DOOR.
     void Check_passcode()
     {
     	 check = 1;
     	 if(Change_flag == 1)
     	 {
     		for(int i = 0; i < 6; i++)
     		{
     			if(new_passcode[i] != enter_passcode[i])
     			{
     				check  =  0;
     				break;
     			}
     			if(count < 5 || count > 6)
     			{
     				check = 0;
     				break;
     			}
     		}
     	 }else
     	 {
     		 for(int i = 0; i < 6; i++)
     		 {
     			 if(passcode[i] != enter_passcode[i])
     			 {
     				 check  =  0;
     				 break;
     			 }
     			 if(count < 5 || count > 6)
     			 {
     				 check = 0;
     				 break;
     			 }
     		 }
     	 }
     	  //IF PASSCODE FALSE => ANNOUNCE AND CLOSE THE DOOR.
     	 if(check != 1)
     	 {
     		 LCD_Clear();
     		 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, 1);
     		 HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, 1);
     		 HAL_Delay(500);
     		 HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, 0);
     		 LCD_SetCursor(0, 5);
     		 LCD_Print("FAIL,");
     		 LCD_SetCursor(1, 1);
     		 LCD_Print("PLEASE AGAIN!");
     		 LCD_DisableCursorBlink();
     		 HAL_Delay(1000);
     		 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, 0);
     		 False_Passcode--;
     		 if(False_Passcode == 0)
     		 {
     			 LCD_Clear();
     			 LCD_SetCursor(0, 0);
     			 LCD_Print("WAITING FOR 30s");
     			 HAL_Delay(30000);
     			 False_Passcode = 3;
     		 }
     		 Reset_LCD();
     	 }
     	 //IF PASSCODE TRUE => OPEN THE DOOR.
     	 else
     	 {
     		 //turn on green LED.
     		 HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3, 1);
     		 //Turn on sound open a door
     		 for(int i=0; i<4; i++)
     		 {
     			 HAL_GPIO_WritePin(GPIOB,GPIO_PIN_11, 1);
     			 HAL_Delay(100);
     			 HAL_GPIO_WritePin(GPIOB,GPIO_PIN_11, 0);
     			 HAL_Delay(100);
     		 }
     		 //Announce open the door.
     		 LCD_Clear();
     		 LCD_SetCursor(0, 1);
     		 LCD_Print("HiHi,SUCCESS!!");
     		 LCD_SetCursor(1, 2);
     		 LCD_Print("WELLCOME!!!");
     		 LCD_DisableCursorBlink();
     		 HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10, 1);

     		 HAL_Delay(6000);
     		 HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10, 0);

     		 HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3, 0);

     		 False_Passcode = 3;
     		 Reset_LCD();
     	 }
     }

     //CHECK KEYPAD-----------------------------------------------
     int KeyPad()
     {
    	 HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_9|GPIO_PIN_8, 1);
    	 //SCAN COLUMN 1.
    	 HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, 0);
    	 HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1|GPIO_PIN_8|GPIO_PIN_9, 1);
    	 // NUMBER 7 BUTTON.
    	 if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4) == 0)
    	 {
    		 HAL_Delay(140);
    		 if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4) == 0)
    		 {
    			 LCD_SetCursor(x, y);
    			 LCD_CursorBlink();
    			 LCD_Print("7");
    			 key = 7;
    			 Read_Keypad(key);
    			 y = y + 1;
    	 }
    	 }
    	 // NUMBER 4 BUTTON.
    	 if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5) == 0)
    	 {
    		 HAL_Delay(130);
    		 if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5) == 0)
    		 {
    			 LCD_SetCursor(x, y);
    			 LCD_CursorBlink();
    			 LCD_Print("4");
    			 key = 4;
    			 Read_Keypad(key);
    			 y = y + 1;
    		 }
    	 }

    	 // NUMBER 1 BUTTON.

    	 if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6) == 0)
    	 {
    		 HAL_Delay(140);
    		 if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6) == 0)
    		 {
    			 LCD_SetCursor(x, y);
    			 LCD_CursorBlink();
    			 LCD_Print("1");
    			 key = 1;
    			 Read_Keypad(key);
    			 y = y + 1;
    		 }
    	 }
    	 // SYMBOL "*" BUTTON.
    	 if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7) == 0)
    	 {
    		 HAL_Delay(140);
    		 if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7) == 0)
    		 {
    			 LCD_SetCursor(x, y);
    			 LCD_CursorBlink();
    			 LCD_Print("*");
    			 y = y + 1;
    		 }
    	 }
    	 //------------------------------------------------------------
    	 // SCAN COLUMN 2
    	 HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, 0);
    	 HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_8|GPIO_PIN_9, 1);
    	 // NUMBER 8 BUTTON.
    	 if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4) == 0)
    	 {
    		 HAL_Delay(140);
    		 if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4) == 0)
    		 {
    			 LCD_SetCursor(x, y);
    			 LCD_CursorBlink();
    			 LCD_Print("8");
    			 key = 8;
    			 Read_Keypad(key);
    			 y = y + 1;
    		 }
    	 }
    	 // NUMBER 5 BUTTON.
    	 if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5) == 0)
    	 {
    		 HAL_Delay(140);
    		 if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5) == 0)
    		 {
    			 LCD_SetCursor(x, y);
    			 LCD_CursorBlink();
    			 LCD_Print("5");
    			 key = 5;
    			 Read_Keypad(key);
    			 y = y + 1;
    		 }
    	 }
    	 // NUMBER 2 BUTTON.
    	 if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6) == 0)
    	 {
    		 HAL_Delay(140);
    		 if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6) == 0)
    		 {
    			 LCD_SetCursor(x, y);
    			 LCD_CursorBlink();
    			 LCD_Print("2");
    			 key = 2;
    			 Read_Keypad(key);
    			 y = y + 1;
    		 }
    	 }
    	 // NUMBER 0 BUTTON.
    	 if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7) == 0)
    	 {
    		 HAL_Delay(140);
    		 if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7) == 0)
    		 {
    			 LCD_SetCursor(x, y);
    			 LCD_CursorBlink();
    			 LCD_Print("0");
    			 key = 0;
    			 Read_Keypad(key);
    			 y = y + 1;
    		 }
    	 }
    	 //-------------------------------------------------------------
    	 // SCAN COLUMN 3.
    	 HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, 0);
    	 HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_9, 1);
    	 // NUMBER 9 BUTTON.
    	 if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4) == 0)
    	 {
    		 HAL_Delay(140);
    		 if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4) == 0)
    		 {

    			 LCD_SetCursor(x, y);
    			 LCD_CursorBlink();
    			 LCD_Print("9");
    			 key = 9;
    			 Read_Keypad(key);
    			 y = y + 1;
    		 }
    	 }
    	 // NUMBER 6 BUTTON.
    	 if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5) == 0)
    	 {
    		 HAL_Delay(140);
    		 if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5) == 0)
    		 {
    			 LCD_SetCursor(x, y);
    			 LCD_CursorBlink();
    			 LCD_Print("6");
    			 key = 6;
    			 Read_Keypad(key);
    			 y = y + 1;
    		 }
    	 }
    	 // NUMBER 3 BUTTON.
    	 if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6) == 0)
    	 {
    		 HAL_Delay(140);
    		 if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6) == 0)
    		 {
    			 LCD_SetCursor(x, y);
    			 LCD_CursorBlink();
    			 LCD_Print("3");
    			 key = 3;
    			 Read_Keypad(key);
    			 y = y + 1;
    		 }
    	 }
    	 // SYMBOL "#" BUTTON.
    	 if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7) == 0)
    	 {
    		 HAL_Delay(140);
    		 if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7) == 0)
    		 {
    			 LCD_SetCursor(x, y);
    			 LCD_CursorBlink();
    			 LCD_Print("#");
    			 y = y + 1;
    		 }
    	 }
    	 //-------------------------------------------------------------
    	 // SCAN COLUMN 4, IT'S FUNCTION BUTTON.
    	 HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, 0);
    	 HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_8	, 1);
    	 // ENTER FUNCTION BUTTON.
    	 if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4) == 0)
    	 {
    		 HAL_Delay(140);
    		 if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4) == 0)
    		 {
    			 switch(Check_Mode)
    			 {
    			 case 0:
    				 Check_passcode();
    				 break;
    			 case 1:
    				 Check_Old_Passcode();
    				 break;
    			 case 2:
    				 Successful_change_screen();
    				 Reset_LCD();
    				 break;
    			 }
    		 }

    	 }
    	 // DELETE FUNCTION BUTTON
    	 if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5) == 0)
    	 {
    		 HAL_Delay(140);
    		 if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5) == 0)
    		 {
    			 y = y - 1;
    			 LCD_CursorBlink();
    			 LCD_Print(" ");
    			 LCD_SetCursor(x, y);
    			 count--;
    		 }
    	 }
    	 //CHANGE FUNCTION BUTTON.
    	 if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6) == 0)
    	 {
    		 HAL_Delay(140);
    		 if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6) == 0)
    		 {
    			 Change_Passcode();
    		 }
    	 }
    	 // RETURN DEFAULT SCREEN WHEN BEGIN.
    	 if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7) == 0)
    	 {
    		 HAL_Delay(140);
    		 if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7) == 0)
    		 {
    			 Reset_LCD();
    		 }
    	 }
    	 return key;
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
