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
 *  @(#) $Id: tusbh_hid.c 698 2017-09-17 16:35:15Z roi $
 */
/*
 *  USB Host Middleware HID CLASS��
 */

#include "tusbh_hid.h"


#define HID_SETIDLE(d, dur, rep)     tusbhControlRequest((d), NULL, HID_SET_TYPE, USB_HID_SET_IDLE, ((dur << 8) | (rep)), 0, 0)
#define HID_SETPROTOCOL(d, p)        tusbhControlRequest((d), NULL, HID_SET_TYPE, USB_HID_SET_PROTOCOL, (((p) != 0) ? 0 : 1), 0, 0)
#define HID_GETREPORT(d, t, i, b, l) tusbhControlRequest((d), (b), HID_GET_TYPE, USB_HID_GET_REPORT, ((t << 8 ) | i), 0, (l))
#define HID_SETREPORT(d, t, i, b, l) tusbhControlRequest((d), (b), HID_SET_TYPE, USB_HID_SET_REPORT, ((t << 8 ) | i), 0, (l))

static TUSBH_ERCODE tusbhHIDInit(TUSBH_Device_t *pdevice);
static TUSBH_ERCODE tusbhHIDDeInit(TUSBH_Device_t *pdevice);
static TUSBH_ERCODE tusbdHIDProcess(TUSBH_Device_t *pdevice, uint8_t *mes);

TUSBH_Class_t HID_Class = {
	NULL,
	"HID",
	HID_CLASS,
	tusbhHIDInit,
	tusbhHIDDeInit,
	tusbdHIDProcess,
	NULL
};

/*
 *  HID���饹���åȥ��å״ؿ�
 */
void tusbhLinkHID(TUSBH_Handle_t *phost)
{
	tusbhLinkClass(phost, &HID_Class);
}

/*
 *  HID���饹�������
 *  parameter1 pdevice:  �ǥХ����ϥ�ɥ�
 *  return     TUSBH_ERCODE
 */
static TUSBH_ERCODE
tusbhHIDInit(TUSBH_Device_t *pdevice)
{
	TUSBH_Handle_t *phost = pdevice->pHost;
	uint8_t max_ep;
	uint8_t interface;
	TUSBH_ERCODE status = TUSBH_E_ERR;
	HID_Handle_t *hhid;

	interface = tusbhFindInterface(pdevice, pdevice->pClass->classCode, HID_BOOT_CODE, 0xFF);

	if(interface == NO_INTERFACE){
		syslog_1(LOG_ERROR, "[HID] Cannot Find the interface for %s class !", pdevice->pClass->Name);
		return TUSBH_E_ERR;
	}

	tusbhSelectInterface(pdevice, interface);
	pdevice->pData = tusbmalloc(sizeof(HID_Handle_t));
	hhid = (HID_Handle_t *)pdevice->pData;
	memset(hhid, 0, sizeof(HID_Handle_t));
	hhid->state = THID_ERROR;

	/*
	 *  �ǥХ����θ���
	 */
	if(pdevice->ItfDesc[pdevice->sel_interface].bInterfaceProtocol == HID_KEYBRD_BOOT_CODE){
		syslog_0(LOG_NOTICE, "[HID] found KeyBoard device !"); 
		hhid->type =  HID_KEYBOARD_TYPE;
	}
	else if(pdevice->ItfDesc[pdevice->sel_interface].bInterfaceProtocol == HID_MOUSE_BOOT_CODE){
		syslog_0(LOG_NOTICE, "[HID] found Mouse device !");
		hhid->type =  HID_MOUSE_TYPE;
	}
	else{
		syslog_0(LOG_ERROR, "[HID] no device !");
		return TUSBH_E_ERR;
	}

	hhid->ep_addr   = pdevice->EpDesc[pdevice->sel_interface][0].bEndpointAddress;
	hhid->length    = pdevice->EpDesc[pdevice->sel_interface][0].wMaxPacketSize;
	hhid->poll      = pdevice->EpDesc[pdevice->sel_interface][0].bInterval ;

	if(hhid->poll < HID_MIN_POLL)
		hhid->poll = HID_MIN_POLL;

	max_ep = pdevice->ItfDesc[pdevice->sel_interface].bNumEndpoints;
	if(max_ep == 0 || (pdevice->EpDesc[pdevice->sel_interface][0].bEndpointAddress & 0x80) == 0)
		return TUSBH_E_ERR;

#if 1	/* ROI DEBUG */
	syslog_3(LOG_NOTICE, "## max_ep(%d) length(%d) poll(%d) ##", max_ep, hhid->length, hhid->poll);
#endif	/* ROI DEBUG */
	if(hhid->length > HID_MAX_REPORT_LENGTH)
		hhid->length = HID_MAX_REPORT_LENGTH;

	/*
	 *  ���󥿡���ץ� ���󥨥�ɥݥ���� �����ץ�
	 */
	hhid->InPipe  = tusbhAllocPipe(phost, hhid->ep_addr);
	tusbhOpenPipe(pdevice, hhid->InPipe, hhid->ep_addr, USB_EP_TYPE_INTR, hhid->length);
	tusbhHDSetToggle(phost, hhid->InPipe, 0);

	/*
	 *  HID DESCRIPTOR����
	 */
	if(tusbhGetDescriptor(pdevice, (USB_RECIPIENT_INTERFACE | USB_REQUEST_TYPE_STANDARD),
						((HID_DESCRIPTOR << 8) & 0xFF00), pdevice->Data, HID_DESCRIPTOR_LENGTH) != TUSBH_E_OK)
		return TUSBH_E_ERR;
	hhid->ReportDescLength  = (pdevice->Data[7] & 0xff) | (pdevice->Data[8] << 8);
	syslog_1(LOG_NOTICE, "## hhid->DescLength(%d) ##", hhid->ReportDescLength);
	if(hhid->ReportDescLength > 64)
		hhid->ReportDescLength = 64;

	/*
	 *  HID REPORT DESCRIPTOR����
	 */
	if(tusbhGetDescriptor(pdevice, (USB_RECIPIENT_INTERFACE | USB_REQUEST_TYPE_STANDARD),
						((HID_REPORT_DESCRIPTOR << 8) & 0xFF00), pdevice->Data, hhid->ReportDescLength) != TUSBH_E_OK)
		return TUSBH_E_ERR;
	tusbmemcpy(hhid->ReportDesc, pdevice->Data, hhid->ReportDescLength);
	syslog_1(LOG_NOTICE, "## hhid->ReportDesc[%08x] ##", hhid->ReportDesc);

	/*
	 *  IDEL����
	 */
	status = HID_SETIDLE(pdevice, 0, 0);
	if(status != TUSBH_E_OK && status != TUSBH_E_NOSPT)
		return status;

	/*
	 *  PROTOCOL #0 ����
	 */
	if((status = HID_SETPROTOCOL(pdevice, 0)) != TUSBH_E_OK)
		return status;

	/* �����ޡ��������� */
	hhid->state     = THID_INIT;
	pdevice->timeid = pdevice->timecount = hhid->poll;
	return TUSBH_E_OK;
}

/*
 *  HUB���饹��λ����
 *  parameter1 pdevice:  �ǥХ����ϥ�ɥ�
 *  return     TUSBH_ERCODE
 */
static TUSBH_ERCODE
tusbhHIDDeInit(TUSBH_Device_t *pdevice)
{
	TUSBH_Handle_t *phost = pdevice->pHost;
	HID_Handle_t *hhid;

	hhid =  (HID_Handle_t *)pdevice->pData;
	if(hhid->InPipe != 0x00){
		tusbhClosePipe(pdevice, hhid->InPipe);
		tusbFreePipe(phost, hhid->InPipe);
		hhid->InPipe = 0;
	}

	if(pdevice->pData != NULL){
		tusbfree(pdevice->pData);
		pdevice->pData = NULL;
	}
	return TUSBH_E_OK;
}


/*
 *  HID���饹�ץ����¹�
 *  parameter1 pdevice:  �ǥХ����ϥ�ɥ�
 *  parameter2 mes:      �̿���å������ؤΥݥ���
 *  return     TUSBH_ERCODE
 */
static TUSBH_ERCODE
tusbdHIDProcess(TUSBH_Device_t *pdevice, uint8_t *mes)
{
	TUSBH_ERCODE status = TUSBH_E_OK, status2;
	HID_Handle_t *hhid = (HID_Handle_t *)pdevice->pData;
	TUSBH_URBState_t URB_Status = mes[3];
	TUSBH_Handle_t *phost = pdevice->pHost;
	void (*func)(TUSBH_Device_t *, uint8_t, uint8_t *) = pdevice->pClass->subfunc;

	switch (hhid->state){
	case THID_INIT:
		if(phost->usrcallback != NULL)
			phost->usrcallback(phost, pdevice, HOST_USER_CLASS_ACTIVE);
	case THID_IDLE:
		hhid->ReportType = 0x01;
		hhid->ReportId   = 0;
		HID_GETREPORT(pdevice, hhid->ReportType, hhid->ReportId, hhid->buffer, hhid->length);
		hhid->state = THID_GETREPORT_WAIT;
		break;
	case THID_GETREPORT_WAIT:
		if((status2 = tusbhControlWait(pdevice, mes)) == TUSBH_E_OK){
			hhid->ReportLength = hhid->length;
			tusbmemcpy(hhid->Report, hhid->buffer, hhid->ReportLength);
			if(func != NULL)
				func(pdevice, 0, hhid->Report);
			syslog_2(LOG_NOTICE, "## THID_GETREPORT_WAIT hhid->Report[%08x] (%d) ##", hhid->Report, hhid->ReportLength);
			hhid->state = THID_GET_DATA;
			pdevice->timeid    = hhid->poll;
			pdevice->timecount = hhid->poll;
		}
		else if(status2 == TUSBH_E_NOSPT){
			if(func != NULL)
				func(pdevice, 0, 0);
			hhid->state = THID_GET_DATA;
			pdevice->timeid    = hhid->poll;
			pdevice->timecount = hhid->poll;
		}
		else if(status2 != TUSBH_E_BUSY && status2 != TUSBH_E_BUSY_URB){
			syslog_1(LOG_NOTICE, "## tusbdHIDProcess ERROR status(%d) ##", status2);
			hhid->state = THID_ERROR;
		}
		break;
	case THID_SETREPORT:
		tusbmemcpy(hhid->buffer, hhid->Report, hhid->ReportLength);
		HID_SETREPORT(pdevice, hhid->ReportType, hhid->ReportId, hhid->buffer, hhid->ReportLength);
		hhid->state = THID_SETREPORT_WAIT;
		break;
	case THID_SETREPORT_WAIT:
		if((status2 = tusbhControlWait(pdevice, mes)) == TUSBH_E_OK){
			syslog_2(LOG_NOTICE, "## THID_SETREPORT_WAIT(1) hhid->Report[%08x] (%d) ##", hhid->Report, hhid->ReportLength);
			hhid->state = THID_GET_DATA;
			pdevice->timeid    = hhid->poll;
			pdevice->timecount = hhid->poll;
		}
		else if(status2 == TUSBH_E_NOSPT){
			syslog_2(LOG_NOTICE, "## THID_SETREPORT_WAIT(2) hhid->Report[%08x] (%d) ##", hhid->Report, hhid->ReportLength);
			hhid->state = THID_GET_DATA;
			pdevice->timeid    = hhid->poll;
			pdevice->timecount = hhid->poll;
		}
		else if(status2 != TUSBH_E_BUSY && status2 != TUSBH_E_BUSY_URB){
			syslog_1(LOG_NOTICE, "## tusbdHIDProcess ERROR status(%d) ##", status2);
			hhid->state = THID_ERROR;
		}
		break;
	case THID_GET_DATA:
		if(pdevice->is_connected == 0)
			break;
		if(hhid->ReqReport == 0xFF){
			hhid->state = THID_SETREPORT;
			tusbSendData(phost->process_event, TUSBH_CLASS_EVENT, pdevice->idx, 0, 0);
			break;
		}
		tusbhInterruptRead(pdevice, hhid->buffer, hhid->length, hhid->InPipe);
		hhid->state = THID_POLL;
		hhid->DataReady = 0;
		break;
	case THID_POLL:
		if(mes[0] != TUSBH_URB_EVENT)
			break;
		if(URB_Status == TUSBH_URB_DONE){
			if(hhid->DataReady == 0){
        		hhid->DataReady = 1;
				if(func != NULL)
					func(pdevice, hhid->length, hhid->buffer);
				pdevice->timeid    = hhid->poll;
				pdevice->timecount = hhid->poll;
				hhid->state = THID_GET_DATA;
			}
		}
		else if(URB_Status == TUSBH_URB_STALL){
			/*
			 *  STALL�ʤ�CLEAR FEATURE�¹�
			 */
			if(tusbhClearFeature(pdevice, hhid->ep_addr) == TUSBH_E_OK){
				/* Change state to issue next IN token */
				hhid->state =THID_CLEARFEATURE_WAIT;
			}
		}
		else{
			pdevice->timeid    = hhid->poll;
			pdevice->timecount = hhid->poll;
			hhid->state = THID_GET_DATA;
		}
		break;
	case THID_CLEARFEATURE_WAIT:
		if((status2 = tusbhControlWait(pdevice, mes)) == TUSBH_E_OK){
			hhid->state = THID_GET_DATA;
			pdevice->timeid    = hhid->poll;
			pdevice->timecount = hhid->poll;
		}
		else if(status2 == TUSBH_E_NOSPT){
			hhid->state = THID_GET_DATA;
			pdevice->timeid    = hhid->poll;
			pdevice->timecount = hhid->poll;
		}
		else if(status2 != TUSBH_E_BUSY && status2 != TUSBH_E_BUSY_URB){
			syslog_1(LOG_ERROR, "tusbdHIDProcess ERROR status(%d) !", status2);
			hhid->state = THID_ERROR;
		}
		break;

	default:
		break;
	}
	return status;
}

/*
 *  HID�ǥХ�����ݡ�������
 *  parameter1 hhid:    HID�ϥ�ɥ�
 *  parameter2 type:    report type
 *  parameter3 id:      report id
 *  parameter4 pbuf:    report data
 *  parameter5 len:     report data length
 *  return     TUSBH_ERCODE
 */
TUSBH_ERCODE
tubhHidSetReport(HID_Handle_t *hhid, uint8_t type, uint8_t id, uint8_t* pbuf, uint8_t len)
{
	hhid->ReportType   = type;
	hhid->ReportId     = id;
	hhid->ReportLength = len;
	tusbmemcpy(hhid->Report, pbuf, hhid->ReportLength);
	hhid->ReqReport = 0xFF;
	while(hhid->ReqReport == 0xFF){
		tusbSleep(10);
	}
	return (TUSBH_ERCODE)hhid->ReqReport;
}

/*
 *  HID�ǥХ��������׼���
 *  parameter1 hhid:    HID�ϥ�ɥ�
 *  return     poll     �ǥХ���������
 */
uint8_t
tusbhHidGetType(TUSBH_Device_t *pdevice)
{
	HID_Handle_t *hhid = (HID_Handle_t *)pdevice->pData;

	if(hhid == NULL)
		return HID_UNKNOWN_TYPE;
	else
		return hhid->type;
}


/*
 *  HID�ݡ���󥰥��󥿡��Х���ּ���
 *  parameter1 hhid:    HID�ϥ�ɥ�
 *  return     poll     ���󥿡��Х����(ms)
 */
uint8_t
tusbhHidGetPollInterval(TUSBH_Device_t *pdevice)
{
	HID_Handle_t *hhid = (HID_Handle_t *)pdevice->pData;

	if(hhid == NULL)
		return 0;
	else
		return hhid->poll;
}

/*
 *  HID ������Хå��ؿ�����
 */
void
tusbhSetHidCallBack(TUSBH_Handle_t *phost, void (*func)(TUSBH_Device_t *pdevice, uint8_t, uint8_t *))
{
	HID_Class.subfunc = func;
}

