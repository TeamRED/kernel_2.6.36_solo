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
 * Date:           Generated on         Tue Nov 17 18:07:02 2009
 *                 MD5 Checksum         7635c8e6fce632fc8dd4fb82126b3847
 *
 * Compiled with:  RDB Utility          combo_header.pl
 *                 RDB Parser           3.0
 *                 unknown              unknown
 *                 Perl Interpreter     5.008008
 *                 Operating System     linux
 *
 * Revision History:
 *
 * $brcm_Log: /magnum/basemodules/chp/7342/rdb/b0/bchp_uartb.h $
 * 
 * Hydra_Software_Devel/1   11/17/09 9:03p albertl
 * SW7342-78: Initial revision.
 *
 ***************************************************************************/

#ifndef BCHP_UARTB_H__
#define BCHP_UARTB_H__

/***************************************************************************
 *UARTB - UART B
 ***************************************************************************/
#define BCHP_UARTB_RBR                           0x00406840 /* Receive Buffer Register */
#define BCHP_UARTB_THR                           0x00406840 /* Transmit Holding Register */
#define BCHP_UARTB_DLH                           0x00406844 /* Divisor Latch High */
#define BCHP_UARTB_DLL                           0x00406840 /* Divisor Latch Low */
#define BCHP_UARTB_IER                           0x00406844 /* Interrupt Enable Register */
#define BCHP_UARTB_IIR                           0x00406848 /* Interrupt Identity Register */
#define BCHP_UARTB_FCR                           0x00406848 /* FIFO Control Register */
#define BCHP_UARTB_LCR                           0x0040684c /* Line Control Register */
#define BCHP_UARTB_MCR                           0x00406850 /* Modem Control Register */
#define BCHP_UARTB_LSR                           0x00406854 /* Line Status Register */
#define BCHP_UARTB_MSR                           0x00406858 /* Modem Status Register */
#define BCHP_UARTB_SCR                           0x0040685c /* Scratchpad Register */

#endif /* #ifndef BCHP_UARTB_H__ */

/* End of File */
