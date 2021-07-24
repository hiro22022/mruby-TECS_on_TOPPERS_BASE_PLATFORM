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
 *  @(#) $Id: tusbh_hub.h 698 2017-09-12 18:44:27Z roi $
 */
/*
 *  USB Host Middleware HUB�����
 */

#ifndef _HUSBH_HUB_H_
#define _HUSBH_HUB_H_

#include "tusbh_base.h"

#ifdef __cplusplus
 extern "C" {
#endif



#define PORT_CONNECTION_FEATURE         (0x00)
#define PORT_ENABLEFEATURE              (0x01)
#define PORT_SUSPEND                    (0x02)
#define PORT_OVER_CURRENT_FEATURE       (0x03)
#define PORT_RESETFEATURE               (0x04)
#define PORT_POWERFEATURE               (0x08)
#define PORT_LOW_SPEED                  (0x09)

#define PORT_CLEAR_CONNECTION_FEATURE   (0x10)
#define PORT_CLEAR_ENABLE_FEATURE       (0x11)
#define PORT_CLEAR_RESET_FEATURE        (0x14)

#define PORT_SETFEATURE_TYPE            (USB_HOST_TO_DEVICE | USB_REQUEST_TYPE_CLASS | USB_RECIPIENT_INTERFACE | USB_RECIPIENT_ENDPOINT)
#define PORT_CLEARFEATURE_TYPE          (USB_HOST_TO_DEVICE | USB_REQUEST_TYPE_CLASS | USB_RECIPIENT_INTERFACE | USB_RECIPIENT_ENDPOINT)
#define PORT_GETSTATUS_TYPE             (USB_DEVICE_TO_HOST | USB_REQUEST_TYPE_CLASS | USB_RECIPIENT_INTERFACE | USB_RECIPIENT_ENDPOINT)

#define PORT_STATUS_CONNECTION          (1<<PORT_CONNECTION_FEATURE)
#define PORT_STATUS_ENABLE              (1<<PORT_ENABLEFEATURE)
#define PORT_STATUS_SUSPEND             (1<<PORT_SUSPEND)
#define PORT_STATUS_OVER_CURRENT        (1<<PORT_OVER_CURRENT_FEATURE)
#define PORT_STATUS_RESET               (1<<PORT_RESETFEATURE)
#define PORT_STATUS_POWER               (1<<PORT_POWERFEATURE)
#define PORT_STATUS_LOW_SPEED           (1<<PORT_LOW_SPEED)

#define PORT_CHANGE_CONNECTION          (1<<16)
#define PORT_CHANGE_ENABLE              (1<<17)
#define PORT_CHANGE_SUSPEND             (1<<18)
#define PORT_CHANGE_OVER_CURRENT        (1<<19)
#define PORT_CHANGE_RESET               (1<<20)

/*
 * HUB�������
 */
enum
{
	THUB_IDLE = 0,
	THUB_SYNC,
	THUB_GET_DATA,
	THUB_POLL,
	THUB_GET_STATUS_WAIT,
	THUB_CLEAR_FEATURE_WAIT,
	THUB_PORT_RESET_WAIT,
	THUB_CONNECT_WAIT,
	THUB_PORT_ERROR,
	THUB_PORT_ERROR_WAIT
};

/*
 *  HUB CONNECT�������
 */
enum
{
	THUB_CONNCET_IDEL = 0,
	THUB_CONNECT_RESET,
	THUB_CONNECT_ENABLE,
	THUB_CONNECT_ACTIVE
};

/*
 *  HUB�ϥ�ɥ����
 */
typedef struct
{
	TUSBH_Device_t            *pDev;
	uint8_t                   state;
	uint8_t                   hubid;
	uint8_t                   numport;
	uint8_t                   characteristics;
	uint32_t                  timer;
	uint32_t                  starttime;

	uint8_t                   InPipe;
	uint8_t                   ep_addr;
	uint8_t                   portno;
	uint8_t                   cstate;
	uint16_t                  length;
	uint16_t                  poll; 
	uint32_t                  reset_portid;
	uint32_t                  dummy2;
	uint8_t                   buffer[64];
} HUB_Handle_t;


void  tusbhLinkHUB(TUSBH_Handle_t *phost);


#ifdef __cplusplus
}
#endif

#endif /* _HUSBH_HUB_H_ */

