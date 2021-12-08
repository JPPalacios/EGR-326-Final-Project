#include "msp.h"
#include "Timers.h"

volatile uint8_t SysTick_count = 0; 

/* SysTick Timer Setup */
void SysTick_Setup(void){

    SysTick->CTRL = 0;
    SysTick->LOAD = 0x00FFFFFF;
    SysTick->VAL = 0;
    SysTick->CTRL = 0x00000007; // SysTick Interrupt Disabled
}

/* SysTick ms delay */
void SysTick_msDelay(int delay){

    SysTick->LOAD = (delay * SYSTICK_CLK) - 1;
    SysTick->VAL = 0;
    while((SysTick->CTRL & 0x00010000) == 0);
}

/* SysTick us delay */
void SysTick_usDelay(int delay){

    SysTick->LOAD = (delay * (SYSTICK_CLK / 1000)) - 1;
    SysTick->VAL = 0;
    while((SysTick->CTRL & 0x00010000) == 0);
}

/* Timer32 Timer Setup */ 
void Timer32_Setup(void){

    // Motor Timer32
    TIMER32_1->CONTROL  = 0xC2;
    TIMER32_1->LOAD     = TIMER32_LOAD_0 - 1;
    TIMER32_1->CONTROL |= 0x20;

    // Non-Motor Timer32
    TIMER32_2->CONTROL  = 0xC2;
    TIMER32_2->LOAD     = TIMER32_LOAD_0 - 1;
    TIMER32_2->CONTROL |= 0x20;
    
    NVIC->ISER[0] = (1 << (T32_INT1_IRQn & 31));
    NVIC->ISER[0] = (1 << (T32_INT2_IRQn & 31));
    __enable_irq();
}

/* Timer32 1 ms delay */
void Timer32_msdelay(int delay){
    
    TIMER32_1->LOAD = (delay * TIMER32_CLK) - 1;
    while((TIMER32_1->RIS & 1) == 0);   /* wait until the RAW_IFG is set */
    TIMER32_1->INTCLR = 0;              /* clear raw interrupt flag */

}

/* Timer32 1 us delay */
void Timer32_usdelay(int delay){
    
    TIMER32_1->LOAD = (delay * (TIMER32_CLK / 1000)) - 1;
    while((TIMER32_1->RIS & 1) == 0);   /* wait until the RAW_IFG is set */
    TIMER32_1->INTCLR = 0;              /* clear raw interrupt flag */

}

/* ACLK defined at 4 seconds w/ divider */
void Clock_Init48MHz(void){
    // Configure Flash wait-state to 1 for both banks 0 & 1
       FLCTL->BANK0_RDCTL = (FLCTL->BANK0_RDCTL & ~(FLCTL_BANK0_RDCTL_WAIT_MASK)) |
       FLCTL_BANK0_RDCTL_WAIT_1;
       FLCTL->BANK1_RDCTL = (FLCTL->BANK0_RDCTL & ~(FLCTL_BANK0_RDCTL_WAIT_MASK)) |
       FLCTL_BANK1_RDCTL_WAIT_1;

    //Configure HFXT to use 48MHz crystal, source to MCLK & HSMCLK*
       PJ->SEL0 |= BIT2 | BIT3;                     // Configure PJ.2/3 for HFXT function
       PJ->SEL1 &= ~(BIT2 | BIT3);
       CS->KEY = CS_KEY_VAL ;                       // Unlock CS module for register access
       CS->CTL2 |= CS_CTL2_HFXT_EN | CS_CTL2_HFXTFREQ_6 | CS_CTL2_HFXTDRIVE;
          while(CS->IFG & CS_IFG_HFXTIFG)
                    CS->CLRIFG |= CS_CLRIFG_CLR_HFXTIFG;

    /* Select MCLK & HSMCLK = HFXT, no divider */
      CS->CTL1 = CS->CTL1 & ~(CS_CTL1_SELM_MASK     |
                              CS_CTL1_DIVM_MASK     |
                              CS_CTL1_SELS_MASK     |
                              CS_CTL1_DIVHS_MASK)   |
                              CS_CTL1_SELM__HFXTCLK |
                              CS_CTL1_SELS__HFXTCLK;

    CS->CTL1 = CS->CTL1 |CS_CTL1_DIVS_4;    // change the SMCLK clock speed to 3MHz.

    CS->KEY = 0x0000695A;                            //password for CS registers
    CS->CTL1 |= 0b00000010000000000000000000000000;   //dividing ACLCK by 4
    CS->KEY = 0;

    CS->KEY = 0;                            // Lock CS module from unintended accesses
}

void WDT_Reset(void){
    
    WDT_A->CTL = 0x5A00 | 1<<5 | 0<<4 | 1<<3 | 4;  // Watchdog Password, ACLK, Watchdog Mode, Clear Timer
}
