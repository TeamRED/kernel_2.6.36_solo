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

#ifndef BCHP_EDU_H__
#define BCHP_EDU_H__

/***************************************************************************
 *EDU
 ***************************************************************************/
#define BCHP_EDU_CONFIG                          0x00442c00
#define BCHP_EDU_DRAM_ADDR                       0x00442c04
#define BCHP_EDU_EXT_ADDR                        0x00442c08
#define BCHP_EDU_LENGTH                          0x00442c0c
#define BCHP_EDU_CMD                             0x00442c10
#define BCHP_EDU_STOP                            0x00442c14
#define BCHP_EDU_STATUS                          0x00442c18
#define BCHP_EDU_DONE                            0x00442c1c
#define BCHP_EDU_ERR_STATUS                      0x00442c20

/***************************************************************************
 *CONFIG
 ***************************************************************************/
/* EDU :: CONFIG :: reserved0 [31:03] */
#define BCHP_EDU_CONFIG_reserved0_MASK                             0xfffffff8
#define BCHP_EDU_CONFIG_reserved0_SHIFT                            3

/* EDU :: CONFIG :: Swap [02:01] */
#define BCHP_EDU_CONFIG_Swap_MASK                                  0x00000006
#define BCHP_EDU_CONFIG_Swap_SHIFT                                 1

/* EDU :: CONFIG :: Mode [00:00] */
#define BCHP_EDU_CONFIG_Mode_MASK                                  0x00000001
#define BCHP_EDU_CONFIG_Mode_SHIFT                                 0

/***************************************************************************
 *DRAM_ADDR
 ***************************************************************************/
/* EDU :: DRAM_ADDR :: Address [31:02] */
#define BCHP_EDU_DRAM_ADDR_Address_MASK                            0xfffffffc
#define BCHP_EDU_DRAM_ADDR_Address_SHIFT                           2

/* EDU :: DRAM_ADDR :: reserved0 [01:00] */
#define BCHP_EDU_DRAM_ADDR_reserved0_MASK                          0x00000003
#define BCHP_EDU_DRAM_ADDR_reserved0_SHIFT                         0

/***************************************************************************
 *EXT_ADDR
 ***************************************************************************/
/* EDU :: EXT_ADDR :: Address [31:02] */
#define BCHP_EDU_EXT_ADDR_Address_MASK                             0xfffffffc
#define BCHP_EDU_EXT_ADDR_Address_SHIFT                            2

/* EDU :: EXT_ADDR :: reserved0 [01:00] */
#define BCHP_EDU_EXT_ADDR_reserved0_MASK                           0x00000003
#define BCHP_EDU_EXT_ADDR_reserved0_SHIFT                          0

/***************************************************************************
 *LENGTH
 ***************************************************************************/
/* EDU :: LENGTH :: reserved0 [31:10] */
#define BCHP_EDU_LENGTH_reserved0_MASK                             0xfffffc00
#define BCHP_EDU_LENGTH_reserved0_SHIFT                            10

/* EDU :: LENGTH :: Length [09:00] */
#define BCHP_EDU_LENGTH_Length_MASK                                0x000003ff
#define BCHP_EDU_LENGTH_Length_SHIFT                               0

/***************************************************************************
 *CMD
 ***************************************************************************/
/* EDU :: CMD :: reserved0 [31:01] */
#define BCHP_EDU_CMD_reserved0_MASK                                0xfffffffe
#define BCHP_EDU_CMD_reserved0_SHIFT                               1

/* EDU :: CMD :: Cmd [00:00] */
#define BCHP_EDU_CMD_Cmd_MASK                                      0x00000001
#define BCHP_EDU_CMD_Cmd_SHIFT                                     0

/***************************************************************************
 *STOP
 ***************************************************************************/
/* EDU :: STOP :: reserved0 [31:01] */
#define BCHP_EDU_STOP_reserved0_MASK                               0xfffffffe
#define BCHP_EDU_STOP_reserved0_SHIFT                              1

/* EDU :: STOP :: Stop [00:00] */
#define BCHP_EDU_STOP_Stop_MASK                                    0x00000001
#define BCHP_EDU_STOP_Stop_SHIFT                                   0

/***************************************************************************
 *STATUS
 ***************************************************************************/
/* EDU :: STATUS :: reserved0 [31:02] */
#define BCHP_EDU_STATUS_reserved0_MASK                             0xfffffffc
#define BCHP_EDU_STATUS_reserved0_SHIFT                            2

/* EDU :: STATUS :: Pending [01:01] */
#define BCHP_EDU_STATUS_Pending_MASK                               0x00000002
#define BCHP_EDU_STATUS_Pending_SHIFT                              1

/* EDU :: STATUS :: Active [00:00] */
#define BCHP_EDU_STATUS_Active_MASK                                0x00000001
#define BCHP_EDU_STATUS_Active_SHIFT                               0

/***************************************************************************
 *DONE
 ***************************************************************************/
/* EDU :: DONE :: reserved0 [31:02] */
#define BCHP_EDU_DONE_reserved0_MASK                               0xfffffffc
#define BCHP_EDU_DONE_reserved0_SHIFT                              2

/* EDU :: DONE :: Done [01:00] */
#define BCHP_EDU_DONE_Done_MASK                                    0x00000003
#define BCHP_EDU_DONE_Done_SHIFT                                   0

/***************************************************************************
 *ERR_STATUS
 ***************************************************************************/
/* EDU :: ERR_STATUS :: reserved0 [31:04] */
#define BCHP_EDU_ERR_STATUS_reserved0_MASK                         0xfffffff0
#define BCHP_EDU_ERR_STATUS_reserved0_SHIFT                        4

/* EDU :: ERR_STATUS :: NandWrErr [03:03] */
#define BCHP_EDU_ERR_STATUS_NandWrErr_MASK                         0x00000008
#define BCHP_EDU_ERR_STATUS_NandWrErr_SHIFT                        3

/* EDU :: ERR_STATUS :: NandECCuncor [02:02] */
#define BCHP_EDU_ERR_STATUS_NandECCuncor_MASK                      0x00000004
#define BCHP_EDU_ERR_STATUS_NandECCuncor_SHIFT                     2

/* EDU :: ERR_STATUS :: NandECCcor [01:01] */
#define BCHP_EDU_ERR_STATUS_NandECCcor_MASK                        0x00000002
#define BCHP_EDU_ERR_STATUS_NandECCcor_SHIFT                       1

/* EDU :: ERR_STATUS :: ErrAck [00:00] */
#define BCHP_EDU_ERR_STATUS_ErrAck_MASK                            0x00000001
#define BCHP_EDU_ERR_STATUS_ErrAck_SHIFT                           0

#endif /* #ifndef BCHP_EDU_H__ */

/* End of File */
