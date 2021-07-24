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
 *  @(#) $Id: tusbh_request.c 698 2017-09-10 16:57:23Z roi $
 */
/*
 *  USB Host Middleware �̿��ꥯ��������
 */

#include "tusbh_base.h"


/*
 *  BULK�̿���WRITE
 *  parameter1 pdevice:  �ǥХ����ϥ�ɥ�
 *  parameter2 buff:     �ǡ����ΰ�
 *  parameter3 length:   �ǡ���Ĺ
 *  parameter4 pipe_num: PIPE�ֹ�
 *  return     TUSBH_ERCOCE
 */
TUSBH_ERCODE
tusbhBulkWrite(TUSBH_Device_t *pdevice, uint8_t *buff, uint16_t length, uint8_t pipe_num)
{ 
	return tusbhSubmitURB(pdevice, pipe_num, SUBMIT_WRITE, USB_EP_TYPE_BULK, buff, length);
}

/*
 *  BULK�̿���READ
 *  parameter1 pdevice:  �ǥХ����ϥ�ɥ�
 *  parameter2 buff:     �ǡ����ΰ�
 *  parameter3 length:   �ǡ���Ĺ
 *  parameter4 pipe_num: PIPE�ֹ�
 *  return     TUSBH_ERCOCE
 */
TUSBH_ERCODE
tusbhBulkRead(TUSBH_Device_t *pdevice, uint8_t *buff, uint16_t length, uint8_t pipe_num)
{
	return tusbhSubmitURB(pdevice, pipe_num, SUBMIT_READ, USB_EP_TYPE_BULK, buff, length);
}

/*
 *  INTERRUPT�̿���WRITE
 *  parameter1 pdevice:  �ǥХ����ϥ�ɥ�
 *  parameter2 buff:     �ǡ����ΰ�
 *  parameter3 length:   �ǡ���Ĺ
 *  parameter4 pipe_num: PIPE�ֹ�
 *  return     TUSBH_ERCOCE
 */
TUSBH_ERCODE
tusbhInterruptWrite(TUSBH_Device_t *pdevice, uint8_t *buff, uint8_t length, uint8_t pipe_num)
{
	return tusbhSubmitURB(pdevice, pipe_num, SUBMIT_WRITE, USB_EP_TYPE_INTR, buff, length);
}

/*
 *  INTERRUPT�̿���READ
 *  parameter1 pdevice:  �ǥХ����ϥ�ɥ�
 *  parameter2 buff:     �ǡ����ΰ�
 *  parameter3 length:   �ǡ���Ĺ
 *  parameter4 pipe_num: PIPE�ֹ�
 *  return     TUSBH_ERCOCE
 */
TUSBH_ERCODE
tusbhInterruptRead(TUSBH_Device_t *pdevice, uint8_t *buff, uint8_t length, uint8_t pipe_num)
{
	return tusbhSubmitURB(pdevice, pipe_num, SUBMIT_READ, USB_EP_TYPE_INTR, buff, length);
}

/*
 *  ISOC�̿���WRITE
 *  parameter1 pdevice:  �ǥХ����ϥ�ɥ�
 *  parameter2 buff:     �ǡ����ΰ�
 *  parameter3 length:   �ǡ���Ĺ
 *  parameter4 pipe_num: PIPE�ֹ�
 *  return     TUSBH_ERCOCE
 */
TUSBH_ERCODE
tusbhIsocWrite(TUSBH_Device_t *pdevice, uint8_t *buff, uint32_t length, uint8_t pipe_num)
{
	return tusbhSubmitURB(pdevice, pipe_num, SUBMIT_WRITE, USB_EP_TYPE_ISOC, buff, length);
}

/**
  * @brief  USBH_IsocReceiveData
  *         Receives the Device Response to the Isochronous IN token
  * @param  phost: Host Handle
  * @param  buff: Buffer pointer in which the response needs to be copied
  * @param  length: Length of the data to be received
  * @param  pipe_num: Pipe Number
  * @retval USBH Status. 
  */
TUSBH_ERCODE
tusbhIsocRead(TUSBH_Device_t *pdevice, uint8_t *buff, uint32_t length, uint8_t pipe_num)
{
	return tusbhSubmitURB(pdevice, pipe_num, SUBMIT_READ, USB_EP_TYPE_ISOC, buff, length);
}


/*
 *  ����ȥ����̿���SETUP����
 *  parameter1 pdevice:  �ǥХ����ϥ�ɥ�
 *  parameter2 buff:     ���åȥ��åץǡ����ΰ�
 *  parameter3 pipe_num: PIPE�ֹ�
 *  return     TUSBH_ERCOCE
 */
TUSBH_ERCODE
tusbhControlSendSetup(TUSBH_Device_t *pdevice, uint8_t *buff, uint8_t pipe_num)
{
	return tusbhSubmitURB(pdevice, pipe_num, SUBMIT_SETUP, USB_EP_TYPE_CTRL, buff, SETUP_PKT_LENGTH);
}

/*
 *  ����ȥ����̿����ǡ�������
 *  parameter1 pdevice:  �ǥХ����ϥ�ɥ�
 *  parameter2 buff:     �ǡ����ΰ�
 *  parameter3 length:   �ǡ���Ĺ
 *  parameter4 pipe_num: �ѥ����ֹ�
 *  return     TUSBH_ERCOCE
 */
TUSBH_ERCODE
tusbhControlSendData(TUSBH_Device_t *pdevice, uint8_t *buff, uint16_t length, uint8_t pipe_num)
{
	return tusbhSubmitURB(pdevice, pipe_num, SUBMIT_WRITE, USB_EP_TYPE_CTRL, buff, length);
}

/*
 *  ����ȥ����̿����ǡ�������
 *  parameter1 pdevice:  �ǥХ����ϥ�ɥ�
 *  parameter2 buff:     �ǡ����ΰ�
 *  parameter3 length:   �ǡ���Ĺ
 *  parameter4 pipe_num: �ѥ����ֹ�
 *  return     TUSBH_ERCOCE
 */
TUSBH_ERCODE
tusbhControlReceiveData(TUSBH_Device_t *pdevice, uint8_t* buff, uint16_t length, uint8_t pipe_num)
{
	return tusbhSubmitURB(pdevice, pipe_num, SUBMIT_READ, USB_EP_TYPE_CTRL, buff, length);
}

/*
 *  ����ȥ����̿���DESCRIPTOR����
 *  parameter1 pdevice:  �ǥХ����ϥ�ɥ�
 *  parameter2 req_type: DESCRIPTOR������
 *  parameter3 value:    DESCRIPTOR�׵���
 *  parameter4 buff:     DESCRIPTO��Ǽ�ΰ�
 *  parameter5 length:   DESCRIPTORĹ
 *  return     TUSBH_ERCOCE
 */
TUSBH_ERCODE
tusbhGetDescriptor(TUSBH_Device_t *pdevice, uint8_t req_type, uint16_t value, uint8_t* buff, uint16_t length)
{
	uint16_t index;

	if((value & 0xff00) == ((STRING_DESCRIPTOR << 8) & 0xFF00)){
		index = 0x0409;
	}
	else
		index = 0;
	return tusbhControlRequest(pdevice, buff, (USB_DEVICE_TO_HOST | req_type), GET_DESCRIPTOR, value, index, length );
}

/*
 *  ����ȥ����̿���SET INTERFACE������
 *  parameter1 pdevice: �ǥХ����ϥ�ɥ�
 *  parameter2 ep_num:  ����ɥݥ�����ֹ�
 *  parameter3 interface: ���󥿥ե�������
 *  return     TUSBH_ERCOCE
 */
TUSBH_ERCODE
tusbhSetInterface(TUSBH_Device_t *pdevice, uint8_t ep_num, uint8_t interface)
{
	return tusbhControlRequest(pdevice, 0, SET_INTERFACE_TYPE, SET_INTERFACE, interface, ep_num, 0);
}

/*
 *  ����ȥ����̿���CLEAR FEATURE������
 *  parameter1 pdevice: �ǥХ����ϥ�ɥ�
 *  parameter2 ep_num:  ����ɥݥ�����ֹ�
 *  return     TUSBH_ERCOCE
 */
TUSBH_ERCODE
tusbhClearFeature(TUSBH_Device_t *pdevice, uint8_t ep_num) 
{
	return tusbhControlRequest(pdevice, 0, CLEAR_FEATURE_TYPE, CLEAR_FEATURE, FEATURE_SELECTOR_ENDPOINT, ep_num, 0 );
}


/*
 *  ����ȥ����̿����׵�
 *  parameter1 pdevice: �ǥХ����ϥ�ɥ�
 *  parameter2 buff:    �̿��Хåե�
 *  parameter3 type:    bmRequestType
 *  parameter4 request: bRequest
 *  parameter5 value:   wValue
 *  parameter6 index:   wIndex
 *  parameter7 length:  wLength
 *  return     TUSBH_ERCOCE
 */
TUSBH_ERCODE
tusbhControlRequest(TUSBH_Device_t *pdevice, uint8_t *buff, uint8_t type, uint8_t request, uint16_t value, uint16_t index, uint16_t length)
{
	TUSBH_Handle_t *phost = pdevice->pHost;
	TUSBH_ERCODE ercd = TUSBH_E_BUSY;
	uint8_t mes[4];

	pdevice->cstate = CONTROL_SETUP;
	pdevice->setupPacket[0] = type;
	pdevice->setupPacket[1] = request;
	pdevice->setupPacket[2] = (uint8_t)value;
	pdevice->setupPacket[3] = (uint8_t)(value >> 8);
	pdevice->setupPacket[4] = (uint8_t)index;
	pdevice->setupPacket[5] = (uint8_t)(index >> 8);
	pdevice->setupPacket[6] = (uint8_t)length; 
	pdevice->setupPacket[7] = (uint8_t)(length >> 8); 
	pdevice->cbuff          = buff;
	mes[0]                  = TUSBH_PORT_EVENT;

	do{
		ercd = tusbhControlWait(pdevice, mes);
		if(pdevice->dstate == DEVICE_CLASS)
			return ercd;
		while(ercd == TUSBH_E_BUSY_URB){
			if(tusbRecvData(phost->process_event, mes, 4000) != E_OK){
				syslog_1(LOG_ERROR, "tusbhControlRequest urb timeout state(%d) !", pdevice->cstate);
				ercd = TUSBH_E_TMOUT;
				break;
			}
			else if(mes[0] == TUSBH_URB_EVENT && mes[1] == pdevice->idx){
				ercd = TUSBH_E_BUSY;
			}
			else if(mes[1] != pdevice->idx){
				TUSBH_Device_t *pdevice2 = phost->pDevice;
				uint8_t i;
				for(i = 0 ; i < mes[1] ; i++)
					pdevice2 = pdevice2->pNext;
				if(pdevice2 != NULL && pdevice2->pClass != NULL && pdevice2->dstate == DEVICE_CLASS)
					pdevice2->pClass->Process(pdevice2, mes);
				else{
#if 1	/* ROI DEBUG */
					syslog_4(LOG_ERROR, "### tusbhControlRequest Can't EXEC [%02x][%02x][%02x][%02x] ###", mes[0], mes[1], mes[2], mes[3]);
#endif	/* ROI DEBUG */
					tusbDelay(10);
					tusbSendData(phost->process_event, mes[0], mes[1], mes[2], mes[3]);
				}
			}
		}
	}while(ercd == TUSBH_E_BUSY);
	return ercd;
}

/*
 *  ����ȥ����̿����Ԥ�
 *  parameter1 pdevice: �ǥХ����ϥ�ɥ�
 *  parameter2 mes:     ��å������ؤΥݥ���
 *  return     TUSBH_ERCOCE
 */
TUSBH_ERCODE
tusbhControlWait(TUSBH_Device_t *pdevice, uint8_t *mes)
{
	TUSBH_Handle_t *phost = pdevice->pHost;
	TUSBH_ERCODE ercd = TUSBH_E_BUSY;
	TUSBH_URBState_t URB_Status = mes[3];
	uint8_t  type = pdevice->setupPacket[0];
	uint16_t length = (pdevice->setupPacket[7] << 8) | (pdevice->setupPacket[6] & 0xFF);

	switch(pdevice->cstate){
	case CONTROL_SETUP:	/* PACKET���åȥ��å� */
		tusbhControlSendSetup(pdevice, (uint8_t *)&pdevice->setupPacket, pdevice->cntl_pipe_out); 
		pdevice->cstate = CONTROL_SETUP_WAIT;
		ercd = TUSBH_E_BUSY_URB;
		break;
	case CONTROL_SETUP_WAIT:	/* PACKET���åȥ��å� URB�Ԥ� */
		if(mes[0] != TUSBH_URB_EVENT)
			break;
		if(URB_Status == TUSBH_URB_DONE){	/* ���ｪλ */
			uint8_t direction = (type & USB_REQUEST_DIR_MASK);
			if(length != 0 ){
				if(direction == USB_DEVICE_TO_HOST){	/* �ǡ���READ */
					tusbhControlReceiveData(pdevice, pdevice->cbuff, length, pdevice->cntl_pipe_in);
					pdevice->cstate = CONTROL_DATA_IN_WAIT;
				}
				else{						/* �ǡ���WRITE */
					tusbhControlSendData(pdevice, pdevice->cbuff, length, pdevice->cntl_pipe_out);
					pdevice->cstate = CONTROL_DATA_OUT_WAIT;
				}
			}
			else{			/* ���ơ������̿� */
				if(direction == USB_DEVICE_TO_HOST){	/* ���ơ��������� */
					tusbhControlSendData(pdevice, 0, 0, pdevice->cntl_pipe_out);
					pdevice->cstate = CONTROL_STATUS_OUT_WAIT;
				}
				else{						/* ���ơ��������� */
					tusbhControlReceiveData(pdevice, 0, 0, pdevice->cntl_pipe_in);
					pdevice->cstate = CONTROL_STATUS_IN_WAIT;
				}
			}
			ercd = TUSBH_E_BUSY_URB;
		}
		else if(URB_Status == TUSBH_URB_ERROR)	/* USB���顼 */
			pdevice->cstate = CONTROL_ERROR;
		else if(URB_Status == TUSBH_URB_NOTREADY){
			syslog_0(LOG_NOTICE, "### tusbhControlRequest REAET STATE1 ###");
			pdevice->cstate = CONTROL_SETUP;
		}
		else{
			syslog_2(LOG_ERROR, "tusbhControlRequest urb error state(%d) urb[%02x] !", pdevice->cstate, URB_Status);
			ercd = TUSBH_E_URB;
		}
		break;
	case CONTROL_DATA_IN_WAIT:		/* �ǡ��������Ԥ� */
		if(mes[0] != TUSBH_URB_EVENT)
			break;
		if(URB_Status == TUSBH_URB_DONE){	/* ���ｪλ */
			tusbhControlSendData(pdevice, 0, 0, pdevice->cntl_pipe_out);
			pdevice->cstate = CONTROL_STATUS_OUT_WAIT;
			ercd = TUSBH_E_BUSY_URB;
		}
		else if(URB_Status == TUSBH_URB_STALL){	/* ���ơ������ */
			pdevice->cstate = CONTROL_IDLE;
			ercd = TUSBH_E_NOSPT;
		}
		else if(URB_Status == TUSBH_URB_ERROR)	/* URB���顼 */
			pdevice->cstate = CONTROL_ERROR;
		else if(URB_Status == TUSBH_URB_NOTREADY){
			syslog_0(LOG_NOTICE, "### tusbhControlRequest REAET STATE2 ###");
			pdevice->cstate = CONTROL_SETUP;
		}
		else{
			syslog_2(LOG_ERROR, "tusbhControlRequest urb error state(%d) urb[%02x] !", pdevice->cstate, URB_Status);
			ercd = TUSBH_E_URB;
		}
		break;
	case CONTROL_DATA_OUT_WAIT:	/* �ǡ���������λ�Ԥ� */
		if(mes[0] != TUSBH_URB_EVENT)
			break;
		if(URB_Status == TUSBH_URB_DONE){	/* ���ｪλ */
			tusbhControlReceiveData(pdevice, 0, 0, pdevice->cntl_pipe_in);
			pdevice->cstate = CONTROL_STATUS_IN_WAIT;
			ercd = TUSBH_E_BUSY_URB;
		}
		else if(URB_Status == TUSBH_URB_STALL){	/* ���ơ������ */
			pdevice->cstate = CONTROL_IDLE;
			ercd = TUSBH_E_NOSPT;
		}
		else if(URB_Status == TUSBH_URB_NOTREADY){
			/* Nack received from device */
			tusbhControlSendData(pdevice, pdevice->cbuff, length, pdevice->cntl_pipe_out);
			ercd = TUSBH_E_BUSY_URB;
		}
		else if(URB_Status == TUSBH_URB_ERROR)	/* USB���顼 */
			pdevice->cstate = CONTROL_ERROR;
		else{
			syslog_2(LOG_ERROR, "tusbhControlRequest urb error state(%d) urb[%02x] !", pdevice->cstate, URB_Status);
			ercd = TUSBH_E_URB;
		}
		break;
	case CONTROL_STATUS_IN_WAIT:	/* ���ơ����������Ԥ� */
		if(mes[0] != TUSBH_URB_EVENT)
			break;
		if(URB_Status == TUSBH_URB_DONE){	/* ���ｪλ */
			pdevice->cstate = CONTROL_IDLE;
			ercd = TUSBH_E_OK;
			tusbSendData(phost->process_event, TUSBH_PORT_EVENT, pdevice->idx, 0, 0);
		}
		else if(URB_Status == TUSBH_URB_ERROR)	/* USB���顼 */
			pdevice->cstate = CONTROL_ERROR;
		else if(URB_Status == TUSBH_URB_STALL){	/* ���ơ������ */
			pdevice->cstate = CONTROL_IDLE;
			ercd = TUSBH_E_NOSPT;
		}
		else if(URB_Status == TUSBH_URB_NOTREADY){
			syslog_0(LOG_NOTICE, "### tusbhControlRequest REAET STATE3 ###");
			pdevice->cstate = CONTROL_SETUP;
		}
		else{
			syslog_2(LOG_ERROR, "tusbhControlRequest urb error state(%d) urb[%02x] !", pdevice->cstate, URB_Status);
			ercd = TUSBH_E_URB;
		}
		break;
	case CONTROL_STATUS_OUT_WAIT:	/* ���ơ����������Ԥ� */
		if(mes[0] != TUSBH_URB_EVENT)
			break;
		if(URB_Status == TUSBH_URB_DONE){
			pdevice->cstate = CONTROL_IDLE;
			ercd = TUSBH_E_OK;
			tusbSendData(phost->process_event, TUSBH_PORT_EVENT, pdevice->idx, 0, 0);
		}
		else if(URB_Status == TUSBH_URB_NOTREADY){ 
			tusbhControlSendData(pdevice, 0, 0, pdevice->cntl_pipe_out);
			ercd = TUSBH_E_BUSY_URB;
		}
		else if(URB_Status == TUSBH_URB_ERROR)	/* USB���顼 */
			pdevice->cstate = CONTROL_ERROR;
		else{
			syslog_2(LOG_ERROR, "tusbhControlRequest urb error state(%d) urb[%02x] !", pdevice->cstate, URB_Status);
			ercd = TUSBH_E_URB;
		}
		break;
	case CONTROL_ERROR:
		/*
		 *  ����ȥ����̿����顼������
		 *  ���顼������Ȥ� TUSBH_MAX_ERROR_COUNT�ʲ��ʤ�Х�ȥ饤����
		 */
		if(++pdevice->cntl_errcount <= TUSBH_MAX_ERROR_COUNT){
			if(pdevice->hub == 0)
				tusbhHDStop(phost);
			else
				tusbhResetHub(pdevice);
			/*
			 *  ��ȥ饤����
			 */
			pdevice->cstate = CONTROL_SETUP;
		}
		else{
			if(phost->usrcallback != NULL)
				phost->usrcallback(phost, pdevice, HOST_USER_UNRECOVERED_ERROR);
			pdevice->cntl_errcount = 0;
			syslog_1(LOG_ERROR, "tusbhControlRequest error count(%d) !", pdevice->cntl_errcount);
			ercd = TUSBH_E_ERR;
		}
		break;
	default:
		break;
	}
	return ercd;
}

