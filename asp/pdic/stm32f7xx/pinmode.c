/*
 *  TOPPERS/ASP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Advanced Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2004-2012 by Embedded and Real-Time Systems Laboratory
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
 *  Copyright (C) 2015-2016 by TOPPERS PROJECT Educational Working Group.
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
 *  $Id: pinmode.c 2416 2016-06-21 11:05:38Z roi $
 */

/*
 *  ARDUNO-PIN�ɥ饤��
 */

#include <kernel.h>
#include <t_syslog.h>
#include <t_stdlib.h>
#include <string.h>
#include <target_syssvc.h>
#include "syssvc/serial.h"
#include "syssvc/syslog.h"
#include "kernel_cfg.h"
#include "device.h"
#include "pinmode.h"

#define sil_orw_mem(a, b)		sil_wrw_mem((a), sil_rew_mem(a) | (b))
#define sil_andw_mem(a, b)		sil_wrw_mem((a), sil_rew_mem(a) & ~(b))
#define sil_modw_mem(a, b, c)	sil_wrw_mem((a), (sil_rew_mem(a) & (~b)) | (c))


static const Arduino_PortControlBlock Digital_Port[] = {
#if defined(TOPPERS_STM32F7_DISCOVERY)
  { (TADR_RCC_BASE+TOFF_RCC_AHB1ENR),  RCC_AHB1ENR_GPIOCEN, TADR_GPIOC_BASE, 7},	/* D0 */
  { (TADR_RCC_BASE+TOFF_RCC_AHB1ENR),  RCC_AHB1ENR_GPIOCEN, TADR_GPIOC_BASE, 6},	/* D1 */
  { (TADR_RCC_BASE+TOFF_RCC_AHB1ENR),  RCC_AHB1ENR_GPIOGEN, TADR_GPIOG_BASE, 6},	/* D2 */
  { (TADR_RCC_BASE+TOFF_RCC_AHB1ENR),  RCC_AHB1ENR_GPIOBEN, TADR_GPIOB_BASE, 4},	/* D3 */
  { (TADR_RCC_BASE+TOFF_RCC_AHB1ENR),  RCC_AHB1ENR_GPIOGEN, TADR_GPIOG_BASE, 7},	/* D4 */
  { (TADR_RCC_BASE+TOFF_RCC_AHB1ENR),  RCC_AHB1ENR_GPIOAEN, TADR_GPIOA_BASE, 8},	/* D5 */
  { (TADR_RCC_BASE+TOFF_RCC_AHB1ENR),  RCC_AHB1ENR_GPIOHEN, TADR_GPIOH_BASE, 6},	/* D6 */
  { (TADR_RCC_BASE+TOFF_RCC_AHB1ENR),  RCC_AHB1ENR_GPIOIEN, TADR_GPIOI_BASE, 3},	/* D7 */
  { (TADR_RCC_BASE+TOFF_RCC_AHB1ENR),  RCC_AHB1ENR_GPIOIEN, TADR_GPIOI_BASE, 2},	/* D8 */
  { (TADR_RCC_BASE+TOFF_RCC_AHB1ENR),  RCC_AHB1ENR_GPIOAEN, TADR_GPIOA_BASE, 15},	/* D9 */
  { (TADR_RCC_BASE+TOFF_RCC_AHB1ENR),  RCC_AHB1ENR_GPIOIEN, TADR_GPIOI_BASE, 0},	/* D10 */
  { (TADR_RCC_BASE+TOFF_RCC_AHB1ENR),  RCC_AHB1ENR_GPIOBEN, TADR_GPIOB_BASE, 15},	/* D11 */
  { (TADR_RCC_BASE+TOFF_RCC_AHB1ENR),  RCC_AHB1ENR_GPIOBEN, TADR_GPIOB_BASE, 14},	/* D12 */
  { (TADR_RCC_BASE+TOFF_RCC_AHB1ENR),  RCC_AHB1ENR_GPIOIEN, TADR_GPIOI_BASE, 1}		/* D13 */
#else	/* TOPPERS_STM32F746_NUCLEO144 TOPPERS_STM32F767_NUCLEO144 */
  { (TADR_RCC_BASE+TOFF_RCC_AHB1ENR),  RCC_AHB1ENR_GPIOGEN, TADR_GPIOG_BASE, 9},	/* D0 */
  { (TADR_RCC_BASE+TOFF_RCC_AHB1ENR),  RCC_AHB1ENR_GPIOGEN, TADR_GPIOG_BASE, 14},	/* D1 */
  { (TADR_RCC_BASE+TOFF_RCC_AHB1ENR),  RCC_AHB1ENR_GPIOFEN, TADR_GPIOF_BASE, 15},	/* D2 */
  { (TADR_RCC_BASE+TOFF_RCC_AHB1ENR),  RCC_AHB1ENR_GPIOEEN, TADR_GPIOE_BASE, 13},	/* D3 */
  { (TADR_RCC_BASE+TOFF_RCC_AHB1ENR),  RCC_AHB1ENR_GPIOFEN, TADR_GPIOF_BASE, 14},	/* D4 */
  { (TADR_RCC_BASE+TOFF_RCC_AHB1ENR),  RCC_AHB1ENR_GPIOEEN, TADR_GPIOE_BASE, 11},	/* D5 */
  { (TADR_RCC_BASE+TOFF_RCC_AHB1ENR),  RCC_AHB1ENR_GPIOEEN, TADR_GPIOE_BASE, 9},	/* D6 */
  { (TADR_RCC_BASE+TOFF_RCC_AHB1ENR),  RCC_AHB1ENR_GPIOFEN, TADR_GPIOF_BASE, 13},	/* D7 */
  { (TADR_RCC_BASE+TOFF_RCC_AHB1ENR),  RCC_AHB1ENR_GPIOFEN, TADR_GPIOF_BASE, 12},	/* D8 */
  { (TADR_RCC_BASE+TOFF_RCC_AHB1ENR),  RCC_AHB1ENR_GPIODEN, TADR_GPIOD_BASE, 15},	/* D9 */
  { (TADR_RCC_BASE+TOFF_RCC_AHB1ENR),  RCC_AHB1ENR_GPIODEN, TADR_GPIOD_BASE, 14},	/* D10 */
  { (TADR_RCC_BASE+TOFF_RCC_AHB1ENR),  RCC_AHB1ENR_GPIOAEN, TADR_GPIOA_BASE, 7},	/* D11 */
  { (TADR_RCC_BASE+TOFF_RCC_AHB1ENR),  RCC_AHB1ENR_GPIOAEN, TADR_GPIOA_BASE, 6},	/* D12 */
  { (TADR_RCC_BASE+TOFF_RCC_AHB1ENR),  RCC_AHB1ENR_GPIOAEN, TADR_GPIOA_BASE, 5}		/* D13 */
#endif
};

#define NUM_DIGITAL_PORT (sizeof(Digital_Port)/sizeof(Arduino_PortControlBlock))

static const Arduino_PortControlBlock Analog_Port[] = {
#if defined(TOPPERS_STM32F7_DISCOVERY)
  { (TADR_RCC_BASE+TOFF_RCC_AHB1ENR),  RCC_AHB1ENR_GPIOAEN, TADR_GPIOA_BASE, 0},	/* A0 */
  { (TADR_RCC_BASE+TOFF_RCC_AHB1ENR),  RCC_AHB1ENR_GPIOFEN, TADR_GPIOF_BASE, 10},	/* A1 */
  { (TADR_RCC_BASE+TOFF_RCC_AHB1ENR),  RCC_AHB1ENR_GPIOFEN, TADR_GPIOF_BASE, 9},	/* A2 */
  { (TADR_RCC_BASE+TOFF_RCC_AHB1ENR),  RCC_AHB1ENR_GPIOFEN, TADR_GPIOF_BASE, 8},	/* A3 */
  { (TADR_RCC_BASE+TOFF_RCC_AHB1ENR),  RCC_AHB1ENR_GPIOFEN, TADR_GPIOF_BASE, 7},	/* A4 */
  { (TADR_RCC_BASE+TOFF_RCC_AHB1ENR),  RCC_AHB1ENR_GPIOFEN, TADR_GPIOF_BASE, 6}		/* A5 */
#else	/* TOPPERS_STM32F746_NUCLEO144 TOPPERS_STM32F767_NUCLEO144 */
  { (TADR_RCC_BASE+TOFF_RCC_AHB1ENR),  RCC_AHB1ENR_GPIOAEN, TADR_GPIOA_BASE, 3},	/* A0 */
  { (TADR_RCC_BASE+TOFF_RCC_AHB1ENR),  RCC_AHB1ENR_GPIOCEN, TADR_GPIOC_BASE, 0},	/* A1 */
  { (TADR_RCC_BASE+TOFF_RCC_AHB1ENR),  RCC_AHB1ENR_GPIOCEN, TADR_GPIOC_BASE, 3},	/* A2 */
  { (TADR_RCC_BASE+TOFF_RCC_AHB1ENR),  RCC_AHB1ENR_GPIOFEN, TADR_GPIOF_BASE, 3},	/* A3 */
  { (TADR_RCC_BASE+TOFF_RCC_AHB1ENR),  RCC_AHB1ENR_GPIOFEN, TADR_GPIOF_BASE, 5},	/* A4 */
  { (TADR_RCC_BASE+TOFF_RCC_AHB1ENR),  RCC_AHB1ENR_GPIOFEN, TADR_GPIOF_BASE, 10}	/* A5 */
#endif
};

#define NUM_ANALOG_PORT (sizeof(Analog_Port)/sizeof(Arduino_PortControlBlock))

void
pinClock(uint8_t no)
{
	const Arduino_PortControlBlock *ppcb = &Digital_Port[no];

	if(no >= NUM_DIGITAL_PORT)
		return;
	sil_orw_mem((uint32_t *)ppcb->gioclockbase, ppcb->gioclockbit);
}

void
digitalWrite(uint8_t no, int sw)
{
	const Arduino_PortControlBlock *ppcb = &Digital_Port[no];

	if(no >= NUM_DIGITAL_PORT)
		return;
	if(sw == 0)
		sil_wrw_mem((uint32_t *)(ppcb->giobase+TOFF_GPIO_BSRR), (1<<(ppcb->giopin+16)));
	else
		sil_wrw_mem((uint32_t *)(ppcb->giobase+TOFF_GPIO_BSRR), (1<<(ppcb->giopin)));
}

int
digitalRead(uint8_t no)
{
	const Arduino_PortControlBlock *ppcb = &Digital_Port[no];
 	int sw;

	if(no >= NUM_DIGITAL_PORT)
		return 0;
	sw = sil_rew_mem((uint32_t *)(ppcb->giobase+TOFF_GPIO_IDR));
	sw = (sw>>(ppcb->giopin)) & 1;
	return sw;
}

Arduino_PortControlBlock*
getGpioTable(uint8_t mode, uint8_t no)
{
	if(mode == ANALOG_PIN){
		if(no >= NUM_ANALOG_PORT)
			return NULL;
		return (Arduino_PortControlBlock*)&Analog_Port[no];
	}
	else{
		if(no >= NUM_DIGITAL_PORT)
			return NULL;
		return (Arduino_PortControlBlock*)&Digital_Port[no];
	}
}


