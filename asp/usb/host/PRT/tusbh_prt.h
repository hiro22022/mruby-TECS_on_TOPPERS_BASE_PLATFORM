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
 *  @(#) $Id: tusbh_prt.h 698 2017-09-21 10:12:27Z roi $
 */
/*
 *  USB Host Middleware PRT�����
 */

#ifndef _HUSBH_PRT_H_
#define _HUSBH_PRT_H_

#include "tusbh_base.h"

#ifdef __cplusplus
 extern "C" {
#endif


#define PRT_VERNDER_REQUEST_EOJ     0
#define PRT_REQUEST_DEVICEID        0
#define PRT_REQUEST_PORTSTATUS      1
#define PRT_REQUEST_SOFTRESET       2

#define PRT_REQUEST_STATUS_TYPE         (USB_DEVICE_TO_HOST | USB_REQUEST_TYPE_CLASS | USB_RECIPIENT_INTERFACE)
#define PRT_VENDER_SEND_TYPE            (USB_HOST_TO_DEVICE | USB_REQUEST_TYPE_VENDOR | USB_RECIPIENT_INTERFACE)

/*
 *  PRT������Хå����������
 */
#define PRT_LINE_STATUS_CHANGED         1
#define PRT_LINE_STATUS_SENDED          2
#define PRT_LINE_STATUS_RECEIVED        3

/*
 *  PRT�ץ����ξ������
 */
enum
{
	PRT_PROCESS_INIT = 0,
	PRT_PROCESS_IDLE,
	PRT_PROCESS_CONTROL,
	PRT_PROCESS_CONTROL_WAIT,
	PRT_PROCESS_SEND,
	PRT_PROCESS_RECEIVE,
	PRT_PROCESS_TRANSFER_WAIT,
	PRT_PROCESS_ERROR,
	PRT_PROCESS_ERROR_WAIT
};

/*
 *  PRT�ϥ�ɥ����
 */
typedef struct
{
	TUSBH_Device_t            *pDev;
	uint8_t                   state;
	uint8_t                   pre_state;
	uint8_t                   cmd;
	uint8_t                   dummy;
	uint16_t                  TrnEpSize;
	uint16_t                  TrnPktSize;

	uint8_t                   InPipe;
	uint8_t                   OutPipe;
	uint8_t                   OutEp;
	uint8_t                   InEp;
	uint16_t                  OutEpSize;
	uint16_t                  InEpSize;
	uint8_t                   controlType;
	uint8_t                   controlRequest;
	uint16_t                  controlLength;
	uint32_t                  TrnLength;
	uint32_t                  TrnSize;
	uint8_t                   *pTrnBuff;
	uint32_t                  timer;
	uint32_t                  urb_wait;
	ID                        rw_taskid;
	uint32_t                  rw_status;
	uint8_t                   buffer[64];
} PRT_Handle_t;


void         tusbhLinkPRT(TUSBH_Handle_t *phost);
TUSBH_ERCODE tusbhPrtEOJ(TUSBH_Handle_t *phost, uint8_t unit, uint8_t cmd, uint8_t prm1, uint8_t prm2);
TUSBH_ERCODE tusbhPrtPortID(TUSBH_Handle_t *phost, uint8_t unit, uint8_t *buf);
TUSBH_ERCODE tusbhPrtSend(TUSBH_Handle_t *phost, uint8_t unit, uint8_t *pbuf, uint32_t length);
TUSBH_ERCODE tusbhPrtReceive(TUSBH_Handle_t *phost, uint8_t unit, uint8_t *pbuf, uint32_t *length, uint32_t timeout);
void tusbhSetPrtCallBack(TUSBH_Handle_t *phost, void (*func)(TUSBH_Device_t *p, uint8_t));


#ifdef __cplusplus
}
#endif

#endif /* _HUSBH_PRT_H_ */

