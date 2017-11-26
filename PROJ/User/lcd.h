#ifndef __LCD_H
#define	__LCD_H


#include "stm32f10x.h"



#define      FSMC_Addr_LCD_CMD         ( ( uint32_t ) 0x60000000 )	    
#define      FSMC_Addr_LCD_DATA        ( ( uint32_t ) 0x60020000 )          

#define      FSMC_Bank1_NORSRAMx           FSMC_Bank1_NORSRAM1



#define      LCD_CS_APBxClock_FUN      RCC_APB2PeriphClockCmd
#define      LCD_CS_CLK                RCC_APB2Periph_GPIOD    
#define      LCD_CS_PORT               GPIOD   
#define      LCD_CS_PIN                GPIO_Pin_7

#define      LCD_DC_APBxClock_FUN      RCC_APB2PeriphClockCmd
#define      LCD_DC_CLK                RCC_APB2Periph_GPIOD    
#define      LCD_DC_PORT               GPIOD   
#define      LCD_DC_PIN                GPIO_Pin_11

#define      LCD_RST_APBxClock_FUN     RCC_APB2PeriphClockCmd
#define      LCD_RST_CLK               RCC_APB2Periph_GPIOE   
#define      LCD_RST_PORT              GPIOE
#define      LCD_RST_PIN               GPIO_Pin_1

#define      LCD_BK_APBxClock_FUN      RCC_APB2PeriphClockCmd
#define      LCD_BK_CLK                RCC_APB2Periph_GPIOD    
#define      LCD_BK_PORT               GPIOD
#define      LCD_BK_PIN                GPIO_Pin_12



#define      DEBUG_DELAY()                



#define      LCD_Default_Max_COLUMN		  240     
#define      LCD_Default_Max_PAGE   		320     



#define      LCD_DispWindow_Start_COLUMN		0     
#define      LCD_DispWindow_Start_PAGE	    0     

#define      LCD_DispWindow_COLUMN		  240     
#define      LCD_DispWindow_PAGE		    320     



#define      WIDTH_EN_CHAR		                 8	      
#define      HEIGHT_EN_CHAR		              16		    


#define      WIDTH_CH_CHAR		                16	    
#define      HEIGHT_CH_CHAR		              16		  

#define      GetGBKCode( ucBuffer, usChar )  	 


#define      BACKGROUND		                 WHITE   

#define      WHITE		 		                 0xFFFF	   
#define      BLACK                         0x0000	 
#define      GREY                          0xF7DE	  
#define      BLUE                          0x001F	  
#define      BLUE2                         0x051F	   
#define      RED                           0xF800	  
#define      MAGENTA                       0xF81F	 
#define      GREEN                         0x07E0	    
#define      CYAN                          0x7FFF	   
#define      YELLOW                        0xFFE0	 
#define      BRED                          0xF81F
#define      GRED                          0xFFE0
#define      GBLUE                         0x07FF



#define      CMD_Set_COLUMN		 		    0x2A	     
#define      CMD_Set_PAGE		 		      0x2B	     
#define      CMD_SetPixel		 		      0x2C	     

void                     LCD_INIT			               ( void );
void                     LCD_Rst                     ( void );
void                     LCD_BackLed_Control         ( FunctionalState enumState );
__inline void            LCD_Write_Cmd               ( uint16_t usCmd );
__inline void            LCD_Write_Data              ( uint16_t usData );
__inline uint16_t        LCD_Read_Data               ( void );
void                     LCD_OpenWindow              ( uint16_t usC, uint16_t usP, uint16_t usWidth, uint16_t usHeight );
void                     LCD_Clear                   ( uint16_t usC, uint16_t usP, uint16_t usWidth, uint16_t usHeight, uint16_t usColor );
void                     LCD_DrawDot           			 ( uint16_t usC, uint16_t usP, uint16_t usColor );
uint16_t                 LCD_GetPointPixel           ( uint16_t usC , uint16_t usP );
void                     LCD_DrawLine                ( uint16_t usC1, uint16_t usP1, uint16_t usC2, uint16_t usP2, uint16_t usColor );
void                     LCD_DrawChar             	 ( uint16_t usC, uint16_t usP, const char cChar);
void                     LCD_DrawString           	 ( uint16_t usC, uint16_t usP, const char * pStr);
void 										 LCD_DrawCircle							 ( uint16_t usC, uint16_t usP, uint16_t R, uint16_t usColor);



#endif 

