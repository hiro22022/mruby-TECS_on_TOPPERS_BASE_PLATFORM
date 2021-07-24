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
 *  @(#) $Id: adc.h 698 2016-02-24 21:08:38Z roi $
 */
/*
 * 
 *  STM32F4xx ADC�ǥХ����ɥ饤�Фγ������
 *
 */

#ifndef _ADC_H_
#define _ADC_H_

#ifdef __cplusplus
 extern "C" {
#endif

/*
 *  ADC�ݡ������
 */
#define ADC1_PORTID             1
#define ADC2_PORTID             2
#define ADC3_PORTID             3
#define NUM_ADCPORT             3

/*
 *  ADC����å��ץꥹ���������(ClockPrescaler)
 */ 
#define ADC_CLOCK_SYNC_PCLK_DIV2        0x00000000
#define ADC_CLOCK_SYNC_PCLK_DIV4        ADC_CCR_ADCPRE_0
#define ADC_CLOCK_SYNC_PCLK_DIV6        ADC_CCR_ADCPRE_1
#define ADC_CLOCK_SYNC_PCLK_DIV8        ADC_CCR_ADCPRE

/*
 *  ADC�꥾��塼��������(Resolution)
 */
#define ADC_RESOLUTION_12B              0x00000000
#define ADC_RESOLUTION_10B              ADC_CR1_RES_0
#define ADC_RESOLUTION_8B               ADC_CR1_RES_1
#define ADC_RESOLUTION_6B               ADC_CR1_RES

/*
 *  ADC�ǡ������饤�����(DataAlign)
 */ 
#define ADC_DATAALIGN_RIGHT             0x00000000
#define ADC_DATAALIGN_LEFT              ADC_CR2_ALIGN


/*
 *  ADC������󥳥�С������⡼��(ScanConvMode)
 */
#define ADC_SCANMODE_DISABLE            0x00000000
#define ADC_SCANMODE_ENABLE             ADC_CR1_SCAN

/*
 *  ADC EOC������������(EOCSelection)
 */
#define ADC_EOC_SEQ_DISABLE             0x00000000
#define ADC_EOC_SEQ_ENABLE              ADC_CR2_EOCS

/*
 *  ADC��³�⡼������(ContinuousConvMode)
 */
#define ADC_CONTINUOUS_DISABLE          0x00000000
#define ADC_CONTINUOUS_ENABLE           ADC_CR2_CONT

/*
 *  ADC DMA�������(DMAContinuousRequests)
 */
#define ADC_DMACONTINUOUS_DISABLE       0x00000000
#define ADC_DMANONECIRCULAR             0x80000000
#define ADC_DMACONTINUOUS_ENABLE        ADC_CR2_DDS
#define ADC_DMACONTINUOUS_ENABLE2       (ADC_CR2_DDS | ADC_DMANONECIRCULAR)

/*
 *  ADC ���³�⡼������(DiscontinuousConvMode)
 */
#define ADC_DISCONTINUOUS_DISABLE       0x00000000
#define ADC_DISCONTINUOUS_ENABLE        ADC_CR1_DISCEN

/*
 *  ADC�����ȥꥬ����������(ExternalTrigConv)
 */
#define ADC_EXTERNALTRIGCONV_T1_CC1     0x00000000
#define ADC_EXTERNALTRIGCONV_T1_CC2     ADC_CR2_EXTSEL_0
#define ADC_EXTERNALTRIGCONV_T1_CC3     ADC_CR2_EXTSEL_1
#define ADC_EXTERNALTRIGCONV_T2_CC2     (ADC_CR2_EXTSEL_1 | ADC_CR2_EXTSEL_0)
#define ADC_EXTERNALTRIGCONV_T2_CC3     ADC_CR2_EXTSEL_2
#define ADC_EXTERNALTRIGCONV_T2_CC4     (ADC_CR2_EXTSEL_2 | ADC_CR2_EXTSEL_0)
#define ADC_EXTERNALTRIGCONV_T2_TRGO    (ADC_CR2_EXTSEL_2 | ADC_CR2_EXTSEL_1)
#define ADC_EXTERNALTRIGCONV_T3_CC1     (ADC_CR2_EXTSEL_2 | ADC_CR2_EXTSEL_1 | ADC_CR2_EXTSEL_0)
#define ADC_EXTERNALTRIGCONV_T3_TRGO    ADC_CR2_EXTSEL_3
#define ADC_EXTERNALTRIGCONV_T4_CC4     (ADC_CR2_EXTSEL_3 | ADC_CR2_EXTSEL_0)
#define ADC_EXTERNALTRIGCONV_T5_CC1     (ADC_CR2_EXTSEL_3 | ADC_CR2_EXTSEL_1)
#define ADC_EXTERNALTRIGCONV_T5_CC2     (ADC_CR2_EXTSEL_3 | ADC_CR2_EXTSEL_1 | ADC_CR2_EXTSEL_0)
#define ADC_EXTERNALTRIGCONV_T5_CC3     (ADC_CR2_EXTSEL_3 | ADC_CR2_EXTSEL_2)
#define ADC_EXTERNALTRIGCONV_T8_CC1     (ADC_CR2_EXTSEL_3 | ADC_CR2_EXTSEL_2 | ADC_CR2_EXTSEL_0)
#define ADC_EXTERNALTRIGCONV_T8_TRGO    (ADC_CR2_EXTSEL_3 | ADC_CR2_EXTSEL_2 | ADC_CR2_EXTSEL_1)
#define ADC_EXTERNALTRIGCONV_Ext_IT11   ADC_CR2_EXTSEL
#define ADC_SOFTWARE_START              (ADC_CR2_EXTSEL + 1)

/*
 *  ADC�����ȥꥬ���å�����()
 */
#define ADC_EXTERNALTRIGCONVEDGE_NONE           0x00000000
#define ADC_EXTERNALTRIGCONVEDGE_RISING         ADC_CR2_EXTEN_0
#define ADC_EXTERNALTRIGCONVEDGE_FALLING        ADC_CR2_EXTEN_1
#define ADC_EXTERNALTRIGCONVEDGE_RISINGFALLING  ADC_CR2_EXTEN


/*
 *  ADC����ͥ����
 */
#define ADC_CHANNEL_0                   0x00000000
#define ADC_CHANNEL_1                   ADC_CR1_AWDCH_0
#define ADC_CHANNEL_2                   ADC_CR1_AWDCH_1
#define ADC_CHANNEL_3                   (ADC_CR1_AWDCH_1 | ADC_CR1_AWDCH_0)
#define ADC_CHANNEL_4                   ADC_CR1_AWDCH_2
#define ADC_CHANNEL_5                   (ADC_CR1_AWDCH_2 | ADC_CR1_AWDCH_0)
#define ADC_CHANNEL_6                   (ADC_CR1_AWDCH_2 | ADC_CR1_AWDCH_1)
#define ADC_CHANNEL_7                   (ADC_CR1_AWDCH_2 | ADC_CR1_AWDCH_1 | ADC_CR1_AWDCH_0)
#define ADC_CHANNEL_8                   ADC_CR1_AWDCH_3
#define ADC_CHANNEL_9                   (ADC_CR1_AWDCH_3 | ADC_CR1_AWDCH_0)
#define ADC_CHANNEL_10                  (ADC_CR1_AWDCH_3 | ADC_CR1_AWDCH_1)
#define ADC_CHANNEL_11                  (ADC_CR1_AWDCH_3 | ADC_CR1_AWDCH_1 | ADC_CR1_AWDCH_0)
#define ADC_CHANNEL_12                  (ADC_CR1_AWDCH_3 | ADC_CR1_AWDCH_2)
#define ADC_CHANNEL_13                  (ADC_CR1_AWDCH_3 | ADC_CR1_AWDCH_2 | ADC_CR1_AWDCH_0)
#define ADC_CHANNEL_14                  (ADC_CR1_AWDCH_3 | ADC_CR1_AWDCH_2 | ADC_CR1_AWDCH_1)
#define ADC_CHANNEL_15                  (ADC_CR1_AWDCH_3 | ADC_CR1_AWDCH_2 | ADC_CR1_AWDCH_1 | ADC_CR1_AWDCH_0)
#define ADC_CHANNEL_16                  ADC_CR1_AWDCH_4
#define ADC_CHANNEL_17                  (ADC_CR1_AWDCH_4 | ADC_CR1_AWDCH_0)
#define ADC_CHANNEL_18                  (ADC_CR1_AWDCH_4 | ADC_CR1_AWDCH_1)

#define ADC_CHANNEL_VREFINT             ADC_CHANNEL_17
#define ADC_CHANNEL_VBAT                ADC_CHANNEL_18

/*
 *  ADC����ץ�󥰻�������
 */
#define ADC_SAMPLETIME_3CYCLES          0x00000000
#define ADC_SAMPLETIME_15CYCLES         ADC_SMPR1_SMP10_0
#define ADC_SAMPLETIME_28CYCLES         ADC_SMPR1_SMP10_1
#define ADC_SAMPLETIME_56CYCLES         (ADC_SMPR1_SMP10_1 | ADC_SMPR1_SMP10_0)
#define ADC_SAMPLETIME_84CYCLES         ADC_SMPR1_SMP10_2
#define ADC_SAMPLETIME_112CYCLES        (ADC_SMPR1_SMP10_2 | ADC_SMPR1_SMP10_0)
#define ADC_SAMPLETIME_144CYCLES        (ADC_SMPR1_SMP10_2 | ADC_SMPR1_SMP10_1)
#define ADC_SAMPLETIME_480CYCLES        ADC_SMPR1_SMP10


/*
 *  ADC���ʥ������å��ɥå��⡼������
 */
#define ADC_ANALOGWATCHDOG_NONE               0x00000000
#define ADC_ANALOGWATCHDOG_SINGLE_REG         (ADC_CR1_AWDSGL | ADC_CR1_AWDEN)
#define ADC_ANALOGWATCHDOG_SINGLE_INJEC       (ADC_CR1_AWDSGL | ADC_CR1_JAWDEN)
#define ADC_ANALOGWATCHDOG_SINGLE_REGINJEC    (ADC_CR1_AWDSGL | ADC_CR1_AWDEN | ADC_CR1_JAWDEN)
#define ADC_ANALOGWATCHDOG_ALL_REG            ADC_CR1_AWDEN
#define ADC_ANALOGWATCHDOG_ALL_INJEC          ADC_CR1_JAWDEN
#define ADC_ANALOGWATCHDOG_ALL_REGINJEC       (ADC_CR1_AWDEN | ADC_CR1_JAWDEN)

/*
 *  ADC���ʥ������å��ɥå�����ߥ⡼������
 */
#define ADC_ANALOGWATCHDOG_ITMODE_DISABLE     0x00000000
#define ADC_ANALOGWATCHDOG_ITMODE_ENABLE      ADC_CR1_AWDIE

/*
 *  ADC�ϡ��ɥ��������깽¤��
 */
typedef struct _ADC_PortControlBlock{
	uint32_t              base;
	uint32_t              adcclockbase;
	uint32_t              adcclockbit;
	uint32_t              dmaclockbase;
	uint32_t              dmaclockbit;
	uint32_t              dmarxbase;
	uint32_t              dmarxchannel;
} ADC_PortControlBlock;

/*
 *  ADC�ν������¤��
 */
typedef struct
{
	uint32_t              ClockPrescaler;				/* ADC����å��ץꥹ��������(��ADC�ˤƶ�����) */
	uint32_t              Resolution;					/* ADC�Υ꥾��塼����� */
	uint32_t              DataAlign;					/* ADC��̥ǡ������饤������ */
	uint32_t              ScanConvMode;					/* ADC������󥳥�С������⡼�� */
	uint32_t              EOCSelection;					/* ADC EOC���� */
	uint32_t              ContinuousConvMode;			/* ADC��³�⡼�� */
	uint32_t              DMAContinuousRequests;		/* ADC-DMA�⡼�� */
	uint32_t              NumConversion;				/* ADC����С������� */
	uint32_t              DiscontinuousConvMode;		/* ADC���³�⡼�� */
	uint32_t              NumDiscConversion;			/* ADC���³�Ѵ��� */
	uint32_t              ExternalTrigConv;				/* ADC�����ȥꥬ���� */
	uint32_t              ExternalTrigConvEdge;			/* ADC�����ȥꥬ���å����� */
}ADC_Init_t;

/*
 *  ADC�ϥ�ɥ鹽¤��
 */ 
typedef struct __ADC_Handle_t ADC_Handle_t;
struct __ADC_Handle_t
{
	uint32_t              base;							/* ADC�١������ɥ쥹 */
	ADC_Init_t            Init;							/* ADC������ǡ��� */
	volatile uint32_t     NumCurrentConversionRank;		/* ADC���ߤΥ���С���������� */
	DMA_Handle_t          *hdmarx;						/* ADC��DMA�ϥ�ɥ�ؤΥݥ��� */
	void                  (*xfercallback)(ADC_Handle_t * hadc);		/* ADC ž����λ������Хå� */
	void                  (*xferhalfcallback)(ADC_Handle_t * hadc);	/* ADC Ⱦʬž��������Хå� */
	void                  (*outofwincallback)(ADC_Handle_t * hadc);	/* ADC ���ʥ������å��ɥå�������Хå� */
	void                  (*errorcallback)(ADC_Handle_t * hadc);	/* ADC ���顼������Хå� */
	volatile uint32_t     status;						/* ADC���ơ����� */
	volatile uint32_t     ErrorCode;					/* ADC���顼�� */
};

/*
 *  �����ͥ����깽¤��
 */
typedef struct
{
	uint32_t              Channel;						/* ADC�����ͥ��ֹ� */
	uint32_t              Rank;							/* ADC����ֹ�(1-16) */
	uint32_t              SamplingTime;					/* ADC����ץ�󥰻��� */
	uint32_t              GpioBase;						/* ADC-GPIO�١������ɥ쥹 */
	uint32_t              GpioPin;						/* ADC-GPIO�ԥ��ֹ� */
}ADC_ChannelConf_t;

/*
 *  ADC���ʥ������å��ɥå����깽¤��
 */
typedef struct
{
	uint32_t              WatchdogMode;					/* ADC���ʥ������å��ɥå��⡼�� */
	uint32_t              HighThreshold;				/* ADC����å���ۡ���ɾ���� */
	uint32_t              LowThreshold;					/* ADC����å���ۡ���ɲ����� */
	uint32_t              Channel;						/* ADC�б�����ͥ��ֹ� */
	uint32_t              ITMode;						/* ADC���ʥ������å��ɥå���������� */
}ADC_AnalogWDGConf_t;

/*
 *  ADC�Υ��ơ��������
 */
#define ADC_STATUS_RESET        0x00    /* ADC ̤���Ѿ��� */
#define ADC_STATUS_READY        0x01    /* ADC ��ǥ����� */
#define ADC_STATUS_BUSY         0x02    /* ADC �ӥ������� */
#define ADC_STATUS_EOC          0x10    /* ADC ��λ���� */
#define ADC_STATUS_BUSY_EOC     0x12    /* ADC �ӥ�����λȯ������ */ 
#define ADC_STATUS_AWD          0x20    /* ADC ���ʥ������å��ɥå�ȯ������ */

/*
 *  ADC���顼���
 */
#define ADC_ERROR_NONE          0x00	/* No error             */
#define ADC_ERROR_OVR           0x01	/* OVR error            */
#define ADC_ERROR_DMA           0x02	/* DMA transfer error   */


extern ADC_Handle_t *adc_init(ID portid, ADC_Init_t *pini);
extern ER adc_deinit(ADC_Handle_t *hadc);
extern ER adc_start_int(ADC_Handle_t* hadc);
extern ER adc_end_int(ADC_Handle_t* hadc);
extern ER adc_start_dma(ADC_Handle_t* hadc, uint32_t* pdata, uint32_t length);
extern ER adc_end_dma(ADC_Handle_t* hadc);
extern ER adc_setupchannel(ADC_Handle_t* hadc, ADC_ChannelConf_t* sConfig);
extern ER adc_setupwatchdog(ADC_Handle_t* hadc, ADC_AnalogWDGConf_t* AnalogWDGConfig);
extern uint32_t adc_getvalue(ADC_Handle_t* hadc);

extern void adc_handler(ADC_Handle_t* hadc);
extern void adc_int_handler(void);

#ifdef __cplusplus
}
#endif

#endif	/* _ADC_H_ */

