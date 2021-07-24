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
#define INDEX_ADC(adcid)    ((uint_t)((adcid) - 1))

#define INIT_CFGR           (ADC_CFGR_RES    | ADC_CFGR_ALIGN  | ADC_CFGR_CONT | \
							 ADC_CFGR_OVRMOD | ADC_CFGR_DISCEN | ADC_CFGR_DISCNUM | \
							 ADC_CFGR_EXTEN  | ADC_CFGR_EXTSEL)

#define INIT_CFGR_2         (ADC_CFGR_DMACFG | ADC_CFGR_AUTDLY)

#define INIT_CFGR2          (ADC_CFGR2_ROVSE | ADC_CFGR2_OVSR  | ADC_CFGR2_OVSS | \
							 ADC_CFGR2_TROVS | ADC_CFGR2_ROVSM)

#define INIT_IER            (ADC_IER_AWD3IE | ADC_IER_AWD2IE  | ADC_IER_AWD1IE | ADC_IER_JQOVFIE | \
							 ADC_IER_OVRIE  | ADC_IER_JEOSIE  | ADC_IER_JEOCIE | ADC_IER_EOSIE   | \
							 ADC_IER_EOCIE  | ADC_IER_EOSMPIE | ADC_IER_ADRDYIE)

#define INIT_ISR            (ADC_ISR_AWD3 | ADC_ISR_AWD2  | ADC_ISR_AWD1 | ADC_ISR_JQOVF | \
							 ADC_ISR_OVR  | ADC_ISR_JEOS  | ADC_ISR_JEOC | ADC_ISR_EOS   | \
							 ADC_ISR_EOC  | ADC_ISR_EOSMP | ADC_ISR_ADRDY)

#define INIT_OFR            (ADC_OFR1_OFFSET1 | ADC_OFR1_OFFSET1_CH | ADC_OFR1_OFFSET1_EN)

/*
 *  ADC�����ॢ���Ȼ���
 */
#define ADC_ENABLE_TIMEOUT            (10*1000)	/* 10ms */
#define ADC_DISABLE_TIMEOUT           (10*1000)	/* 10ms */
#define ADC_STOP_CONVERSION_TIMEOUT   (10*1000)	/* 10ms */
#define ADC_CONVERSION_TIME_MAX_CPU_CYCLES 167168	/* ADC conversion completion time-out value */

/*
 *  ADC�¹��Ԥ�����(��sec)
 */
#define ADC_STAB_DELAY_US              10
/*
 *  ADC���٥��󥵡��¹��Ԥ�����(��sec)
 */
#define ADC_TEMPSENSOR_DELAY_US        10




/*
 *  ADC����С������⡼�����
 */
#define ADC_MODE_INDEPENDENT               0x00000000							/* Independent ADC conversions mode */
#define ADC_DUALMODE_REGSIMULT_INJECSIMULT ADC_CCR_DUAL_0						/* Combined regular simultaneous + injected simultaneous mode */
#define ADC_DUALMODE_REGSIMULT_ALTERTRIG   ADC_CCR_DUAL_1						/* Combined regular simultaneous + alternate trigger mode     */
#define ADC_DUALMODE_REGINTERL_INJECSIMULT (ADC_CCR_DUAL_1 | ADC_CCR_DUAL_0)	/* Combined Interleaved mode + injected simultaneous mode     */
#define ADC_DUALMODE_INJECSIMULT           (ADC_CCR_DUAL_2 | ADC_CCR_DUAL_0)	/* Injected simultaneous mode only */
#define ADC_DUALMODE_REGSIMULT             (ADC_CCR_DUAL_2 | ADC_CCR_DUAL_1)	/* Regular simultaneous mode only */
#define ADC_DUALMODE_INTERL                (ADC_CCR_DUAL_2 | ADC_CCR_DUAL_1 | ADC_CCR_DUAL_0)	/* Interleaved mode only */
#define ADC_DUALMODE_ALTERTRIG             (ADC_CCR_DUAL_3 | ADC_CCR_DUAL_0)	/* Alternate trigger mode only */

/*
 *  ADC����С�����󥰥롼�����
 */
#define ADC_REGULAR_GROUP              (ADC_ISR_EOC | ADC_ISR_EOS)				/* ADC regular group selection */
#define ADC_INJECTED_GROUP             (ADC_ISR_JEOC | ADC_ISR_JEOS)			/* ADC injected group selection */
#define ADC_REGULAR_INJECTED_GROUP     (ADC_REGULAR_GROUP | ADC_INJECTED_GROUP)	/* ADC regular and injected groups selection */ 

/*
 *  ADC�ݡ�������ơ��֥�
 */
static const ADC_PortControlBlock adc_pcb[NUM_ADCPORT] = {
  {	TADR_ADC1_BASE,
	(TADR_RCC_BASE+TOFF_RCC_AHB2ENR),  RCC_AHB2ENR_ADCEN,
	(TADR_RCC_BASE+TOFF_RCC_AHB1ENR),  RCC_AHB1ENR_DMA1EN,
	TADR_DMA1_CH1_BASE, DMA_REQUEST_0 },

  {	TADR_ADC1_BASE,
	(TADR_RCC_BASE+TOFF_RCC_AHB2ENR),  RCC_AHB2ENR_ADCEN,
	(TADR_RCC_BASE+TOFF_RCC_AHB1ENR),  RCC_AHB1ENR_DMA1EN,
	TADR_DMA1_CH2_BASE, DMA_REQUEST_0 },

  {	TADR_ADC1_BASE,
	(TADR_RCC_BASE+TOFF_RCC_AHB2ENR),  RCC_AHB2ENR_ADCEN,
	(TADR_RCC_BASE+TOFF_RCC_AHB1ENR),  RCC_AHB1ENR_DMA1EN,
	TADR_DMA1_CH3_BASE, DMA_REQUEST_0 }
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
 *  ADC���͡��֥�Ƚ��
 */
static bool_t
is_adc_enable(uint32_t base)
{
	if((sil_rew_mem((uint32_t *)(base+TOFF_ADC_CR)) & (ADC_CR_ADEN | ADC_CR_ADDIS)) == ADC_CR_ADEN && 
			(sil_rew_mem((uint32_t *)(base+TOFF_ADC_ISR)) & ADC_ISR_ADRDY) != 0)
		return true;
	else
		return false;
}

/*
 *  ADCͭ����
 */
static ER
adc_enable(ADC_Handle_t* hadc)
{
	ER ercd = E_OK;
	uint32_t tick = 0;

	if(!is_adc_enable(hadc->base)){
		if((sil_rew_mem((uint32_t *)(hadc->base+TOFF_ADC_CR)) & (ADC_CR_ADCAL | ADC_CR_JADSTP | ADC_CR_ADSTP | \
				ADC_CR_JADSTART | ADC_CR_ADSTART | ADC_CR_ADDIS | ADC_CR_ADEN)) != 0){
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
					hadc->status |= ADC_STATUS_ERROR;
					hadc->errorcode = ADC_ERROR_INT;
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

	if(is_adc_enable(hadc->base)){
		if((sil_rew_mem((uint32_t *)(hadc->base+TOFF_ADC_CR)) & (ADC_CR_JADSTART | ADC_CR_ADSTART | ADC_CR_ADEN)) == ADC_CR_ADEN){
			/*
			 *  ADC DISABLE
			 */
			sil_orw_mem((uint32_t *)(hadc->base+TOFF_ADC_CR), ADC_CR_ADDIS);
			sil_wrw_mem((uint32_t *)(hadc->base+TOFF_ADC_ISR), (ADC_ISR_EOSMP | ADC_ISR_ADRDY));
		}
		else{
			hadc->status = ADC_STATUS_ERROR;
			hadc->errorcode |= ADC_ERROR_INT;
			return E_SYS;
		}
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
 * ADC DUAL REGULAR����С������⡼��
 */
static bool_t
adc_dual_regular_conversion(ADC_Handle_t* hadc)
{
	uint32_t adc_dual = sil_rew_mem((uint32_t *)(TADR_ADC_BASE+TOFF_ADC_CCR)) & ADC_CCR_DUAL;
	if(hadc->base != TADR_ADC2_BASE)
		return true;
	if(adc_dual == ADC_MODE_INDEPENDENT || adc_dual == ADC_DUALMODE_INJECSIMULT || adc_dual == ADC_DUALMODE_ALTERTRIG)
		return true;
	else
		return false;
}

/*
 *  ADC����С���������
 */
static ER
adc_conversionstop(ADC_Handle_t* hadc)
{
	ER ercd = E_OK;
	uint32_t ConversionGroup = ADC_REGULAR_INJECTED_GROUP;
	uint32_t tick = 0;
	uint32_t check_cr;

	/*
	 *  ADC����С�����󥹥����Ⱦ��֤ʤ������Ԥ���Ԥ�
	 */
	if((sil_rew_mem((uint32_t *)(hadc->base+TOFF_ADC_CR)) & (ADC_CR_ADSTART | ADC_CR_JADSTART)) != 0){
		/*
		 *  AUTO-INJECTION�⡼�ɤ����̤�����Ԥ�
		 */
		if((sil_rew_mem((uint32_t *)(hadc->base+TOFF_ADC_CFGR)) & ADC_CFGR_JAUTO) != 0 &&
			hadc->Init.ContinuousConvMode == ADC_CONTINUOUS_ENABLE &&
				hadc->Init.LowPowerAutoWait == ADC_LOWAUTOWAIT_ENABLE){
			/*
			 *  �쥮��顼����С�����󥰥롼�פ��ѹ�
			 */
			ConversionGroup = ADC_REGULAR_GROUP;

			/*
			 *  JEOS����Ԥ�
			 */
			while((sil_rew_mem((uint32_t *)(hadc->base+TOFF_ADC_ISR)) & ADC_ISR_JEOS) == 0){
				if(tick >= (ADC_CONVERSION_TIME_MAX_CPU_CYCLES *4)){
					hadc->status = ADC_STATUS_ERROR;
					hadc->errorcode |= ADC_ERROR_INT;
					return E_SYS;
				}
				tick++;
			}

			/*
			 * JEOS���ꥢ
			 */
			sil_wrw_mem((uint32_t *)(hadc->base+TOFF_ADC_ISR), ADC_ISR_JEOS);
		}

		/*
		 *  �쥮��顼����С�����󥰥롼�����
		 */
		if(ConversionGroup != ADC_INJECTED_GROUP){
			if((sil_rew_mem((uint32_t *)(hadc->base+TOFF_ADC_CR)) & (ADC_CR_ADSTART | ADC_CR_ADDIS)) == ADC_CR_ADSTART)
				sil_orw_mem((uint32_t *)(hadc->base+TOFF_ADC_CR), ADC_CR_ADSTP);
		}

		/*
		 *  INJJECTED����С�����󥰥롼�����
		 */
		if(ConversionGroup != ADC_REGULAR_GROUP){
			if((sil_rew_mem((uint32_t *)(hadc->base+TOFF_ADC_CR)) & (ADC_CR_JADSTART | ADC_CR_ADDIS)) == ADC_CR_JADSTART)
				sil_orw_mem((uint32_t *)(hadc->base+TOFF_ADC_CR), ADC_CR_JADSTP);
		}

		switch(ConversionGroup){
		case ADC_REGULAR_INJECTED_GROUP:
			check_cr = (ADC_CR_ADSTART | ADC_CR_JADSTART);
			break;
		case ADC_INJECTED_GROUP:
			check_cr = ADC_CR_JADSTART;
			break;
		default:
			check_cr = ADC_CR_ADSTART;
			break;
		}

		/*
		 *  ����Ԥ�
		 */
		tick = 0;
		while((sil_rew_mem((uint32_t *)(hadc->base+TOFF_ADC_CR)) & check_cr) != 0){
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
	uint32_t     act_adc = 0, tmp, i;

	/*
	 *  ����ѥ�᡼�������å�
	 */
	if(portid < 1 || portid > NUM_ADCPORT || pini == NULL)
		return NULL;

	if(pini->ScanConvMode != ADC_SCANMODE_DISABLE){
		if(pini->NumConversion < 1 || pini->NumConversion > 16)
			return NULL;
		if(pini->DiscontinuousConvMode == ADC_DISCONTINUOUS_ENABLE){
			if(pini->NumDiscConversion < 1 || pini->NumDiscConversion > 8)
				return NULL;
		}
	}
	if(pini->DiscontinuousConvMode == ADC_DISCONTINUOUS_ENABLE && pini->ContinuousConvMode == ADC_CONTINUOUS_ENABLE)
		return NULL;

	hadc = &AdcHandle[INDEX_ADC(portid)];
	memcpy(&hadc->Init, pini, sizeof(ADC_Init_t));

	apcb = &adc_pcb[INDEX_ADC(portid)];
	hadc->base = apcb->base;
	hadc->apcb = apcb;

	for(i = 0 ; i < NUM_ADCPORT ; i++){
		if(AdcHandle[i].status != ADC_STATUS_RESET)
			act_adc |= 1<<i;
	}

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
		hdma_adc.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
		hdma_adc.Init.MemDataAlignment    = DMA_MDATAALIGN_WORD;
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
	 *  ADC DEEP POWER������
	 */
	if((sil_rew_mem((uint32_t *)(hadc->base+TOFF_ADC_CR)) & ADC_CR_DEEPPWD) != 0)
		sil_andw_mem((uint32_t *)(hadc->base+TOFF_ADC_CR), ADC_CR_DEEPPWD);

	/*
	 *  ADC VOLTAGE ����
	 */
	if((sil_rew_mem((uint32_t *)(hadc->base+TOFF_ADC_CR)) & ADC_CR_ADVREGEN) == 0){
		sil_orw_mem((uint32_t *)(hadc->base+TOFF_ADC_CR), ADC_CR_ADVREGEN);
		sil_dly_nse(ADC_STAB_DELAY_US * 1000);
		if((sil_rew_mem((uint32_t *)(hadc->base+TOFF_ADC_CR)) & ADC_CR_ADVREGEN) == 0)
			return NULL;
	}

	/*
	 *  ADC�⡼�ɥ����å�
	 */
	if((sil_rew_mem((uint32_t *)(hadc->base+TOFF_ADC_CR)) & ADC_CR_ADSTART) != 0)
		return NULL;

	/*
	 *  ADC�Υץꥹ����������(���������)
	 */
	if(act_adc == 0)
		sil_modw_mem((uint32_t *)(TADR_ADC_BASE+TOFF_ADC_CCR), (ADC_CCR_PRESC|ADC_CCR_CKMODE), hadc->Init.ClockPrescaler);

	/*
	 *  ADC�������
	 */
	tmp = pini->ContinuousConvMode | pini->Overrun | pini->DataAlign | pini->Resolution | pini->DiscontinuousConvMode;
	if(pini->DiscontinuousConvMode == ADC_DISCONTINUOUS_ENABLE)
		tmp |= pini->NumDiscConversion << 17;
	if(pini->ExternalTrigConv != ADC_SOFTWARE_START && pini->ExternalTrigConvEdge != ADC_EXTERNALTRIGCONVEDGE_NONE)
		tmp |= ( pini->ExternalTrigConv | pini->ExternalTrigConvEdge);
	sil_modw_mem((uint32_t *)(hadc->base+TOFF_ADC_CFGR), INIT_CFGR, tmp);

	if((sil_rew_mem((uint32_t *)(hadc->base+TOFF_ADC_CR)) & (ADC_CR_ADSTART | ADC_CR_JADSTART)) == 0){
		tmp = pini->LowPowerAutoWait | pini->DMAContinuousRequests;
		sil_modw_mem((uint32_t *)(hadc->base+TOFF_ADC_CFGR), INIT_CFGR_2, tmp);

		/*
		 *  ADC�����С�����ץ������
		 */
		if(pini->OversamplingMode == ADC_OVR_SAMPLING_ENABLE){
			if(pini->ExternalTrigConv == ADC_SOFTWARE_START || pini->ExternalTrigConvEdge == ADC_EXTERNALTRIGCONVEDGE_NONE){
				/*
				 *  �����ȥꥬ���ꥨ�顼
				 */
				return NULL;
			}
			sil_modw_mem((uint32_t *)(hadc->base+TOFF_ADC_CFGR2), INIT_CFGR2, (ADC_CFGR2_ROVSE | 
							pini->OversamplingRatio | pini->OversamplingRightBitShift | 
							pini->OversamplingTriggeredMode | pini->OversamplingStopReset));
		}
		else	/* �����С�����ץ�����ꤷ�ʤ� */
			sil_andw_mem((uint32_t *)(hadc->base+TOFF_ADC_CFGR2), ADC_CFGR2_ROVSE);
	}

	if(pini->ScanConvMode == ADC_SCANMODE_ENABLE)
		sil_modw_mem((uint32_t *)(hadc->base+TOFF_ADC_SQR1), ADC_SQR1_L, (pini->NumConversion - 1));
	else
		sil_andw_mem((uint32_t *)(hadc->base+TOFF_ADC_SQR1), ADC_SQR1_L);

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
	uint32_t  act_adc_cnt = 0, i;

	if(hadc == NULL)
		return E_PAR;

	ercd = adc_conversionstop(hadc);
	if(ercd == E_OK){
		sil_orw_mem((uint32_t *)(hadc->base+TOFF_ADC_CFGR), ADC_CFGR_JQM);
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
		 *  �ꥻ�å�CR
		 */
		sil_andw_mem((uint32_t *)(hadc->base+TOFF_ADC_CR), (ADC_CR_ADVREGEN | ADC_CR_ADCALDIF));
		sil_orw_mem((uint32_t *)(hadc->base+TOFF_ADC_CR), ADC_CR_DEEPPWD);
		/*
		 *  �ꥻ�å�CFGR/CGFR2
		 */
		sil_andw_mem((uint32_t *)(hadc->base+TOFF_ADC_CFGR), (INIT_CFGR | INIT_CFGR_2 | ADC_CFGR_AWD1CH | \
						ADC_CFGR_JAUTO | ADC_CFGR_JAWD1EN | ADC_CFGR_AWD1EN  | ADC_CFGR_AWD1SGL | \
						ADC_CFGR_JQM   | ADC_CFGR_JDISCEN | ADC_CFGR_DISCNUM | ADC_CFGR_DMAEN));
		sil_andw_mem((uint32_t *)(hadc->base+TOFF_ADC_CFGR2), (INIT_CFGR2 | ADC_CFGR2_JOVSE));

		/*
		 *  �ꥻ�å�SMPR
		 */
		sil_wrw_mem((uint32_t *)(hadc->base+TOFF_ADC_SMPR1), 0);
		sil_wrw_mem((uint32_t *)(hadc->base+TOFF_ADC_SMPR2), 0);

		/*
		 *  �ꥻ�å�TR
		 */
		sil_andw_mem((uint32_t *)(hadc->base+TOFF_ADC_TR1), (ADC_TR1_LT1 | ADC_TR1_HT1));
		sil_andw_mem((uint32_t *)(hadc->base+TOFF_ADC_TR2), (ADC_TR2_LT2 | ADC_TR2_HT2));
		sil_andw_mem((uint32_t *)(hadc->base+TOFF_ADC_TR3), (ADC_TR3_LT3 | ADC_TR3_HT3));

		/*
		 *  �ꥻ�å�SQR
		 */
		sil_wrw_mem((uint32_t *)(hadc->base+TOFF_ADC_SQR1), 0);
		sil_wrw_mem((uint32_t *)(hadc->base+TOFF_ADC_SQR2), 0);
		sil_wrw_mem((uint32_t *)(hadc->base+TOFF_ADC_SQR3), 0);
		sil_wrw_mem((uint32_t *)(hadc->base+TOFF_ADC_SQR4), 0);

		/*
		 *  �ꥻ�å�OFR
		 */
		sil_andw_mem((uint32_t *)(hadc->base+TOFF_ADC_OFR1), (ADC_OFR1_OFFSET1_EN | ADC_OFR1_OFFSET1_CH | ADC_OFR1_OFFSET1));
		sil_andw_mem((uint32_t *)(hadc->base+TOFF_ADC_OFR2), (ADC_OFR2_OFFSET2_EN | ADC_OFR2_OFFSET2_CH | ADC_OFR2_OFFSET2));
		sil_andw_mem((uint32_t *)(hadc->base+TOFF_ADC_OFR3), (ADC_OFR3_OFFSET3_EN | ADC_OFR3_OFFSET3_CH | ADC_OFR3_OFFSET3));
		sil_andw_mem((uint32_t *)(hadc->base+TOFF_ADC_OFR4), (ADC_OFR4_OFFSET4_EN | ADC_OFR4_OFFSET4_CH | ADC_OFR4_OFFSET4));

		/*
		 *  �ꥻ�å�AWD
		 */
		sil_andw_mem((uint32_t *)(hadc->base+TOFF_ADC_AWD2CR), ADC_AWD2CR_AWD2CH);
		sil_andw_mem((uint32_t *)(hadc->base+TOFF_ADC_AWD3CR), ADC_AWD3CR_AWD3CH);

		/*
		 *  �ꥻ�å�DIFSEL
		 */
		sil_andw_mem((uint32_t *)(hadc->base+TOFF_ADC_DIFSEL), ADC_DIFSEL_DIFSEL);

		/*
		 *  �ꥻ�å�CALFACT
		 */
		sil_andw_mem((uint32_t *)(hadc->base+TOFF_ADC_CALFACT), (ADC_CALFACT_CALFACT_D | ADC_CALFACT_CALFACT_S));

		/*
		 *  �ꥻ�å�COMMON ADC
		 */
		for(i = 0 ; i < NUM_ADCPORT ; i++){
			if(AdcHandle[i].status != ADC_STATUS_RESET)
				act_adc_cnt++;
		}
		if(act_adc_cnt <= 1){
			/*
			 *  �ꥻ�å�CCR
			 */
			sil_andw_mem((uint32_t *)(TADR_ADC_BASE+TOFF_ADC_CCR), (ADC_CCR_CKMODE | ADC_CCR_PRESC | ADC_CCR_VBATEN | \
						ADC_CCR_TSEN | ADC_CCR_VREFEN | ADC_CCR_MDMA | ADC_CCR_DMACFG | ADC_CCR_DELAY | ADC_CCR_DUAL));
			/*
			 *  ADC����å����
			 */
			sil_andw_mem((uint32_t *)(hadc->apcb->adcclockbase), hadc->apcb->adcclockbit);
		}
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
			sil_orw_mem((uint32_t *)(hadc->base+TOFF_ADC_IER), ADC_IER_EOSIE);
		else
			sil_orw_mem((uint32_t *)(hadc->base+TOFF_ADC_IER), ADC_IER_EOCIE);
		if(hadc->Init.Overrun == ADC_OVR_DATA_PRESERVED)
			sil_orw_mem((uint32_t *)(hadc->base+TOFF_ADC_IER), ADC_IER_OVRIE);

		/*
		 *  ADC��������
		 */
		if((sil_rew_mem((uint32_t *)(hadc->base+TOFF_ADC_CFGR)) & ADC_CFGR_JAUTO) != 0)
			hadc->status = ADC_STATUS_BUSY_INJ;
		if(adc_dual_regular_conversion(hadc))
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
adc_start_dma(ADC_Handle_t* hadc, uint32_t* pdata, uint32_t length)
{
	ER ercd = E_OK;

	if(hadc == NULL)
		return E_PAR;

	if((sil_rew_mem((uint32_t *)(hadc->base+TOFF_ADC_CR)) & ADC_CR_ADSTART) != 0)
		return E_OBJ;

	if(!adc_dual_regular_conversion(hadc))
		return E_OBJ;

    /*
	 *  ADCͭ����
	 */
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
		sil_orw_mem((uint32_t *)(hadc->base+TOFF_ADC_CFGR), ADC_CFGR_DMAEN);

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
	else
		ercd = E_OBJ;
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
		sil_andw_mem((uint32_t *)(hadc->base+TOFF_ADC_CFGR), ADC_CFGR_DMAEN);

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
	uint32_t cfg = sil_rew_mem((uint32_t *)(hadc->base+TOFF_ADC_CFGR));

	if(((isr & ier) & ADC_ISR_EOSMP) != 0){
		if(hadc->eosmpcallback != NULL)
			hadc->eosmpcallback(hadc);
		sil_wrw_mem((uint32_t *)(hadc->base+TOFF_ADC_ISR), ADC_ISR_EOSMP);
	}

	/*
	 *  ����ɥ���С������ޤ��ϥ���ɥ�������Ƚ��
	 */
	if(((isr & ier) & (ADC_ISR_EOC | ADC_ISR_EOS)) != 0){
	    /*
		 *  ����С������λ���֤˰ܹ�
		 */
	    if(hadc->errorcode == ADC_ERROR_NONE)
			hadc->status = ADC_STATUS_EOC;

	    /*
		 *  ADC��߽���
		 */
	    if((cfg & ADC_CFGR_EXTEN) == 0){
			uint32_t tmp;
			if(adc_dual_regular_conversion(hadc))
				tmp = cfg;
			else
				tmp = sil_rew_mem((uint32_t *)(TADR_ADC1_BASE+TOFF_ADC_CFGR));
			if((tmp & ADC_CFGR_CONT) == 0){
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
						if(hadc->status == ADC_STATUS_BUSY_INJ)
							hadc->status = ADC_STATUS_READY;
						else
							hadc->status = ADC_STATUS_BUSY_EOC;
					}
					else{	/* ���󥿡��ʥ륨�顼 */
						hadc->status = ADC_STATUS_ERROR;
						hadc->errorcode |= ADC_ERROR_INT;
					}
				}
			}
		}

		/*
		 *  ��λ������Хå��ؿ����ɤ߽Ф�
		 */
		if(hadc->xfercallback != NULL)
			hadc->xfercallback(hadc);

	    /*
		 *  ����ߥ��ꥢ
		 */
		sil_wrw_mem((uint32_t *)(hadc->base+TOFF_ADC_ISR), (ADC_ISR_EOC | ADC_ISR_EOS));
	}

	/*
	 *  INJECTED����ɥ���С������Ƚ��
	 */
	if(((isr & ier) & (ADC_ISR_JEOC | ADC_ISR_JEOS)) != 0){
	    /*
		 *  ����С������λ���֤˰ܹ�
		 */
	    if(hadc->errorcode == ADC_ERROR_NONE)
			hadc->status = ADC_STATUS_EOC_INJ;

		if((isr & ADC_ISR_JEOS) != 0){
			uint32_t tmp;
			if(adc_dual_regular_conversion(hadc))
				tmp = cfg;
			else
				tmp = sil_rew_mem((uint32_t *)(TADR_ADC1_BASE+TOFF_ADC_CFGR));
			if((sil_rew_mem((uint32_t *)(hadc->base+TOFF_ADC_JSQR)) & ADC_JSQR_JEXTEN) == 0 &&
				(tmp & ADC_CFGR_JQM) == 0 && (tmp & (ADC_CFGR_JAUTO|ADC_CFGR_CONT)) != (ADC_CFGR_JAUTO|ADC_CFGR_CONT) &&
					(cfg & ADC_CFGR_EXTEN) == 0){

				if((sil_rew_mem((uint32_t *)(hadc->base+TOFF_ADC_CR)) & ADC_CR_JADSTART) == 0){
					sil_andw_mem((uint32_t *)(hadc->base+TOFF_ADC_IER), (ADC_IER_JEOCIE | ADC_IER_JEOSIE));
					if(hadc->status == ADC_STATUS_BUSY_INJ)
						hadc->status = ADC_STATUS_READY;
					else
						hadc->status = ADC_STATUS_BUSY_EOC;
				}
				else{	/* ���󥿡��ʥ륨�顼 */
					hadc->status = ADC_STATUS_ERROR;
					hadc->errorcode |= ADC_ERROR_INT;
				}
			}
		}

		if(hadc->injectedcallback != NULL)
			hadc->injectedcallback(hadc);

	    /*
		 *  ����ߥ��ꥢ
		 */
		sil_wrw_mem((uint32_t *)(hadc->base+TOFF_ADC_ISR), (ADC_ISR_JEOC | ADC_ISR_JEOS));
	}

	/*
	 *  ���ʥ������å��ɥå�Ƚ��
	 */
	if((isr & ADC_ISR_AWD1) != 0 && (ier & ADC_IER_AWD1IE) != 0){
		/*
		 *  �����å��ɥå�ȯ�����֤��ѹ�
		 */
		hadc->status = ADC_STATUS_AWD1;

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
	if((isr & ADC_ISR_AWD2) != 0 && (ier & ADC_IER_AWD2IE) != 0){
		/*
		 *  �����å��ɥå�ȯ�����֤��ѹ�
		 */
		hadc->status = ADC_STATUS_AWD2;

		/*
		 *  �����å��ɥå�����ߥ��ꥢ
		 */
		sil_wrw_mem((uint32_t *)(hadc->base+TOFF_ADC_ISR), ADC_ISR_AWD2);

		/*
		 *  ������ɥ��ΰ襢���ȤΥ�����Хå��ؿ��ɤ߽Ф�
		 */
		if(hadc->outofwincallback != NULL)
			hadc->outofwincallback(hadc);
	}
	if((isr & ADC_ISR_AWD3) != 0 && (ier & ADC_IER_AWD3IE) != 0){
		/*
		 *  �����å��ɥå�ȯ�����֤��ѹ�
		 */
		hadc->status = ADC_STATUS_AWD3;

		/*
		 *  �����å��ɥå�����ߥ��ꥢ
		 */
		sil_wrw_mem((uint32_t *)(hadc->base+TOFF_ADC_ISR), ADC_ISR_AWD3);

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
			(sil_rew_mem((uint32_t *)(hadc->base+TOFF_ADC_CFGR)) & ADC_CFGR_DMAEN) != 0){

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
 *  ADC12����ߥϥ�ɥ�
 */
void
adc_int12_handler(void)
{
	if(AdcHandle[0].status != ADC_STATUS_RESET)
		adc_handler(&AdcHandle[0]);
	if(AdcHandle[1].status != ADC_STATUS_RESET)
		adc_handler(&AdcHandle[1]);
}

/*
 *  ADC3����ߥϥ�ɥ�
 */
void
adc_int3_handler(void)
{
	if(AdcHandle[2].status != ADC_STATUS_RESET)
		adc_handler(&AdcHandle[2]);
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
	uint32_t clkpin, shift, ccr, tmp;

	if(hadc == NULL || sConfig == NULL)
		return E_PAR;

	clkpin = (sConfig->GpioBase - TADR_GPIOA_BASE) / 0x400;
	if(clkpin > 7 || sConfig->GpioPin >= 16)
		return E_PAR;

	if(sConfig->Rank == 0 || sConfig->Rank > 16)
		return E_PAR;

	if((sil_rew_mem((uint32_t *)(hadc->base+TOFF_ADC_CR)) & ADC_CR_ADSTART) != 0)
		return E_OBJ;

	/*
	 *  ����ͥ�ͭ������
	 */
	sil_orw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_AHB2ENR), (1<<clkpin));
	tmp = sil_rew_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_AHB2ENR));

	GPIO_InitStruct.mode = GPIO_MODE_ANALOG_AD;
	GPIO_InitStruct.pull = GPIO_NOPULL;
	gpio_setup(sConfig->GpioBase, &GPIO_InitStruct, sConfig->GpioPin);

	/*
	 *  �������
	 */
	if (sConfig->Rank < 5){	/* ���1����4 */
		shift = 6 * (sConfig->Rank - 1) + 6;
		sil_modw_mem((uint32_t *)(hadc->base+TOFF_ADC_SQR1), (ADC_SQR2_SQ5 << shift), (sConfig->Channel << shift));
	}
	else if (sConfig->Rank < 10){	/* ���5����9 */
		shift = 6 * (sConfig->Rank - 5);
		sil_modw_mem((uint32_t *)(hadc->base+TOFF_ADC_SQR2), (ADC_SQR2_SQ5 << shift), (sConfig->Channel << shift));
	}
	else if (sConfig->Rank < 15){	/* ���10����14 */
		shift = 6 * (sConfig->Rank - 10);
		sil_modw_mem((uint32_t *)(hadc->base+TOFF_ADC_SQR3), (ADC_SQR2_SQ5 << shift), (sConfig->Channel << shift));
	}
	else{	/* ���15����16 */
		shift = 6 * (sConfig->Rank - 15);
		sil_modw_mem((uint32_t *)(hadc->base+TOFF_ADC_SQR4), (ADC_SQR2_SQ5 << shift), (sConfig->Channel << shift));
	}

	if((sil_rew_mem((uint32_t *)(hadc->base+TOFF_ADC_CR)) & (ADC_CR_ADSTART | ADC_CR_JADSTART)) == 0){
		/*
		 *  ����ץ�󥰻�������
		 */
		if(sConfig->Channel > ADC_CHANNEL_9){	/* ����ͥ�10����18 */
			shift = 3 * (sConfig->Channel - 10);
			sil_modw_mem((uint32_t *)(hadc->base+TOFF_ADC_SMPR2), (ADC_SMPR2_SMP10 << shift), (sConfig->SamplingTime << shift));
		}
		else{ 	/* ����ͥ�0����9 */
			shift = 3 * sConfig->Channel;
			sil_modw_mem((uint32_t *)(hadc->base+TOFF_ADC_SMPR1), (ADC_SMPR1_SMP0 << shift), (sConfig->SamplingTime << shift));
		}
		/*
		 *  ���ե��å�����
		 */
		tmp = sConfig->Offset << ((sil_rew_mem((uint32_t *)(hadc->base+TOFF_ADC_CFGR)) & ADC_CFGR_RES) * 2);
		switch (sConfig->OffsetNumber){
		case ADC_OFFSET_1:
			sil_modw_mem((uint32_t *)(hadc->base+TOFF_ADC_OFR1), INIT_OFR, (ADC_OFR1_OFFSET1_EN | (sConfig->Channel << 26) | tmp));
			break;
		case ADC_OFFSET_2:
			sil_modw_mem((uint32_t *)(hadc->base+TOFF_ADC_OFR2), INIT_OFR, (ADC_OFR2_OFFSET2_EN | (sConfig->Channel << 26) | tmp));
			break;
		case ADC_OFFSET_3:
			sil_modw_mem((uint32_t *)(hadc->base+TOFF_ADC_OFR3), INIT_OFR, (ADC_OFR3_OFFSET3_EN | (sConfig->Channel << 26) | tmp));
			break;
		case ADC_OFFSET_4:
			sil_modw_mem((uint32_t *)(hadc->base+TOFF_ADC_OFR4), INIT_OFR, (ADC_OFR4_OFFSET4_EN | (sConfig->Channel << 26) | tmp));
			break;
		default :	/* OFFSET�ʤ��ξ�� */
			if((sil_rew_mem((uint32_t *)(hadc->base+TOFF_ADC_OFR1)) & ADC_OFR1_OFFSET1_CH) == (sConfig->Channel << 26))
				sil_andw_mem((uint32_t *)(hadc->base+TOFF_ADC_OFR1), ADC_OFR1_OFFSET1_EN);
			if((sil_rew_mem((uint32_t *)(hadc->base+TOFF_ADC_OFR2)) & ADC_OFR2_OFFSET2_CH) == (sConfig->Channel << 26))
				sil_andw_mem((uint32_t *)(hadc->base+TOFF_ADC_OFR2), ADC_OFR2_OFFSET2_EN);
			if((sil_rew_mem((uint32_t *)(hadc->base+TOFF_ADC_OFR3)) & ADC_OFR3_OFFSET3_CH) == (sConfig->Channel << 26))
				sil_andw_mem((uint32_t *)(hadc->base+TOFF_ADC_OFR3), ADC_OFR3_OFFSET3_EN);
			if((sil_rew_mem((uint32_t *)(hadc->base+TOFF_ADC_OFR4)) & ADC_OFR4_OFFSET4_CH) == (sConfig->Channel << 26))
				sil_andw_mem((uint32_t *)(hadc->base+TOFF_ADC_OFR4), ADC_OFR4_OFFSET4_EN);
			break;
		}
	}

	/*
	 *  ADC��ͭ���Ǥʤ���硢Differential input mode�ȸ��̤Υ��󥵡������Ԥ�
	 */
	if(!is_adc_enable(hadc->base)){
		if(sConfig->SingleDiff != ADC_DIFFERENTIAL_ENDED)
			sil_andw_mem((uint32_t *)(hadc->base+TOFF_ADC_DIFSEL), (1 << sConfig->Channel));
		else{
			/* Enable differential mode */
			sil_orw_mem((uint32_t *)(hadc->base+TOFF_ADC_DIFSEL), (1 << sConfig->Channel));

			/*
			 *  ����ץ�󥰥����ߥ󥰤����ͥ�ܣ����ѹ�
			 */
			if(sConfig->Channel >= ADC_CHANNEL_9){
				shift = 3 * (sConfig->Channel - 9);
				sil_modw_mem((uint32_t *)(hadc->base+TOFF_ADC_SMPR2), (ADC_SMPR2_SMP10 << shift), (sConfig->SamplingTime << shift));
			}
			else{	/* For channels 0 to 8, SMPR1 must be configured */
				shift = 3 * (sConfig->Channel + 1);
				sil_modw_mem((uint32_t *)(hadc->base+TOFF_ADC_SMPR1), (ADC_SMPR1_SMP0 << shift), (sConfig->SamplingTime << shift));
			}
		}

		/*
		 * Vbat/VrefInt/TempSensor����ͥ�����
		 */
		ccr = sil_rew_mem((uint32_t *)(TADR_ADC_BASE+TOFF_ADC_CCR));
		if((sConfig->Channel == ADC_CHANNEL_TEMPSENSOR && (ccr & ADC_CCR_TSEN) == 0) ||
			(sConfig->Channel == ADC_CHANNEL_VBAT && (ccr & ADC_CCR_VBATEN) == 0) ||
				(sConfig->Channel == ADC_CHANNEL_VREFINT && (ccr & ADC_CCR_VREFEN) == 0)){
			/*
			 *  ͭ����ADC�������硢���֥��顼�Ȥ���
			 */
			if(!is_adc_enable(TADR_ADC1_BASE) && !is_adc_enable(TADR_ADC2_BASE) && !is_adc_enable(TADR_ADC3_BASE)){
				if(hadc->base != TADR_ADC2_BASE){
					if(sConfig->Channel == ADC_CHANNEL_TEMPSENSOR)
						sil_orw_mem((uint32_t *)(TADR_ADC_BASE+TOFF_ADC_CCR), ADC_CCR_TSEN);
					else if(sConfig->Channel == ADC_CHANNEL_VBAT)
						sil_orw_mem((uint32_t *)(TADR_ADC_BASE+TOFF_ADC_CCR), ADC_CCR_VBATEN);
					else if(sConfig->Channel == ADC_CHANNEL_VREFINT)
						sil_orw_mem((uint32_t *)(TADR_ADC_BASE+TOFF_ADC_CCR), ADC_CCR_VREFEN);
				}
			}
			else{
				return E_OBJ;
			}
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
	uint32_t cfgr, tmp1, tmp2;

	if(hadc == NULL || AnalogWDGConfig == NULL)
		return E_PAR;

	if(AnalogWDGConfig->WatchdogNumber == 0 || AnalogWDGConfig->WatchdogNumber > 3)
		return E_PAR;

	if((sil_rew_mem((uint32_t *)(hadc->base+TOFF_ADC_CR)) & (ADC_CR_ADSTART | ADC_CR_JADSTART)) != 0)
		return E_OBJ;

	cfgr = sil_rew_mem((uint32_t *)(hadc->base+TOFF_ADC_CFGR));
	if(AnalogWDGConfig->WatchdogNumber == 1){
		/*
		 *  AWD1�⡼������
		 */
		cfgr &= ~(ADC_CFGR_AWD1SGL | ADC_CFGR_JAWD1EN | ADC_CFGR_AWD1EN | ADC_CFGR_AWD1CH);
		cfgr |= AnalogWDGConfig->WatchdogMode | (AnalogWDGConfig->Channel << 26);
		sil_wrw_mem((uint32_t *)(hadc->base+TOFF_ADC_CFGR), cfgr);

		/*
		 *  AWD1 HIGH/LOW�Υ���å���ۡ��������
		 */
		tmp1 = AnalogWDGConfig->HighThreshold << (((cfgr & ADC_CFGR_RES) >> 3)*2);
		tmp2 = AnalogWDGConfig->LowThreshold << (((cfgr & ADC_CFGR_RES) >> 3)*2);
		sil_wrw_mem((uint32_t *)(hadc->base+TOFF_ADC_TR1), (tmp1 << 16) | tmp2);

		/*
		 *  AWD1 ����ߥ��ꥢ�ȳ��������
		 */
		sil_wrw_mem((uint32_t *)(hadc->base+TOFF_ADC_ISR), ADC_ISR_AWD1);
		if(AnalogWDGConfig->ITMode == ADC_ANALOGWATCHDOG_ITMODE_ENABLE)
			sil_orw_mem((uint32_t *)(hadc->base+TOFF_ADC_IER), ADC_IER_AWD1IE);
		else
			sil_andw_mem((uint32_t *)(hadc->base+TOFF_ADC_IER), ADC_IER_AWD1IE);
	}
	else{
		/*
		 *  AWD2,3 ����å�������ٷ׻�
		 */
		if((cfgr & ADC_CFGR_RES) != (ADC_CFGR_RES_1 | ADC_CFGR_RES_0)){
			tmp1 = AnalogWDGConfig->HighThreshold >> (4 - (((cfgr & ADC_CFGR_RES) >> 3)*2));
			tmp2 = AnalogWDGConfig->LowThreshold >> (4 - (((cfgr & ADC_CFGR_RES) >> 3)*2));
		}
		else{
			tmp1 = AnalogWDGConfig->HighThreshold << 2;
			tmp2 = AnalogWDGConfig->LowThreshold << 2;
		}
		if(AnalogWDGConfig->WatchdogNumber == 2){
			/*
			 *  AWD2�⡼������
			 */
			if(AnalogWDGConfig->WatchdogMode != ADC_ANALOGWATCHDOG_NONE)
				sil_orw_mem((uint32_t *)(hadc->base+TOFF_ADC_AWD2CR), (1 << AnalogWDGConfig->Channel));
			else
				sil_andw_mem((uint32_t *)(hadc->base+TOFF_ADC_AWD2CR), (1 << AnalogWDGConfig->Channel));

			/*
			 *  AWD2 HIGH/LOW�Υ���å���ۡ��������
			 */
			sil_wrw_mem((uint32_t *)(hadc->base+TOFF_ADC_TR2), (tmp1 << 16) | tmp2);

			/*
			 *  AWD2 ����ߥ��ꥢ�ȳ��������
			 */
			sil_wrw_mem((uint32_t *)(hadc->base+TOFF_ADC_ISR), ADC_ISR_AWD2);
			if(AnalogWDGConfig->ITMode == ADC_ANALOGWATCHDOG_ITMODE_ENABLE)
				sil_orw_mem((uint32_t *)(hadc->base+TOFF_ADC_IER), ADC_IER_AWD2IE);
			else
				sil_andw_mem((uint32_t *)(hadc->base+TOFF_ADC_IER), ADC_IER_AWD2IE);
		}
		else{
			/*
			 *  AWD3�⡼������
			 */
			if(AnalogWDGConfig->WatchdogMode != ADC_ANALOGWATCHDOG_NONE)
				sil_orw_mem((uint32_t *)(hadc->base+TOFF_ADC_AWD3CR), (1 << AnalogWDGConfig->Channel));
			else
				sil_andw_mem((uint32_t *)(hadc->base+TOFF_ADC_AWD3CR), (1 << AnalogWDGConfig->Channel));

			/*
			 *  AWD3 HIGH/LOW�Υ���å���ۡ��������
			 */
			sil_wrw_mem((uint32_t *)(hadc->base+TOFF_ADC_TR3), (tmp1 << 16) | tmp2);

			/*
			 *  AWD3 HIGH/LOW�Υ���å���ۡ��������
			 */
			sil_wrw_mem((uint32_t *)(hadc->base+TOFF_ADC_ISR), ADC_ISR_AWD3);
			if(AnalogWDGConfig->ITMode == ADC_ANALOGWATCHDOG_ITMODE_ENABLE)
				sil_orw_mem((uint32_t *)(hadc->base+TOFF_ADC_IER), ADC_IER_AWD3IE);
			else
				sil_andw_mem((uint32_t *)(hadc->base+TOFF_ADC_IER), ADC_IER_AWD3IE);
		}
	}
	return E_OK;
}


