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

#ifndef TEMPERATUREANDBATTERY_H
#define TEMPERATUREANDBATTERY_H

#include <project.h>

/*Initialization for ADC
Channels: 1, mode = single
Name: ADC
Vref: VDDA (~3.3V)
*/
    
void initTemperatureAndBattery();
uint16 getTemperature();
uint16 getBattery();

#endif