/*
 *  TOPPERS BASE PLATFORM MIDDLEWARE
 * 
 *  Copyright (C) 2017-2017 by TOPPERS PROJECT
 *                             Educational Working Group.
 * 
 *  �嵭����Ԥϡ��ʲ���(1)��(4)�ξ������������˸¤ꡤ�ܥ��եȥ���
 *  �����ܥ��եȥ���������Ѥ�����Τ�ޤࡥ�ʲ�Ʊ���ˤ���ѡ�ʣ������
 *  �ѡ������ۡʰʲ������ѤȸƤ֡ˤ��뤳�Ȥ�̵���ǵ������롥
 *  (1) �ܥ��եȥ������򥽡��������ɤη������Ѥ�����ˤϡ��嵭������
 *      ��ɽ�����������Ѿ�浪��Ӳ�����̵�ݾڵ��꤬�����Τޤޤη��ǥ���
 *      ����������˴ޤޤ�Ƥ��뤳�ȡ�
 *  (2) �ܥ��եȥ������򡤥饤�֥������ʤɡ�¾�Υ��եȥ�������ȯ�˻�
 *      �ѤǤ�����Ǻ����ۤ�����ˤϡ������ۤ�ȼ���ɥ�����ȡ�����
 *      �ԥޥ˥奢��ʤɡˤˡ��嵭�����ɽ�����������Ѿ�浪��Ӳ���
 *      ��̵�ݾڵ����Ǻܤ��뤳�ȡ�
 *  (3) �ܥ��եȥ������򡤵�����Ȥ߹���ʤɡ�¾�Υ��եȥ�������ȯ�˻�
 *      �ѤǤ��ʤ����Ǻ����ۤ�����ˤϡ����Τ����줫�ξ�����������
 *      �ȡ�
 *    (a) �����ۤ�ȼ���ɥ�����ȡ����Ѽԥޥ˥奢��ʤɡˤˡ��嵭����
 *        �ɽ�����������Ѿ�浪��Ӳ�����̵�ݾڵ����Ǻܤ��뤳�ȡ�
 *    (b) �����ۤη��֤��̤�������ˡ�ˤ�äơ�TOPPERS�ץ������Ȥ�
 *        ��𤹤뤳�ȡ�
 *  (4) �ܥ��եȥ����������Ѥˤ��ľ��Ū�ޤ��ϴ���Ū�������뤤���ʤ�»
 *      ������⡤�嵭����Ԥ����TOPPERS�ץ������Ȥ����դ��뤳�ȡ�
 *      �ޤ����ܥ��եȥ������Υ桼���ޤ��ϥ���ɥ桼������Τ����ʤ���
 *      ͳ�˴�Ť����ᤫ��⡤�嵭����Ԥ����TOPPERS�ץ������Ȥ�
 *      ���դ��뤳�ȡ�
 * 
 *  �ܥ��եȥ������ϡ�̵�ݾڤ��󶡤���Ƥ����ΤǤ��롥�嵭����Ԥ�
 *  ���TOPPERS�ץ������Ȥϡ��ܥ��եȥ������˴ؤ��ơ�����λ�����Ū
 *  ���Ф���Ŭ������ޤ�ơ������ʤ��ݾڤ�Ԥ�ʤ����ޤ����ܥ��եȥ���
 *  �������Ѥˤ��ľ��Ū�ޤ��ϴ���Ū�������������ʤ�»���˴ؤ��Ƥ⡤��
 *  ����Ǥ�����ʤ���
 * 
 *  @(#) $Id: usb_device_driver.c 698 2017-11-20 20:10:29Z roi $
 */
/*
 *  USB DEVICE HIGH DRIVERS
 */

#include "kernel_impl.h"
#include <t_syslog.h>
#include <t_stdlib.h>
#include <sil.h>
#include <target_syssvc.h>
#include "device.h"

#include "stdlib.h"
#include "tusbd_base.h"
#include "usb_device.h"


#define sil_orw_mem(a, b)		sil_wrw_mem((a), sil_rew_mem(a) | (b))
#define sil_andw_mem(a, b)		sil_wrw_mem((a), sil_rew_mem(a) & ~(b))
#define sil_orh_mem(a, b)		sil_wrh_mem((a), sil_reh_mem(a) | (b))
#define sil_andh_mem(a, b)		sil_wrh_mem((a), sil_reh_mem(a) & ~(b))

#ifndef MAX_DEVICE_EPS
#define MAX_DEVICE_EPS          (MAX_EPS_CHANNELS-1)
#endif

/*
 *  SETUPSTAGE CALLBACK�ؿ�
 */
static void
tusbdSetupStageCallback(USB_DEV_Handle_t *husb)
{
	tusbdSetupStage(husb->pDev, (uint8_t *)husb->Setup);
}

/*
 *  DATAOUTSTAGE CALLBACK�ؿ�
 */
static void
tusbdDataOutStageCallback(USB_DEV_Handle_t *husb, uint8_t epnum)
{
	tusbdDataOutStage(husb->pDev, epnum, husb->OUT_ep[epnum].xfer_buff);
}

/*
 *  DATAINSTAGE CALLBACK�ؿ�
 */
static void
tusbdDataInStageCallback(USB_DEV_Handle_t *husb, uint8_t epnum)
{
	tusbdDataInStage(husb->pDev, epnum, husb->IN_ep[epnum].xfer_buff);
}

/*
 *  DEVICE RESET CALLBACK�ؿ�
 */
static void
tusbdResetCallback(USB_DEV_Handle_t *husb)
{
	uint8_t speed = USB_DEVICE_SPEED_FULL;

	/* Set USB Current Speed */
	switch(husb->Init.speed){
	case USB_SPEED_HIGH:
		speed = USB_DEVICE_SPEED_HIGH;
		break;
	case USB_SPEED_FULL:
		speed = USB_DEVICE_SPEED_FULL;
		break;
	default:
		speed = USB_DEVICE_SPEED_FULL;
		break;
	}

	/*
	 *  DEVICE�Υꥻ�å�
	 */
	tusbdReset(husb->pDev);
	tusbdSetSpeed(husb->pDev, speed);
}

/*
 *  DEVICE SUSPEND CALLBACK�ؿ�
 */
static void
tusbdSuspendCallback(USB_DEV_Handle_t *husb)
{
	tusbdSuspend(husb->pDev);
#if 1	/* ROI DEBUG */
	syslog_0(LOG_NOTICE, "## tusbdSuspendCallback ##");
#endif
  /*Enter in STOP mode */
	if (husb->Init.low_power_enable){
		/* Set SLEEPDEEP bit and SleepOnExit of Cortex System Control Register */
		sil_orw_mem((uint32_t *)(TADR_SCB_BASE+TOFF_SCB_SCR), (SCB_SCR_SLEEPDEEP_Msk | SCB_SCR_SLEEPONEXIT_Msk));
	}
}

/*
 *  DEVICE RESUME CALLBACK�ؿ�
 */
static void
tusbdResumeCallback(USB_DEV_Handle_t *husb)
{
	TUSBD_Handle_t *pdevice = husb->pDev;
	if(husb->Init.low_power_enable && pdevice->dev_state == TUSBD_STATE_SUSPENDED){
		/* Reset SLEEPDEEP bit of Cortex System Control Register */
		sil_andw_mem((uint32_t *)(TADR_SCB_BASE+TOFF_SCB_SCR), (SCB_SCR_SLEEPDEEP_Msk | SCB_SCR_SLEEPONEXIT_Msk));
		sysclock_config(1);
	}
	tusbdResume(husb->pDev);
#if 1	/* ROI DEBUG */
	syslog_0(LOG_NOTICE, "## tusbdResumeCallback ##");
#endif
}

/*
 *  DEVICE CONNECT CALLBACK�ؿ�
 */
static void
tusbdConnectCallback(USB_DEV_Handle_t *husb)
{
	tusbdDeviceConnected(husb->pDev);
}

/*
 *  DEVICE DISCONNECT CALLBACK�ؿ�
 */
static void
tusbdDisconnectCallback(USB_DEV_Handle_t *husb)
{
	tusbdDeviceDisconnected(husb->pDev);
}

/*
 *  ENDPOINT����
 */
static USB_DEV_EPTypeDef *
tusbdGetEndpoint(USB_DEV_Handle_t *husb, uint8_t addr)
{
	USB_DEV_EPTypeDef *ep;

	if((addr & USB_DEVICE_TO_HOST) != 0){
		ep = &husb->IN_ep[addr & 0x7F];
	}
	else{
		ep = &husb->OUT_ep[addr & 0x7F];
	}
	ep->num   = addr & 0x7F;
	ep->is_in = (USB_DEVICE_TO_HOST & addr) != 0;
	return ep;
}


/*
 *  USB_DEVICE HIGH DRIVER INITIALIZE
 *  parameter1 pdevice:  USB�ǥХ����ϥ�ɥ�
 *  return     TUSBD_ERCODE
 */
TUSBD_ERCODE
usb_device_init(TUSBD_Handle_t *pdevice)
{
	USB_DEV_Handle_t *husb = (USB_DEV_Handle_t *)(pdevice->pSysData);
	uint32_t i = 0;

	/*
	 *  USB�ϥ�ɥ�ˡ�USB�ǥХ����ϥ�ɥ�򥻥å�
	 */
	husb->pDev = pdevice;

	/*
	 *  ������Хå��ؿ��򥻥å�
	 */
	husb->devsetupstagecallback   = tusbdSetupStageCallback;
	husb->devdataoutstagecallback = tusbdDataOutStageCallback;
	husb->devdatainstagecallback  = tusbdDataInStageCallback;
	husb->devsofcallback          = NULL;
	husb->devresetcallback        = tusbdResetCallback;
	husb->devsuspendcallback      = tusbdSuspendCallback;
	husb->devresumecallback       = tusbdResumeCallback;
	husb->devconnectcallback      = tusbdConnectCallback;
	husb->devdisconnectcallback   = tusbdDisconnectCallback;
	husb->devlpmcallback          = NULL;

	/*
	 *  ����ɥݥ���Ȥν����
	 */
	for(i = 0 ; i < MAX_DEVICE_EPS ; i++){
		husb->IN_ep[i].is_in = 1;
		husb->IN_ep[i].num = i;
		husb->IN_ep[i].type = EP_TYPE_CTRL;
		husb->IN_ep[i].maxpacket = 0;
		husb->IN_ep[i].xfer_buff = 0;
		husb->IN_ep[i].xfer_len = 0;

		husb->OUT_ep[i].is_in = 0;
		husb->OUT_ep[i].num = i;
 		husb->OUT_ep[i].type = EP_TYPE_CTRL;
		husb->OUT_ep[i].maxpacket = 0;
		husb->OUT_ep[i].xfer_buff = 0;
		husb->OUT_ep[i].xfer_len = 0;
	}

	/*
	 *  LPM��ͭ����
	 */
	usbd_init_lpm(husb);
	usbd_init_BCD(husb);

	usbd_setupPMA(husb , 0x00 , PCD_SNG_BUF, 0x18);
	usbd_setupPMA(husb , 0x80 , PCD_SNG_BUF, 0x58);
	usbd_setupPMA(husb , 0x81 , PCD_SNG_BUF, 0x100);
	return TUSBD_E_OK;
}

/*
 *  USB_DEVICE HIGH DRIVER DE-INITIALIZE
 *  parameter1 pdevice:  USB�ǥХ����ϥ�ɥ�
 *  return     TUSBD_ERCODE
  */
TUSBD_ERCODE
usb_device_deinit(TUSBD_Handle_t *pdevice)
{
	/*
	 *  �ǥХ������
	 */
	usb_device_stop(pdevice);

	/*
	 *  �ǥХ����ϡ��ɥ�������λ
	 */
	usbd_deinit(pdevice->pSysData);
	return TUSBD_E_OK;
}

/*
 *  USB DEVICE HIGH DRIVER START
 *  parameter1 pdevice:  USB�ǥХ����ϥ�ɥ�
 *  return     TUSBD_ERCODE
  */
TUSBD_ERCODE
usb_device_start(TUSBD_Handle_t *pdevice)
{
	usbd_devconnect(pdevice->pSysData);
	return TUSBD_E_OK;
}

/*
 *  USB DEVICE HIGH DRIVER STOP
 *  parameter1 pdevice:  USB�ǥХ����ϥ�ɥ�
 *  return     TUSBD_ERCODE
  */
TUSBD_ERCODE
usb_device_stop(TUSBD_Handle_t *pdevice)
{
	usbd_devdisconnect(pdevice->pSysData);
	return TUSBD_E_OK;
}

/*
 *  USB DEVICE HIGH DRIVER OPEN ENDPOINT
 *  parameter1 pdevice:  USB�ǥХ����ϥ�ɥ�
 *  parameter2 addr:  endpoint���ɥ쥹
 *  parameter3 type:  endpoint������
 *  parameter4 mps:   MAX PACKET SIZE
 *  return     TUSBD_ERCODE
 */
TUSBD_ERCODE
usb_device_openep(TUSBD_Handle_t *pdevice, uint8_t addr, uint8_t type, uint16_t mps)
{
	USB_DEV_Handle_t  *husb = pdevice->pSysData;
	USB_DEV_EPTypeDef *ep;

	ep = tusbdGetEndpoint(husb, addr);
	ep->maxpacket = mps;
	ep->type = type;
	usbd_activateEndpoint(husb, ep);
	return TUSBD_E_OK;
}

/*
 *  USB DEVICE HIGH DRIVER CLOSE ENDPOINT
 *  parameter1 pdevice:  USB�ǥХ����ϥ�ɥ�
 *  parameter2 addr:  endpoint���ɥ쥹
 *  return     TUSBD_ERCODE
 */
TUSBD_ERCODE
usb_device_closeep(TUSBD_Handle_t *pdevice, uint8_t addr)
{
	USB_DEV_Handle_t  *husb = pdevice->pSysData;
	USB_DEV_EPTypeDef *ep;

	ep = tusbdGetEndpoint(husb, addr);
	usbd_deactivateEndpoint(husb, ep);
	return TUSBD_E_OK;
}

/*
 *  USB DEVICE HIGH DRIVER FLUSH ENDPOINT
 *  parameter1 pdevice:  USB�ǥХ����ϥ�ɥ�
 *  parameter2 addr:  endpoint���ɥ쥹
 *  return     TUSBD_ERCODE
 */
TUSBD_ERCODE
usb_device_flushep(TUSBD_Handle_t *pdevice, uint8_t addr)
{
	return TUSBD_E_OK;
}

/*
 *  USB DEVICE HIGH DRIVER STALL ENDPOINT
 *  parameter1 pdevice:  USB�ǥХ����ϥ�ɥ�
 *  parameter2 addr:  endpoint���ɥ쥹
 *  return     TUSBD_ERCODE
 */
TUSBD_ERCODE
usb_device_stallep(TUSBD_Handle_t *pdevice, uint8_t addr)
{
	USB_DEV_Handle_t  *husb = pdevice->pSysData;
	USB_DEV_EPTypeDef *ep;

	ep = tusbdGetEndpoint(husb, addr);
	ep->is_stall = 1;

	usbd_epsetStall(husb, ep);
	return TUSBD_E_OK;
}

/*
 *  USB DEVICE HIGH DRIVER CLEAR STALL-ENDPOINT
 *  parameter1 pdevice:  USB�ǥХ����ϥ�ɥ�
 *  parameter2 addr:  endpoint���ɥ쥹
 *  return     TUSBD_ERCODE
 */
TUSBD_ERCODE
usb_device_clearep(TUSBD_Handle_t *pdevice, uint8_t addr)
{
	USB_DEV_Handle_t *husb = pdevice->pSysData;
	USB_DEV_EPTypeDef *ep;

	ep = tusbdGetEndpoint(husb, addr);
	ep->is_stall = 0;

	usbd_epclearStall(husb, ep);
	return TUSBD_E_OK; 
}

/*
 *  USB DEVICE HIGH DRIVER GET STALL CONDITION
 *  parameter1 pdevice:  USB�ǥХ����ϥ�ɥ�
 *  parameter2 addr:  endpoint���ɥ쥹
 *  return     1:stall 0:not stall
 */
uint8_t
usb_device_getstallcondition(TUSBD_Handle_t *pdevice, uint8_t addr)
{
	USB_DEV_Handle_t *husb = pdevice->pSysData;

	if((addr & USB_DEVICE_TO_HOST) != 0)
		return husb->IN_ep[addr & 0x7F].is_stall;
	else
		return husb->OUT_ep[addr & 0x7F].is_stall;
}

/*
 *  USB DEVICE HIGH DRIVER TRANSMIT A DATA
 *  parameter1 pdevice:  USB�ǥХ����ϥ�ɥ�
 *  parameter2 addr:  endpoint���ɥ쥹
 *  parameter3 pbuf:  �����ǡ����Хåե�
 *  parameter4 size:  ����������
 *  return     TUSBD_ERCODE
 */
TUSBD_ERCODE
usb_device_transmitdata(TUSBD_Handle_t *pdevice, uint8_t addr, uint8_t *pbuf, uint16_t size)
{
	USB_DEV_Handle_t  *husb = pdevice->pSysData;
	USB_DEV_EPTypeDef *ep;

	ep = tusbdGetEndpoint(husb, (addr | USB_DEVICE_TO_HOST));

	/*
	 *  �����ǡ�������
	 */
	ep->xfer_buff = pbuf;
	ep->xfer_len = size;
	ep->xfer_count = 0;
	usbd_epstartsend(husb, ep);
	return TUSBD_E_OK;
}

/*
 *  USB DEVICE HIGH DRIVER START to RECEIVE
 *  parameter1 pdevice:  USB�ǥХ����ϥ�ɥ�
 *  parameter2 addr:  endpoint���ɥ쥹
 *  parameter3 pbuf:  �����ǡ����Хåե�
 *  parameter4 size:  ����������
 *  return     TUSBD_ERCODE
 */
TUSBD_ERCODE
usb_device_startreceive(TUSBD_Handle_t *pdevice, uint8_t addr, uint8_t *pbuf, uint16_t size)
{
	USB_DEV_Handle_t  *husb = pdevice->pSysData;
	USB_DEV_EPTypeDef *ep;

	ep = tusbdGetEndpoint(husb, (addr & ~USB_DEVICE_TO_HOST));

	/*
	 *  �����ǡ�������
	 */
	ep->xfer_buff = pbuf;
	ep->xfer_len = size;
	ep->xfer_count = 0;
	usbd_epstartreceive(husb, ep);
	return TUSBD_E_OK;
}

/*
 *  USB DEVICE HIGH DRIVER TEST MODE
 *  parameter1 pdevice:  USB�ǥХ����ϥ�ɥ�
 *  return     TUSBD_ERCODE
 */
TUSBD_ERCODE
usb_device_testmode(TUSBD_Handle_t *pdevice)
{
	return TUSBD_E_OK;
}

/*
 *  USB DEVICE HIGH DRIVER REMOTE WAKEUP
 *  parameter1 pdevice:  USB�ǥХ����ϥ�ɥ�
 *  return     TUSBD_ERCODE
 */
void
usb_device_wakeup(USBD_HandleTypeDef *pdevice)
{
	USB_DEV_Handle_t *husb = pdevice->pSysData;

	if(pdevice->dev_remote_wakeup == 1 && pdevice->dev_state == TUSBD_STATE_SUSPENDED){
		if(husb->Init.low_power_enable){
			/* Reset SLEEPDEEP bit of Cortex System Control Register */
			sil_andw_mem((uint32_t *)(TADR_SCB_BASE+TOFF_SCB_SCR), (SCB_SCR_SLEEPDEEP_Msk | SCB_SCR_SLEEPONEXIT_Msk));
			sysclock_config(1);
		}
		/* Activate Remote wakeup */
		if(husb->Init.lpm_enable == 1)	/* Apply L1 Resume */
			sil_orh_mem((uint16_t *)(husb->base+TOFF_USB_CNTR), USB_CNTR_L1RESUME);
		else	/* Apply L2 Resume */
		sil_orh_mem((uint16_t *)(husb->base+TOFF_USB_CNTR), USB_CNTR_RESUME);

		/* Remote wakeup delay */
		sil_dly_nse(10*1000*1000);

		if(husb->Init.lpm_enable == 1)	/* Release L1 Resume */
			sil_andh_mem((uint16_t *)(husb->base+TOFF_USB_CNTR), USB_CNTR_L1RESUME);
		else		/* Release L2 Resume */
			sil_andh_mem((uint16_t *)(husb->base+TOFF_USB_CNTR), USB_CNTR_RESUME);
		/* Change remote_wakeup feature to 0*/
		pdevice->dev_remote_wakeup = 0;
	}
}

