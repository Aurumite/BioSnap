/*******************************************************************************
* File Name: temp_in.c  
* Version 2.20
*
* Description:
*  This file contains APIs to set up the Pins component for low power modes.
*
* Note:
*
********************************************************************************
* Copyright 2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "cytypes.h"
#include "temp_in.h"

static temp_in_BACKUP_STRUCT  temp_in_backup = {0u, 0u, 0u};


/*******************************************************************************
* Function Name: temp_in_Sleep
****************************************************************************//**
*
* \brief Stores the pin configuration and prepares the pin for entering chip 
*  deep-sleep/hibernate modes. This function must be called for SIO and USBIO
*  pins. It is not essential if using GPIO or GPIO_OVT pins.
*
* <b>Note</b> This function is available in PSoC 4 only.
*
* \return 
*  None 
*  
* \sideeffect
*  For SIO pins, this function configures the pin input threshold to CMOS and
*  drive level to Vddio. This is needed for SIO pins when in device 
*  deep-sleep/hibernate modes.
*
* \funcusage
*  \snippet temp_in_SUT.c usage_temp_in_Sleep_Wakeup
*******************************************************************************/
void temp_in_Sleep(void)
{
    #if defined(temp_in__PC)
        temp_in_backup.pcState = temp_in_PC;
    #else
        #if (CY_PSOC4_4200L)
            /* Save the regulator state and put the PHY into suspend mode */
            temp_in_backup.usbState = temp_in_CR1_REG;
            temp_in_USB_POWER_REG |= temp_in_USBIO_ENTER_SLEEP;
            temp_in_CR1_REG &= temp_in_USBIO_CR1_OFF;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(temp_in__SIO)
        temp_in_backup.sioState = temp_in_SIO_REG;
        /* SIO requires unregulated output buffer and single ended input buffer */
        temp_in_SIO_REG &= (uint32)(~temp_in_SIO_LPM_MASK);
    #endif  
}


/*******************************************************************************
* Function Name: temp_in_Wakeup
****************************************************************************//**
*
* \brief Restores the pin configuration that was saved during Pin_Sleep().
*
* For USBIO pins, the wakeup is only triggered for falling edge interrupts.
*
* <b>Note</b> This function is available in PSoC 4 only.
*
* \return 
*  None
*  
* \funcusage
*  Refer to temp_in_Sleep() for an example usage.
*******************************************************************************/
void temp_in_Wakeup(void)
{
    #if defined(temp_in__PC)
        temp_in_PC = temp_in_backup.pcState;
    #else
        #if (CY_PSOC4_4200L)
            /* Restore the regulator state and come out of suspend mode */
            temp_in_USB_POWER_REG &= temp_in_USBIO_EXIT_SLEEP_PH1;
            temp_in_CR1_REG = temp_in_backup.usbState;
            temp_in_USB_POWER_REG &= temp_in_USBIO_EXIT_SLEEP_PH2;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(temp_in__SIO)
        temp_in_SIO_REG = temp_in_backup.sioState;
    #endif
}


/* [] END OF FILE */
