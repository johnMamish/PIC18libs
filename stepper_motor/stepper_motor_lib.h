/* 
 * File:   stepper_motor_lib.h
 * Author: John
 *
 * Created on June 2, 2013, 9:28 PM
 */

#ifndef STEPPER_MOTOR_LIB_H
#define	STEPPER_MOTOR_LIB_H

#include <stdint.h>
#include <UART_lib.h>

#ifdef	__cplusplus
//extern "C" {
#endif

typedef enum
{
    FORWARD,
    BACKWARD
} StepperMotorDirection;

typedef struct
{
    unsigned volatile char* address;
    unsigned char bit_position;
} IOPin;

void initStepperMotor(IOPin*);
void startStepperMotor(uint16_t, StepperMotorDirection, uint16_t);
void release_stepper_motor();
uint8_t stepperMotorFinished();
void STEPPERISR();


#ifdef	__cplusplus
//}
#endif

#endif	/* STEPPER_MOTOR_LIB_H */

