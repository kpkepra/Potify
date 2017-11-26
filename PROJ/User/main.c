#include "stm32f10x.h"
#include "stm32f10x_adc.h"
#include "lcd.h"
#include "dht22.h"
#include "sen0193.h"
#include "ldr.h"
#include "usart.h"
#include "sdcard.h"
#include <stdio.h>

#define BLOCK_SIZE            512 /* Block Size in Bytes */

typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;
/*------------------------------------------------------------
PROJ Template
-------------------------------------------------------------*/

void Delayus(int duration);

uint8_t Buffer_Block_Tx[BLOCK_SIZE], Buffer_Block_Rx[BLOCK_SIZE];
volatile TestStatus EraseStatus = FAILED, TransferStatus1 = FAILED, TransferStatus2 = FAILED;

uint32_t response;
uint16_t humidity, temperature;
uint16_t moisture;
uint16_t light;

// Hex Sensors
char hexResponse[16];
char hexTemperature[16];
char hexHumidity[16];
char hexMoisture[16];
char hexLight[16];

// Hex Status
char hexStatus[16];
char hexType[16];
char hexCapacity[16];
char hexBlockSize[16];
char hexRCA[16];
char hexID[16];

char tx[16];
char rx[512];

uint8_t* pBuffer;

SD_Error Status = SD_ERROR;
extern SD_CardInfo SDCardInfo;	

void SD_SingleBlockTest(void);
void Fill_Buffer(uint8_t *pBuffer, uint32_t BufferLength, uint32_t Offset);
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint32_t BufferLength);

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
	Status = SD_Init();
	LCD_INIT();
	
	if (Status == SD_OK) LCD_DrawString(10, 10, "SD_Init OK");
	else {
		sprintf(hexStatus, "Status : %d", Status);
		LCD_DrawString(10, 10, "SD_Init Failed");
		LCD_DrawString(10, 20, hexStatus);
	}
	
	sprintf(hexType, "Type : %d", SDCardInfo.CardType);
	sprintf(hexCapacity, "Capacity : %d", SDCardInfo.CardCapacity);
	sprintf(hexBlockSize, "BlockSize : %d", SDCardInfo.CardBlockSize);
	sprintf(hexRCA, "RCA : %d", SDCardInfo.RCA);
	sprintf(hexID, "ID : %d", SDCardInfo.SD_cid.ManufacturerID);
	
	LCD_DrawString(10, 25, hexType);
	LCD_DrawString(10, 40, hexCapacity);
	LCD_DrawString(10, 55, hexBlockSize);
	LCD_DrawString(10, 70, hexRCA);
	LCD_DrawString(10, 85, hexID);
	
	SD_SingleBlockTest(); 
	/**/
}

void SD_SingleBlockTest(void)
{  
  /* Fill the buffer to send */
  Fill_Buffer(Buffer_Block_Tx, BLOCK_SIZE, 0x00);

  if (Status == SD_OK)
  {
    /* Write block of 512 bytes on address 0 */
    Status = SD_WriteBlock(Buffer_Block_Tx, 0x00, BLOCK_SIZE);
		
    /* Check if the Transfer is finished */
    Status = SD_WaitWriteOperation();	  
    while(SD_GetStatus() != SD_TRANSFER_OK); 
  }

  if (Status == SD_OK)
  {
    /* Read block of 512 bytes from address 0 */
    Status = SD_ReadBlock(Buffer_Block_Rx, 0x00, BLOCK_SIZE);
    /* Check if the Transfer is finished */
    Status = SD_WaitReadOperation();
    while(SD_GetStatus() != SD_TRANSFER_OK);
  }

  /* Check the correctness of written data */
  if (Status == SD_OK)
  {
    TransferStatus1 = Buffercmp(Buffer_Block_Tx, Buffer_Block_Rx, BLOCK_SIZE);	
  }	
	
	pBuffer = Buffer_Block_Rx;
	sprintf(tx, "%d", *pBuffer);
	LCD_DrawString(10, 120, tx);
	
  if(TransferStatus1 == PASSED)
    LCD_DrawString(10, 10, "One Block Passed");
 
  else  
    LCD_DrawString(10, 10, "One Block Failed");
}

void Fill_Buffer(uint8_t *pBuffer, uint32_t BufferLength, uint32_t Offset)
{
  uint16_t index = 0;

  /* Put in global buffer same values */
  for (index = 0; index < BufferLength; index++ )
  {
    pBuffer[index] = 4;
  }
}

TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint32_t BufferLength)
{
  while (BufferLength--)
  {
    if (*pBuffer1 != *pBuffer2)
    {
      return FAILED;
    }

    pBuffer1++;
    pBuffer2++;
  }

  return PASSED;
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
