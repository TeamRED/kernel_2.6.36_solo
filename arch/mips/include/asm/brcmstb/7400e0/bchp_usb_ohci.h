/***************************************************************************
 *     Copyright (c) 1999-2008, Broadcom Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 * Module Description:
 *                     DO NOT EDIT THIS FILE DIRECTLY
 *
 * This module was generated magically with RDB from a source description
 * file. You must edit the source file for changes to be made to this file.
 *
 *
 * Date:           Generated on         Wed Jan  9 09:03:13 2008
 *                 MD5 Checksum         847dc12a9d71c4c68a648bbf19a883e3
 *
 * Compiled with:  RDB Utility          combo_header.pl
 *                 RDB Parser           3.0
 *                 unknown              unknown
 *                 Perl Interpreter     5.008008
 *                 Operating System     linux
 *
 * Revision History:
 *
 * $brcm_Log: /magnum/basemodules/chp/7400/rdb/e0/bchp_usb_ohci.h $
 * 
 * Hydra_Software_Devel/1   1/9/08 11:53a rpan
 * PR38572: Initial revision.
 *
 ***************************************************************************/

#ifndef BCHP_USB_OHCI_H__
#define BCHP_USB_OHCI_H__

/***************************************************************************
 *USB_OHCI - USB OHCI 1 Control Registers
 ***************************************************************************/
#define BCHP_USB_OHCI_HcRevision                 0x00480400 /* Host Controller Revision Register */
#define BCHP_USB_OHCI_HcControl                  0x00480404 /* Host Controller Control Register */
#define BCHP_USB_OHCI_HcCommandStatus            0x00480408 /* Host Controller Command Status Register */
#define BCHP_USB_OHCI_HcInterruptStatus          0x0048040c /* Host Controller Interrupt Status Register */
#define BCHP_USB_OHCI_HcInterruptEnable          0x00480410 /* Host Controller Interrupt Enable Register */
#define BCHP_USB_OHCI_HcInterruptDisable         0x00480414 /* Host Controller Interrupt Disable Register */
#define BCHP_USB_OHCI_HcHCCA                     0x00480418 /* Host Controller Communication Area Register */
#define BCHP_USB_OHCI_HcPeriodCurrentED          0x0048041c /* Current Isochronous or Interrupt Endpoint Descriptor Register */
#define BCHP_USB_OHCI_HcControlHeadED            0x00480420 /* First Endpoint Descriptor of the Control List */
#define BCHP_USB_OHCI_HcControlCurrentED         0x00480424 /* Current Endpoint Descriptor of the Control List */
#define BCHP_USB_OHCI_HcBulkHeadED               0x00480428 /* First Endpoint Descriptor of the Bulk List */
#define BCHP_USB_OHCI_HcBulkCurrentED            0x0048042c /* Current Endpoint Descriptor of the Bulk List */
#define BCHP_USB_OHCI_HcDoneHead                 0x00480430 /* Last Completed Transfer Descriptor Added to the Done Queue */
#define BCHP_USB_OHCI_HcFmInterval               0x00480434 /* Frame Bit Time Interval Register */
#define BCHP_USB_OHCI_HcFmRemaining              0x00480438 /* Bit Time Remaining in the Current Frame */
#define BCHP_USB_OHCI_HcFmNumber                 0x0048043c /* Frame Number Register */
#define BCHP_USB_OHCI_HcPeriodicStart            0x00480440 /* Register to Start Processing the Periodic List */
#define BCHP_USB_OHCI_HcLSThreshold              0x00480444 /* LS Packet Threshold Register */
#define BCHP_USB_OHCI_HcRhDescriptorA            0x00480448 /* Root Hub Descriptor A Register */
#define BCHP_USB_OHCI_HcRhDescriptorB            0x0048044c /* Root Hub Descriptor B Register */
#define BCHP_USB_OHCI_HcRhStatus                 0x00480450 /* Root Hub Status Register */
#define BCHP_USB_OHCI_HcRhPortStatus1            0x00480454 /* Root Hub Port Status Register for Port 1 */
#define BCHP_USB_OHCI_HcRhPortStatus2            0x00480458 /* Root Hub Port Status Register for Port 2 */

/***************************************************************************
 *HcRevision - Host Controller Revision Register
 ***************************************************************************/
/* USB_OHCI :: HcRevision :: REV [31:00] */
#define BCHP_USB_OHCI_HcRevision_REV_MASK                          0xffffffff
#define BCHP_USB_OHCI_HcRevision_REV_SHIFT                         0

/***************************************************************************
 *HcControl - Host Controller Control Register
 ***************************************************************************/
/* USB_OHCI :: HcControl :: reserved0 [31:11] */
#define BCHP_USB_OHCI_HcControl_reserved0_MASK                     0xfffff800
#define BCHP_USB_OHCI_HcControl_reserved0_SHIFT                    11

/* USB_OHCI :: HcControl :: RWE [10:10] */
#define BCHP_USB_OHCI_HcControl_RWE_MASK                           0x00000400
#define BCHP_USB_OHCI_HcControl_RWE_SHIFT                          10

/* USB_OHCI :: HcControl :: RWC [09:09] */
#define BCHP_USB_OHCI_HcControl_RWC_MASK                           0x00000200
#define BCHP_USB_OHCI_HcControl_RWC_SHIFT                          9

/* USB_OHCI :: HcControl :: IR [08:08] */
#define BCHP_USB_OHCI_HcControl_IR_MASK                            0x00000100
#define BCHP_USB_OHCI_HcControl_IR_SHIFT                           8

/* USB_OHCI :: HcControl :: HCFS [07:06] */
#define BCHP_USB_OHCI_HcControl_HCFS_MASK                          0x000000c0
#define BCHP_USB_OHCI_HcControl_HCFS_SHIFT                         6

/* USB_OHCI :: HcControl :: BLE [05:05] */
#define BCHP_USB_OHCI_HcControl_BLE_MASK                           0x00000020
#define BCHP_USB_OHCI_HcControl_BLE_SHIFT                          5

/* USB_OHCI :: HcControl :: CLE [04:04] */
#define BCHP_USB_OHCI_HcControl_CLE_MASK                           0x00000010
#define BCHP_USB_OHCI_HcControl_CLE_SHIFT                          4

/* USB_OHCI :: HcControl :: IE [03:03] */
#define BCHP_USB_OHCI_HcControl_IE_MASK                            0x00000008
#define BCHP_USB_OHCI_HcControl_IE_SHIFT                           3

/* USB_OHCI :: HcControl :: PLE [02:02] */
#define BCHP_USB_OHCI_HcControl_PLE_MASK                           0x00000004
#define BCHP_USB_OHCI_HcControl_PLE_SHIFT                          2

/* USB_OHCI :: HcControl :: CBSR [01:00] */
#define BCHP_USB_OHCI_HcControl_CBSR_MASK                          0x00000003
#define BCHP_USB_OHCI_HcControl_CBSR_SHIFT                         0

/***************************************************************************
 *HcCommandStatus - Host Controller Command Status Register
 ***************************************************************************/
/* USB_OHCI :: HcCommandStatus :: reserved0 [31:18] */
#define BCHP_USB_OHCI_HcCommandStatus_reserved0_MASK               0xfffc0000
#define BCHP_USB_OHCI_HcCommandStatus_reserved0_SHIFT              18

/* USB_OHCI :: HcCommandStatus :: SOC [17:16] */
#define BCHP_USB_OHCI_HcCommandStatus_SOC_MASK                     0x00030000
#define BCHP_USB_OHCI_HcCommandStatus_SOC_SHIFT                    16

/* USB_OHCI :: HcCommandStatus :: reserved1 [15:04] */
#define BCHP_USB_OHCI_HcCommandStatus_reserved1_MASK               0x0000fff0
#define BCHP_USB_OHCI_HcCommandStatus_reserved1_SHIFT              4

/* USB_OHCI :: HcCommandStatus :: OCR [03:03] */
#define BCHP_USB_OHCI_HcCommandStatus_OCR_MASK                     0x00000008
#define BCHP_USB_OHCI_HcCommandStatus_OCR_SHIFT                    3

/* USB_OHCI :: HcCommandStatus :: BLF [02:02] */
#define BCHP_USB_OHCI_HcCommandStatus_BLF_MASK                     0x00000004
#define BCHP_USB_OHCI_HcCommandStatus_BLF_SHIFT                    2

/* USB_OHCI :: HcCommandStatus :: CLF [01:01] */
#define BCHP_USB_OHCI_HcCommandStatus_CLF_MASK                     0x00000002
#define BCHP_USB_OHCI_HcCommandStatus_CLF_SHIFT                    1

/* USB_OHCI :: HcCommandStatus :: HCR [00:00] */
#define BCHP_USB_OHCI_HcCommandStatus_HCR_MASK                     0x00000001
#define BCHP_USB_OHCI_HcCommandStatus_HCR_SHIFT                    0

/***************************************************************************
 *HcInterruptStatus - Host Controller Interrupt Status Register
 ***************************************************************************/
/* USB_OHCI :: HcInterruptStatus :: reserved0 [31:31] */
#define BCHP_USB_OHCI_HcInterruptStatus_reserved0_MASK             0x80000000
#define BCHP_USB_OHCI_HcInterruptStatus_reserved0_SHIFT            31

/* USB_OHCI :: HcInterruptStatus :: OC [30:30] */
#define BCHP_USB_OHCI_HcInterruptStatus_OC_MASK                    0x40000000
#define BCHP_USB_OHCI_HcInterruptStatus_OC_SHIFT                   30

/* USB_OHCI :: HcInterruptStatus :: reserved1 [29:07] */
#define BCHP_USB_OHCI_HcInterruptStatus_reserved1_MASK             0x3fffff80
#define BCHP_USB_OHCI_HcInterruptStatus_reserved1_SHIFT            7

/* USB_OHCI :: HcInterruptStatus :: RHSC [06:06] */
#define BCHP_USB_OHCI_HcInterruptStatus_RHSC_MASK                  0x00000040
#define BCHP_USB_OHCI_HcInterruptStatus_RHSC_SHIFT                 6

/* USB_OHCI :: HcInterruptStatus :: FNO [05:05] */
#define BCHP_USB_OHCI_HcInterruptStatus_FNO_MASK                   0x00000020
#define BCHP_USB_OHCI_HcInterruptStatus_FNO_SHIFT                  5

/* USB_OHCI :: HcInterruptStatus :: UE [04:04] */
#define BCHP_USB_OHCI_HcInterruptStatus_UE_MASK                    0x00000010
#define BCHP_USB_OHCI_HcInterruptStatus_UE_SHIFT                   4

/* USB_OHCI :: HcInterruptStatus :: RD [03:03] */
#define BCHP_USB_OHCI_HcInterruptStatus_RD_MASK                    0x00000008
#define BCHP_USB_OHCI_HcInterruptStatus_RD_SHIFT                   3

/* USB_OHCI :: HcInterruptStatus :: SF [02:02] */
#define BCHP_USB_OHCI_HcInterruptStatus_SF_MASK                    0x00000004
#define BCHP_USB_OHCI_HcInterruptStatus_SF_SHIFT                   2

/* USB_OHCI :: HcInterruptStatus :: WDH [01:01] */
#define BCHP_USB_OHCI_HcInterruptStatus_WDH_MASK                   0x00000002
#define BCHP_USB_OHCI_HcInterruptStatus_WDH_SHIFT                  1

/* USB_OHCI :: HcInterruptStatus :: SO [00:00] */
#define BCHP_USB_OHCI_HcInterruptStatus_SO_MASK                    0x00000001
#define BCHP_USB_OHCI_HcInterruptStatus_SO_SHIFT                   0

/***************************************************************************
 *HcInterruptEnable - Host Controller Interrupt Enable Register
 ***************************************************************************/
/* USB_OHCI :: HcInterruptEnable :: MIE [31:31] */
#define BCHP_USB_OHCI_HcInterruptEnable_MIE_MASK                   0x80000000
#define BCHP_USB_OHCI_HcInterruptEnable_MIE_SHIFT                  31

/* USB_OHCI :: HcInterruptEnable :: OC [30:30] */
#define BCHP_USB_OHCI_HcInterruptEnable_OC_MASK                    0x40000000
#define BCHP_USB_OHCI_HcInterruptEnable_OC_SHIFT                   30

/* USB_OHCI :: HcInterruptEnable :: reserved0 [29:07] */
#define BCHP_USB_OHCI_HcInterruptEnable_reserved0_MASK             0x3fffff80
#define BCHP_USB_OHCI_HcInterruptEnable_reserved0_SHIFT            7

/* USB_OHCI :: HcInterruptEnable :: RHSC [06:06] */
#define BCHP_USB_OHCI_HcInterruptEnable_RHSC_MASK                  0x00000040
#define BCHP_USB_OHCI_HcInterruptEnable_RHSC_SHIFT                 6

/* USB_OHCI :: HcInterruptEnable :: FNO [05:05] */
#define BCHP_USB_OHCI_HcInterruptEnable_FNO_MASK                   0x00000020
#define BCHP_USB_OHCI_HcInterruptEnable_FNO_SHIFT                  5

/* USB_OHCI :: HcInterruptEnable :: UE [04:04] */
#define BCHP_USB_OHCI_HcInterruptEnable_UE_MASK                    0x00000010
#define BCHP_USB_OHCI_HcInterruptEnable_UE_SHIFT                   4

/* USB_OHCI :: HcInterruptEnable :: RD [03:03] */
#define BCHP_USB_OHCI_HcInterruptEnable_RD_MASK                    0x00000008
#define BCHP_USB_OHCI_HcInterruptEnable_RD_SHIFT                   3

/* USB_OHCI :: HcInterruptEnable :: SF [02:02] */
#define BCHP_USB_OHCI_HcInterruptEnable_SF_MASK                    0x00000004
#define BCHP_USB_OHCI_HcInterruptEnable_SF_SHIFT                   2

/* USB_OHCI :: HcInterruptEnable :: WDH [01:01] */
#define BCHP_USB_OHCI_HcInterruptEnable_WDH_MASK                   0x00000002
#define BCHP_USB_OHCI_HcInterruptEnable_WDH_SHIFT                  1

/* USB_OHCI :: HcInterruptEnable :: SO [00:00] */
#define BCHP_USB_OHCI_HcInterruptEnable_SO_MASK                    0x00000001
#define BCHP_USB_OHCI_HcInterruptEnable_SO_SHIFT                   0

/***************************************************************************
 *HcInterruptDisable - Host Controller Interrupt Disable Register
 ***************************************************************************/
/* USB_OHCI :: HcInterruptDisable :: MIE [31:31] */
#define BCHP_USB_OHCI_HcInterruptDisable_MIE_MASK                  0x80000000
#define BCHP_USB_OHCI_HcInterruptDisable_MIE_SHIFT                 31

/* USB_OHCI :: HcInterruptDisable :: OC [30:30] */
#define BCHP_USB_OHCI_HcInterruptDisable_OC_MASK                   0x40000000
#define BCHP_USB_OHCI_HcInterruptDisable_OC_SHIFT                  30

/* USB_OHCI :: HcInterruptDisable :: reserved0 [29:07] */
#define BCHP_USB_OHCI_HcInterruptDisable_reserved0_MASK            0x3fffff80
#define BCHP_USB_OHCI_HcInterruptDisable_reserved0_SHIFT           7

/* USB_OHCI :: HcInterruptDisable :: RHSC [06:06] */
#define BCHP_USB_OHCI_HcInterruptDisable_RHSC_MASK                 0x00000040
#define BCHP_USB_OHCI_HcInterruptDisable_RHSC_SHIFT                6

/* USB_OHCI :: HcInterruptDisable :: FNO [05:05] */
#define BCHP_USB_OHCI_HcInterruptDisable_FNO_MASK                  0x00000020
#define BCHP_USB_OHCI_HcInterruptDisable_FNO_SHIFT                 5

/* USB_OHCI :: HcInterruptDisable :: UE [04:04] */
#define BCHP_USB_OHCI_HcInterruptDisable_UE_MASK                   0x00000010
#define BCHP_USB_OHCI_HcInterruptDisable_UE_SHIFT                  4

/* USB_OHCI :: HcInterruptDisable :: RD [03:03] */
#define BCHP_USB_OHCI_HcInterruptDisable_RD_MASK                   0x00000008
#define BCHP_USB_OHCI_HcInterruptDisable_RD_SHIFT                  3

/* USB_OHCI :: HcInterruptDisable :: SF [02:02] */
#define BCHP_USB_OHCI_HcInterruptDisable_SF_MASK                   0x00000004
#define BCHP_USB_OHCI_HcInterruptDisable_SF_SHIFT                  2

/* USB_OHCI :: HcInterruptDisable :: WDH [01:01] */
#define BCHP_USB_OHCI_HcInterruptDisable_WDH_MASK                  0x00000002
#define BCHP_USB_OHCI_HcInterruptDisable_WDH_SHIFT                 1

/* USB_OHCI :: HcInterruptDisable :: SO [00:00] */
#define BCHP_USB_OHCI_HcInterruptDisable_SO_MASK                   0x00000001
#define BCHP_USB_OHCI_HcInterruptDisable_SO_SHIFT                  0

/***************************************************************************
 *HcHCCA - Host Controller Communication Area Register
 ***************************************************************************/
/* USB_OHCI :: HcHCCA :: HCCA [31:08] */
#define BCHP_USB_OHCI_HcHCCA_HCCA_MASK                             0xffffff00
#define BCHP_USB_OHCI_HcHCCA_HCCA_SHIFT                            8

/* USB_OHCI :: HcHCCA :: reserved0 [07:00] */
#define BCHP_USB_OHCI_HcHCCA_reserved0_MASK                        0x000000ff
#define BCHP_USB_OHCI_HcHCCA_reserved0_SHIFT                       0

/***************************************************************************
 *HcPeriodCurrentED - Current Isochronous or Interrupt Endpoint Descriptor Register
 ***************************************************************************/
/* USB_OHCI :: HcPeriodCurrentED :: PCED [31:04] */
#define BCHP_USB_OHCI_HcPeriodCurrentED_PCED_MASK                  0xfffffff0
#define BCHP_USB_OHCI_HcPeriodCurrentED_PCED_SHIFT                 4

/* USB_OHCI :: HcPeriodCurrentED :: reserved0 [03:00] */
#define BCHP_USB_OHCI_HcPeriodCurrentED_reserved0_MASK             0x0000000f
#define BCHP_USB_OHCI_HcPeriodCurrentED_reserved0_SHIFT            0

/***************************************************************************
 *HcControlHeadED - First Endpoint Descriptor of the Control List
 ***************************************************************************/
/* USB_OHCI :: HcControlHeadED :: CHED [31:04] */
#define BCHP_USB_OHCI_HcControlHeadED_CHED_MASK                    0xfffffff0
#define BCHP_USB_OHCI_HcControlHeadED_CHED_SHIFT                   4

/* USB_OHCI :: HcControlHeadED :: reserved0 [03:00] */
#define BCHP_USB_OHCI_HcControlHeadED_reserved0_MASK               0x0000000f
#define BCHP_USB_OHCI_HcControlHeadED_reserved0_SHIFT              0

/***************************************************************************
 *HcControlCurrentED - Current Endpoint Descriptor of the Control List
 ***************************************************************************/
/* USB_OHCI :: HcControlCurrentED :: CCED [31:04] */
#define BCHP_USB_OHCI_HcControlCurrentED_CCED_MASK                 0xfffffff0
#define BCHP_USB_OHCI_HcControlCurrentED_CCED_SHIFT                4

/* USB_OHCI :: HcControlCurrentED :: reserved0 [03:00] */
#define BCHP_USB_OHCI_HcControlCurrentED_reserved0_MASK            0x0000000f
#define BCHP_USB_OHCI_HcControlCurrentED_reserved0_SHIFT           0

/***************************************************************************
 *HcBulkHeadED - First Endpoint Descriptor of the Bulk List
 ***************************************************************************/
/* USB_OHCI :: HcBulkHeadED :: BHED [31:04] */
#define BCHP_USB_OHCI_HcBulkHeadED_BHED_MASK                       0xfffffff0
#define BCHP_USB_OHCI_HcBulkHeadED_BHED_SHIFT                      4

/* USB_OHCI :: HcBulkHeadED :: reserved0 [03:00] */
#define BCHP_USB_OHCI_HcBulkHeadED_reserved0_MASK                  0x0000000f
#define BCHP_USB_OHCI_HcBulkHeadED_reserved0_SHIFT                 0

/***************************************************************************
 *HcBulkCurrentED - Current Endpoint Descriptor of the Bulk List
 ***************************************************************************/
/* USB_OHCI :: HcBulkCurrentED :: BCED [31:04] */
#define BCHP_USB_OHCI_HcBulkCurrentED_BCED_MASK                    0xfffffff0
#define BCHP_USB_OHCI_HcBulkCurrentED_BCED_SHIFT                   4

/* USB_OHCI :: HcBulkCurrentED :: reserved0 [03:00] */
#define BCHP_USB_OHCI_HcBulkCurrentED_reserved0_MASK               0x0000000f
#define BCHP_USB_OHCI_HcBulkCurrentED_reserved0_SHIFT              0

/***************************************************************************
 *HcDoneHead - Last Completed Transfer Descriptor Added to the Done Queue
 ***************************************************************************/
/* USB_OHCI :: HcDoneHead :: DH [31:04] */
#define BCHP_USB_OHCI_HcDoneHead_DH_MASK                           0xfffffff0
#define BCHP_USB_OHCI_HcDoneHead_DH_SHIFT                          4

/* USB_OHCI :: HcDoneHead :: reserved0 [03:00] */
#define BCHP_USB_OHCI_HcDoneHead_reserved0_MASK                    0x0000000f
#define BCHP_USB_OHCI_HcDoneHead_reserved0_SHIFT                   0

/***************************************************************************
 *HcFmInterval - Frame Bit Time Interval Register
 ***************************************************************************/
/* USB_OHCI :: HcFmInterval :: FIT [31:31] */
#define BCHP_USB_OHCI_HcFmInterval_FIT_MASK                        0x80000000
#define BCHP_USB_OHCI_HcFmInterval_FIT_SHIFT                       31

/* USB_OHCI :: HcFmInterval :: FSMPS [30:16] */
#define BCHP_USB_OHCI_HcFmInterval_FSMPS_MASK                      0x7fff0000
#define BCHP_USB_OHCI_HcFmInterval_FSMPS_SHIFT                     16

/* USB_OHCI :: HcFmInterval :: reserved0 [15:14] */
#define BCHP_USB_OHCI_HcFmInterval_reserved0_MASK                  0x0000c000
#define BCHP_USB_OHCI_HcFmInterval_reserved0_SHIFT                 14

/* USB_OHCI :: HcFmInterval :: FI [13:00] */
#define BCHP_USB_OHCI_HcFmInterval_FI_MASK                         0x00003fff
#define BCHP_USB_OHCI_HcFmInterval_FI_SHIFT                        0

/***************************************************************************
 *HcFmRemaining - Bit Time Remaining in the Current Frame
 ***************************************************************************/
/* USB_OHCI :: HcFmRemaining :: FRT [31:31] */
#define BCHP_USB_OHCI_HcFmRemaining_FRT_MASK                       0x80000000
#define BCHP_USB_OHCI_HcFmRemaining_FRT_SHIFT                      31

/* USB_OHCI :: HcFmRemaining :: reserved0 [30:14] */
#define BCHP_USB_OHCI_HcFmRemaining_reserved0_MASK                 0x7fffc000
#define BCHP_USB_OHCI_HcFmRemaining_reserved0_SHIFT                14

/* USB_OHCI :: HcFmRemaining :: FR [13:00] */
#define BCHP_USB_OHCI_HcFmRemaining_FR_MASK                        0x00003fff
#define BCHP_USB_OHCI_HcFmRemaining_FR_SHIFT                       0

/***************************************************************************
 *HcFmNumber - Frame Number Register
 ***************************************************************************/
/* USB_OHCI :: HcFmNumber :: reserved0 [31:16] */
#define BCHP_USB_OHCI_HcFmNumber_reserved0_MASK                    0xffff0000
#define BCHP_USB_OHCI_HcFmNumber_reserved0_SHIFT                   16

/* USB_OHCI :: HcFmNumber :: FN [15:00] */
#define BCHP_USB_OHCI_HcFmNumber_FN_MASK                           0x0000ffff
#define BCHP_USB_OHCI_HcFmNumber_FN_SHIFT                          0

/***************************************************************************
 *HcPeriodicStart - Register to Start Processing the Periodic List
 ***************************************************************************/
/* USB_OHCI :: HcPeriodicStart :: reserved0 [31:14] */
#define BCHP_USB_OHCI_HcPeriodicStart_reserved0_MASK               0xffffc000
#define BCHP_USB_OHCI_HcPeriodicStart_reserved0_SHIFT              14

/* USB_OHCI :: HcPeriodicStart :: PS [13:00] */
#define BCHP_USB_OHCI_HcPeriodicStart_PS_MASK                      0x00003fff
#define BCHP_USB_OHCI_HcPeriodicStart_PS_SHIFT                     0

/***************************************************************************
 *HcLSThreshold - LS Packet Threshold Register
 ***************************************************************************/
/* USB_OHCI :: HcLSThreshold :: reserved0 [31:12] */
#define BCHP_USB_OHCI_HcLSThreshold_reserved0_MASK                 0xfffff000
#define BCHP_USB_OHCI_HcLSThreshold_reserved0_SHIFT                12

/* USB_OHCI :: HcLSThreshold :: LST [11:00] */
#define BCHP_USB_OHCI_HcLSThreshold_LST_MASK                       0x00000fff
#define BCHP_USB_OHCI_HcLSThreshold_LST_SHIFT                      0

/***************************************************************************
 *HcRhDescriptorA - Root Hub Descriptor A Register
 ***************************************************************************/
/* USB_OHCI :: HcRhDescriptorA :: POTPGT [31:24] */
#define BCHP_USB_OHCI_HcRhDescriptorA_POTPGT_MASK                  0xff000000
#define BCHP_USB_OHCI_HcRhDescriptorA_POTPGT_SHIFT                 24

/* USB_OHCI :: HcRhDescriptorA :: reserved0 [23:13] */
#define BCHP_USB_OHCI_HcRhDescriptorA_reserved0_MASK               0x00ffe000
#define BCHP_USB_OHCI_HcRhDescriptorA_reserved0_SHIFT              13

/* USB_OHCI :: HcRhDescriptorA :: NOCP [12:12] */
#define BCHP_USB_OHCI_HcRhDescriptorA_NOCP_MASK                    0x00001000
#define BCHP_USB_OHCI_HcRhDescriptorA_NOCP_SHIFT                   12

/* USB_OHCI :: HcRhDescriptorA :: OCPM [11:11] */
#define BCHP_USB_OHCI_HcRhDescriptorA_OCPM_MASK                    0x00000800
#define BCHP_USB_OHCI_HcRhDescriptorA_OCPM_SHIFT                   11

/* USB_OHCI :: HcRhDescriptorA :: DT [10:10] */
#define BCHP_USB_OHCI_HcRhDescriptorA_DT_MASK                      0x00000400
#define BCHP_USB_OHCI_HcRhDescriptorA_DT_SHIFT                     10

/* USB_OHCI :: HcRhDescriptorA :: NPS [09:09] */
#define BCHP_USB_OHCI_HcRhDescriptorA_NPS_MASK                     0x00000200
#define BCHP_USB_OHCI_HcRhDescriptorA_NPS_SHIFT                    9

/* USB_OHCI :: HcRhDescriptorA :: PSM [08:08] */
#define BCHP_USB_OHCI_HcRhDescriptorA_PSM_MASK                     0x00000100
#define BCHP_USB_OHCI_HcRhDescriptorA_PSM_SHIFT                    8

/* USB_OHCI :: HcRhDescriptorA :: NDP [07:00] */
#define BCHP_USB_OHCI_HcRhDescriptorA_NDP_MASK                     0x000000ff
#define BCHP_USB_OHCI_HcRhDescriptorA_NDP_SHIFT                    0

/***************************************************************************
 *HcRhDescriptorB - Root Hub Descriptor B Register
 ***************************************************************************/
/* USB_OHCI :: HcRhDescriptorB :: PPCM [31:16] */
#define BCHP_USB_OHCI_HcRhDescriptorB_PPCM_MASK                    0xffff0000
#define BCHP_USB_OHCI_HcRhDescriptorB_PPCM_SHIFT                   16

/* USB_OHCI :: HcRhDescriptorB :: DR [15:00] */
#define BCHP_USB_OHCI_HcRhDescriptorB_DR_MASK                      0x0000ffff
#define BCHP_USB_OHCI_HcRhDescriptorB_DR_SHIFT                     0

/***************************************************************************
 *HcRhStatus - Root Hub Status Register
 ***************************************************************************/
/* USB_OHCI :: HcRhStatus :: CRWE [31:31] */
#define BCHP_USB_OHCI_HcRhStatus_CRWE_MASK                         0x80000000
#define BCHP_USB_OHCI_HcRhStatus_CRWE_SHIFT                        31

/* USB_OHCI :: HcRhStatus :: reserved0 [30:18] */
#define BCHP_USB_OHCI_HcRhStatus_reserved0_MASK                    0x7ffc0000
#define BCHP_USB_OHCI_HcRhStatus_reserved0_SHIFT                   18

/* USB_OHCI :: HcRhStatus :: OCIC [17:17] */
#define BCHP_USB_OHCI_HcRhStatus_OCIC_MASK                         0x00020000
#define BCHP_USB_OHCI_HcRhStatus_OCIC_SHIFT                        17

/* USB_OHCI :: HcRhStatus :: LPSC [16:16] */
#define BCHP_USB_OHCI_HcRhStatus_LPSC_MASK                         0x00010000
#define BCHP_USB_OHCI_HcRhStatus_LPSC_SHIFT                        16

/* USB_OHCI :: HcRhStatus :: DRWE [15:15] */
#define BCHP_USB_OHCI_HcRhStatus_DRWE_MASK                         0x00008000
#define BCHP_USB_OHCI_HcRhStatus_DRWE_SHIFT                        15

/* USB_OHCI :: HcRhStatus :: reserved1 [14:02] */
#define BCHP_USB_OHCI_HcRhStatus_reserved1_MASK                    0x00007ffc
#define BCHP_USB_OHCI_HcRhStatus_reserved1_SHIFT                   2

/* USB_OHCI :: HcRhStatus :: OCI [01:01] */
#define BCHP_USB_OHCI_HcRhStatus_OCI_MASK                          0x00000002
#define BCHP_USB_OHCI_HcRhStatus_OCI_SHIFT                         1

/* USB_OHCI :: HcRhStatus :: LPS [00:00] */
#define BCHP_USB_OHCI_HcRhStatus_LPS_MASK                          0x00000001
#define BCHP_USB_OHCI_HcRhStatus_LPS_SHIFT                         0

/***************************************************************************
 *HcRhPortStatus1 - Root Hub Port Status Register for Port 1
 ***************************************************************************/
/* USB_OHCI :: HcRhPortStatus1 :: reserved0 [31:21] */
#define BCHP_USB_OHCI_HcRhPortStatus1_reserved0_MASK               0xffe00000
#define BCHP_USB_OHCI_HcRhPortStatus1_reserved0_SHIFT              21

/* USB_OHCI :: HcRhPortStatus1 :: PRSC [20:20] */
#define BCHP_USB_OHCI_HcRhPortStatus1_PRSC_MASK                    0x00100000
#define BCHP_USB_OHCI_HcRhPortStatus1_PRSC_SHIFT                   20

/* USB_OHCI :: HcRhPortStatus1 :: OCIC [19:19] */
#define BCHP_USB_OHCI_HcRhPortStatus1_OCIC_MASK                    0x00080000
#define BCHP_USB_OHCI_HcRhPortStatus1_OCIC_SHIFT                   19

/* USB_OHCI :: HcRhPortStatus1 :: PSSC [18:18] */
#define BCHP_USB_OHCI_HcRhPortStatus1_PSSC_MASK                    0x00040000
#define BCHP_USB_OHCI_HcRhPortStatus1_PSSC_SHIFT                   18

/* USB_OHCI :: HcRhPortStatus1 :: PESC [17:17] */
#define BCHP_USB_OHCI_HcRhPortStatus1_PESC_MASK                    0x00020000
#define BCHP_USB_OHCI_HcRhPortStatus1_PESC_SHIFT                   17

/* USB_OHCI :: HcRhPortStatus1 :: CSC [16:16] */
#define BCHP_USB_OHCI_HcRhPortStatus1_CSC_MASK                     0x00010000
#define BCHP_USB_OHCI_HcRhPortStatus1_CSC_SHIFT                    16

/* USB_OHCI :: HcRhPortStatus1 :: reserved1 [15:10] */
#define BCHP_USB_OHCI_HcRhPortStatus1_reserved1_MASK               0x0000fc00
#define BCHP_USB_OHCI_HcRhPortStatus1_reserved1_SHIFT              10

/* USB_OHCI :: HcRhPortStatus1 :: LSDA [09:09] */
#define BCHP_USB_OHCI_HcRhPortStatus1_LSDA_MASK                    0x00000200
#define BCHP_USB_OHCI_HcRhPortStatus1_LSDA_SHIFT                   9

/* USB_OHCI :: HcRhPortStatus1 :: PPS [08:08] */
#define BCHP_USB_OHCI_HcRhPortStatus1_PPS_MASK                     0x00000100
#define BCHP_USB_OHCI_HcRhPortStatus1_PPS_SHIFT                    8

/* USB_OHCI :: HcRhPortStatus1 :: reserved2 [07:05] */
#define BCHP_USB_OHCI_HcRhPortStatus1_reserved2_MASK               0x000000e0
#define BCHP_USB_OHCI_HcRhPortStatus1_reserved2_SHIFT              5

/* USB_OHCI :: HcRhPortStatus1 :: PRS [04:04] */
#define BCHP_USB_OHCI_HcRhPortStatus1_PRS_MASK                     0x00000010
#define BCHP_USB_OHCI_HcRhPortStatus1_PRS_SHIFT                    4

/* USB_OHCI :: HcRhPortStatus1 :: POCI [03:03] */
#define BCHP_USB_OHCI_HcRhPortStatus1_POCI_MASK                    0x00000008
#define BCHP_USB_OHCI_HcRhPortStatus1_POCI_SHIFT                   3

/* USB_OHCI :: HcRhPortStatus1 :: PSS [02:02] */
#define BCHP_USB_OHCI_HcRhPortStatus1_PSS_MASK                     0x00000004
#define BCHP_USB_OHCI_HcRhPortStatus1_PSS_SHIFT                    2

/* USB_OHCI :: HcRhPortStatus1 :: PES [01:01] */
#define BCHP_USB_OHCI_HcRhPortStatus1_PES_MASK                     0x00000002
#define BCHP_USB_OHCI_HcRhPortStatus1_PES_SHIFT                    1

/* USB_OHCI :: HcRhPortStatus1 :: CCS [00:00] */
#define BCHP_USB_OHCI_HcRhPortStatus1_CCS_MASK                     0x00000001
#define BCHP_USB_OHCI_HcRhPortStatus1_CCS_SHIFT                    0

/***************************************************************************
 *HcRhPortStatus2 - Root Hub Port Status Register for Port 2
 ***************************************************************************/
/* USB_OHCI :: HcRhPortStatus2 :: reserved0 [31:21] */
#define BCHP_USB_OHCI_HcRhPortStatus2_reserved0_MASK               0xffe00000
#define BCHP_USB_OHCI_HcRhPortStatus2_reserved0_SHIFT              21

/* USB_OHCI :: HcRhPortStatus2 :: PRSC [20:20] */
#define BCHP_USB_OHCI_HcRhPortStatus2_PRSC_MASK                    0x00100000
#define BCHP_USB_OHCI_HcRhPortStatus2_PRSC_SHIFT                   20

/* USB_OHCI :: HcRhPortStatus2 :: OCIC [19:19] */
#define BCHP_USB_OHCI_HcRhPortStatus2_OCIC_MASK                    0x00080000
#define BCHP_USB_OHCI_HcRhPortStatus2_OCIC_SHIFT                   19

/* USB_OHCI :: HcRhPortStatus2 :: PSSC [18:18] */
#define BCHP_USB_OHCI_HcRhPortStatus2_PSSC_MASK                    0x00040000
#define BCHP_USB_OHCI_HcRhPortStatus2_PSSC_SHIFT                   18

/* USB_OHCI :: HcRhPortStatus2 :: PESC [17:17] */
#define BCHP_USB_OHCI_HcRhPortStatus2_PESC_MASK                    0x00020000
#define BCHP_USB_OHCI_HcRhPortStatus2_PESC_SHIFT                   17

/* USB_OHCI :: HcRhPortStatus2 :: CSC [16:16] */
#define BCHP_USB_OHCI_HcRhPortStatus2_CSC_MASK                     0x00010000
#define BCHP_USB_OHCI_HcRhPortStatus2_CSC_SHIFT                    16

/* USB_OHCI :: HcRhPortStatus2 :: reserved1 [15:10] */
#define BCHP_USB_OHCI_HcRhPortStatus2_reserved1_MASK               0x0000fc00
#define BCHP_USB_OHCI_HcRhPortStatus2_reserved1_SHIFT              10

/* USB_OHCI :: HcRhPortStatus2 :: LSDA [09:09] */
#define BCHP_USB_OHCI_HcRhPortStatus2_LSDA_MASK                    0x00000200
#define BCHP_USB_OHCI_HcRhPortStatus2_LSDA_SHIFT                   9

/* USB_OHCI :: HcRhPortStatus2 :: PPS [08:08] */
#define BCHP_USB_OHCI_HcRhPortStatus2_PPS_MASK                     0x00000100
#define BCHP_USB_OHCI_HcRhPortStatus2_PPS_SHIFT                    8

/* USB_OHCI :: HcRhPortStatus2 :: reserved2 [07:05] */
#define BCHP_USB_OHCI_HcRhPortStatus2_reserved2_MASK               0x000000e0
#define BCHP_USB_OHCI_HcRhPortStatus2_reserved2_SHIFT              5

/* USB_OHCI :: HcRhPortStatus2 :: PRS [04:04] */
#define BCHP_USB_OHCI_HcRhPortStatus2_PRS_MASK                     0x00000010
#define BCHP_USB_OHCI_HcRhPortStatus2_PRS_SHIFT                    4

/* USB_OHCI :: HcRhPortStatus2 :: POCI [03:03] */
#define BCHP_USB_OHCI_HcRhPortStatus2_POCI_MASK                    0x00000008
#define BCHP_USB_OHCI_HcRhPortStatus2_POCI_SHIFT                   3

/* USB_OHCI :: HcRhPortStatus2 :: PSS [02:02] */
#define BCHP_USB_OHCI_HcRhPortStatus2_PSS_MASK                     0x00000004
#define BCHP_USB_OHCI_HcRhPortStatus2_PSS_SHIFT                    2

/* USB_OHCI :: HcRhPortStatus2 :: PES [01:01] */
#define BCHP_USB_OHCI_HcRhPortStatus2_PES_MASK                     0x00000002
#define BCHP_USB_OHCI_HcRhPortStatus2_PES_SHIFT                    1

/* USB_OHCI :: HcRhPortStatus2 :: CCS [00:00] */
#define BCHP_USB_OHCI_HcRhPortStatus2_CCS_MASK                     0x00000001
#define BCHP_USB_OHCI_HcRhPortStatus2_CCS_SHIFT                    0

#endif /* #ifndef BCHP_USB_OHCI_H__ */

/* End of File */
