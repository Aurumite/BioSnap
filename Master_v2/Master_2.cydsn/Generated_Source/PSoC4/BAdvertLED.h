/*******************************************************************************
* File Name: BAdvertLED.h  
* Version 2.20
*
* Description:
*  This file contains Pin function prototypes and register defines
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_BAdvertLED_H) /* Pins BAdvertLED_H */
#define CY_PINS_BAdvertLED_H

#include "cytypes.h"
#include "cyfitter.h"
#include "BAdvertLED_aliases.h"


/***************************************
*     Data Struct Definitions
***************************************/

/**
* \addtogroup group_structures
* @{
*/
    
/* Structure for sleep mode support */
typedef struct
{
    uint32 pcState; /**< State of the port control register */
    uint32 sioState; /**< State of the SIO configuration */
    uint32 usbState; /**< State of the USBIO regulator */
} BAdvertLED_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   BAdvertLED_Read(void);
void    BAdvertLED_Write(uint8 value);
uint8   BAdvertLED_ReadDataReg(void);
#if defined(BAdvertLED__PC) || (CY_PSOC4_4200L) 
    void    BAdvertLED_SetDriveMode(uint8 mode);
#endif
void    BAdvertLED_SetInterruptMode(uint16 position, uint16 mode);
uint8   BAdvertLED_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void BAdvertLED_Sleep(void); 
void BAdvertLED_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(BAdvertLED__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define BAdvertLED_DRIVE_MODE_BITS        (3)
    #define BAdvertLED_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - BAdvertLED_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the BAdvertLED_SetDriveMode() function.
         *  @{
         */
        #define BAdvertLED_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define BAdvertLED_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define BAdvertLED_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define BAdvertLED_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define BAdvertLED_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define BAdvertLED_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define BAdvertLED_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define BAdvertLED_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define BAdvertLED_MASK               BAdvertLED__MASK
#define BAdvertLED_SHIFT              BAdvertLED__SHIFT
#define BAdvertLED_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in BAdvertLED_SetInterruptMode() function.
     *  @{
     */
        #define BAdvertLED_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define BAdvertLED_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define BAdvertLED_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define BAdvertLED_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(BAdvertLED__SIO)
    #define BAdvertLED_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(BAdvertLED__PC) && (CY_PSOC4_4200L)
    #define BAdvertLED_USBIO_ENABLE               ((uint32)0x80000000u)
    #define BAdvertLED_USBIO_DISABLE              ((uint32)(~BAdvertLED_USBIO_ENABLE))
    #define BAdvertLED_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define BAdvertLED_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define BAdvertLED_USBIO_ENTER_SLEEP          ((uint32)((1u << BAdvertLED_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << BAdvertLED_USBIO_SUSPEND_DEL_SHIFT)))
    #define BAdvertLED_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << BAdvertLED_USBIO_SUSPEND_SHIFT)))
    #define BAdvertLED_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << BAdvertLED_USBIO_SUSPEND_DEL_SHIFT)))
    #define BAdvertLED_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(BAdvertLED__PC)
    /* Port Configuration */
    #define BAdvertLED_PC                 (* (reg32 *) BAdvertLED__PC)
#endif
/* Pin State */
#define BAdvertLED_PS                     (* (reg32 *) BAdvertLED__PS)
/* Data Register */
#define BAdvertLED_DR                     (* (reg32 *) BAdvertLED__DR)
/* Input Buffer Disable Override */
#define BAdvertLED_INP_DIS                (* (reg32 *) BAdvertLED__PC2)

/* Interrupt configuration Registers */
#define BAdvertLED_INTCFG                 (* (reg32 *) BAdvertLED__INTCFG)
#define BAdvertLED_INTSTAT                (* (reg32 *) BAdvertLED__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define BAdvertLED_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(BAdvertLED__SIO)
    #define BAdvertLED_SIO_REG            (* (reg32 *) BAdvertLED__SIO)
#endif /* (BAdvertLED__SIO_CFG) */

/* USBIO registers */
#if !defined(BAdvertLED__PC) && (CY_PSOC4_4200L)
    #define BAdvertLED_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define BAdvertLED_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define BAdvertLED_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define BAdvertLED_DRIVE_MODE_SHIFT       (0x00u)
#define BAdvertLED_DRIVE_MODE_MASK        (0x07u << BAdvertLED_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins BAdvertLED_H */


/* [] END OF FILE */
