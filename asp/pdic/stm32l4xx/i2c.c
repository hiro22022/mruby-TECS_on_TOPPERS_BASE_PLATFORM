/*
 *  TOPPERS/ASP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Advanced Standard Profile Kernel
 * 
 *  Copyright (C) 2008-2011 by Embedded and Real-Time Systems Laboratory
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
 *  @(#) $Id: i2c.c 698 2017-08-03 16:04:31Z roi $
 */
/*
 *
 *  I2C�ɥ饤�дؿ���(STM32L4XX)
 *
 */
#include "kernel_impl.h"
#include <t_syslog.h>
#include <t_stdlib.h>
#include <string.h>
#include <sil.h>
#include <target_syssvc.h>
#include "device.h"
#include "i2c.h"

#define sil_orw_mem(a, b)		sil_wrw_mem((a), sil_rew_mem(a) | (b))
#define sil_andw_mem(a, b)		sil_wrw_mem((a), sil_rew_mem(a) & ~(b))
#define sil_modw_mem(a, b, c)	sil_wrw_mem((a), (sil_rew_mem(a) & (~b)) | (c))

/*
 *  I2C�ݡ���ID��������֥�å�����Ф�����Υޥ���
 */
#define INDEX_I2C(i2cid)        ((uint_t)((i2cid) - 1))

/*
 *  I2C��AF����
 */
#define GPIO_AF4_I2C1          ((uint8_t)0x04)  /* I2C1 Alternate Function mapping */
#define GPIO_AF4_I2C2          ((uint8_t)0x04)  /* I2C2 Alternate Function mapping */
#define GPIO_AF4_I2C3          ((uint8_t)0x04)  /* I2C3 Alternate Function mapping */

#define I2C_TIMEOUT_FLAG            35			/* 35 ms */
#define I2C_TIMEOUT_BUSY_FLAG       10000		/* 10 s  */

#define TIMING_CLEAR_MASK           0xF0FFFFFF	/* I2C TIMING clear register Mask */

/*
 *  ����ž��������
 */
#define MAX_NBYTE_SIZE              255

/*
 *  I2C_RELOAD�⡼�����
 */
#define  I2C_RELOAD_MODE            I2C_CR2_RELOAD
#define  I2C_AUTOEND_MODE           I2C_CR2_AUTOEND
#define  I2C_SOFTEND_MODE           0x00000000

/*
 *  I2C_XFEROPTION���
 */
#define I2C_FIRST_FRAME             I2C_SOFTEND_MODE
#define I2C_FIRST_AND_NEXT_FRAME    (I2C_RELOAD_MODE | I2C_SOFTEND_MODE)
#define I2C_NEXT_FRAME              (I2C_RELOAD_MODE | I2C_SOFTEND_MODE)
#define I2C_FIRST_AND_LAST_FRAME    I2C_AUTOEND_MODE
#define I2C_LAST_FRAME              I2C_AUTOEND_MODE
#define I2C_NO_OPTION_FRAME         0xFFFF0000

/*
 *  I2C_START_STOP_MODE���
 */
#define  I2C_NO_STARTSTOP           (0x00000000U)
#define  I2C_GENERATE_STOP          I2C_CR2_STOP
#define  I2C_GENERATE_START_READ    (I2C_CR2_START | I2C_CR2_RD_WRN)
#define  I2C_GENERATE_START_WRITE   I2C_CR2_START

#define SlaveAddr_SHIFT             7
#define SlaveAddr_MSK               0x06

#define I2C_INT_TX                  (I2C_CR1_TCIE   | I2C_CR1_TXIE | I2C_CR1_STOPIE | I2C_CR1_NACKIE | I2C_CR1_ERRIE)
#define I2C_INT_RX                  (I2C_CR1_TCIE   | I2C_CR1_RXIE | I2C_CR1_STOPIE | I2C_CR1_NACKIE | I2C_CR1_ERRIE)
#define I2C_INT_LISTEN              (I2C_CR1_ADDRIE | I2C_CR1_STOPIE | I2C_CR1_NACKIE | I2C_CR1_ERRIE)
#define I2C_INT_ALL                 (I2C_INT_TX | I2C_INT_RX | I2C_INT_LISTEN)

/*
 *  I2C�ݡ�������ơ��֥�
 */
static const I2C_PortControlBlock i2c_pcb[NUM_I2CPORT] = {
  {	TADR_I2C1_BASE,
	(TADR_RCC_BASE+TOFF_RCC_AHB2ENR),   RCC_AHB2ENR_GPIOBEN, RCC_AHB2ENR_GPIOBEN,
	(TADR_RCC_BASE+TOFF_RCC_APB1ENR1),  RCC_APB1ENR1_I2C1EN,
	(TADR_RCC_BASE+TOFF_RCC_APB1RSTR1), RCC_APB1RSTR1_I2C1RST,
	TADR_GPIOB_BASE, TADR_GPIOB_BASE,
	8, GPIO_AF4_I2C1, 9, GPIO_AF4_I2C1 },

  {	TADR_I2C2_BASE,
	(TADR_RCC_BASE+TOFF_RCC_AHB2ENR),   RCC_AHB2ENR_GPIOBEN, RCC_AHB2ENR_GPIOBEN,
	(TADR_RCC_BASE+TOFF_RCC_APB1ENR1),  RCC_APB1ENR1_I2C2EN,
	(TADR_RCC_BASE+TOFF_RCC_APB1RSTR1), RCC_APB1RSTR1_I2C2RST,
	TADR_GPIOB_BASE, TADR_GPIOB_BASE,
	10, GPIO_AF4_I2C2, 11, GPIO_AF4_I2C2 },

  {	TADR_I2C3_BASE,
	(TADR_RCC_BASE+TOFF_RCC_AHB2ENR),   RCC_AHB2ENR_GPIOAEN, RCC_AHB2ENR_GPIOBEN,
	(TADR_RCC_BASE+TOFF_RCC_APB1ENR1),  RCC_APB1ENR1_I2C3EN,
	(TADR_RCC_BASE+TOFF_RCC_APB1RSTR1), RCC_APB1RSTR1_I2C3RST,
	TADR_GPIOC_BASE, TADR_GPIOC_BASE,
	0, GPIO_AF4_I2C3, 1, GPIO_AF4_I2C3 }
};


/*
 * I2C �ϥ�ɥ�
 */
static I2C_Handle_t I2cHandle[NUM_I2CPORT];

/*
 *  I2Cž������
 *  parameter1 hi2c:       I2C�ϥ�ɥ�ؤΥݥ���
 *  parameter2 DevAddress: �ǥХ������ɥ쥹
 *  parameter3 Size:       ž��������
 *  parameter4 Mode:       ž���⡼��
 *  parameter5 Request:    �������Ⱦ��֥ꥯ������
 *  return                 �ʤ�
 */
static void
i2c_transfconfig(I2C_Handle_t *hi2c, uint16_t DevAddress, uint8_t Size, uint32_t Mode, uint32_t Request)
{
	uint32_t tmp = sil_rew_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR2));

	/*
	 *  ����⡼�ɥ��ꥢ
	 */
	tmp &= ~(I2C_CR2_SADD | I2C_CR2_NBYTES | I2C_CR2_RELOAD | I2C_CR2_AUTOEND | I2C_CR2_RD_WRN | I2C_CR2_START | I2C_CR2_STOP);

	/*
	 *  ž�������Ԥ�
	 */
	tmp |= (uint32_t)((DevAddress & I2C_CR2_SADD) | ((Size << 16) & I2C_CR2_NBYTES) | Mode | Request);
	sil_wrw_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR2), tmp);
}

/*
 *  I2C�����ǡ����쥸�����ե�å���
 *  parameter1 hi2c:    I2C�ϥ�ɥ�ؤΥݥ���
 *  return              �ʤ�
 */
static void
i2c_flushtxdr(I2C_Handle_t *hi2c)
{
	/*
	 *  TXIS�ե饰����ʤ顢TXDR�˥���򥻥å�
	 */
	if((sil_rew_mem((uint32_t *)(hi2c->base+TOFF_I2C_ISR)) & I2C_ISR_TXIS) != 0)
		sil_wrw_mem((uint32_t *)(hi2c->base+TOFF_I2C_TXDR), 0);

	/*
	 *  TXE�ե饰�����դʤ顢EMPTY���å�
	 */
	if((sil_rew_mem((uint32_t *)(hi2c->base+TOFF_I2C_ISR)) & I2C_ISR_TXE) == 0)
		sil_orw_mem((uint32_t *)(hi2c->base+TOFF_I2C_ISR), I2C_ISR_TXE);
}

/*
 *  ISR�쥸�������å��Ԥ�
 *  parameter1 hi2c:    I2C�ϥ�ɥ�ؤΥݥ���
 *  parameter2 Flag:    �Ԥ��ե饰
 *  parameter3 Timeout: �Ԥ�����(ms)
 *  parameter4 pTimeout:�����Ԥ���������ݥ���
 *  return              ���ｪλ����E_OK
 */
static ER
i2c_isrflagsetwait(I2C_Handle_t *hi2c, uint32_t Flag, uint32_t Timeout, uint32_t *pTimeout)
{
	uint32_t tick = 0;
	uint32_t ctime = Timeout * 1000;

	*pTimeout = Timeout;
	while((sil_rew_mem((uint32_t *)(hi2c->base+TOFF_I2C_ISR)) & Flag) == 0){
		if(ctime == 0 || tick > ctime){
			hi2c->status= I2C_STATUS_READY;
			return E_TMOUT;
		}
        sil_dly_nse(1000);
		tick++;
	}
	*pTimeout -= tick / 1000;
	return E_OK;
}

/*
 *  I2C ACK������ǧ
 *  parameter1 hi2c:    I2C�ϥ�ɥ�ؤΥݥ���
 *  parameter2 Timeout: �Ԥ�����(ms)
 *  parameter3 pTimeout:�����Ԥ���������ݥ���
 *  return              ���ｪλ����E_OK
 */
static ER
i2c_checkacknowledgefailed(I2C_Handle_t *hi2c, uint32_t Timeout, uint32_t *pTimeout)
{
	uint32_t tick = 0;

	if((sil_rew_mem((uint32_t *)(hi2c->base+TOFF_I2C_ISR)) & I2C_ISR_NACKF) != 0){
		/*
		 *  NACK�����ʤ�STOP�ե饰�Ԥ�
		 */
		while((sil_rew_mem((uint32_t *)(hi2c->base+TOFF_I2C_ISR)) & I2C_ISR_STOPF) == 0){
			if(tick > (Timeout * 1000)){
				hi2c->status= I2C_STATUS_READY;
				*pTimeout -= tick * 1000;
				return E_TMOUT;
			}
			tick++;
			sil_dly_nse(1000);
		}
		*pTimeout -= tick * 1000;

		/*
		 *  NACK�ե饰���ꥢ
		 */
		sil_wrw_mem((uint32_t *)(hi2c->base+TOFF_I2C_ICR), I2C_ICR_NACKCF);

		/*
		 *  STOP�ե饰���ꥢ
		 */
		sil_wrw_mem((uint32_t *)(hi2c->base+TOFF_I2C_ICR), I2C_ICR_STOPCF);

		/*
		 *  �����쥸�������ꥢ
		 */
		i2c_flushtxdr(hi2c);

		/*
		 *  ž������쥸�������ꥢ
		 */
		sil_andw_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR2), ((I2C_CR2_SADD | I2C_CR2_HEAD10R | I2C_CR2_NBYTES | I2C_CR2_RELOAD | I2C_CR2_RD_WRN)));

		hi2c->ErrorCode = I2C_ERROR_AF;
		hi2c->status= I2C_STATUS_READY;
		return E_SYS;
	}
	return E_OK;
}

/*
 *  I2C TXIS�ե饰���å��Ԥ�
 *  parameter2 Timeout: �Ԥ�����(ms)
 *  parameter3 pTimeout:�����Ԥ���������ݥ���
 *  return              ���ｪλ����E_OK
 */
static ER
i2c_waitsettxisflag(I2C_Handle_t *hi2c, uint32_t Timeout, uint32_t *pTimeout)
{
	ER ercd = E_OK;
	uint32_t tick = 0;

	while((sil_rew_mem((uint32_t *)(hi2c->base+TOFF_I2C_ISR)) & I2C_ISR_TXIS) == 0){
	    /*
		 *  NACK������ǧ
		 */
	    if((ercd = i2c_checkacknowledgefailed(hi2c, Timeout, pTimeout)) != E_OK)
			return ercd;
		if(tick > (Timeout * 1000)){
			hi2c->ErrorCode |= I2C_ERROR_TIMEOUT;
			hi2c->status = I2C_STATUS_READY;
			return E_TMOUT;
		}
		tick++;
		sil_dly_nse(1000);
	}
	*pTimeout -= tick / 1000;
	return E_OK;
}

/*
 *  BUS READY�Ԥ�
 *  parameter1 hi2c:    I2C�ϥ�ɥ�ؤΥݥ���
 *  parameter2 Timeout: �Ԥ�����(ms)
 *  return              ���ｪλ����E_OK
 */
static ER
i2c_busreadywait(I2C_Handle_t *hi2c, uint32_t Timeout)
{
	uint32_t tick = 0;
	uint32_t ctime = Timeout * 1000;

    while((sil_rew_mem((uint32_t *)(hi2c->base+TOFF_I2C_ISR)) & I2C_ISR_BUSY) != 0){
		if(ctime == 0 || tick > ctime){
			hi2c->status= I2C_STATUS_READY;
			return E_TMOUT;
		}
        sil_dly_nse(1000);
		tick++;
	}
	return E_OK;
}

/*
 *  I2Cž���Ԥ�
 *  parameter1 hi2c:   I2C�ϥ�ɥ�ؤΥݥ���
 *  parameter2 Timeout:�����ॢ���Ȼ���(ms)
 */
static ER
i2c_transwait(I2C_Handle_t *hi2c, uint32_t Timeout)
{
	uint32_t tick = 0;

	while(hi2c->status != I2C_STATUS_READY){
		if(tick >= Timeout)
			return E_TMOUT;
		if(hi2c->Init.semid != 0)
			twai_sem(hi2c->Init.semid, 1);
		else
			dly_tsk(1);
		tick++;
	}
	if(hi2c->ErrorCode == I2C_ERROR_NONE)
		return E_OK;
	else
		return E_SYS;
}

/*
 *  �ޥ��������� �ǥХ������ɥ쥹�ȥ��ꥢ�ɥ쥹����
 *  parameter1 hi2c:  I2C�ϥ�ɥ�ؤΥݥ���
 *  parameter2 DevAddress: �ǥХ������ɥ쥹
 *  parameter3 MemAddress: ���ꥢ�ɥ쥹
 *  parameter4 MemAddSize: ���ꥢ�ɥ쥹������
 *  parameter5 Timeout:    �����ॢ���Ȼ���(ms)
 *  return                 ���ｪλ����E_OK
 */
static ER
i2c_masterreadaddress(I2C_Handle_t *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint32_t Timeout)
{
	ER ercd = E_OK;

	if(MemAddSize == 0)
		return E_OK;

	i2c_transfconfig(hi2c, DevAddress, MemAddSize, I2C_SOFTEND_MODE, I2C_GENERATE_START_WRITE);

	/*
	 *  TXIS�ե饰�����Ԥ�
	 */
	if((ercd = i2c_waitsettxisflag(hi2c, Timeout, &Timeout)) != E_OK){
		if(hi2c->ErrorCode == I2C_ERROR_AF)
			return E_SYS;
		else
			return ercd;
	}

	/*
	 *  ���ꥢ�ɥ쥹����
	 */
	if(MemAddSize == I2C_MEMADD_SIZE_8BIT){
		/*
		 *  8�ӥåȥ��ꥢ�ɥ쥹����
		 */
		sil_wrw_mem((uint32_t *)(hi2c->base+TOFF_I2C_TXDR), (MemAddress & 0xff));
	}
	else{
		/*
		 *  16�ӥåȥ��ꥢ�ɥ쥹����
		 */
		sil_wrw_mem((uint32_t *)(hi2c->base+TOFF_I2C_TXDR), ((MemAddress >> 8) & 0xff));

		/*
		 *  TXIS�ե饰�����Ԥ�
		 */
		if((ercd = i2c_waitsettxisflag(hi2c, Timeout, &Timeout)) != E_OK){
			if(hi2c->ErrorCode == I2C_ERROR_AF)
				return E_SYS;
			else
				return ercd;
		}
		sil_wrw_mem((uint32_t *)(hi2c->base+TOFF_I2C_TXDR), (MemAddress & 0xff));
	}

	/*
	 *  TC�ե饰�����Ԥ�
	 */
	return i2c_isrflagsetwait(hi2c, I2C_ISR_TC, Timeout, &Timeout);
}

/*
 *  �ޥ��������� �ǥХ������ɥ쥹�ȥ��ꥢ�ɥ쥹����
 *  parameter1 hi2c:  I2C�ϥ�ɥ�ؤΥݥ���
 *  parameter2 DevAddress: �ǥХ������ɥ쥹
 *  parameter3 MemAddress: ���ꥢ�ɥ쥹
 *  parameter4 MemAddSize: ���ꥢ�ɥ쥹������
 *  parameter5 Timeout:    �����ॢ���Ȼ���(ms)
 *  return                 ���ｪλ����E_OK
 */
static ER
i2c_masterwriteaddress(I2C_Handle_t *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint32_t Timeout)
{
	ER ercd = E_OK;

	if(MemAddSize == 0)
		return E_OK;

	i2c_transfconfig(hi2c,DevAddress,MemAddSize, I2C_RELOAD_MODE, I2C_GENERATE_START_WRITE);

	/*
	 *  TXIS�ե饰�����Ԥ�
	 */
	if((ercd = i2c_waitsettxisflag(hi2c, Timeout, &Timeout)) != E_OK){
		if(hi2c->ErrorCode == I2C_ERROR_AF)
			return E_SYS;
		else
			return ercd;
	}

	/*
	 *  ���ꥢ�ɥ쥹����
	 */
	if(MemAddSize == I2C_MEMADD_SIZE_8BIT){
		/*
		 *  8�ӥåȥ��ꥢ�ɥ쥹����
		 */
		sil_wrw_mem((uint32_t *)(hi2c->base+TOFF_I2C_TXDR), (MemAddress & 0xff));
	}
	else{
		/*
		 *  16�ӥåȥ��ꥢ�ɥ쥹����
		 */
		sil_wrw_mem((uint32_t *)(hi2c->base+TOFF_I2C_TXDR), ((MemAddress >> 8) & 0xff));

		/*
		 *  TXIS�ե饰�����Ԥ�
		 */
		if((ercd = i2c_waitsettxisflag(hi2c, Timeout, &Timeout)) != E_OK){
			if(hi2c->ErrorCode == I2C_ERROR_AF)
				return E_SYS;
			else
				return ercd;
		}

		sil_wrw_mem((uint32_t *)(hi2c->base+TOFF_I2C_TXDR), (MemAddress & 0xff));
	}

	/*
	 *  TCR�ե饰�����Ԥ�
	 */
	return i2c_isrflagsetwait(hi2c, I2C_ISR_TCR, Timeout, &Timeout);
}

/*
 *  I2C�ǥХ����ν����
 *  parameter1 hi2c  I2C�ϥ�ɥ�ؤΥݥ���
 *  return           ���ｪλ����E_OK
 */
I2C_Handle_t *
i2c_init(ID port, I2C_Init_t *ii2c)
{
	GPIO_Init_t GPIO_Init_Data;
	const I2C_PortControlBlock *ipcb;
	I2C_Handle_t *hi2c;
	uint32_t no;

	if(port < I2C1_PORTID || port > NUM_I2CPORT)
		return NULL;
	if(ii2c == NULL)
		return NULL;

	no = INDEX_I2C(port);
	ipcb = &i2c_pcb[no];
	hi2c = &I2cHandle[no];
	if(hi2c->status != I2C_STATUS_RESET)
		return NULL;
	memcpy(&hi2c->Init, ii2c, sizeof(I2C_Init_t));
	hi2c->base  = ipcb->base;
	hi2c->i2cid = port;

	/*
	 *  SCL/SDA�ԥ�/I2C����å�����
	 */
	sil_orw_mem((uint32_t *)ipcb->gioclockbase, ipcb->gioclockbit1);
	sil_orw_mem((uint32_t *)ipcb->gioclockbase, ipcb->gioclockbit2);
	sil_orw_mem((uint32_t *)ipcb->i2cclockbase, ipcb->i2cclockbit);

	GPIO_Init_Data.mode      = GPIO_MODE_AF;
	GPIO_Init_Data.pull      = GPIO_NOPULL;
	GPIO_Init_Data.otype     = GPIO_OTYPE_OD;
	GPIO_Init_Data.speed     = GPIO_SPEED_FAST;
	GPIO_Init_Data.alternate = ipcb->sclaf;
	gpio_setup(ipcb->giobase1, &GPIO_Init_Data, ipcb->sclpin);

	GPIO_Init_Data.alternate = ipcb->sdaaf;
	gpio_setup(ipcb->giobase2, &GPIO_Init_Data, ipcb->sdapin);


	hi2c->status = I2C_STATUS_BUSY;

	/*
	 *  I2C���
	 */
	sil_andw_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR1), I2C_CR1_PE);

	/*
	 *  I2C�����ߥ󥰥쥸��������
	 */
	sil_wrw_mem((uint32_t *)(hi2c->base+TOFF_I2C_TIMINGR), (hi2c->Init.Timing &  TIMING_CLEAR_MASK));

	/*
	 *  I2C�ǥХ������ɥ쥹������
	 */
	sil_andw_mem((uint32_t *)(hi2c->base+TOFF_I2C_OAR1), I2C_OAR1_OA1EN);
	sil_wrw_mem((uint32_t *)(hi2c->base+TOFF_I2C_OAR1), (hi2c->Init.AddressingMode | hi2c->Init.OwnAddress1));

	/*
	 *  I2C�ޥ������⡼������
	 */
	if(hi2c->Init.AddressingMode == I2C_ADDRESSINGMODE_10BIT)
		sil_wrw_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR2), I2C_CR2_ADD10);
	sil_orw_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR2), (I2C_CR2_AUTOEND | I2C_CR2_NACK));

	/*
	 *  I2C�ǥХ������ɥ쥹������
	 */
	sil_andw_mem((uint32_t *)(hi2c->base+TOFF_I2C_OAR2), I2C_DUALADDRESS_ENABLE);
	sil_wrw_mem((uint32_t *)(hi2c->base+TOFF_I2C_OAR2), (hi2c->Init.DualAddressMode | hi2c->Init.OwnAddress2 | hi2c->Init.OwnAddress2Masks));

	/*
	 *  I2C CR1����
	 */
	sil_wrw_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR1), (hi2c->Init.GeneralCallMode | hi2c->Init.NoStretchMode));

	/*
	 *  I2C��ư
	 */
	sil_orw_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR1), I2C_CR1_PE);

	hi2c->ErrorCode = I2C_ERROR_NONE;
	hi2c->status = I2C_STATUS_READY;
	return hi2c;
}

/*
 *  I2C�ǥХ�����̵����
 *  parameter1 hi2c  I2C�ϥ�ɥ�ؤΥݥ���
 *  return           ���ｪλ����E_OK
 */
ER
i2c_deinit(I2C_Handle_t *hi2c)
{
	const I2C_PortControlBlock *ipcb;

	if(hi2c == NULL){
		return E_PAR;
	}

	hi2c->status = I2C_STATUS_BUSY;
	ipcb = &i2c_pcb[INDEX_I2C(hi2c->i2cid)];

	/*
	 *  I2C���
	 */
	sil_andw_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR1), I2C_CR1_PE);

	/*
	 *  I2C����å��ꥻ�å�
	 */
	sil_orw_mem((uint32_t *)ipcb->i2cresetbase, ipcb->i2cresetbit);
	sil_andw_mem((uint32_t *)ipcb->i2cresetbase, ipcb->i2cresetbit);

	/*
	 *  I2C����å����
	 */
	sil_andw_mem((uint32_t *)ipcb->i2cclockbase, ipcb->i2cclockbit);

	hi2c->ErrorCode = I2C_ERROR_NONE;
	hi2c->status = I2C_STATUS_RESET;
	return E_OK;
}

/*
 *  I2C���졼�ּ���
 *  parameter1 hi2c:   I2C�ϥ�ɥ�ؤΥݥ���
 *  parameter2 pData:  �ɤ߽Ф��Хåե��ؤΥݥ���
 *  parameter3 Size:   �ɤ߽Ф�������
 *  return             ���ｪλ�ʤ�E_OK
 */
ER
i2c_slaveread(I2C_Handle_t *hi2c, uint8_t *pData, uint16_t Size)
{
	ER ercd = E_OK;

	if(pData == NULL || Size == 0)
		return E_PAR;

	/*
	 *  �ɤ߽Ф���å�
	 */
	if(hi2c->Init.semlock != 0)
		wai_sem(hi2c->Init.semlock);

	if(hi2c->status != I2C_STATUS_READY){
		if(hi2c->Init.semlock != 0)
			sig_sem(hi2c->Init.semlock);
		return E_OBJ;
	}

    /*
	 *  ���ɥ쥹�����Υ�å���ͭ����
	 */
	sil_andw_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR2), I2C_CR2_NACK);

    hi2c->status = I2C_STATUS_BUSY_RX;
    hi2c->ErrorCode = I2C_ERROR_NONE;

    hi2c->pBuffPtr = pData;
    hi2c->XferSize = Size;
    hi2c->XferCount = Size;
    hi2c->XferCount2 = Size;
	hi2c->XferOptions = I2C_NO_OPTION_FRAME;

    /*
	 *  �ɤ߽Ф�����ߵ���
	 */
	sil_orw_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR1), (I2C_INT_RX | I2C_INT_LISTEN));
	ercd = i2c_transwait(hi2c, 5000);

	/*
	 *  �ɤ߽Ф���å����
	 */
	if(hi2c->Init.semlock != 0)
		sig_sem(hi2c->Init.semlock);
	return ercd;
}

/*
 *  I2C���졼������
 *  parameter1 hi2c:   I2C�ϥ�ɥ�ؤΥݥ���
 *  parameter2 pData:  �ɤ߽Ф��Хåե��ؤΥݥ���
 *  parameter3 Size:   �ɤ߽Ф�������
 *  return             ���ｪλ�ʤ�E_OK
 */
ER
i2c_slavewrite(I2C_Handle_t *hi2c, uint8_t *pData, uint16_t Size)
{
	ER ercd = E_OK;

	if(pData == NULL || Size == 0)
		return E_PAR;

	/*
	 *  �񤭹��ߥ�å�
	 */
	if(hi2c->Init.semlock != 0)
		wai_sem(hi2c->Init.semlock);

	if(hi2c->status != I2C_STATUS_READY){
		if(hi2c->Init.semlock != 0)
			sig_sem(hi2c->Init.semlock);
		return E_OBJ;
	}

    /*
	 *  ���ɥ쥹�����Υ�å���ͭ����
	 */
	sil_andw_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR2), I2C_CR2_NACK);

    hi2c->status = I2C_STATUS_BUSY_TX;
    hi2c->ErrorCode = I2C_ERROR_NONE;

    hi2c->pBuffPtr = pData;
    hi2c->XferSize = Size;
    hi2c->XferCount = Size;
    hi2c->XferCount2 = Size;
    hi2c->XferOptions = I2C_NO_OPTION_FRAME;

    /*
	 *  �񤭹��߳���ߵ���
	 */
	sil_orw_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR1), (I2C_INT_TX | I2C_INT_LISTEN));
	ercd = i2c_transwait(hi2c, 5000);

	/*
	 *  �񤭹��ߥ�å����
	 */
	if(hi2c->Init.semlock != 0)
		sig_sem(hi2c->Init.semlock);
	return ercd;
}

/*
 *  I2C�ǡ����꡼��
 *  parameter1 hi2c:       I2C�ϥ�ɥ�ؤΥݥ���
 *  parameter2 DevAddress: ���졼�֥��ɥ쥹
 *  parameter3 MemAddress: ���ꥢ�ɥ쥹
 *  parameter4 MemAddSize: ���ꥢ�ɥ쥹������
 *  parameter5 pData:      �ɤ߽Ф��Хåե��ؤΥݥ���
 *  parameter6 Size:       �ɤ߽Ф�������
 *  return                 ���ｪλ�ʤ�E_OK
 */
ER
i2c_memread(I2C_Handle_t *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size)
{
	ER ercd = E_OK;
	uint32_t xfermode;

	if(pData == NULL || Size == 0)
		return E_PAR;
	if(hi2c->Init.AddressingMode != I2C_ADDRESSINGMODE_7BIT && MemAddSize != 0)
		return E_PAR;

	/*
	 *  �ɤ߽Ф���å�
	 */
	if(hi2c->Init.semlock != 0)
		wai_sem(hi2c->Init.semlock);

	if(hi2c->status != I2C_STATUS_READY || i2c_busreadywait(hi2c, I2C_TIMEOUT_BUSY_FLAG) != E_OK){
		if(hi2c->Init.semlock != 0)
			sig_sem(hi2c->Init.semlock);
		return E_OBJ;
	}

    hi2c->status = I2C_STATUS_BUSY_RX;
    hi2c->ErrorCode = I2C_ERROR_NONE;

    hi2c->pBuffPtr = pData;
    hi2c->XferSize = Size;
    hi2c->XferCount = Size;
    if(hi2c->XferCount > MAX_NBYTE_SIZE){
		hi2c->XferCount2 = MAX_NBYTE_SIZE;
		xfermode = I2C_RELOAD_MODE;
	}
	else{
		hi2c->XferCount2 = hi2c->XferCount;
		xfermode = I2C_AUTOEND_MODE;
	}

	/* ���졼�֥��ɥ쥹���ɤ߽Ф����ɥ쥹������ */
	if((ercd = i2c_masterreadaddress(hi2c, DevAddress, MemAddress, MemAddSize, I2C_TIMEOUT_FLAG)) != E_OK){
		if(hi2c->Init.semlock != 0)
			sig_sem(hi2c->Init.semlock);
        return ercd;
	}

    /* Set NBYTES to write and reload if hi2c->XferCount > MAX_NBYTE_SIZE and generate RESTART */
	i2c_transfconfig(hi2c,DevAddress, hi2c->XferCount2, xfermode, I2C_GENERATE_START_READ);

    /*
	 *  �ɤ߽Ф�����ߵ���
	 */
	sil_orw_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR1), I2C_INT_RX);
	ercd = i2c_transwait(hi2c, 100);

	/*
	 *  �ɤ߽Ф���å����
	 */
	if(hi2c->Init.semlock != 0)
		sig_sem(hi2c->Init.semlock);
	return ercd;
}

/*
 *  I2C�ǡ����饤��
 *  parameter1 hi2c:       I2C�ϥ�ɥ�ؤΥݥ���
 *  parameter2 DevAddress: ���졼�֥��ɥ쥹
 *  parameter3 MemAddress: ���ꥢ�ɥ쥹
 *  parameter4 MemAddSize: ���ꥢ�ɥ쥹������
 *  parameter5 pData:      ����ߥХåե��ؤΥݥ���
 *  parameter6 Size:       ����ߥ�����
 *  return                 ���ｪλ�ʤ�E_OK
 */
ER
i2c_memwrite(I2C_Handle_t *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size)
{
	ER ercd = E_OK;
	uint32_t xfermode;

	if(pData == NULL || Size == 0)
		return  E_PAR;
	if(hi2c->Init.AddressingMode != I2C_ADDRESSINGMODE_7BIT && MemAddSize != 0)
		return  E_PAR;

	/*
	 *  �񤭹��ߥ�å�
	 */
	if(hi2c->Init.semlock != 0)
		wai_sem(hi2c->Init.semlock);

	if(hi2c->status != I2C_STATUS_READY || i2c_busreadywait(hi2c, I2C_TIMEOUT_BUSY_FLAG) != E_OK){
		if(hi2c->Init.semlock != 0)
			sig_sem(hi2c->Init.semlock);
		return E_OBJ;
	}

	hi2c->status = I2C_STATUS_BUSY_TX;
	hi2c->ErrorCode = I2C_ERROR_NONE;

	hi2c->pBuffPtr = pData;
	hi2c->XferSize = Size;
	hi2c->XferCount = Size;
    if(hi2c->XferCount > MAX_NBYTE_SIZE){
		hi2c->XferCount2 = MAX_NBYTE_SIZE;
		xfermode = I2C_RELOAD_MODE;
	}
	else{
		hi2c->XferCount2 = hi2c->XferCount;
		xfermode = I2C_AUTOEND_MODE;
	}

	/* ���졼�֥��ɥ쥹�Ƚ���ߥ��ɥ쥹������ */
	if((ercd = i2c_masterwriteaddress(hi2c, DevAddress, MemAddress, MemAddSize, I2C_TIMEOUT_FLAG)) != E_OK){
		if(hi2c->Init.semlock != 0)
			sig_sem(hi2c->Init.semlock);
		return ercd;
	}

    /* Set NBYTES to write and reload if hi2c->XferCount > MAX_NBYTE_SIZE and generate RESTART */
	if(MemAddSize == 0)
	    i2c_transfconfig(hi2c, DevAddress, hi2c->XferSize, xfermode, I2C_GENERATE_START_WRITE);
	else
	    i2c_transfconfig(hi2c, DevAddress, hi2c->XferCount2, xfermode, I2C_NO_STARTSTOP);

    /*
	 *  �񤭹��߳���ߵ���
	 */
	sil_orw_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR1), I2C_INT_TX);
	ercd = i2c_transwait(hi2c, 500);

	/*
	 *  �񤭹��ߥ�å����
	 */
	if(hi2c->Init.semlock != 0)
		sig_sem(hi2c->Init.semlock);
	return ercd;
}

/*
 *  I2C��ǥ������å�
 *  parameter1 hi2c:       I2C�ϥ�ɥ�ؤΥݥ���
 *  parameter2 DevAddress: ���졼�֥��ɥ쥹
 *  parameter3 Trials:     �ȥ饤������
 *  parameter4 Timeout:    �����ॢ������
 *  return                 ��ǥ��ʤ�E_OK
 */
ER
i2c_ready(I2C_Handle_t *hi2c, uint16_t DevAddress, uint32_t Trials, uint32_t Timeout)
{
	volatile uint32_t I2C_Trials = 0;
	uint32_t tmp1 = 0;

	if(hi2c->status == I2C_STATUS_READY){
		if((sil_rew_mem((uint32_t *)(hi2c->base+TOFF_I2C_ISR)) & I2C_ISR_BUSY) != 0)
			return E_OBJ;

		/*
		 *  ����������å�
		 */
		if(hi2c->Init.semlock != 0)
			wai_sem(hi2c->Init.semlock);

		hi2c->status    = I2C_STATUS_BUSY;
		hi2c->ErrorCode = I2C_ERROR_NONE;

		do{
			/* START���� */
			if(hi2c->Init.AddressingMode == I2C_ADDRESSINGMODE_7BIT)
				tmp1 = ((DevAddress & I2C_CR2_SADD) | I2C_CR2_START | I2C_CR2_AUTOEND) & (~I2C_CR2_RD_WRN);
			else
				tmp1 = ((DevAddress & I2C_CR2_SADD) | I2C_CR2_ADD10 | I2C_CR2_START) & (~I2C_CR2_RD_WRN);
			sil_wrw_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR2), tmp1);

			/* START STOP/NACK�Ԥ� */
			if(i2c_isrflagsetwait(hi2c, (I2C_ISR_STOPF | I2C_ISR_NACKF), Timeout, &Timeout) != E_OK){
				/*
				 *  ��å����
				 */
				if(hi2c->Init.semlock != 0)
					sig_sem(hi2c->Init.semlock);
				return E_TMOUT;
			}


			/* NACK�ե饰�����å� */
			if((sil_rew_mem((uint32_t *)(hi2c->base+TOFF_I2C_ISR)) & I2C_ISR_NACKF) == 0){
				/*
				 *  STOP�ե饰��Ω�Ĥޤ��Ԥ�
				 */
				if(i2c_isrflagsetwait(hi2c, I2C_ISR_STOPF, Timeout, &Timeout) != E_OK)
					return E_TMOUT;

				/*
				 *  STOP�ե饰���ꥢ
				 */
				sil_wrw_mem((uint32_t *)(hi2c->base+TOFF_I2C_ICR), I2C_ICR_STOPCF);
				hi2c->status = I2C_STATUS_READY;
				/*
				 *  ��å����
				 */
				if(hi2c->Init.semlock != 0)
					sig_sem(hi2c->Init.semlock);
				return E_OK;
			}
			else{	/* �����Υ�å���������� */
				/*
				 *  STOP�ե饰��Ω�Ĥޤ��Ԥ�
				 */
				if(i2c_isrflagsetwait(hi2c, I2C_ISR_STOPF, Timeout, &Timeout) != E_OK)
					return E_TMOUT;

				/*
				 *  NACK�ե饰���ꥢ
				 */
				sil_wrw_mem((uint32_t *)(hi2c->base+TOFF_I2C_ICR), I2C_ICR_NACKCF);

				/*
				 *  STOP�ե饰���ꥢ
				 */
				sil_wrw_mem((uint32_t *)(hi2c->base+TOFF_I2C_ICR), I2C_ICR_STOPCF);
			}
			if(I2C_Trials++ == Trials){
				/*
				 *  ����׵�
				 */
				sil_orw_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR2), I2C_CR2_STOP);

				/*
				 *  STOP�ե饰��Ω�Ĥޤ��Ԥ�
				 */
				if(i2c_isrflagsetwait(hi2c, I2C_ISR_STOPF, Timeout, &Timeout) != E_OK)
					return E_TMOUT;

				/*
				 *  STOP�ե饰���ꥢ
				 */
				sil_wrw_mem((uint32_t *)(hi2c->base+TOFF_I2C_ICR), I2C_ICR_STOPCF);
			}
		}while(I2C_Trials < Trials);

		hi2c->status = I2C_STATUS_READY;
		/*
		 *  ��å����
		 */
		if(hi2c->Init.semlock != 0)
			sig_sem(hi2c->Init.semlock);
	    return E_TMOUT;
	}
	else
		return E_OBJ;
}

/*
 *  I2C�ޥ������������󥷥��ˤ�뽪λ����
 *  parameter1 hi2c:       I2C�ϥ�ɥ�ؤΥݥ���
 *  return                 �ʤ�
 */
static void
i2c_intmastersequentialcomplete(I2C_Handle_t *hi2c)
{
	/* No Generate Stop, to permit restart mode */
	/* The stop will be done at the end of transfer, when I2C_AUTOEND_MODE enable */
	if(hi2c->status == I2C_STATUS_BUSY_TX){
		/*
		 *  ������ʤ�н�λ����
		 */
		hi2c->status = I2C_STATUS_READY;
		sil_andw_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR1), I2C_INT_TX);
		if(hi2c->writecallback != NULL)
			hi2c->writecallback(hi2c);
	}
	else{
		/*
		 *  ������ʤ�н�λ����
		 */
		hi2c->status = I2C_STATUS_READY;
		sil_andw_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR1), I2C_INT_RX);
		if(hi2c->readcallback != NULL)
			hi2c->readcallback(hi2c);
	}
	if(hi2c->Init.semid != 0)
		isig_sem(hi2c->Init.semid);
}

/*
 *  I2C�ޥ�����STOP�ե饰�ˤ�뽪λ����
 *  parameter1 hi2c:       I2C�ϥ�ɥ�ؤΥݥ���
 *  parameter2 isr:        ISR�쥸������
 *  return                 �ʤ�
 */
static void
i2c_intmastercomplete(I2C_Handle_t *hi2c, uint32_t isr)
{
	/*
	 *  STOP�ե饰���ꥢ
	 */
	sil_wrw_mem((uint32_t *)(hi2c->base+TOFF_I2C_ICR), I2C_ICR_STOPCF);

	/*
	 *  ž���쥸�������ꥢ
	 */
	sil_andw_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR2), ((I2C_CR2_SADD | I2C_CR2_HEAD10R | I2C_CR2_NBYTES | I2C_CR2_RELOAD | I2C_CR2_RD_WRN)));

	/*
	 *  ���ץ�������ꥯ�ꥢ
	 */
	hi2c->XferOptions   = I2C_NO_OPTION_FRAME;

	/*
	 *  NACK�����ʤ饨�顼����
	 */

	if((isr & I2C_ISR_NACKF) != 0){
		sil_wrw_mem((uint32_t *)(hi2c->base+TOFF_I2C_ICR), I2C_ICR_NACKCF);
		hi2c->ErrorCode |= I2C_ERROR_AF;
	}

	/*
	 *  �����쥸�����ե�å���
	 */
	i2c_flushtxdr(hi2c);

	/*
	 *  ����ߥޥ���
	 */
	sil_andw_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR1), (I2C_INT_TX | I2C_INT_RX));

	/*
	 *  ���顼ȯ���ʤ�Х��������顼���ղ�
	 */
	if(hi2c->ErrorCode != I2C_ERROR_NONE){
		hi2c->ErrorCode |= I2C_ERROR_SIZE;
		sil_andw_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR1), I2C_INT_ALL);
	}
	else if(hi2c->status == I2C_STATUS_BUSY_TX){
		/*
		 *  ������ʤ�н�λ����
		 */
		hi2c->status = I2C_STATUS_READY;
		if(hi2c->writecallback != NULL)
			hi2c->writecallback(hi2c);
		if(hi2c->Init.semid != 0)
			isig_sem(hi2c->Init.semid);
	}
	else if(hi2c->status == I2C_STATUS_BUSY_RX){
		/*
		 *  ������ʤ�н�λ����
		 */
		hi2c->status = I2C_STATUS_READY;
		if(hi2c->readcallback != NULL)
			hi2c->readcallback(hi2c);
		if(hi2c->Init.semid != 0)
			isig_sem(hi2c->Init.semid);
	}
}

/*
 *  I2C���졼��LISTEN��λ����
 *  parameter1 hi2c:       I2C�ϥ�ɥ�ؤΥݥ���
 *  parameter2 isr:        ISR�쥸������
 *  return                 �ʤ�
 */
static void
i2c_intlistencomplete(I2C_Handle_t *hi2c, uint32_t isr)
{
	/*
	 *  I2C����򥯥ꥢ
	 */
	hi2c->XferOptions = I2C_NO_OPTION_FRAME;
	hi2c->status = I2C_STATUS_READY;

	/* Store Last receive data if any */
	if((isr & I2C_ISR_RXNE) != 0){
	    /*
		 *  �����ǡ���������С�������
		 */
	    (*hi2c->pBuffPtr++) = (uint8_t)sil_rew_mem((uint32_t *)(hi2c->base+TOFF_I2C_RXDR));

		/*
		 *  ���������뤷�Ƥ��ʤ���С�Non-Acknowledge���顼�Ȥ���
		 */

	    if(hi2c->XferCount2 > 0){
			hi2c->XferCount2--;
			hi2c->XferCount--;
			hi2c->ErrorCode |= I2C_ERROR_AF;
		}
	}

	/*
	 *  ������߶ػ�
	 */
	sil_andw_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR1), I2C_INT_ALL);

	/*
	 *  NACK�ե饰���ꥢ
	 */
	sil_wrw_mem((uint32_t *)(hi2c->base+TOFF_I2C_ICR), I2C_ICR_NACKCF);

	/*
	 *  LISTEN������Хå�����
	 */
	if(hi2c->listencallback != NULL)
		hi2c->listencallback(hi2c);
}

/*
 *  I2C���졼�֥��ɥ쥹��λ����
 *  parameter1 hi2c:       I2C�ϥ�ɥ�ؤΥݥ���
 *  parameter2 isr:        ISR�쥸������
 *  return                 �ʤ�
 */
static void
i2c_intaddrcomplete(I2C_Handle_t *hi2c, uint32_t isr)
{
	uint8_t  transferdirection = 0;
	uint16_t slaveaddrcode = 0;
	uint16_t ownadd1code = 0;
	uint16_t ownadd2code = 0;

	/*
	 *  LISTEN��ʤ�С����ɥ쥹�������Ф�
	 */
	if((hi2c->status & I2C_STATUS_LISTEN) == I2C_STATUS_LISTEN){
		transferdirection = (sil_rew_mem((uint32_t *)(hi2c->base+TOFF_I2C_ISR)) & I2C_ISR_DIR) >> 16;
		slaveaddrcode     = (sil_rew_mem((uint32_t *)(hi2c->base+TOFF_I2C_ISR)) & I2C_ISR_ADDCODE) >> 16;
		ownadd1code       = sil_rew_mem((uint32_t *)(hi2c->base+TOFF_I2C_OAR1)) & I2C_OAR1_OA1;
		ownadd2code       = sil_rew_mem((uint32_t *)(hi2c->base+TOFF_I2C_OAR2)) & I2C_OAR2_OA2;

		if(hi2c->Init.AddressingMode == I2C_ADDRESSINGMODE_10BIT){
			/*
			 *  10�ӥåȥ��ɥ쥹�⡼��
			 */
			if((slaveaddrcode & SlaveAddr_MSK) == ((ownadd1code >> SlaveAddr_SHIFT) & SlaveAddr_MSK)){
				slaveaddrcode = ownadd1code;
				hi2c->AddrEventCount++;
				if(hi2c->AddrEventCount == 2){
					/*
					 *  ���٥�ȥ����󥿡����ꥢ
					 */
					hi2c->AddrEventCount = 0;

					/*
					 *  ADDR�ե饰���ꥢ
					 */
					sil_wrw_mem((uint32_t *)(hi2c->base+TOFF_I2C_ICR), I2C_ICR_ADDRCF);

				}
				/*
				 *  ���ɥ쥹������Хå�
				 */
				if(hi2c->addrcallback != NULL)
					hi2c->addrcallback(hi2c, transferdirection, slaveaddrcode);
			}
			else{
				slaveaddrcode = ownadd2code;

				/*
				 *  LISTEN����߶ػ�
				 */
				sil_andw_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR1), I2C_INT_LISTEN);

				/*
				 *  ���ɥ쥹������Хå�
				 */
				if(hi2c->addrcallback != NULL)
					hi2c->addrcallback(hi2c, transferdirection, slaveaddrcode);
			}
		}
		else{
			/*
			 *  7�ӥåȥ��ɥ쥹�⡼�ɡ�LISTEN����߶ػ�
			 */
			sil_andw_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR1), I2C_INT_LISTEN);

			/*
			 *  ���ɥ쥹������Хå�
			 */
			if(hi2c->addrcallback != NULL)
				hi2c->addrcallback(hi2c, transferdirection, slaveaddrcode);
		}
	}
	else{
		/*
		 *  ADDR�ե饰���ꥢ
		 */
		sil_wrw_mem((uint32_t *)(hi2c->base+TOFF_I2C_ICR), I2C_ICR_ADDRCF);
	}
}

/*
 *  I2C���졼�֥������󥷥��ˤ�뽪λ����
 *  parameter1 hi2c:       I2C�ϥ�ɥ�ؤΥݥ���
 *  return                 �ʤ�
  */
static void
i2c_intslavesequencecomplete(I2C_Handle_t *hi2c)
{
	if(hi2c->status == I2C_STATUS_BUSY_TX_LISTEN){
		/*
		 *  ������ʤ齪λ����
		 */
		hi2c->status = I2C_STATUS_LISTEN;
		sil_andw_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR1), I2C_INT_TX);
		if(hi2c->writecallback != NULL)
			hi2c->writecallback(hi2c);
	}
	else if(hi2c->status == I2C_STATUS_BUSY_RX_LISTEN){
		/*
		 *  ������ʤ齪λ����
		 */
	    hi2c->status = I2C_STATUS_LISTEN;
		sil_andw_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR1), I2C_INT_RX);
		if(hi2c->readcallback != NULL)
			hi2c->readcallback(hi2c);
	}
}

/*
 *  I2C���졼��STOP�ե饰�ˤ�뽪λ����
 *  parameter1 hi2c:       I2C�ϥ�ɥ�ؤΥݥ���
 *  parameter2 isr:        ISR�쥸������
 *  return                 �ʤ�
 */
static void
i2c_intslavecomplete(I2C_Handle_t *hi2c, uint32_t isr)
{
	/*
	 *  STOP�ե饰�򥯥ꥢ
	 */
	sil_wrw_mem((uint32_t *)(hi2c->base+TOFF_I2C_ICR), I2C_ICR_STOPCF);

	/*
	 *  ADDR�ե饰�򥯥ꥢ
	 */
	sil_wrw_mem((uint32_t *)(hi2c->base+TOFF_I2C_ICR), I2C_ICR_ADDRCF);

	/*
	 *  ����������
	 */
	sil_andw_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR1), I2C_INT_ALL);

	/*
	 *  ���ɥ쥹�����Υ�å�ͭ����
	 */
	sil_orw_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR2), I2C_CR2_NACK);

	/*
	 *  ž������쥸�����ꥻ�å�
	 */
	sil_andw_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR2), (I2C_CR2_SADD | I2C_CR2_HEAD10R | I2C_CR2_NBYTES | I2C_CR2_RELOAD | I2C_CR2_RD_WRN));

	/*
	 *  �����쥸�����ե�å���
	 */
	i2c_flushtxdr(hi2c);

	if(hi2c->XferCount != 0){
		/*
		 *  ž����λ�Ǥʤ���С�Non-Acknowledge���顼
		 */
		hi2c->ErrorCode |= I2C_ERROR_AF;
	}

	/*
	 *  �����׵᤬����м�����
	 */
	if((isr & I2C_ISR_RXNE) != 0){
		(*hi2c->pBuffPtr++) = (uint8_t)sil_rew_mem((uint32_t *)(hi2c->base+TOFF_I2C_RXDR));
		if((hi2c->XferCount > 0U)){
			hi2c->XferCount2--;
			hi2c->XferCount--;
		}
	}

	if(hi2c->ErrorCode != I2C_ERROR_NONE){
	    /*
		 *  ���顼��ȯ�����Ƥ���Х��������顼��ä���
		 */
		hi2c->ErrorCode |= I2C_ERROR_SIZE;
		sil_andw_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR1), I2C_INT_ALL);

	    if(hi2c->status == I2C_STATUS_LISTEN){
			/*
			 *  LISTEN��ʤ�С�LISTEN��λ������Ԥ�
			 */
			i2c_intlistencomplete(hi2c, isr);
		}
	}
	else if(hi2c->XferOptions != I2C_NO_OPTION_FRAME){
		/*
		 *  ���ץ�������꤬�����硢LISTEN��λ����
		 */
	    hi2c->XferOptions = I2C_NO_OPTION_FRAME;
	    hi2c->status = I2C_STATUS_READY;
		if(hi2c->listencallback != NULL)
			hi2c->listencallback(hi2c);
	}
	else if(hi2c->status == I2C_STATUS_BUSY_RX){
		/*
		 *  ������ʤ齪λ����
		 */
		hi2c->status = I2C_STATUS_READY;
		if(hi2c->readcallback != NULL)
			hi2c->readcallback(hi2c);
	}
	else{
		/*
		 *  ������ʤ齪λ����
		 */
		hi2c->status = I2C_STATUS_READY;
		if(hi2c->writecallback != NULL)
			hi2c->writecallback(hi2c);
	}
}


/*
 *  I2C EV����ߥϥ�ɥ�
 */
void
i2c_ev_handler(I2C_Handle_t *hi2c)
{
	uint32_t isr = sil_rew_mem((uint32_t *)(hi2c->base+TOFF_I2C_ISR));
	uint32_t cr1 = sil_rew_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR1));
	uint16_t devaddress = 0;
	volatile uint32_t tmp;

	/*
	 *  �ޥ������⡼��
	 */
	if(hi2c->Init.OwnAddress1 == 0){
		if((isr & I2C_ISR_NACKF) != 0 && (cr1 & I2C_CR1_NACKIE) != 0){
			/*
			 *  NACK���顼�ե饰���ꥢ
			 */
			sil_wrw_mem((uint32_t *)(hi2c->base+TOFF_I2C_ICR), I2C_ICR_NACKCF);
		    hi2c->ErrorCode |= I2C_ERROR_AF;

		    /*
			 *  �����쥸�����ե�å���
			 */
		    i2c_flushtxdr(hi2c);
		}
		else if((isr & I2C_ISR_RXNE) != 0 && (cr1 & I2C_CR1_RXIE) != 0){
			/*
			 *  �����ǡ���������
			 */
			*hi2c->pBuffPtr++ = (uint8_t)sil_rew_mem((uint32_t *)(hi2c->base+TOFF_I2C_RXDR));
			hi2c->XferCount--;
			hi2c->XferCount2--;
		}
		else if((isr & I2C_ISR_TXIS) != 0 && (cr1 & I2C_CR1_TXIE) != 0){
			/*
			 *  �����ǡ�������
			 */
			tmp = (uint8_t)(*hi2c->pBuffPtr++);
			sil_wrw_mem((uint32_t *)(hi2c->base+TOFF_I2C_TXDR), tmp);
			hi2c->XferCount--;
			hi2c->XferCount2--;
		}
		else if((isr & I2C_ISR_TCR) != 0 && (cr1 & I2C_CR1_TCIE) != 0){
			if(hi2c->XferCount2 == 0 && hi2c->XferCount != 0){
				devaddress = (sil_rew_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR2)) & I2C_CR2_SADD);
				if(hi2c->XferCount > MAX_NBYTE_SIZE){
					hi2c->XferCount2 = MAX_NBYTE_SIZE;
					i2c_transfconfig(hi2c, devaddress, hi2c->XferCount2, I2C_RELOAD_MODE, I2C_NO_STARTSTOP);
				}
				else{
					hi2c->XferCount2 = hi2c->XferCount;
					if(hi2c->XferOptions != I2C_NO_OPTION_FRAME)
						i2c_transfconfig(hi2c, devaddress, hi2c->XferCount2, hi2c->XferOptions, I2C_NO_STARTSTOP);
					else
						i2c_transfconfig(hi2c, devaddress, hi2c->XferCount2, I2C_AUTOEND_MODE, I2C_NO_STARTSTOP);
				}
			}
			else{
				/*
				 *  AUTOEND�⡼�ɤʤ����ｪλ
				 */
				if((sil_rew_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR2)) & I2C_CR2_AUTOEND) != 0)
					i2c_intmastersequentialcomplete(hi2c);
				else{
					/*
					 *  �۾ｪλ������
					 */
					hi2c->ErrorCode |= I2C_ERROR_SIZE;
					sil_andw_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR1), I2C_INT_ALL);
				}
			}
		}
		else if((isr & I2C_ISR_TC) != 0 && (cr1 & I2C_CR1_TCIE) != 0){
			if(hi2c->XferCount == 0){
				if((sil_rew_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR2)) & I2C_CR2_AUTOEND) != I2C_CR2_AUTOEND){
					if(hi2c->XferOptions == I2C_NO_OPTION_FRAME){
						/*
						 *  ���ץ���󤬤ʤ����STOP�׵�
						 */
						sil_orw_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR2), I2C_CR2_STOP);
					}
					else
						i2c_intmastersequentialcomplete(hi2c);
				}
			}
			else{
				/*
				 *  TC�ե饰���顼
				 */
				hi2c->ErrorCode |= I2C_ERROR_SIZE;
				sil_andw_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR1), I2C_INT_ALL);
			}
		}
		if((isr & I2C_ISR_STOPF) != 0 && (cr1 & I2C_CR1_STOPIE) != 0){
			/*
			 *  ��ߥե饰���齪λ����
			 */
			i2c_intmastercomplete(hi2c, isr);
		}
	}

	/*
	 *  ���졼�֥⡼��
	 */
	else{
		if((isr & I2C_ISR_NACKF) != 0 && (cr1 & I2C_CR1_NACKIE) != 0){
			/*
			 *  NACKȯ��
			 */
			if(hi2c->XferCount == 0){
				/*
				 *  ž����λ��
				 */
				if((hi2c->XferOptions == I2C_FIRST_AND_LAST_FRAME || hi2c->XferOptions == I2C_LAST_FRAME) && \
			        (hi2c->status == I2C_STATUS_LISTEN)){
					/* Call I2C Listen complete process */
					i2c_intlistencomplete(hi2c, isr);
				}
				else if(hi2c->XferOptions != I2C_NO_OPTION_FRAME && hi2c->status == I2C_STATUS_BUSY_TX_LISTEN){
					/*
					 *  NACK�ե饰���ꥢ
					 */
					sil_wrw_mem((uint32_t *)(hi2c->base+TOFF_I2C_ICR), I2C_ICR_NACKCF);

					/* Flush TX register */
					i2c_flushtxdr(hi2c);

					/* Last Byte is Transmitted */
					/* Call I2C Slave Sequential complete process */
					i2c_intslavesequencecomplete(hi2c);
				}
				else{
					/*
					 *  NACK�ե饰���ꥢ
					 */
					sil_wrw_mem((uint32_t *)(hi2c->base+TOFF_I2C_ICR), I2C_ICR_NACKCF);
				}
			}
			else{
				/*
				 *  ž������λ���Ƥ��ʤ���硢Non-Acknowledge���顼
				 */
				sil_wrw_mem((uint32_t *)(hi2c->base+TOFF_I2C_ICR), I2C_ICR_NACKCF);
				hi2c->ErrorCode |= I2C_ERROR_AF;
			}
		}
		else if((isr & I2C_ISR_RXNE) != 0 && (cr1 & I2C_CR1_RXIE) != 0){
			/*
			 *  ���������
			 */
			if(hi2c->XferCount > 0){
				(*hi2c->pBuffPtr++) = (uint8_t)sil_rew_mem((uint32_t *)(hi2c->base+TOFF_I2C_RXDR));
				hi2c->XferCount2--;
				hi2c->XferCount--;
			}
			if(hi2c->XferCount == 0 && hi2c->XferOptions != I2C_NO_OPTION_FRAME){
				/*
				 *  I2C���졼�֥������󥹽�λ
				 */
				i2c_intslavesequencecomplete(hi2c);
			}
		}
		else if((isr & I2C_ISR_ADDR) != 0 && (cr1 & I2C_CR1_ADDRIE) != 0){
			/*
			 *  ADDR�ե饰
			 */
			i2c_intaddrcomplete(hi2c, isr);
		}
		else if((isr & I2C_ISR_TXIS) != 0 && (cr1 & I2C_CR1_TXIE) != 0){
			/*
			 *  ���������
			 */
		    if(hi2c->XferCount > 0){
				/* Write data to TXDR */
				tmp = (*hi2c->pBuffPtr++);
				sil_wrw_mem((uint32_t *)(hi2c->base+TOFF_I2C_TXDR), tmp);
				hi2c->XferCount--;
				hi2c->XferCount2--;
			}
			else{
				if(hi2c->XferOptions == I2C_NEXT_FRAME || hi2c->XferOptions == I2C_FIRST_FRAME){
					/*
					 *  I2C���졼�������������󥹽�λ
					 */
 					i2c_intslavesequencecomplete(hi2c);
				}
			}
		}
		if((isr & I2C_ISR_STOPF) != 0 && (cr1 & I2C_CR1_STOPIE) != 0){
			/*
			 *  ��ߥե饰���齪λ����
			 */
			i2c_intslavecomplete(hi2c, isr);
		}
	}
}

/*
 *  I2C ER����ߥϥ�ɥ�
 */
void
i2c_er_handler(I2C_Handle_t *hi2c)
{
	uint32_t isr = sil_rew_mem((uint32_t *)(hi2c->base+TOFF_I2C_ISR));
	uint32_t cr1 = sil_rew_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR1));

	/*
	 *  I2C �Х����顼
	 */
	if(((isr & I2C_ISR_BERR) != 0) && ((cr1 & I2C_CR1_ERRIE) != 0)){
		hi2c->ErrorCode |= I2C_ERROR_BERR;
		sil_wrw_mem((uint32_t *)(hi2c->base+TOFF_I2C_ICR), I2C_ICR_BERRCF);
	}
	isr = sil_rew_mem((uint32_t *)(hi2c->base+TOFF_I2C_ISR));

	/*
	 *  I2C �����С���󡦥��顼
	 */
	if(((isr & I2C_ISR_OVR) != 0) && ((cr1 & I2C_CR1_ERRIE) != 0)){
		hi2c->ErrorCode |= I2C_ERROR_OVR;
		sil_wrw_mem((uint32_t *)(hi2c->base+TOFF_I2C_ICR), I2C_ICR_OVRCF);
	}
	isr = sil_rew_mem((uint32_t *)(hi2c->base+TOFF_I2C_ISR));

	/*
	 *  I2C �����ӥơ�����󡦥������顼
	 */
	if(((isr & I2C_ISR_ARLO) != 0) && ((cr1 & I2C_CR1_ERRIE) != 0)){
		hi2c->ErrorCode |= I2C_ERROR_ARLO;
		sil_wrw_mem((uint32_t *)(hi2c->base+TOFF_I2C_ICR), I2C_ICR_ARLOCF);
	}
	isr = sil_rew_mem((uint32_t *)(hi2c->base+TOFF_I2C_ISR));

	if(hi2c->ErrorCode != I2C_ERROR_NONE){
		hi2c->status = I2C_STATUS_READY;

		/*
		 *  �̿��ػ�
		 */
		sil_andw_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR1), (I2C_CR1_TCIE |
			 I2C_CR1_TXIE | I2C_CR1_RXIE | I2C_CR1_ERRIE | I2C_CR1_STOPIE | I2C_CR1_ADDRIE));

		if(hi2c->errorcallback != NULL)
			hi2c->errorcallback(hi2c);
	}
}


/*
 *  I2C EV����ߥ����ӥ��롼����
 */
void
i2c_ev_isr(intptr_t exinf)
{
	i2c_ev_handler(&I2cHandle[INDEX_I2C((uint32_t)exinf)]);
}

/*
 *  I2C ER����ߥ����ӥ��롼����
 */
void
i2c_er_isr(intptr_t exinf)
{
	i2c_er_handler(&I2cHandle[INDEX_I2C((uint32_t)exinf)]);
}

