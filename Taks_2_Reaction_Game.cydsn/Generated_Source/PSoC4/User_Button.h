/*******************************************************************************
* File Name: User_button.h  
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

#if !defined(CY_PINS_User_button_H) /* Pins User_button_H */
#define CY_PINS_User_button_H

#include "cytypes.h"
#include "cyfitter.h"
#include "User_button_aliases.h"


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
} User_button_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   User_button_Read(void);
void    User_button_Write(uint8 value);
uint8   User_button_ReadDataReg(void);
#if defined(User_button__PC) || (CY_PSOC4_4200L) 
    void    User_button_SetDriveMode(uint8 mode);
#endif
void    User_button_SetInterruptMode(uint16 position, uint16 mode);
uint8   User_button_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void User_button_Sleep(void); 
void User_button_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(User_button__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define User_button_DRIVE_MODE_BITS        (3)
    #define User_button_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - User_button_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the User_button_SetDriveMode() function.
         *  @{
         */
        #define User_button_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define User_button_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define User_button_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define User_button_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define User_button_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define User_button_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define User_button_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define User_button_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define User_button_MASK               User_button__MASK
#define User_button_SHIFT              User_button__SHIFT
#define User_button_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in User_button_SetInterruptMode() function.
     *  @{
     */
        #define User_button_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define User_button_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define User_button_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define User_button_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(User_button__SIO)
    #define User_button_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(User_button__PC) && (CY_PSOC4_4200L)
    #define User_button_USBIO_ENABLE               ((uint32)0x80000000u)
    #define User_button_USBIO_DISABLE              ((uint32)(~User_button_USBIO_ENABLE))
    #define User_button_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define User_button_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define User_button_USBIO_ENTER_SLEEP          ((uint32)((1u << User_button_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << User_button_USBIO_SUSPEND_DEL_SHIFT)))
    #define User_button_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << User_button_USBIO_SUSPEND_SHIFT)))
    #define User_button_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << User_button_USBIO_SUSPEND_DEL_SHIFT)))
    #define User_button_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(User_button__PC)
    /* Port Configuration */
    #define User_button_PC                 (* (reg32 *) User_button__PC)
#endif
/* Pin State */
#define User_button_PS                     (* (reg32 *) User_button__PS)
/* Data Register */
#define User_button_DR                     (* (reg32 *) User_button__DR)
/* Input Buffer Disable Override */
#define User_button_INP_DIS                (* (reg32 *) User_button__PC2)

/* Interrupt configuration Registers */
#define User_button_INTCFG                 (* (reg32 *) User_button__INTCFG)
#define User_button_INTSTAT                (* (reg32 *) User_button__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define User_button_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(User_button__SIO)
    #define User_button_SIO_REG            (* (reg32 *) User_button__SIO)
#endif /* (User_button__SIO_CFG) */

/* USBIO registers */
#if !defined(User_button__PC) && (CY_PSOC4_4200L)
    #define User_button_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define User_button_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define User_button_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define User_button_DRIVE_MODE_SHIFT       (0x00u)
#define User_button_DRIVE_MODE_MASK        (0x07u << User_button_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins User_button_H */


/* [] END OF FILE */
