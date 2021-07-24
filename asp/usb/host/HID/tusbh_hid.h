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
 *  @(#) $Id: tusbh_hid.h 698 2017-09-17 16:23:27Z roi $
 */
/*
 *  USB Host Middleware HID�����
 */

#ifndef _HUSBH_HID_H_
#define _HUSBH_HID_H_

#include "tusbh_base.h"

#ifdef __cplusplus
 extern "C" {
#endif


#define HID_MIN_POLL            10
#define HID_MAX_REPORT_LENGTH   8

#define HID_MOUSE_TYPE          1
#define HID_KEYBOARD_TYPE       2
#define HID_UNKNOWN_TYPE        0xFF

/*
 *  HID BOOT Interface Descriptor field values
 */
#define HID_BOOT_CODE           0x01
#define HID_KEYBRD_BOOT_CODE    0x01
#define HID_MOUSE_BOOT_CODE     0x02


/*
 *  HID���饹�׵�ID
 */
#define USB_HID_GET_REPORT      0x01
#define USB_HID_GET_IDLE        0x02
#define USB_HID_GET_PROTOCOL    0x03
#define USB_HID_SET_REPORT      0x09
#define USB_HID_SET_IDLE        0x0A
#define USB_HID_SET_PROTOCOL    0x0B

#define HID_SET_TYPE            (USB_HOST_TO_DEVICE | USB_REQUEST_TYPE_CLASS | USB_RECIPIENT_INTERFACE)
#define HID_GET_TYPE            (USB_HOST_TO_DEVICE | USB_REQUEST_TYPE_CLASS | USB_RECIPIENT_INTERFACE | USB_RECIPIENT_ENDPOINT)

/*
 *  HID�������
 */
enum {
	THID_INIT = 0,
	THID_IDLE,
	THID_GETREPORT_WAIT,
	THID_SETREPORT,
	THID_SETREPORT_WAIT,
	THID_GET_DATA,
	THID_POLL,
	THID_CLEARFEATURE_WAIT,
	THID_ERROR
};


/*
 *  HID�ϥ�ɥ����
 */
typedef struct
{
	uint8_t                   buffer[32];
	uint8_t                   type;
	uint8_t                   InPipe;
	uint8_t                   state;
	uint8_t                   ep_addr;
	uint8_t                   DataReady;
	uint8_t                   ReqReport;
	uint16_t                  length;
	uint16_t                  poll;
	uint32_t                  timer;
	uint16_t                  ReportDescLength;
	uint8_t                   ReportDesc[66];
	uint8_t                   ReportType;
	uint8_t                   ReportId;
	uint16_t                  ReportLength;
	uint8_t                   Report[32];
} HID_Handle_t;


void    tusbhLinkHID(TUSBH_Handle_t *phost);

TUSBH_ERCODE tubhHidSetReport(HID_Handle_t *hhid, uint8_t type, uint8_t id, uint8_t* pbuf, uint8_t len);
uint8_t tusbhHidGetType(TUSBH_Device_t *pdevice);
uint8_t tusbhHidGetPollInterval(TUSBH_Device_t *hdevice);
void    tusbhSetHidCallBack(TUSBH_Handle_t *phost, void (*func)(TUSBH_Device_t *, uint8_t, uint8_t *));



#ifdef __cplusplus
}
#endif

#endif /* _HUSBH_HID_H_ */

