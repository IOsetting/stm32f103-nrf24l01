# stm32f103-nrf24l01
A working demo of nRF24L01 WiFi module running on STM32F103C8T6

## Pinout

Turn chip side towards you, antenna to the left and Oscillator on top:

|     |      |
| ---- | ---- |
| IRQ  | MISO |
| MOSI | CSK  |
| CSN  | CE   |
| VCC  | GND  |

## Connection with STM32F103

| STM32          |  nRF24L01 |
| -------------- | --------- |
| PA4 SPI1_NSS   |  N/A      |
| PA5 SPI1_SCK   |  SCK      |
| PA6 SPI1_MISO  |  MISO     |
| PA7 SPI1_MOSI  |  MOSI     |
| PB13           |  IRQ      |
| PB14           |  CE       |
| PB15           |  CSN      |

## Build

Create a `env.h` file under user folder with content
```c
#ifndef _ENV_H
#define _ENV_H

#include <stm32f10x.h>

#define USARTx USART1

#define NRF24L01_SPIx     SPI1
#define NRF24L01_GPIOx    GPIOB
#define NRF24L01_GPIO_IRQ GPIO_Pin_13
#define NRF24L01_GPIO_CE  GPIO_Pin_14
#define NRF24L01_GPIO_CSN GPIO_Pin_15

#endif /*_ENV_H*/
```
