/***************************************************************************//**
* \file CapSense_P2_Structure.c
* \version 7.0
*
* \brief
*   This file defines the data structure global variables and provides implementation
*   for the high-level and low-level APIs of the Data Structure module.
*
* \see CapSense_P2 v7.0 Datasheet
*
*//*****************************************************************************
* Copyright (2016-2019), Cypress Semiconductor Corporation.
********************************************************************************
* This software is owned by Cypress Semiconductor Corporation (Cypress) and is
* protected by and subject to worldwide patent protection (United States and
* foreign), United States copyright laws and international treaty provisions.
* Cypress hereby grants to licensee a personal, non-exclusive, non-transferable
* license to copy, use, modify, create derivative works of, and compile the
* Cypress Source Code and derivative works for the sole purpose of creating
* custom software in support of licensee product to be used only in conjunction
* with a Cypress integrated circuit as specified in the applicable agreement.
* Any reproduction, modification, translation, compilation, or representation of
* this software except as specified above is prohibited without the express
* written permission of Cypress.
*
* Disclaimer: CYPRESS MAKES NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, WITH
* REGARD TO THIS MATERIAL, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
* Cypress reserves the right to make changes without further notice to the
* materials described herein. Cypress does not assume any liability arising out
* of the application or use of any product or circuit described herein. Cypress
* does not authorize its products for use as critical components in life-support
* systems where a malfunction or failure may reasonably be expected to result in
* significant injury to the user. The inclusion of Cypress' product in a life-
* support systems application implies that the manufacturer assumes all risk of
* such use and in doing so indemnifies Cypress against all charges. Use may be
* limited by and subject to the applicable Cypress software license agreement.
*******************************************************************************/

#include <cytypes.h>
#include "CyLib.h"
#include "CapSense_P2_Structure.h"
#include "CapSense_P2_Configuration.h"

#if (0u != CapSense_P2_ADC_EN)
    #include "CapSense_P2_Adc.h"
#endif /* (0u != CapSense_P2_ADC_EN) */

#if (CapSense_P2_ENABLE == CapSense_P2_SELF_TEST_EN)
    #include "CapSense_P2_SelfTest.h"
#endif

#if (CapSense_P2_ENABLE == CapSense_P2_CENTROID_5X5_CSD_EN)
    #include "CapSense_P2_AdvancedCentroid_LL.h"
#endif

#if (CapSense_P2_ENABLE == CapSense_P2_GES_GLOBAL_EN)
    #include "CapSense_P2_Gesture.h"
#endif /* (CapSense_P2_ENABLE == CapSense_P2_GES_GLOBAL_EN) */

/*******************************************************************************
* Defines the RAM Data Structure variables and their init data in flash
*******************************************************************************/
/**
* \cond SECTION_GLOBAL_VARIABLES
* \addtogroup group_global_variables
* \{
*/

/**
* The variable that contains the CapSense_P2 configuration, settings and
* scanning results. CapSense_P2_dsRam represents RAM Data Structure.
*/
CapSense_P2_RAM_STRUCT CapSense_P2_dsRam;
/** \}
* \endcond */

/*******************************************************************************
* Declares Widget's De-bounce Counters
*******************************************************************************/
static uint8 CapSense_P2_debounceLinearSlider0[1u];


/***************************************************************************//**
* Declares Noise Envelope data structures
*******************************************************************************/
static SMARTSENSE_CSD_NOISE_ENVELOPE_STRUCT CapSense_P2_noiseEnvlpLinearSlider0[CapSense_P2_LINEARSLIDER0_NUM_SENSORS];

/*******************************************************************************
* Defines and initializes the Flash Data Structure
*******************************************************************************/



/**
* \cond SECTION_API_CONSTANTS
* \addtogroup group_api_constants
* \{
*/
/**
* Constant for the FLASH Data Structure
*/
const CapSense_P2_FLASH_STRUCT CapSense_P2_dsFlash =
{
    /* Flash Widget Initialization Data */
    {
        { /* LinearSlider0 */
            &CapSense_P2_ioList[0u],
            (void *)&CapSense_P2_dsRam.wdgtList.linearslider0,
            CapSense_P2_dsRam.snsList.linearslider0,
            (void *)0u,
            CapSense_P2_debounceLinearSlider0,
            CapSense_P2_LINEARSLIDER0_STATIC_CONFIG,
            CapSense_P2_LINEARSLIDER0_NUM_SENSORS,
            (uint8)CapSense_P2_WD_LINEAR_SLIDER_E,
            CapSense_P2_LINEARSLIDER0_NUM_SENSORS,
            CapSense_P2_LINEARSLIDER0_X_RESOLUTION,
            CapSense_P2_LINEARSLIDER0_X_CENT_MULT,
            CapSense_P2_noiseEnvlpLinearSlider0,
            CapSense_P2_LINEARSLIDER0_IIR_FILTER_COEFF,
        },
    },
};

/**
* The array of the pointers to the electrode specific register.
*/
const CapSense_P2_FLASH_IO_STRUCT CapSense_P2_ioList[CapSense_P2_TOTAL_ELECTRODES] =
{
    { /* 0: LinearSlider0_Sns0 */
        (reg32 *)CapSense_P2_Sns__0__HSIOM,
        (reg32 *)CapSense_P2_Sns__0__PC,
        (reg32 *)CapSense_P2_Sns__0__DR,
        (reg32 *)CapSense_P2_Sns__0__PS,
        CapSense_P2_Sns__0__HSIOM_MASK,
        CapSense_P2_Sns__0__MASK,
        CapSense_P2_Sns__0__HSIOM_SHIFT,
        (uint8)CapSense_P2_Sns__0__SHIFT,
        (uint8)CapSense_P2_Sns__0__SHIFT * 3u,
    },
    { /* 1: LinearSlider0_Sns1 */
        (reg32 *)CapSense_P2_Sns__1__HSIOM,
        (reg32 *)CapSense_P2_Sns__1__PC,
        (reg32 *)CapSense_P2_Sns__1__DR,
        (reg32 *)CapSense_P2_Sns__1__PS,
        CapSense_P2_Sns__1__HSIOM_MASK,
        CapSense_P2_Sns__1__MASK,
        CapSense_P2_Sns__1__HSIOM_SHIFT,
        (uint8)CapSense_P2_Sns__1__SHIFT,
        (uint8)CapSense_P2_Sns__1__SHIFT * 3u,
    },
    { /* 2: LinearSlider0_Sns2 */
        (reg32 *)CapSense_P2_Sns__2__HSIOM,
        (reg32 *)CapSense_P2_Sns__2__PC,
        (reg32 *)CapSense_P2_Sns__2__DR,
        (reg32 *)CapSense_P2_Sns__2__PS,
        CapSense_P2_Sns__2__HSIOM_MASK,
        CapSense_P2_Sns__2__MASK,
        CapSense_P2_Sns__2__HSIOM_SHIFT,
        (uint8)CapSense_P2_Sns__2__SHIFT,
        (uint8)CapSense_P2_Sns__2__SHIFT * 3u,
    },
    { /* 3: LinearSlider0_Sns3 */
        (reg32 *)CapSense_P2_Sns__3__HSIOM,
        (reg32 *)CapSense_P2_Sns__3__PC,
        (reg32 *)CapSense_P2_Sns__3__DR,
        (reg32 *)CapSense_P2_Sns__3__PS,
        CapSense_P2_Sns__3__HSIOM_MASK,
        CapSense_P2_Sns__3__MASK,
        CapSense_P2_Sns__3__HSIOM_SHIFT,
        (uint8)CapSense_P2_Sns__3__SHIFT,
        (uint8)CapSense_P2_Sns__3__SHIFT * 3u,
    },
    { /* 4: LinearSlider0_Sns4 */
        (reg32 *)CapSense_P2_Sns__4__HSIOM,
        (reg32 *)CapSense_P2_Sns__4__PC,
        (reg32 *)CapSense_P2_Sns__4__DR,
        (reg32 *)CapSense_P2_Sns__4__PS,
        CapSense_P2_Sns__4__HSIOM_MASK,
        CapSense_P2_Sns__4__MASK,
        CapSense_P2_Sns__4__HSIOM_SHIFT,
        (uint8)CapSense_P2_Sns__4__SHIFT,
        (uint8)CapSense_P2_Sns__4__SHIFT * 3u,
    },
};



/** \}
* \endcond */

/* Initialization data for RAM widget list */
const CapSense_P2_RAM_WD_LIST_STRUCT CapSense_P2_ramWidgetInit =
{
    { /* LinearSlider0 */
        CapSense_P2_LINEARSLIDER0_RESOLUTION,
        CapSense_P2_LINEARSLIDER0_FINGER_TH,
        CapSense_P2_LINEARSLIDER0_NOISE_TH,
        CapSense_P2_LINEARSLIDER0_NNOISE_TH,
        CapSense_P2_LINEARSLIDER0_HYSTERESIS,
        CapSense_P2_LINEARSLIDER0_ON_DEBOUNCE,
        CapSense_P2_LINEARSLIDER0_LOW_BSLN_RST,
        {
            CapSense_P2_LINEARSLIDER0_IDAC_MOD0,
        },
        CapSense_P2_LINEARSLIDER0_IDAC_GAIN_INDEX,
        CapSense_P2_LINEARSLIDER0_SNS_CLK,
        CapSense_P2_LINEARSLIDER0_SNS_CLK_SOURCE,
        CapSense_P2_LINEARSLIDER0_FINGER_CAP,
        CapSense_P2_LINEARSLIDER0_SIGPFC,
        {
            CapSense_P2_LINEARSLIDER0_POSITION,
        },
    },
};


/* IDAC Initialization Data */
const uint8 CapSense_P2_ramIdacInit[CapSense_P2_TOTAL_SENSORS] =
{
    /* LinearSlider0 */
    CapSense_P2_LINEARSLIDER0_SNS0_IDAC_COMP0,
    CapSense_P2_LINEARSLIDER0_SNS1_IDAC_COMP0,
    CapSense_P2_LINEARSLIDER0_SNS2_IDAC_COMP0,
    CapSense_P2_LINEARSLIDER0_SNS3_IDAC_COMP0,
    CapSense_P2_LINEARSLIDER0_SNS4_IDAC_COMP0,
};





/*******************************************************************************
* Defines internal data types and statements
*******************************************************************************/
#define PARAM_TYPE_OFFSET       (30u)
#define PARAM_TYPE_MASK         (3Lu << PARAM_TYPE_OFFSET)
#define PARAM_RWBIT_OFFSET      (29u)
#define PARAM_RWBIT_MASK        (1Lu << PARAM_RWBIT_OFFSET)
#define PARAM_FBIT_OFFSET       (28u)
#define PARAM_FBIT_MASK         (1Lu << PARAM_FBIT_OFFSET)
#define PARAM_UBIT_OFFSET       (23u)
#define PARAM_UBIT_MASK         (1Lu << PARAM_UBIT_OFFSET)
#define PARAM_WDID_OFFSET       (16u)
#define PARAM_WDID_MASK         (0x7FLu << PARAM_WDID_OFFSET)
#define PARAM_CRC_OFFSET        (24u)
#define PARAM_CRC_MASK          (0x0FLu << PARAM_CRC_OFFSET)

/* Defines align masks for uint16 and uint32 */
#define UINT16_ALIGN_MASK       (1Lu)
#define UINT32_ALIGN_MASK       (3Lu)

/* Defines supported parameter types */
#define DS_PARAM_TYPE_UINT8     (0x01u)
#define DS_PARAM_TYPE_UINT16    (0x02u)
#define DS_PARAM_TYPE_UINT32    (0x03u)


/* Defines PARAM_ID structure */
typedef struct
{
    uint16 offset;      /* parameter offset */
    uint8  widgetId;    /* widget Id parameter belongs to */
    uint8  affectsCrc;  /* parameter affects widget CRC */
    uint8  crc;         /* parameter CRC */
    uint8  flash;       /* parameter located in FLASH flag */
    uint8  rw;          /* parameter is read/write flag */
    uint8  type;        /* parameter type: uint8/uint16/uint32 */
} PARAM_ID_STRUCT;


/*******************************************************************************
* Static Function Prototypes
*******************************************************************************/
/**
* \cond SECTION_C_INTERNAL
* \addtogroup group_c_internal
* \{
*/



static cystatus DsParseParamId(uint32 paramId, PARAM_ID_STRUCT *pData);

/** \}
* \endcond */


/*******************************************************************************
* Function Name: CapSense_P2_DsInitialize
****************************************************************************//**
*
* \brief
*   This function initializes the Data Structure storage.
*
* \details
*   This function copies the default widget configuration parameters
*   from the Flash storage into the RAM Data Structure.
*
*******************************************************************************/
void CapSense_P2_DsInitialize(void)
{
    #if ((CapSense_P2_ENABLE == CapSense_P2_CSX_EN) || \
         (CapSense_P2_ENABLE == CapSense_P2_CSD_IDAC_COMP_EN) || \
        ((CapSense_P2_ENABLE == CapSense_P2_CSX_EN) && \
         (CapSense_P2_ENABLE == CapSense_P2_CSX_SKIP_OVSMPL_SPECIFIC_NODES)))
        uint32 snsId;
        uint32 wdgtId;
        CapSense_P2_RAM_SNS_STRUCT * ptrSnsTmp;
        CapSense_P2_FLASH_WD_STRUCT const * ptrFlashWdgt;
    #endif

    #if ((CapSense_P2_ENABLE == CapSense_P2_CSX_EN) && \
         (CapSense_P2_ENABLE == CapSense_P2_CSX_SKIP_OVSMPL_SPECIFIC_NODES))
        uint16 subConvNumber;
    #endif

    #if ((0u != CapSense_P2_CSX_EN) || (0 != CapSense_P2_CSD_IDAC_COMP_EN))
        uint8 const * ptrIdacInit = CapSense_P2_ramIdacInit;
    #endif

    /* Reset RAM data structure content */
    (void)memset(&CapSense_P2_dsRam, 0, sizeof(CapSense_P2_dsRam));

    /* Initialize configId, deviceId and hwClock registers */
    CapSense_P2_dsRam.configId = CapSense_P2_CONFIG_ID;
    CapSense_P2_dsRam.deviceId = CapSense_P2_DEVICE_ID;
    CapSense_P2_dsRam.hwClock = CapSense_P2_HW_CLOCK;

    /* Initialize global RAM data */
    CapSense_P2_dsRam.csd0Config = CapSense_P2_CSD0_CONFIG;

    CapSense_P2_dsRam.scanCurrentISC = CapSense_P2_SNS_CONNECTION_UNDEFINED;
    #if (0u != CapSense_P2_TOTAL_CSD_WIDGETS)
        CapSense_P2_dsRam.modCsdClk = CapSense_P2_CSD_SCANSPEED_DIVIDER;
        CapSense_P2_dsRam.scanCsdISC = CapSense_P2_CSD_INACTIVE_SNS_CONNECTION;

        #if (0u != CapSense_P2_CSD_COMMON_SNS_CLK_EN)
            CapSense_P2_dsRam.snsCsdClk = CapSense_P2_CSD_SNS_CLK_DIVIDER;
        #endif /* #if (0u != CapSense_P2_CSD_COMMON_SNS_CLK_EN) */
    #endif /* #if (0u != CapSense_P2_TOTAL_CSD_WIDGETS) */

    #if (CapSense_P2_ENABLE == CapSense_P2_CSX_EN)
        CapSense_P2_dsRam.modCsxClk = CapSense_P2_CSX_SCANSPEED_DIVIDER;
        CapSense_P2_dsRam.scanCsxISC = CapSense_P2_CSX_INACTIVE_SNS_CONNECTION;

        #if (0u != CapSense_P2_CSX_COMMON_TX_CLK_EN)
            CapSense_P2_dsRam.snsCsxClk = CapSense_P2_CSX_TX_CLK_DIVIDER;
        #endif /* #if (0u != CapSense_P2_CSX_COMMON_TX_CLK_EN) */
    #endif /* #if (CapSense_P2_ENABLE == CapSense_P2_CSX_EN) */

    #if (CapSense_P2_ENABLE == CapSense_P2_ISX_EN)
        CapSense_P2_dsRam.modIsxClk = CapSense_P2_ISX_SCANSPEED_DIVIDER;
    #endif /* #if (0u != CapSense_P2_ISX_EN) */

    #if (0u != CapSense_P2_SELF_TEST_EN)

        CapSense_P2_dsRam.glbCrc = CapSense_P2_GLB_CRC;
        CapSense_P2_dsRam.selfTestConfig.fineInitTime = (uint8)CapSense_P2_TST_FINE_INIT_TIME;
        CapSense_P2_dsRam.selfTestConfig.startupDelay = CapSense_P2_TST_ANALOG_STARTUP_DELAY_US;

        #if (0u != CapSense_P2_TST_SNS_SHORT_EN)
            CapSense_P2_dsRam.selfTestConfig.snsShortTimeUs = CapSense_P2_TST_SNS_SHORT_TIME;
        #endif

        #if ((0u != CapSense_P2_TST_SNS_CAP_EN) ||\
             (0u != CapSense_P2_TST_SH_CAP_EN))
            CapSense_P2_dsRam.selfTestConfig.snsCapSnsClkDivider = CapSense_P2_TST_SNS_CAP_SNS_CLK_DIVIDER;
            CapSense_P2_dsRam.selfTestConfig.snsCapSnsClkHz = CapSense_P2_TST_SNS_CAP_SNS_CLK_HZ;
            CapSense_P2_dsRam.selfTestConfig.snsCapResolution = CapSense_P2_TST_SNS_CAP_RESOLUTION;
            CapSense_P2_dsRam.selfTestConfig.snsCapVrefGain = CapSense_P2_TST_SNS_CAP_VREF_GAIN ;
            CapSense_P2_dsRam.selfTestConfig.snsCapVrefVoltage = CapSense_P2_TST_SNS_CAP_VREF_MV;
            CapSense_P2_dsRam.selfTestConfig.snsCapModClkDivider = CapSense_P2_TST_MOD_CLK_DIVIDER ;
            CapSense_P2_dsRam.selfTestConfig.snsCapModClkKHz = CapSense_P2_TST_MOD_CLK_KHZ ;
        #endif /* ((0u != CapSense_P2_TST_SNS_CAP_EN) ||
                   (0u != CapSense_P2_TST_SH_CAP_EN)) */

        #if (0u != CapSense_P2_TST_SNS_CAP_EN)
            #if (0u != CapSense_P2_CSD_EN)
                CapSense_P2_dsRam.selfTestConfig.bistCsdSnsCapISC = CapSense_P2_SNS_CAP_CSD_INACTIVE_CONNECTION;
            #endif /* (0u != CapSense_P2_CSD_EN) */
            #if (0u != CapSense_P2_CSX_EN)
                CapSense_P2_dsRam.selfTestConfig.bistCsxSnsCapISC = CapSense_P2_SNS_CAP_CSX_INACTIVE_CONNECTION;
            #endif /* (0u != CapSense_P2_CSX_EN) */
        #endif /* (0u != CapSense_P2_TST_SNS_CAP_EN) */

        #if (0u != CapSense_P2_TST_SH_CAP_EN)
            #if ((0u != CapSense_P2_CSD_EN) && (0u != CapSense_P2_CSD_SHIELD_EN))
                CapSense_P2_dsRam.selfTestConfig.bistCsdShCapISC = CapSense_P2_SHIELD_CAP_INACTIVE_CONNECTION;
            #endif /* ((0u != CapSense_P2_CSD_EN) && (0u != CapSense_P2_CSD_SHIELD_EN)) */
        #endif /* (0u != CapSense_P2_TST_SH_CAP_EN) */

        #if (0u != CapSense_P2_TST_EXTERNAL_CAP_EN)
            CapSense_P2_dsRam.selfTestConfig.extCapVrefGain = CapSense_P2_TST_EXT_CAP_VREF_GAIN;
            CapSense_P2_dsRam.selfTestConfig.extCapVrefVoltage = CapSense_P2_TST_EXT_CAP_VREF_MV;
        #endif /* #if (0u != CapSense_P2_TST_EXTERNAL_CAP_EN) */

        #if (0u != CapSense_P2_TST_VDDA_EN)
            CapSense_P2_dsRam.selfTestConfig.vddaResolution = CapSense_P2_TST_VDDA_RESOLUTION;
            CapSense_P2_dsRam.selfTestConfig.vddaModClkDivider = CapSense_P2_TST_VDDA_MOD_CLK_DIVIDER;

            CapSense_P2_dsRam.selfTestConfig.vddaVrefGain = CapSense_P2_TST_VDDA_VREF_GAIN;
            CapSense_P2_dsRam.selfTestConfig.vddaVrefVoltage = CapSense_P2_TST_VDDA_VREF_MV;
        #endif /* #if (0u != CapSense_P2_TST_VDDA_EN) */

    #endif /* #if (0u != CapSense_P2_SELF_TEST_EN) */

    /* Initialize RAM widget data */
    CapSense_P2_dsRam.wdgtList = CapSense_P2_ramWidgetInit;

    /* Initialize Gesture configuration structure */
    #if ((0u != CapSense_P2_GES_GLOBAL_EN) || (0u != CapSense_P2_BALLISTIC_MULTIPLIER_EN))
        CapSense_P2_dsRam.timestampInterval = CapSense_P2_TIMESTAMP_INTERVAL;
    #endif /* ((0u != CapSense_P2_GES_GLOBAL_EN) || (0u != CapSense_P2_BALLISTIC_MULTIPLIER_EN)) */

    #if (0u != CapSense_P2_GES_GLOBAL_EN)
        CapSense_P2_dsRam.gestures = CapSense_P2_ramGesturesInit;
    #endif /* (0u != CapSense_P2_GES_GLOBAL_EN) */

    #if (0u != CapSense_P2_BALLISTIC_MULTIPLIER_EN)
        CapSense_P2_dsRam.ballisticConfig = CapSense_P2_ramBallisticInit;
    #endif /* (0u != CapSense_P2_BALLISTIC_MULTIPLIER_EN) */

    #if (0u != CapSense_P2_CSX_EN) || (0 != CapSense_P2_CSD_IDAC_COMP_EN)
        /* Initialize IDAC data */
        ptrFlashWdgt = CapSense_P2_dsFlash.wdgtArray;

        for (wdgtId = CapSense_P2_TOTAL_WIDGETS; wdgtId-- > 0u; )
        {
            ptrSnsTmp = ptrFlashWdgt->ptr2SnsRam;

            for (snsId = CapSense_P2_GET_SNS_CNT_BY_PTR(ptrFlashWdgt); snsId-- > 0u;)
            {
                ptrSnsTmp->idacComp[0u] = *ptrIdacInit;
                ptrIdacInit++;

                #if (0u != CapSense_P2_MULTI_FREQ_SCAN_EN)
                    ptrSnsTmp->idacComp[1u] = *ptrIdacInit;
                    ptrIdacInit++;

                    ptrSnsTmp->idacComp[2u] = *ptrIdacInit;
                    ptrIdacInit++;
                #endif /* #if (0u != CapSense_P2_MULTI_FREQ_SCAN_EN) */

                ptrSnsTmp++;
            }

            ptrFlashWdgt++; /* Move to next widget */
        }
    #endif /*(0u != CapSense_P2_CSX_EN) || (0 != CapSense_P2_CSD_IDAC_COMP_EN)*/

    #if ((CapSense_P2_ENABLE == CapSense_P2_CSX_EN) && \
         (CapSense_P2_ENABLE == CapSense_P2_CSX_SKIP_OVSMPL_SPECIFIC_NODES))
        /* Initialize Sub-Conversion Number for each node of each CSX widget */
        ptrFlashWdgt = CapSense_P2_dsFlash.wdgtArray;
        for (wdgtId = CapSense_P2_TOTAL_WIDGETS; wdgtId-- > 0u;)
        {
            /* Check for widget sense method */
            if (CapSense_P2_SENSE_METHOD_CSX_E == CapSense_P2_GET_SENSE_METHOD(ptrFlashWdgt))
            {
                /* Take the sub-convesion number of a CSX widget and initialize each node of this widget */
                subConvNumber = ((CapSense_P2_RAM_WD_BASE_STRUCT *)(ptrFlashWdgt->ptr2WdgtRam))->resolution;
                ptrSnsTmp = ptrFlashWdgt->ptr2SnsRam;
                for (snsId = CapSense_P2_GET_SNS_CNT_BY_PTR(ptrFlashWdgt); snsId-- > 0u;)
                {
                    ptrSnsTmp->subConvNum = subConvNumber;
                    ptrSnsTmp++;
                }
            }
            /* Move to the next widget */
            ptrFlashWdgt++;
        }
    #endif /* ((CapSense_P2_ENABLE == CapSense_P2_CSX_EN) && \
               (CapSense_P2_ENABLE == CapSense_P2_CSX_SKIP_OVSMPL_SPECIFIC_NODES)) */

    #if (0u != CapSense_P2_ADC_EN)
        CapSense_P2_AdcDsInitialize();
    #endif /* (0u != CapSense_P2_ADC_EN) */
}


#if (0u != CapSense_P2_ADC_EN)
    /*******************************************************************************
    * Function Name: CapSense_P2_AdcDsInitialize
    ****************************************************************************//**
    *
    * \brief
    *   Configures the initial Adc datastructure members.
    *
    *******************************************************************************/
    void CapSense_P2_AdcDsInitialize(void)
    {
        CapSense_P2_dsRam.adcResolution = CapSense_P2_ADC_RESOLUTION;
        CapSense_P2_dsRam.adcIdac = (uint8)(CapSense_P2_ADC_IDAC_DEFAULT);
        CapSense_P2_dsRam.adcActiveCh = CapSense_P2_AdcNO_CHANNEL;
    }
#endif /* (0u != CapSense_P2_ADC_EN) */


/*******************************************************************************
* Function Name: DsParseParamId
****************************************************************************//**
*
* \brief
*   This function parses the parameter ID and checks its correctness.
*
* \details
*   This function checks the parameter CRC if the self-test is enabled,
*   validates the offset value and parameter type.
*
* \param paramId The parameter to parse.
* \param pData   The pointer to the structure that will hold parsed data.
*
* \return CYRET_SUCCESS     If the parameter is valid
*         CYRET_BAD_PARAM   If the parameter is invalid
*
*******************************************************************************/
static cystatus DsParseParamId(uint32 paramId, PARAM_ID_STRUCT *pData)
{
    cystatus result = CYRET_SUCCESS;

    #if (0u != CapSense_P2_SELF_TEST_EN)
        const uint8 crcTable[] =
        {
            0x00u, 0x09u, 0x0Bu, 0x02u, 0x0Fu, 0x06u, 0x04u, 0x0Du,
            0x07u, 0x0Eu, 0x0Cu, 0x05u, 0x08u, 0x01u, 0x03u, 0x0Au
        };

        uint32 i;
        uint32 actualCrc = 0u;
        uint32 crcIndex;
    #endif /* #if (0u != CapSense_P2_SELF_TEST_EN) */

    /* Extract parameter data */
    pData->offset     = LO16(paramId);
    pData->affectsCrc = LO8((paramId & PARAM_UBIT_MASK)  >> PARAM_UBIT_OFFSET);
    pData->widgetId   = LO8((paramId & PARAM_WDID_MASK)  >> PARAM_WDID_OFFSET);
    pData->type       = LO8((paramId & PARAM_TYPE_MASK)  >> PARAM_TYPE_OFFSET);
    pData->flash      = LO8((paramId & PARAM_FBIT_MASK)  >> PARAM_FBIT_OFFSET);
    pData->rw         = LO8((paramId & PARAM_RWBIT_MASK) >> PARAM_RWBIT_OFFSET);
    pData->crc        = LO8((paramId & PARAM_CRC_MASK)   >> PARAM_CRC_OFFSET);

    /* Check parameter CRC if self-test is enabled */
    #if (0u != CapSense_P2_SELF_TEST_EN)

        /* Calculate CRC for bits 0..24.
         * The CRC is calculated using nibbles (4-bits).
         * So for 0..24 bits there are in total 6 nibbles. */
        for (i = 6u; i > 0u; i--)
        {
            crcIndex = actualCrc ^ (paramId & 0x0Fu);
            actualCrc = crcTable[crcIndex];
            paramId >>= 4u;
        }

        /* Add bits 28..32 */
        crcIndex = actualCrc ^ ((paramId >> 4u) & 0x0Fu);
        actualCrc = crcTable[crcIndex];

        /* Add bits 24..28 (CRC) */
        crcIndex = actualCrc ^ (paramId & 0x0Fu);
        actualCrc = crcTable[crcIndex];

        if (0u != actualCrc)
        {
            /* CRC mismatch */
            result = CYRET_BAD_PARAM;
        }

        if ((CYRET_SUCCESS == result) &&
            (0u != pData->affectsCrc) && (pData->widgetId >= CapSense_P2_TOTAL_WIDGETS ))
        {
            /* Wrong widgetId for protected parameter */
            result = CYRET_BAD_PARAM;
        }

        /* Check offset value range */
        if (CYRET_SUCCESS == result)
        {
            if (0u == pData->flash)
            {
                /* Check offset for RAM Data Structure range */
                if (pData->offset >= sizeof(CapSense_P2_dsRam))
                {
                    result = CYRET_BAD_PARAM;
                }
            }
            else
            {
                /* Check offset for Flash Data Structure range */
                if (pData->offset >= sizeof(CapSense_P2_dsFlash))
                {
                    result = CYRET_BAD_PARAM;
                }

                /* Check if it is marked as Read Only */
                if (0u != pData->rw)
                {
                    result = CYRET_BAD_PARAM;
                }
            }
        }
    #else /* #if (0u != CapSense_P2_SELF_TEST_EN) */
        /* Check offset value range */
        if (0u == pData->flash)
        {
            /* Check offset for RAM Data Structure range */
            if (pData->offset >= sizeof(CapSense_P2_dsRam))
            {
                result = CYRET_BAD_PARAM;
            }
        }
        else
        {
            /* Check offset for Flash Data Structure range */
            if (pData->offset >= sizeof(CapSense_P2_dsFlash))
            {
                result = CYRET_BAD_PARAM;
            }

            /* Check if it is marked as Read Only */
            if (0u != pData->rw)
            {
                result = CYRET_BAD_PARAM;
            }
        }
    #endif /* #if (0u != CapSense_P2_SELF_TEST_EN) */

    if (CYRET_SUCCESS == result)
    {
        /* Check parameter type, offset alignment */
        switch (pData->type)
        {
        case DS_PARAM_TYPE_UINT16:
            if (0u != (pData->offset & UINT16_ALIGN_MASK))
            {
                result = CYRET_BAD_PARAM;
            }
            break;

        case DS_PARAM_TYPE_UINT32:
            if (0u != (pData->offset & UINT32_ALIGN_MASK))
            {
                result = CYRET_BAD_PARAM;
            }
            break;

        case DS_PARAM_TYPE_UINT8:
            break;

        default:
            result = CYRET_BAD_PARAM;
            break;
        }
    }

    return result;
}

/*******************************************************************************
* Function Name: CapSense_P2_GetParam
****************************************************************************//**
*
* \brief
*  Gets the specified parameter value from the \ref group_structures.
*
* \details
*  This function gets the value of the specified parameter by the paramId
*  argument. The paramId for each register is available in the
*  CapSense_P2 RegisterMap header file as
*  CapSense_P2_<ParameterName>_PARAM_ID. The paramId is a special
*  enumerated value generated by the customizer. The format of paramId is as
*  follows:
*    1. [ byte 3 byte 2 byte 1 byte 0 ]
*    2. [ TTWFCCCC UIIIIIII MMMMMMMM LLLLLLLL ]
*    3. T - encodes the parameter type:
*      - 01b: uint8
*      - 10b: uint16
*      - 11b: uint32
*    4. W - indicates whether the parameter is writable:
*      - 0: ReadOnly
*      - 1: Read/Write
*    5. C - 4 bit CRC (X^3 + 1) of the whole paramId word, the C bits are
*       filled with 0s when the CRC is calculated.
*    6. U - indicates if the parameter affects the RAM Widget Object CRC.
*    7. I - specifies that the widgetId parameter belongs to
*    8. M,L - the parameter offset MSB and LSB accordingly in:
*      - Flash Data Structure if W bit is 0.
*      - RAM Data Structure if W bit is 1.
*
*  Refer to the \ref group_structures section for details of the data structure
*  organization and examples of its register access.
*
* \param paramId
*  Specifies the ID of parameter to get its value.
*  A macro for the parameter ID can be found in the CapSense_P2 RegisterMap header
*  file defined as CapSense_P2_<ParameterName>_PARAM_ID.
*
* \param value
*  The pointer to a variable to be updated with the obtained value.
*
* \return
*  Returns the status of the operation:
*    - CYRET_SUCCESS - The operation is successfully completed.
*    - CYRET_BAD_PARAM - The input parameter is invalid.
*
*******************************************************************************/
cystatus CapSense_P2_GetParam(uint32 paramId, uint32 *value)
{
    PARAM_ID_STRUCT pData;
    cystatus result;

    union
    {
        volatile void   const * ptr;
        volatile uint8  const * ptrUint8;
        volatile uint16 const * ptrUint16;
        volatile uint32 const * ptrUint32;
    } ptrData;

    result = DsParseParamId(paramId, &pData);

    /* Parse and validate paramId */
    if (CYRET_SUCCESS == result)
    {
        /* Get base address of parameter */
        if (0u == pData.flash)
        {
            ptrData.ptr = &CapSense_P2_dsRam;
        }
        else
        {
            ptrData.ptr = &CapSense_P2_dsFlash;
        }

        /* Add offset to base address */
        ptrData.ptrUint8 += pData.offset;

        /* Read data */
        if (DS_PARAM_TYPE_UINT8 == pData.type)
        {
            *value = (uint32) (*ptrData.ptrUint8);
        }
        else if (DS_PARAM_TYPE_UINT16 == pData.type)
        {
            *value = (uint32) (*ptrData.ptrUint16);
        }
        else if (DS_PARAM_TYPE_UINT32 == pData.type)
        {
            *value = *ptrData.ptrUint32;
        }
        else
        {
            /* Parameter comes here already validated. */
            CYASSERT(0u);
        }
    }

    return result;
}


/*******************************************************************************
* Function Name: CapSense_P2_SetParam
****************************************************************************//**
*
* \brief
*  Sets a new value for the specified parameter in the \ref group_structures.
*
* \details
*  This function sets the value of the specified parameter by the paramId
*  argument. The paramId for each register is available in the
*  CapSense_P2 RegisterMap header file as
*  CapSense_P2_<ParameterName>_PARAM_ID. The paramId is a special
*  enumerated value generated by the customizer. The format of paramId is as
*  follows:
*    1. [ byte 3 byte 2 byte 1 byte 0 ]
*    2. [ TTWFCCCC UIIIIIII MMMMMMMM LLLLLLLL ]
*    3. T - encodes the parameter type:
*      - 01b: uint8
*      - 10b: uint16
*      - 11b: uint32
*    4. W - indicates whether the parameter is writable:
*      - 0: ReadOnly
*      - 1: Read/Write
*    5. C - 4 bit CRC (X^3 + 1) of the whole paramId word, the C bits are
*       filled with 0s when the CRC is calculated.
*    6. U - indicates if the parameter affects the RAM Widget Object CRC.
*    7. I - specifies that the widgetId parameter belongs to
*    8. M,L - the parameter offset MSB and LSB accordingly in:
*      - Flash Data Structure if W bit is 0.
*      - RAM Data Structure if W bit is 1.
*
*  Refer to the \ref group_structures section for details of the data structure
*  organization and examples of its register access.
*
*  This function writes specified value into the desired register without
*  other registers update. It is application layer responsibility to keep all
*  the data structure registers aligned. Repeated call of
*  CapSense_P2_Start() function helps aligning dependent register values.
*
* \param paramId
*  Specifies the ID of parameter to set its value.
*  A macro for the parameter ID can be found in the CapSense_P2 RegisterMap header
*  file defined as CapSense_P2_<ParameterName>_PARAM_ID.

* \param value
*  Specifies the new parameter's value.
*
* \return
*  Returns the status of the operation:
*    - CYRET_SUCCESS - The operation is successfully completed.
*    - CYRET_BAD_PARAM - The input parameter is invalid.
*
*******************************************************************************/
cystatus CapSense_P2_SetParam(uint32 paramId, uint32 value)
{
    cystatus result;
    PARAM_ID_STRUCT pData;

    union
    {
        volatile void   * ptr;
        volatile uint8  * ptrUint8;
        volatile uint16 * ptrUint16;
        volatile uint32 * ptrUint32;
    } ptrData;

    result = DsParseParamId(paramId, &pData);

    /* verify if parameter is writable */
    if (CYRET_SUCCESS == result)
    {
        if (0u == pData.rw)
        {
            result = CYRET_BAD_PARAM;
        }
    }

    /* Parse and validate paramId */
    if (CYRET_SUCCESS == result)
    {
        /* Get base address of parameter */
        ptrData.ptr = &CapSense_P2_dsRam;

        /* Add offset to base address */
        ptrData.ptrUint8 += pData.offset;

        /* Write data */
        if (DS_PARAM_TYPE_UINT8 == pData.type)
        {
            *ptrData.ptrUint8 = LO8(value);
        }
        else if (DS_PARAM_TYPE_UINT16 == pData.type)
        {
            *ptrData.ptrUint16 = LO16(value);
        }
        else if (DS_PARAM_TYPE_UINT32 == pData.type)
        {
            *ptrData.ptrUint32 = value;
        }
        else
        {
            /* Parameter comes here already validated. */
            CYASSERT(0u);
        }

        #if (CapSense_P2_ENABLE == (CapSense_P2_TST_WDGT_CRC_EN || CapSense_P2_TST_GLOBAL_CRC_EN))
            /* Update widget or global CRC if self-test is enabled and parameter affects it */
            if (0u != pData.affectsCrc)
            {
                if ((ptrData.ptrUint16 >= &CapSense_P2_dsRam.csd0Config) &&
                    (ptrData.ptrUint16 <  &CapSense_P2_dsRam.glbCrc))
                {

                    #if (CapSense_P2_ENABLE == CapSense_P2_TST_GLOBAL_CRC_EN)
                        CapSense_P2_DsUpdateGlobalCrc();
                    #endif /* (CapSense_P2_ENABLE == CapSense_P2_TST_GLOBAL_CRC_EN) */

                }
                else
                {
                    #if (CapSense_P2_ENABLE == CapSense_P2_TST_WDGT_CRC_EN)
                        CapSense_P2_DsUpdateWidgetCrc((uint32)pData.widgetId);
                    #endif /* (CapSense_P2_ENABLE == CapSense_P2_TST_WDGT_CRC_EN) */
                }
            }
        #endif /* (CapSense_P2_ENABLE == (CapSense_P2_TST_WDGT_CRC_EN || CapSense_P2_TST_GLOBAL_CRC_EN)) */
    }

    return result;
}

/*******************************************************************************
* Function Name: CapSense_P2_IsAnyWidgetActive
****************************************************************************//**
*
* \brief
*  Reports if any widget has detected a touch.
*
* \details
*  This function reports if any widget has detected a touch or not by extracting
*  information from the wdgtStatus registers
*  (CapSense_P2_WDGT_STATUS<X>_VALUE). This function does not process a
*  widget but extracts processed results from the \ref group_structures.
*
* \return
*   Returns the touch detection status of all the widgets:
*     - Zero - No touch is detected in all the widgets or sensors.
*     - Non-zero - At least one widget or sensor detected a touch.
*
*******************************************************************************/
uint32 CapSense_P2_IsAnyWidgetActive(void)
{
    uint32 result = 0Lu;
    uint32 wdWord;

    for (wdWord = CapSense_P2_WDGT_STATUS_WORDS; wdWord-- > 0u;)
    {
        result |= CapSense_P2_dsRam.wdgtStatus[wdWord];
    }

    return result;
}


/*******************************************************************************
* Function Name: CapSense_P2_IsWidgetActive
****************************************************************************//**
*
* \brief
*  Reports if the specified widget detects a touch on any of its sensors.
*
* \details
*  This function reports if the specified widget has detected a touch or not by
*  extracting information from the wdgtStatus registers
*  (CapSense_P2_WDGT_STATUS<X>_VALUE). This function does not process the
*  widget but extracts processed results from the \ref group_structures.
*
* \param widgetId
*  Specifies the ID number of the widget to get its status.
*  A macro for the widget ID can be found in the CapSense_P2 Configuration header
*  file defined as CapSense_P2_<WidgetName>_WDGT_ID.
*
* \return
*  Returns the touch detection status of the specified widgets:
*    - Zero - No touch is detected in the specified widget or a wrong widgetId
*      is specified.
*    - Non-zero if at least one sensor of the specified widget is active, i.e.
*      a touch is detected.
*
*******************************************************************************/
uint32 CapSense_P2_IsWidgetActive(uint32 widgetId)
{
    uint32 result = 0Lu;

    if (widgetId < CapSense_P2_TOTAL_WIDGETS)
    {
        result = CapSense_P2_GET_WIDGET_ACTIVE_STATUS(widgetId);
    }
    return result;
}


/*******************************************************************************
* Function Name: CapSense_P2_IsSensorActive
****************************************************************************//**
*
* \brief
*  Reports if the specified sensor in the widget detects a touch.
*
* \details
*  This function reports if the specified sensor in the widget has detected a
*  touch or not by extracting information from the wdgtStatus registers
*  (CapSense_P2_WDGT_STATUS<X>_VALUE). This function does not process the
*  widget or sensor but extracts processed results from the \ref group_structures.
*
*  For proximity sensors, this function returns the proximity detection status. To
*  get the finger touch status of proximity sensors, use the
*  CapSense_P2_IsProximitySensorActive() function.
*
* \param widgetId
*  Specifies the ID number of the widget.
*  A macro for the widget ID can be found in the CapSense_P2 Configuration header
*  file defined as CapSense_P2_<WidgetName>_WDGT_ID.

* \param sensorId
*  Specifies the ID number of the sensor within the widget to get its touch
*  detection status.
*  A macro for the sensor ID within the specified widget can be found in the
*  CapSense_P2 Configuration header file defined as
*  CapSense_P2_<WidgetName>_SNS<SensorNumber>_ID.
*
* \return
*  Returns the touch detection status of the specified sensor / widget:
*    - Zero if no touch is detected in the specified sensor / widget or a wrong
*      widget ID / sensor ID is specified.
*    - Non-zero if the specified sensor is active i.e. touch is detected. If the
*      specific sensor belongs to a proximity widget, the proximity detection
*      status is returned.
*
*******************************************************************************/
uint32 CapSense_P2_IsSensorActive(uint32 widgetId, uint32 sensorId)
{
    uint32 result = 0Lu;

    if ((widgetId < CapSense_P2_TOTAL_WIDGETS) &&
        (sensorId < CapSense_P2_GET_SENSOR_COUNT(widgetId)))
    {
        #if (CapSense_P2_ENABLE == CapSense_P2_PROXIMITY_WIDGET_EN)
            if (CapSense_P2_WD_PROXIMITY_E ==
                (CapSense_P2_WD_TYPE_ENUM)CapSense_P2_dsFlash.wdgtArray[widgetId].wdgtType)
            {
                sensorId = CapSense_P2_PROX_STS_OFFSET(sensorId);
            }
        #endif /* (CapSense_P2_ENABLE != CapSense_P2_PROXIMITY_WIDGET_EN) */
        result = CapSense_P2_dsRam.snsStatus[widgetId] & (1Lu << sensorId);
    }

    return result;
}

#if (0u != CapSense_P2_MATRIX_WIDGET_EN)
/*******************************************************************************
* Function Name: CapSense_P2_IsMatrixButtonsActive
****************************************************************************//**
*
* \brief
*  Reports the status of the specified matrix button widget.
*
* \details
*  This function reports if the specified matrix widget has detected a touch or
*  not by extracting information from the wdgtStatus registers
*  (CapSense_P2_WDGT_STATUS<X>_VALUE for the CSD widgets and
*  CapSense_P2_SNS_STATUS<WidgetId>_VALUE for CSX widget). In addition, the
*  function provides details of the active sensor including active rows/columns
*  for the CSD widgets. This function is used only with the matrix button
*  widgets. This function does not process the widget but extracts
*  processed results from the \ref group_structures.
*
* \param widgetId
*  Specifies the ID number of the matrix button widget to check the status of its
*  sensors.
*  A macro for the widget ID can be found in the CapSense_P2 Configuration header file
*  defined as CapSense_P2_<WidgetName>_WDGT_ID
*
* \return
*  Returns the touch detection status of the sensors in the specified matrix
*  buttons widget. Zero indicates that no touch is detected in the specified
*  widget or a wrong widgetId is specified.
*    1. For the matrix buttons widgets with the CSD sensing mode:
*      - Bit [31] if set, indicates that one or more sensors in the widget
*        detected a touch.
*      - Bits [30..24] are reserved
*      - Bits [23..16] indicate the logical sensor number of the sensor that
*        detected a touch. If more than one sensor detected a touch for the CSD
*        widget, no status is reported because more than one touch is invalid for the
*        CSD matrix buttons widgets.
*      - Bits [15..8] indicate the active row number.
*      - Bits [7..0] indicate the active column number.
*    2. For the matrix buttons widgets with the CSX widgets, each bit (31..0)
*      corresponds to the TX/RX intersection.
*
*******************************************************************************/
uint32 CapSense_P2_IsMatrixButtonsActive(uint32 widgetId)
{
    uint32 result = 0Lu;
    cystatus state = CYRET_SUCCESS;
    CapSense_P2_FLASH_WD_STRUCT const *ptrFlashWdgt = 0u;

    #if (0u != CapSense_P2_CSD_MATRIX_WIDGET_EN)
        CapSense_P2_RAM_WD_CSD_MATRIX_STRUCT *wdCsdMatrix;
    #endif

    if (widgetId >= CapSense_P2_TOTAL_WIDGETS)
    {
        state = CYRET_BAD_PARAM;
    }
    else
    {
        ptrFlashWdgt = &CapSense_P2_dsFlash.wdgtArray[widgetId];

        if ((CapSense_P2_WD_TYPE_ENUM)ptrFlashWdgt->wdgtType != CapSense_P2_WD_MATRIX_BUTTON_E)
        {
            state = CYRET_BAD_PARAM;
        }
        else if (0u == CapSense_P2_GET_WIDGET_ACTIVE_STATUS(widgetId))
        {
            state = CYRET_BAD_PARAM;
        }
        else
        {
            /* input parameters are OK */
        }
    }

    if (CYRET_SUCCESS == state)
    {
        switch(CapSense_P2_GET_SENSE_METHOD(ptrFlashWdgt))
        {
        #if (0u != CapSense_P2_CSD_MATRIX_WIDGET_EN)
            case CapSense_P2_SENSE_METHOD_CSD_E:
                wdCsdMatrix = ptrFlashWdgt->ptr2WdgtRam;

                result = CapSense_P2_MATRIX_BUTTONS_TOUCHED |
                         ((uint32)wdCsdMatrix->posSnsId << 16u)  |
                         ((uint32)wdCsdMatrix->posRow   << 8u)   |
                         (uint32)wdCsdMatrix->posCol;
                break;
        #endif /* #if (0u != CapSense_P2_CSD_MATRIX_WIDGET_EN) */

        #if (0u != CapSense_P2_CSX_MATRIX_WIDGET_EN)
            case CapSense_P2_SENSE_METHOD_CSX_E:
                result = CapSense_P2_dsRam.snsStatus[widgetId];
                break;
        #endif /* #if (0u != CapSense_P2_CSX_MATRIX_WIDGET_EN) */

        default:
            CYASSERT(0u);
            break;
        }
    }

    return result;
}
#endif /* #if (0u != CapSense_P2_MATRIX_WIDGET_EN) */

#if (0u != CapSense_P2_PROXIMITY_WIDGET_EN)
/*******************************************************************************
* Function Name: CapSense_P2_IsProximitySensorActive
****************************************************************************//**
*
* \brief
*  Reports the finger detection status of the specified proximity widget/sensor.
*
* \details
*  This function reports if the specified proximity sensor has detected a touch
*  or not by extracting information from the wdgtStatus registers
*  (CapSense_P2_SNS_STATUS<WidgetId>_VALUE). This function is used
*  only with proximity sensor widgets. This function does not process the
*  widget but extracts processed results from the \ref group_structures.
*
* \param widgetId
*  Specifies the ID number of the proximity widget.
*  A macro for the widget ID can be found in the CapSense_P2 Configuration header
*  file defined as CapSense_P2_<WidgetName>_WDGT_ID
*
* \param proxId
*  Specifies the ID number of the proximity sensor within the proximity widget to
*  get its touch detection status.
*  A macro for the proximity ID within a specified widget can be found in the
*  CapSense_P2 Configuration header file defined as
*  CapSense_P2_<WidgetName>_SNS<SensorNumber>_ID
*
* \return
*  Returns the status of the specified sensor of the proximity widget. Zero
*  indicates that no touch is detected in the specified sensor / widget or a
*  wrong widgetId / proxId is specified.
*    - Bits [31..2] are reserved.
*    - Bit [1] indicates that a touch is detected.
*    - Bit [0] indicates that a proximity is detected.
*
*******************************************************************************/
uint32 CapSense_P2_IsProximitySensorActive(uint32 widgetId, uint32 proxId)
{
    uint32 result = 0Lu;

    if ((widgetId < CapSense_P2_TOTAL_WIDGETS) && (proxId < CapSense_P2_GET_SENSOR_COUNT(widgetId)) &&
        (CapSense_P2_WD_PROXIMITY_E ==
            (CapSense_P2_WD_TYPE_ENUM)CapSense_P2_dsFlash.wdgtArray[widgetId].wdgtType))
    {
        result = CapSense_P2_dsRam.snsStatus[widgetId];
        result >>= CapSense_P2_PROX_STS_OFFSET(proxId);
        result &=  CapSense_P2_PROX_STS_MASK;
    }

    return result;
}
#endif /* #if (0u != CapSense_P2_PROXIMITY_WIDGET_EN) */


#if (0u != CapSense_P2_SLIDER_WIDGET_EN)
/*******************************************************************************
* Function Name: CapSense_P2_GetCentroidPos
****************************************************************************//**
*
* \brief
*  Reports the centroid position for the specified slider widget.
*
* \details
*  This function reports the centroid value of a specified radial or linear
*  slider widget by extracting information from the wdgtStatus registers
*  (CapSense_P2_<WidgetName>_POSITION<X>_VALUE). This function is
*  used only with radial or linear slider widgets. This function does not
*  process the widget but extracts processed results from the \ref group_structures.
*
* \param widgetId
*  Specifies the ID number of a slider widget to get the centroid of the
*  detected touch.
*  A macro for the widget ID can be found in the
*  CapSense_P2 Configuration header file defined as
*  CapSense_P2_<WidgetName>_WDGT_ID
*
* \return
*  Returns the centroid position of a specified slider widget:
*    - The centroid position if a touch is detected.
*    - CapSense_P2_SLIDER_NO_TOUCH - No touch is detected or a wrong
*      widgetId is specified.
*
*******************************************************************************/
uint32 CapSense_P2_GetCentroidPos(uint32 widgetId)
{
    uint32 result = CapSense_P2_SLIDER_NO_TOUCH;
    CapSense_P2_RAM_WD_SLIDER_STRUCT *wdSlider;

    if ((widgetId < CapSense_P2_TOTAL_WIDGETS) &&
        ((CapSense_P2_WD_LINEAR_SLIDER_E ==
            (CapSense_P2_WD_TYPE_ENUM)CapSense_P2_dsFlash.wdgtArray[widgetId].wdgtType) ||
         (CapSense_P2_WD_RADIAL_SLIDER_E ==
            (CapSense_P2_WD_TYPE_ENUM)CapSense_P2_dsFlash.wdgtArray[widgetId].wdgtType)))
    {
        wdSlider = CapSense_P2_dsFlash.wdgtArray[widgetId].ptr2WdgtRam;
        result = (uint32)wdSlider->position[0u];
    }

    return result;
}
#endif /* #if (0u != CapSense_P2_SLIDER_WIDGET_EN) */


#if (0u != CapSense_P2_TOUCHPAD_WIDGET_EN)
/*******************************************************************************
* Function Name: CapSense_P2_GetXYCoordinates
****************************************************************************//**
*
* \brief
*  Reports the X/Y position detected for the specified touchpad widget.
*
* \details
*  This function reports a touch position (X and Y coordinates) value of a
*  specified touchpad widget by extracting information from the wdgtStatus
*  registers (CapSense_P2_<WidgetName>_POS_Y_VALUE). This function should
*  be used only with the touchpad widgets. This function does not process the
*  widget but extracts processed results from the \ref group_structures.
*
* \param widgetId
*  Specifies the ID number of a touchpad widget to get the X/Y position of a
*  detected touch.
*  A macro for the widget ID can be found in the
*  CapSense_P2 Configuration header file defined as
*  CapSense_P2_<WidgetName>_WDGT_ID.
*
* \return
*  Returns the touch position of a specified touchpad widget:
*    1. If a touch is detected:
*      - Bits [31..16] indicate the Y coordinate.
*      - Bits [15..0] indicate the X coordinate.
*    2. If no touch is detected or a wrong widgetId is specified:
*      - CapSense_P2_TOUCHPAD_NO_TOUCH.
*
*******************************************************************************/
uint32 CapSense_P2_GetXYCoordinates(uint32 widgetId)
{
    CapSense_P2_FLASH_WD_STRUCT const *ptrFlashWdgt = NULL;

    #if (0u != CapSense_P2_CSD_TOUCHPAD_WIDGET_EN)
        CapSense_P2_RAM_WD_CSD_TOUCHPAD_STRUCT *wdCsdTouchpad;
    #endif /* #if (0u != CapSense_P2_CSD_TOUCHPAD_WIDGET_EN) */

    #if (0u != CapSense_P2_CSX_TOUCHPAD_WIDGET_EN)
        CapSense_P2_RAM_WD_CSX_TOUCHPAD_STRUCT *wdCsxTouchpad;
    #endif /* #if (0u != CapSense_P2_CSX_TOUCHPAD_WIDGET_EN) */

    cystatus state = CYRET_SUCCESS;
    uint32 result = CapSense_P2_TOUCHPAD_NO_TOUCH;

    if (widgetId >= CapSense_P2_TOTAL_WIDGETS)
    {
        state = CYRET_BAD_PARAM;
    }
    else
    {
        ptrFlashWdgt = &CapSense_P2_dsFlash.wdgtArray[widgetId];

        if ((CapSense_P2_WD_TYPE_ENUM)ptrFlashWdgt->wdgtType != CapSense_P2_WD_TOUCHPAD_E)
        {
            state = CYRET_BAD_PARAM;
        }
    }

    if (CYRET_SUCCESS == state)
    {
        switch(CapSense_P2_GET_SENSE_METHOD(ptrFlashWdgt))
        {
        #if (0u != CapSense_P2_CSD_TOUCHPAD_WIDGET_EN)
            case CapSense_P2_SENSE_METHOD_CSD_E:
                wdCsdTouchpad = ptrFlashWdgt->ptr2WdgtRam;
                #if (CapSense_P2_ENABLE == CapSense_P2_CENTROID_5X5_CSD_EN)
                    #if (CapSense_P2_ENABLE == CapSense_P2_CENTROID_3X3_CSD_EN)
                        if (0 != (ptrFlashWdgt->staticConfig & CapSense_P2_CENTROID_5X5_MASK))
                        {
                    #endif
                        result = ((uint32)wdCsdTouchpad->position.pos[0u].x) |
                                 ((uint32)wdCsdTouchpad->position.pos[0u].y << 16u);
                    #if (CapSense_P2_ENABLE == CapSense_P2_CENTROID_3X3_CSD_EN)
                        }
                    #endif
                #endif

                #if (CapSense_P2_ENABLE == CapSense_P2_CENTROID_3X3_CSD_EN)
                    #if (CapSense_P2_ENABLE == CapSense_P2_CENTROID_5X5_CSD_EN)
                        if (0 != (ptrFlashWdgt->staticConfig & CapSense_P2_CENTROID_3X3_MASK))
                        {
                    #endif
                        result = ((uint32)wdCsdTouchpad->posX) |
                                 ((uint32)wdCsdTouchpad->posY << 16u);
                    #if (CapSense_P2_ENABLE == CapSense_P2_CENTROID_5X5_CSD_EN)
                        }
                    #endif
                #endif

                break;
        #endif /* #if (0u != CapSense_P2_CSD_TOUCHPAD_WIDGET_EN) */

        #if (0u != CapSense_P2_CSX_TOUCHPAD_WIDGET_EN)
            case CapSense_P2_SENSE_METHOD_CSX_E:
                wdCsxTouchpad = ptrFlashWdgt->ptr2WdgtRam;

                result = ((uint32)wdCsxTouchpad->touch[0u].x) |
                         ((uint32)wdCsxTouchpad->touch[0u].y << 16u);
                break;
        #endif /* #if (0u != CapSense_P2_CSX_TOUCHPAD_WIDGET_EN) */

        default:
            CYASSERT(0u);
            break;
        }
    }

    return result;
}
#endif /* #if (0u != CapSense_P2_TOUCHPAD_WIDGET_EN) */


/* [] END OF FILE */
