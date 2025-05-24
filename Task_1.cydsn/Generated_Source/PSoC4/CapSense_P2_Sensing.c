/***************************************************************************//**
* \file CapSense_P2_Sensing.c
* \version 7.0
*
* \brief
*   This file contains the source of functions common for
*   different sensing methods.
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

#include "cyfitter.h"
#include "CapSense_P2_Configuration.h"
#include "CapSense_P2_Structure.h"
#include "CapSense_P2_Sensing.h"
#if (CapSense_P2_ENABLE == CapSense_P2_CSX_EN)
    #include "CapSense_P2_SensingCSX_LL.h"
#endif
#if (CapSense_P2_ENABLE == CapSense_P2_ISX_EN)
    #include "CapSense_P2_SensingISX_LL.h"
#endif
#if (CapSense_P2_ENABLE == CapSense_P2_CSD_EN)
    #include "CapSense_P2_SensingCSD_LL.h"
#endif
#if (CapSense_P2_CSD_SS_DIS != CapSense_P2_CSD_AUTOTUNE)
    #include "CapSense_P2_SmartSense_LL.h"
#endif
#if (CapSense_P2_ENABLE == CapSense_P2_ADC_EN)
    #include "CapSense_P2_Adc.h"
#endif
#if (CapSense_P2_ENABLE == CapSense_P2_SELF_TEST_EN)
    #include "CapSense_P2_SelfTest.h"
#endif

#if (0u != CapSense_P2_ISX_EN)
    #include "CapSense_P2_DigPrt2.h"
    #if (CapSense_P2_USES_PORT_3)
        #include "CapSense_P2_DigPrt3.h"
    #endif
#endif

/***************************************
* API Constants
***************************************/

#define CapSense_P2_IMO_FREQUENCY_OFFSET_MIN               (0u)
#define CapSense_P2_IMO_FREQUENCY_OFFSET_MAX               (255u)
#define CapSense_P2_AUTOTUNE_CALIBRATION_RESOLUTION        (12u)
#define CapSense_P2_AUTOTUNE_CALIBRATION_FREQ_KHZ          (1500u)
#define CapSense_P2_AUTOTUNE_CAL_LEVEL                     (CapSense_P2_CSD_RAWCOUNT_CAL_LEVEL)
#define CapSense_P2_AUTOTUNE_CAL_UNITS                     (1000u)
#define CapSense_P2_AUTOTUNE_CP_MAX                        (69000Lu)
#define CapSense_P2_CLK_SOURCE_LFSR_SCALE_OFFSET           (4u)

/* HW IP Block limitation */
#if (CapSense_P2_ENABLE == CapSense_P2_CSDV2)
    #define CapSense_P2_CSD_SNS_FREQ_KHZ_MAX               (6000u)
#else
    #define CapSense_P2_CSD_SNS_FREQ_KHZ_MAX               (12600u)
#endif /* (CapSense_P2_ENABLE == CapSense_P2_CSDV2) */

#define CapSense_P2_FLIP_FLOP_DIV                          (1u)

#define CapSense_P2_MOD_CSD_CLK_12000KHZ                   (12000uL)
#define CapSense_P2_MOD_CSD_CLK_24000KHZ                   (24000uL)
#define CapSense_P2_MOD_CSD_CLK_48000KHZ                   (48000uL)

/*****************************************************************************/
/* Enumeration types definition                                              */
/*****************************************************************************/

typedef enum
{
    CapSense_P2_RES_PULLUP_E   = 0x02u,
    CapSense_P2_RES_PULLDOWN_E = 0x03u
} CapSense_P2_PORT_TEST_DM;

typedef enum
{
    CapSense_P2_STS_RESET      = 0x01u,
    CapSense_P2_STS_NO_RESET   = 0x02u
} CapSense_P2_TEST_TYPE;


/*******************************************************************************
* Static Function Prototypes
*******************************************************************************/
/**
* \cond SECTION_C_INTERNAL
* \addtogroup group_c_internal
* \{
*/

#if (CapSense_P2_DISABLE == CapSense_P2_CSDV2)
    static void CapSense_P2_SsTrimIdacs(void);
    #if ((CapSense_P2_ENABLE == CapSense_P2_CSX_EN) || \
         (CapSense_P2_ENABLE == CapSense_P2_ISX_EN) || \
         (CapSense_P2_IDAC_SINKING == CapSense_P2_CSD_IDAC_CONFIG))
        static void CapSense_P2_SsTrimIdacsSinking(void);
    #endif /* ((CapSense_P2_ENABLE == CapSense_P2_CSX_EN) || \
               (CapSense_P2_IDAC_SINKING == CapSense_P2_CSD_IDAC_CONFIG)) */
    #if ((CapSense_P2_ENABLE == CapSense_P2_CSX_EN) || \
         (CapSense_P2_ENABLE == CapSense_P2_ISX_EN) || \
         (CapSense_P2_IDAC_SOURCING == CapSense_P2_CSD_IDAC_CONFIG))
        static void CapSense_P2_SsTrimIdacsSourcing(void);
    #endif /* ((CapSense_P2_ENABLE == CapSense_P2_CSX_EN) || \
               (CapSense_P2_IDAC_SOURCING == CapSense_P2_CSD_IDAC_CONFIG)) */
#endif /* (CapSense_P2_ENABLE == CapSense_P2_CSDV2) */

#if (CapSense_P2_ENABLE == CapSense_P2_CSD_EN)
    static void CapSense_P2_SsCSDDisableMode(void);
#endif /* (CapSense_P2_ENABLE == CapSense_P2_CSD_EN) */
#if (CapSense_P2_ENABLE == CapSense_P2_CSX_EN)
    static void CapSense_P2_SsDisableCSXMode(void);
#endif /* (CapSense_P2_ENABLE == CapSense_P2_CSX_EN) */
#if (CapSense_P2_ENABLE == CapSense_P2_ISX_EN)
    static void CapSense_P2_SsDisableISXMode(void);
#endif /* (CapSense_P2_ENABLE == CapSense_P2_ISX_EN) */

#if(((CapSense_P2_ENABLE == CapSense_P2_CSX_EN) && \
     (CapSense_P2_ENABLE == CapSense_P2_CSDV2) && \
     (CapSense_P2_CLK_SOURCE_PRSAUTO == CapSense_P2_CSX_TX_CLK_SOURCE) && \
     (CapSense_P2_DISABLE == CapSense_P2_CSX_SKIP_OVSMPL_SPECIFIC_NODES)) ||\
    ((CapSense_P2_ENABLE == CapSense_P2_CSD_EN) && \
     (CapSense_P2_ENABLE == CapSense_P2_CSDV2) && \
     (CapSense_P2_CLK_SOURCE_PRSAUTO == CapSense_P2_CSD_SNS_CLK_SOURCE)))
    static uint8 CapSense_P2_SsCalcLfsrSize(uint32 snsClkDivider, uint32 conversionsNum);
    static uint8 CapSense_P2_SsCalcLfsrScale(uint32 snsClkDivider, uint8 lfsrSize);
#endif

#if (CapSense_P2_ENABLE == CapSense_P2_CSD_EN)
    static void CapSense_P2_SsSetWidgetSenseClkSrc(uint32 wdgtIndex, CapSense_P2_RAM_WD_BASE_STRUCT * ptrWdgt);
#endif

#if ((CapSense_P2_ENABLE == CapSense_P2_CSX_EN) && (CapSense_P2_ENABLE == CapSense_P2_CSDV2))
    static void CapSense_P2_SsSetWidgetTxClkSrc(uint32 wdgtIndex, CapSense_P2_RAM_WD_BASE_STRUCT * ptrWdgt);
#endif


/** \}
* \endcond */

/*******************************************************************************
* Defines module variables
*******************************************************************************/

CapSense_P2_SENSE_METHOD_ENUM CapSense_P2_currentSenseMethod = CapSense_P2_UNDEFINED_E;

#if(CapSense_P2_ENABLE == CapSense_P2_MULTI_FREQ_SCAN_EN)
    /* Module variable keep track of multi-frequency scan channel index */
    uint8 CapSense_P2_scanFreqIndex = 0u;
    /* Variable keep frequency offsets */
    uint8 CapSense_P2_immunity[CapSense_P2_NUM_SCAN_FREQS] = {0u, 0u, 0u};
#else
    /* const allows C-compiler to do optimization */
    const uint8 CapSense_P2_scanFreqIndex = 0u;
#endif

/* Global software variables */
volatile uint8 CapSense_P2_widgetIndex = 0u;    /* Index of the scanning widget */
volatile uint8 CapSense_P2_sensorIndex = 0u;    /* Index of the scanning sensor */
uint8 CapSense_P2_requestScanAllWidget = 0u;

/* Pointer to RAM_SNS_STRUCT structure */
CapSense_P2_RAM_SNS_STRUCT *CapSense_P2_curRamSnsPtr;

#if ((CapSense_P2_ENABLE == CapSense_P2_CSD_GANGED_SNS_EN) || \
     (CapSense_P2_ENABLE == CapSense_P2_CSX_EN) || \
     (CapSense_P2_ENABLE == CapSense_P2_ISX_EN))
    /* Pointer to Flash structure holding configuration of widget to be scanned */
    CapSense_P2_FLASH_WD_STRUCT const *CapSense_P2_curFlashWdgtPtr = 0u;
#endif

#if (CapSense_P2_ENABLE == CapSense_P2_CSD_GANGED_SNS_EN)
    /* Pointer to Flash structure holding info of sensor to be scanned */
    CapSense_P2_FLASH_SNS_STRUCT const *CapSense_P2_curFlashSnsPtr = 0u;
#endif

/* Pointer to Flash structure to hold Sns electrode that was connected previously */
CapSense_P2_FLASH_IO_STRUCT const *CapSense_P2_curSnsIOPtr;

CapSense_P2_IDAC_GAIN_TABLE_STRUCT const CapSense_P2_idacGainTable[CapSense_P2_IDAC_GAIN_NUMBER] = CapSense_P2_IDAC_GAIN_TABLE_INIT;


/*******************************************************************************
* Function Name: CapSense_P2_IsBusy
****************************************************************************//**
*
* \brief
*  Returns the current status of the Component (Scan is completed or Scan is in
*  progress).
*
* \details
*  This function returns a status of the hardware block whether a scan is
*  currently in progress or not. If the Component is busy, no new scan or Widget
*  setup is made. The critical section (i.e. disable global interrupt)
*  is recommended for the application when the device transitions from
*  the active mode to sleep or deep sleep modes.
*
* \return
*  Returns the current status of the Component:
*    - CapSense_P2_NOT_BUSY - No scan is in progress and a next scan
*      can be initiated.
*    - CapSense_P2_SW_STS_BUSY - The previous scanning is not completed
*      and the hardware block is busy.
*
*******************************************************************************/
uint32 CapSense_P2_IsBusy(void)
{
    return (CapSense_P2_dsRam.status & CapSense_P2_SW_STS_BUSY);
}

/*******************************************************************************
* Function Name: CapSense_P2_SetupWidget
****************************************************************************//**
*
* \brief
*  Performs the initialization required to scan the specified widget.
*
* \details
*  This function prepares the Component to scan all the sensors in the specified
*  widget by executing the following tasks:
*    1. Re-initialize the hardware if it is not configured to perform the
*       sensing method used by the specified widget, this happens only if multiple
*       sensing methods are used in the Component.
*    2. Initialize the hardware with specific sensing configuration (e.g.
*       sensor clock, scan resolution) used by the widget.
*    3. Disconnect all previously connected electrodes, if the electrodes
*       connected by the lower level SetupWidgetExt() or ConnectSns() functions
        and not disconnected.
*
*  This function does not start sensor scanning, the CapSense_P2_Scan()
*  function must be called to start the scan sensors in the widget. If this
*  function is called more than once, it does not break the Component operation,
*  but only the last initialized widget is in effect.
*
* \param widgetId
*  Specifies the ID number of the widget to be initialized for scanning.
*  A macro for the widget ID can be found in the
*  CapSense_P2 Configuration header file defined as
*  CapSense_P2_<WidgetName>_WDGT_ID.
*
* \return
*  Returns the status of the widget setting up operation:
*    - CYRET_SUCCESS - The operation is successfully completed.
*    - CYRET_BAD_PARAM - The widget is invalid or if the specified widget is
*      disabled
*    - CYRET_INVALID_STATE - The previous scanning is not completed and the
*      hardware block is busy.
*    - CYRET_UNKNOWN - An unknown sensing method is used by the widget or any
*      other spurious error occurred.
*
**********************************************************************************/
cystatus CapSense_P2_SetupWidget(uint32 widgetId)
{
    cystatus widgetStatus;

    if (CapSense_P2_WDGT_SW_STS_BUSY == (CapSense_P2_dsRam.status & CapSense_P2_WDGT_SW_STS_BUSY))
    {
        /* Previous widget is being scanned. Return error. */
        widgetStatus = CYRET_INVALID_STATE;
    }
    /*
     *  Check if widget id is valid, specified widget is enabled and widget did not
     *  detect any fault conditions if BIST is enabled. If all conditions are met,
     *  set widgetStatus as good, if not, set widgetStatus as bad.
     */
    else if ((CapSense_P2_TOTAL_WIDGETS > widgetId) &&
        (0uL != CapSense_P2_GET_WIDGET_EN_STATUS(widgetId)))

    {
        widgetStatus = CYRET_SUCCESS;
    }
    else
    {
        widgetStatus = CYRET_BAD_PARAM;
    }

    /*
     * Check widgetStatus flag that is set earlier, if flag is good, then set up only
     * widget
     */
    if (CYRET_SUCCESS == widgetStatus)
    {
        switch (CapSense_P2_GET_SENSE_METHOD(&CapSense_P2_dsFlash.wdgtArray[widgetId]))
        {
        #if (CapSense_P2_ENABLE == CapSense_P2_CSX_EN)
            case CapSense_P2_SENSE_METHOD_CSX_E:
                /* Set up widget for CSX scan */
                CapSense_P2_CSXSetupWidget(widgetId);
                break;
        #endif
        #if (CapSense_P2_ENABLE == CapSense_P2_CSD_EN)
            case CapSense_P2_SENSE_METHOD_CSD_E:
                /* Set up widget for CSD scan */
                CapSense_P2_CSDSetupWidget(widgetId);
                break;
        #endif
        #if (CapSense_P2_ENABLE == CapSense_P2_ISX_EN)
            case CapSense_P2_SENSE_METHOD_ISX_E:
                /* Set up widget for ISX scan */
                CapSense_P2_ISXSetupWidget(widgetId);
                break;
        #endif
        default:
            /* Sensing method is invalid, return error to caller */
            widgetStatus = CYRET_UNKNOWN;
            break;
        }
    }
    return (widgetStatus);
}


/*******************************************************************************
* Function Name: CapSense_P2_Scan
****************************************************************************//**
*
* \brief
*  Initiates scanning of all the sensors in the widget initialized by
*  CapSense_P2_SetupWidget(), if no scan is in progress.
*
* \details
*  This function is called only after the CapSense_P2_SetupWidget()
*  function is called to start the scanning of the sensors in the widget. The
*  status of a sensor scan must be checked using the CapSense_P2_IsBusy()
*  API prior to starting a next scan or setting up another widget.
*
* \return
*  Returns the status of the scan initiation operation:
*  - CYRET_SUCCESS - Scanning is successfully started.
*  - CYRET_INVALID_STATE - The previous scanning is not completed and the
*    hardware block is busy.
*  - CYRET_UNKNOWN - An unknown sensing method is used by the widget.
*
********************************************************************************/
cystatus CapSense_P2_Scan(void)
{
    cystatus scanStatus = CYRET_SUCCESS;

    if (CapSense_P2_WDGT_SW_STS_BUSY == (CapSense_P2_dsRam.status & CapSense_P2_WDGT_SW_STS_BUSY))
    {
        /* Previous widget is being scanned. Return error. */
        scanStatus = CYRET_INVALID_STATE;
    }
    else
    {
        switch (CapSense_P2_currentSenseMethod)
        {
        #if (CapSense_P2_ENABLE == CapSense_P2_CSX_EN)
            case CapSense_P2_SENSE_METHOD_CSX_E:
                CapSense_P2_CSXScan();
                break;
        #endif
        #if (CapSense_P2_ENABLE == CapSense_P2_CSD_EN)
            case CapSense_P2_SENSE_METHOD_CSD_E:
                CapSense_P2_CSDScan();
                break;
        #endif
        #if (CapSense_P2_ENABLE == CapSense_P2_ISX_EN)
            case CapSense_P2_SENSE_METHOD_ISX_E:
                CapSense_P2_ISXScan();
                break;
        #endif
        default:
            scanStatus = CYRET_UNKNOWN;
            break;
        }
    }
    return (scanStatus);
}


/*******************************************************************************
* Function Name: CapSense_P2_ScanAllWidgets
****************************************************************************//**
*
* \brief
*  Initializes the first enabled widget and scanning of all the sensors in the
*  widget, then the same process is repeated for all the widgets in the Component,
*  i.e. scanning of all the widgets in the Component.
*
* \details
*  This function initializes a widget and scans all the sensors in the widget,
*  and then repeats the same for all the widgets in the Component. The tasks of
*  the CapSense_P2_SetupWidget() and CapSense_P2_Scan() functions are
*  executed by these functions. The status of a sensor scan must be checked
*  using the CapSense_P2_IsBusy() API prior to starting a next scan
*  or setting up another widget.
*
* \return
*  Returns the status of the operation:
*  - CYRET_SUCCESS - Scanning is successfully started.
*  - CYRET_BAD_PARAM - All the widgets are disabled.
*  - CYRET_INVALID_STATE - The previous scanning is not completed and the HW block is busy.
*  - CYRET_UNKNOWN - There are unknown errors.
*
*******************************************************************************/
cystatus CapSense_P2_ScanAllWidgets(void)
{
    cystatus scanStatus = CYRET_UNKNOWN;

    uint32 wdgtIndex;

    if (CapSense_P2_SW_STS_BUSY == (CapSense_P2_dsRam.status & CapSense_P2_SW_STS_BUSY))
    {
        /* Previous widget is being scanned. Return error. */
        scanStatus = CYRET_INVALID_STATE;
    }
    else
    {
        /*
         *  Set up widget first widget.
         *  If widget returned error, set up next, continue same until widget does not return error.
         */
        for (wdgtIndex = 0u;
             wdgtIndex < CapSense_P2_TOTAL_WIDGETS;
             wdgtIndex++)
        {

            scanStatus = CapSense_P2_SetupWidget(wdgtIndex);

            if (CYRET_SUCCESS == scanStatus)
            {
                #if (0u != (CapSense_P2_TOTAL_WIDGETS - 1u))
                    /* If there are more than one widget to be scanned, request callback to scan next widget */
                    if ((CapSense_P2_TOTAL_WIDGETS - 1u) > wdgtIndex)
                    {
                         /* Request callback to scan next widget in ISR */
                        CapSense_P2_requestScanAllWidget = CapSense_P2_ENABLE;
                    }
                    else
                    {
                        /* Request to exit in ISR (Do not scan the next widgets) */
                        CapSense_P2_requestScanAllWidget = CapSense_P2_DISABLE;
                    }
                #else
                    {
                        /* Request to exit in ISR (We have only one widget) */
                        CapSense_P2_requestScanAllWidget = CapSense_P2_DISABLE;
                    }
                #endif  /* (0u != (CapSense_P2_TOTAL_WIDGETS - 1u)) */

                /* Initiate scan and quit loop */
                scanStatus = CapSense_P2_Scan();

                break;
            }
        }
    }

    return (scanStatus);
}


/*******************************************************************************
* Function Name: CapSense_P2_SetupWidgetExt
****************************************************************************//**
*
* \brief
*  Performs extended initialization for the specified widget and also performs
*  initialization required for a specific sensor in the widget. This function
*  requires using the CapSense_P2_ScanExt() function to initiate a scan.
*
* \details
*  This function does the same as CapSense_P2_SetupWidget() and also
*  does the following tasks:
*    1. Connects the first sensor of the widget.
*    2. Configures the CSD HW block to perform a scan of the specified sensor.
*
*  Once this function is called to initialize a widget and a sensor, the
*  CapSense_P2_ScanExt() function is called to scan the sensor.
*
*  This function is called when no scanning is in progress. I.e.
*  CapSense_P2_IsBusy() returns a non-busy status.
*
*  Calling this function directly from the application layer is not
*  recommended. This function is used to implement only the user's specific
*  use cases (for faster execution time or pipeline scanning for example).
*
* \param widgetId
*  Specifies the ID number of the widget to perform hardware and firmware
*  initialization required for scanning the specific sensor in the specific
*  widget.
*  A macro for the widget ID can be found in the CapSense_P2
*  Configuration header file defined as CapSense_P2_<WidgetName>_WDGT_ID.
*
* \param sensorId
*  Specifies the ID number of the sensor within the widget to perform hardware
*  and firmware initialization required for scanning a specific sensor in a
*  specific widget.
*  A macro for the sensor ID within a specified widget can be found in the
*  CapSense_P2 Configuration header file defined as
*  CapSense_P2_<WidgetName>_SNS<SensorNumber>_ID
*
* \return
*  Returns the status of the operation:
*  - CYRET_SUCCESS - The operation is successfully completed.
*  - CYRET_BAD_PARAM - The widget is invalid or if the specified widget is
*    disabled
*  - CYRET_INVALID_STATE - The previous scanning is not completed and the
*    hardware block is busy.
*  - CYRET_UNKNOWN - An unknown sensing method is used by the widget or any
*    other spurious error occurred.
*
*******************************************************************************/
cystatus CapSense_P2_SetupWidgetExt(uint32 widgetId, uint32 sensorId)
{
    cystatus widgetStatus;

    if (CapSense_P2_WDGT_SW_STS_BUSY == (CapSense_P2_dsRam.status & CapSense_P2_WDGT_SW_STS_BUSY))
    {
        /* Previous widget is being scanned. Return error. */
        widgetStatus = CYRET_INVALID_STATE;
    }
    /*
     *  Check if widget id is valid, specified widget is enabled and widget did not
     *  detect any fault conditions if BIST is enabled. If all conditions are met,
     *  set widgetStatus as good, if not, set widgetStatus as bad.
     */
    else if ((CapSense_P2_TOTAL_WIDGETS > widgetId) &&
        (0uL != CapSense_P2_GET_WIDGET_EN_STATUS(widgetId)) &&
        (sensorId < CapSense_P2_GET_SENSOR_COUNT(widgetId)))

    {
        widgetStatus = CYRET_SUCCESS;
    }
    else
    {
        widgetStatus = CYRET_BAD_PARAM;
    }

    /*
     * Check widgetStatus flag that is set earlier, if flag is good, then set up only
     * widget
     */
    if (CYRET_SUCCESS == widgetStatus)
    {
        switch (CapSense_P2_GET_SENSE_METHOD(&CapSense_P2_dsFlash.wdgtArray[widgetId]))
        {
        #if (CapSense_P2_ENABLE == CapSense_P2_CSX_EN)
            case CapSense_P2_SENSE_METHOD_CSX_E:
                /* Set up widget for CSX scan */
                CapSense_P2_CSXSetupWidgetExt(widgetId, sensorId);
                break;
        #endif
        #if (CapSense_P2_ENABLE == CapSense_P2_CSD_EN)
            case CapSense_P2_SENSE_METHOD_CSD_E:
                /* Set up widget for CSD scan */
                CapSense_P2_CSDSetupWidgetExt(widgetId, sensorId);
                break;
        #endif
        #if (CapSense_P2_ENABLE == CapSense_P2_ISX_EN)
            case CapSense_P2_SENSE_METHOD_ISX_E:
                /* Set up widget for ISX scan */
                CapSense_P2_ISXSetupWidgetExt(widgetId, sensorId);
                break;
        #endif
        default:
            /* Sensing method is invalid, return error to caller */
            widgetStatus = CYRET_UNKNOWN;
            break;
        }
    }
    return (widgetStatus);
}


/*******************************************************************************
* Function Name: CapSense_P2_ScanExt
****************************************************************************//**
*
* \brief
*  Starts a conversion on the pre-configured sensor. This function requires
*  using the CapSense_P2_SetupWidgetExt() function to set up the a
*  widget.
*
* \details
*  This function performs single scanning of one sensor in the widget configured
*  by the CapSense_P2_SetupWidgetExt() function.
*
*  Calling this function directly from the application layer is not
*  recommended. This function is used to implement only the user's specific
*  use cases (for faster execution time or pipeline scanning for example).
*  This function is called when no scanning is in progress. I.e.
*  CapSense_P2_IsBusy() returns a non-busy status.
*
*  The sensor must be preconfigured by using the
*  CapSense_P2_SetupWidgetExt() API prior to calling this function.
*  The sensor remains ready for a next scan if a previous scan was triggered
*  by using the CapSense_P2_ScanExt() function. In this case, calling
*  CapSense_P2_SetupWidgetExt() is not required every time before the
*  CapSense_P2_ScanExt() function. If a previous scan was triggered in
*  any other way - CapSense_P2_Scan(), CapSense_P2_ScanAllWidgets() or
*  CapSense_P2_RunTuner() - (see the CapSense_P2_RunTuner() function
*  description for more details), the sensor must be preconfigured again by
*  using the CapSense_P2_SetupWidgetExt() API prior to calling the
*  CapSense_P2_ScanExt() function.
*
*  If disconnection of the sensors is required after calling
*  CapSense_P2_ScanExt(), the CapSense_P2_CSDDisconnectSns() or
*  CapSense_P2_CSXDisconnectTx() or CapSense_P2_CSXDisconnectRx()
*  functions can be used.
*
* \return
*  Returns the status of the scan initiation operation:
*  - CYRET_SUCCESS - Scanning is successfully started.
*  - CYRET_INVALID_STATE - The previous scanning is not completed and the
*    hardware block is busy.
*  - CYRET_UNKNOWN - An unknown sensing method is used by the widget.
*
*******************************************************************************/
cystatus CapSense_P2_ScanExt(void)
{
    cystatus scanStatus = CYRET_SUCCESS;

    if (CapSense_P2_WDGT_SW_STS_BUSY == (CapSense_P2_dsRam.status & CapSense_P2_WDGT_SW_STS_BUSY))
    {
        /* Previous widget is being scanned. Return error. */
        scanStatus = CYRET_INVALID_STATE;
    }
    else
    {
        switch (CapSense_P2_currentSenseMethod)
        {
        #if (CapSense_P2_ENABLE == CapSense_P2_CSX_EN)
            case CapSense_P2_SENSE_METHOD_CSX_E:
                CapSense_P2_CSXScanExt();
                break;
        #endif
        #if (CapSense_P2_ENABLE == CapSense_P2_CSD_EN)
            case CapSense_P2_SENSE_METHOD_CSD_E:
                CapSense_P2_CSDScanExt();
                break;
        #endif
        #if (CapSense_P2_ENABLE == CapSense_P2_ISX_EN)
            case CapSense_P2_SENSE_METHOD_ISX_E:
                CapSense_P2_ISXScanExt();
                break;
        #endif
        default:
            scanStatus = CYRET_UNKNOWN;
            break;
        }
    }
    return (scanStatus);
}


/*******************************************************************************
* Function Name: CapSense_P2_SsInitialize
****************************************************************************//**
*
* \brief
*  Performs hardware and firmware initialization required for proper operation
*  of the CapSense_P2 Component. This function is called from
*  the CapSense_P2_Start() API prior to calling any other APIs of the Component.
*
* \details
*  Performs hardware and firmware initialization required for proper operation
*  of the CapSense_P2 Component. This function is called from
*  the CapSense_P2_Start() API prior to calling any other APIs of the Component.
*  1. Depending on the configuration, the function initializes the CSD block
*     for the corresponding sensing mode.
*  2. The function updates the dsRam.wdgtWorking variable with 1 when Self-test
*     is enabled.
*
*  Calling the CapSense_P2_Start API is the recommended method to initialize
*  the CapSense_P2 Component at power-up. The CapSense_P2_SsInitialize()
*  API should not be used for initialization, resume, or wake-up operations.
*  The dsRam.wdgtWorking variable is updated.
*
* \return status
*  Returns status of operation:
*  - Zero        - Indicates successful initialization.
*  - Non-zero    - One or more errors occurred in the initialization process.
*
*******************************************************************************/
cystatus CapSense_P2_SsInitialize(void)
{
    cystatus initStatus;

    #if ((CapSense_P2_ENABLE == CapSense_P2_MULTI_FREQ_SCAN_EN) &&\
         (CapSense_P2_MFS_IMO == CapSense_P2_MFS_METHOD))
        uint8 centerFreqTrim;

        centerFreqTrim = (uint8)CY_GET_REG32(CY_SYS_CLK_IMO_TRIM1_PTR);
        CapSense_P2_immunity[CapSense_P2_FREQ_CHANNEL_0] = centerFreqTrim;

        if (centerFreqTrim < CapSense_P2_IMO_FREQUENCY_OFFSET_F1)
        {
            CapSense_P2_immunity[CapSense_P2_FREQ_CHANNEL_1] = centerFreqTrim + CapSense_P2_IMO_FREQUENCY_OFFSET_F1;
            CapSense_P2_immunity[CapSense_P2_FREQ_CHANNEL_2] = centerFreqTrim + (CapSense_P2_IMO_FREQUENCY_OFFSET_F1 +
                                                                                           CapSense_P2_IMO_FREQUENCY_OFFSET_F2);
        }
        else if (centerFreqTrim > (CapSense_P2_IMO_FREQUENCY_OFFSET_MAX - CapSense_P2_IMO_FREQUENCY_OFFSET_F2))
        {
            CapSense_P2_immunity[CapSense_P2_FREQ_CHANNEL_1] = centerFreqTrim - CapSense_P2_IMO_FREQUENCY_OFFSET_F1;
            CapSense_P2_immunity[CapSense_P2_FREQ_CHANNEL_2] = centerFreqTrim - (CapSense_P2_IMO_FREQUENCY_OFFSET_F1 +
                                                                                           CapSense_P2_IMO_FREQUENCY_OFFSET_F2);
        }
        else
        {
            CapSense_P2_immunity[CapSense_P2_FREQ_CHANNEL_1] = centerFreqTrim - CapSense_P2_IMO_FREQUENCY_OFFSET_F1;
            CapSense_P2_immunity[CapSense_P2_FREQ_CHANNEL_2] = centerFreqTrim + CapSense_P2_IMO_FREQUENCY_OFFSET_F2;
        }

    #endif /* ((CapSense_P2_ENABLE == CapSense_P2_MULTI_FREQ_SCAN_EN) &&\
               (CapSense_P2_MFS_IMO == CapSense_P2_MFS_METHOD)) */

    initStatus = CYRET_SUCCESS;

    #if (CapSense_P2_DISABLE == CapSense_P2_CSDV2)
        CapSense_P2_SsTrimIdacs();
    #endif /* (CapSense_P2_ENABLE == CapSense_P2_CSDV2) */

    #if((CapSense_P2_ENABLE == CapSense_P2_CSD_EN) ||\
    ((CapSense_P2_ENABLE == CapSense_P2_CSX_EN) && (CapSense_P2_ENABLE == CapSense_P2_CSDV2)))
        CapSense_P2_SsInitializeSourceSenseClk();
    #endif /* ((CapSense_P2_ENABLE == CapSense_P2_CSD_EN) ||\
              ((CapSense_P2_ENABLE == CapSense_P2_CSX_EN) && (CapSense_P2_ENABLE == CapSense_P2_CSDV2))) */

    /* Set all IO states to the default state */
    CapSense_P2_SsSetAllIOsState((uint32)CapSense_P2_SNS_CONNECTION_GROUND);
    CapSense_P2_dsRam.scanCurrentISC = CapSense_P2_SNS_CONNECTION_GROUND;

    CapSense_P2_SsSwitchSensingMode(CapSense_P2_UNDEFINED_E);
    #if (CapSense_P2_ENABLE == CapSense_P2_MANY_SENSE_MODES_EN)
    #elif (CapSense_P2_ENABLE == CapSense_P2_CSD_EN)
        CapSense_P2_SsSwitchSensingMode(CapSense_P2_SENSE_METHOD_CSD_E);
    #elif (CapSense_P2_ENABLE == CapSense_P2_CSX_EN)
        CapSense_P2_SsSwitchSensingMode(CapSense_P2_SENSE_METHOD_CSX_E);
    #elif (CapSense_P2_ENABLE == CapSense_P2_ISX_EN)
        CapSense_P2_SsSwitchSensingMode(CapSense_P2_SENSE_METHOD_ISX_E);
    #else
        #error "No sensing method enabled, Component cannot work in this mode"
        initStatus = CYRET_UNKNOWN;
    #endif

    return (initStatus);
}


/*******************************************************************************
* Function Name: CapSense_P2_SetPinState
****************************************************************************//**
*
* \brief
*  Sets the state (drive mode and output state) of the port pin used by a sensor.
*  The possible states are GND, Shield, High-Z, Tx or Rx, Sensor. If the sensor
*  specified in the input parameter is a ganged sensor, then the state of all pins
*  associated with the ganged sensor is updated.
*
* \details
*  This function sets a specified state for a specified sensor element. For the
*  CSD widgets, sensor element is a sensor ID, for the CSX widgets, it is either
*  an Rx or Tx electrode ID. If the specified sensor is a ganged sensor, then
*  the specified state is set for all the electrodes belong to the sensor.
*  This function must not be called while the Component is in the busy state.
*
*  This function accepts the CapSense_P2_SHIELD and
*  CapSense_P2_SENSOR states as an input only if there is at least
*  one CSD widget. Similarly, this function accepts the CapSense_P2_TX_PIN
*  and CapSense_P2_RX_PIN states as an input only if there is at least
*  one CSX widget in the project.

*  Calling this function directly from the application layer is not
*  recommended. This function is used to implement only the custom-specific
*  use cases. Functions that perform a setup and scan of a sensor/widget
*  automatically set the required pin states. They ignore changes
*  in the design made by the CapSense_P2_SetPinState() function.
*  This function neither check wdgtIndex nor sensorElement for the correctness.
*
* \param widgetId
*  Specifies the ID of the widget to change the pin state of the specified
*  sensor.
*  A macro for the widget ID can be found in the CapSense_P2 Configuration
*  header file defined as CapSense_P2_<WidgetName>_WDGT_ID.
*
* \param sensorElement
*  Specifies the ID of the sensor element within the widget to change
*  its pin state.
*  \if SECTION_C_LOW_LEVEL
*  For the CSD widgets, sensorElement is the sensor ID and can be found in the
*  CapSense_P2 Configuration header file defined as
*  - CapSense_P2_<WidgetName>_SNS<SensorNumber>_ID.
*  For the CSX widgets, sensorElement is defined either as Rx ID or Tx ID.
*  The first Rx in a widget corresponds to sensorElement = 0, the second
*  Rx in a widget corresponds to sensorElement = 1, and so on.
*  The last Tx in a widget corresponds to sensorElement = (RxNum + TxNum).
*  \endif
*  Macros for Rx and Tx IDs can be found in the
*  CapSense_P2 Configuration header file defined as:
*  - CapSense_P2_<WidgetName>_RX<RXNumber>_ID
*  - CapSense_P2_<WidgetName>_TX<TXNumber>_ID.
*
* \param state
*  Specifies the state of the sensor to be set:
*     1. CapSense_P2_GROUND - The pin is connected to the ground.
*     2. CapSense_P2_HIGHZ - The drive mode of the pin is set to High-Z
*        Analog.
*     3. CapSense_P2_SHIELD - The shield signal is routed to the pin
*        (available only if CSD sensing method with shield electrode is enabled).
*     4. CapSense_P2_SENSOR - The pin is connected to the scanning bus
*        (available only if CSD sensing method is enabled).
*     5. CapSense_P2_TX_PIN - The Tx or Lx signal is routed to the sensor
*        (available only if CSX or ISX sensing method is enabled).
*     6. CapSense_P2_RX_PIN - The pin is connected to the scanning bus
*        (available only if CSX or ISX sensing method is enabled).
*
*******************************************************************************/
void CapSense_P2_SetPinState(uint32 widgetId, uint32 sensorElement, uint32 state)
{
    uint32 eltdNum;
    uint32 eltdIndex;
    uint8  interruptState;
    CapSense_P2_FLASH_IO_STRUCT const *ioPtr;
    #if (CapSense_P2_ENABLE == CapSense_P2_GANGED_SNS_EN)
        CapSense_P2_FLASH_SNS_STRUCT const *curFlashSnsPtr;
    #endif

    /* Getting sensor element pointer and number of electrodes */
    #if (CapSense_P2_ENABLE == CapSense_P2_GANGED_SNS_EN)
        /* Check the ganged sns flag */
        if (CapSense_P2_GANGED_SNS_MASK == (CapSense_P2_dsFlash.wdgtArray[widgetId].staticConfig & CapSense_P2_GANGED_SNS_MASK))
        {
            curFlashSnsPtr = CapSense_P2_dsFlash.wdgtArray[widgetId].ptr2SnsFlash;
            curFlashSnsPtr += sensorElement;
            ioPtr = &CapSense_P2_ioList[curFlashSnsPtr->firstPinId];
            eltdNum = curFlashSnsPtr->numPins;
        }
        else
        {
            ioPtr = (CapSense_P2_FLASH_IO_STRUCT const *)CapSense_P2_dsFlash.wdgtArray[widgetId].ptr2SnsFlash + sensorElement;
            eltdNum = 1u;
        }
    #else
        ioPtr = (CapSense_P2_FLASH_IO_STRUCT const *)CapSense_P2_dsFlash.wdgtArray[widgetId].ptr2SnsFlash + sensorElement;
        eltdNum = 1u;
    #endif

    /* Loop through all electrodes of the specified sensor element */
    for (eltdIndex = 0u; eltdIndex < eltdNum; eltdIndex++)
    {
        /* Reset HSIOM and PC registers */
        interruptState = CyEnterCriticalSection();
        CY_SET_REG32(ioPtr->hsiomPtr, CY_GET_REG32(ioPtr->hsiomPtr) & ~(CapSense_P2_HSIOM_SEL_MASK << ioPtr->hsiomShift));
        CY_SET_REG32(ioPtr->pcPtr, CY_GET_REG32(ioPtr->pcPtr) & ~(CapSense_P2_GPIO_PC_MASK << ioPtr->shift));
        CyExitCriticalSection(interruptState);

        switch (state)
        {
        case CapSense_P2_GROUND:

            interruptState = CyEnterCriticalSection();
            CY_SET_REG32(ioPtr->pcPtr, CY_GET_REG32(ioPtr->pcPtr) | (CapSense_P2_SNS_GROUND_CONNECT << ioPtr->shift));
            CY_SET_REG32(ioPtr->drPtr, CY_GET_REG32(ioPtr->drPtr) & (uint32)(~(uint32)((uint32)1u << ioPtr->drShift)));
            CyExitCriticalSection(interruptState);
            break;

        case CapSense_P2_HIGHZ:
            interruptState = CyEnterCriticalSection();
            CY_SET_REG32(ioPtr->drPtr, CY_GET_REG32(ioPtr->drPtr) & (uint32)~(uint32)((uint32)1u << ioPtr->drShift));
            CyExitCriticalSection(interruptState);
            break;

        #if (CapSense_P2_ENABLE == CapSense_P2_CSD_EN)
            case CapSense_P2_SENSOR:
                /* Enable sensor */
                CapSense_P2_CSDConnectSns(ioPtr);
                break;

            #if (CapSense_P2_ENABLE == CapSense_P2_CSD_SHIELD_EN)
                case CapSense_P2_SHIELD:
                    /* Set appropriate bit in HSIOM register to configure pin to shield mode */
                    interruptState = CyEnterCriticalSection();
                    CY_SET_REG32(ioPtr->hsiomPtr, CY_GET_REG32(ioPtr->hsiomPtr) | (CapSense_P2_HSIOM_SEL_CSD_SHIELD << ioPtr->hsiomShift));
                    CyExitCriticalSection(interruptState);
                    break;
            #endif /* (CapSense_P2_ENABLE == CapSense_P2_CSD_SHIELD_EN) */
        #endif /* (CapSense_P2_ENABLE == CapSense_P2_CSD_EN) */

        #if ((CapSense_P2_ENABLE == CapSense_P2_CSX_EN) || \
             (CapSense_P2_ENABLE == CapSense_P2_ISX_EN))
            case CapSense_P2_TX_PIN:
                interruptState = CyEnterCriticalSection();
                CY_SET_REG32(ioPtr->hsiomPtr, CY_GET_REG32(ioPtr->hsiomPtr) | (CapSense_P2_HSIOM_SEL_CSD_SENSE << ioPtr->hsiomShift));
                CY_SET_REG32(ioPtr->pcPtr, CY_GET_REG32(ioPtr->pcPtr) | (CapSense_P2_GPIO_STRGDRV << ioPtr->shift));
                CyExitCriticalSection(interruptState);
                break;

            case CapSense_P2_RX_PIN:
                interruptState = CyEnterCriticalSection();
                CY_SET_REG32(ioPtr->hsiomPtr, CY_GET_REG32(ioPtr->hsiomPtr) | (CapSense_P2_HSIOM_SEL_AMUXA << ioPtr->hsiomShift));
                CY_SET_REG32(ioPtr->pcPtr, CY_GET_REG32(ioPtr->pcPtr) & ~(CapSense_P2_GPIO_PC_MASK << ioPtr->shift));
                CyExitCriticalSection(interruptState);
                break;
        #endif

        default:
            /* Wrong state */
            break;
        }

        ioPtr++;
    }
}

#if (CapSense_P2_ENABLE == CapSense_P2_CSD_EN)
    /*******************************************************************************
    * Function Name: CapSense_P2_SsCSDDisableMode
    ****************************************************************************//**
    *
    * \brief
    *  This function disables CSD mode.
    *
    * \details
    *  To disable CSD mode the following tasks are performed:
    *  1. Disconnect Cmod from AMUXBUS-A.
    *  2. Disconnect previous CSX electrode if it has been connected.
    *  3. Disconnect Csh from AMUXBUS-B.
    *  4. Disable Shield Electrodes.
    *
    *******************************************************************************/
    static void CapSense_P2_SsCSDDisableMode(void)
    {
        #if ((CapSense_P2_ENABLE == CapSense_P2_CSDV2) && \
             (CapSense_P2_ENABLE == CapSense_P2_CSD_IDAC_COMP_EN))

            uint32 newRegValue;

            /* Disconnect IDACA and IDACB */
            newRegValue = CY_GET_REG32(CapSense_P2_SW_REFGEN_SEL_PTR);
            newRegValue &= (uint32)(~CapSense_P2_SW_REFGEN_SEL_SW_IAIB_MASK);
            CY_SET_REG32(CapSense_P2_SW_REFGEN_SEL_PTR, newRegValue);
        #endif /* ((CapSense_P2_ENABLE == CapSense_P2_CSDV2) && \
                   (CapSense_P2_ENABLE == CapSense_P2_CSD_IDAC_COMP_EN)) */

        /* Disconnect Cmod from AMUXBUS-A using HSIOM registers */
        CapSense_P2_WriteBitsSafe(CapSense_P2_CMOD_HSIOM_PTR, CapSense_P2_CMOD_HSIOM_MASK, 0u);

        /* Disconnect previous CSD electrode if it has been connected */
        CapSense_P2_SsCSDElectrodeCheck();

        /* Disconnect Csh from AMUXBUS-B */
        #if ((CapSense_P2_ENABLE == CapSense_P2_CSD_SHIELD_EN) && \
             (CapSense_P2_ENABLE == CapSense_P2_CSD_SHIELD_TANK_EN))
            CapSense_P2_WriteBitsSafe(CapSense_P2_CSH_HSIOM_PTR, CapSense_P2_CSH_HSIOM_MASK, 0u);
        #endif

        #if ((CapSense_P2_ENABLE == CapSense_P2_CSD_SHIELD_EN) && \
             (0u != CapSense_P2_CSD_TOTAL_SHIELD_COUNT))
            CapSense_P2_SsCSDDisableShieldElectrodes();
        #endif
    }
#endif /* (CapSense_P2_ENABLE == CapSense_P2_CSD_EN) */


#if (CapSense_P2_ENABLE == CapSense_P2_CSX_EN)
    /*******************************************************************************
    * Function Name: CapSense_P2_SsDisableCSXMode
    ****************************************************************************//**
    *
    * \brief
    *  This function disables CSX mode.
    *
    * \details
    *  To disable CSX mode the following tasks are performed:
    *  1. Disconnect CintA and CintB from AMUXBUS-A.
    *  2. Disconnect previous CSX electrode if it has been connected.
    *
    *******************************************************************************/
    static void CapSense_P2_SsDisableCSXMode(void)
    {
        /* Disconnect CintA from AMUXBUS-A using HSIOM registers */
        CapSense_P2_WriteBitsSafe(CapSense_P2_CintA_HSIOM_PTR, CapSense_P2_CintA_HSIOM_MASK, 0u);
        /* Disconnect CintB from AMUXBUS-A using HSIOM registers */
        CapSense_P2_WriteBitsSafe(CapSense_P2_CintB_HSIOM_PTR, CapSense_P2_CintB_HSIOM_MASK, 0u);
        /* Disconnect previous CSX electrode if it has been connected */
        CapSense_P2_CSXElectrodeCheck();
    }
#endif /* (CapSense_P2_ENABLE == CapSense_P2_CSX_EN) */


#if (CapSense_P2_ENABLE == CapSense_P2_ISX_EN)
    /*******************************************************************************
    * Function Name: CapSense_P2_SsDisableISXMode
    ****************************************************************************//**
    *
    * \brief
    *  This function disables ISX mode.
    *
    * \details
    *  To disable ISX mode the following tasks are performed:
    *  1. Disconnect CintA and CintB from AMUXBUS-A
    *  2. Disconnect previous ISX electrode if it has been connected
    *
    *******************************************************************************/
    static void CapSense_P2_SsDisableISXMode(void)
    {
        /* Disconnect CintA from AMUXBUS-A using HSIOM registers */
        CapSense_P2_WriteBitsSafe(CapSense_P2_CintA_HSIOM_PTR, CapSense_P2_CintA_HSIOM_MASK, 0u);
        /* Disconnect CintB from AMUXBUS-A using HSIOM registers */
        CapSense_P2_WriteBitsSafe(CapSense_P2_CintB_HSIOM_PTR, CapSense_P2_CintB_HSIOM_MASK, 0u);
        /* Disconnect previous ISX electrode if it has been connected */
        CapSense_P2_ISXElectrodeCheck();
        CapSense_P2_DigPrt2_Stop();
        #if (CapSense_P2_USES_PORT_3)
            CapSense_P2_DigPrt3_Stop();
        #endif
    }
#endif /* (CapSense_P2_ENABLE == CapSense_P2_ISX_EN) */


/*******************************************************************************
* Function Name: CapSense_P2_SsSwitchSensingMode
****************************************************************************//**
*
* \brief
*  This function changes the Сomponent settings for operation in a specified
*  sensing mode.
*
* \details
*  This function performs following tasks:
*  1. Releasing the CSD HW block.
*  2. Disconnecting external capacitors.
*  3. Setting new mode.
*  4. Configuring HW to the new sensing mode.
*
* \param mode
*  Specifies the scan mode:
*  - CapSense_P2_SENSE_METHOD_CSD_E
*  - CapSense_P2_SENSE_METHOD_CSX_E
*  - CapSense_P2_SENSE_METHOD_ISX_E
*  - CapSense_P2_SENSE_METHOD_BIST_E
*  - CapSense_P2_UNDEFINED_E
*
*******************************************************************************/
void CapSense_P2_SsSwitchSensingMode(CapSense_P2_SENSE_METHOD_ENUM mode)
{
    if (CapSense_P2_currentSenseMethod != mode)
    {
        /* The requested mode differs to the current one. Disable the current mode */
        if (CapSense_P2_SENSE_METHOD_CSD_E == CapSense_P2_currentSenseMethod)
        {
            #if (CapSense_P2_ENABLE == CapSense_P2_CSD_EN)
                CapSense_P2_SsCSDDisableMode();
            #endif /* (CapSense_P2_ENABLE == CapSense_P2_CSD_EN) */
        }
        else if (CapSense_P2_SENSE_METHOD_CSX_E == CapSense_P2_currentSenseMethod)
        {
            #if (CapSense_P2_ENABLE == CapSense_P2_CSX_EN)
                CapSense_P2_SsDisableCSXMode();
            #endif /* (CapSense_P2_ENABLE == CapSense_P2_CSX_EN) */
        }
        else if (CapSense_P2_SENSE_METHOD_ISX_E == CapSense_P2_currentSenseMethod)
        {
            #if (CapSense_P2_ENABLE == CapSense_P2_ISX_EN)
                CapSense_P2_SsDisableISXMode();
            #endif /* (CapSense_P2_ENABLE == CapSense_P2_ISX_EN) */
        }
        else if (CapSense_P2_SENSE_METHOD_BIST_E == CapSense_P2_currentSenseMethod)
        {
            #if (CapSense_P2_ENABLE == CapSense_P2_SELF_TEST_EN)
                CapSense_P2_BistDisableMode();
            #endif /* (CapSense_P2_ENABLE == CapSense_P2_SELF_TEST_EN) */
        }
        else
        {
            #if (CapSense_P2_ENABLE == CapSense_P2_ADC_EN)
                /* Release ADC resources */
                (void)CapSense_P2_AdcReleaseResources();
            #endif /* (CapSense_P2_ENABLE == CapSense_P2_ADC_EN) */
        }

        /* Enable the specified mode */
        if (CapSense_P2_SENSE_METHOD_CSD_E == mode)
        {
            #if (CapSense_P2_ENABLE == CapSense_P2_CSD_EN)
                /* Initialize CSD mode to guarantee configured CSD mode */
                CapSense_P2_SsCSDInitialize();
                CapSense_P2_currentSenseMethod = CapSense_P2_SENSE_METHOD_CSD_E;
            #endif /* (CapSense_P2_ENABLE == CapSense_P2_CSD_EN) */
        }
        else if (CapSense_P2_SENSE_METHOD_CSX_E == mode)
        {
            #if (CapSense_P2_ENABLE == CapSense_P2_CSX_EN)
                /* Initialize CSX mode to guarantee configured CSX mode */
                CapSense_P2_CSXInitialize();
                CapSense_P2_currentSenseMethod = CapSense_P2_SENSE_METHOD_CSX_E;
            #endif /* (CapSense_P2_ENABLE == CapSense_P2_CSX_EN) */
        }
        else if (CapSense_P2_SENSE_METHOD_ISX_E == mode)
        {
            #if (CapSense_P2_ENABLE == CapSense_P2_ISX_EN)
                /* Initialize ISX mode to guarantee configured ISX mode */
                CapSense_P2_ISXInitialize();
                CapSense_P2_currentSenseMethod = CapSense_P2_SENSE_METHOD_ISX_E;
            #endif /* (CapSense_P2_ENABLE == CapSense_P2_ISX_EN) */
        }
        else if (CapSense_P2_SENSE_METHOD_BIST_E == mode)
        {
            #if (CapSense_P2_ENABLE == CapSense_P2_SELF_TEST_EN)
                CapSense_P2_BistInitialize();
                CapSense_P2_currentSenseMethod = CapSense_P2_SENSE_METHOD_BIST_E;
            #endif /* (CapSense_P2_ENABLE == CapSense_P2_SELF_TEST_EN) */
        }
        else
        {
            CapSense_P2_currentSenseMethod = CapSense_P2_UNDEFINED_E;
        }
    }
}


/*******************************************************************************
* Function Name: CapSense_P2_SsSetAllIOsState
****************************************************************************//**
*
* \brief
*  Sets all electrodes into a default state.
*
* \details
*  Sets all the CSD/CSX IOs into a default state:
*  - HSIOM   - Disconnected, the GPIO mode.
*  - DM      - Strong drive.
*  - State   - Zero.
*
*  Sets all the ADC channels into a default state:
*  - HSIOM   - Disconnected, the GPIO mode.
*  - DM      - HiZ-Analog.
*  - State   - Zero.
*
*  It is not recommended to call this function directly from the application
*  layer.
*
*******************************************************************************/
void CapSense_P2_SsSetAllIOsState(uint32 state)
{
    uint8  interruptState;

    uint32 loopIndex;

    uint32 newPinPcVal;
    uint32 newPinHsiomVal;

    uint32 tmpPcDrRegValue;
    uint32 tmpHsiomRegValue;

    CapSense_P2_FLASH_IO_STRUCT const *snsIoPtr;

    switch(state)
    {
        case CapSense_P2_SNS_CONNECTION_GROUND:
            newPinPcVal = CY_SYS_PINS_DM_STRONG;
            newPinHsiomVal = CapSense_P2_HSIOM_SEL_GPIO;
            break;
        case CapSense_P2_SNS_CONNECTION_HIGHZ:
            newPinPcVal = CY_SYS_PINS_DM_ALG_HIZ;
            newPinHsiomVal = CapSense_P2_HSIOM_SEL_GPIO;
            break;
        case CapSense_P2_SNS_CONNECTION_SHIELD:
            newPinPcVal = CY_SYS_PINS_DM_ALG_HIZ;
            newPinHsiomVal = CapSense_P2_HSIOM_SEL_CSD_SHIELD;
            break;
        default:
            newPinPcVal = CY_SYS_PINS_DM_STRONG;
            newPinHsiomVal = CapSense_P2_HSIOM_SEL_GPIO;
            break;
    }

    /* Loop through all electrodes */
    snsIoPtr = &CapSense_P2_ioList[0u];
    for (loopIndex = 0u; loopIndex < CapSense_P2_TOTAL_ELECTRODES; loopIndex++)
    {
        interruptState = CyEnterCriticalSection();

        tmpHsiomRegValue = CY_GET_REG32 (snsIoPtr->hsiomPtr);
        tmpHsiomRegValue &= ~(snsIoPtr->hsiomMask);
        CY_SET_REG32 (snsIoPtr->hsiomPtr, tmpHsiomRegValue);

        tmpPcDrRegValue = CY_GET_REG32 (snsIoPtr->pcPtr);
        tmpPcDrRegValue &= ~(CapSense_P2_GPIO_PC_MASK << snsIoPtr->shift);
        tmpPcDrRegValue |=  (newPinPcVal << snsIoPtr->shift);
        CY_SET_REG32 (snsIoPtr->pcPtr, tmpPcDrRegValue);

        tmpHsiomRegValue |=  (newPinHsiomVal << snsIoPtr->hsiomShift);
        CY_SET_REG32 (snsIoPtr->hsiomPtr, tmpHsiomRegValue);

        tmpPcDrRegValue = CY_GET_REG32 (snsIoPtr->drPtr);
        tmpPcDrRegValue &= ~(snsIoPtr->mask);
        CY_SET_REG32 (snsIoPtr->drPtr, tmpPcDrRegValue);

        CyExitCriticalSection(interruptState);

        /* Get next electrode */
        snsIoPtr++;
    }

    #if ((CapSense_P2_ENABLE == CapSense_P2_CSD_EN) &&\
         (CapSense_P2_ENABLE == CapSense_P2_CSD_SHIELD_EN) &&\
         (0u != CapSense_P2_CSD_TOTAL_SHIELD_COUNT))
        CapSense_P2_SsCSDSetShieldElectrodesState(state);
    #endif

    #if(CapSense_P2_ENABLE == CapSense_P2_ADC_EN)
        CapSense_P2_ClearAdcChannels();
    #endif

}

#if (CapSense_P2_ENABLE == CapSense_P2_ADC_EN)
/*******************************************************************************
* Function Name: CapSense_P2_SsReleaseResources()
****************************************************************************//**
*
* \brief
*  This function sets the resources that do not belong to the sensing HW block to
*  default state.
*
* \details
*  The function performs following tasks:
*  1. Checks if CSD block busy and returns error if it is busy
*  2. Disconnects integration capacitors (CintA, CintB for CSX mode and
*     Cmod for CSD mode)
*  3. Disconnect electroded if they have been connected.
*
* \return
*  Returns the status of the operation:
*  - Zero        - Resources released successfully.
*  - Non-zero    - One or more errors occurred in releasing process.
*
*******************************************************************************/
cystatus CapSense_P2_SsReleaseResources(void)
{
    cystatus busyStatus = CYRET_SUCCESS;

    if (CapSense_P2_SW_STS_BUSY == (CapSense_P2_dsRam.status & CapSense_P2_SW_STS_BUSY))
    {
        /* Previous widget is being scanned. Return error. */
        busyStatus = CYRET_INVALID_STATE;
    }
    else
    {
        #if (CapSense_P2_ENABLE == CapSense_P2_CSX_EN)
            CapSense_P2_SsDisableCSXMode();
        #endif /* (CapSense_P2_ENABLE == CapSense_P2_CSX_EN) */

        #if (CapSense_P2_ENABLE == CapSense_P2_ISX_EN)
            CapSense_P2_SsDisableISXMode();
        #endif /* (CapSense_P2_ENABLE == CapSense_P2_ISX_EN) */

        #if (CapSense_P2_ENABLE == CapSense_P2_CSD_EN)
            CapSense_P2_SsCSDDisableMode();
        #endif /* (CapSense_P2_ENABLE == CapSense_P2_CSD_EN) */

        #if (CapSense_P2_ENABLE == CapSense_P2_SELF_TEST_EN)
            CapSense_P2_BistDisableMode();
        #endif /* (CapSense_P2_ENABLE == CapSense_P2_SELF_TEST_EN) */

        CapSense_P2_SsSetAllIOsState((uint32)CapSense_P2_SNS_CONNECTION_GROUND);

        /*
         * Reset of the currentSenseMethod variable to make sure that the next
         * call of SetupWidget() API setups the correct widget mode
         */
        CapSense_P2_currentSenseMethod = CapSense_P2_UNDEFINED_E;
    }

    return busyStatus;
}
#endif /* (CapSense_P2_ENABLE == CapSense_P2_ADC_EN) */


/*******************************************************************************
* Function Name: CapSense_P2_SsPostAllWidgetsScan
****************************************************************************//**
*
* \brief
*  The ISR function for multiple widget scanning implementation.
*
* \details
*  This is the function used by the CapSense_P2 ISR to implement multiple widget
*  scanning.
*  Should not be used by the application layer.
*
*******************************************************************************/
void CapSense_P2_SsPostAllWidgetsScan(void)
{
    /*
    *  1. Increment widget index
    *  2. Check if all the widgets are scanned
    *  3. If all the widgets are not scanned, set up and scan next widget
    */
    #if (1u != CapSense_P2_TOTAL_WIDGETS)
        cystatus postScanStatus;

        do
        {
            CapSense_P2_widgetIndex++;

            postScanStatus = CapSense_P2_SetupWidget((uint32)CapSense_P2_widgetIndex);

            if (CYRET_SUCCESS == postScanStatus)
            {
                if((CapSense_P2_TOTAL_WIDGETS - 1u) == CapSense_P2_widgetIndex)
                {
                    /* The last widget will be scanned. Reset flag to skip configuring the next widget setup in ISR. */
                    CapSense_P2_requestScanAllWidget = CapSense_P2_DISABLE;
                }
                (void)CapSense_P2_Scan();
            }
            else if((CapSense_P2_TOTAL_WIDGETS - 1u) == CapSense_P2_widgetIndex)
            {
                #if ((CapSense_P2_ENABLE == CapSense_P2_BLOCK_OFF_AFTER_SCAN_EN) && \
                     (CapSense_P2_ENABLE == CapSense_P2_CSD_EN))
                    if (CapSense_P2_SENSE_METHOD_CSD_E ==
                        CapSense_P2_GET_SENSE_METHOD(&CapSense_P2_dsFlash.wdgtArray[CapSense_P2_widgetIndex]))
                    {
                        /* Disable the CSD block */
                        CY_SET_REG32(CapSense_P2_CONFIG_PTR, CapSense_P2_configCsd);
                    }
                #endif /* ((CapSense_P2_ENABLE == CapSense_P2_BLOCK_OFF_AFTER_SCAN_EN) && \
                           (CapSense_P2_ENABLE == CapSense_P2_CSD_EN)) */

                /* Update scan Counter */
                CapSense_P2_dsRam.scanCounter++;
                /* All widgets are totally processed. Reset BUSY flag */
                CapSense_P2_dsRam.status &= ~CapSense_P2_SW_STS_BUSY;

                /* Update status with with the failure */
                CapSense_P2_dsRam.status &= ~CapSense_P2_STATUS_ERR_MASK;
                CapSense_P2_dsRam.status |= ((postScanStatus & CapSense_P2_STATUS_ERR_SIZE) << CapSense_P2_STATUS_ERR_SHIFT);

                /* Set postScanStatus to exit the while loop */
                postScanStatus = CYRET_SUCCESS;
            }
            else
            {
                /* Update status with with the failure. Configure the next widget in while() loop */
                CapSense_P2_dsRam.status &= ~CapSense_P2_STATUS_ERR_MASK;
                CapSense_P2_dsRam.status |= ((postScanStatus & CapSense_P2_STATUS_ERR_SIZE) << CapSense_P2_STATUS_ERR_SHIFT);
            }
        } while (CYRET_SUCCESS != postScanStatus);
    #endif /* (1u != CapSense_P2_TOTAL_WIDGETS) */
}


/*******************************************************************************
* Function Name: CapSense_P2_SsIsrInitialize
****************************************************************************//**
*
* \brief
*  Enables and initializes for the function pointer for a callback for the ISR.
*
* \details
*  The "address" is a special type cyisraddress defined by Cylib. This function
*  is used by Component APIs and should not be used by an application program for
*  proper working of the Component.
*
* \param address
*  The address of the function to be called when interrupt is fired.
*
*******************************************************************************/
void CapSense_P2_SsIsrInitialize(cyisraddress address)
{
    CapSense_P2_ISR_StartEx(address);
}


/*******************************************************************************
* Function Name: CapSense_P2_SsSetModClkClockDivider
****************************************************************************//**
*
* \brief
*  Sets the divider values for the modulator clock and then starts
*  the modulator clock.
*
* \details
*  It is not recommended to call this function directly by the application layer.
*  It is used by initialization, widget APIs or wakeup functions to
*  enable the clocks.
*
* \param modClk
*  The divider value for the modulator clock.
*
*******************************************************************************/
void CapSense_P2_SsSetModClkClockDivider(uint32 modClk)
{
#if (CapSense_P2_ENABLE == CapSense_P2_IS_M0S8PERI_BLOCK)
    /* Stop modulator clock */
    CY_SET_REG32(CapSense_P2_MODCLK_CMD_PTR,
                 ((uint32)CapSense_P2_ModClk__DIV_ID <<
                 CapSense_P2_MODCLK_CMD_DIV_SHIFT)|
                 ((uint32)CapSense_P2_MODCLK_CMD_DISABLE_MASK));

    /*
     * Set divider value for sense modClk.
     * 1u is subtracted from modClk because Divider register value 0 corresponds
     * to dividing by 1.
     */
    CY_SET_REG32(CapSense_P2_MODCLK_DIV_PTR, ((modClk - 1u) << 8u));

    /* Check whether previous modulator clock start command has finished. */
    while(0u != (CY_GET_REG32(CapSense_P2_MODCLK_CMD_PTR) & CapSense_P2_MODCLK_CMD_ENABLE_MASK))
    {
        /* Wait until previous modulator clock start command has finished */
    }

    /* Start modulator clock, aligned to HFCLK */
    CY_SET_REG32(CapSense_P2_MODCLK_CMD_PTR,
                 (uint32)(((uint32)CapSense_P2_ModClk__DIV_ID << CapSense_P2_MODCLK_CMD_DIV_SHIFT) |
                  ((uint32)CapSense_P2_ModClk__PA_DIV_ID << CapSense_P2_MODCLK_CMD_PA_DIV_SHIFT) |
                  CapSense_P2_MODCLK_CMD_ENABLE_MASK));
#else
    uint32 newRegValue;

    /* Clear bit to disable ModClk clock. */
    CY_SET_REG32(CapSense_P2_MODCLK_CMD_PTR,
                 CY_GET_REG32(CapSense_P2_MODCLK_CMD_PTR) &
                 (uint32)(~CapSense_P2_ModClk__ENABLE_MASK));

    /*
     * Update ModClk clock divider.
     * 1u is subtracted from modClk because Divider register value has range (0-65535).
     */
    newRegValue = CY_GET_REG32(CapSense_P2_MODCLK_DIV_PTR) & (uint32)(~(uint32)CapSense_P2_ModClk__DIVIDER_MASK);
    newRegValue |= (modClk - 1u);
    CY_SET_REG32(CapSense_P2_MODCLK_DIV_PTR, newRegValue);

    /* Set bit to enable ModClk clock. */
    CY_SET_REG32(CapSense_P2_MODCLK_CMD_PTR,
                 CY_GET_REG32(CapSense_P2_MODCLK_CMD_PTR) |
                CapSense_P2_ModClk__ENABLE_MASK);
#endif /* (CapSense_P2_ENABLE == CapSense_P2_IS_M0S8PERI_BLOCK) */
}


/*******************************************************************************
* Function Name: CapSense_P2_SsSetSnsClockDivider
****************************************************************************//**
*
* \brief
*  Sets the divider values for the sense clock and then starts
*  the sense clock.
*
* \details
*  It is not recommended to call this function directly by the application layer.
*  It is used by initialization, widget APIs or wakeup functions to
*  enable the clocks.
*
* \param
*  snsClk The divider value for the sense clock.
*
*******************************************************************************/
void CapSense_P2_SsSetSnsClockDivider(uint32 snsClk)
{
#if (CapSense_P2_ENABLE == CapSense_P2_CSDV2)
    uint32 newRegValue;

    /*
     * Set divider value for sense clock.
     * 1u is subtracted from snsClk because SENSE_DIV value 0 corresponds
     * to dividing by 1.
     */
    newRegValue = CY_GET_REG32(CapSense_P2_SENSE_PERIOD_PTR);
    newRegValue &= (uint32)(~CapSense_P2_SENSE_PERIOD_SENSE_DIV_MASK);
    newRegValue |= snsClk - 1u;
    CY_SET_REG32(CapSense_P2_SENSE_PERIOD_PTR, newRegValue);
#else
    #if (CapSense_P2_ENABLE == CapSense_P2_IS_M0S8PERI_BLOCK)
        /* Stop sense clock clock   */
        CY_SET_REG32(CapSense_P2_SNSCLK_CMD_PTR,
                     ((uint32)CapSense_P2_SnsClk__DIV_ID <<
                     CapSense_P2_SNSCLK_CMD_DIV_SHIFT)|
                     ((uint32)CapSense_P2_SNSCLK_CMD_DISABLE_MASK));

        /*
         * Set divider value for sense clock.
         * 1u is subtracted from snsClk because Divider register value 0 corresponds
         * to dividing by 1.
         */
        CY_SET_REG32(CapSense_P2_SNSCLK_DIV_PTR, ((snsClk - 1u) << 8u));

        /* Check whether previous sense clock start command has finished. */
        while(0u != (CY_GET_REG32(CapSense_P2_SNSCLK_CMD_PTR) & CapSense_P2_SNSCLK_CMD_ENABLE_MASK))
        {
            /* Wait until previous sense clock start command has finished. */
        }

        /* Start sense clock aligned to previously started modulator clock. */
        CY_SET_REG32(CapSense_P2_SNSCLK_CMD_PTR,
                     (uint32)(((uint32)CapSense_P2_SnsClk__DIV_ID << CapSense_P2_SNSCLK_CMD_DIV_SHIFT) |
                      ((uint32)CapSense_P2_ModClk__PA_DIV_ID << CapSense_P2_SNSCLK_CMD_PA_DIV_SHIFT) |
                      CapSense_P2_SNSCLK_CMD_ENABLE_MASK));
    #else
        uint32 newRegValue;

        /* Clear bit to disable SnsClk clock. */
        CY_SET_REG32(CapSense_P2_SNSCLK_CMD_PTR,
                     CY_GET_REG32(CapSense_P2_SNSCLK_CMD_PTR) &
                     (uint32)(~(uint32)CapSense_P2_SnsClk__ENABLE_MASK));

        /*
         * Update snsClk clock divider.
         * 1u is subtracted from snsClk because Divider register value has range (0-65535).
         */
        newRegValue = CY_GET_REG32(CapSense_P2_SNSCLK_DIV_PTR) & (uint32)(~(uint32)CapSense_P2_SnsClk__DIVIDER_MASK);
        newRegValue |= (snsClk - 1u);
        CY_SET_REG32(CapSense_P2_SNSCLK_DIV_PTR, newRegValue);

        /* Set bit to enable SnsClk clock. */
        CY_SET_REG32(CapSense_P2_SNSCLK_CMD_PTR,
                     CY_GET_REG32(CapSense_P2_SNSCLK_CMD_PTR) |
                    CapSense_P2_SnsClk__ENABLE_MASK);
    #endif /* (CapSense_P2_ENABLE == CapSense_P2_IS_M0S8PERI_BLOCK) */
#endif /* (CapSense_P2_ENABLE == CapSense_P2_CSDV2) */
}


/*******************************************************************************
* Function Name: CapSense_P2_SsSetClockDividers
****************************************************************************//**
*
* \brief
*  Sets the divider values for sense and modulator clocks and then starts
*  a modulator clock-phase aligned to HFCLK and sense clock-phase aligned to
*  the modulator clock.
*
* \details
*  It is not recommended to call this function directly by the application layer.
*  It is used by initialization, widget APIs or wakeup functions to
*  enable the clocks.
*
* \param
*  snsClk The divider value for the sense clock.
*  modClk The divider value for the modulator clock.
*
*******************************************************************************/
void CapSense_P2_SsSetClockDividers(uint32 snsClk, uint32 modClk)
{
    /* Configure Mod clock */
    CapSense_P2_SsSetModClkClockDivider(modClk);

    /* Configure Sns clock */
    CapSense_P2_SsSetSnsClockDivider(snsClk);
}


#if (CapSense_P2_ANYMODE_AUTOCAL)
    /*******************************************************************************
    * Function Name: CapSense_P2_CalibrateWidget
    ****************************************************************************//**
    *
    * \brief
    *  Calibrates the IDACs for all the sensors in the specified widget to the default
    *  target, this function detects the sensing method used by the
    *  widget prior to calibration.
    *
    * \details
    *  This function performs exactly the same tasks as
    *  CapSense_P2_CalibrateAllWidgets, but only for a specified widget.
    *  This function detects the sensing method used by the widgets and uses
    *  the Enable compensation IDAC parameter.
    *  \if SECTION_I_REP
    *  For ISX mode, the frequency is also calibrated.
    *  \endif
    *
    *  \if SECTION_C_LOW_LEVEL
    *  This function is available when the CSD and/or CSX Enable IDAC
    *  auto-calibration parameter is enabled.
    *  \endif
    *  \if SECTION_I_REP
    *  This function is available when the ISX Enable auto-calibration parameter
    *  is enabled.
    *  \endif
    *
    * \param widgetId
    *  Specifies the ID number of the widget to calibrate its raw count.
    *  A macro for the widget ID can be found in the
    *  CapSense_P2 Configuration header file defined as
    *  CapSense_P2_<WidgetName>_WDGT_ID.
    *
    * \return
    *  Returns the status of the specified widget calibration:
    *  - CYRET_SUCCESS - The operation is successfully completed.
    *  - CYRET_BAD_PARAM - The input parameter is invalid.
    *  - CYRET_BAD_DATA - The calibration failed and the Component may not
    *    operate as expected.
    *
    *******************************************************************************/
    cystatus CapSense_P2_CalibrateWidget(uint32 widgetId)
    {
        cystatus calibrateStatus = CYRET_SUCCESS;

        do
        {
            if (CapSense_P2_TOTAL_WIDGETS <= widgetId)
            {
                calibrateStatus = CYRET_BAD_PARAM;
            }

            /*
             *  Check if widget id is valid, specified widget did not
             *  detect any faults conditions if BIST is enabled.
             */
            #if (CapSense_P2_ENABLE == CapSense_P2_SELF_TEST_EN)
                if (0u != CapSense_P2_GET_WIDGET_EN_STATUS(widgetId))
                {
                    calibrateStatus = CYRET_SUCCESS;
                }
                else
                {
                    calibrateStatus = CYRET_INVALID_STATE;
                }
            #endif  /* (CapSense_P2_ENABLE == CapSense_P2_SELF_TEST_EN) */

            if (CYRET_SUCCESS != calibrateStatus)
            {
                /* Exit from the loop because of a fail */
                break;
            }

            /* If both CSD and CSX are enabled, calibrate widget using sensing method */
            #if (CapSense_P2_ENABLE == CapSense_P2_MANY_WIDGET_METHODS_EN)
                /* Check widget sensing method and call appropriate APIs */
                #if (CapSense_P2_ENABLE == CapSense_P2_ISX_IDAC_AUTOCAL_EN)
                    if (CapSense_P2_SENSE_METHOD_ISX_E ==
                        CapSense_P2_GET_SENSE_METHOD(&CapSense_P2_dsFlash.wdgtArray[widgetId]))
                    {
                        /* Calibrate CSX widget */
                       CapSense_P2_ISXCalibrateWidget(widgetId, CapSense_P2_ISX_RAWCOUNT_CAL_LEVEL);
                    }
                #endif  /* (CapSense_P2_ENABLE == CapSense_P2_ISX_IDAC_AUTOCAL_EN) */

                /* Check widget sensing method and call appropriate APIs */
                #if (CapSense_P2_ENABLE == CapSense_P2_CSX_IDAC_AUTOCAL_EN)
                    if (CapSense_P2_SENSE_METHOD_CSX_E ==
                        CapSense_P2_GET_SENSE_METHOD(&CapSense_P2_dsFlash.wdgtArray[widgetId]))
                    {
                        /* Calibrate CSX widget */
                       calibrateStatus = CapSense_P2_CSXCalibrateWidget(widgetId, CapSense_P2_CSX_RAWCOUNT_CAL_LEVEL);
                    }
                #endif  /* (CapSense_P2_ENABLE == CapSense_P2_CSX_IDAC_AUTOCAL_EN) */

                #if (CapSense_P2_ENABLE == CapSense_P2_CSD_IDAC_AUTOCAL_EN)
                    if (CapSense_P2_SENSE_METHOD_CSD_E ==
                        CapSense_P2_GET_SENSE_METHOD(&CapSense_P2_dsFlash.wdgtArray[widgetId]))
                    {
                        /* Calibrate CSD widget */
                        calibrateStatus = CapSense_P2_CSDCalibrateWidget(widgetId, CapSense_P2_CSD_RAWCOUNT_CAL_LEVEL);
                    }
                #endif  /* (CapSense_P2_ENABLE == CapSense_P2_CSD_IDAC_AUTOCAL_EN) */

            /* If only CSD is enabled, calibrate CSD sensor */
            #elif (CapSense_P2_ENABLE == CapSense_P2_CSD_EN)
                calibrateStatus = CapSense_P2_CSDCalibrateWidget(widgetId, CapSense_P2_CSD_RAWCOUNT_CAL_LEVEL);

            /* If only CSX is enabled, call CSX scan */
            #elif (CapSense_P2_ENABLE == CapSense_P2_CSX_EN)
                calibrateStatus = CapSense_P2_CSXCalibrateWidget(widgetId, CapSense_P2_CSX_RAWCOUNT_CAL_LEVEL);

            /* If only ISX is enabled, call ISX scan */
            #elif (CapSense_P2_ENABLE == CapSense_P2_ISX_EN)
                CapSense_P2_ISXCalibrateWidget(widgetId, CapSense_P2_ISX_RAWCOUNT_CAL_LEVEL);

            #else
                calibrateStatus = CYRET_UNKNOWN;
            #endif
        } while (0u);

        return calibrateStatus;
    }


    /*******************************************************************************
    * Function Name: CapSense_P2_CalibrateAllWidgets
    ****************************************************************************//**
    *
    * \brief
    *  Calibrates the IDACs for all the widgets in the Component to the default
    *  target, this function detects the sensing method used by the widgets
    *  prior to calibration.
    *
    * \details
    *  Calibrates the IDACs for all the widgets in the Component to the default
    *  target value. This function detects the sensing method used by the widgets
    *  and regards the Enable compensation IDAC parameter.
    *  \if SECTION_I_REP
    *  For ISX mode, the frequency is also calibrated.
    *  \endif
    *
    *  \cond SECTION_C_LOW_LEVEL
    *  This function is available when the CSD and/or CSX Enable IDAC
    *  auto-calibration parameter is enabled.
    *  \endcond
    *  \if SECTION_I_REP
    *  This function is available when the ISX Enable Auto-calibration parameter
    *  is enabled.
    *  \endif
    *
    * \return
    *  Returns the status of the calibration process:
    *  - CYRET_SUCCESS - The operation is successfully completed.
    *  - CYRET_BAD_DATA - The calibration failed and the Component may not
    *    operate as expected.
    *
    *******************************************************************************/
    cystatus CapSense_P2_CalibrateAllWidgets(void)
    {
        cystatus calibrateStatus = CYRET_SUCCESS;
        uint32 wdgtIndex;

        for (wdgtIndex = 0u; wdgtIndex < CapSense_P2_TOTAL_WIDGETS; wdgtIndex++)
        {
            calibrateStatus |= CapSense_P2_CalibrateWidget(wdgtIndex);
        }

        return calibrateStatus;
    }
#endif /* (CapSense_P2_ANYMODE_AUTOCAL) */


#if (CapSense_P2_CSD_SS_DIS != CapSense_P2_CSD_AUTOTUNE)
    /*******************************************************************************
    * Function Name: CapSense_P2_SsAutoTuneWidget
    ****************************************************************************//**
    *
    * \brief
    *  This API performs the parameters auto-tuning for the optimal CapSense_P2
    *  operation for a desired widget.
    *
    * \details
    *  This API performs the following:
    *  - Calibrates Modulator and Compensation IDACs.
    *  - Tunes the Sense Clock optimal value to get a Sense Clock period greater than
    *    2*5*R*Cp.
    *  - Calculates the resolution for the optimal finger capacitance.
    *
    * \param widgetId
    *  Specifies the ID number of a desired widget.
    *  A macro for the widget ID can be found in the
    *  CapSense_P2 Configuration header file defined as
    *  CapSense_P2_<WidgetName>_WDGT_ID.
    *
    * \return
    *  Returns the status of the operation:
    *  - Zero     - All the widgets are auto-tuned successfully.
    *  - Non-zero - Auto-tuning failed for any widget.
    *
    *******************************************************************************/
    cystatus CapSense_P2_SsAutoTuneWidget(uint32 widgetId)
    {
        cystatus autoTuneStatus = CYRET_SUCCESS;

        uint32 snsIndex;
        uint32 inputSnsClk;
        uint32 minSnsClkDiv;
        uint32 minSnsClkDivTmp;

        AUTO_TUNE_CONFIG_TYPE autoTuneConfig;

        uint32 maxRaw = 0u;
        uint32 maxCp = 0u;
        uint32 maxIdacComp = 0u;

        #if (CapSense_P2_CSD_MATRIX_WIDGET_EN || CapSense_P2_CSD_TOUCHPAD_WIDGET_EN)
            uint32 maxCpSnsId = 0u;
            uint32 maxCpRowSnsId = 0u;
        #endif

        CapSense_P2_FLASH_WD_STRUCT const *ptrFlashWidget = &CapSense_P2_dsFlash.wdgtArray[widgetId];
        CapSense_P2_RAM_WD_BASE_STRUCT *ptrRamWidget = (CapSense_P2_RAM_WD_BASE_STRUCT *)ptrFlashWidget->ptr2WdgtRam;
        CapSense_P2_RAM_SNS_STRUCT *ptrRamSensor = ptrFlashWidget->ptr2SnsRam;

        /* Find input of sensor clock frequency */
        #if (CapSense_P2_ENABLE == CapSense_P2_CSDV2)
            /* Dividers are chained (Fourth-generation HW block). Flip flop is not available */
            inputSnsClk = ((uint32)CYDEV_BCLK__HFCLK__KHZ / (uint32)CapSense_P2_dsRam.modCsdClk);

            /* Minimum value of SnaClk divider that is allowed for all Fourth-generation
             * CSD HW block based devices.
            */
            minSnsClkDiv = CapSense_P2_MIN_SNS_CLK_DIVIDER;
        #elif (CapSense_P2_ENABLE == CapSense_P2_IS_M0S8PERI_BLOCK)
            /* Dividers are not chained */
            inputSnsClk = CYDEV_BCLK__HFCLK__KHZ >> CapSense_P2_FLIP_FLOP_DIV;

            /* Minimum value of SnaClk divider that is allowed for PSoC 4100 and PSoC 4200
               device families.
            */
            minSnsClkDiv = CapSense_P2_dsRam.modCsdClk;
        #else
            /* Dividers are chained (PSoC 4100, PSoC 4200) */
            inputSnsClk = (CYDEV_BCLK__HFCLK__KHZ / CapSense_P2_dsRam.modCsdClk) >> CapSense_P2_FLIP_FLOP_DIV;

            /* Minimum value of SnaClk divider that is allowed for all Fourth-generation
             * CSD HW block based device except PSoC 4100 and PSoC 4200 device families.
            */
            minSnsClkDiv = CapSense_P2_MIN_SNS_CLK_DIVIDER;
        #endif /* (CapSense_P2_ENABLE == CapSense_P2_CSDV2) */

        /* Minimum value of SnsClk divider required to ensure the CSD HW block
         * operation in allowed frequency range.
        */
        minSnsClkDivTmp = inputSnsClk * CapSense_P2_AUTOTUNE_CAL_UNITS;
        minSnsClkDivTmp /= CapSense_P2_CSD_SNS_FREQ_KHZ_MAX;

        if (minSnsClkDiv < minSnsClkDivTmp)
        {
            minSnsClkDiv = minSnsClkDivTmp;
        }

        minSnsClkDiv = (minSnsClkDiv + (CapSense_P2_AUTOTUNE_CAL_UNITS - 1u)) / CapSense_P2_AUTOTUNE_CAL_UNITS;

        /* Make divider even */
        if (0u != (minSnsClkDiv & 0x01u))
        {
            minSnsClkDiv++;
        }

        /* Initialize auto-tuning configuration structure */
        autoTuneConfig.snsClkInputClock = (uint16)inputSnsClk;
        autoTuneConfig.snsClkConstantR = CapSense_P2_CSD_SNSCLK_R_CONST;
        autoTuneConfig.vRef = CapSense_P2_CSD_VREF_MV;
        autoTuneConfig.fingerCap = ptrRamWidget->fingerCap;
        autoTuneConfig.sigPFC = &ptrRamWidget->sigPFC;

        /*
        * Autotune phase #1:
        * - performing the first calibration at fixed settings
        * - getting maximum sensor Cp
        * - getting sense clock frequency based on Cp
        */

        ptrRamWidget->resolution = CapSense_P2_AUTOTUNE_CALIBRATION_RESOLUTION;

        ptrRamWidget->snsClk = (uint16)(inputSnsClk / CapSense_P2_AUTOTUNE_CALIBRATION_FREQ_KHZ);
        ptrRamWidget->snsClkSource = (uint8)CapSense_P2_CLK_SOURCE_DIRECT;

        #if (CapSense_P2_CSD_MATRIX_WIDGET_EN || CapSense_P2_CSD_TOUCHPAD_WIDGET_EN)
            ptrRamWidget->rowSnsClk = (uint16)(inputSnsClk / CapSense_P2_AUTOTUNE_CALIBRATION_FREQ_KHZ);
            ptrRamWidget->rowSnsClkSource = (uint8)CapSense_P2_CLK_SOURCE_DIRECT;
        #endif

        /* Calibrate CSD widget to the default calibration target */
        (void)CapSense_P2_CSDCalibrateWidget(widgetId, CapSense_P2_AUTOTUNE_CAL_LEVEL);

        /* Find raw count and IDAC of a sensor with maximum Cp */
        for (snsIndex = 0u; snsIndex < ptrFlashWidget->numCols; snsIndex++)
        {
            #if (CapSense_P2_ENABLE == CapSense_P2_CSD_IDAC_COMP_EN)
                if (maxIdacComp < ptrRamSensor->idacComp[0u])
                {
                    #if (CapSense_P2_CSD_MATRIX_WIDGET_EN || CapSense_P2_CSD_TOUCHPAD_WIDGET_EN)
                        maxCpSnsId = snsIndex;
                    #endif
                    maxIdacComp = ptrRamSensor->idacComp[0u];
                    maxRaw = ptrRamSensor->raw[0u];
                }
            #else
                if (maxRaw < ptrRamSensor->raw[0u])
                {
                    #if (CapSense_P2_CSD_MATRIX_WIDGET_EN || CapSense_P2_CSD_TOUCHPAD_WIDGET_EN)
                        maxCpSnsId = snsIndex;
                    #endif
                    maxRaw = ptrRamSensor->raw[0u];
                }
            #endif
            ptrRamSensor++;
        }

        /* Update auto-tuning configuration structure */
        autoTuneConfig.iDacMod = ptrRamWidget->idacMod[0u];
        autoTuneConfig.iDacComp = (uint8)maxIdacComp;
        autoTuneConfig.iDacGain = CapSense_P2_idacGainTable[ptrRamWidget->idacGainIndex].gainValue;
        autoTuneConfig.ptrSenseClk = &ptrRamWidget->snsClk;
        autoTuneConfig.calTarget = (uint16)((maxRaw * CapSense_P2_AUTOTUNE_CAL_UNITS) /
                ((uint32)(0x01uL << CapSense_P2_AUTOTUNE_CALIBRATION_RESOLUTION) - 1u));

        /* Find sensor Cp and optimum sense clock value */
        maxCp = SmartSense_TunePrescalers(&autoTuneConfig);
        autoTuneConfig.sensorCap = maxCp;

        /* Increase sensor clock divider to valid value */
        while(ptrRamWidget->snsClk < minSnsClkDiv)
        {
            ptrRamWidget->snsClk <<= 1u;
        }

        /* Do the same for row for touchpad or matrix buttons widgets */
        #if (CapSense_P2_CSD_MATRIX_WIDGET_EN || CapSense_P2_CSD_TOUCHPAD_WIDGET_EN)
            if ((CapSense_P2_WD_TOUCHPAD_E == CapSense_P2_GET_WIDGET_TYPE(ptrFlashWidget)) ||
                (CapSense_P2_WD_MATRIX_BUTTON_E == CapSense_P2_GET_WIDGET_TYPE(ptrFlashWidget)))
            {
                /* Find a sensor with maximum Cp */
                maxRaw = 0u;
                maxIdacComp = 0u;
                for (snsIndex = ptrFlashWidget->numCols; snsIndex < ptrFlashWidget->totalNumSns; snsIndex++)
                {
                    #if (CapSense_P2_ENABLE == CapSense_P2_CSD_IDAC_COMP_EN)
                        if (maxIdacComp < ptrRamSensor->idacComp[0u])
                        {
                            maxCpRowSnsId = snsIndex;
                            maxIdacComp = ptrRamSensor->idacComp[0u];
                            maxRaw = ptrRamSensor->raw[0u];
                        }
                    #else
                        if (maxRaw < ptrRamSensor->raw[0u])
                        {
                            maxCpRowSnsId = snsIndex;
                            maxRaw = ptrRamSensor->raw[0u];
                        }
                    #endif
                    ptrRamSensor++;
                }

                /* Configure auto-tuning configuration structure */
                autoTuneConfig.iDacMod = ptrRamWidget->rowIdacMod[0u];
                autoTuneConfig.iDacComp = (uint8)maxIdacComp;
                autoTuneConfig.ptrSenseClk = &ptrRamWidget->rowSnsClk;
                autoTuneConfig.calTarget = (uint16)((maxRaw * CapSense_P2_AUTOTUNE_CAL_UNITS) /
                        ((uint32)(0x01uL << CapSense_P2_AUTOTUNE_CALIBRATION_RESOLUTION) - 1u));

                /* Find correct sense clock value */
                maxCp = SmartSense_TunePrescalers(&autoTuneConfig);

                /* Save maximum sensor Cp and corresponding sensor Id */
                if (autoTuneConfig.sensorCap < maxCp)
                {
                    autoTuneConfig.sensorCap = maxCp;
                    maxCpSnsId = maxCpRowSnsId;
                }

                /* Increase sensor clock divider to valid value */
                while(ptrRamWidget->rowSnsClk < minSnsClkDiv)
                {
                    ptrRamWidget->rowSnsClk <<= 1u;
                }
            }
        #endif /* (CapSense_P2_CSD_MATRIX_WIDGET_EN || CapSense_P2_CSD_TOUCHPAD_WIDGET_EN) */

        /* Check tuning results */
        if (CapSense_P2_AUTOTUNE_CP_MAX < maxCp)
        {
            autoTuneStatus = CYRET_BAD_DATA;
        }

        /*
        * Autotune phase #2:
        * - repeating calibration with new sense clock frequency
        * - getting resolution
        */

        /* Calibrate CSD widget to the default calibration target with newly defined sense frequency */
        (void)CapSense_P2_CSDCalibrateWidget(widgetId, CapSense_P2_AUTOTUNE_CAL_LEVEL);

        autoTuneConfig.iDacGain = CapSense_P2_idacGainTable[ptrRamWidget->idacGainIndex].gainValue;
        autoTuneConfig.iDacMod = ptrRamWidget->idacMod[0u];
        autoTuneConfig.ptrSenseClk = &ptrRamWidget->snsClk;

        #if (CapSense_P2_CSD_MATRIX_WIDGET_EN || CapSense_P2_CSD_TOUCHPAD_WIDGET_EN)
            if ((CapSense_P2_WD_TOUCHPAD_E == CapSense_P2_GET_WIDGET_TYPE(ptrFlashWidget)) ||
                (CapSense_P2_WD_MATRIX_BUTTON_E == CapSense_P2_GET_WIDGET_TYPE(ptrFlashWidget)))
            {
                if (maxCpSnsId >= ptrFlashWidget->numCols)
                {
                    autoTuneConfig.iDacMod = ptrRamWidget->rowIdacMod[0u];
                    autoTuneConfig.ptrSenseClk = &ptrRamWidget->rowSnsClk;
                }
            }
        #endif

        /* Find resolution */
        ptrRamWidget->resolution = SmartSense_TuneSensitivity(&autoTuneConfig);

        /*
        * Autotune phase #3:
        * - selecting a widget clock source if AUTO
        * - repeating calibration with found clock frequency, resolution and clock source
        * - updating sensitivity
        */

        /* Tune widget sensitivity */
        CapSense_P2_SsSetWidgetSenseClkSrc(widgetId, ptrRamWidget);

        /* Calibrate CSD widget to the default calibration target with newly defined resolution */
        autoTuneStatus |= CapSense_P2_CSDCalibrateWidget(widgetId, CapSense_P2_AUTOTUNE_CAL_LEVEL);

        #if (CapSense_P2_ENABLE == CapSense_P2_TST_WDGT_CRC_EN)
            CapSense_P2_DsUpdateWidgetCrc(widgetId);
        #endif /* (CapSense_P2_ENABLE == CapSense_P2_TST_WDGT_CRC_EN) */

        return autoTuneStatus;
    }


    /*******************************************************************************
    * Function Name: CapSense_P2_SsAutoTune
    ****************************************************************************//**
    *
    * \brief
    *  This API performs the parameters auto-tuning for the optimal CapSense_P2
    *  operation.
    *
    * \details
    *  This API performs the following:
    *  - Calibrates Modulator and Compensation IDACs.
    *  - Tunes the Sense Clock optimal value to get a Sense Clock period greater than
    *    2*5*R*Cp.
    *  - Calculates the resolution for the optimal finger capacitance.
    *
    * \return
    *  Returns the status of the operation:
    *  - Zero     - All the widgets are auto-tuned successfully.
    *  - Non-zero - Auto-tuning failed for any widget.
    *
    *******************************************************************************/
    cystatus CapSense_P2_SsAutoTune(void)
    {
        cystatus autoTuneStatus = CYRET_SUCCESS;
        uint32 wdgtIndex;

        /* Tune each CSD widget */
        for (wdgtIndex = 0u; wdgtIndex < CapSense_P2_TOTAL_WIDGETS; wdgtIndex++)
        {
            if (CapSense_P2_SENSE_METHOD_CSD_E ==
                CapSense_P2_GET_SENSE_METHOD(&CapSense_P2_dsFlash.wdgtArray[wdgtIndex]))
            {
                autoTuneStatus |= CapSense_P2_SsAutoTuneWidget(wdgtIndex);
            }
        }
        return autoTuneStatus;
    }
#endif /* (CapSense_P2_CSD_SS_DIS != CapSense_P2_CSD_AUTOTUNE)) */


#if (CapSense_P2_DISABLE == CapSense_P2_CSDV2)
    #if ( ((CapSense_P2_ENABLE == CapSense_P2_CSX_EN) \
        || (CapSense_P2_ENABLE == CapSense_P2_ISX_EN)) ||\
         (CapSense_P2_IDAC_SINKING == CapSense_P2_CSD_IDAC_CONFIG))
        /*******************************************************************************
        * Function Name: CapSense_P2_SsTrimIdacsSinking
        ****************************************************************************//**
        *
        * \brief
        *  This function loads trim values from SFLASH rows to calibrate
        *  IDAC1 and IDAC2 for Sinking CSD Mode
        *
        * \details
        *  Function reads trim value from SFLASH and loads it into IDAC trim register.
        *  If Compensation IDAC is disabled the function loads trim values for IDAC1 only.
        *
        *******************************************************************************/
        static void CapSense_P2_SsTrimIdacsSinking(void)
        {
            uint32 trimValue;

            /* Mod iDAC Sinking Mode */
            trimValue = CY_GET_REG32(CapSense_P2_TRIM2_PTR) & ~CapSense_P2_IDAC_TRIM2_MOD_SNK_MASK;
            trimValue |= ((uint32)CY_GET_REG8(CapSense_P2_SFLASH_TRIM2_PTR) &
                                                      CapSense_P2_SFLASH_TRIM_IDAC_MOD_MASK) ;

            #if (CapSense_P2_ENABLE == CapSense_P2_CSD_IDAC_COMP_EN)
                /* Comp iDAC Sinking Mode */
                trimValue &= ~CapSense_P2_IDAC_TRIM2_COMP_SNK_MASK;
                trimValue |= ((uint32)CY_GET_REG8(CapSense_P2_SFLASH_TRIM2_PTR) &
                                                          CapSense_P2_SFLASH_TRIM_IDAC_COMP_MASK);
            #endif /* (CapSense_P2_ENABLE == CapSense_P2_CSD_IDAC_COMP_EN) */

            /* Update IDAC trim bits for gain control in current sink mode */
            CY_SET_REG32(CapSense_P2_TRIM2_PTR, trimValue);
        }
    #endif /* ((CapSense_P2_ENABLE == CapSense_P2_CSX_EN) || \
               (CapSense_P2_IDAC_SINKING == CapSense_P2_CSD_IDAC_CONFIG)) */


    #if ( ((CapSense_P2_ENABLE == CapSense_P2_CSX_EN) \
        || (CapSense_P2_ENABLE == CapSense_P2_ISX_EN)) || \
         (CapSense_P2_IDAC_SOURCING == CapSense_P2_CSD_IDAC_CONFIG))
        /*******************************************************************************
        * Function Name: CapSense_P2_SsTrimIdacsSourcing
        ****************************************************************************//**
        *
        * \brief
        *  This function loads trim values from SFLASH rows to calibrate
        *  IDAC1 and IDAC2 for Sourcing CSD Mode
        *
        * \details
        *  Function reads trim value from SFLASH and loads it into IDAC trim register.
        *  If Compensation IDAC is disabled the function loads trim values for IDAC1 only.
        *
        *******************************************************************************/
        static void CapSense_P2_SsTrimIdacsSourcing(void)
        {
            uint32 trimValue;

            /* Mod iDAC Sourcing Mode */
            trimValue = CY_GET_REG32(CapSense_P2_TRIM1_PTR) & ~CapSense_P2_IDAC_TRIM1_MOD_SRC_MASK;
            trimValue |= ((uint32)CY_GET_REG8(CapSense_P2_SFLASH_TRIM1_PTR) &
                                                      CapSense_P2_SFLASH_TRIM_IDAC_MOD_MASK);

            #if (CapSense_P2_ENABLE == CapSense_P2_CSD_IDAC_COMP_EN)
                 /* Comp iDAC Sourcing Mode */
                trimValue &= ~CapSense_P2_IDAC_TRIM1_COMP_SRC_MASK;
                trimValue |= ((uint32)CY_GET_REG8(CapSense_P2_SFLASH_TRIM1_PTR) &
                                                          CapSense_P2_SFLASH_TRIM_IDAC_COMP_MASK);
            #endif /* (CapSense_P2_ENABLE == CapSense_P2_CSD_IDAC_COMP_EN) */

            /* Update IDAC trim bits for gain control in current source mode */
            CY_SET_REG32(CapSense_P2_TRIM1_PTR, trimValue);
        }
    #endif /* ((CapSense_P2_ENABLE == CapSense_P2_CSX_EN) || \
               (CapSense_P2_IDAC_SOURCING == CapSense_P2_CSD_IDAC_CONFIG)) */


    /*******************************************************************************
    * Function Name: CapSense_P2_SsTrimIdacs
    ****************************************************************************//**
    *
    * \brief
    *  This function loads trim values from SFLASH rows to calibrate
    *  IDAC1 and IDAC2 for CSD Mode
    *
    * \details
    *  If CSX mode is enabled the function loads trim values for both sink and source
    *  mode. If CSX mode is disabled the function loads trim values for sink or
    *  source mode based on sink/source mode configuration.
    *  If Compensation IDAC is disabled the function loads trim values for IDAC1 only.
    *
    *******************************************************************************/
    static void CapSense_P2_SsTrimIdacs(void)
    {
        #if (CapSense_P2_ENABLE == CapSense_P2_CSX_EN) || (CapSense_P2_ENABLE == CapSense_P2_ISX_EN)
            CapSense_P2_SsTrimIdacsSinking();
            CapSense_P2_SsTrimIdacsSourcing();
        #elif (CapSense_P2_IDAC_SINKING == CapSense_P2_CSD_IDAC_CONFIG)
            CapSense_P2_SsTrimIdacsSinking();
        #elif (CapSense_P2_IDAC_SOURCING == CapSense_P2_CSD_IDAC_CONFIG)
            CapSense_P2_SsTrimIdacsSourcing();
        #else
            #error "Not supported Mode, Component cannot work in this mode"
        #endif /* (CapSense_P2_ENABLE == CapSense_P2_CSX_EN) || (CapSense_P2_ENABLE == CapSense_P2_ISX_EN) */
    }
#endif  /* (CapSense_P2_DISABLE == CapSense_P2_CSDV2) */


#if (CapSense_P2_ENABLE == CapSense_P2_MULTI_FREQ_SCAN_EN)
    /*******************************************************************************
    * Function Name: CapSense_P2_SsChangeImoFreq
    ****************************************************************************//**
    *
    * \brief
    *  This function changes the IMO frequency.
    *
    * \details
    *  The IMO frequency can have three offsets: 0%, -5% and +5%. The frequency
    *  trims are contained in the CapSense_P2_immunity[value] array for each
    *  frequency channel.
    *
    * \param value The frequency channel ID.
    *
    *******************************************************************************/
    void CapSense_P2_SsChangeImoFreq(uint32 value)
    {
        CY_SET_REG32(CY_SYS_CLK_IMO_TRIM1_PTR, CapSense_P2_immunity[value]);
    }

    /*******************************************************************************
    * Function Name: CapSense_P2_SsChangeClkFreq
    ****************************************************************************//**
    *
    * \brief
    *  This function changes the sensor clock frequency by configuring
    *  the corresponding divider.
    *
    * \details
    *  This function changes the sensor clock frequency by configuring
    *  the corresponding divider.
    *
    * \param chId
    *  The frequency channel ID.
    *
    *******************************************************************************/
    void CapSense_P2_SsChangeClkFreq(uint32 chId)
    {
        uint32 snsClkDivider;
        uint32 freqOffset1 = 0u;
        uint32 freqOffset2 = 0u;

        #if (0u != CapSense_P2_TOTAL_CSD_WIDGETS)
            #if (CapSense_P2_ENABLE == CapSense_P2_CSDV2)
                uint32 conversionsNum;
            #endif /* (CapSense_P2_ENABLE == CapSense_P2_CSDV2) */

            #if((CapSense_P2_CLK_SOURCE_PRS8  == CapSense_P2_CSD_SNS_CLK_SOURCE) ||\
                (CapSense_P2_CLK_SOURCE_PRS12 == CapSense_P2_CSD_SNS_CLK_SOURCE) ||\
                (CapSense_P2_CLK_SOURCE_PRSAUTO == CapSense_P2_CSD_SNS_CLK_SOURCE))
                uint32 snsClkSrc;
            #endif
        #endif

        #if ((0u != CapSense_P2_TOTAL_CSD_WIDGETS) || \
             ((CapSense_P2_DISABLE == CapSense_P2_CSX_COMMON_TX_CLK_EN) && (0u != CapSense_P2_TOTAL_CSX_WIDGETS)))
            CapSense_P2_FLASH_WD_STRUCT const *ptrFlashWdgt = &CapSense_P2_dsFlash.wdgtArray[CapSense_P2_widgetIndex];
            CapSense_P2_RAM_WD_BASE_STRUCT const *ptrWdgt = (CapSense_P2_RAM_WD_BASE_STRUCT *)ptrFlashWdgt->ptr2WdgtRam;
        #endif

        switch(CapSense_P2_GET_SENSE_METHOD(&CapSense_P2_dsFlash.wdgtArray[CapSense_P2_widgetIndex]))
        {
        #if (0u != CapSense_P2_TOTAL_CSD_WIDGETS)
            case CapSense_P2_SENSE_METHOD_CSD_E:
                /* Get sensor clock divider value */
                #if (CapSense_P2_ENABLE == CapSense_P2_CSD_COMMON_SNS_CLK_EN)
                    snsClkDivider = CapSense_P2_dsRam.snsCsdClk;
                #else /* (CapSense_P2_ENABLE == CapSense_P2_CSD_COMMON_SNS_CLK_EN) */
                    #if (CapSense_P2_CSD_MATRIX_WIDGET_EN || CapSense_P2_CSD_TOUCHPAD_WIDGET_EN)
                        /* Get SnsClck divider for rows or columns */
                        if (CapSense_P2_dsFlash.wdgtArray[CapSense_P2_widgetIndex].numCols <= CapSense_P2_sensorIndex)
                        {
                            snsClkDivider = ptrWdgt->rowSnsClk;
                        }
                        else
                        {
                            snsClkDivider = ptrWdgt->snsClk;
                        }
                    #else
                        snsClkDivider = ptrWdgt->snsClk;
                    #endif /* (CapSense_P2_CSD_MATRIX_WIDGET_EN || CapSense_P2_CSD_TOUCHPAD_WIDGET_EN) */
                #endif /* (CapSense_P2_ENABLE == CapSense_P2_CSD_COMMON_SNS_CLK_EN) */

                freqOffset1 = CapSense_P2_CSD_MFS_DIVIDER_OFFSET_F1;
                freqOffset2 = CapSense_P2_CSD_MFS_DIVIDER_OFFSET_F2;

                #if((CapSense_P2_CLK_SOURCE_PRS8  == CapSense_P2_CSD_SNS_CLK_SOURCE) ||\
                    (CapSense_P2_CLK_SOURCE_PRS12 == CapSense_P2_CSD_SNS_CLK_SOURCE) ||\
                    (CapSense_P2_CLK_SOURCE_PRSAUTO == CapSense_P2_CSD_SNS_CLK_SOURCE))
                    /* Get sense clk source */
                    #if (CapSense_P2_CSD_MATRIX_WIDGET_EN || CapSense_P2_CSD_TOUCHPAD_WIDGET_EN)
                        /* Get SnsClc Source for rows or columns */
                        if (CapSense_P2_dsFlash.wdgtArray[CapSense_P2_widgetIndex].numCols <= CapSense_P2_sensorIndex)
                        {
                            snsClkSrc = (uint32)ptrWdgt->rowSnsClkSource;
                        }
                        else
                        {
                            snsClkSrc = (uint32)ptrWdgt->snsClkSource;
                        }
                    #else
                        snsClkSrc = (uint32)ptrWdgt->snsClkSource;
                    #endif /* (CapSense_P2_CSD_MATRIX_WIDGET_EN || CapSense_P2_CSD_TOUCHPAD_WIDGET_EN) */

                    switch (snsClkSrc)
                    {
                    case CapSense_P2_CLK_SOURCE_PRS8:
                    case CapSense_P2_CLK_SOURCE_PRS12:
                        /* Multiply by 2 for PRS8/PRS12 mode */
                        freqOffset1 <<= 1u;
                        freqOffset2 <<= 1u;
                        break;

                    default:
                        break;
                    }
                #endif

                /* Change the divider based on the chId */
                switch (chId)
                {
                    case CapSense_P2_FREQ_CHANNEL_1:
                    {
                        snsClkDivider += freqOffset1;
                        break;
                    }
                    case CapSense_P2_FREQ_CHANNEL_2:
                    {
                        snsClkDivider += freqOffset2;
                        break;
                    }
                    default:
                    {
                        break;
                    }
                }

                #if (CapSense_P2_ENABLE == CapSense_P2_CSDV2)
                    /* Set Number Of Conversions based on scanning resolution */
                    conversionsNum = CapSense_P2_SsCSDGetNumberOfConversions(snsClkDivider, (uint32)ptrWdgt->resolution, (uint32)ptrWdgt->snsClkSource);
                    CY_SET_REG32(CapSense_P2_SEQ_NORM_CNT_PTR, (conversionsNum & CapSense_P2_SEQ_NORM_CNT_CONV_CNT_MASK));
                #endif /* (CapSense_P2_ENABLE == CapSense_P2_CSDV2) */

                #if((CapSense_P2_CLK_SOURCE_PRS8  == CapSense_P2_CSD_SNS_CLK_SOURCE) ||\
                    (CapSense_P2_CLK_SOURCE_PRS12 == CapSense_P2_CSD_SNS_CLK_SOURCE) ||\
                    (CapSense_P2_CLK_SOURCE_PRSAUTO == CapSense_P2_CSD_SNS_CLK_SOURCE))
                    switch (snsClkSrc)
                    {
                    case CapSense_P2_CLK_SOURCE_PRS8:
                    case CapSense_P2_CLK_SOURCE_PRS12:
                        /* Divide by 2 for PRS8/PRS12 mode */
                        snsClkDivider >>= 1;
                        if (snsClkDivider == 0u)
                        {
                            snsClkDivider = 1u;
                        }
                        break;

                    default:
                        break;
                    }
                #endif

                /* Configure the new divider */
                CapSense_P2_SsSetSnsClockDivider(snsClkDivider);

                break;
        #endif /* #if (0u != CapSense_P2_TOTAL_CSD_WIDGETS) */

        #if (0u != CapSense_P2_TOTAL_CSX_WIDGETS)
            case CapSense_P2_SENSE_METHOD_CSX_E:
                #if (CapSense_P2_ENABLE == CapSense_P2_CSX_COMMON_TX_CLK_EN)
                    snsClkDivider = CapSense_P2_dsRam.snsCsxClk;
                #else /* (CapSense_P2_ENABLE == CapSense_P2_CSX_COMMON_TX_CLK_EN) */
                    snsClkDivider = ptrWdgt->snsClk;
                #endif /* (CapSense_P2_ENABLE == CapSense_P2_CSX_COMMON_TX_CLK_EN) */
                freqOffset1 = CapSense_P2_CSX_MFS_DIVIDER_OFFSET_F1;
                freqOffset2 = CapSense_P2_CSX_MFS_DIVIDER_OFFSET_F2;

                /* Change the divider based on the chId */
                switch (chId)
                {
                    case CapSense_P2_FREQ_CHANNEL_1:
                    {
                        snsClkDivider += freqOffset1;
                        break;
                    }
                    case CapSense_P2_FREQ_CHANNEL_2:
                    {
                        snsClkDivider += freqOffset2;
                        break;
                    }
                    default:
                    {
                        break;
                    }
                }

                /* Configure the new divider */
                CapSense_P2_SsSetSnsClockDivider(snsClkDivider);

                break;
        #endif /* #if (0u != CapSense_P2_TOTAL_CSX_WIDGETS) */

        default:
            CYASSERT(0);
            break;
        }
    }
#endif  /* (CapSense_P2_ENABLE == CapSense_P2_MULTI_FREQ_SCAN_EN) */


#if((CapSense_P2_ENABLE == CapSense_P2_CSD_EN) || \
    ((CapSense_P2_ENABLE == CapSense_P2_CSX_EN) && \
     (CapSense_P2_ENABLE == CapSense_P2_CSDV2)))
/*******************************************************************************
* Function Name: CapSense_P2_SsInitializeSourceSenseClk
****************************************************************************//**
*
* \brief
*  Sets a source for Sense Clk for all CSD widgets.
*
* \details
*  Updates snsClkSource and rowSnsClkSource with a source for the sense Clk.
*  for all CSD widgets.
*
*******************************************************************************/
void CapSense_P2_SsInitializeSourceSenseClk(void)
{
    uint32 wdgtIndex;
    CapSense_P2_RAM_WD_BASE_STRUCT *ptrWdgt;

    for (wdgtIndex = 0u; wdgtIndex < CapSense_P2_TOTAL_WIDGETS; wdgtIndex++)
    {
        ptrWdgt = (CapSense_P2_RAM_WD_BASE_STRUCT *)CapSense_P2_dsFlash.wdgtArray[wdgtIndex].ptr2WdgtRam;

        switch (CapSense_P2_GET_SENSE_METHOD(&CapSense_P2_dsFlash.wdgtArray[wdgtIndex]))
        {
        #if (CapSense_P2_ENABLE == CapSense_P2_CSD_EN)
            case CapSense_P2_SENSE_METHOD_CSD_E:
                CapSense_P2_SsSetWidgetSenseClkSrc(wdgtIndex, ptrWdgt);
                break;
        #endif /* (CapSense_P2_ENABLE == CapSense_P2_CSD_EN) */
        #if ((CapSense_P2_ENABLE == CapSense_P2_CSX_EN) && (CapSense_P2_ENABLE == CapSense_P2_CSDV2))
            case CapSense_P2_SENSE_METHOD_CSX_E:
                CapSense_P2_SsSetWidgetTxClkSrc(wdgtIndex, ptrWdgt);
                break;
        #endif /* (((CapSense_P2_ENABLE == CapSense_P2_CSX_EN) || (CapSense_P2_ENABLE == CapSense_P2_ISX_EN)) && (CapSense_P2_ENABLE == CapSense_P2_CSDV2)) */
        default:
            break;
        }
        #if (CapSense_P2_ENABLE == CapSense_P2_TST_WDGT_CRC_EN)
            CapSense_P2_DsUpdateWidgetCrc(wdgtIndex);
        #endif /* (CapSense_P2_ENABLE == CapSense_P2_TST_WDGT_CRC_EN) */
    }
}
#endif /* ((CapSense_P2_ENABLE == CapSense_P2_CSD_EN) || \
           (((CapSense_P2_ENABLE == CapSense_P2_CSX_EN) || \
           (CapSense_P2_ENABLE == CapSense_P2_ISX_EN)) && \
           (CapSense_P2_ENABLE == CapSense_P2_CSDV2))) */


#if (CapSense_P2_ENABLE == CapSense_P2_CSD_EN)
    /*******************************************************************************
    * Function Name: CapSense_P2_SsSetWidgetSenseClkSrc
    ****************************************************************************//**
    *
    * \brief
    *  Sets a source for the sense clock for a widget.
    *
    * \param wdgtIndex
    *  Specifies the ID of the widget.
    * \param ptrWdgt
    *  The pointer to the RAM_WD_BASE_STRUCT structure.
    *
    * \details
    *  Updates snsClkSource and rowSnsClkSource with a source for the sense Clk for a
    *  widget.
    *
    *******************************************************************************/
    static void CapSense_P2_SsSetWidgetSenseClkSrc(uint32 wdgtIndex, CapSense_P2_RAM_WD_BASE_STRUCT * ptrWdgt)
    {
        uint8 lfsrSize;
        uint8 lfsrScale;

        #if(CapSense_P2_CLK_SOURCE_PRSAUTO == CapSense_P2_CSD_SNS_CLK_SOURCE)
            #if(CapSense_P2_ENABLE == CapSense_P2_CSDV2)
                uint32 conversionsNum;
            #endif /* (CapSense_P2_ENABLE == CapSense_P2_CSDV2) */
            uint32 snsClkDivider;
        #endif /* (CapSense_P2_CLK_SOURCE_PRSAUTO == CapSense_P2_CSD_SNS_CLK_SOURCE) */

        #if(CapSense_P2_CLK_SOURCE_PRSAUTO == CapSense_P2_CSD_SNS_CLK_SOURCE)
            snsClkDivider = CapSense_P2_SsCSDGetColSnsClkDivider(wdgtIndex);

            #if(CapSense_P2_ENABLE == CapSense_P2_CSDV2)
                conversionsNum = CapSense_P2_SsCSDGetNumberOfConversions(snsClkDivider, (uint32)ptrWdgt->resolution,
                                                                                             CapSense_P2_CLK_SOURCE_DIRECT);
                lfsrSize = CapSense_P2_SsCalcLfsrSize(snsClkDivider, conversionsNum);
                if (CapSense_P2_CLK_SOURCE_DIRECT == lfsrSize)
                {
                    /*
                    * Multiplying of the snsClkDivider by 2 is needed to pass the
                    * average PRS frequency through the argument.
                    */
                    lfsrSize = CapSense_P2_SsCSDCalcPrsSize(snsClkDivider << 1uL, (uint32)ptrWdgt->resolution);
                }
                lfsrScale = CapSense_P2_SsCalcLfsrScale(snsClkDivider, lfsrSize);
            #else
                /*
                * Multiplying of the snsClkDivider by 2 is needed to pass the
                * average PRS frequency through the argument.
                */
                lfsrSize = CapSense_P2_SsCSDCalcPrsSize(snsClkDivider << 1uL, (uint32)ptrWdgt->resolution);
                lfsrScale = 0u;
            #endif /* (CapSense_P2_ENABLE == CapSense_P2_CSDV2) */
        #else
            lfsrSize = (uint8)CapSense_P2_DEFAULT_MODULATION_MODE;
            lfsrScale = 0u;
            (void)wdgtIndex; /* This parameter is unused in such configurations */
        #endif /* (CapSense_P2_CLK_SOURCE_PRSAUTO == CapSense_P2_CSD_SNS_CLK_SOURCE) */

        ptrWdgt->snsClkSource = lfsrSize | (uint8)(lfsrScale << CapSense_P2_CLK_SOURCE_LFSR_SCALE_OFFSET);

        #if (CapSense_P2_CSD_MATRIX_WIDGET_EN || CapSense_P2_CSD_TOUCHPAD_WIDGET_EN)
            if ((CapSense_P2_WD_TOUCHPAD_E == (CapSense_P2_WD_TYPE_ENUM)CapSense_P2_dsFlash.wdgtArray[wdgtIndex].wdgtType) ||
                (CapSense_P2_WD_MATRIX_BUTTON_E == (CapSense_P2_WD_TYPE_ENUM)CapSense_P2_dsFlash.wdgtArray[wdgtIndex].wdgtType))
            {
                #if(CapSense_P2_CLK_SOURCE_PRSAUTO == CapSense_P2_CSD_SNS_CLK_SOURCE)
                    snsClkDivider = CapSense_P2_SsCSDGetRowSnsClkDivider(wdgtIndex);

                    #if(CapSense_P2_ENABLE == CapSense_P2_CSDV2)
                        lfsrSize = CapSense_P2_SsCalcLfsrSize(snsClkDivider, conversionsNum);
                        if (CapSense_P2_CLK_SOURCE_DIRECT == lfsrSize)
                        {
                            /*
                            * Multiplying of the snsClkDivider by 2 is needed to pass the
                            * average PRS frequency through the argument.
                            */
                            lfsrSize = CapSense_P2_SsCSDCalcPrsSize(snsClkDivider << 1uL, (uint32)ptrWdgt->resolution);
                        }
                        lfsrScale = CapSense_P2_SsCalcLfsrScale(snsClkDivider, lfsrSize);
                    #else
                        /*
                        * Multiplying of the snsClkDivider by 2 is needed to pass the
                        * average PRS frequency through the argument.
                        */
                        lfsrSize = CapSense_P2_SsCSDCalcPrsSize(snsClkDivider << 1uL, (uint32)ptrWdgt->resolution);
                        lfsrScale = 0u;
                    #endif /* (CapSense_P2_ENABLE == CapSense_P2_CSDV2) */
                #else
                    lfsrSize = (uint8)CapSense_P2_DEFAULT_MODULATION_MODE;
                    lfsrScale = 0u;
                #endif /* (CapSense_P2_CLK_SOURCE_PRSAUTO == CapSense_P2_CSD_SNS_CLK_SOURCE) */
                ptrWdgt->rowSnsClkSource = lfsrSize | (uint8)(lfsrScale << CapSense_P2_CLK_SOURCE_LFSR_SCALE_OFFSET);
            }
        #endif /* (CapSense_P2_CSD_MATRIX_WIDGET_EN || CapSense_P2_CSD_TOUCHPAD_WIDGET_EN) */
    }
#endif /* (CapSense_P2_ENABLE == CapSense_P2_CSD_EN) */


#if ((CapSense_P2_ENABLE == CapSense_P2_CSX_EN) && (CapSense_P2_ENABLE == CapSense_P2_CSDV2))
    /*******************************************************************************
    * Function Name: CapSense_P2_SsSetWidgetTxClkSrc
    ****************************************************************************//**
    *
    * \brief
    *  Sets a source for the Tx clock for a widget.
    *
    * \param wdgtIndex
    *  Specifies the ID of the widget.
    * \param ptrWdgt
    *  The pointer to the RAM_WD_BASE_STRUCT structure.
    *
    * \details
    *  Updates snsClkSource with a source for Tx Clk for a widget.
    *
    *******************************************************************************/
    static void CapSense_P2_SsSetWidgetTxClkSrc(uint32 wdgtIndex, CapSense_P2_RAM_WD_BASE_STRUCT * ptrWdgt)
    {
        uint8 lfsrSize;
        uint8 lfsrScale;

        #if ((CapSense_P2_CLK_SOURCE_PRSAUTO == CapSense_P2_CSX_TX_CLK_SOURCE) && \
             (CapSense_P2_DISABLE == CapSense_P2_CSX_SKIP_OVSMPL_SPECIFIC_NODES))
            uint32 conversionsNum;
            uint32 snsClkDivider;
        #endif

        #if(CapSense_P2_CLK_SOURCE_PRSAUTO == CapSense_P2_CSX_TX_CLK_SOURCE)
            #if (CapSense_P2_DISABLE == CapSense_P2_CSX_SKIP_OVSMPL_SPECIFIC_NODES)
                conversionsNum = (uint32)ptrWdgt->resolution;
                snsClkDivider = CapSense_P2_SsCSXGetTxClkDivider(wdgtIndex);
                lfsrSize = CapSense_P2_SsCalcLfsrSize(snsClkDivider, conversionsNum);
                lfsrScale = CapSense_P2_SsCalcLfsrScale(snsClkDivider, lfsrSize);
            #else
                lfsrSize = (uint8)CapSense_P2_CLK_SOURCE_DIRECT;
                lfsrScale = 0u;
                /* Unused function argument */
                (void)wdgtIndex;
            #endif /* (CapSense_P2_ENABLE == CapSense_P2_CSX_SKIP_OVSMPL_SPECIFIC_NODES) */
        #else
            lfsrSize = (uint8)CapSense_P2_CSX_TX_CLK_SOURCE;
            lfsrScale = 0u;
            /* Unused function argument */
            (void)wdgtIndex;
        #endif /* (CapSense_P2_CLK_SOURCE_PRSAUTO == CapSense_P2_CSX_TX_CLK_SOURCE) */

        ptrWdgt->snsClkSource = lfsrSize | (uint8)(lfsrScale << CapSense_P2_CLK_SOURCE_LFSR_SCALE_OFFSET);
    }
#endif /* ((CapSense_P2_ENABLE == CapSense_P2_CSX_EN) && (CapSense_P2_ENABLE == CapSense_P2_CSDV2)) */


#if(((CapSense_P2_ENABLE == CapSense_P2_CSX_EN) && \
     (CapSense_P2_DISABLE == CapSense_P2_CSX_SKIP_OVSMPL_SPECIFIC_NODES) && \
     (CapSense_P2_ENABLE == CapSense_P2_CSDV2) && \
     (CapSense_P2_CLK_SOURCE_PRSAUTO == CapSense_P2_CSX_TX_CLK_SOURCE)) ||\
    ((CapSense_P2_ENABLE == CapSense_P2_CSD_EN) && \
     (CapSense_P2_ENABLE == CapSense_P2_CSDV2) && \
     (CapSense_P2_CLK_SOURCE_PRSAUTO == CapSense_P2_CSD_SNS_CLK_SOURCE)))
/*******************************************************************************
* Function Name: CapSense_P2_SsCalcLfsrSize
****************************************************************************//**
*
* \brief
*  This is an internal function that finds a SSC polynomial size in the Auto mode.
*
* \details
*  The SSC polynomial size in the auto mode is found based on the following
*  requirements:
*  - an LFSR value should be selected so that the max clock dither is limited with +/-10%.
*  - at least one full spread spectrum polynomial should pass during the scan time.
*  - the value of the number of conversions should be an integer multiple of the
*    repeat period of the programmed LFSR_SIZE.
*
* \param
*  snsClkDivider The divider value for the sense clock.
*  resolution The widget resolution.
*
* \return lfsrSize The LFSRSIZE value for the SENSE_PERIOD register.
*
*******************************************************************************/
static uint8 CapSense_P2_SsCalcLfsrSize(uint32 snsClkDivider, uint32 conversionsNum)
{
    uint8 lfsrSize = 0u;

    /* Find LFSR value */
    if((CapSense_P2_SNSCLK_SSC4_THRESHOLD <= snsClkDivider) &&
       (CapSense_P2_SNSCLK_SSC4_PERIOD <= conversionsNum) &&
       (0uL == (conversionsNum % CapSense_P2_SNSCLK_SSC4_PERIOD)))
    {
        lfsrSize = CapSense_P2_CLK_SOURCE_SSC4;
    }
    else if((CapSense_P2_SNSCLK_SSC3_THRESHOLD <= snsClkDivider) &&
            (CapSense_P2_SNSCLK_SSC3_PERIOD <= conversionsNum) &&
            (0uL == (conversionsNum % CapSense_P2_SNSCLK_SSC3_PERIOD)))
    {
        lfsrSize = CapSense_P2_CLK_SOURCE_SSC3;
    }
    else if((CapSense_P2_SNSCLK_SSC2_THRESHOLD <= snsClkDivider) &&
            (CapSense_P2_SNSCLK_SSC2_PERIOD <= conversionsNum) &&
            (0uL == (conversionsNum % CapSense_P2_SNSCLK_SSC2_PERIOD)))
    {
        lfsrSize = CapSense_P2_CLK_SOURCE_SSC2;
    }
    else if((CapSense_P2_SNSCLK_SSC1_THRESHOLD <= snsClkDivider) &&
            (CapSense_P2_SNSCLK_SSC1_PERIOD <= conversionsNum) &&
            (0uL == (conversionsNum % CapSense_P2_SNSCLK_SSC1_PERIOD)))
    {
        lfsrSize = CapSense_P2_CLK_SOURCE_SSC1;
    }
    else
    {
        lfsrSize = (uint8)CapSense_P2_CLK_SOURCE_DIRECT;
    }

    return (lfsrSize);
}


/*******************************************************************************
* Function Name: CapSense_P2_SsCalcLfsrScale
****************************************************************************//**
*
* \brief
*  This is an internal function that calculates the LFSR scale value.
*
* \details
*  The LFSR scale value is used to increase the clock dither if the desired dither
*  is wider than can be achieved with the current Sense Clock Divider and current LFSR
*  period.
*
*  This returns the LFSR scale value needed to keep the clock dither in
*  range +/-10%.
*
* \param
*  snsClkDivider The divider value for the sense clock.
*  lfsrSize The period of the LFSR sequence.
*          For devices with CapSense_P2_CSDV2_REF9P6UA_EN = TRUE, the
*          mode parameters can take the following values:
*          CapSense_P2_CLK_SOURCE_DIRECT The spreadspectrum is not used.
*          CapSense_P2_CLK_SOURCE_SSC1   The length of LFSR sequence is 63 cycles.
*          CapSense_P2_CLK_SOURCE_SSC2   The length of LFSR sequence is 127 cycles.
*          CapSense_P2_CLK_SOURCE_SSC3   The length of LFSR sequence is 255 cycles.
*          CapSense_P2_CLK_SOURCE_SSC4   The length of LFSR sequence is 511 cycles.
*
*          For devices with CapSense_P2_CSDV2_REF9P6UA_EN = TRUE, the
*          mode parameters can take the following values:
*          CapSense_P2_CLK_SOURCE_DIRECT The spreadspectrum is not used.
*          CapSense_P2_CLK_SOURCE_SSC1   The length of LFSR sequence is 3 cycles.
*          CapSense_P2_CLK_SOURCE_SSC2   The length of LFSR sequence is 7 cycles.
*          CapSense_P2_CLK_SOURCE_SSC3   The length of LFSR sequence is 15 cycles.
*          CapSense_P2_CLK_SOURCE_SSC4   The length of LFSR sequence is 31 cycles.
*
* \return
*  Returns the LFSR scale value needed to keep the clock dither in range +/-10%.
*
*******************************************************************************/
static uint8 CapSense_P2_SsCalcLfsrScale(uint32 snsClkDivider, uint8 lfsrSize)
{
    uint32 lfsrScale;
    uint32 lfsrRange;
    uint32 lfsrDither;

    /* Initialize the lfsrSize variable with the LFSR Range for given Lfsr Size. */
    switch(lfsrSize)
    {
        case CapSense_P2_CLK_SOURCE_SSC1:
        {
            lfsrRange = CapSense_P2_SNSCLK_SSC1_RANGE;
            break;
        }
        case CapSense_P2_CLK_SOURCE_SSC2:
        {
            lfsrRange = CapSense_P2_SNSCLK_SSC2_RANGE;
            break;
        }
        case CapSense_P2_CLK_SOURCE_SSC3:
        {
            lfsrRange = CapSense_P2_SNSCLK_SSC3_RANGE;
            break;
        }
        case CapSense_P2_CLK_SOURCE_SSC4:
        {
            lfsrRange = CapSense_P2_SNSCLK_SSC4_RANGE;
            break;
        }
        default:
        {
            lfsrRange = 0u;
            break;
        }
    }

    /* Calculate the LFSR Scale value that is required to keep the Clock dither
     * as close as possible to the +/-10% limit of the used frequency.
     */
    if((lfsrSize != CapSense_P2_CLK_SOURCE_DIRECT) && (0u != lfsrRange))
    {
        /* Calculate the LFSR Dither in percents. */
        lfsrDither  = ((lfsrRange * 100uL) / snsClkDivider);
        lfsrScale = 0uL;

        while(lfsrDither < CapSense_P2_LFSR_DITHER_PERCENTAGE)
        {
            lfsrScale++;
            lfsrDither <<=1uL;
        }

        if(lfsrDither > CapSense_P2_LFSR_DITHER_PERCENTAGE)
        {
            lfsrScale--;
        }
    }
    else
    {
        lfsrScale = 0uL;
    }

    return ((uint8)lfsrScale);
}

#endif /* (((CapSense_P2_ENABLE == CapSense_P2_CSX_EN) && \
           (CapSense_P2_DISABLE == CapSense_P2_CSX_SKIP_OVSMPL_SPECIFIC_NODES) && \
           (CapSense_P2_ENABLE == CapSense_P2_CSDV2) && \
           (CapSense_P2_CLK_SOURCE_PRSAUTO == CapSense_P2_CSX_TX_CLK_SOURCE)) ||\
          ((CapSense_P2_ENABLE == CapSense_P2_CSD_EN) && \
           (CapSense_P2_ENABLE == CapSense_P2_CSDV2) && \
           (CapSense_P2_CLK_SOURCE_PRSAUTO == CapSense_P2_CSD_SNS_CLK_SOURCE))) */


#if (CapSense_P2_ENABLE == CapSense_P2_CSD_EN)


    /*******************************************************************************
    * Function Name: CapSense_P2_SsCSDGetColSnsClkDivider
    ****************************************************************************//**
    *
    * \brief
    *  This function gets the Sense Clock Divider value for one-dimension widgets
    *  and Column Sense Clock divider value for two-dimension widgets.
    *
    * \details
    *  This function gets the Sense Clock Divider value based on the Component
    *  configuration. The function is applicable for one-dimension widgets and for
    *  two-dimension widgets.
    *
    * \param widgetId
    *  Specifies the ID of the widget.
    *
    * \return
    *  Returns the Sense Clock Divider value for one-dimension widgets
    *  and the Column Sense Clock divider value for two-dimension widgets.
    *
    *******************************************************************************/
    uint32 CapSense_P2_SsCSDGetColSnsClkDivider(uint32 widgetId)
    {
        uint32 retVal;

        /* Get sense divider based on configuration */
        #if (CapSense_P2_ENABLE != CapSense_P2_CSD_COMMON_SNS_CLK_EN)
            CapSense_P2_RAM_WD_BASE_STRUCT *ptrWdgt;

            ptrWdgt = (CapSense_P2_RAM_WD_BASE_STRUCT *)
            CapSense_P2_dsFlash.wdgtArray[widgetId].ptr2WdgtRam;

            retVal = (uint32)(ptrWdgt->snsClk);
        #else
            retVal = (uint32)CapSense_P2_dsRam.snsCsdClk;

            (void)widgetId; /* This parameter is unused in such configurations */
        #endif /* (CapSense_P2_ENABLE == CapSense_P2_CSD_COMMON_SNS_CLK_EN) */

        return (retVal);
    }


    #if (CapSense_P2_CSD_MATRIX_WIDGET_EN || CapSense_P2_CSD_TOUCHPAD_WIDGET_EN)
        /*******************************************************************************
        * Function Name: CapSense_P2_SsCSDGetRowSnsClkDivider
        ****************************************************************************//**
        *
        * \brief
        *  This function gets the Sense Clock Divider value for one-dimension widgets
        *  and the Column Sense Clock divider value for two-dimension widgets.
        *
        * \details
        *  This function gets the Sense Clock Divider value based on the Component
        *  configuration. The function is applicable for one-dimension widgets and for
        *  two-dimension widgets.
        *
        * \param
        *  widgetId Specifies the ID of the widget.
        *
        * \return
        *  Returns the sense clock divider value for one-dimension widgets
        *  and column sense clock divider value for two-dimension widgets.
        *
        *******************************************************************************/
        uint32 CapSense_P2_SsCSDGetRowSnsClkDivider(uint32 widgetId)
        {
            uint32 retVal;

            /* Get sense divider based on configuration */
            #if (CapSense_P2_ENABLE != CapSense_P2_CSD_COMMON_SNS_CLK_EN)
                CapSense_P2_RAM_WD_BASE_STRUCT *ptrWdgt;

                ptrWdgt = (CapSense_P2_RAM_WD_BASE_STRUCT *)
                CapSense_P2_dsFlash.wdgtArray[widgetId].ptr2WdgtRam;

                retVal = ptrWdgt->rowSnsClk;
            #else
                retVal = (uint32)CapSense_P2_dsRam.snsCsdClk;

                (void)widgetId; /* This parameter is unused in such configurations */
            #endif /* (CapSense_P2_ENABLE == CapSense_P2_CSD_COMMON_SNS_CLK_EN) */

            return (retVal);
        }

    #endif /* (CapSense_P2_CSD_MATRIX_WIDGET_EN || CapSense_P2_CSD_TOUCHPAD_WIDGET_EN) */


    #if (CapSense_P2_CLK_SOURCE_PRSAUTO == CapSense_P2_CSD_SNS_CLK_SOURCE)
        /*******************************************************************************
        * Function Name: CapSense_P2_SsCSDCalcPrsSize
        ****************************************************************************//**
        *
        * \brief
        *  The function finds PRS polynomial size in the Auto mode.
        *
        * \details
        *  The PRS polynomial size in the Auto mode is found based on the following
        *  requirements:
        *  - at least one full spread spectrum polynomial should pass during scan time.
        *
        * \param
        *  snsClkDivider The divider value for the sense clock.
        *  resolution The widget resolution.
        *
        * \return prsSize PRS value for SENSE_PERIOD register.
        *
        *******************************************************************************/
        uint8 CapSense_P2_SsCSDCalcPrsSize(uint32 snsClkDivider, uint32 resolution)
        {
            uint32 prsSize;
            uint32 modClkDivider = 1u;

            #if (CapSense_P2_ENABLE != CapSense_P2_CSDV2)
                #if(CapSense_P2_ENABLE == CapSense_P2_IS_M0S8PERI_BLOCK)
                    modClkDivider = (uint32)CapSense_P2_dsRam.modCsdClk;
                #endif

                /*
                * This operation is needed for Third-generation HW Block
                * based devices to take into account DIV-2 flip-flop.
                */
                snsClkDivider <<= 1uL;
            #endif

            if ((snsClkDivider * CapSense_P2_PRS_LENGTH_12_BITS) <= (modClkDivider * ((0x00000001Lu << resolution) - 1u)))
            {
                /* Set PRS12 mode */
                prsSize = CapSense_P2_PRS_12_CONFIG;
            }
            else if ((snsClkDivider * CapSense_P2_PRS_LENGTH_8_BITS) <= (modClkDivider * ((0x00000001Lu << resolution) - 1u)))
            {
                /* Set PRS8 mode */
                prsSize = CapSense_P2_PRS_8_CONFIG;
            }
            else
            {
                /* Set Direct clock mode */
                prsSize = CapSense_P2_CLK_SOURCE_DIRECT;
            }

            return (uint8)prsSize;
        }
    #endif /* (CapSense_P2_CLK_SOURCE_PRSAUTO == CapSense_P2_CSD_SNS_CLK_SOURCE) */
#endif /* (CapSense_P2_ENABLE == CapSense_P2_CSD_EN) */


#if (CapSense_P2_ENABLE == CapSense_P2_CSDV2)
    /*******************************************************************************
    * Function Name: CapSense_P2_DischargeExtCapacitors
    ****************************************************************************//**
    *
    * \brief
    *  The internal function discharge available external capacitors.
    *
    * \details
    *  The function discharge available external capacitors by connection them
    *  to GND using STRONG GPIO drive mode. Additionaly, the function disconnects
    *  the capacitors from analog mux buses if connected.
    *  Note: the function does not restore the connection to analog mux busses
    *  and supposes that all the capacitors belong to a single device port.
    *
    * \param
    *  dischargeTime The discharge time in us.
    *
    *******************************************************************************/
    void CapSense_P2_DischargeExtCapacitors(uint32 dischargeTime)
    {
        uint32 hsiomReg;
        uint32 drReg;
        uint32 pcReg;
        uint32 newRegValue;
        uint8  interruptState;

        /* Store register state */
        hsiomReg = CY_GET_REG32(CapSense_P2_EXT_CAP_HSIOM_PTR) & CapSense_P2_EXT_CAP_HSIOM_MASK;
        drReg = CY_GET_REG32(CapSense_P2_EXT_CAP_DR_PTR) & CapSense_P2_EXT_CAP_DR_MASK;
        pcReg = CY_GET_REG32(CapSense_P2_EXT_CAP_PC_PTR) & CapSense_P2_EXT_CAP_PC_MASK;

        /* Disconnect Ext Cap from AMUXBUS-A using HSIOM registers */
        interruptState = CyEnterCriticalSection();
        newRegValue = CY_GET_REG32(CapSense_P2_EXT_CAP_HSIOM_PTR);
        newRegValue &= (uint32)(~(uint32)CapSense_P2_EXT_CAP_HSIOM_MASK);
        CY_SET_REG32(CapSense_P2_EXT_CAP_HSIOM_PTR, newRegValue);
        CyExitCriticalSection(interruptState);

        /* Set output port register to 0 to connect to GND */
        interruptState = CyEnterCriticalSection();
        newRegValue = CY_GET_REG32(CapSense_P2_EXT_CAP_DR_PTR);
        newRegValue &= (uint32)(~(uint32)CapSense_P2_EXT_CAP_DR_MASK);
        CY_SET_REG32(CapSense_P2_EXT_CAP_DR_PTR, newRegValue);
        CyExitCriticalSection(interruptState);

        /* Set port configuration register (drive mode) in STRONG mode */
        interruptState = CyEnterCriticalSection();
        newRegValue = CY_GET_REG32(CapSense_P2_EXT_CAP_PC_PTR);
        newRegValue &= (uint32)(~((uint32)CapSense_P2_EXT_CAP_PC_MASK));
        newRegValue |= (CapSense_P2_EXT_CAP_PC_STRONG_CFG);
        CY_SET_REG32(CapSense_P2_EXT_CAP_PC_PTR, newRegValue);
        CyExitCriticalSection(interruptState);

        /* Delay to discharge external capacitors */
        CyDelayUs((uint16)dischargeTime);

        /* Restore previous register values */
        interruptState = CyEnterCriticalSection();
        newRegValue = CY_GET_REG32(CapSense_P2_EXT_CAP_PC_PTR);
        newRegValue &= (uint32)(~((uint32)CapSense_P2_EXT_CAP_PC_MASK));
        newRegValue |= pcReg;
        CY_SET_REG32(CapSense_P2_EXT_CAP_PC_PTR, newRegValue);
        CyExitCriticalSection(interruptState);

        interruptState = CyEnterCriticalSection();
        newRegValue = CY_GET_REG32(CapSense_P2_EXT_CAP_DR_PTR);
        newRegValue &= (uint32)(~(uint32)CapSense_P2_EXT_CAP_DR_MASK);
        newRegValue |= drReg;
        CY_SET_REG32(CapSense_P2_EXT_CAP_DR_PTR, newRegValue);
        CyExitCriticalSection(interruptState);

        interruptState = CyEnterCriticalSection();
        newRegValue = CY_GET_REG32(CapSense_P2_EXT_CAP_HSIOM_PTR);
        newRegValue &= (uint32)(~(uint32)CapSense_P2_EXT_CAP_HSIOM_MASK);
        newRegValue |= hsiomReg;
        CY_SET_REG32(CapSense_P2_EXT_CAP_HSIOM_PTR, newRegValue);
        CyExitCriticalSection(interruptState);
    }

#endif /* (CapSense_P2_ENABLE == CapSense_P2_CSDV2) */


#if (CapSense_P2_ENABLE == CapSense_P2_CSDV1)
    /*******************************************************************************
    * Function Name: CapSense_P2_DischargeExtCapacitors
    ****************************************************************************//**
    *
    * \brief
    *  The internal function discharge available external capacitors.
    *
    * \details
    *  The function discharge available external capacitors by connection them
    *  to GND using STRONG GPIO drive mode. Additionaly, the function disconnects
    *  the capacitors from analog mux buses if connected.
    *  Note: the function does not restore the connection to analog mux busses
    *  and supposes that all the capacitors belong to a single device port.
    *
    * \param
    *  dischargeTime The discharge time in us.
    *
    *******************************************************************************/
    void CapSense_P2_DischargeExtCapacitors(uint32 dischargeTime)
    {
        /* Store the pin configuration, the CMOD capacitor is connected to. */
        #if (CapSense_P2_ENABLE == CapSense_P2_CSD_EN)
            uint32 cmodHsiom = CY_GET_REG32(CapSense_P2_CMOD_HSIOM_PTR) & CapSense_P2_EXT_CAP_HSIOM_MASK_CMOD;
            uint32 cmodDr = CY_GET_REG32(CapSense_P2_CMOD_DR_PTR) & CapSense_P2_EXT_CAP_DR_MASK_CMOD;
            uint32 cmodPc = CY_GET_REG32(CapSense_P2_CMOD_PC_PTR) & CapSense_P2_EXT_CAP_PC_MASK_CMOD;

            /* Store the pin configuration, the CSH capacitor is connected to. */
            #if((CapSense_P2_ENABLE == CapSense_P2_CSD_SHIELD_EN) &&\
                (CapSense_P2_ENABLE == CapSense_P2_CSD_SHIELD_TANK_EN))
                uint32 cshHsiom = CY_GET_REG32(CapSense_P2_CSH_HSIOM_PTR) & CapSense_P2_EXT_CAP_HSIOM_MASK_CSH;
                uint32 cshDr = CY_GET_REG32(CapSense_P2_CSH_DR_PTR) & CapSense_P2_EXT_CAP_DR_MASK_CSH;
                uint32 cshPc = CY_GET_REG32(CapSense_P2_CSH_PC_PTR) & CapSense_P2_EXT_CAP_PC_MASK_CSH;
            #endif
        #endif

        #if (CapSense_P2_ENABLE == CapSense_P2_CSX_EN)
            /* Store the pin configuration, the CintA capacitor is connected to. */
            uint32 cintAHsiom = CY_GET_REG32(CapSense_P2_CintA_HSIOM_PTR) & CapSense_P2_EXT_CAP_HSIOM_MASK_CINTA;
            uint32 cintADr = CY_GET_REG32(CapSense_P2_CintA_DR_PTR) & CapSense_P2_EXT_CAP_DR_MASK_CINTA;
            uint32 cintAPc = CY_GET_REG32(CapSense_P2_CintA_PC_PTR) & CapSense_P2_EXT_CAP_PC_MASK_CINTA;

            /* Store the pin configuration, the CintB capacitor is connected to. */
            uint32 cintBHsiom = CY_GET_REG32(CapSense_P2_CintB_HSIOM_PTR) & CapSense_P2_EXT_CAP_HSIOM_MASK_CINTB;
            uint32 cintBDr = CY_GET_REG32(CapSense_P2_CintB_DR_PTR) & CapSense_P2_EXT_CAP_DR_MASK_CINTB;
            uint32 cintBPc = CY_GET_REG32(CapSense_P2_CintB_PC_PTR) & CapSense_P2_EXT_CAP_PC_MASK_CINTB;
        #endif

        #if (CapSense_P2_ENABLE == CapSense_P2_CSD_EN)
            /* Disconnect the CMOD capacitor from the AMUXBUS and connect it to GND. */
            CapSense_P2_WriteBitsSafe(CapSense_P2_CMOD_HSIOM_PTR, CapSense_P2_EXT_CAP_HSIOM_MASK_CMOD,
                                           CapSense_P2_HSIOM_SEL_GPIO);
            CapSense_P2_WriteBitsSafe(CapSense_P2_CMOD_DR_PTR, CapSense_P2_EXT_CAP_DR_MASK_CMOD, 0u);
            CapSense_P2_WriteBitsSafe(CapSense_P2_CMOD_PC_PTR, CapSense_P2_EXT_CAP_PC_MASK_CMOD,
                                           CapSense_P2_EXT_CAP_PC_STRONG_CFG_CMOD);

            #if((CapSense_P2_ENABLE == CapSense_P2_CSD_SHIELD_EN) &&\
                (CapSense_P2_ENABLE == CapSense_P2_CSD_SHIELD_TANK_EN))
                /* Disconnect the CSH capacitor from the AMUXBUS and connect it to GND. */
                CapSense_P2_WriteBitsSafe(CapSense_P2_CSH_HSIOM_PTR, CapSense_P2_EXT_CAP_HSIOM_MASK_CSH,
                                               CapSense_P2_HSIOM_SEL_GPIO);
                CapSense_P2_WriteBitsSafe(CapSense_P2_CSH_DR_PTR, CapSense_P2_EXT_CAP_DR_MASK_CSH, 0u);
                CapSense_P2_WriteBitsSafe(CapSense_P2_CSH_PC_PTR, CapSense_P2_EXT_CAP_PC_MASK_CSH,
                                               CapSense_P2_EXT_CAP_PC_STRONG_CFG_CSH);
            #endif
        #endif

        #if (CapSense_P2_ENABLE == CapSense_P2_CSX_EN)
            /* Disconnect the CintA capacitor from the AMUXBUS and connect it to GND. */
            CapSense_P2_WriteBitsSafe(CapSense_P2_CintA_HSIOM_PTR, CapSense_P2_EXT_CAP_HSIOM_MASK_CINTA,
                                           CapSense_P2_HSIOM_SEL_GPIO);
            CapSense_P2_WriteBitsSafe(CapSense_P2_CintA_DR_PTR, CapSense_P2_EXT_CAP_DR_MASK_CINTA, 0u);
            CapSense_P2_WriteBitsSafe(CapSense_P2_CintA_PC_PTR, CapSense_P2_EXT_CAP_PC_MASK_CINTA,
                                           CapSense_P2_EXT_CAP_PC_STRONG_CFG_CINTA);

            /* Disconnect the CintB capacitor from the AMUXBUS and connect it to GND. */
            CapSense_P2_WriteBitsSafe(CapSense_P2_CintB_HSIOM_PTR, CapSense_P2_EXT_CAP_HSIOM_MASK_CINTB,
                                           CapSense_P2_HSIOM_SEL_GPIO);
            CapSense_P2_WriteBitsSafe(CapSense_P2_CintB_DR_PTR, CapSense_P2_EXT_CAP_DR_MASK_CINTB, 0u);
            CapSense_P2_WriteBitsSafe(CapSense_P2_CintB_PC_PTR, CapSense_P2_EXT_CAP_PC_MASK_CINTB,
                                           CapSense_P2_EXT_CAP_PC_STRONG_CFG_CINTB);
        #endif

        /* Delay to discharge external capacitors */
        CyDelayUs((uint16)dischargeTime);

        #if (CapSense_P2_ENABLE == CapSense_P2_CSD_EN)
            /* Restore the pin configuration, the CMOD capacitor is connected to. */
            CapSense_P2_WriteBitsSafe(CapSense_P2_CMOD_PC_PTR,
                                           CapSense_P2_EXT_CAP_PC_MASK_CMOD, cmodPc);
            CapSense_P2_WriteBitsSafe(CapSense_P2_CMOD_DR_PTR,
                                           CapSense_P2_EXT_CAP_DR_MASK_CMOD, cmodDr);
            CapSense_P2_WriteBitsSafe(CapSense_P2_CMOD_HSIOM_PTR,
                                           CapSense_P2_EXT_CAP_HSIOM_MASK_CMOD, cmodHsiom);

            #if((CapSense_P2_ENABLE == CapSense_P2_CSD_SHIELD_EN) &&\
                (CapSense_P2_ENABLE == CapSense_P2_CSD_SHIELD_TANK_EN))
                /* Restore the pin configuration, the CSH capacitor is connected to. */
                CapSense_P2_WriteBitsSafe(CapSense_P2_CSH_PC_PTR,
                                               CapSense_P2_EXT_CAP_PC_MASK_CSH, cshPc);
                CapSense_P2_WriteBitsSafe(CapSense_P2_CSH_DR_PTR,
                                               CapSense_P2_EXT_CAP_DR_MASK_CSH, cshDr);
                CapSense_P2_WriteBitsSafe(CapSense_P2_CSH_HSIOM_PTR,
                                               CapSense_P2_EXT_CAP_HSIOM_MASK_CSH, cshHsiom);
            #endif
        #endif

        #if (CapSense_P2_ENABLE == CapSense_P2_CSX_EN)
            /* Restore the pin configuration, the CintA capacitor is connected to. */
            CapSense_P2_WriteBitsSafe(CapSense_P2_CintA_PC_PTR,
                                           CapSense_P2_EXT_CAP_PC_MASK_CINTA, cintAPc);
            CapSense_P2_WriteBitsSafe(CapSense_P2_CintA_DR_PTR,
                                           CapSense_P2_EXT_CAP_DR_MASK_CINTA, cintADr);
            CapSense_P2_WriteBitsSafe(CapSense_P2_CintA_HSIOM_PTR,
                                           CapSense_P2_EXT_CAP_HSIOM_MASK_CINTA, cintAHsiom);

            /* Restore the pin configuration, the CintB capacitor is connected to. */
            CapSense_P2_WriteBitsSafe(CapSense_P2_CintB_PC_PTR,
                                           CapSense_P2_EXT_CAP_PC_MASK_CINTB, cintBPc);
            CapSense_P2_WriteBitsSafe(CapSense_P2_CintB_DR_PTR,
                                           CapSense_P2_EXT_CAP_DR_MASK_CINTB, cintBDr);
            CapSense_P2_WriteBitsSafe(CapSense_P2_CintB_HSIOM_PTR,
                                           CapSense_P2_EXT_CAP_HSIOM_MASK_CINTB, cintBHsiom);
        #endif
    }
#endif /* (CapSense_P2_ENABLE == CapSense_P2_CSDV1) */


#if((CapSense_P2_ENABLE == CapSense_P2_CSD_EN) ||\
    (CapSense_P2_ENABLE == CapSense_P2_CSX_EN))
    /*******************************************************************************
    * Function Name: CapSense_P2_SetInactiveElectrodeState
    ****************************************************************************//**
    *
    * \brief
    *  The function updates the RAM data structure with the desired state of inactive
    *  electrodes for the specified operation mode. The state of pins is not changed
    *  in scope of this routine.
    *
    * \details
    *  The function updates the following registers of RAM data structure:
    *   - CapSense_P2_SCAN_CSD_ISC_VALUE - Connection of inactive CSD and CSX
    *     electrodes during the regular CSD scan. By default, this register is initialized
    *     with the value of Inactive Sensor Connection combobox on the CSD Settings tab.
    *     The CapSense_P2_SCAN_CSD_E value should be used as the Mode parameter
    *     to update this register.
    *
    *   - CapSense_P2_SCAN_CSX_ISC_VALUE - Connection of inactive CSD, CSX and
    *     the dedicated Shield electrodes during the regular CSX scan. By default, this
    *     register is initialized with the value of Inactive Sensor Connection combobox
    *     on the CSX Settings tab.
    *     The CapSense_P2_SCAN_CSX_E value should be used as the Mode parameter
    *     to update this register.
    *
    *   - CapSense_P2_BIST_CSD_SNS_CAP_ISC_VALUE - Connection of inactive CSD and CSX
    *     electrodes during measurement of CSD electrodes capacitance.
    *     This register is initialized with the CapSense_P2_SNS_CONNECTION_GROUND value
    *     by default.
    *     The CapSense_P2_BIST_CSD_SNS_CAP_E value should be used as the Mode parameter
    *     to update this register.
    *
    *   - CapSense_P2_BIST_CSX_SNS_CAP_ISC_VALUE - Connection of inactive CSD, CSX and
    *     the dedicated Shield electrodes during measurement of CSX electrodes (Tx and Rx)
    *     capacitance. This register is initialized with the CapSense_P2_SNS_CONNECTION_GROUND
    *     value by default.
    *     The CapSense_P2_BIST_CSX_SNS_CAP_E value should be used as the Mode parameter
    *     to update this register.
    *
    *   - CapSense_P2_BIST_CSD_SH_CAP_ISC_VALUE - Connection of inactive CSD and CSX electrodes
    *     measurement of dedicated Shield  electrodes capacitance. This register is initialized with
    *     the CapSense_P2_SNS_CONNECTION_GROUND value by default.
    *     The CapSense_P2_BIST_CSD_SH_CAP_E value should be used as the Mode parameter
    *     to update this register.
    *
    * \param
    *  mode Operation mode, the state of inactive sensors should be configured for.
    *       This parameter can take the following values:
    *        - CapSense_P2_SCAN_CSD_E - Regular CSD scan.
    *        - CapSense_P2_SCAN_CSX_E - Regular CSX scan.
    *        - CapSense_P2_BIST_CSD_SNS_CAP_E - Measurement of the CSD sensor capacitance.
    *        - CapSense_P2_BIST_CSX_SNS_CAP_E - Measurement of the CSX electrode capacitance.
    *        - CapSense_P2_BIST_CSD_SH_CAP_E - Measurement of the dedicated CSD Shield
    *          electrode capacitance.
    *
    * \param
    *  state The desired state of inactive sensors.
    *        This parameter can take the following values:
    *         - CapSense_P2_SNS_CONNECTION_GROUND - Inactive sensors are connected to the ground.
    *         - CapSense_P2_SNS_CONNECTION_HIGHZ - Inactive sensors are floating (not
    *           connected to GND or Shield).
    *         - CapSense_P2_SNS_CONNECTION_SHIELD - Inactive sensors are connected to the shield.
    *           This option is available only if the Enable shield electrode check box is set.
    *           At least one dedicated shield electrode is required to use the
    *           CapSense_P2_SNS_CONNECTION_SHIELD option for the CapSense_P2_BIST_CSD_SH_CAP_E
    *           operation mode.
    *
    * \return
    *  Returns the status of the operation:
    *  - CYRET_SUCCESS - The operation was successfully completed.
    *  - CYRET_BAD_PARAM - The input parameter is invalid.
    *
    *******************************************************************************/
    uint32_t CapSense_P2_SetInactiveElectrodeState(CapSense_P2_OPERATION_MODE_ENUM mode, uint32_t state)
    {
        uint32_t retVal = CYRET_SUCCESS;

        switch(mode)
        {
        #if(CapSense_P2_ENABLE == CapSense_P2_CSD_EN)
            case CapSense_P2_SCAN_CSD_E:
                CapSense_P2_dsRam.scanCsdISC = (uint8)state;
                break;
        #endif

        #if(CapSense_P2_ENABLE == CapSense_P2_CSX_EN)
            case CapSense_P2_SCAN_CSX_E:
                CapSense_P2_dsRam.scanCsxISC = (uint8)state;
                break;
        #endif

        #if((CapSense_P2_ENABLE == CapSense_P2_SELF_TEST_EN) &&\
            (CapSense_P2_ENABLE == CapSense_P2_TST_SNS_CAP_EN) &&\
            (CapSense_P2_ENABLE == CapSense_P2_CSD_EN))
            case CapSense_P2_BIST_CSD_SNS_CAP_E:
                CapSense_P2_dsRam.selfTestConfig.bistCsdSnsCapISC = (uint8)state;
                break;
        #endif

        #if((CapSense_P2_ENABLE == CapSense_P2_SELF_TEST_EN) &&\
            (CapSense_P2_ENABLE == CapSense_P2_TST_SNS_CAP_EN) &&\
            (CapSense_P2_ENABLE == CapSense_P2_CSX_EN))
            case CapSense_P2_BIST_CSX_SNS_CAP_E:
                CapSense_P2_dsRam.selfTestConfig.bistCsxSnsCapISC = (uint8)state;
                break;
        #endif

        #if((CapSense_P2_ENABLE == CapSense_P2_SELF_TEST_EN) &&\
            (CapSense_P2_ENABLE == CapSense_P2_TST_SH_CAP_EN) &&\
            (CapSense_P2_ENABLE == CapSense_P2_CSD_EN) &&\
            (CapSense_P2_ENABLE == CapSense_P2_CSD_SHIELD_EN))
            case CapSense_P2_BIST_CSD_SH_CAP_E:
                if((CapSense_P2_CSD_TOTAL_SHIELD_COUNT == 0u) &&
                   (CapSense_P2_SNS_CONNECTION_SHIELD != state))
                {
                    retVal = CYRET_BAD_PARAM;
                }
                else
                {
                    CapSense_P2_dsRam.selfTestConfig.bistCsdShCapISC = (uint8)state;
                }
                break;
        #endif

        default:
            CYASSERT(0u);
            retVal = CYRET_BAD_PARAM;
            break;
        }

        return(retVal);
    }
#endif /* ((CapSense_P2_ENABLE == CapSense_P2_CSD_EN) ||\
           (CapSense_P2_ENABLE == CapSense_P2_CSX_EN)) */


/*******************************************************************************
* Function Name: CapSense_P2_WriteBitsSafe
****************************************************************************//**
*
* \brief
*  Writes a field in port-pin register, specified by the mask parameter
*  with the specified value in critical section.
*
* \details
*  Writes a field in port-pin register, specified by the mask parameter
*  with the specified value in critical section.
*
* \param regPtr
*  Specifies a pointer to port-pin register.
*
* \param mask
*  Specifies bits to be modified.
*
* \param value
*  Specifies a value to be written to the register.
*
*******************************************************************************/
void CapSense_P2_WriteBitsSafe(reg32 * regPtr, uint32 mask, uint32 value)
{
    uint8 interruptState = CyEnterCriticalSection();
    (* regPtr) = ((* regPtr) & ((uint32)~mask)) | (value & mask);
    CyExitCriticalSection(interruptState);
}


/* [] END OF FILE */
