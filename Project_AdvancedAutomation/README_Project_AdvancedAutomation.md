# 🏭 Sistema Festo MPS — Controlo de PLC com Automação Avançada

> **Mestrado em Engenharia Eletrotécnica e de Computadores**  
> Instituto Politécnico do Cávado e do Ave — Escola Superior de Tecnologia  
> Bruno Rodrigues (31015) · Pedro Rego (14905) · Tiago Miranda (30843)  
> Docente: Bruno Matos · Julho 2025

---

## 📋 Índice

1. [Visão Geral](#-visão-geral)
2. [Estações MPS](#-estações-mps)
3. [Arquitetura do Sistema](#-arquitetura-do-sistema)
4. [Hardware e Software Utilizados](#-hardware-e-software-utilizados)
5. [Comunicação — O Coração do Projeto](#-comunicação--o-coração-do-projeto)
   - [FINS/TCP — Comunicação entre PLCs](#finstcp--comunicação-entre-plcs)
   - [FINS/TCP — Comunicação PC ↔ PLC](#finstcp--comunicação-pc--plc)
   - [Modbus TCP — Simulação com Factory I/O](#modbus-tcp--simulação-com-factory-io)
6. [Modos de Operação](#-modos-de-operação)
7. [Interface Homem-Máquina (HMI)](#-interface-homem-máquina-hmi)
8. [Simulação — Factory I/O](#-simulação--factory-io)
9. [Modelação — GRAFCET](#-modelação--grafcet)
10. [Programação dos PLCs](#-programação-dos-plcs)
11. [Segurança](#-segurança)
12. [Testes e Resultados](#-testes-e-resultados)
13. [Conclusão](#-conclusão)
14. [Estrutura do Repositório](#-estrutura-do-repositório)

---

## 🎯 Visão Geral

Este projeto consiste na **automatização completa de uma linha de fabrico industrial** usando as estações modulares MPS (*Modular Production System*) da Festo — especificamente os módulos **Pick & Place** e **Handling**. O objetivo é integrar mecânica, pneumática, eletrónica, programação de PLCs, comunicação industrial e supervisão por software numa única solução coesa.

Na prática, o projeto é **dois projetos em um**:

| Camada | Descrição |
|--------|-----------|
| **Controlo físico** | Dois PLCs Omron programados em Ladder, sensores, atuadores pneumáticos, tapete transportador e garra de paletização |
| **Supervisão e simulação** | Aplicação C# (HMI), simulação 3D em Factory I/O e comunicação industrial via FINS/TCP e Modbus TCP |

A linha opera de forma **autónoma**, transportando peças, detetando a sua cor e organizando-as em paletes numa sequência predefinida pelo operador.

---

## 🤖 Estações MPS

### Estação Pick & Place

A estação Pick & Place é responsável pelo **transporte e montagem inicial das peças**. Está equipada com um módulo de dois eixos que retira componentes de uma rampa e os insere em carcaças, que seguem no tapete transportador. Utiliza um sistema de **vácuo** para manipulação precisa e sensores óticos para detetar a presença das peças.

> 📷 *[Figura 1 — Estação de Pick & Place]*  
> *(Adicionar imagem: `docs/images/pick_and_place.png`)*

### Estação Handling

A estação Handling tem como função principal a **paletização das peças** provenientes da Pick & Place. Equipada com um sistema elétrico de dois eixos e uma **pinça pneumática**, identifica a cor das peças através de sensores e organiza-as em paletes em conjuntos de quatro, segundo uma sequência definida pelo utilizador. Permite também separação ou encaminhamento para diferentes locais.

> 📷 *[Figura 2 — Estação de Handling]*  
> *(Adicionar imagem: `docs/images/handling.png`)*

---

## 🏗️ Arquitetura do Sistema

O sistema é composto por vários subsistemas que comunicam entre si em tempo real:

```
┌─────────────────────────────────────────────────────────────────┐
│                        PC (Windows)                             │
│                                                                 │
│   ┌─────────────────────┐      ┌───────────────────────┐       │
│   │  App C# (HMI)       │      │    Factory I/O        │       │
│   │  - Supervisão       │      │  (Simulação 3D MPS)   │       │
│   │  - Controlo Manual  │      │                       │       │
│   │  - Seleção de Cores │      │  Modbus TCP (porta    │       │
│   └────────┬────────────┘      │  502, Slave ID:1)     │       │
│            │ FINS/TCP           └──────────┬────────────┘       │
│            │ (porta 9600)                  │ Modbus TCP         │
└────────────│───────────────────────────────│────────────────────┘
             │                               │
             │                               ▼
             │              ┌────────────────────────────┐
             │              │   PLC — Omron NX1P2        │
             ▼              │   (Sysmac Studio)          │
┌────────────────────────┐  │   IP: 192.168.250.2        │
│   PLC — Omron CJ2M     │  │   MASTER (FINS/TCP)        │
│   (CX-Programmer)      │◄─┤   Cliente (Modbus TCP)     │
│   IP: 192.168.250.1    │  │                            │
│   SLAVE (FINS/TCP)     │  │   Estação: Handling        │
│                        │  │   (Garra, Paletização)     │
│   Estação: Pick&Place  │  └────────────────────────────┘
│   (Tapete, Vácuo)      │
└────────────────────────┘

        ↕ I/Os Físicos                ↕ I/Os Físicos
   Sensores · Atuadores          Sensores · Atuadores
   Pneumática · Tapete           Garra · Plataforma
```

---

## 🔧 Hardware e Software Utilizados

### Hardware

| Componente | Descrição |
|------------|-----------|
| **Omron CJ2M** | PLC modular — controla a estação Pick & Place |
| **Omron NX1P2** | PLC compacto avançado — controla a estação Handling e é o master da comunicação |
| **ID211** | Carta de entradas digitais |
| **OD211** | Carta de saídas digitais |
| **ETN21** | Módulo de comunicação Ethernet |
| Sensores óticos difusos | Deteção de presença e cor das peças |
| Fotocélulas e fins de curso | Posição dos atuadores |
| Cilindros pneumáticos | Movimentação linear |
| Ventosas e unidade de vácuo | Manipulação de peças |
| Motores DC | Tapete transportador |
| Pinça pneumática | Agarrar e largar peças (Handling) |

### Software

| Ferramenta | Função |
|------------|--------|
| **FluidSIM Pneumática** | Modelação GRAFCET e simulação de circuitos pneumáticos |
| **CX-Programmer** | Programação em Ladder do PLC Omron CJ2M |
| **Sysmac Studio** | Programação em Ladder + ST do PLC Omron NX1P2 |
| **Visual Studio (C#)** | Desenvolvimento da aplicação de supervisão (HMI) |
| **Factory I/O** | Simulação 3D da linha de fabrico |

---

## 📡 Comunicação — O Coração do Projeto

A comunicação é o elemento mais crítico e complexo deste projeto. Existem **três canais de comunicação distintos**, cada um com um protocolo e propósito diferentes.

---

### FINS/TCP — Comunicação entre PLCs

O protocolo **FINS** (*Factory Interface Network Service*) é o protocolo proprietário da Omron que corre sobre TCP/IP, permitindo troca de dados direta entre equipamentos Omron em redes Ethernet.

**Configuração:**

| Parâmetro | NX1P2 (Master) | CJ2M (Slave) |
|-----------|----------------|--------------|
| IP | `192.168.250.2` | `192.168.250.1` |
| Papel | Inicia comunicação | Responde a pedidos |
| Gestão | Gere todo o fluxo | Executa comandos recebidos |

**Como funciona:**

O NX1P2 é configurado como **master** e é ele que inicia toda a comunicação. No Sysmac Studio, os blocos de função `FINS_Write` e `FINS_Read` tratam da troca de dados:

- **Escrita** (`FINS_Write`): o NX1P2 envia dados para registos do CJ2M — comandos de Start/Stop, sinais de emergência, sequências de cores para paletização, estado da garra.
- **Leitura** (`FINS_Read`): o NX1P2 lê registos do CJ2M — estado dos sensores do tapete (início, cor, fim), sinalização de peça disponível.

Os dados transmitidos incluem:
- **Estados de sensores** — `A1_S_IT` (sensor início tapete), `A1_S_C` (sensor de cor), `A1_S_FT` (sensor fim tapete)
- **Comandos de arranque/paragem** — Start, Stop, Reset partilhados entre estações
- **Sequências de paletização** — array de cores enviado pelo C# para o NX1P2 e retransmitido ao CJ2M
- **Sinais de emergência** — paragem coordenada de ambas as estações

> 📷 *[Figura 51 — Blocos FINS no Sysmac Studio]*  
> *(Adicionar imagem: `docs/images/fins_communication.png`)*

> 📷 *[Figura 52 — Exemplo de Leitura e Escrita no PLC]*  
> *(Adicionar imagem: `docs/images/plc_read_write.png`)*

---

### FINS/TCP — Comunicação PC ↔ PLC

A aplicação C# comunica diretamente com os PLCs usando também o protocolo **FINS/TCP**, permitindo que o HMI leia e escreva variáveis (tags) na memória dos controladores em tempo real.

**O que o PC envia para o PLC:**
- Sequência de cores para paletização (array definido pelo operador no HMI)
- Comandos remotos de Start, Stop e Reset

**O que o PC recebe do PLC:**
- Estado atual do sistema (em execução, parado, emergência)
- Alarmes e erros ativos
- Confirmação de receção da sequência de cores

---

### Modbus TCP — Simulação com Factory I/O

Quando a linha física não está disponível, o Factory I/O substitui o hardware real. A comunicação é feita via **Modbus TCP**, com o NX1P2 configurado como **cliente Modbus** e o Factory I/O como **servidor** (Slave ID: 1, endereço `127.0.0.1`, porta `502`).

> 📷 *[Figura 53 — Bloco Modbus Client no Sysmac Studio]*  
> *(Adicionar imagem: `docs/images/modbus_communication.png`)*

**Mapeamento de I/Os via Modbus:**

As variáveis do Factory I/O são mapeadas para registos Modbus que o NX1P2 lê e escreve a cada 100 ms:

| Direção | Modbus | Variável | Descrição |
|---------|--------|----------|-----------|
| IN | Input 0 | Sensor Deteta Peça Base | Deteção de peça na base |
| IN | Input 4 | Sensor Início Tapete | Peça no início do tapete |
| IN | Input 5 | Sensor Leitura Cor | Cor da peça |
| IN | Input 8 | Sensor Fim Tapete | Peça no fim do tapete |
| IN | Input 11–13 | Sensores Vácuo (F/T/C) | Estados do sistema de vácuo |
| IN | Input 17 | Sensor Posição Recolha | Posição de recolha (Handling) |
| IN | Input 25–27 | Start/Stop/Reset A1 | Botões painel Pick & Place |
| IN | Input 29–31 | Start/Stop/Reset A2 | Botões painel Handling |
| OUT | Coil 1 | Pneumático Início | Cilindro inicial |
| OUT | Coil 3 | Motor Tapete | Liga/desliga tapete |
| OUT | Coil 9 | Avanço Pneumático Vácuo | Extensão do braço de vácuo |
| OUT | Coil 10 | Baixa Pneumático Vácuo | Descida do braço de vácuo |
| OUT | Coil 11 | Vácuo | Ativa ventosa |
| OUT | Coil 14 | Garra Prende | Fecha pinça |
| OUT | Coil 15–16 | Plataforma Frente/Trás | Movimento da plataforma |
| Holding Reg 0–2 | — | Garra X/Y/Z | Posição analógica da garra |

> 📷 *[Figura 54/55 — Tabela de I/Os Modbus completa]*  
> *(Adicionar imagem: `docs/images/modbus_io_mapping.png`)*

**Vantagem desta abordagem:** o programa Ladder corre exatamente igual, seja com hardware físico ou com o simulador. A única diferença está nos valores dos timers, que foram ajustados para os tempos de resposta do ambiente virtual.

---

## 🎮 Modos de Operação

### Modo Automático
Execução contínua e autónoma do ciclo completo:
1. A Pick & Place deteta uma peça → ativa o sistema de vácuo → coloca a peça na carcaça
2. O tapete transporta a peça até ao fim da linha
3. O sensor de cor identifica a peça
4. O Handling recolhe a peça e coloca-a na posição correta da palete (segundo a sequência definida)
5. Ciclo repete até completar a palete de 4 peças

### Modo Manual
Controlo individual de cada componente — útil para manutenção, diagnóstico e ajuste de sensores. O HMI em C# disponibiliza botões dedicados para cada saída.

### Sequência de Arranque / Paragem

```
RESET → [verificação de condições] → PRONTO
PRONTO + START → AUTOMÁTICO em execução
AUTOMÁTICO + STOP → paragem controlada no fim do ciclo
EMERGÊNCIA → paragem imediata → aguarda RESET
```

---

## 🖥️ Interface Homem-Máquina (HMI)

A aplicação desenvolvida em **C# (Visual Studio)** centraliza toda a supervisão e controlo do sistema. É composta por três formulários principais:

### Menu Principal
Mostra logs em tempo real da comunicação FINS com o PLC: estados do sistema, mensagens de erro, alarmes e emergências. Dá acesso às duas funcionalidades principais.

> 📷 *[Figura 48 — Menu Principal da Aplicação]*  
> *(Adicionar imagem: `docs/images/hmi_main.png`)*

### Seleção de Cores
Painel gráfico com blocos que representam as 4 posições da palete. O operador clica para alternar entre cores (vermelho/preto) e define a sequência de paletização antes de clicar **"Executar — Enviar para o PLC"**. Apenas após confirmação o controlador recebe a configuração.

> 📷 *[Figura 49 — Seleção de Cores]*  
> *(Adicionar imagem: `docs/images/hmi_colors.png`)*

### Controlo Manual de Saídas
Botões individuais para cada saída digital: transportadores, cilindros pneumáticos, ventosas, stoppers, garra e plataforma. Permite testes sem recorrer ao modo automático.

> 📷 *[Figura 50 — Controlo Manual de Saídas]*  
> *(Adicionar imagem: `docs/images/hmi_manual.png`)*

---

## 🎮 Simulação — Factory I/O

Dado que o equipamento físico é partilhado por vários grupos e um dos motores avariou durante o projeto, o **Factory I/O** foi essencial para continuidade do desenvolvimento.

> 📷 *[Figura 56 — Simulação 3D da Linha MPS no Factory I/O]*  
> *(Adicionar imagem: `docs/images/factory_io.png`)*

A simulação replica fielmente o ambiente físico em 3D:
- Transportadores e tapete
- Sensores óticos e stoppers
- Atuadores pneumáticos e sistema de vácuo
- Sistema de paletização com garra

**Fluxo de desenvolvimento com simulação:**

```
Editar Ladder/ST → Compilar → Descarregar para NX1P2
        ↓
NX1P2 liga via Modbus → Factory I/O reage
        ↓
Observar comportamento → Corrigir lógica → Repetir
        ↓
Sistema validado → Testar no hardware real (apenas ajustar timers)
```

---

## 📐 Modelação — GRAFCET

O processo foi modelado em **GRAFCET** usando o FluidSIM, organizando a lógica em módulos independentes que funcionam em paralelo:

| Módulo GRAFCET | Função |
|---------------|--------|
| **Máquina de Estados** | Estado global do sistema (Reset, Init, Running, Stop, Emergência) |
| **Início** | Sequência de inicialização do cilindro pneumático Pick & Place |
| **Tapete** | Controlo do motor do tapete e das pausas durante deteção/transporte |
| **Vácuo** | Controlo completo do sistema de ventosa (avanço, descida, ativação, recuo) |
| **Garra** | Controlo da pinça e plataforma da estação Handling com lógica de posicionamento |

Cada GRAFCET foi posteriormente convertido para as equações de estado e implementado em linguagem Ladder nos respetivos PLCs.

---

## 💻 Programação dos PLCs

### CX-Programmer — Omron CJ2M (Estação Pick & Place)

Programado em **Ladder** e estruturado em secções:
- **Inicialização** — reset de variáveis e configuração ao primeiro ciclo
- **Comunicação** — transmissão dos sensores `A1_S_IT`, `A1_S_C`, `A1_S_FT` para o NX1P2
- **Tapete** — controlo do motor `A1_MT` e do stopper `A1_SP`
- **Vácuo** — sequência de 9 rungs para controlo do braço pneumático e ventosa
- **Emergências** — timers de segurança (10 s) para deteção de bloqueios

Mapeamento de I/Os principais do CJ2M:

| Variável | End. | Descrição |
|----------|------|-----------|
| `A1_S_IT` | 0.08 | Sensor início tapete |
| `A1_S_C` | 0.13 | Sensor leitura cor |
| `A1_S_FT` | 0.14 | Sensor fim tapete (negado) |
| `A1_S_VPnF/T/C` | 0.09–0.11 | Sensores posição vácuo |
| `A1_MT` | 1.06 | Motor tapete |
| `A1_SP` | 1.11 | Stop peça |
| `A1_V` | 1.12 | Vácuo (ventosa) |
| `A1_APnV` | 1.07 | Avanço pneumático vácuo |
| `A1_BPnV` | 1.15 | Baixa pneumático vácuo |

### Sysmac Studio — Omron NX1P2 (Estação Handling)

Programado em **Ladder** e **Structured Text (ST)**, organizando:
- **Máquina de Estados** — gestão global (E0–E6): Reset, Init, Tapete pronto, Timer início, Start, Stop
- **Garra** — 23 rungs cobrindo estados E1[0]–E1[20] com controlo analógico de posição (WORD)
- **Posição da Peça (ST)** — lógica de alocação de posições na palete:

```pascal
(* Procurar uma posição compatível *)
IF init_program = TRUE THEN
    FOR i := 0 TO 3 DO
        IF (cor_desejada[i] = cor_automato_1[j]) AND (espaco_posicao[i] = 0) THEN
            posicao_escolhida := i;
            espaco_posicao[i] := 1;  (* Marcar como ocupada *)
            EXIT;
        END_IF;
    END_FOR;
    j := j + 1;
    IF j >= 4 THEN j := 0; END_IF;
    coloca_peca := TRUE;
END_IF;
```

Mapeamento de I/Os principais do NX1P2:

| Variável | End. | Descrição |
|----------|------|-----------|
| `A2_S_PR` | Bit 00 | Sensor posição recolha |
| `A2_S_GB` | Bit 05 | Sensor garra baixo |
| `A2_S_GC` | Bit 04 | Sensor garra cima |
| `A2_S_GP` | Bit 06 | Sensor garra prende |
| `A2_GE` | Out 00 | Garra esquerda |
| `A2_GP` | Out 02 | Garra prender |
| `A2_GB` | Out 01 | Garra baixo |
| `A2_PlF` | Out 08 | Plataforma frente |
| `A2_PlT` | Out 09 | Plataforma trás |

---

## 🔒 Segurança

O sistema inclui múltiplas camadas de segurança:

**Dispositivos físicos:**
- **Start** — inicia o ciclo de operação
- **Stop** — paragem de emergência imediata
- **Reset** — rearme após paragem ou falha
- **Seletor manual/automático** — alterna modo de operação

**Comportamento em emergência:**
1. Paragem imediata de todos os atuadores
2. Sistema aguarda ação do operador
3. Retoma apenas após pressionar **Reset** e verificação das condições de segurança

**Timers de emergência (NX1P2):**
- `Emergency_claw_down` — 10 s sem confirmar descida da garra
- `Emergency_claw_left/right` — 10 s sem confirmar posição lateral
- `Emergency_paletization_back/front` — 10 s sem confirmar posição da plataforma

**Modo manual:** movimentos potencialmente perigosos são inibidos para evitar colisões entre atuadores.

---

## 🧪 Testes e Resultados

Os testes cobriram ambos os modos de operação:

**Modo Automático:**
- ✅ Transporte correto das peças pelo Pick & Place
- ✅ Deteção precisa por sensores óticos
- ✅ Inserção das peças nas carcaças
- ✅ Paletização com sequência de cores respeitada
- ✅ Ciclos completos de 4 peças por palete

**Modo Manual:**
- ✅ Todos os atuadores testados individualmente
- ✅ Diagnóstico de sensores via HMI

**Dificuldades encontradas:**
- Partilha do equipamento entre grupos → testado extensivamente via simulação
- Avaria de um motor → resolvido com Factory I/O até reparação
- Calibração dos sensores de cor → resolvido ajustando sensibilidade e lógica de temporização
- Sincronização de sinais entre estações → resolvido refinando timers no Ladder

---

## 🏁 Conclusão

O projeto atingiu todos os objetivos definidos. A integração entre dois PLCs Omron com protocolos industriais, um HMI em C# e simulação 3D demonstrou ser uma solução robusta e flexível. A possibilidade de alternar entre hardware físico e simulação sem alterar o programa de controlo (exceto timers) é um resultado particularmente relevante.

**Trabalho futuro:**
- Integração de visão artificial para controlo de qualidade automatizado
- Otimização de tempo de ciclo com algoritmos de controlo avançados
- Expansão para mais estações MPS

---

## 📁 Estrutura do Repositório

```
/
├── README.md
├── docs/
│   └── images/               ← Imagens para este README
│       ├── pick_and_place.png
│       ├── handling.png
│       ├── hmi_main.png
│       ├── hmi_colors.png
│       ├── hmi_manual.png
│       ├── fins_communication.png
│       ├── modbus_communication.png
│       ├── modbus_io_mapping.png
│       └── factory_io.png
│
├── PLC_CJ2M/                 ← Projeto CX-Programmer (Pick & Place)
│   └── *.cxp
│
├── PLC_NX1P2/                ← Projeto Sysmac Studio (Handling)
│   └── *.smc2
│
├── HMI_CSharp/               ← Aplicação Visual Studio C#
│   └── AUTOFOX/
│
└── FactoryIO/                ← Cena de simulação Factory I/O
    └── *.factoryio
```

---

<div align="center">

**Instituto Politécnico do Cávado e do Ave · EST · 2025**  
*Automação Avançada — Grupo C*

</div>
