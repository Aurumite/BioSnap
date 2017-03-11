/*******************************************************************************
* File Name: bat_in.h  
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

#if !defined(CY_PINS_bat_in_ALIASES_H) /* Pins bat_in_ALIASES_H */
#define CY_PINS_bat_in_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define bat_in_0			(bat_in__0__PC)
#define bat_in_0_PS		(bat_in__0__PS)
#define bat_in_0_PC		(bat_in__0__PC)
#define bat_in_0_DR		(bat_in__0__DR)
#define bat_in_0_SHIFT	(bat_in__0__SHIFT)
#define bat_in_0_INTR	((uint16)((uint16)0x0003u << (bat_in__0__SHIFT*2u)))

#define bat_in_INTR_ALL	 ((uint16)(bat_in_0_INTR))


#endif /* End Pins bat_in_ALIASES_H */


/* [] END OF FILE */
