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
 *  $Id: aqm0802_st7032.c 2416 2017-11-23 11:40:56Z roi $
 */

/* 
 *  AQM0802 ST7032 LCD����ץ���������
 */

#include <kernel.h>
#include <t_syslog.h>
#include <t_stdlib.h>
#include <stdio.h>
#include <string.h>
#include <target_syssvc.h>
#include "syssvc/serial.h"
#include "syssvc/syslog.h"
#include "kernel_cfg.h"
#include "device.h"
#include "aqm0802_st7032.h"

#define TXBUFFERSIZE  2

#if defined(TOPPERS_STM32F746_NUCLEO144) || defined(TOPPERS_STM32F767_NUCLEO144) || defined(TOPPERS_STM32F769_DISCOVERY)
#define i2c_send(h, a, b, l)    i2c_memwrite((h), (a), 0, 0, (b), (l), 500)
#else
#define i2c_send(h, a, b, l)    i2c_memwrite((h), (a), 0, 0, (b), (l))
#endif

static uint8_t aTxBuffer[TXBUFFERSIZE];

/*
 *  LCD�ν����
 */
ER
aqm0802_init(CLCD_Handler_t *hlcd)
{
	I2C_Handle_t *hi2c = hlcd->hi2c;
	ER ercd;

	hlcd->max_col = 8;
	hlcd->max_raw = 2;

	/*
	 *  LED COMMAND 0x38
	 */
    aTxBuffer[0] = ST7032_CMD;
    aTxBuffer[1] = 0x38;
	if((ercd = i2c_send(hi2c, hlcd->saddr, aTxBuffer, TXBUFFERSIZE)) != E_OK)
		return ercd;

    // Wait 70us
    sil_dly_nse(LCD_INTERVAL1*1000);
    /*
	 *  LCD COMMAND 0x39
	 */
    aTxBuffer[0] = ST7032_CMD;
    aTxBuffer[1] = 0x39;
	if((ercd = i2c_send(hi2c, hlcd->saddr, aTxBuffer, TXBUFFERSIZE)) != E_OK)
		return ercd;

    // Wait 26.3us
    sil_dly_nse(LCD_INTERVAL2*1000);
	/*
	 *  LCD COMMAND 0x14
	 */
    aTxBuffer[0] = ST7032_CMD;
    aTxBuffer[1] = 0x14;
	if((ercd = i2c_send(hi2c, hlcd->saddr, aTxBuffer, TXBUFFERSIZE)) != E_OK)
		return ercd;

    // Wait 26.3us
    sil_dly_nse(LCD_INTERVAL2*1000);
	/*
	 *  LCD COMMAND 0x70
	 */
    aTxBuffer[0] = ST7032_CMD;
    aTxBuffer[1] = 0x70;
	if((ercd = i2c_send(hi2c, hlcd->saddr, aTxBuffer, TXBUFFERSIZE)) != E_OK)
		return ercd;

    // Wait 26.3us
    sil_dly_nse(LCD_INTERVAL2*1000);
	/*
	 *  LCD COMMAND 0x56
	 */
    aTxBuffer[0] = ST7032_CMD;
    aTxBuffer[1] = 0x56;
	if((ercd = i2c_send(hi2c, hlcd->saddr, aTxBuffer, TXBUFFERSIZE)) != E_OK)
		return ercd;

    // Wait 26.3us
    sil_dly_nse(LCD_INTERVAL2*1000);
	/*
	 *  LCD COMMAND 0x6C
	 */
    aTxBuffer[0] = ST7032_CMD;
    aTxBuffer[1] = 0x6C;
	if((ercd = i2c_send(hi2c, hlcd->saddr, aTxBuffer, TXBUFFERSIZE)) != E_OK)
		return ercd;

    // Wait 200ms
	dly_tsk(200);
	/*
	 *  LCD COMMAND 0x38
	 */
    aTxBuffer[0] = ST7032_CMD;
    aTxBuffer[1] = 0x38;
	if((ercd = i2c_send(hi2c, hlcd->saddr, aTxBuffer, TXBUFFERSIZE)) != E_OK)
		return ercd;

    // Wait 26.3us
    sil_dly_nse(LCD_INTERVAL2*1000);
	/*
	 * LCD COMMAND 0x0C Display ON/OFF control
	 */
    aTxBuffer[0] = ST7032_CMD;
    aTxBuffer[1] = 0x0C;
	if((ercd = i2c_send(hi2c, hlcd->saddr, aTxBuffer, TXBUFFERSIZE)) != E_OK)
		return ercd;

    // Wait 26.3us
    sil_dly_nse(LCD_INTERVAL2*1000);
	/*
	 *  LCD COMMAND 0x01 Clear Display
	 */
    aTxBuffer[0] = ST7032_CMD;
    aTxBuffer[1] = 0x01;
	if((ercd = i2c_send(hi2c, hlcd->saddr, aTxBuffer, TXBUFFERSIZE)) != E_OK)
		return ercd;

    // Wait 1.08ms
	dly_tsk(2);
	/*
	 *  LCD COMMAND 0x80
	 */
    aTxBuffer[0] = ST7032_CMD;
    aTxBuffer[1] = 0x80;
	i2c_send(hi2c, hlcd->saddr, aTxBuffer, TXBUFFERSIZE);
	return ercd;
}

/*
 *  LCD�ؤΥ��ޥ�������ؿ�
 */
ER
aqm0802_set_command(CLCD_Handler_t *hlcd, uint8_t c)
{
    aTxBuffer[0] = ST7032_CMD;
    aTxBuffer[1] = c;
	return i2c_send(hlcd->hi2c, hlcd->saddr, aTxBuffer, TXBUFFERSIZE);
}

/*
 *  LCD�ؤΥǡ��������ؿ�
 */
ER
aqm0802_set_data(CLCD_Handler_t *hlcd, uint8_t *p, uint8_t len)
{
	ER  ercd = E_OK;
	int i;

	for(i = 0 ; i < len ; i++){
	    aTxBuffer[0] = ST7032_DAT;
    	aTxBuffer[1] = *p++;
		ercd = i2c_send(hlcd->hi2c, hlcd->saddr, aTxBuffer, TXBUFFERSIZE);
		if(ercd != E_OK)
			break;
    }
	return ercd;
}

