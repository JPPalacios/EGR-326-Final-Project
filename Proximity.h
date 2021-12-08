#ifndef   PROXIMITY_H_
#define   PROXIMITY_H_

#define PROX      P2 //macros
#define PROX_TRIG P10
#define TRIG      BIT5
#define ECHO      BIT5
#define SOUND     BIT7
#define PROX_LED  BIT4

/* Proximity Sensor */
void TimerA3_Setup(void);
void TimerA0_Setup(void);
void read_Proximity(void);

/* ---------- Global Variables ---------- */
int read_data;
float period_irq, actualIN, actualCM, proxLED;

int timerA_flag, blink_flag;



#endif /* PROXIMITY_H_ */

