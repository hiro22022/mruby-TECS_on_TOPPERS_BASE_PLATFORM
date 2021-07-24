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
 *  @(#) $Id: adc.h 698 2017-07-07 10:08:38Z roi $
 */
/*
 * 
 *  STM32L0xx ADC�ǥХ����ɥ饤�Фγ������
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
#define NUM_ADCPORT             1

/*
 *  ADC����å��ץꥹ���������(ClockPrescaler)
 */
#define ADC_CLOCK_ASYNC_DIV1            0x00000000							/* ADC Asynchronous clock mode divided by 1 */
#define ADC_CLOCK_ASYNC_DIV2            ADC_CCR_PRESC_0						/* ADC Asynchronous clock mode divided by 2 */
#define ADC_CLOCK_ASYNC_DIV4            ADC_CCR_PRESC_1						/* ADC Asynchronous clock mode divided by 2 */
#define ADC_CLOCK_ASYNC_DIV6            (ADC_CCR_PRESC_1 | ADC_CCR_PRESC_0)	/* ADC Asynchronous clock mode divided by 2 */
#define ADC_CLOCK_ASYNC_DIV8            ADC_CCR_PRESC_2						/* ADC Asynchronous clock mode divided by 2 */
#define ADC_CLOCK_ASYNC_DIV10           (ADC_CCR_PRESC_2 | ADC_CCR_PRESC_0)	/* ADC Asynchronous clock mode divided by 2 */
#define ADC_CLOCK_ASYNC_DIV12           (ADC_CCR_PRESC_2 | ADC_CCR_PRESC_1)	/* ADC Asynchronous clock mode divided by 2 */
#define ADC_CLOCK_ASYNC_DIV16           (ADC_CCR_PRESC_2 | ADC_CCR_PRESC_1 | ADC_CCR_PRESC_0)	/* ADC Asynchronous clock mode divided by 2 */
#define ADC_CLOCK_ASYNC_DIV32           ADC_CCR_PRESC_3						/* ADC Asynchronous clock mode divided by 2 */
#define ADC_CLOCK_ASYNC_DIV64           (ADC_CCR_PRESC_3 | ADC_CCR_PRESC_0)	/* ADC Asynchronous clock mode divided by 2 */
#define ADC_CLOCK_ASYNC_DIV128          (ADC_CCR_PRESC_3 | ADC_CCR_PRESC_1)	/*!< ADC Asynchronous clock mode divided by 2 */
#define ADC_CLOCK_ASYNC_DIV256          (ADC_CCR_PRESC_3 | ADC_CCR_PRESC_1 | ADC_CCR_PRESC_0)	/* ADC Asynchronous clock mode divided by 2 */

#define ADC_CLOCK_SYNC_PCLK_DIV1        ADC_CFGR2_CKMODE					/* Synchronous clock mode divided by 1 */
#define ADC_CLOCK_SYNC_PCLK_DIV2        ADC_CFGR2_CKMODE_0					/* Synchronous clock mode divided by 2 */
#define ADC_CLOCK_SYNC_PCLK_DIV4        ADC_CFGR2_CKMODE_1					/* Synchronous clock mode divided by 4 */

/*
 *  ADC�꥾��塼��������(Resolution)
 */
#define ADC_RESOLUTION_12B              0x00000000
#define ADC_RESOLUTION_10B              ADC_CFGR1_RES_0
#define ADC_RESOLUTION_8B               ADC_CFGR1_RES_1
#define ADC_RESOLUTION_6B               ADC_CFGR1_RES

/*
 *  ADC�ǡ������饤�����(DataAlign)
 */ 
#define ADC_DATAALIGN_RIGHT             0x00000000
#define ADC_DATAALIGN_LEFT              ADC_CFGR1_ALIGN

/*
 *  ADC������󥳥�С������⡼��(ScanConvMode)
 */
#define ADC_SCANMODE_DISABLE            0x00000000
#define ADC_SCANMODE_ENABLE             0x00000000
#define ADC_SCAN_DIRECTION_FORWARD      ADC_SCANMODE_ENABLE
#define ADC_SCAN_DIRECTION_BACKWARD     ADC_CFGR1_SCANDIR

/*
 *  ADC EOC������������(EOCSelection)
 */
#define ADC_EOC_SEQ_DISABLE             0x00000000
#define ADC_EOC_SINGLE_CONV             ADC_ISR_EOC
#define ADC_EOC_SEQ_CONV                ADC_ISR_EOS
#define ADC_EOC_SINGLE_SEQ_CONV         (ADC_ISR_EOC | ADC_ISR_EOS)
#define ADC_EOC_SEQ_ENABLE              ADC_EOC_SINGLE_SEQ_CONV

/*
 *  ADC��³�⡼������(ContinuousConvMode)
 */
#define ADC_CONTINUOUS_DISABLE          0x00000000
#define ADC_CONTINUOUS_ENABLE           ADC_CFGR1_CONT

/*
 *  ADC DMA�������(DMAContinuousRequests)
 */
#define ADC_DMACONTINUOUS_DISABLE       0x00000000
#define ADC_DMACONTINUOUS_ENABLE        ADC_CFGR1_DMACFG

/*
 *  ADC ���³�⡼������(DiscontinuousConvMode)
 */
#define ADC_DISCONTINUOUS_DISABLE       0x00000000
#define ADC_DISCONTINUOUS_ENABLE        ADC_CFGR1_DISCEN

/*
 *  ADC�����ȥꥬ����������(ExternalTrigConv)
 */
#define ADC_EXTERNALTRIGCONV_T6_TRGO    0x00000000
#define ADC_EXTERNALTRIGCONV_T21_CC2    (ADC_CFGR1_EXTSEL_0)
#define ADC_EXTERNALTRIGCONV_T2_TRGO    (ADC_CFGR1_EXTSEL_1)
#define ADC_EXTERNALTRIGCONV_T2_CC4     (ADC_CFGR1_EXTSEL_1 | ADC_CFGR1_EXTSEL_0)
#define ADC_EXTERNALTRIGCONV_T22_TRGO   (ADC_CFGR1_EXTSEL_2)
#define ADC_EXTERNALTRIGCONV_T3_TRGO    (ADC_CFGR1_EXTSEL_2 | ADC_CFGR1_EXTSEL_1)
#define ADC_EXTERNALTRIGCONV_EXT_IT11   (ADC_CFGR1_EXTSEL_2 | ADC_CFGR1_EXTSEL_1 | ADC_CFGR1_EXTSEL_0)
#define ADC_SOFTWARE_START              (ADC_CFGR1_EXTSEL + 1)

/*
 *  ADC�����ȥꥬ���å�����(ExternalTrigConvEdge)
 */
#define ADC_EXTERNALTRIGCONVEDGE_NONE           0x00000000
#define ADC_EXTERNALTRIGCONVEDGE_RISING         ADC_CFGR1_EXTEN_0
#define ADC_EXTERNALTRIGCONVEDGE_FALLING        ADC_CFGR1_EXTEN_1
#define ADC_EXTERNALTRIGCONVEDGE_RISINGFALLING  ADC_CFGR1_EXTEN

/*
 *  ADC����ץ�󥰻�������
 */
#define ADC_SAMPLETIME_1CYCLE_5         0x00000000			/* ADC sampling time 1.5 cycle */
#define ADC_SAMPLETIME_3CYCLES_5        ADC_SMPR_SMP_0		/* ADC sampling time 3.5 CYCLES */
#define ADC_SAMPLETIME_7CYCLES_5        ADC_SMPR_SMP_1		/* ADC sampling time 7.5 CYCLES */
#define ADC_SAMPLETIME_12CYCLES_5       (ADC_SMPR_SMP_1 | ADC_SMPR_SMP_0)	/* ADC sampling time 12.5 CYCLES */
#define ADC_SAMPLETIME_19CYCLES_5       ADC_SMPR_SMP_2		/* ADC sampling time 19.5 CYCLES */
#define ADC_SAMPLETIME_39CYCLES_5       (ADC_SMPR_SMP_2 | ADC_SMPR_SMP_0)	/* ADC sampling time 39.5 CYCLES */
#define ADC_SAMPLETIME_79CYCLES_5       (ADC_SMPR_SMP_2 | ADC_SMPR_SMP_1)	/* ADC sampling time 79.5 CYCLES */
#define ADC_SAMPLETIME_160CYCLES_5      ADC_SMPR_SMP		/* ADC sampling time 160.5 CYCLES */

/*
 * ADC���ѥ�����ȥѥ����
 */
#define ADC_LOWAUTOPOWEROFF_DISABLE     0x00000000
#define ADC_LOWAUTOPOWEROFF_ENABLE      ADC_CFGR1_AUTOFF

/*
 *  ADC���ѥFREQUENCY����
 */
#define ADC_LOWFREQUENCY_DISABLE        0x00000000
#define ADC_LOWFREQUENCY_ENABLE         ADC_CCR_LFMEN

/*
 *  ADC���ѥ�����ȥ�������
 */
#define ADC_LOWAUTOWAIT_DISABLE         0x00000000
#define ADC_LOWAUTOWAIT_ENABLE          ADC_CFGR1_WAIT

/*
 *  ADC�����С��ǡ����饤�ƥ���
 */
#define ADC_OVR_DATA_PRESERVED          0x00000000
#define ADC_OVR_DATA_OVERWRITTEN        ADC_CFGR1_OVRMOD

/*
 *  ADC�����С�����ץ�󥰥⡼��
 */
#define ADC_OVR_SAMPLING_DISABLE        0x00000000
#define ADC_OVR_SAMPLING_ENABLE         ADC_CFGR2_OVSE

/*
 *  ADC�����С�����ץ�󥰥�ƥ���
 */
#define ADC_OVERSAMPLING_RATIO_2        0x00000000
#define ADC_OVERSAMPLING_RATIO_4        ADC_CFGR2_OVSR_0
#define ADC_OVERSAMPLING_RATIO_8        ADC_CFGR2_OVSR_1
#define ADC_OVERSAMPLING_RATIO_16       (ADC_CFGR2_OVSR_0 | ADC_CFGR2_OVSR_1)
#define ADC_OVERSAMPLING_RATIO_32       ADC_CFGR2_OVSR_2
#define ADC_OVERSAMPLING_RATIO_64       (ADC_CFGR2_OVSR_0 | ADC_CFGR2_OVSR_2)
#define ADC_OVERSAMPLING_RATIO_128      (ADC_CFGR2_OVSR_1 | ADC_CFGR2_OVSR_2)
#define ADC_OVERSAMPLING_RATIO_256      ADC_CFGR2_OVSR

/*
 *  ADC�����С�����ץ�󥰥饤�ȥӥåȥ��ե�
 */
#define ADC_RIGHTBITSHIFT_NONE          0x00000000
#define ADC_RIGHTBITSHIFT_1             ADC_CFGR2_OVSS_0
#define ADC_RIGHTBITSHIFT_2             ADC_CFGR2_OVSS_1
#define ADC_RIGHTBITSHIFT_3             (ADC_CFGR2_OVSS_0 | ADC_CFGR2_OVSS_1)
#define ADC_RIGHTBITSHIFT_4             ADC_CFGR2_OVSS_2
#define ADC_RIGHTBITSHIFT_5             (ADC_CFGR2_OVSS_0 | ADC_CFGR2_OVSS_2)
#define ADC_RIGHTBITSHIFT_6             (ADC_CFGR2_OVSS_1 | ADC_CFGR2_OVSS_2)
#define ADC_RIGHTBITSHIFT_7             (ADC_CFGR2_OVSS_0 | ADC_CFGR2_OVSS_1 | ADC_CFGR2_OVSS_2)
#define ADC_RIGHTBITSHIFT_8             ADC_CFGR2_OVSS_3

/*
 *  ADC�����С�����ץ�󥰥ȥꥬ
 */
#define ADC_TRIGGEREDMODE_SINGLE_TRIGGER    0x00000000
#define ADC_TRIGGEREDMODE_MULTI_TRIGGER     ADC_CFGR2_TOVS


/*
 *  ADC����ͥ����
 */
#define ADC_CHANNEL_0                   ADC_CHSELR_CHSEL0
#define ADC_CHANNEL_1                   (ADC_CHSELR_CHSEL1 | ADC_CFGR1_AWDCH_0)
#define ADC_CHANNEL_2                   (ADC_CHSELR_CHSEL2 | ADC_CFGR1_AWDCH_1)
#define ADC_CHANNEL_3                   (ADC_CHSELR_CHSEL3 | ADC_CFGR1_AWDCH_1 | ADC_CFGR1_AWDCH_0)
#define ADC_CHANNEL_4                   (ADC_CHSELR_CHSEL4 | ADC_CFGR1_AWDCH_2)
#define ADC_CHANNEL_5                   (ADC_CHSELR_CHSEL5 | ADC_CFGR1_AWDCH_2| ADC_CFGR1_AWDCH_0)
#define ADC_CHANNEL_6                   (ADC_CHSELR_CHSEL6 | ADC_CFGR1_AWDCH_2| ADC_CFGR1_AWDCH_1)
#define ADC_CHANNEL_7                   (ADC_CHSELR_CHSEL7 | ADC_CFGR1_AWDCH_2| ADC_CFGR1_AWDCH_1 | ADC_CFGR1_AWDCH_0)
#define ADC_CHANNEL_8                   (ADC_CHSELR_CHSEL8 | ADC_CFGR1_AWDCH_3)
#define ADC_CHANNEL_9                   (ADC_CHSELR_CHSEL9 | ADC_CFGR1_AWDCH_3| ADC_CFGR1_AWDCH_0)
#define ADC_CHANNEL_10                  (ADC_CHSELR_CHSEL10 | ADC_CFGR1_AWDCH_3| ADC_CFGR1_AWDCH_1)
#define ADC_CHANNEL_11                  (ADC_CHSELR_CHSEL11 | ADC_CFGR1_AWDCH_3| ADC_CFGR1_AWDCH_1| ADC_CFGR1_AWDCH_0)
#define ADC_CHANNEL_12                  (ADC_CHSELR_CHSEL12 | ADC_CFGR1_AWDCH_3| ADC_CFGR1_AWDCH_2)
#define ADC_CHANNEL_13                  (ADC_CHSELR_CHSEL13 | ADC_CFGR1_AWDCH_3| ADC_CFGR1_AWDCH_2| ADC_CFGR1_AWDCH_0)
#define ADC_CHANNEL_14                  (ADC_CHSELR_CHSEL14 | ADC_CFGR1_AWDCH_3| ADC_CFGR1_AWDCH_2| ADC_CFGR1_AWDCH_1)
#define ADC_CHANNEL_15                  (ADC_CHSELR_CHSEL15 | ADC_CFGR1_AWDCH_3| ADC_CFGR1_AWDCH_2| ADC_CFGR1_AWDCH_1| ADC_CFGR1_AWDCH_0)
#if defined(ADC_CHSELR_CHSEL16)
#define ADC_CHANNEL_16                  (ADC_CHSELR_CHSEL16 | ADC_CFGR1_AWDCH_4)
#endif
#define ADC_CHANNEL_17                  (ADC_CHSELR_CHSEL17 | ADC_CFGR1_AWDCH_4| ADC_CFGR1_AWDCH_0)
#define ADC_CHANNEL_18                  (ADC_CHSELR_CHSEL18 | ADC_CFGR1_AWDCH_4| ADC_CFGR1_AWDCH_1)

#define ADC_CHANNEL_VLCD                ADC_CHANNEL_16
#define ADC_CHANNEL_VREFINT             ADC_CHANNEL_17
#define ADC_CHANNEL_TEMPSENSOR          ADC_CHANNEL_18

/*
 *  ADC����ͥ���
 */
#define ADC_RANK_CHANNEL_NUMBER         0x00001000	/* ����ͥ�Υ������ͭ�� */
#define ADC_RANK_NONE                   0x00001001	/* ����ͥ�Υ������̵�� */

/*
 *  ADC���ʥ������å��ɥå��⡼������
 */
#define ADC_ANALOGWATCHDOG_NONE         0x00000000
#define ADC_ANALOGWATCHDOG_SINGLE_REG   (ADC_CFGR1_AWDSGL | ADC_CFGR1_AWDEN))
#define ADC_ANALOGWATCHDOG_ALL_REG      ADC_CFGR1_AWDEN

/*
 *  ADC���ʥ������å��ɥå�����ߥ⡼������
 */
#define ADC_ANALOGWATCHDOG_ITMODE_DISABLE     0x00000000
#define ADC_ANALOGWATCHDOG_ITMODE_ENABLE      ADC_IER_AWDIE

/*
 *  ADC�ϡ��ɥ��������깽¤��
 */
typedef struct _ADC_PortControlBlock{
	uint32_t              base;
	uint32_t              adcclockbase;
	uint32_t              adcclockbit;
	uint32_t              dmaclockbase;
	uint32_t              dmaclockbit;
	uint32_t              dmarxchannel;
	uint32_t              dmarxrequest;
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
	uint32_t              DiscontinuousConvMode;		/* ADC���³�⡼�� */
	uint32_t              ExternalTrigConv;				/* ADC�����ȥꥬ���� */
	uint32_t              ExternalTrigConvEdge;			/* ADC�����ȥꥬ���å����� */
 	uint32_t              SamplingTime;					/* ADC����ץ�󥰻��� */

	uint32_t              LowPowerAutoPowerOff;			/* ADC���ѥ�����ȥ���ץ�󥰥��� */
	uint32_t              LowPowerFrequencyMode;		/* ADC���ѥ�ե쥭�奨�󥷥��⡼�� */
	uint32_t              LowPowerAutoWait;				/* ADC���ѥ�����ȥ������� */
	uint32_t              Overrun;						/* ADCDR�쥸�����Υ����С����񤭹������� */
	uint32_t              OversamplingMode;				/* ADC�����С���󥵥�ץ������ */
	uint32_t              OversamplingRatio;			/* ADC�����С���󥵥�ץ�� Ratio. */
	uint32_t              OversamplingRightBitShift;	/* ADC�����С���󥵥�ץ�� Configures the division coefficient */
	uint32_t              OversamplingTriggeredMode;	/* ADC�����С���󥵥�ץ�� triggered  mode. */
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
	const ADC_PortControlBlock  *apcb;					/* ADC PCB */
	void                  (*xfercallback)(ADC_Handle_t * hadc);		/* ADC ž����λ������Хå� */
	void                  (*xferhalfcallback)(ADC_Handle_t * hadc);	/* ADC Ⱦʬž��������Хå� */
	void                  (*outofwincallback)(ADC_Handle_t * hadc);	/* ADC ���ʥ������å��ɥå�������Хå� */
	void                  (*errorcallback)(ADC_Handle_t * hadc);	/* ADC ���顼������Хå� */
	volatile uint32_t     status;						/* ADC���ơ����� */
	volatile uint32_t     errorcode;					/* ADC���顼�� */
};

/*
 *  �����ͥ����깽¤��
 */
typedef struct
{
	uint32_t              Channel;						/* ADC�����ͥ��ֹ� */
	uint32_t              Rank;							/* ADC����ֹ�(1-16) */
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
#define ADC_STATUS_ERROR		0x40	/* ADC ���顼���� */

/*
 *  ADC���顼���
 */
#define ADC_ERROR_NONE          0x00	/* No error             */
#define ADC_ERROR_OVR           0x01	/* OVR error            */
#define ADC_ERROR_DMA           0x02	/* DMA transfer error   */
#define ADC_ERROR_INT           0x04	/* INTERNAL error       */


extern ADC_Handle_t *adc_init(ID portid, ADC_Init_t *pini);
extern ER adc_deinit(ADC_Handle_t *hadc);
extern ER adc_start_int(ADC_Handle_t* hadc);
extern ER adc_end_int(ADC_Handle_t* hadc);
extern ER adc_start_dma(ADC_Handle_t* hadc, uint16_t* pdata, uint32_t length);
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

