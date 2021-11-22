#include "msp.h"

#define PERIOD 3000
#define HALFSEC 500
#define TURN_SIG P3
#define TURN_BLINK BIT3


void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

    TURN_SIG->SEL0 &= ~TURN_BLINK;
    TURN_SIG->SEL1 &= ~TURN_BLINK;
    TURN_SIG->DIR |= TURN_BLINK;

    while(1){

        TURN_SIG->OUT ^= TURN_BLINK;
        __delay_cycles(PERIOD * HALFSEC);

    }
}
