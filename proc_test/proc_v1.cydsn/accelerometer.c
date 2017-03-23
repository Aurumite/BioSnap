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

#include <accelerometer.h>
//#include <math.h>

void initAccelControl(){     
    
    //reset accelerometer
    readAccel(CTRL_REG1);
    writeAccel(CTRL_REG1, 0x00);     
    
    /* REG 2 Settings
    Self Test:                      Disabled
    Software Reset:                 Enabled
    Sleep Power                     Low noise, Low power
    Auto-sleep:                     Disable
    Active Power:                   Low noise, Low power
    */
    writeAccel(CTRL_REG2, 0b01001001);
    
    /* REG 3 Settings
    FIFO Gate:                      Bypassed
    Transient Wake:                 Bypassed
    LNDPRT Wake:                    Bypassed
    Pulse Wake:                     Bypassed
    FF/MT Wake:                     Bypassed
    Interrupt Polarity:             Active High
    Interrupt:                      Push-Pull
    */
    writeAccel(CTRL_REG3, 0b00000010);
    
    /* REG 4 Settings
    Auto-Sleep/Wake Interrupt:      Disabled
    FIFO Interrupt:                 Disabled
    Transient Interrupt:            Disabled
    LNDPRT Interrupt:               Disabled
    Pulse Interrupt:                Disabled
    FF/MT Interrupt:                Disabled
    Data Ready Interrupt:           Enabled
    */
    writeAccel(CTRL_REG4, 0b00000001);
    
    /* REG 5 Settings
    Auto-Sleep/Wake Interrupt:      Int 2
    FIFO Interrupt:                 Int 2
    Transient Interrupt:            Int 2
    LNDPRT Interrupt:               Int 2
    Pulse Interrupt:                Int 2
    FF/MT Interrupt:                Int 2
    Data Ready Interrupt:           Int 1
    */
    writeAccel(CTRL_REG5, 0b00000001);
    
     /* REG 1 Settings
    Sleep mode sample frequency:    default
    Data rate Selection:            12.5Hz
    Fast Read                       On
    FS Selection:                   Active
    */
    writeAccel(CTRL_REG1, 0b00101011);    
}

void initAccelFFMT(){
    /* FF/MT Configuration
    Event Latch:                    Enabled
    Mode:                           Freefall
    Z Flag:                         Enabled (flags detection when acceleration > threshold)
    y Flag:                         Enabled (flags detection when acceleration > threshold)
    x Flag:                         Enabled (flags detection when acceleration > threshold)
    */
    writeAccel(FF_MT_CFG, 0b10111000);
    
    /* FF/MT Threshold
    Debounce Mode:                  Increment/Decrement
    Threshold:                      0 (for now)
    */
    writeAccel(FF_MT_THS, 0b00000000);
    
    /* FF/MT Debounce ( minimum number of debounce sample counts that continuously match the detection condition)
    Debounce:                       80 ms (based on ODR)
    */
    writeAccel(FF_MT_COUNT, 0b00000000); //not needed?
}

void initAccelTransient(){
    /* Transient Configuration
    Event Latch:                    Enabled
    Z Flag:                         Enabled (flags detection when acceleration > threshold)
    y Flag:                         Enabled (flags detection when acceleration > threshold)
    x Flag:                         Enabled (flags detection when acceleration > threshold)
    High Pass Filter:               Disabled
    */
    writeAccel(TRANSIENT_CFG, 0b00011110);
    
    /* Transient Threshold
    Debounce Mode:                  Increment/Decrement
    Threshold:                      0 (for now)
    */
    writeAccel(TRANSIENT_THS, 0b00000000);
    
    /* FF/MT Debounce ( minimum number of debounce sample counts that continuously match the detection condition)
    Debounce:                       80 ms (based on ODR)
    */
    writeAccel(TRANSIENT_COUNT, 0b00000000); //not needed?
}

void initAccelPulse(){
    /* Pulse Configuration
    Double Pulse Abort:             Not aborted
    Event Latch:                    Enabled
    Z Flag Double:                  Enabled (flags detection when acceleration > threshold)
    Z Flag Single:                  Enabled (flags detection when acceleration > threshold)
    y Flag Double:                  Enabled (flags detection when acceleration > threshold)
    y Flag Single:                  Enabled (flags detection when acceleration > threshold)
    x Flag Double:                  Enabled (flags detection when acceleration > threshold)
    x Flag Single:                  Enabled (flags detection when acceleration > threshold)
    */
    writeAccel(PULSE_CFG, 0b01111111);
    
    /* Pulse Thresholds
    Threshold:                      0 (for now)
    */
    writeAccel(PULSE_THSX, 0b00000000);
    writeAccel(PULSE_THSY, 0b00000000);
    writeAccel(PULSE_THSZ, 0b00000000);
    
    /* Pulse Time Limit (Max length of a pulse)
    Time:                       20 ms (based on ODR)
    */
    writeAccel(PULSE_TMLT, 0b00000000); //not needed?
    
    /* Pulse Latency (Min time between pulses?)
    Time:                       40 ms (based on ODR)
    */
    writeAccel(PULSE_LTCY, 0b00000000); //not needed?
    
    /* Pulse Window (Max time after latency period that a second tap can be detected)
    Time:                       40 ms (based on ODR)
    */
    writeAccel(PULSE_WIND, 0b00000000); //not needed?
}

void initAccelSleep(){
    /* Auto-Wake/Sleep Detection (Min period of inactivty)
    Time:                       80 ms (based on ODR)
    */
    writeAccel(ASLP_COUNT, 0b00000000); //not needed?
}

uint8 getAccelX(){
    // Read x value of acceleration
    return readAccel(X_MSB);
}

uint8 getAccelY(){
    // Read y value of acceleration
    return readAccel(Y_MSB);
}

uint8 getAccelZ(){
    // Read z value of acceleration
    return readAccel(Z_MSB);
}

int getAccelXYZ(){
    int x = getAccelX();
    int y = getAccelY();
    int z = getAccelZ();
    
    //reads x, y, and z value of acceleration and combines it into an 32 bit int
    return (x << 16) + (y << 8) + z;
}

void writeAccel(uint8 reg, uint8 data){
    uint8 Write_Buf[2]={0};
    Write_Buf[0] = reg;              //Assign the first element to be the register you want to write to (Parameter 1)
    Write_Buf[1] = data;             //Assign the second elemnt to be the value you wish to write to the register (Parameter 2)
    
    
    //write to writebuf and wait until success
    I2C_1_I2CMasterWriteBuf(ACCEL_ADDR, (uint8 *)Write_Buf, 2, I2C_1_I2C_MODE_COMPLETE_XFER);
    while((I2C_1_I2CMasterStatus()&I2C_1_I2C_MSTAT_WR_CMPLT)==0);
        
    return;
}

uint8 readAccel(uint8 reg){
    uint8 Write_Buf[1] = {0}; //Buffer that will contain the register that will be read from
    Write_Buf[0] = reg;
    
    uint8 Read_Buf[1]={0};  //Buffer that will store the value read from the register
    
    //tell acclereometer to load read buffer
    I2C_1_I2CMasterWriteBuf(ACCEL_ADDR, (uint8 *)Write_Buf, 1, I2C_1_I2C_MODE_NO_STOP);
    while((I2C_1_I2CMasterStatus() & I2C_1_I2C_MSTAT_WR_CMPLT)==0){}
    
    //read from readbuf
    I2C_1_I2CMasterReadBuf(ACCEL_ADDR, (uint8 *)Read_Buf, 1, I2C_1_I2C_MODE_REPEAT_START);
    while((I2C_1_I2CMasterStatus() & I2C_1_I2C_MSTAT_RD_CMPLT)==0){}
    
    return Read_Buf[0];
}

/* [] END OF FILE */
