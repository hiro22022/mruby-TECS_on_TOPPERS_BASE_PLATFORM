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
 *  @(#) $Id: adc.c 698 2017-07-11 21:03:58Z roi $
 */
/*
 *
 *  ADC�ɥ饤�дؿ���
 *
 */
#include <kernel.h>
#include <t_syslog.h>
#include <t_stdlib.h>
#include <stdio.h>
#include <string.h>
#include "syssvc/serial.h"
#include "syssvc/syslog.h"
#include <target_syssvc.h>
#include "device.h"
#include "adc.h"

/*
 *  SIL�ؿ��Υޥ������
 */
#define sil_orw_mem(a, b)		sil_wrw_mem((a), sil_rew_mem(a) | (b))
#define sil_andw_mem(a, b)		sil_wrw_mem((a), sil_rew_mem(a) & ~(b))
#define sil_modw_mem(a, b, c)	sil_wrw_mem((a), (sil_rew_mem(a) & (~b)) | (c))

/*
 *  ADC�ݡ���ID��������֥�å�����Ф�����Υޥ���
 */
#define INDEX_ADC(adcid)        ((uint_t)((adcid) - 1))

#define INIT_CFGR1              (ADC_CFGR1_ALIGN | ADC_CFGR1_SCANDIR | ADC_CFGR1_EXTSEL | \
								 ADC_CFGR1_EXTEN | ADC_CFGR1_CONT    | ADC_CFGR1_DMACFG | \
								 ADC_CFGR1_OVRMOD | ADC_CFGR1_WAIT   | ADC_CFGR1_AUTOFF | ADC_CFGR1_DISCEN)

#define INIT_IER                (ADC_IER_ADRDYIE | ADC_IER_EOSMPIE | ADC_IER_EOCIE | \
								 ADC_IER_EOSIE   | ADC_IER_OVRIE   | ADC_IER_AWD1IE )

#define INIT_ISR                (ADC_ISR_ADRDY | ADC_ISR_EOSMP | ADC_ISR_EOC | \
								 ADC_ISR_EOS   | ADC_ISR_OVR   | ADC_ISR_AWD1 )


/*
 *  ADC�����ॢ���Ȼ���
 */
#define ADC_ENABLE_TIMEOUT            (10*1000)	/* 10ms */
#define ADC_DISABLE_TIMEOUT           (10*1000)	/* 10ms */
#define ADC_STOP_CONVERSION_TIMEOUT   (10*1000)	/* 10ms */

/*
 *  ADC�¹��Ԥ�����(��sec)
 */
#define ADC_STAB_DELAY_US              3
/*
 *  ADC���٥��󥵡��¹��Ԥ�����(��sec)
 */
#define ADC_TEMPSENSOR_DELAY_US        10

/*
 *  ADC�ݡ�������ơ��֥�
 */
static const ADC_PortControlBlock adc_pcb[NUM_ADCPORT] = {
  {	TADR_ADC1_BASE,
	(TADR_RCC_BASE+TOFF_RCC_APB2ENR),  RCC_APB2ENR_ADCEN,
	(TADR_RCC_BASE+TOFF_RCC_AHBENR),   RCC_AHBENR_DMA1EN,
	TADR_DMA1_CH1_BASE, DMA_REQUEST_1 }
};

static ADC_Handle_t  AdcHandle[NUM_ADCPORT];

/*
 *  ADC DMAž����λ������Хå��ؿ�
 */
static void
adc_dmatrans_func(DMA_Handle_t *hdma)
{
	ADC_Handle_t* hadc = ( ADC_Handle_t* )((DMA_Handle_t* )hdma)->localdata;

	if(hadc->Init.ContinuousConvMode == ADC_CONTINUOUS_DISABLE)
		hadc->status = ADC_STATUS_EOC;
	else
		hadc->status = ADC_STATUS_BUSY_EOC;
	if(hadc->xfercallback != NULL)
		hadc->xfercallback(hadc);
}

/*
 *  ADC DMA�ϡ���ž��������Хå��ؿ�
 */
static void
adc_dmahalftrans_func(DMA_Handle_t *hdma)
{
	ADC_Handle_t* hadc = ( ADC_Handle_t* )((DMA_Handle_t* )hdma)->localdata;

	if(hadc->xferhalfcallback != NULL)
		hadc->xferhalfcallback(hadc);
}

/*
 *  ADC DMA���顼������Хå��ؿ�
 */
static void
adc_dmaerror_func(DMA_Handle_t *hdma)
{
	ADC_Handle_t* hadc = ( ADC_Handle_t* )((DMA_Handle_t* )hdma)->localdata;

	hadc->status = ADC_STATUS_READY;
	hadc->errorcode |= ADC_ERROR_DMA;
	if(hadc->errorcallback != NULL)
		hadc->errorcallback(hadc);
}

/*
 *  ADCͭ����
 */
static ER
adc_enable(ADC_Handle_t* hadc)
{
	ER ercd = E_OK;
	uint32_t tick = 0;

	if((sil_rew_mem((uint32_t *)(hadc->base+TOFF_ADC_CR)) & (ADC_CR_ADEN | ADC_CR_ADDIS)) != ADC_CR_ADEN ||
			 ((sil_rew_mem((uint32_t *)(hadc->base+TOFF_ADC_ISR)) & ADC_ISR_ADRDY) == 0 && 
				(sil_rew_mem((uint32_t *)(hadc->base+TOFF_ADC_CFGR1)) & ADC_CFGR1_AUTOFF) == 0)){
		if((sil_rew_mem((uint32_t *)(hadc->base+TOFF_ADC_CR)) & (ADC_CR_ADCAL | ADC_CR_ADSTP | ADC_CR_ADSTART | ADC_CR_ADDIS | ADC_CR_ADEN)) != 0){
			hadc->status = ADC_STATUS_ERROR;
			hadc->errorcode |= ADC_ERROR_INT;
			ercd = E_SYS;
		}
		else{
			/*
			 *  ADC ENABLE
			 */
			sil_orw_mem((uint32_t *)(hadc->base+TOFF_ADC_CR), ADC_CR_ADEN);
			sil_dly_nse(ADC_STAB_DELAY_US * 1000);
			while((sil_rew_mem((uint32_t *)(hadc->base+TOFF_ADC_ISR)) & ADC_ISR_ADRDY) == 0){
				if(tick > ADC_ENABLE_TIMEOUT){
					hadc->status = ADC_STATUS_ERROR;
					hadc->errorcode |= ADC_ERROR_INT;
					ercd = E_TMOUT;
					break;
				}
				tick++;
				sil_dly_nse(1000);
			}
		}
	}
	return ercd;
}

/*
 *  ADC̵����
 */
static ER
adc_disable(ADC_Handle_t* hadc)
{
	ER ercd = E_OK;
	uint32_t tick = 0;

	if((sil_rew_mem((uint32_t *)(hadc->base+TOFF_ADC_CR)) & (ADC_CR_ADEN | ADC_CR_ADDIS)) == ADC_CR_ADEN && 
			((sil_rew_mem((uint32_t *)(hadc->base+TOFF_ADC_ISR)) & ADC_ISR_ADRDY) != 0
				|| (sil_rew_mem((uint32_t *)(hadc->base+TOFF_ADC_CFGR1)) & ADC_CFGR1_AUTOFF) != 0)){
		/*
		 *  ADC DISABLE
		 */
		sil_orw_mem((uint32_t *)(hadc->base+TOFF_ADC_CR), ADC_CR_ADDIS);
		sil_wrw_mem((uint32_t *)(hadc->base+TOFF_ADC_ISR), (ADC_ISR_ADRDY | ADC_ISR_EOSMP));
		while((sil_rew_mem((uint32_t *)(hadc->base+TOFF_ADC_CR)) & ADC_CR_ADEN) != 0){
			if(tick > ADC_DISABLE_TIMEOUT){
				hadc->errorcode = ADC_ERROR_INT;
				ercd = E_TMOUT;
			}
			tick++;
			sil_dly_nse(1000);
		}
	}
	return ercd;
}

/*
 *  ADC����С���������
 */
static ER
adc_conversionstop(ADC_Handle_t* hadc)
{
	ER ercd = E_OK;
	uint32_t tick = 0;

	/*
	 *  ADC����С�����󥹥����Ⱦ��֤ʤ������Ԥ���Ԥ�
	 */
	if((sil_rew_mem((uint32_t *)(hadc->base+TOFF_ADC_CR)) & ADC_CR_ADSTART) != 0){
		if((sil_rew_mem((uint32_t *)(hadc->base+TOFF_ADC_CR)) & ADC_CR_ADDIS) == 0)
			sil_orw_mem((uint32_t *)(hadc->base+TOFF_ADC_CR), ADC_CR_ADSTP);
		while((sil_rew_mem((uint32_t *)(hadc->base+TOFF_ADC_CR)) & ADC_CR_ADSTART) != 0){
			if(tick > ADC_STOP_CONVERSION_TIMEOUT){
				hadc->status = ADC_STATUS_ERROR;
				hadc->errorcode |= ADC_ERROR_INT;
				ercd = E_TMOUT;
				break;
			}
			tick++;
			sil_dly_nse(1000);
		}
	}
	return ercd;
}

/*
 *  ADC�������
 *  parameter1  port: ADC�ݡ����ֹ�
 *  parameter2  pini: ADC������깽¤�ΤؤΥݥ���
 *  return ADC�ϥ�ɥ�ؤΥݥ��󥿡�NULL�ǥ��顼
 */
ADC_Handle_t *
adc_init(ID portid, ADC_Init_t* pini)
{
	static DMA_Handle_t  hdma_adc;
	ADC_Handle_t *hadc;
	const ADC_PortControlBlock *apcb;

	/*
	 *  ����ѥ�᡼�������å�
	 */
	if(portid < 1 || portid > NUM_ADCPORT || pini == NULL)
		return NULL;

	if(pini->DiscontinuousConvMode != 0 && pini->ContinuousConvMode != 0)
		return NULL;

	hadc = &AdcHandle[INDEX_ADC(portid)];
	memcpy(&hadc->Init, pini, sizeof(ADC_Init_t));

	apcb = &adc_pcb[INDEX_ADC(portid)];
	hadc->base = apcb->base;

	/*
	 *  ADC����å�����
	 */
	sil_orw_mem((uint32_t *)(apcb->adcclockbase), apcb->adcclockbit);
	sil_rew_mem((uint32_t *)(apcb->adcclockbase));

	/*
	 *  DMA�б�������С�DMA�����Ԥ�
	 */
	if(apcb->dmaclockbase != 0 && pini->DMAContinuousRequests != ADC_DMACONTINUOUS_DISABLE){
		sil_orw_mem((uint32_t *)(apcb->dmaclockbase), apcb->dmaclockbit);
		sil_rew_mem((uint32_t *)(apcb->dmaclockbase));

		hdma_adc.cbase                    = apcb->dmarxchannel;
		hdma_adc.Init.Request             = apcb->dmarxrequest;
		hdma_adc.Init.Direction           = DMA_PERIPH_TO_MEMORY;
		hdma_adc.Init.PeriphInc           = DMA_PINC_DISABLE;
		hdma_adc.Init.MemInc              = DMA_MINC_ENABLE;
		hdma_adc.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
		hdma_adc.Init.MemDataAlignment    = DMA_MDATAALIGN_HALFWORD;
		hdma_adc.Init.Mode                = DMA_CIRCULAR;
		hdma_adc.Init.Priority            = DMA_PRIORITY_MEDIUM;

		dma_init(&hdma_adc);

		hadc->hdmarx = &hdma_adc;
		hdma_adc.localdata = hadc;
	}
	else{
		hadc->hdmarx = NULL;
	}

	/*
	 *  ADC�Υץꥹ����������(���������)
	 */
	sil_modw_mem((uint32_t *)(hadc->base+TOFF_ADC_CFGR2), ADC_CFGR2_CKMODE, hadc->Init.ClockPrescaler);

	/*
	 *  ADC�������
	 */
	sil_modw_mem((uint32_t *)(hadc->base+TOFF_ADC_CFGR1), ADC_CFGR1_RES, hadc->Init.Resolution);
	sil_modw_mem((uint32_t *)(hadc->base+TOFF_ADC_CFGR1), INIT_CFGR1, (hadc->Init.DataAlign |
							hadc->Init.ScanConvMode | hadc->Init.ContinuousConvMode |
							hadc->Init.DiscontinuousConvMode | hadc->Init.DMAContinuousRequests |
							hadc->Init.Overrun | hadc->Init.LowPowerAutoWait | hadc->Init.LowPowerAutoPowerOff));

	/*
	 *  ADC�����ȥꥬ����
	 */
	if(hadc->Init.ExternalTrigConv != ADC_SOFTWARE_START){
		sil_modw_mem((uint32_t *)(hadc->base+TOFF_ADC_CFGR1), (ADC_CFGR1_EXTSEL | ADC_CFGR1_EXTEN),
			(hadc->Init.ExternalTrigConv | hadc->Init.ExternalTrigConvEdge));
	}

	/*
	 *  ����ץ�󥰻�������
	 */
	sil_modw_mem((uint32_t *)(hadc->base+TOFF_ADC_SMPR), ADC_SMPR_SMP, hadc->Init.SamplingTime);

	/*
	 *  ADC�������֤�
	 */
	hadc->errorcode = ADC_ERROR_NONE;
	hadc->status = ADC_STATUS_READY;
	return hadc;
}

/*
 *  ADC��λ����
 *  parameter1  hadc: ADC�ϥ�ɥ�ؤΥݥ���
 *  return ER������
 */
ER
adc_deinit(ADC_Handle_t* hadc)
{
	ER ercd = E_OK;

	if(hadc == NULL)
		return E_PAR;

	ercd = adc_conversionstop(hadc);
	if(ercd == E_OK){
		/*
		 *  ADC̵����
		 */
		ercd = adc_disable(hadc);
	}

	if(ercd == E_OK){
		/*
		 *  �ꥻ�å�IER/ISR
		 */
		sil_andw_mem((uint32_t *)(hadc->base+TOFF_ADC_IER), INIT_IER);
		sil_wrw_mem((uint32_t *)(hadc->base+TOFF_ADC_ISR), INIT_ISR);

		/*
		 *  �ꥻ�å�CFGR1/CGFR2
		 */
		sil_andw_mem((uint32_t *)(hadc->base+TOFF_ADC_CFGR1), (INIT_CFGR1 | ADC_CFGR1_AWD1CH  | ADC_CFGR1_AWD1EN |
															ADC_CFGR1_RES | ADC_CFGR1_DMAEN));
		sil_andw_mem((uint32_t *)(hadc->base+TOFF_ADC_CFGR2), ADC_CFGR2_CKMODE);

		/*
		 *  �ꥻ�å�SMPR
		 */
		sil_andw_mem((uint32_t *)(hadc->base+TOFF_ADC_SMPR), ADC_SMPR_SMP);

		/*
		 *  �ꥻ�å�TR
		 */
		sil_andw_mem((uint32_t *)(hadc->base+TOFF_ADC_TR), (ADC_TR1_LT1 | ADC_TR1_HT1));

		/*
		 *  �ꥻ�å�CHSELR
		 */
		sil_andw_mem((uint32_t *)(hadc->base+TOFF_ADC_CHSELR), ADC_CHSELR_CHSEL);
	}

	/*
	 *  DMA���
	 */
	if(hadc->hdmarx != NULL)
		dma_deinit(hadc->hdmarx);

	/*
	 *  ADC�ξ��ֽ����
	 */
	hadc->errorcode = ADC_ERROR_NONE;
	hadc->status = ADC_STATUS_RESET;
	return ercd;
}

/*
 *  ADC-����ߥ⡼�ɳ��Ͻ���
 *  parameter1  hadc: ADC�ϥ�ɥ�ؤΥݥ���
 *  return ER������
 */
ER
adc_start_int(ADC_Handle_t* hadc)
{
	ER ercd = E_OK;

	if(hadc == NULL)
		return E_PAR;

	if((sil_rew_mem((uint32_t *)(hadc->base+TOFF_ADC_CR)) & ADC_CR_ADSTART) != 0)
		return E_OBJ;

    /*
	 *  ADCͭ����
	 */
	if(hadc->Init.LowPowerAutoPowerOff != ADC_LOWAUTOPOWEROFF_ENABLE)
		ercd = adc_enable(hadc);

	if(ercd == E_OK){
		/*
		 *  �¹Ծ��֤˰ܹ�
		 */
		hadc->status = ADC_STATUS_BUSY;
		hadc->errorcode = ADC_ERROR_NONE;

		/*
		 *  ����ߥ��ꥢ
		 */
		sil_wrw_mem((uint32_t *)(hadc->base+TOFF_ADC_ISR), (ADC_ISR_EOC | ADC_ISR_EOS | ADC_ISR_OVR));

		/*
		 *  ADC���������
		 */
		if(hadc->Init.EOCSelection == ADC_EOC_SEQ_CONV)
			sil_andw_mem((uint32_t *)(hadc->base+TOFF_ADC_IER), ADC_IER_EOCIE);
		else
			sil_orw_mem((uint32_t *)(hadc->base+TOFF_ADC_IER), ADC_IER_EOCIE);
		sil_orw_mem((uint32_t *)(hadc->base+TOFF_ADC_IER), (ADC_IER_EOSIE | ADC_IER_OVRIE));

		/*
		 *  ADC��������
		 */
		sil_orw_mem((uint32_t *)(hadc->base+TOFF_ADC_CR), ADC_CR_ADSTART);
	}
	return ercd;
}

/*
 *  ADC-����ߥ⡼����߽���
 *  parameter1  hadc: ADC�ϥ�ɥ�ؤΥݥ���
 *  return ER������
 */
ER
adc_end_int(ADC_Handle_t* hadc)
{
	ER ercd = E_OK;

	if(hadc == NULL)
		return E_PAR;

	/*
	 *  ADC����С�����󥹥����Ⱦ��֤ʤ������Ԥ���Ԥ�
	 */
	ercd = adc_conversionstop(hadc);

	/*
	 *  ADC��߽���
	 */
	if(ercd == E_OK){
		/*
		 *  ž�������顼��������
		 */
		sil_andw_mem((uint32_t *)(hadc->base+TOFF_ADC_IER), (ADC_IER_EOCIE | ADC_IER_EOSIE | ADC_IER_OVRIE));

		/*
		 *  ADC̵����
		 */
		ercd = adc_disable(hadc);
	}
	if(ercd == E_OK)
		hadc->status = ADC_STATUS_READY;
	return ercd;
}

/*
 *  ADC-DMA�⡼�ɳ��Ͻ���
 *  parameter1  hadc: ADC�ϥ�ɥ�ؤΥݥ���
 *  parameter2  pdata: �����ΰ�Υݥ���
 *  parameter3  length: �����ǡ���Ĺ
 *  return ER������
 */
ER
adc_start_dma(ADC_Handle_t* hadc, uint16_t* pdata, uint32_t length)
{
	ER ercd = E_OK;

	if(hadc == NULL)
		return E_PAR;

	if((sil_rew_mem((uint32_t *)(hadc->base+TOFF_ADC_CR)) & ADC_CR_ADSTART) != 0)
		return E_OBJ;

    /*
	 *  ADCͭ����
	 */
	if(hadc->Init.LowPowerAutoPowerOff != ADC_LOWAUTOPOWEROFF_ENABLE)
		ercd = adc_enable(hadc);

    /*
	 *  ADC-DMA�������Ƚ���
	 */
    if(ercd == E_OK){
		/*
		 *  DMA������Хå��ؿ�����
		 */
		hadc->hdmarx->xfercallback = adc_dmatrans_func;
		hadc->hdmarx->xferhalfcallback = adc_dmahalftrans_func;
		hadc->hdmarx->errorcallback = adc_dmaerror_func;

		/*
		 *  ����ߥ��ꥢ
		 */
		sil_wrw_mem((uint32_t *)(hadc->base+TOFF_ADC_ISR), (ADC_ISR_EOC | ADC_ISR_EOS | ADC_ISR_OVR));

		/*
		 *  ADC�����С������������
		 */
		sil_orw_mem((uint32_t *)(hadc->base+TOFF_ADC_IER), ADC_IER_OVRIE);

		/*
		 *  ADC DMA����
		 */
		sil_orw_mem((uint32_t *)(hadc->base+TOFF_ADC_CFGR1), ADC_CFGR1_DMAEN);

		/*
		 *  �¹Ծ��֤˰ܹ�
		 */
		hadc->status = ADC_STATUS_BUSY;
		hadc->errorcode = ADC_ERROR_NONE;

		/*
		 *  DMA��������
		 */
		dma_start(hadc->hdmarx, hadc->base+TOFF_ADC_DR, (uint32_t)pdata, length);

		/*
		 *  ADC��������
		 */
		sil_orw_mem((uint32_t *)(hadc->base+TOFF_ADC_CR), ADC_CR_ADSTART);
	}
	return ercd;
}

/*
 *  ADC-DMA�⡼����߽���
 *  parameter1  hadc: ADC�ϥ�ɥ�ؤΥݥ���
 *  return ER������
 */
ER
adc_end_dma(ADC_Handle_t* hadc)
{
	ER ercd = E_OK;

	if(hadc == NULL)
		return E_PAR;

	/*
	 *  ADC����С�����󥹥����Ⱦ��֤ʤ������Ԥ���Ԥ�
	 */
	ercd = adc_conversionstop(hadc);

	/*
	 *  ADC��߽���
	 */
	if(ercd == E_OK){
		/*
		 *  ADC��DMA�⡼�ɤ򥪥�
		 */
		sil_andw_mem((uint32_t *)(hadc->base+TOFF_ADC_CFGR1), ADC_CFGR1_DMAEN);

		/*
		 *  DMA�����
		 */
		if(hadc->hdmarx != NULL)
			ercd = dma_end(hadc->hdmarx);
		if(ercd != E_OK)
			hadc->status |= ADC_STATUS_ERROR;

		/*
		 *  �����С�������ߤ����
		 */
		sil_andw_mem((uint32_t *)(hadc->base+TOFF_ADC_IER), ADC_IER_OVRIE);

		/*
		 *  ADC̵����
		 */
		ercd = adc_disable(hadc);
	}
	if(ercd == E_OK)
		hadc->status = ADC_STATUS_READY;
	return ercd;
}

/*
 *  ADC ����ߥϥ�ɥ�¹Դؿ�
 */
void
adc_handler(ADC_Handle_t* hadc)
{
	uint32_t isr = sil_rew_mem((uint32_t *)(hadc->base+TOFF_ADC_ISR));
	uint32_t ier = sil_rew_mem((uint32_t *)(hadc->base+TOFF_ADC_IER));
	uint32_t cr1 = sil_rew_mem((uint32_t *)(hadc->base+TOFF_ADC_CFGR1));

	if(((isr & ier) & (ADC_ISR_EOC | ADC_ISR_EOS)) != 0){
	    /*
		 *  ����С������λ���֤˰ܹ�
		 */
	    if(hadc->errorcode == ADC_ERROR_NONE)
			hadc->status = ADC_STATUS_EOC;

	    /*
		 *  ADC��߽���
		 */
	    if((cr1 & ADC_CFGR1_EXTEN) == 0 && hadc->Init.ContinuousConvMode == ADC_CONTINUOUS_DISABLE){
		    /*
			 *  END OF SEQUENCE������
			 */
		    if((isr & ADC_ISR_EOS) != 0){
		        /*
				 *  EOSEQ������ѽ�λ������ADSTART�ξ��֤��ǧ
				 */
				if((sil_rew_mem((uint32_t *)(hadc->base+TOFF_ADC_CR)) & ADC_CR_ADSTART) == 0){
					/*
					 *  ����ߥޥ���
					 */
					sil_andw_mem((uint32_t *)(hadc->base+TOFF_ADC_IER), (ADC_IER_EOCIE | ADC_IER_EOSIE));
					hadc->status = ADC_STATUS_BUSY_EOC;
				}
				else{	/* ���󥿡��ʥ륨�顼 */
					hadc->status |= ADC_STATUS_ERROR;
					hadc->errorcode = ADC_ERROR_INT;
				 }
			}
		}

		/*
		 *  ��λ������Хå��ؿ����ɤ߽Ф�
		 */
		if(hadc->xfercallback != NULL)
			hadc->xfercallback(hadc);

	    /*
		 *  ���ѥͭ���⡼�ɤǤʤ���г���ߥ��ꥢ
		 */
	    if(hadc->Init.LowPowerAutoWait != ADC_LOWAUTOWAIT_ENABLE){
			sil_wrw_mem((uint32_t *)(hadc->base+TOFF_ADC_ISR), (ADC_ISR_EOC | ADC_ISR_EOS));
		}
	}

	/*
	 *  ���ʥ������å��ɥå�Ƚ��
	 */
	if((isr & ADC_ISR_AWD1) != 0 && (cr1 & ADC_CFGR1_AWD1EN) != 0){
		/*
		 *  �����å��ɥå�ȯ�����֤��ѹ�
		 */
		hadc->status = ADC_STATUS_AWD;

		/*
		 *  �����å��ɥå�����ߥ��ꥢ
		 */
		sil_wrw_mem((uint32_t *)(hadc->base+TOFF_ADC_ISR), ADC_ISR_AWD1);

		/*
		 *  ������ɥ��ΰ襢���ȤΥ�����Хå��ؿ��ɤ߽Ф�
		 */
		if(hadc->outofwincallback != NULL)
			hadc->outofwincallback(hadc);
	}

	/*
	 *  �����С���󥨥顼Ƚ��
	 */
	if((isr & ADC_ISR_OVR) != 0 && (ier & ADC_IER_OVRIE) != 0){
		/*
		 *  �����С�������ߥ��ꥢ
		 */
		sil_wrw_mem((uint32_t *)(hadc->base+TOFF_ADC_ISR), ADC_ISR_OVR);

	    /*
		 * �����С�������ꡧADC_OVR_DATA_PRESERVED���ޤ���DMA����ǥ��顼������Ԥ�
		 */
	    if(hadc->Init.Overrun == ADC_OVR_DATA_PRESERVED ||
			(sil_rew_mem((uint32_t *)(hadc->base+TOFF_ADC_CFGR1)) & ADC_CFGR1_DMAEN) != 0){

			/*
			 *  �����С����ȯ�����֤��ѹ�
			 */
			hadc->status = ADC_STATUS_READY;
			hadc->errorcode |= ADC_ERROR_OVR;

			/*
			 *  ���顼������Хå��ؿ����ɤ߽Ф�
			 */
			if(hadc->errorcallback != NULL)
				hadc->errorcallback(hadc);
	    }
	}
}

/*
 *  ADC����ߥϥ�ɥ�
 */
void
adc_int_handler(void)
{
	uint32_t i;
	ADC_Handle_t* hadc = &AdcHandle[0];

	for(i = 0 ; i < NUM_ADCPORT ; i++, hadc++){
		if(hadc->status > ADC_STATUS_RESET)
			adc_handler(hadc);
	}
}


/*
 *  �쥮��顼�쥸�����μ��Ф�
 *  parameter1  hadc: ADC�ϥ�ɥ�ؤΥݥ���
 *  return �쥮��顼�쥸������
 */
uint32_t
adc_getvalue(ADC_Handle_t* hadc)
{
	return sil_rew_mem((uint32_t *)(hadc->base+TOFF_ADC_DR));
}

/*
 *  ADC����ͥ�����
 *  parameter1  hadc: ADC�ϥ�ɥ�ؤΥݥ���
 *  parameter2  sConfig: ����ͥ����깽¤�ΤؤΥݥ���
 *  return ER������
 */
ER
adc_setupchannel(ADC_Handle_t* hadc, ADC_ChannelConf_t* sConfig)
{
	GPIO_Init_t GPIO_InitStruct;
	uint32_t clkpin;

	if(hadc == NULL || sConfig == NULL)
		return E_PAR;

	clkpin = (sConfig->GpioBase - TADR_GPIOA_BASE) / 0x400;
	if(clkpin >= 7 || sConfig->GpioPin >= 16)
		return E_PAR;

	/*
	 *  ����ͥ�ͭ������
	 */
	sil_orw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_AHBENR), 1<<(clkpin+17));
	sil_rew_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_AHBENR));

	GPIO_InitStruct.mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.pull = GPIO_NOPULL;
	gpio_setup(sConfig->GpioBase, &GPIO_InitStruct, sConfig->GpioPin);

	/*
	 *  �������
	 */
	if(sConfig->Rank != ADC_RANK_NONE){
		/*
		 *  ����ͥ�����
		 */
		sil_orw_mem((uint32_t *)(hadc->base+TOFF_ADC_CHSELR), (sConfig->Channel & ADC_CHSELR_CHSEL));

		/*
		 *  ���٥��󥵡�����
		 */
	    if(sConfig->Channel == ADC_CHANNEL_TEMPSENSOR){
			sil_orw_mem((uint32_t *)(TADR_ADC_BASE+TOFF_ADC_CCR), ADC_CCR_TSEN);
			sil_dly_nse(ADC_TEMPSENSOR_DELAY_US*1000);
		}

		/*
		 *  VREFINT����ͥ�����
		 */
		if(sConfig->Channel == ADC_CHANNEL_VREFINT){
			sil_orw_mem((uint32_t *)(TADR_ADC_BASE+TOFF_ADC_CCR), ADC_CCR_VREFEN);
		}

		/*
		 *  VBAT����ͥ�����
		 */
		if(sConfig->Channel == ADC_CHANNEL_VBAT){
			sil_orw_mem((uint32_t *)(TADR_ADC_BASE+TOFF_ADC_CCR), ADC_CCR_VBATEN);
		}
	}
	else{
		/*
		 *  ����ͥ�����ꥻ�å�
		 */
		sil_andw_mem((uint32_t *)(hadc->base+TOFF_ADC_CHSELR), (sConfig->Channel & ADC_CHSELR_CHSEL));

		/*
		 *  ���٥��󥵡�������
		 */
		if(sConfig->Channel == ADC_CHANNEL_TEMPSENSOR){
			sil_andw_mem((uint32_t *)(TADR_ADC_BASE+TOFF_ADC_CCR), ADC_CCR_TSEN);
		}

		/*
		 *  VREFINT����ͥ�������
		 */
		if(sConfig->Channel == ADC_CHANNEL_VREFINT){
			sil_andw_mem((uint32_t *)(TADR_ADC_BASE+TOFF_ADC_CCR), ADC_CCR_VREFEN);
		}

		/*
		 *  VBAT����ͥ�������
		 */
		if(sConfig->Channel == ADC_CHANNEL_VBAT){
			sil_andw_mem((uint32_t *)(TADR_ADC_BASE+TOFF_ADC_CCR), ADC_CCR_VBATEN);
		}
	}
	return E_OK;
}

/*
 *  ADC���ʥ������å��ɥå�����
 *  parameter1  hadc: ADC�ϥ�ɥ�ؤΥݥ���
 *  parameter2  AnalogWDGConfig: ���ʥ������å��ɥå����깽¤�ΤؤΥݥ���
 *  return ER������
 */
ER
adc_setupwatchdog(ADC_Handle_t* hadc, ADC_AnalogWDGConf_t* AnalogWDGConfig)
{
	uint32_t shift;

	if(hadc == NULL || AnalogWDGConfig == NULL)
		return E_PAR;

	if((sil_rew_mem((uint32_t *)(hadc->base+TOFF_ADC_CR)) & ADC_CR_ADSTART) != 0)
		return E_OBJ;

	/*
	 *  ���������
	 */
	sil_wrw_mem((uint32_t *)(hadc->base+TOFF_ADC_ISR), ADC_ISR_AWD1);
	sil_modw_mem((uint32_t *)(hadc->base+TOFF_ADC_IER), ADC_IER_AWD1IE, AnalogWDGConfig->ITMode);

	/*
	 *  �⡼������
	 */
	sil_andw_mem((uint32_t *)(hadc->base+TOFF_ADC_CFGR1), (ADC_CFGR1_AWD1SGL | ADC_CFGR1_AWD1EN | ADC_CFGR1_AWD1CH));

	/* Set the analog watchdog enable mode */
	sil_orw_mem((uint32_t *)(hadc->base+TOFF_ADC_CFGR1), (AnalogWDGConfig->WatchdogMode |
									(AnalogWDGConfig->Channel & ADC_CFGR1_AWD1CH)));

	/*
	 *  HIGH/LOW�Υ���å���ۡ��������
	 */
	shift = ((sil_rew_mem((uint32_t *)(hadc->base+ADC_CFGR1_RES)) & ADC_CFGR1_RES) >> 3) * 2;
	sil_wrw_mem((uint32_t *)(hadc->base+TOFF_ADC_TR), ((AnalogWDGConfig->HighThreshold << (shift+16)) | (AnalogWDGConfig->LowThreshold<<shift)));
	return E_OK;
}


