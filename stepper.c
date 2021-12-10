#include "msp.h"
#include "stdio.h"
#include "stepper.h"
#include "Timers.h"


uint8_t i = 0, j = 0;

/* Hall Effect Sensor Setup */
void hall_Setup(void){

    // Hall Effect Sensor
    GPIO->SEL0 &=~ HALL;    // GPIO Input
    GPIO->SEL1 &=~ HALL;
    GPIO->DIR  &=~ HALL;
    GPIO->REN  |=  HALL;    // Internal Pull-up Resistor
    GPIO->OUT  &=~ HALL;
    GPIO->IE   |=  HALL;
    GPIO->IES  |=  HALL;
    GPIO->IFG  &=~ HALL;
    NVIC->ISER[1] = 1 << ((PORT2_IRQn) & 31); // Enabled interrupts for pushODO_BTN detection
    __enable_interrupt();
}

/* Bipolar Stepper Motor Setup */
void stepper_Setup(void){

    // Tachometer and Speedometer
    BIPOLAR->SEL0 &=~ (TACHO);
    BIPOLAR->SEL1 &=~ (TACHO);
    BIPOLAR->DIR  |=  (TACHO);
    BIPOLAR->OUT  &=~ (TACHO);
}

//void run_Speedometer(int steps, int direction){
//    int count = 0;
//    state = (bipolar_state) speedHeldState;
//
//    while(1){
//        WDT_Reset();
//
//        if(steps <= 80000){
//            switch(state){
//                case Step1:
//                    BIPOLAR->OUT |=  SINT_1;
//                    BIPOLAR->OUT |=  SINT_2;
//                    BIPOLAR->OUT |=  SINT_3;
//                    BIPOLAR->OUT &=~ SINT_4;
//
//                    Timer32_usdelay(SPEED);
//                    count++;
//
//                    state = (direction == 0) ? Step2 : Step4;
//                    break;
//                case Step2:
//                    BIPOLAR->OUT &=~ SINT_1;
//                    BIPOLAR->OUT |=  SINT_2;
//                    BIPOLAR->OUT |=  SINT_3;
//                    BIPOLAR->OUT |=  SINT_4;
//
//                    Timer32_usdelay(SPEED);
//                    count++;
//
//                    state = (direction == 0) ? Step3 : Step1;
//                    break;
//                case Step3:
//                    BIPOLAR->OUT |=  SINT_1;
//                    BIPOLAR->OUT &=~ SINT_2;
//                    BIPOLAR->OUT |=  SINT_3;
//                    BIPOLAR->OUT |=  SINT_4;
//
//                    Timer32_usdelay(SPEED);
//                    count++;
//
//                    state = (direction == 0) ? Step4 : Step2;
//                    break;
//                case Step4:
//                    BIPOLAR->OUT |=  SINT_1;
//                    BIPOLAR->OUT |=  SINT_2;
//                    BIPOLAR->OUT &=~ SINT_3;
//                    BIPOLAR->OUT |=  SINT_4;
//
//                    Timer32_usdelay(SPEED);
//                    count++;
//
//                    state = (direction == 0) ? Step1 : Step3;
//                    break;
//                default:
//                    if(count >= steps){
//                        speedHeldState = state;
//                        break;
//                    }
//                    state = Step1;
//            }
//        }else break;
//
//        if(count >= steps) break;
//
//    }
//}

/* Run Tachometer motor n amount of steps */
void run_Tachometer(int steps){
    WDT_Reset();
    int i = 0;
    currSteps = steps;
    tachoSteps = currSteps - prevSteps;

    if(tachoSteps < 0){
        direction = 0;
        count = 1;
    }else if(tachoSteps > 0){
        direction = 1;
        count = 1;
    }else{
        if(currSteps != 0){
            tachoSteps = MAXT - currSteps;
            direction = 0;
        }else
            count = 0;
    }


    for(i = 0; i < abs(tachoSteps) / 2; i++){
        //printf("cycle %d\n", i + 1);
        switch(count){
        case 0:
            P5->OUT &=~ (BIT3 | BIT2 | BIT1 | BIT0);
            break;
        case 1:
            P5->OUT  |=  (BIT2 | BIT1);
            P5->OUT  &= ~(BIT3 | BIT0);
            __delay_cycles(SPEED);
            //Timer32_msdelay(SPEED);

            count = (direction == 1) ? 2 : 4;
            break;
        case 2:
            P5->OUT  |=  (BIT2 | BIT0);
            P5->OUT  &= ~(BIT3 | BIT1);
            __delay_cycles(SPEED);
            //Timer32_msdelay(SPEED);

            count = (direction == 1) ? 3 : 1;
            break;
        case 3:
            P5->OUT  |=  (BIT3 | BIT0);
            P5->OUT  &= ~(BIT2 | BIT1);
            __delay_cycles(SPEED);
            //Timer32_msdelay(SPEED);

            count = (direction == 1) ? 4 : 2;
            break;
        case 4:
            P5->OUT  |=  (BIT3 | BIT1);
            P5->OUT  &= ~(BIT2 | BIT0);
            __delay_cycles(SPEED);
            //Timer32_msdelay(SPEED);

            count = (direction == 1) ? 1 : 3;
            break;
        }

        prevSteps = currSteps;
    }
}

void odometer_Setup(void){

    SPI_Write(0x0F, 0x00);                              //Turn off display test
    SPI_Write(0x09, 0xFF);                              //BCD mode
    SPI_Write(0x0A, 0x01);                              //Set intensity
    SPI_Write(0x0B, 0x07);                              //Set scan limit to all of the digits on
    SPI_Write(0x0C, 0x01);                              //Turn on chip

    //Turn off digits
    SPI_Write(0x01, 0x0F);
    SPI_Write(0x02, 0x0F);
    SPI_Write(0x03, 0x0F);
    SPI_Write(0x04, 0x0F);
    SPI_Write(0x05, 0x0F);
    SPI_Write(0x06, 0x0F);
    SPI_Write(0x07, 0x0F);
    SPI_Write(0x08, 0x0F);
}

void odometer_DataWrite(odometer_state odoReading, int miles){
//    uint8_t odometerAddress[8] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
//    int i = 0;
//    int digits = ceil(log10(miles + 1));    // How many digits do the miles have?
//    int emptyDigits = 8 - digits;


    switch(odoReading){
    case Reset:
        //            for(i = 0; i < 8; i++)  // Writes empty digits to the odometer
        //                SPI_Write(odometerAddress[i], 0x0F);

        odoReading = TripA;
    case Mileage:

        SPI_Write(0x01, (miles % 10) / 1); printf("digit 1\n");

        if(miles >= 10){      SPI_Write(0x02, (miles % 100) / 10); printf("digit 2\n"); }
        else                  SPI_Write(0x02, 0x0F);

        if (miles >= 100){    SPI_Write(0x03, (miles % 1000) / 100); printf("digit 3\n");}
        else                  SPI_Write(0x03, 0x0F);

        if (miles >= 1000){   SPI_Write(0x04, (miles % 10000) / 1000); printf("digit 4\n"); }
        else                  SPI_Write(0x04, 0x0F);

        if (miles >= 10000){  SPI_Write(0x05, (miles % 100000) / 10000); printf("digit 5\n");}
        else                  SPI_Write(0x05, 0x0F);

        if (miles >= 100000){ SPI_Write(0x06, (miles % 1000000) / 100000); printf("digit 6\n"); }
        else                  SPI_Write(0x06, 0x0F);

        //            for(i = 0; i < digits; i++){    // Writes each digit to the odometer
        //                SPI_Write(odometerAddress[i], (miles % (int)pow(10, i + 1)) / (int)pow(10, i));
        //                printf("digit %d\n", i + 1);
        //            }
        //
        //            for(i = 7; i > emptyDigits; i--){  // Writes empty digit to the odometer
        //                if()
        //                SPI_Write(odometerAddress[i], 0x0F);
        //                printf("digit %d\n", i + 1);
        //            }

        break;
    case TripA:

        //            for(i = 0; i < digits; i++){    // Writes each digit to the odometer
        //                SPI_Write(odometerAddress[i], (miles % (int)pow(10, i + 1)) / (int)pow(10, i));
        //            }
        //
        //            for(i = emptyDigits; i < (digits + emptyDigits); i++){  // Writes empty digit to the odometer
        //                SPI_Write(odometerAddress[i], 0x0F);
        //            }

        break;
    default: odoReading = Mileage;

    }
}

void SPI_Write(uint8_t address, uint8_t data){

    ODO->OUT &=~ ODO_CS;                           // Set CS Low

    while(!(EUSCI_B3->IFG & 2));                // Wait for IFG *
    EUSCI_B3->TXBUF = address;                  // Fill TxBuff with address

    while(!(EUSCI_B3->IFG & 2));                // Wait for IFG *
    EUSCI_B3->TXBUF = data;                     // Fill TxBuff with data

    Timer32_msdelay(2);                        // Wait for ~1 ms

    ODO->OUT |= ODO_CS;                            // Set CS High
    Timer32_msdelay(2);                        // Wait for ~1 ms
}
