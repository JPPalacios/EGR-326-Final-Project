#include "msp.h"
#include "Keypad.h"
#include "SysTick.h"
#include <stdio.h>

/*---------------------------------------------------
    Keypad Setup Function: GPIO w/ pull-up resistors
    @parameters: none
    @returns:    none
---------------------------------------------------*/ 
void keypad_setup(){

    P4->SEL0 &= ~0x7F;
    P4->SEL1 &= ~0x7F;
    P4->DIR  &= ~0x7F; //clear rows and cols - bits 0-6
    P4->REN  |=  0x0F; //resistor enable for rows and cols - bits 0-6
    P4->OUT  |=  0x0F; //pull up - bits 0-6
}

/*---------------------------------------------------
    Keypad Read Function: Scans the rows, cols for key
    press, prints keypress (debug purposes)
    @parameters: none
    @returns:    1 - key press, 0 - no key press
---------------------------------------------------*/ 
uint8_t read_keypad(){

    uint8_t col; //var for columns
    uint8_t row; //var for counting rows

    for(col = 0; col < 3; col++){ //columns 0-2
        P4->DIR = 0;                                    //reset direction
        P4->DIR |= BIT(4 + col);                        //set direction to current column bit
        P4->OUT &= ~BIT(4 + col);                       //clear direction column
        SysTick_ms_delay(10);
        row = (P4->IN & 0x0F);                          //set all rows for reading

        while((!(P4->IN & BIT0)) | (!(P4->IN & BIT1)) | (!(P4->IN & BIT2)) | (!(P4->IN & BIT3))); //if button held nothing prints

        if(row != 0x0F) //if row is pressed for some col
            break; //continue
    }

    P4->DIR = 0; //reset direction
    SysTick_ms_delay(100);

    if(col == 3) //if all 3 cols are done being scanned thru, redo fcn to scan again
        return 0; //exit

    if(row == 0x0E)   num = (col + 1);      //row 1 - lower nibble 14
    if(row == 0x0D)   num = (col + 4);      //row 2 - lower nibble 13
    if(row == 0x0B)   num = (col + 7);      //row 3 - lower nibble 12
    if(row == 0x07)   num = (col + 10);     //row 4 - lower nibble 7

    if(num == 11){
        num = 0;    // quick fix: returns an actual zero instead of 0x
    }

    if (num <= 9)     printf("%d\n", num);
    if (num == 10)    printf("*\n");
    if (num == 0)     printf("%d\n", num);
    if (num == 12)    printf("#\n");

    return 1;                               //exit
}
