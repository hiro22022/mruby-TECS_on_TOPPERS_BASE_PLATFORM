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
 *  @(#) $Id: dfsdm.c 698 2017-03-20 21:50:31Z roi $
 */
/*
 * 
 *  DFSDM�ɥ饤�дؿ���
 *
 */
#include <kernel.h>
#include <t_syslog.h>
#include <t_stdlib.h>
#include "syssvc/serial.h"
#include "syssvc/syslog.h"
#include <target_syssvc.h>

#include "device.h"
#include "dfsdm.h"
#include "wm8994.h"

#define sil_orw_mem(a, b)		sil_wrw_mem((a), sil_rew_mem(a) | (b))
#define sil_andw_mem(a, b)		sil_wrw_mem((a), sil_rew_mem(a) & ~(b))
#define sil_modw_mem(a, b, c)	sil_wrw_mem((a), (sil_rew_mem(a) & (~b)) | (c))
#define get_channelno(b)        (((b)-TADR_DFSDM1_BASE) / 0x20)

/*
 *  SAI�ݡ������
 */
#define SAI1_PORTID           1
#define SAI2_PORTID           2

/*
 *  �����ॢ���Ȼ������
 */ 
#define HAL_MAX_DELAY           0xFFFFFFFFU
#define PLLI2S_TIMEOUT_VALUE    100			/* Timeout value fixed to 100 ms  */
#define DFSDM_CKAB_TIMEOUT      5000

/*
 *  ����ͥ��������
 */
#define DFSDM_LSB_MASK          0x0000FFFF
#define DFSDM1_CHANNEL_NUMBER   8

/*
 *  ����ͥ����
 */
static volatile uint32_t       Dfsdm1ChannelCounter = 0;
static DFSDM_Channel_Handle_t* Dfsdm1ChannelHandle[DFSDM1_CHANNEL_NUMBER] = {NULL};


/*
 *  DFSDM����å�����ե����졼�����
 *  parameter1  port:SAI PORTID
 *  parameter2  AudioFreq: Audio���ȿ�
 *  parameter3  Params: ����ѥ�᡼���ݥ���
 */
void
dfsdm_clockconfig(ID port, uint32_t AudioFreq)
{
	uint32_t PLLI2SN, PLLI2SQ, PLLI2SDivQ;
	uint32_t tick = 0;

	/*
	 *  AUDIO���ȿ�����PLL�����ͤμ���
	 */
	if((AudioFreq == AUDIO_FREQUENCY_11K) || (AudioFreq == AUDIO_FREQUENCY_22K) || (AudioFreq == AUDIO_FREQUENCY_44K)){
		/* Configure PLLI2S prescalers */
		/* PLLI2S_VCO: VCO_429M
		I2S_CLK(first level) = PLLI2S_VCO/PLLI2SQ = 429/2 = 214.5 Mhz
		I2S_CLK_x = I2S_CLK(first level)/PLLI2SDIVQ = 214.5/19 = 11.289 Mhz */
		PLLI2SN = 429;
		PLLI2SQ = 2;
		PLLI2SDivQ = 19;
	}
	else /* AUDIO_FREQUENCY_8K, AUDIO_FREQUENCY_16K, AUDIO_FREQUENCY_48K, AUDIO_FREQUENCY_96K */{
		/* I2S clock config
		PLLI2S_VCO: VCO_344M
		I2S_CLK(first level) = PLLI2S_VCO/PLLI2SQ = 344/7 = 49.142 Mhz
		I2S_CLK_x = I2S_CLK(first level)/PLLI2SDIVQ = 49.142/1 = 49.142 Mhz */
		PLLI2SN = 344;
		PLLI2SQ = 7;
		PLLI2SDivQ = 1;
	}

	/*
	 *  SAI1/2����å�����
	 */
	if(port == SAI1_PORTID){
		sil_modw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_DCKCFGR1), RCC_DCKCFGR1_SAI1SEL, RCC_DCKCFGR1_SAI1SEL_0);
	}
	else{
		sil_modw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_DCKCFGR1), RCC_DCKCFGR1_SAI2SEL, RCC_DCKCFGR1_SAI2SEL_0);
	}

	/*
	 *  PLLI2S�ǥ����֥�
	 */
	sil_andw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CR), RCC_CR_PLLI2SON);

    /*
	 *  PLLI2S����Ԥ�
	 */
	tick = 0;
	while((sil_rew_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CR)) & RCC_CR_PLLI2SRDY) != 0){
		if(tick > PLLI2S_TIMEOUT_VALUE){
			return;		/* �����ॢ���ȥ��顼 */
		}
		dly_tsk(1);
		tick++;
	}

    /*
	 *  PLLI2SSN/PLLI2SN������
	 */
	sil_modw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_PLLI2SCFGR), (RCC_PLLI2SCFGR_PLLI2SN | RCC_PLLI2SCFGR_PLLI2SQ), ((PLLI2SN << 6) | (PLLI2SQ << 24)));

	/*
	 *  PLLI2SDIVQ����
	 */
	sil_modw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_DCKCFGR1), RCC_DCKCFGR1_PLLI2SDIVQ, (PLLI2SDivQ-1));

	/*
	 *  PLLI2S���͡��֥�
	 */
	sil_orw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CR), RCC_CR_PLLI2SON);

    /*
	 *  PLLI2S�Ƴ��Ԥ�
	 */
	tick = 0;
	while((sil_rew_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CR)) & RCC_CR_PLLI2SRDY) == 0){
		if(tick > PLLI2S_TIMEOUT_VALUE){
			return;		/* �����ॢ���ȥ��顼 */
		}
		dly_tsk(1);
		tick++;
	}
}

/*
 *  DFSDM����ͥ�����
 *  parameter1 hdfsc  DFSDM����ͥ�ϥ�ɥ�ؤΥݥ���
 *  return            ���ｪλ����E_OK
 */
ER dfsdm_channel_init(DFSDM_Channel_Handle_t *hdfsc)
{
	/*
	 *  �ѥ�᡼�������å�
	 */
	if(hdfsc == NULL)
		return E_PAR;
	if((hdfsc->Init.OutClockActivation & DFSDM_MASK) != 0)
		return E_PAR;
	if(hdfsc->Init.InputMultiplexer != DFSDM_CHANNEL_EXTERNAL_INPUTS &&
		hdfsc->Init.InputMultiplexer != DFSDM_CHANNEL_INTERNAL_REGISTER)
		return E_PAR;
	if(hdfsc->Init.InputDataPacking != DFSDM_CHANNEL_STANDARD_MODE &&
		hdfsc->Init.InputDataPacking != DFSDM_CHANNEL_INTERLEAVED_MODE &&
			hdfsc->Init.InputDataPacking != DFSDM_CHANNEL_DUAL_MODE)
		return E_PAR;
	if(hdfsc->Init.InputPins != DFSDM_CHANNEL_SAME_CHANNEL_PINS && 
		hdfsc->Init.InputPins != DFSDM_CHANNEL_FOLLOWING_CHANNEL_PINS)
		return E_PAR;
	if(hdfsc->Init.SerialType != DFSDM_CHANNEL_SPI_RISING &&
		hdfsc->Init.SerialType != DFSDM_CHANNEL_SPI_FALLING &&
			hdfsc->Init.SerialType != DFSDM_CHANNEL_MANCHESTER_RISING &&
				hdfsc->Init.SerialType != DFSDM_CHANNEL_MANCHESTER_FALLING)
		return E_PAR;
	if(hdfsc->Init.SerialSpiClock != DFSDM_CHANNEL_SPI_CLOCK_EXTERNAL &&
		hdfsc->Init.SerialSpiClock != DFSDM_CHANNEL_SPI_CLOCK_INTERNAL &&
			hdfsc->Init.SerialSpiClock != DFSDM_CHANNEL_SPI_CLOCK_INTERNAL_DIV2_FALLING &&
				hdfsc->Init.SerialSpiClock != DFSDM_CHANNEL_SPI_CLOCK_INTERNAL_DIV2_RISING)
		return E_PAR;
	if(hdfsc->Init.AwdFilterOrder != DFSDM_CHANNEL_FASTSINC_ORDER &&
		hdfsc->Init.AwdFilterOrder != DFSDM_CHANNEL_SINC1_ORDER &&
			hdfsc->Init.AwdFilterOrder != DFSDM_CHANNEL_SINC2_ORDER &&
				hdfsc->Init.AwdFilterOrder != DFSDM_CHANNEL_SINC3_ORDER)
		return E_PAR;
	if(hdfsc->Init.AwdOversampling < 1 || hdfsc->Init.AwdOversampling > 32)
		return E_PAR;
	if(hdfsc->Init.Offset < -8388608 || hdfsc->Init.Offset > 8388607)
		return E_PAR;
	if(hdfsc->Init.RightBitShift > 0x1F)
		return E_PAR;

	/*
	 *  ����ͥ�����ơ��֥�Υ����å�
	 */
	if(Dfsdm1ChannelHandle[get_channelno(hdfsc->base)] != NULL)
		return E_OBJ;

	/*
	 *  �����Х�DFSDM����
	 */
	if(++Dfsdm1ChannelCounter == 1){
		/*
		 *  �����ȥץåȡ����ꥢ�륯��å�����������
		 */
		sil_andw_mem((uint32_t *)(TADR_DFSDM1_CHANNEL0_BASE+TOFF_DFSDM_CHCFGR1), DFSDM_CHCFGR1_CKOUTSRC);
		sil_orw_mem((uint32_t *)(TADR_DFSDM1_CHANNEL0_BASE+TOFF_DFSDM_CHCFGR1), hdfsc->Init.OutClockSelection);

		/*
		 *  ����å�ʬ������
		 */
		sil_andw_mem((uint32_t *)(TADR_DFSDM1_CHANNEL0_BASE+TOFF_DFSDM_CHCFGR1), DFSDM_CHCFGR1_CKOUTDIV);
		if(hdfsc->Init.OutClockActivation == DFSDM_ENABLE)
			sil_orw_mem((uint32_t *)(TADR_DFSDM1_CHANNEL0_BASE+TOFF_DFSDM_CHCFGR1), ((hdfsc->Init.OutClockDivider - 1) << 16));

		/*
		 *  DFSDM�����Х륤�󥿡��ե���������
		 */
		sil_orw_mem((uint32_t *)(TADR_DFSDM1_CHANNEL0_BASE+TOFF_DFSDM_CHCFGR1), DFSDM_CHCFGR1_DFSDMEN);
	}

	/*
	 *  ����ͥ���������
	 */
	sil_andw_mem((uint32_t *)(hdfsc->base+TOFF_DFSDM_CHCFGR1), (DFSDM_CHCFGR1_DATPACK | DFSDM_CHCFGR1_DATMPX | DFSDM_CHCFGR1_CHINSEL));
	sil_orw_mem((uint32_t *)(hdfsc->base+TOFF_DFSDM_CHCFGR1), (hdfsc->Init.InputMultiplexer | hdfsc->Init.InputDataPacking | hdfsc->Init.InputPins));

	/*
	 *  ���ꥢ�륤�󥿡��ե���������
	 */
	sil_andw_mem((uint32_t *)(hdfsc->base+TOFF_DFSDM_CHCFGR1), (DFSDM_CHCFGR1_SITP | DFSDM_CHCFGR1_SPICKSEL));
	sil_orw_mem((uint32_t *)(hdfsc->base+TOFF_DFSDM_CHCFGR1), (hdfsc->Init.SerialType | hdfsc->Init.SerialSpiClock));

	/*
	 *  ���ʥ������å��ɥå�����
	 */
	sil_andw_mem((uint32_t *)(hdfsc->base+TOFF_DFSDM_CHAWSCDR), (DFSDM_CHAWSCDR_AWFORD | DFSDM_CHAWSCDR_AWFOSR));
	sil_orw_mem((uint32_t *)(hdfsc->base+TOFF_DFSDM_CHAWSCDR), (hdfsc->Init.AwdFilterOrder | ((hdfsc->Init.AwdOversampling - 1) << 16)));

	/*
	 *  ����ͥ륪�ե��åȡ��饤�ȥӥåȥ��ե�����
	 */
	sil_andw_mem((uint32_t *)(hdfsc->base+TOFF_DFSDM_CHCFGR2), (DFSDM_CHCFGR2_OFFSET | DFSDM_CHCFGR2_DTRBS));
	sil_orw_mem((uint32_t *)(hdfsc->base+TOFF_DFSDM_CHCFGR2), ((hdfsc->Init.Offset << 8) | (hdfsc->Init.RightBitShift << 3)));

	/*
	 *  DFSDM����ͥ�ͭ����
	 */
	sil_orw_mem((uint32_t *)(hdfsc->base+TOFF_DFSDM_CHCFGR1), DFSDM_CHCFGR1_CHEN);
	hdfsc->state = DFSDM_CHANNEL_STATE_READY;

	/*
	 *  DFSDM����ͥ�ơ��֥�����
	 */
	Dfsdm1ChannelHandle[get_channelno(hdfsc->base)] = hdfsc;
	return E_OK;
}

/*
 *  DFSDM����ͥ�̵����
 *  parameter1 hdfsc  DFSDM����ͥ�ϥ�ɥ�ؤΥݥ���
 *  return            ���ｪλ����E_OK
 */
ER
dfsdm_channel_deinit(DFSDM_Channel_Handle_t *hdfsc)
{
	if(hdfsc == NULL)
		return E_PAR;

	/*
	 *  DFSDM����ͥ�ơ��֥�����å�
	 */
	if(Dfsdm1ChannelHandle[get_channelno(hdfsc->base)] == NULL){
		return E_OBJ;
	}

	/*
	 *  DFSDM����ͥ�̵����
	 */
	sil_andw_mem((uint32_t *)(hdfsc->base+TOFF_DFSDM_CHCFGR1), DFSDM_CHCFGR1_CHEN);

	/*
	 *  �����Х�DFSDM̵����
	 */
	if(--Dfsdm1ChannelCounter == 0){
		sil_andw_mem((uint32_t *)(TADR_DFSDM1_CHANNEL0_BASE+TOFF_DFSDM_CHCFGR1), DFSDM_CHCFGR1_DFSDMEN);
	}

	/*
	 *  DFSDM����ͥ�ơ��֥�ꥻ�å�
	 */
	hdfsc->state = DFSDM_CHANNEL_STATE_RESET;
	Dfsdm1ChannelHandle[get_channelno(hdfsc->base)] = (DFSDM_Channel_Handle_t *)NULL;
	return E_OK;
}


/*
 *  ����å������֥��󥹥�������
 *  parameter1 hdfsc  DFSDM����ͥ�ϥ�ɥ�ؤΥݥ���
 *  return            ���ｪλ����E_OK
 */
ER
dfsdm_channelCkabStart(DFSDM_Channel_Handle_t *hdfsc)
{
	uint32_t channel;
	uint32_t tick = 0;

	if(hdfsc == NULL)
		return E_PAR;

	if(hdfsc->state != DFSDM_CHANNEL_STATE_READY)	/* ����Ƚ�� */
		return E_OBJ;

    /*
	 *  ����ͥ��ֹ����
	 */
    channel = get_channelno(hdfsc->base);

    /*
	 *  ����å����֥��󥹡��ե饰���ꥢ
	 */
    while((((sil_rew_mem((uint32_t *)(TADR_DFSDM1_FILTER0_BASE+TOFF_DFSDM_FLTISR)) & DFSDM_FLTISR_CKABF) >> (16 + channel)) & 1) != 0){
		sil_wrw_mem((uint32_t *)(TADR_DFSDM1_FILTER0_BASE+TOFF_DFSDM_FLTICR), (1 << (16 + channel)));
		if(tick++ > DFSDM_CKAB_TIMEOUT)
			return E_TMOUT;	/* �����ॢ���� */
		dly_tsk(1);
    }

	/*
	 *  ����å����֥��󥹳��������
	 */
	sil_orw_mem((uint32_t *)(TADR_DFSDM1_FILTER0_BASE+TOFF_DFSDM_FLTCR2), DFSDM_FLTCR2_CKABIE);

	/*
	 *  ����å����֥��󥹡���������
	 */
	sil_orw_mem((uint32_t *)(hdfsc->base+TOFF_DFSDM_CHCFGR1), DFSDM_CHCFGR1_CKABEN);
	return E_OK;
}

/*
 *  ����å������֥��󥹥��ȥå�
 *  parameter1 hdfsc  DFSDM����ͥ�ϥ�ɥ�ؤΥݥ���
 *  return            ���ｪλ����E_OK
 */
ER
dfsdm_channelCkabStop(DFSDM_Channel_Handle_t *hdfsc)
{
	uint32_t channel;

	if(hdfsc == NULL)
		return E_PAR;

	if(hdfsc->state != DFSDM_CHANNEL_STATE_READY)	/* ����Ƚ�� */
		return E_OBJ;

	/*
	 *  ����å����֥��󥹡����
	 */
	sil_andw_mem((uint32_t *)(hdfsc->base+TOFF_DFSDM_CHCFGR1), DFSDM_CHCFGR1_CKABEN);

	/*
	 *  ����å����֥��󥹡��ե饰���ꥢ
	 */
    channel = get_channelno(hdfsc->base);
	sil_wrw_mem((uint32_t *)(TADR_DFSDM1_FILTER0_BASE+TOFF_DFSDM_FLTICR), (1 << (16 + channel)));

	/*
	 *  ����å����֥��󥹳�������
	 */
	sil_andw_mem((uint32_t *)(TADR_DFSDM1_FILTER0_BASE+TOFF_DFSDM_FLTCR2), DFSDM_FLTCR2_CKABIE);
	return E_OK;
}

/*
 *  ���硼�ȡ��������åȥ�������
 *  parameter1 hdfsc       DFSDM����ͥ�ϥ�ɥ�ؤΥݥ���
 *  parameter2 Threshold   ���硼�ȡ��������åȡ�����å���ۡ����(0-255)
 *  parameter3 BreakSignal �֥졼�������ʥ�
 *  return            ���ｪλ����E_OK
 */
ER
dfsdm_channelScdStart(DFSDM_Channel_Handle_t *hdfsc, uint32_t Threshold, uint32_t BreakSignal)
{
	if(hdfsc == NULL)
		return E_PAR;
	if(Threshold > 255)
		return E_PAR;
	if(BreakSignal > 15)
		return E_PAR;

	if(hdfsc->state != DFSDM_CHANNEL_STATE_READY)	/* ����Ƚ�� */
		return E_OBJ;

	/*
	 *  ���硼�ȡ��������åȳ��������
	 */
	sil_orw_mem((uint32_t *)(TADR_DFSDM1_FILTER0_BASE+TOFF_DFSDM_FLTCR2), DFSDM_FLTCR2_SCDIE);

	/*
	 *  ����å���ۡ���ɡ��֥졼�������ʥ�����
	 */
	sil_andw_mem((uint32_t *)(hdfsc->base+TOFF_DFSDM_CHAWSCDR), (DFSDM_CHAWSCDR_BKSCD | DFSDM_CHAWSCDR_SCDT));
	sil_orw_mem((uint32_t *)(hdfsc->base+TOFF_DFSDM_CHAWSCDR), ((BreakSignal << 12) | Threshold));

	/*
	 *  ���硼�ȡ��������åȥ�������
	 */
	sil_orw_mem((uint32_t *)(hdfsc->base+TOFF_DFSDM_CHCFGR1), DFSDM_CHCFGR1_SCDEN);
	return E_OK;
}

/*
 *  ���硼�ȡ��������åȥ��ȥå�
 *  parameter1 hdfsc  DFSDM����ͥ�ϥ�ɥ�ؤΥݥ���
 *  return            ���ｪλ����E_OK
 */
ER
dfsdm_channelScdStop(DFSDM_Channel_Handle_t *hdfsc)
{
	uint32_t channel;

	if(hdfsc == NULL)
		return E_PAR;

	if(hdfsc->state != DFSDM_CHANNEL_STATE_READY)	/* ����Ƚ�� */
		return E_OBJ;

	/*
	 *  ���硼�ȡ��������å����
	 */
	sil_andw_mem((uint32_t *)(hdfsc->base+TOFF_DFSDM_CHCFGR1), DFSDM_CHCFGR1_SCDEN);

    /*
	 *  ���硼�ȡ��������åȥե饰���ꥢ
	 */
	channel = get_channelno(hdfsc->base);
	sil_wrw_mem((uint32_t *)(TADR_DFSDM1_FILTER0_BASE+TOFF_DFSDM_FLTICR), (1 << (24 + channel)));

	/*
	 *  ���硼�ȡ��������åȳ�������
	 */
	sil_andw_mem((uint32_t *)(TADR_DFSDM1_FILTER0_BASE+TOFF_DFSDM_FLTCR2), DFSDM_FLTCR2_SCDIE);
	return E_OK;
}

/*
 *  ���ʥ��������å��ɥå��ͼ���
 *  parameter1 hdfsc  DFSDM����ͥ�ϥ�ɥ�ؤΥݥ���
 *  return            ���ʥ��������å��ɥå���
 */
int16_t
dfsdm_channelGetAwdValue(DFSDM_Channel_Handle_t *hdfsc)
{
	return (int16_t)sil_rew_mem((uint32_t *)(hdfsc->base+TOFF_DFSDM_CHWDATAR));
}

/*
 *  ����ͥ륪�ե��å����ѹ�
 *  parameter1 hdfsc  DFSDM����ͥ�ϥ�ɥ�ؤΥݥ���
 *  parameter2 Offset ����ͥ륪�ե��å���(-8388608 - 8388607)
 *  return            ���ｪλ����E_OK
 */
ER
dfsdm_channelModifyOffset(DFSDM_Channel_Handle_t *hdfsc, int32_t Offset)
{
	if(hdfsc == NULL)
		return E_PAR;
	if(Offset < -8388608 || Offset > 8388607)
		return E_PAR;

	if(hdfsc->state != DFSDM_CHANNEL_STATE_READY)
		return E_OBJ;

	/*
	 *  ����ͥ륪�ե��å�������
	 */
	sil_andw_mem((uint32_t *)(hdfsc->base+TOFF_DFSDM_CHCFGR2), DFSDM_CHCFGR2_OFFSET);
	sil_orw_mem((uint32_t *)(hdfsc->base+TOFF_DFSDM_CHCFGR2), ((uint32_t) Offset << 8));
	return E_OK;
}


/*
 *  ���󥸥����ȥ���ͥ������
 */
static uint32_t
DFSDM_GetInjChannelsNbr(uint32_t Channels)
{
	uint32_t nbChannels = 0;
	uint32_t tmp = (Channels & DFSDM_LSB_MASK);

	/*
	 *  ����ͥ�ӥåȥե�����ɤ���������
	 */
	while(tmp != 0){
		if((tmp & 1) != 0)
			nbChannels++;
		tmp >>= 1;
	}
	return nbChannels;
}

/*
 *  �쥮��顼�Ѵ�����
 */
static void
DFSDM_RegConvStart(DFSDM_Filter_Handle_t* hdfsf)
{
	/*
	 *  �쥮��顼�ȥꥬ�ʤ�¨��������
	 */
	if(hdfsf->Init.RegTrigger == DFSDM_FILTER_SW_TRIGGER)
		sil_orw_mem((uint32_t *)(hdfsf->base+TOFF_DFSDM_FLTCR1), DFSDM_FLTCR1_RSWSTART);
	else{	/* ���󥯥�ʥ��ȥꥬ�ξ�� */
		/*
		 *  DFSDM�ե��륿���ǥ������֥�
		 */
		sil_andw_mem((uint32_t *)(hdfsf->base+TOFF_DFSDM_FLTCR1), DFSDM_FLTCR1_DFEN);

		/*
		 * RSYNC�ӥåȥ��å�
		 */
		sil_orw_mem((uint32_t *)(hdfsf->base+TOFF_DFSDM_FLTCR1), DFSDM_FLTCR1_RSYNC);

		/*
		 *  DFSDM�ե��륿�����͡��֥�
		 */
		sil_orw_mem((uint32_t *)(hdfsf->base+TOFF_DFSDM_FLTCR1), DFSDM_FLTCR1_DFEN);

		/*
		 *  ���󥸥������Ѵ���ʤ�ƥ�������
		 */
		if(hdfsf->state == DFSDM_FILTER_STATE_INJ){
			if(hdfsf->Init.InjTrigger == DFSDM_FILTER_SW_TRIGGER)
				sil_orw_mem((uint32_t *)(hdfsf->base+TOFF_DFSDM_FLTCR1), DFSDM_FLTCR1_JSWSTART);
			hdfsf->InjConvRemaining = (hdfsf->Init.InjScanMode == DFSDM_ENABLE) ? hdfsf->InjectedChannelsNbr : 1;
		}
	}
	/*
	 *  DFSDM�ե��륿�����ֹ���
	 */
	hdfsf->state = (hdfsf->state == DFSDM_FILTER_STATE_READY) ? DFSDM_FILTER_STATE_REG : DFSDM_FILTER_STATE_REG_INJ;
}

/*
 *  �쥮��顼�Ѵ����
 */
static void
DFSDM_RegConvStop(DFSDM_Filter_Handle_t* hdfsf)
{
	/*
	 *  DFSDM�ե��륿���ǥ������֥�
	 */
	sil_andw_mem((uint32_t *)(hdfsf->base+TOFF_DFSDM_FLTCR1), DFSDM_FLTCR1_DFEN);

	/*
	 *  ���󥯥�ʥ��ȥꥬ�ʤ� RSYNC�ӥåȥꥻ�å�
	 */
	if(hdfsf->Init.RegTrigger == DFSDM_FILTER_SYNC_TRIGGER)
		sil_andw_mem((uint32_t *)(hdfsf->base+TOFF_DFSDM_FLTCR1), DFSDM_FLTCR1_RSYNC);

	/*
	 *  DFSDM�ե��륿�����͡��֥�
	 */
	sil_orw_mem((uint32_t *)(hdfsf->base+TOFF_DFSDM_FLTCR1), DFSDM_FLTCR1_DFEN);
  
	/*
	 *  ���󥸥������Ѵ���ʤ�ꥹ������
	 */
	if(hdfsf->state == DFSDM_FILTER_STATE_REG_INJ){
		if(hdfsf->Init.InjTrigger == DFSDM_FILTER_SW_TRIGGER)
			sil_orw_mem((uint32_t *)(hdfsf->base+TOFF_DFSDM_FLTCR1), DFSDM_FLTCR1_JSWSTART);
		hdfsf->InjConvRemaining = (hdfsf->Init.InjScanMode == DFSDM_ENABLE) ? hdfsf->InjectedChannelsNbr : 1;
	}

	/*
	 *  DFSDM�ե��륿�����ֹ���
	 */
	hdfsf->state = (hdfsf->state == DFSDM_FILTER_STATE_REG) ? DFSDM_FILTER_STATE_READY : DFSDM_FILTER_STATE_INJ;
}

/*
 *  ���󥸥������Ѵ�����
 */
static void
DFSDM_InjConvStart(DFSDM_Filter_Handle_t* hdfsf)
{
	/*
	 *  ���󥸥����ȥȥꥬ�ʤ�¨��������
	 */
	if(hdfsf->Init.InjTrigger == DFSDM_FILTER_SW_TRIGGER)
		sil_orw_mem((uint32_t *)(hdfsf->base+TOFF_DFSDM_FLTCR1), DFSDM_FLTCR1_JSWSTART);
	else{	/* �����ޤ��ϥ��󥯥�ʥ��ȥꥬ�ξ�� */
		/*
		 *  DFSDM�ե��륿���ǥ������֥�
		 */
		sil_andw_mem((uint32_t *)(hdfsf->base+TOFF_DFSDM_FLTCR1), DFSDM_FLTCR1_DFEN);

		/*
		 *  ���󥯥�ʥ��ȥꥬ�ʤ�JSYNC�ӥåȥ��å�
		 *  �����ȥꥬ�ʤ�JEXTEN�ӥåȥ��å�
		 */
		if(hdfsf->Init.InjTrigger == DFSDM_FILTER_SYNC_TRIGGER)
			sil_orw_mem((uint32_t *)(hdfsf->base+TOFF_DFSDM_FLTCR1), DFSDM_FLTCR1_JSYNC);
		else
			sil_orw_mem((uint32_t *)(hdfsf->base+TOFF_DFSDM_FLTCR1), hdfsf->Init.InjExtTriggerEdge);

		/*
		 *  DFSDM�ե��륿�����͡��֥�
		 */
		sil_orw_mem((uint32_t *)(hdfsf->base+TOFF_DFSDM_FLTCR1), DFSDM_FLTCR1_DFEN);

		/* If regular conversion was in progress, restart it */
		if(hdfsf->state == DFSDM_FILTER_STATE_REG && hdfsf->Init.RegTrigger == DFSDM_FILTER_SW_TRIGGER)
			sil_orw_mem((uint32_t *)(hdfsf->base+TOFF_DFSDM_FLTCR1), DFSDM_FLTCR1_RSWSTART);
	}

	/*
	 *  DFSDM�ե��륿�����ֹ���
	 */
	hdfsf->state = (hdfsf->state == DFSDM_FILTER_STATE_READY) ? DFSDM_FILTER_STATE_INJ : DFSDM_FILTER_STATE_REG_INJ;
}

/*
 *  ���󥸥������Ѵ����
 */
static void
DFSDM_InjConvStop(DFSDM_Filter_Handle_t* hdfsf)
{
	/*
	 *  DFSDM�ե��륿���ǥ������֥�
	 */
	sil_andw_mem((uint32_t *)(hdfsf->base+TOFF_DFSDM_FLTCR1), DFSDM_FLTCR1_DFEN);

	/*
	 *  ���󥯥�ʥ��ȥꥬ�ʤ�JSYNC�ӥåȥꥻ�å�
	 *  �����ȥꥬ�ʤ�JEXTEN�ӥåȥꥻ�å�
	 */
	if(hdfsf->Init.InjTrigger == DFSDM_FILTER_SYNC_TRIGGER)
		sil_andw_mem((uint32_t *)(hdfsf->base+TOFF_DFSDM_FLTCR1), DFSDM_FLTCR1_JSYNC);
	else if(hdfsf->Init.InjTrigger == DFSDM_FILTER_EXT_TRIGGER)
		sil_andw_mem((uint32_t *)(hdfsf->base+TOFF_DFSDM_FLTCR1), DFSDM_FLTCR1_JEXTEN);

	/*
	 *  DFSDM�ե��륿�����͡��֥�
	 */
	sil_orw_mem((uint32_t *)(hdfsf->base+TOFF_DFSDM_FLTCR1), DFSDM_FLTCR1_DFEN);

	/*
	 *  �쥮��顼�Ѵ���ʤ顢�ꥹ������
	 */
	if(hdfsf->state == DFSDM_FILTER_STATE_REG_INJ && hdfsf->Init.RegTrigger == DFSDM_FILTER_SW_TRIGGER)
		sil_orw_mem((uint32_t *)(hdfsf->base+TOFF_DFSDM_FLTCR1), DFSDM_FLTCR1_RSWSTART);

	/*
	 *  ���󥸥����Ⱦ��ֹ���
	 */
	hdfsf->InjConvRemaining = (hdfsf->Init.InjScanMode == DFSDM_ENABLE) ? hdfsf->InjectedChannelsNbr : 1;

	/*
	 *  DFSDM�ե��륿�����ֹ���
	 */
	hdfsf->state = (hdfsf->state == DFSDM_FILTER_STATE_INJ) ? DFSDM_FILTER_STATE_READY : DFSDM_FILTER_STATE_REG;
}


/*
 *  DFSDM�ե��륿�������
 *  parameter1 hdfsc  DFSDM�ե��륿���ϥ�ɥ�ؤΥݥ���
 *  return            ���ｪλ����E_OK
 */
ER
dfsdm_filter_init(DFSDM_Filter_Handle_t *hdfsf)
{
	if(hdfsf == NULL)
		return E_PAR;
	if(hdfsf->Init.RegTrigger != DFSDM_FILTER_SW_TRIGGER && hdfsf->Init.RegTrigger != DFSDM_FILTER_SYNC_TRIGGER)
		return E_PAR;
	if((hdfsf->Init.RegFastMode & DFSDM_MASK) != 0)
		return E_PAR;
	if((hdfsf->Init.RegDmaMode & DFSDM_MASK) != 0)
		return E_PAR;
	if((hdfsf->Init.InjScanMode & DFSDM_MASK) != 0)
		return E_PAR;
	if((hdfsf->Init.InjDmaMode & DFSDM_MASK) != 0)
		return E_PAR;
	if(hdfsf->Init.FilterSincOrder != DFSDM_FILTER_FASTSINC_ORDER &&
		hdfsf->Init.FilterSincOrder != DFSDM_FILTER_SINC1_ORDER &&
			hdfsf->Init.FilterSincOrder != DFSDM_FILTER_SINC2_ORDER &&
				hdfsf->Init.FilterSincOrder != DFSDM_FILTER_SINC3_ORDER &&
					hdfsf->Init.FilterSincOrder != DFSDM_FILTER_SINC4_ORDER &&
						hdfsf->Init.FilterSincOrder != DFSDM_FILTER_SINC5_ORDER)
		return E_PAR;
	if(hdfsf->Init.FilterOversampling < 1 || hdfsf->Init.FilterOversampling > 1024)
		return E_PAR;
	if(hdfsf->Init.FilterIntOversampling < 1 || hdfsf->Init.FilterIntOversampling > 256)
		return E_PAR;

	/*
	 *  Check parameters compatibility */
	if((hdfsf->base == TADR_DFSDM1_FILTER0_BASE) && 
		((hdfsf->Init.RegTrigger  == DFSDM_FILTER_SYNC_TRIGGER) || 
			(hdfsf->Init.InjTrigger == DFSDM_FILTER_SYNC_TRIGGER)))
		return E_PAR;

	/*
	 *  DFSDM�ե��륿���ǥե��������
	 */
	hdfsf->RegularContMode     = DFSDM_CONTINUOUS_CONV_OFF;
	hdfsf->InjectedChannelsNbr = 1;
	hdfsf->InjConvRemaining    = 1;
	hdfsf->errorcode           = DFSDM_FILTER_ERROR_NONE;

	/*
	 *  �쥮��顼�Ѵ�����
	 */
	sil_andw_mem((uint32_t *)(hdfsf->base+TOFF_DFSDM_FLTCR1), DFSDM_FLTCR1_RSYNC);
	if(hdfsf->Init.RegFastMode == DFSDM_ENABLE)
		sil_orw_mem((uint32_t *)(hdfsf->base+TOFF_DFSDM_FLTCR1), DFSDM_FLTCR1_FAST);
	else
		sil_andw_mem((uint32_t *)(hdfsf->base+TOFF_DFSDM_FLTCR1), DFSDM_FLTCR1_FAST);

	if(hdfsf->Init.RegDmaMode == DFSDM_ENABLE)
		sil_orw_mem((uint32_t *)(hdfsf->base+TOFF_DFSDM_FLTCR1), DFSDM_FLTCR1_RDMAEN);
	else
		sil_andw_mem((uint32_t *)(hdfsf->base+TOFF_DFSDM_FLTCR1), DFSDM_FLTCR1_RDMAEN);

	/*
	 *  ���󥸥������Ѵ�����
	 */
	sil_andw_mem((uint32_t *)(hdfsf->base+TOFF_DFSDM_FLTCR1), (DFSDM_FLTCR1_JSYNC | DFSDM_FLTCR1_JEXTEN | DFSDM_FLTCR1_JEXTSEL));
	if(hdfsf->Init.InjTrigger == DFSDM_FILTER_EXT_TRIGGER){
		if((hdfsf->Init.InjExtTrigger & ~DFSDM_FLTCR1_JEXTSEL) != 0)
			return E_PAR;
		sil_orw_mem((uint32_t *)(hdfsf->base+TOFF_DFSDM_FLTCR1), hdfsf->Init.InjExtTrigger);
	}

	if(hdfsf->Init.InjScanMode == DFSDM_ENABLE)
		sil_orw_mem((uint32_t *)(hdfsf->base+TOFF_DFSDM_FLTCR1), DFSDM_FLTCR1_JSCAN);
	else
		sil_andw_mem((uint32_t *)(hdfsf->base+TOFF_DFSDM_FLTCR1), DFSDM_FLTCR1_JSCAN);

	if(hdfsf->Init.InjDmaMode == DFSDM_ENABLE)
		sil_orw_mem((uint32_t *)(hdfsf->base+TOFF_DFSDM_FLTCR1), DFSDM_FLTCR1_JDMAEN);
	else
		sil_andw_mem((uint32_t *)(hdfsf->base+TOFF_DFSDM_FLTCR1), DFSDM_FLTCR1_JDMAEN);

	/*
	 *  �ե��륿���ѥѥ�᡼������
	 */
	sil_andw_mem((uint32_t *)(hdfsf->base+TOFF_DFSDM_FLTFCR), (DFSDM_FLTFCR_FORD | DFSDM_FLTFCR_FOSR | DFSDM_FLTFCR_IOSR));
	sil_orw_mem((uint32_t *)(hdfsf->base+TOFF_DFSDM_FLTFCR), (hdfsf->Init.FilterSincOrder |
                                    ((hdfsf->Init.FilterOversampling - 1) << 16) |
                                  (hdfsf->Init.FilterIntOversampling - 1)));

	/*
	 *  DFSDM�ե��륿��ͭ����
	 */
	sil_orw_mem((uint32_t *)(hdfsf->base+TOFF_DFSDM_FLTCR1), DFSDM_FLTCR1_DFEN);
	hdfsf->state = DFSDM_FILTER_STATE_READY;
	return E_OK;
}

/*
 *  DFSDM�ե��륿��̵����
 *  parameter1 hdfsc  DFSDM�ե��륿���ϥ�ɥ�ؤΥݥ���
 *  return            ���ｪλ����E_OK
 */
ER
dfsdm_filter_deinit(DFSDM_Filter_Handle_t *hdfsf)
{
	if(hdfsf == NULL)
		return E_PAR;

	/*
	 *  DFSDM�ե��륿��ͭ����
	 */
	sil_andw_mem((uint32_t *)(hdfsf->base+TOFF_DFSDM_FLTCR1), DFSDM_FLTCR1_DFEN);
	hdfsf->state = DFSDM_FILTER_STATE_RESET;
	return E_OK;
}

/*
 *  DFSDM�ե��륿���쥮��顼�Ѵ�����
 *  parameter1 hdfsc    DFSDM�ե��륿���ϥ�ɥ�ؤΥݥ���
 *  parameter2 Channel  ����ͥ��ֹ�
 *  parameter3 ContMode ����ƥ��˥塼ͭ��̵������
 *  return              ���ｪλ����E_OK
 */
ER
dfsdm_filter_config_reg(DFSDM_Filter_Handle_t *hdfsf, uint32_t Channel, uint32_t ContinuousMode)
{
	if(hdfsf == NULL)
		return E_PAR;
	if(Channel < DFSDM_CHANNEL_0 || Channel > DFSDM_CHANNEL_7)
		return E_PAR;
	if(ContinuousMode != DFSDM_CONTINUOUS_CONV_OFF && ContinuousMode != DFSDM_CONTINUOUS_CONV_ON)
		return E_PAR;

	/*
	 *  ����Ƚ��
	 */
	if(hdfsf->state != DFSDM_FILTER_STATE_RESET && hdfsf->state != DFSDM_FILTER_STATE_ERROR){
		/*
		 *  �쥮��顼�Ѵ��ѡ�����ͥ�ȷ�³�⡼������
		 */
		sil_andw_mem((uint32_t *)(hdfsf->base+TOFF_DFSDM_FLTCR1), (DFSDM_FLTCR1_RCH | DFSDM_FLTCR1_RCONT));
		if(ContinuousMode == DFSDM_CONTINUOUS_CONV_ON)
			sil_orw_mem((uint32_t *)(hdfsf->base+TOFF_DFSDM_FLTCR1), (((Channel << 8) & DFSDM_FLTCR1_RCH) |
                                                     DFSDM_FLTCR1_RCONT));
		else
			sil_orw_mem((uint32_t *)(hdfsf->base+TOFF_DFSDM_FLTCR1), ((Channel << 8) & DFSDM_FLTCR1_RCH));
		hdfsf->RegularContMode = ContinuousMode;
		return E_OK;
	}
	else
		return E_OBJ;
}

/*
 *  DFSDM�ե��륿�����󥸥������Ѵ�����
 *  parameter1 hdfsc    DFSDM�ե��륿���ϥ�ɥ�ؤΥݥ���
 *  parameter2 Channel  ����ͥ��ֹ�
 *  return              ���ｪλ����E_OK
 */
ER dfsdm_filter_config_inj(DFSDM_Filter_Handle_t *hdfsf, uint32_t Channel)
{
	if(hdfsf == NULL)
		return E_PAR;

	if(Channel == 0 || Channel > 0x000F00FF)
		return E_PAR;

	/*
	 *  ����Ƚ��
	 */
	if(hdfsf->state != DFSDM_FILTER_STATE_RESET && hdfsf->state != DFSDM_FILTER_STATE_ERROR){
		/*
		 *  ���󥸥������Ѵ��ѡ�����ͥ�����
		 */
		sil_wrw_mem((uint32_t *)(hdfsf->base+TOFF_DFSDM_FLTJCHGR), (Channel & DFSDM_LSB_MASK));
		hdfsf->InjectedChannelsNbr = DFSDM_GetInjChannelsNbr(Channel);
		hdfsf->InjConvRemaining = (hdfsf->Init.InjScanMode == DFSDM_ENABLE) ? hdfsf->InjectedChannelsNbr : 1;
		return E_OK;
	}
	else
		return E_OBJ;
}

/*
 *  DFSDM�ե��륿���쥮��顼�Ѵ�DMA�ϡ��ս�λ������Хå�
 */
static void
DFSDM_DMARegularHalfConvCplt(DMA_Handle_t *hdma)
{
	DFSDM_Filter_Handle_t* hdfsf = (DFSDM_Filter_Handle_t*) ((DMA_Handle_t*)hdma)->localdata;

	/*
	 *  ������Хå��ؿ��θƤӽФ�
	 */
	if(hdfsf->regconvhalfcallback != NULL)
		hdfsf->regconvhalfcallback(hdfsf);
}

/*
 *  DFSDM�ե��륿���쥮��顼�Ѵ�DMA��λ������Хå�
 */
static void
DFSDM_DMARegularConvCplt(DMA_Handle_t *hdma)
{
	DFSDM_Filter_Handle_t* hdfsf = (DFSDM_Filter_Handle_t*) ((DMA_Handle_t*)hdma)->localdata;

	/*
	 *  ������Хå��ؿ��θƤӽФ�
	 */
	if(hdfsf->regconvcallback != NULL)
		hdfsf->regconvcallback(hdfsf);
}

/*
 *  DFSDM�ե��륿�����󥸥������Ѵ�DMA�ϡ��ս�λ������Хå�
 */
static void
DFSDM_DMAInjectedHalfConvCplt(DMA_Handle_t *hdma)
{
	DFSDM_Filter_Handle_t* hdfsf = (DFSDM_Filter_Handle_t*) ((DMA_Handle_t*)hdma)->localdata;

	/*
	 *  ������Хå��ؿ��θƤӽФ�
	 */
	if(hdfsf->injconvhalfcallback != NULL)
		hdfsf->injconvhalfcallback(hdfsf);
}

/*
 *  DFSDM�ե��륿�����󥸥������Ѵ�DMA��λ������Хå�
 */
static void
DFSDM_DMAInjectedConvCplt(DMA_Handle_t *hdma)
{
	DFSDM_Filter_Handle_t* hdfsf = (DFSDM_Filter_Handle_t*) ((DMA_Handle_t*)hdma)->localdata;

	/*
	 *  ������Хå��ؿ��θƤӽФ�
	 */
	if(hdfsf->injconvcallback != NULL)
		hdfsf->injconvcallback(hdfsf);
}

/*
 *  DFSDM�ե��륿��DMA���顼������Хå�
 */
static void
DFSDM_DMAError(DMA_Handle_t *hdma)
{
	DFSDM_Filter_Handle_t* hdfsf = (DFSDM_Filter_Handle_t*) ((DMA_Handle_t*)hdma)->localdata;

	/*
	 *  ���顼����������
	 */
	hdfsf->errorcode = DFSDM_FILTER_ERROR_DMA;

	/*
	 *  DMA���顼������Хå�
	 */
	if(hdfsf->errorcallback != NULL)
		hdfsf->errorcallback(hdfsf);
}


/*
 *  DFSDM�ե��륿���쥮��顼�Ѵ���������(32BIT�Ѵ�)
 *  parameter1 hdfsc    DFSDM�ե��륿���ϥ�ɥ�ؤΥݥ���
 *  paramter 2 pData    �Ѵ��ǡ��������ΰ�Υݥ���
 *  parameter3 Length   �ΰ�Ĺ
 *  return              ���ｪλ����E_OK
  */
ER
dfsdm_filterRegularStart(DFSDM_Filter_Handle_t *hdfsf, int32_t *pData, uint32_t Length)
{
	if(hdfsf == NULL || pData == NULL || Length == 0)
		return E_PAR;

	/*
	 *  ��������ͤγ�ǧ
	 */
	if((hdfsf->Init.RegTrigger == DFSDM_FILTER_SW_TRIGGER) && \
          (hdfsf->RegularContMode == DFSDM_CONTINUOUS_CONV_OFF) && \
          (hdfsf->hdmaReg->Init.Mode == DMA_NORMAL) && Length != 1)
		return E_PAR;

	if((hdfsf->Init.RegTrigger == DFSDM_FILTER_SW_TRIGGER) && \
          (hdfsf->RegularContMode == DFSDM_CONTINUOUS_CONV_OFF) && \
          (hdfsf->hdmaReg->Init.Mode == DMA_CIRCULAR))
		return E_OBJ;

	/*
	 *  DMA��ͭ����ǧ
	 */
	if((sil_rew_mem((uint32_t *)(hdfsf->base+TOFF_DFSDM_FLTCR1)) & DFSDM_FLTCR1_RDMAEN) == 0)
		return E_OBJ;

	/*
	 *  DFSDM�ե��륿���ξ��֥����å�
	 */
	if(hdfsf->state == DFSDM_FILTER_STATE_READY || hdfsf->state == DFSDM_FILTER_STATE_INJ){
		ER ercd = E_OK;
		/*
		 *  ������Хå��ؿ�������
		 */
		hdfsf->hdmaReg->xfercallback = DFSDM_DMARegularConvCplt;
		hdfsf->hdmaReg->errorcallback = DFSDM_DMAError;
		hdfsf->hdmaReg->xferhalfcallback = (hdfsf->hdmaReg->Init.Mode == DMA_CIRCULAR) ? DFSDM_DMARegularHalfConvCplt : NULL;

		/*
		 *  DMA��������
		 */
		if((ercd = dma_start(hdfsf->hdmaReg, (uint32_t)(hdfsf->base+TOFF_DFSDM_FLTRDATAR), (uint32_t)pData, Length)) != E_OK)
			hdfsf->state = DFSDM_FILTER_STATE_ERROR;	/* DMA���顼 */
		else	/* �쥮��顼�Ѵ��������� */
			DFSDM_RegConvStart(hdfsf);
		return ercd;
	}
	else
		return E_OBJ;
}

/*
 *  DFSDM�ե��륿���쥮��顼�Ѵ���������(16BIT�Ѵ�)
 *  parameter1 hdfsc    DFSDM�ե��륿���ϥ�ɥ�ؤΥݥ���
 *  paramter 2 pData    �Ѵ��ǡ��������ΰ�Υݥ���
 *  parameter3 Length   �ΰ�Ĺ
 *  return              ���ｪλ����E_OK
 */
ER
dfsdm_filterRegularMsbStart(DFSDM_Filter_Handle_t *hdfsf, int16_t *pData, uint32_t Length)
{
	if(hdfsf == NULL || pData == NULL || Length == 0)
		return E_PAR;

	/*
	 *  ��������ͤγ�ǧ
	 */
	if((hdfsf->Init.RegTrigger == DFSDM_FILTER_SW_TRIGGER) && \
          (hdfsf->RegularContMode == DFSDM_CONTINUOUS_CONV_OFF) && \
          (hdfsf->hdmaReg->Init.Mode == DMA_NORMAL) && (Length != 1))
		return E_PAR;

	if((hdfsf->Init.RegTrigger == DFSDM_FILTER_SW_TRIGGER) && \
          (hdfsf->RegularContMode == DFSDM_CONTINUOUS_CONV_OFF) && \
          (hdfsf->hdmaReg->Init.Mode == DMA_CIRCULAR))
		return E_OBJ;

	/*
	 *  DMA��ͭ����ǧ
	 */
	if((sil_rew_mem((uint32_t *)(hdfsf->base+TOFF_DFSDM_FLTCR1)) & DFSDM_FLTCR1_RDMAEN) == 0)
		return E_OBJ;

	/*
	 *  DFSDM�ե��륿���ξ��֥����å�
	 */
	if(hdfsf->state == DFSDM_FILTER_STATE_READY || hdfsf->state == DFSDM_FILTER_STATE_INJ){
		ER ercd = E_OK;
		/*
		 *  ������Хå��ؿ�������
		 */
		hdfsf->hdmaReg->xfercallback = DFSDM_DMARegularConvCplt;
		hdfsf->hdmaReg->errorcallback = DFSDM_DMAError;
		hdfsf->hdmaReg->xferhalfcallback = (hdfsf->hdmaReg->Init.Mode == DMA_CIRCULAR) ? DFSDM_DMARegularHalfConvCplt : NULL;

		/*
		 *  DMA��������
		 */
		if((ercd = dma_start(hdfsf->hdmaReg, (uint32_t)(hdfsf->base+TOFF_DFSDM_FLTRDATAR+2), (uint32_t)pData, Length)) != E_OK)
			hdfsf->state = DFSDM_FILTER_STATE_ERROR;	/* DMA�������ȥ��顼 */
		else	/* �쥮��顼�Ѵ��������� */
			DFSDM_RegConvStart(hdfsf);
		return ercd;
	}
	else
		return E_OBJ;
}

/*
 *  DFSDM�ե��륿���쥮��顼�Ѵ����
 *  parameter1 hdfsc    DFSDM�ե��륿���ϥ�ɥ�ؤΥݥ���
 *  return              ���ｪλ����E_OK
 */
ER
dfsdm_filterRegularStop(DFSDM_Filter_Handle_t *hdfsf)
{
	ER ercd = E_OK;

	if(hdfsf == NULL)
		return E_PAR;

	/*
	 *  DFSDM�ե��륿���ξ��֥����å�
	 */
	if(hdfsf->state != DFSDM_FILTER_STATE_REG && hdfsf->state != DFSDM_FILTER_STATE_REG_INJ)
		return E_OBJ;

	 /*
	  *  DMA���
	  */
	if((ercd = dma_end(hdfsf->hdmaReg)) != E_OK)	/* Set DFSDM filter in error state */
		hdfsf->state = DFSDM_FILTER_STATE_ERROR;
	else	/* �쥮��顼�Ѵ���� */
		DFSDM_RegConvStop(hdfsf);
	return ercd;
}

/*
 *  DFSDM�ե��륿���쥮��顼�Ѵ��ͼ���
 *  parameter1 hdfsc    DFSDM�ե��륿���ϥ�ɥ�ؤΥݥ���
 *  parameter2 Channel  DFSDM����ͥ��ֹ������ѥݥ���
 *  return              �쥮��顼�Ѵ���
 */
int32_t
dfsdm_filterGetRegularValue(DFSDM_Filter_Handle_t *hdfsf, uint32_t *Channel)
{
	uint32_t reg = 0;

	if(hdfsf == NULL || Channel == NULL)
		return 0;

	/*
	 *  �쥮��顼�Ѵ��ͤȥ���ͥ��ֹ��Ф�
	 */
	reg = sil_rew_mem((uint32_t *)(hdfsf->base+TOFF_DFSDM_FLTRDATAR));

	/* Extract channel and regular conversion value */
	*Channel = (reg & DFSDM_FLTRDATAR_RDATACH);
	return (int32_t)((reg & DFSDM_FLTRDATAR_RDATA) >> 18);
}

/*
 *  DFSDM�ե��륿�����󥸥������Ѵ���������(32BIT�Ѵ�)
 *  parameter1 hdfsc    DFSDM�ե��륿���ϥ�ɥ�ؤΥݥ���
 *  paramter 2 pData    �Ѵ��ǡ��������ΰ�Υݥ���
 *  parameter3 Length   �ΰ�Ĺ
 *  return              ���ｪλ����E_OK
 */
ER
dfsdm_filterInjectedStart(DFSDM_Filter_Handle_t *hdfsf, int32_t *pData, uint32_t Length)
{
	if(hdfsf == NULL || pData == NULL || Length == 0)
		return E_PAR;

	/*
	 *  ��������ͤγ�ǧ
	 */
	if((hdfsf->Init.InjTrigger == DFSDM_FILTER_SW_TRIGGER) && \
          (hdfsf->hdmaInj->Init.Mode == DMA_NORMAL) && Length > hdfsf->InjConvRemaining)
		return E_PAR;

	if((hdfsf->Init.InjTrigger == DFSDM_FILTER_SW_TRIGGER) && hdfsf->hdmaInj->Init.Mode == DMA_CIRCULAR)
		return E_OBJ;

	/*
	 *  DMA��ͭ����ǧ
	 */
	if((sil_rew_mem((uint32_t *)(hdfsf->base+TOFF_DFSDM_FLTCR1)) & DFSDM_FLTCR1_JDMAEN) == 0)
		return  E_OBJ;

	/*
	 *  DFSDM�ե��륿���ξ��֥����å�
	 */
	if(hdfsf->state == DFSDM_FILTER_STATE_READY || hdfsf->state == DFSDM_FILTER_STATE_REG){
		ER ercd = E_OK;
		/*
		 *  ������Хå��ؿ�������
		 */
		hdfsf->hdmaInj->xfercallback = DFSDM_DMAInjectedConvCplt;
		hdfsf->hdmaInj->errorcallback = DFSDM_DMAError;
		hdfsf->hdmaInj->xferhalfcallback = (hdfsf->hdmaInj->Init.Mode == DMA_CIRCULAR) ?\
                                                   DFSDM_DMAInjectedHalfConvCplt : NULL;

		/*
		 *  DMA��������
		 */
		if((ercd = dma_start(hdfsf->hdmaInj, (uint32_t)(hdfsf->base+TOFF_DFSDM_FLTJDATAR), (uint32_t)pData, Length)) != E_OK)
			hdfsf->state = DFSDM_FILTER_STATE_ERROR;
		else	/* ���󥸥������Ѵ��������� */
			DFSDM_InjConvStart(hdfsf);
		return ercd;
	}
	else
		return E_OBJ;
}

/*
 *  DFSDM�ե��륿�����󥸥������Ѵ���������(16BIT�Ѵ�)
 *  parameter1 hdfsc    DFSDM�ե��륿���ϥ�ɥ�ؤΥݥ���
 *  paramter 2 pData    �Ѵ��ǡ��������ΰ�Υݥ���
 *  parameter3 Length   �ΰ�Ĺ
 *  return              ���ｪλ����E_OK
 */
ER
dfsdm_filterInjectedMsbStart(DFSDM_Filter_Handle_t *hdfsf, int16_t *pData, uint32_t Length)
{
	if(hdfsf == NULL || pData == NULL || Length == 0)
		return E_PAR;

	/*
	 *  ��������ͤγ�ǧ
	 */
	if((hdfsf->Init.InjTrigger == DFSDM_FILTER_SW_TRIGGER) && \
          (hdfsf->hdmaInj->Init.Mode == DMA_NORMAL) && (Length > hdfsf->InjConvRemaining)){
		return E_PAR;
	}
	else if((hdfsf->Init.InjTrigger == DFSDM_FILTER_SW_TRIGGER) && \
          (hdfsf->hdmaInj->Init.Mode == DMA_CIRCULAR)){
		return E_OBJ;
 	}

	/*
	 *  DMA��ͭ����ǧ
	 */
	if((sil_rew_mem((uint32_t *)(hdfsf->base+TOFF_DFSDM_FLTCR1)) & DFSDM_FLTCR1_JDMAEN) == 0)
		return E_OBJ;

	/*
	 *  DFSDM�ե��륿���ξ��֥����å�
	 */
	if(hdfsf->state == DFSDM_FILTER_STATE_READY || hdfsf->state == DFSDM_FILTER_STATE_REG){
		ER ercd = E_OK;
		/*
		 *  ������Хå��ؿ�������
		 */
		hdfsf->hdmaInj->xfercallback = DFSDM_DMAInjectedConvCplt;
		hdfsf->hdmaInj->errorcallback = DFSDM_DMAError;
		hdfsf->hdmaInj->xferhalfcallback = (hdfsf->hdmaInj->Init.Mode == DMA_CIRCULAR) ? DFSDM_DMAInjectedHalfConvCplt : NULL;

		/*
		 *  DMA��������
		 */
		if((ercd = dma_start(hdfsf->hdmaInj, (uint32_t)(hdfsf->base+TOFF_DFSDM_FLTJDATAR+2), (uint32_t)pData, Length)) != E_OK)
			hdfsf->state = DFSDM_FILTER_STATE_ERROR;
		else
			DFSDM_InjConvStart(hdfsf);	/* ���󥸥������Ѵ��������� */
		return ercd;
	}
	else
		return E_OBJ;
}

/*
 *  DFSDM�ե��륿�����󥸥������Ѵ����
 *  parameter1 hdfsc    DFSDM�ե��륿���ϥ�ɥ�ؤΥݥ���
 *  return              ���ｪλ����E_OK
 */
ER
dfsdm_filterInjectedStop(DFSDM_Filter_Handle_t *hdfsf)
{
	ER ercd = E_OK;

	if(hdfsf == NULL)
		return E_PAR;

	/*
	 *  DFSDM�ե��륿���ξ��֥����å�
	 */
	if(hdfsf->state != DFSDM_FILTER_STATE_INJ && hdfsf->state != DFSDM_FILTER_STATE_REG_INJ)
		return E_OBJ;

	/*
	 *  DMA���
	 */
	if((ercd = dma_end(hdfsf->hdmaInj)) != E_OK)
		hdfsf->state = DFSDM_FILTER_STATE_ERROR;
	else
		DFSDM_InjConvStop(hdfsf);	/* ���󥸥������Ѵ���� */
	return ercd;
}

/*
 *  DFSDM�ե��륿�����󥸥������Ѵ��ͼ���
 *  parameter1 hdfsc    DFSDM�ե��륿���ϥ�ɥ�ؤΥݥ���
 *  parameter2 Channel  DFSDM����ͥ��ֹ������ѥݥ���
 *  return              ���󥸥������Ѵ���
 */
int32_t
dfsdm_filterGetInjectedValue(DFSDM_Filter_Handle_t *hdfsf, uint32_t *Channel)
{
	uint32_t reg = 0;

	if(hdfsf == NULL || Channel == NULL)
		return 0;

	/*
	 *  ���󥸥������Ѵ��ͤȥ���ͥ�����
	 */
	reg = sil_rew_mem((uint32_t *)(hdfsf->base+TOFF_DFSDM_FLTRDATAR));

	*Channel = (reg & DFSDM_FLTJDATAR_JDATACH);
	return (int32_t)((reg & DFSDM_FLTJDATAR_JDATA) >> 8);
}

/*
 *  ���ʥ륰�����å��ɥå���������
 *  parameter1 hdfsc    DFSDM�ե��륿���ϥ�ɥ�ؤΥݥ���
 *  paramter 2 awdParam ���ʥ륰�����å��ɥå����깽¤�ΤؤΥݥ���
 *  return              ���ｪλ����E_OK
 */
ER
dfsdm_filterAwdStart(DFSDM_Filter_Handle_t *hdfsf, DFSDM_Filter_AwdParamTypeDef *awdParam)
{
	if(hdfsf == NULL)
		return E_PAR;
	if(awdParam->DataSource != DFSDM_FILTER_AWD_FILTER_DATA &&
		awdParam->DataSource != DFSDM_FILTER_AWD_CHANNEL_DATA)
		return E_PAR;
	if(awdParam->Channel == 0 || awdParam->Channel > 0x000F00FF)
		return E_PAR;
	if(awdParam->HighThreshold < -8388608 && awdParam->HighThreshold > 8388607)
		return E_PAR;
	if(awdParam->LowThreshold < -8388608 && awdParam->LowThreshold > 8388607)
		return E_PAR;
	if(awdParam->HighBreakSignal > 0xF)
		return E_PAR;
	if(awdParam->LowBreakSignal > 0xF)
		return E_PAR;

	/*
	 *  DFSDM�ե��륿�����֥����å�
	 */
	if(hdfsf->state == DFSDM_FILTER_STATE_RESET || hdfsf->state == DFSDM_FILTER_STATE_ERROR)
		return E_OBJ;

	/*
	 *  ���ʥ륰�����å��ɥå����ǡ�������������
	 */
	sil_andw_mem((uint32_t *)(hdfsf->base+TOFF_DFSDM_FLTCR1), DFSDM_FLTCR1_AWFSEL);
	sil_orw_mem((uint32_t *)(hdfsf->base+TOFF_DFSDM_FLTCR1), awdParam->DataSource);

	/*
	 *  ����å���ۡ���ɡ��֥졼�������ʥ�����
	 */
	sil_andw_mem((uint32_t *)(hdfsf->base+TOFF_DFSDM_FLTAWHTR), (DFSDM_FLTAWHTR_AWHT | DFSDM_FLTAWHTR_BKAWH));
	sil_orw_mem((uint32_t *)(hdfsf->base+TOFF_DFSDM_FLTAWHTR), ((awdParam->HighThreshold << 8) |
                                        awdParam->HighBreakSignal));
	sil_andw_mem((uint32_t *)(hdfsf->base+TOFF_DFSDM_FLTAWLTR), (DFSDM_FLTAWLTR_AWLT | DFSDM_FLTAWLTR_BKAWL));
	sil_orw_mem((uint32_t *)(hdfsf->base+TOFF_DFSDM_FLTAWLTR), ((awdParam->LowThreshold << 8) |
                                        awdParam->LowBreakSignal));

	/*
	 *  ����ͥ�ȳ��������
	 */
	sil_andw_mem((uint32_t *)(hdfsf->base+TOFF_DFSDM_FLTCR2), DFSDM_FLTCR2_AWDCH);
	sil_orw_mem((uint32_t *)(hdfsf->base+TOFF_DFSDM_FLTCR2), (((awdParam->Channel & DFSDM_LSB_MASK) << 16) |
                                        DFSDM_FLTCR2_AWDIE));
	return E_OK;
}

/*
 *  ���ʥ륰�����å��ɥå����
 *  parameter1 hdfsc    DFSDM�ե��륿���ϥ�ɥ�ؤΥݥ���
 *  return              ���ｪλ����E_OK
 */
ER
dfsdm_filterAwdStop(DFSDM_Filter_Handle_t *hdfsf)
{
	if(hdfsf == NULL)
		return E_PAR;

	/*
	 *  DFSDM�ե��륿�����֥����å�
	 */
	if(hdfsf->state == DFSDM_FILTER_STATE_RESET || hdfsf->state == DFSDM_FILTER_STATE_ERROR)
		return E_OBJ;

	/*
	 *  ����ͥ�ȳ���ߤΥꥻ�å�
	 */
	sil_andw_mem((uint32_t *)(hdfsf->base+TOFF_DFSDM_FLTCR2), (DFSDM_FLTCR2_AWDCH | DFSDM_FLTCR2_AWDIE));

	/*
	 *  ���ʥ������å��ɥå��ե饰���ꥢ
	 */
	sil_wrw_mem((uint32_t *)(hdfsf->base+TOFF_DFSDM_FLTAWCFR), (DFSDM_FLTAWCFR_CLRAWHTF | DFSDM_FLTAWCFR_CLRAWLTF));

	/*
	 *  ����å���ۡ���ɤȥ֥졼�������ʥ��ꥻ�å�
	 */
	sil_andw_mem((uint32_t *)(hdfsf->base+TOFF_DFSDM_FLTAWHTR), (DFSDM_FLTAWHTR_AWHT | DFSDM_FLTAWHTR_BKAWH));
	sil_andw_mem((uint32_t *)(hdfsf->base+TOFF_DFSDM_FLTAWLTR), (DFSDM_FLTAWLTR_AWLT | DFSDM_FLTAWLTR_BKAWL));

	/*
	 *  �ǡ�����������ꥻ�å�
	 */
	sil_andw_mem((uint32_t *)(hdfsf->base+TOFF_DFSDM_FLTCR1), DFSDM_FLTCR1_AWFSEL);
	return E_OK;
}

/*
 *  DFSDM�ե��륿�����������ȥ꡼�ม�Х�������
 *  parameter1 hdfsc    DFSDM�ե��륿���ϥ�ɥ�ؤΥݥ���
 *  parameter2 Channel  ����ͥ��ֹ�
 *  return              ���ｪλ����E_OK
 */
ER
dfsdm_filterExdStart(DFSDM_Filter_Handle_t *hdfsf, uint32_t Channel)
{
	if(hdfsf == NULL)
		return E_PAR;

	/*
	 *  DFSDM�ե��륿�����֥����å�
	 */
	if(hdfsf->state == DFSDM_FILTER_STATE_RESET || hdfsf->state == DFSDM_FILTER_STATE_ERROR)
		return E_OBJ;

	/*
	 *  �������ȥ꡼�ม�Х���ͥ�����
	 */
	sil_andw_mem((uint32_t *)(hdfsf->base+TOFF_DFSDM_FLTCR2), DFSDM_FLTCR2_EXCH);
	sil_orw_mem((uint32_t *)(hdfsf->base+TOFF_DFSDM_FLTCR2), ((Channel & DFSDM_LSB_MASK) << 8));
	return E_OK;
}

/*
 *  DFSDM�ե��륿�����������ȥ꡼�ม�����
 *  parameter1 hdfsc    DFSDM�ե��륿���ϥ�ɥ�ؤΥݥ���
 *  return              ���ｪλ����E_OK
 */
ER
dfsdm_filterExdStop(DFSDM_Filter_Handle_t *hdfsf)
{
	volatile uint32_t     reg;

	if(hdfsf == NULL)
		return E_PAR;

	/*
	 *  DFSDM�ե��륿�����֥����å�
	 */
	if(hdfsf->state == DFSDM_FILTER_STATE_RESET || hdfsf->state == DFSDM_FILTER_STATE_ERROR)
		return E_OBJ;

	/*
	 *  �������ȥ꡼�ม�Х���ͥ�ꥻ�å�
	 */
	sil_andw_mem((uint32_t *)(hdfsf->base+TOFF_DFSDM_FLTCR2), DFSDM_FLTCR2_EXCH);

    /*
	 *  �������ȥ꡼�ม���ͼ���
	 */
	reg = sil_rew_mem((uint32_t *)(hdfsf->base+TOFF_DFSDM_FLTEXMAX));
	reg = sil_rew_mem((uint32_t *)(hdfsf->base+TOFF_DFSDM_FLTEXMIN));
    ((void)(reg));
	return E_OK;
}

/*
 *  DFSDM�ե��륿�����������ȥ꡼�ม�к����ͼ���
 *  parameter1 hdfsc    DFSDM�ե��륿���ϥ�ɥ�ؤΥݥ���
 *  parameter2 Chnnel   ����ͥ���¸�ΰ�Υݥ���
 *  return              ������
 */
int32_t
dfsdm_filterGetExdMaxValue(DFSDM_Filter_Handle_t *hdfsf, uint32_t *Channel)
{
	uint32_t reg = 0;

	if(hdfsf == NULL || Channel == NULL)
		return E_PAR;

	/*
	 *  ����ͥ�Ⱥ����ͼ���
	 */
	reg = sil_rew_mem((uint32_t *)(hdfsf->base+TOFF_DFSDM_FLTEXMAX));
	*Channel = (reg & DFSDM_FLTEXMAX_EXMAXCH);
	return (int32_t)((reg & DFSDM_FLTEXMAX_EXMAX) >> 8);
}

/*
 *  DFSDM�ե��륿�����������ȥ꡼�ม�кǾ��ͼ���
 *  parameter1 hdfsc    DFSDM�ե��륿���ϥ�ɥ�ؤΥݥ���
 *  parameter2 Chnnel   ����ͥ���¸�ΰ�Υݥ���
 *  return              �Ǿ���
 */
int32_t
dfsdm_filterGetExdMinValue(DFSDM_Filter_Handle_t *hdfsf, uint32_t *Channel)
{
	uint32_t reg = 0;

	if(hdfsf == NULL || Channel == NULL)
		return E_PAR;

	/*
	 *  ����ͥ�ȺǾ��ͼ���
	 */
	reg = sil_rew_mem((uint32_t *)(hdfsf->base+TOFF_DFSDM_FLTEXMIN));
	*Channel = (reg & DFSDM_FLTEXMIN_EXMINCH);
	return (int32_t)((reg & DFSDM_FLTEXMIN_EXMIN) >> 8);
}

/*
 *  �Ѵ����ּ���
 *  parameter1 hdfsc    DFSDM�ե��륿���ϥ�ɥ�ؤΥݥ���
 *  return              ����(��)
 */
uint32_t
dfsdm_filterGetConvTimeValue(DFSDM_Filter_Handle_t *hdfsf)
{
	uint32_t reg = 0;

	if(hdfsf == NULL)
		return E_PAR;

	/*
	 *  �Ѵ����֥쥸�����ͼ���
	 */
	reg = sil_rew_mem((uint32_t *)(hdfsf->base+TOFF_DFSDM_FLTCNVTIMR));
	return (uint32_t)((reg & DFSDM_FLTCNVTIMR_CNVCNT) >> 4);
}


/*
 *  DFSDM�ե��륿������ߥϥ�ɥ�
 */
void dfsdm_irqhandler(DFSDM_Filter_Handle_t *hdfsf)
{
	DFSDM_Channel_Handle_t *hdfsc;
	uint32_t isr = sil_rew_mem((uint32_t *)(hdfsf->base+TOFF_DFSDM_FLTISR));
	uint32_t cr2 = sil_rew_mem((uint32_t *)(hdfsf->base+TOFF_DFSDM_FLTCR2));

	/*
	 * �쥮��顼�Ѵ������С���󥨥顼
	 */
	if(((isr & cr2) & DFSDM_FLTISR_ROVRF) != 0){
		/*
		 *  �����С�����װ����ꥢ
		 */
		sil_wrw_mem((uint32_t *)(hdfsf->base+TOFF_DFSDM_FLTICR), DFSDM_FLTICR_CLRROVRF);

		/*
		 *  ���顼����ȥ�����Хå�
		 */
		hdfsf->errorcode = DFSDM_FILTER_ERROR_REGULAR_OVERRUN;
		if(hdfsf->errorcallback != NULL)
			hdfsf->errorcallback(hdfsf);
	}
	/*
	 *  ���󥸥������Ѵ������С���󥨥顼
	 */
	else if(((isr & cr2) & DFSDM_FLTISR_JOVRF) != 0){
		/*
		 *  �����С�����װ����ꥢ
		 */
		sil_wrw_mem((uint32_t *)(hdfsf->base+TOFF_DFSDM_FLTICR), DFSDM_FLTICR_CLRJOVRF);

		/*
		 *  ���顼����ȥ�����Хå�
		 */
		hdfsf->errorcode = DFSDM_FILTER_ERROR_INJECTED_OVERRUN;
		if(hdfsf->errorcallback != NULL)
			hdfsf->errorcallback(hdfsf);
	}
	/*
	 *  �쥮��顼�Ѵ���λ
	 */
	else if(((isr & cr2) & DFSDM_FLTISR_REOCF) != 0){
		/*
		 *  ������Хå�
		 */
		if(hdfsf->regconvcallback != NULL)
			hdfsf->regconvcallback(hdfsf);

		/*
		 *  ���³�⡼�ɡ����ġ����եȥ������ȥꥬ�ξ��
		 */
		if((hdfsf->RegularContMode == DFSDM_CONTINUOUS_CONV_OFF) &&
			(hdfsf->Init.RegTrigger == DFSDM_FILTER_SW_TRIGGER)){
			/*
			 *  �쥮��顼�Ѵ���������
			 */
			sil_andw_mem((uint32_t *)(hdfsf->base+TOFF_DFSDM_FLTCR2), DFSDM_FLTCR2_REOCIE);

			/*
			 *  DFSDM�ե��륿�����ֹ���
			 */
			hdfsf->state = (hdfsf->state == DFSDM_FILTER_STATE_REG) ? DFSDM_FILTER_STATE_READY : DFSDM_FILTER_STATE_INJ;
		}
	}
	/*
	 *  ���󥸥������Ѵ���λ
	 */
	else if(((isr & cr2) & DFSDM_FLTISR_JEOCF) != 0){
		/*
		 *  ������Хå�
		 */
		if(hdfsf->injconvcallback != NULL)
			hdfsf->injconvcallback(hdfsf);

		/*
		 *  �Ĥ�Υ��󥸥������Ѵ�����
		 */
		if(--hdfsf->InjConvRemaining == 0){
			/*
			 *  ���եȥ������ȥꥬ�ʤ顢���󥸥������Ѵ����
			 */
			if(hdfsf->Init.InjTrigger == DFSDM_FILTER_SW_TRIGGER){
				sil_andw_mem((uint32_t *)(hdfsf->base+TOFF_DFSDM_FLTCR2), DFSDM_FLTCR2_JEOCIE);
				hdfsf->state = (hdfsf->state == DFSDM_FILTER_STATE_INJ) ? DFSDM_FILTER_STATE_READY : DFSDM_FILTER_STATE_REG;
			}
			/*
			 *  ���󥸥������Ѵ��⡼�ɹ���
			 */
			hdfsf->InjConvRemaining = (hdfsf->Init.InjScanMode == DFSDM_ENABLE) ? hdfsf->InjectedChannelsNbr : 1;
		}
	}
	/*
	 *  ���ʥ������å��ɥå�ȯ��
	 */
	else if(((isr & cr2) &DFSDM_FLTISR_AWDF) != 0){
		uint32_t reg = sil_rew_mem((uint32_t *)(hdfsf->base+TOFF_DFSDM_FLTAWSR));
		uint32_t threshold = 0;
		uint32_t channel = 0;

		/*
		 *  ����ͥ��ֹ�ȥ���å���ۡ���ɼ���
		 */
		threshold = ((reg & DFSDM_FLTAWSR_AWLTF) != 0) ? DFSDM_AWD_LOW_THRESHOLD : DFSDM_AWD_HIGH_THRESHOLD;
		if(threshold == DFSDM_AWD_HIGH_THRESHOLD){
			reg = reg >> 8;
		}
		while((reg & 1) == 0){
			channel++;
			reg = reg >> 1;
		}
		/*
		 *  ���ʥ륰�����å��ɥå����ե饰���ꥢ
		 */
		sil_wrw_mem((uint32_t *)(hdfsf->base+TOFF_DFSDM_FLTAWCFR), (threshold == DFSDM_AWD_HIGH_THRESHOLD) ? \
                                        (1 << (8 + channel)) : (1 << channel));

		/*
		 *  ���ʥ륰�����å��ɥå���������Хå�
		 */
		if(hdfsf->awdconvcallback != NULL)
			hdfsf->awdconvcallback(hdfsf);
	}
	/*
	 *  ����å����֥���ȯ��
	 */
	else if(hdfsf->base == TADR_DFSDM1_FILTER0_BASE &&
		(isr & DFSDM_FLTISR_CKABF) != 0 && (cr2 & DFSDM_FLTCR2_CKABIE) != 0){
		uint32_t reg = (sil_rew_mem((uint32_t *)(hdfsf->base+TOFF_DFSDM_FLTISR)) & DFSDM_FLTISR_CKABF) >> 16;
		uint32_t channel = 0;

		while(channel < DFSDM1_CHANNEL_NUMBER){
			/*
			 *  ͭ������ͥ륵����
			 */
			if(((reg & 1) != 0) && (Dfsdm1ChannelHandle[channel] != NULL)){
				/*
				 *  ����å����֥��󥹡����͡��֥�Υ���ͥ������å�
				 */
				if((sil_rew_mem((uint32_t *)(Dfsdm1ChannelHandle[channel]->base+TOFF_DFSDM_CHCFGR1)) & DFSDM_CHCFGR1_CKABEN) != 0){
					/*
					 *  ����å����֥��󥹡��ե饰���ꥢ
					 */
					sil_wrw_mem((uint32_t *)(hdfsf->base+TOFF_DFSDM_FLTICR), (1 << (16 + channel)));

					/*
					 *  �б�����ͥ�Υ�����Хå�
					 */
					hdfsc = Dfsdm1ChannelHandle[channel];
					if(hdfsc != NULL && hdfsc->ckabcallback != NULL)
						hdfsc->ckabcallback(hdfsc);
				}
			}
			channel++;
			reg = reg >> 1;
		}
	}
	/*
	 *  ���硼�ȥ������åȡ��ǥƥ������ȯ��
	 */
	else if(hdfsf->base == TADR_DFSDM1_FILTER0_BASE &&
		(isr & DFSDM_FLTISR_SCDF) != 0 && (cr2 & DFSDM_FLTCR2_SCDIE) != 0){
		uint32_t reg = 0;
		uint32_t channel = 0;

		/*
		 *  ����ͥ��ֹ����
		 */
		reg = (sil_rew_mem((uint32_t *)(hdfsf->base+TOFF_DFSDM_FLTISR)) & DFSDM_FLTISR_SCDF) >> 24;
		while((reg & 1) == 0){
			channel++;
			reg = reg >> 1;
		}

		/*
		 *  ���硼�ȥ������åȡ��ǥƥ������ե饰���ꥢ
		 */
		sil_wrw_mem((uint32_t *)(hdfsf->base+TOFF_DFSDM_FLTICR), (1 << (24 + channel)));

		/*
		 *  ���硼�ȥ������åȡ��ǥƥ�����󡦥�����Хå�
		 */
		hdfsc = Dfsdm1ChannelHandle[channel];
		if(hdfsc != NULL && hdfsc->scdcallback != NULL)
			hdfsc->scdcallback(hdfsc);
	}
}

