/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lwip/opt.h"
#include "lwip/api.h"
#include "lwip/inet.h"
#include "lwip/sockets.h"

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
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId socketTaskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void socketTaskStart(void const * argument);

extern void MX_LWIP_Init(void);
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of socketTask */
  osThreadDef(socketTask, socketTaskStart, osPriorityIdle, 0, 512);
  socketTaskHandle = osThreadCreate(osThread(socketTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* init code for LWIP */
  MX_LWIP_Init();
  /* USER CODE BEGIN StartDefaultTask */

  /* Infinite loop */
  for(;;)
  {
    osDelay(1);

  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_socketTaskStart */
/**
* @brief Function implementing the socketTask thread.
* @param argument: Not used
* @retval None
*/


typedef union CANserial_msg_u{
	uint8_t bytes[19];

	struct __attribute__ ((__packed__)) data_s{
		uint8_t SOF;
		uint32_t Timestamp;
		uint8_t DLC;
		uint32_t ID;
		uint8_t END[9];
	}data;
}CANserial_msg;


/* USER CODE END Header_socketTaskStart */
void socketTaskStart(void const * argument)
{
  /* USER CODE BEGIN socketTaskStart */

	//Wait a second for the link to be init
	vTaskDelay(1000);

	int sock, newconn, size;
	  struct sockaddr_in address, remotehost;

	 /* create a TCP socket */
	  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	  {
		return;
	  }

	  /* bind to port at any interface */
	  address.sin_family = AF_INET;
	  address.sin_port = htons(1234);
	  address.sin_addr.s_addr = INADDR_ANY;

	  if (bind(sock, (struct sockaddr *)&address, sizeof (address)) < 0)
	  {
		return;
	  }


	  /* listen for incoming connections (TCP listen backlog = 5) */
	  listen(sock, 5);

	  size = sizeof(remotehost);
	  //int buflen = 1500;
	  //unsigned char recv_buffer[1500];
	  //int ret = read(newconn, recv_buffer, sizeof(recv_buffer));

	/* Infinite loop */
	for (;;) {
		newconn = accept(sock, (struct sockaddr* )&remotehost,
				(socklen_t* )&size);


		for (;;) {
			osDelay(100);

			HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);




			char data[19] = {
					0xAA,	0x66, 0x73, 0x00, 0x00,	0x01,	0x01, 0x00, 0x00, 0x00,	0x11, 0xBB,
			};

/*			//Create frame
			data[0] = 0xAA;
			// Start of frame
			data[1] = 0;//Timestamp
			data[2] = 0;//Timestamp
			data[3] = 0;//Timestamp
			data[4] = 0;//Timestamp

			data[5] = 4; //DLC

			data[6] = 1; //Arbitration ID 4
			data[7] = 1; //Arbitration ID 3
			data[8] = 1; //Arbitration ID 2
			data[9] = 1; //Arbitration ID 1

			data[10] = 0; //Payload
			data[11] = 0;
			data[12] = 0;
			data[13] = 0;

			data[14] = 0xBB; //End of frame
*/

			CANserial_msg msg;
			msg.data.SOF = 0xAA;
			msg.data.Timestamp = 123;
			msg.data.DLC = 8;
			msg.data.ID = 0x234;
			msg.data.END[8] = 0xBB;

			uint8_t len = 19;

			write(newconn, (const unsigned char* )(msg.bytes), len);

		}

		//http_server_serve(newconn);
	}
	/* USER CODE END socketTaskStart */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
