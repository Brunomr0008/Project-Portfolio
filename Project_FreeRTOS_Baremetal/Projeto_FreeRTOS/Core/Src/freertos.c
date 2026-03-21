/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "adc.h"
#include "gpio.h"
#include "LCD1602.h"
#include "Data_Variables.h"
#include "string.h"
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
int8_t button = 0; // variavel que indica quando o botao esta pressionado, 1 -> pressionado, 0 -> solto
float ldr_value = 0; // variavel que me guarda o valor do ldr convertido para 0 a 3,3 volts
int16_t frequencia_Buzzer = 500; // variavel que me altera a frequancia do toque intermitente do buzzer
int16_t frequencia_Led = 500; // variavel que me altera a frequancia do toque intermitente do buzzer

// Lista para armazenar os comandos ativos
char comandos_ativos[5][5];
uint8_t num_comandos_ativos = 0; // Quantidade de comandos ativos

/* USER CODE END Variables */
osThreadId myTask01_ButtonHandle;
osThreadId myTask02_BuzzerHandle;
osThreadId myTask03_LedHandle;
osThreadId myTask04_USBHandle;
osMessageQId myQueue01Handle;
osMessageQId myQueue02Handle;
osMessageQId myQueue03Handle;
osMessageQId myQueue04Handle;
osMessageQId myQueue05Handle;
osTimerId myTimerMultiAndLongCliquesHandle;
osTimerId myTimerToqueIntermitenteBuzzerHandle;
osTimerId myTimerToqueRapido3sBuzzerHandle;
osTimerId myTimerTask3_1sHandle;
osTimerId myTimerToqueIntermitenteLedHandle;
osTimerId myTimerToqueRapido3sLedHandle;
osTimerId myTimerYellow5hzHandle;
osSemaphoreId mySemMultiAndLongCliqueHandle;
osSemaphoreId mySemBuzzerHandle;
osSemaphoreId mySemLedHandle;
osSemaphoreId mySemBuzzerWaitHandle;
osSemaphoreId mySemLedWaitHandle;
osSemaphoreId mySemButtonBlockHandle;
osSemaphoreId mySemUSB_BlockHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void floatToString(float valor, char *str);
void LCD_methed(uint8_t i);
void adicionar_comando(char *comando);
void remover_comando(char *comando);
/* USER CODE END FunctionPrototypes */

void StartTask01_Button(void const * argument);
void StartTask02_Buzzer(void const * argument);
void StartTask03_Led(void const * argument);
void StartTask04_USB(void const * argument);
void CallbackMultiAndLongCliques(void const * argument);
void CallbackToqueIntermitenteBuzzer(void const * argument);
void CallbackToqueRapido3sBuzzer(void const * argument);
void CallbackTask3_1s(void const * argument);
void CallbackToqueIntermitenteLed(void const * argument);
void CallbackToqueRapido3sLed(void const * argument);
void CallbackYellow5hz(void const * argument);

extern void MX_USB_DEVICE_Init(void);
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* GetTimerTaskMemory prototype (linked to static allocation support) */
void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize );

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

/* USER CODE BEGIN GET_TIMER_TASK_MEMORY */
static StaticTask_t xTimerTaskTCBBuffer;
static StackType_t xTimerStack[configTIMER_TASK_STACK_DEPTH];

void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize )
{
  *ppxTimerTaskTCBBuffer = &xTimerTaskTCBBuffer;
  *ppxTimerTaskStackBuffer = &xTimerStack[0];
  *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
  /* place for user code */
}
/* USER CODE END GET_TIMER_TASK_MEMORY */

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

  /* Create the semaphores(s) */
  /* definition and creation of mySemMultiAndLongClique */
  osSemaphoreDef(mySemMultiAndLongClique);
  mySemMultiAndLongCliqueHandle = osSemaphoreCreate(osSemaphore(mySemMultiAndLongClique), 1);

  /* definition and creation of mySemBuzzer */
  osSemaphoreDef(mySemBuzzer);
  mySemBuzzerHandle = osSemaphoreCreate(osSemaphore(mySemBuzzer), 1);

  /* definition and creation of mySemLed */
  osSemaphoreDef(mySemLed);
  mySemLedHandle = osSemaphoreCreate(osSemaphore(mySemLed), 1);

  /* definition and creation of mySemBuzzerWait */
  osSemaphoreDef(mySemBuzzerWait);
  mySemBuzzerWaitHandle = osSemaphoreCreate(osSemaphore(mySemBuzzerWait), 1);

  /* definition and creation of mySemLedWait */
  osSemaphoreDef(mySemLedWait);
  mySemLedWaitHandle = osSemaphoreCreate(osSemaphore(mySemLedWait), 1);

  /* definition and creation of mySemButtonBlock */
  osSemaphoreDef(mySemButtonBlock);
  mySemButtonBlockHandle = osSemaphoreCreate(osSemaphore(mySemButtonBlock), 1);

  /* definition and creation of mySemUSB_Block */
  osSemaphoreDef(mySemUSB_Block);
  mySemUSB_BlockHandle = osSemaphoreCreate(osSemaphore(mySemUSB_Block), 1);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  osSemaphoreWait(mySemMultiAndLongCliqueHandle, 1);
  osSemaphoreWait(mySemBuzzerHandle, 1);
  osSemaphoreWait(mySemLedHandle, 1);
  /* USER CODE END RTOS_SEMAPHORES */

  /* Create the timer(s) */
  /* definition and creation of myTimerMultiAndLongCliques */
  osTimerDef(myTimerMultiAndLongCliques, CallbackMultiAndLongCliques);
  myTimerMultiAndLongCliquesHandle = osTimerCreate(osTimer(myTimerMultiAndLongCliques), osTimerOnce, NULL);

  /* definition and creation of myTimerToqueIntermitenteBuzzer */
  osTimerDef(myTimerToqueIntermitenteBuzzer, CallbackToqueIntermitenteBuzzer);
  myTimerToqueIntermitenteBuzzerHandle = osTimerCreate(osTimer(myTimerToqueIntermitenteBuzzer), osTimerPeriodic, NULL);

  /* definition and creation of myTimerToqueRapido3sBuzzer */
  osTimerDef(myTimerToqueRapido3sBuzzer, CallbackToqueRapido3sBuzzer);
  myTimerToqueRapido3sBuzzerHandle = osTimerCreate(osTimer(myTimerToqueRapido3sBuzzer), osTimerOnce, NULL);

  /* definition and creation of myTimerTask3_1s */
  osTimerDef(myTimerTask3_1s, CallbackTask3_1s);
  myTimerTask3_1sHandle = osTimerCreate(osTimer(myTimerTask3_1s), osTimerPeriodic, NULL);

  /* definition and creation of myTimerToqueIntermitenteLed */
  osTimerDef(myTimerToqueIntermitenteLed, CallbackToqueIntermitenteLed);
  myTimerToqueIntermitenteLedHandle = osTimerCreate(osTimer(myTimerToqueIntermitenteLed), osTimerPeriodic, NULL);

  /* definition and creation of myTimerToqueRapido3sLed */
  osTimerDef(myTimerToqueRapido3sLed, CallbackToqueRapido3sLed);
  myTimerToqueRapido3sLedHandle = osTimerCreate(osTimer(myTimerToqueRapido3sLed), osTimerOnce, NULL);

  /* definition and creation of myTimerYellow5hz */
  osTimerDef(myTimerYellow5hz, CallbackYellow5hz);
  myTimerYellow5hzHandle = osTimerCreate(osTimer(myTimerYellow5hz), osTimerPeriodic, NULL);

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* definition and creation of myQueue01 */
  osMessageQDef(myQueue01, 16, uint32_t);
  myQueue01Handle = osMessageCreate(osMessageQ(myQueue01), NULL);

  /* definition and creation of myQueue02 */
  osMessageQDef(myQueue02, 16, uint32_t);
  myQueue02Handle = osMessageCreate(osMessageQ(myQueue02), NULL);

  /* definition and creation of myQueue03 */
  osMessageQDef(myQueue03, 16, uint32_t);
  myQueue03Handle = osMessageCreate(osMessageQ(myQueue03), NULL);

  /* definition and creation of myQueue04 */
  osMessageQDef(myQueue04, 16, uint16_t);
  myQueue04Handle = osMessageCreate(osMessageQ(myQueue04), NULL);

  /* definition and creation of myQueue05 */
  osMessageQDef(myQueue05, 16, uint16_t);
  myQueue05Handle = osMessageCreate(osMessageQ(myQueue05), NULL);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of myTask01_Button */
  osThreadDef(myTask01_Button, StartTask01_Button, osPriorityNormal, 0, 128);
  myTask01_ButtonHandle = osThreadCreate(osThread(myTask01_Button), NULL);

  /* definition and creation of myTask02_Buzzer */
  osThreadDef(myTask02_Buzzer, StartTask02_Buzzer, osPriorityBelowNormal, 0, 128);
  myTask02_BuzzerHandle = osThreadCreate(osThread(myTask02_Buzzer), NULL);

  /* definition and creation of myTask03_Led */
  osThreadDef(myTask03_Led, StartTask03_Led, osPriorityBelowNormal, 0, 128);
  myTask03_LedHandle = osThreadCreate(osThread(myTask03_Led), NULL);

  /* definition and creation of myTask04_USB */
  osThreadDef(myTask04_USB, StartTask04_USB, osPriorityAboveNormal, 0, 128);
  myTask04_USBHandle = osThreadCreate(osThread(myTask04_USB), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartTask01_Button */
/**
  * @brief  Function implementing the myTask01_Button thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartTask01_Button */
void StartTask01_Button(void const * argument)
{
  /* init code for USB_DEVICE */
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN StartTask01_Button */
  static uint8_t n_button = 0;
  static uint8_t button_pressed = 0;
  static uint8_t Start_ToqueIntermitente = 0;
  static uint8_t toggle_action_2_button = 0;
  /* Infinite loop */
  for(;;){
	  // Verifica se o botão foi pressionado, utilizando o semáforo para controle
	  if(osSemaphoreWait(mySemButtonBlockHandle, 1) == osOK){
	      // Se o botão foi pressionado (valor 1)
	      if(button == 1){
	          button = 0; // Reseta o estado do botão
	          n_button++; // Incrementa o contador de cliques

	          // Se for o primeiro clique
	          if(n_button == 1){
	              button_pressed = 1; // Marca o botão como pressionado
	              // Inicia um timer de 1 segundo para múltiplos cliques
	              osTimerStart(myTimerMultiAndLongCliquesHandle, 1000);
	          }
	      // Verifica se o botão foi solto (valor -1)
	      }else if(button == -1){
	          button = 0; // Reseta o estado do botão
	          button_pressed = 0; // Marca o botão como não pressionado
	      }
	      // Libera o semáforo para permitir o uso por outros processos
	      osSemaphoreRelease(mySemButtonBlockHandle);
	  }

	  // Verifica se o semáforo liberado pelo Timer01 foi ativado (controle para múltiplos cliques)
	  if(osSemaphoreWait(mySemMultiAndLongCliqueHandle, 10) == osOK){
	      // Se o botão já foi solto
	      if(button_pressed == 0){
	          // Se a ação de toque intermitente não estiver ativa
	          if(Start_ToqueIntermitente == 0){
	              // Executa a ação correspondente ao número de cliques

	              // Se houve 1 clique
	              if(n_button == 1){
	                  Start_ToqueIntermitente = 1; // Marca que o toque intermitente foi iniciado
	                  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET); // Liga o GPIO correspondente

	                  // Envia mensagens para ativar o buzzer e a luz (L)
	                  osMessagePut(myQueue01Handle, (uint32_t)"ON B", 10);
	                  osMessagePut(myQueue02Handle, (uint32_t)"ON L", 10);

	                  // Adiciona os comandos "B" e "L" na lista de comandos ativos
	                  adicionar_comando("B");
	                  adicionar_comando("L");
	              }
	          }else{
	              // Se houver 2 cliques
	              if(n_button == 2){
	                  HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_1); // Alterna o estado do GPIO (liga/desliga)

	                  // Verifica se a ação já está ativada
	                  if(toggle_action_2_button ^= 1){
	                      // Se não estiver ativada, envia mensagens para ligar o buzzer e a luz (B2, L2)
	                      osMessagePut(myQueue01Handle, (uint32_t)"ON B2", 10);
	                      osMessagePut(myQueue02Handle, (uint32_t)"ON L2", 10);
	                      adicionar_comando("B2");
	                      adicionar_comando("L2");
	                  }else{
	                      // Se já estiver ativada, envia mensagens para desligar o buzzer e a luz (B2, L2)
	                      osMessagePut(myQueue01Handle, (uint32_t)"OFF B2", 10);
	                      osMessagePut(myQueue02Handle, (uint32_t)"OFF L2", 10);
	                      remover_comando("B2");
	                      remover_comando("L2");
	                  }
	              // Se houver 3 cliques
	              }else if(n_button == 3){
	                  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET); // Liga o GPIO correspondente

	                  // Envia mensagens para ligar o buzzer e a luz (B3, L3)
	                  osMessagePut(myQueue01Handle, (uint32_t)"ON B3", 10);
	                  osMessagePut(myQueue02Handle, (uint32_t)"ON L3", 10);
	              }
	          }
	      }else{
	          // Se o botão ainda estiver pressionado, desliga o toque intermitente e reseta as variáveis de controle
	          Start_ToqueIntermitente = 0;
	          toggle_action_2_button = 0;

	          // Envia mensagens para desligar o buzzer e a luz (B, L)
	          osMessagePut(myQueue01Handle, (uint32_t)"OFF B", 10);
	          osMessagePut(myQueue02Handle, (uint32_t)"OFF L", 10);

	          // Remove os comandos da lista de comandos ativos
	          remover_comando("B");
	          remover_comando("L");
	          remover_comando("B2");
	          remover_comando("L2");

	          // Desliga os GPIOs correspondentes
	          HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
	          HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, GPIO_PIN_RESET);
	          HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
	      }

	      // Reseta o contador de cliques
	      n_button = 0;
	  }


	osDelay(30); // Pequeno atraso para evitar uso excessivo de CPU
  }
  /* USER CODE END StartTask01_Button */
}

/* USER CODE BEGIN Header_StartTask02_Buzzer */
/**
* @brief Function implementing the myTask02_Buzzer thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask02_Buzzer */
void StartTask02_Buzzer(void const * argument)
{
  /* USER CODE BEGIN StartTask02_Buzzer */
	osEvent evt;

	static int8_t invert = 1;
	static uint8_t i = 0;
	static int8_t action_2_buzzer = 0;

  /* Infinite loop */
  for(;;){
	  // Obtém mensagem da fila myQueue01Handle, com tempo de espera de 10ms
	  evt = osMessageGet(myQueue01Handle, 10);
	  if(evt.status == osEventMessage){
	      uint8_t *val = (uint8_t *)evt.value.p;

	      // Verifica qual mensagem foi recebida e executa a ação correspondente
	      if(compare_buf(val, "ON B")){ // (A) Toque intermitente de 0,5 segundos, repetido indefinidamente (1ª ação)
	          frequencia_Buzzer = 500; // Define a frequência do buzzer para 0,5 segundos
	          osTimerStart(myTimerToqueIntermitenteBuzzerHandle, frequencia_Buzzer); // Inicia o timer para toque intermitente

	      }else if(compare_buf(val, "ON B2")){ // Aumenta ou diminui a frequência do toque intermitente (toque mais rápido)
	          action_2_buzzer = 1; // Marca que a ação 2 do buzzer está ativa

	      }else if(compare_buf(val, "ON B3")){ // Toque rápido de 0,15 segundos por 3 segundos
	          if(action_2_buzzer == 1){
	              osSemaphoreWait(mySemBuzzerWaitHandle, 1); // Espera o semáforo relacionado à ação 2 do buzzer
	          }

	          osTimerStart(myTimerToqueIntermitenteBuzzerHandle, 150); // Inicia o timer para toque rápido de 0,15 segundos
	          osTimerStart(myTimerToqueRapido3sBuzzerHandle, 3000); // Inicia o timer para desligar o toque rápido após 3 segundos

	      }else if(compare_buf(val, "OFF B")){ // Desliga o toque
	          action_2_buzzer = 0; // Reseta a ação 2
	          HAL_GPIO_WritePin(GPIOG, GPIO_PIN_2, GPIO_PIN_RESET); // Desliga o buzzer (GPIO)
	          osTimerStop(myTimerToqueIntermitenteBuzzerHandle); // Para o timer do toque intermitente

	      }else if(compare_buf(val, "OFF B2")){ // Desliga a ação 2 do buzzer
	          action_2_buzzer = 0; // Desativa a ação 2

	      }else if(compare_buf(val, "ON BU")){ // Situação de emergência, buzzer ligado
	          HAL_GPIO_WritePin(GPIOG, GPIO_PIN_2, GPIO_PIN_SET); // Liga o buzzer (GPIO)

	      }else if(compare_buf(val, "OFF BU")){ // Situação de emergência, buzzer desligado
	          HAL_GPIO_WritePin(GPIOG, GPIO_PIN_2, GPIO_PIN_RESET); // Desliga o buzzer (GPIO)
	      }
	  }

	  // Verifica se a ação 2 do buzzer está ativa
	  if(action_2_buzzer == 1){
	      // Espera o semáforo do buzzer ser liberado
	      if(osSemaphoreWait(mySemBuzzerHandle, 1) == osOK){
	          if(osSemaphoreWait(mySemBuzzerWaitHandle, 1) == osOK){
	              osSemaphoreRelease(mySemBuzzerWaitHandle); // Libera o semáforo

	              // Alterna a ação (controla a alternância entre as frequências)
	              if(i ^= 1){
	                  // Se a frequência estiver muito alta (<= 60), inverte a direção para diminuir a frequência
	                  if(frequencia_Buzzer <= 60){
	                      invert = 1; // Inverte a ação para aumentar a frequência
	                  }else if(frequencia_Buzzer >= 760){ // Se a frequência estiver muito baixa (>= 760)
	                      invert = -1; // Inverte a ação para diminuir a frequência
	                  }

	                  // Ajusta a frequência do buzzer de acordo com a direção (aumentando ou diminuindo)
	                  frequencia_Buzzer = frequencia_Buzzer + 20 * invert;
	                  osTimerStart(myTimerToqueIntermitenteBuzzerHandle, frequencia_Buzzer); // Reinicia o timer com a nova frequência
	              }
	          }
	      }
	  }


      osDelay(50);// Pequeno atraso para evitar uso excessivo de CPU
  }
  /* USER CODE END StartTask02_Buzzer */
}

/* USER CODE BEGIN Header_StartTask03_Led */
/**
* @brief Function implementing the myTask03_Led thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask03_Led */
void StartTask03_Led(void const * argument)
{
  /* USER CODE BEGIN StartTask03_Led */
	osEvent evt;

	static int8_t invert = 1;
	static uint8_t i = 0;
	static int8_t action_2_led = 0;

  /* Infinite loop */
  for(;;){
	  // Obtém mensagem da fila myQueue02Handle
	  evt = osMessageGet(myQueue02Handle, 10);
	  if(evt.status == osEventMessage){
	      uint8_t *val = (uint8_t *)evt.value.p;

	      // Verifica o comando recebido e executa a ação correspondente
	      if(compare_buf(val, "ON L")){  // Aciona LED intermitente com frequência de 500ms
	          frequencia_Led = 500;
	          osTimerStart(myTimerToqueIntermitenteLedHandle, frequencia_Led);
	          osTimerStart(myTimerTask3_1sHandle, 1000);  // Timer adicional de 1 segundo

	      }else if(compare_buf(val, "ON L2")){  // Ativa o modo de ação 2 do LED
	          action_2_led = 1;

	      }else if(compare_buf(val, "ON L3")){  // Aciona o LED rápido durante 3 segundos
	          if(action_2_led == 1){  // Espera semáforo se ação 2 estiver ativa
	              osSemaphoreWait(mySemLedWaitHandle, 1);
	          }

	          // Aciona o LED intermitente rápido (150ms) por 3 segundos
	          osTimerStart(myTimerToqueIntermitenteLedHandle, 150);
	          osTimerStart(myTimerToqueRapido3sLedHandle, 3000);

	      }else if(compare_buf(val, "OFF L")){  // Desativa o LED
	          action_2_led = 0;
	          osTimerStop(myTimerToqueIntermitenteLedHandle);  // Para o LED intermitente
	          osTimerStop(myTimerTask3_1sHandle);  // Para o timer
	          HAL_GPIO_WritePin(GPIOC, Green_Pin, GPIO_PIN_RESET);  // Desativa LEDs verde e amarelo
	          HAL_GPIO_WritePin(GPIOC, Yellow_Pin, GPIO_PIN_RESET);

	      }else if(compare_buf(val, "OFF L2")){  // Desativa ação 2 do LED
	          action_2_led = 0;

	      }else if(compare_buf(val, "ON LED")){  // Aciona LED vermelho em situação de emergência
	          HAL_GPIO_WritePin(GPIOC, Red_Pin, GPIO_PIN_SET);

	      }else if(compare_buf(val, "OFF LED")){  // Desativa LED vermelho em situação de emergência
	          HAL_GPIO_WritePin(GPIOC, Red_Pin, GPIO_PIN_RESET);
	      }
	  }

	  // Se ação 2 do LED estiver ativa, ajusta a frequência intermitente
	  if(action_2_led == 1){
	      if(osSemaphoreWait(mySemLedHandle, 1) == osOK){
	          if(osSemaphoreWait(mySemLedWaitHandle, 1) == osOK){
	              osSemaphoreRelease(mySemLedWaitHandle);

	              // Alterna o estado do LED intermitente
	              if(i ^= 1){
	                  // Ajusta a frequência: se estiver baixa (<60ms), aumenta, se alta (>760ms), diminui
	                  if(frequencia_Led <= 60){
	                      invert = 1;  // Aumenta a frequência (diminui o intervalo)
	                  }else if (frequencia_Led >= 760){
	                      invert = -1;  // Diminui a frequência (aumenta o intervalo)
	                  }

	                  // Ajusta a frequência do LED intermitente
	                  frequencia_Led = frequencia_Led + 20 * invert;
	                  osTimerStart(myTimerToqueIntermitenteLedHandle, frequencia_Led);
	              }
	          }
	      }
	  }

	  osDelay(50); // Pequeno atraso para evitar uso excessivo de CPU
  }
  /* USER CODE END StartTask03_Led */
}

/* USER CODE BEGIN Header_StartTask04_USB */
/**
* @brief Function implementing the myTask04_USB thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask04_USB */
void StartTask04_USB(void const * argument)
{
  /* USER CODE BEGIN StartTask04_USB */
	osEvent evt;

	static uint8_t start_ldr = 0;
	static uint8_t alarme = 0;
	static char str[5];

	static uint8_t display_leitura = 0;
	static uint8_t pisca = 0;

	char *command[10] = {"ON B", "OFF B", "ON B2", "OFF B2", "ON B3", "ON L", "OFF L", "ON L2", "OFF L2", "ON L3"};
	char *add_command[10] = {"B", "B", "B2", "B2", "", "L", "L", "L2", "L2", ""};

	LCD_methed(0); // LCD on Start
  /* Infinite loop */
	for(;;){
		// Obtém uma mensagem da fila myQueue03Handle
		evt = osMessageGet(myQueue03Handle, 10);
		if(evt.status == osEventMessage){
		    uint8_t *val = (uint8_t *)evt.value.p;

		    // Verifica se o valor recebido corresponde a um comando na lista de 'command'
		    for(int h = 0; h < 10; h++){
		        if(compare_buf(val, command[h])){  // Compara o valor recebido com o comando
		            if((h >= 0) && (h <= 4)){  // Se o comando está nos primeiros 5 índices, envia para fila myQueue01Handle
		                osMessagePut(myQueue01Handle, (uint32_t)command[h], 10);
		            }else{  // Se está nos próximos 5, envia para fila myQueue02Handle
		                osMessagePut(myQueue02Handle, (uint32_t)command[h], 10);
		            }

		            // Adiciona ou remove comandos dependendo do índice
		            if((h == 0) || (h == 2) || (h == 5) || (h == 7)){
		                adicionar_comando(add_command[h]);  // Adiciona comando
		            }else if((h == 1) || (h == 3) || (h == 6) || (h == 8)){
		                remover_comando(add_command[h]);  // Remove comando
		            }
		            break;
		        }
		    }

		    // Verifica comandos específicos "ON LDR" e "OFF LDR"
		    if(compare_buf(val, "ON LDR")){  // Inicia o sensor LDR
		        if(start_ldr == 0){  // Se o LDR não foi iniciado ainda
		            HAL_ADC_Start_IT(&hadc3);  // Inicia o ADC para leitura do LDR

		            start_ldr = 1;  // Marca que o LDR foi iniciado
		            LCD_methed(1);  // Atualiza o LCD para mostrar o status do LDR
		        }

		    }else if(compare_buf(val, "OFF LDR")){  // Para o sensor LDR
		        HAL_ADC_Stop_IT(&hadc3);  // Para a leitura do LDR (ADC)

		        ldr_value = 0;  // Zera o valor do LDR
		        start_ldr = 0;  // Marca que o LDR foi desativado
		        osMessagePut(myQueue01Handle, (uint32_t)"OFF BU", 10);  // Envia comando para desligar o buzzer
		        osMessagePut(myQueue02Handle, (uint32_t)"OFF LED", 10);  // Envia comando para desligar o LED

		        // Remove os comandos relacionados ao Buzzer e LED
		        remover_comando("B");
		        remover_comando("B2");
		        remover_comando("L");
		        remover_comando("L2");

		        LCD_methed(0);  // Atualiza o LCD para o estado inicial
		    }
		}


		if (start_ldr) {  // Check if the LDR process is enabled (likely a flag).
		    if (display_leitura++ == 10) {  // Increment the 'display_leitura' counter, and when it reaches 10:
		        display_leitura = 0;  // Reset the 'display_leitura' counter to 0.

		        // Toggle the 'pisca' variable (blinking flag). If 'pisca' is 1 (true):
		        if (pisca ^= 1) {
		            lcd_put_cur(1, 15);  // Move the cursor to row 1, column 15 on the LCD.
		            lcd_send_string("!");  // Display an exclamation mark "!".
		        } else {  // If 'pisca' is 0 (false):
		            lcd_put_cur(1, 15);  // Move the cursor to row 1, column 15 on the LCD.
		            lcd_send_string(".");  // Display a dot ".".
		        }

		        // Update the LCD display with the LDR sensor value.
		        lcd_put_cur(0, 12);  // Move the cursor to row 0, column 12 on the LCD.
		        floatToString(ldr_value, str);  // Convert the 'ldr_value' (a float) to a string.
		        lcd_send_string(str);  // Display the LDR value as a string on the LCD.
		    }

		    // Check if LDR value exceeds 2.5 and the alarm is not yet active.
		    if (ldr_value > 2.5 && !alarme) {
		        // Turn off specific components by sending messages to the queues.
		        osMessagePut(myQueue01Handle, (uint32_t)"OFF B", 10);  // Send "OFF B" message to Queue01.
		        osMessagePut(myQueue02Handle, (uint32_t)"OFF L", 10);  // Send "OFF L" message to Queue02.

		        // Activate the buzzer and LED by sending messages to the queues.
		        osMessagePut(myQueue01Handle, (uint32_t)"ON BU", 10);  // Send "ON BU" to turn on the buzzer.
		        osMessagePut(myQueue02Handle, (uint32_t)"ON LED", 10);  // Send "ON LED" to turn on the LED.

		        // Acquire semaphores to block certain actions (likely USB or button press actions).
		        osSemaphoreWait(mySemButtonBlockHandle, 1);  // Wait on the button block semaphore.
		        osSemaphoreWait(mySemUSB_BlockHandle, 1);  // Wait on the USB block semaphore.

		        alarme = 1;  // Set the 'alarme' flag to indicate that the alarm is active.

		    // If the LDR value drops below 2.2 and the alarm is active, deactivate the alarm.
		    } else if (ldr_value < 2.2 && alarme) {
		        // Turn off the buzzer and LED by sending messages to the queues.
		        osMessagePut(myQueue01Handle, (uint32_t)"OFF BU", 10);  // Send "OFF BU" to turn off the buzzer.
		        osMessagePut(myQueue02Handle, (uint32_t)"OFF LED", 10);  // Send "OFF LED" to turn off the LED.

		        // Release the semaphores to unblock actions.
		        osSemaphoreRelease(mySemButtonBlockHandle);  // Release the button block semaphore.
		        osSemaphoreRelease(mySemUSB_BlockHandle);  // Release the USB block semaphore.

		        // Reactivate commands that were previously active.
		        for (int i = 0; i < num_comandos_ativos; i++) {  // Iterate through active commands.
		            if (strcmp(comandos_ativos[i], "B") == 0) {
		                osMessagePut(myQueue01Handle, (uint32_t)"ON B", 10);  // Send "ON B" to Queue01.
		            } else if (strcmp(comandos_ativos[i], "B2") == 0) {
		                osMessagePut(myQueue01Handle, (uint32_t)"ON B2", 10);  // Send "ON B2" to Queue01.
		            } else if (strcmp(comandos_ativos[i], "L") == 0) {
		                osMessagePut(myQueue02Handle, (uint32_t)"ON L", 10);  // Send "ON L" to Queue02.
		            } else if (strcmp(comandos_ativos[i], "L2") == 0) {
		                osMessagePut(myQueue02Handle, (uint32_t)"ON L2", 10);  // Send "ON L2" to Queue02.
		            }
		        }

		        alarme = 0;  // Reset the 'alarme' flag, indicating the alarm is no longer active.
		    }
		}

		osDelay(50);
	}
  /* USER CODE END StartTask04_USB */
}

/* CallbackMultiAndLongCliques function */
void CallbackMultiAndLongCliques(void const * argument)
{
  /* USER CODE BEGIN CallbackMultiAndLongCliques */
	osSemaphoreRelease(mySemMultiAndLongCliqueHandle);
  /* USER CODE END CallbackMultiAndLongCliques */
}

/* CallbackToqueIntermitenteBuzzer function */
void CallbackToqueIntermitenteBuzzer(void const * argument)
{
  /* USER CODE BEGIN CallbackToqueIntermitenteBuzzer */
	HAL_GPIO_TogglePin(GPIOG, GPIO_PIN_2);
	osSemaphoreRelease(mySemBuzzerHandle);
  /* USER CODE END CallbackToqueIntermitenteBuzzer */
}

/* CallbackToqueRapido3sBuzzer function */
void CallbackToqueRapido3sBuzzer(void const * argument)
{
  /* USER CODE BEGIN CallbackToqueRapido3sBuzzer */
	osSemaphoreRelease(mySemBuzzerWaitHandle);
	osTimerStart(myTimerToqueIntermitenteBuzzerHandle, frequencia_Buzzer);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
  /* USER CODE END CallbackToqueRapido3sBuzzer */
}

/* CallbackTask3_1s function */
void CallbackTask3_1s(void const * argument)
{
  /* USER CODE BEGIN CallbackTask3_1s */
	static uint8_t change_3s = 0;

	if(change_3s++ == 2){
		change_3s = 0;
		osTimerStart(myTimerYellow5hzHandle, 100);
		HAL_GPIO_WritePin(GPIOC, Yellow_Pin, GPIO_PIN_SET);
	}
  /* USER CODE END CallbackTask3_1s */
}

/* CallbackToqueIntermitenteLed function */
void CallbackToqueIntermitenteLed(void const * argument)
{
  /* USER CODE BEGIN CallbackToqueIntermitenteLed */
	HAL_GPIO_TogglePin(GPIOC, Green_Pin);
	osSemaphoreRelease(mySemLedHandle);
  /* USER CODE END CallbackToqueIntermitenteLed */
}

/* CallbackToqueRapido3sLed function */
void CallbackToqueRapido3sLed(void const * argument)
{
  /* USER CODE BEGIN CallbackToqueRapido3sLed */
	osSemaphoreRelease(mySemLedWaitHandle);
	osTimerStart(myTimerToqueIntermitenteLedHandle, frequencia_Led);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
  /* USER CODE END CallbackToqueRapido3sLed */
}

/* CallbackYellow5hz function */
void CallbackYellow5hz(void const * argument)
{
  /* USER CODE BEGIN CallbackYellow5hz */
	static uint8_t change_5hz = 0;

	HAL_GPIO_TogglePin(GPIOC, Yellow_Pin);

	if(change_5hz++ >= 9){
		change_5hz = 0;
		osTimerStop(myTimerYellow5hzHandle);
		HAL_GPIO_WritePin(GPIOC, Yellow_Pin, GPIO_PIN_RESET);
	}
  /* USER CODE END CallbackYellow5hz */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void floatToString(float valor, char *str) {
    // Parte inteira do valor
    int parteInteira = (int)valor;
    // Calcula a parte fracionária
    int parteFracionaria = (int)((valor - parteInteira) * 100);

    // Converte a parte inteira para string
    int indice = 0;
    str[indice++] = parteInteira + '0';

    // Adiciona o ponto decimal
    str[indice++] = '.';

    // Converte a parte fracionária para string
    str[indice++] = ((parteFracionaria / 10) % 10) + '0';
    str[indice++] = (parteFracionaria % 10) + '0';

    // Finaliza a string
    str[indice] = '\0';
}

void LCD_methed(uint8_t i){
	lcd_clear();

	if(i){
		lcd_put_cur(0,0);
		lcd_send_string("LDR Value : ");
		lcd_put_cur(1,0);
		lcd_send_string("SE -> FreeRTOS");
	}else{
		lcd_put_cur(0,6);
		lcd_send_string("OFF!");
		lcd_put_cur(1,0);
		lcd_send_string("MEEC-> SETR-> G3");
	}
}

// Função para adicionar um comando à lista de comandos ativos
void adicionar_comando(char *comando) {
    // Verificar se o comando já está ativo
    for (int i = 0; i < num_comandos_ativos; i++) {
        if (strcmp(comandos_ativos[i], comando) == 0) {
            // Se o comando já está ativo, não adiciona novamente
            return;
        }
    }

    // Se o comando não está ativo, adiciona-o à lista
    if (num_comandos_ativos < 10) {
        strcpy(comandos_ativos[num_comandos_ativos], comando);
        num_comandos_ativos++;
    }
}

// Função para remover um comando da lista de comandos ativos
void remover_comando(char *comando) {
    for (int i = 0; i < num_comandos_ativos; i++) {
        if (strcmp(comandos_ativos[i], comando) == 0) {
            // Mover os comandos para preencher o espaço do comando removido
            for (int j = i; j < num_comandos_ativos - 1; j++) {
                strcpy(comandos_ativos[j], comandos_ativos[j + 1]);
            }
            num_comandos_ativos--;
            break;
        }
    }
}
/* USER CODE END Application */
