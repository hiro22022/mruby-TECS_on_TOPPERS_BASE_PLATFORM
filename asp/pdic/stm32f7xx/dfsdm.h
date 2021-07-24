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
 *  @(#) $Id: dfsdm.h 698 2017-03-19 18:40:55Z roi $
 */
/*
 * 
 *  STM32F7xx DFSDM�ǥХ����ɥ饤�Фγ������
 *
 */

#ifndef _DFSDM_H_
#define _DFSDM_H_

#ifdef __cplusplus
 extern "C" {
#endif

/*
 *  DFSDM CHANNEL�������
 */
#define DFSDM_CHANNEL_STATE_RESET   0x00000000	/* DFSDM CHANNEL�ꥻ�åȾ��� */
#define DFSDM_CHANNEL_STATE_READY   0x00000001	/* DFSDM CHANNEL��ǥ����� */
#define DFSDM_CHANNEL_STATE_ERROR   0x00080000	/* DFSDM CHANNEL���顼���� */

/*
 *  DFSDM CHANNEL��������
 */
typedef struct
{
	uint32_t                OutClockActivation;		/* �����ȥץåȥ���å�ͭ��̵�� */
	uint32_t                OutClockSelection;		/* �����ȥץåȥ���å����� */
	uint32_t                OutClockDivider;		/* �����ȥץåȥ���å��ǥХ����� */
	uint32_t                InputMultiplexer;		/* ���ϡ��������ꥢ��ޤ��������쥸���� */
	uint32_t                InputDataPacking;		/* ���ϡ��ǡ����ԥå��� */
	uint32_t                InputPins;				/* ���ϡ��ԥ� */
	uint32_t                SerialType;				/* ���ꥢ����̡�SPI or Manchester modes. */
	uint32_t                SerialSpiClock;			/* SPI����å�����(external or internal with different sampling point) */
	uint32_t                AwdFilterOrder;			/* ���ʥ������å��ɥå� ���󥯥ե��륿����� */
	uint32_t                AwdOversampling;		/* ���ʥ������å��ɥå� oversampli�����С�����ץ�� RATIO */
	int32_t                 Offset;					/* DFSDM����ͥ륪�ե��å� */
	uint32_t                RightBitShift;			/* DFSDM����ͥ뱦�ӥåȥ��ե� */
}DFSDM_Channel_Init_t;

/*
 *  DFSDM CHANNEL�ϥ�ɥ����
 */
typedef struct DFSDM_Channel_struct DFSDM_Channel_Handle_t;
struct DFSDM_Channel_struct
{
	uint32_t                base;					/* DFSDM����ͥ�١������ɥ쥹 */
	DFSDM_Channel_Init_t    Init;					/* DFSDM������ǡ��� */
	void                    (*ckabcallback)(DFSDM_Channel_Handle_t * hdfsc);	/* Channel Ckab������Хå��ؿ� */
	void                    (*scdcallback)(DFSDM_Channel_Handle_t * hdfsc);		/* Channel Scd������Хå��ؿ� */
	volatile uint32_t       state;					/* DFSDM����ͥ���� */
};

/*
 *  DFSDM�ե��륿���������
 */
#define DFSDM_FILTER_STATE_RESET    0x00000000	/* DFSDM�ե��륿���ꥻ�åȾ��� */
#define DFSDM_FILTER_STATE_READY    0x00000001	/* DFSDM�ե��륿����ǥ����� */
#define DFSDM_FILTER_STATE_REG      0x00000002	/* DFSDM�ե��륿���쥮��顼�Ѵ����� */
#define DFSDM_FILTER_STATE_INJ      0x00000003	/* DFSDM�ե��륿�����󥸥������Ѵ����� */
#define DFSDM_FILTER_STATE_REG_INJ  0x00000004	/* DFSDM�ե��륿���쥮��顼�����󥸥������Ѵ����� */
#define DFSDM_FILTER_STATE_ERROR    0x00080000	/* DFSDM�ե��륿�����顼���� */

/*
 *  DFSDM�ե��륿�����顼���e
 */
#define DFSDM_FILTER_ERROR_NONE             0x00000000	/* ���顼�ʤ� */
#define DFSDM_FILTER_ERROR_REGULAR_OVERRUN  0x00000001	/* �쥮��顼�Ѵ������Х�󥨥顼 */
#define DFSDM_FILTER_ERROR_INJECTED_OVERRUN 0x00000002	/* ���󥸥������Ѵ������С���󥨥顼 */
#define DFSDM_FILTER_ERROR_DMA              0x00000003	/* DMA���顼 */

/*
 *  DFSDM�ե��륿����������
 */
typedef struct
{
	uint32_t                RegTrigger;				/* �쥮��顼�Ѵ���software or synchronous. */
	uint32_t                RegFastMode;			/* �쥮��顼�Ѵ���fast mode Enable/disable fast mode */
	uint32_t                RegDmaMode;				/* �쥮��顼�Ѵ���DMA Enable/disable */
	uint32_t                InjTrigger;				/* ���󥸥������Ѵ���Trigger: software, external or synchronous. */
	uint32_t                InjScanMode;			/* ���󥸥������Ѵ���scanning mode Enable/disable */
	uint32_t                InjDmaMode;				/* ���󥸥������Ѵ���DMA Enable/disable */
	uint32_t                InjExtTrigger;			/* ���󥸥������Ѵ���External trigger */
	uint32_t                InjExtTriggerEdge;		/* ���󥸥������Ѵ���External trigger edge: rising, falling or both. */
	uint32_t                FilterSincOrder;		/* Sinc filter order. */
	uint32_t                FilterOversampling;		/* Filter oversampling ratio. */
	uint32_t                FilterIntOversampling;	/* Integrator oversampling ratio. */
}DFSDM_Filter_Init_t;

/*
 *  DFSDM�ե��륿���ϥ�ɥ����
 */
typedef struct DFSDM_Filter_struct DFSDM_Filter_Handle_t;
struct DFSDM_Filter_struct
{
	uint32_t                base;					/* DFSDM�ե��륿���١������ɥ쥹 */
	DFSDM_Filter_Init_t     Init;					/* DFSDM�ե��륿��������ǡ��� */
	DMA_Handle_t            *hdmaReg;				/* �쥮��顼�Ѵ���DMA�ϥ�ɥ�ؤΥݥ��� */
	DMA_Handle_t            *hdmaInj;				/* ���󥸥������Ѵ���DMA�ϥ�ɥ�ؤΥݥ��� */
	uint32_t                RegularContMode;		/* �쥮��顼�Ѵ���continuous mode */
	uint32_t                InjectedChannelsNbr;	/* ���󥸥������Ѵ���Number of channels */
	uint32_t                InjConvRemaining;		/* ���󥸥������Ѵ���remaining */
	void                    (*regconvcallback)(DFSDM_Filter_Handle_t * hdfsf);		/* RegConv������Хå��ؿ� */
	void                    (*regconvhalfcallback)(DFSDM_Filter_Handle_t * hdfsf);	/* RegConv half������Хå��ؿ� */
	void                    (*injconvcallback)(DFSDM_Filter_Handle_t * hdfsf);		/* InjConv������Хå��ؿ� */
	void                    (*injconvhalfcallback)(DFSDM_Filter_Handle_t * hdfsf);	/* InjConv half������Хå��ؿ� */
	void                    (*awdconvcallback)(DFSDM_Filter_Handle_t * hdfsf);		/* Analog Wd������Хå��ؿ� */
	void                    (*errorcallback)(DFSDM_Filter_Handle_t * hdfsf);		/* ���顼������Хå��ؿ� */
	volatile uint32_t       state;					/* DFSDM�ե��륿������ */
	volatile uint32_t       errorcode;				/* DFSDM�ե��륿�����顼 */
};

/*
 *  DFSDM�ե��륿�� analog watchdog parameters���
 */
typedef struct
{
	uint32_t                DataSource;				/* �ǡ�����������digital filter or from channel watchdog filter. */
	uint32_t                Channel;				/* ����ͥ�����. */
	int32_t                 HighThreshold;			/* �ϥ�����å����� */
	int32_t                 LowThreshold;			/* ������å����� */
	uint32_t                HighBreakSignal;		/* �ϥ�����åɡ��֥졼�������ʥ� */
	uint32_t                LowBreakSignal;			/* ������åɡ��֥졼�������ʥ� */
} DFSDM_Filter_AwdParamTypeDef;


/*
 *  DFSDM���͡��֥롦�ǥ����֥����
 */
#define DFSDM_DISABLE               0x00000000
#define DFSDM_ENABLE                0x00000001
#define DFSDM_MASK                  (~DFSDM_ENABLE)

/*
 *  DFSDM����ͥ� OutClockSelection���
 */
#define DFSDM_CHANNEL_OUTPUT_CLOCK_SYSTEM   0x00000000				/* ��������ouput clock is system clock */
#define DFSDM_CHANNEL_OUTPUT_CLOCK_AUDIO    DFSDM_CHCFGR1_CKOUTSRC  /* ��������ouput clock is audio clock */

/*
 *  DFSDM����ͥ� InputMultiplexer���
 */
#define DFSDM_CHANNEL_EXTERNAL_INPUTS       0x00000000				/* ��������ץå� */
#define DFSDM_CHANNEL_INTERNAL_REGISTER     DFSDM_CHCFGR1_DATMPX_1	/* �����쥸���� */

/*
 *  DFSDM����ͥ� InputDataPacking���
 */
#define DFSDM_CHANNEL_STANDARD_MODE         0x00000000				/* Standard data packing mode */
#define DFSDM_CHANNEL_INTERLEAVED_MODE      DFSDM_CHCFGR1_DATPACK_0	/* Interleaved data packing mode */
#define DFSDM_CHANNEL_DUAL_MODE             DFSDM_CHCFGR1_DATPACK_1	/* Dual data packing mode */

/*
 *  DFSDM����ͥ� InputPins���
 */
#define DFSDM_CHANNEL_SAME_CHANNEL_PINS     0x00000000				/* ����ץåȥԥ�pins on same channel */
#define DFSDM_CHANNEL_FOLLOWING_CHANNEL_PINS DFSDM_CHCFGR1_CHINSEL	/* ����ץåȥԥ�following channel */

/*
 *  DFSDM����ͥ� SerialType���
 */
#define DFSDM_CHANNEL_SPI_RISING            0x00000000				/* SPI with rising edge */
#define DFSDM_CHANNEL_SPI_FALLING           DFSDM_CHCFGR1_SITP_0	/* SPI with falling edge */
#define DFSDM_CHANNEL_MANCHESTER_RISING     DFSDM_CHCFGR1_SITP_1	/* Manchester with rising edge */
#define DFSDM_CHANNEL_MANCHESTER_FALLING    DFSDM_CHCFGR1_SITP		/* Manchester with falling edge */

/*
 *  DFSDM����ͥ� SerialSpi clock selection���
 */
#define DFSDM_CHANNEL_SPI_CLOCK_EXTERNAL    0x00000000				/* ����SPI����å� */
#define DFSDM_CHANNEL_SPI_CLOCK_INTERNAL    DFSDM_CHCFGR1_SPICKSEL_0			/* ����SPI����å� */
#define DFSDM_CHANNEL_SPI_CLOCK_INTERNAL_DIV2_FALLING DFSDM_CHCFGR1_SPICKSEL_1	/* ����SPI����å� divided by 2, falling edge */
#define DFSDM_CHANNEL_SPI_CLOCK_INTERNAL_DIV2_RISING  DFSDM_CHCFGR1_SPICKSEL	/* ����SPI����å� divided by 2, rising edge */

/*
 *  DFSDM����ͥ� analog watchdog filter order���
 */
#define DFSDM_CHANNEL_FASTSINC_ORDER        0x00000000				/* FastSinc filter type */
#define DFSDM_CHANNEL_SINC1_ORDER           DFSDM_CHAWSCDR_AWFORD_0	/* Sinc 1 filter type */
#define DFSDM_CHANNEL_SINC2_ORDER           DFSDM_CHAWSCDR_AWFORD_1	/* Sinc 2 filter type */
#define DFSDM_CHANNEL_SINC3_ORDER           DFSDM_CHAWSCDR_AWFORD	/* Sinc 3 filter type */


/*
 *  DFSDM�ե��륿�� Regular/Injected conversion trigger���
 */
#define DFSDM_FILTER_SW_TRIGGER             0x00000000			/* Software trigger */
#define DFSDM_FILTER_SYNC_TRIGGER           0x00000001			/* Synchronous with DFSDM_FLT0 */
#define DFSDM_FILTER_EXT_TRIGGER            0x00000002			/* External trigger (only for injected conversion) */

/*
 *  DFSDM�ե��륿�� external trigger���(DFSDM filter 0, 1, 2 and 3)
 */
#define DFSDM_FILTER_EXT_TRIG_TIM1_TRGO     0x00000000
#define DFSDM_FILTER_EXT_TRIG_TIM1_TRGO2    DFSDM_FLTCR1_JEXTSEL_0
#define DFSDM_FILTER_EXT_TRIG_TIM8_TRGO     DFSDM_FLTCR1_JEXTSEL_1
#define DFSDM_FILTER_EXT_TRIG_TIM8_TRGO2    (DFSDM_FLTCR1_JEXTSEL_0 | DFSDM_FLTCR1_JEXTSEL_1)
#define DFSDM_FILTER_EXT_TRIG_TIM3_TRGO     DFSDM_FLTCR1_JEXTSEL_2
#define DFSDM_FILTER_EXT_TRIG_TIM4_TRGO     (DFSDM_FLTCR1_JEXTSEL_0 | DFSDM_FLTCR1_JEXTSEL_2)
#define DFSDM_FILTER_EXT_TRIG_TIM10_OC1     (DFSDM_FLTCR1_JEXTSEL_1 | DFSDM_FLTCR1_JEXTSEL_2)
#define DFSDM_FILTER_EXT_TRIG_TIM6_TRGO     (DFSDM_FLTCR1_JEXTSEL_0 | DFSDM_FLTCR1_JEXTSEL_1 | DFSDM_FLTCR1_JEXTSEL_2)
#define DFSDM_FILTER_EXT_TRIG_TIM7_TRGO     DFSDM_FLTCR1_JEXTSEL_3
#define DFSDM_FILTER_EXT_TRIG_EXTI11        (DFSDM_FLTCR1_JEXTSEL_3 | DFSDM_FLTCR1_JEXTSEL_4)
#define DFSDM_FILTER_EXT_TRIG_EXTI15        (DFSDM_FLTCR1_JEXTSEL_0 | DFSDM_FLTCR1_JEXTSEL_3 | DFSDM_FLTCR1_JEXTSEL_4)							/* For DFSDM filter 0, 1, 2 and 3 */
#define DFSDM_FILTER_EXT_TRIG_LPTIM1_OUT    (DFSDM_FLTCR1_JEXTSEL_1 | DFSDM_FLTCR1_JEXTSEL_3 | DFSDM_FLTCR1_JEXTSEL_4)

/*
 *  DFSDM�ե��륿�� external trigger edge���
 */
#define DFSDM_FILTER_EXT_TRIG_RISING_EDGE   DFSDM_FLTCR1_JEXTEN_0	/* ���� Ω�夬�ꥨ�å� */
#define DFSDM_FILTER_EXT_TRIG_FALLING_EDGE  DFSDM_FLTCR1_JEXTEN_1	/* ���� Ω���ꥨ�å� */
#define DFSDM_FILTER_EXT_TRIG_BOTH_EDGES    DFSDM_FLTCR1_JEXTEN		/* ���� ξ���å� */

/*
 *  DFSDM�ե��륿�� sinc order���
 */
#define DFSDM_FILTER_FASTSINC_ORDER         0x00000000							/* FastSinc filter type */
#define DFSDM_FILTER_SINC1_ORDER            DFSDM_FLTFCR_FORD_0					/* Sinc 1 filter type */
#define DFSDM_FILTER_SINC2_ORDER            DFSDM_FLTFCR_FORD_1					/* Sinc 2 filter type */
#define DFSDM_FILTER_SINC3_ORDER            (DFSDM_FLTFCR_FORD_0 | DFSDM_FLTFCR_FORD_1)	/* Sinc 3 filter type */
#define DFSDM_FILTER_SINC4_ORDER            DFSDM_FLTFCR_FORD_2					/* Sinc 4 filter type */
#define DFSDM_FILTER_SINC5_ORDER            (DFSDM_FLTFCR_FORD_0 | DFSDM_FLTFCR_FORD_2)	/* Sinc 5 filter type */

/*
 *  DFSDM�ե��륿�� AwdDataSource DataSource���
 */
#define DFSDM_FILTER_AWD_FILTER_DATA        0x00000000			/* �ǥ�����ե��륿���������� */
#define DFSDM_FILTER_AWD_CHANNEL_DATA       DFSDM_FLTCR1_AWFSEL	/* ���ʥ������å��ɥå�����ͥ뤫������ */

/*
 *  DFSDM����ͥ����
 */
#define DFSDM_CHANNEL_0                     0x00000001
#define DFSDM_CHANNEL_1                     0x00010002
#define DFSDM_CHANNEL_2                     0x00020004
#define DFSDM_CHANNEL_3                     0x00030008
#define DFSDM_CHANNEL_4                     0x00040010
#define DFSDM_CHANNEL_5                     0x00050020
#define DFSDM_CHANNEL_6                     0x00060040
#define DFSDM_CHANNEL_7                     0x00070080

/*
 *  DFSDM�ե��륿�� Continuous Mode���
 */
#define DFSDM_CONTINUOUS_CONV_OFF           0x00000000	/* �Ѵ����³���ʤ� */
#define DFSDM_CONTINUOUS_CONV_ON            0x00000001	/* �Ѵ����³���� */

/*
 *  DFSDM�ե��륿�� AwdThreshold���
 */
#define DFSDM_AWD_HIGH_THRESHOLD            0x00000000	/* ���ʥ������å��ɥå� �ϥ����쥷��ۡ���� */
#define DFSDM_AWD_LOW_THRESHOLD             0x00000001	/* ���ʥ������å��ɥå� �����쥷��ۡ���� */


extern void dfsdm_clockconfig(ID port, uint32_t AudioFreq);
extern ER dfsdm_channel_init(DFSDM_Channel_Handle_t *hdfsc);
extern ER dfsdm_channel_deinit(DFSDM_Channel_Handle_t *hdfsc);

extern ER dfsdm_channelCkabStart(DFSDM_Channel_Handle_t *hdfsc);
extern ER dfsdm_channelCkabStop(DFSDM_Channel_Handle_t *hdfsc);
extern ER dfsdm_channelScdStart(DFSDM_Channel_Handle_t *hdfsc, uint32_t Threshold, uint32_t BreakSignal);
extern ER dfsdm_channelScdStop(DFSDM_Channel_Handle_t *hdfsc);
extern int16_t dfsdm_channelGetAwdValue(DFSDM_Channel_Handle_t *hdfsc);
extern ER dfsdm_channelModifyOffset(DFSDM_Channel_Handle_t *hdfsc, int32_t Offset);


extern ER dfsdm_filter_init(DFSDM_Filter_Handle_t *hdfsf);
extern ER dfsdm_filter_deinit(DFSDM_Filter_Handle_t *hdfsf);
extern ER dfsdm_filter_config_reg(DFSDM_Filter_Handle_t *hdfsf, uint32_t Channel, uint32_t ContinuousMode);
extern ER dfsdm_filter_config_inj(DFSDM_Filter_Handle_t *hdfsf, uint32_t Channel);

extern ER dfsdm_filterRegularStart(DFSDM_Filter_Handle_t *hdfsf, int32_t *pData, uint32_t Length);
extern ER dfsdm_filterRegularMsbStart(DFSDM_Filter_Handle_t *hdfsf, int16_t *pData, uint32_t Length);
extern ER dfsdm_filterRegularStop(DFSDM_Filter_Handle_t *hdfsf);
extern ER dfsdm_filterInjectedStart(DFSDM_Filter_Handle_t *hdfsf, int32_t *pData, uint32_t Length);
extern ER dfsdm_filterInjectedMsbStart(DFSDM_Filter_Handle_t *hdfsf, int16_t *pData, uint32_t Length);
extern ER dfsdm_filterInjectedStop(DFSDM_Filter_Handle_t *hdfsf);
extern ER dfsdm_filterAwdStart(DFSDM_Filter_Handle_t *hdfsf, DFSDM_Filter_AwdParamTypeDef* awdParam);
extern ER dfsdm_filterAwdStop(DFSDM_Filter_Handle_t *hdfsf);
extern ER dfsdm_filterExdStart(DFSDM_Filter_Handle_t *hdfsf, uint32_t Channel);
extern ER dfsdm_filterExdStop(DFSDM_Filter_Handle_t *hdfsf);

extern int32_t  dfsdm_filterGetRegularValue(DFSDM_Filter_Handle_t *hdfsf, uint32_t* Channel);
extern int32_t  dfsdm_filterGetInjectedValue(DFSDM_Filter_Handle_t *hdfsf, uint32_t* Channel);
extern int32_t  dfsdm_filterGetExdMaxValue(DFSDM_Filter_Handle_t *hdfsf, uint32_t* Channel);
extern int32_t  dfsdm_filterGetExdMinValue(DFSDM_Filter_Handle_t *hdfsf, uint32_t* Channel);
extern uint32_t dfsdm_filterGetConvTimeValue(DFSDM_Filter_Handle_t *hdfsf);

extern void dfsdm_irqhandler(DFSDM_Filter_Handle_t *hdfsf);


#ifdef __cplusplus
}
#endif

#endif	/* _DFSDM_H_ */

