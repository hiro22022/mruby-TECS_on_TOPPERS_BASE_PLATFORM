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
 *  @(#) $Id: tusbh_prt.c 698 2017-09-21 10:16:56Z roi $
 */
/*
 *  USB Host Middleware PRT CLASS��
 */

#include "tusbh_prt.h"


#define CONTROL_STATUS                  1
#define CONTROL_VENDER                  2


#define PRT_REQUEST_VENDER(d, b, c, l)  tusbhControlRequest((d), (b), PRT_VENDER_SEND_TYPE, (c), 0, 0, (l))
#define PRT_REQUEST_STATUS(d, b, c, l)  tusbhControlRequest((d), (b), PRT_REQUEST_STATUS_TYPE, (c), 0, 0, (l))


static TUSBH_ERCODE tusbhPRTInit(TUSBH_Device_t *pdevice);
static TUSBH_ERCODE tusbhPRTDeInit(TUSBH_Device_t *pdevice);
static TUSBH_ERCODE tusbhPRTProcess(TUSBH_Device_t *pdevice, uint8_t *mes);

static TUSBH_Class_t PRT_Class = {
	NULL,
	"PRT",
	PRT_CLASS,
	tusbhPRTInit,
	tusbhPRTDeInit,
	tusbhPRTProcess,
	NULL
};

/*
 *  PRT���饹���åȥ��å״ؿ�
 */
void tusbhLinkPRT(TUSBH_Handle_t *phost)
{
	tusbhLinkClass(phost, &PRT_Class);
}

/*
 *  PRT���饹�������
 *  parameter1 pdevice:  �ǥХ����ϥ�ɥ�
 *  return     TUSBH_ERCODE
 */
static TUSBH_ERCODE
tusbhPRTInit(TUSBH_Device_t *pdevice)
{
	uint8_t interface = 0; 
	PRT_Handle_t *hprt;
	TUSBH_Handle_t *phost = pdevice->pHost;
	uint8_t max_ep, i;

	interface = tusbhFindInterface(pdevice, pdevice->pClass->classCode, 0x01, 0x02);

	if(interface == NO_INTERFACE){
		syslog_1(LOG_ERROR, "[PRT] Cannot Find the interface for %s class.", pdevice->pClass->Name);
		return TUSBH_E_ERR;
	}
	tusbhSelectInterface(pdevice, interface);

	hprt = (PRT_Handle_t *)tusbmalloc(sizeof(PRT_Handle_t));
	pdevice->pData = hprt;
	memset(hprt, 0, sizeof(PRT_Handle_t));
	max_ep = ((pdevice->ItfDesc[interface].bNumEndpoints <= 2) ?
              pdevice->ItfDesc[interface].bNumEndpoints : 2);
#if 1	/* ROI DEBUG */
	syslog_3(LOG_NOTICE, "## tusbhCDCInit hmsc[%08x] max_ep(%d) buffer(%d) ##", hprt, max_ep, offsetof(PRT_Handle_t, buffer));
#endif	/* ROI DEBUG */

	for(i = 0 ; i < max_ep; i++){
		if(pdevice->EpDesc[interface][i].bEndpointAddress & 0x80){
			hprt->InEp     = (pdevice->EpDesc[interface][i].bEndpointAddress);
			hprt->InEpSize = pdevice->EpDesc[interface][i].wMaxPacketSize;
			hprt->InPipe = tusbhAllocPipe(phost, hprt->InEp);
		}
		else{
			hprt->OutEp = (pdevice->EpDesc[interface][i].bEndpointAddress);
			hprt->OutEpSize = pdevice->EpDesc[interface][i].wMaxPacketSize;
			hprt->OutPipe = tusbhAllocPipe(phost, hprt->OutEp);
		}
	}
	if(hprt->OutPipe == 0 || hprt->InPipe == 0)
		return TUSBH_E_ERR;

	hprt->state = PRT_PROCESS_INIT;
	hprt->timer = 0;
	hprt->pDev = pdevice;

	/*
	 *  NSC����ͥ륪���ץ�
	 */
	tusbhOpenPipe(pdevice, hprt->OutPipe, hprt->OutEp, USB_EP_TYPE_BULK, hprt->OutEpSize);
	tusbhHDSetToggle(phost, hprt->OutPipe, 0);
	tusbhOpenPipe(pdevice, hprt->InPipe, hprt->InEp, USB_EP_TYPE_BULK, hprt->InEpSize);
	tusbhHDSetToggle(phost, hprt->InPipe, 0);

	/* �����ޡ��������� */
	pdevice->timeid = pdevice->timecount = 100;
	return TUSBH_E_OK;
}

/*
 *  PRT���饹��λ����
 *  parameter1 pdevice:  �ǥХ����ϥ�ɥ�
 *  return     TUSBH_ERCODE
 */
static TUSBH_ERCODE
tusbhPRTDeInit(TUSBH_Device_t *pdevice)
{
	PRT_Handle_t *hprt;
	TUSBH_Handle_t *phost = pdevice->pHost;

	hprt = (PRT_Handle_t *)pdevice->pData;
	if(hprt->OutPipe != 0){
		tusbhClosePipe(pdevice, hprt->OutPipe);
		tusbFreePipe(phost, hprt->OutPipe);
		hprt->OutPipe = 0;
	}

	if(hprt->InPipe != 0){
		tusbhClosePipe(pdevice, hprt->InPipe);
		tusbFreePipe(phost, hprt->InPipe);
		hprt->InPipe = 0;
	}

	if(pdevice->pData != NULL){
		tusbfree(pdevice->pData);
		pdevice->pData = NULL;
	}
	return TUSBH_E_OK;
}

/*
 *  PRT���饹�ץ����¹�
 *  parameter1 pdevice:  �ǥХ����ϥ�ɥ�
 *  parameter2 mes:      �̿���å������ؤΥݥ���
 *  return     TUSBH_ERCODE
 */
static TUSBH_ERCODE
tusbhPRTProcess(TUSBH_Device_t *pdevice, uint8_t *mes)
{
	PRT_Handle_t   *hprt  = (PRT_Handle_t *)pdevice->pData;
	TUSBH_Handle_t *phost = pdevice->pHost;
	void (*func)(TUSBH_Device_t *, uint8_t) = pdevice->pClass->subfunc;
	TUSBH_URBState_t URB_Status = mes[3];
	TUSBH_ERCODE ercd = TUSBH_E_BUSY;
	int32_t length;
	uint8_t cmd = PRT_LINE_STATUS_CHANGED;

    if(mes[0] == TUSBH_URB_EVENT){
		hprt->urb_wait = 0;
    }
	else if(mes[0] == TUSBH_TIME_EVENT){
		hprt->timer += mes[3];
		pdevice->timecount = mes[3];
	}
	switch (hprt->state){
	case PRT_PROCESS_INIT:
		if(phost->usrcallback != NULL)
			phost->usrcallback(phost, pdevice, HOST_USER_CLASS_ACTIVE);
		hprt->state = PRT_PROCESS_IDLE;
	case PRT_PROCESS_IDLE:
		ercd = TUSBH_E_OK;
		break;
	case PRT_PROCESS_CONTROL:
		syslog_2(LOG_NOTICE, "## PRT_PROCESS_CONTROL type(%d) req(%d) ##", hprt->controlType, hprt->controlRequest);
		if(hprt->controlType == CONTROL_VENDER)
			PRT_REQUEST_VENDER(pdevice, hprt->buffer, hprt->controlRequest, hprt->controlLength);
		else
			PRT_REQUEST_STATUS(pdevice, hprt->buffer, hprt->controlRequest, hprt->controlLength);
		hprt->state = PRT_PROCESS_CONTROL_WAIT;
		break;
	case PRT_PROCESS_CONTROL_WAIT:
		ercd = tusbhControlWait(pdevice, mes);
		if(ercd == TUSBH_E_OK){
			if(hprt->controlType == CONTROL_STATUS)
				syslog_5(LOG_NOTICE, "## len(%d) 0[%02x] 1[%02x] 2[%02x] 3[%02x] ##", hprt->controlLength, hprt->buffer[0], hprt->buffer[1], hprt->buffer[2], hprt->buffer[3]);
			hprt->state = PRT_PROCESS_IDLE;
			hprt->controlType = 0;
		}
		else if(ercd != TUSBH_E_BUSY && ercd != TUSBH_E_BUSY_URB){
			hprt->state = PRT_PROCESS_ERROR;
			hprt->controlType = 0;
		}
		break;
	case PRT_PROCESS_SEND:
		length = hprt->TrnSize - hprt->TrnLength;
		if(length > hprt->TrnEpSize)
			hprt->TrnPktSize = hprt->TrnEpSize;
		else
			hprt->TrnPktSize = length;
		tusbhBulkWrite(pdevice, hprt->pTrnBuff, hprt->TrnPktSize, hprt->OutPipe);
		hprt->state = PRT_PROCESS_TRANSFER_WAIT;
		break;
	case PRT_PROCESS_RECEIVE:
		length = hprt->TrnSize - hprt->TrnLength;
		if(length > hprt->TrnEpSize)
			hprt->TrnPktSize = hprt->TrnEpSize;
		else
			hprt->TrnPktSize = length;
		tusbhBulkRead(pdevice, hprt->pTrnBuff, hprt->TrnPktSize, hprt->InPipe);
		hprt->state = PRT_PROCESS_TRANSFER_WAIT;
		break;
	case PRT_PROCESS_TRANSFER_WAIT:
		if(hprt->rw_status != TUSBH_E_BUSY)
			hprt->state = PRT_PROCESS_ERROR;
		else if(mes[0] != TUSBH_URB_EVENT)
			 break;
		else if(URB_Status == TUSBH_URB_DONE){
			if(hprt->pre_state == PRT_PROCESS_RECEIVE){
				hprt->TrnPktSize = tusbhHDTrasLength(phost, hprt->InPipe);
				cmd = PRT_LINE_STATUS_RECEIVED;
			}
			else
				cmd = PRT_LINE_STATUS_SENDED;
			hprt->TrnLength += hprt->TrnPktSize;
			hprt->pTrnBuff += hprt->TrnPktSize;
			if(hprt->TrnSize > hprt->TrnLength && hprt->pre_state == PRT_PROCESS_SEND)
				hprt->state = hprt->pre_state;
			else{
				hprt->state = PRT_PROCESS_IDLE;
				hprt->rw_status = TUSBH_E_OK;
				if(func != NULL)
					func(pdevice, cmd);
				if(hprt->rw_taskid != 0)
					tusbWakeup(hprt->rw_taskid);
			}
		}
		else if(URB_Status == TUSBH_URB_NOTREADY)
			hprt->state = hprt->pre_state;
		else{
			hprt->state = PRT_PROCESS_ERROR;
			hprt->rw_status = TUSBH_E_ERR;
			if(func != NULL)
				func(pdevice, cmd);
			if(hprt->rw_taskid != 0)
				tusbWakeup(hprt->rw_taskid);
		}
		tusbSendData(phost->process_event, TUSBH_CLASS_EVENT, pdevice->idx, 0, 0);
		break;
	case PRT_PROCESS_ERROR:
		tusbhClearFeature(pdevice, 0x00);
		hprt->state = PRT_PROCESS_ERROR_WAIT;
		break;
	case PRT_PROCESS_ERROR_WAIT:
		if(tusbhControlWait(pdevice, mes) == TUSBH_E_OK)
			hprt->state = PRT_PROCESS_IDLE;
		break;


	default:
		break;
	}
	return ercd;
}

/*
 *  PRT SEND EOJ
 *  parameter1 phost:   �ۥ��ȥϥ�ɥ�
 *  parameter2 unit:    UNIT#
 *  parameter3 cmd:     ���ꥳ�ޥ��
 *  return     TUSBH_ERCODE
 */
TUSBH_ERCODE
tusbhPrtEOJ(TUSBH_Handle_t *phost, uint8_t unit, uint8_t cmd, uint8_t prm1, uint8_t prm2)
{
	TUSBH_Device_t *pdevice = tusbhSearchDevice(phost, PRT_Class.classCode, &unit);
	PRT_Handle_t *hprt;

	if(pdevice == NULL)
		return TUSBH_E_OBJ;
	hprt = (PRT_Handle_t *)pdevice->pData;
	if(pdevice->dstate == DEVICE_CLASS){
		hprt->buffer[0] = cmd;
		hprt->buffer[1] = prm1;
		hprt->buffer[2] = prm2;
		hprt->controlType    = CONTROL_VENDER;
		hprt->controlRequest  = PRT_VERNDER_REQUEST_EOJ;
		hprt->controlLength  = 3;
		hprt->state = PRT_PROCESS_CONTROL;
		tusbSendData(phost->process_event, TUSBH_CLASS_EVENT, pdevice->idx, 0, 0);
		while(hprt->controlType != 0){
			tusbSleep(1);
		}
	}
	return TUSBH_E_OK;
}

/*
 *  PRT GET PORTID
 *  parameter1 phost:   �ۥ��ȥϥ�ɥ�
 *  parameter2 unit:    UNIT#
 *  parameter3 cmd:     ���ꥳ�ޥ��
 *  return     TUSBH_ERCODE
 */
TUSBH_ERCODE
tusbhPrtPortID(TUSBH_Handle_t *phost, uint8_t unit, uint8_t *buf)
{
	TUSBH_Device_t *pdevice = tusbhSearchDevice(phost, PRT_Class.classCode, &unit);
	PRT_Handle_t *hprt;

	if(pdevice == NULL)
		return TUSBH_E_OBJ;
	hprt = (PRT_Handle_t *)pdevice->pData;
	if(pdevice->dstate == DEVICE_CLASS){
		hprt->controlType    = CONTROL_STATUS;
		hprt->controlRequest = PRT_REQUEST_PORTSTATUS;
		hprt->controlLength  = 1;
		hprt->state = PRT_PROCESS_CONTROL;
		tusbSendData(phost->process_event, TUSBH_CLASS_EVENT, pdevice->idx, 0, 0);
		while(hprt->controlType != 0){
			tusbSleep(1);
		}
		*buf = hprt->buffer[0];
	}
	return TUSBH_E_OK;
}

/*
 *  PRT DATA RECEIVE
 *  parameter1 phost:   �ۥ��ȥϥ�ɥ�
 *  parameter2 unit:    UNIT#
 *  parameter3 pbuf:    �����ǡ����ΰ�ؤΥݥ���
 *  parameter4 length:  �����֥�å�Ĺ
 *  parameter5 tmout:   �����ॢ������(ms)
 *  return     TUSBH_ERCODE
 */
TUSBH_ERCODE
tusbhPrtReceive(TUSBH_Handle_t *phost, uint8_t unit, uint8_t *pbuf, uint32_t *length, uint32_t timeout)
{
	TUSBH_Device_t *pdevice = tusbhSearchDevice(phost, PRT_Class.classCode, &unit);
	PRT_Handle_t *hprt;
	TUSBH_ERCODE status = TUSBH_E_BUSY;
	uint32_t i;

	if(pdevice != NULL){
		hprt = pdevice->pData;
		if(pdevice->dstate != DEVICE_CLASS)
			return TUSBH_E_OBJ;
		if(hprt->state == PRT_PROCESS_ERROR)
			return TUSBH_E_ERR;
		if(hprt->state != PRT_PROCESS_IDLE)
			return TUSBH_E_OBJ;
	}
	else
		return TUSBH_E_OBJ;

    /*
	 *  �����ǡ�������
	 */
	hprt->TrnSize   = *length;
	hprt->TrnLength = 0;
	hprt->pTrnBuff  = pbuf;
	hprt->TrnEpSize = hprt->InEpSize;
	hprt->state     = PRT_PROCESS_RECEIVE;
	hprt->pre_state = PRT_PROCESS_RECEIVE;
	hprt->rw_status = TUSBH_E_BUSY;
	hprt->rw_taskid = tusbGetTaskID();
	tusbSendData(phost->process_event, TUSBH_CLASS_EVENT, pdevice->idx, 0, 0);

	if(timeout != 0)
		timeout += 9;
	timeout /= 10;
	for(i = 0 ; i < timeout ; i++){
		tusbSleep(10);
		if((status = hprt->rw_status) != TUSBH_E_BUSY)
			break;
	}
	if(status == TUSBH_E_BUSY){
		hprt->rw_status = TUSBH_E_TMOUT;
		status = TUSBH_E_TMOUT;
	}
	*length = hprt->TrnLength;
	hprt->rw_taskid = 0;
	return status;
}

/*
 *  PRT DATA SEND
 *  parameter1 phost:   �ۥ��ȥϥ�ɥ�
 *  parameter2 unit:    UNIT#
 *  parameter3 pbuf:    �����ǡ����ΰ�ؤΥݥ���
 *  parameter4 length:  �����ǡ���Ĺ
 *  return     TUSBH_ERCODE
 */
TUSBH_ERCODE
tusbhPrtSend(TUSBH_Handle_t *phost, uint8_t unit, uint8_t *pbuf, uint32_t length)
{
	TUSBH_Device_t *pdevice = tusbhSearchDevice(phost, PRT_Class.classCode, &unit);
	PRT_Handle_t *hprt;
	TUSBH_ERCODE status = TUSBH_E_BUSY;
	uint32_t i;

	if(pdevice != NULL){
		hprt = pdevice->pData;
		if(pdevice->dstate != DEVICE_CLASS)
			return TUSBH_E_OBJ;
		if(hprt->state == PRT_PROCESS_ERROR)
			return TUSBH_E_ERR;
		if(hprt->state != PRT_PROCESS_IDLE)
			return TUSBH_E_OBJ;
	}
	else
		return TUSBH_E_OBJ;

    /*
	 *  �����ǡ�������
	 */
	hprt->TrnSize   = length;
	hprt->TrnLength = 0;
	hprt->pTrnBuff  = pbuf;
	hprt->TrnEpSize = hprt->OutEpSize;
	hprt->state     = PRT_PROCESS_SEND;
	hprt->pre_state = PRT_PROCESS_SEND;
	hprt->rw_status = TUSBH_E_BUSY;
	hprt->rw_taskid = tusbGetTaskID();
	tusbSendData(phost->process_event, TUSBH_CLASS_EVENT, pdevice->idx, 0, 0);

	for(i = 0 ; i < (length * 10) ; i++){
		tusbSleep(50);
		if((status = hprt->rw_status) != TUSBH_E_BUSY)
			break;
	}
	if(status == TUSBH_E_BUSY){
		hprt->rw_status = TUSBH_E_TMOUT;
		status = TUSBH_E_TMOUT;
	}
	hprt->rw_taskid = 0;
	return status;
}

/*
 *  SERIAL������Хå��ؿ�
 *  parameter1 phost:   �ۥ��ȥϥ�ɥ�
 *  parameter2 func:    �ؿ��ݥ���
 */
void
tusbhSetPrtCallBack(TUSBH_Handle_t *phost, void (*func)(TUSBH_Device_t *p, uint8_t))
{
	PRT_Class.subfunc = func;
}


