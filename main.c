#include "LSTD_TYPES.h"
#include "LBIT_MATH.h"
#include "avr/io.h"
#include "MDIO_interface.h"
#include "MTIMER_interface.h"
#include "HGLCD_interface.h" 
#include "HGLCD_cfg.h"
#include <stdio.h>
#include <string.h>
int main(void)
{

    
    /*Printing default set up for PWM Drawer*/
    char duty_display[50];
    char frequency_display[50];

    float Ton,Toff,Period;
    char Ton1[10];
    char Toff1[10];
    

    u8_t duty;    //in percentage 
    u32_t frequency;  // in hz
   


    hglcd_init();		/* Initialize GLCD */
	GLCD_ClearAll();	/* Clear all GLCD display */


    mdio_setPinStatus(PORTB, PIN3, OUTPUT);         //Generates PWM Signal
    mdio_setPinStatus(PORTD,PIN6,INPUT_FLOAT);      //Recieves PWM Signal 

    mtimer_init(TIMER_CHANNEL_0, TIMER_PWM_MODE, TIMER_FREQ_30_HZ);
    mtimer_init(TIMER_CHANNEL_1, TIMER_ICU_MODE, TIMER_ICU_PRESCALER);

    mtimer_startPWM_signal(TIMER_CHANNEL_0);
    mtimer_runPWM_signal(TIMER_CHANNEL_0, 128);    //Duty Cycle ~= 50 %


    while(1)
    {

    mtimer_getSignal_duty_and_freq(&duty,&frequency);
     

    /*Converting from integer to string in order to print on glcd*/
    int duty1=duty;
    int freq1=frequency;

    float freq_temp=freq1;
    float duty_temp=duty1;   /*Used in Displaying The signal*/

    if(freq1>1000)
    {   
        freq1=freq1/1000;

    /*Displaying default set-up for PWM drawer*/

    mdio_setPinValue(PORTC,PIN4,LOW); 
	mdio_setPinValue(PORTC,PIN3,HIGH);           /*Choosing the Left Half to display duty*/

    hglcd_displayString(0,"Freq:     KHz");

    mdio_setPinValue(PORTC,PIN4,HIGH); 
	mdio_setPinValue(PORTC,PIN3,LOW);           /*Choosing the right Half to display duty*/

    hglcd_displayString(0,"Duty:    %");
    }
    else
    {
        
     /*Displaying default set-up for PWM drawer*/
     mdio_setPinValue(PORTC,PIN4,LOW); 
	mdio_setPinValue(PORTC,PIN3,HIGH);           /*Choosing the Left Half to display duty*/

    hglcd_displayString(0,"Freq:    Hz");

    mdio_setPinValue(PORTC,PIN4,HIGH); 
	mdio_setPinValue(PORTC,PIN3,LOW);           /*Choosing the right Half to display duty*/

    
    hglcd_displayString(0,"Duty:    %");

    }

    sprintf(duty_display, "%d", duty1);
    sprintf(frequency_display, "%d", freq1);    /*converting integers to string*/



    mdio_setPinValue(PORTC,PIN4,LOW); 
	mdio_setPinValue(PORTC,PIN3,HIGH);           /*Choosing the Left Half to display duty*/

    set_column(95);
    hglcd_displayString(0,frequency_display);


    mdio_setPinValue(PORTC,PIN4,HIGH); 
	mdio_setPinValue(PORTC,PIN3,LOW);           /*Choosing the right Half to display duty*/

    set_column(105);
    hglcd_displayString(0,duty_display);



/********************************************DISPLAYING SIGNAL****************************************************/

/*Make both cs1 & cs2 HIGH to replicate the wave*/

/*Choosing both halves*/

mdio_setPinValue(PORTC,PIN4,HIGH); 
mdio_setPinValue(PORTC,PIN3,HIGH); 


set_column(0x40);   /*Starting at Y= 0*/
Calculate_Time_Parameters(&freq_temp,&duty_temp,&Ton,&Toff,&Period);
for (int j=0;j<Ton;j++)
{
    Ton1[j]='-';
}
for (int j=0;j<Toff;j++)
{
    Toff1[j]='-';
}



    set_column(0x40);   /*Starting at Y= 0*/
    hglcd_displayString(6,Ton1);
    
    hglcd_displayString(3,Toff1);

    
    

    while(1);
     

    }
  
    return 0;
}