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
 *  @(#) $Id: tusbh_rtos.h 698 2018-04-04 21:04:00Z roi $
 */
/*
 *  USB Middleware RTOS��¸�����
 */

#ifndef _TUSB_RTOS_H_
#define _TUSB_RTOS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <kernel.h>
#include <t_syslog.h>
#include <t_stdlib.h>
#include <sil.h>
#include <target_syssvc.h>
#include "syssvc/serial.h"
#include "syssvc/syslog.h"
#include "t_syslog.h"
#include "device.h"

#ifdef CACHE_LINE_SIZE
#define USB_DATA_ALIGN      CACHE_LINE_SIZE
#else
#define USB_DATA_ALIGN      4
#endif

#define TUSB_PROCESS_PRIO       6
#define TUSB_PROCESS_STACK_SIZE 4096
#define TUSB_HANDLE_STACK_SIZE  1024

/*
 *  SYSSVC��������ؿ�
 */
extern void *malloc_cache(uint32_t len);
extern void free_cache(void *addr);


extern ER tusbSendData(ID qid, uint8_t evt, uint8_t prm1, uint8_t prm2, uint8_t prm3);
extern ER tusbiSendData(ID qid, uint8_t evt, uint8_t prm1, uint8_t prm2, uint8_t prm3);
extern ER tusbRecvData(ID qid, uint8_t *pmes, uint32_t timeout);
extern ER tusbStartTask(ID taskid);
extern int32_t tusbGetTaskID(void);
extern ER tusbTimerControl(uint8_t activate);

#define tusbmalloc          malloc_cache
#define tusbfree            free_cache
extern void tusbmemset(void *d, const char val, int len);
extern void tusbmemcpy(void *d, void *s, int len);

#define tusbCpuLock         loc_cpu
#define tusbCpuUnLock       unl_cpu
#define tusbDelay(t)        dly_tsk((t))
#define tusbSleep(t)        tslp_tsk((t))
#define tusbWakeup(id)      wup_tsk((id))


#endif /* _TUSB_RTOS_H_ */

