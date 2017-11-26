#include "main.h"

int main(void)
{
	/* SENSORS
	// INIT
	SEN0193_Init();
	//DHT22_Init();
	LDR_Init();
	
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
	/**/
	
	/* BLUETOOTH TEST
	uint16_t rxdata = 0;
	USARTx_INIT();
	LCD_INIT();
	
	while(1) {
		Delayus(100000);
		
		if (USART_GetFlagStatus(USART1,USART_FLAG_RXNE) == SET) {
			rxdata = USART_ReceiveData(USART1);
		  if (rxdata == 0x30) {
          GPIO_SetBits(GPIOB, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_5);
			}	
			else if (rxdata == 'R') {
		     GPIO_ResetBits(GPIOB, GPIO_Pin_5);		
         GPIO_SetBits(GPIOB, GPIO_Pin_0 | GPIO_Pin_1);
			}	
			else if (rxdata == 'G') {
		     GPIO_ResetBits(GPIOB, GPIO_Pin_0);		
         GPIO_SetBits(GPIOB, GPIO_Pin_5 | GPIO_Pin_1);
			}	
			else if (rxdata == 'B') {
		     GPIO_ResetBits(GPIOB, GPIO_Pin_1);		
         GPIO_SetBits(GPIOB, GPIO_Pin_0 | GPIO_Pin_5);
			}	
		}
	}
	/**/
	
	/* SDCARD */
	LCD_INIT();
	SDCard_Init();
	
	SDCard_StoreData(0x00, "asdf");
	
	//SD_SingleBlockTest(); 
	/**/
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
