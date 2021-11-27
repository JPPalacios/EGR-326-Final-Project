#ifndef MAIN_H_
#define MAIN_H_

#include "ST7735.h"

#define LCD P9
#define LCD_CLK BIT5
#define LCD_SDA BIT7
#define RS  BIT2
#define RST BIT3
#define LCD_CS  BIT4

void Clock_Init48MHz(void); 


#endif /* MAIN_H_ */
