/***************************************************************************//**
* \file CapSense_P2_Centroid_LL.h
* \version 7.0
*
* \brief
*   This file provides the function prototypes for the centroid calculation
*   methods.
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

#if !defined(CY_SENSE_CapSense_P2_CENTROID_LL_H)
#define CY_SENSE_CapSense_P2_CENTROID_LL_H

#include "CapSense_P2_Configuration.h"
#include "CapSense_P2_Structure.h"

/*******************************************************************************
* Definitions
*******************************************************************************/

/* Centroid-related definitions */
#define CapSense_P2_CENTROID_POS_PREV                  (0u)
#define CapSense_P2_CENTROID_POS                       (1u)
#define CapSense_P2_CENTROID_POS_NEXT                  (2u)
#define CapSense_P2_CENTROID_ROUND_VALUE               (0x7Fu)
#define CapSense_P2_CENTROID_MAXIMUM                   (0xFFu)
#define CapSense_P2_CENTROID_DIPLEX_SECTION_LENGTH     (0x02u)

/*******************************************************************************
* Function Prototypes
*******************************************************************************/

/*******************************************************************************
* Function Prototypes - internal functions
*******************************************************************************/
/**
* \cond SECTION_C_INTERNAL
* \addtogroup group_c_internal
* \{
*/

#if ((CapSense_P2_ENABLE == CapSense_P2_CENTROID_3X3_CSD_EN) || \
     (CapSense_P2_ENABLE  == CapSense_P2_SLIDER_WIDGET_EN))
    #if (CapSense_P2_ENABLE == CapSense_P2_4PTS_LOCAL_MAX_EN)
        uint32 CapSense_P2_DpFindLocalMaxSd4pts(CapSense_P2_FLASH_WD_STRUCT const *ptrFlashWdgt,
                                                     CapSense_P2_RAM_SNS_STRUCT ptrSns[],
                                                     uint32 snsCount, uint32 fingerThreshold);
    #else
        uint32 CapSense_P2_DpFindLocalMaxSd(CapSense_P2_RAM_SNS_STRUCT *ptrSns,
                                                 uint32 snsCount, uint32 fingerThreshold);
    #endif
#endif

#if (0u != CapSense_P2_TOTAL_DIPLEXED_SLIDERS)
    uint32 CapSense_P2_DpFindLocalMaxDiplex(
                    CapSense_P2_RAM_SNS_STRUCT *ptrSns,
                    uint32 snsCount,
                    uint8 const *diplexTable,
                    uint32 fingerThreshold);
#endif

#if ((0u != CapSense_P2_TOTAL_LINEAR_SLIDERS) || \
     (CapSense_P2_ENABLE == CapSense_P2_CENTROID_3X3_CSD_EN))
    uint32 CapSense_P2_DpCalcLinearCentroid(uint16 *position, uint32 multiplier, uint32 snsCount, uint32 offset);
#endif

#if (0u != CapSense_P2_TOTAL_RADIAL_SLIDERS)
    uint32 CapSense_P2_DpCalcRadialCentroid(uint16 *position, uint32 multiplier, uint32 snsCount);
#endif

#if (CapSense_P2_ENABLE == CapSense_P2_CSX_TOUCHPAD_WIDGET_EN)
    void CapSense_P2_DpFindLocalMaxDd(CapSense_P2_FLASH_WD_STRUCT const *ptrFlashWdgt);
    void CapSense_P2_DpCalcTouchPadCentroid(CapSense_P2_FLASH_WD_STRUCT const *ptrFlashWdgt);
    void CapSense_P2_DpTouchTracking(CapSense_P2_FLASH_WD_STRUCT const *ptrFlashWdgt);
    uint32 CapSense_P2_DpFilterTouchRecord(CapSense_P2_FLASH_WD_STRUCT const *ptrFlashWdgt);
#endif

#if (0u != CapSense_P2_POSITION_FILTER_EN)
    void CapSense_P2_InitPosFiltersSd(CapSense_P2_SLIDER_POS_HISTORY_STRUCT *ptrHistory, uint16 value);
    #if ((0u != CapSense_P2_CSD_TOUCHPAD_WIDGET_EN) || (0u != CapSense_P2_CSX_TOUCHPAD_WIDGET_EN))
        void CapSense_P2_InitPosFiltersDd(CapSense_P2_TOUCHPAD_POS_HISTORY_STRUCT *ptrHistory, uint16 posX, uint16 posY);
        void CapSense_P2_RunPosFiltersDd(CapSense_P2_FLASH_WD_STRUCT const *ptrFlashWdgt, uint32 i, uint32 j,
                    uint16 * posX, uint16 * posY);
    #endif /* ((0u != CapSense_P2_CSD_TOUCHPAD_WIDGET_EN) || (0u != CapSense_P2_CSX_TOUCHPAD_WIDGET_EN)) */
#endif /* (0u != CapSense_P2_POSITION_FILTER_EN) */

/** \}
* \endcond */

#endif /* End CY_SENSE_CapSense_P2_CENTROID_LL_H */


/* [] END OF FILE */
