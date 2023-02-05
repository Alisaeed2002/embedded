/**
 * @file    HGLCD_cfg.h
 * @author  Ali Saeed 
 * @brief   This file contains configuration information related to LCD module
 * @version 1.0
 
 * 
 * 
 */

/*Header file guard*/
#ifndef __HGLCD_CFG_H__
#define __HGLCD_CFG_H__

/***************************************************************************************************/
/*                                         Configuration macros                                    */
/***************************************************************************************************/

/*LCD Control lines configurations*/
#define LCD_CONTROL_LINES_PORT      (PORTC)
#define LCD_RS_PIN                  (PIN0)
#define LCD_RW_PIN                  (PIN1)
#define LCD_EN_PIN                  (PIN2)
#define CS1_PIN		                (PIN3)
#define CS2_PIN		                (PIN4)
#define RST_PIN	                    (PIN5)	 

#define Command_Port_Dir            (DDRC)		/* Define command port for GLCD */


/*LCD Data lines configurations*/
#define LCD_DATA_LINES_PORT         (PORTA)
#define LCD_A0_PIN                  (PIN0)
#define LCD_A1_PIN                  (PIN1)
#define LCD_A2_PIN                  (PIN2)
#define LCD_A3_PIN                  (PIN3)
#define LCD_A4_PIN                  (PIN4)
#define LCD_A5_PIN                  (PIN5)
#define LCD_A6_PIN                  (PIN6)
#define LCD_A7_PIN                  (PIN7)
#define Data_Port_Dir	            (DDRA)		/* Define data port for GLCD */

/*LCD Instructions*/
#define Display_OFF   (0x3E)		
#define Set_Y_address (0x40)	   //column	
#define Set_X_address (0xB8)	  //row 
#define Set_Z_address (0xC0)	  //start line 
#define Display_ON    (0x3F)	

#endif /*__HLCD_CFG_H__*/