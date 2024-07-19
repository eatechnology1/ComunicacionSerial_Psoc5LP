/*******************************************************************************
* File Name: LPWM.c
* Version 3.30
*
* Description:
*  The PWM User Module consist of an 8 or 16-bit counter with two 8 or 16-bit
*  comparitors. Each instance of this user module is capable of generating
*  two PWM outputs with the same period. The pulse width is selectable between
*  1 and 255/65535. The period is selectable between 2 and 255/65536 clocks.
*  The compare value output may be configured to be active when the present
*  counter is less than or less than/equal to the compare value.
*  A terminal count output is also provided. It generates a pulse one clock
*  width wide when the counter is equal to zero.
*
* Note:
*
*******************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#include "LPWM.h"

/* Error message for removed <resource> through optimization */
#ifdef LPWM_PWMUDB_genblk1_ctrlreg__REMOVED
    #error PWM_v3_30 detected with a constant 0 for the enable or \
         constant 1 for reset. This will prevent the component from operating.
#endif /* LPWM_PWMUDB_genblk1_ctrlreg__REMOVED */

uint8 LPWM_initVar = 0u;


/*******************************************************************************
* Function Name: LPWM_Start
********************************************************************************
*
* Summary:
*  The start function initializes the pwm with the default values, the
*  enables the counter to begin counting.  It does not enable interrupts,
*  the EnableInt command should be called if interrupt generation is required.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  LPWM_initVar: Is modified when this function is called for the
*   first time. Is used to ensure that initialization happens only once.
*
*******************************************************************************/
void LPWM_Start(void) 
{
    /* If not Initialized then initialize all required hardware and software */
    if(LPWM_initVar == 0u)
    {
        LPWM_Init();
        LPWM_initVar = 1u;
    }
    LPWM_Enable();

}


/*******************************************************************************
* Function Name: LPWM_Init
********************************************************************************
*
* Summary:
*  Initialize component's parameters to the parameters set by user in the
*  customizer of the component placed onto schematic. Usually called in
*  LPWM_Start().
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void LPWM_Init(void) 
{
    #if (LPWM_UsingFixedFunction || LPWM_UseControl)
        uint8 ctrl;
    #endif /* (LPWM_UsingFixedFunction || LPWM_UseControl) */

    #if(!LPWM_UsingFixedFunction)
        #if(LPWM_UseStatus)
            /* Interrupt State Backup for Critical Region*/
            uint8 LPWM_interruptState;
        #endif /* (LPWM_UseStatus) */
    #endif /* (!LPWM_UsingFixedFunction) */

    #if (LPWM_UsingFixedFunction)
        /* You are allowed to write the compare value (FF only) */
        LPWM_CONTROL |= LPWM_CFG0_MODE;
        #if (LPWM_DeadBand2_4)
            LPWM_CONTROL |= LPWM_CFG0_DB;
        #endif /* (LPWM_DeadBand2_4) */

        ctrl = LPWM_CONTROL3 & ((uint8 )(~LPWM_CTRL_CMPMODE1_MASK));
        LPWM_CONTROL3 = ctrl | LPWM_DEFAULT_COMPARE1_MODE;

         /* Clear and Set SYNCTC and SYNCCMP bits of RT1 register */
        LPWM_RT1 &= ((uint8)(~LPWM_RT1_MASK));
        LPWM_RT1 |= LPWM_SYNC;

        /*Enable DSI Sync all all inputs of the PWM*/
        LPWM_RT1 &= ((uint8)(~LPWM_SYNCDSI_MASK));
        LPWM_RT1 |= LPWM_SYNCDSI_EN;

    #elif (LPWM_UseControl)
        /* Set the default compare mode defined in the parameter */
        ctrl = LPWM_CONTROL & ((uint8)(~LPWM_CTRL_CMPMODE2_MASK)) &
                ((uint8)(~LPWM_CTRL_CMPMODE1_MASK));
        LPWM_CONTROL = ctrl | LPWM_DEFAULT_COMPARE2_MODE |
                                   LPWM_DEFAULT_COMPARE1_MODE;
    #endif /* (LPWM_UsingFixedFunction) */

    #if (!LPWM_UsingFixedFunction)
        #if (LPWM_Resolution == 8)
            /* Set FIFO 0 to 1 byte register for period*/
            LPWM_AUX_CONTROLDP0 |= (LPWM_AUX_CTRL_FIFO0_CLR);
        #else /* (LPWM_Resolution == 16)*/
            /* Set FIFO 0 to 1 byte register for period */
            LPWM_AUX_CONTROLDP0 |= (LPWM_AUX_CTRL_FIFO0_CLR);
            LPWM_AUX_CONTROLDP1 |= (LPWM_AUX_CTRL_FIFO0_CLR);
        #endif /* (LPWM_Resolution == 8) */

        LPWM_WriteCounter(LPWM_INIT_PERIOD_VALUE);
    #endif /* (!LPWM_UsingFixedFunction) */

    LPWM_WritePeriod(LPWM_INIT_PERIOD_VALUE);

        #if (LPWM_UseOneCompareMode)
            LPWM_WriteCompare(LPWM_INIT_COMPARE_VALUE1);
        #else
            LPWM_WriteCompare1(LPWM_INIT_COMPARE_VALUE1);
            LPWM_WriteCompare2(LPWM_INIT_COMPARE_VALUE2);
        #endif /* (LPWM_UseOneCompareMode) */

        #if (LPWM_KillModeMinTime)
            LPWM_WriteKillTime(LPWM_MinimumKillTime);
        #endif /* (LPWM_KillModeMinTime) */

        #if (LPWM_DeadBandUsed)
            LPWM_WriteDeadTime(LPWM_INIT_DEAD_TIME);
        #endif /* (LPWM_DeadBandUsed) */

    #if (LPWM_UseStatus || LPWM_UsingFixedFunction)
        LPWM_SetInterruptMode(LPWM_INIT_INTERRUPTS_MODE);
    #endif /* (LPWM_UseStatus || LPWM_UsingFixedFunction) */

    #if (LPWM_UsingFixedFunction)
        /* Globally Enable the Fixed Function Block chosen */
        LPWM_GLOBAL_ENABLE |= LPWM_BLOCK_EN_MASK;
        /* Set the Interrupt source to come from the status register */
        LPWM_CONTROL2 |= LPWM_CTRL2_IRQ_SEL;
    #else
        #if(LPWM_UseStatus)

            /* CyEnterCriticalRegion and CyExitCriticalRegion are used to mark following region critical*/
            /* Enter Critical Region*/
            LPWM_interruptState = CyEnterCriticalSection();
            /* Use the interrupt output of the status register for IRQ output */
            LPWM_STATUS_AUX_CTRL |= LPWM_STATUS_ACTL_INT_EN_MASK;

             /* Exit Critical Region*/
            CyExitCriticalSection(LPWM_interruptState);

            /* Clear the FIFO to enable the LPWM_STATUS_FIFOFULL
                   bit to be set on FIFO full. */
            LPWM_ClearFIFO();
        #endif /* (LPWM_UseStatus) */
    #endif /* (LPWM_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: LPWM_Enable
********************************************************************************
*
* Summary:
*  Enables the PWM block operation
*
* Parameters:
*  None
*
* Return:
*  None
*
* Side Effects:
*  This works only if software enable mode is chosen
*
*******************************************************************************/
void LPWM_Enable(void) 
{
    /* Globally Enable the Fixed Function Block chosen */
    #if (LPWM_UsingFixedFunction)
        LPWM_GLOBAL_ENABLE |= LPWM_BLOCK_EN_MASK;
        LPWM_GLOBAL_STBY_ENABLE |= LPWM_BLOCK_STBY_EN_MASK;
    #endif /* (LPWM_UsingFixedFunction) */

    /* Enable the PWM from the control register  */
    #if (LPWM_UseControl || LPWM_UsingFixedFunction)
        LPWM_CONTROL |= LPWM_CTRL_ENABLE;
    #endif /* (LPWM_UseControl || LPWM_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: LPWM_Stop
********************************************************************************
*
* Summary:
*  The stop function halts the PWM, but does not change any modes or disable
*  interrupts.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Side Effects:
*  If the Enable mode is set to Hardware only then this function
*  has no effect on the operation of the PWM
*
*******************************************************************************/
void LPWM_Stop(void) 
{
    #if (LPWM_UseControl || LPWM_UsingFixedFunction)
        LPWM_CONTROL &= ((uint8)(~LPWM_CTRL_ENABLE));
    #endif /* (LPWM_UseControl || LPWM_UsingFixedFunction) */

    /* Globally disable the Fixed Function Block chosen */
    #if (LPWM_UsingFixedFunction)
        LPWM_GLOBAL_ENABLE &= ((uint8)(~LPWM_BLOCK_EN_MASK));
        LPWM_GLOBAL_STBY_ENABLE &= ((uint8)(~LPWM_BLOCK_STBY_EN_MASK));
    #endif /* (LPWM_UsingFixedFunction) */
}

#if (LPWM_UseOneCompareMode)
    #if (LPWM_CompareMode1SW)


        /*******************************************************************************
        * Function Name: LPWM_SetCompareMode
        ********************************************************************************
        *
        * Summary:
        *  This function writes the Compare Mode for the pwm output when in Dither mode,
        *  Center Align Mode or One Output Mode.
        *
        * Parameters:
        *  comparemode:  The new compare mode for the PWM output. Use the compare types
        *                defined in the H file as input arguments.
        *
        * Return:
        *  None
        *
        *******************************************************************************/
        void LPWM_SetCompareMode(uint8 comparemode) 
        {
            #if(LPWM_UsingFixedFunction)

                #if(0 != LPWM_CTRL_CMPMODE1_SHIFT)
                    uint8 comparemodemasked = ((uint8)((uint8)comparemode << LPWM_CTRL_CMPMODE1_SHIFT));
                #else
                    uint8 comparemodemasked = comparemode;
                #endif /* (0 != LPWM_CTRL_CMPMODE1_SHIFT) */

                LPWM_CONTROL3 &= ((uint8)(~LPWM_CTRL_CMPMODE1_MASK)); /*Clear Existing Data */
                LPWM_CONTROL3 |= comparemodemasked;

            #elif (LPWM_UseControl)

                #if(0 != LPWM_CTRL_CMPMODE1_SHIFT)
                    uint8 comparemode1masked = ((uint8)((uint8)comparemode << LPWM_CTRL_CMPMODE1_SHIFT)) &
                                                LPWM_CTRL_CMPMODE1_MASK;
                #else
                    uint8 comparemode1masked = comparemode & LPWM_CTRL_CMPMODE1_MASK;
                #endif /* (0 != LPWM_CTRL_CMPMODE1_SHIFT) */

                #if(0 != LPWM_CTRL_CMPMODE2_SHIFT)
                    uint8 comparemode2masked = ((uint8)((uint8)comparemode << LPWM_CTRL_CMPMODE2_SHIFT)) &
                                               LPWM_CTRL_CMPMODE2_MASK;
                #else
                    uint8 comparemode2masked = comparemode & LPWM_CTRL_CMPMODE2_MASK;
                #endif /* (0 != LPWM_CTRL_CMPMODE2_SHIFT) */

                /*Clear existing mode */
                LPWM_CONTROL &= ((uint8)(~(LPWM_CTRL_CMPMODE1_MASK |
                                            LPWM_CTRL_CMPMODE2_MASK)));
                LPWM_CONTROL |= (comparemode1masked | comparemode2masked);

            #else
                uint8 temp = comparemode;
            #endif /* (LPWM_UsingFixedFunction) */
        }
    #endif /* LPWM_CompareMode1SW */

#else /* UseOneCompareMode */

    #if (LPWM_CompareMode1SW)


        /*******************************************************************************
        * Function Name: LPWM_SetCompareMode1
        ********************************************************************************
        *
        * Summary:
        *  This function writes the Compare Mode for the pwm or pwm1 output
        *
        * Parameters:
        *  comparemode:  The new compare mode for the PWM output. Use the compare types
        *                defined in the H file as input arguments.
        *
        * Return:
        *  None
        *
        *******************************************************************************/
        void LPWM_SetCompareMode1(uint8 comparemode) 
        {
            #if(0 != LPWM_CTRL_CMPMODE1_SHIFT)
                uint8 comparemodemasked = ((uint8)((uint8)comparemode << LPWM_CTRL_CMPMODE1_SHIFT)) &
                                           LPWM_CTRL_CMPMODE1_MASK;
            #else
                uint8 comparemodemasked = comparemode & LPWM_CTRL_CMPMODE1_MASK;
            #endif /* (0 != LPWM_CTRL_CMPMODE1_SHIFT) */

            #if (LPWM_UseControl)
                LPWM_CONTROL &= ((uint8)(~LPWM_CTRL_CMPMODE1_MASK)); /*Clear existing mode */
                LPWM_CONTROL |= comparemodemasked;
            #endif /* (LPWM_UseControl) */
        }
    #endif /* LPWM_CompareMode1SW */

#if (LPWM_CompareMode2SW)


    /*******************************************************************************
    * Function Name: LPWM_SetCompareMode2
    ********************************************************************************
    *
    * Summary:
    *  This function writes the Compare Mode for the pwm or pwm2 output
    *
    * Parameters:
    *  comparemode:  The new compare mode for the PWM output. Use the compare types
    *                defined in the H file as input arguments.
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void LPWM_SetCompareMode2(uint8 comparemode) 
    {

        #if(0 != LPWM_CTRL_CMPMODE2_SHIFT)
            uint8 comparemodemasked = ((uint8)((uint8)comparemode << LPWM_CTRL_CMPMODE2_SHIFT)) &
                                                 LPWM_CTRL_CMPMODE2_MASK;
        #else
            uint8 comparemodemasked = comparemode & LPWM_CTRL_CMPMODE2_MASK;
        #endif /* (0 != LPWM_CTRL_CMPMODE2_SHIFT) */

        #if (LPWM_UseControl)
            LPWM_CONTROL &= ((uint8)(~LPWM_CTRL_CMPMODE2_MASK)); /*Clear existing mode */
            LPWM_CONTROL |= comparemodemasked;
        #endif /* (LPWM_UseControl) */
    }
    #endif /*LPWM_CompareMode2SW */

#endif /* UseOneCompareMode */


#if (!LPWM_UsingFixedFunction)


    /*******************************************************************************
    * Function Name: LPWM_WriteCounter
    ********************************************************************************
    *
    * Summary:
    *  Writes a new counter value directly to the counter register. This will be
    *  implemented for that currently running period and only that period. This API
    *  is valid only for UDB implementation and not available for fixed function
    *  PWM implementation.
    *
    * Parameters:
    *  counter:  The period new period counter value.
    *
    * Return:
    *  None
    *
    * Side Effects:
    *  The PWM Period will be reloaded when a counter value will be a zero
    *
    *******************************************************************************/
    void LPWM_WriteCounter(uint16 counter) \
                                       
    {
        CY_SET_REG16(LPWM_COUNTER_LSB_PTR, counter);
    }


    /*******************************************************************************
    * Function Name: LPWM_ReadCounter
    ********************************************************************************
    *
    * Summary:
    *  This function returns the current value of the counter.  It doesn't matter
    *  if the counter is enabled or running.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  The current value of the counter.
    *
    *******************************************************************************/
    uint16 LPWM_ReadCounter(void) 
    {
        /* Force capture by reading Accumulator */
        /* Must first do a software capture to be able to read the counter */
        /* It is up to the user code to make sure there isn't already captured data in the FIFO */
          (void)CY_GET_REG8(LPWM_COUNTERCAP_LSB_PTR_8BIT);

        /* Read the data from the FIFO */
        return (CY_GET_REG16(LPWM_CAPTURE_LSB_PTR));
    }

    #if (LPWM_UseStatus)


        /*******************************************************************************
        * Function Name: LPWM_ClearFIFO
        ********************************************************************************
        *
        * Summary:
        *  This function clears all capture data from the capture FIFO
        *
        * Parameters:
        *  None
        *
        * Return:
        *  None
        *
        *******************************************************************************/
        void LPWM_ClearFIFO(void) 
        {
            while(0u != (LPWM_ReadStatusRegister() & LPWM_STATUS_FIFONEMPTY))
            {
                (void)LPWM_ReadCapture();
            }
        }

    #endif /* LPWM_UseStatus */

#endif /* !LPWM_UsingFixedFunction */


/*******************************************************************************
* Function Name: LPWM_WritePeriod
********************************************************************************
*
* Summary:
*  This function is used to change the period of the counter.  The new period
*  will be loaded the next time terminal count is detected.
*
* Parameters:
*  period:  Period value. May be between 1 and (2^Resolution)-1.  A value of 0
*           will result in the counter remaining at zero.
*
* Return:
*  None
*
*******************************************************************************/
void LPWM_WritePeriod(uint16 period) 
{
    #if(LPWM_UsingFixedFunction)
        CY_SET_REG16(LPWM_PERIOD_LSB_PTR, (uint16)period);
    #else
        CY_SET_REG16(LPWM_PERIOD_LSB_PTR, period);
    #endif /* (LPWM_UsingFixedFunction) */
}

#if (LPWM_UseOneCompareMode)


    /*******************************************************************************
    * Function Name: LPWM_WriteCompare
    ********************************************************************************
    *
    * Summary:
    *  This funtion is used to change the compare1 value when the PWM is in Dither
    *  mode. The compare output will reflect the new value on the next UDB clock.
    *  The compare output will be driven high when the present counter value is
    *  compared to the compare value based on the compare mode defined in
    *  Dither Mode.
    *
    * Parameters:
    *  compare:  New compare value.
    *
    * Return:
    *  None
    *
    * Side Effects:
    *  This function is only available if the PWM mode parameter is set to
    *  Dither Mode, Center Aligned Mode or One Output Mode
    *
    *******************************************************************************/
    void LPWM_WriteCompare(uint16 compare) \
                                       
    {
        #if(LPWM_UsingFixedFunction)
            CY_SET_REG16(LPWM_COMPARE1_LSB_PTR, (uint16)compare);
        #else
            CY_SET_REG16(LPWM_COMPARE1_LSB_PTR, compare);
        #endif /* (LPWM_UsingFixedFunction) */

        #if (LPWM_PWMMode == LPWM__B_PWM__DITHER)
            #if(LPWM_UsingFixedFunction)
                CY_SET_REG16(LPWM_COMPARE2_LSB_PTR, (uint16)(compare + 1u));
            #else
                CY_SET_REG16(LPWM_COMPARE2_LSB_PTR, (compare + 1u));
            #endif /* (LPWM_UsingFixedFunction) */
        #endif /* (LPWM_PWMMode == LPWM__B_PWM__DITHER) */
    }


#else


    /*******************************************************************************
    * Function Name: LPWM_WriteCompare1
    ********************************************************************************
    *
    * Summary:
    *  This funtion is used to change the compare1 value.  The compare output will
    *  reflect the new value on the next UDB clock.  The compare output will be
    *  driven high when the present counter value is less than or less than or
    *  equal to the compare register, depending on the mode.
    *
    * Parameters:
    *  compare:  New compare value.
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void LPWM_WriteCompare1(uint16 compare) \
                                        
    {
        #if(LPWM_UsingFixedFunction)
            CY_SET_REG16(LPWM_COMPARE1_LSB_PTR, (uint16)compare);
        #else
            CY_SET_REG16(LPWM_COMPARE1_LSB_PTR, compare);
        #endif /* (LPWM_UsingFixedFunction) */
    }


    /*******************************************************************************
    * Function Name: LPWM_WriteCompare2
    ********************************************************************************
    *
    * Summary:
    *  This funtion is used to change the compare value, for compare1 output.
    *  The compare output will reflect the new value on the next UDB clock.
    *  The compare output will be driven high when the present counter value is
    *  less than or less than or equal to the compare register, depending on the
    *  mode.
    *
    * Parameters:
    *  compare:  New compare value.
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void LPWM_WriteCompare2(uint16 compare) \
                                        
    {
        #if(LPWM_UsingFixedFunction)
            CY_SET_REG16(LPWM_COMPARE2_LSB_PTR, compare);
        #else
            CY_SET_REG16(LPWM_COMPARE2_LSB_PTR, compare);
        #endif /* (LPWM_UsingFixedFunction) */
    }
#endif /* UseOneCompareMode */

#if (LPWM_DeadBandUsed)


    /*******************************************************************************
    * Function Name: LPWM_WriteDeadTime
    ********************************************************************************
    *
    * Summary:
    *  This function writes the dead-band counts to the corresponding register
    *
    * Parameters:
    *  deadtime:  Number of counts for dead time
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void LPWM_WriteDeadTime(uint8 deadtime) 
    {
        /* If using the Dead Band 1-255 mode then just write the register */
        #if(!LPWM_DeadBand2_4)
            CY_SET_REG8(LPWM_DEADBAND_COUNT_PTR, deadtime);
        #else
            /* Otherwise the data has to be masked and offset */
            /* Clear existing data */
            LPWM_DEADBAND_COUNT &= ((uint8)(~LPWM_DEADBAND_COUNT_MASK));

            /* Set new dead time */
            #if(LPWM_DEADBAND_COUNT_SHIFT)
                LPWM_DEADBAND_COUNT |= ((uint8)((uint8)deadtime << LPWM_DEADBAND_COUNT_SHIFT)) &
                                                    LPWM_DEADBAND_COUNT_MASK;
            #else
                LPWM_DEADBAND_COUNT |= deadtime & LPWM_DEADBAND_COUNT_MASK;
            #endif /* (LPWM_DEADBAND_COUNT_SHIFT) */

        #endif /* (!LPWM_DeadBand2_4) */
    }


    /*******************************************************************************
    * Function Name: LPWM_ReadDeadTime
    ********************************************************************************
    *
    * Summary:
    *  This function reads the dead-band counts from the corresponding register
    *
    * Parameters:
    *  None
    *
    * Return:
    *  Dead Band Counts
    *
    *******************************************************************************/
    uint8 LPWM_ReadDeadTime(void) 
    {
        /* If using the Dead Band 1-255 mode then just read the register */
        #if(!LPWM_DeadBand2_4)
            return (CY_GET_REG8(LPWM_DEADBAND_COUNT_PTR));
        #else

            /* Otherwise the data has to be masked and offset */
            #if(LPWM_DEADBAND_COUNT_SHIFT)
                return ((uint8)(((uint8)(LPWM_DEADBAND_COUNT & LPWM_DEADBAND_COUNT_MASK)) >>
                                                                           LPWM_DEADBAND_COUNT_SHIFT));
            #else
                return (LPWM_DEADBAND_COUNT & LPWM_DEADBAND_COUNT_MASK);
            #endif /* (LPWM_DEADBAND_COUNT_SHIFT) */
        #endif /* (!LPWM_DeadBand2_4) */
    }
#endif /* DeadBandUsed */

#if (LPWM_UseStatus || LPWM_UsingFixedFunction)


    /*******************************************************************************
    * Function Name: LPWM_SetInterruptMode
    ********************************************************************************
    *
    * Summary:
    *  This function configures the interrupts mask control of theinterrupt
    *  source status register.
    *
    * Parameters:
    *  uint8 interruptMode: Bit field containing the interrupt sources enabled
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void LPWM_SetInterruptMode(uint8 interruptMode) 
    {
        CY_SET_REG8(LPWM_STATUS_MASK_PTR, interruptMode);
    }


    /*******************************************************************************
    * Function Name: LPWM_ReadStatusRegister
    ********************************************************************************
    *
    * Summary:
    *  This function returns the current state of the status register.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8 : Current status register value. The status register bits are:
    *  [7:6] : Unused(0)
    *  [5]   : Kill event output
    *  [4]   : FIFO not empty
    *  [3]   : FIFO full
    *  [2]   : Terminal count
    *  [1]   : Compare output 2
    *  [0]   : Compare output 1
    *
    *******************************************************************************/
    uint8 LPWM_ReadStatusRegister(void) 
    {
        return (CY_GET_REG8(LPWM_STATUS_PTR));
    }

#endif /* (LPWM_UseStatus || LPWM_UsingFixedFunction) */


#if (LPWM_UseControl)


    /*******************************************************************************
    * Function Name: LPWM_ReadControlRegister
    ********************************************************************************
    *
    * Summary:
    *  Returns the current state of the control register. This API is available
    *  only if the control register is not removed.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8 : Current control register value
    *
    *******************************************************************************/
    uint8 LPWM_ReadControlRegister(void) 
    {
        uint8 result;

        result = CY_GET_REG8(LPWM_CONTROL_PTR);
        return (result);
    }


    /*******************************************************************************
    * Function Name: LPWM_WriteControlRegister
    ********************************************************************************
    *
    * Summary:
    *  Sets the bit field of the control register. This API is available only if
    *  the control register is not removed.
    *
    * Parameters:
    *  uint8 control: Control register bit field, The status register bits are:
    *  [7]   : PWM Enable
    *  [6]   : Reset
    *  [5:3] : Compare Mode2
    *  [2:0] : Compare Mode2
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void LPWM_WriteControlRegister(uint8 control) 
    {
        CY_SET_REG8(LPWM_CONTROL_PTR, control);
    }

#endif /* (LPWM_UseControl) */


#if (!LPWM_UsingFixedFunction)


    /*******************************************************************************
    * Function Name: LPWM_ReadCapture
    ********************************************************************************
    *
    * Summary:
    *  Reads the capture value from the capture FIFO.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8/uint16: The current capture value
    *
    *******************************************************************************/
    uint16 LPWM_ReadCapture(void) 
    {
        return (CY_GET_REG16(LPWM_CAPTURE_LSB_PTR));
    }

#endif /* (!LPWM_UsingFixedFunction) */


#if (LPWM_UseOneCompareMode)


    /*******************************************************************************
    * Function Name: LPWM_ReadCompare
    ********************************************************************************
    *
    * Summary:
    *  Reads the compare value for the compare output when the PWM Mode parameter is
    *  set to Dither mode, Center Aligned mode, or One Output mode.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8/uint16: Current compare value
    *
    *******************************************************************************/
    uint16 LPWM_ReadCompare(void) 
    {
        #if(LPWM_UsingFixedFunction)
            return ((uint16)CY_GET_REG16(LPWM_COMPARE1_LSB_PTR));
        #else
            return (CY_GET_REG16(LPWM_COMPARE1_LSB_PTR));
        #endif /* (LPWM_UsingFixedFunction) */
    }

#else


    /*******************************************************************************
    * Function Name: LPWM_ReadCompare1
    ********************************************************************************
    *
    * Summary:
    *  Reads the compare value for the compare1 output.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8/uint16: Current compare value.
    *
    *******************************************************************************/
    uint16 LPWM_ReadCompare1(void) 
    {
        return (CY_GET_REG16(LPWM_COMPARE1_LSB_PTR));
    }


    /*******************************************************************************
    * Function Name: LPWM_ReadCompare2
    ********************************************************************************
    *
    * Summary:
    *  Reads the compare value for the compare2 output.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8/uint16: Current compare value.
    *
    *******************************************************************************/
    uint16 LPWM_ReadCompare2(void) 
    {
        return (CY_GET_REG16(LPWM_COMPARE2_LSB_PTR));
    }

#endif /* (LPWM_UseOneCompareMode) */


/*******************************************************************************
* Function Name: LPWM_ReadPeriod
********************************************************************************
*
* Summary:
*  Reads the period value used by the PWM hardware.
*
* Parameters:
*  None
*
* Return:
*  uint8/16: Period value
*
*******************************************************************************/
uint16 LPWM_ReadPeriod(void) 
{
    #if(LPWM_UsingFixedFunction)
        return ((uint16)CY_GET_REG16(LPWM_PERIOD_LSB_PTR));
    #else
        return (CY_GET_REG16(LPWM_PERIOD_LSB_PTR));
    #endif /* (LPWM_UsingFixedFunction) */
}

#if ( LPWM_KillModeMinTime)


    /*******************************************************************************
    * Function Name: LPWM_WriteKillTime
    ********************************************************************************
    *
    * Summary:
    *  Writes the kill time value used by the hardware when the Kill Mode
    *  is set to Minimum Time.
    *
    * Parameters:
    *  uint8: Minimum Time kill counts
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void LPWM_WriteKillTime(uint8 killtime) 
    {
        CY_SET_REG8(LPWM_KILLMODEMINTIME_PTR, killtime);
    }


    /*******************************************************************************
    * Function Name: LPWM_ReadKillTime
    ********************************************************************************
    *
    * Summary:
    *  Reads the kill time value used by the hardware when the Kill Mode is set
    *  to Minimum Time.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8: The current Minimum Time kill counts
    *
    *******************************************************************************/
    uint8 LPWM_ReadKillTime(void) 
    {
        return (CY_GET_REG8(LPWM_KILLMODEMINTIME_PTR));
    }

#endif /* ( LPWM_KillModeMinTime) */

/* [] END OF FILE */
