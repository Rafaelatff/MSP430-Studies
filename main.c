//Using Code Composer Studio - Version: 10.2.0.00009


#include <msp430.h> 


// Global variables
int x=0;


void GPIOs_devkit_start(void);
void SW_read(void);
void clock_config(void);


/**
 * main.c
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	//For FRAM devices, at start up, the GPO power-on default
	//high-impedance mode needs to be disabled to activate previously
	//configured port settings. This can be done by clearing the LOCKLPM5
	//bit in PM5CTL0 register
	PM5CTL0 &= ~LOCKLPM5; //without this, code doesn`t work

	GPIOs_devkit_start(); //Configure GPIOs of devkit;
	SW_read(); //Testing S1 and S2 of devkit;
	clock_config();

	
	while(1){
	    P5OUT ^= BIT1;
	}

}

void clock_config(void){
    //With nothing configurated, and toggle command inside while(1) returns a frequency of 75KHz

    //DCOCLK: Internal Digitally Controlled Oscilator (up to 16 MHz)
    //VLOCLK: Internal Very Low Oscilator (10 kHz)
    //LFXT: External 32.768 kHz Oscilator (XT1)
    //REFO: Integrated 32-kHz Oscillator

    //MCLK: Main Clock Source
    //SMCLK: Sub-Main Clock Source (derives from MCLK)
    //ACLK: Auxiliary Clock Source


    //Frequency of DCO  on P1.0?
    // PLL Frequency on P1.1?

    //XTAL are connected to P2.0 XOUT and P2.1 XIN
    P2SEL0 |= BIT0;
    P2SEL1 &= ~BIT0;

    P2SEL0 |= BIT1;
    P2SEL1 &= ~BIT1;


}

void GPIOs_devkit_start(void){

    //Fist, configure the Pin as GPIO, setting the PxSEL0 and PxSEL1 to low;
    //To configure the GPIOs, as output, the PxDIR must be high;
    //To configure the GPIOs, as input, the PxDIR must be low;
    //Configure pull-up resistors for inputs;

    //----------Setting all physical connections of LP-MSP430FR2476------------//

    // Set RGB LEDs (LED2) as output direction
    // LED Blue: Port P4 / Pin 7 [J8.3 needs to be connected]
    P4SEL0 &= ~BIT7;
    P4SEL1 &= ~BIT7;
    P4DIR |= BIT7; // Set blue LED as Output
    P4OUT &= ~BIT7; // turn off blue LED

    // LED Green: Port P5 / Pin 0 [J8.2 needs to be connected]
    P5SEL0 &= ~BIT0;
    P5SEL1 &= ~BIT0;
    P5DIR |= BIT0; // Set green LED as Output
    P5OUT &= ~BIT0; // turn off green LED

    // LED Red: Port P5 / Pin 1 [J8.1 needs to be connected]
    P5SEL0 &= ~BIT0;
    P5SEL1 &= ~BIT0;
    P5DIR |= BIT1; // Set red LED as Output
    P5OUT &= ~BIT1; // turn off red LED

    // Set LED1 as output direction [J7 needs to be connected]
    // LED White: Port P1 / Pin 0
    P1SEL0 &= ~BIT0;
    P1SEL1 &= ~BIT0;
    P1DIR |= BIT0; // Set LED1 as Output
    P1OUT &= ~BIT0; // turn off LED1

    //Set P4.0 (S1) as input
    //Switch S1: Port P4 / Pin 0 - closed is 0
    //In development kit, circuit has an pull-up resistor, but it is DNP (R9 - P4.0)
    P4SEL0 &= ~BIT0;
    P4SEL1 &= ~BIT0;
    P4DIR &= (~BIT0); //Set P4.0 as input
    //P4REN &= ~BIT0; //Turn off pull up/down resistor
    P4REN |= BIT0; // Turn on pull up/down resistor
    P4OUT |= BIT0; // Select pull up resistor

    //Set P2.3 (S2) as input
    //Switch S1: Port P2 / Pin 3 - closed is 0
    //In development kit, circuit has an pull-up resistor, but it is DNP (R10 - P2.3)
    P2SEL0 &= ~BIT3;
    P2SEL1 &= ~BIT3;
    P2DIR &= ~BIT3; //Set P2.3 as input
    //P2REN &= ~BIT3; //Turn off pull up/down resistor
    P2REN |= BIT3; // Turn on pull up/down resistor
    P2OUT |= BIT3; // Select pull up resistor

    //Examples for output
    //P5OUT |= BIT1; // Turn red LED on (P5.1 high)
    //P5OUT &= ~BIT1; Turn red LED off (P5.1 low)
    //P5OUT ^= BIT1; // Toggle red LED

}

void SW_read(void){
    //Local variables
    char SW2=0;

    //Simplest way of reading input P4.0
    if ((P4IN & BIT0) == 0){ //if P4.0 is low (S1 closed), then:
        P1OUT |= (BIT0); // Turn LED1 on;
    }
    else { //else, P4.0 is not low, then:
        P1OUT &= (~BIT0); // Turn LED1 off;
    }

    //Different way, depends on a global/local variable, but works the same as above
    SW2 = ((P2IN & BIT3)? 1 : 0); // Variable receives status of P2.3
    if (SW2 == 0){ // Variable is 0, then:
        P4OUT |= (BIT7); // Turn blue led on;
    }
    else if (SW2 == 1){ //else, variable is 1, then:
        P4OUT &= (~BIT7); //Turns blue led off;
    }
}


