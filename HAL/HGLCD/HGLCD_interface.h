/**
 * @file    HGLCD_interface.h
 * @author  Ali Saeed
 * @brief   This file contains interfacing information related to LCD module
 * @version 1.0
 
 * 
 * 
 */

/*Header file guard*/
#ifndef __HGLCD_INTERFACE_H__
#define __HGLCD_INTERFACE_H__

/***************************************************************************************************/
/*                                         Interfacing macros                                      */
/***************************************************************************************************/





/***************************************************************************************************/
/*                                        Functions' prototypes                                    */
/***************************************************************************************************/

/**
 * @brief This function is used to initialize the LCD module
 * 
 */
void hglcd_init(void);

/**
 * @brief This function is responsible for displaying a string over LCD
 * 
 * @param pu8_stringData 
 */
void hglcd_displayString(char page_no,char* str);

/**
 * @brief This function is responsible for displaying a PWM signal over LCD
 * 
 * @param frequency_Duty 
 */
void Calculate_Time_Parameters(float *frequency,float *duty_cycle,float *Ton,float *Toff,float *T);


/**
 * @brief This function is responsible for clearing GLCD
 */

void GLCD_ClearAll();

/**
 * @brief This function is responsible for setting position of display
 * 
 * @param Postition
 */
void set_column(int y_address);

#endif /*__HGLCD_INTERFACE_H__*/