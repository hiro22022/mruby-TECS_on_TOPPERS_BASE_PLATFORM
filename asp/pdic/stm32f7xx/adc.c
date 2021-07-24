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
 *  @(#) $Id: adc.c 698 2016-02-25 13:22:51Z roi $
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
 *  I2C�ݡ���ID��������֥�å�����Ф�����Υޥ���
 */
#define INDEX_ADC(adcid)        ((uint_t)((adcid) - 1))

#define ADC_CHANNEL_TEMPSENSOR  ((uint32_t)ADC_CHANNEL_16)

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
	(TADR_RCC_BASE+TOFF_RCC_APB2ENR),  RCC_APB2ENR_ADC1EN,
	(TADR_RCC_BASE+TOFF_RCC_AHB1ENR),  RCC_AHB1ENR_DMA2EN,
	TADR_DMA2_STM0_BASE, DMA_CHANNEL_0 },
  {	TADR_ADC2_BASE,
	(TADR_RCC_BASE+TOFF_RCC_APB2ENR),  RCC_APB2ENR_ADC2EN,
	(TADR_RCC_BASE+TOFF_RCC_AHB1ENR),  RCC_AHB1ENR_DMA2EN,
	TADR_DMA2_STM2_BASE, DMA_CHANNEL_1 },
  {	TADR_ADC3_BASE,
	(TADR_RCC_BASE+TOFF_RCC_APB2ENR),  RCC_APB2ENR_ADC3EN,
	(TADR_RCC_BASE+TOFF_RCC_AHB1ENR),  RCC_AHB1ENR_DMA2EN,
	TADR_DMA2_STM0_BASE, DMA_CHANNEL_2 }
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
	hadc->ErrorCode |= ADC_ERROR_DMA;
	if(hadc->errorcallback != NULL)
		hadc->errorcallback(hadc);
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
	if(portid < 1 || portid > NUM_ADCPORT)
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

		hdma_adc.base = apcb->dmarxbase;
		hdma_adc.Init.Channel  = apcb->dmarxchannel;
		hdma_adc.Init.Direction = DMA_PERIPH_TO_MEMORY;
		hdma_adc.Init.PeriphInc = DMA_PINC_DISABLE;
		hdma_adc.Init.MemInc = DMA_MINC_ENABLE;
		hdma_adc.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
		hdma_adc.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
		if((pini->DMAContinuousRequests & ADC_DMANONECIRCULAR) != 0)
			hdma_adc.Init.Mode = DMA_NORMAL;
		else
			hdma_adc.Init.Mode = DMA_CIRCULAR;
		hdma_adc.Init.Priority = DMA_PRIORITY_HIGH;
		hdma_adc.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
		hdma_adc.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_HALFFULL;
		hdma_adc.Init.MemBurst = DMA_MBURST_SINGLE;
		hdma_adc.Init.PeriphBurst = DMA_PBURST_SINGLE;

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
	sil_modw_mem((uint32_t *)(TADR_ADC_BASE+TOFF_ADC_CCR), ADC_CCR_ADCPRE, hadc->Init.ClockPrescaler);

	/*
	 *  ADC�������
	 */
	sil_modw_mem((uint32_t *)(hadc->base+TOFF_ADC_CR1), ADC_CR1_SCAN, hadc->Init.ScanConvMode);
	sil_modw_mem((uint32_t *)(hadc->base+TOFF_ADC_CR1), ADC_CR1_RES, hadc->Init.Resolution);
	sil_modw_mem((uint32_t *)(hadc->base+TOFF_ADC_CR2), ADC_CR2_ALIGN, hadc->Init.DataAlign);

	/*
	 *  ADC�����ȥꥬ����
	 */
	if(hadc->Init.ExternalTrigConv != ADC_SOFTWARE_START){
		sil_modw_mem((uint32_t *)(hadc->base+TOFF_ADC_CR2), ADC_CR2_EXTSEL, hadc->Init.ExternalTrigConv);
		sil_modw_mem((uint32_t *)(hadc->base+TOFF_ADC_CR2), ADC_CR2_EXTEN, hadc->Init.ExternalTrigConvEdge);
	}
	else{
		sil_andw_mem((uint32_t *)(hadc->base+TOFF_ADC_CR2), (ADC_CR2_EXTSEL | ADC_CR2_EXTEN));
	}

	/*
	 *  ADC��³�⡼������
	 */
	sil_modw_mem((uint32_t *)(hadc->base+TOFF_ADC_CR2), ADC_CR2_CONT, hadc->Init.ContinuousConvMode);
	sil_modw_mem((uint32_t *)(hadc->base+TOFF_ADC_CR1), ADC_CR1_DISCEN, hadc->Init.DiscontinuousConvMode);
	sil_modw_mem((uint32_t *)(hadc->base+TOFF_ADC_CR1), ADC_CR1_DISCNUM, hadc->Init.NumDiscConversion << 13);

	/*
	 *  ����С�����������
	 */
	sil_modw_mem((uint32_t *)(hadc->base+TOFF_ADC_SQR1), ADC_SQR1_L, (hadc->Init.NumConversion - 1) << 20);

	/*
	 *  ADC-DMA�⡼������
	 */
	sil_modw_mem((uint32_t *)(hadc->base+TOFF_ADC_CR2), ADC_CR2_DDS, hadc->Init.DMAContinuousRequests);

	/*
	 *  ADC EOC����
	 */
	sil_modw_mem((uint32_t *)(hadc->base+TOFF_ADC_CR2), ADC_CR2_EOCS, hadc->Init.EOCSelection);

	/*
	 *  ADC�������֤�
	 */
	hadc->ErrorCode = ADC_ERROR_NONE;
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
	if(hadc == NULL)
		return E_PAR;

	/*
	 *  ADC�ꥻ�å�(���������)
	 */
	sil_orw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_APB2RSTR), RCC_APB2RSTR_ADCRST);
	sil_andw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_APB2RSTR), RCC_APB2RSTR_ADCRST);

	/*
	 *  DMA���
	 */
	if(hadc->hdmarx != NULL)
		dma_deinit(hadc->hdmarx);

	/*
	 *  ADC�ξ��ֽ����
	 */
	hadc->ErrorCode = ADC_ERROR_NONE;
	hadc->status = ADC_STATUS_RESET;
	return E_OK;
}

/*
 *  ADC-����ߥ⡼�ɳ��Ͻ���
 *  parameter1  hadc: ADC�ϥ�ɥ�ؤΥݥ���
 *  return ER������
 */
ER
adc_start_int(ADC_Handle_t* hadc)
{
	if(hadc == NULL)
		return E_PAR;

	/*
	 *  �¹Ծ��֤˰ܹ�
	 */
	hadc->status = ADC_STATUS_BUSY;
	hadc->ErrorCode = ADC_ERROR_NONE;

	/*
	 *  ADC��ͭ���Ǥʤ���С�ADC�����ꤷ�¹��Ԥ���Ԥ�
	 */
	if((sil_rew_mem((uint32_t *)(hadc->base+TOFF_ADC_CR2)) & ADC_CR2_ADON) == 0){
		sil_orw_mem((uint32_t *)(hadc->base+TOFF_ADC_CR2), ADC_CR2_ADON);
		sil_dly_nse(ADC_STAB_DELAY_US*1000);
	}

	/*
	 *  ���顼ž�����������
	 */
	sil_orw_mem((uint32_t *)(hadc->base+TOFF_ADC_CR1), (ADC_CR1_OVRIE | ADC_CR1_EOCIE));

	/*
	 *  �ޥ���⡼��Ƚ��
	 */
	if((sil_rew_mem((uint32_t *)(TADR_ADC_BASE+TOFF_ADC_CCR)) & ADC_CCR_MULTI) == 0){
		/*
		 *  ��ޥ���⡼�ɡ������ȥꥬ�Ǥʤ���Х��եȥ�������
		 */
		if((sil_rew_mem((uint32_t *)(hadc->base+TOFF_ADC_CR2)) & ADC_CR2_EXTEN) == 0){
			sil_orw_mem((uint32_t *)(hadc->base+TOFF_ADC_CR2), ADC_CR2_SWSTART);
		}
	}
	else{
		/*
		 *  �ޥ���⡼�ɡ�ADC1�ǳ����ȥꥬ�Ǥʤ���Х��եȥ�������
		 */
		if((hadc->base == TADR_ADC1_BASE) && ((sil_rew_mem((uint32_t *)(hadc->base+TOFF_ADC_CR2)) & ADC_CR2_EXTEN) == 0)){
			sil_orw_mem((uint32_t *)(hadc->base+TOFF_ADC_CR2), ADC_CR2_SWSTART);
		}
	}
	return E_OK;
}

/*
 *  ADC-����ߥ⡼����߽���
 *  parameter1  hadc: ADC�ϥ�ɥ�ؤΥݥ���
 *  return ER������
 */
ER
adc_end_int(ADC_Handle_t* hadc)
{
	if(hadc == NULL)
		return E_PAR;

	/*
	 *  ž�������顼��������
	 */
	sil_andw_mem((uint32_t *)(hadc->base+TOFF_ADC_CR1), (ADC_CR1_EOCIE | ADC_CR1_JEOCIE | ADC_CR1_OVRIE));

	/*
	 *  ADC�⥸�塼�����
	 */
	sil_andw_mem((uint32_t *)(hadc->base+TOFF_ADC_CR2), ADC_CR2_ADON);

	hadc->status = ADC_STATUS_READY;
	return E_OK;
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
	if(hadc == NULL)
		return E_PAR;

	/*
	 *  ADC DMA����
	 */
	sil_orw_mem((uint32_t *)(hadc->base+TOFF_ADC_CR2), ADC_CR2_DMA);

	/*
	 *  ADC�����С������������
	 */
	sil_orw_mem((uint32_t *)(hadc->base+TOFF_ADC_CR1), ADC_CR1_OVRIE);

	/*
	 *  DMA������Хå��ؿ�����
	 */
	hadc->hdmarx->xfercallback = adc_dmatrans_func;
	hadc->hdmarx->xferhalfcallback = adc_dmahalftrans_func;
	hadc->hdmarx->errorcallback = adc_dmaerror_func;

	/*
	 *  �¹Ծ��֤˰ܹ�
	 */
	hadc->status = ADC_STATUS_BUSY;
	hadc->ErrorCode = ADC_ERROR_NONE;
	hadc->NumCurrentConversionRank = hadc->Init.NumConversion;

	/*
	 *  DMA��������
	 */
	dma_start(hadc->hdmarx, hadc->base+TOFF_ADC_DR, (uint32_t)pdata, length);

	/*
	 *  ADC��ͭ���Ǥʤ���С�ADC�����ꤷ�¹��Ԥ���Ԥ�
	 */
	if((sil_rew_mem((uint32_t *)(hadc->base+TOFF_ADC_CR2)) & ADC_CR2_ADON) == 0){
		sil_orw_mem((uint32_t *)(hadc->base+TOFF_ADC_CR2), ADC_CR2_ADON);
		sil_dly_nse(ADC_STAB_DELAY_US*1000);
	}

	/*
	 *  �����ȥꥬ���꤬�ʤ���С����եȥ���������С�����󥹥����������Ԥ�
	 */
	if((sil_rew_mem((uint32_t *)(hadc->base+TOFF_ADC_CR2)) & ADC_CR2_EXTEN) == 0){
		sil_orw_mem((uint32_t *)(hadc->base+TOFF_ADC_CR2), ADC_CR2_SWSTART);
	}
	return E_OK;
}

/*
 *  ADC-DMA�⡼����߽���
 *  parameter1  hadc: ADC�ϥ�ɥ�ؤΥݥ���
 *  return ER������
 */
ER
adc_end_dma(ADC_Handle_t* hadc)
{
	if(hadc == NULL)
		return E_PAR;

	/*
	 *  ADC�⥸�塼�����
	 */
	sil_andw_mem((uint32_t *)(hadc->base+TOFF_ADC_CR2), ADC_CR2_ADON);

	/*
	 *  �����С�������ߤ����
	 */
	sil_andw_mem((uint32_t *)(hadc->base+TOFF_ADC_CR1), ADC_CR1_OVRIE);

	/*
	 *  ADC��DMA�⡼�ɤ򥪥�
	 */
	sil_andw_mem((uint32_t *)(hadc->base+TOFF_ADC_CR2), ADC_CR2_DMA);

	/*
	 *  DMA�����
	 */
	if(hadc->hdmarx != NULL)
		dma_end(hadc->hdmarx);

	hadc->status = ADC_STATUS_READY;
	return E_OK;
}

/*
 *  ADC ����ߥϥ�ɥ�¹Դؿ�
 */
void
adc_handler(ADC_Handle_t* hadc)
{
	uint32_t sr  = sil_rew_mem((uint32_t *)(hadc->base+TOFF_ADC_SR));
	uint32_t cr1 = sil_rew_mem((uint32_t *)(hadc->base+TOFF_ADC_CR1));
	uint32_t cr2 = sil_rew_mem((uint32_t *)(hadc->base+TOFF_ADC_CR2));

	/*
	 *  �쥮��顼�쥸�����ν�λȽ��
	 */
	if((sr & ADC_SR_EOC) != 0 && (cr1 & ADC_CR1_EOCIE) != 0){
		if((cr2 & ADC_CR2_CONT) == 0)		/* ���ơ��������ѹ� */
			hadc->status = ADC_STATUS_EOC;
		else
			hadc->status = ADC_STATUS_BUSY_EOC;

		if((hadc->Init.ContinuousConvMode == ADC_CONTINUOUS_DISABLE) && ((cr2 & ADC_CR2_EXTEN) == 0)){
			if(hadc->Init.EOCSelection == ADC_EOC_SEQ_DISABLE){
				/*
				 *  ž���ȥ��顼����ߤ����
				 */
				sil_andw_mem((uint32_t *)(hadc->base+TOFF_ADC_CR1), ADC_CR1_EOCIE);
				sil_andw_mem((uint32_t *)(hadc->base+TOFF_ADC_CR1), ADC_CR1_OVRIE);
			}
			else{
				if (hadc->NumCurrentConversionRank != 0){
					hadc->NumCurrentConversionRank--;
				}

				/*
				 *  RANK��λȽ��
				 */
				if(hadc->NumCurrentConversionRank == 0){
					/*
					 *  ž���ȥ��顼����ߤ����
					 */
					sil_andw_mem((uint32_t *)(hadc->base+TOFF_ADC_CR1), ADC_CR1_EOCIE);
					sil_andw_mem((uint32_t *)(hadc->base+TOFF_ADC_CR1), ADC_CR1_OVRIE);
				}
			}
		}

		/*
		 *  ��λ������Хå��ؿ����ɤ߽Ф�
		 */
		if(hadc->xfercallback != NULL)
			hadc->xfercallback(hadc);

		/*
		 * �쥮��顼ž���γ���ߥ��ꥢ
		 */
		sil_wrw_mem((uint32_t *)(hadc->base+TOFF_ADC_SR), ~ADC_SR_EOC);
	}

	/*
	 *  ���󥸥����ƥ��åȥ���ͥ�ν�λȽ��
	 */
	sr = sil_rew_mem((uint32_t *)(hadc->base+TOFF_ADC_SR));
	if((sr & ADC_SR_JEOC) != 0 && (cr1 & ADC_CR1_JEOCIE) != 0){
		if((cr2 & ADC_CR2_CONT) == 0)		/* ���ơ��������ѹ� */
			hadc->status = ADC_STATUS_EOC;
		else
			hadc->status = ADC_STATUS_BUSY_EOC;

		if(((hadc->Init.ContinuousConvMode == ADC_CONTINUOUS_DISABLE) || (cr1 & ADC_CR1_JAUTO) == 0) && (cr2 & ADC_CR2_JEXTEN) == 0){
			/*
			 *  ���󥸥����ƥ��åȥ���ͥ�ž�������
			 */
			sil_andw_mem((uint32_t *)(hadc->base+TOFF_ADC_CR1), ADC_CR1_JEOCIE);
		}

		/*
		 *  ���󥸥����ƥ��åȥ���ͥ����ߥ��ꥢ
		 */
		sil_wrw_mem((uint32_t *)(hadc->base+TOFF_ADC_SR), ~ADC_SR_JEOC);
	}

	/*
	 *  ���ʥ������å��ɥå�Ƚ��
	 */
	sr = sil_rew_mem((uint32_t *)(hadc->base+TOFF_ADC_SR));
	if((sr & ADC_SR_AWD) != 0 && (cr1 & ADC_CR1_AWDIE) != 0){
		/*
		 *  �����å��ɥå�ȯ�����֤��ѹ�
		 */
		hadc->status = ADC_STATUS_AWD;

		/*
		 *  �����å��ɥå�����ߥ��ꥢ
		 */
		sil_wrw_mem((uint32_t *)(hadc->base+TOFF_ADC_SR), ~ADC_SR_AWD);

		/*
		 *  ������ɥ��ΰ襢���ȤΥ�����Хå��ؿ��ɤ߽Ф�
		 */
		if(hadc->outofwincallback != NULL)
			hadc->outofwincallback(hadc);
	}

	/*
	 *  �����С���󥨥顼Ƚ��
	 */
	sr = sil_rew_mem((uint32_t *)(hadc->base+TOFF_ADC_SR));
	if((sr & ADC_SR_OVR) != 0 && (cr1 & ADC_CR1_OVRIE) != 0){
		/*
		 *  �����С����ȯ�����֤��ѹ�
		 */
		hadc->status = ADC_STATUS_READY;
		hadc->ErrorCode |= ADC_ERROR_OVR;

		/*
		 *  �����С�������ߥ��ꥢ
		 */
		sil_wrw_mem((uint32_t *)(hadc->base+TOFF_ADC_SR), ~ADC_SR_OVR);

		/*
		 *  ���顼������Хå��ؿ����ɤ߽Ф�
		 */
		if(hadc->errorcallback != NULL)
			hadc->errorcallback(hadc);
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
		if(hadc->status > ADC_STATUS_READY)
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
	uint32_t shift, clkpin;

	if(hadc == NULL || sConfig == NULL)
		return E_PAR;

	clkpin = (sConfig->GpioBase - TADR_GPIOA_BASE) / 0x400;
	if(clkpin >= 12 || sConfig->GpioPin >= 16)
		return E_PAR;

	/*
	 *  ����ͥ�ͭ������
	 */
	sil_orw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_AHB1ENR), 1<<clkpin);
	sil_rew_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_AHB1ENR));

	GPIO_InitStruct.mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.pull = GPIO_NOPULL;
	gpio_setup(sConfig->GpioBase, &GPIO_InitStruct, sConfig->GpioPin);

	/*
	 *  ����ץ�󥰻�������
	 */
	if (sConfig->Channel > ADC_CHANNEL_9){	/* ����ͥ�10����18 */
		shift = 3 * (sConfig->Channel - 10);
		sil_modw_mem((uint32_t *)(hadc->base+TOFF_ADC_SMPR1), (ADC_SMPR1_SMP10 << shift), (sConfig->SamplingTime << shift));
	}
	else{ 	/* ����ͥ�0����9 */
		shift = 3 * sConfig->Channel;
		sil_modw_mem((uint32_t *)(hadc->base+TOFF_ADC_SMPR2), (ADC_SMPR2_SMP0 << shift), (sConfig->SamplingTime << shift));
	}

	/*
	 *  �������
	 */
	if (sConfig->Rank < 7){	/* ���1����6 */
		shift = 5 * (sConfig->Rank - 1);
		sil_modw_mem((uint32_t *)(hadc->base+TOFF_ADC_SQR3), (ADC_SQR3_SQ1 << shift), (sConfig->Channel << shift));
	}
	else if (sConfig->Rank < 13){	/* ���7����12 */
		shift = 5 * (sConfig->Rank - 7);
		sil_modw_mem((uint32_t *)(hadc->base+TOFF_ADC_SQR2), (ADC_SQR2_SQ7 << shift), (sConfig->Channel << shift));
	}
	else{	/* ���13����16 */
		shift = 5 * (sConfig->Rank - 13);
		sil_modw_mem((uint32_t *)(hadc->base+TOFF_ADC_SQR1), (ADC_SQR1_SQ13 << shift), (sConfig->Channel << shift));
	}

	/*
	 *  �Ű������ѥ���ͥ�����
	 */
	if ((hadc->base == TADR_ADC1_BASE) && (sConfig->Channel == ADC_CHANNEL_VBAT)){
		sil_orw_mem((uint32_t *)(TADR_ADC_BASE+TOFF_ADC_CCR), ADC_CCR_VBATE);
	}

	/*
	 *  ���٥��󥵡����ѥ���ͥ�����
	 */
	if ((hadc->base == TADR_ADC1_BASE) && ((sConfig->Channel == ADC_CHANNEL_TEMPSENSOR) || (sConfig->Channel == ADC_CHANNEL_VREFINT))){
		sil_orw_mem((uint32_t *)(TADR_ADC_BASE+TOFF_ADC_CCR), ADC_CCR_TSVREFE);
		if((sConfig->Channel == ADC_CHANNEL_TEMPSENSOR)){
			sil_dly_nse(ADC_TEMPSENSOR_DELAY_US*1000);
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
	if(hadc == NULL || AnalogWDGConfig == NULL)
		return E_PAR;

	/*
	 *  ���������
	 */
	sil_modw_mem((uint32_t *)(hadc->base+TOFF_ADC_CR1), ADC_CR1_AWDIE, AnalogWDGConfig->ITMode);

	/*
	 *  �⡼������
	 */
	sil_andw_mem((uint32_t *)(hadc->base+TOFF_ADC_CR1), (ADC_CR1_AWDSGL | ADC_CR1_JAWDEN | ADC_CR1_AWDEN));

	/* Set the analog watchdog enable mode */
	sil_orw_mem((uint32_t *)(hadc->base+TOFF_ADC_CR1), AnalogWDGConfig->WatchdogMode);
	sil_wrw_mem((uint32_t *)(hadc->base+TOFF_ADC_HTR), AnalogWDGConfig->HighThreshold);
  
	/*
	 *  HIGH/LOW�Υ���å���ۡ��������
	 */
	sil_wrw_mem((uint32_t *)(hadc->base+TOFF_ADC_LTR), AnalogWDGConfig->LowThreshold);
	sil_andw_mem((uint32_t *)(hadc->base+TOFF_ADC_CR1), ADC_CR1_AWDCH);

	/*
	 *  �б�����ͥ�����
	 */
    sil_orw_mem((uint32_t *)(hadc->base+TOFF_ADC_CR1), AnalogWDGConfig->Channel);
	return E_OK;
}


