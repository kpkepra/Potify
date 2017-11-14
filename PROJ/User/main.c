#include "stm32f10x.h"
#include "stm32f10x_adc.h"
#include "lcd.h"
#include "dht22.h"
#include "sen0193.h"
#include "ldr.h"
#include "delay.h"
#include "uart.h"
#include <stdio.h>


/*------------------------------------------------------------
PROJ Template
-------------------------------------------------------------*/

uint32_t response;
uint16_t humidity, temperature;
uint16_t moisture;
uint16_t light;

char hexResponse[16];
char hexTemperature[16];
char hexHumidity[16];
char hexMoisture[16];
char hexLight[16];

void manual_mode(const u8 byte) { //Manual Mode Function
	if ((byte == ' ')) {
		uart_tx(COM3, "Switched to manual mode\n");
	}
}

int main(void)
{
	// INIT
	uart_init(COM3, 115200);
	uart_interrupt_init(COM3, &manual_mode);
	
	LCD_INIT();
	SEN0193_Init();
	//DHT22_Init();
	LDR_Init();
	ticks_init();
	
  while (1) {
		DHT22_Init();
		response = DHT22_GetReadings();
		if (response == DHT22_RCV_OK) {
			response = DHT22_DecodeReadings();
			temperature = DHT22_GetTemperature();
			humidity = DHT22_GetHumidity();
		}
		// Temperature & Humidity : Divide by 10
		//sprintf(hexResponse, "DHT22 Response : %d", response);
		//LCD_DrawString(10, 10, hexResponse);
		moisture = getADC(14);
		light = getVal_LDR(15);
		
		sprintf(hexTemperature, "Temp : %d", temperature);
		sprintf(hexHumidity, "Humidity : %d", humidity);
		sprintf(hexMoisture, "Moisture : %d", moisture);
		sprintf(hexLight, "Light : %d", light/2);
		
		LCD_DrawString(10, 25, hexTemperature);
		LCD_DrawString(10, 40, hexHumidity);
		LCD_DrawString(10, 55, hexMoisture);
		LCD_DrawString(10, 70, hexLight);
		Delay_ms(50);
  }
}


void Delayus(int duration)
{
		while(duration--) 
		{
			int i=0x02;				
			while(i--)
			__asm("nop");
		}
}
