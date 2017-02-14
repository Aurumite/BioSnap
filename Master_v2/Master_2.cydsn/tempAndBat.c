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

#include <tempAndBat.h>

void initTemperatureAndBattery(){
    
}

uint16 getTemperature(){
    // Read from ADC abd convert to millivolts
    return ADC_CountsTo_mVolts(0, ADC_GetResult16(0));
}

uint16 getBattery(){
    // Read from ADC abd convert to millivolts
    return ADC_CountsTo_mVolts(1, ADC_GetResult16(1));
}