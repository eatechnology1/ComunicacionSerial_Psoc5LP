/*******************************************************************************
* File Name: RPWM.c
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

#include "RPWM.h"

/* Error message for removed <resource> through optimization */
#ifdef RPWM_PWMUDB_genblk1_ctrlreg__REMOVED
    #error PWM_v3_30 detected with a constant 0 for the enable or \
         constant 1 for reset. This will prevent the component from operating.
#endif /* RPWM_PWMUDB_genblk1_ctrlreg__REMOVED */

uint8 RPWM_initVar = 0u;


/*******************************************************************************
* Function Name: RPWM_Start
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
*  RPWM_initVar: Is modified when this function is called for the
*   first time. Is used to ensure that initialization happens only once.
*
*******************************************************************************/
void RPWM_Start(void) 
{
    /* If not Initialized then initialize all required hardware and software */
    if(RPWM_initVar == 0u)
    {
        RPWM_Init();
        RPWM_initVar = 1u;
    }
    RPWM_Enable();

}


/*******************************************************************************
* Function Name: RPWM_Init
********************************************************************************
*
* Summary:
*  Initialize component's parameters to the parameters set by user in the
*  customizer of the component placed onto schematic. Usually called in
*  RPWM_Start().
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void RPWM_Init(void) 
{
    #if (RPWM_UsingFixedFunction || RPWM_UseControl)
        uint8 ctrl;
    #endif /* (RPWM_UsingFixedFunction || RPWM_UseControl) */

    #if(!RPWM_UsingFixedFunction)
        #if(RPWM_UseStatus)
            /* Interrupt State Backup for Critical Region*/
            uint8 RPWM_interruptState;
        #endif /* (RPWM_UseStatus) */
    #endif /* (!RPWM_UsingFixedFunction) */

    #if (RPWM_UsingFixedFunction)
        /* You are allowed to write the compare value (FF only) */
        RPWM_CONTROL |= RPWM_CFG0_MODE;
        #if (RPWM_DeadBand2_4)
            RPWM_CONTROL |= RPWM_CFG0_DB;
        #endif /* (RPWM_DeadBand2_4) */

        ctrl = RPWM_CONTROL3 & ((uint8 )(~RPWM_CTRL_CMPMODE1_MASK));
        RPWM_CONTROL3 = ctrl | RPWM_DEFAULT_COMPARE1_MODE;

         /* Clear and Set SYNCTC and SYNCCMP bits of RT1 register */
        RPWM_RT1 &= ((uint8)(~RPWM_RT1_MASK));
        RPWM_RT1 |= RPWM_SYNC;

        /*Enable DSI Sync all all inputs of the PWM*/
        RPWM_RT1 &= ((uint8)(~RPWM_SYNCDSI_MASK));
        RPWM_RT1 |= RPWM_SYNCDSI_EN;

    #elif (RPWM_UseControl)
        /* Set the default compare mode defined in the parameter */
        ctrl = RPWM_CONTROL & ((uint8)(~RPWM_CTRL_CMPMODE2_MASK)) &
                ((uint8)(~RPWM_CTRL_CMPMODE1_MASK));
        RPWM_CONTROL = ctrl | RPWM_DEFAULT_COMPARE2_MODE |
                                   RPWM_DEFAULT_COMPARE1_MODE;
    #endif /* (RPWM_UsingFixedFunction) */

    #if (!RPWM_UsingFixedFunction)
        #if (RPWM_Resolution == 8)
            /* Set FIFO 0 to 1 byte register for period*/
            RPWM_AUX_CONTROLDP0 |= (RPWM_AUX_CTRL_FIFO0_CLR);
        #else /* (RPWM_Resolution == 16)*/
            /* Set FIFO 0 to 1 byte register for period */
            RPWM_AUX_CONTROLDP0 |= (RPWM_AUX_CTRL_FIFO0_CLR);
            RPWM_AUX_CONTROLDP1 |= (RPWM_AUX_CTRL_FIFO0_CLR);
        #endif /* (RPWM_Resolution == 8) */

        RPWM_WriteCounter(RPWM_INIT_PERIOD_VALUE);
    #endif /* (!RPWM_UsingFixedFunction) */

    RPWM_WritePeriod(RPWM_INIT_PERIOD_VALUE);

        #if (RPWM_UseOneCompareMode)
            RPWM_WriteCompare(RPWM_INIT_COMPARE_VALUE1);
        #else
            RPWM_WriteCompare1(RPWM_INIT_COMPARE_VALUE1);
            RPWM_WriteCompare2(RPWM_INIT_COMPARE_VALUE2);
        #endif /* (RPWM_UseOneCompareMode) */

        #if (RPWM_KillModeMinTime)
            RPWM_WriteKillTime(RPWM_MinimumKillTime);
        #endif /* (RPWM_KillModeMinTime) */

        #if (RPWM_DeadBandUsed)
            RPWM_WriteDeadTime(RPWM_INIT_DEAD_TIME);
        #endif /* (RPWM_DeadBandUsed) */

    #if (RPWM_UseStatus || RPWM_UsingFixedFunction)
        RPWM_SetInterruptMode(RPWM_INIT_INTERRUPTS_MODE);
    #endif /* (RPWM_UseStatus || RPWM_UsingFixedFunction) */

    #if (RPWM_UsingFixedFunction)
        /* Globally Enable the Fixed Function Block chosen */
        RPWM_GLOBAL_ENABLE |= RPWM_BLOCK_EN_MASK;
        /* Set the Interrupt source to come from the status register */
        RPWM_CONTROL2 |= RPWM_CTRL2_IRQ_SEL;
    #else
        #if(RPWM_UseStatus)

            /* CyEnterCriticalRegion and CyExitCriticalRegion are used to mark following region critical*/
            /* Enter Critical Region*/
            RPWM_interruptState = CyEnterCriticalSection();
            /* Use the interrupt output of the status register for IRQ output */
            RPWM_STATUS_AUX_CTRL |= RPWM_STATUS_ACTL_INT_EN_MASK;

             /* Exit Critical Region*/
            CyExitCriticalSection(RPWM_interruptState);

            /* Clear the FIFO to enable the RPWM_STATUS_FIFOFULL
                   bit to be set on FIFO full. */
            RPWM_ClearFIFO();
        #endif /* (RPWM_UseStatus) */
    #endif /* (RPWM_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: RPWM_Enable
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
void RPWM_Enable(void) 
{
    /* Globally Enable the Fixed Function Block chosen */
    #if (RPWM_UsingFixedFunction)
        RPWM_GLOBAL_ENABLE |= RPWM_BLOCK_EN_MASK;
        RPWM_GLOBAL_STBY_ENABLE |= RPWM_BLOCK_STBY_EN_MASK;
    #endif /* (RPWM_UsingFixedFunction) */

    /* Enable the PWM from the control register  */
    #if (RPWM_UseControl || RPWM_UsingFixedFunction)
        RPWM_CONTROL |= RPWM_CTRL_ENABLE;
    #endif /* (RPWM_UseControl || RPWM_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: RPWM_Stop
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
void RPWM_Stop(void) 
{
    #if (RPWM_UseControl || RPWM_UsingFixedFunction)
        RPWM_CONTROL &= ((uint8)(~RPWM_CTRL_ENABLE));
    #endif /* (RPWM_UseControl || RPWM_UsingFixedFunction) */

    /* Globally disable the Fixed Function Block chosen */
    #if (RPWM_UsingFixedFunction)
        RPWM_GLOBAL_ENABLE &= ((uint8)(~RPWM_BLOCK_EN_MASK));
        RPWM_GLOBAL_STBY_ENABLE &= ((uint8)(~RPWM_BLOCK_STBY_EN_MASK));
    #endif /* (RPWM_UsingFixedFunction) */
}

#if (RPWM_UseOneCompareMode)
    #if (RPWM_CompareMode1SW)


        /*******************************************************************************
        * Function Name: RPWM_SetCompareMode
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
        void RPWM_SetCompareMode(uint8 comparemode) 
        {
            #if(RPWM_UsingFixedFunction)

                #if(0 != RPWM_CTRL_CMPMODE1_SHIFT)
                    uint8 comparemodemasked = ((uint8)((uint8)comparemode << RPWM_CTRL_CMPMODE1_SHIFT));
                #else
                    uint8 comparemodemasked = comparemode;
                #endif /* (0 != RPWM_CTRL_CMPMODE1_SHIFT) */

                RPWM_CONTROL3 &= ((uint8)(~RPWM_CTRL_CMPMODE1_MASK)); /*Clear Existing Data */
                RPWM_CONTROL3 |= comparemodemasked;

            #elif (RPWM_UseControl)

                #if(0 != RPWM_CTRL_CMPMODE1_SHIFT)
                    uint8 comparemode1masked = ((uint8)((uint8)comparemode << RPWM_CTRL_CMPMODE1_SHIFT)) &
                                                RPWM_CTRL_CMPMODE1_MASK;
                #else
                    uint8 comparemode1masked = comparemode & RPWM_CTRL_CMPMODE1_MASK;
                #endif /* (0 != RPWM_CTRL_CMPMODE1_SHIFT) */

                #if(0 != RPWM_CTRL_CMPMODE2_SHIFT)
                    uint8 comparemode2masked = ((uint8)((uint8)comparemode << RPWM_CTRL_CMPMODE2_SHIFT)) &
                                               RPWM_CTRL_CMPMODE2_MASK;
                #else
                    uint8 comparemode2masked = comparemode & RPWM_CTRL_CMPMODE2_MASK;
                #endif /* (0 != RPWM_CTRL_CMPMODE2_SHIFT) */

                /*Clear existing mode */
                RPWM_CONTROL &= ((uint8)(~(RPWM_CTRL_CMPMODE1_MASK |
                                            RPWM_CTRL_CMPMODE2_MASK)));
                RPWM_CONTROL |= (comparemode1masked | comparemode2masked);

            #else
                uint8 temp = comparemode;
            #endif /* (RPWM_UsingFixedFunction) */
        }
    #endif /* RPWM_CompareMode1SW */

#else /* UseOneCompareMode */

    #if (RPWM_CompareMode1SW)


        /*******************************************************************************
        * Function Name: RPWM_SetCompareMode1
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
        void RPWM_SetCompareMode1(uint8 comparemode) 
        {
            #if(0 != RPWM_CTRL_CMPMODE1_SHIFT)
                uint8 comparemodemasked = ((uint8)((uint8)comparemode << RPWM_CTRL_CMPMODE1_SHIFT)) &
                                           RPWM_CTRL_CMPMODE1_MASK;
            #else
                uint8 comparemodemasked = comparemode & RPWM_CTRL_CMPMODE1_MASK;
            #endif /* (0 != RPWM_CTRL_CMPMODE1_SHIFT) */

            #if (RPWM_UseControl)
                RPWM_CONTROL &= ((uint8)(~RPWM_CTRL_CMPMODE1_MASK)); /*Clear existing mode */
                RPWM_CONTROL |= comparemodemasked;
            #endif /* (RPWM_UseControl) */
        }
    #endif /* RPWM_CompareMode1SW */

#if (RPWM_CompareMode2SW)


    /*******************************************************************************
    * Function Name: RPWM_SetCompareMode2
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
    void RPWM_SetCompareMode2(uint8 comparemode) 
    {

        #if(0 != RPWM_CTRL_CMPMODE2_SHIFT)
            uint8 comparemodemasked = ((uint8)((uint8)comparemode << RPWM_CTRL_CMPMODE2_SHIFT)) &
                                                 RPWM_CTRL_CMPMODE2_MASK;
        #else
            uint8 comparemodemasked = comparemode & RPWM_CTRL_CMPMODE2_MASK;
        #endif /* (0 != RPWM_CTRL_CMPMODE2_SHIFT) */

        #if (RPWM_UseControl)
            RPWM_CONTROL &= ((uint8)(~RPWM_CTRL_CMPMODE2_MASK)); /*Clear existing mode */
            RPWM_CONTROL |= comparemodemasked;
        #endif /* (RPWM_UseControl) */
    }
    #endif /*RPWM_CompareMode2SW */

#endif /* UseOneCompareMode */


#if (!RPWM_UsingFixedFunction)


    /*******************************************************************************
    * Function Name: RPWM_WriteCounter
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
    void RPWM_WriteCounter(uint16 counter) \
                                       
    {
        CY_SET_REG16(RPWM_COUNTER_LSB_PTR, counter);
    }


    /*******************************************************************************
    * Function Name: RPWM_ReadCounter
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
    uint16 RPWM_ReadCounter(void) 
    {
        /* Force capture by reading Accumulator */
        /* Must first do a software capture to be able to read the counter */
        /* It is up to the user code to make sure there isn't already captured data in the FIFO */
          (void)CY_GET_REG8(RPWM_COUNTERCAP_LSB_PTR_8BIT);

        /* Read the data from the FIFO */
        return (CY_GET_REG16(RPWM_CAPTURE_LSB_PTR));
    }

    #if (RPWM_UseStatus)


        /*******************************************************************************
        * Function Name: RPWM_ClearFIFO
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
        void RPWM_ClearFIFO(void) 
        {
            while(0u != (RPWM_ReadStatusRegister() & RPWM_STATUS_FIFONEMPTY))
            {
                (void)RPWM_ReadCapture();
            }
        }

    #endif /* RPWM_UseStatus */

#endif /* !RPWM_UsingFixedFunction */


/*******************************************************************************
* Function Name: RPWM_WritePeriod
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
void RPWM_WritePeriod(uint16 period) 
{
    #if(RPWM_UsingFixedFunction)
        CY_SET_REG16(RPWM_PERIOD_LSB_PTR, (uint16)period);
    #else
        CY_SET_REG16(RPWM_PERIOD_LSB_PTR, period);
    #endif /* (RPWM_UsingFixedFunction) */
}

#if (RPWM_UseOneCompareMode)


    /*******************************************************************************
    * Function Name: RPWM_WriteCompare
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
    void RPWM_WriteCompare(uint16 compare) \
                                       
    {
        #if(RPWM_UsingFixedFunction)
            CY_SET_REG16(RPWM_COMPARE1_LSB_PTR, (uint16)compare);
        #else
            CY_SET_REG16(RPWM_COMPARE1_LSB_PTR, compare);
        #endif /* (RPWM_UsingFixedFunction) */

        #if (RPWM_PWMMode == RPWM__B_PWM__DITHER)
            #if(RPWM_UsingFixedFunction)
                CY_SET_REG16(RPWM_COMPARE2_LSB_PTR, (uint16)(compare + 1u));
            #else
                CY_SET_REG16(RPWM_COMPARE2_LSB_PTR, (compare + 1u));
            #endif /* (RPWM_UsingFixedFunction) */
        #endif /* (RPWM_PWMMode == RPWM__B_PWM__DITHER) */
    }


#else


    /*******************************************************************************
    * Function Name: RPWM_WriteCompare1
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
    void RPWM_WriteCompare1(uint16 compare) \
                                        
    {
        #if(RPWM_UsingFixedFunction)
            CY_SET_REG16(RPWM_COMPARE1_LSB_PTR, (uint16)compare);
        #else
            CY_SET_REG16(RPWM_COMPARE1_LSB_PTR, compare);
        #endif /* (RPWM_UsingFixedFunction) */
    }


    /*******************************************************************************
    * Function Name: RPWM_WriteCompare2
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
    void RPWM_WriteCompare2(uint16 compare) \
                                        
    {
        #if(RPWM_UsingFixedFunction)
            CY_SET_REG16(RPWM_COMPARE2_LSB_PTR, compare);
        #else
            CY_SET_REG16(RPWM_COMPARE2_LSB_PTR, compare);
        #endif /* (RPWM_UsingFixedFunction) */
    }
#endif /* UseOneCompareMode */

#if (RPWM_DeadBandUsed)


    /*******************************************************************************
    * Function Name: RPWM_WriteDeadTime
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
    void RPWM_WriteDeadTime(uint8 deadtime) 
    {
        /* If using the Dead Band 1-255 mode then just write the register */
        #if(!RPWM_DeadBand2_4)
            CY_SET_REG8(RPWM_DEADBAND_COUNT_PTR, deadtime);
        #else
            /* Otherwise the data has to be masked and offset */
            /* Clear existing data */
            RPWM_DEADBAND_COUNT &= ((uint8)(~RPWM_DEADBAND_COUNT_MASK));

            /* Set new dead time */
            #if(RPWM_DEADBAND_COUNT_SHIFT)
                RPWM_DEADBAND_COUNT |= ((uint8)((uint8)deadtime << RPWM_DEADBAND_COUNT_SHIFT)) &
                                                    RPWM_DEADBAND_COUNT_MASK;
            #else
                RPWM_DEADBAND_COUNT |= deadtime & RPWM_DEADBAND_COUNT_MASK;
            #endif /* (RPWM_DEADBAND_COUNT_SHIFT) */

        #endif /* (!RPWM_DeadBand2_4) */
    }


    /*******************************************************************************
    * Function Name: RPWM_ReadDeadTime
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
    uint8 RPWM_ReadDeadTime(void) 
    {
        /* If using the Dead Band 1-255 mode then just read the register */
        #if(!RPWM_DeadBand2_4)
            return (CY_GET_REG8(RPWM_DEADBAND_COUNT_PTR));
        #else

            /* Otherwise the data has to be masked and offset */
            #if(RPWM_DEADBAND_COUNT_SHIFT)
                return ((uint8)(((uint8)(RPWM_DEADBAND_COUNT & RPWM_DEADBAND_COUNT_MASK)) >>
                                                                           RPWM_DEADBAND_COUNT_SHIFT));
            #else
                return (RPWM_DEADBAND_COUNT & RPWM_DEADBAND_COUNT_MASK);
            #endif /* (RPWM_DEADBAND_COUNT_SHIFT) */
        #endif /* (!RPWM_DeadBand2_4) */
    }
#endif /* DeadBandUsed */

#if (RPWM_UseStatus || RPWM_UsingFixedFunction)


    /*******************************************************************************
    * Function Name: RPWM_SetInterruptMode
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
    void RPWM_SetInterruptMode(uint8 interruptMode) 
    {
        CY_SET_REG8(RPWM_STATUS_MASK_PTR, interruptMode);
    }


    /*******************************************************************************
    * Function Name: RPWM_ReadStatusRegister
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
    uint8 RPWM_ReadStatusRegister(void) 
    {
        return (CY_GET_REG8(RPWM_STATUS_PTR));
    }

#endif /* (RPWM_UseStatus || RPWM_UsingFixedFunction) */


#if (RPWM_UseControl)


    /*******************************************************************************
    * Function Name: RPWM_ReadControlRegister
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
    uint8 RPWM_ReadControlRegister(void) 
    {
        uint8 result;

        result = CY_GET_REG8(RPWM_CONTROL_PTR);
        return (result);
    }


    /*******************************************************************************
    * Function Name: RPWM_WriteControlRegister
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
    void RPWM_WriteControlRegister(uint8 control) 
    {
        CY_SET_REG8(RPWM_CONTROL_PTR, control);
    }

#endif /* (RPWM_UseControl) */


#if (!RPWM_UsingFixedFunction)


    /*******************************************************************************
    * Function Name: RPWM_ReadCapture
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
    uint16 RPWM_ReadCapture(void) 
    {
        return (CY_GET_REG16(RPWM_CAPTURE_LSB_PTR));
    }

#endif /* (!RPWM_UsingFixedFunction) */


#if (RPWM_UseOneCompareMode)


    /*******************************************************************************
    * Function Name: RPWM_ReadCompare
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
    uint16 RPWM_ReadCompare(void) 
    {
        #if(RPWM_UsingFixedFunction)
            return ((uint16)CY_GET_REG16(RPWM_COMPARE1_LSB_PTR));
        #else
            return (CY_GET_REG16(RPWM_COMPARE1_LSB_PTR));
        #endif /* (RPWM_UsingFixedFunction) */
    }

#else


    /*******************************************************************************
    * Function Name: RPWM_ReadCompare1
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
    uint16 RPWM_ReadCompare1(void) 
    {
        return (CY_GET_REG16(RPWM_COMPARE1_LSB_PTR));
    }


    /*******************************************************************************
    * Function Name: RPWM_ReadCompare2
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
    uint16 RPWM_ReadCompare2(void) 
    {
        return (CY_GET_REG16(RPWM_COMPARE2_LSB_PTR));
    }

#endif /* (RPWM_UseOneCompareMode) */


/*******************************************************************************
* Function Name: RPWM_ReadPeriod
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
uint16 RPWM_ReadPeriod(void) 
{
    #if(RPWM_UsingFixedFunction)
        return ((uint16)CY_GET_REG16(RPWM_PERIOD_LSB_PTR));
    #else
        return (CY_GET_REG16(RPWM_PERIOD_LSB_PTR));
    #endif /* (RPWM_UsingFixedFunction) */
}

#if ( RPWM_KillModeMinTime)


    /*******************************************************************************
    * Function Name: RPWM_WriteKillTime
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
    void RPWM_WriteKillTime(uint8 killtime) 
    {
        CY_SET_REG8(RPWM_KILLMODEMINTIME_PTR, killtime);
    }


    /*******************************************************************************
    * Function Name: RPWM_ReadKillTime
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
    uint8 RPWM_ReadKillTime(void) 
    {
        return (CY_GET_REG8(RPWM_KILLMODEMINTIME_PTR));
    }

#endif /* ( RPWM_KillModeMinTime) */

/* [] END OF FILE */
