/***************************************************************************
 *     Copyright (c) 1999-2009, Broadcom Corporation
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
 * Date:           Generated on         Mon Dec  7 01:17:16 2009
 *                 MD5 Checksum         ab1ca75441be64f31cabaf0b93ebb8cc
 *
 * Compiled with:  RDB Utility          combo_header.pl
 *                 RDB Parser           3.0
 *                 unknown              unknown
 *                 Perl Interpreter     5.008008
 *                 Operating System     linux
 *
 * Revision History:
 *
 * $brcm_Log: $
 *
 ***************************************************************************/

#ifndef BCHP_MEMC_DDR23_APHY_WL1_0_H__
#define BCHP_MEMC_DDR23_APHY_WL1_0_H__

/***************************************************************************
 *MEMC_DDR23_APHY_WL1_0
 ***************************************************************************/
#define BCHP_MEMC_DDR23_APHY_WL1_0_WORD_SLICE_DLL_RESET 0x003b7000
#define BCHP_MEMC_DDR23_APHY_WL1_0_WORDSLICE_CNTRL_0 0x003b7004
#define BCHP_MEMC_DDR23_APHY_WL1_0_WORDSLICE_CNTRL_1 0x003b7008
#define BCHP_MEMC_DDR23_APHY_WL1_0_BYTE0_VCDL_PHASE_CNTL 0x003b7010
#define BCHP_MEMC_DDR23_APHY_WL1_0_BYTE1_VCDL_PHASE_CNTL 0x003b7014
#define BCHP_MEMC_DDR23_APHY_WL1_0_READ_DQS_GATE_CNTRL 0x003b7018
#define BCHP_MEMC_DDR23_APHY_WL1_0_RX_ODT_CNTRL  0x003b701c
#define BCHP_MEMC_DDR23_APHY_WL1_0_ANALOG_BYPASS_CNTRL 0x003b7020
#define BCHP_MEMC_DDR23_APHY_WL1_0_PFIFO_RD_WR_PNTR 0x003b7024
#define BCHP_MEMC_DDR23_APHY_WL1_0_PAD_SSTL_DDR2_MODE 0x003b7028
#define BCHP_MEMC_DDR23_APHY_WL1_0_DDR_PAD_CNTRL 0x003b702c
#define BCHP_MEMC_DDR23_APHY_WL1_0_DDR_PAD_SLEW_CNTRL 0x003b7030
#define BCHP_MEMC_DDR23_APHY_WL1_0_DDR_PAD_RX_DRV_CNTRL 0x003b7034
#define BCHP_MEMC_DDR23_APHY_WL1_0_DDR_PAD_TX_DRV_CNTRL 0x003b7038
#define BCHP_MEMC_DDR23_APHY_WL1_0_MISC          0x003b703c
#define BCHP_MEMC_DDR23_APHY_WL1_0_SPARE0_RW     0x003b7040
#define BCHP_MEMC_DDR23_APHY_WL1_0_SPARE0_RO     0x003b7044

/***************************************************************************
 *WORD_SLICE_DLL_RESET
 ***************************************************************************/
/* MEMC_DDR23_APHY_WL1_0 :: WORD_SLICE_DLL_RESET :: reserved0 [31:01] */
#define BCHP_MEMC_DDR23_APHY_WL1_0_WORD_SLICE_DLL_RESET_reserved0_MASK 0xfffffffe
#define BCHP_MEMC_DDR23_APHY_WL1_0_WORD_SLICE_DLL_RESET_reserved0_SHIFT 1

/* MEMC_DDR23_APHY_WL1_0 :: WORD_SLICE_DLL_RESET :: DLL_RESET [00:00] */
#define BCHP_MEMC_DDR23_APHY_WL1_0_WORD_SLICE_DLL_RESET_DLL_RESET_MASK 0x00000001
#define BCHP_MEMC_DDR23_APHY_WL1_0_WORD_SLICE_DLL_RESET_DLL_RESET_SHIFT 0

/***************************************************************************
 *WORDSLICE_CNTRL_0
 ***************************************************************************/
/* MEMC_DDR23_APHY_WL1_0 :: WORDSLICE_CNTRL_0 :: DLL_CONTROL [31:00] */
#define BCHP_MEMC_DDR23_APHY_WL1_0_WORDSLICE_CNTRL_0_DLL_CONTROL_MASK 0xffffffff
#define BCHP_MEMC_DDR23_APHY_WL1_0_WORDSLICE_CNTRL_0_DLL_CONTROL_SHIFT 0

/***************************************************************************
 *WORDSLICE_CNTRL_1
 ***************************************************************************/
/* MEMC_DDR23_APHY_WL1_0 :: WORDSLICE_CNTRL_1 :: reserved0 [31:05] */
#define BCHP_MEMC_DDR23_APHY_WL1_0_WORDSLICE_CNTRL_1_reserved0_MASK 0xffffffe0
#define BCHP_MEMC_DDR23_APHY_WL1_0_WORDSLICE_CNTRL_1_reserved0_SHIFT 5

/* MEMC_DDR23_APHY_WL1_0 :: WORDSLICE_CNTRL_1 :: PWRDN_DLL_ON_SELFREF [04:04] */
#define BCHP_MEMC_DDR23_APHY_WL1_0_WORDSLICE_CNTRL_1_PWRDN_DLL_ON_SELFREF_MASK 0x00000010
#define BCHP_MEMC_DDR23_APHY_WL1_0_WORDSLICE_CNTRL_1_PWRDN_DLL_ON_SELFREF_SHIFT 4

/* MEMC_DDR23_APHY_WL1_0 :: WORDSLICE_CNTRL_1 :: reserved1 [03:03] */
#define BCHP_MEMC_DDR23_APHY_WL1_0_WORDSLICE_CNTRL_1_reserved1_MASK 0x00000008
#define BCHP_MEMC_DDR23_APHY_WL1_0_WORDSLICE_CNTRL_1_reserved1_SHIFT 3

/* MEMC_DDR23_APHY_WL1_0 :: WORDSLICE_CNTRL_1 :: LDO_PWRDN [02:02] */
#define BCHP_MEMC_DDR23_APHY_WL1_0_WORDSLICE_CNTRL_1_LDO_PWRDN_MASK 0x00000004
#define BCHP_MEMC_DDR23_APHY_WL1_0_WORDSLICE_CNTRL_1_LDO_PWRDN_SHIFT 2

/* MEMC_DDR23_APHY_WL1_0 :: WORDSLICE_CNTRL_1 :: LDO_CTRL [01:00] */
#define BCHP_MEMC_DDR23_APHY_WL1_0_WORDSLICE_CNTRL_1_LDO_CTRL_MASK 0x00000003
#define BCHP_MEMC_DDR23_APHY_WL1_0_WORDSLICE_CNTRL_1_LDO_CTRL_SHIFT 0

/***************************************************************************
 *BYTE0_VCDL_PHASE_CNTL
 ***************************************************************************/
/* MEMC_DDR23_APHY_WL1_0 :: BYTE0_VCDL_PHASE_CNTL :: reserved0 [31:14] */
#define BCHP_MEMC_DDR23_APHY_WL1_0_BYTE0_VCDL_PHASE_CNTL_reserved0_MASK 0xffffc000
#define BCHP_MEMC_DDR23_APHY_WL1_0_BYTE0_VCDL_PHASE_CNTL_reserved0_SHIFT 14

/* MEMC_DDR23_APHY_WL1_0 :: BYTE0_VCDL_PHASE_CNTL :: BYTE_N_PHASE [13:08] */
#define BCHP_MEMC_DDR23_APHY_WL1_0_BYTE0_VCDL_PHASE_CNTL_BYTE_N_PHASE_MASK 0x00003f00
#define BCHP_MEMC_DDR23_APHY_WL1_0_BYTE0_VCDL_PHASE_CNTL_BYTE_N_PHASE_SHIFT 8

/* MEMC_DDR23_APHY_WL1_0 :: BYTE0_VCDL_PHASE_CNTL :: reserved1 [07:06] */
#define BCHP_MEMC_DDR23_APHY_WL1_0_BYTE0_VCDL_PHASE_CNTL_reserved1_MASK 0x000000c0
#define BCHP_MEMC_DDR23_APHY_WL1_0_BYTE0_VCDL_PHASE_CNTL_reserved1_SHIFT 6

/* MEMC_DDR23_APHY_WL1_0 :: BYTE0_VCDL_PHASE_CNTL :: BYTE_P_PHASE [05:00] */
#define BCHP_MEMC_DDR23_APHY_WL1_0_BYTE0_VCDL_PHASE_CNTL_BYTE_P_PHASE_MASK 0x0000003f
#define BCHP_MEMC_DDR23_APHY_WL1_0_BYTE0_VCDL_PHASE_CNTL_BYTE_P_PHASE_SHIFT 0

/***************************************************************************
 *BYTE1_VCDL_PHASE_CNTL
 ***************************************************************************/
/* MEMC_DDR23_APHY_WL1_0 :: BYTE1_VCDL_PHASE_CNTL :: reserved0 [31:14] */
#define BCHP_MEMC_DDR23_APHY_WL1_0_BYTE1_VCDL_PHASE_CNTL_reserved0_MASK 0xffffc000
#define BCHP_MEMC_DDR23_APHY_WL1_0_BYTE1_VCDL_PHASE_CNTL_reserved0_SHIFT 14

/* MEMC_DDR23_APHY_WL1_0 :: BYTE1_VCDL_PHASE_CNTL :: BYTE_N_PHASE [13:08] */
#define BCHP_MEMC_DDR23_APHY_WL1_0_BYTE1_VCDL_PHASE_CNTL_BYTE_N_PHASE_MASK 0x00003f00
#define BCHP_MEMC_DDR23_APHY_WL1_0_BYTE1_VCDL_PHASE_CNTL_BYTE_N_PHASE_SHIFT 8

/* MEMC_DDR23_APHY_WL1_0 :: BYTE1_VCDL_PHASE_CNTL :: reserved1 [07:06] */
#define BCHP_MEMC_DDR23_APHY_WL1_0_BYTE1_VCDL_PHASE_CNTL_reserved1_MASK 0x000000c0
#define BCHP_MEMC_DDR23_APHY_WL1_0_BYTE1_VCDL_PHASE_CNTL_reserved1_SHIFT 6

/* MEMC_DDR23_APHY_WL1_0 :: BYTE1_VCDL_PHASE_CNTL :: BYTE_P_PHASE [05:00] */
#define BCHP_MEMC_DDR23_APHY_WL1_0_BYTE1_VCDL_PHASE_CNTL_BYTE_P_PHASE_MASK 0x0000003f
#define BCHP_MEMC_DDR23_APHY_WL1_0_BYTE1_VCDL_PHASE_CNTL_BYTE_P_PHASE_SHIFT 0

/***************************************************************************
 *READ_DQS_GATE_CNTRL
 ***************************************************************************/
/* MEMC_DDR23_APHY_WL1_0 :: READ_DQS_GATE_CNTRL :: reserved0 [31:02] */
#define BCHP_MEMC_DDR23_APHY_WL1_0_READ_DQS_GATE_CNTRL_reserved0_MASK 0xfffffffc
#define BCHP_MEMC_DDR23_APHY_WL1_0_READ_DQS_GATE_CNTRL_reserved0_SHIFT 2

/* MEMC_DDR23_APHY_WL1_0 :: READ_DQS_GATE_CNTRL :: GR_EDGE_SEL [01:00] */
#define BCHP_MEMC_DDR23_APHY_WL1_0_READ_DQS_GATE_CNTRL_GR_EDGE_SEL_MASK 0x00000003
#define BCHP_MEMC_DDR23_APHY_WL1_0_READ_DQS_GATE_CNTRL_GR_EDGE_SEL_SHIFT 0

/***************************************************************************
 *RX_ODT_CNTRL
 ***************************************************************************/
/* MEMC_DDR23_APHY_WL1_0 :: RX_ODT_CNTRL :: reserved0 [31:02] */
#define BCHP_MEMC_DDR23_APHY_WL1_0_RX_ODT_CNTRL_reserved0_MASK     0xfffffffc
#define BCHP_MEMC_DDR23_APHY_WL1_0_RX_ODT_CNTRL_reserved0_SHIFT    2

/* MEMC_DDR23_APHY_WL1_0 :: RX_ODT_CNTRL :: DEV_DATA_RT60B_OHM [01:01] */
#define BCHP_MEMC_DDR23_APHY_WL1_0_RX_ODT_CNTRL_DEV_DATA_RT60B_OHM_MASK 0x00000002
#define BCHP_MEMC_DDR23_APHY_WL1_0_RX_ODT_CNTRL_DEV_DATA_RT60B_OHM_SHIFT 1

/* MEMC_DDR23_APHY_WL1_0 :: RX_ODT_CNTRL :: DQS_RT60B_OHM [00:00] */
#define BCHP_MEMC_DDR23_APHY_WL1_0_RX_ODT_CNTRL_DQS_RT60B_OHM_MASK 0x00000001
#define BCHP_MEMC_DDR23_APHY_WL1_0_RX_ODT_CNTRL_DQS_RT60B_OHM_SHIFT 0

/***************************************************************************
 *ANALOG_BYPASS_CNTRL
 ***************************************************************************/
/* MEMC_DDR23_APHY_WL1_0 :: ANALOG_BYPASS_CNTRL :: reserved0 [31:02] */
#define BCHP_MEMC_DDR23_APHY_WL1_0_ANALOG_BYPASS_CNTRL_reserved0_MASK 0xfffffffc
#define BCHP_MEMC_DDR23_APHY_WL1_0_ANALOG_BYPASS_CNTRL_reserved0_SHIFT 2

/* MEMC_DDR23_APHY_WL1_0 :: ANALOG_BYPASS_CNTRL :: DISABLE_BYPASS_WLANEDLL [01:01] */
#define BCHP_MEMC_DDR23_APHY_WL1_0_ANALOG_BYPASS_CNTRL_DISABLE_BYPASS_WLANEDLL_MASK 0x00000002
#define BCHP_MEMC_DDR23_APHY_WL1_0_ANALOG_BYPASS_CNTRL_DISABLE_BYPASS_WLANEDLL_SHIFT 1

/* MEMC_DDR23_APHY_WL1_0 :: ANALOG_BYPASS_CNTRL :: BYPASS_WLANE [00:00] */
#define BCHP_MEMC_DDR23_APHY_WL1_0_ANALOG_BYPASS_CNTRL_BYPASS_WLANE_MASK 0x00000001
#define BCHP_MEMC_DDR23_APHY_WL1_0_ANALOG_BYPASS_CNTRL_BYPASS_WLANE_SHIFT 0

/***************************************************************************
 *PFIFO_RD_WR_PNTR
 ***************************************************************************/
/* MEMC_DDR23_APHY_WL1_0 :: PFIFO_RD_WR_PNTR :: reserved0 [31:08] */
#define BCHP_MEMC_DDR23_APHY_WL1_0_PFIFO_RD_WR_PNTR_reserved0_MASK 0xffffff00
#define BCHP_MEMC_DDR23_APHY_WL1_0_PFIFO_RD_WR_PNTR_reserved0_SHIFT 8

/* MEMC_DDR23_APHY_WL1_0 :: PFIFO_RD_WR_PNTR :: RD_PNTR1 [07:06] */
#define BCHP_MEMC_DDR23_APHY_WL1_0_PFIFO_RD_WR_PNTR_RD_PNTR1_MASK  0x000000c0
#define BCHP_MEMC_DDR23_APHY_WL1_0_PFIFO_RD_WR_PNTR_RD_PNTR1_SHIFT 6

/* MEMC_DDR23_APHY_WL1_0 :: PFIFO_RD_WR_PNTR :: RD_PNTR0 [05:04] */
#define BCHP_MEMC_DDR23_APHY_WL1_0_PFIFO_RD_WR_PNTR_RD_PNTR0_MASK  0x00000030
#define BCHP_MEMC_DDR23_APHY_WL1_0_PFIFO_RD_WR_PNTR_RD_PNTR0_SHIFT 4

/* MEMC_DDR23_APHY_WL1_0 :: PFIFO_RD_WR_PNTR :: WR_PNTR1 [03:02] */
#define BCHP_MEMC_DDR23_APHY_WL1_0_PFIFO_RD_WR_PNTR_WR_PNTR1_MASK  0x0000000c
#define BCHP_MEMC_DDR23_APHY_WL1_0_PFIFO_RD_WR_PNTR_WR_PNTR1_SHIFT 2

/* MEMC_DDR23_APHY_WL1_0 :: PFIFO_RD_WR_PNTR :: WR_PNTR0 [01:00] */
#define BCHP_MEMC_DDR23_APHY_WL1_0_PFIFO_RD_WR_PNTR_WR_PNTR0_MASK  0x00000003
#define BCHP_MEMC_DDR23_APHY_WL1_0_PFIFO_RD_WR_PNTR_WR_PNTR0_SHIFT 0

/***************************************************************************
 *PAD_SSTL_DDR2_MODE
 ***************************************************************************/
/* MEMC_DDR23_APHY_WL1_0 :: PAD_SSTL_DDR2_MODE :: reserved0 [31:02] */
#define BCHP_MEMC_DDR23_APHY_WL1_0_PAD_SSTL_DDR2_MODE_reserved0_MASK 0xfffffffc
#define BCHP_MEMC_DDR23_APHY_WL1_0_PAD_SSTL_DDR2_MODE_reserved0_SHIFT 2

/* MEMC_DDR23_APHY_WL1_0 :: PAD_SSTL_DDR2_MODE :: DATA [01:01] */
#define BCHP_MEMC_DDR23_APHY_WL1_0_PAD_SSTL_DDR2_MODE_DATA_MASK    0x00000002
#define BCHP_MEMC_DDR23_APHY_WL1_0_PAD_SSTL_DDR2_MODE_DATA_SHIFT   1

/* MEMC_DDR23_APHY_WL1_0 :: PAD_SSTL_DDR2_MODE :: DQS [00:00] */
#define BCHP_MEMC_DDR23_APHY_WL1_0_PAD_SSTL_DDR2_MODE_DQS_MASK     0x00000001
#define BCHP_MEMC_DDR23_APHY_WL1_0_PAD_SSTL_DDR2_MODE_DQS_SHIFT    0

/***************************************************************************
 *DDR_PAD_CNTRL
 ***************************************************************************/
/* MEMC_DDR23_APHY_WL1_0 :: DDR_PAD_CNTRL :: reserved0 [31:03] */
#define BCHP_MEMC_DDR23_APHY_WL1_0_DDR_PAD_CNTRL_reserved0_MASK    0xfffffff8
#define BCHP_MEMC_DDR23_APHY_WL1_0_DDR_PAD_CNTRL_reserved0_SHIFT   3

/* MEMC_DDR23_APHY_WL1_0 :: DDR_PAD_CNTRL :: IDDQ_MODE_ON_SELFREF [02:02] */
#define BCHP_MEMC_DDR23_APHY_WL1_0_DDR_PAD_CNTRL_IDDQ_MODE_ON_SELFREF_MASK 0x00000004
#define BCHP_MEMC_DDR23_APHY_WL1_0_DDR_PAD_CNTRL_IDDQ_MODE_ON_SELFREF_SHIFT 2

/* MEMC_DDR23_APHY_WL1_0 :: DDR_PAD_CNTRL :: CNTRL [01:00] */
#define BCHP_MEMC_DDR23_APHY_WL1_0_DDR_PAD_CNTRL_CNTRL_MASK        0x00000003
#define BCHP_MEMC_DDR23_APHY_WL1_0_DDR_PAD_CNTRL_CNTRL_SHIFT       0

/***************************************************************************
 *DDR_PAD_SLEW_CNTRL
 ***************************************************************************/
/* MEMC_DDR23_APHY_WL1_0 :: DDR_PAD_SLEW_CNTRL :: reserved0 [31:02] */
#define BCHP_MEMC_DDR23_APHY_WL1_0_DDR_PAD_SLEW_CNTRL_reserved0_MASK 0xfffffffc
#define BCHP_MEMC_DDR23_APHY_WL1_0_DDR_PAD_SLEW_CNTRL_reserved0_SHIFT 2

/* MEMC_DDR23_APHY_WL1_0 :: DDR_PAD_SLEW_CNTRL :: SLEW [01:00] */
#define BCHP_MEMC_DDR23_APHY_WL1_0_DDR_PAD_SLEW_CNTRL_SLEW_MASK    0x00000003
#define BCHP_MEMC_DDR23_APHY_WL1_0_DDR_PAD_SLEW_CNTRL_SLEW_SHIFT   0

/***************************************************************************
 *DDR_PAD_RX_DRV_CNTRL
 ***************************************************************************/
/* MEMC_DDR23_APHY_WL1_0 :: DDR_PAD_RX_DRV_CNTRL :: reserved0 [31:02] */
#define BCHP_MEMC_DDR23_APHY_WL1_0_DDR_PAD_RX_DRV_CNTRL_reserved0_MASK 0xfffffffc
#define BCHP_MEMC_DDR23_APHY_WL1_0_DDR_PAD_RX_DRV_CNTRL_reserved0_SHIFT 2

/* MEMC_DDR23_APHY_WL1_0 :: DDR_PAD_RX_DRV_CNTRL :: RX_DRV [01:00] */
#define BCHP_MEMC_DDR23_APHY_WL1_0_DDR_PAD_RX_DRV_CNTRL_RX_DRV_MASK 0x00000003
#define BCHP_MEMC_DDR23_APHY_WL1_0_DDR_PAD_RX_DRV_CNTRL_RX_DRV_SHIFT 0

/***************************************************************************
 *DDR_PAD_TX_DRV_CNTRL
 ***************************************************************************/
/* MEMC_DDR23_APHY_WL1_0 :: DDR_PAD_TX_DRV_CNTRL :: reserved0 [31:01] */
#define BCHP_MEMC_DDR23_APHY_WL1_0_DDR_PAD_TX_DRV_CNTRL_reserved0_MASK 0xfffffffe
#define BCHP_MEMC_DDR23_APHY_WL1_0_DDR_PAD_TX_DRV_CNTRL_reserved0_SHIFT 1

/* MEMC_DDR23_APHY_WL1_0 :: DDR_PAD_TX_DRV_CNTRL :: TX_DRV [00:00] */
#define BCHP_MEMC_DDR23_APHY_WL1_0_DDR_PAD_TX_DRV_CNTRL_TX_DRV_MASK 0x00000001
#define BCHP_MEMC_DDR23_APHY_WL1_0_DDR_PAD_TX_DRV_CNTRL_TX_DRV_SHIFT 0

/***************************************************************************
 *MISC
 ***************************************************************************/
/* MEMC_DDR23_APHY_WL1_0 :: MISC :: reserved_for_eco0 [31:04] */
#define BCHP_MEMC_DDR23_APHY_WL1_0_MISC_reserved_for_eco0_MASK     0xfffffff0
#define BCHP_MEMC_DDR23_APHY_WL1_0_MISC_reserved_for_eco0_SHIFT    4

/* MEMC_DDR23_APHY_WL1_0 :: MISC :: AUTO_SQUELCH [03:03] */
#define BCHP_MEMC_DDR23_APHY_WL1_0_MISC_AUTO_SQUELCH_MASK          0x00000008
#define BCHP_MEMC_DDR23_APHY_WL1_0_MISC_AUTO_SQUELCH_SHIFT         3

/* MEMC_DDR23_APHY_WL1_0 :: MISC :: ALLOW_DQM_TOGGLE [02:02] */
#define BCHP_MEMC_DDR23_APHY_WL1_0_MISC_ALLOW_DQM_TOGGLE_MASK      0x00000004
#define BCHP_MEMC_DDR23_APHY_WL1_0_MISC_ALLOW_DQM_TOGGLE_SHIFT     2

/* MEMC_DDR23_APHY_WL1_0 :: MISC :: FUNC1 [01:01] */
#define BCHP_MEMC_DDR23_APHY_WL1_0_MISC_FUNC1_MASK                 0x00000002
#define BCHP_MEMC_DDR23_APHY_WL1_0_MISC_FUNC1_SHIFT                1

/* MEMC_DDR23_APHY_WL1_0 :: MISC :: FUNC0 [00:00] */
#define BCHP_MEMC_DDR23_APHY_WL1_0_MISC_FUNC0_MASK                 0x00000001
#define BCHP_MEMC_DDR23_APHY_WL1_0_MISC_FUNC0_SHIFT                0

/***************************************************************************
 *SPARE0_RW
 ***************************************************************************/
/* MEMC_DDR23_APHY_WL1_0 :: SPARE0_RW :: reserved_for_eco0 [31:00] */
#define BCHP_MEMC_DDR23_APHY_WL1_0_SPARE0_RW_reserved_for_eco0_MASK 0xffffffff
#define BCHP_MEMC_DDR23_APHY_WL1_0_SPARE0_RW_reserved_for_eco0_SHIFT 0

/***************************************************************************
 *SPARE0_RO
 ***************************************************************************/
/* MEMC_DDR23_APHY_WL1_0 :: SPARE0_RO :: reserved_for_eco0 [31:00] */
#define BCHP_MEMC_DDR23_APHY_WL1_0_SPARE0_RO_reserved_for_eco0_MASK 0xffffffff
#define BCHP_MEMC_DDR23_APHY_WL1_0_SPARE0_RO_reserved_for_eco0_SHIFT 0

#endif /* #ifndef BCHP_MEMC_DDR23_APHY_WL1_0_H__ */

/* End of File */
