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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ff.h"
#include "CS43.h"
#include "file_system.h"
#include "ssd1306.h"
#include "display_control.h"
#include "mp3dec.h"

#define K0	!HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14)
#define K1	!HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_13)
#define K2	!HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12)
#define K3	!HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11)
#define K4	!HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1)
#define K5	!HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0)
#define K6	!HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3)
#define K7	!HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2)

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
I2C_HandleTypeDef hi2c1;

I2S_HandleTypeDef hi2s3;
DMA_HandleTypeDef hdma_spi3_tx;

SPI_HandleTypeDef hspi1;
SPI_HandleTypeDef hspi2;
DMA_HandleTypeDef hdma_spi1_rx;

TIM_HandleTypeDef htim4;

DMA_HandleTypeDef hdma_memtomem_dma2_stream0;
/* USER CODE BEGIN PV */

char buffer[256];
static FATFS FatFs;
FRESULT fresult;
FIL mfile;
WORD bytes_written;
WORD bytes_read;

//extern const uint8_t rawAudio[123200];

#define SM	44100

uint16_t audio[SM];
//uint16_t read[SM];
uint16_t sample[2];

unsigned sam = 0;

volatile unsigned long filesize = 0;
unsigned chs = 1;
int pr = 0;

int button = 0;
uint8_t mode = 0;
uint8_t stop = 0;
int volume = 50;
//fs
char* fname = "/";
uint8_t fpos = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_SPI1_Init(void);
static void MX_I2C1_Init(void);
static void MX_I2S3_Init(void);
static void MX_TIM4_Init(void);
static void MX_SPI2_Init(void);
/* USER CODE BEGIN PFP */

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM4)
	{
		if(filesize>0 && stop==0)
		{
			while(HAL_I2S_GetState(&hi2s3) == HAL_I2S_STATE_BUSY_TX)
			{}

			/*if(filesize<SM)
			{
				//readData(audio,&mfile,filesize);
				filesize=0;
			}
			else
			{
				//readData(audio,&mfile,SM);
				filesize-=SM;
				//HAL_I2S_Transmit_DMA
			}*/
			/*sample[0]=rawAudio[sam];
			sample[1]=rawAudio[sam];
			HAL_I2S_Transmit(&hi2s3, sample, 2, 5);
			sam++;
			if(sam==123200) sam =0;*/

			/*if(chs==1)
			{
				sample[0] = audio[sam];
				sample[1] = audio[sam];
				sam++;
			}
			else if(chs==2)
			{
				sample[0] = audio[sam];
				sample[1] = audio[sam+1];
				sam+=2;
			}
			if(sam>=SM)
			{
				sam=0;
				filesize-=SM;
				//HAL_DMA_Start(&hdma_memtomem_dma2_stream0, *read, *sample, SM);

				readData(audio,&mfile,SM);
				pr++;
			}
			if(sam<filesize)
			{

				HAL_I2S_Transmit(&hi2s3, sample, 2, 5);

				HAL_I2S_Transmit_DMA(&hi2s3, sample, 2);

			}
			else
			{
				filesize = 0;
				closeFile(&mfile);
			}*/

			if(filesize<SM)
			{
				HAL_I2S_Transmit_DMA(&hi2s3, audio, filesize);
				filesize = 0;
			}
			else
			{
				HAL_I2S_Transmit_DMA(&hi2s3, audio, SM);
				filesize -= SM;
				readData(audio, &mfile,SM);
			}
		}
	}
}




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
  MX_SPI1_Init();
  MX_I2C1_Init();
  MX_I2S3_Init();
  MX_TIM4_Init();
  MX_SPI2_Init();
  /* USER CODE BEGIN 2 */

  char* ts = "au.wav";




  fresult = fatInit(&FatFs);
  HAL_Delay(1);
  fresult = openFile(ts, &mfile);
  HAL_Delay(1);
  filesize = setUp(&mfile, &hi2s3, &chs);
  HAL_Delay(1);

	/*HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, 0);

	fresult = f_mount(&FatFs, "", 0);
	fresult = f_open(&file, "write.txt", FA_OPEN_ALWAYS | FA_WRITE);
	volatile int len = 0;


    FRESULT res;
    DIR dir;
    static FILINFO fno;
    volatile int ile = 0;
    res = f_opendir(&dir,"/");
    if(res == FR_OK)
    {
    	while(1)
		{
			res = f_readdir(&dir,&fno);
			if(res != FR_OK || fno.fname[0] == 0) break;
			else if(fno.fattrib & AM_SYS) continue;
			else ile++;
			len = sprintf( buffer, "\n");
			fresult = f_write(&file,buffer,len,&bytes_written);
			len = sprintf( buffer, fno.fname);
			fresult = f_write(&file,buffer,len,&bytes_written);

		}
    }
    fresult = f_close (&file);
    volatile int v1 = 1;*/


  	//readData(read,&mfile,SM);
  	//HAL_DMA_Start(&hdma_memtomem_dma2_stream0, *read, *sample, SM);
  	readData(audio,&mfile,SM);
  	//readData(read,&mfile,SM);
    CS43_Start(&hi2c1);

    int lll = 0;
    setVolume(&hi2c1,100);


    //HAL_TIM_Base_Start_IT(&htim4);
    //filesize = 1;
    displayInit();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

	  /*v1 = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14);
	  if(!v1)
	  {
		  HAL_GPIO_TogglePin(GPIOD,GPIO_PIN_14);
	  }
	  v1=1;
	  HAL_Delay(500);*/

	  //HAL_I2S_Transmit_DMA(&hi2s3, sample, 2);
		//while(HAL_I2S_GetState(&hi2s3) == HAL_I2S_STATE_BUSY_TX)
		//{}
	  	  if(stop==0)
	  	  {
	  		if(lll<SM)
	  			  	{
	  			  		//HAL_I2S_Transmit(&hi2s3, &audio[lll], 2, 20);
	  			  		//HAL_I2S_Transmit_DMA(&hi2s3, audio, SM);
	  			  		HAL_I2S_Transmit_DMA(&hi2s3, audio, SM);
	  			  		//HAL_I2S_Transmit_DMA(&hi2s3, &audio[lll++], 1);
	  			  		//HAL_I2S_Transmit_DMA(&hi2s3, &audio[lll++], 1);
	  			  		HAL_GPIO_TogglePin(GPIOD,GPIO_PIN_14);
	  			  		//HAL_I2S_Transmit_DMA(&hi2s3, &audio[lll], 2);
	  			  		//HAL_I2S_Transmit(&hi2s3, &audio[lll], 2, 5);
	  			  		lll+=SM;
	  			  	}
	  			  	else
	  			  	{
	  			  		lll=0;
	  			  		if(filesize<SM)
	  					{
	  						readData(audio,&mfile,filesize);
	  						filesize=0;
	  					}
	  					else
	  					{
	  						readData(audio,&mfile,SM);
	  						filesize-=SM;
	  						//HAL_I2S_Transmit_DMA
	  					}
	  			  	}
	  	  }



	  if(K0)
	  {
		  if(pr) continue;
		  pr=1;
		  if(fresult!=FR_OK)
		  {
			  fresult = fatInit(&FatFs);
			  displayInit();
		  }
		  else
		  {
			  mode = (mode+1)%2;
			  //change mode
			  fpos = 0;
			  update(&fpos);
		  }

	  }
	  else if(K1)	//stop play
	  {
		  HAL_StatusTypeDef status;
		 if(pr) continue;
		 pr=1;
		 stop = (++stop)%2;
		 if(stop){
			 while ((status = HAL_I2C_Mem_Write(&hi2c1, 0x94, 0x0F, 1, 253, 1, 100)) != HAL_OK);
		 }
		 else{
			 while ((status = HAL_I2C_Mem_Write(&hi2c1, 0x94, 0x0F, 1, 0, 1, 100)) != HAL_OK);
			 setVolume(&hi2c1,volume);
		 }
	  }
	  else if(K2)
	  {
		  if(pr) continue;
		  pr=1;
		  if(volume<100) volume+=10;
		  setVolume(&hi2c1,volume);
	  }
	  else if(K3)
	  {
		  if(pr) continue;
		  pr=1;
		  if(volume>0) volume-=10;
		  setVolume(&hi2c1,volume);
	  }
	  else if(K4)
	  {
		  if(pr) continue;
		  pr=1;
		  goBack(&fpos);
		  fpos=0;
		  update(&fpos);
	  }
	  else if(K5)
	  {
		  if(pr) continue;
		  pr=1;
		  //open
		  if(open(&fpos, &mfile)==1) {
			  fpos=0;
			  update(&fpos);
		  }
	  }
	  else if(K6)
	  {
		  if(pr) continue;
		  pr=1;
		  scrollUp(&fpos);
		  if(fpos==4) open(&fpos, &mfile);
		  update(&fpos);
		  HAL_Delay(1);
	  }
	  else if(K7)
	  {
		  if(pr) continue;
		  pr=1;
		  scrollDown(&fpos);
		  if(fpos==0) {
			  open(&fpos, &mfile);
		  }
		  update(&fpos);
	  }
	  else pr=0;




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
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

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
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_I2S;
  PeriphClkInitStruct.PLLI2S.PLLI2SN = 192;
  PeriphClkInitStruct.PLLI2S.PLLI2SR = 2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief I2S3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2S3_Init(void)
{

  /* USER CODE BEGIN I2S3_Init 0 */

  /* USER CODE END I2S3_Init 0 */

  /* USER CODE BEGIN I2S3_Init 1 */

  /* USER CODE END I2S3_Init 1 */
  hi2s3.Instance = SPI3;
  hi2s3.Init.Mode = I2S_MODE_MASTER_TX;
  hi2s3.Init.Standard = I2S_STANDARD_PHILIPS;
  hi2s3.Init.DataFormat = I2S_DATAFORMAT_16B;
  hi2s3.Init.MCLKOutput = I2S_MCLKOUTPUT_ENABLE;
  hi2s3.Init.AudioFreq = I2S_AUDIOFREQ_16K;
  hi2s3.Init.CPOL = I2S_CPOL_LOW;
  hi2s3.Init.ClockSource = I2S_CLOCK_PLL;
  hi2s3.Init.FullDuplexMode = I2S_FULLDUPLEXMODE_DISABLE;
  if (HAL_I2S_Init(&hi2s3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2S3_Init 2 */

  /* USER CODE END I2S3_Init 2 */

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
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
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
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
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
  * @brief TIM4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM4_Init(void)
{

  /* USER CODE BEGIN TIM4_Init 0 */

  /* USER CODE END TIM4_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 41;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 1;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM4_Init 2 */

  /* USER CODE END TIM4_Init 2 */

}

/** 
  * Enable DMA controller clock
  * Configure DMA for memory to memory transfers
  *   hdma_memtomem_dma2_stream0
  */
static void MX_DMA_Init(void) 
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* Configure DMA request hdma_memtomem_dma2_stream0 on DMA2_Stream0 */
  hdma_memtomem_dma2_stream0.Instance = DMA2_Stream0;
  hdma_memtomem_dma2_stream0.Init.Channel = DMA_CHANNEL_0;
  hdma_memtomem_dma2_stream0.Init.Direction = DMA_MEMORY_TO_MEMORY;
  hdma_memtomem_dma2_stream0.Init.PeriphInc = DMA_PINC_ENABLE;
  hdma_memtomem_dma2_stream0.Init.MemInc = DMA_MINC_ENABLE;
  hdma_memtomem_dma2_stream0.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
  hdma_memtomem_dma2_stream0.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
  hdma_memtomem_dma2_stream0.Init.Mode = DMA_NORMAL;
  hdma_memtomem_dma2_stream0.Init.Priority = DMA_PRIORITY_LOW;
  hdma_memtomem_dma2_stream0.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
  hdma_memtomem_dma2_stream0.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
  hdma_memtomem_dma2_stream0.Init.MemBurst = DMA_MBURST_SINGLE;
  hdma_memtomem_dma2_stream0.Init.PeriphBurst = DMA_PBURST_SINGLE;
  if (HAL_DMA_Init(&hdma_memtomem_dma2_stream0) != HAL_OK)
  {
    Error_Handler( );
  }

  /* DMA interrupt init */
  /* DMA1_Stream5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);
  /* DMA2_Stream2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream2_IRQn);

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
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, D_C_Pin|CS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(RES_GPIO_Port, RES_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15 
                          |GPIO_PIN_4|GPIO_PIN_7, GPIO_PIN_RESET);

  /*Configure GPIO pins : K7_Pin K6_Pin */
  GPIO_InitStruct.Pin = K7_Pin|K6_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : D_C_Pin CS_Pin */
  GPIO_InitStruct.Pin = D_C_Pin|CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : K5_Pin K4_Pin K3_Pin K2_Pin 
                           K1_Pin K0_Pin */
  GPIO_InitStruct.Pin = K5_Pin|K4_Pin|K3_Pin|K2_Pin 
                          |K1_Pin|K0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : RES_Pin */
  GPIO_InitStruct.Pin = RES_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(RES_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PD12 PD13 PD14 PD15 
                           PD4 PD7 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15 
                          |GPIO_PIN_4|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : PC9 */
  GPIO_InitStruct.Pin = GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

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
