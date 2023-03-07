#include <msp430.h> 
#include "LCD_I2C.H"

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

void initTimerA0(void); //Timer for counting 10 seconds
void initTimerA1(void); //Timer for PWM

void delayMS(int msecs); //Creating delay with TimerA0

unsigned int controlHeater = 0;

unsigned int Count_1ms;

unsigned int Set_Temp;
int adc_value;
unsigned int current_temperature;
char str[20];

unsigned int adc_read(unsigned int channel);//Function to read analog value from sensor

int scan_keypad(void);


void  main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	BCSCTL1 = CALBC1_1MHZ;
	DCOCTL = CALDCO_1MHZ;

	P2DIR |= BIT2;  //P2.2 Port have been made output for pwm
	P2SEL |= BIT2;  // Changed from GPIO to PWM

	P1DIR = 0x00;
	P1DIR |= BIT0 + BIT1 + BIT2 + BIT3; //P1.0,P1.1,P1.2,P1.3 are rows
	P1OUT = 0x00;
	P1REN |= BIT4; //Pull down for column1 P1.4
	P1OUT &= ~(BIT4);//Make sure that column bits are zero

	P2REN |= BIT0 + BIT1; //pull down for P2.0,P2.1  2,3.column
	P2OUT &= ~(BIT0 + BIT1);


	__enable_interrupt();
	initTimerA0(); //Configure TimerA0
	initTimerA1(); //Configure TimerA1

	lcd_init(0x27);
	set_backlight();
	lcd_setcursor(0,0);
	lcd_print("Set Temp:");
	Set_Temp = scan_keypad();

	sprintf(str, "%d", Set_Temp);
	lcd_print(str);
	TA1CCR0 = 1000;
	while(1)
	{
	    adc_value = adc_read(INCH_5); //read from channel5
	    current_temperature = (3.6*(float)adc_value*100)/1023.0;
	    lcd_setcursor(1, 0);
	    sprintf(str, "%d", current_temperature);
	    lcd_print(str);
	    if(current_temperature < Set_Temp)
	    {
	        TA1CCR1 = 800; //Set PWM with %95 duty cycle
	    }
	    else if (current_temperature >= Set_Temp)
	    {
	        controlHeater = 1;
	        delayMS(10000); //10 saniye bekle;
	    }
	}

}



void initTimerA0(void)
{
    TA0CCR0 = 0; //initially stop the timer
    TA0CCTL0 |= CCIE; //Enable interrupt for TA0CCR0
    TA0CTL = TASSEL_2 + ID_0 + MC_1; //SMCLK, no division, up mode
}

void initTimerA1(void)
{
    TA1CCR0 = 0;
    TA1CCTL1 = OUTMOD_7; //Set Reset Mode
    TA1CTL = TASSEL_2 + ID_0 + MC_1;
}

void delayMS(int msecs)
{
    Count_1ms = 0; //Reset
    TA0CCR0 = 999; //1ms

    while(Count_1ms <= msecs);


    TA0CCR0 = 0; //Stop timer
}


#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A_CCR0_ISR(void)
{
    Count_1ms++;
    if(controlHeater)
    {
        TA1CCR1 = 0;
    }

}
int scan_keypad(void)
{
    unsigned int i = 0;
    char scanned_value[3];
    unsigned int temperature_setted;
    while(scanned_value[2] != '#')
    {
        P1OUT |= BIT0;
        delayMS(1);
        if((P1IN & BIT4))
        {
            while((P1IN & BIT4));
            scanned_value[i] = '1';
            i++;
        }
        else if((P2IN & BIT1))
        {
            while((P2IN & BIT1));
            scanned_value[i] = '2';
            i++;
        }
        else if((P2IN & BIT0))
        {
            while((P2IN & BIT0));
            scanned_value[i] = '3';
            i++;
        }
        P1OUT &= ~BIT0;

        P1OUT |= BIT1;
        delayMS(1);
        if((P1IN & BIT4))
        {
            while((P1IN & BIT4));
            scanned_value[i] = '4';
            i++;
        }
        else if((P2IN & BIT1))
        {
            while((P2IN & BIT1));
            scanned_value[i] = '5';
            i++;
        }
        else if((P2IN & BIT0))
        {
            while((P2IN & BIT0));
            scanned_value[i] = '6';
            i++;
        }
        P1OUT &= ~BIT1;

        P1OUT |= BIT2;
        delayMS(1);
        if((P1IN & BIT4))
        {
            while((P1IN & BIT4));
            scanned_value[i] = '7';
            i++;
        }
        else if((P2IN & BIT1))
        {
            while((P2IN & BIT1));
            scanned_value[i] = '8';
            i++;
        }
        else if((P2IN & BIT0))
        {
            while((P2IN & BIT0));
            scanned_value[i] = '9';
            i++;
        }
        P1OUT &= ~BIT2;

        P1OUT |= BIT3;
        delayMS(1);
        if((P1IN & BIT4))
        {
            continue;
        }
        else if((P2IN & BIT1))
        {
            while((P2IN & BIT1));
            scanned_value[i] = '0';
            i++;
        }
        else if((P2IN & BIT0))
        {
            while((P2IN & BIT0));
            scanned_value[i] = '#';
            i++;
        }
        P1OUT &= ~BIT3;
    }


    temperature_setted = ((scanned_value[0] - '0')*10) + (scanned_value[1] - '0');
    return temperature_setted;
}

unsigned int adc_read(unsigned int channel)
{
    ADC10CTL0 &= ~ENC; // Disabled ADC
    ADC10CTL0 = ADC10SHT_2 + ADC10ON; // 8 clock ticks, ADC on

    ADC10CTL1 = ADC10SSEL_3 + channel + CONSEQ_2 + SHS_0 + MSC; //Set 'chan', SMCLK
    ADC10CTL0 |= ENC + ADC10SC;
    ADC10AE0 = BIT5;

    while(!(ADC10CTL0 & ADC10IFG));

    return ADC10MEM;
}
