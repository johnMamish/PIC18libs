/**
 * @file        stepper_motor_lib.c
 *
 * @brief       Library of functions to drive a portescap 55M048D2U stepper
 *              motor on the PIC18F45K20 using PORTB.
 * 
 * This library makes use of TIMER0 and the lower 4 bits of PORTB.
 */
 
#include <stepper_motor_lib.h>
#include <xc.h>

/**
 * @brief       This variable keeps track of which step the motor is on.
 */ 
static volatile uint8_t currentStep;

/**
 * @brief       This variable keeps track of how many steps to complete until
 *              the stepper motor should be stopped.
 */
static volatile uint16_t stepsToGo;

/**
 * @brief       Tells which direction the stepper motor is currently going in.
 */
static volatile StepperMotorDirection direction;

volatile StepperMotorDirection previousDirection;

/**
 * @brief       Value to preload the stepper motor with to control speed.
 */
static volatile uint16_t speed;

#define STEPPER_MOTOR_MAX_SPEED 0xcb00

#define PRINT_BASE 16

IOPin motor_pins[4];

char num[10];

/**
 * @brief       Initializes TIMER1 and PORTB to for use by stepper_motor_lib.
 */
void initStepperMotor(IOPin* motor_pins_in)
{
    //initialize TIMER0 configuration
    T1CONbits.RD16 = 0b1;               //read timer 16 bits at a time
    T1CONbits.T1CKPS = 0b0;             //1:1 prescale
    T1CONbits.TMR1CS = 0b0;             //system clock drives tmr1
    T1CONbits.TMR1ON = 0b1;
    
    //init library vars
    currentStep = 0;
    stepsToGo = 0;
    motor_pins[0] = *(motor_pins_in);
    motor_pins[1] = *(motor_pins_in + 1);
    motor_pins[2] = *(motor_pins_in + 2);
    motor_pins[3] = *(motor_pins_in + 3);
}

/**
 * @brief       Starts a new stepper motor task (this may override a previous one).
 */
void startStepperMotor(uint16_t distance, StepperMotorDirection dir, uint16_t targetSpeed)
{
    //refresh previousDirection
    previousDirection = direction;

    //set distance left to go
    stepsToGo = distance;

    //set preload value
    if(targetSpeed > STEPPER_MOTOR_MAX_SPEED)
    {
        speed = STEPPER_MOTOR_MAX_SPEED;
    }
    else
    {
        speed = targetSpeed;
    }
    TMR1 = speed;
    direction = dir;
    
    //if we aren't continuing in the same direction, we need to change the state
    //that the stepper is in.
    if(direction != previousDirection)
    {
        switch(direction)
        {
            case FORWARD:
            {
                currentStep += 1;
                break;
            }
            case BACKWARD:
            {
                currentStep -= 1;
                break;
            }
        }

        currentStep %= 4;
    }

    //enable TIMER1 interrupts
    PIE1bits.TMR1IE = 0b1;
}

/**
 * @brief       Checks the status of the stepper motor to see if it is running
 *              or not.
 */
uint8_t stepperMotorFinished()
{
    return (stepsToGo == 0);
}

/**
 * 
 */
void release_stepper_motor()
{
    *motor_pins[0].address &= (0xfe << motor_pins[0].bit_position);
    *motor_pins[1].address &= (0xfe << motor_pins[1].bit_position);
    *motor_pins[2].address &= (0xfe << motor_pins[2].bit_position);
    *motor_pins[3].address &= (0xfe << motor_pins[3].bit_position);
}

/**
 * @brief       Should be inserted in main ISR to service interrupts that drive
 *              the stepper motor.
 */
void STEPPERISR()
{
    if(PIR1bits.TMR1IF && PIE1bits.TMR1IE)
    {
        //clear interrupt flag
        PIR1bits.TMR1IF = 0b0;

        //reset timer value
        TMR1 = speed;

        switch(currentStep)
        {
            case 0:
            {
                *(motor_pins[0].address) |= (0x01 << motor_pins[0].bit_position);
                *(motor_pins[1].address) &= ~(0x01 << motor_pins[1].bit_position);
                *(motor_pins[2].address) |= (0x01 << motor_pins[2].bit_position);
                *(motor_pins[3].address) &= ~(0x01 << motor_pins[3].bit_position);
                switch(direction)
                {
                    case FORWARD:
                    {
                        currentStep++;
                        break;
                    }

                    case BACKWARD:
                    {
                        currentStep = 3;
                        break;
                    }
                }
                break;
            }

            case 1:
            {
                *motor_pins[0].address |= (0x01 << motor_pins[0].bit_position);
                *motor_pins[1].address &= ~(0x01 << motor_pins[1].bit_position);
                *motor_pins[2].address &= ~(0x01 << motor_pins[2].bit_position);
                *motor_pins[3].address |= (0x01 << motor_pins[3].bit_position);
                switch(direction)
                {
                    case FORWARD:
                    {
                        currentStep++;
                        break;
                    }

                    case BACKWARD:
                    {
                        currentStep--;
                        break;
                    }
                }
                break;
            }

            case 2:
            {
                *motor_pins[0].address &= ~(0x01 << motor_pins[0].bit_position);
                *motor_pins[1].address |= (0x01 << motor_pins[1].bit_position);
                *motor_pins[2].address &= ~(0x01 << motor_pins[2].bit_position);
                *motor_pins[3].address |= (0x01 << motor_pins[3].bit_position);
                switch(direction)
                {
                    case FORWARD:
                    {
                        currentStep++;
                        break;
                    }

                    case BACKWARD:
                    {
                        currentStep--;
                        break;
                    }
                }
                break;
            }

            case 3:
            {
                *motor_pins[0].address &= ~(0x01 << motor_pins[0].bit_position);
                *motor_pins[1].address |= (0x01 << motor_pins[1].bit_position);
                *motor_pins[2].address |= (0x01 << motor_pins[2].bit_position);
                *motor_pins[3].address &= ~(0x01 << motor_pins[3].bit_position);
                switch(direction)
                {
                    case FORWARD:
                    {
                        currentStep = 0;
                        break;
                    }

                    case BACKWARD:
                    {
                        currentStep--;
                        break;
                    }
                }
                break;
            }
        }
        
        //decrement steps to go.  If it is 0, disable timer interrupt.
        --stepsToGo;
        if(stepsToGo == 0)
        {
            PIE1bits.TMR1IE = 0b0;
        }
    }
}