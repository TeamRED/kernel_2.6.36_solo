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
 * Date:           Generated on         Mon May  4 11:59:20 2009
 *                 MD5 Checksum         4059064ad715d5aa8317c4361361b10e
 *
 * Compiled with:  RDB Utility          combo_header.pl
 *                 RDB Parser           3.0
 *                 unknown              unknown
 *                 Perl Interpreter     5.008008
 *                 Operating System     linux
 *
 * Revision History:
 *
 * $brcm_Log: /magnum/basemodules/chp/7420/rdb/b0/bchp_hif_top_ctrl.h $
 * 
 * Hydra_Software_Devel/4   5/4/09 1:05p tdo
 * PR50865: Update 7420 B0 header files
 *
 ***************************************************************************/

#ifndef BCHP_HIF_TOP_CTRL_H__
#define BCHP_HIF_TOP_CTRL_H__

/***************************************************************************
 *HIF_TOP_CTRL - HIF Top Control Registers
 ***************************************************************************/
#define BCHP_HIF_TOP_CTRL_EXT_IRQ_LEVEL          0x00442400 /* External IRQ Active Level Control Register */
#define BCHP_HIF_TOP_CTRL_SPI_DBG_SEL            0x00442404 /* SPI test port select register */
#define BCHP_HIF_TOP_CTRL_SCRATCH                0x00442408 /* HIF Scratch Register */
#define BCHP_HIF_TOP_CTRL_PM_CTRL                0x0044240c /* HIF Power Management Control Register */
#define BCHP_HIF_TOP_CTRL_STRAP_INTERCEPT        0x00442410 /* HIF Strap Intercept Register */
#define BCHP_HIF_TOP_CTRL_SPI_CLK_SEL            0x00442414 /* HIF SPI Interface Clock Select Register */
#define BCHP_HIF_TOP_CTRL_FLASH_TYPE             0x00442418 /* HIF Decoded Flash Type */
#define BCHP_HIF_TOP_CTRL_MWIN_CTRL              0x0044241c /* HIF PCI ConfigurationManager Mode MWIN Control Register */

/***************************************************************************
 *EXT_IRQ_LEVEL - External IRQ Active Level Control Register
 ***************************************************************************/
/* HIF_TOP_CTRL :: EXT_IRQ_LEVEL :: reserved0 [31:15] */
#define BCHP_HIF_TOP_CTRL_EXT_IRQ_LEVEL_reserved0_MASK             0xffff8000
#define BCHP_HIF_TOP_CTRL_EXT_IRQ_LEVEL_reserved0_SHIFT            15

/* HIF_TOP_CTRL :: EXT_IRQ_LEVEL :: ext_irq_14_level [14:14] */
#define BCHP_HIF_TOP_CTRL_EXT_IRQ_LEVEL_ext_irq_14_level_MASK      0x00004000
#define BCHP_HIF_TOP_CTRL_EXT_IRQ_LEVEL_ext_irq_14_level_SHIFT     14
#define BCHP_HIF_TOP_CTRL_EXT_IRQ_LEVEL_ext_irq_14_level_LOW       0
#define BCHP_HIF_TOP_CTRL_EXT_IRQ_LEVEL_ext_irq_14_level_HIGH      1

/* HIF_TOP_CTRL :: EXT_IRQ_LEVEL :: ext_irq_13_level [13:13] */
#define BCHP_HIF_TOP_CTRL_EXT_IRQ_LEVEL_ext_irq_13_level_MASK      0x00002000
#define BCHP_HIF_TOP_CTRL_EXT_IRQ_LEVEL_ext_irq_13_level_SHIFT     13
#define BCHP_HIF_TOP_CTRL_EXT_IRQ_LEVEL_ext_irq_13_level_LOW       0
#define BCHP_HIF_TOP_CTRL_EXT_IRQ_LEVEL_ext_irq_13_level_HIGH      1

/* HIF_TOP_CTRL :: EXT_IRQ_LEVEL :: ext_irq_12_level [12:12] */
#define BCHP_HIF_TOP_CTRL_EXT_IRQ_LEVEL_ext_irq_12_level_MASK      0x00001000
#define BCHP_HIF_TOP_CTRL_EXT_IRQ_LEVEL_ext_irq_12_level_SHIFT     12
#define BCHP_HIF_TOP_CTRL_EXT_IRQ_LEVEL_ext_irq_12_level_LOW       0
#define BCHP_HIF_TOP_CTRL_EXT_IRQ_LEVEL_ext_irq_12_level_HIGH      1

/* HIF_TOP_CTRL :: EXT_IRQ_LEVEL :: ext_irq_11_level [11:11] */
#define BCHP_HIF_TOP_CTRL_EXT_IRQ_LEVEL_ext_irq_11_level_MASK      0x00000800
#define BCHP_HIF_TOP_CTRL_EXT_IRQ_LEVEL_ext_irq_11_level_SHIFT     11
#define BCHP_HIF_TOP_CTRL_EXT_IRQ_LEVEL_ext_irq_11_level_LOW       0
#define BCHP_HIF_TOP_CTRL_EXT_IRQ_LEVEL_ext_irq_11_level_HIGH      1

/* HIF_TOP_CTRL :: EXT_IRQ_LEVEL :: ext_irq_10_level [10:10] */
#define BCHP_HIF_TOP_CTRL_EXT_IRQ_LEVEL_ext_irq_10_level_MASK      0x00000400
#define BCHP_HIF_TOP_CTRL_EXT_IRQ_LEVEL_ext_irq_10_level_SHIFT     10
#define BCHP_HIF_TOP_CTRL_EXT_IRQ_LEVEL_ext_irq_10_level_LOW       0
#define BCHP_HIF_TOP_CTRL_EXT_IRQ_LEVEL_ext_irq_10_level_HIGH      1

/* HIF_TOP_CTRL :: EXT_IRQ_LEVEL :: ext_irq_9_level [09:09] */
#define BCHP_HIF_TOP_CTRL_EXT_IRQ_LEVEL_ext_irq_9_level_MASK       0x00000200
#define BCHP_HIF_TOP_CTRL_EXT_IRQ_LEVEL_ext_irq_9_level_SHIFT      9
#define BCHP_HIF_TOP_CTRL_EXT_IRQ_LEVEL_ext_irq_9_level_LOW        0
#define BCHP_HIF_TOP_CTRL_EXT_IRQ_LEVEL_ext_irq_9_level_HIGH       1

/* HIF_TOP_CTRL :: EXT_IRQ_LEVEL :: ext_irq_8_level [08:08] */
#define BCHP_HIF_TOP_CTRL_EXT_IRQ_LEVEL_ext_irq_8_level_MASK       0x00000100
#define BCHP_HIF_TOP_CTRL_EXT_IRQ_LEVEL_ext_irq_8_level_SHIFT      8
#define BCHP_HIF_TOP_CTRL_EXT_IRQ_LEVEL_ext_irq_8_level_LOW        0
#define BCHP_HIF_TOP_CTRL_EXT_IRQ_LEVEL_ext_irq_8_level_HIGH       1

/* HIF_TOP_CTRL :: EXT_IRQ_LEVEL :: ext_irq_7_level [07:07] */
#define BCHP_HIF_TOP_CTRL_EXT_IRQ_LEVEL_ext_irq_7_level_MASK       0x00000080
#define BCHP_HIF_TOP_CTRL_EXT_IRQ_LEVEL_ext_irq_7_level_SHIFT      7
#define BCHP_HIF_TOP_CTRL_EXT_IRQ_LEVEL_ext_irq_7_level_LOW        0
#define BCHP_HIF_TOP_CTRL_EXT_IRQ_LEVEL_ext_irq_7_level_HIGH       1

/* HIF_TOP_CTRL :: EXT_IRQ_LEVEL :: ext_irq_6_level [06:06] */
#define BCHP_HIF_TOP_CTRL_EXT_IRQ_LEVEL_ext_irq_6_level_MASK       0x00000040
#define BCHP_HIF_TOP_CTRL_EXT_IRQ_LEVEL_ext_irq_6_level_SHIFT      6
#define BCHP_HIF_TOP_CTRL_EXT_IRQ_LEVEL_ext_irq_6_level_LOW        0
#define BCHP_HIF_TOP_CTRL_EXT_IRQ_LEVEL_ext_irq_6_level_HIGH       1

/* HIF_TOP_CTRL :: EXT_IRQ_LEVEL :: ext_irq_5_level [05:05] */
#define BCHP_HIF_TOP_CTRL_EXT_IRQ_LEVEL_ext_irq_5_level_MASK       0x00000020
#define BCHP_HIF_TOP_CTRL_EXT_IRQ_LEVEL_ext_irq_5_level_SHIFT      5
#define BCHP_HIF_TOP_CTRL_EXT_IRQ_LEVEL_ext_irq_5_level_LOW        0
#define BCHP_HIF_TOP_CTRL_EXT_IRQ_LEVEL_ext_irq_5_level_HIGH       1

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

/* HIF_TOP_CTRL :: SPI_DBG_SEL :: SPI_TP_SEL [00:00] */
#define BCHP_HIF_TOP_CTRL_SPI_DBG_SEL_SPI_TP_SEL_MASK              0x00000001
#define BCHP_HIF_TOP_CTRL_SPI_DBG_SEL_SPI_TP_SEL_SHIFT             0

/***************************************************************************
 *SCRATCH - HIF Scratch Register
 ***************************************************************************/
/* HIF_TOP_CTRL :: SCRATCH :: SCRATCH_BITS [31:00] */
#define BCHP_HIF_TOP_CTRL_SCRATCH_SCRATCH_BITS_MASK                0xffffffff
#define BCHP_HIF_TOP_CTRL_SCRATCH_SCRATCH_BITS_SHIFT               0

/***************************************************************************
 *PM_CTRL - HIF Power Management Control Register
 ***************************************************************************/
/* HIF_TOP_CTRL :: PM_CTRL :: reserved0 [31:14] */
#define BCHP_HIF_TOP_CTRL_PM_CTRL_reserved0_MASK                   0xffffc000
#define BCHP_HIF_TOP_CTRL_PM_CTRL_reserved0_SHIFT                  14

/* HIF_TOP_CTRL :: PM_CTRL :: EBI_PM_IN_DRIVE_INACTIVE [13:13] */
#define BCHP_HIF_TOP_CTRL_PM_CTRL_EBI_PM_IN_DRIVE_INACTIVE_MASK    0x00002000
#define BCHP_HIF_TOP_CTRL_PM_CTRL_EBI_PM_IN_DRIVE_INACTIVE_SHIFT   13

/* HIF_TOP_CTRL :: PM_CTRL :: PCI_PM_IN_DRIVE_INACTIVE [12:12] */
#define BCHP_HIF_TOP_CTRL_PM_CTRL_PCI_PM_IN_DRIVE_INACTIVE_MASK    0x00001000
#define BCHP_HIF_TOP_CTRL_PM_CTRL_PCI_PM_IN_DRIVE_INACTIVE_SHIFT   12

/* HIF_TOP_CTRL :: PM_CTRL :: MPI_PM_IN_DRIVE_INACTIVE [11:11] */
#define BCHP_HIF_TOP_CTRL_PM_CTRL_MPI_PM_IN_DRIVE_INACTIVE_MASK    0x00000800
#define BCHP_HIF_TOP_CTRL_PM_CTRL_MPI_PM_IN_DRIVE_INACTIVE_SHIFT   11

/* HIF_TOP_CTRL :: PM_CTRL :: PM_OUT_TRISTATE_CS6 [10:10] */
#define BCHP_HIF_TOP_CTRL_PM_CTRL_PM_OUT_TRISTATE_CS6_MASK         0x00000400
#define BCHP_HIF_TOP_CTRL_PM_CTRL_PM_OUT_TRISTATE_CS6_SHIFT        10

/* HIF_TOP_CTRL :: PM_CTRL :: PM_OUT_TRISTATE_CS5 [09:09] */
#define BCHP_HIF_TOP_CTRL_PM_CTRL_PM_OUT_TRISTATE_CS5_MASK         0x00000200
#define BCHP_HIF_TOP_CTRL_PM_CTRL_PM_OUT_TRISTATE_CS5_SHIFT        9

/* HIF_TOP_CTRL :: PM_CTRL :: PM_OUT_TRISTATE_CS4 [08:08] */
#define BCHP_HIF_TOP_CTRL_PM_CTRL_PM_OUT_TRISTATE_CS4_MASK         0x00000100
#define BCHP_HIF_TOP_CTRL_PM_CTRL_PM_OUT_TRISTATE_CS4_SHIFT        8

/* HIF_TOP_CTRL :: PM_CTRL :: PM_OUT_TRISTATE_CS3 [07:07] */
#define BCHP_HIF_TOP_CTRL_PM_CTRL_PM_OUT_TRISTATE_CS3_MASK         0x00000080
#define BCHP_HIF_TOP_CTRL_PM_CTRL_PM_OUT_TRISTATE_CS3_SHIFT        7

/* HIF_TOP_CTRL :: PM_CTRL :: PM_OUT_TRISTATE_CS2 [06:06] */
#define BCHP_HIF_TOP_CTRL_PM_CTRL_PM_OUT_TRISTATE_CS2_MASK         0x00000040
#define BCHP_HIF_TOP_CTRL_PM_CTRL_PM_OUT_TRISTATE_CS2_SHIFT        6

/* HIF_TOP_CTRL :: PM_CTRL :: PM_OUT_TRISTATE_CS1 [05:05] */
#define BCHP_HIF_TOP_CTRL_PM_CTRL_PM_OUT_TRISTATE_CS1_MASK         0x00000020
#define BCHP_HIF_TOP_CTRL_PM_CTRL_PM_OUT_TRISTATE_CS1_SHIFT        5

/* HIF_TOP_CTRL :: PM_CTRL :: PM_OUT_TRISTATE_CS0 [04:04] */
#define BCHP_HIF_TOP_CTRL_PM_CTRL_PM_OUT_TRISTATE_CS0_MASK         0x00000010
#define BCHP_HIF_TOP_CTRL_PM_CTRL_PM_OUT_TRISTATE_CS0_SHIFT        4

/* HIF_TOP_CTRL :: PM_CTRL :: PM_OUT_TRISTATE_PCIGNT [03:03] */
#define BCHP_HIF_TOP_CTRL_PM_CTRL_PM_OUT_TRISTATE_PCIGNT_MASK      0x00000008
#define BCHP_HIF_TOP_CTRL_PM_CTRL_PM_OUT_TRISTATE_PCIGNT_SHIFT     3

/* HIF_TOP_CTRL :: PM_CTRL :: EBI_PM_OUT_DRIVE_LOW [02:02] */
#define BCHP_HIF_TOP_CTRL_PM_CTRL_EBI_PM_OUT_DRIVE_LOW_MASK        0x00000004
#define BCHP_HIF_TOP_CTRL_PM_CTRL_EBI_PM_OUT_DRIVE_LOW_SHIFT       2

/* HIF_TOP_CTRL :: PM_CTRL :: PCI_PM_OUT_DRIVE_LOW [01:01] */
#define BCHP_HIF_TOP_CTRL_PM_CTRL_PCI_PM_OUT_DRIVE_LOW_MASK        0x00000002
#define BCHP_HIF_TOP_CTRL_PM_CTRL_PCI_PM_OUT_DRIVE_LOW_SHIFT       1

/* HIF_TOP_CTRL :: PM_CTRL :: MPI_PM_OUT_DRIVE_LOW [00:00] */
#define BCHP_HIF_TOP_CTRL_PM_CTRL_MPI_PM_OUT_DRIVE_LOW_MASK        0x00000001
#define BCHP_HIF_TOP_CTRL_PM_CTRL_MPI_PM_OUT_DRIVE_LOW_SHIFT       0

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

/* HIF_TOP_CTRL :: STRAP_INTERCEPT :: STRAP_PCI_EXT_ARB [00:00] */
#define BCHP_HIF_TOP_CTRL_STRAP_INTERCEPT_STRAP_PCI_EXT_ARB_MASK   0x00000001
#define BCHP_HIF_TOP_CTRL_STRAP_INTERCEPT_STRAP_PCI_EXT_ARB_SHIFT  0

/***************************************************************************
 *SPI_CLK_SEL - HIF SPI Interface Clock Select Register
 ***************************************************************************/
/* HIF_TOP_CTRL :: SPI_CLK_SEL :: reserved0 [31:02] */
#define BCHP_HIF_TOP_CTRL_SPI_CLK_SEL_reserved0_MASK               0xfffffffc
#define BCHP_HIF_TOP_CTRL_SPI_CLK_SEL_reserved0_SHIFT              2

/* HIF_TOP_CTRL :: SPI_CLK_SEL :: BSPI_CLK_FREQ_SEL [01:00] */
#define BCHP_HIF_TOP_CTRL_SPI_CLK_SEL_BSPI_CLK_FREQ_SEL_MASK       0x00000003
#define BCHP_HIF_TOP_CTRL_SPI_CLK_SEL_BSPI_CLK_FREQ_SEL_SHIFT      0

/***************************************************************************
 *FLASH_TYPE - HIF Decoded Flash Type
 ***************************************************************************/
/* HIF_TOP_CTRL :: FLASH_TYPE :: reserved0 [31:04] */
#define BCHP_HIF_TOP_CTRL_FLASH_TYPE_reserved0_MASK                0xfffffff0
#define BCHP_HIF_TOP_CTRL_FLASH_TYPE_reserved0_SHIFT               4

/* HIF_TOP_CTRL :: FLASH_TYPE :: InvalidStrap [03:03] */
#define BCHP_HIF_TOP_CTRL_FLASH_TYPE_InvalidStrap_MASK             0x00000008
#define BCHP_HIF_TOP_CTRL_FLASH_TYPE_InvalidStrap_SHIFT            3

/* HIF_TOP_CTRL :: FLASH_TYPE :: BUS_MODE_2 [02:02] */
#define BCHP_HIF_TOP_CTRL_FLASH_TYPE_BUS_MODE_2_MASK               0x00000004
#define BCHP_HIF_TOP_CTRL_FLASH_TYPE_BUS_MODE_2_SHIFT              2

/* HIF_TOP_CTRL :: FLASH_TYPE :: FLASH_TYPE [01:00] */
#define BCHP_HIF_TOP_CTRL_FLASH_TYPE_FLASH_TYPE_MASK               0x00000003
#define BCHP_HIF_TOP_CTRL_FLASH_TYPE_FLASH_TYPE_SHIFT              0

/***************************************************************************
 *MWIN_CTRL - HIF PCI ConfigurationManager Mode MWIN Control Register
 ***************************************************************************/
/* HIF_TOP_CTRL :: MWIN_CTRL :: reserved0 [31:04] */
#define BCHP_HIF_TOP_CTRL_MWIN_CTRL_reserved0_MASK                 0xfffffff0
#define BCHP_HIF_TOP_CTRL_MWIN_CTRL_reserved0_SHIFT                4

/* HIF_TOP_CTRL :: MWIN_CTRL :: MWIN2_EN [03:03] */
#define BCHP_HIF_TOP_CTRL_MWIN_CTRL_MWIN2_EN_MASK                  0x00000008
#define BCHP_HIF_TOP_CTRL_MWIN_CTRL_MWIN2_EN_SHIFT                 3

/* HIF_TOP_CTRL :: MWIN_CTRL :: MWIN1_EN [02:02] */
#define BCHP_HIF_TOP_CTRL_MWIN_CTRL_MWIN1_EN_MASK                  0x00000004
#define BCHP_HIF_TOP_CTRL_MWIN_CTRL_MWIN1_EN_SHIFT                 2

/* HIF_TOP_CTRL :: MWIN_CTRL :: MWIN_SIZE [01:00] */
#define BCHP_HIF_TOP_CTRL_MWIN_CTRL_MWIN_SIZE_MASK                 0x00000003
#define BCHP_HIF_TOP_CTRL_MWIN_CTRL_MWIN_SIZE_SHIFT                0

#endif /* #ifndef BCHP_HIF_TOP_CTRL_H__ */

/* End of File */
