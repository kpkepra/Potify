#include "sdcard.h"
#include "lcd.h"
#include <stdio.h>

#define BLOCK_SIZE            512 /* Block Size in Bytes */

typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

void SDCard_Init(void);
void SDCard_StoreData(uint32_t addr, const char * pStr);
uint8_t SDCard_ReadData(uint32_t addr);
void SD_SingleBlockTest(void);
void Fill_Buffer(uint8_t *pBuffer, uint32_t BufferLength, uint32_t Offset);
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint32_t BufferLength);