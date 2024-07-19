/*******************************************************************************
* File Name: LPWM_PM.c
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

#include "LPWM.h"

static LPWM_backupStruct LPWM_backup;


/*******************************************************************************
* Function Name: LPWM_SaveConfig
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
*  LPWM_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void LPWM_SaveConfig(void) 
{

    #if(!LPWM_UsingFixedFunction)
        #if(!LPWM_PWMModeIsCenterAligned)
            LPWM_backup.PWMPeriod = LPWM_ReadPeriod();
        #endif /* (!LPWM_PWMModeIsCenterAligned) */
        LPWM_backup.PWMUdb = LPWM_ReadCounter();
        #if (LPWM_UseStatus)
            LPWM_backup.InterruptMaskValue = LPWM_STATUS_MASK;
        #endif /* (LPWM_UseStatus) */

        #if(LPWM_DeadBandMode == LPWM__B_PWM__DBM_256_CLOCKS || \
            LPWM_DeadBandMode == LPWM__B_PWM__DBM_2_4_CLOCKS)
            LPWM_backup.PWMdeadBandValue = LPWM_ReadDeadTime();
        #endif /*  deadband count is either 2-4 clocks or 256 clocks */

        #if(LPWM_KillModeMinTime)
             LPWM_backup.PWMKillCounterPeriod = LPWM_ReadKillTime();
        #endif /* (LPWM_KillModeMinTime) */

        #if(LPWM_UseControl)
            LPWM_backup.PWMControlRegister = LPWM_ReadControlRegister();
        #endif /* (LPWM_UseControl) */
    #endif  /* (!LPWM_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: LPWM_RestoreConfig
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
*  LPWM_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void LPWM_RestoreConfig(void) 
{
        #if(!LPWM_UsingFixedFunction)
            #if(!LPWM_PWMModeIsCenterAligned)
                LPWM_WritePeriod(LPWM_backup.PWMPeriod);
            #endif /* (!LPWM_PWMModeIsCenterAligned) */

            LPWM_WriteCounter(LPWM_backup.PWMUdb);

            #if (LPWM_UseStatus)
                LPWM_STATUS_MASK = LPWM_backup.InterruptMaskValue;
            #endif /* (LPWM_UseStatus) */

            #if(LPWM_DeadBandMode == LPWM__B_PWM__DBM_256_CLOCKS || \
                LPWM_DeadBandMode == LPWM__B_PWM__DBM_2_4_CLOCKS)
                LPWM_WriteDeadTime(LPWM_backup.PWMdeadBandValue);
            #endif /* deadband count is either 2-4 clocks or 256 clocks */

            #if(LPWM_KillModeMinTime)
                LPWM_WriteKillTime(LPWM_backup.PWMKillCounterPeriod);
            #endif /* (LPWM_KillModeMinTime) */

            #if(LPWM_UseControl)
                LPWM_WriteControlRegister(LPWM_backup.PWMControlRegister);
            #endif /* (LPWM_UseControl) */
        #endif  /* (!LPWM_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: LPWM_Sleep
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
*  LPWM_backup.PWMEnableState:  Is modified depending on the enable
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void LPWM_Sleep(void) 
{
    #if(LPWM_UseControl)
        if(LPWM_CTRL_ENABLE == (LPWM_CONTROL & LPWM_CTRL_ENABLE))
        {
            /*Component is enabled */
            LPWM_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            LPWM_backup.PWMEnableState = 0u;
        }
    #endif /* (LPWM_UseControl) */

    /* Stop component */
    LPWM_Stop();

    /* Save registers configuration */
    LPWM_SaveConfig();
}


/*******************************************************************************
* Function Name: LPWM_Wakeup
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
*  LPWM_backup.pwmEnable:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void LPWM_Wakeup(void) 
{
     /* Restore registers values */
    LPWM_RestoreConfig();

    if(LPWM_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        LPWM_Enable();
    } /* Do nothing if component's block was disabled before */

}


/* [] END OF FILE */
