#include "sdcard_user.h"

SD_Error Status;
extern SD_CardInfo SDCardInfo;	
char hexStatus[16];
char hexType[16];
char hexCapacity[16];
char hexBlockSize[16];
char hexRCA[16];
char hexID[16];

volatile TransferStatus1 = FAILED;
uint8_t Buffer_Block_Tx[BLOCK_SIZE], Buffer_Block_Rx[BLOCK_SIZE];

char buffer_tx[16];
char buffer_rx[16];

uint8_t* pBuffer;

void SDCard_Init(void) {
	Status = SD_Init();
	if (Status == SD_OK) LCD_DrawString(10, 10, "SD_Init OK");
	else {
		sprintf(hexStatus, "Status : %d", Status);
		LCD_DrawString(10, 25, hexStatus);
	}
	
	//sprintf(hexType, "Type : %d", SDCardInfo.CardType);
	//sprintf(hexCapacity, "Capacity : %d", SDCardInfo.CardCapacity);
	sprintf(hexBlockSize, "Block Size : %d", SDCardInfo.CardBlockSize);
	//sprintf(hexRCA, "RCA : %d", SDCardInfo.RCA);
	//sprintf(hexID, "ID : %d", SDCardInfo.SD_cid.ManufacturerID);
	
	//LCD_DrawString(10, 25, hexType);
	//LCD_DrawString(10, 40, hexCapacity);
	LCD_DrawString(10, 40, hexBlockSize);
	//LCD_DrawString(10, 70, hexRCA);
	//LCD_DrawString(10, 85, hexID);
}

void SDCard_StoreData(uint32_t addr, const char * pStr) {
	uint16_t index = 0;
	for (index = 0; index < 4; index++) {
		Buffer_Block_Tx[index] = *pStr;
		pStr++;
	}
	
	 if (Status == SD_OK) {
    /* Write block of 512 bytes on address 0 */
    Status = SD_WriteBlock(Buffer_Block_Tx, 0x200, BLOCK_SIZE);
		
    /* Check if the Transfer is finished */
    Status = SD_WaitWriteOperation();	  
    while(SD_GetStatus() != SD_TRANSFER_OK); 
  }
	 
	if (Status == SD_OK) {
    /* Read block of 512 bytes from address 0 */
    Status = SD_ReadBlock(Buffer_Block_Rx, 0x000, BLOCK_SIZE);
    /* Check if the Transfer is finished */
    Status = SD_WaitReadOperation();
    while(SD_GetStatus() != SD_TRANSFER_OK);
  }
	
	pBuffer = Buffer_Block_Rx;
	sprintf(buffer_tx, "%c", *pBuffer);
	LCD_DrawString(10, 120, buffer_tx);
}

void SD_SingleBlockTest(void)
{  
  /* Fill the buffer to send */
  Fill_Buffer(Buffer_Block_Tx, BLOCK_SIZE, 0x00);

  if (Status == SD_OK) {
    /* Write block of 512 bytes on address 0 */
    Status = SD_WriteBlock(Buffer_Block_Tx, 0x00, BLOCK_SIZE);
		
    /* Check if the Transfer is finished */
    Status = SD_WaitWriteOperation();	  
    while(SD_GetStatus() != SD_TRANSFER_OK); 
  }

  if (Status == SD_OK) {
    /* Read block of 512 bytes from address 0 */
    Status = SD_ReadBlock(Buffer_Block_Rx, 0x00, BLOCK_SIZE);
    /* Check if the Transfer is finished */
    Status = SD_WaitReadOperation();
    while(SD_GetStatus() != SD_TRANSFER_OK);
  }

  /* Check the correctness of written data */
  if (Status == SD_OK)									TransferStatus1 = Buffercmp(Buffer_Block_Tx, Buffer_Block_Rx, BLOCK_SIZE);	
	
  if (TransferStatus1 == PASSED) LCD_DrawString(10, 55, "One Block Passed");
  else																				LCD_DrawString(10, 55, "One Block Failed");
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