/*
 *  TOPPERS/ASP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Advanced Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2004-2012 by Embedded and Real-Time Systems Laboratory
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
 *  Copyright (C) 2015-2017 by TOPPERS PROJECT Educational Working Group.
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
 *  $Id: i2ctest.c 2416 2012-09-07 08:06:20Z ertl-hiro $
 */

/* 
 *  I2C�ƥ��Ȥ�����
 *  �¹Ԥ�����ˤϡ���������˥��ǡ�task 3��ư
 */

#include <kernel.h>
#include <t_syslog.h>
#include <t_stdlib.h>
#include <sil.h>
#include "syssvc/serial.h"
#include "syssvc/syslog.h"
#include "kernel_cfg.h"
#include <target_syssvc.h>
#include "device.h"
#include "i2ctest.h"

/*
 *  �����ӥ�������Υ��顼�Υ�����
 */
Inline void
svc_perror(const char *file, int_t line, const char *expr, ER ercd)
{
	if (ercd < 0) {
		t_perror(LOG_ERROR, file, line, expr, ercd);
	}
}

#define	SVC_PERROR(expr)	svc_perror(__FILE__, __LINE__, #expr, (expr))

/* I2C TIMING Register define when I2C clock source is SYSCLK */
/* I2C TIMING is calculated in case of the I2C Clock source is the SYSCLK = 32 MHz */
//#define I2C_TIMING    0x10A13E56 /* 100 kHz with analog Filter ON, Rise Time 400ns, Fall Time 100ns */ 
#define I2C_TIMING      0x00B1112E /* 400 kHz with analog Filter ON, Rise Time 250ns, Fall Time 100ns */     

#define RXBUFFERSIZE                      8

#define I2C_ADDRESS        (0x3E<<1)
#define I2C_ADDRESS2       (0x50<<1)

CLCD_Handler_t  CLcdHandle;

/* Buffer used for reception */
uint8_t aRxBuffer[RXBUFFERSIZE];

uint8_t lcd_buf[] = {'H', 'e', 'l', 'l', 'o', ' ', '!'};


/*
 *  SW1�����
 */
void sw_int(void)
{
	syslog_0(LOG_NOTICE, "## sw_int() ##");
}

/*
 *  I2C SEND CALLBACK FUNCTION
 */
static void
I2C_TxCpltCallback(I2C_Handle_t *hi2c)
{
	syslog_1(LOG_INFO, "## I2C_TxCpltCallback(%08x) ##", hi2c);
}

/*
 *  I2C RECEIVE CALLBACK
 */
static void
I2C_RxCpltCallback(I2C_Handle_t *hi2c)
{
	syslog_1(LOG_INFO, "## I2C_RxCpltCallback(%08x) ##", hi2c);
}

/*
 *  I2C ERROR CALLBACK
 */
static void
I2C_ErrorCallback(I2C_Handle_t *hi2c)
{
	syslog_1(LOG_ERROR, "## I2C_ErrorCallback(%08x) ##", hi2c);
}


/*
 *  �ᥤ�󥿥���
 */
void main_task(intptr_t exinf)
{
	I2C_Init_t i2c_initd;
	I2C_Handle_t *hi2c;
	ER_UINT	ercd;

	SVC_PERROR(syslog_msk_log(LOG_UPTO(LOG_INFO), LOG_UPTO(LOG_EMERG)));
	syslog(LOG_NOTICE, "Sample program starts (exinf = %d).", (int_t) exinf);

	/*
	 *  ���ꥢ��ݡ��Ȥν����
	 *
	 *  �����ƥ����������Ʊ�����ꥢ��ݡ��Ȥ�Ȥ����ʤɡ����ꥢ��
	 *  �ݡ��Ȥ������ץ�Ѥߤξ��ˤϤ�����E_OBJ���顼�ˤʤ뤬���پ��
	 *  �ʤ���
	 */
	ercd = serial_opn_por(TASK_PORTID);
	if (ercd < 0 && MERCD(ercd) != E_OBJ) {
		syslog(LOG_ERROR, "%s (%d) reported by `serial_opn_por'.",
									itron_strerror(ercd), SERCD(ercd));
	}
	SVC_PERROR(serial_ctl_por(TASK_PORTID,
							(IOCTL_CRLF | IOCTL_FCSND | IOCTL_FCRCV)));

	syslog_0(LOG_NOTICE, "I2C SEND/RECV START");

	/*
	 *  I2C�����
	 */
	i2c_initd.Timing          = I2C_TIMING;
//	i2c_initd.DutyCycle       = I2C_DUTYCYCLE_16_9;
	i2c_initd.AddressingMode  = I2C_ADDRESSINGMODE_7BIT;
	i2c_initd.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	i2c_initd.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	i2c_initd.NoStretchMode   = I2C_NOSTRETCH_DISABLE;
	i2c_initd.OwnAddress1     = 0;
	i2c_initd.OwnAddress2     = 0;
	i2c_initd.OwnAddress2Masks = I2C_OA2_NOMASK;
	i2c_initd.semid           = I2CTRS_SEM;
	i2c_initd.semlock         = I2CLOC_SEM;

	if((hi2c = i2c_init(I2C_PORTID, &i2c_initd)) == NULL){
		/* Initialization Error */
		syslog_0(LOG_ERROR, "## I2C ERROR(1) ##");
	}
	syslog_0(LOG_NOTICE, "## PASS1 ##");

	hi2c->writecallback = I2C_TxCpltCallback;
	hi2c->readcallback  = I2C_RxCpltCallback;
	hi2c->errorcallback = I2C_ErrorCallback;

	ercd = i2c_ready(hi2c, (uint16_t)I2C_ADDRESS, 10, 500);
	syslog_2(LOG_NOTICE, "## i2c_ready[%02x](%d) ##", I2C_ADDRESS, ercd);

	CLcdHandle.hi2c  = hi2c;
	CLcdHandle.saddr = I2C_ADDRESS;
	if((ercd = aqm0802_init(&CLcdHandle)) == E_OK)
		aqm0802_set_data(&CLcdHandle, lcd_buf, sizeof(lcd_buf));
	else
		syslog_2(LOG_ERROR, "## LCD INIT ERROR(%d)[%08x] ##", ercd, hi2c->ErrorCode);

#if 0	/* ROI DEBUG */
	syslog_0(LOG_NOTICE, "## STOP ##");
	slp_tsk();
#else	/* ROI DEBUG */
	syslog_1(LOG_NOTICE, "## READ [%08x] ##", aRxBuffer);
#endif	/* ROI DEBUG */

	/*
	 *  EEPROM�Υ�������
	 */
	ercd = i2c_ready(hi2c, (uint16_t)I2C_ADDRESS2, 10, 500);
	syslog_2(LOG_NOTICE, "## i2c_ready[%02x](%d) ##", I2C_ADDRESS2, ercd);
	if(i2c_memread(hi2c, (uint16_t)I2C_ADDRESS2, 0, I2C_MEMADD_SIZE_16BIT, (uint8_t*)aRxBuffer, RXBUFFERSIZE)!= E_OK){
		syslog_1(LOG_ERROR, "## I2C ERROR(13)[%08x] ##", hi2c->ErrorCode);
	}

	syslog_2(LOG_NOTICE, "## aRxBuffer[%02x](%d) ##", aRxBuffer, RXBUFFERSIZE);

#if 0	/* WRITE */
	if(i2c_memwrite(hi2c, (uint16_t)I2C_ADDRESS2, 0, I2C_MEMADD_SIZE_16BIT, (uint8_t*)aRxBuffer, RXBUFFERSIZE)!= E_OK){
		syslog_1(LOG_ERROR, "## I2C ERROR(15)[%08x] ##", hi2c->ErrorCode);
	}

	syslog_2(LOG_NOTICE, "## aRxBuffer[%02x](%d) ##", aRxBuffer, RXBUFFERSIZE);
#endif	/* WRITE */

#if 1	/* ROI DEBUG */
	syslog_0(LOG_NOTICE, "## STOP ##");
	slp_tsk();
#endif	/* ROI DEBUG */

	syslog(LOG_NOTICE, "Sample program ends.");
//	SVC_PERROR(ext_ker());
}
