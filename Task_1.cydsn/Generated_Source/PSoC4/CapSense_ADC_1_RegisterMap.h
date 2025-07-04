/***************************************************************************//**
* \file CapSense_ADC_1_RegisterMap.h
* \version 7.0
*
* \brief
*   This file provides the definitions for the Component data structure register.
*
* \see CapSense_ADC_1 v7.0 Datasheet
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

#if !defined(CY_SENSE_CapSense_ADC_1_REGISTER_MAP_H)
#define CY_SENSE_CapSense_ADC_1_REGISTER_MAP_H

#include <cytypes.h>
#include "CapSense_ADC_1_Configuration.h"
#include "CapSense_ADC_1_Structure.h"

/*****************************************************************************/
/* RAM Data structure register definitions                                   */
/*****************************************************************************/
#define CapSense_ADC_1_CONFIG_ID_VALUE                      (CapSense_ADC_1_dsRam.configId)
#define CapSense_ADC_1_CONFIG_ID_OFFSET                     (0u)
#define CapSense_ADC_1_CONFIG_ID_SIZE                       (2u)
#define CapSense_ADC_1_CONFIG_ID_PARAM_ID                   (0x87000000u)

#define CapSense_ADC_1_DEVICE_ID_VALUE                      (CapSense_ADC_1_dsRam.deviceId)
#define CapSense_ADC_1_DEVICE_ID_OFFSET                     (2u)
#define CapSense_ADC_1_DEVICE_ID_SIZE                       (2u)
#define CapSense_ADC_1_DEVICE_ID_PARAM_ID                   (0x8B000002u)

#define CapSense_ADC_1_HW_CLOCK_VALUE                       (CapSense_ADC_1_dsRam.hwClock)
#define CapSense_ADC_1_HW_CLOCK_OFFSET                      (4u)
#define CapSense_ADC_1_HW_CLOCK_SIZE                        (2u)
#define CapSense_ADC_1_HW_CLOCK_PARAM_ID                    (0x86000004u)

#define CapSense_ADC_1_TUNER_CMD_VALUE                      (CapSense_ADC_1_dsRam.tunerCmd)
#define CapSense_ADC_1_TUNER_CMD_OFFSET                     (6u)
#define CapSense_ADC_1_TUNER_CMD_SIZE                       (2u)
#define CapSense_ADC_1_TUNER_CMD_PARAM_ID                   (0xA1000006u)

#define CapSense_ADC_1_SCAN_COUNTER_VALUE                   (CapSense_ADC_1_dsRam.scanCounter)
#define CapSense_ADC_1_SCAN_COUNTER_OFFSET                  (8u)
#define CapSense_ADC_1_SCAN_COUNTER_SIZE                    (2u)
#define CapSense_ADC_1_SCAN_COUNTER_PARAM_ID                (0x85000008u)

#define CapSense_ADC_1_STATUS_VALUE                         (CapSense_ADC_1_dsRam.status)
#define CapSense_ADC_1_STATUS_OFFSET                        (12u)
#define CapSense_ADC_1_STATUS_SIZE                          (4u)
#define CapSense_ADC_1_STATUS_PARAM_ID                      (0xCB00000Cu)

#define CapSense_ADC_1_WDGT_ENABLE0_VALUE                   (CapSense_ADC_1_dsRam.wdgtEnable[0u])
#define CapSense_ADC_1_WDGT_ENABLE0_OFFSET                  (16u)
#define CapSense_ADC_1_WDGT_ENABLE0_SIZE                    (4u)
#define CapSense_ADC_1_WDGT_ENABLE0_PARAM_ID                (0xE6000010u)

#define CapSense_ADC_1_WDGT_STATUS0_VALUE                   (CapSense_ADC_1_dsRam.wdgtStatus[0u])
#define CapSense_ADC_1_WDGT_STATUS0_OFFSET                  (20u)
#define CapSense_ADC_1_WDGT_STATUS0_SIZE                    (4u)
#define CapSense_ADC_1_WDGT_STATUS0_PARAM_ID                (0xCC000014u)

#define CapSense_ADC_1_SNS_STATUS0_VALUE                    (CapSense_ADC_1_dsRam.snsStatus[0u])
#define CapSense_ADC_1_SNS_STATUS0_OFFSET                   (24u)
#define CapSense_ADC_1_SNS_STATUS0_SIZE                     (1u)
#define CapSense_ADC_1_SNS_STATUS0_PARAM_ID                 (0x48000018u)

#define CapSense_ADC_1_ADC_RESULT0_VALUE                    (CapSense_ADC_1_dsRam.adcResult[0u])
#define CapSense_ADC_1_ADC_RESULT0_OFFSET                   (26u)
#define CapSense_ADC_1_ADC_RESULT0_SIZE                     (2u)
#define CapSense_ADC_1_ADC_RESULT0_PARAM_ID                 (0x8C00001Au)

#define CapSense_ADC_1_ADC_CODE0_VALUE                      (CapSense_ADC_1_dsRam.adcCode[0u])
#define CapSense_ADC_1_ADC_CODE0_OFFSET                     (28u)
#define CapSense_ADC_1_ADC_CODE0_SIZE                       (2u)
#define CapSense_ADC_1_ADC_CODE0_PARAM_ID                   (0x8100001Cu)

#define CapSense_ADC_1_ADC_STATUS_VALUE                     (CapSense_ADC_1_dsRam.adcStatus)
#define CapSense_ADC_1_ADC_STATUS_OFFSET                    (30u)
#define CapSense_ADC_1_ADC_STATUS_SIZE                      (1u)
#define CapSense_ADC_1_ADC_STATUS_PARAM_ID                  (0x4500001Eu)

#define CapSense_ADC_1_ADC_IDAC_VALUE                       (CapSense_ADC_1_dsRam.adcIdac)
#define CapSense_ADC_1_ADC_IDAC_OFFSET                      (31u)
#define CapSense_ADC_1_ADC_IDAC_SIZE                        (1u)
#define CapSense_ADC_1_ADC_IDAC_PARAM_ID                    (0x4300001Fu)

#define CapSense_ADC_1_ADC_ACTIVE_CH_VALUE                  (CapSense_ADC_1_dsRam.adcActiveCh)
#define CapSense_ADC_1_ADC_ACTIVE_CH_OFFSET                 (32u)
#define CapSense_ADC_1_ADC_ACTIVE_CH_SIZE                   (1u)
#define CapSense_ADC_1_ADC_ACTIVE_CH_PARAM_ID               (0x45000020u)

#define CapSense_ADC_1_CSD0_CONFIG_VALUE                    (CapSense_ADC_1_dsRam.csd0Config)
#define CapSense_ADC_1_CSD0_CONFIG_OFFSET                   (34u)
#define CapSense_ADC_1_CSD0_CONFIG_SIZE                     (2u)
#define CapSense_ADC_1_CSD0_CONFIG_PARAM_ID                 (0xA7800022u)

#define CapSense_ADC_1_MOD_CSD_CLK_VALUE                    (CapSense_ADC_1_dsRam.modCsdClk)
#define CapSense_ADC_1_MOD_CSD_CLK_OFFSET                   (36u)
#define CapSense_ADC_1_MOD_CSD_CLK_SIZE                     (1u)
#define CapSense_ADC_1_MOD_CSD_CLK_PARAM_ID                 (0x62800024u)

#define CapSense_ADC_1_ADC_RESOLUTION_VALUE                 (CapSense_ADC_1_dsRam.adcResolution)
#define CapSense_ADC_1_ADC_RESOLUTION_OFFSET                (37u)
#define CapSense_ADC_1_ADC_RESOLUTION_SIZE                  (1u)
#define CapSense_ADC_1_ADC_RESOLUTION_PARAM_ID              (0x4F800025u)

#define CapSense_ADC_1_ADC_AZTIME_VALUE                     (CapSense_ADC_1_dsRam.adcAzTime)
#define CapSense_ADC_1_ADC_AZTIME_OFFSET                    (38u)
#define CapSense_ADC_1_ADC_AZTIME_SIZE                      (1u)
#define CapSense_ADC_1_ADC_AZTIME_PARAM_ID                  (0x45800026u)

#define CapSense_ADC_1_LINEARSLIDER0_RESOLUTION_VALUE       (CapSense_ADC_1_dsRam.wdgtList.linearslider0.resolution)
#define CapSense_ADC_1_LINEARSLIDER0_RESOLUTION_OFFSET      (40u)
#define CapSense_ADC_1_LINEARSLIDER0_RESOLUTION_SIZE        (2u)
#define CapSense_ADC_1_LINEARSLIDER0_RESOLUTION_PARAM_ID    (0x82800028u)

#define CapSense_ADC_1_LINEARSLIDER0_FINGER_TH_VALUE        (CapSense_ADC_1_dsRam.wdgtList.linearslider0.fingerTh)
#define CapSense_ADC_1_LINEARSLIDER0_FINGER_TH_OFFSET       (42u)
#define CapSense_ADC_1_LINEARSLIDER0_FINGER_TH_SIZE         (2u)
#define CapSense_ADC_1_LINEARSLIDER0_FINGER_TH_PARAM_ID     (0x8E80002Au)

#define CapSense_ADC_1_LINEARSLIDER0_NOISE_TH_VALUE         (CapSense_ADC_1_dsRam.wdgtList.linearslider0.noiseTh)
#define CapSense_ADC_1_LINEARSLIDER0_NOISE_TH_OFFSET        (44u)
#define CapSense_ADC_1_LINEARSLIDER0_NOISE_TH_SIZE          (1u)
#define CapSense_ADC_1_LINEARSLIDER0_NOISE_TH_PARAM_ID      (0x4B80002Cu)

#define CapSense_ADC_1_LINEARSLIDER0_NNOISE_TH_VALUE        (CapSense_ADC_1_dsRam.wdgtList.linearslider0.nNoiseTh)
#define CapSense_ADC_1_LINEARSLIDER0_NNOISE_TH_OFFSET       (45u)
#define CapSense_ADC_1_LINEARSLIDER0_NNOISE_TH_SIZE         (1u)
#define CapSense_ADC_1_LINEARSLIDER0_NNOISE_TH_PARAM_ID     (0x4D80002Du)

#define CapSense_ADC_1_LINEARSLIDER0_HYSTERESIS_VALUE       (CapSense_ADC_1_dsRam.wdgtList.linearslider0.hysteresis)
#define CapSense_ADC_1_LINEARSLIDER0_HYSTERESIS_OFFSET      (46u)
#define CapSense_ADC_1_LINEARSLIDER0_HYSTERESIS_SIZE        (1u)
#define CapSense_ADC_1_LINEARSLIDER0_HYSTERESIS_PARAM_ID    (0x4780002Eu)

#define CapSense_ADC_1_LINEARSLIDER0_ON_DEBOUNCE_VALUE      (CapSense_ADC_1_dsRam.wdgtList.linearslider0.onDebounce)
#define CapSense_ADC_1_LINEARSLIDER0_ON_DEBOUNCE_OFFSET     (47u)
#define CapSense_ADC_1_LINEARSLIDER0_ON_DEBOUNCE_SIZE       (1u)
#define CapSense_ADC_1_LINEARSLIDER0_ON_DEBOUNCE_PARAM_ID   (0x4180002Fu)

#define CapSense_ADC_1_LINEARSLIDER0_LOW_BSLN_RST_VALUE     (CapSense_ADC_1_dsRam.wdgtList.linearslider0.lowBslnRst)
#define CapSense_ADC_1_LINEARSLIDER0_LOW_BSLN_RST_OFFSET    (48u)
#define CapSense_ADC_1_LINEARSLIDER0_LOW_BSLN_RST_SIZE      (1u)
#define CapSense_ADC_1_LINEARSLIDER0_LOW_BSLN_RST_PARAM_ID  (0x4D800030u)

#define CapSense_ADC_1_LINEARSLIDER0_IDAC_MOD0_VALUE        (CapSense_ADC_1_dsRam.wdgtList.linearslider0.idacMod[0u])
#define CapSense_ADC_1_LINEARSLIDER0_IDAC_MOD0_OFFSET       (49u)
#define CapSense_ADC_1_LINEARSLIDER0_IDAC_MOD0_SIZE         (1u)
#define CapSense_ADC_1_LINEARSLIDER0_IDAC_MOD0_PARAM_ID     (0x46000031u)

#define CapSense_ADC_1_LINEARSLIDER0_IDAC_GAIN_INDEX_VALUE  (CapSense_ADC_1_dsRam.wdgtList.linearslider0.idacGainIndex)
#define CapSense_ADC_1_LINEARSLIDER0_IDAC_GAIN_INDEX_OFFSET (50u)
#define CapSense_ADC_1_LINEARSLIDER0_IDAC_GAIN_INDEX_SIZE   (1u)
#define CapSense_ADC_1_LINEARSLIDER0_IDAC_GAIN_INDEX_PARAM_ID (0x41800032u)

#define CapSense_ADC_1_LINEARSLIDER0_SNS_CLK_VALUE          (CapSense_ADC_1_dsRam.wdgtList.linearslider0.snsClk)
#define CapSense_ADC_1_LINEARSLIDER0_SNS_CLK_OFFSET         (52u)
#define CapSense_ADC_1_LINEARSLIDER0_SNS_CLK_SIZE           (2u)
#define CapSense_ADC_1_LINEARSLIDER0_SNS_CLK_PARAM_ID       (0x84800034u)

#define CapSense_ADC_1_LINEARSLIDER0_SNS_CLK_SOURCE_VALUE   (CapSense_ADC_1_dsRam.wdgtList.linearslider0.snsClkSource)
#define CapSense_ADC_1_LINEARSLIDER0_SNS_CLK_SOURCE_OFFSET  (54u)
#define CapSense_ADC_1_LINEARSLIDER0_SNS_CLK_SOURCE_SIZE    (1u)
#define CapSense_ADC_1_LINEARSLIDER0_SNS_CLK_SOURCE_PARAM_ID (0x40800036u)

#define CapSense_ADC_1_LINEARSLIDER0_FINGER_CAP_VALUE       (CapSense_ADC_1_dsRam.wdgtList.linearslider0.fingerCap)
#define CapSense_ADC_1_LINEARSLIDER0_FINGER_CAP_OFFSET      (56u)
#define CapSense_ADC_1_LINEARSLIDER0_FINGER_CAP_SIZE        (2u)
#define CapSense_ADC_1_LINEARSLIDER0_FINGER_CAP_PARAM_ID    (0xA1000038u)

#define CapSense_ADC_1_LINEARSLIDER0_SIGPFC_VALUE           (CapSense_ADC_1_dsRam.wdgtList.linearslider0.sigPFC)
#define CapSense_ADC_1_LINEARSLIDER0_SIGPFC_OFFSET          (58u)
#define CapSense_ADC_1_LINEARSLIDER0_SIGPFC_SIZE            (2u)
#define CapSense_ADC_1_LINEARSLIDER0_SIGPFC_PARAM_ID        (0xAD00003Au)

#define CapSense_ADC_1_LINEARSLIDER0_POSITION0_VALUE        (CapSense_ADC_1_dsRam.wdgtList.linearslider0.position[0u])
#define CapSense_ADC_1_LINEARSLIDER0_POSITION0_OFFSET       (60u)
#define CapSense_ADC_1_LINEARSLIDER0_POSITION0_SIZE         (2u)
#define CapSense_ADC_1_LINEARSLIDER0_POSITION0_PARAM_ID     (0x8B00003Cu)

#define CapSense_ADC_1_LINEARSLIDER0_SNS0_RAW0_VALUE        (CapSense_ADC_1_dsRam.snsList.linearslider0[0u].raw[0u])
#define CapSense_ADC_1_LINEARSLIDER0_SNS0_RAW0_OFFSET       (62u)
#define CapSense_ADC_1_LINEARSLIDER0_SNS0_RAW0_SIZE         (2u)
#define CapSense_ADC_1_LINEARSLIDER0_SNS0_RAW0_PARAM_ID     (0x8700003Eu)

#define CapSense_ADC_1_LINEARSLIDER0_SNS0_BSLN0_VALUE       (CapSense_ADC_1_dsRam.snsList.linearslider0[0u].bsln[0u])
#define CapSense_ADC_1_LINEARSLIDER0_SNS0_BSLN0_OFFSET      (64u)
#define CapSense_ADC_1_LINEARSLIDER0_SNS0_BSLN0_SIZE        (2u)
#define CapSense_ADC_1_LINEARSLIDER0_SNS0_BSLN0_PARAM_ID    (0x8A000040u)

#define CapSense_ADC_1_LINEARSLIDER0_SNS0_BSLN_EXT0_VALUE   (CapSense_ADC_1_dsRam.snsList.linearslider0[0u].bslnExt[0u])
#define CapSense_ADC_1_LINEARSLIDER0_SNS0_BSLN_EXT0_OFFSET  (66u)
#define CapSense_ADC_1_LINEARSLIDER0_SNS0_BSLN_EXT0_SIZE    (1u)
#define CapSense_ADC_1_LINEARSLIDER0_SNS0_BSLN_EXT0_PARAM_ID (0x4E000042u)

#define CapSense_ADC_1_LINEARSLIDER0_SNS0_DIFF_VALUE        (CapSense_ADC_1_dsRam.snsList.linearslider0[0u].diff)
#define CapSense_ADC_1_LINEARSLIDER0_SNS0_DIFF_OFFSET       (68u)
#define CapSense_ADC_1_LINEARSLIDER0_SNS0_DIFF_SIZE         (2u)
#define CapSense_ADC_1_LINEARSLIDER0_SNS0_DIFF_PARAM_ID     (0x8B000044u)

#define CapSense_ADC_1_LINEARSLIDER0_SNS0_NEG_BSLN_RST_CNT0_VALUE (CapSense_ADC_1_dsRam.snsList.linearslider0[0u].negBslnRstCnt[0u])
#define CapSense_ADC_1_LINEARSLIDER0_SNS0_NEG_BSLN_RST_CNT0_OFFSET (70u)
#define CapSense_ADC_1_LINEARSLIDER0_SNS0_NEG_BSLN_RST_CNT0_SIZE (1u)
#define CapSense_ADC_1_LINEARSLIDER0_SNS0_NEG_BSLN_RST_CNT0_PARAM_ID (0x4F000046u)

#define CapSense_ADC_1_LINEARSLIDER0_SNS0_IDAC_COMP0_VALUE  (CapSense_ADC_1_dsRam.snsList.linearslider0[0u].idacComp[0u])
#define CapSense_ADC_1_LINEARSLIDER0_SNS0_IDAC_COMP0_OFFSET (71u)
#define CapSense_ADC_1_LINEARSLIDER0_SNS0_IDAC_COMP0_SIZE   (1u)
#define CapSense_ADC_1_LINEARSLIDER0_SNS0_IDAC_COMP0_PARAM_ID (0x49000047u)

#define CapSense_ADC_1_LINEARSLIDER0_SNS1_RAW0_VALUE        (CapSense_ADC_1_dsRam.snsList.linearslider0[1u].raw[0u])
#define CapSense_ADC_1_LINEARSLIDER0_SNS1_RAW0_OFFSET       (72u)
#define CapSense_ADC_1_LINEARSLIDER0_SNS1_RAW0_SIZE         (2u)
#define CapSense_ADC_1_LINEARSLIDER0_SNS1_RAW0_PARAM_ID     (0x88000048u)

#define CapSense_ADC_1_LINEARSLIDER0_SNS1_BSLN0_VALUE       (CapSense_ADC_1_dsRam.snsList.linearslider0[1u].bsln[0u])
#define CapSense_ADC_1_LINEARSLIDER0_SNS1_BSLN0_OFFSET      (74u)
#define CapSense_ADC_1_LINEARSLIDER0_SNS1_BSLN0_SIZE        (2u)
#define CapSense_ADC_1_LINEARSLIDER0_SNS1_BSLN0_PARAM_ID    (0x8400004Au)

#define CapSense_ADC_1_LINEARSLIDER0_SNS1_BSLN_EXT0_VALUE   (CapSense_ADC_1_dsRam.snsList.linearslider0[1u].bslnExt[0u])
#define CapSense_ADC_1_LINEARSLIDER0_SNS1_BSLN_EXT0_OFFSET  (76u)
#define CapSense_ADC_1_LINEARSLIDER0_SNS1_BSLN_EXT0_SIZE    (1u)
#define CapSense_ADC_1_LINEARSLIDER0_SNS1_BSLN_EXT0_PARAM_ID (0x4100004Cu)

#define CapSense_ADC_1_LINEARSLIDER0_SNS1_DIFF_VALUE        (CapSense_ADC_1_dsRam.snsList.linearslider0[1u].diff)
#define CapSense_ADC_1_LINEARSLIDER0_SNS1_DIFF_OFFSET       (78u)
#define CapSense_ADC_1_LINEARSLIDER0_SNS1_DIFF_SIZE         (2u)
#define CapSense_ADC_1_LINEARSLIDER0_SNS1_DIFF_PARAM_ID     (0x8500004Eu)

#define CapSense_ADC_1_LINEARSLIDER0_SNS1_NEG_BSLN_RST_CNT0_VALUE (CapSense_ADC_1_dsRam.snsList.linearslider0[1u].negBslnRstCnt[0u])
#define CapSense_ADC_1_LINEARSLIDER0_SNS1_NEG_BSLN_RST_CNT0_OFFSET (80u)
#define CapSense_ADC_1_LINEARSLIDER0_SNS1_NEG_BSLN_RST_CNT0_SIZE (1u)
#define CapSense_ADC_1_LINEARSLIDER0_SNS1_NEG_BSLN_RST_CNT0_PARAM_ID (0x47000050u)

#define CapSense_ADC_1_LINEARSLIDER0_SNS1_IDAC_COMP0_VALUE  (CapSense_ADC_1_dsRam.snsList.linearslider0[1u].idacComp[0u])
#define CapSense_ADC_1_LINEARSLIDER0_SNS1_IDAC_COMP0_OFFSET (81u)
#define CapSense_ADC_1_LINEARSLIDER0_SNS1_IDAC_COMP0_SIZE   (1u)
#define CapSense_ADC_1_LINEARSLIDER0_SNS1_IDAC_COMP0_PARAM_ID (0x41000051u)

#define CapSense_ADC_1_LINEARSLIDER0_SNS2_RAW0_VALUE        (CapSense_ADC_1_dsRam.snsList.linearslider0[2u].raw[0u])
#define CapSense_ADC_1_LINEARSLIDER0_SNS2_RAW0_OFFSET       (82u)
#define CapSense_ADC_1_LINEARSLIDER0_SNS2_RAW0_SIZE         (2u)
#define CapSense_ADC_1_LINEARSLIDER0_SNS2_RAW0_PARAM_ID     (0x83000052u)

#define CapSense_ADC_1_LINEARSLIDER0_SNS2_BSLN0_VALUE       (CapSense_ADC_1_dsRam.snsList.linearslider0[2u].bsln[0u])
#define CapSense_ADC_1_LINEARSLIDER0_SNS2_BSLN0_OFFSET      (84u)
#define CapSense_ADC_1_LINEARSLIDER0_SNS2_BSLN0_SIZE        (2u)
#define CapSense_ADC_1_LINEARSLIDER0_SNS2_BSLN0_PARAM_ID    (0x8E000054u)

#define CapSense_ADC_1_LINEARSLIDER0_SNS2_BSLN_EXT0_VALUE   (CapSense_ADC_1_dsRam.snsList.linearslider0[2u].bslnExt[0u])
#define CapSense_ADC_1_LINEARSLIDER0_SNS2_BSLN_EXT0_OFFSET  (86u)
#define CapSense_ADC_1_LINEARSLIDER0_SNS2_BSLN_EXT0_SIZE    (1u)
#define CapSense_ADC_1_LINEARSLIDER0_SNS2_BSLN_EXT0_PARAM_ID (0x4A000056u)

#define CapSense_ADC_1_LINEARSLIDER0_SNS2_DIFF_VALUE        (CapSense_ADC_1_dsRam.snsList.linearslider0[2u].diff)
#define CapSense_ADC_1_LINEARSLIDER0_SNS2_DIFF_OFFSET       (88u)
#define CapSense_ADC_1_LINEARSLIDER0_SNS2_DIFF_SIZE         (2u)
#define CapSense_ADC_1_LINEARSLIDER0_SNS2_DIFF_PARAM_ID     (0x8D000058u)

#define CapSense_ADC_1_LINEARSLIDER0_SNS2_NEG_BSLN_RST_CNT0_VALUE (CapSense_ADC_1_dsRam.snsList.linearslider0[2u].negBslnRstCnt[0u])
#define CapSense_ADC_1_LINEARSLIDER0_SNS2_NEG_BSLN_RST_CNT0_OFFSET (90u)
#define CapSense_ADC_1_LINEARSLIDER0_SNS2_NEG_BSLN_RST_CNT0_SIZE (1u)
#define CapSense_ADC_1_LINEARSLIDER0_SNS2_NEG_BSLN_RST_CNT0_PARAM_ID (0x4900005Au)

#define CapSense_ADC_1_LINEARSLIDER0_SNS2_IDAC_COMP0_VALUE  (CapSense_ADC_1_dsRam.snsList.linearslider0[2u].idacComp[0u])
#define CapSense_ADC_1_LINEARSLIDER0_SNS2_IDAC_COMP0_OFFSET (91u)
#define CapSense_ADC_1_LINEARSLIDER0_SNS2_IDAC_COMP0_SIZE   (1u)
#define CapSense_ADC_1_LINEARSLIDER0_SNS2_IDAC_COMP0_PARAM_ID (0x4F00005Bu)

#define CapSense_ADC_1_LINEARSLIDER0_SNS3_RAW0_VALUE        (CapSense_ADC_1_dsRam.snsList.linearslider0[3u].raw[0u])
#define CapSense_ADC_1_LINEARSLIDER0_SNS3_RAW0_OFFSET       (92u)
#define CapSense_ADC_1_LINEARSLIDER0_SNS3_RAW0_SIZE         (2u)
#define CapSense_ADC_1_LINEARSLIDER0_SNS3_RAW0_PARAM_ID     (0x8C00005Cu)

#define CapSense_ADC_1_LINEARSLIDER0_SNS3_BSLN0_VALUE       (CapSense_ADC_1_dsRam.snsList.linearslider0[3u].bsln[0u])
#define CapSense_ADC_1_LINEARSLIDER0_SNS3_BSLN0_OFFSET      (94u)
#define CapSense_ADC_1_LINEARSLIDER0_SNS3_BSLN0_SIZE        (2u)
#define CapSense_ADC_1_LINEARSLIDER0_SNS3_BSLN0_PARAM_ID    (0x8000005Eu)

#define CapSense_ADC_1_LINEARSLIDER0_SNS3_BSLN_EXT0_VALUE   (CapSense_ADC_1_dsRam.snsList.linearslider0[3u].bslnExt[0u])
#define CapSense_ADC_1_LINEARSLIDER0_SNS3_BSLN_EXT0_OFFSET  (96u)
#define CapSense_ADC_1_LINEARSLIDER0_SNS3_BSLN_EXT0_SIZE    (1u)
#define CapSense_ADC_1_LINEARSLIDER0_SNS3_BSLN_EXT0_PARAM_ID (0x48000060u)

#define CapSense_ADC_1_LINEARSLIDER0_SNS3_DIFF_VALUE        (CapSense_ADC_1_dsRam.snsList.linearslider0[3u].diff)
#define CapSense_ADC_1_LINEARSLIDER0_SNS3_DIFF_OFFSET       (98u)
#define CapSense_ADC_1_LINEARSLIDER0_SNS3_DIFF_SIZE         (2u)
#define CapSense_ADC_1_LINEARSLIDER0_SNS3_DIFF_PARAM_ID     (0x8C000062u)

#define CapSense_ADC_1_LINEARSLIDER0_SNS3_NEG_BSLN_RST_CNT0_VALUE (CapSense_ADC_1_dsRam.snsList.linearslider0[3u].negBslnRstCnt[0u])
#define CapSense_ADC_1_LINEARSLIDER0_SNS3_NEG_BSLN_RST_CNT0_OFFSET (100u)
#define CapSense_ADC_1_LINEARSLIDER0_SNS3_NEG_BSLN_RST_CNT0_SIZE (1u)
#define CapSense_ADC_1_LINEARSLIDER0_SNS3_NEG_BSLN_RST_CNT0_PARAM_ID (0x49000064u)

#define CapSense_ADC_1_LINEARSLIDER0_SNS3_IDAC_COMP0_VALUE  (CapSense_ADC_1_dsRam.snsList.linearslider0[3u].idacComp[0u])
#define CapSense_ADC_1_LINEARSLIDER0_SNS3_IDAC_COMP0_OFFSET (101u)
#define CapSense_ADC_1_LINEARSLIDER0_SNS3_IDAC_COMP0_SIZE   (1u)
#define CapSense_ADC_1_LINEARSLIDER0_SNS3_IDAC_COMP0_PARAM_ID (0x4F000065u)

#define CapSense_ADC_1_LINEARSLIDER0_SNS4_RAW0_VALUE        (CapSense_ADC_1_dsRam.snsList.linearslider0[4u].raw[0u])
#define CapSense_ADC_1_LINEARSLIDER0_SNS4_RAW0_OFFSET       (102u)
#define CapSense_ADC_1_LINEARSLIDER0_SNS4_RAW0_SIZE         (2u)
#define CapSense_ADC_1_LINEARSLIDER0_SNS4_RAW0_PARAM_ID     (0x8D000066u)

#define CapSense_ADC_1_LINEARSLIDER0_SNS4_BSLN0_VALUE       (CapSense_ADC_1_dsRam.snsList.linearslider0[4u].bsln[0u])
#define CapSense_ADC_1_LINEARSLIDER0_SNS4_BSLN0_OFFSET      (104u)
#define CapSense_ADC_1_LINEARSLIDER0_SNS4_BSLN0_SIZE        (2u)
#define CapSense_ADC_1_LINEARSLIDER0_SNS4_BSLN0_PARAM_ID    (0x82000068u)

#define CapSense_ADC_1_LINEARSLIDER0_SNS4_BSLN_EXT0_VALUE   (CapSense_ADC_1_dsRam.snsList.linearslider0[4u].bslnExt[0u])
#define CapSense_ADC_1_LINEARSLIDER0_SNS4_BSLN_EXT0_OFFSET  (106u)
#define CapSense_ADC_1_LINEARSLIDER0_SNS4_BSLN_EXT0_SIZE    (1u)
#define CapSense_ADC_1_LINEARSLIDER0_SNS4_BSLN_EXT0_PARAM_ID (0x4600006Au)

#define CapSense_ADC_1_LINEARSLIDER0_SNS4_DIFF_VALUE        (CapSense_ADC_1_dsRam.snsList.linearslider0[4u].diff)
#define CapSense_ADC_1_LINEARSLIDER0_SNS4_DIFF_OFFSET       (108u)
#define CapSense_ADC_1_LINEARSLIDER0_SNS4_DIFF_SIZE         (2u)
#define CapSense_ADC_1_LINEARSLIDER0_SNS4_DIFF_PARAM_ID     (0x8300006Cu)

#define CapSense_ADC_1_LINEARSLIDER0_SNS4_NEG_BSLN_RST_CNT0_VALUE (CapSense_ADC_1_dsRam.snsList.linearslider0[4u].negBslnRstCnt[0u])
#define CapSense_ADC_1_LINEARSLIDER0_SNS4_NEG_BSLN_RST_CNT0_OFFSET (110u)
#define CapSense_ADC_1_LINEARSLIDER0_SNS4_NEG_BSLN_RST_CNT0_SIZE (1u)
#define CapSense_ADC_1_LINEARSLIDER0_SNS4_NEG_BSLN_RST_CNT0_PARAM_ID (0x4700006Eu)

#define CapSense_ADC_1_LINEARSLIDER0_SNS4_IDAC_COMP0_VALUE  (CapSense_ADC_1_dsRam.snsList.linearslider0[4u].idacComp[0u])
#define CapSense_ADC_1_LINEARSLIDER0_SNS4_IDAC_COMP0_OFFSET (111u)
#define CapSense_ADC_1_LINEARSLIDER0_SNS4_IDAC_COMP0_SIZE   (1u)
#define CapSense_ADC_1_LINEARSLIDER0_SNS4_IDAC_COMP0_PARAM_ID (0x4100006Fu)

#define CapSense_ADC_1_SNR_TEST_WIDGET_ID_VALUE             (CapSense_ADC_1_dsRam.snrTestWidgetId)
#define CapSense_ADC_1_SNR_TEST_WIDGET_ID_OFFSET            (112u)
#define CapSense_ADC_1_SNR_TEST_WIDGET_ID_SIZE              (1u)
#define CapSense_ADC_1_SNR_TEST_WIDGET_ID_PARAM_ID          (0x66000070u)

#define CapSense_ADC_1_SNR_TEST_SENSOR_ID_VALUE             (CapSense_ADC_1_dsRam.snrTestSensorId)
#define CapSense_ADC_1_SNR_TEST_SENSOR_ID_OFFSET            (113u)
#define CapSense_ADC_1_SNR_TEST_SENSOR_ID_SIZE              (1u)
#define CapSense_ADC_1_SNR_TEST_SENSOR_ID_PARAM_ID          (0x60000071u)

#define CapSense_ADC_1_SNR_TEST_SCAN_COUNTER_VALUE          (CapSense_ADC_1_dsRam.snrTestScanCounter)
#define CapSense_ADC_1_SNR_TEST_SCAN_COUNTER_OFFSET         (114u)
#define CapSense_ADC_1_SNR_TEST_SCAN_COUNTER_SIZE           (2u)
#define CapSense_ADC_1_SNR_TEST_SCAN_COUNTER_PARAM_ID       (0x89000072u)

#define CapSense_ADC_1_SNR_TEST_RAW_COUNT0_VALUE            (CapSense_ADC_1_dsRam.snrTestRawCount[0u])
#define CapSense_ADC_1_SNR_TEST_RAW_COUNT0_OFFSET           (116u)
#define CapSense_ADC_1_SNR_TEST_RAW_COUNT0_SIZE             (2u)
#define CapSense_ADC_1_SNR_TEST_RAW_COUNT0_PARAM_ID         (0x84000074u)

#define CapSense_ADC_1_SCAN_CSD_ISC_VALUE                   (CapSense_ADC_1_dsRam.scanCsdISC)
#define CapSense_ADC_1_SCAN_CSD_ISC_OFFSET                  (118u)
#define CapSense_ADC_1_SCAN_CSD_ISC_SIZE                    (1u)
#define CapSense_ADC_1_SCAN_CSD_ISC_PARAM_ID                (0x40000076u)

#define CapSense_ADC_1_SCAN_CURRENT_ISC_VALUE               (CapSense_ADC_1_dsRam.scanCurrentISC)
#define CapSense_ADC_1_SCAN_CURRENT_ISC_OFFSET              (119u)
#define CapSense_ADC_1_SCAN_CURRENT_ISC_SIZE                (1u)
#define CapSense_ADC_1_SCAN_CURRENT_ISC_PARAM_ID            (0x46000077u)


/*****************************************************************************/
/* Flash Data structure register definitions                                 */
/*****************************************************************************/
#define CapSense_ADC_1_LINEARSLIDER0_PTR2SNS_FLASH_VALUE    (CapSense_ADC_1_dsFlash.wdgtArray[0].ptr2SnsFlash)
#define CapSense_ADC_1_LINEARSLIDER0_PTR2SNS_FLASH_OFFSET   (0u)
#define CapSense_ADC_1_LINEARSLIDER0_PTR2SNS_FLASH_SIZE     (4u)
#define CapSense_ADC_1_LINEARSLIDER0_PTR2SNS_FLASH_PARAM_ID (0xD1000000u)

#define CapSense_ADC_1_LINEARSLIDER0_PTR2WD_RAM_VALUE       (CapSense_ADC_1_dsFlash.wdgtArray[0].ptr2WdgtRam)
#define CapSense_ADC_1_LINEARSLIDER0_PTR2WD_RAM_OFFSET      (4u)
#define CapSense_ADC_1_LINEARSLIDER0_PTR2WD_RAM_SIZE        (4u)
#define CapSense_ADC_1_LINEARSLIDER0_PTR2WD_RAM_PARAM_ID    (0xD0000004u)

#define CapSense_ADC_1_LINEARSLIDER0_PTR2SNS_RAM_VALUE      (CapSense_ADC_1_dsFlash.wdgtArray[0].ptr2SnsRam)
#define CapSense_ADC_1_LINEARSLIDER0_PTR2SNS_RAM_OFFSET     (8u)
#define CapSense_ADC_1_LINEARSLIDER0_PTR2SNS_RAM_SIZE       (4u)
#define CapSense_ADC_1_LINEARSLIDER0_PTR2SNS_RAM_PARAM_ID   (0xD3000008u)

#define CapSense_ADC_1_LINEARSLIDER0_PTR2FLTR_HISTORY_VALUE (CapSense_ADC_1_dsFlash.wdgtArray[0].ptr2FltrHistory)
#define CapSense_ADC_1_LINEARSLIDER0_PTR2FLTR_HISTORY_OFFSET (12u)
#define CapSense_ADC_1_LINEARSLIDER0_PTR2FLTR_HISTORY_SIZE  (4u)
#define CapSense_ADC_1_LINEARSLIDER0_PTR2FLTR_HISTORY_PARAM_ID (0xD200000Cu)

#define CapSense_ADC_1_LINEARSLIDER0_PTR2DEBOUNCE_VALUE     (CapSense_ADC_1_dsFlash.wdgtArray[0].ptr2DebounceArr)
#define CapSense_ADC_1_LINEARSLIDER0_PTR2DEBOUNCE_OFFSET    (16u)
#define CapSense_ADC_1_LINEARSLIDER0_PTR2DEBOUNCE_SIZE      (4u)
#define CapSense_ADC_1_LINEARSLIDER0_PTR2DEBOUNCE_PARAM_ID  (0xD4000010u)

#define CapSense_ADC_1_LINEARSLIDER0_STATIC_CONFIG_VALUE    (CapSense_ADC_1_dsFlash.wdgtArray[0].staticConfig)
#define CapSense_ADC_1_LINEARSLIDER0_STATIC_CONFIG_OFFSET   (20u)
#define CapSense_ADC_1_LINEARSLIDER0_STATIC_CONFIG_SIZE     (4u)
#define CapSense_ADC_1_LINEARSLIDER0_STATIC_CONFIG_PARAM_ID (0xD5000014u)

#define CapSense_ADC_1_LINEARSLIDER0_TOTAL_NUM_SNS_VALUE    (CapSense_ADC_1_dsFlash.wdgtArray[0].totalNumSns)
#define CapSense_ADC_1_LINEARSLIDER0_TOTAL_NUM_SNS_OFFSET   (24u)
#define CapSense_ADC_1_LINEARSLIDER0_TOTAL_NUM_SNS_SIZE     (2u)
#define CapSense_ADC_1_LINEARSLIDER0_TOTAL_NUM_SNS_PARAM_ID (0x99000018u)

#define CapSense_ADC_1_LINEARSLIDER0_TYPE_VALUE             (CapSense_ADC_1_dsFlash.wdgtArray[0].wdgtType)
#define CapSense_ADC_1_LINEARSLIDER0_TYPE_OFFSET            (26u)
#define CapSense_ADC_1_LINEARSLIDER0_TYPE_SIZE              (1u)
#define CapSense_ADC_1_LINEARSLIDER0_TYPE_PARAM_ID          (0x5D00001Au)

#define CapSense_ADC_1_LINEARSLIDER0_NUM_COLS_VALUE         (CapSense_ADC_1_dsFlash.wdgtArray[0].numCols)
#define CapSense_ADC_1_LINEARSLIDER0_NUM_COLS_OFFSET        (27u)
#define CapSense_ADC_1_LINEARSLIDER0_NUM_COLS_SIZE          (1u)
#define CapSense_ADC_1_LINEARSLIDER0_NUM_COLS_PARAM_ID      (0x5B00001Bu)

#define CapSense_ADC_1_LINEARSLIDER0_X_RESOLUTION_VALUE     (CapSense_ADC_1_dsFlash.wdgtArray[0].xResolution)
#define CapSense_ADC_1_LINEARSLIDER0_X_RESOLUTION_OFFSET    (28u)
#define CapSense_ADC_1_LINEARSLIDER0_X_RESOLUTION_SIZE      (2u)
#define CapSense_ADC_1_LINEARSLIDER0_X_RESOLUTION_PARAM_ID  (0x9800001Cu)

#define CapSense_ADC_1_LINEARSLIDER0_X_CENT_MULT_VALUE      (CapSense_ADC_1_dsFlash.wdgtArray[0].xCentroidMultiplier)
#define CapSense_ADC_1_LINEARSLIDER0_X_CENT_MULT_OFFSET     (32u)
#define CapSense_ADC_1_LINEARSLIDER0_X_CENT_MULT_SIZE       (4u)
#define CapSense_ADC_1_LINEARSLIDER0_X_CENT_MULT_PARAM_ID   (0xDB000020u)

#define CapSense_ADC_1_LINEARSLIDER0_PTR2NOISE_ENVLP_VALUE  (CapSense_ADC_1_dsFlash.wdgtArray[0].ptr2NoiseEnvlp)
#define CapSense_ADC_1_LINEARSLIDER0_PTR2NOISE_ENVLP_OFFSET (36u)
#define CapSense_ADC_1_LINEARSLIDER0_PTR2NOISE_ENVLP_SIZE   (4u)
#define CapSense_ADC_1_LINEARSLIDER0_PTR2NOISE_ENVLP_PARAM_ID (0xDA000024u)

#define CapSense_ADC_1_LINEARSLIDER0_IIR_FILTER_COEFF_VALUE (CapSense_ADC_1_dsFlash.wdgtArray[0].iirFilterCoeff)
#define CapSense_ADC_1_LINEARSLIDER0_IIR_FILTER_COEFF_OFFSET (40u)
#define CapSense_ADC_1_LINEARSLIDER0_IIR_FILTER_COEFF_SIZE  (1u)
#define CapSense_ADC_1_LINEARSLIDER0_IIR_FILTER_COEFF_PARAM_ID (0x5E000028u)


#endif /* End CY_SENSE_CapSense_ADC_1_REGISTER_MAP_H */


/* [] END OF FILE */
