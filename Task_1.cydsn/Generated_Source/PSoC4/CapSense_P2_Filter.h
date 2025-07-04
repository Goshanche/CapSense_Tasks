/***************************************************************************//**
* \file CapSense_P2_Filter.h
* \version 7.0
*
* \brief
*   This file contains the definitions for all firmware filters
*   implementation.
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

#if !defined(CY_SENSE_CapSense_P2_FILTER_H)
#define CY_SENSE_CapSense_P2_FILTER_H

#include "cytypes.h"
#include "CyLib.h"
#include "cyfitter.h"

#include "CapSense_P2_Structure.h"
#include "CapSense_P2_Configuration.h"

/***************************************
* Function Prototypes
***************************************/

/*******************************************************************************
* LOW LEVEL API
*******************************************************************************/
/**
* \cond (SECTION_C_LOW_LEVEL || SECTION_I_LOW_LEVEL)
* \addtogroup group_c_low_level
* \{
*/

cystatus CapSense_P2_UpdateAllBaselines(void);
cystatus CapSense_P2_UpdateWidgetBaseline(uint32 widgetId);
cystatus CapSense_P2_UpdateSensorBaseline(uint32 widgetId, uint32 sensorId);

void CapSense_P2_InitializeAllBaselines(void);
void CapSense_P2_InitializeWidgetBaseline(uint32 widgetId);
void CapSense_P2_InitializeSensorBaseline(uint32 widgetId, uint32 sensorId);

#if ((CapSense_P2_ENABLE == CapSense_P2_RC_FILTER_EN) || \
     (0u != (CapSense_P2_CSD_AUTOTUNE & CapSense_P2_CSD_SS_TH_EN)))
    void CapSense_P2_InitializeAllFilters(void);
    void CapSense_P2_InitializeWidgetFilter(uint32 widgetId);
#endif /* ((CapSense_P2_ENABLE == CapSense_P2_RC_FILTER_EN) || \
           (0u != (CapSense_P2_CSD_AUTOTUNE & CapSense_P2_CSD_SS_TH_EN))) */

/** \}
* \endcond */


/*******************************************************************************
* Function Prototypes - internal functions
*******************************************************************************/
/**
* \cond SECTION_C_INTERNAL
* \addtogroup group_c_internal
* \{
*/

void CapSense_P2_FtInitialize(void);

#if (0u != (CapSense_P2_CSD_AUTOTUNE & CapSense_P2_CSD_SS_TH_EN))
    void CapSense_P2_RunNoiseEnvelope(uint32 widgetId, uint32 sensorId);
    void CapSense_P2_InitializeNoiseEnvelope(uint32 widgetId, uint32 sensorId);
#endif /* (0u != (CapSense_P2_CSD_AUTOTUNE & CapSense_P2_CSD_SS_TH_EN)) */

#if (CapSense_P2_REGULAR_RC_IIR_FILTER_EN || CapSense_P2_PROX_RC_IIR_FILTER_EN)
    void CapSense_P2_InitializeIIR(uint32 widgetId, uint32 sensorId);
    void CapSense_P2_RunIIR(uint32 widgetId, uint32 sensorId);
#endif /* (CapSense_P2_REGULAR_RC_IIR_FILTER_EN || CapSense_P2_PROX_RC_IIR_FILTER_EN) */

#if (CapSense_P2_REGULAR_RC_MEDIAN_FILTER_EN || CapSense_P2_PROX_RC_MEDIAN_FILTER_EN)
    void CapSense_P2_InitializeMedian(uint32 widgetId, uint32 sensorId);
    void CapSense_P2_RunMedian(uint32 widgetId, uint32 sensorId);
#endif /* (CapSense_P2_REGULAR_RC_MEDIAN_FILTER_EN || CapSense_P2_PROX_RC_MEDIAN_FILTER_EN) */

#if (CapSense_P2_REGULAR_RC_AVERAGE_FILTER_EN || CapSense_P2_PROX_RC_AVERAGE_FILTER_EN)
    void CapSense_P2_InitializeAverage(uint32 widgetId, uint32 sensorId);
    void CapSense_P2_RunAverage(uint32 widgetId, uint32 sensorId);
#endif /* (CapSense_P2_REGULAR_RC_AVERAGE_FILTER_EN || CapSense_P2_PROX_RC_AVERAGE_FILTER_EN) */

void CapSense_P2_FtInitializeBaseline(CapSense_P2_RAM_SNS_STRUCT *ptrSensor, uint32 wdType);
uint32 CapSense_P2_FtUpdateBaseline(
                            CapSense_P2_RAM_WD_BASE_STRUCT *ptrWidgetRam,
                            CapSense_P2_RAM_SNS_STRUCT *ptrSensor,
                            uint32 wdType);

#if (CapSense_P2_POS_MEDIAN_FILTER_EN || CapSense_P2_REGULAR_RC_MEDIAN_FILTER_EN || CapSense_P2_PROX_RC_MEDIAN_FILTER_EN)
uint32 CapSense_P2_FtMedian(uint32 x1, uint32 x2, uint32 x3);
#endif /*CapSense_P2_POS_MEDIAN_FILTER_EN || CapSense_P2_REGULAR_RC_MEDIAN_FILTER_EN || CapSense_P2_PROX_RC_MEDIAN_FILTER_EN*/

uint32 CapSense_P2_FtIIR1stOrder(uint32 input, uint32 prevOutput, uint32 n, uint32 shift);

#if (CapSense_P2_POS_JITTER_FILTER_EN)
    uint32 CapSense_P2_FtJitter(uint32 input, uint32 prevOutput);
#endif /* CapSense_P2_POS_JITTER_FILTER_EN */

void CapSense_P2_FtInitializeBaselineChannel(CapSense_P2_RAM_SNS_STRUCT *ptrSensor, uint32 wdType, uint32 channel);

#if (CapSense_P2_ENABLE == CapSense_P2_RC_FILTER_EN)
    void CapSense_P2_FtRunEnabledFilters(uint32 widgetId, uint32 sensorId);
    void CapSense_P2_FtRunEnabledFiltersInternal(CapSense_P2_PTR_FILTER_VARIANT ptrFilterHistObj,
                                                      CapSense_P2_RAM_SNS_STRUCT *ptrSensorObj, uint32 wdType);
#endif /* (CapSense_P2_ENABLE == CapSense_P2_RC_FILTER_EN) */


#if (CapSense_P2_REGULAR_RC_IIR_FILTER_EN || CapSense_P2_PROX_RC_IIR_FILTER_EN)
    void CapSense_P2_InitializeIIRInternal(CapSense_P2_PTR_FILTER_VARIANT ptrFilterHistObj,
                                                  CapSense_P2_RAM_SNS_STRUCT *ptrSensorObj, uint32 wdType);
    void CapSense_P2_RunIIRInternal(CapSense_P2_PTR_FILTER_VARIANT ptrFilterHistObj,
                                           CapSense_P2_RAM_SNS_STRUCT *ptrSensorObj, uint32 wdType);
#endif /* (CapSense_P2_REGULAR_RC_IIR_FILTER_EN || CapSense_P2_PROX_RC_IIR_FILTER_EN) */

#if (CapSense_P2_REGULAR_RC_MEDIAN_FILTER_EN || CapSense_P2_PROX_RC_MEDIAN_FILTER_EN)
    void CapSense_P2_InitializeMedianInternal(CapSense_P2_PTR_FILTER_VARIANT ptrFilterHistObj,
                                                     CapSense_P2_RAM_SNS_STRUCT *ptrSensorObj, uint32 wdType);
    void CapSense_P2_RunMedianInternal(CapSense_P2_PTR_FILTER_VARIANT ptrFilterHistObj,
                                              CapSense_P2_RAM_SNS_STRUCT *ptrSensorObj, uint32 wdType);
#endif /* (CapSense_P2_REGULAR_RC_MEDIAN_FILTER_EN || CapSense_P2_PROX_RC_MEDIAN_FILTER_EN) */

#if (CapSense_P2_REGULAR_RC_AVERAGE_FILTER_EN || CapSense_P2_PROX_RC_AVERAGE_FILTER_EN)
    void CapSense_P2_InitializeAverageInternal(CapSense_P2_PTR_FILTER_VARIANT ptrFilterHistObj,
                                                      CapSense_P2_RAM_SNS_STRUCT *ptrSensorObj, uint32 wdType);
    void CapSense_P2_RunAverageInternal(CapSense_P2_PTR_FILTER_VARIANT ptrFilterHistObj,
                                               CapSense_P2_RAM_SNS_STRUCT *ptrSensorObj, uint32 wdType);
#endif /* (CapSense_P2_REGULAR_RC_AVERAGE_FILTER_EN || CapSense_P2_PROX_RC_AVERAGE_FILTER_EN) */

#if (CapSense_P2_ENABLE == CapSense_P2_ALP_FILTER_EN)
    void CapSense_P2_InitializeALP(uint32 widgetId, uint32 sensorId);
    void CapSense_P2_RunALP(uint32 widgetId, uint32 sensorId);
    void CapSense_P2_InitializeALPInternal(CapSense_P2_PTR_FILTER_VARIANT ptrFilterHistObj,
                                                CapSense_P2_RAM_SNS_STRUCT *ptrSensorObj,
                                                uint32 wdType);
    void CapSense_P2_ConfigRunALPInternal(CapSense_P2_PTR_FILTER_VARIANT ptrFilterHistObj,
                                        CapSense_P2_RAM_WD_BASE_STRUCT *ptrRamWdgt,
                                        CapSense_P2_RAM_SNS_STRUCT *ptrSensorObj,
                                        uint32 wdType);
    void CapSense_P2_RunALPInternal(CapSense_P2_PTR_FILTER_VARIANT ptrFilterHistObj,
                                        ALP_FLTR_CONFIG_STRUCT *ptrAlpFilterConfig,
                                        CapSense_P2_RAM_SNS_STRUCT *ptrSensorObj,
                                        uint32 wdType);
#endif /* (CapSense_P2_ENABLE == CapSense_P2_ALP_FILTER_EN) */

/** \}
* \endcond */

/***************************************
* Initial Parameter Constants
***************************************/
#define NOISE_ENVELOPE_SHIFT                        (0x02u)
#define NOISE_ENVELOPE_RUN_DIFF_SHIFT               (0x03u)
#define NOISE_ENVELOPE_SIGN_REG                     (0x0Fu)
#define NOISE_ENVELOPE_SIGN_REG_SHIFT               (0x01u)
#define NOISE_ENVELOPE_RESET_COUNTER                (0x0Au)
#define NOISE_ENVELOPE_4_TIMES                      (0x02u)

#endif /* End CY_SENSE_CapSense_P2_FILTER_H */


/* [] END OF FILE */
