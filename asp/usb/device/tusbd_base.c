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
 *  @(#) $Id: tusbd_base.c 698 2017-10-28 10:53:51Z roi $
 */
/*
 *  USB Device Middleware BASE��
 */

#include "tusbd_base.h"
#include "tusbd_device.h"


#define NUM_MAX_REQUEST      13
#define NUM_STRING_TABLE     7

static void DeviceGetStatus(TUSBD_Handle_t *pdevice, UsbSetupRequest *req);
static void DeviceClearFeature(TUSBD_Handle_t *pdevice, UsbSetupRequest *req);
static void DeviceSetFeature(TUSBD_Handle_t *pdevice, UsbSetupRequest *req);
static void DeviceGetDescriptor(TUSBD_Handle_t *pdevice, UsbSetupRequest *req);
static void DeviceSetAddress(TUSBD_Handle_t *pdevice, UsbSetupRequest *req);
static void DeviceGetConfiguration(TUSBD_Handle_t *pdevice, UsbSetupRequest *req);
static void DeviceSetConfiguration(TUSBD_Handle_t *pdevice, UsbSetupRequest *req);
static void EndpointGetStatus(TUSBD_Handle_t *pdevice, UsbSetupRequest *req);
static void EndpointClearFeature(TUSBD_Handle_t *pdevice, UsbSetupRequest *req);
static void EndpointSetFeature(TUSBD_Handle_t *pdevice, UsbSetupRequest *req);


/*
 *  DEVICE REQUEST�ؿ��ơ��֥�
 */
static void (*usb_pbRequestDev[NUM_MAX_REQUEST])(TUSBD_Handle_t *pdevice, UsbSetupRequest *req) = {
	DeviceGetStatus,        DeviceClearFeature,     tusbdControlSendStall, DeviceSetFeature,
	tusbdControlSendStall,  DeviceSetAddress,       DeviceGetDescriptor,   tusbdControlSendStall,
	DeviceGetConfiguration, DeviceSetConfiguration, tusbdControlSendStall, tusbdControlSendStall,
	tusbdControlSendStall
};

/*
 *  ENDPOINT REQUEST�ؿ��ơ��֥�
 */
static void (*usb_pbRequestEp[NUM_MAX_REQUEST])(TUSBD_Handle_t *pdevice, UsbSetupRequest *req) = {
	EndpointGetStatus,      EndpointClearFeature,   tusbdControlSendStall, EndpointSetFeature,
	tusbdControlSendStall,  tusbdControlSendStall,  tusbdControlSendStall, tusbdControlSendStall,
	tusbdControlSendStall,  tusbdControlSendStall,  tusbdControlSendStall, tusbdControlSendStall,
	tusbdControlSendStall
};

static uint8_t *usb_string_table[NUM_STRING_TABLE] = {
	TUSBD_LANGUAGEID_DESCRIPTOR,
	TUSBD_MANUFACTURER_STRING,
	TUSBD_PRODUCT_STRING,
	TUSBD_SERIAL_STRING,
	TUSBD_CONFIGURATION_STRING,
	TUSBD_INTERFACE_STRING,
	TUSBD_USER_STRING
};


/*
 *  DEVICE REQUEST GET STATUS
 */
static void
DeviceGetStatus(TUSBD_Handle_t *pdevice, UsbSetupRequest *req)
{
	switch (pdevice->dev_state){
	case TUSBD_STATE_ADDRESSED:
	case TUSBD_STATE_CONFIGURED:
		pdevice->devData[0] = TUSBD_CONFIG_SELF_POWERED;
		pdevice->devData[1] = 0;
		if(pdevice->dev_remote_wakeup)
			pdevice->devData[0] |= TUSBD_CONFIG_REMOTE_WAKEUP;
		tusbdControlSendData(pdevice, (uint8_t *)pdevice->devData, 2);
		break;
	default:
		tusbdControlSendStall(pdevice, req);
		break;
	}
}

/*
 *  DEVICE REQUEST CLEAR FEATURE
 */
static void
DeviceClearFeature(TUSBD_Handle_t *pdevice, UsbSetupRequest *req)
{
	switch (pdevice->dev_state){
	case TUSBD_STATE_ADDRESSED:
	case TUSBD_STATE_CONFIGURED:
		if(req->wValue == USB_FEATURE_REMOTE_WAKEUP){
			pdevice->dev_remote_wakeup = 0;
			tusbdClassSetup(pdevice, req);
			tusbdControlSendStatus(pdevice);
		}
		break;
	default:
		tusbdControlSendStall(pdevice, req);
		break;
	}
}

/*
 *  DEVICE REQUEST SET FEATURE
 */
static void
DeviceSetFeature(TUSBD_Handle_t *pdevice, UsbSetupRequest *req)
{
	if(req->wValue == USB_FEATURE_REMOTE_WAKEUP){
		pdevice->dev_remote_wakeup = 1;
		tusbdClassSetup(pdevice, req);
		tusbdControlSendStatus(pdevice);
	}
}

/*
 *  DEVICE REQUEST GET DESCRIPTOR
 */
static void
DeviceGetDescriptor(TUSBD_Handle_t *pdevice, UsbSetupRequest *req)
{
	uint16_t len;
	uint8_t *pbuf;

	switch (req->wValue >> 8){
#if defined(TUSBD_BOS_DESCRIPTOR)
	case BOS_DESCRIPTOR:
		pbuf = TUSBD_BOS_DESCRIPTOR;
		len  = TUSBD_BOS_DESCRIPTOR[0];
		break;
#endif
	case DEVICE_DESCRIPTOR:
		pbuf = TUSBD_DEVICE_DESCRIPTOR;
		len  = TUSBD_DEVICE_DESCRIPTOR[0];
		break;
	case CONFIGURATION_DESCRIPTOR:
		if(pdevice->dev_speed == USB_DEVICE_SPEED_HIGH){
			pbuf = TUSBD_HS_CONFIG_DESCRIPTOR;
			len  = TUSBD_HS_CONFIG_DESC_LENGTH;
		}
		else{
			pbuf = TUSBD_FS_CONFIG_DESCRIPTOR;
			len  = TUSBD_FS_CONFIG_DESC_LENGTH;
		}
		break;
	case STRING_DESCRIPTOR:
		if(((uint8_t)req->wValue) < NUM_STRING_TABLE){
			pbuf = usb_string_table[(uint8_t)req->wValue];
			if(pbuf == NULL){
				tusbdControlSendStall(pdevice, req);
				return;
			}
			len  = pbuf[0];
		}
		else{
			tusbdControlSendStall(pdevice, req);
			return;
		}
		break;
	case DEVICE_QUALIFIER_DESCRIPTOR:
		if(pdevice->dev_speed == USB_DEVICE_SPEED_HIGH){
			pbuf = TUSBD_DEVICE_QUALIFIER_DESCRIPTOR;
			len  = TUSBD_DEVICE_QUALIFIER_DESCRIPTOR[0];
			break;
		}
		else{
			tusbdControlSendStall(pdevice, req);
			return;
		}
	case OTHER_SPEED_CONFIGURATION_DESC:
		if(pdevice->dev_speed == USB_DEVICE_SPEED_HIGH){
			pbuf = TUSBD_OTR_CONFIG_DESCRIPTOR;
			len  = TUSBD_OTR_CONFIG_DESC_LENGTH;
			break;
		}
		else{
			tusbdControlSendStall(pdevice, req);
			return;
		}
	default:
		tusbdControlSendStall(pdevice, req);
		return;
	}

	if(len != 0 && req->wLength != 0){
		if(len > req->wLength)
			len = req->wLength;
		tusbdControlSendData(pdevice, pbuf, len);
	}
}

/*
 *  DEVICE REQUEST SET ADDRESS
 */
static void
DeviceSetAddress(TUSBD_Handle_t *pdevice, UsbSetupRequest *req)
{
	uint8_t  dev_addr; 

	if(req->wIndex == 0 && req->wLength == 0){
		dev_addr = (uint8_t)(req->wValue) & 0x7F;
		if(pdevice->dev_state == TUSBD_STATE_CONFIGURED){
			tusbdControlSendStall(pdevice , req);
		}
		else{
			pdevice->dev_address = dev_addr;
			tusbdDriverSetUSBAddress(pdevice, dev_addr);
			tusbdControlSendStatus(pdevice);
			if(dev_addr != 0){
				pdevice->dev_state = TUSBD_STATE_ADDRESSED;
			}
			else{
				pdevice->dev_state = TUSBD_STATE_INIT;
			}
		}
	}
	else{
		tusbdControlSendStall(pdevice, req);
	}
}

/*
 *  DEVICE REQUEST GET CONFIGURATION
 */
static void
DeviceGetConfiguration(TUSBD_Handle_t *pdevice, UsbSetupRequest *req)
{
	if(req->wLength != 1){
		tusbdControlSendStall(pdevice , req);
	}
	else{
		switch (pdevice->dev_state){
		case TUSBD_STATE_ADDRESSED:
			pdevice->devData[0] = 0;
			tusbdControlSendData(pdevice, (uint8_t *)pdevice->devData, 1);
			break;
		case TUSBD_STATE_CONFIGURED:
			pdevice->devData[0] = pdevice->dev_config;
			tusbdControlSendData(pdevice, (uint8_t *)pdevice->devData, 1);
			break;
		default:
			tusbdControlSendStall(pdevice, req);
			break;
		}
	}
}

/*
 *  DEVICE REQUEST SET CONFIGURATION
 */
static void
DeviceSetConfiguration(TUSBD_Handle_t *pdevice, UsbSetupRequest *req)
{
	uint8_t  cfgidx;

	cfgidx = (uint8_t)(req->wValue);
	if(cfgidx > TUSBD_MAX_NUM_CONFIGURATION){
		tusbdControlSendStall(pdevice , req);
		return;
	}

	switch (pdevice->dev_state){
	case TUSBD_STATE_ADDRESSED:
		if(cfgidx){
			pdevice->dev_config = cfgidx;
			pdevice->dev_state = TUSBD_STATE_CONFIGURED;
			if(tusbdClassInit(pdevice, cfgidx) != TUSBD_E_OK){
				tusbdControlSendStall(pdevice , req);
				return;
			}
			tusbdControlSendStatus(pdevice);
		}
		else{
			tusbdControlSendStatus(pdevice);
		}
		break;
	case TUSBD_STATE_CONFIGURED:
		if(cfgidx == 0){
			pdevice->dev_state = TUSBD_STATE_ADDRESSED;
			pdevice->dev_config = cfgidx;
			tusbdClassDeInit(pdevice, cfgidx);
			tusbdControlSendStatus(pdevice);
		}
		else if(cfgidx != pdevice->dev_config){
			/*
			 *  �Ť�CONFIGURATION��ꥻ�å�
			 */
			tusbdClassDeInit(pdevice, cfgidx);

			/*
			 *  ������CONFIGURATION�ǽ����
			 */
			pdevice->dev_config = cfgidx;
			if(tusbdClassInit(pdevice, cfgidx) != TUSBD_E_OK){
				tusbdControlSendStall(pdevice , req);
				return;
			}
			tusbdControlSendStatus(pdevice);
		}
		else{
			tusbdControlSendStatus(pdevice);
		}
		break;
	default:
		tusbdControlSendStall(pdevice, req);
		break;
	}
}

/*
 *  ENDPOINT REQUEST GET STATUS
 */
static void
EndpointGetStatus(TUSBD_Handle_t *pdevice, UsbSetupRequest *req)
{
	TUSBD_Endpoint_t *pep;
	uint8_t ep_addr = req->wIndex & 0xFF;

	switch (pdevice->dev_state){
	case TUSBD_STATE_ADDRESSED:
		if((ep_addr & 0x7F) != 0x00){
			tusbdDriverStallEp(pdevice , ep_addr);
		}
		break;
	case TUSBD_STATE_CONFIGURED:
		pep = ((ep_addr & USB_DEVICE_TO_HOST) != 0) ? &pdevice->ep_in[ep_addr & 0x7F] : &pdevice->ep_out[ep_addr & 0x7F];
		if(tusbdDriverStallConditionEp(pdevice, ep_addr)){
			pep->status = 0x0001;
		}
		else{
			pep->status = 0x0000;
		}
		tusbdControlSendData(pdevice, (uint8_t *)&pep->status, 2);
		break;
	default:
		tusbdControlSendStall(pdevice, req);
		break;
	}
}

/*
 *  ENDPOINT REQUEST CLEAR FEATURE
 */
static void
EndpointClearFeature(TUSBD_Handle_t *pdevice, UsbSetupRequest *req)
{
	uint8_t ep_addr = req->wIndex & 0xFF;

	switch (pdevice->dev_state){
	case TUSBD_STATE_ADDRESSED:
		if((ep_addr & 0x7F) != 0x00){
			tusbdDriverStallEp(pdevice , ep_addr);
		}
		break;
	case TUSBD_STATE_CONFIGURED:
		if(req->wValue == USB_FEATURE_EP_HALT){
			if((ep_addr & 0x7F) != 0x00){
				tusbdDriverClearStallEp(pdevice , ep_addr);
				tusbdClassSetup(pdevice, req);
			}
			tusbdControlSendStatus(pdevice);
		}
		break;
	default:
		tusbdControlSendStall(pdevice, req);
		break;
	}
}

/*
 *  ENDPOINT REQUEST SET FEATURE
 */
static void
EndpointSetFeature(TUSBD_Handle_t *pdevice, UsbSetupRequest *req)
{
	uint8_t ep_addr = req->wIndex & 0xFF;

	switch (pdevice->dev_state){
	case TUSBD_STATE_ADDRESSED:
		if((ep_addr & 0x7F) != 0x00){
			tusbdDriverStallEp(pdevice , ep_addr);
		}
		break;
	case TUSBD_STATE_CONFIGURED:
		if(req->wValue == USB_FEATURE_EP_HALT){
			if((ep_addr & 0x7F) != 0x00){
				tusbdDriverStallEp(pdevice , ep_addr);
			}
		}
		tusbdClassSetup(pdevice, req);
		tusbdControlSendStatus(pdevice);
		break;
	default:
		tusbdControlSendStall(pdevice, req);
		break;
	}
}


/*
 *  USB DEVICE�����
 *  parameter1 pdevice:  USB DEVICE�ϥ�ɥ�
 *  parameter2 id:    �ǥХ���ID
 *  return     TUSBD_ERCODE
 */
TUSBD_ERCODE
tusbdInit(TUSBD_Handle_t *pdevice, uint8_t id)
{
	if(pdevice == NULL)
		return TUSBD_E_ERR;

	/*
	 *  �ϥ�ɥ�����
	 */
	pdevice->dev_state = TUSBD_STATE_INIT;
	pdevice->id = id;

	/*
	 *  �ϡ��ɥ����������
	 */
	return tusbdDriverInit(pdevice);
}

/*
 *  USB DEVICE��λ
 *  parameter1 pdevice:  USB DEVICE�ϥ�ɥ�
 *  return    TUSBD_ERCODE
 */
TUSBD_ERCODE
tusbdDeInit(TUSBD_Handle_t *pdevice)
{
	/*
	 *  �ǥХ������֤�ꥻ�åȤ�
	 */
	pdevice->dev_state = TUSBD_STATE_RESET;

	/*
	 *  �ɥ饤�Х��饹�����
	 */
	tusbdClassDeInit(pdevice, pdevice->dev_config);

	/*
	 *  �ϡ��ɥ�������λ
	 */
	return tusbdDriverDeInit(pdevice);
}

/*
 *  USB DEVICE����
 *  parameter1 pdevice:  USB DEVICE�ϥ�ɥ�
 *  return    TUSBD_ERCODE
 */
TUSBD_ERCODE
tusbdStart(TUSBD_Handle_t *pdevice)
{
	pdevice->ep0_state = TUSBD_EP0_IDLE;
	/*
	 *  USB DEVICE�ɥ饤�г���
	 */
	return tusbdDriverStart(pdevice);
}

/*
 *  USB DEVICE ���
 *  parameter1 pdevice:  USB DEVICE�ϥ�ɥ�
 *  return    TUSBD_ERCODE
 */
TUSBD_ERCODE
tusbdStop(TUSBD_Handle_t *pdevice)
{
	/*
	 *  �ɥ饤�Х��饹���
	 */
	tusbdClassDeInit(pdevice, pdevice->dev_config);

	/*
	 *  USB DEVICE�ɥ饤�����
	 */
	tusbdDriverStop(pdevice);
	return TUSBD_E_OK;
}


/*
 *  USB DEVICE EP0 SETUP STAGE����
 *  parameter1 pdevice:  USB DEVICE�ϥ�ɥ�
 *  parameter2 psetup:���åȥ��åץǡ���
 *  return     TUSBD_ERCODE
 */
TUSBD_ERCODE
tusbdSetupStage(TUSBD_Handle_t *pdevice, uint8_t *psetup)
{
	UsbSetupRequest  *req = &pdevice->request;
	volatile uint8_t *p = psetup;

	req->bmRequest     = p[0];
	req->bRequest      = p[1];
	req->wValue        = p[2] | (p[3] << 8);
	req->wIndex        = p[4] | (p[5] << 8);
	req->wLength       = p[6] | (p[7] << 8);

	pdevice->ep0_state = TUSBD_EP0_SETUP;
	pdevice->ep0_data_len = pdevice->request.wLength;

	switch (pdevice->request.bmRequest & 0x1F){
	case USB_RECIPIENT_DEVICE:
		if(req->bRequest < NUM_MAX_REQUEST)
			usb_pbRequestDev[req->bRequest](pdevice, req);
		else
			tusbdControlSendStall(pdevice, req);
		break;
	case USB_RECIPIENT_INTERFACE:
		if(pdevice->dev_state == TUSBD_STATE_CONFIGURED){
			if((req->wIndex & 0xFF) <= TUSBD_MAX_NUM_INTERFACES){
				tusbdClassSetup(pdevice, req);
				if(req->wLength == 0){
					tusbdControlSendStatus(pdevice);
				}
			}
		}
		else
			tusbdControlSendStall(pdevice, req);
		break;
	case USB_RECIPIENT_ENDPOINT:
		if((req->bmRequest & USB_REQUEST_TYPE_MASK) == USB_REQUEST_TYPE_CLASS){
			tusbdClassSetup(pdevice, req);
		}
		else{
			if(req->bRequest < NUM_MAX_REQUEST)
				usb_pbRequestEp[req->bRequest](pdevice, req);
			else
				tusbdControlSendStall(pdevice, req);
		}
		break;
	default:
		tusbdControlSendStall(pdevice, req);
		break;
	}
	return TUSBD_E_OK;
}

/*
 *  USB DEVICE DATAOUT(����) STAGE����
 *  parameter1 pdevice:  USB DEVICE�ϥ�ɥ�
 *  parameter2 epnum: ����ɥݥ�����ֹ�
 *  parameter3 pdata: ���ߤμ����ǡ����ݥ���
 *  return    TUSBD_ERCODE
 */
TUSBD_ERCODE
tusbdDataOutStage(TUSBD_Handle_t *pdevice, uint8_t epnum, uint8_t *pdata)
{
	if(epnum == 0){	/* ����ɥݥ����#0 */
		TUSBD_Endpoint_t *pep = &pdevice->ep_out[0];
		if(pdevice->ep0_state == TUSBD_EP0_DATAOUT){
			if(pep->remlength > pep->maxpacket){
				pep->remlength -=  pep->maxpacket;
				if(pep->remlength < pep->maxpacket)
					tusbdControlContinueReceive(pdevice, pdata, pep->remlength);
				else
					tusbdControlContinueReceive(pdevice, pdata, pep->maxpacket);
			}
			else{
				if(pdevice->dev_state == TUSBD_STATE_CONFIGURED){
					tusbdClassEp0RxReady(pdevice);
				}
				tusbdControlSendStatus(pdevice);
			}
		}
	}
	else if(pdevice->dev_state == TUSBD_STATE_CONFIGURED){
		tusbdClassDataOut(pdevice, epnum);
	}
	return TUSBD_E_OK;
}

/*
 *  USB DEVICE DATAIN(����) STAGE����
 *  parameter1 pdevice:  USB DEVICE�ϥ�ɥ�
 *  parameter2 epnum: ����ɥݥ�����ֹ�
 *  parameter3 pdata: ���ߤ������ǡ����ݥ���
 *  return     TUSBD_ERCODE
 */
TUSBD_ERCODE
tusbdDataInStage(TUSBD_Handle_t *pdevice, uint8_t epnum, uint8_t *pdata)
{
	if(epnum == 0){	/* ����ɥݥ����#0 */
		TUSBD_Endpoint_t *pep = &pdevice->ep_in[0];
		if(pdevice->ep0_state == TUSBD_EP0_DATAIN){
			if(pep->remlength > pep->maxpacket){
				pep->remlength -=  pep->maxpacket;
				tusbdControlContinueSendData(pdevice, pdata, pep->remlength);
				/*
				 *  ������������
				 */
				tusbdDriverSetupReceive(pdevice, 0, NULL, 0);
			}
			else{ /* ZERO LENGTH PACKET�������� */
				if((pep->xfersize % pep->maxpacket) == 0 && pep->xfersize >= pep->maxpacket
						&& pep->xfersize < pdevice->ep0_data_len){
					tusbdControlContinueSendData(pdevice , NULL, 0);
					pdevice->ep0_data_len = 0;
					/*
					 *  ������������
					 */
					tusbdDriverSetupReceive(pdevice, 0, NULL, 0);
				}
				else{
					if(pdevice->dev_state == TUSBD_STATE_CONFIGURED){
						tusbdClassEp0TxSent(pdevice);
					}
					tusbdControlReceiveStatus(pdevice);
				}
			}
		}
		if(pdevice->dev_test_mode == 1){
			tusbdDriverTestMode(pdevice);
			pdevice->dev_test_mode = 0;
		}
	}
	else if(pdevice->dev_state == TUSBD_STATE_CONFIGURED){
		tusbdClassDataIn(pdevice, epnum);
	}
	return TUSBD_E_OK;
}

/*
 *  USB DEVICE RESET����
 *  parameter1 pdevice:  USB DEVICE�ϥ�ɥ�
 *  return    TUSBD_ERCODE
 */

TUSBD_ERCODE
tusbdReset(TUSBD_Handle_t  *pdevice)
{
	/*
	 *  ����ɥݥ����#0 �����ץ�
	 */
	tusbdDriverOpenEp(pdevice, 0x00, USB_EP_TYPE_CTRL, USB_MAX_EP0_SIZE);
	pdevice->ep_out[0].maxpacket = USB_MAX_EP0_SIZE;
	tusbdDriverOpenEp(pdevice, 0x80, USB_EP_TYPE_CTRL, USB_MAX_EP0_SIZE);
	pdevice->ep_in[0].maxpacket = USB_MAX_EP0_SIZE;

	/*
	 *  �ǥХ����桼�������ơ��Ȥ�ǥե���Ȥ�
	 */
	pdevice->dev_state = TUSBD_STATE_INIT;

	/*
	 *  �ǥХ������饹��ꥻ�å�
	 */
	return tusbdClassDeInit(pdevice, pdevice->dev_config);
}


/*
 *  USB DEVICE SETUP SPEED����
 *  parameter1 pdevice:  USB DEVICE�ϥ�ɥ�
 *  parameter2 speed: �ǥХ������ԡ���
 *  return     TUSBD_ERCODE
 */
TUSBD_ERCODE
tusbdSetSpeed(TUSBD_Handle_t  *pdevice, uint8_t speed)
{
	pdevice->dev_speed = speed & 3;
	if(pdevice->makeDescriptor != NULL)
		pdevice->makeDescriptor(pdevice->dev_speed);
	return TUSBD_E_OK;
}

/*
 *  USB DEVICE SUSPEND REQUEST����
 *  parameter1 pdevice:  USB DEVICE�ϥ�ɥ�
 *  return     TUSBD_ERCODE
 */
TUSBD_ERCODE
tusbdSuspend(TUSBD_Handle_t *pdevice)
{
	pdevice->dev_old_state =  pdevice->dev_state;
	pdevice->dev_state  = TUSBD_STATE_SUSPENDED;
	tusbdClassSuspend(pdevice);
	return TUSBD_E_OK;
}

/*
 *  USB DEVICE RESUME REQUEST����
 *  parameter1 pdevice:  USB DEVICE�ϥ�ɥ�
 *  return     TUSBD_ERCODE
 */

TUSBD_ERCODE
tusbdResume(TUSBD_Handle_t *pdevice)
{
	pdevice->dev_state = pdevice->dev_old_state;
	tusbdClassResume(pdevice);
	return TUSBD_E_OK;
}

/*
 *  USB DEVICE ISOIN INCOMPLETE����
 *  parameter1 pdevice:  USB DEVICE�ϥ�ɥ�
 *  parameter2 epnum: ����ɥݥ�����ֹ�
 *  return     TUSBD_ERCODE
 */
TUSBD_ERCODE
tusbdIsoInIncomplete(TUSBD_Handle_t *pdevice, uint8_t epnum)
{
	tusbdClassInIncomlete(pdevice, epnum);
	return TUSBD_E_OK;
}

/*
 *  USB DEVICE ISOOUT INCOMPLETE����
 *  parameter1 pdevice:  USB DEVICE�ϥ�ɥ�
 *  parameter2 epnum: ����ɥݥ�����ֹ�
 *  return     TUSBD_ERCODE
 */
TUSBD_ERCODE
tusbdIsoOutIncomplete(TUSBD_Handle_t *pdevice, uint8_t epnum)
{
	tusbdClassOutIncomlete(pdevice, epnum);
	return TUSBD_E_OK;
}

/*
 *  USB DEVICE CONNECTED����
 *  parameter1 pdevice:  USB DEVICE�ϥ�ɥ�
 *  return     TUSBD_ERCODE
 */
TUSBD_ERCODE
tusbdDeviceConnected(TUSBD_Handle_t *pdevice)
{
	return TUSBD_E_OK;
}

/*
 *  USB DEVICE DISCONNECTED����
 *  parameter1 pdevice:  USB DEVICE�ϥ�ɥ�
 *  return     TUSBD_ERCODE
 */
TUSBD_ERCODE
tusbdDeviceDisconnected(TUSBD_Handle_t *pdevice)
{
	/* Free Class Resources */
	pdevice->dev_state = TUSBD_STATE_INIT;
	tusbdClassDeInit(pdevice, pdevice->dev_config);
	return TUSBD_E_OK;
}

/*
 *  USB DEVICE SEARCH DIECRIPTOR
 *  parameter1 pdevice:  USB DEVICE�ϥ�ɥ�
 *  parameter2 type:     ����DESCRIPTOR������
 *  return     DESCRIPTOR�ݥ���
 */
uint8_t *
tusbdFindDescriptor(TUSBD_Handle_t *pdevice, uint8_t type)
{
	uint16_t wTotalLength;
	uint8_t *cptr = TUSBD_FS_CONFIG_DESCRIPTOR;
	uint8_t *dptr;

    if(cptr == NULL)
		return NULL;
	if(cptr[0] != CONFIGURATION_DESCRIPTOR_LENGTH || cptr[1] != CONFIGURATION_DESCRIPTOR)
		return NULL;

	wTotalLength = cptr[2] | (cptr[3] << 8);
    if(wTotalLength <= (CONFIGURATION_DESCRIPTOR_LENGTH+2))
		return NULL;

	dptr = cptr + CONFIGURATION_DESCRIPTOR_LENGTH;
    do{
		if(dptr[1] == type)
			return dptr;
		dptr += dptr[0];	/* bLength */
	}while(dptr < (cptr + wTotalLength));
    return NULL;
}


