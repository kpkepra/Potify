#include "main.h"

uint16_t rxdata = 0;
TIM_TimeBaseInitTypeDef	TIM_TimeBaseStructure;
TIM_OCInitTypeDef TIM_OCInitStructure;
int timer = 0;
int pwm = 10000;
int sirem = 0;
int manual = 0;
int second = 0;
int min = 0;

int i = 0;

uint8_t * ptr;

int main(void)
{
	/* SENSORS */
	// INIT
	LCD_INIT();
	USARTx_INIT();
	SEN0193_Init();
	SDCard_Init();
	LDR_Init();
	ticks_init();
	motor_init(71, 50000, 0);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_DeInit(GPIOA);
	
	SDCard_StoreData(0x000, "1");
	SDCard_StoreData(0x200, "2");
	SDCard_StoreData(0x400, "3");
	
	*SD_Buffer_RX = SDCard_ReadData(0x000);		
	
	idMoist = 2800;
	idLight = 150;
	idTemp = 20;
	idHumid = 80;

  while (1) {
		second = get_real_ticks() / 1000;
		if (second >= 60) {
			second = 0;
			min++;
		}
		
		if (min >= 60) {
			min = 0;
		}
		
		DHT22_Init();
		
		response = DHT22_GetReadings();
		if (response == DHT22_RCV_OK) {
			response = DHT22_DecodeReadings();
			temperature = DHT22_GetTemperature();
			humidity = DHT22_GetHumidity();
		}
		moisture = getADC(14);
		light = getVal_LDR(15);
		
		sprintf(hexTemperature, 	"Temp  : %d.%d C", temperature / 10, temperature % 10);
		sprintf(hexHumidity, 				"Humid : %d.%d", humidity / 10, humidity % 10);
		sprintf(hexMoisture, 				"Moist : %d.%d", moisture / 100, moisture % 100);
		sprintf(hexLight, 						"Light : %d", light/3);
		sprintf(hexTime,							"%d:%d", min, second);
		
		sprintf(hexSirem,						"Water : %d", sirem);
		
		LCD_DrawString(5, 5, hexTime);
		LCD_DrawString(10, 100, hexTemperature);
		LCD_DrawString(10, 115, hexHumidity);
		LCD_DrawString(10, 130, hexMoisture);
		LCD_DrawString(10, 145, hexLight);
		
		LCD_DrawString(10, 175, hexSirem);
				
		timer = TIM3->CNT;
		
		sprintf(hexIdMoist,	"Id. Mois : %d", idMoist);
		sprintf(hexIdLight,		"Id. Light : %d", idLight);
		sprintf(hexIdTemp,	"Id. Temp : %d", idTemp);
		sprintf(hexIdHumid,	"Id. Humid : %d", idHumid);
		LCD_DrawString(10, 25, hexIdMoist);
		LCD_DrawString(10, 40, hexIdLight);
		LCD_DrawString(10, 55, hexIdTemp);
		LCD_DrawString(10, 70, hexIdHumid);
		
		if (!manual) {
			if (moisture - idMoist >= 200 ||
				temperature - idTemp*10 >= 50 ||
				//abs(light/3 - idLight) <= 10 &&
				humidity/10 - idHumid <= -10) {
				sirem = 1;
			} else { sirem = 0; }
		}
		
		if (sirem) {
			if (timer <= pwm) {
				GPIO_Init(GPIOA,&GPIO_InitStructure);
			} else {
				GPIO_DeInit(GPIOA);
			}
		}
		
		if (USART_GetFlagStatus(USART1,USART_FLAG_RXNE) == SET) {
			rxdata = USART_ReceiveData(USART1);
		  if (rxdata == 0x30) {
          GPIO_SetBits(GPIOB, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_5);
			}	else if (rxdata == 'G') {
				 GPIO_ResetBits(GPIOB, GPIO_Pin_0);		
         GPIO_SetBits(GPIOB, GPIO_Pin_1 | GPIO_Pin_5);
			}	else if (rxdata == 'B') {
		     GPIO_ResetBits(GPIOB, GPIO_Pin_1);		
         GPIO_SetBits(GPIOB, GPIO_Pin_0 | GPIO_Pin_5);
			}	else if (rxdata == 'S') {
				manual = 1;
				sirem = (sirem ? 0 : 1);
			} else if (rxdata == '1') {
				*SD_Buffer_RX = SDCard_ReadData(0x000);
				if (*SD_Buffer_RX == '1') {
					idMoist = 2600;
					idLight = 150;
					idTemp = 20;
					idHumid = 80;
				}
			} else if (rxdata == '2') {
				*SD_Buffer_RX = SDCard_ReadData(0x200);
				if (*SD_Buffer_RX == '2') {
					idMoist = 2700;
					idLight = 250;
					idTemp = 24;
					idHumid = 65;
				}
			} else if (rxdata == '3') {
				*SD_Buffer_RX = SDCard_ReadData(0x400);
				if (*SD_Buffer_RX == '3') {
					idMoist = 2600;
					idLight = 260;
					idTemp = 22;
					idHumid = 70;
				}
			}
			
		}
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
