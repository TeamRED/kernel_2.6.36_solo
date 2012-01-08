/*
 *
 * Copyright (c) 2002-2005 Broadcom Corporation 
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
// File Name  : bcmmii.c
//
// Description: Broadcom PHY/GPHY/Ethernet Switch Configuration
// Revision:	09/25/2008, L.Sun created.
//               
//**************************************************************************

#include <linux/types.h>
#include <linux/delay.h>
#include <linux/wait.h>
#include <linux/mii.h>
#include <linux/ethtool.h>
#include <asm/brcmstb/brcmstb.h>

#include "bcmgenet_map.h"
#include "bcmgenet.h"
#include "bcmmii.h"

/* read a value from the MII */
int mii_read(struct net_device *dev, int phy_id, int location) 
{
	int ret;
    BcmEnet_devctrl *pDevCtrl = netdev_priv(dev);
    volatile uniMacRegs *umac = pDevCtrl->umac;

	if(phy_id == BRCM_PHY_ID_NONE)
		return location == MII_BMSR ? 0x782d : 0;
	
	mutex_lock(&pDevCtrl->mdio_mutex);
	
   	umac->mdio_cmd = MDIO_RD | (phy_id << MDIO_PMD_SHIFT) | (location << MDIO_REG_SHIFT);
	/* Start MDIO transaction*/
	umac->mdio_cmd |= MDIO_START_BUSY;
	wait_event_timeout(pDevCtrl->wq, !(umac->mdio_cmd & MDIO_START_BUSY) , HZ/100);
	mutex_unlock(&pDevCtrl->mdio_mutex);
	ret = umac->mdio_cmd;
	if (ret & MDIO_READ_FAIL)
	{
		TRACE(("MDIO read failure\n"));
		ret = 0;
	}
    return ret & 0xffff;
}

/* write a value to the MII */
void mii_write(struct net_device *dev, int phy_id, int location, int val)
{
    BcmEnet_devctrl *pDevCtrl = netdev_priv(dev);
    volatile uniMacRegs *umac = pDevCtrl->umac;

	if(phy_id == BRCM_PHY_ID_NONE)
		return;
	mutex_lock(&pDevCtrl->mdio_mutex);
   	umac->mdio_cmd = MDIO_WR | (phy_id << MDIO_PMD_SHIFT ) | (location << MDIO_REG_SHIFT) | (0xffff & val);
	umac->mdio_cmd |= MDIO_START_BUSY;
	wait_event_timeout(pDevCtrl->wq, !(umac->mdio_cmd & MDIO_START_BUSY), HZ/100);
	mutex_unlock(&pDevCtrl->mdio_mutex);
}
/* probe for an external PHY via MDIO; return PHY address */
int mii_probe(struct net_device * dev, int phy_id)
{
	BcmEnet_devctrl * pDevCtrl = netdev_priv(dev);
	int i;

	if(phy_id == BRCM_PHY_ID_AUTO)
	{
		/* 
	 	 * Enable RGMII to interface external PHY, disable internal 10/100 MII.
	 	 */
		GENET_RGMII_OOB_CTRL(pDevCtrl) |= RGMII_MODE_EN;
		/* Power down EPHY */
		pDevCtrl->ext->ext_pwr_mgmt |= (EXT_PWR_DOWN_PHY | EXT_PWR_DOWN_DLL | EXT_PWR_DOWN_BIAS);

		for (i = 0; i < 32; i++) {
			if( mii_read(dev, i, MII_BMSR) != 0)
			{
				pDevCtrl->phyAddr = i;
#if defined(CONFIG_BCMGENET_0_GPHY)
				if(i == 1) continue;
#endif
				return 0;
			}
			TRACE(("I=%d\n", i));
		}
		return -ENODEV;
	}
	return 0;
}

/*
 * restart auto-negotiation, config UMAC and RGMII block
 */
void mii_setup(struct net_device *dev)
{
    BcmEnet_devctrl *pDevCtrl = netdev_priv(dev);
	struct ethtool_cmd ecmd ;
	volatile uniMacRegs * umac = pDevCtrl->umac;

	TRACE(("%s: %s\n", __FUNCTION__, netif_carrier_ok(pDevCtrl->dev)? "netif_carrier_on":"netif_carrier_off"));
	if(pDevCtrl->phyType == BRCM_PHY_TYPE_MOCA)
	{
		/* MoCA case */
		netif_carrier_on(pDevCtrl->dev);
		pDevCtrl->dev->flags |= IFF_RUNNING;
		return ;
	}
	mii_ethtool_gset(&pDevCtrl->mii, &ecmd);

	if(mii_link_ok(&pDevCtrl->mii)&& !netif_carrier_ok(pDevCtrl->dev))
	{
		printk(KERN_INFO "%s: Link is up, %d Mbps %s Duplex\n",
			pDevCtrl->dev->name, 
			ecmd.speed,
			ecmd.duplex == DUPLEX_FULL ? "Full":"Half");
	}else if (!mii_link_ok(&pDevCtrl->mii) && netif_carrier_ok(pDevCtrl->dev))
	{
		printk(KERN_INFO "%s: Link is down\n", pDevCtrl->dev->name);
		return;
	}

	mii_check_link(&pDevCtrl->mii);
	/*
	 * program UMAC and RGMII block accordingly, if the PHY is 
	 * not capable of in-band signaling.
	 */
	if(pDevCtrl->phyType != BRCM_PHY_TYPE_EXT_GMII_IBS)
	{
		GENET_RGMII_OOB_CTRL(pDevCtrl) &= ~OOB_DISABLE;
		GENET_RGMII_OOB_CTRL(pDevCtrl) |= RGMII_LINK;
		if(ecmd.duplex == DUPLEX_FULL)
			umac->cmd &= ~CMD_HD_EN;
		else
			umac->cmd |= CMD_HD_EN;
		/* speed */
		umac->cmd = umac->cmd & ~(CMD_SPEED_MASK << CMD_SPEED_SHIFT);
		if(ecmd.speed == SPEED_10)
			umac->cmd = umac->cmd  | (UMAC_SPEED_10 << CMD_SPEED_SHIFT);
		else if (ecmd.speed == SPEED_100)
			umac->cmd = umac->cmd  | (UMAC_SPEED_100 << CMD_SPEED_SHIFT);
		else if (ecmd.speed == SPEED_1000)
			umac->cmd = umac->cmd  | (UMAC_SPEED_1000 << CMD_SPEED_SHIFT);
	}
	/* pause capability */
	if(pDevCtrl->phyType == BRCM_PHY_TYPE_INT ||
			pDevCtrl->phyType == BRCM_PHY_TYPE_EXT_MII)
	{
		unsigned int val;
		val = mii_read(dev, pDevCtrl->phyAddr, MII_LPA);
		if(!(val & LPA_PAUSE_CAP))
		{
			umac->cmd |= CMD_RX_PAUSE_IGNORE;
			umac->cmd |= CMD_TX_PAUSE_IGNORE;
		}
	}else if (pDevCtrl->phyType == BRCM_PHY_TYPE_EXT_GMII ||
			pDevCtrl->phyType == BRCM_PHY_TYPE_EXT_GMII_IBS)
	{
		unsigned int val;
    	val = mii_read(dev, pDevCtrl->phyAddr, MII_BRCM_AUX_STAT_SUM);
		if(!(val & MII_BRCM_AUX_GPHY_RX_PAUSE))
			umac->cmd |= CMD_RX_PAUSE_IGNORE;
		if(!(val & MII_BRCM_AUX_GPHY_TX_PAUSE))
			umac->cmd |= CMD_TX_PAUSE_IGNORE;
	}
		
}

int mii_init(struct net_device *dev)
{
    BcmEnet_devctrl *pDevCtrl = netdev_priv(dev);
    volatile uniMacRegs *umac;

    umac = pDevCtrl->umac;
	pDevCtrl->mii.phy_id = pDevCtrl->phyAddr;
	pDevCtrl->mii.phy_id_mask = 0x1f;
	pDevCtrl->mii.reg_num_mask = 0x1f;
	pDevCtrl->mii.dev = dev;
	pDevCtrl->mii.mdio_read = mii_read;
	pDevCtrl->mii.mdio_write = mii_write;

    switch(pDevCtrl->phyType) {

		case BRCM_PHY_TYPE_INT:
			pDevCtrl->mii.supports_gmii = 0;
			pDevCtrl->sys->sys_port_ctrl = PORT_MODE_INT_EPHY;
			printk(KERN_INFO "Config internal EPHY through MDIO\n");
			break;
		case BRCM_PHY_TYPE_EXT_MII:
			pDevCtrl->mii.supports_gmii = 0;
			pDevCtrl->sys->sys_port_ctrl = PORT_MODE_EXT_EPHY;
			printk(KERN_INFO "Config EPHY through MDIO\n");
            break;
		case BRCM_PHY_TYPE_EXT_GMII:
			GENET_RGMII_OOB_CTRL(pDevCtrl) |= RGMII_MODE_EN;
			GENET_RGMII_OOB_CTRL(pDevCtrl) |= (1 << 16);	/* Don't shift tx clock by 90 degree */
			pDevCtrl->mii.supports_gmii = 1;
			pDevCtrl->sys->sys_port_ctrl = PORT_MODE_EXT_GPHY;
			printk(KERN_INFO "Config GPHY through MDIO\n");
            break;
		case BRCM_PHY_TYPE_EXT_GMII_IBS:
			GENET_RGMII_OOB_CTRL(pDevCtrl) |= RGMII_MODE_EN;
			GENET_RGMII_OOB_CTRL(pDevCtrl) |= (1 << 16);
			/* Use in-band signaling for auto config.*/
			GENET_RGMII_OOB_CTRL(pDevCtrl) |= OOB_DISABLE;
			umac->cmd |= CMD_AUTO_CONFIG;
			pDevCtrl->mii.supports_gmii = 1;
			pDevCtrl->sys->sys_port_ctrl = PORT_MODE_EXT_GPHY;
			printk(KERN_INFO "Automatic Config GPHY \n");
            break;
		case BRCM_PHY_TYPE_MOCA:
			printk(KERN_INFO "Config MoCA...\n");
			umac->cmd = umac->cmd  | (UMAC_SPEED_1000 << CMD_SPEED_SHIFT);
			pDevCtrl->mii.force_media = 1;
			pDevCtrl->sys->sys_port_ctrl = PORT_MODE_INT_GPHY;
			break;
        default:
			printk(KERN_ERR "unknown phy_type : %d\n", pDevCtrl->phyType);
            break;
    }

    return 0;
}
// End of file
