#ifndef __MAIN_H
#define __MAIN_H

#include "stm32f10x.h"
#include "stm32f10x_adc.h"
#include "lcd.h"
#include "dht22.h"
#include "sen0193.h"
#include "ldr.h"
#include "usart.h"
#include "motor.h"
#include "ticks.h"
#include "sdcard_user.h"
#include "main.h"
#include <stdio.h>

/* VARIABLES */
uint32_t response;
uint16_t humidity, temperature;
uint16_t moisture;
uint16_t light;
uint16_t idMoist;
uint16_t idLight;
uint16_t idTemp;
uint16_t idHumid;

uint8_t SD_Buffer_TX[BLOCK_SIZE], SD_Buffer_RX[BLOCK_SIZE];
extern GPIO_InitTypeDef GPIO_InitStructure;

// Hex Sensors
char hexResponse[16];
char hexTemperature[16];
char hexHumidity[16];
char hexMoisture[16];
char hexLight[16];
char hexTimer[16];
char hexTime[16];
char hexSirem[16];

char hexIdMoist[16];
char hexIdLight[16];
char hexIdTemp[16];
char hexIdHumid[16];

void Delayus(int duration);

#endif	/* __MAIN_H */

