# **FreeRTOS Embedded System Project**

## **Overview**
This project implements a real-time embedded system using FreeRTOS on an STM32 microcontroller. The system features multiple interactive components including button input processing, LED control, buzzer output, and LCD display, all managed through a multi-tasking architecture.

## **Features**
Task Architecture

The system is divided into four main tasks:

### Button Task (Task01_Button)

- Handles button input detection

- Processes multiple click patterns

- Manages button state transitions

- Priority: Normal


### Buzzer Task (Task02_Buzzer)

- Controls buzzer output patterns

- Manages frequency modulation

- Implements different sound patterns

- Priority: Below Normal


### LED Task (Task03_Led)

- Controls LED status and patterns

- Manages LED state transitions

- Implements visual feedback patterns

- Priority: Below Normal


### USB/Display Task (Task04_USB)

- Handles USB communication

- Manages LCD display updates

- Processes LDR (Light Dependent Resistor) readings

- Priority: Above Normal



## **Inter-Task Communication**
**The system utilizes various FreeRTOS communication mechanisms:**

- Message Queues for command transmission between tasks

- Semaphores for synchronization of multiple clicks

- Timers for managing periodic events and timeouts

## **Features Implementation**

### Button Interface

- Single click: Activates intermittent buzzer and LED patterns

- Double click: Toggles secondary patterns

- Triple click: Activates temporary patterns

- Long press detection


### Sound and Visual Feedback

- Variable frequency buzzer patterns

- Synchronized LED patterns

- Multiple pattern combinations

- Emergency alert patterns


### Environmental Monitoring

- LDR-based light level detection

- Threshold-based alert system

- Real-time display updates

- Emergency mode activation


### Display Interface

- Real-time status display

- LDR value visualization

- System state indication

- Alert status display



## **Technical Details**
**Hardware Requirements**

- STM32 Microcontroller

- LCD 1602 Display

- Buzzer

- LEDs (multiple)

- Push Button

- Light Dependent Resistor (LDR)

- Software Architecture

**Operating System: FreeRTOS** 
**Framework: STM32HAL**
**Development Environment: STM32CubeIDE**

## **Task Priorities**
**The system implements a priority-based scheduling with:**

- USB/Display Task: Highest priority for responsive user interface

- Button Task: Normal priority for input handling

- Buzzer and LED Tasks: Lower priority for output management

### **Memory Management**

- Static task creation with optimized stack sizes

- Efficient queue management for inter-task communication

- Shared resource protection through semaphores

## **System States**
**Normal Operation**

- System starts in idle state

- Responds to button inputs

- Updates display with current status

- Maintains LED and buzzer patterns as commanded

### **Alert Mode**

- Activates when LDR threshold is exceeded

- Triggers emergency patterns

- Overrides normal operation patterns

- Automatically returns to normal mode when condition clears

## **Future Improvements**

- Additional sensor integration

- Enhanced pattern configurations

- Expanded USB functionality

- Power optimization features

- Advanced error handling mechanisms


### **Contributors**
- Part of the Real-Time Embedded Systems course

MEEC (Master in Electrical and Computer Engineering)

> [!TIP]
> ### **Implementation Notes**
> ### Uses hardware timer for system timing
> ### Implements debouncing for button inputs
> ### Maintains command history for pattern restoration
> ### Provides fail-safe operation modes

:+1: This project demonstrates the effective use of FreeRTOS features to create a responsive and robust embedded system with multiple interactive components and real-time processing capabilities.
