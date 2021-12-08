#ifndef TIMERS_H_
#define TIMERS_H_

#define SYSTICK_CLK 48000
#define TIMER32_CLK 48000
#define TIMER32_LOAD_0 48000000


void SysTick_Setup(void);
void SysTick_msDelay(int delay);
void SysTick_usDelay(int delay);

void Timer32_Setup(void);
void Timer32_msdelay(int delay);
void Timer32_usdelay(int delay); 

void Clock_Init48MHz(void);

void WDT_Reset(void); 

#endif /* TIMERS_H_ */
