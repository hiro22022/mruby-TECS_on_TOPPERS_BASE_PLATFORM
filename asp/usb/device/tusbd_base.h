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
 *  @(#) $Id: tusbd_base.h 698 2017-10-05 21:31:22Z roi $
 */
/*
 *  USB Device Middleware BASE�����
 */

#ifndef _TUSBD_BASE_H_
#define _TUSBD_BASE_H_

#include "tusb_rtos.h"
#include "tusb_types.h"

#ifdef __cplusplus
 extern "C" {
#endif

#ifndef TUSBD_NUM_ENDPOINT
#define TUSBD_NUM_ENDPOINT                    15
#endif

#define USB_REQUEST_TYPE_MASK                 0x60

#define TUSBD_CONFIG_REMOTE_WAKEUP            0x02

#define USB_FEATURE_EP_HALT                   0
#define USB_FEATURE_REMOTE_WAKEUP             1
#define USB_FEATURE_TEST_MODE                 2



#define USB_HS_MAX_PACKET_SIZE                512
#define USB_FS_MAX_PACKET_SIZE                64
#define USB_MAX_EP0_SIZE                      USB_FS_MAX_PACKET_SIZE

/*
 *  �ǥХ����������
 */
#define TUSBD_STATE_RESET                     0		/* ̤�¹Ծ��� */
#define TUSBD_STATE_INIT                      1		/* ��������� */
#define TUSBD_STATE_ADDRESSED                 2		/* ���ɥ쥹����Ѥ߾��� */
#define TUSBD_STATE_CONFIGURED                3		/* ���ʥߥ�졼�����λ���� */
#define TUSBD_STATE_SUSPENDED                 4		/* �����ڥ�ɾ��� */

/*
 *  ����ɥݥ����#0����
 */
#define TUSBD_EP0_IDLE                        0		/* �����ɥ���� */
#define TUSBD_EP0_SETUP                       1		/* SETUP���� */
#define TUSBD_EP0_DATAIN                      2		/* DATAIN���� */
#define TUSBD_EP0_DATAOUT                     3		/* DATAOUT���� */
#define TUSBD_EP0_STATUSIN                    4		/* STATUSIN���� */
#define TUSBD_EP0_STATUSOUT                   5		/* STATUSOUT���� */

/* bmAttributes in configuration descriptor */
/* C_RESERVED must always be set */
#define C_RESERVED                            (1<<7)
#define C_SELF_POWERED                        (1<<6)
#define C_REMOTE_WAKEUP                       (1<<5)

/* bMaxPower in configuration descriptor */
#define C_POWER(mA)                           ((mA)/2)

/*
 *  ����ɥݥ���ȴ�����
 */
typedef struct
{
	uint16_t                   status;
	uint16_t                   maxpacket;
	uint32_t                   xfersize;
	uint32_t                   remlength;
} TUSBD_Endpoint_t;

/*
 *  USB�ǥХ����ϥ�ɥ�
 */
typedef struct
{
	uint8_t                    devData[32];
	uint8_t                    id;
	uint8_t                    dev_speed;
	uint8_t                    dev_address;
	uint8_t                    dev_config;
	uint8_t                    dev_test_mode;
	uint8_t                    dev_remote_wakeup;
	uint8_t                    ep0_state;
	uint16_t                   ep0_data_len;
	uint8_t                    dev_state;
	uint8_t                    dev_old_state;
	TUSBD_Endpoint_t           ep_in[TUSBD_NUM_ENDPOINT];
	TUSBD_Endpoint_t           ep_out[TUSBD_NUM_ENDPOINT];

	UsbSetupRequest            request;
	void                       *pClassData;
	void                       *pSysData;
	void                       *pUsrData;
	void                       (*makeDescriptor)(uint8_t speed);
} TUSBD_Handle_t;

#include "tusbd_conf.h"

#ifndef MAX_PACKET_SIZE_HS_EPBULK
#define MAX_PACKET_SIZE_HS_EPBULK             512
#endif
#ifndef MAX_PACKET_SIZE_FS_EPBULK
#define MAX_PACKET_SIZE_FS_EPBULK             64
#endif


/*
 *  �ؿ��ץ�ȥ��������
 */
TUSBD_ERCODE tusbdInit(TUSBD_Handle_t *pdevice, uint8_t id);
TUSBD_ERCODE tusbdDeInit(TUSBD_Handle_t *pdevice);
TUSBD_ERCODE tusbdStart(TUSBD_Handle_t *pdevice);
TUSBD_ERCODE tusbdStop(TUSBD_Handle_t *pdevice);

TUSBD_ERCODE tusbdSetupStage(TUSBD_Handle_t *pdevice, uint8_t *psetup);
TUSBD_ERCODE tusbdDataOutStage(TUSBD_Handle_t *pdevice, uint8_t epnum, uint8_t *pdata);
TUSBD_ERCODE tusbdDataInStage(TUSBD_Handle_t *pdevice, uint8_t epnum, uint8_t *pdata);

TUSBD_ERCODE tusbdReset(TUSBD_Handle_t *pdevice);
TUSBD_ERCODE tusbdSetSpeed(TUSBD_Handle_t *pdevice, uint8_t speed);
TUSBD_ERCODE tusbdSuspend(TUSBD_Handle_t *pdevice);
TUSBD_ERCODE tusbdResume(TUSBD_Handle_t *pdevice);
TUSBD_ERCODE tusbdIsoInIncomplete(TUSBD_Handle_t *pdevice, uint8_t epnum);
TUSBD_ERCODE tusbdIsoOutIncomplete(TUSBD_Handle_t *pdevice, uint8_t epnum);

TUSBD_ERCODE tusbdDeviceConnected(TUSBD_Handle_t *pdevice);
TUSBD_ERCODE tusbdDeviceDisconnected(TUSBD_Handle_t *pdevice);

TUSBD_ERCODE tusbdControlSendData(TUSBD_Handle_t *pdevice, uint8_t *buf, uint16_t len);
TUSBD_ERCODE tusbdControlReceiveStart(TUSBD_Handle_t *pdevice, uint8_t *pbuf, uint16_t len);
TUSBD_ERCODE tusbdControlSendStatus(TUSBD_Handle_t *pdevice);
TUSBD_ERCODE tusbdControlReceiveStatus(TUSBD_Handle_t *pdevice);
void         tusbdControlSendStall(TUSBD_Handle_t *pdevice, UsbSetupRequest *req);

#define tusbdControlContinueSendData(d, b, l)   tusbdDriverStartTransmit((d), 0x00, (b), (l))
#define tusbdControlContinueReceive(d, b, l) tusbdDriverSetupReceive((d), 0x00, (b), (l))

uint8_t      *tusbdFindDescriptor(TUSBD_Handle_t *pdevice, uint8_t type);

#ifdef __cplusplus
}
#endif

#endif /* _TUSBD_BASE_H_ */

