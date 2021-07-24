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
 *  @(#) $Id: tusbh_rtos.c 698 2017-09-20 18:35:20Z roi $
 */
/*
 *  USB Middleware RTOS��¸��
 */

#include "tusb_rtos.h"
#include "kernel_cfg.h"


/*
 *  ���������֤ǤΥ�å���������
 */
ER
tusbSendData(ID qid, uint8_t evt, uint8_t prm1, uint8_t prm2, uint8_t prm3)
{
	intptr_t data;
	ER       ercd;

	data = (evt << 24) | (prm1 << 16) | (prm2 << 8) | prm3;
	ercd = psnd_dtq(qid, data);
	if(ercd != E_OK){
		syslog_5(LOG_ERROR, "tusbSendData send(%d) error [%02x][%02x][%02x][%02x]", qid, evt, prm1, prm2, prm3);
	}
	return ercd;
}

/*
 *  �󥿥������֤ǤΥ�å���������
 */
ER
tusbiSendData(ID qid, uint8_t evt, uint8_t prm1, uint8_t prm2, uint8_t prm3)
{
	intptr_t data;
	ER       ercd;

	data = (evt << 24) | (prm1 << 16) | (prm2 << 8) | prm3;
	ercd = ipsnd_dtq(qid, data);
	if(ercd != E_OK){
		syslog_5(LOG_ERROR, "tusbiSendData send(%d) error [%02x][%02x][%02x][%02x]", qid, evt, prm1, prm2, prm3);
	}
	return ercd;
}

/*
 *  ���������֤ǤΥ�å���������
 */
ER
tusbRecvData(ID qid, uint8_t *pmes, uint32_t timeout)
{
	intptr_t data;
	ER       ercd;

	if(timeout == 0)
		ercd = rcv_dtq(qid, &data);
	else
		ercd = trcv_dtq(qid, &data, timeout);
	if(ercd != E_OK){
		syslog_1(LOG_ERROR, "tusbRecvData(%d) error !", qid);
	}
	else{
		pmes[0] = (data >> 24) & 0xff;
		pmes[1] = (data >> 16) & 0xff;
		pmes[2] = (data >> 8) & 0xff;
		pmes[3] = data & 0xff;
	}
	return ercd;
}

/*
 *  ������ID��Ф�
 */
int32_t
tusbGetTaskID(void)
{
	ER ercd;
	ID tid = 0;

	ercd = get_tid(&tid);
	if(ercd != E_OK){
		syslog_1(LOG_ERROR, "tusbhGetTaskID get fail(%d) !", ercd);
		return 0;
	}
	else
		return (int32_t)tid;
}

/*
 *  ��������ư
 */
ER
tusbStartTask(ID taskid)
{
	ER ercd = act_tsk(taskid);

	if(ercd != E_OK)
		syslog_2(LOG_ERROR, "tusbStartTask actvate error id(%d) error(%d) !", taskid, ercd);
	return ercd;
}

/*
 *  �����ޡ�����
 */
ER
tusbTimerControl(uint8_t activate)
{
	ER ercd;

	if(activate == 0)
		ercd = stp_cyc(TUSBH_CYC_HDR);
	else
		ercd = sta_cyc(TUSBH_CYC_HDR);
	return ercd;
}

/*
 *  �������memset�ؿ�
 */
void
tusbmemset(void *d, const char val, int len)
{
	char *dd = d;
	int i;
	for(i = 0 ; i < len ; i++){
		*dd++ = val;
	}
}

/*
 *  �������memcpy�ؿ�
 */
void
tusbmemcpy(void *d, void *s, int len)
{
	uint8_t *dd = d;
	uint8_t *ss = s;
	int i;
	for(i = 0 ; i < len ; i++){
		*dd++ = *ss++;
	}
}

