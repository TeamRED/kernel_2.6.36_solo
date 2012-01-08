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
 * Date:           Generated on         Mon Dec  7 01:17:15 2009
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

#ifndef BCHP_PCIX_BRIDGE_H__
#define BCHP_PCIX_BRIDGE_H__

/***************************************************************************
 *PCIX_BRIDGE
 ***************************************************************************/
#define BCHP_PCIX_BRIDGE_REVISION                0x00180200
#define BCHP_PCIX_BRIDGE_PCIX_CTRL               0x00180204
#define BCHP_PCIX_BRIDGE_SATA_CFG_INDEX          0x00180208
#define BCHP_PCIX_BRIDGE_SATA_CFG_DATA           0x0018020c
#define BCHP_PCIX_BRIDGE_PCIX_SLV_MEM_WIN_BASE   0x00180210
#define BCHP_PCIX_BRIDGE_PCIX_SLV_MEM_WIN_MODE   0x00180214
#define BCHP_PCIX_BRIDGE_CPU_TO_SATA_MEM_WIN_BASE 0x00180218
#define BCHP_PCIX_BRIDGE_CPU_TO_SATA_IO_WIN_BASE 0x0018021c
#define BCHP_PCIX_BRIDGE_RETRY_TIMER             0x00180220
#define BCHP_PCIX_BRIDGE_SCRATCH                 0x00180234
#define BCHP_PCIX_BRIDGE_DEBUGGER_MBIST_TM_CTRL  0x00180238
#define BCHP_PCIX_BRIDGE_TP_OUT                  0x0018023c
#define BCHP_PCIX_BRIDGE_PCIX_TESTMODE_CTRL      0x00180240
#define BCHP_PCIX_BRIDGE_PCIX_TESTMODE_PRBS_SEED 0x00180244
#define BCHP_PCIX_BRIDGE_PCIX_TESTMODE_CRC_W0    0x00180248
#define BCHP_PCIX_BRIDGE_PCIX_TESTMODE_CRC_W1    0x0018024c
#define BCHP_PCIX_BRIDGE_PCIX_TESTMODE_CRC_W2    0x00180250

/***************************************************************************
 *REVISION
 ***************************************************************************/
/* PCIX_BRIDGE :: REVISION :: reserved0 [31:16] */
#define BCHP_PCIX_BRIDGE_REVISION_reserved0_MASK                   0xffff0000
#define BCHP_PCIX_BRIDGE_REVISION_reserved0_SHIFT                  16

/* PCIX_BRIDGE :: REVISION :: MAJOR [15:08] */
#define BCHP_PCIX_BRIDGE_REVISION_MAJOR_MASK                       0x0000ff00
#define BCHP_PCIX_BRIDGE_REVISION_MAJOR_SHIFT                      8

/* PCIX_BRIDGE :: REVISION :: MINOR [07:00] */
#define BCHP_PCIX_BRIDGE_REVISION_MINOR_MASK                       0x000000ff
#define BCHP_PCIX_BRIDGE_REVISION_MINOR_SHIFT                      0

/***************************************************************************
 *PCIX_CTRL
 ***************************************************************************/
/* PCIX_BRIDGE :: PCIX_CTRL :: reserved0 [31:06] */
#define BCHP_PCIX_BRIDGE_PCIX_CTRL_reserved0_MASK                  0xffffffc0
#define BCHP_PCIX_BRIDGE_PCIX_CTRL_reserved0_SHIFT                 6

/* PCIX_BRIDGE :: PCIX_CTRL :: SERR_ENABLE [05:05] */
#define BCHP_PCIX_BRIDGE_PCIX_CTRL_SERR_ENABLE_MASK                0x00000020
#define BCHP_PCIX_BRIDGE_PCIX_CTRL_SERR_ENABLE_SHIFT               5
#define BCHP_PCIX_BRIDGE_PCIX_CTRL_SERR_ENABLE_ASSERTED            1
#define BCHP_PCIX_BRIDGE_PCIX_CTRL_SERR_ENABLE_DEASSERTED          0

/* PCIX_BRIDGE :: PCIX_CTRL :: PERR_ENABLE [04:04] */
#define BCHP_PCIX_BRIDGE_PCIX_CTRL_PERR_ENABLE_MASK                0x00000010
#define BCHP_PCIX_BRIDGE_PCIX_CTRL_PERR_ENABLE_SHIFT               4
#define BCHP_PCIX_BRIDGE_PCIX_CTRL_PERR_ENABLE_ASSERTED            1
#define BCHP_PCIX_BRIDGE_PCIX_CTRL_PERR_ENABLE_DEASSERTED          0

/* PCIX_BRIDGE :: PCIX_CTRL :: CFG_BYTE_ALIGN [03:03] */
#define BCHP_PCIX_BRIDGE_PCIX_CTRL_CFG_BYTE_ALIGN_MASK             0x00000008
#define BCHP_PCIX_BRIDGE_PCIX_CTRL_CFG_BYTE_ALIGN_SHIFT            3

/* PCIX_BRIDGE :: PCIX_CTRL :: PARK_LAST [02:02] */
#define BCHP_PCIX_BRIDGE_PCIX_CTRL_PARK_LAST_MASK                  0x00000004
#define BCHP_PCIX_BRIDGE_PCIX_CTRL_PARK_LAST_SHIFT                 2

/* PCIX_BRIDGE :: PCIX_CTRL :: BUS_MASTER [01:01] */
#define BCHP_PCIX_BRIDGE_PCIX_CTRL_BUS_MASTER_MASK                 0x00000002
#define BCHP_PCIX_BRIDGE_PCIX_CTRL_BUS_MASTER_SHIFT                1
#define BCHP_PCIX_BRIDGE_PCIX_CTRL_BUS_MASTER_ASSERTED             1
#define BCHP_PCIX_BRIDGE_PCIX_CTRL_BUS_MASTER_DEASSERTED           0

/* PCIX_BRIDGE :: PCIX_CTRL :: MEMORY_SPACE [00:00] */
#define BCHP_PCIX_BRIDGE_PCIX_CTRL_MEMORY_SPACE_MASK               0x00000001
#define BCHP_PCIX_BRIDGE_PCIX_CTRL_MEMORY_SPACE_SHIFT              0
#define BCHP_PCIX_BRIDGE_PCIX_CTRL_MEMORY_SPACE_ASSERTED           1
#define BCHP_PCIX_BRIDGE_PCIX_CTRL_MEMORY_SPACE_DEASSERTED         0

/***************************************************************************
 *SATA_CFG_INDEX
 ***************************************************************************/
/* PCIX_BRIDGE :: SATA_CFG_INDEX :: reserved0 [31:11] */
#define BCHP_PCIX_BRIDGE_SATA_CFG_INDEX_reserved0_MASK             0xfffff800
#define BCHP_PCIX_BRIDGE_SATA_CFG_INDEX_reserved0_SHIFT            11

/* PCIX_BRIDGE :: SATA_CFG_INDEX :: FUNC_NUM [10:08] */
#define BCHP_PCIX_BRIDGE_SATA_CFG_INDEX_FUNC_NUM_MASK              0x00000700
#define BCHP_PCIX_BRIDGE_SATA_CFG_INDEX_FUNC_NUM_SHIFT             8

/* PCIX_BRIDGE :: SATA_CFG_INDEX :: REG_NUM [07:00] */
#define BCHP_PCIX_BRIDGE_SATA_CFG_INDEX_REG_NUM_MASK               0x000000ff
#define BCHP_PCIX_BRIDGE_SATA_CFG_INDEX_REG_NUM_SHIFT              0

/***************************************************************************
 *SATA_CFG_DATA
 ***************************************************************************/
/* PCIX_BRIDGE :: SATA_CFG_DATA :: DATA [31:00] */
#define BCHP_PCIX_BRIDGE_SATA_CFG_DATA_DATA_MASK                   0xffffffff
#define BCHP_PCIX_BRIDGE_SATA_CFG_DATA_DATA_SHIFT                  0

/***************************************************************************
 *PCIX_SLV_MEM_WIN_BASE
 ***************************************************************************/
/* PCIX_BRIDGE :: PCIX_SLV_MEM_WIN_BASE :: BASE_ADRS [31:00] */
#define BCHP_PCIX_BRIDGE_PCIX_SLV_MEM_WIN_BASE_BASE_ADRS_MASK      0xffffffff
#define BCHP_PCIX_BRIDGE_PCIX_SLV_MEM_WIN_BASE_BASE_ADRS_SHIFT     0

/***************************************************************************
 *PCIX_SLV_MEM_WIN_MODE
 ***************************************************************************/
/* PCIX_BRIDGE :: PCIX_SLV_MEM_WIN_MODE :: reserved0 [31:02] */
#define BCHP_PCIX_BRIDGE_PCIX_SLV_MEM_WIN_MODE_reserved0_MASK      0xfffffffc
#define BCHP_PCIX_BRIDGE_PCIX_SLV_MEM_WIN_MODE_reserved0_SHIFT     2

/* PCIX_BRIDGE :: PCIX_SLV_MEM_WIN_MODE :: ENDIAN_MODE [01:00] */
#define BCHP_PCIX_BRIDGE_PCIX_SLV_MEM_WIN_MODE_ENDIAN_MODE_MASK    0x00000003
#define BCHP_PCIX_BRIDGE_PCIX_SLV_MEM_WIN_MODE_ENDIAN_MODE_SHIFT   0

/***************************************************************************
 *CPU_TO_SATA_MEM_WIN_BASE
 ***************************************************************************/
/* PCIX_BRIDGE :: CPU_TO_SATA_MEM_WIN_BASE :: BASE_ADRS [31:16] */
#define BCHP_PCIX_BRIDGE_CPU_TO_SATA_MEM_WIN_BASE_BASE_ADRS_MASK   0xffff0000
#define BCHP_PCIX_BRIDGE_CPU_TO_SATA_MEM_WIN_BASE_BASE_ADRS_SHIFT  16

/* PCIX_BRIDGE :: CPU_TO_SATA_MEM_WIN_BASE :: reserved0 [15:02] */
#define BCHP_PCIX_BRIDGE_CPU_TO_SATA_MEM_WIN_BASE_reserved0_MASK   0x0000fffc
#define BCHP_PCIX_BRIDGE_CPU_TO_SATA_MEM_WIN_BASE_reserved0_SHIFT  2

/* PCIX_BRIDGE :: CPU_TO_SATA_MEM_WIN_BASE :: ENDIAN_MODE [01:00] */
#define BCHP_PCIX_BRIDGE_CPU_TO_SATA_MEM_WIN_BASE_ENDIAN_MODE_MASK 0x00000003
#define BCHP_PCIX_BRIDGE_CPU_TO_SATA_MEM_WIN_BASE_ENDIAN_MODE_SHIFT 0

/***************************************************************************
 *CPU_TO_SATA_IO_WIN_BASE
 ***************************************************************************/
/* PCIX_BRIDGE :: CPU_TO_SATA_IO_WIN_BASE :: BASE_ADRS [31:16] */
#define BCHP_PCIX_BRIDGE_CPU_TO_SATA_IO_WIN_BASE_BASE_ADRS_MASK    0xffff0000
#define BCHP_PCIX_BRIDGE_CPU_TO_SATA_IO_WIN_BASE_BASE_ADRS_SHIFT   16

/* PCIX_BRIDGE :: CPU_TO_SATA_IO_WIN_BASE :: reserved0 [15:02] */
#define BCHP_PCIX_BRIDGE_CPU_TO_SATA_IO_WIN_BASE_reserved0_MASK    0x0000fffc
#define BCHP_PCIX_BRIDGE_CPU_TO_SATA_IO_WIN_BASE_reserved0_SHIFT   2

/* PCIX_BRIDGE :: CPU_TO_SATA_IO_WIN_BASE :: ENDIAN_MODE [01:00] */
#define BCHP_PCIX_BRIDGE_CPU_TO_SATA_IO_WIN_BASE_ENDIAN_MODE_MASK  0x00000003
#define BCHP_PCIX_BRIDGE_CPU_TO_SATA_IO_WIN_BASE_ENDIAN_MODE_SHIFT 0

/***************************************************************************
 *RETRY_TIMER
 ***************************************************************************/
/* PCIX_BRIDGE :: RETRY_TIMER :: reserved0 [31:08] */
#define BCHP_PCIX_BRIDGE_RETRY_TIMER_reserved0_MASK                0xffffff00
#define BCHP_PCIX_BRIDGE_RETRY_TIMER_reserved0_SHIFT               8

/* PCIX_BRIDGE :: RETRY_TIMER :: TIMEOUT_COUNT [07:00] */
#define BCHP_PCIX_BRIDGE_RETRY_TIMER_TIMEOUT_COUNT_MASK            0x000000ff
#define BCHP_PCIX_BRIDGE_RETRY_TIMER_TIMEOUT_COUNT_SHIFT           0

/***************************************************************************
 *SCRATCH
 ***************************************************************************/
/* PCIX_BRIDGE :: SCRATCH :: DATA [31:00] */
#define BCHP_PCIX_BRIDGE_SCRATCH_DATA_MASK                         0xffffffff
#define BCHP_PCIX_BRIDGE_SCRATCH_DATA_SHIFT                        0

/***************************************************************************
 *DEBUGGER_MBIST_TM_CTRL
 ***************************************************************************/
/* PCIX_BRIDGE :: DEBUGGER_MBIST_TM_CTRL :: reserved0 [31:16] */
#define BCHP_PCIX_BRIDGE_DEBUGGER_MBIST_TM_CTRL_reserved0_MASK     0xffff0000
#define BCHP_PCIX_BRIDGE_DEBUGGER_MBIST_TM_CTRL_reserved0_SHIFT    16

/* PCIX_BRIDGE :: DEBUGGER_MBIST_TM_CTRL :: DBG_TM_5 [15:14] */
#define BCHP_PCIX_BRIDGE_DEBUGGER_MBIST_TM_CTRL_DBG_TM_5_MASK      0x0000c000
#define BCHP_PCIX_BRIDGE_DEBUGGER_MBIST_TM_CTRL_DBG_TM_5_SHIFT     14

/* PCIX_BRIDGE :: DEBUGGER_MBIST_TM_CTRL :: DBG_TM_4 [13:12] */
#define BCHP_PCIX_BRIDGE_DEBUGGER_MBIST_TM_CTRL_DBG_TM_4_MASK      0x00003000
#define BCHP_PCIX_BRIDGE_DEBUGGER_MBIST_TM_CTRL_DBG_TM_4_SHIFT     12

/* PCIX_BRIDGE :: DEBUGGER_MBIST_TM_CTRL :: DBG_TM_3 [11:10] */
#define BCHP_PCIX_BRIDGE_DEBUGGER_MBIST_TM_CTRL_DBG_TM_3_MASK      0x00000c00
#define BCHP_PCIX_BRIDGE_DEBUGGER_MBIST_TM_CTRL_DBG_TM_3_SHIFT     10

/* PCIX_BRIDGE :: DEBUGGER_MBIST_TM_CTRL :: DBG_TM_2 [09:08] */
#define BCHP_PCIX_BRIDGE_DEBUGGER_MBIST_TM_CTRL_DBG_TM_2_MASK      0x00000300
#define BCHP_PCIX_BRIDGE_DEBUGGER_MBIST_TM_CTRL_DBG_TM_2_SHIFT     8

/* PCIX_BRIDGE :: DEBUGGER_MBIST_TM_CTRL :: DBG_TM_1 [07:04] */
#define BCHP_PCIX_BRIDGE_DEBUGGER_MBIST_TM_CTRL_DBG_TM_1_MASK      0x000000f0
#define BCHP_PCIX_BRIDGE_DEBUGGER_MBIST_TM_CTRL_DBG_TM_1_SHIFT     4

/* PCIX_BRIDGE :: DEBUGGER_MBIST_TM_CTRL :: DBG_TM_0 [03:00] */
#define BCHP_PCIX_BRIDGE_DEBUGGER_MBIST_TM_CTRL_DBG_TM_0_MASK      0x0000000f
#define BCHP_PCIX_BRIDGE_DEBUGGER_MBIST_TM_CTRL_DBG_TM_0_SHIFT     0

/***************************************************************************
 *TP_OUT
 ***************************************************************************/
/* PCIX_BRIDGE :: TP_OUT :: PCIX_TP_OUT [31:00] */
#define BCHP_PCIX_BRIDGE_TP_OUT_PCIX_TP_OUT_MASK                   0xffffffff
#define BCHP_PCIX_BRIDGE_TP_OUT_PCIX_TP_OUT_SHIFT                  0

/***************************************************************************
 *PCIX_TESTMODE_CTRL
 ***************************************************************************/
/* PCIX_BRIDGE :: PCIX_TESTMODE_CTRL :: SCB_RD_CYC_DLY [31:20] */
#define BCHP_PCIX_BRIDGE_PCIX_TESTMODE_CTRL_SCB_RD_CYC_DLY_MASK    0xfff00000
#define BCHP_PCIX_BRIDGE_PCIX_TESTMODE_CTRL_SCB_RD_CYC_DLY_SHIFT   20

/* PCIX_BRIDGE :: PCIX_TESTMODE_CTRL :: SCB_WR_CYC_DLY [19:08] */
#define BCHP_PCIX_BRIDGE_PCIX_TESTMODE_CTRL_SCB_WR_CYC_DLY_MASK    0x000fff00
#define BCHP_PCIX_BRIDGE_PCIX_TESTMODE_CTRL_SCB_WR_CYC_DLY_SHIFT   8

/* PCIX_BRIDGE :: PCIX_TESTMODE_CTRL :: reserved0 [07:04] */
#define BCHP_PCIX_BRIDGE_PCIX_TESTMODE_CTRL_reserved0_MASK         0x000000f0
#define BCHP_PCIX_BRIDGE_PCIX_TESTMODE_CTRL_reserved0_SHIFT        4

/* PCIX_BRIDGE :: PCIX_TESTMODE_CTRL :: LD_PRBS_SEED_H [03:03] */
#define BCHP_PCIX_BRIDGE_PCIX_TESTMODE_CTRL_LD_PRBS_SEED_H_MASK    0x00000008
#define BCHP_PCIX_BRIDGE_PCIX_TESTMODE_CTRL_LD_PRBS_SEED_H_SHIFT   3

/* PCIX_BRIDGE :: PCIX_TESTMODE_CTRL :: LD_PRBS_SEED_L [02:02] */
#define BCHP_PCIX_BRIDGE_PCIX_TESTMODE_CTRL_LD_PRBS_SEED_L_MASK    0x00000004
#define BCHP_PCIX_BRIDGE_PCIX_TESTMODE_CTRL_LD_PRBS_SEED_L_SHIFT   2

/* PCIX_BRIDGE :: PCIX_TESTMODE_CTRL :: EN_RW_SCB_DLYS [01:01] */
#define BCHP_PCIX_BRIDGE_PCIX_TESTMODE_CTRL_EN_RW_SCB_DLYS_MASK    0x00000002
#define BCHP_PCIX_BRIDGE_PCIX_TESTMODE_CTRL_EN_RW_SCB_DLYS_SHIFT   1

/* PCIX_BRIDGE :: PCIX_TESTMODE_CTRL :: PCIX_TEST_MODE [00:00] */
#define BCHP_PCIX_BRIDGE_PCIX_TESTMODE_CTRL_PCIX_TEST_MODE_MASK    0x00000001
#define BCHP_PCIX_BRIDGE_PCIX_TESTMODE_CTRL_PCIX_TEST_MODE_SHIFT   0

/***************************************************************************
 *PCIX_TESTMODE_PRBS_SEED
 ***************************************************************************/
/* PCIX_BRIDGE :: PCIX_TESTMODE_PRBS_SEED :: PCIX_TESTMODE_PRBS_SEED [31:00] */
#define BCHP_PCIX_BRIDGE_PCIX_TESTMODE_PRBS_SEED_PCIX_TESTMODE_PRBS_SEED_MASK 0xffffffff
#define BCHP_PCIX_BRIDGE_PCIX_TESTMODE_PRBS_SEED_PCIX_TESTMODE_PRBS_SEED_SHIFT 0

/***************************************************************************
 *PCIX_TESTMODE_CRC_W0
 ***************************************************************************/
/* PCIX_BRIDGE :: PCIX_TESTMODE_CRC_W0 :: PCIX_TESTMODE_CRC_W0 [31:00] */
#define BCHP_PCIX_BRIDGE_PCIX_TESTMODE_CRC_W0_PCIX_TESTMODE_CRC_W0_MASK 0xffffffff
#define BCHP_PCIX_BRIDGE_PCIX_TESTMODE_CRC_W0_PCIX_TESTMODE_CRC_W0_SHIFT 0

/***************************************************************************
 *PCIX_TESTMODE_CRC_W1
 ***************************************************************************/
/* PCIX_BRIDGE :: PCIX_TESTMODE_CRC_W1 :: PCIX_TESTMODE_CRC_W1 [31:00] */
#define BCHP_PCIX_BRIDGE_PCIX_TESTMODE_CRC_W1_PCIX_TESTMODE_CRC_W1_MASK 0xffffffff
#define BCHP_PCIX_BRIDGE_PCIX_TESTMODE_CRC_W1_PCIX_TESTMODE_CRC_W1_SHIFT 0

/***************************************************************************
 *PCIX_TESTMODE_CRC_W2
 ***************************************************************************/
/* PCIX_BRIDGE :: PCIX_TESTMODE_CRC_W2 :: PCIX_TESTMODE_CRC_W2 [31:00] */
#define BCHP_PCIX_BRIDGE_PCIX_TESTMODE_CRC_W2_PCIX_TESTMODE_CRC_W2_MASK 0xffffffff
#define BCHP_PCIX_BRIDGE_PCIX_TESTMODE_CRC_W2_PCIX_TESTMODE_CRC_W2_SHIFT 0

#endif /* #ifndef BCHP_PCIX_BRIDGE_H__ */

/* End of File */