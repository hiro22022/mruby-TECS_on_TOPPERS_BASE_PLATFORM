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
 *  @(#) $Id: i2c.c 698 2016-02-10 12:05:16Z roi $
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
#ifdef TOPPERS_STM32F401_NUCLEO
#define GPIO_AFN_I2C2          ((uint8_t)0x09)  /* I2C2 Alternate Function mapping */
#define GPIO_AFN_I2C3          ((uint8_t)0x09)  /* I2C3 Alternate Function mapping */
#define GPIO_I2C3_SDA          TADR_GPIOB_BASE
#define I2C3_SDA               4
#else
#define GPIO_AFN_I2C2          ((uint8_t)0x04)  /* I2C2 Alternate Function mapping */
#define GPIO_AFN_I2C3          ((uint8_t)0x04)  /* I2C3 Alternate Function mapping */
#define GPIO_I2C3_SDA          TADR_GPIOC_BASE
#define I2C3_SDA               9
#endif
#ifdef TOPPERS_STM32F4_DISCOVERY
#define I2C2_SDA               11
#else
#define I2C2_SDA               3
#endif

#define I2C_TIMEOUT_FLAG            35			/* 35 ms */
#define I2C_TIMEOUT_BUSY_FLAG       10000		/* 10 s  */

#define I2C_FREQRANGE(_PCLK_)       ((_PCLK_)/1000000)

#define I2C_RISE_TIME(_FREQ_, _SPEED_)            (((_SPEED_) <= 100000) ? ((_FREQ_) + 1) : ((((_FREQ_) * 300) / 1000) + 1))
#define I2C_SPEED_STANDARD(_PCLK_, _SPEED_)       (((((_PCLK_)/((_SPEED_) << 1)) & I2C_CCR_CCR) < 4)? 4:((_PCLK_) / ((_SPEED_) << 1)))
#define I2C_SPEED_FAST(_PCLK_, _SPEED_, _DCYCLE_) (((_DCYCLE_) == I2C_DUTYCYCLE_2)? ((_PCLK_) / ((_SPEED_) * 3)) : (((_PCLK_) / ((_SPEED_) * 25)) | I2C_DUTYCYCLE_16_9))
#define I2C_SPEED(_PCLK_, _SPEED_, _DCYCLE_)      (((_SPEED_) <= 100000)? (I2C_SPEED_STANDARD((_PCLK_), (_SPEED_))) : \
                                                                  ((I2C_SPEED_FAST((_PCLK_), (_SPEED_), (_DCYCLE_)) & I2C_CCR_CCR) == 0)? 1 : \
                                                                  ((I2C_SPEED_FAST((_PCLK_), (_SPEED_), (_DCYCLE_))) | I2C_CCR_FS))

#define I2C_7BIT_ADD_WRITE(_ADDR_)  ((uint8_t)((_ADDR_) & (~I2C_OAR1_ADD0)))
#define I2C_7BIT_ADD_READ(_ADDR_)   ((uint8_t)((_ADDR_) | I2C_OAR1_ADD0))

#define I2C_10BIT_ADDRESS(_ADDR_)   ((uint8_t)(((_ADDR_) & 0x00FF)))
#define I2C_10BIT_HEADWRITE(_ADDR_) ((uint8_t)(((((_ADDR_) & 0x0300) >> 7) | 0xF0)))
#define I2C_10BIT_HEADREAD(_ADDR_)  ((uint8_t)(((((_ADDR_) & 0x0300) >> 7) | 0xF1)))

#define I2C_MEM_ADD_MSB(_ADDR_)     ((uint8_t)(((_ADDR_) & 0xFF00) >> 8))
#define I2C_MEM_ADD_LSB(_ADDR_)     ((uint8_t)((_ADDR_) & 0x00FF))


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
	(TADR_RCC_BASE+TOFF_RCC_AHB1ENR),  RCC_AHB1ENR_GPIOBEN, RCC_AHB1ENR_GPIOBEN,
	(TADR_RCC_BASE+TOFF_RCC_APB1ENR),  RCC_APB1ENR_I2C2EN,
	(TADR_RCC_BASE+TOFF_RCC_APB1RSTR), RCC_APB1RSTR_I2C2RST,
	TADR_GPIOB_BASE, TADR_GPIOB_BASE,
	10, GPIO_AF4_I2C2, I2C2_SDA, GPIO_AFN_I2C2 },

  {	TADR_I2C3_BASE,
	(TADR_RCC_BASE+TOFF_RCC_AHB1ENR),  RCC_AHB1ENR_GPIOAEN, RCC_AHB1ENR_GPIOBEN,
	(TADR_RCC_BASE+TOFF_RCC_APB1ENR),  RCC_APB1ENR_I2C3EN,
	(TADR_RCC_BASE+TOFF_RCC_APB1RSTR), RCC_APB1RSTR_I2C3RST,
	TADR_GPIOA_BASE, GPIO_I2C3_SDA,
	8, GPIO_AF4_I2C3, I2C3_SDA, GPIO_AFN_I2C3 }
};


/*
 * I2C �ϥ�ɥ�
 */
static I2C_Handle_t I2cHandle[NUM_I2CPORT];

/*
 *  SR1�쥸�������å��Ԥ�
 *  parameter1 hi2c:    I2C�ϥ�ɥ�ؤΥݥ���
 *  parameter2 Flag:    �Ԥ��ե饰
 *  parameter3 Timeout: �Ԥ�����(ms)
 *  return              ���ｪλ����E_OK
 */
static ER
i2c_sr1flagsetwait(I2C_Handle_t *hi2c, uint32_t Flag, uint32_t Timeout)
{
	uint32_t tick = 0;
	uint32_t ctime = Timeout * 1000;

	while((sil_rew_mem((uint32_t *)(hi2c->base+TOFF_I2C_SR1)) & Flag) == 0){
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

    while((sil_rew_mem((uint32_t *)(hi2c->base+TOFF_I2C_SR2)) & I2C_SR2_BUSY) != 0){
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
 * ���ɥ쥹�ϥե饰�����Ԥ�
 *  parameter1 hi2c:    I2C�ϥ�ɥ�ؤΥݥ���
 *  parameter2 Flag:    �Ԥ��ե饰
 *  parameter3 Timeout: �Ԥ�����(ms)
 *  return              ���ｪλ����E_OK
 */
static ER
i2c_addrflagwait(I2C_Handle_t *hi2c, uint32_t Flag, uint32_t Timeout)
{
	uint32_t tick = 0;
	uint32_t ctime = Timeout * 1000;

	while((sil_rew_mem((uint32_t *)(hi2c->base+TOFF_I2C_SR1)) & Flag) == 0){
		/*
		 *  �����Υ�å��ե����Ƚ��
		 */
		if((sil_rew_mem((uint32_t *)(hi2c->base+TOFF_I2C_SR1)) & I2C_SR1_AF) != 0){
			sil_orw_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR1), I2C_CR1_STOP);
			sil_wrw_mem((uint32_t *)(hi2c->base+TOFF_I2C_SR1), ~I2C_SR1_AF);
			hi2c->ErrorCode = I2C_ERROR_AF;
			hi2c->status= I2C_STATUS_READY;
			return E_SYS;
		}
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
 *  ���ɥ쥹�ե饰���ꥢ
 *  parameter1 hi2c:   I2C�ϥ�ɥ�ؤΥݥ���
 */
static void
i2c_clear_addr(I2C_Handle_t *hi2c)
{
	volatile uint32_t tmp;

	tmp = sil_rew_mem((uint32_t *)(hi2c->base+TOFF_I2C_SR1));
	tmp = sil_rew_mem((uint32_t *)(hi2c->base+TOFF_I2C_SR2));
	(void)(tmp);
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
	return E_OK;
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

	/*
	 *  �����Υ�å��������
	 */
	sil_orw_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR1), I2C_CR1_ACK);

	/*
	 *  ������������������Ԥ�
	 */
	sil_orw_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR1), I2C_CR1_START);
	if(i2c_sr1flagsetwait(hi2c, I2C_SR1_SB, Timeout) != E_OK){
		return E_TMOUT;
	}

	/*
	 *  �ǥХ������ɥ쥹����������Ԥ�
	 */
	if(hi2c->Init.AddressingMode == I2C_ADDRESSINGMODE_7BIT){
		if(MemAddSize == 0)	/* ���ꥢ�ɥ쥹�����ꤷ�ʤ���硢�������� */
			sil_wrw_mem((uint32_t *)(hi2c->base+TOFF_I2C_DR), I2C_7BIT_ADD_READ(DevAddress));
		else
			sil_wrw_mem((uint32_t *)(hi2c->base+TOFF_I2C_DR), I2C_7BIT_ADD_WRITE(DevAddress));
	}
	else{	/* ���ꥢ�ɥ쥹����ʤ� */
		/*
		 *  10�ӥåȥإå�����������Ԥ�
		 */
		sil_wrw_mem((uint32_t *)(hi2c->base+TOFF_I2C_DR), I2C_10BIT_HEADWRITE(DevAddress));
		if((ercd = i2c_addrflagwait(hi2c, I2C_SR1_ADD10, Timeout)) != E_OK){
			return ercd;
		}

		/*
		 *  10�ӥåȥ��ɥ쥹����������Ԥ�
		 */
		sil_wrw_mem((uint32_t *)(hi2c->base+TOFF_I2C_DR), I2C_10BIT_ADDRESS(DevAddress));
		if((ercd = i2c_addrflagwait(hi2c, I2C_SR1_ADDR, Timeout)) != E_OK){
			return ercd;
		}

		/*
		 *  ���ɥ쥹�ե饰���ꥢ
		 */
		i2c_clear_addr(hi2c);

 		/*
		 *  �ꥹ����������������Ԥ�
		 */
		sil_orw_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR1), I2C_CR1_START);
		if(i2c_sr1flagsetwait(hi2c, I2C_SR1_SB, Timeout) != E_OK){
			return E_TMOUT;
	    }

	   	/*
		 *  ���졼�֥��ɥ쥹������⡼�ɤ�����
		 */
		sil_wrw_mem((uint32_t *)(hi2c->base+TOFF_I2C_DR), I2C_10BIT_HEADREAD(DevAddress));
	}

	/*
	 *  ���ɥ쥹�ե饰�����Ԥ�
	 */
	if((ercd = i2c_addrflagwait(hi2c, I2C_SR1_ADDR, Timeout)) != E_OK){
		return ercd;
	}

	/*
	 *  ���ꥢ�ɥ쥹����ʤ��ʤ����ｪλ
	 */
	if(MemAddSize == 0)
		return E_OK;

	/*
	 *  ���ɥ쥹�ե饰���ꥢ
	 */
	i2c_clear_addr(hi2c);

	/*
	 *  TXE�ե饰����ޤ��Ԥ�
	 */
	if(i2c_sr1flagsetwait(hi2c, I2C_SR1_TXE, Timeout) != E_OK){
		return E_TMOUT;
	}

	/*
	 *  ���ꥢ�ɥ쥹����������Ԥ�
	 */
	if(MemAddSize == I2C_MEMADD_SIZE_8BIT){
		sil_wrw_mem((uint32_t *)(hi2c->base+TOFF_I2C_DR), I2C_MEM_ADD_LSB(MemAddress));
	}
	else{	/* 16�ӥåȥ����� */
		sil_wrw_mem((uint32_t *)(hi2c->base+TOFF_I2C_DR), I2C_MEM_ADD_MSB(MemAddress));
		if(i2c_sr1flagsetwait(hi2c, I2C_SR1_TXE, Timeout) != E_OK){
			return E_TMOUT;
		}
		sil_wrw_mem((uint32_t *)(hi2c->base+TOFF_I2C_DR), I2C_MEM_ADD_LSB(MemAddress));
	}

	/*
	 *  TXE�ե饰����ޤ��Ԥ�
	 */
	if(i2c_sr1flagsetwait(hi2c, I2C_SR1_TXE, Timeout) != E_OK){
		return E_TMOUT;
	}

	/*
	 *  �ꥹ����������������Ԥ�
	 */
	sil_orw_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR1), I2C_CR1_START);
	if(i2c_sr1flagsetwait(hi2c, I2C_SR1_SB, Timeout) != E_OK){
		return E_TMOUT;
	}

	/*
	 *  �ǥХ������ɥ쥹������⡼�ɤ�����(7�ӥåȤΤ�)
	 */
	sil_wrw_mem((uint32_t *)(hi2c->base+TOFF_I2C_DR), I2C_7BIT_ADD_READ(DevAddress));

	/*
	 *  ���ɥ쥹�ե饰�����Ԥ�
	 */
	return i2c_addrflagwait(hi2c, I2C_SR1_ADDR, Timeout);
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

	/*
	 *  ������������������Ԥ�
	 */
	sil_orw_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR1), I2C_CR1_START);
	if(i2c_sr1flagsetwait(hi2c, I2C_SR1_SB, Timeout) != E_OK){
		return E_TMOUT;
	}

	/*
	 *  �ǥХ������ɥ쥹����������Ԥ�
	 */
	if(hi2c->Init.AddressingMode == I2C_ADDRESSINGMODE_7BIT){
		sil_wrw_mem((uint32_t *)(hi2c->base+TOFF_I2C_DR), I2C_7BIT_ADD_WRITE(DevAddress));
	}
	else{
		sil_wrw_mem((uint32_t *)(hi2c->base+TOFF_I2C_DR), I2C_10BIT_HEADWRITE(DevAddress));
		if((ercd = i2c_addrflagwait(hi2c, I2C_SR1_ADD10, Timeout)) != E_OK){
			return ercd;
		}

		/*
		 *  10�ӥåȥ��ɥ쥹����������Ԥ�
		 */
		sil_wrw_mem((uint32_t *)(hi2c->base+TOFF_I2C_DR), I2C_10BIT_ADDRESS(DevAddress));
	}

	/*
	 *  ���ɥ쥹�ե饰�����Ԥ�
	 */
	if((ercd = i2c_addrflagwait(hi2c, I2C_SR1_ADDR, Timeout)) != E_OK){
		return ercd;
	}

	/*
	 *  ���ɥ쥹�ե饰���ꥢ
	 */
	i2c_clear_addr(hi2c);

	/*
	 *  ���ꥢ�ɥ쥹����ʤ��ʤ����ｪλ
	 */
	if(MemAddSize == 0)
		return E_OK;

	/*
	 *  TXE�ե饰����ޤ��Ԥ�
	 */
	if(i2c_sr1flagsetwait(hi2c, I2C_SR1_TXE, Timeout) != E_OK){
		return E_TMOUT;
	}

	/*
	 *  ���ꥢ�ɥ쥹����������Ԥ�
	 */
	if(MemAddSize == I2C_MEMADD_SIZE_8BIT){
		sil_wrw_mem((uint32_t *)(hi2c->base+TOFF_I2C_DR), I2C_MEM_ADD_LSB(MemAddress));
	}
	else{	/* 16�ӥåȥ����� */
		sil_wrw_mem((uint32_t *)(hi2c->base+TOFF_I2C_DR), I2C_MEM_ADD_MSB(MemAddress));
		if(i2c_sr1flagsetwait(hi2c, I2C_SR1_TXE, Timeout) != E_OK){
			return E_TMOUT;
		}
		sil_wrw_mem((uint32_t *)(hi2c->base+TOFF_I2C_DR), I2C_MEM_ADD_LSB(MemAddress));
	}
	return E_OK;
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
	uint32_t freqrange = 0;
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
	 * ���ܼ��ȿ�(PCLK1)����󥸤����
	 */
	freqrange = I2C_FREQRANGE(SysFrePCLK1);

	/*
	 *  I2C���ȿ��������
	 */
	sil_wrw_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR2), freqrange);

	/*
	 *  I2C�饤����������
	 */
	sil_wrw_mem((uint32_t *)(hi2c->base+TOFF_I2C_TRISE), I2C_RISE_TIME(freqrange, hi2c->Init.ClockSpeed));

	/*
	 *  I2C����å����ԡ�������
	 */
	sil_wrw_mem((uint32_t *)(hi2c->base+TOFF_I2C_CCR), I2C_SPEED(SysFrePCLK1, hi2c->Init.ClockSpeed, hi2c->Init.DutyCycle));

	/*
	 *  I2C CR1����
	 */
	sil_wrw_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR1), (hi2c->Init.GeneralCallMode | hi2c->Init.NoStretchMode));

	/*
	 *  I2C�ǥХ������ɥ쥹������
	 */
	sil_wrw_mem((uint32_t *)(hi2c->base+TOFF_I2C_OAR1), (hi2c->Init.AddressingMode | hi2c->Init.OwnAddress1));

	/*
	 *  I2C�ǥХ������ɥ쥹������
	 */
	sil_wrw_mem((uint32_t *)(hi2c->base+TOFF_I2C_OAR2), (hi2c->Init.DualAddressMode | hi2c->Init.OwnAddress2));

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

	if(hi2c->status != I2C_STATUS_READY || i2c_busreadywait(hi2c, I2C_TIMEOUT_BUSY_FLAG) != E_OK){
		if(hi2c->Init.semlock != 0)
			sig_sem(hi2c->Init.semlock);
		return E_OBJ;
	}

	/*
	 *  Acknowledge/PEC Position�ػ�
	 */
	sil_andw_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR1), I2C_CR1_POS);

    hi2c->status = I2C_STATUS_BUSY_RX;
    hi2c->ErrorCode = I2C_ERROR_NONE;

    hi2c->pBuffPtr = pData;
    hi2c->XferSize = Size;
    hi2c->XferCount = Size;

    /*
	 *  ���ɥ쥹�������Υ�å�����
	 */
	sil_orw_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR1), I2C_CR1_ACK);

    /*
	 *  �ɤ߽Ф�����ߵ���
	 */
	sil_orw_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR2), (I2C_CR2_ITEVTEN | I2C_CR2_ITBUFEN | I2C_CR2_ITERREN));
	ercd = i2c_transwait(hi2c, 500);

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

	if(hi2c->status != I2C_STATUS_READY || i2c_busreadywait(hi2c, I2C_TIMEOUT_BUSY_FLAG) != E_OK){
		if(hi2c->Init.semlock != 0)
			sig_sem(hi2c->Init.semlock);
		return E_OBJ;
	}

	/*
	 *  Acknowledge/PEC Position�ػ�
	 */
	sil_andw_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR1), I2C_CR1_POS);

    hi2c->status = I2C_STATUS_BUSY_TX;
    hi2c->ErrorCode = I2C_ERROR_NONE;

    hi2c->pBuffPtr = pData;
    hi2c->XferSize = Size;
    hi2c->XferCount = Size;

    /*
	 *  ���ɥ쥹�������Υ�å�����
	 */
	sil_orw_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR1), I2C_CR1_ACK);

    /*
	 *  �񤭹��߳���ߵ���
	 */
	sil_orw_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR2), (I2C_CR2_ITEVTEN | I2C_CR2_ITBUFEN | I2C_CR2_ITERREN));
	ercd = i2c_transwait(hi2c, 500);

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

	/*
	 *  Acknowledge/PEC Position�ػ�
	 */
	sil_andw_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR1), I2C_CR1_POS);

    hi2c->status = I2C_STATUS_BUSY_RX;
    hi2c->ErrorCode = I2C_ERROR_NONE;

    hi2c->pBuffPtr = pData;
    hi2c->XferSize = Size;
    hi2c->XferCount = Size;

	/* ���졼�֥��ɥ쥹���ɤ߽Ф����ɥ쥹������ */
	if((ercd = i2c_masterreadaddress(hi2c, DevAddress, MemAddress, MemAddSize, I2C_TIMEOUT_FLAG)) != E_OK){
		if(hi2c->Init.semlock != 0)
			sig_sem(hi2c->Init.semlock);
        return ercd;
	}

	if(hi2c->XferCount == 1){
		sil_andw_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR1), I2C_CR1_ACK);
		i2c_clear_addr(hi2c);
		sil_orw_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR1), I2C_CR1_STOP);
	}
	else if(hi2c->XferCount == 2){
		sil_andw_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR1), I2C_CR1_ACK);
		sil_orw_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR1), I2C_CR1_POS);
		i2c_clear_addr(hi2c);
	}
	else{
		sil_orw_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR1), I2C_CR1_ACK);
		i2c_clear_addr(hi2c);
	}

    /*
	 *  �ɤ߽Ф�����ߵ���
	 */
	sil_orw_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR2), (I2C_CR2_ITEVTEN | I2C_CR2_ITBUFEN | I2C_CR2_ITERREN));
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

	/*
	 *  Acknowledge/PEC Position�ػ�
	 */
	sil_andw_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR1), I2C_CR1_POS);

	hi2c->status = I2C_STATUS_BUSY_TX;
	hi2c->ErrorCode = I2C_ERROR_NONE;

	hi2c->pBuffPtr = pData;
	hi2c->XferSize = Size;
	hi2c->XferCount = Size;

	/* ���졼�֥��ɥ쥹�Ƚ���ߥ��ɥ쥹������ */
	if((ercd = i2c_masterwriteaddress(hi2c, DevAddress, MemAddress, MemAddSize, I2C_TIMEOUT_FLAG)) != E_OK){
		if(hi2c->Init.semlock != 0)
			sig_sem(hi2c->Init.semlock);
		return ercd;
	}

    /*
	 *  �񤭹��߳���ߵ���
	 */
	sil_orw_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR2), (I2C_CR2_ITEVTEN | I2C_CR2_ITBUFEN | I2C_CR2_ITERREN));
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
	uint32_t tmp1 = 0, tmp2 = 0;
	uint32_t tick = 0;

	if(hi2c->status == I2C_STATUS_READY){
		if((sil_rew_mem((uint32_t *)(hi2c->base+TOFF_I2C_SR2)) & I2C_SR2_BUSY) != 0)
			return E_OBJ;

		/*
		 *  ����������å�
		 */
		if(hi2c->Init.semlock != 0)
			wai_sem(hi2c->Init.semlock);

		sil_andw_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR1), I2C_CR1_POS);
		hi2c->status    = I2C_STATUS_BUSY;
		hi2c->ErrorCode = I2C_ERROR_NONE;

		do{
			/* START���� */
			sil_orw_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR1), I2C_CR1_START);

			/* START BIT�Ԥ� */
			if(i2c_sr1flagsetwait(hi2c, I2C_SR1_SB, Timeout) != E_OK){
				/*
				 *  ��å����
				 */
				if(hi2c->Init.semlock != 0)
					sig_sem(hi2c->Init.semlock);
				return E_TMOUT;
			}

			/* ���졼�֥��ɥ쥹���� */
			sil_wrw_mem((uint32_t *)(hi2c->base+TOFF_I2C_DR), I2C_7BIT_ADD_WRITE(DevAddress));

			/* ���ɥ쥹�����������Υ�å��Ԥ� */
			tmp1 = sil_rew_mem((uint32_t *)(hi2c->base+TOFF_I2C_SR1)) & (I2C_SR1_ADDR | I2C_SR1_AF);
			tmp2 = hi2c->status;
			while(tmp1 == 0 && tmp2 != I2C_STATUS_TIMEOUT){
				if(tick > Timeout){
					hi2c->status = I2C_STATUS_TIMEOUT;
				}
				tmp1 = sil_rew_mem((uint32_t *)(hi2c->base+TOFF_I2C_SR1)) & (I2C_SR1_ADDR | I2C_SR1_AF);
				tmp2 = hi2c->status;
				dly_tsk(1);
				tick++;
			}
			hi2c->status = I2C_STATUS_READY;

			/* ���ɥ쥹�����ե饰�����å� */
			if((sil_rew_mem((uint32_t *)(hi2c->base+TOFF_I2C_SR1)) & I2C_SR1_ADDR) != 0){
				/* ���ȥå����� */
				sil_orw_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR1), I2C_CR1_STOP);

				/* ���ɥ쥹�����ե饰���ꥢ */
				tmp1 = sil_rew_mem((uint32_t *)(hi2c->base+TOFF_I2C_SR1));
				tmp1 = sil_rew_mem((uint32_t *)(hi2c->base+TOFF_I2C_SR2));

				/* ��ǥ��Ԥ� */
				if(i2c_busreadywait(hi2c, I2C_TIMEOUT_BUSY_FLAG) != E_OK){
					/*
					 *  ��å����
					 */
					if(hi2c->Init.semlock != 0)
						sig_sem(hi2c->Init.semlock);
					return E_TMOUT;
				}

				hi2c->status = I2C_STATUS_READY;
				/*
				 *  ��å����
				 */
				if(hi2c->Init.semlock != 0)
					sig_sem(hi2c->Init.semlock);
				return E_OK;
			}
			else{	/* �����Υ�å����� */
				/* ���ȥå����� */
				sil_orw_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR1), I2C_CR1_STOP);

				/* ���ɥ쥹�����ե饰�ꥻ�å� */
				sil_andw_mem((uint32_t *)(hi2c->base+TOFF_I2C_SR1), I2C_SR1_ADDR);

				/* ��ǥ��Ԥ� */
				if(i2c_busreadywait(hi2c, I2C_TIMEOUT_BUSY_FLAG) != E_OK){
					/*
					 *  ��å����
					 */
					if(hi2c->Init.semlock != 0)
						sig_sem(hi2c->Init.semlock);
					return E_TMOUT;
				}
			}
		}while(I2C_Trials++ < Trials);

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
 *  I2C���Х�������
 *  parameter1 hi2c:       I2C�ϥ�ɥ�ؤΥݥ���
 */
static void i2c_transmit_byte(I2C_Handle_t *hi2c)
{
	if(hi2c->XferCount != 0){
		sil_wrw_mem((uint32_t *)(hi2c->base+TOFF_I2C_DR), (uint32_t)(*hi2c->pBuffPtr++));
		hi2c->XferCount--;
	}
}

/*
 *  I2C���Х��ȼ���
 *  parameter1 hi2c:       I2C�ϥ�ɥ�ؤΥݥ���
 */
static void i2c_receive_byte(I2C_Handle_t *hi2c)
{
	if(hi2c->XferCount != 0){
		(*hi2c->pBuffPtr++) = (uint8_t)sil_rew_mem((uint32_t *)(hi2c->base+TOFF_I2C_DR));
		hi2c->XferCount--;
	}
}

/*
 *  I2C�ޥ��������������(BTF�����)
 */
static void
i2c_mastertrans_BTF(I2C_Handle_t *hi2c)
{
	if(hi2c->XferCount != 0){	/* �ǡ������� */
		sil_wrw_mem((uint32_t *)(hi2c->base+TOFF_I2C_DR), (uint32_t)(*hi2c->pBuffPtr++));
		hi2c->XferCount--;
	}
	else{	/* ������λ */
		sil_andw_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR2), (I2C_CR2_ITEVTEN | I2C_CR2_ITBUFEN | I2C_CR2_ITERREN));
		sil_orw_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR1), I2C_CR1_STOP);

		hi2c->status = I2C_STATUS_READY;
		if(hi2c->writecallback != NULL)
			hi2c->writecallback(hi2c);
	}
}


/*
 *  I2C�ޥ��������������(TXE�����)
 */
static void
i2C_MasterTransmit_TXE(I2C_Handle_t *hi2c)
{
	/*
	 *  ���Х�������
	 */
	sil_wrw_mem((uint32_t *)(hi2c->base+TOFF_I2C_DR), (uint32_t)(*hi2c->pBuffPtr++));
	hi2c->XferCount--;

	if(hi2c->XferCount == 0){	/* ��� */
		sil_andw_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR2), I2C_CR2_ITBUFEN);
	}
}

/*
 *  I2C�ޥ��������������(RXNE�����)
 */
static void
i2c_masterreceiv_RXNE(I2C_Handle_t *hi2c)
{
	uint32_t tmp = 0;

	tmp = hi2c->XferCount;
	if(tmp > 3){	/* �Ĥ�4�Х��Ȱʾ� */
		(*hi2c->pBuffPtr++) = (uint8_t)sil_rew_mem((uint32_t *)(hi2c->base+TOFF_I2C_DR));
		hi2c->XferCount--;
	}
	else if(tmp == 2 || tmp == 3){	/* �Ĥ�2�ޤ���3�Х��� */
		sil_andw_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR2), I2C_CR2_ITBUFEN);
	}
	else{	/* ������λ */
		sil_andw_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR2), (I2C_CR2_ITEVTEN | I2C_CR2_ITBUFEN | I2C_CR2_ITERREN));

		(*hi2c->pBuffPtr++) = (uint8_t)sil_rew_mem((uint32_t *)(hi2c->base+TOFF_I2C_DR));
		hi2c->XferCount--;

		hi2c->status = I2C_STATUS_READY;
		if(hi2c->readcallback != NULL)
			hi2c->readcallback(hi2c);
	}
}

/*
 *  I2C�ޥ��������������(BTF�����)
 */
static void
i2c_masterreceiv_BTF(I2C_Handle_t *hi2c)
{
	if(hi2c->XferCount == 3){		/* �Ĥꥫ�����3�Х��� */
		sil_andw_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR1), I2C_CR1_ACK);
		(*hi2c->pBuffPtr++) = (uint8_t)sil_rew_mem((uint32_t *)(hi2c->base+TOFF_I2C_DR));
		hi2c->XferCount--;
	}
	else if(hi2c->XferCount == 2){	/* �Ĥꥫ�����2�Х��� */
		sil_orw_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR1), I2C_CR1_STOP);
		(*hi2c->pBuffPtr++) = (uint8_t)sil_rew_mem((uint32_t *)(hi2c->base+TOFF_I2C_DR));
		hi2c->XferCount--;
		(*hi2c->pBuffPtr++) = (uint8_t)sil_rew_mem((uint32_t *)(hi2c->base+TOFF_I2C_DR));
		hi2c->XferCount--;

		/*
		 *  ��������
		 */
		sil_andw_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR2), (I2C_CR2_ITEVTEN | I2C_CR2_ITERREN));

		hi2c->status = I2C_STATUS_READY;
		if(hi2c->readcallback != NULL)
			hi2c->readcallback(hi2c);
	}
	else{
		(*hi2c->pBuffPtr++) = (uint8_t)sil_rew_mem((uint32_t *)(hi2c->base+TOFF_I2C_DR));
		hi2c->XferCount--;
	}
}

/*
 *  I2C EV����ߥϥ�ɥ�
 */
void
i2c_ev_handler(I2C_Handle_t *hi2c)
{
	uint32_t sr1 = sil_rew_mem((uint32_t *)(hi2c->base+TOFF_I2C_SR1));
	uint32_t sr2 = sil_rew_mem((uint32_t *)(hi2c->base+TOFF_I2C_SR2));
	uint32_t cr2 = sil_rew_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR2));
	volatile uint32_t tmp;

	/*
	 *  �ޥ������⡼��
	 */
	if((sr2 & I2C_SR2_MSL) != 0){
		if((sr2 & I2C_SR2_TRA) != 0){	/* �����⡼�� */
			if(((sr1 & I2C_SR1_TXE) != 0) && ((cr2 & I2C_CR2_ITBUFEN) != 0) && ((sr1 & I2C_SR1_BTF) == 0)){
				i2C_MasterTransmit_TXE(hi2c);
			}
			else if(((sr1 & I2C_SR1_BTF) != 0) && ((cr2 & I2C_CR2_ITEVTEN) != 0)){
				i2c_mastertrans_BTF(hi2c);
			}
		}
		else{							/* �����⡼�� */
			if(((sr1 & I2C_SR1_RXNE) != 0) && ((cr2 & I2C_CR2_ITBUFEN) != 0) && ((sr1 & I2C_SR1_BTF) == 0)){
				i2c_masterreceiv_RXNE(hi2c);
			}
 			else if(((sr1 & I2C_SR1_BTF) != 0) && ((cr2 & I2C_CR2_ITEVTEN) != 0)){
				i2c_masterreceiv_BTF(hi2c);
			}
		}
		if(hi2c->status == I2C_STATUS_READY && hi2c->Init.semid != 0)
			isig_sem(hi2c->Init.semid);
	}
	/*
	 *  ���졼�֥⡼��
	 */
	else{
		if(((sr1 & I2C_SR1_ADDR) != 0) && ((cr2 & I2C_CR2_ITEVTEN) != 0)){	/* ADDR�ե饰 */
			i2c_clear_addr(hi2c);
		}
		else if(((sr1 & I2C_SR1_STOPF) != 0) && ((cr2 & I2C_CR2_ITEVTEN) != 0)){	/* STOP�ե饰 */
			sil_andw_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR2), (I2C_CR2_ITEVTEN | I2C_CR2_ITBUFEN | I2C_CR2_ITERREN));
			/*
			 * STOP�ե饰���ꥢ
			 */
			tmp = sil_rew_mem((uint32_t *)(hi2c->base+TOFF_I2C_SR1));
			sil_orw_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR1), I2C_CR1_PE);
			(void)(tmp);

		    /*
			 *  ���ɥ쥹�������Υ�å��ػ�
			 */
			sil_andw_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR1), I2C_CR1_ACK);

			hi2c->status = I2C_STATUS_READY;
			if(hi2c->readcallback != NULL)
				hi2c->readcallback(hi2c);
		}
		else if((sr2 & I2C_SR2_TRA) != 0){	/* ���졼������ */
			if(((sr1 & I2C_SR1_TXE) != 0) && ((cr2 & I2C_CR2_ITBUFEN) != 0) && ((sr1 & I2C_SR1_BTF) == 0)){
				i2c_transmit_byte(hi2c);
			}
			else if(((sr1 & I2C_SR1_BTF) != 0) && ((cr2 & I2C_CR2_ITEVTEN) != 0)){
				i2c_transmit_byte(hi2c);
			}
		}
		else{								/* ���졼�ּ��� */
			if(((sr1 & I2C_SR1_RXNE) != 0) && ((cr2 & I2C_CR2_ITBUFEN) != 0) && ((sr1 & I2C_SR1_BTF) == 0)){
				i2c_receive_byte(hi2c);
			}
			else if(((sr1 & I2C_SR1_BTF) != 0) && ((cr2 & I2C_CR2_ITEVTEN) != 0)){
				i2c_receive_byte(hi2c);
			}
		}
	}
}

/*
 *  I2C ER����ߥϥ�ɥ�
 */
void
i2c_er_handler(I2C_Handle_t *hi2c)
{
	uint32_t sr1 = sil_rew_mem((uint32_t *)(hi2c->base+TOFF_I2C_SR1));
	uint32_t cr2 = sil_rew_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR2));

	/*
	 *  I2C �Х����顼
	 */
	if(((sr1 & I2C_SR1_BERR) != 0) && ((cr2 & I2C_CR2_ITERREN) != 0)){
		hi2c->ErrorCode |= I2C_ERROR_BERR;
		sil_wrw_mem((uint32_t *)(hi2c->base+TOFF_I2C_SR1), ~I2C_SR1_BERR);
	}
	sr1 = sil_rew_mem((uint32_t *)(hi2c->base+TOFF_I2C_SR1));

	/*
	 *  I2C �����ӥơ�����󡦥������顼
	 */
	if(((sr1 & I2C_SR1_ARLO) != 0) && ((cr2 & I2C_CR2_ITERREN) != 0)){
		hi2c->ErrorCode |= I2C_ERROR_ARLO;
		sil_wrw_mem((uint32_t *)(hi2c->base+TOFF_I2C_SR1), ~I2C_SR1_ARLO);
	}
	sr1 = sil_rew_mem((uint32_t *)(hi2c->base+TOFF_I2C_SR1));

	/*
	 *  I2C �����Υ�å����ե����롦���顼
	 */
	if(((sr1 & I2C_SR1_AF) != 0) && ((cr2 & I2C_CR2_ITERREN) != 0)){
		uint32_t sr2 = sil_rew_mem((uint32_t *)(hi2c->base+TOFF_I2C_SR2));
		if(((sr2 & I2C_SR2_MSL) == 0) && (hi2c->XferCount == 0) && (hi2c->status == I2C_STATUS_BUSY_TX)){
			/* Disable EVT, BUF and ERR interrupt */
			sil_andw_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR2), (I2C_CR2_ITEVTEN | I2C_CR2_ITBUFEN | I2C_CR2_ITERREN));
			/* Clear AF flag */
			sil_wrw_mem((uint32_t *)(hi2c->base+TOFF_I2C_SR1), ~I2C_SR1_AF);
			/* Disable Acknowledge */
			sil_andw_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR1), I2C_CR1_ACK);
			hi2c->status = I2C_STATUS_READY;
			if(hi2c->writecallback != NULL)
				hi2c->writecallback(hi2c);
		}
		else{
			hi2c->ErrorCode |= I2C_ERROR_AF;
			sil_wrw_mem((uint32_t *)(hi2c->base+TOFF_I2C_SR1), ~I2C_SR1_AF);
		}
	}
	sr1 = sil_rew_mem((uint32_t *)(hi2c->base+TOFF_I2C_SR1));

	/*
	 *  I2C �����С����/���������󡦥��顼
	 */
	if(((sr1 & I2C_SR1_OVR) != 0/*tmp1 == SET*/) && ((cr2 & I2C_CR2_ITERREN) != 0)){
		hi2c->ErrorCode |= I2C_ERROR_OVR;
		sil_wrw_mem((uint32_t *)(hi2c->base+TOFF_I2C_SR1), ~I2C_SR1_OVR);
	}

	if(hi2c->ErrorCode != I2C_ERROR_NONE){
		hi2c->status = I2C_STATUS_READY;

		/*
		 *  Acknowledge/PEC Position�ػ�
		 */
		sil_andw_mem((uint32_t *)(hi2c->base+TOFF_I2C_CR1), I2C_CR1_POS);

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

