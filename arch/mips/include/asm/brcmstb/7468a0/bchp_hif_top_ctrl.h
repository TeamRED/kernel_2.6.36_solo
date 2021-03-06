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
 * Date:           Generated on         Fri Sep 11 19:45:22 2009
 *                 MD5 Checksum         957f01e03a68c1766fd2e8ad6484f5f9
 *
 * Compiled with:  RDB Utility          combo_header.pl
 *                 RDB Parser           3.0
 *                 unknown              unknown
 *                 Perl Interpreter     5.008008
 *                 Operating System     linux
 *
 * Revision History:
 *
 * $brcm_Log: /magnum/basemodules/chp/7468/rdb/a0/bchp_hif_top_ctrl.h $
 * 
 * Hydra_Software_Devel/1   9/14/09 4:33p albertl
 * SW7468-3: Initial revision.
 *
 ***************************************************************************/

#ifndef BCHP_HIF_TOP_CTRL_H__
#define BCHP_HIF_TOP_CTRL_H__

/***************************************************************************
 *HIF_TOP_CTRL - HIF Top Control Registers
 ***************************************************************************/
#define BCHP_HIF_TOP_CTRL_EXT_IRQ_LEVEL          0x00312400 /* External IRQ Active Level Control Register */
#define BCHP_HIF_TOP_CTRL_SPI_DBG_SEL            0x00312404 /* SPI test port select register */
#define BCHP_HIF_TOP_CTRL_SDIO_CTRL              0x00312408 /* SDIO Control Register */
#define BCHP_HIF_TOP_CTRL_PM_CTRL                0x0031240c /* HIF Power Management Control Register */
#define BCHP_HIF_TOP_CTRL_STRAP_INTERCEPT        0x00312410 /* HIF Strap Intercept Register */
#define BCHP_HIF_TOP_CTRL_FLASH_TYPE             0x00312418 /* HIF Decoded Flash Type */
#define BCHP_HIF_TOP_CTRL_SCRATCH                0x0031241c /* HIF Scratch register */

/***************************************************************************
 *EXT_IRQ_LEVEL - External IRQ Active Level Control Register
 ***************************************************************************/
/* HIF_TOP_CTRL :: EXT_IRQ_LEVEL :: reserved0 [31:05] */
#define BCHP_HIF_TOP_CTRL_EXT_IRQ_LEVEL_reserved0_MASK             0xffffffe0
#define BCHP_HIF_TOP_CTRL_EXT_IRQ_LEVEL_reserved0_SHIFT            5

/* HIF_TOP_CTRL :: EXT_IRQ_LEVEL :: ext_irq_4_level [04:04] */
#define BCHP_HIF_TOP_CTRL_EXT_IRQ_LEVEL_ext_irq_4_level_MASK       0x00000010
#define BCHP_HIF_TOP_CTRL_EXT_IRQ_LEVEL_ext_irq_4_level_SHIFT      4
#define BCHP_HIF_TOP_CTRL_EXT_IRQ_LEVEL_ext_irq_4_level_LOW        0
#define BCHP_HIF_TOP_CTRL_EXT_IRQ_LEVEL_ext_irq_4_level_HIGH       1

/* HIF_TOP_CTRL :: EXT_IRQ_LEVEL :: ext_irq_3_level [03:03] */
#define BCHP_HIF_TOP_CTRL_EXT_IRQ_LEVEL_ext_irq_3_level_MASK       0x00000008
#define BCHP_HIF_TOP_CTRL_EXT_IRQ_LEVEL_ext_irq_3_level_SHIFT      3
#define BCHP_HIF_TOP_CTRL_EXT_IRQ_LEVEL_ext_irq_3_level_LOW        0
#define BCHP_HIF_TOP_CTRL_EXT_IRQ_LEVEL_ext_irq_3_level_HIGH       1

/* HIF_TOP_CTRL :: EXT_IRQ_LEVEL :: ext_irq_2_level [02:02] */
#define BCHP_HIF_TOP_CTRL_EXT_IRQ_LEVEL_ext_irq_2_level_MASK       0x00000004
#define BCHP_HIF_TOP_CTRL_EXT_IRQ_LEVEL_ext_irq_2_level_SHIFT      2
#define BCHP_HIF_TOP_CTRL_EXT_IRQ_LEVEL_ext_irq_2_level_LOW        0
#define BCHP_HIF_TOP_CTRL_EXT_IRQ_LEVEL_ext_irq_2_level_HIGH       1

/* HIF_TOP_CTRL :: EXT_IRQ_LEVEL :: ext_irq_1_level [01:01] */
#define BCHP_HIF_TOP_CTRL_EXT_IRQ_LEVEL_ext_irq_1_level_MASK       0x00000002
#define BCHP_HIF_TOP_CTRL_EXT_IRQ_LEVEL_ext_irq_1_level_SHIFT      1
#define BCHP_HIF_TOP_CTRL_EXT_IRQ_LEVEL_ext_irq_1_level_LOW        0
#define BCHP_HIF_TOP_CTRL_EXT_IRQ_LEVEL_ext_irq_1_level_HIGH       1

/* HIF_TOP_CTRL :: EXT_IRQ_LEVEL :: ext_irq_0_level [00:00] */
#define BCHP_HIF_TOP_CTRL_EXT_IRQ_LEVEL_ext_irq_0_level_MASK       0x00000001
#define BCHP_HIF_TOP_CTRL_EXT_IRQ_LEVEL_ext_irq_0_level_SHIFT      0
#define BCHP_HIF_TOP_CTRL_EXT_IRQ_LEVEL_ext_irq_0_level_LOW        0
#define BCHP_HIF_TOP_CTRL_EXT_IRQ_LEVEL_ext_irq_0_level_HIGH       1

/***************************************************************************
 *SPI_DBG_SEL - SPI test port select register
 ***************************************************************************/
/* HIF_TOP_CTRL :: SPI_DBG_SEL :: reserved0 [31:03] */
#define BCHP_HIF_TOP_CTRL_SPI_DBG_SEL_reserved0_MASK               0xfffffff8
#define BCHP_HIF_TOP_CTRL_SPI_DBG_SEL_reserved0_SHIFT              3

/* HIF_TOP_CTRL :: SPI_DBG_SEL :: DISABLE_MSPI_FLUSH [02:02] */
#define BCHP_HIF_TOP_CTRL_SPI_DBG_SEL_DISABLE_MSPI_FLUSH_MASK      0x00000004
#define BCHP_HIF_TOP_CTRL_SPI_DBG_SEL_DISABLE_MSPI_FLUSH_SHIFT     2

/* HIF_TOP_CTRL :: SPI_DBG_SEL :: SPI_RBUS_TIMER_EN [01:01] */
#define BCHP_HIF_TOP_CTRL_SPI_DBG_SEL_SPI_RBUS_TIMER_EN_MASK       0x00000002
#define BCHP_HIF_TOP_CTRL_SPI_DBG_SEL_SPI_RBUS_TIMER_EN_SHIFT      1

/* HIF_TOP_CTRL :: SPI_DBG_SEL :: reserved1 [00:00] */
#define BCHP_HIF_TOP_CTRL_SPI_DBG_SEL_reserved1_MASK               0x00000001
#define BCHP_HIF_TOP_CTRL_SPI_DBG_SEL_reserved1_SHIFT              0

/***************************************************************************
 *SDIO_CTRL - SDIO Control Register
 ***************************************************************************/
/* HIF_TOP_CTRL :: SDIO_CTRL :: reserved_for_eco0 [31:21] */
#define BCHP_HIF_TOP_CTRL_SDIO_CTRL_reserved_for_eco0_MASK         0xffe00000
#define BCHP_HIF_TOP_CTRL_SDIO_CTRL_reserved_for_eco0_SHIFT        21

/* HIF_TOP_CTRL :: SDIO_CTRL :: HREADY_IDLE_ENA [20:20] */
#define BCHP_HIF_TOP_CTRL_SDIO_CTRL_HREADY_IDLE_ENA_MASK           0x00100000
#define BCHP_HIF_TOP_CTRL_SDIO_CTRL_HREADY_IDLE_ENA_SHIFT          20

/* HIF_TOP_CTRL :: SDIO_CTRL :: HREADY_IDLE_PULSE [19:19] */
#define BCHP_HIF_TOP_CTRL_SDIO_CTRL_HREADY_IDLE_PULSE_MASK         0x00080000
#define BCHP_HIF_TOP_CTRL_SDIO_CTRL_HREADY_IDLE_PULSE_SHIFT        19

/* HIF_TOP_CTRL :: SDIO_CTRL :: DATA_PENDING [18:18] */
#define BCHP_HIF_TOP_CTRL_SDIO_CTRL_DATA_PENDING_MASK              0x00040000
#define BCHP_HIF_TOP_CTRL_SDIO_CTRL_DATA_PENDING_SHIFT             18

/* HIF_TOP_CTRL :: SDIO_CTRL :: WR_FLUSH [17:17] */
#define BCHP_HIF_TOP_CTRL_SDIO_CTRL_WR_FLUSH_MASK                  0x00020000
#define BCHP_HIF_TOP_CTRL_SDIO_CTRL_WR_FLUSH_SHIFT                 17

/* HIF_TOP_CTRL :: SDIO_CTRL :: MF_NUM_WR [16:16] */
#define BCHP_HIF_TOP_CTRL_SDIO_CTRL_MF_NUM_WR_MASK                 0x00010000
#define BCHP_HIF_TOP_CTRL_SDIO_CTRL_MF_NUM_WR_SHIFT                16

/* HIF_TOP_CTRL :: SDIO_CTRL :: WORD_ABO [15:15] */
#define BCHP_HIF_TOP_CTRL_SDIO_CTRL_WORD_ABO_MASK                  0x00008000
#define BCHP_HIF_TOP_CTRL_SDIO_CTRL_WORD_ABO_SHIFT                 15

/* HIF_TOP_CTRL :: SDIO_CTRL :: FRAME_NBO [14:14] */
#define BCHP_HIF_TOP_CTRL_SDIO_CTRL_FRAME_NBO_MASK                 0x00004000
#define BCHP_HIF_TOP_CTRL_SDIO_CTRL_FRAME_NBO_SHIFT                14

/* HIF_TOP_CTRL :: SDIO_CTRL :: FRAME_NHW [13:13] */
#define BCHP_HIF_TOP_CTRL_SDIO_CTRL_FRAME_NHW_MASK                 0x00002000
#define BCHP_HIF_TOP_CTRL_SDIO_CTRL_FRAME_NHW_SHIFT                13

/* HIF_TOP_CTRL :: SDIO_CTRL :: BUFFER_ABO [12:12] */
#define BCHP_HIF_TOP_CTRL_SDIO_CTRL_BUFFER_ABO_MASK                0x00001000
#define BCHP_HIF_TOP_CTRL_SDIO_CTRL_BUFFER_ABO_SHIFT               12

/* HIF_TOP_CTRL :: SDIO_CTRL :: SCB_BUF_ACC [11:11] */
#define BCHP_HIF_TOP_CTRL_SDIO_CTRL_SCB_BUF_ACC_MASK               0x00000800
#define BCHP_HIF_TOP_CTRL_SDIO_CTRL_SCB_BUF_ACC_SHIFT              11

/* HIF_TOP_CTRL :: SDIO_CTRL :: SCB_SEQ_EN [10:10] */
#define BCHP_HIF_TOP_CTRL_SDIO_CTRL_SCB_SEQ_EN_MASK                0x00000400
#define BCHP_HIF_TOP_CTRL_SDIO_CTRL_SCB_SEQ_EN_SHIFT               10

/* HIF_TOP_CTRL :: SDIO_CTRL :: SCB_RD_THRESH [09:05] */
#define BCHP_HIF_TOP_CTRL_SDIO_CTRL_SCB_RD_THRESH_MASK             0x000003e0
#define BCHP_HIF_TOP_CTRL_SDIO_CTRL_SCB_RD_THRESH_SHIFT            5

/* HIF_TOP_CTRL :: SDIO_CTRL :: SCB_SIZE [04:00] */
#define BCHP_HIF_TOP_CTRL_SDIO_CTRL_SCB_SIZE_MASK                  0x0000001f
#define BCHP_HIF_TOP_CTRL_SDIO_CTRL_SCB_SIZE_SHIFT                 0

/***************************************************************************
 *PM_CTRL - HIF Power Management Control Register
 ***************************************************************************/
/* HIF_TOP_CTRL :: PM_CTRL :: reserved0 [31:14] */
#define BCHP_HIF_TOP_CTRL_PM_CTRL_reserved0_MASK                   0xffffc000
#define BCHP_HIF_TOP_CTRL_PM_CTRL_reserved0_SHIFT                  14

/* HIF_TOP_CTRL :: PM_CTRL :: PM_SF_OUT_FORCE_TRISTATE [13:13] */
#define BCHP_HIF_TOP_CTRL_PM_CTRL_PM_SF_OUT_FORCE_TRISTATE_MASK    0x00002000
#define BCHP_HIF_TOP_CTRL_PM_CTRL_PM_SF_OUT_FORCE_TRISTATE_SHIFT   13
#define BCHP_HIF_TOP_CTRL_PM_CTRL_PM_SF_OUT_FORCE_TRISTATE_DISABLE 0
#define BCHP_HIF_TOP_CTRL_PM_CTRL_PM_SF_OUT_FORCE_TRISTATE_ENABLE  1

/* HIF_TOP_CTRL :: PM_CTRL :: PM_NAND_DATA_DRIVE_LOW [12:12] */
#define BCHP_HIF_TOP_CTRL_PM_CTRL_PM_NAND_DATA_DRIVE_LOW_MASK      0x00001000
#define BCHP_HIF_TOP_CTRL_PM_CTRL_PM_NAND_DATA_DRIVE_LOW_SHIFT     12
#define BCHP_HIF_TOP_CTRL_PM_CTRL_PM_NAND_DATA_DRIVE_LOW_DISABLE   0
#define BCHP_HIF_TOP_CTRL_PM_CTRL_PM_NAND_DATA_DRIVE_LOW_ENABLE    1

/* HIF_TOP_CTRL :: PM_CTRL :: PM_NAND_REB_FORCE_TRISTATE [11:11] */
#define BCHP_HIF_TOP_CTRL_PM_CTRL_PM_NAND_REB_FORCE_TRISTATE_MASK  0x00000800
#define BCHP_HIF_TOP_CTRL_PM_CTRL_PM_NAND_REB_FORCE_TRISTATE_SHIFT 11
#define BCHP_HIF_TOP_CTRL_PM_CTRL_PM_NAND_REB_FORCE_TRISTATE_DISABLE 0
#define BCHP_HIF_TOP_CTRL_PM_CTRL_PM_NAND_REB_FORCE_TRISTATE_ENABLE 1

/* HIF_TOP_CTRL :: PM_CTRL :: PM_NAND_WEB_FORCE_TRISTATE [10:10] */
#define BCHP_HIF_TOP_CTRL_PM_CTRL_PM_NAND_WEB_FORCE_TRISTATE_MASK  0x00000400
#define BCHP_HIF_TOP_CTRL_PM_CTRL_PM_NAND_WEB_FORCE_TRISTATE_SHIFT 10
#define BCHP_HIF_TOP_CTRL_PM_CTRL_PM_NAND_WEB_FORCE_TRISTATE_DISABLE 0
#define BCHP_HIF_TOP_CTRL_PM_CTRL_PM_NAND_WEB_FORCE_TRISTATE_ENABLE 1

/* HIF_TOP_CTRL :: PM_CTRL :: PM_NAND_ALE_FORCE_TRISTATE [09:09] */
#define BCHP_HIF_TOP_CTRL_PM_CTRL_PM_NAND_ALE_FORCE_TRISTATE_MASK  0x00000200
#define BCHP_HIF_TOP_CTRL_PM_CTRL_PM_NAND_ALE_FORCE_TRISTATE_SHIFT 9
#define BCHP_HIF_TOP_CTRL_PM_CTRL_PM_NAND_ALE_FORCE_TRISTATE_DISABLE 0
#define BCHP_HIF_TOP_CTRL_PM_CTRL_PM_NAND_ALE_FORCE_TRISTATE_ENABLE 1

/* HIF_TOP_CTRL :: PM_CTRL :: PM_NAND_CLE_FORCE_TRISTATE [08:08] */
#define BCHP_HIF_TOP_CTRL_PM_CTRL_PM_NAND_CLE_FORCE_TRISTATE_MASK  0x00000100
#define BCHP_HIF_TOP_CTRL_PM_CTRL_PM_NAND_CLE_FORCE_TRISTATE_SHIFT 8
#define BCHP_HIF_TOP_CTRL_PM_CTRL_PM_NAND_CLE_FORCE_TRISTATE_DISABLE 0
#define BCHP_HIF_TOP_CTRL_PM_CTRL_PM_NAND_CLE_FORCE_TRISTATE_ENABLE 1

/* HIF_TOP_CTRL :: PM_CTRL :: PM_NAND_DATA_FORCE_TRISTATE [07:07] */
#define BCHP_HIF_TOP_CTRL_PM_CTRL_PM_NAND_DATA_FORCE_TRISTATE_MASK 0x00000080
#define BCHP_HIF_TOP_CTRL_PM_CTRL_PM_NAND_DATA_FORCE_TRISTATE_SHIFT 7
#define BCHP_HIF_TOP_CTRL_PM_CTRL_PM_NAND_DATA_FORCE_TRISTATE_DISABLE 0
#define BCHP_HIF_TOP_CTRL_PM_CTRL_PM_NAND_DATA_FORCE_TRISTATE_ENABLE 1

/* HIF_TOP_CTRL :: PM_CTRL :: PM_OUT_TRISTATE_CS2 [06:06] */
#define BCHP_HIF_TOP_CTRL_PM_CTRL_PM_OUT_TRISTATE_CS2_MASK         0x00000040
#define BCHP_HIF_TOP_CTRL_PM_CTRL_PM_OUT_TRISTATE_CS2_SHIFT        6

/* HIF_TOP_CTRL :: PM_CTRL :: PM_OUT_TRISTATE_CS1 [05:05] */
#define BCHP_HIF_TOP_CTRL_PM_CTRL_PM_OUT_TRISTATE_CS1_MASK         0x00000020
#define BCHP_HIF_TOP_CTRL_PM_CTRL_PM_OUT_TRISTATE_CS1_SHIFT        5

/* HIF_TOP_CTRL :: PM_CTRL :: PM_OUT_TRISTATE_CS0 [04:04] */
#define BCHP_HIF_TOP_CTRL_PM_CTRL_PM_OUT_TRISTATE_CS0_MASK         0x00000010
#define BCHP_HIF_TOP_CTRL_PM_CTRL_PM_OUT_TRISTATE_CS0_SHIFT        4

/* HIF_TOP_CTRL :: PM_CTRL :: reserved1 [03:00] */
#define BCHP_HIF_TOP_CTRL_PM_CTRL_reserved1_MASK                   0x0000000f
#define BCHP_HIF_TOP_CTRL_PM_CTRL_reserved1_SHIFT                  0

/***************************************************************************
 *STRAP_INTERCEPT - HIF Strap Intercept Register
 ***************************************************************************/
/* HIF_TOP_CTRL :: STRAP_INTERCEPT :: reserved0 [31:05] */
#define BCHP_HIF_TOP_CTRL_STRAP_INTERCEPT_reserved0_MASK           0xffffffe0
#define BCHP_HIF_TOP_CTRL_STRAP_INTERCEPT_reserved0_SHIFT          5

/* HIF_TOP_CTRL :: STRAP_INTERCEPT :: STRAP_EBI_BOOT_MEMORY [04:04] */
#define BCHP_HIF_TOP_CTRL_STRAP_INTERCEPT_STRAP_EBI_BOOT_MEMORY_MASK 0x00000010
#define BCHP_HIF_TOP_CTRL_STRAP_INTERCEPT_STRAP_EBI_BOOT_MEMORY_SHIFT 4

/* HIF_TOP_CTRL :: STRAP_INTERCEPT :: STRAP_EBI_BOOT_MEMORY_INTERCEPT_EN [03:03] */
#define BCHP_HIF_TOP_CTRL_STRAP_INTERCEPT_STRAP_EBI_BOOT_MEMORY_INTERCEPT_EN_MASK 0x00000008
#define BCHP_HIF_TOP_CTRL_STRAP_INTERCEPT_STRAP_EBI_BOOT_MEMORY_INTERCEPT_EN_SHIFT 3

/* HIF_TOP_CTRL :: STRAP_INTERCEPT :: STRAP_NAND_FLASH [02:02] */
#define BCHP_HIF_TOP_CTRL_STRAP_INTERCEPT_STRAP_NAND_FLASH_MASK    0x00000004
#define BCHP_HIF_TOP_CTRL_STRAP_INTERCEPT_STRAP_NAND_FLASH_SHIFT   2

/* HIF_TOP_CTRL :: STRAP_INTERCEPT :: STRAP_NAND_FLASH_INTERCEPT_EN [01:01] */
#define BCHP_HIF_TOP_CTRL_STRAP_INTERCEPT_STRAP_NAND_FLASH_INTERCEPT_EN_MASK 0x00000002
#define BCHP_HIF_TOP_CTRL_STRAP_INTERCEPT_STRAP_NAND_FLASH_INTERCEPT_EN_SHIFT 1

/* HIF_TOP_CTRL :: STRAP_INTERCEPT :: reserved1 [00:00] */
#define BCHP_HIF_TOP_CTRL_STRAP_INTERCEPT_reserved1_MASK           0x00000001
#define BCHP_HIF_TOP_CTRL_STRAP_INTERCEPT_reserved1_SHIFT          0

/***************************************************************************
 *FLASH_TYPE - HIF Decoded Flash Type
 ***************************************************************************/
/* HIF_TOP_CTRL :: FLASH_TYPE :: reserved0 [31:04] */
#define BCHP_HIF_TOP_CTRL_FLASH_TYPE_reserved0_MASK                0xfffffff0
#define BCHP_HIF_TOP_CTRL_FLASH_TYPE_reserved0_SHIFT               4

/* HIF_TOP_CTRL :: FLASH_TYPE :: InvalidStrap [03:03] */
#define BCHP_HIF_TOP_CTRL_FLASH_TYPE_InvalidStrap_MASK             0x00000008
#define BCHP_HIF_TOP_CTRL_FLASH_TYPE_InvalidStrap_SHIFT            3

/* HIF_TOP_CTRL :: FLASH_TYPE :: reserved1 [02:02] */
#define BCHP_HIF_TOP_CTRL_FLASH_TYPE_reserved1_MASK                0x00000004
#define BCHP_HIF_TOP_CTRL_FLASH_TYPE_reserved1_SHIFT               2

/* HIF_TOP_CTRL :: FLASH_TYPE :: FLASH_TYPE [01:00] */
#define BCHP_HIF_TOP_CTRL_FLASH_TYPE_FLASH_TYPE_MASK               0x00000003
#define BCHP_HIF_TOP_CTRL_FLASH_TYPE_FLASH_TYPE_SHIFT              0

/***************************************************************************
 *SCRATCH - HIF Scratch register
 ***************************************************************************/
/* HIF_TOP_CTRL :: SCRATCH :: SCRATCH_BITS [31:00] */
#define BCHP_HIF_TOP_CTRL_SCRATCH_SCRATCH_BITS_MASK                0xffffffff
#define BCHP_HIF_TOP_CTRL_SCRATCH_SCRATCH_BITS_SHIFT               0

#endif /* #ifndef BCHP_HIF_TOP_CTRL_H__ */

/* End of File */
