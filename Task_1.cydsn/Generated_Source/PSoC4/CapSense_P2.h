/***************************************************************************//**
* \file CapSense_P2.h
* \version 7.0
*
* \brief
*   This file includes all header files of the CapSense_P2 Component modules.
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

#if !defined(CY_SENSE_CapSense_P2_H)

#define CY_SENSE_CapSense_P2_H

#include "CapSense_P2_Configuration.h"
#include "CapSense_P2_Control.h"
#include "CapSense_P2_Filter.h"
#include "CapSense_P2_Processing.h"
#include "CapSense_P2_Sensing.h"
#if (CapSense_P2_ENABLE == CapSense_P2_CSD_EN)
    #include "CapSense_P2_SensingCSD_LL.h"
#endif /* (CapSense_P2_ENABLE == CapSense_P2_CSD_EN) */
#if (CapSense_P2_ENABLE == CapSense_P2_CSX_EN)
    #include "CapSense_P2_SensingCSX_LL.h"
#endif /* (CapSense_P2_ENABLE == CapSense_P2_CSX_EN) */
#if (CapSense_P2_ENABLE == CapSense_P2_ISX_EN)
    #include "CapSense_P2_SensingISX_LL.h"
#endif /* (CapSense_P2_ENABLE == CapSense_P2_ISX_EN) */
#include "CapSense_P2_Structure.h"
#include "CapSense_P2_Tuner.h"

#endif /* End CY_SENSE_CapSense_P2_H */


/* [] END OF FILE */
