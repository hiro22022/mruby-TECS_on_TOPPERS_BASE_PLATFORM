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
 *  @(#) $Id: tusbd_hid.h 698 2017-10-10 18:37:30Z roi $
 */
/* Copyright (c) 2010-2011 mbed.org, MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 * and associated documentation files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/*
 *  USB Device HID CLASSS�����
 */

#ifndef _TUSBD_HID_H_
#define _TUSBD_HID_H_

#ifdef __cplusplus
 extern "C" {
#endif

#include  "tusbd_base.h"


#define EPIN_ADDR                 0x81
#define EPIN_SIZE                 0x04

#define TOTAL_HID_DESCRIPTOR_LENGTH ((1 * CONFIGURATION_DESCRIPTOR_LENGTH) \
                               + (1 * INTERFACE_DESCRIPTOR_LENGTH) \
                               + (1 * HID_DESCRIPTOR_LENGTH) \
                               + (1 * ENDPOINT_DESCRIPTOR_LENGTH))

#define NUM_MAX_OUTREPORT         128

#define USB_GET_REPORT            0x1
#define USB_GET_IDLE              0x2
#define USB_GET_PROTOCOL          0x3
#define USB_SET_REPORT            0x9
#define USB_SET_IDLE              0xA
#define USB_SET_PROTOCOL          0xB


/* Main items */
#define INPUT(size)               (0x80 | size)
#define OUTPUT(size)              (0x90 | size)
#define FEATURE(size)             (0xb0 | size)
#define COLLECTION(size)          (0xa0 | size)
#define END_COLLECTION(size)      (0xc0 | size)

/* Global items */
#define USAGE_PAGE(size)          (0x04 | size)
#define LOGICAL_MINIMUM(size)     (0x14 | size)
#define LOGICAL_MAXIMUM(size)     (0x24 | size)
#define PHYSICAL_MINIMUM(size)    (0x34 | size)
#define PHYSICAL_MAXIMUM(size)    (0x44 | size)
#define UNIT_EXPONENT(size)       (0x54 | size)
#define UNIT(size)                (0x64 | size)
#define REPORT_SIZE(size)         (0x74 | size)
#define REPORT_ID(size)           (0x84 | size)
#define REPORT_COUNT(size)        (0x94 | size)
#define PUSH(size)                (0xa4 | size)
#define POP(size)                 (0xb4 | size)

/* Local items */
#define USAGE(size)               (0x08 | size)
#define USAGE_MINIMUM(size)       (0x18 | size)
#define USAGE_MAXIMUM(size)       (0x28 | size)
#define DESIGNATOR_INDEX(size)    (0x38 | size)
#define DESIGNATOR_MINIMUM(size)  (0x48 | size)
#define DESIGNATOR_MAXIMUM(size)  (0x58 | size)
#define STRING_INDEX(size)        (0x78 | size)
#define STRING_MINIMUM(size)      (0x88 | size)
#define STRING_MAXIMUM(size)      (0x98 | size)
#define DELIMITER(size)           (0xa8 | size)

/*
 *  HID�ǡ����������
 */
#define HID_DATA_IDLE             0
#define HID_DATA_BUSY             1



typedef struct
{
	uint8_t             hidData[32];
	uint8_t             devReport[NUM_MAX_OUTREPORT];
	uint16_t            devReportLength;
	uint8_t             Protocol;
	uint8_t             IdleState;
	uint8_t             AltSetting;
	uint8_t             datastate;
} TUSBD_HID_Handle_t; 


extern uint8_t configurationHidDescriptor[];
extern uint8_t configurationHidOtherDescriptor[];


TUSBD_ERCODE tusbdHidSendData(TUSBD_Handle_t *pdevice, uint8_t *pdata, uint16_t len);
TUSBD_ERCODE tusbdHidSetupReport(TUSBD_Handle_t *pdevice, uint8_t *report, uint16_t len);

TUSBD_ERCODE tusbdHidInit(TUSBD_Handle_t *pdevice, uint8_t idx);
TUSBD_ERCODE tusbdHidDeInit(TUSBD_Handle_t *pdevice, uint8_t idx);
void         tusbdHidSetup(TUSBD_Handle_t *pdevice, UsbSetupRequest *req);
void         tusbdHidDataIn(TUSBD_Handle_t *pdevice, uint8_t epnum);


#ifdef __cplusplus
}
#endif

#endif /* _TUSBD_HID_H_ */

