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
#include "adc.h"
#include "memorymap.h"
#include "tim.h"
#include "usb_device.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "LCD1602.h"
#include "Data_Variables.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define CLICK_TIMEOUT 1000  // Tempo máximo entre cliques (1s)
#define TIMEOUT_ACTION3 3000  // Tempo máximo entre cliques (3s)
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
int8_t button = 0;              // Estado do botão: 1 = pressionado, -1 = solto

uint8_t flag_bloqueado = 0;
char command_send[16] = {0};

uint8_t flag_callback_timer6 = 0;
uint8_t flag_callback_timer7 = 0;

uint16_t frequencia_Buzzer = 0;        // Período do timer (em ms) para buzzer/LED
uint16_t frequencia_Led = 0;

uint8_t action_2_buzzer = 0;
uint8_t action_2_led = 0;

uint32_t time_action3_buzzer = 0;
uint32_t time_action3_led = 0;
uint32_t time_yellow_5Hz = 0;

float ldr_value = 0;

char comandos_ativos[5][5];  // Até 5 comandos, cada um com até 5 caracteres
volatile uint8_t num_comandos_ativos = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void TaskButton(void);
void TaskBuzzer(char *command);
void Action2_TaskBuzzer(void);
void TaskLed(char *command);
void Action2_TaskLed(void);
void TaskUSB(void);
void Action3(void);
void Yellow_5Hz(void);

void Float_To_String(float valor, char *str);
void LCD_Methed(uint8_t i);
void Add_Comand(char *comando);
void Remove_Comand(char *comando);
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
  MX_USB_DEVICE_Init();
  MX_ADC3_Init();
  MX_TIM1_Init();
  MX_TIM5_Init();
  MX_TIM6_Init();
  MX_TIM7_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start(&htim1);
  lcd_init();
  LCD_Methed(0); // LCD on Start
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  TaskButton();
	  TaskUSB();

	  Yellow_5Hz();

	  if(flag_callback_timer7){
		  flag_callback_timer7 = 0,
		  Action2_TaskBuzzer();
	  }

	  if(flag_callback_timer6){
		  flag_callback_timer6 = 0;
		  Action2_TaskLed();
	  }

	  Action3();
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

  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 42;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_3;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void TaskButton(void){
	static uint32_t button_press_time = 0; // Armazena o tempo em que o botão foi pressionado.
	static uint8_t Start_ToqueIntermitente = 0; // Indica se o toque intermitente foi iniciado.
	static uint8_t toggle_action_2_button = 0; // Controla o estado de ações com dois cliques.
	static uint8_t button_pressed = 0; // Indica se o botão está pressionado.
	static uint8_t n_button = 0; // Conta o número de cliques.

	// Se o sistema não está bloqueado:
	if (!flag_bloqueado) {
	    if (button == 1) { // Quando o botão é pressionado:
	        button = 0; // Reseta o estado do botão.
	        n_button++; // Incrementa o número de cliques.
	        if (n_button == 1) { // Ao pressionar pela primeira vez:
	            button_pressed = 1; // Indica que o botão foi pressionado.
	            button_press_time = HAL_GetTick(); // Armazena o tempo da primeira pressão.
	        }
	    } else if (button == -1) { // Quando o botão é solto:
	        button = 0; // Reseta o estado do botão.
	        button_pressed = 0; // Marca que o botão foi solto.
	    }
	}

	// Verifica se passou 1 segundo (tempo de clique longo):
	if ((HAL_GetTick() - button_press_time >= CLICK_TIMEOUT) && button_press_time != 0) {
	    if (button_pressed == 0) { // Se o botão não está mais pressionado:
	        if (Start_ToqueIntermitente == 0) { // Se o toque intermitente não começou:
	            if (n_button == 1) { // Clique simples:
	                Start_ToqueIntermitente = 1; // Inicia toque intermitente.
	                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET); // Liga um pino GPIO.
	                TaskBuzzer("ON B"); // Liga o buzzer.
	                TaskLed("ON L"); // Liga o LED.
	                Add_Comand("B"); // Adiciona o comando "B".
	                Add_Comand("L"); // Adiciona o comando "L".
	            }
	        } else {
	            if (n_button == 2) { // Clique duplo:
	                HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_1); // Alterna o estado de um pino GPIO.
	                if (toggle_action_2_button ^= 1) {
	                    TaskBuzzer("ON B2"); // Liga o segundo buzzer.
	                    TaskLed("ON L2"); // Liga o segundo LED.
	                    Add_Comand("B2"); // Adiciona o comando "B2".
	                    Add_Comand("L2"); // Adiciona o comando "L2".
	                } else {
	                    TaskBuzzer("OFF B2"); // Desliga o segundo buzzer.
	                    TaskLed("OFF L2"); // Desliga o segundo LED.
	                    Remove_Comand("B2"); // Remove o comando "B2".
	                    Remove_Comand("L2"); // Remove o comando "L2".
	                }
	            } else if (n_button == 3) { // Clique triplo:
	                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET); // Liga outro pino GPIO.
	                TaskBuzzer("ON B3"); // Liga o terceiro buzzer.
	                TaskLed("ON L3"); // Liga o terceiro LED.
	            }
	        }
	    } else { // Se o botão ainda estiver pressionado:
	        // Reseta o sistema e desliga tudo:
	        Start_ToqueIntermitente = 0;
	        toggle_action_2_button = 0;
	        TaskBuzzer("OFF B");
	        TaskLed("OFF L");
	        Remove_Comand("B");
	        Remove_Comand("L");
	        Remove_Comand("B2");
	        Remove_Comand("L2");
	        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
	        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, GPIO_PIN_RESET);
	        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
	    }
	    // Reseta o tempo de pressão e a contagem de cliques após processar o evento.
	    button_press_time = 0;
	    n_button = 0;
	}
}

void TaskBuzzer(char *command) {
    if (compare_buf(command, "ON B")) { // Ativa toque intermitente (0,5s)
        frequencia_Buzzer = 500; // Define a frequência do som.
        __HAL_TIM_SET_AUTORELOAD(&htim7, frequencia_Buzzer * 10); // Configura o timer para 500ms.
		__HAL_TIM_SET_COUNTER(&htim7, 0); // Reinicia o contador.
        HAL_TIM_Base_Start_IT(&htim7); // Inicia o timer com interrupção.

    } else if (compare_buf(command, "ON B2")) { // Ativa a ação 2 do buzzer (ajuste de frequência)
    	action_2_buzzer = 1; // Define a variável para executar a lógica de ajuste de frequência.

    } else if (compare_buf(command, "ON B3")) { // Toque rápido de 3s
    	if (action_2_buzzer == 1) {
    		action_2_buzzer = 2; // Coloca o buzzer em modo de espera (aguarda para não interromper).
    	}

    	time_action3_buzzer = HAL_GetTick(); // Armazena o tempo atual (usado para controlar a duração do som).
        __HAL_TIM_SET_AUTORELOAD(&htim7, 150 * 10); // Configura o timer para toque rápido de 150ms.
		__HAL_TIM_SET_COUNTER(&htim7, 0); // Reinicia o contador.
        HAL_TIM_Base_Start_IT(&htim7); // Inicia o timer com interrupção.

    } else if (compare_buf(command, "OFF B")) { // Desativa o buzzer
    	action_2_buzzer = 0; // Reseta a ação 2.
        HAL_GPIO_WritePin(GPIOG, GPIO_PIN_2, GPIO_PIN_RESET); // Desliga o GPIO responsável pelo buzzer.
        HAL_TIM_Base_Stop_IT(&htim7); // Para o timer do buzzer.

    } else if (compare_buf(command, "OFF B2")) { // Desativa a ação 2 do buzzer
    	action_2_buzzer = 0; // Reseta o modo da ação 2.

    } else if (compare_buf(command, "ON BU")) { // Emergência: buzzer ativo
        HAL_GPIO_WritePin(GPIOG, GPIO_PIN_2, GPIO_PIN_SET); // Liga o buzzer diretamente (modo emergência).

    } else if (compare_buf(command, "OFF BU")) { // Emergência: buzzer desativado
        HAL_GPIO_WritePin(GPIOG, GPIO_PIN_2, GPIO_PIN_RESET); // Desliga o buzzer em modo de emergência.
    }
}

void Action2_TaskBuzzer(void) {
	static int8_t invert = 1; // Controla o sentido do ajuste de frequência (aumentar/diminuir).
	static uint8_t toggle = 0; // Alterna o estado a cada chamada.

	if (action_2_buzzer == 1) { // Se a ação 2 estiver ativa:
		if (toggle ^= 1) { // Alterna o valor de toggle (muda o estado a cada chamada).
			if (frequencia_Buzzer <= 60) { // Se a frequência for muito alta, inverte o sentido (aumenta).
				invert = 1;
			} else if (frequencia_Buzzer >= 760) { // Se a frequência for muito baixa, inverte o sentido (diminui).
				invert = -1;
			}

			// Ajusta a frequência do buzzer aumentando ou diminuindo 20 unidades.
			frequencia_Buzzer = frequencia_Buzzer + 20 * invert;
			__HAL_TIM_SET_AUTORELOAD(&htim7, frequencia_Buzzer * 10); // Atualiza o timer com a nova frequência.
			__HAL_TIM_SET_COUNTER(&htim7, 0); // Reinicia o contador do timer.
			HAL_TIM_Base_Start_IT(&htim7); // Reinicia o timer.
		}
	}
}

void TaskLed(char *command) {
    if (compare_buf(command, "ON L")) { // Ativa o LED com frequência de 500ms
        frequencia_Led = 500; // Define a frequência do LED em 500ms
        __HAL_TIM_SET_AUTORELOAD(&htim6, frequencia_Led * 10); // Configura o timer 6 para 500ms
        __HAL_TIM_SET_COUNTER(&htim6, 0); // Reinicia o contador
        HAL_TIM_Base_Start_IT(&htim6); // Inicia o timer 6 com interrupção

        __HAL_TIM_SET_AUTORELOAD(&htim5, 1000 * 10); // Configura o timer 5 para 1000ms (1 segundo)
        __HAL_TIM_SET_COUNTER(&htim5, 0); // Reinicia o contador
        HAL_TIM_Base_Start_IT(&htim5); // Inicia o timer 5 com interrupção

    } else if (compare_buf(command, "ON L2")) { // Ativa a ação 2 do LED (ajuste de frequência)
        action_2_led = 1; // Sinaliza para iniciar o ajuste de frequência do LED

    } else if (compare_buf(command, "ON L3")) { // Toque rápido de LED
        if(action_2_led == 1){ // Se a ação 2 está ativa, coloca o LED em modo de espera
            action_2_led = 2; // Modo de espera
        }

        time_action3_led = HAL_GetTick(); // Armazena o tempo atual
        __HAL_TIM_SET_AUTORELOAD(&htim6, 150 * 10); // Configura o timer 6 para 150ms
        __HAL_TIM_SET_COUNTER(&htim6, 0); // Reinicia o contador
        HAL_TIM_Base_Start_IT(&htim6); // Inicia o timer 6 com interrupção

    } else if (compare_buf(command, "OFF L")) { // Desativa o LED
        action_2_led = 0; // Reseta a ação 2
        HAL_TIM_Base_Stop_IT(&htim5); // Para o timer 5
        HAL_TIM_Base_Stop_IT(&htim6); // Para o timer 6
        HAL_GPIO_WritePin(GPIOC, Green_Pin, GPIO_PIN_RESET); // Desliga o LED verde
        HAL_GPIO_WritePin(GPIOC, Yellow_Pin, GPIO_PIN_RESET); // Desliga o LED amarelo

    } else if (compare_buf(command, "OFF L2")) { // Desativa a ação 2 do LED
        action_2_led = 0; // Reseta a ação 2

    } else if (compare_buf(command, "ON LED")) { // Emergência: Ativa o LED vermelho
        HAL_GPIO_WritePin(GPIOC, Red_Pin, GPIO_PIN_SET); // Liga o LED vermelho

    } else if (compare_buf(command, "OFF LED")) { // Emergência: Desativa o LED vermelho
        HAL_GPIO_WritePin(GPIOC, Red_Pin, GPIO_PIN_RESET); // Desliga o LED vermelho
    }
}

void Yellow_5Hz(void) {
    static uint8_t count = 0;

    if ((HAL_GetTick() - time_yellow_5Hz >= 100) && time_yellow_5Hz != 0) {
        HAL_GPIO_TogglePin(GPIOC, Yellow_Pin); // Alterna o estado do LED amarelo

        if (count++ <= 9) { // Faz o LED piscar 5 vezes
            time_yellow_5Hz = HAL_GetTick(); // Atualiza o tempo para a próxima alternância
        } else {
            count = 0;
            time_yellow_5Hz = 0; // Reseta o tempo
            HAL_GPIO_WritePin(GPIOC, Yellow_Pin, GPIO_PIN_RESET); // Desliga o LED amarelo
        }
    }
}

void Action2_TaskLed(void) {
    static int8_t invert = 1; // Controla o sentido do ajuste de frequência
    static uint8_t toggle = 0; // Alterna o estado

    if (action_2_led == 1) { // Se a ação 2 está ativa
        if (toggle ^= 1) { // Alterna o valor de toggle (muda o estado a cada chamada)
            if (frequencia_Led <= 60) { // Frequência muito alta, inverte o sentido (aumentar frequência)
                invert = 1;
            } else if (frequencia_Led >= 760) { // Frequência muito baixa, inverte o sentido (diminuir frequência)
                invert = -1;
            }

            // Ajusta a frequência do LED
            frequencia_Led = frequencia_Led + 20 * invert;
            __HAL_TIM_SET_AUTORELOAD(&htim6, frequencia_Led * 10); // Atualiza o timer com a nova frequência
            __HAL_TIM_SET_COUNTER(&htim6, 0); // Reinicia o contador do timer
            HAL_TIM_Base_Start_IT(&htim6); // Reinicia o timer
        }
    }
}

void Action3(void) {
    // Verifica se o tempo limite (TIMEOUT_ACTION3) para a ação do buzzer foi atingido
    if ((HAL_GetTick() - time_action3_buzzer >= TIMEOUT_ACTION3) && time_action3_buzzer != 0) {
        time_action3_buzzer = 0; // Reseta o tempo do buzzer

        if (action_2_buzzer == 2) { // Se o buzzer está em modo de espera (2), retorna para ação 1
            action_2_buzzer = 1;
        }

        // Reinicia o timer do buzzer com a frequência atual
        __HAL_TIM_SET_AUTORELOAD(&htim7, frequencia_Buzzer * 10);
        __HAL_TIM_SET_COUNTER(&htim7, 0);
        HAL_TIM_Base_Start_IT(&htim7); // Inicia o timer do buzzer
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET); // Desliga o pino do GPIO (aciona hardware)
    }

    // Verifica se o tempo limite (TIMEOUT_ACTION3) para a ação do LED foi atingido
    if ((HAL_GetTick() - time_action3_led >= TIMEOUT_ACTION3) && time_action3_led != 0) {
        time_action3_led = 0; // Reseta o tempo do LED

        if (action_2_led == 2) { // Se o LED está em modo de espera (2), retorna para ação 1
            action_2_led = 1;
        }

        // Reinicia o timer do LED com a frequência atual
        __HAL_TIM_SET_AUTORELOAD(&htim6, frequencia_Led * 10);
        __HAL_TIM_SET_COUNTER(&htim6, 0);
        HAL_TIM_Base_Start_IT(&htim6); // Inicia o timer do LED
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET); // Desliga o pino do GPIO (aciona hardware)
    }
}

void TaskUSB(void) {
    static uint8_t start_ldr = 0;       // Flag para indicar se o LDR está ativo
    static uint8_t alarme = 0;          // Flag para ativar/desativar alarme
    static char str[5];                 // Buffer para armazenar valor do LDR como string

    static uint8_t display_leitura = 0; // Contador para exibir leituras no display
    static uint8_t pisca = 0;           // Controla o piscar de caracteres no display

    // Comandos e correspondentes para o Buzzer e LED
    char *command[10] = {"ON B", "OFF B", "ON B2", "OFF B2", "ON B3", "ON L", "OFF L", "ON L2", "OFF L2", "ON L3"};
    char *add_command[10] = {"B", "B", "B2", "B2", "", "L", "L", "L2", "L2", ""};

    // Processa o comando recebido
    if (command_send[0] != 0) {
        for (int h = 0; h < 10; h++) {
            if (compare_buf(command_send, command[h])) { // Compara comando
                if ((h >= 0) && (h <= 4)) {
                    TaskBuzzer(command[h]); // Aciona o Buzzer
                } else {
                    TaskLed(command[h]);    // Aciona o LED
                }

                // Adiciona ou remove comandos na lista de ativos
                if ((h == 0) || (h == 2) || (h == 5) || (h == 7)) {
                    Add_Comand(add_command[h]);
                } else if ((h == 1) || (h == 3) || (h == 6) || (h == 8)) {
                    Remove_Comand(add_command[h]);
                }
                break;
            }
        }

        // Comando para ativar o sensor LDR
        if (compare_buf(command_send, "ON LDR")) {
            if (start_ldr == 0) {
                HAL_ADC_Start_IT(&hadc3); // Inicia a leitura do LDR
                start_ldr = 1;
                LCD_Methed(1); // Muda o display para modo LDR
            }

        // Comando para desativar o LDR
        } else if (compare_buf(command_send, "OFF LDR")) {
            HAL_ADC_Stop_IT(&hadc3); // Para a leitura do LDR
            ldr_value = 0;
            start_ldr = 0;
            LCD_Methed(0); // Retorna o display ao modo inicial
        }

        // Limpa o buffer de comando
        for (int i = 0; i < 16; i++) {
            command_send[i] = 0;
        }
    }

    // Se o LDR está ativo, atualiza o display e verifica o valor do sensor
    if (start_ldr) {
        if (display_leitura++ == 10) { // Atualiza a cada 10 ciclos
            display_leitura = 0;

            // Pisca um caractere no display
            if (pisca ^= 1) {
                lcd_put_cur(1, 15);
                lcd_send_string("!");
            } else {
                lcd_put_cur(1, 15);
                lcd_send_string(".");
            }

            // Exibe o valor do LDR no display
            lcd_put_cur(0, 12);
            Float_To_String(ldr_value, str);
            lcd_send_string(str);
        }

        // Se o valor do LDR é maior que 2.5 e o alarme não está ativado
        if (ldr_value > 2.5 && !alarme) {
            TaskBuzzer("OFF B");  // Desativa o Buzzer
            TaskLed("OFF L");     // Desativa o LED

            TaskBuzzer("ON BU");  // Ativa o Buzzer de emergência
            TaskLed("ON LED");    // Ativa o LED vermelho de emergência

            flag_bloqueado = 1;   // Bloqueia a função
            alarme = 1;           // Ativa o alarme

        // Se o valor do LDR é menor que 2.2 e o alarme está ativado
        } else if (ldr_value < 2.2 && alarme) {
            TaskBuzzer("OFF BU"); // Desativa o Buzzer de emergência
            TaskLed("OFF LED");   // Desativa o LED de emergência

            flag_bloqueado = 0;   // Desbloqueia a função

            // Verifica quais comandos estavam ativos antes do alarme
            for (int i = 0; i < num_comandos_ativos; i++) {
                if (strcmp(comandos_ativos[i], "B") == 0) {
                    TaskBuzzer("ON B");
                } else if (strcmp(comandos_ativos[i], "B2") == 0) {
                    TaskBuzzer("ON B2");
                } else if (strcmp(comandos_ativos[i], "L") == 0) {
                    TaskLed("ON L");
                } else if (strcmp(comandos_ativos[i], "L2") == 0) {
                    TaskLed("ON L2");
                }
            }

            alarme = 0; // Desativa o alarme
        }
    }
}




void Float_To_String(float valor, char *str) {
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

void LCD_Methed(uint8_t Methed){
	lcd_clear();

	if(Methed){
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
void Add_Comand(char *comando) {
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
void Remove_Comand(char *comando) {
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
