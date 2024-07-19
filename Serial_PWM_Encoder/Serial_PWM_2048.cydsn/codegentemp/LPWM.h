/*******************************************************************************
* File Name: LPWM.h
* Version 3.30
*
* Description:
*  Contains the prototypes and constants for the functions available to the
*  PWM user module.
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#if !defined(CY_PWM_LPWM_H)
#define CY_PWM_LPWM_H

#include "cyfitter.h"
#include "cytypes.h"
#include "CyLib.h" /* For CyEnterCriticalSection() and CyExitCriticalSection() functions */

extern uint8 LPWM_initVar;


/***************************************
* Conditional Compilation Parameters
***************************************/
#define LPWM_Resolution                     (16u)
#define LPWM_UsingFixedFunction             (0u)
#define LPWM_DeadBandMode                   (0u)
#define LPWM_KillModeMinTime                (0u)
#define LPWM_KillMode                       (0u)
#define LPWM_PWMMode                        (0u)
#define LPWM_PWMModeIsCenterAligned         (0u)
#define LPWM_DeadBandUsed                   (0u)
#define LPWM_DeadBand2_4                    (0u)

#if !defined(LPWM_PWMUDB_genblk8_stsreg__REMOVED)
    #define LPWM_UseStatus                  (1u)
#else
    #define LPWM_UseStatus                  (0u)
#endif /* !defined(LPWM_PWMUDB_genblk8_stsreg__REMOVED) */

#if !defined(LPWM_PWMUDB_genblk1_ctrlreg__REMOVED)
    #define LPWM_UseControl                 (1u)
#else
    #define LPWM_UseControl                 (0u)
#endif /* !defined(LPWM_PWMUDB_genblk1_ctrlreg__REMOVED) */

#define LPWM_UseOneCompareMode              (1u)
#define LPWM_MinimumKillTime                (1u)
#define LPWM_EnableMode                     (0u)

#define LPWM_CompareMode1SW                 (0u)
#define LPWM_CompareMode2SW                 (0u)

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component PWM_v3_30 requires cy_boot v3.0 or later
#endif /* (CY_ PSOC5LP) */

/* Use Kill Mode Enumerated Types */
#define LPWM__B_PWM__DISABLED 0
#define LPWM__B_PWM__ASYNCHRONOUS 1
#define LPWM__B_PWM__SINGLECYCLE 2
#define LPWM__B_PWM__LATCHED 3
#define LPWM__B_PWM__MINTIME 4


/* Use Dead Band Mode Enumerated Types */
#define LPWM__B_PWM__DBMDISABLED 0
#define LPWM__B_PWM__DBM_2_4_CLOCKS 1
#define LPWM__B_PWM__DBM_256_CLOCKS 2


/* Used PWM Mode Enumerated Types */
#define LPWM__B_PWM__ONE_OUTPUT 0
#define LPWM__B_PWM__TWO_OUTPUTS 1
#define LPWM__B_PWM__DUAL_EDGE 2
#define LPWM__B_PWM__CENTER_ALIGN 3
#define LPWM__B_PWM__DITHER 5
#define LPWM__B_PWM__HARDWARESELECT 4


/* Used PWM Compare Mode Enumerated Types */
#define LPWM__B_PWM__LESS_THAN 1
#define LPWM__B_PWM__LESS_THAN_OR_EQUAL 2
#define LPWM__B_PWM__GREATER_THAN 3
#define LPWM__B_PWM__GREATER_THAN_OR_EQUAL_TO 4
#define LPWM__B_PWM__EQUAL 0
#define LPWM__B_PWM__FIRMWARE 5



/***************************************
* Data Struct Definition
***************************************/


/**************************************************************************
 * Sleep Wakeup Backup structure for PWM Component
 *************************************************************************/
typedef struct
{

    uint8 PWMEnableState;

    #if(!LPWM_UsingFixedFunction)
        uint16 PWMUdb;               /* PWM Current Counter value  */
        #if(!LPWM_PWMModeIsCenterAligned)
            uint16 PWMPeriod;
        #endif /* (!LPWM_PWMModeIsCenterAligned) */
        #if (LPWM_UseStatus)
            uint8 InterruptMaskValue;   /* PWM Current Interrupt Mask */
        #endif /* (LPWM_UseStatus) */

        /* Backup for Deadband parameters */
        #if(LPWM_DeadBandMode == LPWM__B_PWM__DBM_256_CLOCKS || \
            LPWM_DeadBandMode == LPWM__B_PWM__DBM_2_4_CLOCKS)
            uint8 PWMdeadBandValue; /* Dead Band Counter Current Value */
        #endif /* deadband count is either 2-4 clocks or 256 clocks */

        /* Backup Kill Mode Counter*/
        #if(LPWM_KillModeMinTime)
            uint8 PWMKillCounterPeriod; /* Kill Mode period value */
        #endif /* (LPWM_KillModeMinTime) */

        /* Backup control register */
        #if(LPWM_UseControl)
            uint8 PWMControlRegister; /* PWM Control Register value */
        #endif /* (LPWM_UseControl) */

    #endif /* (!LPWM_UsingFixedFunction) */

}LPWM_backupStruct;


/***************************************
*        Function Prototypes
 **************************************/

void    LPWM_Start(void) ;
void    LPWM_Stop(void) ;

#if (LPWM_UseStatus || LPWM_UsingFixedFunction)
    void  LPWM_SetInterruptMode(uint8 interruptMode) ;
    uint8 LPWM_ReadStatusRegister(void) ;
#endif /* (LPWM_UseStatus || LPWM_UsingFixedFunction) */

#define LPWM_GetInterruptSource() LPWM_ReadStatusRegister()

#if (LPWM_UseControl)
    uint8 LPWM_ReadControlRegister(void) ;
    void  LPWM_WriteControlRegister(uint8 control)
          ;
#endif /* (LPWM_UseControl) */

#if (LPWM_UseOneCompareMode)
   #if (LPWM_CompareMode1SW)
       void    LPWM_SetCompareMode(uint8 comparemode)
               ;
   #endif /* (LPWM_CompareMode1SW) */
#else
    #if (LPWM_CompareMode1SW)
        void    LPWM_SetCompareMode1(uint8 comparemode)
                ;
    #endif /* (LPWM_CompareMode1SW) */
    #if (LPWM_CompareMode2SW)
        void    LPWM_SetCompareMode2(uint8 comparemode)
                ;
    #endif /* (LPWM_CompareMode2SW) */
#endif /* (LPWM_UseOneCompareMode) */

#if (!LPWM_UsingFixedFunction)
    uint16   LPWM_ReadCounter(void) ;
    uint16 LPWM_ReadCapture(void) ;

    #if (LPWM_UseStatus)
            void LPWM_ClearFIFO(void) ;
    #endif /* (LPWM_UseStatus) */

    void    LPWM_WriteCounter(uint16 counter)
            ;
#endif /* (!LPWM_UsingFixedFunction) */

void    LPWM_WritePeriod(uint16 period)
        ;
uint16 LPWM_ReadPeriod(void) ;

#if (LPWM_UseOneCompareMode)
    void    LPWM_WriteCompare(uint16 compare)
            ;
    uint16 LPWM_ReadCompare(void) ;
#else
    void    LPWM_WriteCompare1(uint16 compare)
            ;
    uint16 LPWM_ReadCompare1(void) ;
    void    LPWM_WriteCompare2(uint16 compare)
            ;
    uint16 LPWM_ReadCompare2(void) ;
#endif /* (LPWM_UseOneCompareMode) */


#if (LPWM_DeadBandUsed)
    void    LPWM_WriteDeadTime(uint8 deadtime) ;
    uint8   LPWM_ReadDeadTime(void) ;
#endif /* (LPWM_DeadBandUsed) */

#if ( LPWM_KillModeMinTime)
    void LPWM_WriteKillTime(uint8 killtime) ;
    uint8 LPWM_ReadKillTime(void) ;
#endif /* ( LPWM_KillModeMinTime) */

void LPWM_Init(void) ;
void LPWM_Enable(void) ;
void LPWM_Sleep(void) ;
void LPWM_Wakeup(void) ;
void LPWM_SaveConfig(void) ;
void LPWM_RestoreConfig(void) ;


/***************************************
*         Initialization Values
**************************************/
#define LPWM_INIT_PERIOD_VALUE          (1024u)
#define LPWM_INIT_COMPARE_VALUE1        (1u)
#define LPWM_INIT_COMPARE_VALUE2        (63u)
#define LPWM_INIT_INTERRUPTS_MODE       (uint8)(((uint8)(0u <<   \
                                                    LPWM_STATUS_TC_INT_EN_MASK_SHIFT)) | \
                                                    (uint8)((uint8)(0u <<  \
                                                    LPWM_STATUS_CMP2_INT_EN_MASK_SHIFT)) | \
                                                    (uint8)((uint8)(0u <<  \
                                                    LPWM_STATUS_CMP1_INT_EN_MASK_SHIFT )) | \
                                                    (uint8)((uint8)(0u <<  \
                                                    LPWM_STATUS_KILL_INT_EN_MASK_SHIFT )))
#define LPWM_DEFAULT_COMPARE2_MODE      (uint8)((uint8)1u <<  LPWM_CTRL_CMPMODE2_SHIFT)
#define LPWM_DEFAULT_COMPARE1_MODE      (uint8)((uint8)1u <<  LPWM_CTRL_CMPMODE1_SHIFT)
#define LPWM_INIT_DEAD_TIME             (1u)


/********************************
*         Registers
******************************** */

#if (LPWM_UsingFixedFunction)
   #define LPWM_PERIOD_LSB              (*(reg16 *) LPWM_PWMHW__PER0)
   #define LPWM_PERIOD_LSB_PTR          ( (reg16 *) LPWM_PWMHW__PER0)
   #define LPWM_COMPARE1_LSB            (*(reg16 *) LPWM_PWMHW__CNT_CMP0)
   #define LPWM_COMPARE1_LSB_PTR        ( (reg16 *) LPWM_PWMHW__CNT_CMP0)
   #define LPWM_COMPARE2_LSB            (0x00u)
   #define LPWM_COMPARE2_LSB_PTR        (0x00u)
   #define LPWM_COUNTER_LSB             (*(reg16 *) LPWM_PWMHW__CNT_CMP0)
   #define LPWM_COUNTER_LSB_PTR         ( (reg16 *) LPWM_PWMHW__CNT_CMP0)
   #define LPWM_CAPTURE_LSB             (*(reg16 *) LPWM_PWMHW__CAP0)
   #define LPWM_CAPTURE_LSB_PTR         ( (reg16 *) LPWM_PWMHW__CAP0)
   #define LPWM_RT1                     (*(reg8 *)  LPWM_PWMHW__RT1)
   #define LPWM_RT1_PTR                 ( (reg8 *)  LPWM_PWMHW__RT1)

#else
   #if (LPWM_Resolution == 8u) /* 8bit - PWM */

       #if(LPWM_PWMModeIsCenterAligned)
           #define LPWM_PERIOD_LSB      (*(reg8 *)  LPWM_PWMUDB_sP16_pwmdp_u0__D1_REG)
           #define LPWM_PERIOD_LSB_PTR  ((reg8 *)   LPWM_PWMUDB_sP16_pwmdp_u0__D1_REG)
       #else
           #define LPWM_PERIOD_LSB      (*(reg8 *)  LPWM_PWMUDB_sP16_pwmdp_u0__F0_REG)
           #define LPWM_PERIOD_LSB_PTR  ((reg8 *)   LPWM_PWMUDB_sP16_pwmdp_u0__F0_REG)
       #endif /* (LPWM_PWMModeIsCenterAligned) */

       #define LPWM_COMPARE1_LSB        (*(reg8 *)  LPWM_PWMUDB_sP16_pwmdp_u0__D0_REG)
       #define LPWM_COMPARE1_LSB_PTR    ((reg8 *)   LPWM_PWMUDB_sP16_pwmdp_u0__D0_REG)
       #define LPWM_COMPARE2_LSB        (*(reg8 *)  LPWM_PWMUDB_sP16_pwmdp_u0__D1_REG)
       #define LPWM_COMPARE2_LSB_PTR    ((reg8 *)   LPWM_PWMUDB_sP16_pwmdp_u0__D1_REG)
       #define LPWM_COUNTERCAP_LSB      (*(reg8 *)  LPWM_PWMUDB_sP16_pwmdp_u0__A1_REG)
       #define LPWM_COUNTERCAP_LSB_PTR  ((reg8 *)   LPWM_PWMUDB_sP16_pwmdp_u0__A1_REG)
       #define LPWM_COUNTER_LSB         (*(reg8 *)  LPWM_PWMUDB_sP16_pwmdp_u0__A0_REG)
       #define LPWM_COUNTER_LSB_PTR     ((reg8 *)   LPWM_PWMUDB_sP16_pwmdp_u0__A0_REG)
       #define LPWM_CAPTURE_LSB         (*(reg8 *)  LPWM_PWMUDB_sP16_pwmdp_u0__F1_REG)
       #define LPWM_CAPTURE_LSB_PTR     ((reg8 *)   LPWM_PWMUDB_sP16_pwmdp_u0__F1_REG)

   #else
        #if(CY_PSOC3) /* 8-bit address space */
            #if(LPWM_PWMModeIsCenterAligned)
               #define LPWM_PERIOD_LSB      (*(reg16 *) LPWM_PWMUDB_sP16_pwmdp_u0__D1_REG)
               #define LPWM_PERIOD_LSB_PTR  ((reg16 *)  LPWM_PWMUDB_sP16_pwmdp_u0__D1_REG)
            #else
               #define LPWM_PERIOD_LSB      (*(reg16 *) LPWM_PWMUDB_sP16_pwmdp_u0__F0_REG)
               #define LPWM_PERIOD_LSB_PTR  ((reg16 *)  LPWM_PWMUDB_sP16_pwmdp_u0__F0_REG)
            #endif /* (LPWM_PWMModeIsCenterAligned) */

            #define LPWM_COMPARE1_LSB       (*(reg16 *) LPWM_PWMUDB_sP16_pwmdp_u0__D0_REG)
            #define LPWM_COMPARE1_LSB_PTR   ((reg16 *)  LPWM_PWMUDB_sP16_pwmdp_u0__D0_REG)
            #define LPWM_COMPARE2_LSB       (*(reg16 *) LPWM_PWMUDB_sP16_pwmdp_u0__D1_REG)
            #define LPWM_COMPARE2_LSB_PTR   ((reg16 *)  LPWM_PWMUDB_sP16_pwmdp_u0__D1_REG)
            #define LPWM_COUNTERCAP_LSB     (*(reg16 *) LPWM_PWMUDB_sP16_pwmdp_u0__A1_REG)
            #define LPWM_COUNTERCAP_LSB_PTR ((reg16 *)  LPWM_PWMUDB_sP16_pwmdp_u0__A1_REG)
            #define LPWM_COUNTER_LSB        (*(reg16 *) LPWM_PWMUDB_sP16_pwmdp_u0__A0_REG)
            #define LPWM_COUNTER_LSB_PTR    ((reg16 *)  LPWM_PWMUDB_sP16_pwmdp_u0__A0_REG)
            #define LPWM_CAPTURE_LSB        (*(reg16 *) LPWM_PWMUDB_sP16_pwmdp_u0__F1_REG)
            #define LPWM_CAPTURE_LSB_PTR    ((reg16 *)  LPWM_PWMUDB_sP16_pwmdp_u0__F1_REG)
        #else
            #if(LPWM_PWMModeIsCenterAligned)
               #define LPWM_PERIOD_LSB      (*(reg16 *) LPWM_PWMUDB_sP16_pwmdp_u0__16BIT_D1_REG)
               #define LPWM_PERIOD_LSB_PTR  ((reg16 *)  LPWM_PWMUDB_sP16_pwmdp_u0__16BIT_D1_REG)
            #else
               #define LPWM_PERIOD_LSB      (*(reg16 *) LPWM_PWMUDB_sP16_pwmdp_u0__16BIT_F0_REG)
               #define LPWM_PERIOD_LSB_PTR  ((reg16 *)  LPWM_PWMUDB_sP16_pwmdp_u0__16BIT_F0_REG)
            #endif /* (LPWM_PWMModeIsCenterAligned) */

            #define LPWM_COMPARE1_LSB       (*(reg16 *) LPWM_PWMUDB_sP16_pwmdp_u0__16BIT_D0_REG)
            #define LPWM_COMPARE1_LSB_PTR   ((reg16 *)  LPWM_PWMUDB_sP16_pwmdp_u0__16BIT_D0_REG)
            #define LPWM_COMPARE2_LSB       (*(reg16 *) LPWM_PWMUDB_sP16_pwmdp_u0__16BIT_D1_REG)
            #define LPWM_COMPARE2_LSB_PTR   ((reg16 *)  LPWM_PWMUDB_sP16_pwmdp_u0__16BIT_D1_REG)
            #define LPWM_COUNTERCAP_LSB     (*(reg16 *) LPWM_PWMUDB_sP16_pwmdp_u0__16BIT_A1_REG)
            #define LPWM_COUNTERCAP_LSB_PTR ((reg16 *)  LPWM_PWMUDB_sP16_pwmdp_u0__16BIT_A1_REG)
            #define LPWM_COUNTER_LSB        (*(reg16 *) LPWM_PWMUDB_sP16_pwmdp_u0__16BIT_A0_REG)
            #define LPWM_COUNTER_LSB_PTR    ((reg16 *)  LPWM_PWMUDB_sP16_pwmdp_u0__16BIT_A0_REG)
            #define LPWM_CAPTURE_LSB        (*(reg16 *) LPWM_PWMUDB_sP16_pwmdp_u0__16BIT_F1_REG)
            #define LPWM_CAPTURE_LSB_PTR    ((reg16 *)  LPWM_PWMUDB_sP16_pwmdp_u0__16BIT_F1_REG)
        #endif /* (CY_PSOC3) */

       #define LPWM_AUX_CONTROLDP1          (*(reg8 *)  LPWM_PWMUDB_sP16_pwmdp_u1__DP_AUX_CTL_REG)
       #define LPWM_AUX_CONTROLDP1_PTR      ((reg8 *)   LPWM_PWMUDB_sP16_pwmdp_u1__DP_AUX_CTL_REG)

   #endif /* (LPWM_Resolution == 8) */

   #define LPWM_COUNTERCAP_LSB_PTR_8BIT ( (reg8 *)  LPWM_PWMUDB_sP16_pwmdp_u0__A1_REG)
   #define LPWM_AUX_CONTROLDP0          (*(reg8 *)  LPWM_PWMUDB_sP16_pwmdp_u0__DP_AUX_CTL_REG)
   #define LPWM_AUX_CONTROLDP0_PTR      ((reg8 *)   LPWM_PWMUDB_sP16_pwmdp_u0__DP_AUX_CTL_REG)

#endif /* (LPWM_UsingFixedFunction) */

#if(LPWM_KillModeMinTime )
    #define LPWM_KILLMODEMINTIME        (*(reg8 *)  LPWM_PWMUDB_sKM_killmodecounterdp_u0__D0_REG)
    #define LPWM_KILLMODEMINTIME_PTR    ((reg8 *)   LPWM_PWMUDB_sKM_killmodecounterdp_u0__D0_REG)
    /* Fixed Function Block has no Kill Mode parameters because it is Asynchronous only */
#endif /* (LPWM_KillModeMinTime ) */

#if(LPWM_DeadBandMode == LPWM__B_PWM__DBM_256_CLOCKS)
    #define LPWM_DEADBAND_COUNT         (*(reg8 *)  LPWM_PWMUDB_sDB255_deadbandcounterdp_u0__D0_REG)
    #define LPWM_DEADBAND_COUNT_PTR     ((reg8 *)   LPWM_PWMUDB_sDB255_deadbandcounterdp_u0__D0_REG)
    #define LPWM_DEADBAND_LSB_PTR       ((reg8 *)   LPWM_PWMUDB_sDB255_deadbandcounterdp_u0__A0_REG)
    #define LPWM_DEADBAND_LSB           (*(reg8 *)  LPWM_PWMUDB_sDB255_deadbandcounterdp_u0__A0_REG)
#elif(LPWM_DeadBandMode == LPWM__B_PWM__DBM_2_4_CLOCKS)
    
    /* In Fixed Function Block these bits are in the control blocks control register */
    #if (LPWM_UsingFixedFunction)
        #define LPWM_DEADBAND_COUNT         (*(reg8 *)  LPWM_PWMHW__CFG0)
        #define LPWM_DEADBAND_COUNT_PTR     ((reg8 *)   LPWM_PWMHW__CFG0)
        #define LPWM_DEADBAND_COUNT_MASK    (uint8)((uint8)0x03u << LPWM_DEADBAND_COUNT_SHIFT)

        /* As defined by the Register Map as DEADBAND_PERIOD[1:0] in CFG0 */
        #define LPWM_DEADBAND_COUNT_SHIFT   (0x06u)
    #else
        /* Lower two bits of the added control register define the count 1-3 */
        #define LPWM_DEADBAND_COUNT         (*(reg8 *)  LPWM_PWMUDB_genblk7_dbctrlreg__CONTROL_REG)
        #define LPWM_DEADBAND_COUNT_PTR     ((reg8 *)   LPWM_PWMUDB_genblk7_dbctrlreg__CONTROL_REG)
        #define LPWM_DEADBAND_COUNT_MASK    (uint8)((uint8)0x03u << LPWM_DEADBAND_COUNT_SHIFT)

        /* As defined by the verilog implementation of the Control Register */
        #define LPWM_DEADBAND_COUNT_SHIFT   (0x00u)
    #endif /* (LPWM_UsingFixedFunction) */
#endif /* (LPWM_DeadBandMode == LPWM__B_PWM__DBM_256_CLOCKS) */



#if (LPWM_UsingFixedFunction)
    #define LPWM_STATUS                 (*(reg8 *) LPWM_PWMHW__SR0)
    #define LPWM_STATUS_PTR             ((reg8 *) LPWM_PWMHW__SR0)
    #define LPWM_STATUS_MASK            (*(reg8 *) LPWM_PWMHW__SR0)
    #define LPWM_STATUS_MASK_PTR        ((reg8 *) LPWM_PWMHW__SR0)
    #define LPWM_CONTROL                (*(reg8 *) LPWM_PWMHW__CFG0)
    #define LPWM_CONTROL_PTR            ((reg8 *) LPWM_PWMHW__CFG0)
    #define LPWM_CONTROL2               (*(reg8 *) LPWM_PWMHW__CFG1)
    #define LPWM_CONTROL3               (*(reg8 *) LPWM_PWMHW__CFG2)
    #define LPWM_GLOBAL_ENABLE          (*(reg8 *) LPWM_PWMHW__PM_ACT_CFG)
    #define LPWM_GLOBAL_ENABLE_PTR      ( (reg8 *) LPWM_PWMHW__PM_ACT_CFG)
    #define LPWM_GLOBAL_STBY_ENABLE     (*(reg8 *) LPWM_PWMHW__PM_STBY_CFG)
    #define LPWM_GLOBAL_STBY_ENABLE_PTR ( (reg8 *) LPWM_PWMHW__PM_STBY_CFG)


    /***********************************
    *          Constants
    ***********************************/

    /* Fixed Function Block Chosen */
    #define LPWM_BLOCK_EN_MASK          (LPWM_PWMHW__PM_ACT_MSK)
    #define LPWM_BLOCK_STBY_EN_MASK     (LPWM_PWMHW__PM_STBY_MSK)
    
    /* Control Register definitions */
    #define LPWM_CTRL_ENABLE_SHIFT      (0x00u)

    /* As defined by Register map as MODE_CFG bits in CFG2*/
    #define LPWM_CTRL_CMPMODE1_SHIFT    (0x04u)

    /* As defined by Register map */
    #define LPWM_CTRL_DEAD_TIME_SHIFT   (0x06u)  

    /* Fixed Function Block Only CFG register bit definitions */
    /*  Set to compare mode */
    #define LPWM_CFG0_MODE              (0x02u)   

    /* Enable the block to run */
    #define LPWM_CFG0_ENABLE            (0x01u)   
    
    /* As defined by Register map as DB bit in CFG0 */
    #define LPWM_CFG0_DB                (0x20u)   

    /* Control Register Bit Masks */
    #define LPWM_CTRL_ENABLE            (uint8)((uint8)0x01u << LPWM_CTRL_ENABLE_SHIFT)
    #define LPWM_CTRL_RESET             (uint8)((uint8)0x01u << LPWM_CTRL_RESET_SHIFT)
    #define LPWM_CTRL_CMPMODE2_MASK     (uint8)((uint8)0x07u << LPWM_CTRL_CMPMODE2_SHIFT)
    #define LPWM_CTRL_CMPMODE1_MASK     (uint8)((uint8)0x07u << LPWM_CTRL_CMPMODE1_SHIFT)

    /* Control2 Register Bit Masks */
    /* As defined in Register Map, Part of the TMRX_CFG1 register */
    #define LPWM_CTRL2_IRQ_SEL_SHIFT    (0x00u)
    #define LPWM_CTRL2_IRQ_SEL          (uint8)((uint8)0x01u << LPWM_CTRL2_IRQ_SEL_SHIFT)

    /* Status Register Bit Locations */
    /* As defined by Register map as TC in SR0 */
    #define LPWM_STATUS_TC_SHIFT        (0x07u)   
    
    /* As defined by the Register map as CAP_CMP in SR0 */
    #define LPWM_STATUS_CMP1_SHIFT      (0x06u)   

    /* Status Register Interrupt Enable Bit Locations */
    #define LPWM_STATUS_KILL_INT_EN_MASK_SHIFT          (0x00u)
    #define LPWM_STATUS_TC_INT_EN_MASK_SHIFT            (LPWM_STATUS_TC_SHIFT - 4u)
    #define LPWM_STATUS_CMP2_INT_EN_MASK_SHIFT          (0x00u)
    #define LPWM_STATUS_CMP1_INT_EN_MASK_SHIFT          (LPWM_STATUS_CMP1_SHIFT - 4u)

    /* Status Register Bit Masks */
    #define LPWM_STATUS_TC              (uint8)((uint8)0x01u << LPWM_STATUS_TC_SHIFT)
    #define LPWM_STATUS_CMP1            (uint8)((uint8)0x01u << LPWM_STATUS_CMP1_SHIFT)

    /* Status Register Interrupt Bit Masks */
    #define LPWM_STATUS_TC_INT_EN_MASK              (uint8)((uint8)LPWM_STATUS_TC >> 4u)
    #define LPWM_STATUS_CMP1_INT_EN_MASK            (uint8)((uint8)LPWM_STATUS_CMP1 >> 4u)

    /*RT1 Synch Constants */
    #define LPWM_RT1_SHIFT             (0x04u)

    /* Sync TC and CMP bit masks */
    #define LPWM_RT1_MASK              (uint8)((uint8)0x03u << LPWM_RT1_SHIFT)
    #define LPWM_SYNC                  (uint8)((uint8)0x03u << LPWM_RT1_SHIFT)
    #define LPWM_SYNCDSI_SHIFT         (0x00u)

    /* Sync all DSI inputs */
    #define LPWM_SYNCDSI_MASK          (uint8)((uint8)0x0Fu << LPWM_SYNCDSI_SHIFT)

    /* Sync all DSI inputs */
    #define LPWM_SYNCDSI_EN            (uint8)((uint8)0x0Fu << LPWM_SYNCDSI_SHIFT)


#else
    #define LPWM_STATUS                (*(reg8 *)   LPWM_PWMUDB_genblk8_stsreg__STATUS_REG )
    #define LPWM_STATUS_PTR            ((reg8 *)    LPWM_PWMUDB_genblk8_stsreg__STATUS_REG )
    #define LPWM_STATUS_MASK           (*(reg8 *)   LPWM_PWMUDB_genblk8_stsreg__MASK_REG)
    #define LPWM_STATUS_MASK_PTR       ((reg8 *)    LPWM_PWMUDB_genblk8_stsreg__MASK_REG)
    #define LPWM_STATUS_AUX_CTRL       (*(reg8 *)   LPWM_PWMUDB_genblk8_stsreg__STATUS_AUX_CTL_REG)
    #define LPWM_CONTROL               (*(reg8 *)   LPWM_PWMUDB_genblk1_ctrlreg__CONTROL_REG)
    #define LPWM_CONTROL_PTR           ((reg8 *)    LPWM_PWMUDB_genblk1_ctrlreg__CONTROL_REG)


    /***********************************
    *          Constants
    ***********************************/

    /* Control Register bit definitions */
    #define LPWM_CTRL_ENABLE_SHIFT      (0x07u)
    #define LPWM_CTRL_RESET_SHIFT       (0x06u)
    #define LPWM_CTRL_CMPMODE2_SHIFT    (0x03u)
    #define LPWM_CTRL_CMPMODE1_SHIFT    (0x00u)
    #define LPWM_CTRL_DEAD_TIME_SHIFT   (0x00u)   /* No Shift Needed for UDB block */
    
    /* Control Register Bit Masks */
    #define LPWM_CTRL_ENABLE            (uint8)((uint8)0x01u << LPWM_CTRL_ENABLE_SHIFT)
    #define LPWM_CTRL_RESET             (uint8)((uint8)0x01u << LPWM_CTRL_RESET_SHIFT)
    #define LPWM_CTRL_CMPMODE2_MASK     (uint8)((uint8)0x07u << LPWM_CTRL_CMPMODE2_SHIFT)
    #define LPWM_CTRL_CMPMODE1_MASK     (uint8)((uint8)0x07u << LPWM_CTRL_CMPMODE1_SHIFT)

    /* Status Register Bit Locations */
    #define LPWM_STATUS_KILL_SHIFT          (0x05u)
    #define LPWM_STATUS_FIFONEMPTY_SHIFT    (0x04u)
    #define LPWM_STATUS_FIFOFULL_SHIFT      (0x03u)
    #define LPWM_STATUS_TC_SHIFT            (0x02u)
    #define LPWM_STATUS_CMP2_SHIFT          (0x01u)
    #define LPWM_STATUS_CMP1_SHIFT          (0x00u)

    /* Status Register Interrupt Enable Bit Locations - UDB Status Interrupt Mask match Status Bit Locations*/
    #define LPWM_STATUS_KILL_INT_EN_MASK_SHIFT          (LPWM_STATUS_KILL_SHIFT)
    #define LPWM_STATUS_FIFONEMPTY_INT_EN_MASK_SHIFT    (LPWM_STATUS_FIFONEMPTY_SHIFT)
    #define LPWM_STATUS_FIFOFULL_INT_EN_MASK_SHIFT      (LPWM_STATUS_FIFOFULL_SHIFT)
    #define LPWM_STATUS_TC_INT_EN_MASK_SHIFT            (LPWM_STATUS_TC_SHIFT)
    #define LPWM_STATUS_CMP2_INT_EN_MASK_SHIFT          (LPWM_STATUS_CMP2_SHIFT)
    #define LPWM_STATUS_CMP1_INT_EN_MASK_SHIFT          (LPWM_STATUS_CMP1_SHIFT)

    /* Status Register Bit Masks */
    #define LPWM_STATUS_KILL            (uint8)((uint8)0x00u << LPWM_STATUS_KILL_SHIFT )
    #define LPWM_STATUS_FIFOFULL        (uint8)((uint8)0x01u << LPWM_STATUS_FIFOFULL_SHIFT)
    #define LPWM_STATUS_FIFONEMPTY      (uint8)((uint8)0x01u << LPWM_STATUS_FIFONEMPTY_SHIFT)
    #define LPWM_STATUS_TC              (uint8)((uint8)0x01u << LPWM_STATUS_TC_SHIFT)
    #define LPWM_STATUS_CMP2            (uint8)((uint8)0x01u << LPWM_STATUS_CMP2_SHIFT)
    #define LPWM_STATUS_CMP1            (uint8)((uint8)0x01u << LPWM_STATUS_CMP1_SHIFT)

    /* Status Register Interrupt Bit Masks  - UDB Status Interrupt Mask match Status Bit Locations */
    #define LPWM_STATUS_KILL_INT_EN_MASK            (LPWM_STATUS_KILL)
    #define LPWM_STATUS_FIFOFULL_INT_EN_MASK        (LPWM_STATUS_FIFOFULL)
    #define LPWM_STATUS_FIFONEMPTY_INT_EN_MASK      (LPWM_STATUS_FIFONEMPTY)
    #define LPWM_STATUS_TC_INT_EN_MASK              (LPWM_STATUS_TC)
    #define LPWM_STATUS_CMP2_INT_EN_MASK            (LPWM_STATUS_CMP2)
    #define LPWM_STATUS_CMP1_INT_EN_MASK            (LPWM_STATUS_CMP1)

    /* Datapath Auxillary Control Register bit definitions */
    #define LPWM_AUX_CTRL_FIFO0_CLR         (0x01u)
    #define LPWM_AUX_CTRL_FIFO1_CLR         (0x02u)
    #define LPWM_AUX_CTRL_FIFO0_LVL         (0x04u)
    #define LPWM_AUX_CTRL_FIFO1_LVL         (0x08u)
    #define LPWM_STATUS_ACTL_INT_EN_MASK    (0x10u) /* As defined for the ACTL Register */
#endif /* LPWM_UsingFixedFunction */

#endif  /* CY_PWM_LPWM_H */


/* [] END OF FILE */
