/*******************************************************************************
* File Name: L_PWM.h  
* Version 2.20
*
* Description:
*  This file contains Pin function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_L_PWM_H) /* Pins L_PWM_H */
#define CY_PINS_L_PWM_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "L_PWM_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 L_PWM__PORT == 15 && ((L_PWM__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    L_PWM_Write(uint8 value);
void    L_PWM_SetDriveMode(uint8 mode);
uint8   L_PWM_ReadDataReg(void);
uint8   L_PWM_Read(void);
void    L_PWM_SetInterruptMode(uint16 position, uint16 mode);
uint8   L_PWM_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the L_PWM_SetDriveMode() function.
     *  @{
     */
        #define L_PWM_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define L_PWM_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define L_PWM_DM_RES_UP          PIN_DM_RES_UP
        #define L_PWM_DM_RES_DWN         PIN_DM_RES_DWN
        #define L_PWM_DM_OD_LO           PIN_DM_OD_LO
        #define L_PWM_DM_OD_HI           PIN_DM_OD_HI
        #define L_PWM_DM_STRONG          PIN_DM_STRONG
        #define L_PWM_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define L_PWM_MASK               L_PWM__MASK
#define L_PWM_SHIFT              L_PWM__SHIFT
#define L_PWM_WIDTH              1u

/* Interrupt constants */
#if defined(L_PWM__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in L_PWM_SetInterruptMode() function.
     *  @{
     */
        #define L_PWM_INTR_NONE      (uint16)(0x0000u)
        #define L_PWM_INTR_RISING    (uint16)(0x0001u)
        #define L_PWM_INTR_FALLING   (uint16)(0x0002u)
        #define L_PWM_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define L_PWM_INTR_MASK      (0x01u) 
#endif /* (L_PWM__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define L_PWM_PS                     (* (reg8 *) L_PWM__PS)
/* Data Register */
#define L_PWM_DR                     (* (reg8 *) L_PWM__DR)
/* Port Number */
#define L_PWM_PRT_NUM                (* (reg8 *) L_PWM__PRT) 
/* Connect to Analog Globals */                                                  
#define L_PWM_AG                     (* (reg8 *) L_PWM__AG)                       
/* Analog MUX bux enable */
#define L_PWM_AMUX                   (* (reg8 *) L_PWM__AMUX) 
/* Bidirectional Enable */                                                        
#define L_PWM_BIE                    (* (reg8 *) L_PWM__BIE)
/* Bit-mask for Aliased Register Access */
#define L_PWM_BIT_MASK               (* (reg8 *) L_PWM__BIT_MASK)
/* Bypass Enable */
#define L_PWM_BYP                    (* (reg8 *) L_PWM__BYP)
/* Port wide control signals */                                                   
#define L_PWM_CTL                    (* (reg8 *) L_PWM__CTL)
/* Drive Modes */
#define L_PWM_DM0                    (* (reg8 *) L_PWM__DM0) 
#define L_PWM_DM1                    (* (reg8 *) L_PWM__DM1)
#define L_PWM_DM2                    (* (reg8 *) L_PWM__DM2) 
/* Input Buffer Disable Override */
#define L_PWM_INP_DIS                (* (reg8 *) L_PWM__INP_DIS)
/* LCD Common or Segment Drive */
#define L_PWM_LCD_COM_SEG            (* (reg8 *) L_PWM__LCD_COM_SEG)
/* Enable Segment LCD */
#define L_PWM_LCD_EN                 (* (reg8 *) L_PWM__LCD_EN)
/* Slew Rate Control */
#define L_PWM_SLW                    (* (reg8 *) L_PWM__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define L_PWM_PRTDSI__CAPS_SEL       (* (reg8 *) L_PWM__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define L_PWM_PRTDSI__DBL_SYNC_IN    (* (reg8 *) L_PWM__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define L_PWM_PRTDSI__OE_SEL0        (* (reg8 *) L_PWM__PRTDSI__OE_SEL0) 
#define L_PWM_PRTDSI__OE_SEL1        (* (reg8 *) L_PWM__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define L_PWM_PRTDSI__OUT_SEL0       (* (reg8 *) L_PWM__PRTDSI__OUT_SEL0) 
#define L_PWM_PRTDSI__OUT_SEL1       (* (reg8 *) L_PWM__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define L_PWM_PRTDSI__SYNC_OUT       (* (reg8 *) L_PWM__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(L_PWM__SIO_CFG)
    #define L_PWM_SIO_HYST_EN        (* (reg8 *) L_PWM__SIO_HYST_EN)
    #define L_PWM_SIO_REG_HIFREQ     (* (reg8 *) L_PWM__SIO_REG_HIFREQ)
    #define L_PWM_SIO_CFG            (* (reg8 *) L_PWM__SIO_CFG)
    #define L_PWM_SIO_DIFF           (* (reg8 *) L_PWM__SIO_DIFF)
#endif /* (L_PWM__SIO_CFG) */

/* Interrupt Registers */
#if defined(L_PWM__INTSTAT)
    #define L_PWM_INTSTAT            (* (reg8 *) L_PWM__INTSTAT)
    #define L_PWM_SNAP               (* (reg8 *) L_PWM__SNAP)
    
	#define L_PWM_0_INTTYPE_REG 		(* (reg8 *) L_PWM__0__INTTYPE)
#endif /* (L_PWM__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_L_PWM_H */


/* [] END OF FILE */
