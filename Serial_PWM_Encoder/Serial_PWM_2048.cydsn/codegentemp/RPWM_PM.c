/*******************************************************************************
* File Name: RPWM_PM.c
* Version 3.30
*
* Description:
*  This file provides the power management source code to API for the
*  PWM.
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "RPWM.h"

static RPWM_backupStruct RPWM_backup;


/*******************************************************************************
* Function Name: RPWM_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the current user configuration of the component.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  RPWM_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void RPWM_SaveConfig(void) 
{

    #if(!RPWM_UsingFixedFunction)
        #if(!RPWM_PWMModeIsCenterAligned)
            RPWM_backup.PWMPeriod = RPWM_ReadPeriod();
        #endif /* (!RPWM_PWMModeIsCenterAligned) */
        RPWM_backup.PWMUdb = RPWM_ReadCounter();
        #if (RPWM_UseStatus)
            RPWM_backup.InterruptMaskValue = RPWM_STATUS_MASK;
        #endif /* (RPWM_UseStatus) */

        #if(RPWM_DeadBandMode == RPWM__B_PWM__DBM_256_CLOCKS || \
            RPWM_DeadBandMode == RPWM__B_PWM__DBM_2_4_CLOCKS)
            RPWM_backup.PWMdeadBandValue = RPWM_ReadDeadTime();
        #endif /*  deadband count is either 2-4 clocks or 256 clocks */

        #if(RPWM_KillModeMinTime)
             RPWM_backup.PWMKillCounterPeriod = RPWM_ReadKillTime();
        #endif /* (RPWM_KillModeMinTime) */

        #if(RPWM_UseControl)
            RPWM_backup.PWMControlRegister = RPWM_ReadControlRegister();
        #endif /* (RPWM_UseControl) */
    #endif  /* (!RPWM_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: RPWM_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration of the component.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  RPWM_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void RPWM_RestoreConfig(void) 
{
        #if(!RPWM_UsingFixedFunction)
            #if(!RPWM_PWMModeIsCenterAligned)
                RPWM_WritePeriod(RPWM_backup.PWMPeriod);
            #endif /* (!RPWM_PWMModeIsCenterAligned) */

            RPWM_WriteCounter(RPWM_backup.PWMUdb);

            #if (RPWM_UseStatus)
                RPWM_STATUS_MASK = RPWM_backup.InterruptMaskValue;
            #endif /* (RPWM_UseStatus) */

            #if(RPWM_DeadBandMode == RPWM__B_PWM__DBM_256_CLOCKS || \
                RPWM_DeadBandMode == RPWM__B_PWM__DBM_2_4_CLOCKS)
                RPWM_WriteDeadTime(RPWM_backup.PWMdeadBandValue);
            #endif /* deadband count is either 2-4 clocks or 256 clocks */

            #if(RPWM_KillModeMinTime)
                RPWM_WriteKillTime(RPWM_backup.PWMKillCounterPeriod);
            #endif /* (RPWM_KillModeMinTime) */

            #if(RPWM_UseControl)
                RPWM_WriteControlRegister(RPWM_backup.PWMControlRegister);
            #endif /* (RPWM_UseControl) */
        #endif  /* (!RPWM_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: RPWM_Sleep
********************************************************************************
*
* Summary:
*  Disables block's operation and saves the user configuration. Should be called
*  just prior to entering sleep.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  RPWM_backup.PWMEnableState:  Is modified depending on the enable
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void RPWM_Sleep(void) 
{
    #if(RPWM_UseControl)
        if(RPWM_CTRL_ENABLE == (RPWM_CONTROL & RPWM_CTRL_ENABLE))
        {
            /*Component is enabled */
            RPWM_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            RPWM_backup.PWMEnableState = 0u;
        }
    #endif /* (RPWM_UseControl) */

    /* Stop component */
    RPWM_Stop();

    /* Save registers configuration */
    RPWM_SaveConfig();
}


/*******************************************************************************
* Function Name: RPWM_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration. Should be called just after
*  awaking from sleep.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  RPWM_backup.pwmEnable:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void RPWM_Wakeup(void) 
{
     /* Restore registers values */
    RPWM_RestoreConfig();

    if(RPWM_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        RPWM_Enable();
    } /* Do nothing if component's block was disabled before */

}


/* [] END OF FILE */
