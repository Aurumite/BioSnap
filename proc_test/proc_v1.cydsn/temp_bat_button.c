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

#include <temp_bat_button.h>

void initTempBatButton(){
    //Useless right now   
}

uint16 getTemperature(){
    // Read from ADC and convert to millivolts
    return ADC_CountsTo_mVolts(0, ADC_GetResult16(0));
}

uint16 getBattery(){
    // Read from ADC and convert to millivolts
    return ADC_CountsTo_mVolts(1, ADC_GetResult16(0));
}

uint16 getButton(){
    // Read from ADC and decide if value is greater than 3300/2
    return ADC_CountsTo_mVolts(2, ADC_GetResult16(0)) > (3300/2);
}