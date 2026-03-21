/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : usbd_cdc_if.c
  * @version        : v1.0_Cube
  * @brief          : Usb device for Virtual Com Port.
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
#include "usbd_cdc_if.h"

/* USER CODE BEGIN INCLUDE */
#include "stdarg.h"
#include "cmsis_os.h"
#include "Data_Variables.h"

/* USER CODE END INCLUDE */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @brief Usb device library.
  * @{
  */

/** @addtogroup USBD_CDC_IF
  * @{
  */

/** @defgroup USBD_CDC_IF_Private_TypesDefinitions USBD_CDC_IF_Private_TypesDefinitions
  * @brief Private types.
  * @{
  */

/* USER CODE BEGIN PRIVATE_TYPES */

/* USER CODE END PRIVATE_TYPES */

/**
  * @}
  */

/** @defgroup USBD_CDC_IF_Private_Defines USBD_CDC_IF_Private_Defines
  * @brief Private defines.
  * @{
  */

/* USER CODE BEGIN PRIVATE_DEFINES */
/* USER CODE END PRIVATE_DEFINES */

/**
  * @}
  */

/** @defgroup USBD_CDC_IF_Private_Macros USBD_CDC_IF_Private_Macros
  * @brief Private macros.
  * @{
  */

/* USER CODE BEGIN PRIVATE_MACRO */
enum
 {
 	PERIPHERAL_USART,	// UART serial device
 	PERIPHERAL_USB 		// USB serial device
 };

void myPrintf(uint16_t peripheral, char *format, ...)
 {
 	char buffer[64];

 	va_list args;
 	va_start(args, format);
 	vsprintf(buffer, format, args);
 	va_end(args);

 	switch(peripheral)
 	{
 	case PERIPHERAL_USART:
 		// TODO:
 		break;
 	case PERIPHERAL_USB:
 		CDC_Transmit_FS ((uint8_t *)buffer, strlen(buffer));
 		break;
 	default:
 		break;
 	}
 }

/* USER CODE END PRIVATE_MACRO */

/**
  * @}
  */

/** @defgroup USBD_CDC_IF_Private_Variables USBD_CDC_IF_Private_Variables
  * @brief Private variables.
  * @{
  */
/* Create buffer for reception and transmission           */
/* It's up to user to redefine and/or remove those define */
/** Received data over USB are stored in this buffer      */
uint8_t UserRxBufferFS[APP_RX_DATA_SIZE];

/** Data to send over USB CDC are stored in this buffer   */
uint8_t UserTxBufferFS[APP_TX_DATA_SIZE];

/* USER CODE BEGIN PRIVATE_VARIABLES */

/* USER CODE END PRIVATE_VARIABLES */

/**
  * @}
  */

/** @defgroup USBD_CDC_IF_Exported_Variables USBD_CDC_IF_Exported_Variables
  * @brief Public variables.
  * @{
  */

extern USBD_HandleTypeDef hUsbDeviceFS;

/* USER CODE BEGIN EXPORTED_VARIABLES */
extern osMessageQId myQueue03Handle;
extern osSemaphoreId mySemUSB_BlockHandle;
/* USER CODE END EXPORTED_VARIABLES */

/**
  * @}
  */

/** @defgroup USBD_CDC_IF_Private_FunctionPrototypes USBD_CDC_IF_Private_FunctionPrototypes
  * @brief Private functions declaration.
  * @{
  */

static int8_t CDC_Init_FS(void);
static int8_t CDC_DeInit_FS(void);
static int8_t CDC_Control_FS(uint8_t cmd, uint8_t* pbuf, uint16_t length);
static int8_t CDC_Receive_FS(uint8_t* pbuf, uint32_t *Len);
static int8_t CDC_TransmitCplt_FS(uint8_t *pbuf, uint32_t *Len, uint8_t epnum);

/* USER CODE BEGIN PRIVATE_FUNCTIONS_DECLARATION */

/* USER CODE END PRIVATE_FUNCTIONS_DECLARATION */

/**
  * @}
  */

USBD_CDC_ItfTypeDef USBD_Interface_fops_FS =
{
  CDC_Init_FS,
  CDC_DeInit_FS,
  CDC_Control_FS,
  CDC_Receive_FS,
  CDC_TransmitCplt_FS
};

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Initializes the CDC media low layer over the FS USB IP
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CDC_Init_FS(void)
{
  /* USER CODE BEGIN 3 */
  /* Set Application Buffers */
  USBD_CDC_SetTxBuffer(&hUsbDeviceFS, UserTxBufferFS, 0);
  USBD_CDC_SetRxBuffer(&hUsbDeviceFS, UserRxBufferFS);
  return (USBD_OK);
  /* USER CODE END 3 */
}

/**
  * @brief  DeInitializes the CDC media low layer
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CDC_DeInit_FS(void)
{
  /* USER CODE BEGIN 4 */
  return (USBD_OK);
  /* USER CODE END 4 */
}

/**
  * @brief  Manage the CDC class requests
  * @param  cmd: Command code
  * @param  pbuf: Buffer containing command data (request parameters)
  * @param  length: Number of data to be sent (in bytes)
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CDC_Control_FS(uint8_t cmd, uint8_t* pbuf, uint16_t length)
{
  /* USER CODE BEGIN 5 */
  switch(cmd)
  {
    case CDC_SEND_ENCAPSULATED_COMMAND:

    break;

    case CDC_GET_ENCAPSULATED_RESPONSE:

    break;

    case CDC_SET_COMM_FEATURE:

    break;

    case CDC_GET_COMM_FEATURE:

    break;

    case CDC_CLEAR_COMM_FEATURE:

    break;

  /*******************************************************************************/
  /* Line Coding Structure                                                       */
  /*-----------------------------------------------------------------------------*/
  /* Offset | Field       | Size | Value  | Description                          */
  /* 0      | dwDTERate   |   4  | Number |Data terminal rate, in bits per second*/
  /* 4      | bCharFormat |   1  | Number | Stop bits                            */
  /*                                        0 - 1 Stop bit                       */
  /*                                        1 - 1.5 Stop bits                    */
  /*                                        2 - 2 Stop bits                      */
  /* 5      | bParityType |  1   | Number | Parity                               */
  /*                                        0 - None                             */
  /*                                        1 - Odd                              */
  /*                                        2 - Even                             */
  /*                                        3 - Mark                             */
  /*                                        4 - Space                            */
  /* 6      | bDataBits  |   1   | Number Data bits (5, 6, 7, 8 or 16).          */
  /*******************************************************************************/
    case CDC_SET_LINE_CODING:

    break;

    case CDC_GET_LINE_CODING:

    break;

    case CDC_SET_CONTROL_LINE_STATE:

    break;

    case CDC_SEND_BREAK:

    break;

  default:
    break;
  }

  return (USBD_OK);
  /* USER CODE END 5 */
}

/**
  * @brief  Data received over USB OUT endpoint are sent over CDC interface
  *         through this function.
  *
  *         @note
  *         This function will issue a NAK packet on any OUT packet received on
  *         USB endpoint until exiting this function. If you exit this function
  *         before transfer is complete on CDC interface (ie. using DMA controller)
  *         it will result in receiving more data while previous ones are still
  *         not sent.
  *
  * @param  Buf: Buffer of data to be received
  * @param  Len: Number of data received (in bytes)
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CDC_Receive_FS(uint8_t* Buf, uint32_t *Len)
{
  /* USER CODE BEGIN 6 */
	USBD_CDC_SetRxBuffer(&hUsbDeviceFS, &Buf[0]);
	USBD_CDC_ReceivePacket(&hUsbDeviceFS);

	static uint8_t verify_error[2] = {0, 0};

	uint8_t msg[16] = {0};

	char *message_send[12] = {"USB -> Buzzer On\n", "USB -> Led On\n", "USB -> Buzzer Task 2 On\n", "USB -> Buzzer Task 2 Off\n",
			"USB -> Buzzer Task 3 On\n", "USB -> Led Task 2 On\n", "USB -> Led Task 2 Off\n",
			"USB -> Led Task 3 On\n", "USB -> LDR On\n", "USB -> Buzzer Off\n", "USB -> Led Off\n", "USB -> LDR Off\n"};

	char *comand_verified[12] = {"ON B", "ON L", "ON B2", "OFF B2", "ON B3", "ON L2",
								   "OFF L2", "ON L3", "ON LDR", "OFF B", "OFF L", "OFF LDR"};

	for(int i = 0; i < *Len; i++){
		msg[i] = Buf[i];
	}

	// Attempt to acquire the USB semaphore, non-blocking with a timeout of 1.
	if (osSemaphoreWait(mySemUSB_BlockHandle, 1) == osOK) {
	    osSemaphoreRelease(mySemUSB_BlockHandle);  // Release the semaphore immediately after acquiring it.

	    // Check if the length of the message (*Len) is less than 16.
	    if (*Len < 16) {
	        // Loop through a set of 12 pre-verified commands.
	        for (int i = 0; i < 12; i++) {
	            // Compare the received message 'msg' with a list of verified commands 'comand_verified'.
	            if (compare_buf(msg, comand_verified[i])) {

	                // If the command matches index 0 or 1, set the corresponding error flag (activate).
	                if (i == 0 || i == 1) {
	                    verify_error[i] = 1;
	                }
	                // If the command matches index 9 or 10, reset the corresponding error flag (deactivate).
	                else if (i == 9 || i == 10) {
	                    verify_error[i - 9] = 0;  // Set verify_error[0] or verify_error[1] to 0.
	                }
	                // For command indexes 2, 3, or 4, check if the buzzer is activated (verify_error[0] == 1).
	                else if ((i >= 2) && (i <= 4)) {
	                    if (verify_error[0] == 0) {  // If the buzzer is not activated:
	                        myPrintf(PERIPHERAL_USB, "Buzzer not activated\n");  // Print a message.
	                        break;  // Exit the loop early.
	                    }
	                }
	                // For command indexes 5, 6, or 7, check if the LED is activated (verify_error[1] == 1).
	                else if ((i >= 5) && (i <= 7)) {
	                    if (verify_error[1] == 0) {  // If the LED is not activated:
	                        myPrintf(PERIPHERAL_USB, "Led not activated\n");  // Print a message.
	                        break;  // Exit the loop early.
	                    }
	                }

	                // If a command is matched and passed the checks, send the message to Queue03.
	                osMessagePut(myQueue03Handle, (uint32_t)&msg, osWaitForever);
	                // Print the corresponding response message based on the command.
	                myPrintf(PERIPHERAL_USB, message_send[i]);
	                break;  // Exit the loop after finding a match.
	            }
	        }
	    } else {
	        // If the message length exceeds the limit, print a stack overflow error message.
	        myPrintf(PERIPHERAL_USB, "Erro: Stack Overflow\n");
	    }
	} else {
	    // If semaphore acquisition fails (USB is blocked), print an emergency activation error message.
	    myPrintf(PERIPHERAL_USB, "Erro: Emergency Activated\n");
	}


	return (USBD_OK);
  /* USER CODE END 6 */
}

/**
  * @brief  CDC_Transmit_FS
  *         Data to send over USB IN endpoint are sent over CDC interface
  *         through this function.
  *         @note
  *
  *
  * @param  Buf: Buffer of data to be sent
  * @param  Len: Number of data to be sent (in bytes)
  * @retval USBD_OK if all operations are OK else USBD_FAIL or USBD_BUSY
  */
uint8_t CDC_Transmit_FS(uint8_t* Buf, uint16_t Len)
{
  uint8_t result = USBD_OK;
  /* USER CODE BEGIN 7 */
  USBD_CDC_HandleTypeDef *hcdc = (USBD_CDC_HandleTypeDef*)hUsbDeviceFS.pClassData;
  if (hcdc->TxState != 0){
    return USBD_BUSY;
  }
  USBD_CDC_SetTxBuffer(&hUsbDeviceFS, Buf, Len);
  result = USBD_CDC_TransmitPacket(&hUsbDeviceFS);
  /* USER CODE END 7 */
  return result;
}

/**
  * @brief  CDC_TransmitCplt_FS
  *         Data transmitted callback
  *
  *         @note
  *         This function is IN transfer complete callback used to inform user that
  *         the submitted Data is successfully sent over USB.
  *
  * @param  Buf: Buffer of data to be received
  * @param  Len: Number of data received (in bytes)
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CDC_TransmitCplt_FS(uint8_t *Buf, uint32_t *Len, uint8_t epnum)
{
  uint8_t result = USBD_OK;
  /* USER CODE BEGIN 13 */
  UNUSED(Buf);
  UNUSED(Len);
  UNUSED(epnum);
  /* USER CODE END 13 */
  return result;
}

/* USER CODE BEGIN PRIVATE_FUNCTIONS_IMPLEMENTATION */
int compare_buf(uint8_t *msg, char *comparar){
	uint8_t i = 0;
	while(!((msg[i] == '\0' || msg[i] == '\n') && (comparar[i] == '\0' || comparar[i] == '\n'))){
		if(!(msg[i] == comparar[i] || msg[i] == (comparar[i] + 32))){
			return 0;
		}
		i++;
	}
	return 1;
}
/* USER CODE END PRIVATE_FUNCTIONS_IMPLEMENTATION */

/**
  * @}
  */

/**
  * @}
  */
