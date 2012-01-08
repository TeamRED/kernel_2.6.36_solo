/*
 *
 * Copyright (c) 2002-2008 Broadcom Corporation 
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
 *
*/
//**************************************************************************
// File Name  : bcmunimac.c 
//
// Description: This is Linux network driver for the BCMUNIMAC
// 				internal Ethenet Controller onboard the 7420
//               
// Updates    : 
// 09/24/2008  lei sun. Created from bcmemac.c 
// 05/04/2009  lei sun, Revision for 7420B0
// 08/21/2009  lei sun, bug fixes and added ring buffer and legacy netaccel hooks.
//**************************************************************************

#define CARDNAME    "bcmgenet"
#define VERSION     "2.0"
#define VER_STR     "v" VERSION " " __DATE__ " " __TIME__

#if defined(CONFIG_MODVERSIONS) && ! defined(MODVERSIONS)
   #include <config/modversions.h> 
   #define MODVERSIONS
#endif  

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/types.h>
#include <linux/fcntl.h>
#include <linux/interrupt.h>
#include <linux/string.h>
#include <linux/init.h>
#include <linux/errno.h>
#include <linux/delay.h>
#include <linux/platform_device.h>
#include <linux/dma-mapping.h>

#include <linux/mii.h>
#include <linux/ethtool.h>
#include <linux/netdevice.h>
#include <linux/inetdevice.h>
#include <linux/etherdevice.h>
#include <linux/skbuff.h>
#include <linux/in.h>

#include <asm/mipsregs.h>
#include <asm/cacheflush.h>
#include <asm/brcmstb/brcmstb.h>
#include "bcmmii.h"
#include "bcmgenet.h"
#include "if_net.h"

//#define B_HAS_GENET_TX_MULTI_QUEUES 1
#define B_HAS_GENET_TX_SG			1

#define RX_BUF_LENGTH		2048	/* 2Kb buffer */
#define RX_BUF_BITS			12
#define SKB_ALIGNMENT		32		/* 256B alignment */
#define DMA_DESC_THRES		4		/* Rx Descriptor throttle threshold */
#define HFB_TCP_LEN 		19		/* filter content length for TCP packet */
#define HFB_ARP_LEN			21		/* filter content length for ARP packet */
#define NAPI_BUDGET			64		/* NAPI polling budget */

// --------------------------------------------------------------------------
//      External, indirect entry points. 
// --------------------------------------------------------------------------
static int bcmgenet_ioctl(struct net_device *dev, struct ifreq *rq, int cmd);
// --------------------------------------------------------------------------
//      Called for "ifconfig ethX up" & "down"
// --------------------------------------------------------------------------
static int bcmgenet_open(struct net_device * dev);
static int bcmgenet_close(struct net_device * dev);
// --------------------------------------------------------------------------
//      Watchdog timeout
// --------------------------------------------------------------------------
static void bcmgenet_timeout(struct net_device * dev);
// --------------------------------------------------------------------------
//      Packet transmission. 
// --------------------------------------------------------------------------
static int bcmgenet_xmit(struct sk_buff * skb, struct net_device * dev);
// --------------------------------------------------------------------------
//      Set address filtering mode
// --------------------------------------------------------------------------
static void bcmgenet_set_multicast_list(struct net_device * dev);
// --------------------------------------------------------------------------
//      Set the hardware MAC address.
// --------------------------------------------------------------------------
static int bcmgenet_set_mac_addr(struct net_device *dev, void *p);

// --------------------------------------------------------------------------
//      Interrupt routine, for all interrupts except ring buffer interrupts
// --------------------------------------------------------------------------
static irqreturn_t bcmgenet_isr0(int irq, void * dev_id);
//---------------------------------------------------------------------------
//      IRQ handler for ring buffer interrupt.
//---------------------------------------------------------------------------
static irqreturn_t bcmgenet_isr1(int irq, void * dev_id);
// --------------------------------------------------------------------------
//      dev->poll() method
// --------------------------------------------------------------------------
static int bcmgenet_poll(struct napi_struct * napi, int budget);
static int bcmgenet_ring_poll(struct napi_struct * napi, int budget);
// --------------------------------------------------------------------------
//      Process recived packet for descriptor based DMA 
// --------------------------------------------------------------------------
static unsigned int bcmgenet_desc_rx(void *ptr, unsigned int budget);
// --------------------------------------------------------------------------
//      Process recived packet for ring buffer DMA
// --------------------------------------------------------------------------
static unsigned int bcmgenet_ring_rx(void * ptr, unsigned int budget);
// --------------------------------------------------------------------------
//      Update hardware filter block.
// --------------------------------------------------------------------------
int bcmgenet_update_hfb(struct net_device *dev, unsigned int *data, int len, int user);
// --------------------------------------------------------------------------
//      Initialize ring buffer for Tx/Rx.
// --------------------------------------------------------------------------
int bcmgenet_init_ringbuf(struct net_device * dev, int direction, unsigned int index, 
		unsigned int size, int buf_len, unsigned char ** buf);
// --------------------------------------------------------------------------
//      Internal routines
// --------------------------------------------------------------------------
/* Allocate and initialize tx/rx buffer descriptor pools */
static int bcmgenet_init_dev(BcmEnet_devctrl *pDevCtrl);
static void bcmgenet_uninit_dev(BcmEnet_devctrl *pDevCtrl);
/* Assign the Rx descriptor ring */
static int assign_rx_buffers(BcmEnet_devctrl *pDevCtrl);
/* Initialize the uniMac control registers */
static int init_umac(BcmEnet_devctrl *pDevCtrl);
/* Initialize DMA control register */
static void init_edma(BcmEnet_devctrl *pDevCtrl);
/* Interrupt bottom-half */
static void bcmgenet_irq_task(struct work_struct * work);
/* power management */
static void bcmgenet_power_down(BcmEnet_devctrl *pDevCtrl, int mode);
static void bcmgenet_power_up(BcmEnet_devctrl *pDevCtrl, int mode);
/* Display hex base data */
static void __maybe_unused dumpHexData(unsigned char *head, int len);
/* dumpMem32 dump out the number of 32 bit hex data  */
static void __maybe_unused dumpMem32(unsigned int * pMemAddr, int iNumWords);
/* experimental ring buffer functions */
static struct sk_buff * __bcmgenet_alloc_skb_from_buf(unsigned char * buf, int len, int headroom, int users);
static int __bcmgenet_free_skb(struct sk_buff * skb, int free );

/* misc variables */
static struct net_device *eth_root_dev = NULL;
static int DmaDescThres = DMA_DESC_THRES;
static struct kmem_cache * genet_skb_cache __read_mostly;
/* 
 * HFB data for ARP request.
 * In WoL (Magic Packet or ACPI) mode, we need to response
 * ARP request, so dedicate an HFB to filter the ARP request.
 * NOTE: the last two words are to be filled by destination.
 */
static unsigned int hfb_arp[] =
{
	0x000FFFFF, 0x000FFFFF, 0x000FFFFF,	0x00000000,	0x00000000,
	0x00000000, 0x000F0806,	0x000F0001,	0x000F0800,	0x000F0604,
	0x000F0001,	0x00000000,	0x00000000,	0x00000000,	0x00000000,
	0x00000000,	0x000F0000,	0x000F0000,	0x000F0000,	0x000F0000,
	0x000F0000
};
/* -------------------------------------------------------------------------
 *  The following bcmemac_xxxx() functions are legacy netaccel hook, will be 
 *  replaced!
 *  -----------------------------------------------------------------------*/
struct net_device * bcmemac_get_device(void) {
	return eth_root_dev;
}
/* --------------------------------------------------------------------------
    Name: bcmemac_get_free_txdesc
 Purpose: Get Current Available TX desc count
-------------------------------------------------------------------------- */
int bcmemac_get_free_txdesc( struct net_device *dev ){
    BcmEnet_devctrl *pDevCtrl = netdev_priv(dev);
    return pDevCtrl->txFreeBds;
}
/* --------------------------------------------------------------------------
    Name: bcmemac_xmit_check
 Purpose: Reclaims TX descriptors
-------------------------------------------------------------------------- */
int bcmemac_xmit_check(struct net_device * dev)
{
    BcmEnet_devctrl *pDevCtrl = netdev_priv(dev);
    Enet_CB *txCBPtr;
    unsigned long flags,ret;
	unsigned int read_ptr = 0;
	int lastTxedCnt = 0, i = 0;
    /*
     * Obtain exclusive access to transmitter.  This is necessary because
     * we might have more than one stack transmitting at once.
     */
    spin_lock_irqsave(&pDevCtrl->lock, flags);
	/* Compute how many buffers are transmited since last xmit call */
	read_ptr =  (DMA_RW_POINTER_MASK & pDevCtrl->txDma->tDmaRings[DMA_RING_DESC_INDEX].tdma_read_pointer) >> 1;

	if(read_ptr >= pDevCtrl->txLastCIndex) {
		lastTxedCnt = read_ptr - pDevCtrl->txLastCIndex;
	}else {
		lastTxedCnt = pDevCtrl->nrTxBds - pDevCtrl->txLastCIndex + read_ptr;
	}
	TRACE(("read_ptr=%d lastTxedCnt=%d txLastCIndex=%d\n", 
				read_ptr, lastTxedCnt, pDevCtrl->txLastCIndex));
	
	/* Recalaim transmitted buffers */
	i = pDevCtrl->txLastCIndex;
	while(lastTxedCnt-- > 0)
	{
		txCBPtr = &pDevCtrl->txCbs[i];
		if(txCBPtr->skb != NULL) {
			dma_unmap_single(&pDevCtrl->dev->dev, 
					txCBPtr->dma_addr, 
					txCBPtr->skb->len, 
					DMA_TO_DEVICE);
            dev_kfree_skb_any (txCBPtr->skb);
			txCBPtr->skb = NULL;
		}
		pDevCtrl->txFreeBds += 1;
		if( i == (TOTAL_DESC - 1 ))
			i = 0;
		else
			i++;
	}
	pDevCtrl->txLastCIndex = read_ptr;
    if (pDevCtrl->txFreeBds > 0) {
		/* Disable txdma bdone/pdone interrupt if we have free tx bds */
		pDevCtrl->intrl2_0->cpu_mask_set |= UMAC_IRQ_TXDMA_BDONE | UMAC_IRQ_TXDMA_PDONE;
		netif_wake_queue(dev);
		ret = 0;
    }else {
		ret = 1;
	}
	spin_unlock_irqrestore(&pDevCtrl->lock, flags);

	return ret;
}
/* --------------------------------------------------------------------------
    Name: bcmemac_xmit_fragment
 Purpose: Send ethernet traffic Buffer DESC and submit to UDMA
-------------------------------------------------------------------------- */
int bcmemac_xmit_fragment( int ch, unsigned char *buf, int buf_len, 
                           unsigned long tx_flags , struct net_device *dev)
{
	BcmEnet_devctrl *pDevCtrl = netdev_priv(dev);
    Enet_CB *txCBPtr;
	unsigned int write_ptr = 0;

	if(pDevCtrl->txFreeBds == 0)
		return 1;
	/*
	 * We must don't have 64B status block enabled in this case!
	 */
	write_ptr = (DMA_RW_POINTER_MASK & pDevCtrl->txDma->tDmaRings[DMA_RING_DESC_INDEX].tdma_write_pointer) >> 1;
	
	/* Obtain transmit control block */
	txCBPtr = &pDevCtrl->txCbs[write_ptr];
	txCBPtr->BdAddr = &pDevCtrl->txBds[write_ptr];
	txCBPtr->dma_addr = dma_map_single(&pDevCtrl->dev->dev, buf, 
			buf_len, DMA_TO_DEVICE);

    /*
     * Add the buffer to the ring.
     * Set addr and length of DMA BD to be transmitted.
     */
    txCBPtr->BdAddr->address = txCBPtr->dma_addr;
	txCBPtr->BdAddr->length_status = ((unsigned long)(buf_len))<<16;
	txCBPtr->BdAddr->length_status |= tx_flags | DMA_TX_APPEND_CRC;

#ifdef CONFIG_BCMGENET_DUMP_DATA
    printk("%s: len %d", __FUNCTION__, buf_len);
    dumpHexData(buf, buf_len);
#endif

    /* Decrement total BD count and advance our write pointer */
    pDevCtrl->txFreeBds -= 1;

	if(write_ptr == pDevCtrl->nrTxBds - 1) {
		write_ptr = 0;
	}else {
		write_ptr++;
	}
	/* advance producer index and write pointer.*/
	pDevCtrl->txDma->tDmaRings[DMA_RING_DESC_INDEX].tdma_producer_index += 1;
	pDevCtrl->txDma->tDmaRings[DMA_RING_DESC_INDEX].tdma_write_pointer += 2;

    if ( pDevCtrl->txFreeBds == 0 ) {
        TRACE(("%s: %s no transmit queue space -- stopping queues\n", dev->name, __FUNCTION__));
		/* Enable Tx bdone/pdone interrupt !*/
		pDevCtrl->intrl2_0->cpu_mask_clear |= UMAC_IRQ_TXDMA_BDONE | UMAC_IRQ_TXDMA_PDONE;
        netif_stop_queue(dev);
    }
    /* update stats */
    dev->stats.tx_bytes += buf_len;
    dev->stats.tx_packets++;

    dev->trans_start = jiffies;

    return 0;
}
/* --------------------------------------------------------------------------
    Name: bcmemac_xmit_multibuf
 Purpose: Send ethernet traffic in multi buffers (hdr, buf, tail)
-------------------------------------------------------------------------- */
int bcmemac_xmit_multibuf( int ch, unsigned char *hdr, int hdr_len, unsigned char *buf, int buf_len, unsigned char *tail, int tail_len, struct net_device *dev)
{
	unsigned long flags;
	BcmEnet_devctrl *pDevCtrl = netdev_priv(dev);
    
	while(bcmemac_xmit_check(dev));

    /*
     * Obtain exclusive access to transmitter.  This is necessary because
     * we might have more than one stack transmitting at once.
     */
    spin_lock_irqsave(&pDevCtrl->lock, flags);

    /* Header + Optional payload in two parts */
    if((hdr_len> 0) && (buf_len > 0) && (tail_len > 0) && (hdr) && (buf) && (tail)){ 
        /* Send Header */
        while(bcmemac_xmit_fragment( ch, hdr, hdr_len, DMA_SOP, dev))
            bcmemac_xmit_check(dev);
        /* Send First Fragment */  
        while(bcmemac_xmit_fragment( ch, buf, buf_len, 0, dev))
            bcmemac_xmit_check(dev);
        /* Send 2nd Fragment */ 	
        while(bcmemac_xmit_fragment( ch, tail, tail_len, DMA_EOP, dev))
            bcmemac_xmit_check(dev);
    }
    /* Header + Optional payload */
    else if((hdr_len> 0) && (buf_len > 0) && (hdr) && (buf)){
        /* Send Header */
        while(bcmemac_xmit_fragment( ch, hdr, hdr_len, DMA_SOP, dev))
            bcmemac_xmit_check(dev);
        /* Send First Fragment */
        while(bcmemac_xmit_fragment( ch, buf, buf_len, DMA_EOP, dev))
            bcmemac_xmit_check(dev);
    }
    /* Header Only (includes payload) */
    else if((hdr_len> 0) && (hdr)){ 
        /* Send Header */
        while(bcmemac_xmit_fragment( ch, hdr, hdr_len, DMA_SOP | DMA_EOP, dev))
            bcmemac_xmit_check(dev);
    }
    else{
    	spin_unlock_irqrestore(&pDevCtrl->lock, flags);
        return 0; /* Drop the packet */
    }
    spin_unlock_irqrestore(&pDevCtrl->lock, flags);
    return 0;
}
/*
 * dumpHexData dump out the hex base binary data
 */
static void __maybe_unused dumpHexData(unsigned char *head, int len)
{
    int i;
    unsigned char *curPtr = head;

    for (i = 0; i < len; ++i)
    {
        if (i % 16 == 0)
            printk("\n");       
        printk("0x%02X, ", *curPtr++);
    }
    printk("\n");

}

/*
 * dumpMem32 dump out the number of 32 bit hex data 
 */
static void __maybe_unused dumpMem32(unsigned int * pMemAddr, int iNumWords)
{
    int i = 0;
    static char buffer[80];

    sprintf(buffer, "%08X: ", (unsigned int)pMemAddr);
    printk(buffer);
    while (iNumWords) {
        sprintf(buffer, "%08X ", (unsigned int)*pMemAddr++);
        printk(buffer);
        iNumWords--;
        i++;
        if ((i % 4) == 0 && iNumWords) {
            sprintf(buffer, "\n%08X: ", (unsigned int)pMemAddr);
            printk(buffer);
        }
    }
    printk("\n");
}
static inline void handleAlignment(BcmEnet_devctrl *pDevCtrl, struct sk_buff* skb)
{
    /* 
	 * We request to allocate 2048 + 32 bytes of buffers, and the dev_alloc_skb() added
	 * 16B for NET_SKB_PAD, so we totally requested 2048+32+16 bytes buffer,
	 * the size was aligned to SMP_CACHE_BYTES, which is 64B.(is it?), so we 
	 * finnally ended up got 2112 bytes of buffer! Among which, the first 16B is reserved
	 * for NET_SKB_PAD, to make the skb->data aligned 32B  boundary, we should have 
	 * enough space to fullfill the 2KB buffer after alignment!
     */

    volatile unsigned long boundary32, curData, resHeader;

    curData = (unsigned long) skb->data;
    boundary32 = (curData + (SKB_ALIGNMENT - 1)) & ~(SKB_ALIGNMENT - 1);
    resHeader = boundary32 - curData ;
	/* 4 bytes for skb pointer */
	if(resHeader < 4) {
		boundary32 += 32;
	}

	resHeader = boundary32 - curData - 4;
	/* We'd have minimum 16B reserved by default. */
	skb_reserve(skb, resHeader);

	*(unsigned int *)skb->data = (unsigned int)skb;
	skb_reserve(skb, 4);
    /*
	 * Make sure it is on 32B boundary, should never happen if our
	 * calculation was correct.
	 */
    if ((unsigned long) skb->data & (SKB_ALIGNMENT - 1)) {
        printk("$$$$$$$$$$$$$$$$$$$ skb buffer is NOT aligned on %d boundary\n", SKB_ALIGNMENT);
    }

	/* 
	 *  we don't reserve 2B for IP Header optimization here,
	 *  use skb_pull when receiving packets
	 */
}
/* --------------------------------------------------------------------------
    Name: bcmgenet_gphy_link_status
 Purpose: GPHY link status monitoring task
-------------------------------------------------------------------------- */
static void bcmgenet_gphy_link_status(struct work_struct *work)
{
	BcmEnet_devctrl *pDevCtrl = container_of(work, BcmEnet_devctrl, bcmgenet_link_work);
	int link;

	link = mii_link_ok(&pDevCtrl->mii);
	if(link && !netif_carrier_ok(pDevCtrl->dev))
	{
		mii_setup(pDevCtrl->dev);
		pDevCtrl->dev->flags |= IFF_RUNNING;
		netif_carrier_on(pDevCtrl->dev);
		rtmsg_ifinfo(RTM_NEWLINK, pDevCtrl->dev, IFF_RUNNING);
	}else if(!link && netif_carrier_ok(pDevCtrl->dev))
	{
		printk(KERN_INFO "%s: Link is down\n", pDevCtrl->dev->name);
		netif_carrier_off(pDevCtrl->dev);
		pDevCtrl->dev->flags &= ~IFF_RUNNING;
		rtmsg_ifinfo(RTM_DELLINK, pDevCtrl->dev, IFF_RUNNING);
	}
}
/* --------------------------------------------------------------------------
    Name: bcmgenet_gphy_link_timer
 Purpose: GPHY link status monitoring timer function
-------------------------------------------------------------------------- */
static void bcmgenet_gphy_link_timer(unsigned long data)
{
	BcmEnet_devctrl * pDevCtrl = (BcmEnet_devctrl *)data;
	schedule_work(&pDevCtrl->bcmgenet_link_work);
	mod_timer(&pDevCtrl->timer, jiffies + HZ);
}
/* --------------------------------------------------------------------------
    Name: bcmgenet_open
 Purpose: Open and Initialize the EMAC on the chip
-------------------------------------------------------------------------- */
static int bcmgenet_open(struct net_device * dev)
{
    BcmEnet_devctrl *pDevCtrl = netdev_priv(dev);

    /* disable ethernet MAC while updating its registers */
    pDevCtrl->umac->cmd &= ~(CMD_TX_EN | CMD_RX_EN);

	pDevCtrl->txDma->tdma_ctrl &= ~(1 << (DMA_RING_DESC_INDEX + DMA_RING_BUF_EN_SHIFT) | DMA_EN);
	pDevCtrl->rxDma->rdma_ctrl &= ~(1 << (DMA_RING_DESC_INDEX + DMA_RING_BUF_EN_SHIFT) | DMA_EN);
	pDevCtrl->umac->tx_flush = 1;
	//pDevCtrl->rbuf->rbuf_flush_ctrl = 1;
	GENET_RBUF_FLUSH_CTRL(pDevCtrl) = 1;
	udelay(10);
	pDevCtrl->umac->tx_flush = 0;
	//pDevCtrl->rbuf->rbuf_flush_ctrl = 0;
	GENET_RBUF_FLUSH_CTRL(pDevCtrl) = 0;

	/* reset dma, start from begainning of the ring. */
    init_edma(pDevCtrl);
	/* reset internal book keeping variables. */
	pDevCtrl->txLastCIndex = 0;
	pDevCtrl->rxBdAssignPtr = pDevCtrl->rxBds;
	assign_rx_buffers(pDevCtrl);
    pDevCtrl->txFreeBds = pDevCtrl->nrTxBds;

	/*Always enable ring 16 - descriptor ring */
	pDevCtrl->rxDma->rdma_ctrl |= (1 << (DMA_RING_DESC_INDEX + DMA_RING_BUF_EN_SHIFT) | DMA_EN);
	pDevCtrl->txDma->tdma_ctrl |= (1 << (DMA_RING_DESC_INDEX + DMA_RING_BUF_EN_SHIFT) | DMA_EN);

	if(pDevCtrl->phyType == BRCM_PHY_TYPE_EXT_MII ||
		pDevCtrl->phyType == BRCM_PHY_TYPE_EXT_GMII ||
		pDevCtrl->phyType == BRCM_PHY_TYPE_EXT_GMII_IBS)
	{
		mod_timer(&pDevCtrl->timer, jiffies);
	}
    // Start the network engine
    netif_start_queue(dev);
	napi_enable(&pDevCtrl->napi);
    
	pDevCtrl->umac->cmd |= (CMD_TX_EN | CMD_RX_EN);

    return 0;
}


/* --------------------------------------------------------------------------
    Name: bcmgenet_close
 	Purpose: Stop communicating with the outside world
    Note: Caused by 'ifconfig ethX down'
-------------------------------------------------------------------------- */
static int bcmgenet_close(struct net_device * dev)
{
    BcmEnet_devctrl *pDevCtrl = netdev_priv(dev);
	int timeout = 0;

    TRACE(("%s: bcgenet_close\n", dev->name));

	napi_disable(&pDevCtrl->napi);
    netif_stop_queue(dev);
	/* Stop Tx DMA */
	pDevCtrl->txDma->tdma_ctrl &= ~DMA_EN;
	while(timeout < 5000)
	{
		if(pDevCtrl->txDma->tdma_status & DMA_EN)
			break;
		udelay(1);
		timeout++;
	}
	if(timeout == 5000)
		printk(KERN_ERR "Timed out while shutting down Tx DMA\n");

	/* Disable Rx DMA*/
	pDevCtrl->rxDma->rdma_ctrl &= ~DMA_EN;
	timeout = 0;
	while(timeout < 5000)
	{
		if(pDevCtrl->rxDma->rdma_status & DMA_EN)
			break;
		udelay(1);
		timeout++;
	}
	if(timeout == 5000)
		printk(KERN_ERR "Timed out while shutting down Rx DMA\n");

    pDevCtrl->umac->cmd &= ~(CMD_RX_EN | CMD_TX_EN);

    /* free the transmited skb */
	bcmgenet_xmit(NULL, dev);

	if(pDevCtrl->phyType == BRCM_PHY_TYPE_EXT_MII ||
		pDevCtrl->phyType == BRCM_PHY_TYPE_EXT_GMII ||
		pDevCtrl->phyType == BRCM_PHY_TYPE_EXT_GMII_IBS)
	{
		del_timer_sync(&pDevCtrl->timer);
		cancel_work_sync(&pDevCtrl->bcmgenet_link_work);
	}
	return 0;
}

/* --------------------------------------------------------------------------
    Name: bcmgenet_net_timeout
 Purpose: 
-------------------------------------------------------------------------- */
static void bcmgenet_timeout(struct net_device * dev)
{
    BUG_ON(dev == NULL);

    TRACE(("%s: bcmgenet_timeout\n", dev->name));

    dev->trans_start = jiffies;

	dev->stats.tx_errors++;
    netif_wake_queue(dev);
}

/* --------------------------------------------------------------------------
 Name: bcmgenet_set_multicast_list
 Purpose: Set the multicast mode, ie. promiscuous or multicast
-------------------------------------------------------------------------- */
static void bcmgenet_set_multicast_list(struct net_device * dev)
{
    BcmEnet_devctrl *pDevCtrl = netdev_priv(dev);
	struct dev_mc_list * dmi;
	int i, mc;
#define MAX_MC_COUNT	16	

    TRACE(("%s: bcmgenet_set_multicast_list: %08X\n", dev->name, dev->flags));

    /* Promiscous mode */
    if (dev->flags & IFF_PROMISC) {
		pDevCtrl->umac->cmd |= CMD_PROMISC;   
		pDevCtrl->umac->mdf_ctrl = 0;
		return;
    } else {
		pDevCtrl->umac->cmd &= ~CMD_PROMISC;
    }

	/* UniMac doesn't support ALLMULTI */
	if (dev->flags & IFF_ALLMULTI)
		return;
	
	/* update MDF filter */
	i = 0;
	mc = 0;
	/* Broadcast */
	pDevCtrl->umac->mdf_addr[i] = 0xFFFF;
	pDevCtrl->umac->mdf_addr[i+1] = 0xFFFFFFFF;
	pDevCtrl->umac->mdf_ctrl |= (1 << (MAX_MC_COUNT - mc));
	i += 2;
	mc++;
	/* Unicast*/
	pDevCtrl->umac->mdf_addr[i] = (dev->dev_addr[0] << 8) | dev->dev_addr[1];
	pDevCtrl->umac->mdf_addr[i+1] = dev->dev_addr[2] << 24 | dev->dev_addr[3] << 16 | dev->dev_addr[4] << 8 | dev->dev_addr[5];
	pDevCtrl->umac->mdf_ctrl |= (1 << (MAX_MC_COUNT - mc));
	i += 2;
	mc++;
	if (dev->mc_count == 0 || dev->mc_count > (MAX_MC_COUNT - 1)) {
		return;
	}
	/* Multicast */
	for(dmi = dev->mc_list; dmi; dmi = dmi->next) {
		pDevCtrl->umac->mdf_addr[i] = dmi->dmi_addr[0] << 8 | dmi->dmi_addr[1];
		pDevCtrl->umac->mdf_addr[i+1] = dmi->dmi_addr[2] << 24|dmi->dmi_addr[3] << 16|dmi->dmi_addr[4] << 8|dmi->dmi_addr[5];
		pDevCtrl->umac->mdf_ctrl |= (1 << (MAX_MC_COUNT - mc));
		i += 2;
		mc++;
	}
}
/*
 * Set the hardware MAC address.
 */
static int bcmgenet_set_mac_addr(struct net_device *dev, void *p)
{
    struct sockaddr *addr=p;

    if(netif_running(dev))
        return -EBUSY;

    memcpy(dev->dev_addr, addr->sa_data, dev->addr_len);

    return 0;
}
/* --------------------------------------------------------------------------
 Name: bcmgenet_select_queue
 Purpose: select which xmit queue to use based on skb->queue_mapping.
-------------------------------------------------------------------------- */
static u16 __maybe_unused bcmgenet_select_queue(struct net_device *dev, struct sk_buff * skb)
{
#ifdef NET_ACT_SKBEDIT
	/* 
	 * if the kernel's QoS/Classifier is editing skb->queue_mapping, then
	 * we only use descriptor based Tx queue.
	 */
	return DMA_ING_DESC_INDEX;
#else
	return skb->queue_mapping;
#endif
}
/* --------------------------------------------------------------------------
 Name: __bcmgenet_alloc_skb_from_buf
 Purpose: Allocated an skb from exsiting buffer.
-------------------------------------------------------------------------- */
static struct sk_buff * __bcmgenet_alloc_skb_from_buf(unsigned char * buf, int len, int headroom, int users)
{
	struct skb_shared_info * shinfo;
	struct sk_buff * skb;

	skb = kmem_cache_alloc(genet_skb_cache, GFP_ATOMIC);
	if(!skb)
		return NULL;
	memset(skb, 0, offsetof(struct sk_buff, tail));
	skb->truesize = len + sizeof(struct sk_buff);
	/* Set user count to 2, upper layer won't free it */
	atomic_set(&skb->users, users);
	skb->head = buf;
	skb->data = buf;
	skb_reset_tail_pointer(skb);
	skb->end = skb->tail + len - sizeof(struct skb_shared_info);
	skb->cloned = SKB_FCLONE_UNAVAILABLE;

	skb_reserve(skb, headroom);
	shinfo = skb_shinfo(skb);
	atomic_set(&shinfo->dataref, 1);
	shinfo->nr_frags = 0;
	shinfo->gso_size = 0;
	shinfo->gso_segs = 0;
	shinfo->gso_type = 0;
	shinfo->ip6_frag_id = 0;
	shinfo->tx_flags.flags = 0;
	shinfo->frag_list = NULL;
	memset(&shinfo->hwtstamps, 0, sizeof(shinfo->hwtstamps));
	
	return skb;
}
/* --------------------------------------------------------------------------
 Name: __bcmgenet_free_skb
 Purpose: reclaim or free an skb for ring buffer.
 Note: For ring buffer experiment implmentation. 
-------------------------------------------------------------------------- */
static int __bcmgenet_free_skb(struct sk_buff * skb, int free )
{
	struct skb_shared_info * shinfo;
	
	if(skb_is_nonlinear(skb) ) {
		printk(KERN_EMERG "Trying to recycle nonlinear skb for ring buffer\n"
			"len=%d head=%p data=%p tail=%p end=%p\n",
			skb->len, skb->head, skb->data, skb->tail, skb->end);
		/* How could this be happening ?*/
		BUG();
		return 0;
	}
	TRACE(("skb->users=%d\n", atomic_read(&skb->users)) );
	if(skb_shared(skb) || skb_cloned(skb))
		return 0;
	//skb_release_head_state(skb);
	if(free) {
		kmem_cache_free((struct kmem_cache *)skb, NULL);
		return 1;
	}
	atomic_set(&skb->users, 2);
	shinfo = skb_shinfo(skb);
	atomic_set(&shinfo->dataref, 1);
	shinfo->nr_frags = 0;
	shinfo->gso_size = 0;
	shinfo->gso_segs = 0;
	shinfo->gso_type = 0;
	shinfo->ip6_frag_id = 0;
	shinfo->tx_flags.flags = 0;
	shinfo->frag_list = NULL;
	memset(&shinfo->hwtstamps, 0, sizeof(shinfo->hwtstamps));
	memset(skb, 0, offsetof(struct sk_buff, tail));
	skb->data = skb->head ;
	skb_reset_tail_pointer(skb);

	return 1;
}
/* --------------------------------------------------------------------------
 Name: bcmgenet_alloc_txring_skb
 Purpose: Allocated skb for tx ring buffer.
-------------------------------------------------------------------------- */
struct sk_buff * bcmgenet_alloc_txring_skb(struct net_device * dev, int index)
{
	unsigned long flags;
	struct sk_buff * skb = NULL;
    BcmEnet_devctrl *pDevCtrl = netdev_priv(dev);
	if(!(pDevCtrl->txDma->tdma_ctrl & (1 << (index + DMA_RING_BUF_EN_SHIFT))))
	{
		printk(KERN_ERR "Ring %d is not enabled\n", index);
		BUG();
	}
	spin_lock_irqsave(&pDevCtrl->lock, flags);
	if(pDevCtrl->txRingFreeBds[index] == 0) {
		/* This shouldn't happen, upper level should check if the tx queue stopped 
		 * before calling this.
		 */
		printk(KERN_ERR "%s: BUG! Trying to alloc skb for ring %d when queue stopped\n", __FUNCTION__, index);
		spin_unlock_irqrestore(&pDevCtrl->lock, flags);
		return skb;
	}
	skb = __bcmgenet_alloc_skb_from_buf(
			(unsigned char *)phys_to_virt(pDevCtrl->txDma->tDmaRings[index].tdma_write_pointer),
			RX_BUF_LENGTH,
			64,
			1);
	
	spin_unlock_irqrestore(&pDevCtrl->lock, flags);

	return skb;
}
/* --------------------------------------------------------------------------
 Name: bcmgenet_xmit
 Purpose: Send ethernet traffic
-------------------------------------------------------------------------- */
static int bcmgenet_xmit(struct sk_buff * skb, struct net_device * dev)
{
    BcmEnet_devctrl *pDevCtrl = netdev_priv(dev);
    Enet_CB *txCBPtr;
	unsigned int read_ptr = 0, write_ptr = 0;
	int lastTxedCnt = 0, i = 0;
    unsigned long flags;
	StatusBlock * Status;

    /*
     * Obtain exclusive access to transmitter.  This is necessary because
     * we might have more than one stack transmitting at once.
     */
    spin_lock_irqsave(&pDevCtrl->lock, flags);
        
#ifdef B_HAS_GENET_TX_SG
	if (skb && pDevCtrl->txFreeBds <= (skb_shinfo(skb)->nr_frags + 1))
#else
    if ( skb && pDevCtrl->txFreeBds == 0 ) 
#endif
	{
        netif_stop_queue(dev);
    	spin_unlock_irqrestore(&pDevCtrl->lock, flags);
		printk(KERN_ERR "%s: BUG! Tx ring full when queue awake\n", __FUNCTION__);
		return 1;
    }
	/* Compute how many buffers are transmited since last xmit call */
	read_ptr =  (DMA_RW_POINTER_MASK & pDevCtrl->txDma->tDmaRings[DMA_RING_DESC_INDEX].tdma_read_pointer) >> 1;
	write_ptr = (DMA_RW_POINTER_MASK & pDevCtrl->txDma->tDmaRings[DMA_RING_DESC_INDEX].tdma_write_pointer) >> 1;

	if(read_ptr >= pDevCtrl->txLastCIndex) {
		lastTxedCnt = read_ptr - pDevCtrl->txLastCIndex;
	}else {
		lastTxedCnt = pDevCtrl->nrTxBds - pDevCtrl->txLastCIndex + read_ptr;
	}
	TRACE(("%s: read_ptr=%d write_ptr=%d lastTxedCnt=%d txLastCIndex=%d\n", __FUNCTION__,
				read_ptr, write_ptr, lastTxedCnt, pDevCtrl->txLastCIndex));

	/* Recalaim transmitted buffers */
	i = pDevCtrl->txLastCIndex;
	while(lastTxedCnt-- > 0)
	{
		txCBPtr = &pDevCtrl->txCbs[i];
		if(txCBPtr->skb != NULL) {
			skb_dma_unmap(&pDevCtrl->dev->dev, 
					txCBPtr->skb,
					DMA_TO_DEVICE);
            dev_kfree_skb_any (txCBPtr->skb);
			txCBPtr->skb = NULL;
		}
		pDevCtrl->txFreeBds += 1;
		if( i == (TOTAL_DESC - 1 )) i = 0;
		else i++;
	}
	if(skb == NULL)
	{
#ifdef B_HAS_GENET_TX_SG
		if (pDevCtrl->txFreeBds > (MAX_SKB_FRAGS + 1)
			&& netif_queue_stopped(dev))
#else
    	if (skb == NULL && pDevCtrl->txFreeBds > 0
			&& netif_queue_stopped(dev))
#endif
		{
			/* Disable txdma bdone/pdone interrupt if we have free tx bds */
			pDevCtrl->intrl2_0->cpu_mask_set |= UMAC_IRQ_TXDMA_BDONE | UMAC_IRQ_TXDMA_PDONE;
			netif_wake_queue(dev);
		}
		pDevCtrl->txLastCIndex = read_ptr;
        spin_unlock_irqrestore(&pDevCtrl->lock, flags);
        return 0;
    }
	pDevCtrl->txLastCIndex = read_ptr;

	/* 
	 * If 64 byte status block enabled, must make sure skb has
	 * enough headroom for us to insert 64B status block.
	 */
	//if(pDevCtrl->rbuf->tbuf_ctrl & RBUF_64B_EN)
	if(GENET_TBUF_CTRL(pDevCtrl) & RBUF_64B_EN)
	{
		if(likely(skb_headroom(skb) < 64)) {
			/*TODO: does the kernel free the original SKB ? */
			if((skb = skb_realloc_headroom(skb, 64)) == NULL)
			{
    			dev->stats.tx_errors++;
				dev->stats.tx_dropped++;
    			spin_unlock_irqrestore(&pDevCtrl->lock, flags);
				return 0;
			}
		}
		skb_push(skb, 64);
		Status = (StatusBlock *)skb->data;
	}
	/* Obtain transmit control block */
	txCBPtr = &pDevCtrl->txCbs[write_ptr];
    txCBPtr->skb = skb;
	txCBPtr->BdAddr = &pDevCtrl->txBds[write_ptr];
	
	/* Advancing local write pointer */
	if(write_ptr == (TOTAL_DESC - 1)) write_ptr = 0;
	else write_ptr++;
	
	//if((skb->ip_summed  == CHECKSUM_PARTIAL) && (pDevCtrl->rbuf->tbuf_ctrl & RBUF_64B_EN))
	if((skb->ip_summed  == CHECKSUM_PARTIAL) && (GENET_TBUF_CTRL(pDevCtrl) & RBUF_64B_EN))
	{
		u16 offset;
		offset = skb->csum_start - skb_headroom(skb) - 64;
		/* Insert 64B TSB and set the flag */
		Status->tx_csum_info = (offset << STATUS_TX_CSUM_START_SHIFT) | (offset + skb->csum_offset) | STATUS_TX_CSUM_LV;
	}

    /*
     * Add the buffer to the ring.
     * Set addr and length of DMA BD to be transmitted.
     */
	if(!skb_shinfo(skb)->nr_frags) {
		if(skb_dma_map(&pDevCtrl->dev->dev, skb, DMA_TO_DEVICE)) {
			dev_err(&pDevCtrl->dev->dev, "Tx DMA map failed\n");
			return 0;
		}
    	//txCBPtr->BdAddr->address = skb_shinfo(skb)->dma_maps[0];
    	txCBPtr->BdAddr->address = skb_shinfo(skb)->dma_head;
		txCBPtr->BdAddr->length_status = ((unsigned long)((skb->len < ETH_ZLEN) ? ETH_ZLEN : skb->len))<<16;
		txCBPtr->BdAddr->length_status |= DMA_SOP | DMA_EOP | DMA_TX_APPEND_CRC;
		if(skb->ip_summed  == CHECKSUM_PARTIAL)
			txCBPtr->BdAddr->length_status |= DMA_TX_DO_CSUM;

		/* Default QTAG for MoCA */
		txCBPtr->BdAddr->length_status |= (DMA_TX_QTAG_MASK << DMA_TX_QTAG_SHIFT);
    	/* Decrement total BD count and advance our write pointer */
    	pDevCtrl->txFreeBds -= 1;
#ifdef CONFIG_BCMGENET_DUMP_DATA
		printk("%s: len %d", __FUNCTION__, skb->len);
		dumpHexData(skb->data, skb->len);
#endif
		/* advance producer index and write pointer.*/
		pDevCtrl->txDma->tDmaRings[DMA_RING_DESC_INDEX].tdma_producer_index += 1;
		pDevCtrl->txDma->tDmaRings[DMA_RING_DESC_INDEX].tdma_write_pointer += 2;
		pDevCtrl->txDma->tDmaRings[DMA_RING_DESC_INDEX].tdma_write_pointer &= ((TOTAL_DESC << 1)-1);
    	/* update stats */
    	dev->stats.tx_bytes += ((skb->len < ETH_ZLEN) ? ETH_ZLEN : skb->len);
    	dev->stats.tx_packets++;
	}else {
		if(skb_dma_map(&pDevCtrl->dev->dev, skb, DMA_TO_DEVICE)) {
			dev_err(&pDevCtrl->dev->dev, "Tx DMA map failed\n");
			return 0;
		}
		
    	//txCBPtr->BdAddr->address = skb_shinfo(skb)->dma_maps[0];
    	txCBPtr->BdAddr->address = skb_shinfo(skb)->dma_head;
		txCBPtr->BdAddr->length_status = skb_headlen(skb) << 16;
		txCBPtr->BdAddr->length_status |= DMA_SOP | DMA_TX_APPEND_CRC;
		if(skb->ip_summed  == CHECKSUM_PARTIAL)
			txCBPtr->BdAddr->length_status |= DMA_TX_DO_CSUM;

		txCBPtr->BdAddr->length_status |= (DMA_TX_QTAG_MASK << DMA_TX_QTAG_SHIFT);
#ifdef CONFIG_BCMGENET_DUMP_DATA
		printk("%s: frag head len %d", __FUNCTION__, skb_headlen(skb));
		dumpHexData(skb->data, skb_headlen(skb));
#endif
    	/* Decrement total BD count and advance our write pointer */
    	pDevCtrl->txFreeBds -= 1;
		dev->stats.tx_bytes += skb_headlen(skb);

		/* xmit fragment */
		for( i = 0; i < skb_shinfo(skb)->nr_frags; i++) {
			skb_frag_t * frag = &skb_shinfo(skb)->frags[i];
			txCBPtr = &pDevCtrl->txCbs[write_ptr];
    		txCBPtr->skb = NULL;
			txCBPtr->BdAddr = &pDevCtrl->txBds[write_ptr];
			//txCBPtr->BdAddr->address = skb_shinfo(skb)->dma_maps[i+1];
			txCBPtr->BdAddr->address = skb_shinfo(skb)->dma_maps[i];
#ifdef CONFIG_BCMGENET_DUMP_DATA
			printk("%s: frag%d len %d", __FUNCTION__, i, frag->size);
			dumpHexData((page_address(frag->page)+frag->page_offset), frag->size);
#endif
			txCBPtr->BdAddr->length_status = ((unsigned long)(frag->size))<<16;
			txCBPtr->BdAddr->length_status |= (DMA_TX_QTAG_MASK << DMA_TX_QTAG_SHIFT);
			if(i == skb_shinfo(skb)->nr_frags - 1)
				txCBPtr->BdAddr->length_status |= DMA_EOP;
    		
			/* Advancing local write pointer */
			if(write_ptr == (TOTAL_DESC - 1)) write_ptr = 0;
			else write_ptr++;
    		/* Decrement total BD count and advance our write pointer */
    		pDevCtrl->txFreeBds -= 1;
			dev->stats.tx_bytes += frag->size;
		}
    	dev->stats.tx_packets++;
		/* advance producer index and write pointer.*/
		pDevCtrl->txDma->tDmaRings[DMA_RING_DESC_INDEX].tdma_producer_index += skb_shinfo(skb)->nr_frags + 1;
		pDevCtrl->txDma->tDmaRings[DMA_RING_DESC_INDEX].tdma_write_pointer += ((skb_shinfo(skb)->nr_frags + 1) << 1);
		pDevCtrl->txDma->tDmaRings[DMA_RING_DESC_INDEX].tdma_write_pointer &= ((TOTAL_DESC << 1)-1);
	}
#ifdef B_HAS_GENET_TX_SG
	if (pDevCtrl->txFreeBds <= (MAX_SKB_FRAGS + 1))
#else
    if ( pDevCtrl->txFreeBds < 1 )
#endif
	{
		/* Enable Tx bdone/pdone interrupt !*/
		pDevCtrl->intrl2_0->cpu_mask_clear |= UMAC_IRQ_TXDMA_BDONE | UMAC_IRQ_TXDMA_PDONE;
        netif_stop_queue(dev);
    }

    dev->trans_start = jiffies;

    spin_unlock_irqrestore(&pDevCtrl->lock, flags);

    return 0;
}

/* --------------------------------------------------------------------------
 Name: bcmgenet_ring_xmit
 Purpose: Send ethernet traffic through ring buffer
-------------------------------------------------------------------------- */
static int __maybe_unused bcmgenet_ring_xmit(struct sk_buff * skb, struct net_device * dev, int index)
{
    BcmEnet_devctrl *pDevCtrl = netdev_priv(dev);
    Enet_CB *txCBPtr;
	unsigned int p_index = 0, c_index = 0;
	int lastTxedCnt = 0, i = 0;
	StatusBlock * Status;

	/* Compute how many buffers are transmited since last xmit call */
	p_index = (DMA_PRODUCER_INDEX_MASK & pDevCtrl->txDma->tDmaRings[index].tdma_producer_index);
	c_index = (DMA_CONSUMER_INDEX_MASK & pDevCtrl->txDma->tDmaRings[index].tdma_consumer_index);
	
	p_index = p_index & (pDevCtrl->txRingSize[index] - 1);
	c_index = c_index & (pDevCtrl->txRingSize[index] - 1);

	if(c_index >= pDevCtrl->txRingCIndex[index]) {
		lastTxedCnt = c_index - pDevCtrl->txRingCIndex[index];
	}else {
		lastTxedCnt = pDevCtrl->txRingSize[index] - pDevCtrl->txRingCIndex[index] + c_index;
	}
	TRACE(("%s: ring %d: p_index=%d r_index=%d lastTxedCnt=%d txLastCIndex=%d\n", __FUNCTION__,
				index, p_index, c_index, lastTxedCnt, pDevCtrl->txRingCIndex[index]));
	
	/* Recycle transmitted skbs */
	i = pDevCtrl->txRingCIndex[index];
	while(lastTxedCnt-- > 0)
	{
		txCBPtr = pDevCtrl->txRingCBs[index] + i;
		if(txCBPtr->skb != NULL) {
			skb_dma_unmap(&pDevCtrl->dev->dev, 
					txCBPtr->skb,
					DMA_TO_DEVICE);
			if(likely(__bcmgenet_free_skb(txCBPtr->skb, 0)) ) {
				txCBPtr->skb = NULL;
				pDevCtrl->txRingFreeBds[index] += 1;
				if( i == ((pDevCtrl->txDma->tDmaRings[index].tdma_ring_buf_size >> DMA_RING_SIZE_SHIFT) - 1 ))
					i = 0;
				else
					i += 1;
			}else {
				/* ooops, who touched our tx ring skb ?*/
				printk(KERN_EMERG "Failed to free tx skb (head=0x%08x)\n", (unsigned int)txCBPtr->skb->head);
				BUG();
			}
		}
	}
    if (skb == NULL && pDevCtrl->txRingFreeBds[index] > 0) {
		/* Disable txdma multibuf done interrupt for this ring if we have free tx bds */
		pDevCtrl->intrl2_1->cpu_mask_set |= (1 << index);
		netif_wake_subqueue(dev, index);
        return 0;
    }
	pDevCtrl->txRingCIndex[index] = c_index;
	
	/* Obtain a tx control block */
	txCBPtr = pDevCtrl->txRingCBs[index] + p_index;
    txCBPtr->skb = skb;
	/* Can't use skb_dma_map here, since we need to map the skb->head */
	txCBPtr->dma_addr = dma_map_single(&pDevCtrl->dev->dev, skb->head, pDevCtrl->rxBufLen, DMA_TO_DEVICE);
	/* 
	 * Make sure we have headroom for us to insert 64B status block.
	 */
	if(unlikely(skb_headroom(skb) < 64)) {
		printk(KERN_ERR "no enough headroom for TSB (head=0x%08x)\n", (unsigned int)skb->head);
		BUG();
	}
	Status = (StatusBlock *)skb->head;
	//if((skb->ip_summed  == CHECKSUM_PARTIAL) && (pDevCtrl->rbuf->tbuf_ctrl & RBUF_64B_EN))
	if((skb->ip_summed  == CHECKSUM_PARTIAL) && (GENET_TBUF_CTRL(pDevCtrl) & RBUF_64B_EN))
	{
		u16 offset;
		offset = skb->csum_start - skb_headroom(skb) - 64;
		/* Insert 64B TSB and set the flag */
		Status->tx_csum_info = (offset << STATUS_TX_CSUM_START_SHIFT) | (offset + skb->csum_offset) | STATUS_TX_CSUM_LV;
		Status->length_status |= DMA_TX_DO_CSUM;
		TRACE(("Tx Hw Csum: head=0x%08x data=0x%08x csum_start=%d csum_offset=%d\n", 
					skb->head, skb->data, skb->csum_start, skb->csum_offset));
	}

	Status->length_status = ((unsigned long)((skb->len < ETH_ZLEN) ? ETH_ZLEN : skb->len))<<16;
	Status->length_status |= DMA_SOP | DMA_EOP | DMA_TX_APPEND_CRC;

#ifdef CONFIG_BCMGENET_DUMP_DATA
    printk("bcmgenet_xmit: len %d", skb->len);
    dumpHexData(skb->head, skb->len);
#endif

    /* Decrement total BD count and advance our write pointer */
    pDevCtrl->txRingFreeBds[index] -= 1;

	/* advance producer index and write pointer.*/
	pDevCtrl->txDma->tDmaRings[index].tdma_producer_index += 1;
	if((pDevCtrl->txDma->tDmaRings[index].tdma_write_pointer + RX_BUF_LENGTH) > 
		pDevCtrl->txDma->tDmaRings[index].tdma_end_addr) {
		pDevCtrl->txDma->tDmaRings[index].tdma_write_pointer = pDevCtrl->txDma->tDmaRings[index].tdma_start_addr;
	}else {
		pDevCtrl->txDma->tDmaRings[index].tdma_write_pointer += RX_BUF_LENGTH;
	}
	

    if ( pDevCtrl->txRingFreeBds[index] == 0 ) {
        TRACE(("%s: bcmgenet_xmit no transmit queue space -- stopping queues\n", dev->name));
		/* Enable Tx bdone/pdone interrupt !*/
		pDevCtrl->intrl2_0->cpu_mask_clear |= (1 << index);
        netif_stop_subqueue(dev, index);
    }

    /* update stats */
    dev->stats.tx_bytes += ((skb->len < ETH_ZLEN) ? ETH_ZLEN : skb->len);
    dev->stats.tx_packets++;

    dev->trans_start = jiffies;

    return 0;
}
/* NAPI polling method*/
static int bcmgenet_poll(struct napi_struct * napi, int budget)
{
	BcmEnet_devctrl *pDevCtrl = container_of(napi, struct BcmEnet_devctrl, napi);
	volatile intrl2Regs * intrl2 = pDevCtrl->intrl2_0;
	unsigned int work_done;
	work_done = bcmgenet_desc_rx(pDevCtrl, budget);

	bcmgenet_xmit(NULL, pDevCtrl->dev);
	/* Allocate new SKBs for the BD ring */
	assign_rx_buffers(pDevCtrl);
	pDevCtrl->rxDma->rDmaRings[DMA_RING_DESC_INDEX].rdma_consumer_index += work_done;
	pDevCtrl->rxDma->rDmaRings[DMA_RING_DESC_INDEX].rdma_read_pointer += (work_done << 1);
	pDevCtrl->rxDma->rDmaRings[DMA_RING_DESC_INDEX].rdma_read_pointer &= ((TOTAL_DESC << 1)-1);
	if(work_done < budget)
	{
		napi_complete(napi);
		intrl2->cpu_mask_clear |= UMAC_IRQ_RXDMA_BDONE;
	}
	return work_done;
}
/* 
 * NAPI polling for ring buffer.
 */
static int bcmgenet_ring_poll(struct napi_struct * napi, int budget)
{
	BcmEnet_devctrl *pDevCtrl = container_of(napi, struct BcmEnet_devctrl, ring_napi);
	volatile intrl2Regs * intrl2 = pDevCtrl->intrl2_1;
	unsigned int work_done;
	work_done = bcmgenet_ring_rx(pDevCtrl, budget);

	if(work_done < budget)
	{
		napi_complete(napi);
		intrl2->cpu_mask_clear |= ((pDevCtrl->rxDma->rdma_ctrl >> 1) << 16);
	}
	return work_done;
}
/*
 * Interrupt bottom half
 */
static void bcmgenet_irq_task(struct work_struct * work)
{
    BcmEnet_devctrl *pDevCtrl = container_of(work, struct BcmEnet_devctrl, bcmgenet_irq_work);

	TRACE(("%s\n", __FUNCTION__));
	/* Cable plugged/unplugged event */
	if (pDevCtrl->irq0_stat & UMAC_IRQ_PHY_DET_R) {
		pDevCtrl->irq0_stat &= ~UMAC_IRQ_PHY_DET_R;
		printk(KERN_CRIT "%s cable plugged in, powering up\n", pDevCtrl->dev->name);
		bcmgenet_power_up(pDevCtrl, GENET_POWER_CABLE_SENSE);

	}else if (pDevCtrl->irq0_stat & UMAC_IRQ_PHY_DET_F) {
		pDevCtrl->irq0_stat &= ~UMAC_IRQ_PHY_DET_F;
		printk(KERN_CRIT "%s cable unplugged, powering down\n", pDevCtrl->dev->name);
		bcmgenet_power_down(pDevCtrl, GENET_POWER_CABLE_SENSE);
	}
	if (pDevCtrl->irq0_stat & UMAC_IRQ_MPD_R) {
		pDevCtrl->irq0_stat &= ~UMAC_IRQ_MPD_R;
		printk(KERN_CRIT "%s magic packet detected, waking up\n", pDevCtrl->dev->name);
		/* disable mpd interrupt */
		pDevCtrl->intrl2_0->cpu_mask_set |= UMAC_IRQ_MPD_R;
		/* disable CRC forward.*/
		pDevCtrl->umac->cmd &= ~CMD_CRC_FWD;
		bcmgenet_power_up(pDevCtrl, GENET_POWER_WOL_MAGIC);
		
	}else if (pDevCtrl->irq0_stat & (UMAC_IRQ_HFB_SM | UMAC_IRQ_HFB_MM)) {
		pDevCtrl->irq0_stat &= ~(UMAC_IRQ_HFB_SM | UMAC_IRQ_HFB_MM);
		printk(KERN_CRIT "%s ACPI pattern matched, waking up\n", pDevCtrl->dev->name);
		/* disable HFB match interrupts */
		pDevCtrl->intrl2_0->cpu_mask_set |= (UMAC_IRQ_HFB_SM | UMAC_IRQ_HFB_MM);
		bcmgenet_power_up(pDevCtrl, GENET_POWER_WOL_ACPI);
	}

	/* Link UP/DOWN event */
	if(pDevCtrl->irq0_stat & UMAC_IRQ_LINK_UP)
	{
		printk(KERN_CRIT "%s Link UP.\n", pDevCtrl->dev->name);
		/* Clear soft-copy of irq status*/
		pDevCtrl->irq0_stat &= ~UMAC_IRQ_LINK_UP;
		
		if (!(pDevCtrl->umac->cmd & CMD_AUTO_CONFIG)) {
			printk(KERN_CRIT "Auto config phy\n");
			mii_setup(pDevCtrl->dev);
		}
		if(!netif_carrier_ok(pDevCtrl->dev))
		{
			pDevCtrl->dev->flags |= IFF_RUNNING;
			netif_carrier_on(pDevCtrl->dev);
			rtmsg_ifinfo(RTM_NEWLINK, pDevCtrl->dev, IFF_RUNNING);
		}
			
	}else if (pDevCtrl->irq0_stat & UMAC_IRQ_LINK_DOWN)
	{
		printk(KERN_CRIT "%s Link DOWN.\n", pDevCtrl->dev->name);
		pDevCtrl->irq0_stat &= ~UMAC_IRQ_LINK_DOWN;	/* clear soft-copy of irq status */
		/* TODO:Disable DMA Rx channels.  */
		/* In case there are packets in the Rx descriptor */
		if(netif_carrier_ok(pDevCtrl->dev))
		{
			netif_carrier_off(pDevCtrl->dev);
			pDevCtrl->dev->flags &= ~IFF_RUNNING;
			rtmsg_ifinfo(RTM_DELLINK, pDevCtrl->dev, IFF_RUNNING);
		}
	}
}
/* try to recycle rx skb allocated for ring buffer
 * pDevctrl device private data pointer
 * index: ring index.
 * budget: loop count.
 * wrap: if none-zero , means DMA wrapp occured.
 * return: if all skbs have been recycled reurn 0, otherwise return budget.
 */
static int bcmgenet_rxskb_recycle(BcmEnet_devctrl * pDevCtrl, int index, int budget, int wrap)
{
	Enet_CB * cb;
	struct sk_buff * skb;
	int loopcnt = 0, retvalue = 0;
	int p_index = (DMA_PRODUCER_INDEX_MASK) & pDevCtrl->rxDma->rDmaRings[index].rdma_producer_index;
	if(!wrap)
		p_index = p_index & (pDevCtrl->rxRingSize[index] - 1);
	while(loopcnt++ < budget)
	{
		/* 
		 * Advancing consumer index if buffer has been consumed by stack layer
		 */
		TRACE(("p_index=%d CIndex=%d\n", p_index, pDevCtrl->rxRingCIndex[index]));
		//printk("p_index=%d CIndex=%d\n", p_index, pDevCtrl->rxRingCIndex[index]);
		if(!wrap) {
			if(pDevCtrl->rxRingCIndex[index] == p_index) {
				retvalue = 0;
				break;
			}
		}
		cb = pDevCtrl->rxRingCbs[index] + pDevCtrl->rxRingCIndex[index];
		skb = cb->skb;
		if(__bcmgenet_free_skb(skb, 0)) {
			TRACE(("buffer 0x%x is recycled \n", skb->data));
			//printk("buffer 0x%x is recycled \n", skb->data);
			pDevCtrl->rxDma->rDmaRings[index].rdma_consumer_index++;
			if(pDevCtrl->rxRingCIndex[index] == pDevCtrl->rxRingSize[index] - 1)
				pDevCtrl->rxRingCIndex[index] = 0;
			else
				pDevCtrl->rxRingCIndex[index]++;
		}else {
			/* Tells NAPI that we are not done even no new packet came in! */
			retvalue = budget;
			break;
		}
	}

	return retvalue;
}

/*
 * bcmgenet_ring_rx: ring buffer rx function.
 */
static unsigned int bcmgenet_ring_rx(void * ptr, unsigned int budget)
{
    BcmEnet_devctrl *pDevCtrl = ptr;
	int i, len;
	Enet_CB * cb;
	struct sk_buff * skb;
    unsigned long dmaFlag;
	volatile StatusBlock * status;
    unsigned int rxpktprocessed = 0, rxpkttoprocess = 0, retvalue = 0;
	unsigned int read_ptr = 0, write_ptr = 0, p_index = 0, c_index = 0;
    
	TRACE(("%s: irq_stat=0x%08x\n", __FUNCTION__, pDevCtrl->irq1_stat));

	/* loop for each ring */
	for(i = 0; i < GENET_RX_RING_COUNT; i++)
	{
		if(!(pDevCtrl->rxDma->rdma_ctrl & (1 << (i + DMA_RING_BUF_EN_SHIFT))) )
			continue;
		
		write_ptr = pDevCtrl->rxDma->rDmaRings[i].rdma_write_pointer;
		read_ptr =  pDevCtrl->rxDma->rDmaRings[i].rdma_read_pointer;
		p_index = (DMA_PRODUCER_INDEX_MASK) & pDevCtrl->rxDma->rDmaRings[i].rdma_producer_index;
		c_index = (DMA_PRODUCER_INDEX_MASK) & pDevCtrl->rxDma->rDmaRings[i].rdma_consumer_index;
		
		/* Check to see if the packets we pushed up have been consumed
		 * or not, if they have, advancing consumer index.
		 * Note that producer index is 16 bits, we need to find modulo of ring size to 
		 * figure out where it's pointing to in the ring.
		 */

		if(p_index - c_index == pDevCtrl->rxRingSize[i]) {
			/* overflow, we couldn't keep up with DMA and DMA wrapped around */
			pDevCtrl->dev->stats.rx_over_errors += (pDevCtrl->rxDma->rDmaRings[i].rdma_producer_index >> 16) 
				- pDevCtrl->rxRingDiscCnt[i];
			pDevCtrl->rxRingDiscCnt[i] = pDevCtrl->rxDma->rDmaRings[i].rdma_producer_index >> 16;
			/* Try to recycle rx skbs and increment consumer index*/
			retvalue = bcmgenet_rxskb_recycle(pDevCtrl, i, budget, 1);
		}else  {
			retvalue = bcmgenet_rxskb_recycle(pDevCtrl, i, budget, 0);
		}
		if(!(pDevCtrl->irq1_stat & (1 << (16 + i))) )
			continue;
		
		/* 
		 * We can't use produer/consumer index to compute how many outstanding packets are there, 
		 * because we are not advancing consumer index right after packets are moved out of DMA.
		 * Here we use read/write pointer to do the math.
		 */
		if(write_ptr < read_ptr) {
			rxpkttoprocess = (pDevCtrl->rxDma->rDmaRings[i].rdma_end_addr + 1 - read_ptr) >> (RX_BUF_BITS - 1);
			rxpkttoprocess += (write_ptr - pDevCtrl->rxDma->rDmaRings[i].rdma_start_addr) >> (RX_BUF_BITS - 1);
		}else {
			rxpkttoprocess = (write_ptr - read_ptr) >> (RX_BUF_BITS - 1);
		}
		TRACE(("%s: p_inde=%d write_ptr=0x%08x read_ptr=0x%08x rxpkttoprocess=%d\n", __FUNCTION__,
						p_index, write_ptr, read_ptr, rxpkttoprocess));

		while((rxpktprocessed < rxpkttoprocess) && (rxpktprocessed < budget)) 
		{
			unsigned int cbi;
			/* 
			 * Find out Which buffer in the ring are we pointing to. 
			 */
			cbi = (read_ptr - pDevCtrl->rxDma->rDmaRings[i].rdma_start_addr) >> (RX_BUF_BITS - 1);
			cb = pDevCtrl->rxRingCbs[i] + cbi;
			status = (StatusBlock *)cb->BdAddr;
			TRACE(("cbi = %d cb->BdAddr=0x%08x length_status=0x%08x f_index=0x%08x\n", 
					cbi, (unsigned long)cb->BdAddr, status->length_status, status->filter_index));
			dmaFlag = status->length_status & 0xffff;
			len = status->length_status >> 16;
			/*
			 * Advancing our read pointer.
			 */
			if(read_ptr + RX_BUF_LENGTH > pDevCtrl->rxDma->rDmaRings[i].rdma_end_addr)
				read_ptr = pDevCtrl->rxDma->rDmaRings[i].rdma_start_addr;
			else
				read_ptr += RX_BUF_LENGTH;
			pDevCtrl->rxDma->rDmaRings[i].rdma_read_pointer = read_ptr;
			/*
			 * start processing packet.
			 */
			skb = cb->skb;
			rxpktprocessed++;
			BUG_ON(skb == NULL);
			
			/* report errors */
			if(unlikely(!(dmaFlag & DMA_EOP) || !(dmaFlag & DMA_SOP)) ) {
				/* probably can't do this for scater gather ?*/
				printk(KERN_WARNING "Droping fragmented packet!\n");
				pDevCtrl->dev->stats.rx_dropped++;
				pDevCtrl->dev->stats.rx_errors++;
				continue;
			}
       		if (unlikely(dmaFlag & (DMA_RX_CRC_ERROR | DMA_RX_OV | DMA_RX_NO | DMA_RX_LG |DMA_RX_RXER))) {
				TRACE(("ERROR: dmaFlag=0x%x\n", (unsigned int)dmaFlag));
           		if (dmaFlag & DMA_RX_CRC_ERROR) 
               		pDevCtrl->dev->stats.rx_crc_errors++;
           		if (dmaFlag & DMA_RX_OV)
	                pDevCtrl->dev->stats.rx_over_errors++;
           		if (dmaFlag & DMA_RX_NO)
	                pDevCtrl->dev->stats.rx_frame_errors++;
           		if (dmaFlag & DMA_RX_LG)
	                pDevCtrl->dev->stats.rx_length_errors++;
          		
				pDevCtrl->dev->stats.rx_dropped++;
           		pDevCtrl->dev->stats.rx_errors++;
				continue;
			}/* error packet */

			skb_put(skb, len);
			/* we must have 64B rx status block enabled.*/
			if(pDevCtrl->rbuf->rbuf_chk_ctrl & RBUF_RXCHK_EN)
			{
				if(status->rx_csum & STATUS_RX_CSUM_OK) {
					skb->csum = status->rx_csum ;
					/*should swap bytes based on rbuf->endian_ctrl */
					skb->csum = ((skb->csum << 8) & 0xFF00) | ((skb->csum >> 8) & 0xFF);
				}
				skb->ip_summed = CHECKSUM_COMPLETE;
			}
			/*TODO: check filter index and compare with ring index, report error if not matched */
			skb_pull(skb, 64);
			len -= 64;

			if(pDevCtrl->bIPHdrOptimize)
			{
				skb_pull(skb, 2);
				len -= 2;
			}

			if(pDevCtrl->umac->cmd &CMD_CRC_FWD)
			{
				skb_trim(skb, len - 4);
				len -= 4;
			}
#ifdef CONFIG_BCMGENET_DUMP_DATA
       		printk("%s:\n", __FUNCTION__);
       		dumpHexData(skb->data, skb->len);
#endif
       		/* Finish setting up the received SKB and send it to the kernel */
       		skb->dev = pDevCtrl->dev;
       		skb->protocol = eth_type_trans(skb, pDevCtrl->dev);
       		pDevCtrl->dev->stats.rx_packets++;
       		pDevCtrl->dev->stats.rx_bytes += len;
			if(dmaFlag & DMA_RX_MULT)
				pDevCtrl->dev->stats.multicast++;

			skb->queue_mapping = i;
       		/* Notify kernel */
			netif_receive_skb(skb);
       		TRACE(("pushed up to kernel\n"));

		}/* packet process loop */

	}/* ring index loop */

	if(retvalue == 0)
		retvalue = rxpktprocessed;
		
	return retvalue;;
}
/*
 * bcmgenet_isr1: interrupt handler for ring buffer.
 */
static irqreturn_t bcmgenet_isr1(int irq, void * dev_id)
{
    BcmEnet_devctrl *pDevCtrl = dev_id;
	volatile intrl2Regs * intrl2 = pDevCtrl->intrl2_1;
	
	/* Save irq status for bottom-half processing. */
	pDevCtrl->irq1_stat = intrl2->cpu_stat & ~intrl2->cpu_mask_status;
	/* clear inerrupts*/
	intrl2->cpu_clear |= pDevCtrl->irq1_stat;

	TRACE(("%s: IRQ=0x%x\n", __FUNCTION__, pDevCtrl->irq1_stat));
	if (pDevCtrl->irq1_stat & 0xffff0000) {
		/*
		 * We use NAPI here, because of the fact that we are not advancing 
		 * consumer index right after data moved out of DMA, instead we
		 * advance it only when we found out upper level has consumed it.
		 */
		if(likely(napi_schedule_prep(&pDevCtrl->ring_napi))) {
			/* Disable all rx ring interrupt */
			intrl2->cpu_mask_set |= 0xffff0000;
			__napi_schedule(&pDevCtrl->ring_napi);
		}
	}
    return IRQ_HANDLED;
}
/*
 * bcmgenet_isr0: Handle various interrupts.
 */
static irqreturn_t bcmgenet_isr0(int irq, void * dev_id)
{
    BcmEnet_devctrl *pDevCtrl = dev_id;
	volatile intrl2Regs * intrl2 = pDevCtrl->intrl2_0;
	
	/* Save irq status for bottom-half processing. */
	pDevCtrl->irq0_stat = intrl2->cpu_stat & ~intrl2->cpu_mask_status;
	/* clear inerrupts*/
	intrl2->cpu_clear |= pDevCtrl->irq0_stat;

	TRACE(("IRQ=0x%x\n", pDevCtrl->irq0_stat));
#ifndef CONFIG_BCMGENET_RX_DESC_THROTTLE
	if (pDevCtrl->irq0_stat & (UMAC_IRQ_RXDMA_BDONE|UMAC_IRQ_RXDMA_PDONE)) {
		/*
		 * We use NAPI(software interrupt throttling, if
		 * Rx Descriptor throttling is not used.
		 * Disable interrupt, will be enabled in the poll method.
		 */
		if(likely(napi_schedule_prep(&pDevCtrl->napi))) {
			TRACE(("%s:Disabling RXDMA_BDONE interrupt\n", __FUNCTION__));
			intrl2->cpu_mask_set |= UMAC_IRQ_RXDMA_BDONE;
			__napi_schedule(&pDevCtrl->napi);
		}
	}
#else
	/* Multiple buffer done event. */
	if(pDevCtrl->irq0_stat & UMAC_IRQ_DESC_THROT)
	{
		pDevCtrl->irq0_stat &= ~UMAC_IRQ_DESC_THROT;
		TRACE(("%s: %d packets avaiable\n", __FUNCTION__, DmaDescThres));
		bcmgenet_desc_rx(pDevCtrl, DmaDescThres);
	}
#endif
	if(pDevCtrl->irq0_stat & (UMAC_IRQ_TXDMA_BDONE | UMAC_IRQ_TXDMA_PDONE) )
	{
		bcmgenet_xmit(NULL, pDevCtrl->dev);
	}
	if(pDevCtrl->irq0_stat & (UMAC_IRQ_PHY_DET_R |
				UMAC_IRQ_PHY_DET_F |
				UMAC_IRQ_LINK_UP |
				UMAC_IRQ_LINK_DOWN |
				UMAC_IRQ_HFB_SM |
				UMAC_IRQ_HFB_MM |
				UMAC_IRQ_MPD_R) )
	{
		/* all other interested interrupts handled in bottom half */
		schedule_work(&pDevCtrl->bcmgenet_irq_work);
	}

    return IRQ_HANDLED;
}
/*
 *  bcmgenet_desc_rx - descriptor based rx process.
 *  this could be called from bottom half, or from NAPI polling method.
 */
static unsigned int bcmgenet_desc_rx(void *ptr, unsigned int budget)
{
    BcmEnet_devctrl *pDevCtrl = ptr;
	struct net_device * dev = pDevCtrl->dev;
	Enet_CB * cb;
	struct sk_buff * skb;
    unsigned long dmaFlag;
    int len;
    unsigned int rxpktprocessed = 0, rxpkttoprocess = 0;
	unsigned int p_index = 0, c_index = 0, write_ptr = 0, read_ptr = 0;
	
	p_index = DMA_PRODUCER_INDEX_MASK & pDevCtrl->rxDma->rDmaRings[DMA_RING_DESC_INDEX].rdma_producer_index;
	c_index = DMA_CONSUMER_INDEX_MASK & pDevCtrl->rxDma->rDmaRings[DMA_RING_DESC_INDEX].rdma_consumer_index;
	write_ptr = DMA_RW_POINTER_MASK & pDevCtrl->rxDma->rDmaRings[DMA_RING_DESC_INDEX].rdma_write_pointer;
	read_ptr = DMA_RW_POINTER_MASK & pDevCtrl->rxDma->rDmaRings[DMA_RING_DESC_INDEX].rdma_read_pointer;

	if(p_index < c_index)
	{
		rxpkttoprocess = (DMA_CONSUMER_INDEX_MASK+1) - c_index + p_index;
	}else {
		rxpkttoprocess = p_index - c_index;
	}
	TRACE(("RDMA: rxpkttoprocess=%d\n", rxpkttoprocess));
	read_ptr = read_ptr >> 1;
	
	while((rxpktprocessed < rxpkttoprocess)
			&& (rxpktprocessed < budget) )
	{
    	dmaFlag = (pDevCtrl->rxBds[read_ptr].length_status & 0xffff);
        len = ((pDevCtrl->rxBds[read_ptr].length_status)>>16);
		TRACE(("%s:p_index=%d c_index=%d read_ptr=%d write_ptr=%d length_status=0x%08x\n", __FUNCTION__, 
					p_index, c_index, read_ptr, write_ptr, (pDevCtrl->rxBds[read_ptr].length_status) ));

		rxpktprocessed++;

		cb = &pDevCtrl->rxCbs[read_ptr];
		skb = cb->skb;
		BUG_ON(skb == NULL);
		
		pDevCtrl->rxBds[read_ptr].address = 0;
	
		if(read_ptr == pDevCtrl->nrRxBds-1 )
			read_ptr = 0;
		else
			read_ptr++;
		
		if(unlikely(!(dmaFlag & DMA_EOP) || !(dmaFlag & DMA_SOP)) )
		{
			printk(KERN_WARNING "Droping fragmented packet!\n");
			dev->stats.rx_dropped++;
			dev->stats.rx_errors++;
			dev_kfree_skb_any(cb->skb);
			continue;
		}
		/* report errors */
        if (unlikely(dmaFlag & (DMA_RX_CRC_ERROR | DMA_RX_OV | DMA_RX_NO | DMA_RX_LG |DMA_RX_RXER))) {
			TRACE(("ERROR: dmaFlag=0x%x\n", (unsigned int)dmaFlag));
            if (dmaFlag & DMA_RX_CRC_ERROR) {
                dev->stats.rx_crc_errors++;
            }
            if (dmaFlag & DMA_RX_OV) {
                dev->stats.rx_over_errors++;
            }
            if (dmaFlag & DMA_RX_NO) {
                dev->stats.rx_frame_errors++;
            }
            if (dmaFlag & DMA_RX_LG) {
                dev->stats.rx_length_errors++;
            }
            dev->stats.rx_dropped++;
            dev->stats.rx_errors++;

			/* discard the packet and advance consumer index.*/
			dev_kfree_skb_any(cb->skb);
			cb->skb = NULL;
			continue;
		}/* error packet */

		skb_put(skb, len);
		if(pDevCtrl->rbuf->rbuf_ctrl & RBUF_64B_EN)
		{
			StatusBlock * status = (StatusBlock*)skb->data;
			/* we have 64B rx status block enabled.*/
			if(pDevCtrl->rbuf->rbuf_chk_ctrl & RBUF_RXCHK_EN)
			{
				if(status->rx_csum & STATUS_RX_CSUM_OK) {
					skb->ip_summed = CHECKSUM_UNNECESSARY;
				}else {
					skb->ip_summed = CHECKSUM_NONE;
				}
			}
			skb_pull(skb, 64);
			len -= 64;
		}

		if(pDevCtrl->bIPHdrOptimize)
		{
			skb_pull(skb, 2);
			len -= 2;
		}

		if(pDevCtrl->umac->cmd &CMD_CRC_FWD)
		{
			skb_trim(skb, len - 4);
			len -= 4;
		}
#ifdef CONFIG_BCMGENET_DUMP_DATA
        printk("bcmgenet_desc_rx :");
        dumpHexData(skb->data, skb->len);
#endif

        /* Finish setting up the received SKB and send it to the kernel */
        skb->dev = pDevCtrl->dev;
        skb->protocol = eth_type_trans(skb, pDevCtrl->dev);
        dev->stats.rx_packets++;
        dev->stats.rx_bytes += len;
		if(dmaFlag & DMA_RX_MULT)
			dev->stats.multicast++;

        /* Notify kernel */
#ifdef CONFIG_BCMGENET_RX_DESC_THROTTLE
		netif_rx(skb);
#else
        netif_receive_skb(skb);
#endif
		cb->skb = NULL;
        TRACE(("pushed up to kernel\n"));
    }

    return rxpktprocessed;
}


/*
 * assign_rx_buffers: 
 * Assign skb to RX DMA descriptor. 
 */
static int assign_rx_buffers(BcmEnet_devctrl *pDevCtrl)
{
    struct sk_buff *skb;
    unsigned short  bdsfilled=0;
	unsigned long flags;
	
	TRACE(("%s\n", __FUNCTION__));

    /*
     * This function may be called from irq bottom-half.
     */
#ifndef CONFIG_BCMGENET_RX_DESC_THROTTLE
	(void)flags;
	spin_lock_bh(&pDevCtrl->bh_lock);
#else
	spin_lock_irqsave(&pDevCtrl->lock, flags);
#endif

    /* loop here for each buffer needing assign */
	while(pDevCtrl->rxBdAssignPtr->address == 0)
	{
		Enet_CB * cb = &pDevCtrl->rxCbs[pDevCtrl->rxBdAssignPtr - pDevCtrl->rxBds];
		skb = netdev_alloc_skb(pDevCtrl->dev, pDevCtrl->rxBufLen + SKB_ALIGNMENT );
		if(!skb) {
			printk(KERN_CRIT " failed to allocate skb for rx\n");
			break;
		}
        handleAlignment(pDevCtrl, skb);

        /* keep count of any BD's we refill */
        bdsfilled++;

		cb->skb = skb;
		cb->dma_addr = dma_map_single(&pDevCtrl->dev->dev, skb->data, pDevCtrl->rxBufLen, DMA_FROM_DEVICE);
        /* assign packet, prepare descriptor, and advance pointer */
        pDevCtrl->rxBdAssignPtr->address = cb->dma_addr;
        pDevCtrl->rxBdAssignPtr->length_status  = (pDevCtrl->rxBufLen<<16);

        /* turn on the newly assigned BD for DMA to use */
        if (pDevCtrl->rxBdAssignPtr == pDevCtrl->rxBds + pDevCtrl->nrRxBds - 1) {
            pDevCtrl->rxBdAssignPtr = pDevCtrl->rxBds ;
        }
        else {
            pDevCtrl->rxBdAssignPtr++;
        }
    }

	/* Enable rx DMA incase it was disabled due to running out of rx BD */
	pDevCtrl->rxDma->rdma_ctrl |= DMA_EN;

#ifndef CONFIG_BCMGENET_RX_DESC_THROTTLE
	spin_unlock_bh(&pDevCtrl->bh_lock);
#else
	spin_unlock_irqrestore(&pDevCtrl->lock, flags);
#endif

	TRACE(("%s return bdsfilled=%d\n", __FUNCTION__, bdsfilled));
    return bdsfilled;
}

/*
 * init_umac: Initializes the uniMac controller
 */
static int init_umac(BcmEnet_devctrl *pDevCtrl)
{
    volatile uniMacRegs *umac;
	volatile intrl2Regs *intrl2; 
	struct net_device * dev = pDevCtrl->dev;
	
	umac = pDevCtrl->umac;;
	intrl2 = pDevCtrl->intrl2_0;

    TRACE(("bcmgenet: init_umac "));

    /* disable MAC while updating its registers */
    umac->cmd = 0 ;

    /* issue soft reset, wait for it to complete */
    umac->cmd = CMD_SW_RESET;
	udelay(1000);
	umac->cmd = 0;
    /* clear tx/rx counter */
    umac->mib_ctrl = MIB_RESET_RX | MIB_RESET_TX | MIB_RESET_RUNT;
	umac->mib_ctrl = 0;

#ifdef MAC_LOOPBACK
	/* Enable GMII/MII loopback */
    umac->cmd |= CMD_LCL_LOOP_EN;
#endif
	umac->max_frame_len = ENET_MAX_MTU_SIZE;
    /* 
	 * init rx registers, enable ip header optimization.
	 */
    if (pDevCtrl->bIPHdrOptimize) {
        pDevCtrl->rbuf->rbuf_ctrl |= RBUF_ALIGN_2B ;
    }
	umac->mac_0 = dev->dev_addr[0] << 24 | dev->dev_addr[1] << 16 | dev->dev_addr[2] << 8 | dev->dev_addr[3];
	umac->mac_1 = dev->dev_addr[4] << 8 | dev->dev_addr[5];
	
	/* Mask all interrupts.*/
	intrl2->cpu_mask_set = 0xFFFFFFFF;
	intrl2->cpu_clear = 0xFFFFFFFF;
	intrl2->cpu_mask_clear = 0x0;

#ifdef CONFIG_BCMGENET_RX_DESC_THROTTLE
	intrl2->cpu_mask_clear |= UMAC_IRQ_RXDMA_MBDONE;
#else
	intrl2->cpu_mask_clear |= UMAC_IRQ_RXDMA_BDONE;
	TRACE(("%s:Enabling RXDMA_BDONE interrupt\n", __FUNCTION__));
#endif /* CONFIG_BCMGENET_RX_DESC_THROTTLE */
    
	/* Monitor cable plug/unpluged event for internal PHY */
	if (pDevCtrl->phyType == BRCM_PHY_TYPE_INT)
	{
		intrl2->cpu_mask_clear |= UMAC_IRQ_PHY_DET_R | UMAC_IRQ_PHY_DET_F;
		intrl2->cpu_mask_clear |= UMAC_IRQ_LINK_DOWN | UMAC_IRQ_LINK_UP ;
		/*GENET bug */
		pDevCtrl->ext->ext_pwr_mgmt |= EXT_ENERGY_DET_MASK;
	}else if (pDevCtrl->phyType == BRCM_PHY_TYPE_EXT_MII ||
			pDevCtrl->phyType == BRCM_PHY_TYPE_EXT_GMII)
	{
		intrl2->cpu_mask_clear |= UMAC_IRQ_LINK_DOWN | UMAC_IRQ_LINK_UP ;

	}

	/* Enable rx/tx engine.*/

	TRACE(("done init umac\n"));

    return 0;

}
/*
 * init_edma: Initialize DMA control register
 */
static void init_edma(BcmEnet_devctrl *pDevCtrl)
{
#ifdef CONFIG_BCMGENET_RX_DESC_THROTTLE
	int speeds[] = {10, 100, 1000, 2500};
	int speed_id = 1;
#endif
    TRACE(("bcmgenet: init_edma\n"));

	/* init rDma, disable it first while updating register */
	//pDevCtrl->rxDma->rdma_ctrl = 0;
	pDevCtrl->rxDma->rdma_scb_burst_size = DMA_MAX_BURST_LENGTH;
	/* by default, enable ring 16 (descriptor based) */
	pDevCtrl->rxDma->rDmaRings[DMA_RING_DESC_INDEX].rdma_write_pointer = 0;
	pDevCtrl->rxDma->rDmaRings[DMA_RING_DESC_INDEX].rdma_producer_index = 0;
	pDevCtrl->rxDma->rDmaRings[DMA_RING_DESC_INDEX].rdma_consumer_index = 0;
	pDevCtrl->rxDma->rDmaRings[DMA_RING_DESC_INDEX].rdma_ring_buf_size = (TOTAL_DESC << DMA_RING_SIZE_SHIFT) | RX_BUF_LENGTH;
	pDevCtrl->rxDma->rDmaRings[DMA_RING_DESC_INDEX].rdma_start_addr = 0;
	pDevCtrl->rxDma->rDmaRings[DMA_RING_DESC_INDEX].rdma_end_addr = 2*TOTAL_DESC -1;
	pDevCtrl->rxDma->rDmaRings[DMA_RING_DESC_INDEX].rdma_xon_xoff_threshold = (DMA_FC_THRESH_LO << DMA_XOFF_THRESHOLD_SHIFT) | DMA_FC_THRESH_HI;
	pDevCtrl->rxDma->rDmaRings[DMA_RING_DESC_INDEX].rdma_read_pointer = 0;

#ifdef  CONFIG_BCMGENET_RX_DESC_THROTTLE
	/* 
	 * Use descriptor throttle, fire interrupt only when multiple packets are done!
	 */
	pDevCtrl->rxDma->rDmaRings[DMA_RING_DESC_INDEX].rdma_mbuf_done_threshold = DMA_DESC_THRES;
	/* 
	 * Enable push timer, that is, force the IRQ_DESC_THROT to fire when timeout
	 * occcred, to prevent system slow reponse when handling low throughput data.
	 */
	speed_id = (pDevCtrl->umac->cmd >> CMD_SPEED_SHIFT) & CMD_SPEED_MASK;
	pDevCtrl->rxDma->rdma_timeout[DMA_RING_DESC_INDEX] = (2*(DMA_DESC_THRES*ENET_MAX_MTU_SIZE)/speeds[speed_id]) & DMA_TIMEOUT_MASK;
#endif	/* CONFIG_BCMGENET_RX_DESC_THROTTLE */


	/* Init tDma */
	//pDevCtrl->txDma->tdma_ctrl = 0;
	pDevCtrl->txDma->tdma_scb_burst_size = DMA_MAX_BURST_LENGTH;
	/* by default, enable ring DMA_RING_DESC_INDEX (descriptor based) */
	pDevCtrl->txDma->tDmaRings[DMA_RING_DESC_INDEX].tdma_read_pointer = 0;
	pDevCtrl->txDma->tDmaRings[DMA_RING_DESC_INDEX].tdma_producer_index = 0;
	pDevCtrl->txDma->tDmaRings[DMA_RING_DESC_INDEX].tdma_consumer_index = 0;
	pDevCtrl->txDma->tDmaRings[DMA_RING_DESC_INDEX].tdma_ring_buf_size = (TOTAL_DESC << DMA_RING_SIZE_SHIFT) | RX_BUF_LENGTH;
	pDevCtrl->txDma->tDmaRings[DMA_RING_DESC_INDEX].tdma_start_addr = 0;
	pDevCtrl->txDma->tDmaRings[DMA_RING_DESC_INDEX].tdma_end_addr = 2*TOTAL_DESC - 1;
	pDevCtrl->txDma->tDmaRings[DMA_RING_DESC_INDEX].tdma_mbuf_done_threshold = 0;
	/* Disable rate control for now */
	pDevCtrl->txDma->tDmaRings[DMA_RING_DESC_INDEX].tdma_flow_period = 0;
	pDevCtrl->txDma->tDmaRings[DMA_RING_DESC_INDEX].tdma_write_pointer = 0;
	
}
/*------------------------------------------------------------------------------------------------------------ 
 * exported function , Initialize ring buffer
 * dev: device pointer.
 * direction: 0 for rx 1 for tx.
 * index: ring index.
 * size: ring size, number of buffer in the ring, must be power of 2.
 * buf_len: buffer length, must be 32 bytes aligned, we assume 2Kb here.
 * buf: pointer to the buffer, continues memory, if *buf == NULL, buffer will be allocated by this function.
 *------------------------------------------------------------------------------------------------------------*/
int bcmgenet_init_ringbuf(struct net_device * dev, int direction, unsigned int index, unsigned int size, int buf_len, unsigned char ** buf)
{
	int speeds[] = {10, 100, 1000, 2500};
	int speed_id = 1;
	int i, dma_enable;
	dma_addr_t dma_start;
	Enet_CB * cb;
	unsigned long flags;
    BcmEnet_devctrl *pDevCtrl = netdev_priv(dev);

	if(index < 0 || index > 15 || size & (size - 1 ))
		return -EINVAL;

	spin_lock_irqsave(&pDevCtrl->lock, flags);
	if (direction == GENET_ALLOC_RX_RING)
	{
		if ( * buf == NULL)
		{
			*buf = kmalloc(size * buf_len, GFP_KERNEL);
			if (*buf == NULL)
				return -ENOMEM;
		}
		if((cb  = kmalloc(size*sizeof(Enet_CB), GFP_KERNEL)) == NULL)
			return -ENOMEM;
		
		pDevCtrl->rxRingCbs[index] = cb;
		pDevCtrl->rxRingSize[index] = size;
		pDevCtrl->rxRingCIndex[index] = 0;
		pDevCtrl->rxRingDiscCnt[index] = 0;
		
		for(i = 0; i < size; i++) {
			cb->skb = __bcmgenet_alloc_skb_from_buf(*buf + i*buf_len, buf_len, 0, 2);
			cb->BdAddr = (DmaDesc*)(*buf + i*buf_len);
			if(cb->skb == NULL) {
				printk(KERN_ERR "Failed to allocate skb for ring buffer %d\n", index);
				kfree(pDevCtrl->rxRingCbs[index]);
				return -ENOMEM;
			}
			cb++;
		}

		dma_enable = pDevCtrl->rxDma->rdma_ctrl & DMA_EN;
		pDevCtrl->rxDma->rdma_ctrl &= ~DMA_EN;
		pDevCtrl->rxDma->rDmaRings[index].rdma_producer_index = 0;
		pDevCtrl->rxDma->rDmaRings[index].rdma_consumer_index = 0;
		pDevCtrl->rxDma->rDmaRings[index].rdma_ring_buf_size = (size << DMA_RING_SIZE_SHIFT) | buf_len;
		dma_start = dma_map_single(&dev->dev, *buf, buf_len * size, DMA_FROM_DEVICE);
		pDevCtrl->rxDma->rDmaRings[index].rdma_start_addr = dma_start; 
		pDevCtrl->rxDma->rDmaRings[index].rdma_end_addr = dma_start + size * buf_len - 1;
		pDevCtrl->rxDma->rDmaRings[index].rdma_xon_xoff_threshold = (DMA_FC_THRESH_LO << DMA_XOFF_THRESHOLD_SHIFT) | DMA_FC_THRESH_HI;
		pDevCtrl->rxDma->rDmaRings[index].rdma_read_pointer = dma_start;
		pDevCtrl->rxDma->rDmaRings[index].rdma_write_pointer = dma_start;

		/* 
	 	* Use descriptor throttle, fire interrupt only when multiple packets are done!
	 	*/
		pDevCtrl->rxDma->rDmaRings[index].rdma_mbuf_done_threshold = DMA_DESC_THRES;
		/* 
	 	* Enable push timer, that is, force the IRQ_DESC_THROT to fire when timeout
	 	* occcred, to prevent system slow reponse when handling low throughput data.
	 	*/
		speed_id = (pDevCtrl->umac->cmd >> CMD_SPEED_SHIFT) & CMD_SPEED_MASK;
		/* set large pushtimer value to reduce interrupt rate */
		pDevCtrl->rxDma->rdma_timeout[index] = (16*2*(DMA_DESC_THRES*ENET_MAX_MTU_SIZE)/speeds[speed_id]) & DMA_TIMEOUT_MASK;

		/* Enable interrupt for this ring */
		pDevCtrl->intrl2_1->cpu_mask_clear |= ( 1<< (index + 16));
		pDevCtrl->rxDma->rdma_ctrl = (1 << (index + DMA_RING_BUF_EN_SHIFT));
		if(!(pDevCtrl->rbuf->rbuf_ctrl & RBUF_64B_EN))
			pDevCtrl->rbuf->rbuf_ctrl |= RBUF_64B_EN;
		if(dma_enable)
			pDevCtrl->rxDma->rdma_ctrl |= DMA_EN;
	}else {
		if((cb  = kmalloc(size*sizeof(Enet_CB), GFP_KERNEL)) == NULL)
			return -ENOMEM;
		pDevCtrl->txRingCBs[index] = cb;
		pDevCtrl->txRingSize[index] = size;
		pDevCtrl->txRingCIndex[index] = 0;
		
		dma_enable = pDevCtrl->txDma->tdma_ctrl & DMA_EN;
		pDevCtrl->txDma->tdma_ctrl &= ~DMA_EN;
		pDevCtrl->txDma->tDmaRings[index].tdma_producer_index = 0;
		pDevCtrl->txDma->tDmaRings[index].tdma_consumer_index = 0;
		pDevCtrl->txDma->tDmaRings[index].tdma_ring_buf_size = (size << DMA_RING_SIZE_SHIFT) | buf_len;
		dma_start = dma_map_single(&dev->dev, buf, buf_len * size, DMA_TO_DEVICE);
		pDevCtrl->txDma->tDmaRings[index].tdma_start_addr = dma_start; 
		pDevCtrl->txDma->tDmaRings[index].tdma_end_addr = dma_start + size * buf_len - 1;
		pDevCtrl->txDma->tDmaRings[index].tdma_flow_period = ENET_MAX_MTU_SIZE << 16;
		pDevCtrl->txDma->tDmaRings[index].tdma_read_pointer = dma_start;
		pDevCtrl->txDma->tDmaRings[index].tdma_write_pointer = dma_start;
		//if(!(pDevCtrl->rbuf->tbuf_ctrl & RBUF_64B_EN))
		if(!(GENET_TBUF_CTRL(pDevCtrl) & RBUF_64B_EN))
		{
			//pDevCtrl->rbuf->tbuf_ctrl |= RBUF_64B_EN;
			GENET_TBUF_CTRL(pDevCtrl) |= RBUF_64B_EN;
			if(dev->needed_headroom < 64)
				dev->needed_headroom += 64;
		}
		if(dma_enable)
			pDevCtrl->txDma->tdma_ctrl |= DMA_EN;
	}
	
	spin_unlock_irqrestore(&pDevCtrl->lock, flags);
	return 0;
}
/* 
 * bcmgenet_uninit_ringbuf : cleanup ring buffer
 * if "free" is non-zero , it will free the buffer.
 */
int bcmgenet_uninit_ringbuf(struct net_device * dev, int direction, unsigned int index, int free)
{
	int dma_enable, size, buflen, i;
	Enet_CB * cb;
	void * buf;
	unsigned long flags;
    BcmEnet_devctrl *pDevCtrl = netdev_priv(dev);
	
	if(index < 0 || index > 15 || size & (size - 1 ))
		return -EINVAL;

	spin_lock_irqsave(&pDevCtrl->lock, flags);
	if(direction == GENET_ALLOC_RX_RING)
	{
		size = pDevCtrl->rxDma->rDmaRings[index].rdma_ring_buf_size >> DMA_RING_SIZE_SHIFT; 
		buflen = pDevCtrl->rxDma->rDmaRings[index].rdma_ring_buf_size & 0xFFFF;
		/* Disble this ring first */
		dma_enable = pDevCtrl->rxDma->rdma_ctrl & DMA_EN;
		pDevCtrl->rxDma->rdma_ctrl &= ~DMA_EN;
		pDevCtrl->rxDma->rdma_ctrl &= ~(1 << (index + DMA_RING_BUF_EN_SHIFT));
		buf = (void*)phys_to_virt((dma_addr_t)pDevCtrl->rxDma->rDmaRings[index].rdma_start_addr);
		dma_unmap_single(&pDevCtrl->dev->dev, 
				pDevCtrl->rxDma->rDmaRings[index].rdma_start_addr,
				size * buflen,
				DMA_FROM_DEVICE);
		
		/*release resources */
		cb = pDevCtrl->rxRingCbs[index];
		
		for(i = 0; i < size; i++) {
			if(cb->skb != NULL) {
				if(!__bcmgenet_free_skb(cb->skb, 1)) {
					printk(KERN_WARNING "Failed to free skb (0x%08x)\n", (unsigned int)cb->skb->head);
				}
			}
			cb++;
		}
		kfree(pDevCtrl->rxRingCbs[index]);
		if(free) {
			kfree(buf);
		}
		if(dma_enable)
			pDevCtrl->rxDma->rdma_ctrl |= DMA_EN;
	}else 
	{
		size = pDevCtrl->txDma->tDmaRings[index].tdma_ring_buf_size >> DMA_RING_SIZE_SHIFT; 
		buflen = pDevCtrl->txDma->tDmaRings[index].tdma_ring_buf_size & 0xFFFF;
		dma_enable = pDevCtrl->txDma->tdma_ctrl & DMA_EN;
		pDevCtrl->txDma->tdma_ctrl &= ~DMA_EN;
		/* Disble this ring first */
		pDevCtrl->txDma->tdma_ctrl &= ~(1 << (index + DMA_RING_BUF_EN_SHIFT));
		dma_unmap_single(&pDevCtrl->dev->dev, 
				pDevCtrl->txDma->tDmaRings[index].tdma_start_addr,
				size * buflen,
				DMA_TO_DEVICE);
		
		/*release resources */
		cb = pDevCtrl->txRingCBs[index];
		kfree(cb);
		/* if all rings are disabled and tx csum offloading is off, disable TSB */
		if(!(pDevCtrl->txDma->tdma_ctrl & (0xFFFF << 1)) && !(dev->features & NETIF_F_IP_CSUM))
		{
			//pDevCtrl->rbuf->tbuf_ctrl &= ~RBUF_64B_EN;
			GENET_TBUF_CTRL(pDevCtrl) &= ~RBUF_64B_EN;
			if(dev->needed_headroom > 64)
				dev->needed_headroom -= 64;
		}
		if(dma_enable)
			pDevCtrl->rxDma->rdma_ctrl |= DMA_EN;
	}

	spin_unlock_irqrestore(&pDevCtrl->lock, flags);
	return 0;
}
/*
 * bcmgenet_init_dev: initialize uniMac devie
 * allocate Tx/Rx buffer descriptors pool, Tx control block pool.
 */
static int bcmgenet_init_dev(BcmEnet_devctrl *pDevCtrl)
{
    int i, ret;
    void *ptxCbs, *prxCbs;
	volatile DmaDesc * lastBd;

	TRACE(("%s\n", __FUNCTION__));
    /* setup buffer/pointer relationships here */
    pDevCtrl->nrTxBds = pDevCtrl->nrRxBds = TOTAL_DESC;
	/* Always use 2KB buffer for 7420*/
    pDevCtrl->rxBufLen = RX_BUF_LENGTH;

	/* register block locations */
	pDevCtrl->sys = (SysRegs *)(pDevCtrl->dev->base_addr);
	pDevCtrl->grb = (GrBridgeRegs *)(pDevCtrl->dev->base_addr + UMAC_GR_BRIDGE_REG_OFFSET);
	pDevCtrl->ext = (ExtRegs *)(pDevCtrl->dev->base_addr + UMAC_EXT_REG_OFFSET);
	pDevCtrl->intrl2_0 = (intrl2Regs *)(pDevCtrl->dev->base_addr + UMAC_INTRL2_0_REG_OFFSET);
	pDevCtrl->intrl2_1 = (intrl2Regs *)(pDevCtrl->dev->base_addr + UMAC_INTRL2_1_REG_OFFSET);
	pDevCtrl->rbuf = (rbufRegs *)(pDevCtrl->dev->base_addr + UMAC_RBUF_REG_OFFSET);
	pDevCtrl->umac = (uniMacRegs *)(pDevCtrl->dev->base_addr + UMAC_UMAC_REG_OFFSET);
	pDevCtrl->hfb = (unsigned long*)(pDevCtrl->dev->base_addr + UMAC_HFB_OFFSET);
	pDevCtrl->txDma = (tDmaRegs *)(pDevCtrl->dev->base_addr + UMAC_TDMA_REG_OFFSET + 2*TOTAL_DESC*sizeof(unsigned long));
	pDevCtrl->rxDma = (rDmaRegs *)(pDevCtrl->dev->base_addr + UMAC_RDMA_REG_OFFSET + 2*TOTAL_DESC*sizeof(unsigned long));
#ifdef CONFIG_BRCM_HAS_GENET2
	pDevCtrl->tbuf = (tbufRegs *)(pDevCtrl->dev->base_addr + UMAC_TBUF_REG_OFFSET);
	pDevCtrl->hfbReg = (hfbRegs *)(pDevCtrl->dev->base_addr + UMAC_HFB_REG_OFFSET);
#endif

    pDevCtrl->rxBds = (DmaDesc *) (pDevCtrl->dev->base_addr + UMAC_RDMA_REG_OFFSET);
    pDevCtrl->txBds = (DmaDesc *) (pDevCtrl->dev->base_addr + UMAC_TDMA_REG_OFFSET);
	TRACE(("%s: rxbds=0x%08x txbds=0x%08x\n", __FUNCTION__, (unsigned int)pDevCtrl->rxBds, (unsigned int)pDevCtrl->txBds));
	
    /* alloc space for the tx control block pool */
    if (!(ptxCbs = kmalloc(pDevCtrl->nrTxBds*sizeof(Enet_CB), GFP_KERNEL))) {
        return -ENOMEM;
    }
    memset(ptxCbs, 0, pDevCtrl->nrTxBds*sizeof(Enet_CB));
    pDevCtrl->txCbs = (Enet_CB *)ptxCbs;

    /* initialize rx ring pointer variables. */
    pDevCtrl->rxBdAssignPtr = pDevCtrl->rxBds;
    
	if (!(prxCbs = kmalloc(pDevCtrl->nrRxBds*sizeof(Enet_CB), GFP_KERNEL))) {
		ret = -ENOMEM;
		goto error2;
    }
    memset(prxCbs, 0, pDevCtrl->nrRxBds*sizeof(Enet_CB));
    pDevCtrl->rxCbs = (Enet_CB *)prxCbs;

    /* init the receive buffer descriptor ring */
    for (i = 0; i < pDevCtrl->nrRxBds; i++)
    {
        (pDevCtrl->rxBds + i)->length_status = (pDevCtrl->rxBufLen<<16);
        (pDevCtrl->rxBds + i)->address = 0;
    }
	lastBd = pDevCtrl->rxBds + pDevCtrl->nrRxBds - 1;

    /* clear the transmit buffer descriptors */
    for (i = 0; i < pDevCtrl->nrTxBds; i++)
    {
        (pDevCtrl->txBds + i)->length_status = 0<<16;
        (pDevCtrl->txBds + i)->address = 0;
    }
	lastBd = pDevCtrl->txBds + pDevCtrl->nrTxBds - 1;
    pDevCtrl->txFreeBds = pDevCtrl->nrTxBds;

    /* fill receive buffers */
	if(assign_rx_buffers(pDevCtrl) == 0)
	{
		printk(KERN_ERR "Failed to assign rx buffers\n");
		ret = -ENOMEM;
		goto error1;
	}

	TRACE(("%s done! \n", __FUNCTION__));
    /* init umac registers */
    if (init_umac(pDevCtrl)) {
		ret = -EFAULT;
		goto error1;
    }
    
	/* init dma registers */
    init_edma(pDevCtrl);
#if 0
	/* DEBUG code , init ring buffer # 0 */
	{
		unsigned char * buf = NULL;
		static unsigned int hfb[] = { 0x000F0010, 0x000F1888, 0x000FE28B};
		if(bcmgenet_update_hfb(pDevCtrl->dev, hfb, 3, 0) < 0)
			printk(KERN_ERR "Failed to program hfb\n");
		pDevCtrl->rbuf->rbuf_hfb_ctrl |= RBUF_HFB_EN;
		if( bcmgenet_init_ringbuf(pDevCtrl->dev, 0, 0, 512, RX_BUF_LENGTH, &buf) < 0)
			printk(KERN_ERR "Failed to init ring buffer\n");
		napi_enable(&pDevCtrl->ring_napi);
	}
#endif

	TRACE(("%s done! \n", __FUNCTION__));
    /* if we reach this point, we've init'ed successfully */
    return 0;
error1:
	kfree(prxCbs);
error2:
	kfree(ptxCbs);

	TRACE(("%s Failed!\n", __FUNCTION__));
	return ret;
}

/* Uninitialize tx/rx buffer descriptor pools */
static void bcmgenet_uninit_dev(BcmEnet_devctrl *pDevCtrl)
{
    int i;

    if (pDevCtrl) {
        /* disable DMA */
		pDevCtrl->rxDma->rdma_ctrl = 0;
		pDevCtrl->txDma->tdma_ctrl = 0;

		for ( i = 0; i < pDevCtrl->nrTxBds; i++)
		{
			if(pDevCtrl->txCbs[i].skb != NULL) {
				dev_kfree_skb(pDevCtrl->txCbs[i].skb);
				pDevCtrl->txCbs[i].skb = NULL;
			}
		}
		for ( i = 0; i < pDevCtrl->nrRxBds; i++)
		{
			if(pDevCtrl->rxCbs[i].skb != NULL) {
				dev_kfree_skb(pDevCtrl->rxCbs[i].skb);
				pDevCtrl->rxCbs[i].skb = NULL;
			}
		}

        /* free the transmit buffer descriptor */
        if (pDevCtrl->txBds) {
            pDevCtrl->txBds = NULL;
        }
        /* free the receive buffer descriptor */
        if (pDevCtrl->rxBds) {
            pDevCtrl->rxBds = NULL;
        }
        /* free the transmit control block pool */
        if (pDevCtrl->txCbs) {
            kfree(pDevCtrl->txCbs);
            pDevCtrl->txCbs = NULL;
        }
        /* free the transmit control block pool */
        if (pDevCtrl->txCbs) {
            kfree(pDevCtrl->rxCbs);
            pDevCtrl->rxCbs = NULL;
        }
    }
}
/*
 * Program ACPI pattern into HFB. Return filter index if succesful.
 * if user == 1, the data will be copied from user space.
 */
int bcmgenet_update_hfb(struct net_device *dev, unsigned int *data, int len, int user)
{
    BcmEnet_devctrl *pDevCtrl = netdev_priv(dev);
	//volatile rbufRegs * rbuf = pDevCtrl->rbuf;
	int filter, offset, count;
	unsigned int * tmp;
	
	TRACE(("Updating HFB len=0x%d\n", len));
	//if(rbuf->rbuf_hfb_ctrl & RBUF_HFB_256B) {
	if(GENET_HFB_CTRL(pDevCtrl) & RBUF_HFB_256B)
	{
		if (len > 256)
			return -EINVAL;
		count = 8;
		offset = 256;
	}else {
		if (len > 128)
			return -EINVAL;
		count = 16;
		offset = 128;
	}
	/* find next unused filter */
	for (filter = 0; filter < count; filter++) {
		//if(!((rbuf->rbuf_hfb_ctrl >> (filter + RBUF_HFB_FILTER_EN_SHIFT)) & 0X1))
		if(!((GENET_HFB_CTRL(pDevCtrl) >> (filter + RBUF_HFB_FILTER_EN_SHIFT)) & 0x01))
			break;
	}
	if(filter == count) {
		printk(KERN_ERR "no unused filter available!\n");
		return -EINVAL;	/* all filters have been enabled*/
	}
	
	if( user)
	{
		if((tmp = kmalloc(len*sizeof(unsigned int), GFP_KERNEL)) == NULL)
		{
			printk(KERN_ERR "%s: Malloc faild\n", __FUNCTION__);
			return -EFAULT;
		}
		/* copy pattern data */
		if(copy_from_user(tmp, data, len*sizeof(unsigned int)) != 0)
		{
			printk(KERN_ERR "Failed to copy user data: src=%p, dst=%p\n", 
				data, pDevCtrl->hfb + filter*offset);
			return -EFAULT;
		}
	}else {
		tmp = data;
	}
	/* Copy pattern data into HFB registers.*/
	for(count = 0; count < offset; count++)
	{
		if( count < len)
			pDevCtrl->hfb[filter * offset + count] = *(tmp + count);
		else
			pDevCtrl->hfb[filter * offset + count] = 0;
	}
	if(user)
	{
		kfree(tmp);
	}

	/* set the filter length*/
	//rbuf->rbuf_fltr_len[3-(filter>>2)] |= (len*2 << (RBUF_FLTR_LEN_SHIFT * (filter&0x03)) );
	GENET_HFB_FLTR_LEN(pDevCtrl, 3-(filter>>2)) |= (len*2 << (RBUF_FLTR_LEN_SHIFT * (filter&0x03)) );
	
	/*enable this filter.*/
	//rbuf->rbuf_hfb_ctrl |= (1 << (RBUF_HFB_FILTER_EN_SHIFT + filter));
	GENET_HFB_CTRL(pDevCtrl) |= (1 << (RBUF_HFB_FILTER_EN_SHIFT + filter));

	return filter;
	
}
/*
 * read ACPI pattern data for a particular filter.
 */
static int bcmgenet_read_hfb(struct net_device * dev, struct acpi_data * u_data)
{
	int filter, offset, count, len;
	BcmEnet_devctrl *pDevCtrl = netdev_priv(dev);
	//volatile rbufRegs *rbuf = pDevCtrl->rbuf;

	if(get_user(filter, &(u_data->fltr_index)) ) {
		printk(KERN_ERR "Failed to get user data\n");
		return -EFAULT;
	}
	
	//if(rbuf->rbuf_hfb_ctrl & RBUF_HFB_256B) {
	if(GENET_HFB_CTRL(pDevCtrl) & RBUF_HFB_256B){
		count = 8;
		offset = 256;
	}else {
		count = 16;
		offset = 128;
	}
	if (filter > count)
		return -EINVAL;
	
	/* see if this filter is enabled, if not, return length 0 */
	//if ((rbuf->rbuf_hfb_ctrl & (1 << (filter + RBUF_HFB_FILTER_EN_SHIFT)) ) == 0) {
	if ((GENET_HFB_CTRL(pDevCtrl) & (1 << (filter + RBUF_HFB_FILTER_EN_SHIFT)) ) == 0) {
		len = 0;
		put_user(len , &u_data->count);
		return 0;
	}
	/* check the filter length, in bytes */
	//len = RBUF_FLTR_LEN_MASK & (rbuf->rbuf_fltr_len[filter>>2] >> (RBUF_FLTR_LEN_SHIFT * (filter & 0x3)) );
	len = RBUF_FLTR_LEN_MASK & (GENET_HFB_FLTR_LEN(pDevCtrl, filter>>2) >> (RBUF_FLTR_LEN_SHIFT * (filter & 0x3)) );
	if( u_data->count < len)
		return -EINVAL;
	/* copy pattern data */
	if(copy_to_user((void*)(u_data->p_data), (void*)(pDevCtrl->hfb + filter*offset), len)) {
		printk(KERN_ERR "Failed to copy data to user space: src=%p, dst=%p\n", 
				pDevCtrl->hfb+filter*offset, u_data->p_data);
		return -EFAULT;
	}
	return len;
}
/*
 * clear the HFB, disable filter indexed by "filter" argument.
 */
static inline void bcmgenet_clear_hfb(BcmEnet_devctrl * pDevCtrl, int filter)
{
	int offset;

	//if(pDevCtrl->rbuf->rbuf_hfb_ctrl & RBUF_HFB_256B) {
	if(GENET_HFB_CTRL(pDevCtrl) & RBUF_HFB_256B) {
		offset = 256;
	}else {
		offset = 128;
	}
	if (filter == CLEAR_ALL_HFB)
	{
		//pDevCtrl->rbuf->rbuf_hfb_ctrl &= ~(0xffff << (RBUF_HFB_FILTER_EN_SHIFT));
		//pDevCtrl->rbuf->rbuf_hfb_ctrl &= ~RBUF_HFB_EN;
		GENET_HFB_CTRL(pDevCtrl) &= ~(0xffff << (RBUF_HFB_FILTER_EN_SHIFT));
		GENET_HFB_CTRL(pDevCtrl) &= ~RBUF_HFB_EN;
	}else 
	{
		/* disable this filter */
		//pDevCtrl->rbuf->rbuf_hfb_ctrl &= ~(1 << (RBUF_HFB_FILTER_EN_SHIFT + filter));
		GENET_HFB_CTRL(pDevCtrl) &= ~(1 << (RBUF_HFB_FILTER_EN_SHIFT + filter));
		/* clear filter length register */
		//pDevCtrl->rbuf->rbuf_fltr_len[3-(filter>>2)] &= ~(0xff << (RBUF_FLTR_LEN_SHIFT * (filter & 0x03)) );
		GENET_HFB_FLTR_LEN(pDevCtrl,(3-(filter>>2))) &= ~(0xff << (RBUF_FLTR_LEN_SHIFT * (filter & 0x03)) );
	}
	
}
/* 
 * Utility function to get interface ip address in kernel space.
 */
static inline unsigned int bcmgenet_getip(struct net_device * dev)
{
	struct net_device * pnet_device;
	unsigned int ip = 0;
	
	read_lock(&dev_base_lock);
	/* read all devices */
	for_each_netdev(&init_net, pnet_device)
	{
		if((netif_running(pnet_device)) &&
				(pnet_device->ip_ptr != NULL) &&
				(!strcmp(pnet_device->name, dev->name)) )
		{
			struct in_device * pin_dev;
			pin_dev = (struct in_device *)(pnet_device->ip_ptr);
			ip = htonl(pin_dev->ifa_list->ifa_address);
			break;
		}
	}
	read_unlock(&dev_base_lock);
	return ip;
}				
	
/* 
 * ethtool function - get WOL (Wake on LAN) settings, 
 * Only Magic Packet Detection is supported through ethtool,
 * the ACPI (Pattern Matching) WOL option is supported in
 * bcmumac_do_ioctl function.
 */
static void bcmgenet_get_wol(struct net_device *dev, struct ethtool_wolinfo *wol)
{
	BcmEnet_devctrl * pDevCtrl = netdev_priv(dev);
	volatile uniMacRegs * umac = pDevCtrl->umac;
	wol->supported = WAKE_MAGIC | WAKE_MAGICSECURE;
	
	if(umac->mpd_ctrl & MPD_EN)
		wol->wolopts = WAKE_MAGIC;
	if(umac->mpd_ctrl & MPD_PW_EN)
	{
		unsigned short pwd_ms;
		unsigned long pwd_ls;
		wol->wolopts |= WAKE_MAGICSECURE;
		pwd_ls = umac->mpd_pw_ls;
		copy_to_user(&wol->sopass[0], &pwd_ls, 4);
		pwd_ms = umac->mpd_pw_ms & 0xFFFF;
		copy_to_user(&wol->sopass[4], &pwd_ms, 2);
	}else {
		memset(&wol->sopass[0], 0, sizeof(wol->sopass));
	}
}
/*
 * ethtool function - set WOL (Wake on LAN) settings.
 * Only for magic packet detection mode.
 */
static int bcmgenet_set_wol(struct net_device *dev, struct ethtool_wolinfo *wol)
{
	BcmEnet_devctrl * pDevCtrl = netdev_priv(dev);
	volatile uniMacRegs * umac = pDevCtrl->umac;
	unsigned int ip;

	if(wol->wolopts & ~(WAKE_MAGIC | WAKE_MAGICSECURE))
		return -EINVAL;

	if((ip = bcmgenet_getip(dev)) == 0)
	{
		printk("IP address is not set, can't put in WoL mode\n");
		return -EINVAL;
	}else {
		hfb_arp[HFB_ARP_LEN-2] |= (ip >> 16);
		hfb_arp[HFB_ARP_LEN-1] |= (ip & 0xFFFF);
		/* Enable HFB, to response to ARP request.*/
		if(bcmgenet_update_hfb(dev, hfb_arp, HFB_ARP_LEN, 0) < 0)
		{
			printk(KERN_ERR "%s: Unable to update HFB\n", __FUNCTION__);
			return -EFAULT;
		} 
		//pDevCtrl->rbuf->rbuf_hfb_ctrl |= RBUF_HFB_EN;
		GENET_HFB_CTRL(pDevCtrl) |= RBUF_HFB_EN;
	}
	if(wol->wolopts & WAKE_MAGICSECURE)
	{
		umac->mpd_pw_ls = *(unsigned long *)&wol->sopass[0];
		umac->mpd_pw_ms = *(unsigned short*)&wol->sopass[4];
		umac->mpd_ctrl |= MPD_PW_EN;
	}
	if(wol->wolopts & WAKE_MAGIC)
	{
		/* Power down the umac, with magic packet mode.*/
		bcmgenet_power_down(pDevCtrl, GENET_POWER_WOL_MAGIC);
	}
	return 0;
}
/*
 * ethtool function - get generic settings.
 */
static int bcmgenet_get_settings(struct net_device *dev, struct ethtool_cmd *cmd)
{
	BcmEnet_devctrl * pDevCtrl = netdev_priv(dev);
	return mii_ethtool_gset(&pDevCtrl->mii, cmd);
}
/*
 * ethtool function - set settings.
 */
static int bcmgenet_set_settings(struct net_device *dev, struct ethtool_cmd *cmd)
{
	int err = 0;
	BcmEnet_devctrl * pDevCtrl = netdev_priv(dev);

	if((err = mii_ethtool_sset(&pDevCtrl->mii, cmd)) < 0)
		return err;
	mii_setup(dev);
	
	if(cmd->maxrxpkt != 0)
		DmaDescThres = cmd->maxrxpkt;

	return err;
}
/*
 * ethtool function - get driver info.
 */
static void bcmgenet_get_drvinfo(struct net_device *dev, struct ethtool_drvinfo *info)
{
	strncpy(info->driver, CARDNAME, sizeof(info->driver));
	strncpy(info->version, VER_STR, sizeof(info->version));
	
}
static u32 bcmgenet_get_rx_csum(struct net_device * dev)
{
    BcmEnet_devctrl *pDevCtrl = netdev_priv(dev);
	if(pDevCtrl->rbuf->rbuf_chk_ctrl & RBUF_RXCHK_EN)
		return 1;
	
	return 0;
}
static int bcmgenet_set_rx_csum(struct net_device * dev, u32 val)
{
    BcmEnet_devctrl *pDevCtrl = netdev_priv(dev);
	spin_lock_bh(&pDevCtrl->bh_lock);
	if( val == 0)
	{
		//pDevCtrl->rbuf->rbuf_endian_ctrl &= ~RBUF_ENDIAN_NOSWAP;
		pDevCtrl->rbuf->rbuf_ctrl &= ~RBUF_64B_EN;
		pDevCtrl->rbuf->rbuf_chk_ctrl &= ~RBUF_RXCHK_EN;
	}else {
		//pDevCtrl->rbuf->rbuf_endian_ctrl &= ~RBUF_ENDIAN_NOSWAP;
		pDevCtrl->rbuf->rbuf_ctrl |= RBUF_64B_EN;
		pDevCtrl->rbuf->rbuf_chk_ctrl |= RBUF_RXCHK_EN ;
	}
	spin_unlock_bh(&pDevCtrl->bh_lock);
	return 0;
}
static u32 bcmgenet_get_tx_csum(struct net_device * dev)
{
	return dev->features & NETIF_F_IP_CSUM;
}
static int bcmgenet_set_tx_csum(struct net_device * dev, u32 val)
{
    unsigned long flags;
    BcmEnet_devctrl *pDevCtrl = netdev_priv(dev);
    spin_lock_irqsave(&pDevCtrl->lock, flags);
	if(val == 0) {
		dev->features &= ~NETIF_F_IP_CSUM;
		//pDevCtrl->rbuf->tbuf_ctrl &= ~RBUF_64B_EN;
		GENET_TBUF_CTRL(pDevCtrl) &= ~RBUF_64B_EN;
		if(dev->needed_headroom > 64)
			dev->needed_headroom -= 64;
	}else {
		dev->features |= NETIF_F_IP_CSUM;
		//pDevCtrl->rbuf->tbuf_ctrl |= RBUF_64B_EN;
		GENET_TBUF_CTRL(pDevCtrl) |= RBUF_64B_EN;
		if(dev->needed_headroom < 64)
			dev->needed_headroom += 64;
	}
	spin_unlock_irqrestore(&pDevCtrl->lock, flags);
	return 0;
}
static int bcmgenet_set_sg(struct net_device * dev, u32 val)
{
    unsigned long flags;
    BcmEnet_devctrl *pDevCtrl = netdev_priv(dev);
	if(val && !(dev->features & NETIF_F_IP_CSUM)) {
		printk(KERN_WARNING "Tx Checksum offloading disabled, not setting SG\n");
		return -EINVAL;
	}
	spin_lock_irqsave(&pDevCtrl->lock, flags);
	if(val)
		dev->features |= NETIF_F_SG;
	else
		dev->features &= ~NETIF_F_SG;
	/* must have 64B tx status enabled */
	spin_unlock_irqrestore(&pDevCtrl->lock, flags);
	return 0;
}
static u32 bcmgenet_get_sg(struct net_device * dev)
{
	return dev->features & NETIF_F_SG ;
}
/* 
 * standard ethtool support functions.
 */
static struct ethtool_ops bcmgenet_ethtool_ops = {
	.get_settings		= bcmgenet_get_settings,
	.set_settings		= bcmgenet_set_settings,
	.get_drvinfo		= bcmgenet_get_drvinfo,
	.get_wol			= bcmgenet_get_wol,
	.set_wol			= bcmgenet_set_wol,
	.get_rx_csum		= bcmgenet_get_rx_csum,
	.set_rx_csum		= bcmgenet_set_rx_csum,
	.get_tx_csum		= bcmgenet_get_tx_csum,
	.set_tx_csum		= bcmgenet_set_tx_csum,
	.get_sg				= bcmgenet_get_sg,
	.set_sg				= bcmgenet_set_sg,
	.get_link			= ethtool_op_get_link,
};
/*
 * disable clocks according to mode, cable sense/WoL
 */
static void bcmgenet_disable_clocks(BcmEnet_devctrl * pDevCtrl, int mode)
{
	/*TODO: move it to global PM lib */
}
/*
 * Enable clocks.
 */
static void bcmgenet_enable_clocks(BcmEnet_devctrl * pDevCtrl, int mode)
{
	/*TODO: move it to global PM lib */
}
/*
 * Power down the unimac, based on mode.
 */
static void bcmgenet_power_down(BcmEnet_devctrl *pDevCtrl, int mode)
{
	int retries = 0;

	switch(mode) {
		case GENET_POWER_CABLE_SENSE:
			/* PHY bug , disble DLL only for now */
			pDevCtrl->ext->ext_pwr_mgmt |= EXT_PWR_DOWN_DLL;
			//	EXT_PWR_DOWN_PHY ;
			//pDevCtrl->rbuf->rgmii_oob_ctrl &= ~RGMII_MODE_EN;
			GENET_RGMII_OOB_CTRL(pDevCtrl) &= ~RGMII_MODE_EN;
			bcmgenet_disable_clocks(pDevCtrl, mode);
			break;
		case GENET_POWER_WOL_MAGIC:
			/* ENable CRC forward */
			pDevCtrl->umac->cmd |= CMD_CRC_FWD;
			pDevCtrl->umac->mpd_ctrl |= MPD_EN;
			while(!(pDevCtrl->rbuf->rbuf_status & RBUF_STATUS_WOL)) {
				retries++;
				if(retries > 5) {
					printk(KERN_CRIT "bcmumac_power_down polling wol mode timeout\n");
					pDevCtrl->umac->mpd_ctrl &= ~MPD_EN;
					return;
				}
				udelay(100);
			}
			/* Service Rx BD untill empty */
			bcmgenet_disable_clocks(pDevCtrl, mode);
			pDevCtrl->intrl2_0->cpu_mask_clear |= UMAC_IRQ_MPD_R;
			pDevCtrl->intrl2_0->cpu_mask_clear |= UMAC_IRQ_HFB_MM | UMAC_IRQ_HFB_SM;
			break;
		case GENET_POWER_WOL_ACPI:
			//pDevCtrl->rbuf->rbuf_hfb_ctrl |= RBUF_ACPI_EN;
			GENET_HFB_CTRL(pDevCtrl) |= RBUF_ACPI_EN;
			while(!(pDevCtrl->rbuf->rbuf_status & RBUF_STATUS_WOL)) {
				retries++;
				if(retries > 5) {
					printk(KERN_CRIT "bcmumac_power_down polling wol mode timeout\n");
					//pDevCtrl->rbuf->rbuf_hfb_ctrl &= ~RBUF_ACPI_EN;
					GENET_HFB_CTRL(pDevCtrl) &= ~RBUF_ACPI_EN;
					return;
				}
				udelay(100);
			}
			/* Service RX BD untill empty */
			bcmgenet_disable_clocks(pDevCtrl, mode);
			pDevCtrl->intrl2_0->cpu_mask_clear |= UMAC_IRQ_HFB_MM | UMAC_IRQ_HFB_SM;
			break;
		default:
			break;
	}
	
}
static void bcmgenet_power_up(BcmEnet_devctrl *pDevCtrl, int mode)
{
	switch(mode) {
		case GENET_POWER_CABLE_SENSE:
			pDevCtrl->ext->ext_pwr_mgmt &= ~EXT_PWR_DOWN_DLL;
			pDevCtrl->ext->ext_pwr_mgmt &= ~EXT_PWR_DOWN_PHY;
			pDevCtrl->ext->ext_pwr_mgmt &= ~EXT_PWR_DOWN_BIAS;
			pDevCtrl->ext->ext_pwr_mgmt |= EXT_PHY_RESET;
			udelay(5);
			pDevCtrl->ext->ext_pwr_mgmt &= ~EXT_PHY_RESET;
			//bcmgenet_enable_clocks(pDevCtrl, mode);
			break;
		case GENET_POWER_WOL_MAGIC:
			pDevCtrl->umac->mpd_ctrl &= ~MPD_EN;
			/* 
			 * If ACPI is enabled at the same time, disable it, since 
			 * we have been waken up.
			 */
			//if( !(pDevCtrl->rbuf->rbuf_hfb_ctrl & RBUF_ACPI_EN))
			if( !(GENET_HFB_CTRL(pDevCtrl) & RBUF_ACPI_EN))
			{
				//pDevCtrl->rbuf->rbuf_hfb_ctrl &= RBUF_ACPI_EN;
				GENET_HFB_CTRL(pDevCtrl) &= RBUF_ACPI_EN;
				bcmgenet_enable_clocks(pDevCtrl, GENET_POWER_WOL_ACPI);
				/* Stop monitoring ACPI interrupts */
				pDevCtrl->intrl2_0->cpu_mask_set |= (UMAC_IRQ_HFB_SM | UMAC_IRQ_HFB_MM);
			}
			bcmgenet_clear_hfb(pDevCtrl, CLEAR_ALL_HFB);
			//bcmgenet_enable_clocks(pDevCtrl, mode);
			break;
		case GENET_POWER_WOL_ACPI:
			//pDevCtrl->rbuf->rbuf_hfb_ctrl &= ~RBUF_ACPI_EN;
			GENET_HFB_CTRL(pDevCtrl) &= ~RBUF_ACPI_EN;
			/* 
			 * If Magic packet is enabled at the same time, disable it, 
			 */
			if(!(pDevCtrl->umac->mpd_ctrl & MPD_EN))
			{
				pDevCtrl->umac->mpd_ctrl &= ~MPD_EN;
				bcmgenet_enable_clocks(pDevCtrl, GENET_POWER_WOL_ACPI);
				/* stop monitoring magic packet interrupt and disable crc forward */
				pDevCtrl->intrl2_0->cpu_mask_set |= UMAC_IRQ_MPD_R;
				pDevCtrl->umac->cmd &= ~CMD_CRC_FWD;
			}
			bcmgenet_clear_hfb(pDevCtrl, CLEAR_ALL_HFB);
			//bcmgenet_enable_clocks(pDevCtrl,mode); 
			break;
		default:
			break;
	}
}
/* 
 * ioctl handle special commands that are not present in ethtool.
 */
static int bcmgenet_ioctl(struct net_device *dev, struct ifreq *rq, int cmd)
{
    BcmEnet_devctrl *pDevCtrl = netdev_priv(dev);
    unsigned long flags;
    struct acpi_data *u_data;
    int val = 0;

    /* we can add sub-command in ifr_data if we need to in the future */
    switch (cmd)
    {
	case SIOCSACPISET:
		spin_lock_irqsave(&pDevCtrl->lock, flags);
		bcmgenet_power_down(pDevCtrl, GENET_POWER_WOL_ACPI);
		spin_unlock_irqrestore(&pDevCtrl->lock, flags);
		break;
	case SIOCSACPICANCEL:
		spin_lock_irqsave(&pDevCtrl->lock, flags);
		bcmgenet_power_up(pDevCtrl, GENET_POWER_WOL_ACPI);
		spin_unlock_irqrestore(&pDevCtrl->lock, flags);
		break;
	case SIOCSPATTERN:
		u_data = (struct acpi_data *)rq->ifr_data;
		val =  bcmgenet_update_hfb(dev, (unsigned int *)u_data->p_data, u_data->count,1 );
		if(val >= 0)
			put_user(val, &u_data->fltr_index);
		break;
	case SIOCGPATTERN:
		u_data = (struct acpi_data *)rq->ifr_data;
		val = bcmgenet_read_hfb(dev, u_data);
		break;
	default:
		val = -EINVAL;
		break;
    }

    return val;       
}
static const struct net_device_ops bcmgenet_netdev_ops = {
	.ndo_open = bcmgenet_open,
	.ndo_stop = bcmgenet_close,
	.ndo_start_xmit = bcmgenet_xmit,
#ifdef B_HAS_GENET_TX_MULTI_QUEUES
	.ndo_select_queue = bcmgenet_select_queue,
#endif
	.ndo_tx_timeout = bcmgenet_timeout,
	.ndo_set_multicast_list = bcmgenet_set_multicast_list,
	.ndo_set_mac_address = bcmgenet_set_mac_addr,
	.ndo_do_ioctl = bcmgenet_ioctl,
};
static int bcmgenet_drv_probe(struct platform_device *pdev)
{
	struct resource *mres, *ires;
	void __iomem *base;
	unsigned long res_size;
	int err;
	/*
	 * bcmemac and bcmgenet use same platform data structure.
	 */
	struct bcmemac_platform_data *cfg = pdev->dev.platform_data;
	BcmEnet_devctrl *pDevCtrl;
	struct net_device *dev;

	mres = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	ires = platform_get_resource(pdev, IORESOURCE_IRQ, 0);
	if(! mres || ! ires) {
		printk(KERN_ERR "%s: can't get resources\n", __FUNCTION__);
		return(-EIO);
	}
	res_size = mres->end - mres->start + 1;
	if(!request_mem_region(mres->start, res_size, CARDNAME))
	{
		printk(KERN_ERR "%s: can't request mem region: start: 0x%x size: %lu\n", 
				CARDNAME, mres->start, res_size);
		return -ENODEV;
	}
	base = ioremap(mres->start, mres->end - mres->start + 1);
	TRACE(("%s: base=0x%x\n", __FUNCTION__, (unsigned int)base));

	if(! base) {
		printk(KERN_ERR "%s: can't ioremap\n", __FUNCTION__);
		return(-EIO);
	}

#ifdef B_HAS_GENET_TX_MULTI_QUEUES 
	dev = alloc_etherdev_mq(sizeof(*(pDevCtrl)), 16);
#else
	dev = alloc_etherdev(sizeof(*pDevCtrl));
#endif
	if(dev == NULL)
	{
		printk(KERN_ERR "bcmgenet: can't allocate net device\n");
		err = -ENOMEM;
		goto err0;
	}
	dev->base_addr = (unsigned long)base;
	pDevCtrl = (BcmEnet_devctrl *)netdev_priv(dev);
	dev_set_drvdata(&pdev->dev, pDevCtrl);
	memcpy(dev->dev_addr, cfg->macaddr, 6);
	dev->irq = pDevCtrl->irq0;
	dev->watchdog_timeo         = 2*HZ;
	SET_ETHTOOL_OPS(dev, &bcmgenet_ethtool_ops);
	dev->netdev_ops = &bcmgenet_netdev_ops;
	netif_napi_add(dev, &pDevCtrl->napi, bcmgenet_poll, 64);
	netif_napi_add(dev, &pDevCtrl->ring_napi, bcmgenet_ring_poll, 64);
	
	// Let boot setup info override default settings.
	netdev_boot_setup_check(dev);

	pDevCtrl->dev = dev;
	pDevCtrl->irq0 = platform_get_irq(pdev, 0);
	pDevCtrl->irq1 = platform_get_irq(pdev, 1);
	pDevCtrl->devnum = pdev->id;
	pDevCtrl->bIPHdrOptimize = 1;
	
	spin_lock_init(&pDevCtrl->lock);
	spin_lock_init(&pDevCtrl->bh_lock);
	mutex_init(&pDevCtrl->mdio_mutex);
	/* Mii wait queue */
	init_waitqueue_head(&pDevCtrl->wq);
	
	pDevCtrl->phyType = cfg->phy_type;
	
	/* Init GENET registers, Tx/Rx buffers */
	if(bcmgenet_init_dev(pDevCtrl) < 0)
	{
		goto err1;
	}
	
	if(cfg->phy_id == BRCM_PHY_ID_AUTO)
	{
		if( mii_probe(dev, cfg->phy_id) < 0)
		{
			printk(KERN_ERR "No PHY detected, not registering interface:%d\n", pdev->id);
			goto err1;
		}else {
			printk(KERN_CRIT "Found PHY at Address %d\n", pDevCtrl->phyAddr);
		}
	}else {
		pDevCtrl->phyAddr = cfg->phy_id;
	}
	mii_init(dev);

	INIT_WORK(&pDevCtrl->bcmgenet_irq_work, bcmgenet_irq_task);

	if(request_irq(pDevCtrl->irq0, bcmgenet_isr0, IRQF_SHARED, dev->name, pDevCtrl) < 0)
	{
		printk(KERN_ERR "can't request IRQ %d\n", pDevCtrl->irq0);
		goto err2;
	}
	if (request_irq(pDevCtrl->irq1, bcmgenet_isr1, IRQF_SHARED, dev->name, pDevCtrl) < 0)
	{
		printk(KERN_ERR "can't request IRQ %d\n", pDevCtrl->irq1);
		free_irq(pDevCtrl->irq0, pDevCtrl);
		goto err2;
	}
	netif_carrier_off(pDevCtrl->dev);

	if(pDevCtrl->phyType == BRCM_PHY_TYPE_EXT_MII ||
		pDevCtrl->phyType == BRCM_PHY_TYPE_EXT_GMII ||
		pDevCtrl->phyType == BRCM_PHY_TYPE_EXT_GMII_IBS)
	{
		INIT_WORK(&pDevCtrl->bcmgenet_link_work, bcmgenet_gphy_link_status);
		init_timer(&pDevCtrl->timer);
		pDevCtrl->timer.data = (unsigned long)pDevCtrl;
		pDevCtrl->timer.function = bcmgenet_gphy_link_timer;
	}else {
		/* check link status */
		mii_setup(dev);
	}
#ifdef B_HAS_GENET_TX_SG
	dev->features |= NETIF_F_SG | NETIF_F_IP_CSUM;
#endif
	
	if((err = register_netdev(dev)) != 0)
	{
		goto err2;
	}
	/* Turn off these features by default */
	bcmgenet_set_tx_csum(dev, 0);
	bcmgenet_set_sg(dev, 0);
	
	return(0);

err2:
	bcmgenet_uninit_dev(pDevCtrl);
err1:
	iounmap(base);
	free_netdev(dev);
err0:
	release_mem_region(mres->start, res_size);
	return(err);
}

static int bcmgenet_drv_remove(struct platform_device *pdev)
{
	BcmEnet_devctrl *pDevCtrl = dev_get_drvdata(&pdev->dev);

	unregister_netdev(pDevCtrl->dev);
	free_irq(pDevCtrl->irq0, pDevCtrl);
	free_irq(pDevCtrl->irq1, pDevCtrl);
	bcmgenet_uninit_dev(pDevCtrl);
	iounmap((void __iomem *)pDevCtrl->base_addr);
	free_netdev(pDevCtrl->dev);
	/*TODO : release mem region*/
	return(0);
}

static struct platform_driver bcmgenet_plat_drv = {
	.probe =		bcmgenet_drv_probe,
	.remove =		bcmgenet_drv_remove,
	.driver = {
		.name =		"bcmgenet",
		.owner =	THIS_MODULE,
	},
};

static int bcmgenet_module_init(void)
{
	genet_skb_cache = kmem_cache_create("genet_skb_cache", 
			sizeof(struct sk_buff), 0, SLAB_HWCACHE_ALIGN|SLAB_PANIC,
			NULL);
	if(genet_skb_cache == NULL) 
		printk(KERN_ERR "Failed to create cache\n");
	
	platform_driver_register(&bcmgenet_plat_drv);
	return(0);
}

static void bcmgenet_module_cleanup(void)
{
	kmem_cache_destroy(genet_skb_cache);
	platform_driver_unregister(&bcmgenet_plat_drv);
}

module_init(bcmgenet_module_init);
module_exit(bcmgenet_module_cleanup);

MODULE_LICENSE("GPL");
EXPORT_SYMBOL(bcmemac_get_device);
EXPORT_SYMBOL(bcmemac_get_free_txdesc);
EXPORT_SYMBOL(bcmemac_xmit_check);
EXPORT_SYMBOL(bcmemac_xmit_fragment);
EXPORT_SYMBOL(bcmemac_xmit_multibuf);
EXPORT_SYMBOL(bcmgenet_init_ringbuf);
EXPORT_SYMBOL(bcmgenet_uninit_ringbuf);
EXPORT_SYMBOL(bcmgenet_update_hfb);
EXPORT_SYMBOL(bcmgenet_alloc_txring_skb);
