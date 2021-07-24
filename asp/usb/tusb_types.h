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
 *  @(#) $Id: tusb_types.h 698 2017-09-09 18:12:15Z roi $
 */
/* mbed USBHost Library
 * Copyright (c) 2006-2013 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
/*
 *  USB Host/Device Middleware ���������
 */

#ifndef _TUSB_TYPES_H_
#define _TUSB_TYPES_H_


#ifdef __cplusplus
 extern "C" {
#endif

/* Control RequestType Fields */
#define USB_DEVICE_TO_HOST         0x80
#define USB_HOST_TO_DEVICE         0x00
#define USB_REQUEST_DIR_MASK       USB_DEVICE_TO_HOST
#define USB_REQUEST_TYPE_VENDOR    0x40
#define USB_REQUEST_TYPE_CLASS     0x20
#define USB_REQUEST_TYPE_STANDARD  0x00
#define USB_RECIPIENT_OTHER        0x03
#define USB_RECIPIENT_ENDPOINT     0x02
#define USB_RECIPIENT_INTERFACE    0x01
#define USB_RECIPIENT_DEVICE       0x00

/* USB Standard Requests */
#define GET_STATUS                 0x00
#define CLEAR_FEATURE              0x01
#define SET_FEATURE                0x03
#define SET_ADDRESS                0x05
#define GET_DESCRIPTOR             0x06
#define SET_DESCRIPTOR             0x07
#define GET_CONFIGURATION          0x08
#define SET_CONFIGURATION          0x09
#define GET_INTERFACE              0x0A
#define SET_INTERFACE              0x0B

/* Descriptor Types of USB Specifications */
#define DEVICE_DESCRIPTOR              (1)
#define CONFIGURATION_DESCRIPTOR       (2)
#define STRING_DESCRIPTOR              (3)
#define INTERFACE_DESCRIPTOR           (4)
#define ENDPOINT_DESCRIPTOR            (5)
#define DEVICE_QUALIFIER_DESCRIPTOR    (6)
#define OTHER_SPEED_CONFIGURATION_DESC (7)
#define BOS_DESCRIPTOR                 (15)
#define HID_DESCRIPTOR                 (33)
#define HID_REPORT_DESCRIPTOR          (34)

#define SET_CONFIGURATION_TYPE 	   (USB_HOST_TO_DEVICE | USB_RECIPIENT_DEVICE | USB_REQUEST_TYPE_STANDARD)
#define SET_ADDRESS_TYPE           (USB_HOST_TO_DEVICE | USB_RECIPIENT_DEVICE | USB_REQUEST_TYPE_STANDARD)
#define SET_INTERFACE_TYPE         (USB_HOST_TO_DEVICE | USB_RECIPIENT_INTERFACE | USB_REQUEST_TYPE_STANDARD)
#define CLEAR_FEATURE_TYPE         (USB_HOST_TO_DEVICE | USB_RECIPIENT_ENDPOINT | USB_REQUEST_TYPE_STANDARD)

/* USB Descriptor Length */
#define DEVICE_DESCRIPTOR_LENGTH        0x12
#define CONFIGURATION_DESCRIPTOR_LENGTH 0x09
#define INTERFACE_DESCRIPTOR_LENGTH     0x09
#define ENDPOINT_DESCRIPTOR_LENGTH      0x07
#define HID_DESCRIPTOR_LENGTH           0x09


#define USB_EP_TYPE_CTRL           0x00
#define USB_EP_TYPE_ISOC           0x01
#define USB_EP_TYPE_BULK           0x02
#define USB_EP_TYPE_INTR           0x03

#define USB_PID_SETUP              0
#define USB_PID_DATA               1

/* Class code */
#define AUDIO_CLASS                0x01
#define CDC_CLASS                  0x02
#define HID_CLASS                  0x03
#define PRT_CLASS                  0x07
#define MSC_CLASS                  0x08
#define HUB_CLASS                  0x09
#define DATA_INTERFACE_CLASS       0x0A

/* usb speed */
#define USB_DEVICE_SPEED_HIGH      0
#define USB_DEVICE_SPEED_FULL      1
#define USB_DEVICE_SPEED_LOW       2


#define SETUP_PKT_LENGTH           8

#define MAX_DEVICE_CONNECTED       4
#ifndef MAX_EPS_PIPES
#define MAX_EPS_PIPES              32
#endif

#define TUSBH_MAX_ERROR_COUNT      2
#define TUSBH_STR_LENGTH           64

typedef struct {
    uint8_t   bLength;
    uint8_t   bDescriptorType;
    uint16_t  bcdUSB;
    uint8_t   bDeviceClass;
    uint8_t   bDeviceSubClass; 
    uint8_t   bDeviceProtocol;
    uint8_t   bMaxPacketSize;
    uint16_t  idVendor;
    uint16_t  idProduct;
    uint16_t  bcdDevice;
    uint8_t   iManufacturer;
    uint8_t   iProduct;
    uint8_t   iSerialNumber;
    uint8_t   bNumConfigurations;
} DeviceDescriptor;

typedef struct {
    uint8_t   bLength;
    uint8_t   bDescriptorType;
    uint16_t  wTotalLength;
    uint8_t   bNumInterfaces;
    uint8_t   bConfigurationValue;
    uint8_t   iConfiguration;
    uint8_t   bmAttributes;
    uint8_t   bMaxPower;
} ConfigurationDescriptor;

typedef struct {
	uint8_t   bLength;
	uint8_t   bDescriptorType;
	uint8_t   bInterfaceNumber;
	uint8_t   bAlternateSetting;
	uint8_t   bNumEndpoints;
	uint8_t   bInterfaceClass;
	uint8_t   bInterfaceSubClass;
	uint8_t   bInterfaceProtocol;
	uint8_t   iInterface;
} InterfaceDescriptor;

typedef struct {
    uint8_t   bLength;
    uint8_t   bDescriptorType;
    uint8_t   bEndpointAddress;
    uint8_t   bmAttributes;
    uint16_t  wMaxPacketSize;
    uint8_t   bInterval;
} EndpointDescriptor;

typedef struct {
	uint8_t   bDescLength;
	uint8_t   bDescriptorType;
	uint8_t   bNbrPorts;
	uint8_t   wHubCharacteristics[2];
	uint8_t   bPwrOn2PwrGood;
	uint8_t   bHubContrCurrent;
	uint8_t   DeviceRemovable;
	uint8_t   PortPweCtrlMak;
} HubDescriptor;

typedef struct {
	uint8_t   bmRequest;
	uint8_t   bRequest;
	uint16_t  wValue;
	uint16_t  wIndex;
	uint16_t  wLength;
} UsbSetupRequest;

typedef struct {
    uint32_t  baudrate;
    uint8_t   stop_bits;	/* 0-1 Stop bit 1-1.5 Stop bits 2-2 Stop bits */
    uint8_t   parity;		/* 0:None 1:Odd 2:Even 3:Mark 4:Space */
    uint8_t   data_bits;
} Line_Coding_t;


/*
 *  TUSBH ERROR CODE��
 */
typedef enum {
	TUSBH_E_OK = 0,
	TUSBH_E_BUSY,
	TUSBH_E_BUSY_URB,
	TUSBH_E_PAR,
	TUSBH_E_OBJ,
	TUSBH_E_ERR,
	TUSBH_E_URB,
	TUSBH_E_RTOS,
	TUSBH_E_NOSPT,
	TUSBH_E_TMOUT,
	TUSBH_E_SYS,
} TUSBH_ERCODE;

/*
 *  TUSBD ERROR CODE��
 */
typedef enum {
	TUSBD_E_OK = 0,
	TUSBD_E_BUSY,
	TUSBD_E_ERR
} TUSBD_ERCODE;

#ifdef __cplusplus
}
#endif

#endif	/* _TUSB_TYPES_H_ */

