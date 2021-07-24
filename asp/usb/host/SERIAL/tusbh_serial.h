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
 *  @(#) $Id: tusbh_serial.h 698 2017-09-20 18:44:27Z roi $
 */
/*
 *  USB Host Middleware CDC:SERIAL�����
 */

#ifndef _HUSBH_SERIAL_H_
#define _HUSBH_SERIAL_H_

#include "tusbh_base.h"

#ifdef __cplusplus
 extern "C" {
#endif


/*
 *  COMMUNCATION SUB CLASS CODE���
 */
#define COMMUNICATIONS_INTERFACE_CLASS  0x00
#define DIRECT_LINE_CONTROL_MODEL       0x01
#define ABSTRACT_CONTROL_MODEL          0x02
#define TELEPHONE_CONTROL_MODEL         0x03
#define MULTICHANNEL_CONTROL_MODEL      0x04
#define CAPI_CONTROL_MODEL              0x05
#define ETHERNET_NETWORKING_CONTROL_MODEL 0x06
#define ATM_NETWORKING_CONTROL_MODEL    0x07
#define WIRELESS_HANDSET_CONTROL_MODEL  0x08
#define MOBILE_DIRECT_LINE_MODEL        0x0A
#define ETHERNET_EMULATION_MODEL        0x0C
#define NETWORK_CONTROL_MODEL           0x0D


/*
 *  COMMUNICATION INTERFCAE CLASS ����ץ�ȥ��륳�������
 */
#define CDC_NO_SPECIFIC_PROTOCOL_CODE   0x00
#define CDC_ITU_T_V_250                 0x01
#define CDC_VENDOR_SPECIFIC             0xFF

/*
 *  DATA INTERFCAE CLASS ����ץ�ȥ��륳�������
 */
#define DIC_NO_SPECIFIC_PROTOCOL_CODE   0x00
#define DIC_USBNCM_1_0                  0x01


#define CDC_SET_LINE_CODING             0x20
#define CDC_GET_LINE_CODING             0x21
#define CDC_SET_CONTROL_LINE_STATE      0x22
#define CDC_SEND_BREAK                  0x23

#define CDC_REQUEST_GETLINE_TYPE        (USB_DEVICE_TO_HOST | USB_REQUEST_TYPE_CLASS | USB_RECIPIENT_INTERFACE)
#define CDC_REQUEST_SETLINE_TYPE        (USB_HOST_TO_DEVICE | USB_REQUEST_TYPE_CLASS | USB_RECIPIENT_INTERFACE)

#define LINE_CODING_STRUCTURE_SIZE      0x07

/*
 *  CDC������Хå����������
 */
#define CDC_LINE_STATUS_CHANGED         1
#define CDC_LINE_STATUS_SENDED          2
#define CDC_LINE_STATUS_RECEIVED        3

/*
 *  CDC�ץ����ξ������
 */
enum
{
	CDC_PROCESS_INIT = 0,
	CDC_PROCESS_INIT_WAIT,
	CDC_PROCESS_IDLE,
	CDC_PROCESS_SET_LINE_CODING,
	CDC_PROCESS_SET_LINE_CODING_WAIT,
	CDC_PROCESS_GET_LINE_CODING,
	CDC_PROCESS_GET_LINE_CODING_WAIT,
	CDC_PROCESS_SEND,
	CDC_PROCESS_RECEIVE,
	CDC_PROCESS_TRANSFER_WAIT,
	CDC_PROCESS_ERROR,
	CDC_PROCESS_ERROR_WAIT
};

/*
 *  CDC�ϥ�ɥ����
 */
typedef struct
{
	TUSBH_Device_t            *pDev;
	uint8_t                   state;
	uint8_t                   pre_state;
	uint16_t                  TrnEpSize;
	uint16_t                  TrnPktSize;

	uint8_t                   CommInPipe;
	uint8_t                   CommInEp;
	uint16_t                  CommInEpSize;
	uint8_t                   InPipe;
	uint8_t                   OutPipe;
	uint8_t                   OutEp;
	uint8_t                   InEp;
	uint16_t                  OutEpSize;
	uint16_t                  InEpSize;
	uint32_t                  TrnLength;
	uint32_t                  TrnSize;
	uint8_t                   *pTrnBuff;
	uint32_t                  timer;
	uint32_t                  urb_wait;
	ID                        rw_taskid;
	uint32_t                  rw_status;
	Line_Coding_t             oLineCoding;
	Line_Coding_t             uLineCoding;
} CDC_Handle_t;


void         tusbhLinkSERIAL(TUSBH_Handle_t *phost);
TUSBH_ERCODE tusbhCdcSetLineCoding(TUSBH_Handle_t *phost, uint8_t unit, Line_Coding_t *linecod);
TUSBH_ERCODE tusbhCdcGetLineCoding(TUSBH_Handle_t *phost, uint8_t unit, Line_Coding_t *linecod);
TUSBH_ERCODE tusbhCdcSend(TUSBH_Handle_t *phost, uint8_t unit, uint8_t *pbuf, uint32_t length);
TUSBH_ERCODE tusbhCdcReceive(TUSBH_Handle_t *phost, uint8_t unit, uint8_t *pbuf, uint32_t *length, uint32_t timeout);
void tusbhSetCdcCallBack(TUSBH_Handle_t *phost, void (*func)(TUSBH_Device_t *p, uint8_t));


#ifdef __cplusplus
}
#endif

#endif /* _HUSBH_SERIAL_H_ */

