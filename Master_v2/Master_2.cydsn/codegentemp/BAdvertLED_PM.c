/*******************************************************************************
* File Name: BAdvertLED.c  
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
#include "BAdvertLED.h"

static BAdvertLED_BACKUP_STRUCT  BAdvertLED_backup = {0u, 0u, 0u};


/*******************************************************************************
* Function Name: BAdvertLED_Sleep
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
*  \snippet BAdvertLED_SUT.c usage_BAdvertLED_Sleep_Wakeup
*******************************************************************************/
void BAdvertLED_Sleep(void)
{
    #if defined(BAdvertLED__PC)
        BAdvertLED_backup.pcState = BAdvertLED_PC;
    #else
        #if (CY_PSOC4_4200L)
            /* Save the regulator state and put the PHY into suspend mode */
            BAdvertLED_backup.usbState = BAdvertLED_CR1_REG;
            BAdvertLED_USB_POWER_REG |= BAdvertLED_USBIO_ENTER_SLEEP;
            BAdvertLED_CR1_REG &= BAdvertLED_USBIO_CR1_OFF;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(BAdvertLED__SIO)
        BAdvertLED_backup.sioState = BAdvertLED_SIO_REG;
        /* SIO requires unregulated output buffer and single ended input buffer */
        BAdvertLED_SIO_REG &= (uint32)(~BAdvertLED_SIO_LPM_MASK);
    #endif  
}


/*******************************************************************************
* Function Name: BAdvertLED_Wakeup
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
*  Refer to BAdvertLED_Sleep() for an example usage.
*******************************************************************************/
void BAdvertLED_Wakeup(void)
{
    #if defined(BAdvertLED__PC)
        BAdvertLED_PC = BAdvertLED_backup.pcState;
    #else
        #if (CY_PSOC4_4200L)
            /* Restore the regulator state and come out of suspend mode */
            BAdvertLED_USB_POWER_REG &= BAdvertLED_USBIO_EXIT_SLEEP_PH1;
            BAdvertLED_CR1_REG = BAdvertLED_backup.usbState;
            BAdvertLED_USB_POWER_REG &= BAdvertLED_USBIO_EXIT_SLEEP_PH2;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(BAdvertLED__SIO)
        BAdvertLED_SIO_REG = BAdvertLED_backup.sioState;
    #endif
}


/* [] END OF FILE */
