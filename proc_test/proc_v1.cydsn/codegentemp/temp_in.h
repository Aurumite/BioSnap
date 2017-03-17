/*******************************************************************************
* File Name: temp_in.h  
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

#if !defined(CY_PINS_temp_in_H) /* Pins temp_in_H */
#define CY_PINS_temp_in_H

#include "cytypes.h"
#include "cyfitter.h"
#include "temp_in_aliases.h"


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
} temp_in_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   temp_in_Read(void);
void    temp_in_Write(uint8 value);
uint8   temp_in_ReadDataReg(void);
#if defined(temp_in__PC) || (CY_PSOC4_4200L) 
    void    temp_in_SetDriveMode(uint8 mode);
#endif
void    temp_in_SetInterruptMode(uint16 position, uint16 mode);
uint8   temp_in_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void temp_in_Sleep(void); 
void temp_in_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(temp_in__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define temp_in_DRIVE_MODE_BITS        (3)
    #define temp_in_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - temp_in_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the temp_in_SetDriveMode() function.
         *  @{
         */
        #define temp_in_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define temp_in_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define temp_in_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define temp_in_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define temp_in_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define temp_in_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define temp_in_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define temp_in_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define temp_in_MASK               temp_in__MASK
#define temp_in_SHIFT              temp_in__SHIFT
#define temp_in_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in temp_in_SetInterruptMode() function.
     *  @{
     */
        #define temp_in_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define temp_in_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define temp_in_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define temp_in_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(temp_in__SIO)
    #define temp_in_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(temp_in__PC) && (CY_PSOC4_4200L)
    #define temp_in_USBIO_ENABLE               ((uint32)0x80000000u)
    #define temp_in_USBIO_DISABLE              ((uint32)(~temp_in_USBIO_ENABLE))
    #define temp_in_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define temp_in_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define temp_in_USBIO_ENTER_SLEEP          ((uint32)((1u << temp_in_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << temp_in_USBIO_SUSPEND_DEL_SHIFT)))
    #define temp_in_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << temp_in_USBIO_SUSPEND_SHIFT)))
    #define temp_in_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << temp_in_USBIO_SUSPEND_DEL_SHIFT)))
    #define temp_in_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(temp_in__PC)
    /* Port Configuration */
    #define temp_in_PC                 (* (reg32 *) temp_in__PC)
#endif
/* Pin State */
#define temp_in_PS                     (* (reg32 *) temp_in__PS)
/* Data Register */
#define temp_in_DR                     (* (reg32 *) temp_in__DR)
/* Input Buffer Disable Override */
#define temp_in_INP_DIS                (* (reg32 *) temp_in__PC2)

/* Interrupt configuration Registers */
#define temp_in_INTCFG                 (* (reg32 *) temp_in__INTCFG)
#define temp_in_INTSTAT                (* (reg32 *) temp_in__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define temp_in_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(temp_in__SIO)
    #define temp_in_SIO_REG            (* (reg32 *) temp_in__SIO)
#endif /* (temp_in__SIO_CFG) */

/* USBIO registers */
#if !defined(temp_in__PC) && (CY_PSOC4_4200L)
    #define temp_in_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define temp_in_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define temp_in_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define temp_in_DRIVE_MODE_SHIFT       (0x00u)
#define temp_in_DRIVE_MODE_MASK        (0x07u << temp_in_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins temp_in_H */


/* [] END OF FILE */
