#include "stm32f10x.h"
#include "stm32f10x_adc.h"
#include "lcd.h"
#include "dht22.h"
#include "sen0193.h"
#include "ldr.h"
#include "usart.h"
#include "sdcard_user.h"
#include <stdio.h>

/* VARIABLES */
uint32_t response;
uint16_t humidity, temperature;
uint16_t moisture;
uint16_t light;

uint8_t SD_Buffer_TX[BLOCK_SIZE], SD_Buffer_RX[BLOCK_SIZE];

// Hex Sensors
char hexResponse[16];
char hexTemperature[16];
char hexHumidity[16];
char hexMoisture[16];
char hexLight[16];

void Delayus(int duration);

/* FUNCTIONS */
