/***************************************************************************//**
* \file CapSense_ADC_1_Adc.h
* \version 7.0
*
* \brief
*   This file provides the sources of APIs specific to the ADC implementation.
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

#if !defined(CY_SENSE_CapSense_ADC_1_ADC_H)
#define CY_SENSE_CapSense_ADC_1_ADC_H

#include "cytypes.h"
#include "CapSense_ADC_1_Configuration.h"
#include "CapSense_ADC_1_Structure.h"

#if (CapSense_ADC_1_ADC_EN)

/*******************************************************************************
* Function Prototypes
*******************************************************************************/

/**
* \cond SECTION_STANDALONE_ADC
* \addtogroup group_adc_public
* \{
*/
#if (CapSense_ADC_1_ENABLE == CapSense_ADC_1_ADC_STANDALONE_EN)
    void CapSense_ADC_1_Start(void);
    void CapSense_ADC_1_Sleep(void);
    void CapSense_ADC_1_Wakeup(void);
#endif  /* (CapSense_ADC_1_ENABLE == CapSense_ADC_1_ADC_STANDALONE_EN) */

/** \}
* \endcond */

/**
* \cond SECTION_ADC_PUBLIC
* \addtogroup group_adc_public
* \{
*/
cystatus CapSense_ADC_1_AdcStartConvert(uint8 chId);
uint8 CapSense_ADC_1_AdcIsBusy(void);
uint16 CapSense_ADC_1_AdcReadResult_mVolts(uint8 chId);
uint16 CapSense_ADC_1_AdcGetResult_mVolts(uint8 chId);
cystatus CapSense_ADC_1_AdcCalibrate(void);

void CapSense_ADC_1_AdcStop(void);
void CapSense_ADC_1_AdcResume(void);

/** \}
* \endcond */

CY_ISR_PROTO(CapSense_ADC_1_AdcIntrHandler);

/**
* \cond SECTION_ADC_INTERNAL
* \addtogroup group_adc_internal
* \{
*/

void CapSense_ADC_1_AdcInitialize(void);
void CapSense_ADC_1_SetAdcChannel(uint8 chId, uint32 state);
void CapSense_ADC_1_ConfigAdcResources(void);
void CapSense_ADC_1_StartAdcFSM(uint32 measureMode);
cystatus CapSense_ADC_1_AdcCaptureResources(void);
cystatus CapSense_ADC_1_AdcReleaseResources(void);
void CapSense_ADC_1_ClearAdcChannels(void);
void CapSense_ADC_1_SetNonDedicatedAdcChannel(uint8 chId, uint32 state);

/** \}
* \endcond */

/**************************************
* Global software/external variables
**************************************/

extern uint16 CapSense_ADC_1_adcVrefMv;

/**************************************
*           API Constants
**************************************/

/* Error value if given bad channel ID. */
#define CapSense_ADC_1_AdcVALUE_BAD_CHAN_ID            (0x0000FFFFuL)
#define CapSense_ADC_1_AdcVALUE_BAD_RESULT             (0x0000FFFFuL)

/* Statuses defined for use with IsBusy */
#define CapSense_ADC_1_AdcSTATUS_LASTCHAN_MASK         (0x0000000FuL)
#define CapSense_ADC_1_AdcSTATUS_FSM_MASK              (0x000000F0uL)
#define CapSense_ADC_1_AdcSTATUS_IDLE                  (0x00u)
#define CapSense_ADC_1_AdcSTATUS_CALIBPH1              (0x10u)
#define CapSense_ADC_1_AdcSTATUS_CALIBPH2              (0x20u)
#define CapSense_ADC_1_AdcSTATUS_CALIBPH3              (0x30u)
#define CapSense_ADC_1_AdcSTATUS_CONVERTING            (0x40u)
#define CapSense_ADC_1_AdcSTATUS_OVERFLOW              (0x80u)

/* Potential channel states */
#define CapSense_ADC_1_AdcCHAN_CONNECT                 (1uL)
#define CapSense_ADC_1_AdcCHAN_DISCONNECT              (0uL)

/* Active channel when ADC is not configured */
#define CapSense_ADC_1_AdcNO_CHANNEL                   (0xFFu)

#define CapSense_ADC_1_AdcGPIO_PC_INPUT                (0x1uL)

/* Default filter delay */
#define CapSense_ADC_1_AdcFILTER_DELAY_DEFAULT         (2uL)

/* Adc Config */
#define CapSense_ADC_1_AdcCONFIG_DEFAULT               (CapSense_ADC_1_CONFIG_ENABLE_MASK | \
                                                                            CapSense_ADC_1_CONFIG_SAMPLE_SYNC_MASK | \
                                                                            CapSense_ADC_1_CONFIG_SENSE_EN_MASK | \
                                                                            CapSense_ADC_1_CONFIG_DSI_COUNT_SEL_MASK | \
                                                                            CapSense_ADC_1_AdcFILTER_DELAY_DEFAULT)

/* Measurement modes */
#define CapSense_ADC_1_AdcMEASMODE_OFF                 (0x0uL << CYFLD_CSD_ADC_MODE__OFFSET)
#define CapSense_ADC_1_AdcMEASMODE_VREF                (0x1uL << CYFLD_CSD_ADC_MODE__OFFSET)
#define CapSense_ADC_1_AdcMEASMODE_VREFBY2             (0x2uL << CYFLD_CSD_ADC_MODE__OFFSET)
#define CapSense_ADC_1_AdcMEASMODE_VIN                 (0x3uL << CYFLD_CSD_ADC_MODE__OFFSET)

/* Clock defines */
#define CapSense_ADC_1_AdcSENSE_DIV_DEFAULT            (0x4uL)
#define CapSense_ADC_1_AdcTOTAL_CLOCK_DIV              (CapSense_ADC_1_ADC_MODCLK_DIV_DEFAULT * \
                                                                         CapSense_ADC_1_AdcSENSE_DIV_DEFAULT)

/* The MAX possible value of the AZ time in CSD_SENSE cycles. The value is limited by the width of the SEQ_TIME register */
#define CapSense_ADC_1_ADC_SEQ_TIME_MAX                               (0x100u)

#define CapSense_ADC_1_ADC_SEQ_TIME_CYCLES                            (((CYDEV_BCLK__HFCLK__HZ * CapSense_ADC_1_ADC_AZ_TIME) / \
                                                                         CapSense_ADC_1_AdcTOTAL_CLOCK_DIV) / 1000000uL)

#if (CapSense_ADC_1_ADC_SEQ_TIME_MAX < CapSense_ADC_1_ADC_SEQ_TIME_CYCLES)
    #define CapSense_ADC_1_AdcSEQ_TIME_BASE            (CapSense_ADC_1_ADC_SEQ_TIME_MAX)
#else
    #define CapSense_ADC_1_AdcSEQ_TIME_BASE            (CapSense_ADC_1_ADC_SEQ_TIME_CYCLES)
#endif

#if (0 == CapSense_ADC_1_AdcSEQ_TIME_BASE)
    #define CapSense_ADC_1_AdcSEQ_TIME_DEFAUL          (1u)
#else
    #define CapSense_ADC_1_AdcSEQ_TIME_DEFAUL          (CapSense_ADC_1_AdcSEQ_TIME_BASE)
#endif

/* Acquisition time definitions: ADC_CTL */
#define CapSense_ADC_1_AdcACQUISITION_BASE             ((CapSense_ADC_1_ADC_ACQUISITION_TIME_US * \
                                                                        (CYDEV_BCLK__HFCLK__MHZ)) / \
                                                                         CapSense_ADC_1_AdcTOTAL_CLOCK_DIV)

/* SEQ_START field definitions */
#define CapSense_ADC_1_AdcFSMSETTING_ABORT             (0x1uL << CYFLD_CSD_ABORT__OFFSET)
#define CapSense_ADC_1_AdcFSMSETTING_DSI_START_EN      (0x1uL << CYFLD_CSD_DSI_START_EN__OFFSET)
#define CapSense_ADC_1_AdcFSMSETTING_AZ0SKIP           (0x1uL << CYFLD_CSD_AZ0_SKIP__OFFSET)
#define CapSense_ADC_1_AdcFSMSETTING_AZ1SKIP           (0x1uL << CYFLD_CSD_AZ1_SKIP__OFFSET)


#define CapSense_ADC_1_AdcFSMSETTING_NOAZSKIP          (0uL)
#define CapSense_ADC_1_AdcFSMSETTING_AZSKIP_DEFAULT    (CapSense_ADC_1_AdcFSMSETTING_AZ0SKIP \
                                                                            | ((0u != CapSense_ADC_1_ADC_AZ_EN) \
                                                                            ? 0u \
                                                                            : CapSense_ADC_1_AdcFSMSETTING_AZ1SKIP))
#define CapSense_ADC_1_AdcFSMSETTING_DSIIGNORE         (0x00000000uL)
#define CapSense_ADC_1_AdcFSMSETTING_NOABORT           (0x00000000uL)
#define CapSense_ADC_1_AdcFSMSETTING_SEQMODE           (0x00000000uL)
#define CapSense_ADC_1_AdcFSMSETTING_START             (0x00000001uL)

/* IDACB definitions
*  The idac configuration for ADC use is mostly static, with only the VAL field varying.
*   Dynamic Polarity = 1 << 7
*   Polarity (normal) = 0 << 8
*   Balance, Leg1, Leg2 modes = don't care.
*   DSI Control Enable (no mix) = 0 << 21
*   Range (low) = 0 << 22
*   Leg1, Leg2 enable = 0
*   Leg3 enable = 1 << 26
*/
#define CapSense_ADC_1_AdcIDACB_CONFIG                 (0x04000080uL)
#define CapSense_ADC_1_AdcCLK16_MASK                   (0x0000FFFFuL)
#define CapSense_ADC_1_AdcCLK16VAL_SHIFT               (8uL)

/* Switch definitions */
#define CapSense_ADC_1_AdcSW_HSP_DEFAULT               (0x10000000uL)
#define CapSense_ADC_1_AdcSW_HSN_DEFAULT               (0x00100000uL)
#define CapSense_ADC_1_AdcSW_HSP_GETINPOL              (0x00010000uL)
#define CapSense_ADC_1_AdcSW_HSN_GETINPOL              (0x01000000uL)
#define CapSense_ADC_1_AdcSW_SHIELD_DEFAULT            (0x00000000uL)
#define CapSense_ADC_1_AdcSW_SHIELD_VDDA2CSDBUSB       (0x00000100uL)
#define CapSense_ADC_1_AdcSW_BYP_DEFAULT               (0x00110000uL)
#define CapSense_ADC_1_AdcSW_CMPP_DEFAULT              (0x00000000uL)
#define CapSense_ADC_1_AdcSW_CMPN_DEFAULT              (0x00000000uL)
#define CapSense_ADC_1_AdcSW_REFGEN_DEFAULT            (0x10000000uL)

#define CapSense_ADC_1_AdcSW_FWMOD_DEFAULT             (0x01100000uL)
#define CapSense_ADC_1_AdcSW_FWTANK_DEFAULT            (0x01100000uL)

/* The reference voltage macros */
#define CapSense_ADC_1_AdcSW_CTANK_PINSHIFT            (9uL)
#define CapSense_ADC_1_AdcSW_CMOD_PINSHIFT             (6uL)
#define CapSense_ADC_1_AdcSW_CMOD_PORT_MASK            (0x400uL)

#define CapSense_ADC_1_AdcLVTHRESH                     (2700uL)

/* The reference voltage is measured at nominal 2400 mV. Measured amount is stored in CYREG_SFLASH_CSDV2_CSD0_ADC_TRIM1 */
#define CapSense_ADC_1_AdcVREFCALIB_BASE               (2400uL)

/* RefGen settings */
#define CapSense_ADC_1_AdcREFGEN_GAIN_SHIFT            (CYFLD_CSD_GAIN__OFFSET)

/* At low voltage, REFGEN is enabled and bypassed. */
#define CapSense_ADC_1_AdcSW_AMUBUF_LV                 (0x01000100uL)
#define CapSense_ADC_1_AdcAMBUF_LV                     (0x00000002uL)
/* At normal voltage, REFGEN uses customizer-defined gain */
#define CapSense_ADC_1_AdcREFGEN_NORM                  (0x00000041uL | (CapSense_ADC_1_ADC_GAIN << \
                                                                            CapSense_ADC_1_AdcREFGEN_GAIN_SHIFT))
#define CapSense_ADC_1_AdcSW_AMUBUF_NORM               (0x00000000uL)

/* HSCOMP definitions */
#define CapSense_ADC_1_AdcHSCMP_AZ_DEFAULT             (CapSense_ADC_1_HSCMP_EN_MASK | \
                                                                            (CapSense_ADC_1_ADC_AZ_EN << CYFLD_CSD_AZ_EN__OFFSET))

/* ADC_RES definitions */
#define CapSense_ADC_1_AdcRES_MAX                      ((1uL << CapSense_ADC_1_ADC_RESOLUTION) - 1uL)
#define CapSense_ADC_1_AdcADC_RES_OVERFLOW_MASK        (0x40000000uL)
#define CapSense_ADC_1_AdcADC_RES_ABORT_MASK           (0x80000000uL)
#define CapSense_ADC_1_AdcADC_RES_HSCMPPOL_MASK        (0x00010000uL)
#define CapSense_ADC_1_AdcADC_RES_VALUE_MASK           (0x0000FFFFuL)

#endif  /* CapSense_ADC_1_ADC_EN */

#endif  /* CY_SENSE_CapSense_ADC_1_ADC_H */


/* [] END OF FILE */
