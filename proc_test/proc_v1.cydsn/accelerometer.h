/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H
    
#include <project.h>

/*Initialization for I2C
Set to Master
Name: I2C_1
*/

//Control Registers
#define CTRL_REG1       0x2A        //System control 1 register
#define CTRL_REG2       0x2B        //System control 2 register
#define CTRL_REG3       0x2C        //Interrupt control register
#define CTRL_REG4       0x2D        //Interrupt enable register
#define CTRL_REG5       0x2E        //Interrupt configuration register

//Other Registers
#define X_MSB           0x01        //MSB of x axis acceleration
#define X_LSB           0x02        //LSB of x axis acceleration
#define Y_MSB           0x03        //MSB of y axis acceleration
#define Y_LSB           0x04        //LSB of y axis acceleration
#define Z_MSB           0x05        //MSB of z axis acceleration
#define Z_LSB           0x06        //LSB of z axis acceleration
#define WHO_AM_I        0x0D        //Who am I
#define ACCEL_ADDR      0x1D

//FF/MT Registers
#define FF_MT_CFG       0x15        //Freefall/Motion Configuration register 
#define FF_MT_SRC       0x16        //Freefall/Motion Source register
#define FF_MT_THS       0x17        //Freefall and Motion Threshold register
#define FF_MT_COUNT     0x18        //Debounce register 

//Transient Registers
#define TRANSIENT_CFG   0x1D
#define TRANSIENT_SRC   0x1E
#define TRANSIENT_THS   0x1F
#define TRANSIENT_COUNT 0x20

//Pulse Registers
#define PULSE_CFG       0x21
#define PULSE_SRC       0x22
#define PULSE_THSX      0x23
#define PULSE_THSY      0x24
#define PULSE_THSZ      0x25
#define PULSE_TMLT      0x26
#define PULSE_LTCY      0x27
#define PULSE_WIND      0x28

//Sleep Register
#define ASLP_COUNT      0x29

//Interrupt Masks
#define INT_CFG_DRDY     (0x01)      //Data ready
#define INT_CFG_FF_MT    (0x03)      //Freefall/Motion
#define INT_CFG_PULSE    (0x04)      //Pulse Detection
#define INT_CFG_LNDPRT   (0x10)      //Portrait/Landscape
#define INT_CFG_TRANS    (0x20)      //Transient

//Functions
void initAccelControl();
void initAccelFFMT();
void initAccelTransient();
void initAccelPulse();
void initAccelSleep();
void initAccelAll();
uint8 getAccelX();
uint8 getAccelY();
uint8 getAccelZ();
int getAccelXYZ();
void writeAccel(uint8 reg, uint8 data);
uint8 readAccel(uint8 reg);

#endif

/* [] END OF FILE */
