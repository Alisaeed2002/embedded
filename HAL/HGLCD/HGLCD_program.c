/**
 * @file    HGLCD_program.c
 * @author  Ali Saeed
 * @brief   This file contains logical implementation related to LCD module
 * @version 1.0

 * 
 * 
 */

/***************************************************************************************************/
/*                                             Includes                                            */
/***************************************************************************************************/

#include "LSTD_TYPES.h"
#include "LBIT_MATH.h"
#include "MDIO_interface.h"
#include "avr/io.h"
#include "HGLCD_cfg.h"
#include "HGLCD_interface.h"
#include "util/delay.h"
#include <stdio.h>	
#include "FONT5X7.h"		

/***************************************************************************************************/
/*                                          Important macros                                       */
/***************************************************************************************************/

/*Data byte bits*/
#define BIT_0                  (0)
#define BIT_1                  (1)
#define BIT_2                  (2)
#define BIT_3                  (3)
#define BIT_4                  (4)
#define BIT_5                  (5)
#define BIT_6                  (6)
#define BIT_7                  (7)

/***************************************************************************************************/
/*                                       Functions' definitions                                    */
/***************************************************************************************************/

static void hglcd_sendData(char au8_data) 
{
    /*Setting Data on PORTA*/

    PORTA = au8_data;		/* Copy data on data pin */
	LCD_CONTROL_LINES_PORT |=  (1 << LCD_RS_PIN);	/* Make RS HIGH for data register */
	LCD_CONTROL_LINES_PORT &= ~(1 << LCD_RW_PIN);	/* Make RW LOW for write operation */
	LCD_CONTROL_LINES_PORT |=  (1 << LCD_EN_PIN);	/* HIGH-LOW transition on Enable */
	_delay_us(5);
	LCD_CONTROL_LINES_PORT &= ~(1 << LCD_EN_PIN);
	_delay_us(5);
    
    /*Return from this function*/
    return;
}

static void hglcd_sendCommand(u8_t au8_command)
{
    /*Setting Command on PORTA*/

    PORTA = au8_command;		/* Copy command on data pin */
	LCD_CONTROL_LINES_PORT &= ~(1 << LCD_RS_PIN);	/* Make RS LOW for command register*/
	LCD_CONTROL_LINES_PORT &= ~(1 << LCD_RW_PIN);	/* Make RW LOW for write operation */
	LCD_CONTROL_LINES_PORT |=  (1 << LCD_EN_PIN);	/* HIGH-LOW transition on Enable */
	_delay_us(5);
	LCD_CONTROL_LINES_PORT &= ~(1 << LCD_EN_PIN);
	_delay_us(5);


    /*Return from this function*/
    return;
}

void hglcd_init(void)
{

    Data_Port_Dir = 0xFF;
	Command_Port_Dir = 0xFF;
	/* Select both left & right half of display & Keep reset pin high */
	LCD_CONTROL_LINES_PORT |= (1 << CS1_PIN) | (1 << CS2_PIN) | (1 << RST_PIN);

	_delay_ms(20);
	hglcd_sendCommand(Display_OFF);		/* Display OFF */
	hglcd_sendCommand(Set_Y_address);		/* Set Y address (column=0) */
	hglcd_sendCommand(Set_X_address);		/* Set x address (page=0) */
	hglcd_sendCommand(Set_Z_address);		/* Set z address (start line=0) */
	hglcd_sendCommand(Display_ON);		/* Display ON */

     /*Return from this function*/
    return;
}

void GLCD_ClearAll()			/* GLCD all display clear function */
{
	int i,j;
	/* Select both left & right half of display */
	LCD_CONTROL_LINES_PORT |= (1 << CS1_PIN) | (1 << CS2_PIN);
	for(i = 0; i < 8; i++)
	{
		hglcd_sendCommand((0xB8) + i);/* Increment page */
		for(j = 0; j < 64; j++)
		{
			hglcd_sendData(0);	/* Write zeros to all 64 column */
		}
	}
	hglcd_sendCommand(0x40);		/* Set Y address (column=0) */
	hglcd_sendCommand(0xB8);		/* Set x address (page=0) */
}  

void hglcd_displayString(char page_no,char * str)
{
    unsigned int i, column;
	unsigned int Page = ((0xB8) + page_no);
	unsigned int Y_address = 0;
	float Page_inc = 0.5;
	
	
	
	hglcd_sendCommand(Page);
	for(i = 0; str[i] != 0; i++)	/* Print char in string till null */
	{
		if (Y_address > (1024-(((page_no)*128)+FontWidth)))
		break;
		if (str[i]!=32)
		{
			for (column=1; column<=FontWidth; column++)
			{
				if ((Y_address+column)==(128*((int)(Page_inc+0.5))))
				{
					if (column == FontWidth)
					break;
					hglcd_sendCommand(0x40);
					Y_address = Y_address + column;
					LCD_CONTROL_LINES_PORT ^= (1 << CS1_PIN);
					LCD_CONTROL_LINES_PORT ^= (1 << CS2_PIN);
					hglcd_sendData(Page + Page_inc);
					Page_inc = Page_inc + 0.5;
				}
			}
		}
		if (Y_address>(1024-(((page_no)*128)+FontWidth)))
		break;
		if((font[((str[i]-32)*FontWidth)+4])==0 || str[i]==32)
		{
			for(column=0; column<FontWidth; column++)
			{
				hglcd_sendData(font[str[i]-32][column]);
				if((Y_address+1)%64==0)
				{
					LCD_CONTROL_LINES_PORT ^= (1 << CS1_PIN);
					LCD_CONTROL_LINES_PORT ^= (1 << CS2_PIN);
					hglcd_sendCommand((Page+Page_inc));
					Page_inc = Page_inc + 0.5;
				}
				Y_address++;
			}
		}
		else
		{
			for(column=0; column<FontWidth; column++)
			{
				hglcd_sendData(font[str[i]-32][column]);
				if((Y_address+1)%64==0)
				{
					LCD_CONTROL_LINES_PORT ^= (1 << CS1_PIN);
					LCD_CONTROL_LINES_PORT ^= (1 << CS2_PIN);
					hglcd_sendCommand((Page+Page_inc));
					Page_inc = Page_inc + 0.5;
				}
				Y_address++;
			}
			hglcd_sendData(0);
			Y_address++;
			if((Y_address)%64 == 0)
			{
				LCD_CONTROL_LINES_PORT ^= (1 << CS1_PIN);
				LCD_CONTROL_LINES_PORT ^= (1 << CS2_PIN);
				hglcd_sendCommand((Page+Page_inc));
				Page_inc = Page_inc + 0.5;
			}
		}
	}
	
       // hglcd_sendCommand(0x40);	/* Set Y address (column=0) */

    /*Return from this function*/
    return;
}

void Calculate_Time_Parameters(float *frequency,float *duty_cycle,float *Ton,float *Toff,float *T)
{
    /*Scaling Factor*/
    // 1 Dash will be equivalent to 1 ms

    *T=1/(*frequency);
    float duty11=(*duty_cycle)/100; //changing it from percentage to number
    (*Ton)=duty11*(*T);    //T +ve
    (*Toff)=*T-*Ton;      //T-ve

    /*See the range of The frequency*/
    if(*frequency>28 && *frequency<70)
    {
        *Ton=(*Ton)*300;
        *Toff=(*Toff)*300;
        
    }
    else if(*frequency>90 && *frequency<300)
    {
        *Ton=(*Ton)*2400;
        *Toff=(*Toff)*2400;
        
    }
    else if(*frequency>350 && *frequency<1500)
    {
        *Ton=(*Ton)*8000;
        *Toff=(*Toff)*8000;
        
    }
    else if(*frequency>2000 && *frequency<9000)
    {
       *Ton=(*Ton)*35000;
        *Toff=(*Toff)*35000;
    }

    else
    {
        *Ton=(*Ton)*130000;
        *Toff=(*Toff)*130000;
        
    }







   /*Return from this function*/
    return;
}

void set_column(int y_address)
{

    hglcd_sendCommand(y_address);		/* Set Y address */
	


    return;
    /*Returning forom function*/
}