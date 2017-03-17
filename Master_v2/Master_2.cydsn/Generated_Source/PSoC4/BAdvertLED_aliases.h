/*******************************************************************************
* File Name: BAdvertLED.h  
* Version 2.20
*
* Description:
*  This file contains the Alias definitions for Per-Pin APIs in cypins.h. 
*  Information on using these APIs can be found in the System Reference Guide.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_BAdvertLED_ALIASES_H) /* Pins BAdvertLED_ALIASES_H */
#define CY_PINS_BAdvertLED_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define BAdvertLED_0			(BAdvertLED__0__PC)
#define BAdvertLED_0_PS		(BAdvertLED__0__PS)
#define BAdvertLED_0_PC		(BAdvertLED__0__PC)
#define BAdvertLED_0_DR		(BAdvertLED__0__DR)
#define BAdvertLED_0_SHIFT	(BAdvertLED__0__SHIFT)
#define BAdvertLED_0_INTR	((uint16)((uint16)0x0003u << (BAdvertLED__0__SHIFT*2u)))

#define BAdvertLED_INTR_ALL	 ((uint16)(BAdvertLED_0_INTR))


#endif /* End Pins BAdvertLED_ALIASES_H */


/* [] END OF FILE */
