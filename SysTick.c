#include "msp.h"
#include "SysTick.h"

/*---------------------------------------------------
    SysTick Timer Setup: Interrupts Enabled
    @parameters: none
    @returns:    none
---------------------------------------------------*/ 
void SysTick_setup(){

    SysTick->CTRL = 0;
    SysTick->LOAD = 0x00FFFFFF; // 3,000,000 counts/second
    SysTick->VAL = 0;
    SysTick->CTRL = 0x00000007; // SysTick Interrupt Enabled
}

/*---------------------------------------------------
    SysTick Milliseconds Delay:
    @parameters: delay - time delay in milliseconds
    @returns:    none
---------------------------------------------------*/ 
void SysTick_ms_delay(uint16_t delay){

    SysTick->LOAD = delay * 3000;
    SysTick->VAL = 0;
    while((SysTick->CTRL & 0x00010000));
}
