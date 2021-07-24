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
 *  @(#) $Id: tusbh_serial.c 698 2017-09-12 18:46:56Z roi $
 */
/*
 *  USB Host Middleware CDC:SERIAL CLASS��
 */

#include "tusbh_serial.h"


#define CDC_REQUEST_GETLINE(d, b)   tusbhControlRequest((d), (b), CDC_REQUEST_GETLINE_TYPE, CDC_GET_LINE_CODING, 0, 0, LINE_CODING_STRUCTURE_SIZE)
#define CDC_REQUEST_SETLINE(d, b)   tusbhControlRequest((d), (b), CDC_REQUEST_SETLINE_TYPE, CDC_SET_LINE_CODING, 0, 0, LINE_CODING_STRUCTURE_SIZE)


static TUSBH_ERCODE tusbhCDCInit(TUSBH_Device_t *pdevice);
static TUSBH_ERCODE tusbhCDCDeInit(TUSBH_Device_t *pdevice);
static TUSBH_ERCODE tusbhCDCProcess(TUSBH_Device_t *pdevice, uint8_t *mes);

static TUSBH_Class_t SERAIL_Class = {
	NULL,
	"CDC",
	CDC_CLASS,
	tusbhCDCInit,
	tusbhCDCDeInit,
	tusbhCDCProcess,
	NULL
};

/*
 *  SERIAL���饹���åȥ��å״ؿ�
 */
void tusbhLinkSERIAL(TUSBH_Handle_t *phost)
{
	tusbhLinkClass(phost, &SERAIL_Class);
}

/*
 *  CDC���饹�������
 *  parameter1 pdevice:  �ǥХ����ϥ�ɥ�
 *  return     TUSBH_ERCODE
 */
static TUSBH_ERCODE
tusbhCDCInit(TUSBH_Device_t *pdevice)
{
	uint8_t interface = 0; 
	CDC_Handle_t *hcdc;
	TUSBH_Handle_t *phost = pdevice->pHost;
	uint8_t max_ep, i;

	interface = tusbhFindInterface(pdevice, pdevice->pClass->classCode, ABSTRACT_CONTROL_MODEL, CDC_ITU_T_V_250);

	if(interface == NO_INTERFACE){
		syslog_1(LOG_ERROR, "[CDC] Cannot Find the interface for %s class.", pdevice->pClass->Name);
		return TUSBH_E_ERR;
	}
	tusbhSelectInterface(pdevice, interface);

	hcdc = (CDC_Handle_t *)tusbmalloc(sizeof(CDC_Handle_t));
	pdevice->pData = hcdc;
	memset(hcdc, 0, sizeof(CDC_Handle_t));
	max_ep = ((pdevice->ItfDesc[interface].bNumEndpoints <= 2) ?
              pdevice->ItfDesc[interface].bNumEndpoints : 2);
#if 1	/* ROI DEBUG */
	syslog_3(LOG_NOTICE, "## tusbhCDCInit hmsc[%08x] max_ep(%d) csw(%d) ##", hcdc, max_ep, 0);
#endif	/* ROI DEBUG */
	if(max_ep == 0 || (pdevice->EpDesc[interface][0].bEndpointAddress & 0x80) == 0)
		return TUSBH_E_ERR;

	hcdc->CommInEp     = pdevice->EpDesc[interface][0].bEndpointAddress;
    hcdc->CommInEpSize = pdevice->EpDesc[interface][0].wMaxPacketSize;
	hcdc->CommInPipe   = tusbhAllocPipe(phost, hcdc->CommInEp);

	/*
	 *  COMMNUCATION IN ENDPOINT OPEN
	 */
	tusbhOpenPipe(pdevice, hcdc->CommInPipe, hcdc->CommInEp, USB_EP_TYPE_INTR, hcdc->CommInEpSize);
	tusbhHDSetToggle(phost, hcdc->CommInPipe, 0);

    interface = tusbhFindInterface(pdevice, DATA_INTERFACE_CLASS, 0x00, DIC_NO_SPECIFIC_PROTOCOL_CODE);
	if(interface == NO_INTERFACE){
		syslog_1(LOG_ERROR, "[CDC] Cannot Find the interface for %s class.", pdevice->pClass->Name);
		return TUSBH_E_ERR;
	}
	max_ep = ((pdevice->ItfDesc[interface].bNumEndpoints <= 2) ?
              pdevice->ItfDesc[interface].bNumEndpoints : 2);

	for(i = 0 ; i < max_ep; i++){
		if(pdevice->EpDesc[interface][i].bEndpointAddress & 0x80){
			hcdc->InEp     = (pdevice->EpDesc[interface][i].bEndpointAddress);
			hcdc->InEpSize = pdevice->EpDesc[interface][i].wMaxPacketSize;
			hcdc->InPipe = tusbhAllocPipe(phost, hcdc->InEp);
		}
		else{
			hcdc->OutEp = (pdevice->EpDesc[interface][i].bEndpointAddress);
			hcdc->OutEpSize = pdevice->EpDesc[interface][i].wMaxPacketSize;
			hcdc->OutPipe = tusbhAllocPipe(phost, hcdc->OutEp);
		}
	}
	if(hcdc->OutPipe == 0 || hcdc->InPipe == 0)
		return TUSBH_E_ERR;

	hcdc->state = CDC_PROCESS_INIT;
	hcdc->timer = 0;
	hcdc->pDev = pdevice;

	/*
	 *  NSC����ͥ륪���ץ�
	 */
	tusbhOpenPipe(pdevice, hcdc->OutPipe, hcdc->OutEp, USB_EP_TYPE_BULK, hcdc->OutEpSize);
	tusbhHDSetToggle(phost, hcdc->OutPipe, 0);
	tusbhOpenPipe(pdevice, hcdc->InPipe, hcdc->InEp, USB_EP_TYPE_BULK, hcdc->InEpSize);
	tusbhHDSetToggle(phost, hcdc->InPipe, 0);

	/* �����ޡ��������� */
	pdevice->timeid = pdevice->timecount = 100;
	return TUSBH_E_OK;
}

/*
 *  CDC���饹��λ����
 *  parameter1 pdevice:  �ǥХ����ϥ�ɥ�
 *  return     TUSBH_ERCODE
 */
static TUSBH_ERCODE
tusbhCDCDeInit(TUSBH_Device_t *pdevice)
{
	CDC_Handle_t *hcdc;
	TUSBH_Handle_t *phost = pdevice->pHost;

	hcdc = (CDC_Handle_t *)pdevice->pData;
	if(hcdc->CommInPipe != 0){
		tusbhClosePipe(pdevice, hcdc->CommInPipe);
		tusbFreePipe(phost, hcdc->CommInPipe);
		hcdc->CommInPipe = 0;
	}

	if(hcdc->OutPipe != 0){
		tusbhClosePipe(pdevice, hcdc->OutPipe);
		tusbFreePipe(phost, hcdc->OutPipe);
		hcdc->OutPipe = 0;
	}

	if(hcdc->InPipe != 0){
		tusbhClosePipe(pdevice, hcdc->InPipe);
		tusbFreePipe(phost, hcdc->InPipe);
		hcdc->InPipe = 0;
	}

	if(pdevice->pData != NULL){
		tusbfree(pdevice->pData);
		pdevice->pData = NULL;
	}
	return TUSBH_E_OK;
}

/*
 *  CDC���饹�ץ����¹�
 *  parameter1 pdevice:  �ǥХ����ϥ�ɥ�
 *  parameter2 mes:      �̿���å������ؤΥݥ���
 *  return     TUSBH_ERCODE
 */
static TUSBH_ERCODE
tusbhCDCProcess(TUSBH_Device_t *pdevice, uint8_t *mes)
{
	CDC_Handle_t   *hcdc  = (CDC_Handle_t *)pdevice->pData;
	TUSBH_Handle_t *phost = pdevice->pHost;
	void (*func)(TUSBH_Device_t *, uint8_t) = pdevice->pClass->subfunc;
	TUSBH_URBState_t URB_Status = mes[3];
	TUSBH_ERCODE ercd = TUSBH_E_BUSY;
	TUSBH_ERCODE status;
	int32_t length;
	uint8_t cmd = CDC_LINE_STATUS_CHANGED;

    if(mes[0] == TUSBH_URB_EVENT)
		hcdc->urb_wait = 0;
	else if(mes[0] == TUSBH_TIME_EVENT){
		hcdc->timer += mes[3];
		pdevice->timecount = mes[3];
	}
	switch (hcdc->state){
	case CDC_PROCESS_INIT:
		CDC_REQUEST_GETLINE(pdevice, (uint8_t *)(&hcdc->oLineCoding));
		hcdc->state = CDC_PROCESS_INIT_WAIT;
		break;
	case CDC_PROCESS_INIT_WAIT:
		status = tusbhControlWait(pdevice, mes);
		if(status == TUSBH_E_OK){
			if(phost->usrcallback != NULL)
				phost->usrcallback(phost, pdevice, HOST_USER_CLASS_ACTIVE);
			hcdc->state = CDC_PROCESS_IDLE;
		}
		else if(status != TUSBH_E_BUSY && status != TUSBH_E_BUSY_URB)
			hcdc->state = CDC_PROCESS_ERROR;
		break;
	case CDC_PROCESS_IDLE:
		ercd = TUSBH_E_OK;
		break;
	case CDC_PROCESS_SET_LINE_CODING:
		CDC_REQUEST_SETLINE(pdevice, (uint8_t *)&hcdc->uLineCoding);
		hcdc->state = CDC_PROCESS_SET_LINE_CODING_WAIT;
		break;
	case CDC_PROCESS_SET_LINE_CODING_WAIT:
		status = tusbhControlWait(pdevice, mes);
		if(status == TUSBH_E_OK)
			hcdc->state = CDC_PROCESS_GET_LINE_CODING;
		else if(status != TUSBH_E_BUSY && status != TUSBH_E_BUSY_URB)
			hcdc->state = CDC_PROCESS_ERROR;
		break;
	case CDC_PROCESS_GET_LINE_CODING:
		CDC_REQUEST_GETLINE(pdevice, (uint8_t *)&hcdc->uLineCoding);
		hcdc->state = CDC_PROCESS_GET_LINE_CODING_WAIT;
		break;
	case CDC_PROCESS_GET_LINE_CODING_WAIT:
		status = tusbhControlWait(pdevice, mes);
		if(status == TUSBH_E_OK){
			hcdc->state = CDC_PROCESS_IDLE;
			if((hcdc->oLineCoding.stop_bits != hcdc->uLineCoding.stop_bits) ||
				(hcdc->oLineCoding.data_bits != hcdc->uLineCoding.data_bits) ||
				(hcdc->oLineCoding.parity != hcdc->uLineCoding.parity) ||
				(hcdc->oLineCoding.baudrate != hcdc->uLineCoding.baudrate)){
					if(func != NULL)
						func(pdevice, CDC_LINE_STATUS_CHANGED);
			}
		}
		else if(status != TUSBH_E_BUSY && status != TUSBH_E_BUSY_URB)
			hcdc->state = CDC_PROCESS_ERROR;
		break;
	case CDC_PROCESS_SEND:
		length = hcdc->TrnSize - hcdc->TrnLength;
		if(length > hcdc->TrnEpSize)
			hcdc->TrnPktSize = hcdc->TrnEpSize;
		else
			hcdc->TrnPktSize = length;
		tusbhBulkWrite(pdevice, hcdc->pTrnBuff, hcdc->TrnPktSize, hcdc->OutPipe);
		hcdc->state = CDC_PROCESS_TRANSFER_WAIT;
		break;
	case CDC_PROCESS_RECEIVE:
		length = hcdc->TrnSize - hcdc->TrnLength;
		if(length > hcdc->TrnEpSize)
			hcdc->TrnPktSize = hcdc->TrnEpSize;
		else
			hcdc->TrnPktSize = length;
		tusbhBulkRead(pdevice, hcdc->pTrnBuff, hcdc->TrnPktSize, hcdc->InPipe);
		hcdc->state = CDC_PROCESS_TRANSFER_WAIT;
		break;
	case CDC_PROCESS_TRANSFER_WAIT:
		if(hcdc->rw_status != TUSBH_E_BUSY)
			hcdc->state = CDC_PROCESS_ERROR;
		else if(mes[0] != TUSBH_URB_EVENT)
			 break;
		else if(URB_Status == TUSBH_URB_DONE){
			if(hcdc->pre_state == CDC_PROCESS_RECEIVE){
				hcdc->TrnPktSize = tusbhHDTrasLength(phost, hcdc->InPipe);
				cmd = CDC_LINE_STATUS_RECEIVED;
			}
			else
				cmd = CDC_LINE_STATUS_SENDED;
			hcdc->TrnLength += hcdc->TrnPktSize;
			hcdc->pTrnBuff += hcdc->TrnPktSize;
			if(hcdc->TrnSize > hcdc->TrnLength && hcdc->pre_state == CDC_PROCESS_SEND)
				hcdc->state = hcdc->pre_state;
			else{
				hcdc->state = CDC_PROCESS_IDLE;
				hcdc->rw_status = TUSBH_E_OK;
				if(func != NULL)
					func(pdevice, cmd);
				if(hcdc->rw_taskid != 0)
					tusbWakeup(hcdc->rw_taskid);
			}
		}
		else if(URB_Status == TUSBH_URB_NOTREADY)
			hcdc->state = hcdc->pre_state;
		else{
			hcdc->state = CDC_PROCESS_ERROR;
			hcdc->rw_status = TUSBH_E_ERR;
			if(func != NULL)
				func(pdevice, cmd);
			if(hcdc->rw_taskid != 0)
				tusbWakeup(hcdc->rw_taskid);
		}
		tusbSendData(phost->process_event, TUSBH_CLASS_EVENT, pdevice->idx, 0, 0);
		break;
	case CDC_PROCESS_ERROR:
		tusbhClearFeature(pdevice, 0x00);
		hcdc->state = CDC_PROCESS_ERROR_WAIT;
		break;
	case CDC_PROCESS_ERROR_WAIT:
		if(tusbhControlWait(pdevice, mes) == TUSBH_E_OK)
			hcdc->state = CDC_PROCESS_IDLE;
		break;
	default:
		break;
	}
	return ercd;
}

/*
 *  SERIAL LINE CODING�ǡ����ΰ������
 *  parameter1 phost:   �ۥ��ȥϥ�ɥ�
 *  parameter2 unit:    UNIT#
 *  parameter3 linecod: �桼����LINE CODING��Ǽ�ΰ�ؤΥݥ���
 *  return     TUSBH_ERCODE
 */
TUSBH_ERCODE
tusbhCdcSetLineCoding(TUSBH_Handle_t *phost, uint8_t unit, Line_Coding_t *linecod)
{
	TUSBH_Device_t *pdevice = tusbhSearchDevice(phost, SERAIL_Class.classCode, &unit);
	CDC_Handle_t *hcdc;

	if(pdevice == NULL)
		return TUSBH_E_OBJ;
	hcdc = (CDC_Handle_t *)pdevice->pData;
	if(pdevice->dstate == DEVICE_CLASS){
		tusbmemcpy(&hcdc->uLineCoding, linecod, sizeof(Line_Coding_t));
		hcdc->state = CDC_PROCESS_SET_LINE_CODING;
		tusbSendData(phost->process_event, TUSBH_CLASS_EVENT, pdevice->idx, 0, 0);
	}
	return TUSBH_E_OK;
}

/*
 *  SERIAL LINE CODING�ǡ�������
 *  parameter1 phost:   �ۥ��ȥϥ�ɥ�
 *  parameter2 unit:    UNIT#
 *  parameter3 linecod: LINE CODING��Ǽ�ΰ�ؤΥݥ���
 *  return     TUSBH_ERCODE
*/
TUSBH_ERCODE
tusbhCdcGetLineCoding(TUSBH_Handle_t *phost, uint8_t unit, Line_Coding_t *linecod)
{
	TUSBH_Device_t *pdevice = tusbhSearchDevice(phost, SERAIL_Class.classCode, &unit);
	CDC_Handle_t *hcdc;

	if(pdevice == NULL)
		return TUSBH_E_OBJ;
	hcdc = pdevice->pData;
	*linecod = hcdc->oLineCoding;
	return TUSBH_E_OK;
}


/*
 *  CDC DATA RECEIVE
 *  parameter1 phost:   �ۥ��ȥϥ�ɥ�
 *  parameter2 unit:    UNIT#
 *  parameter3 pbuf:    �����ǡ����ΰ�ؤΥݥ���
 *  parameter4 length:  �����֥�å�Ĺ
 *  return     TUSBH_ERCODE
 */
TUSBH_ERCODE
tusbhCdcReceive(TUSBH_Handle_t *phost, uint8_t unit, uint8_t *pbuf, uint32_t *length, uint32_t timeout)
{
	TUSBH_Device_t *pdevice = tusbhSearchDevice(phost, SERAIL_Class.classCode, &unit);
	CDC_Handle_t *hcdc;
	TUSBH_ERCODE status = TUSBH_E_BUSY;
	uint32_t i;

	if(pdevice != NULL){
		hcdc = pdevice->pData;
		if(pdevice->dstate != DEVICE_CLASS)
			return TUSBH_E_OBJ;
		if(hcdc->state == CDC_PROCESS_ERROR)
			return TUSBH_E_ERR;
		if(hcdc->state != CDC_PROCESS_IDLE)
			return TUSBH_E_OBJ;
	}
	else
		return TUSBH_E_OBJ;

    /*
	 *  �����ǡ�������
	 */
	hcdc->TrnSize   = *length;
	hcdc->TrnLength = 0;
	hcdc->pTrnBuff  = pbuf;
	hcdc->TrnEpSize = hcdc->InEpSize;
	hcdc->state     = CDC_PROCESS_RECEIVE;
	hcdc->pre_state = CDC_PROCESS_RECEIVE;
	hcdc->rw_status = TUSBH_E_BUSY;
	hcdc->rw_taskid = tusbGetTaskID();
	tusbSendData(phost->process_event, TUSBH_CLASS_EVENT, pdevice->idx, 0, 0);

	if(timeout != 0)
		timeout += 9;
	timeout /= 10;
	for(i = 0 ; i < timeout ; i++){
		tusbSleep(10);
		if((status = hcdc->rw_status) != TUSBH_E_BUSY)
			break;
	}
	if(status == TUSBH_E_BUSY){
		hcdc->rw_status = TUSBH_E_TMOUT;
		status = TUSBH_E_TMOUT;
	}
	*length = hcdc->TrnLength;
	hcdc->rw_taskid = 0;
	return status;
}

/*
 *  CDC DATA SEND
 *  parameter1 phost:   �ۥ��ȥϥ�ɥ�
 *  parameter2 unit:    UNIT#
 *  parameter3 pbuf:    �����ǡ����ΰ�ؤΥݥ���
 *  parameter4 length:  �����ǡ���Ĺ
 *  return     TUSBH_ERCODE
 */
TUSBH_ERCODE
tusbhCdcSend(TUSBH_Handle_t *phost, uint8_t unit, uint8_t *pbuf, uint32_t length)
{
	TUSBH_Device_t *pdevice = tusbhSearchDevice(phost, SERAIL_Class.classCode, &unit);
	CDC_Handle_t *hcdc;
	TUSBH_ERCODE status = TUSBH_E_BUSY;
	uint32_t i;

	if(pdevice != NULL){
		hcdc = pdevice->pData;
		if(pdevice->dstate != DEVICE_CLASS)
			return TUSBH_E_OBJ;
		if(hcdc->state == CDC_PROCESS_ERROR)
			return TUSBH_E_ERR;
		if(hcdc->state != CDC_PROCESS_IDLE)
			return TUSBH_E_OBJ;
	}
	else
		return TUSBH_E_OBJ;

    /*
	 *  �����ǡ�������
	 */
	hcdc->TrnSize   = length;
	hcdc->TrnLength = 0;
	hcdc->pTrnBuff  = pbuf;
	hcdc->TrnEpSize = hcdc->OutEpSize;
	hcdc->state     = CDC_PROCESS_SEND;
	hcdc->pre_state = CDC_PROCESS_SEND;
	hcdc->rw_status = TUSBH_E_BUSY;
	hcdc->rw_taskid = tusbGetTaskID();
	tusbSendData(phost->process_event, TUSBH_CLASS_EVENT, pdevice->idx, 0, 0);

	for(i = 0 ; i < (length * 10) ; i++){
		tusbSleep(50);
		if((status = hcdc->rw_status) != TUSBH_E_BUSY)
			break;
	}
	if(status == TUSBH_E_BUSY){
		hcdc->rw_status = TUSBH_E_TMOUT;
		status = TUSBH_E_TMOUT;
	}
	hcdc->rw_taskid = 0;
	return status;
}

/*
 *  SERIAL������Хå��ؿ�
 *  parameter1 phost:   �ۥ��ȥϥ�ɥ�
 *  parameter2 func:    �ؿ��ݥ���
 */
void
tusbhSetCdcCallBack(TUSBH_Handle_t *phost, void (*func)(TUSBH_Device_t *p, uint8_t))
{
	SERAIL_Class.subfunc = func;
}


