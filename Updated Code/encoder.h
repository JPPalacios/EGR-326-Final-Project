#ifndef ENCODER_H_
#define ENCODER_H_

#include "stdbool.h"

#define ENCODER P3
#define ENC_CLK    BIT7
#define ENC_DT     BIT6
#define ENC_SW     BIT5
#define ENC_SETUP (ENC_CLK | ENC_DT | ENC_SW)

void encoder_Setup(void);

// bool ccwFlag, cwFlag, swFlag;

#endif /* ENCODER_H_ */
