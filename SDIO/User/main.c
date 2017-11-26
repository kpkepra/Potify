#include "stm32f10x.h"
#include "sdcard.h"
#include "usart.h"
#include "lcd.h"

typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;


#define BLOCK_SIZE            512 /* Block Size in Bytes */

#define NUMBER_OF_BLOCKS      32  /* For Multi Blocks operation (Read/Write) */
#define MULTI_BUFFER_SIZE    (BLOCK_SIZE * NUMBER_OF_BLOCKS)   


uint8_t Buffer_Block_Tx[BLOCK_SIZE], Buffer_Block_Rx[BLOCK_SIZE];
uint8_t readbuff[BLOCK_SIZE];
uint8_t Buffer_MultiBlock_Tx[MULTI_BUFFER_SIZE], Buffer_MultiBlock_Rx[MULTI_BUFFER_SIZE];
volatile TestStatus EraseStatus = FAILED, TransferStatus1 = FAILED, TransferStatus2 = FAILED;
SD_Error Status = SD_ERROR;
extern SD_CardInfo SDCardInfo;	
int i;
 
void SD_EraseTest(void);
void SD_SingleBlockTest(void);
void SD_MultiBlockTest(void);
void Fill_Buffer(uint8_t *pBuffer, uint32_t BufferLength, uint32_t Offset);
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint32_t BufferLength);
TestStatus eBuffercmp(uint8_t* pBuffer, uint32_t BufferLength);

char message1[100];
char hexStatus[16];
char hexType[16];
char hexCapacity[16];
char hexBlockSize[16];
char hexRCA[16];
char hexID[16];

char tx[16];
char rx[512];

uint8_t* pBuffer;

int main(void)
{									   
	
	//NVIC_Configuration();

  //USARTx_Config();	

	Status = SD_Init();
	LCD_INIT();
	
	if(Status == SD_OK) 
	{    
		LCD_DrawString(10, 10, "SD_Init OK");
	}
	else
	{
		sprintf(hexStatus, "Status : %d", Status);
		LCD_DrawString(10, 10, "SD_Init Failed");
		LCD_DrawString(10, 20, hexStatus);
	}			

	sprintf(hexType, "Type : %d", SDCardInfo.CardType);
	LCD_DrawString(10, 25, hexType);
	sprintf(hexCapacity, "Capacity : %d", SDCardInfo.CardCapacity);
	LCD_DrawString(10, 40, hexCapacity);
	sprintf(hexBlockSize, "BlockSize : %d", SDCardInfo.CardBlockSize);
	LCD_DrawString(10, 55, hexBlockSize);
	sprintf(hexRCA, "RCA : %d", SDCardInfo.RCA);
	LCD_DrawString(10, 70, hexRCA);
	sprintf(hexID, "ID : %d", SDCardInfo.SD_cid.ManufacturerID);
	LCD_DrawString(10, 85, hexID);

	//SD_EraseTest();
	
	SD_SingleBlockTest(); 
	
	//SD_MultiBlockTest();  

	while (1)
	{}
}


void SD_EraseTest(void)
{
  if (Status == SD_OK)
  {    
    Status = SD_Erase(0x00, (BLOCK_SIZE * NUMBER_OF_BLOCKS));
  }

  if (Status == SD_OK)
  {	
    Status = SD_ReadMultiBlocks(Buffer_MultiBlock_Rx, 0x00, BLOCK_SIZE, NUMBER_OF_BLOCKS);   
    Status = SD_WaitReadOperation(); 
    while(SD_GetStatus() != SD_TRANSFER_OK);
  }
 
  if (Status == SD_OK)
  {	
    EraseStatus = eBuffercmp(Buffer_MultiBlock_Rx, MULTI_BUFFER_SIZE);
  }
  
  if(EraseStatus == PASSED)
		LCD_DrawString(10, 10, "Erase Passed");
 
  else	  
  	LCD_DrawString(10, 10, "Erase Failed");
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

void SD_MultiBlockTest(void)
{
  /* Fill the buffer to send */
  Fill_Buffer(Buffer_MultiBlock_Tx, MULTI_BUFFER_SIZE, 0x0);

  if (Status == SD_OK)
  {
    /* Write multiple block of many bytes on address 0 */
    Status = SD_WriteMultiBlocks(Buffer_MultiBlock_Tx, 0x00, BLOCK_SIZE, NUMBER_OF_BLOCKS);
    /* Check if the Transfer is finished */
    Status = SD_WaitWriteOperation();
    while(SD_GetStatus() != SD_TRANSFER_OK);
  }

  if (Status == SD_OK)
  {
    /* Read block of many bytes from address 0 */
    Status = SD_ReadMultiBlocks(Buffer_MultiBlock_Rx, 0x00, BLOCK_SIZE, NUMBER_OF_BLOCKS);
    /* Check if the Transfer is finished */
    Status = SD_WaitReadOperation();
    while(SD_GetStatus() != SD_TRANSFER_OK);
  }

  /* Check the correctness of written data */
  if (Status == SD_OK)
  {
    TransferStatus2 = Buffercmp(Buffer_MultiBlock_Tx, Buffer_MultiBlock_Rx, MULTI_BUFFER_SIZE);
  }
  
  if(TransferStatus2 == PASSED)	  
    LCD_DrawString(10, 10, "Multi Block Passed");

  else 
  	LCD_DrawString(10, 10, "Multi Block Failed");

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


void Fill_Buffer(uint8_t *pBuffer, uint32_t BufferLength, uint32_t Offset)
{
  uint16_t index = 0;

  /* Put in global buffer same values */
  for (index = 0; index < BufferLength; index++ )
  {
    pBuffer[index] = 1;
  }
}

TestStatus eBuffercmp(uint8_t* pBuffer, uint32_t BufferLength)
{
  while (BufferLength--)
  {
    /* In some SD Cards the erased state is 0xFF, in others it's 0x00 */
    if ((*pBuffer != 0xFF) && (*pBuffer != 0x00))
    {
      return FAILED;
    }

    pBuffer++;
  }

  return PASSED;
}

/***************************END OF FILE*******************************/
