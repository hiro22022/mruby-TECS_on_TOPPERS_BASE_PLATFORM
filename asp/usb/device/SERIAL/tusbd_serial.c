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
 *  @(#) $Id: tusbd_serial.c 698 2017-10-15 15:14:19Z roi $
 */
/*
 *  USB Device Middleware CDC:SERIAL CLASS��
 */

#include "tusbd_serial.h"


#define CDC_INTERVAL_TIME    16

/*
 *  USB CDC HS DEVICE CONFIGURATION DESCRIPTOR
 */
uint8_t configurationCdcHsDescriptor[TOTAL_CDC_DESCRIPTOR_LENGTH] __attribute__ ((aligned (USB_DATA_ALIGN))) =
{
	CONFIGURATION_DESCRIPTOR_LENGTH,	// bLength
	CONFIGURATION_DESCRIPTOR,			// bDescriptorType
	(TOTAL_CDC_DESCRIPTOR_LENGTH & 0xFF),   // wTotalLength (LSB)
	(TOTAL_CDC_DESCRIPTOR_LENGTH >> 8),     // wTotalLength (MSB)
	0x02,                               // bNumInterfaces
	0x01,                               // bConfigurationValue
	0x00,                               // iConfiguration
	C_RESERVED | C_SELF_POWERED,        // bmAttributes
	C_POWER(100),                       // bMaxPower

	INTERFACE_DESCRIPTOR_LENGTH,        // bLength
	INTERFACE_DESCRIPTOR,               // bDescriptorType
	0x00,                               // bInterfaceNumber
	0x00,                               // bAlternateSetting
	0x01,                               // bNumEndpoints
	CDC_CLASS,                          // bInterfaceClass
	0x02,                               // bInterfaceSubClass
	0x01,                               // bInterfaceProtocol
	0x00,                               // iInterface

	/* CDC Header Functional Descriptor, CDC Spec 5.2.3.1, Table 26 */
 	0x05,                               // bFunctionLength
	0x24,                               // bDescriptorType
	0x00,                               // bDescriptorSubtype
	0x10, 0x01,                         // bcdCDC
	/* Call Management Functional Descriptor, CDC Spec 5.2.3.2, Table 27 */
	0x05,                               // bFunctionLength
	0x24,                               // bDescriptorType
	0x01,                               // bDescriptorSubtype
	0x03,                               // bmCapabilities
	0x01,                               // bDataInterface
	/* Abstract Control Management Functional Descriptor, CDC Spec 5.2.3.3, Table 28 */
	0x04,                               // bFunctionLength
	0x24,                               // bDescriptorType
	0x02,                               // bDescriptorSubtype
	0x06,                               // bmCapabilities
	/* Union Functional Descriptor, CDC Spec 5.2.3.8, Table 33 */
	0x05,                               // bFunctionLength
	0x24,                               // bDescriptorType
	0x06,                               // bDescriptorSubtype
	0x00,                               // bMasterInterface
	0x01,                               // bSlaveInterface0

	ENDPOINT_DESCRIPTOR_LENGTH,         // bLength
	ENDPOINT_DESCRIPTOR,                // bDescriptorType
	CDC_EPINT_ADDR,                     // bEndpointAddress
	USB_EP_TYPE_INTR,                   // bmAttributes
	(CDC_PACKET_SIZE_EPINT & 0xFF),     // wMaxPacketSize (LSB)
	(CDC_PACKET_SIZE_EPINT >> 8),       // wMaxPacketSize (MSB)
	CDC_INTERVAL_TIME,                  // bInterval

	INTERFACE_DESCRIPTOR_LENGTH,        // bLength
	INTERFACE_DESCRIPTOR,               // bDescriptorType
	0x01,                               // bInterfaceNumber
	0x00,                               // bAlternateSetting
	0x02,                               // bNumEndpoints
	DATA_INTERFACE_CLASS,               // bInterfaceClass
	0x00,                               // bInterfaceSubClass
	0x00,                               // bInterfaceProtocol
	0x00,                               // iInterface

	ENDPOINT_DESCRIPTOR_LENGTH,         // bLength
	ENDPOINT_DESCRIPTOR,                // bDescriptorType
	CDC_EPBULKOUT_ADDR,                 // bEndpointAddress
	USB_EP_TYPE_BULK,                   // bmAttributes
	(MAX_PACKET_SIZE_HS_EPBULK & 0xFF), // wMaxPacketSize (LSB)
	(MAX_PACKET_SIZE_HS_EPBULK >> 8),   // wMaxPacketSize (MSB)
	0x00,                               // bInterval (milliseconds)

	ENDPOINT_DESCRIPTOR_LENGTH,         // bLength
	ENDPOINT_DESCRIPTOR,                // bDescriptorType
	CDC_EPBULKIN_ADDR,                  // bEndpointAddress
	USB_EP_TYPE_BULK,                   // bmAttributes
	(MAX_PACKET_SIZE_HS_EPBULK & 0xFF), // wMaxPacketSize (LSB)
	(MAX_PACKET_SIZE_HS_EPBULK >> 8),   // wMaxPacketSize (MSB)
	0x00                                // bInterval (milliseconds)
};

/*
 *  USB CDC FS DEVICE CONFIGURATION DESCRIPTOR
 */
uint8_t configurationCdcFsDescriptor[TOTAL_CDC_DESCRIPTOR_LENGTH] __attribute__ ((aligned (USB_DATA_ALIGN))) =
{
  /*Configuration Descriptor*/
	CONFIGURATION_DESCRIPTOR_LENGTH,	// bLength
	CONFIGURATION_DESCRIPTOR,			// bDescriptorType
	(TOTAL_CDC_DESCRIPTOR_LENGTH & 0xFF),   // wTotalLength (LSB)
	(TOTAL_CDC_DESCRIPTOR_LENGTH >> 8),     // wTotalLength (MSB)
	0x02,                               // bNumInterfaces
	0x01,                               // bConfigurationValue
	0x00,                               // iConfiguration
	C_RESERVED | C_SELF_POWERED,        // bmAttributes
	C_POWER(100),                       // bMaxPower

	INTERFACE_DESCRIPTOR_LENGTH,        // bLength
	INTERFACE_DESCRIPTOR,               // bDescriptorType
	0x00,                               // bInterfaceNumber
	0x00,                               // bAlternateSetting
	0x01,                               // bNumEndpoints
	CDC_CLASS,                          // bInterfaceClass
	0x02,                               // bInterfaceSubClass
	0x01,                               // bInterfaceProtocol
	0x00,                               // iInterface

	/* CDC Header Functional Descriptor, CDC Spec 5.2.3.1, Table 26 */
 	0x05,                               // bFunctionLength
	0x24,                               // bDescriptorType
	0x00,                               // bDescriptorSubtype
	0x10, 0x01,                         // bcdCDC
	/* Call Management Functional Descriptor, CDC Spec 5.2.3.2, Table 27 */
	0x05,                               // bFunctionLength
	0x24,                               // bDescriptorType
	0x01,                               // bDescriptorSubtype
	0x03,                               // bmCapabilities
	0x01,                               // bDataInterface
	/* Abstract Control Management Functional Descriptor, CDC Spec 5.2.3.3, Table 28 */
	0x04,                               // bFunctionLength
	0x24,                               // bDescriptorType
	0x02,                               // bDescriptorSubtype
	0x06,                               // bmCapabilities
	/* Union Functional Descriptor, CDC Spec 5.2.3.8, Table 33 */
	0x05,                               // bFunctionLength
	0x24,                               // bDescriptorType
	0x06,                               // bDescriptorSubtype
	0x00,                               // bMasterInterface
	0x01,                               // bSlaveInterface0

	ENDPOINT_DESCRIPTOR_LENGTH,         // bLength
	ENDPOINT_DESCRIPTOR,                // bDescriptorType
	CDC_EPINT_ADDR,                     // bEndpointAddress
	USB_EP_TYPE_INTR,                   // bmAttributes
	(CDC_PACKET_SIZE_EPINT & 0xFF),     // wMaxPacketSize (LSB)
	(CDC_PACKET_SIZE_EPINT >> 8),       // wMaxPacketSize (MSB)
	CDC_INTERVAL_TIME,                  // bInterval

	INTERFACE_DESCRIPTOR_LENGTH,        // bLength
	INTERFACE_DESCRIPTOR,               // bDescriptorType
	0x01,                               // bInterfaceNumber
	0x00,                               // bAlternateSetting
	0x02,                               // bNumEndpoints
	DATA_INTERFACE_CLASS,               // bInterfaceClass
	0x00,                               // bInterfaceSubClass
	0x00,                               // bInterfaceProtocol
	0x00,                               // iInterface

	ENDPOINT_DESCRIPTOR_LENGTH,         // bLength
	ENDPOINT_DESCRIPTOR,                // bDescriptorType
	CDC_EPBULKOUT_ADDR,                 // bEndpointAddress
	USB_EP_TYPE_BULK,                   // bmAttributes
	(MAX_PACKET_SIZE_FS_EPBULK & 0xFF), // wMaxPacketSize (LSB)
	(MAX_PACKET_SIZE_FS_EPBULK >> 8),   // wMaxPacketSize (MSB)
	0x00,                               // bInterval (milliseconds)

	ENDPOINT_DESCRIPTOR_LENGTH,         // bLength
	ENDPOINT_DESCRIPTOR,                // bDescriptorType
	CDC_EPBULKIN_ADDR,                  // bEndpointAddress
	USB_EP_TYPE_BULK,                   // bmAttributes
	(MAX_PACKET_SIZE_FS_EPBULK & 0xFF), // wMaxPacketSize (LSB)
	(MAX_PACKET_SIZE_FS_EPBULK >> 8),   // wMaxPacketSize (MSB)
	0x00                                // bInterval (milliseconds)
} ;

/*
 *  USB CDC OTHER DEVICE CONFIGURATION DESCRIPTOR
 */
uint8_t configurationCdcOtrDescriptor[TOTAL_CDC_DESCRIPTOR_LENGTH] __attribute__ ((aligned (USB_DATA_ALIGN))) =
{
	CONFIGURATION_DESCRIPTOR_LENGTH,	// bLength
	OTHER_SPEED_CONFIGURATION_DESC,     // bDescriptorType
	(TOTAL_CDC_DESCRIPTOR_LENGTH & 0xFF),   // wTotalLength (LSB)
	(TOTAL_CDC_DESCRIPTOR_LENGTH >> 8),     // wTotalLength (MSB)
	0x02,                               // bNumInterfaces
	0x01,                               // bConfigurationValue
	0x04,                               // iConfiguration
	C_RESERVED | C_SELF_POWERED,        // bmAttributes
	C_POWER(100),                       // bMaxPower

	INTERFACE_DESCRIPTOR_LENGTH,        // bLength
	INTERFACE_DESCRIPTOR,               // bDescriptorType
	0x00,                               // bInterfaceNumber
	0x00,                               // bAlternateSetting
	0x01,                               // bNumEndpoints
	CDC_CLASS,                          // bInterfaceClass
	0x02,                               // bInterfaceSubClass
	0x01,                               // bInterfaceProtocol
	0x00,                               // iInterface

	/* CDC Header Functional Descriptor, CDC Spec 5.2.3.1, Table 26 */
 	0x05,                               // bFunctionLength
	0x24,                               // bDescriptorType
	0x00,                               // bDescriptorSubtype
	0x10, 0x01,                         // bcdCDC
	/* Call Management Functional Descriptor, CDC Spec 5.2.3.2, Table 27 */
	0x05,                               // bFunctionLength
	0x24,                               // bDescriptorType
	0x01,                               // bDescriptorSubtype
	0x03,                               // bmCapabilities
	0x01,                               // bDataInterface
	/* Abstract Control Management Functional Descriptor, CDC Spec 5.2.3.3, Table 28 */
	0x04,                               // bFunctionLength
	0x24,                               // bDescriptorType
	0x02,                               // bDescriptorSubtype
	0x06,                               // bmCapabilities
	/* Union Functional Descriptor, CDC Spec 5.2.3.8, Table 33 */
	0x05,                               // bFunctionLength
	0x24,                               // bDescriptorType
	0x06,                               // bDescriptorSubtype
	0x00,                               // bMasterInterface
	0x01,                               // bSlaveInterface0

	ENDPOINT_DESCRIPTOR_LENGTH,         // bLength
	ENDPOINT_DESCRIPTOR,                // bDescriptorType
	CDC_EPINT_ADDR,                     // bEndpointAddress
	USB_EP_TYPE_INTR,                   // bmAttributes
	(CDC_PACKET_SIZE_EPINT & 0xFF),     // wMaxPacketSize (LSB)
	(CDC_PACKET_SIZE_EPINT >> 8),       // wMaxPacketSize (MSB)
	CDC_INTERVAL_TIME,                  // bInterval

	INTERFACE_DESCRIPTOR_LENGTH,        // bLength
	INTERFACE_DESCRIPTOR,               // bDescriptorType
	0x01,                               // bInterfaceNumber
	0x00,                               // bAlternateSetting
	0x02,                               // bNumEndpoints
	DATA_INTERFACE_CLASS,               // bInterfaceClass
	0x00,                               // bInterfaceSubClass
	0x00,                               // bInterfaceProtocol
	0x00,                               // iInterface

	ENDPOINT_DESCRIPTOR_LENGTH,         // bLength
	ENDPOINT_DESCRIPTOR,                // bDescriptorType
	CDC_EPBULKOUT_ADDR,                 // bEndpointAddress
	USB_EP_TYPE_BULK,                   // bmAttributes
	(MAX_PACKET_SIZE_FS_EPBULK & 0xFF), // wMaxPacketSize (LSB)
	(MAX_PACKET_SIZE_FS_EPBULK >> 8),   // wMaxPacketSize (MSB)
	0x00,                               // bInterval (milliseconds)

	ENDPOINT_DESCRIPTOR_LENGTH,         // bLength
	ENDPOINT_DESCRIPTOR,                // bDescriptorType
	CDC_EPBULKIN_ADDR,                  // bEndpointAddress
	USB_EP_TYPE_BULK,                   // bmAttributes
	(MAX_PACKET_SIZE_FS_EPBULK & 0xFF), // wMaxPacketSize (LSB)
	(MAX_PACKET_SIZE_FS_EPBULK >> 8),   // wMaxPacketSize (MSB)
	0x00                                // bInterval (milliseconds)
};

static TUSBD_CDC_Handle_t CDC_CLASS_DATA  __attribute__ ((aligned (USB_DATA_ALIGN)));


/*
 *  USB DEVICE CDC�����
 *  parameter1 pdevice:  USB DEVICE�ϥ�ɥ�
 *  parameter2 idx:      Configuration index
 *  return     TUSBD_ERCODE
 */
TUSBD_ERCODE
tusbdCdcInit(TUSBD_Handle_t *pdevice, uint8_t idx)
{
	TUSBD_CDC_Handle_t *hcdc;

	memset(&CDC_CLASS_DATA, 0, sizeof(TUSBD_CDC_Handle_t));
	pdevice->pClassData = &CDC_CLASS_DATA;
	hcdc = (TUSBD_CDC_Handle_t*) pdevice->pClassData;
	if(pdevice->dev_speed == USB_DEVICE_SPEED_HIGH)
		hcdc->packetsize = MAX_PACKET_SIZE_HS_EPBULK;
	else
		hcdc->packetsize = MAX_PACKET_SIZE_FS_EPBULK;
	hcdc->TxState = 0;
	hcdc->CmdCode = 0xFF;

	/*
	 *  ENDPOINT OPEN
	 */
	tusbdDriverOpenEp(pdevice, CDC_EPBULKIN_ADDR, USB_EP_TYPE_BULK, hcdc->packetsize);
	tusbdDriverOpenEp(pdevice, CDC_EPBULKOUT_ADDR, USB_EP_TYPE_BULK, hcdc->packetsize);
	tusbdDriverOpenEp(pdevice, CDC_EPINT_ADDR, USB_EP_TYPE_INTR, CDC_PACKET_SIZE_EPINT);

	/*
	 *  SETUP RECEIVED PACKET AREA
	 */
	tusbdCdcSetReceivePacket(pdevice);
	return TUSBD_E_OK;
}

/*
 *  USB DEVICE HID���饹��λ
 *  parameter1 pdevice:  USB DEVICE�ϥ�ɥ�
 *  parameter2 idx:      Configuration index
 *  return     TUSBD_ERCODE
 */
TUSBD_ERCODE
tusbdCdcDeInit(TUSBD_Handle_t *pdevice, uint8_t cfgidx)
{
	/*
	 * ENDPOINT CLOSE
	 */
	tusbdDriverCloseEp(pdevice, CDC_EPBULKIN_ADDR);
	tusbdDriverCloseEp(pdevice, CDC_EPBULKOUT_ADDR);
	tusbdDriverCloseEp(pdevice, CDC_EPINT_ADDR);
	return TUSBD_E_OK;
}

/*
 *  USB DEVICE CDC���饹 EP0����READY
 *  parameter1 pdevice:  USB DEVICE�ϥ�ɥ�
 *  return     TUSBD_ERCODE
 */
void
tusbdCdcEP0RxReady(TUSBD_Handle_t *pdevice)
{
	TUSBD_CDC_Handle_t *hcdc = (TUSBD_CDC_Handle_t*)pdevice->pClassData;
	void (*func)(TUSBD_CDC_Handle_t *, uint8_t, uint32_t);

	if(((func = pdevice->pUsrData) != NULL) && (hcdc->CmdCode != 0xFF)){
		func(hcdc, hcdc->CmdCode, hcdc->CmdLength);
		hcdc->CmdCode = 0xFF;
	}
}

/*
 *  USB DEVICE CDC���饹���åȥ��å�
 *  parameter1 pdevice:  USB DEVICE�ϥ�ɥ�
 *  parameter2 req:      usb control requests
 *  return     TUSBD_ERCODE
 */
void
tusbdCdcSetup(TUSBD_Handle_t *pdevice, UsbSetupRequest *req)
{
	TUSBD_CDC_Handle_t *hcdc = (TUSBD_CDC_Handle_t*) pdevice->pClassData;
	void (*func)(TUSBD_CDC_Handle_t *, uint8_t, uint32_t);

	func = pdevice->pUsrData;
	switch (req->bmRequest & USB_REQUEST_TYPE_MASK){
	case USB_REQUEST_TYPE_CLASS:
		if(req->wLength){
			if(req->bmRequest & USB_DEVICE_TO_HOST){
				if(func != NULL)
					func(hcdc, req->bRequest, req->wLength);
				tusbdControlSendData(pdevice, (uint8_t *)hcdc->cmddata, req->wLength);
			}
			else{
				hcdc->CmdCode   = req->bRequest;
				hcdc->CmdLength = req->wLength;
				tusbdControlReceiveStart(pdevice, (uint8_t *)hcdc->cmddata, req->wLength);
			}
		}
		else if(func != NULL)
			func(hcdc, req->bRequest, 0);
		break;
	case USB_REQUEST_TYPE_STANDARD:
		switch(req->bRequest){
		case GET_INTERFACE:
			pdevice->devData[0] = 0;
			tusbdControlSendData(pdevice, (uint8_t *)pdevice->devData, 1);
			break;
		case SET_INTERFACE:
		default:
			break;
		}
	default:
		break;
	}
}

/*
 *  USB DEVICE CDC���饹�ǡ�������
 *  parameter1 pdevice:  USB DEVICE�ϥ�ɥ�
 *  parameter2 epnum:    Endpoint#
 *  return     TUSBD_ERCODE
 */
void
tusbdCdcDataIn(TUSBD_Handle_t *pdevice, uint8_t epnum)
{
	TUSBD_CDC_Handle_t *hcdc = (TUSBD_CDC_Handle_t*) pdevice->pClassData;

	if(pdevice->pClassData != NULL){
		hcdc->TxState = 0;
	}
}

/*
 *  USB DEVICE CDC���饹�ǡ���������
 *  parameter1 pdevice:  USB DEVICE�ϥ�ɥ�
 *  parameter2 epnum:    Endpoint#
 *  return     TUSBD_ERCODE
 */
void
tusbdCdcDataOut(TUSBD_Handle_t *pdevice, uint8_t epnum)
{
	TUSBD_CDC_Handle_t *hcdc = (TUSBD_CDC_Handle_t*) pdevice->pClassData;
	void (*func)(TUSBD_CDC_Handle_t *, uint8_t, uint32_t);

	hcdc->RxLength = tusbdDriverGetRxDataSize(pdevice, epnum);
	if((func = pdevice->pUsrData) != NULL){
		func(hcdc, CDC_RECEIVED, hcdc->RxLength);
	}
}

/*
 *  USB DEVICE CDC START SENS DATA
 *  parameter1 pdevice:  USB DEVICE�ϥ�ɥ�
 *  parameter2 pbuff:    �����ǡ���
 *  parameter2 length:   �����ǡ���Ĺ
 *  return     TUSBD_ERCODE
 */
TUSBD_ERCODE
tusbdCdcStartTransmit(TUSBD_Handle_t *pdevice, uint8_t *pbuff, uint32_t length)
{
	TUSBD_CDC_Handle_t *hcdc = (TUSBD_CDC_Handle_t*)pdevice->pClassData;

	if(pdevice->pClassData != NULL){
		if(hcdc->TxState == 0){
			hcdc->TxState = 1;
			hcdc->TxBuffer = pbuff;
			hcdc->TxLength = length;

			tusbCpuLock();
			tusbdDriverStartTransmit(pdevice, CDC_EPBULKIN_ADDR, hcdc->TxBuffer, hcdc->TxLength);
			tusbCpuUnLock();
			return TUSBD_E_OK;
		}
		else
			return TUSBD_E_BUSY;
	}
	else
		return TUSBD_E_ERR;
}

/*
 *  USB DEVICE CDC SETUP RECEVE PACKET
 *  parameter1 pdevice:  USB DEVICE�ϥ�ɥ�
 *  return     TUSBD_ERCODE
 */
TUSBD_ERCODE
tusbdCdcSetReceivePacket(TUSBD_Handle_t *pdevice)
{
	TUSBD_CDC_Handle_t *hcdc = (TUSBD_CDC_Handle_t*)pdevice->pClassData;

	if(pdevice->pClassData != NULL){
		tusbCpuLock();
		tusbdDriverSetupReceive(pdevice, CDC_EPBULKOUT_ADDR, hcdc->rxdata, hcdc->packetsize);
		tusbCpuUnLock();
		return TUSBD_E_OK;
	}
	else
		return TUSBD_E_ERR;
}

