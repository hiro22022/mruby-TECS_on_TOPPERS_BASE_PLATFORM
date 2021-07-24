/*
 *  TOPPERS/ASP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Advanced Standard Profile Kernel
 * 
 *  Copyright (C) 2008-2011 by Embedded and Real-Time Systems Laboratory
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
 *  @(#) $Id: i2c.c 698 2016-02-01 15:15:16Z roi $
 */
/*
 * 
 *  I2C�ɥ饤�дؿ���
 *
 */
#include "kernel_impl.h"
#include <t_syslog.h>
#include <t_stdlib.h>
#include <string.h>
#include <sil.h>
#include <target_syssvc.h>
#include "kernel_cfg.h"
#include "device.h"
#include "i2c.h"

#define sil_orw_mem(a, b)		sil_wrw_mem((a), sil_rew_mem(a) | (b))
#define sil_andw_mem(a, b)		sil_wrw_mem((a), sil_rew_mem(a) & ~(b))
#define sil_modw_mem(a, b, c)	sil_wrw_mem((a), (sil_rew_mem(a) & (~b)) | (c))

/*
 *  I2C�ݡ���ID��������֥�å�����Ф�����Υޥ���
 */
#define INDEX_I2C(i2cid)        ((uint_t)((i2cid) - 1))

#define GPIO_AF4_I2C1           0x04	/* I2C1 Alternate Function mapping */
#define GPIO_AF4_I2C2           0x04	/* I2C2 Alternate Function mapping */
#define GPIO_AF4_I2C3           0x04	/* I2C3 Alternate Function mapping */
#define GPIO_AF4_I2C4           0x04	/* I2C4 Alternate Function mapping */
#define GPIO_AF11_I2C4          0x0B	/* I2C4 Alternate Function mapping */

/*
 *  I2C�ݡ�������ơ��֥�
 */
static const I2C_PortControlBlock i2c_pcb[NUM_I2CPORT] = {
  {	TADR_I2C1_BASE,
	(TADR_RCC_BASE+TOFF_RCC_AHB1ENR),  RCC_AHB1ENR_GPIOBEN, RCC_AHB1ENR_GPIOBEN,
	(TADR_RCC_BASE+TOFF_RCC_APB1ENR),  RCC_APB1ENR_I2C1EN,
	(TADR_RCC_BASE+TOFF_RCC_APB1RSTR), RCC_APB1RSTR_I2C1RST,
	TADR_GPIOB_BASE, TADR_GPIOB_BASE,
	8, GPIO_AF4_I2C1, 9, GPIO_AF4_I2C1 },

  {	TADR_I2C2_BASE,
	(TADR_RCC_BASE+TOFF_RCC_AHB1ENR),  RCC_AHB1ENR_GPIOHEN, RCC_AHB1ENR_GPIOHEN,
	(TADR_RCC_BASE+TOFF_RCC_APB1ENR),  RCC_APB1ENR_I2C2EN,
	(TADR_RCC_BASE+TOFF_RCC_APB1RSTR), RCC_APB1RSTR_I2C2RST,
	TADR_GPIOH_BASE, TADR_GPIOH_BASE,
	1, GPIO_AF4_I2C2, 0, GPIO_AF4_I2C2 },

  {	TADR_I2C3_BASE,
	(TADR_RCC_BASE+TOFF_RCC_AHB1ENR),  RCC_AHB1ENR_GPIOHEN, RCC_AHB1ENR_GPIOHEN,
	(TADR_RCC_BASE+TOFF_RCC_APB1ENR),  RCC_APB1ENR_I2C3EN,
	(TADR_RCC_BASE+TOFF_RCC_APB1RSTR), RCC_APB1RSTR_I2C3RST,
	TADR_GPIOH_BASE, TADR_GPIOH_BASE,
	7, GPIO_AF4_I2C3, 8, GPIO_AF4_I2C3 },

#if defined(TOPPERS_STM32F769_DISCOVERY)
  {	TADR_I2C4_BASE,
	(TADR_RCC_BASE+TOFF_RCC_AHB1ENR),  RCC_AHB1ENR_GPIODEN, RCC_AHB1ENR_GPIOBEN,
	(TADR_RCC_BASE+TOFF_RCC_APB1ENR),  RCC_APB1ENR_I2C4EN,
	(TADR_RCC_BASE+TOFF_RCC_APB1RSTR), RCC_APB1RSTR_I2C4RST,
	TADR_GPIOD_BASE, TADR_GPIOB_BASE,
	12, GPIO_AF4_I2C4, 7, GPIO_AF11_I2C4 }
#else
  {	TADR_I2C4_BASE,
	(TADR_RCC_BASE+TOFF_RCC_AHB1ENR),  RCC_AHB1ENR_GPIODEN, RCC_AHB1ENR_GPIODEN,
	(TADR_RCC_BASE+TOFF_RCC_APB1ENR),  RCC_APB1ENR_I2C4EN,
	(TADR_RCC_BASE+TOFF_RCC_APB1RSTR), RCC_APB1RSTR_I2C4RST,
	TADR_GPIOD_BASE, TADR_GPIOB_BASE,
	12, GPIO_AF4_I2C4, 13, GPIO_AF4_I2C4 }
#endif
};

static I2C_Handle_t I2cHandle[NUM_I2CPORT];

/*
 *  I2C �������ȥ��ȥåץ⡼�����
 */
#define I2C_NO_STARTSTOP    0x00000000

/*
 *  I2C ����ɥ���ɥ⡼�����
 */
#define I2C_SOFTEND_MODE    0x00000000

#define TIMING_CLEAR_MASK   0xF0FFFFFF		/* I2C TIMING clear register Mask */

#define I2C_CR1_TRANS1      (I2C_CR1_TCIE | I2C_CR1_STOPIE | I2C_CR1_NACKIE | I2C_CR1_TXIE | I2C_CR1_ADDRIE)
#define I2C_CR1_TRANS2      (I2C_CR1_TCIE | I2C_CR1_STOPIE | I2C_CR1_NACKIE | I2C_CR1_TXIE)

#define I2C_CR1_RECV1       (I2C_CR1_TCIE| I2C_CR1_STOPIE| I2C_CR1_NACKIE | I2C_CR1_RXIE | I2C_CR1_ADDRIE)
#define I2C_CR1_RECV2       (I2C_CR1_TCIE| I2C_CR1_STOPIE| I2C_CR1_NACKIE | I2C_CR1_RXIE)
#define I2C_CR1_ERROR       (I2C_CR1_ERRIE | I2C_CR1_TCIE | I2C_CR1_STOPIE | I2C_CR1_NACKIE)
#define I2C_CR2_RESET       (I2C_CR2_SADD | I2C_CR2_HEAD10R | I2C_CR2_NBYTES | I2C_CR2_RELOAD | I2C_CR2_RD_WRN)

/*
 *  I2C�ǥХ����ν����
 *  parameter1 hi2c  I2C�ϥ�ɥ�ؤΥݥ���
 *  return           ���ｪλ����E_OK
 */
I2C_Handle_t *
i2c_init(ID port, I2C_Init_t *pini)
{
	const I2C_PortControlBlock *ipcb;
	GPIO_Init_t  gpio_init;
	I2C_Handle_t *hi2c;
	volatile uint32_t tmp;
	int no;

	if(port < I2C1_PORTID || port > NUM_I2CPORT)
		return NULL;
	if(pini == NULL)
		return NULL;

	no = INDEX_I2C(port);
	ipcb = &i2c_pcb[no];

	hi2c = &I2cHandle[no];
	if(hi2c->State != I2C_STATE_RESET)
		return NULL;
	memcpy(&hi2c->Init, pini, sizeof(I2C_Init_t));
	hi2c->base  = ipcb->base;
	hi2c->State = I2C_STATE_BUSY;
	hi2c->i2cid = port;

	/*
	 *  I2C-GPIO����å�����
	 */
	sil_orw_mem((uint32_t *)ipcb->gioclockbase, ipcb->gioclockbit1);
	tmp = sil_rew_mem((uint32_t *)ipcb->gioclockbase);
	sil_orw_mem((uint32_t *)ipcb->gioclockbase, ipcb->gioclockbit2);
	tmp = sil_rew_mem((uint32_t *)ipcb->gioclockbase);

	/*
	 *  SCL�ԥ�����
	 */
	gpio_init.mode  = GPIO_MODE_AF;
	gpio_init.pull  = GPIO_NOPULL;
	gpio_init.otype = GPIO_OTYPE_OD;
	gpio_init.speed = GPIO_SPEED_FAST;
	gpio_init.alternate = ipcb->sclaf;
	gpio_setup(ipcb->giobase1, &gpio_init, ipcb->sclpin);

	/*
	 *  SDA�ԥ�����
	 */
	gpio_init.alternate = ipcb->sdaaf;
	gpio_setup(ipcb->giobase2, &gpio_init, ipcb->sdapin);

	sil_orw_mem((uint32_t *)ipcb->i2cclockbase, ipcb->i2cclockbit);
	tmp = sil_rew_mem((uint32_t *)ipcb->i2cclockbase);
	(void)(tmp);
	sil_orw_mem((uint32_t *)ipcb->i2cresetbase, ipcb->i2cresetbit);
	sil_andw_mem((uint32_t *)ipcb->i2cresetbase, ipcb->i2cresetbit);

	/* I2C Disable */
	sil_andw_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR1), I2C_CR1_PE);

	/*
	 *  I2C TIMINGR����
	 */
	sil_wrw_mem((uint32_t *)(hi2c->base+TOFF_I2C_TIMINGR), (hi2c->Init.Timing & TIMING_CLEAR_MASK));

	/*
	 *  I2C���ɥ쥹1����
	 */
	sil_andw_mem((uint32_t *)(hi2c->base+TOFF_I2C_OAR1), I2C_OAR1_OA1EN);
	if(hi2c->Init.AddressingMode == I2C_ADDRESSINGMODE_7BIT)
		sil_wrw_mem((uint32_t *)(hi2c->base+TOFF_I2C_OAR1), (I2C_OAR1_OA1EN | hi2c->Init.OwnAddress1));
	else /* I2C_ADDRESSINGMODE_10BIT */
		sil_wrw_mem((uint32_t *)(hi2c->base+TOFF_I2C_OAR1), (I2C_OAR1_OA1EN | I2C_OAR1_OA1MODE | hi2c->Init.OwnAddress1));

	/*
	 *  I2C CR2�쥸��������
	 */
	if(hi2c->Init.AddressingMode == I2C_ADDRESSINGMODE_10BIT)
		sil_wrw_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR2), (I2C_CR2_ADD10));
	sil_orw_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR2), (I2C_CR2_AUTOEND | I2C_CR2_NACK));

	/*
	 *  I2C���ɥ쥹2����
	 */
	sil_wrw_mem((uint32_t *)(hi2c->base+TOFF_I2C_OAR2), (hi2c->Init.DualAddressMode | hi2c->Init.OwnAddress2 | (hi2c->Init.OwnAddress2Masks << 8)));

	/*
	 *  I2C CR1�쥸��������
	 */
	sil_wrw_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR1), (hi2c->Init.GeneralCallMode | hi2c->Init.NoStretchMode));
	sil_orw_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR1), I2C_CR1_PE);

	hi2c->ErrorCode = I2C_ERROR_NONE;
	hi2c->State = I2C_STATE_READY;
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
	/* �ϥ�ɥ�Υ����å� */
	if(hi2c == NULL)
		return E_PAR;

	hi2c->State = I2C_STATE_BUSY;

	/*
	 *  I2C����å����
	 */
	sil_andw_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR1), I2C_CR1_PE);

	hi2c->ErrorCode = I2C_ERROR_NONE;
	hi2c->State = I2C_STATE_RESET;
	return E_OK;
}

/*
 *  I2C�̿��Υ�����������
 *  parameter1 hi2c       I2C�ϥ�ɥ�ؤΥݥ���
 *  parameter2 DevAddress ���졼�֥��ɥ쥹
 *  parameter3 Size       ���ꥢ�ɥ쥹�Υ�����
 *  parameter4 Mode       ����⡼��
 *  return     �ʤ�
 */
static void
i2c_transfarconfig(I2C_Handle_t *hi2c, uint16_t DevAddress, uint8_t Size, uint32_t Mode, uint32_t Request)
{
	uint32_t tmpreg = 0;

	/*
	 * I2C CR2�쥸����������
	 */
	tmpreg = sil_rew_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR2));
	tmpreg &= (uint32_t)~((uint32_t)(I2C_CR2_SADD | I2C_CR2_NBYTES | I2C_CR2_RELOAD | I2C_CR2_AUTOEND | I2C_CR2_RD_WRN | I2C_CR2_START | I2C_CR2_STOP));
	tmpreg |= (uint32_t)(((uint32_t)DevAddress & I2C_CR2_SADD) | (((uint32_t)Size << 16 ) & I2C_CR2_NBYTES) |
            (uint32_t)Mode | (uint32_t)Request);
	sil_wrw_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR2), tmpreg);
}

/*
 *  I2C�ե饰����ꤷ�������Ԥ��ؿ�
 *  parameter1 hi2c  I2C�ϥ�ɥ�ؤΥݥ���
 *  parameter2 Flag  �Ԥ��ե饰
 *  parameter3 Timeout �����ॢ���Ȼ���(ms)
 *  return           ���ｪλ E_OK
 */
static ER
i2c_waitsetflag(I2C_Handle_t *hi2c, uint32_t Flag, uint32_t Timeout)
{
	uint32_t tick = 0;

	/*
	 *  �ե饰�����Ԥ�
	 */
	while((sil_rew_mem((uint32_t *)(hi2c->base+TOFF_I2C_ISR)) & Flag) != Flag){
		if(tick > Timeout){
			hi2c->State= I2C_STATE_READY;
			return E_TMOUT;
		}
		dly_tsk(1);
		tick++;
	}
	return E_OK;
}

/*
 *  I2C ���졼�֥��ɥ쥹�ȥ��ꥢ�ɥ쥹����
 *  parameter1 hi2c       I2C�ϥ�ɥ�ؤΥݥ���
 *  parameter2 DevAddress:���졼�֥��ɥ쥹
 *  parameter3 MemAddress:���ꥢ�ɥ쥹
 *  parameter4 MemAddSize:���ꥢ�ɥ쥹�Υ�����
 *  parameter5 Mode      :����⡼��
 *  parameter6 Timeout:�����ॢ���Ȼ���(ms)
 *  return     �������� E_OK
 */
static ER
i2c_requestmemaddr(I2C_Handle_t *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint32_t Mode, uint32_t Timeout)
{
	ER err = E_OK;

	i2c_transfarconfig(hi2c,DevAddress,MemAddSize, Mode, I2C_CR2_START);
	/*
	 * TXIS�ե饰�����Ԥ�
	 */
	if((err = i2c_waitsetflag(hi2c, I2C_ISR_TXIS, Timeout)) != E_OK)
		return err;

	/*
	 * ���ꥢ�ɥ쥹����
	 */
	if(MemAddSize == I2C_MEMADD_SIZE_8BIT){	/* 8bit���� */
		sil_wrw_mem((uint32_t *)(hi2c->base+TOFF_I2C_TXDR), (MemAddress & 0xff));
	}
	else{									/* 16bit���� */
		/* ���ꥢ�ɥ쥹MSB���� */
		sil_wrw_mem((uint32_t *)(hi2c->base+TOFF_I2C_TXDR), ((MemAddress>>8) & 0xff));
		if((err = i2c_waitsetflag(hi2c, I2C_ISR_TXIS, Timeout)) != E_OK)
			return err;
		/* ���ꥢ�ɥ쥹LSB���� */
		sil_wrw_mem((uint32_t *)(hi2c->base+TOFF_I2C_TXDR), (MemAddress & 0xff));
	}
	/*
	 *  TC(R)�ե饰�����Ԥ�
	 */
	if(Mode == I2C_SOFTEND_MODE)
		err = i2c_waitsetflag(hi2c, I2C_ISR_TC, Timeout);
	else
		err = i2c_waitsetflag(hi2c, I2C_ISR_TCR, Timeout);
	return err;
}

/*
 *  I2Cž���Ԥ�
 *  parameter1 hi2c    I2C�ϥ�ɥ�ؤΥݥ���
 *  parameter2 Timeout:�����ॢ���Ȼ���(ms)
 */
static ER
i2c_transwait(I2C_Handle_t *hi2c, uint32_t Timeout)
{
	uint32_t tick = 0;

	while(hi2c->State != I2C_STATE_READY){
		if(tick >= Timeout)
			return E_TMOUT;
		if(hi2c->Init.semid != 0)
			twai_sem(hi2c->Init.semid, 1);
		else
			dly_tsk(1);
		tick++;
	}
	return E_OK;
}

/*
 *  I2C���졼�ּ���
 *  parameter1 hi2c   I2C�ϥ�ɥ�ؤΥݥ���
 *  parameter2 pData: �ɤ߽Ф��Хåե��ؤΥݥ���
 *  parameter3 Size:  �ɤ߽Ф�������
 *  return            ���ｪλ�ʤ�E_OK
 */
ER
i2c_slaveread(I2C_Handle_t *hi2c, uint8_t *pData, uint16_t Size)
{
	ER err = E_OK;

	if(pData == NULL || Size == 0)
		return E_PAR;

	/*
	 *  �ɤ߽Ф���å�
	 */
	if(hi2c->Init.semlock != 0)
		wai_sem(hi2c->Init.semlock);

	if(hi2c->State != I2C_STATE_READY){
		if(hi2c->Init.semlock != 0)
			sig_sem(hi2c->Init.semlock);
		return E_OBJ;
	}

    hi2c->State = I2C_STATE_SLAVE_BUSY_RX;
    hi2c->ErrorCode   = I2C_ERROR_NONE;

    /*
	 *  ���ɥ쥹�������Υ�å�����
	 */
	sil_andw_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR2), I2C_CR2_NACK);

    hi2c->pBuffPtr = pData;
    hi2c->XferSize = Size;
    hi2c->XferCount = Size;

    /*
	 *  �ɤ߽Ф�����ߵ���
	 */
	sil_orw_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR1), (I2C_CR1_ERROR | I2C_CR1_ADDRIE | I2C_CR1_RXIE));
	err = i2c_transwait(hi2c, 500);

	/*
	 *  ��å����
	 */
	if(hi2c->Init.semlock != 0)
		sig_sem(hi2c->Init.semlock);
    return err;
}

/*
 *  I2C���졼������
 *  parameter1 hi2c   I2C�ϥ�ɥ�ؤΥݥ���
 *  parameter2 pData: �ɤ߽Ф��Хåե��ؤΥݥ���
 *  parameter3 Size:  �ɤ߽Ф�������
 *  return            ���ｪλ�ʤ�E_OK
 */
ER
i2c_slavewrite(I2C_Handle_t *hi2c, uint8_t *pData, uint16_t Size)
{
	ER err = E_OK;

	if(pData == NULL || Size == 0)
		return E_PAR;

	/*
	 *  �ɤ߽Ф���å�
	 */
	if(hi2c->Init.semlock != 0)
		wai_sem(hi2c->Init.semlock);

	if(hi2c->State != I2C_STATE_READY){
		if(hi2c->Init.semlock != 0)
			sig_sem(hi2c->Init.semlock);
		return E_OBJ;
	}

    hi2c->State = I2C_STATE_SLAVE_BUSY_TX;
    hi2c->ErrorCode   = I2C_ERROR_NONE;

    /*
	 *  ���ɥ쥹�������Υ�å�����
	 */
	sil_andw_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR2), I2C_CR2_NACK);

    hi2c->pBuffPtr = pData;
    hi2c->XferSize = Size;
    hi2c->XferCount = Size;

    /*
	 *  �񤭹��߳���ߵ���
	 */
	sil_orw_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR1), (I2C_CR1_ERROR | I2C_CR1_ADDRIE | I2C_CR1_TXIE));
	err = i2c_transwait(hi2c, 500);

	/*
	 *  ��å����
	 */
	if(hi2c->Init.semlock != 0)
		sig_sem(hi2c->Init.semlock);
    return err;
}

/*
 *  I2C�ޥ������ǡ����꡼��
 *  parameter1 hi2c        I2C�ϥ�ɥ�ؤΥݥ���
 *  parameter2 DevAddress: ���졼�֥��ɥ쥹
 *  parameter3 MemAddress: ���ꥢ�ɥ쥹
 *  parameter4 MemAddSize: ���ꥢ�ɥ쥹������
 *  parameter5 pData:      �ɤ߽Ф��Хåե��ؤΥݥ���
 *  parameter6 Size:       �ɤ߽Ф�������
 *  parameter7 Timeout:    �����ॢ���Ȼ���(ms)
 *  return                 ���ｪλ�ʤ�E_OK
 */
ER
i2c_memread(I2C_Handle_t *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
	ER err = E_OK;
	uint32_t Mode = I2C_CR2_RELOAD;

	if((pData == NULL) || (Size == 0))
		return E_PAR;

	/*
	 *  �ɤ߽Ф���å�
	 */
	if(hi2c->Init.semlock != 0)
		wai_sem(hi2c->Init.semlock);

	if(hi2c->State != I2C_STATE_READY
		 || (sil_rew_mem((uint32_t *)(hi2c->base+TOFF_I2C_ISR)) & I2C_ISR_BUSY) == I2C_ISR_BUSY){
		if(hi2c->Init.semlock != 0)
			sig_sem(hi2c->Init.semlock);
		return E_OBJ;
	}

	hi2c->State = I2C_STATE_MASTER_BUSY_RX;
	hi2c->ErrorCode = I2C_ERROR_NONE;

	hi2c->pBuffPtr = pData;
	hi2c->XferCount = Size;
	if(Size > 255)
		hi2c->XferSize = 255;
	else
		hi2c->XferSize = Size;

	/* ���졼�֥��ɥ쥹���ɤ߽Ф����ɥ쥹������ */
	if(MemAddSize != 0){
	    if((err = i2c_requestmemaddr(hi2c, DevAddress, MemAddress, MemAddSize, I2C_SOFTEND_MODE, Timeout)) != E_OK){
			if(hi2c->Init.semlock != 0)
				sig_sem(hi2c->Init.semlock);
			return err;
		}
	}

	/*
	 *  ���ɥ쥹�񤭹��ߥե�����
	 */
    if( (hi2c->XferSize == 255) && (hi2c->XferSize < hi2c->XferCount) )
		Mode = I2C_CR2_RELOAD;
	else
		Mode = I2C_CR2_AUTOEND;
	i2c_transfarconfig(hi2c, DevAddress, hi2c->XferSize, Mode, (I2C_CR2_START | I2C_CR2_RD_WRN));

    /*
	 *  �ɤ߽Ф�����ߵ���
	 */
	sil_orw_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR1), (I2C_CR1_ERROR | I2C_CR1_RXIE));
	err = i2c_transwait(hi2c, Timeout);

	/*
	 *  ��å����
	 */
	if(hi2c->Init.semlock != 0)
		sig_sem(hi2c->Init.semlock);
	return err;
}

/*
 *  I2C�ޥ������ǡ����饤��
 *  parameter1 hi2c        I2C�ϥ�ɥ�ؤΥݥ���
 *  parameter2 DevAddress: ���졼�֥��ɥ쥹
 *  parameter3 MemAddress: ���ꥢ�ɥ쥹
 *  parameter4 MemAddSize: ���ꥢ�ɥ쥹������
 *  parameter5 pData:      ����ߥХåե��ؤΥݥ���
 *  parameter6 Size:       ����ߥ�����
 *  parameter7 Timeout:    �����ॢ���Ȼ���(ms)
 */
ER
i2c_memwrite(I2C_Handle_t *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
	ER err = E_OK;
	uint32_t Mode = I2C_CR2_RELOAD;
	uint32_t Request = I2C_CR2_START /*I2C_GENERATE_START_WRITE*/;

	if((pData == NULL) || (Size == 0))
		return E_PAR;

	/*
	 *  ����ߥ�å�
	 */
	if(hi2c->Init.semlock != 0)
		wai_sem(hi2c->Init.semlock);

	if(hi2c->State != I2C_STATE_READY
		  || (sil_rew_mem((uint32_t *)(hi2c->base+TOFF_I2C_ISR)) & I2C_ISR_BUSY) == I2C_ISR_BUSY){
		if(hi2c->Init.semlock != 0)
			sig_sem(hi2c->Init.semlock);
		return E_OBJ;
	}

	hi2c->State = I2C_STATE_MASTER_BUSY_TX;
	hi2c->ErrorCode = I2C_ERROR_NONE;

	hi2c->pBuffPtr = pData;
	hi2c->XferCount = Size;
	if(Size > 255)
		hi2c->XferSize = 255;
	else
		hi2c->XferSize = Size;

	/* ���졼�֥��ɥ쥹�Ƚ���ߥ��ɥ쥹������ */
	if(MemAddSize != 0){
		Request = I2C_NO_STARTSTOP;
		if((err = i2c_requestmemaddr(hi2c, DevAddress, MemAddress, MemAddSize, I2C_CR2_RELOAD, Timeout)) != E_OK){
			if(hi2c->Init.semlock != 0)
			sig_sem(hi2c->Init.semlock);
			return err;
		}
	}

	/*
	 *  ���ɥ쥹�񤭹��ߥե�����
	 */
    if(hi2c->XferSize == 255 && hi2c->XferSize < hi2c->XferCount)
		Mode = I2C_CR2_RELOAD;
	else
		Mode = I2C_CR2_AUTOEND;
	i2c_transfarconfig(hi2c, DevAddress, hi2c->XferSize, Mode, Request);

    /*
	 *  �񤭹��߳���ߵ���
	 */
	sil_orw_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR1), (I2C_CR1_ERROR | I2C_CR1_TXIE));
	err = i2c_transwait(hi2c, Timeout);

	/*
	 *  ��å����
	 */
	if(hi2c->Init.semlock != 0)
		sig_sem(hi2c->Init.semlock);
	return err;
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
	uint32_t tick = 0;
	uint32_t tmp;

	if(hi2c->State == I2C_STATE_READY){
		if((sil_rew_mem((uint32_t *)(hi2c->base+TOFF_I2C_ISR)) & I2C_ISR_BUSY) != 0)
			return E_OBJ;

		/*
		 *  ����������å�
		 */
		if(hi2c->Init.semlock != 0)
			wai_sem(hi2c->Init.semlock);

		hi2c->State =     I2C_STATE_BUSY;
		hi2c->ErrorCode = I2C_ERROR_NONE;

		do{
			/* ������������ */
			if(hi2c->Init.AddressingMode == I2C_ADDRESSINGMODE_7BIT)
				tmp = (((DevAddress & I2C_CR2_SADD) | I2C_CR2_START | I2C_CR2_AUTOEND) & ~I2C_CR2_RD_WRN);
			else
				tmp = (((DevAddress & I2C_CR2_SADD) | I2C_CR2_ADD10 | I2C_CR2_START) & ~I2C_CR2_RD_WRN);
			sil_wrw_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR2), tmp);

		    /*
			 *  ���ȥåץե饰�ޤ���NACK�ե饰�Ԥ�
			 */
			while((sil_rew_mem((uint32_t *)(hi2c->base+TOFF_I2C_ISR)) & (I2C_ISR_STOPF | I2C_ISR_NACKF)) == 0){
				if(tick > Timeout){
					/* Device is ready */
					hi2c->State = I2C_STATE_READY;
					/*
					 *  ��å����
					 */
					if(hi2c->Init.semlock != 0)
						sig_sem(hi2c->Init.semlock);
					return E_TMOUT;
				}
				dly_tsk(1);
				tick++;
			}

			/* NACK�ե饰���� */
			if((sil_rew_mem((uint32_t *)(hi2c->base+TOFF_I2C_ISR)) & I2C_ISR_NACKF) == 0){
				/* ���ȥåץե饰�ꥻ�å��Ԥ� */
				if(i2c_waitsetflag(hi2c, I2C_ISR_STOPF, Timeout) != E_OK){
					/*
					 *  ��å����
					 */
					if(hi2c->Init.semlock != 0)
						sig_sem(hi2c->Init.semlock);
					return E_TMOUT;
				}

				/* ���ȥåץե饰���ꥢ */
				sil_wrw_mem((uint32_t *)(hi2c->base+TOFF_I2C_ICR), I2C_ICR_STOPCF);

				hi2c->State = I2C_STATE_READY;
				/*
				 *  ��å����
				 */
				if(hi2c->Init.semlock != 0)
					sig_sem(hi2c->Init.semlock);
				return E_OK;
			}
			else{	/* ���ȥåץե饰���� */
				/* ���ȥåץե饰�ꥻ�å��Ԥ� */ 
				if(i2c_waitsetflag(hi2c, I2C_ISR_STOPF, Timeout) != E_OK){
					/*
					 *  ��å����
					 */
					if(hi2c->Init.semlock != 0)
						sig_sem(hi2c->Init.semlock);
					return E_TMOUT;
				}

				/* NACK�ե饰���ꥢ */
				sil_wrw_mem((uint32_t *)(hi2c->base+TOFF_I2C_ICR), I2C_ICR_NACKCF);

				/* ���ȥåץե饰���ꥢ */
				sil_wrw_mem((uint32_t *)(hi2c->base+TOFF_I2C_ICR), I2C_ICR_STOPCF);
			}

			/* Check if the maximum allowed number of trials has been reached */
			if(I2C_Trials++ == Trials){
				/* ���ȥå����� */
				sil_orw_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR2), I2C_CR2_STOP);

				/* ���ȥåץե饰�ꥻ�å��Ԥ� */ 
				if(i2c_waitsetflag(hi2c, I2C_ISR_STOPF, Timeout) != E_OK){
					/*
					 *  ��å����
					 */
					if(hi2c->Init.semlock != 0)
						sig_sem(hi2c->Init.semlock);
					return E_TMOUT;
				}

				/* ���ȥåץե饰���ꥢ */
				sil_wrw_mem((uint32_t *)(hi2c->base+TOFF_I2C_ICR), I2C_ICR_STOPCF);
			}
		}while(I2C_Trials < Trials);

		hi2c->State = I2C_STATE_READY;
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
 *  I2C�ޥ��������������
 */
static void
i2c_mastertrans_isr(I2C_Handle_t *hi2c)
{
	uint32_t isr = sil_rew_mem((uint32_t *)(hi2c->base+TOFF_I2C_ISR));
	uint16_t DevAddress;

	if((isr & I2C_ISR_TXIS) != 0){	/* ������ */
		sil_wrw_mem((uint32_t *)(hi2c->base+TOFF_I2C_TXDR), (*hi2c->pBuffPtr++));
		hi2c->XferSize--;
		hi2c->XferCount--;
	}
	else if((isr & I2C_ISR_TCR) != 0){	/* ���������� */
		if(hi2c->XferSize == 0 && hi2c->XferCount != 0){
			DevAddress = sil_rew_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR2)) & I2C_CR2_SADD;
			if(hi2c->XferCount > 255){
				i2c_transfarconfig(hi2c,DevAddress,255,  I2C_CR2_RELOAD, I2C_NO_STARTSTOP);
				hi2c->XferSize = 255;
			}
			else{
				i2c_transfarconfig(hi2c,DevAddress,hi2c->XferCount, I2C_CR2_AUTOEND, I2C_NO_STARTSTOP);
				hi2c->XferSize = hi2c->XferCount;
			}
		}
		else{	/* ���������顼 */
			hi2c->ErrorCode |= I2C_ERROR_SIZE;
			if(hi2c->errorcallback != NULL)
				hi2c->errorcallback(hi2c);
		}
	}
	else if((isr & I2C_ISR_TC) != 0){	/* ������λ */
		if(hi2c->XferCount == 0){
			sil_orw_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR2), I2C_CR2_STOP);
		}
		else{	/* ���������顼 */
			hi2c->ErrorCode |= I2C_ERROR_SIZE;
			if(hi2c->errorcallback != NULL)
				hi2c->errorcallback(hi2c);
		}
	}
	else if((isr & I2C_ISR_STOPF) != 0){	/* ���ȥåץե饰 */
		sil_andw_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR1), (I2C_CR1_ERROR | I2C_CR1_TXIE));
		/*
		 *  ���ȥåץե饰���ꥢ
		 */
		sil_wrw_mem((uint32_t *)(hi2c->base+TOFF_I2C_ICR), I2C_ICR_STOPCF);
		/*
		 * CR2�򥯥ꥢ
		 */
		sil_andw_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR2), I2C_CR2_RESET);
		hi2c->State = I2C_STATE_READY;
		if(hi2c->writecallback != NULL)
			hi2c->writecallback(hi2c);
	}
	else if((isr & I2C_ISR_NACKF) != 0){	/* NACK�ե饰 */
		sil_wrw_mem((uint32_t *)(hi2c->base+TOFF_I2C_ICR), I2C_ICR_NACKCF);
		hi2c->ErrorCode |= I2C_ERROR_AF;
		if(hi2c->errorcallback != NULL)
			hi2c->errorcallback(hi2c);
	}
}

/*
 *  I2C�ޥ��������������
 */
static void
i2c_masterreceiv_isr(I2C_Handle_t *hi2c)
{
	uint32_t isr = sil_rew_mem((uint32_t *)(hi2c->base+TOFF_I2C_ISR));
	uint16_t DevAddress;

	if((isr & I2C_ISR_RXNE) != 0){		/* ������ */
		(*hi2c->pBuffPtr++) = sil_rew_mem((uint32_t *)(hi2c->base+TOFF_I2C_RXDR));
		hi2c->XferSize--;
		hi2c->XferCount--;
	}
	else if((isr & I2C_ISR_TCR) != 0){	/* ������³ */
		if(hi2c->XferSize == 0 && hi2c->XferCount != 0){
			DevAddress = sil_rew_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR2)) & I2C_CR2_SADD;
			if(hi2c->XferCount > 255){
				i2c_transfarconfig(hi2c, DevAddress, 255, I2C_CR2_RELOAD, I2C_NO_STARTSTOP);
				hi2c->XferSize = 255;
			}
			else{
				i2c_transfarconfig(hi2c, DevAddress, hi2c->XferCount, I2C_CR2_AUTOEND, I2C_NO_STARTSTOP);
				hi2c->XferSize = hi2c->XferCount;
			}
		}
		else{	/* ���������顼 */
			hi2c->ErrorCode |= I2C_ERROR_SIZE;
			if(hi2c->errorcallback != NULL)
				hi2c->errorcallback(hi2c);
		}
	}
	else if((isr & I2C_ISR_TC) != 0){	/* ������λ */
		if(hi2c->XferCount == 0){
			sil_orw_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR2), I2C_CR2_STOP);
		}
		else{
			/* Wrong size Status regarding TCR flag event */
			hi2c->ErrorCode |= I2C_ERROR_SIZE;
			if(hi2c->errorcallback != NULL)
				hi2c->errorcallback(hi2c);
		}
	}
	else if((isr & I2C_ISR_STOPF) != 0){	/* ���ȥåץե饰 */
		sil_andw_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR1), (I2C_CR1_ERROR | I2C_CR1_RXIE));
		/*
		 *  ���ȥåץե饰���ꥢ
		 */
		sil_wrw_mem((uint32_t *)(hi2c->base+TOFF_I2C_ICR), I2C_ICR_STOPCF);
		/*
		 * CR2�򥯥ꥢ
		 */
		sil_andw_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR2), I2C_CR2_RESET);
		hi2c->State = I2C_STATE_READY;
		if(hi2c->readcallback != NULL)
			hi2c->readcallback(hi2c);
	}
	else if((isr & I2C_ISR_NACKF) != 0){	/* NACK�ե饰 */
		sil_wrw_mem((uint32_t *)(hi2c->base+TOFF_I2C_ICR), I2C_ICR_NACKCF);
		hi2c->ErrorCode |= I2C_ERROR_AF;
		if(hi2c->errorcallback != NULL)
			hi2c->errorcallback(hi2c);
	}
}

/*
 *  I2C���졼�����������
 */
static void
i2c_slavesend_isr(I2C_Handle_t *hi2c)
{
	uint32_t isr = sil_rew_mem((uint32_t *)(hi2c->base+TOFF_I2C_ISR));

	if((isr & I2C_ISR_NACKF) != 0){	/* NACK�ե饰 */
		if(hi2c->XferCount == 0){	/* NACK�򥯥ꥢ */
			sil_wrw_mem((uint32_t *)(hi2c->base+TOFF_I2C_ICR), I2C_ICR_NACKCF);
		}
		else{		/* �ǡ�������ǥ����Υ�å��ʤ����顼 */
			sil_wrw_mem((uint32_t *)(hi2c->base+TOFF_I2C_ICR), I2C_ICR_NACKCF);
			hi2c->ErrorCode |= I2C_ERROR_AF;
			if(hi2c->errorcallback != NULL)
				hi2c->errorcallback(hi2c);
		}
	}
	else if((isr & I2C_ISR_ADDR) != 0){
		/* Clear ADDR flag */
		sil_wrw_mem((uint32_t *)(hi2c->base+TOFF_I2C_ICR), I2C_ICR_ADDRCF);
	}
	else if((isr & I2C_ISR_STOPF) != 0){	/* ���ȥåץե饰 */
		sil_andw_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR1), (I2C_CR1_ERROR | I2C_CR1_ADDRIE | I2C_CR1_RXIE | I2C_CR1_TXIE));
	    /*
		 *  ���ɥ쥹�������Υ�å��ػ�
	 	 */
		sil_orw_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR2), I2C_CR2_NACK);
		/*
		 *  ���ȥåץե饰���ꥢ
		 */
		sil_wrw_mem((uint32_t *)(hi2c->base+TOFF_I2C_ICR), I2C_ICR_STOPCF);
		hi2c->State = I2C_STATE_READY;
		if(hi2c->writecallback != NULL)
			hi2c->writecallback(hi2c);
	}
	else if((isr & I2C_ISR_TXIS) != 0){	/* �������� */
		if(hi2c->XferCount > 0){
			sil_wrw_mem((uint32_t *)(hi2c->base+TOFF_I2C_TXDR), (*hi2c->pBuffPtr++));
			hi2c->XferCount--;
		}
	}
}

/*
 *  I2C���졼�ּ��������
 */
static void
i2c_slavereceiv_isr(I2C_Handle_t *hi2c)
{
	uint32_t isr = sil_rew_mem((uint32_t *)(hi2c->base+TOFF_I2C_ISR));

	if((isr & I2C_ISR_NACKF) != 0){		/* NACK�ե饰 */
		sil_wrw_mem((uint32_t *)(hi2c->base+TOFF_I2C_ICR), I2C_ICR_NACKCF);
		hi2c->ErrorCode |= I2C_ERROR_AF;
		if(hi2c->errorcallback != NULL)
			hi2c->errorcallback(hi2c);
	}
	else if((isr & I2C_ISR_ADDR) != 0){	/* ���ɥ쥹�ޥå� */
	    sil_wrw_mem((uint32_t *)(hi2c->base+TOFF_I2C_ICR), I2C_ICR_ADDRCF);
	}
	else if((isr & I2C_ISR_RXNE) != 0){	/* ������ */
		(*hi2c->pBuffPtr++) = sil_rew_mem((uint32_t *)(hi2c->base+TOFF_I2C_RXDR));
		hi2c->XferSize--;
		hi2c->XferCount--;
	}
	else if((isr & I2C_ISR_STOPF) != 0){/* ���ȥåץե饰 */
		sil_andw_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR1), (I2C_CR1_ERROR | I2C_CR1_ADDRIE | I2C_CR1_RXIE));
	    /*
		 *  ���ɥ쥹�������Υ�å��ػ�
	 	 */
		sil_orw_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR2), I2C_CR2_NACK);
		/*
		 *  ���ȥåץե饰���ꥢ
		 */
		sil_wrw_mem((uint32_t *)(hi2c->base+TOFF_I2C_ICR), I2C_ICR_STOPCF);
		hi2c->State = I2C_STATE_READY;
		if(hi2c->readcallback != NULL)
			hi2c->readcallback(hi2c);
	}
}

/*
 *  I2C EV����ߥϥ�ɥ�
 */
void
i2c_ev_handler(I2C_Handle_t *hi2c)
{
	uint32_t cr1 = sil_rew_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR1));
	uint32_t isr = sil_rew_mem((uint32_t *)(hi2c->base+TOFF_I2C_ISR));

	/*
	 * I2C ���졼�������⡼��
	 */
	if(((isr & (I2C_ISR_TXIS | I2C_ISR_TCR | I2C_ISR_TC | I2C_ISR_STOPF | I2C_ISR_NACKF | I2C_ISR_ADDR)) != 0) && ((cr1 & I2C_CR1_TRANS1) == I2C_CR1_TRANS1)){     
		if(hi2c->State == I2C_STATE_SLAVE_BUSY_TX){
			i2c_slavesend_isr(hi2c);
		}
	}
	isr = sil_rew_mem((uint32_t *)(hi2c->base+TOFF_I2C_ISR));

	/*
	 *  I2C �ޥ����������⡼��
	 */
	if(((isr & (I2C_ISR_TXIS | I2C_ISR_TCR | I2C_ISR_TC | I2C_ISR_STOPF | I2C_ISR_NACKF)) != 0) && ((cr1 & I2C_CR1_TRANS2) == I2C_CR1_TRANS2)){
		if(hi2c->State == I2C_STATE_MASTER_BUSY_TX){
			i2c_mastertrans_isr(hi2c);
		}
	}
	isr = sil_rew_mem((uint32_t *)(hi2c->base+TOFF_I2C_ISR));

	/*
	 *  I2C ���졼�ּ����⡼��
	 */
	if(((isr & (I2C_ISR_RXNE | I2C_ISR_TCR | I2C_ISR_TC | I2C_ISR_STOPF | I2C_ISR_NACKF | I2C_ISR_ADDR)) != 0) && ((cr1 & I2C_CR1_RECV1) == I2C_CR1_RECV1)){
		if(hi2c->State == I2C_STATE_SLAVE_BUSY_RX){
			i2c_slavereceiv_isr(hi2c);
		}
	} 
	isr = sil_rew_mem((uint32_t *)(hi2c->base+TOFF_I2C_ISR));

	/*
	 *  I2C �ޥ����������⡼��
	 */
	if(((isr & (I2C_ISR_RXNE | I2C_ISR_TCR | I2C_ISR_TC | I2C_ISR_STOPF | I2C_ISR_NACKF)) != 0) && ((cr1 & I2C_CR1_RECV2) == I2C_CR1_RECV2)){
		if(hi2c->State == I2C_STATE_MASTER_BUSY_RX){
			i2c_masterreceiv_isr(hi2c);
		}
	}

	if(hi2c->State == I2C_STATE_READY && hi2c->Init.semid != 0)
		isig_sem(hi2c->Init.semid);
}

/*
 *  I2C ER����ߥϥ�ɥ�
 */
void
i2c_er_handler(I2C_Handle_t *hi2c)
{
	uint32_t cr1 = sil_rew_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR1));

	/*
	 *  I2C �Х����顼
	 */
	if(((sil_rew_mem((uint32_t *)(hi2c->base+TOFF_I2C_ISR)) & I2C_ISR_BERR) != 0) && ((cr1 & I2C_CR1_ERRIE) != 0)){
		hi2c->ErrorCode |= I2C_ERROR_BERR;
		sil_wrw_mem((uint32_t *)(hi2c->base+TOFF_I2C_ICR), I2C_ICR_BERRCF);
	}

	/*
	 *  I2C �����С����/����������
	 */
	if(((sil_rew_mem((uint32_t *)(hi2c->base+TOFF_I2C_ISR)) & I2C_ISR_OVR) != 0) && ((cr1 & I2C_CR1_ERRIE) != 0)){
		hi2c->ErrorCode |= I2C_ERROR_OVR;
		sil_wrw_mem((uint32_t *)(hi2c->base+TOFF_I2C_ICR), I2C_ICR_OVRCF);
	}

	/*
	 *  I2C �����ӥȥ졼����󡦥������顼
	 */
	if(((sil_rew_mem((uint32_t *)(hi2c->base+TOFF_I2C_ISR)) & I2C_ISR_ARLO) != 0) && ((cr1 & I2C_CR1_ERRIE) != 0)){
		hi2c->ErrorCode |= I2C_ERROR_ARLO;
		sil_wrw_mem((uint32_t *)(hi2c->base+TOFF_I2C_ICR), I2C_ICR_ARLOCF);
	}

	/*
	 *  ���顼������Хå��ؿ��ƤӽФ�
	 */
	if(hi2c->ErrorCode != I2C_ERROR_NONE){
		hi2c->State = I2C_STATE_READY;
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

