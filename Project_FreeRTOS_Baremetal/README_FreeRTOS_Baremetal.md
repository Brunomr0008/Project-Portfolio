# 🔧 STM32 Embedded System — Bare Metal vs FreeRTOS

![STM32](https://img.shields.io/badge/MCU-STM32H743ZI2-blue?style=flat-square)
![FreeRTOS](https://img.shields.io/badge/OS-FreeRTOS-green?style=flat-square)
![HAL](https://img.shields.io/badge/Framework-STM32HAL-orange?style=flat-square)
![Status](https://img.shields.io/badge/Status-Concluído-brightgreen?style=flat-square)

---

## 📋 Descrição

Este projeto implementa o **mesmo sistema embebido** de duas formas diferentes, permitindo comparar diretamente as abordagens **Bare Metal** e **FreeRTOS**. O sistema controla LEDs, buzzer, display LCD e lê um sensor de luz (LDR), tudo acionado por um botão físico com deteção de clique simples, duplo, triplo e longo.

---

## 🎮 Demonstração

### FreeRTOS

https://github.com/user-attachments/assets/070cfb22-4d01-41fa-b131-11f85a0fd2fa

### Bare Metal

https://github.com/user-attachments/assets/749e743d-47bb-47dc-a689-9ba3a509a7c8

> **Nota:** Se os vídeos não carregarem diretamente, estão disponíveis em `docs/videos/`.

---

## ⚙️ Hardware Utilizado

<div align="center">
    
| Componente | Função |
|---|---|
| **STM32H7** | Microcontrolador principal |
| **LCD 1602** | Display de texto 16x2 |
| **Buzzer** | Saída de som |
| **LED Verde** | Indicador de estado normal |
| **LED Amarelo** | Indicador de padrão 5Hz |
| **LED Vermelho** | Alerta de emergência |
| **Botão** | Input do utilizador |
| **LDR** | Sensor de luz (via ADC) |
</div>

---

## 🕹️ Funcionamento — O que o sistema faz

O comportamento é **idêntico** nas duas implementações:

<div align="center">
    
| Ação no botão | O que acontece |
|---|---|
| **1 clique** | Buzzer + LED verde piscam a 0.5s (toque intermitente) |
| **2 cliques** | Alterna frequência do buzzer e LED (mais rápido/mais lento) |
| **3 cliques** | Toque rápido a 150ms durante 3 segundos |
| **Pressão longa** | Desliga tudo e reinicia o estado |
| **USB `ON LDR`** | Ativa sensor de luz — emergência se LDR > 2.5V |
| **Emergência** | Buzzer contínuo + LED vermelho, tudo bloqueado |
</div>

---

## 🔩 Bare Metal

Na abordagem Bare Metal **não existe sistema operativo**. O microcontrolador corre um único ciclo `while(1)` que chama todas as funções sequencialmente. É o programador que gere o tempo e a ordem de execução.

```c
// main.c — loop principal
while (1) {
    TaskButton();        // Verifica botão
    TaskUSB();           // Processa comandos USB e LDR
    Yellow_5Hz();        // Controla LED amarelo
    if (flag_callback_timer7) Action2_TaskBuzzer(); // Timer do buzzer
    if (flag_callback_timer6) Action2_TaskLed();    // Timer do LED
    Action3();           // Controla padrão de 3s
}
```

**Como funciona o tempo?** Através de timers de hardware (`TIM6`, `TIM7`) que disparam interrupções. As interrupções ativam flags (`flag_callback_timer6`, `flag_callback_timer7`) que o loop principal verifica para saber quando agir.

```c
// Interrupção do timer → ativa flag → loop principal reage
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim == &htim7) flag_callback_timer7 = 1; // Buzzer
    if (htim == &htim6) flag_callback_timer6 = 1; // LED
}
```

---

## 🟢 FreeRTOS

No FreeRTOS existe um **sistema operativo** que gere múltiplas tarefas em paralelo. Cada funcionalidade é uma tarefa independente com a sua própria prioridade. O RTOS decide qual tarefa corre em cada momento.

```c
// 4 tarefas criadas na inicialização
osThreadDef(myTask01_Button, StartTask01_Button, osPriorityNormal,      0, 128);
osThreadDef(myTask02_Buzzer, StartTask02_Buzzer, osPriorityBelowNormal, 0, 128);
osThreadDef(myTask03_Led,    StartTask03_Led,    osPriorityBelowNormal, 0, 128);
osThreadDef(myTask04_USB,    StartTask04_USB,    osPriorityAboveNormal, 0, 128);
```

As tarefas comunicam entre si através de **filas de mensagens** (queues):

```c
// Tarefa do Botão envia comando → Tarefa do Buzzer recebe e age
osMessagePut(myQueue01Handle, (uint32_t)"ON B", 10);   // Botão → Buzzer
osMessagePut(myQueue02Handle, (uint32_t)"ON L", 10);   // Botão → LED

// Tarefa do USB envia para ambos (em emergência)
osMessagePut(myQueue01Handle, (uint32_t)"ON BU", 10);  // USB → Buzzer (emergência)
osMessagePut(myQueue02Handle, (uint32_t)"ON LED", 10); // USB → LED (emergência)
```

---

## ⚖️ Comparação: Bare Metal vs FreeRTOS

<div align="center">

| Característica | Bare Metal | FreeRTOS |
|---|---|---|
| **Estrutura** | Um loop `while(1)` | 4 tarefas independentes |
| **Timing** | Timers de hardware + flags | Timers de software do RTOS |
| **Comunicação** | Variáveis globais partilhadas | Queues + Semáforos |
| **Prioridades** | Tudo tem a mesma prioridade | Cada tarefa tem prioridade própria |
| **Bloqueio** | Flag `flag_bloqueado` global | Semáforo `mySemButtonBlockHandle` |
| **Complexidade do código** | Mais simples | Mais estruturado |
| **Escalabilidade** | Difícil de expandir | Fácil de adicionar tarefas |
| **Uso de memória** | Menor | Maior (stack por tarefa) |
| **Determinismo** | Depende do loop | Garantido pelo scheduler |
</div>

---

## 🧩 Arquitetura — Bare Metal

<div align="center">
  <pre>
┌─────────────────────────────────────────────────────┐
│                   while(1) loop                     │
│                                                     │
│  TaskButton() ──► TaskBuzzer() / TaskLed()          │
│  TaskUSB()    ──► (LDR + comandos USB)              │
│  Yellow_5Hz() ──► (LED amarelo direto)              │
│  Action3()    ──► (timeout 3s)                      │
│                                                     │
│  ┌──────────┐   ┌──────────┐   Interrupções:        │
│  │  TIM6    │   │  TIM7    │   flag → loop reage    │
│  │ (LED)    │   │ (Buzzer) │                        │
│  └──────────┘   └──────────┘                        │
└─────────────────────────────────────────────────────┘
  </pre>
</div>

## 🧩 Arquitetura — FreeRTOS

<div align="center">
  <pre>
┌────────────────────────────────────────────────────────────────┐
│                        FreeRTOS Scheduler                      │
│                                                                │
│  ┌─────────────┐  Queue01  ┌─────────────┐                     │
│  │  Task01     │──────────►│  Task02     │                     │
│  │  Button     │           │  Buzzer     │                     │
│  │ (Normal)    │  Queue02  │(BelowNormal)│                     │
│  └─────────────┘──────────►┌─────────────┐                     │
│                            │  Task03     │                     │
│  ┌─────────────┐  Queue03  │  LED        │                     │
│  │  Task04     │──────────►│(BelowNormal)│                     │
│  │  USB + LDR  │           └─────────────┘                     │
│  │(AboveNormal)│  Queue01+02 ──► (emergência → todas tarefas)  │
│  └─────────────┘                                               │
│                                                                │
│  Semáforos: ButtonBlock, USBBlock, BuzzerWait, LedWait         │
└────────────────────────────────────────────────────────────────┘
  </pre>
</div>

---

## 📁 Estrutura do Repositório

```
📁 STM32-Project/
├── 📁 BareMetalImplementation/
│   ├── Core/Src/main.c          ← Loop principal + todas as funções
│   └── Core/Src/...             ← Periféricos (ADC, TIM, USB, GPIO)
├── 📁 FreeRTOSImplementation/
│   ├── Core/Src/freertos.c      ← 4 tarefas + queues + semáforos
│   └── Core/Src/...             ← Periféricos (ADC, TIM, USB, GPIO)
└── 📁 docs/
    └── 📁 videos/
        ├── FreeRTOS.mp4
        └── baremetal.mp4
```

---

## 🛠️ Tecnologias

<div align="center">

| Tecnologia | Utilização |
|---|---|
| **STM32CubeIDE** | IDE de desenvolvimento |
| **STM32 HAL** | Abstração de hardware |
| **FreeRTOS / CMSIS-OS** | Sistema operativo em tempo real |
| **LCD1602** | Biblioteca de display |
| **USB CDC** | Comunicação série via USB |
</div>

---

## 🏫 Instituição

**Instituto Politécnico do Cávado e do Ave (IPCA)**  
Escola Superior de Tecnologia  
MEEC — Mestrado em Engenharia Eletrotécnica e de Computadores  
Sistemas Embebidos em Tempo Real
