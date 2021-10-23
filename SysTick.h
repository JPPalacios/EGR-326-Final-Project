#ifndef SYSTICK_H_
#define SYSTICK_H_

void SysTick_setup();

void SysTick_ms_delay(uint16_t delay);

uint16_t delay; 

#endif /* SYSTICK_H_ */