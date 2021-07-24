/*
 *  TOPPERS/ASP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Advanced Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2004-2012 by Embedded and Real-Time Systems Laboratory
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
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
 *  $Id: adctest.c 2416 2012-09-07 08:06:20Z ertl-hiro $
 */

/* 
 *  ADC�ƥ��Ȥ�����
 *  EXEC_MODE=0  DMA CIRCULAR CONTINUOUS
 *  EXEC_MODE=1  DMA CIRCULAR CONTINUOUS FREE RUN
 *  EXEC_MODE=2  INT CONTINUOUS FREE RUN
 *  EXEC_MODE=3  INT NOT CONTINUOUS
 *  �¹Ԥ�����ˤϡ���������˥��ǡ�task 3��ư
 */

#include <kernel.h>
#include <t_syslog.h>
#include <t_stdlib.h>
#include <string.h>
#include <sil.h>
#include "syssvc/serial.h"
#include "syssvc/syslog.h"
#include "kernel_cfg.h"
#include <target_syssvc.h>
#include "device.h"
#include "adctest.h"

/*
 *  �����ӥ�������Υ��顼�Υ�����
 */
Inline void
svc_perror(const char *file, int_t line, const char *expr, ER ercd)
{
	if (ercd < 0) {
		t_perror(LOG_ERROR, file, line, expr, ercd);
	}
}

#define	SVC_PERROR(expr)	svc_perror(__FILE__, __LINE__, #expr, (expr))


/*
 *  SW1�����
 */
void sw_int(void)
{
	syslog_0(LOG_NOTICE, "## sw_int() ##");
}

#define NUM_ADCBUF   96		/* 1ms */
volatile uint16_t uhADCxConvertedValue[NUM_ADCBUF];


/*
 *  ����С������λ������Хå�
 */
static void ADC_ConvCpltCallback2(ADC_Handle_t* hadc)
{
//	syslog_1(LOG_NOTICE, "## ADC_ConvCpltCallback[%08x] ##", hadc);
  isig_sem(ADCDMA_SEM);
}

/*
 *  ADC���顼������Хå�
 */
static void ADC_ErrorCallback(ADC_Handle_t* hadc)
{
	syslog_1(LOG_EMERG, "## ADC_ErrorCallback(%08x) ##", hadc);
}

/*
 *  �ᥤ�󥿥���
 */
void main_task(intptr_t exinf)
{
	ADC_Init_t     aInit;
	ADC_ChannelConf_t sConfig;
	ADC_Handle_t *hadc;
	ER_UINT	ercd;
	SYSTIM tim1, tim2;
	int i;

	SVC_PERROR(syslog_msk_log(LOG_UPTO(LOG_INFO), LOG_UPTO(LOG_EMERG)));
	syslog(LOG_NOTICE, "Sample program starts (exinf = %d).", (int_t) exinf);

	/*
	 *  ���ꥢ��ݡ��Ȥν����
	 *
	 *  �����ƥ����������Ʊ�����ꥢ��ݡ��Ȥ�Ȥ����ʤɡ����ꥢ��
	 *  �ݡ��Ȥ������ץ�Ѥߤξ��ˤϤ�����E_OBJ���顼�ˤʤ뤬���پ��
	 *  �ʤ���
	 */
	ercd = serial_opn_por(TASK_PORTID);
	if (ercd < 0 && MERCD(ercd) != E_OBJ) {
		syslog(LOG_ERROR, "%s (%d) reported by `serial_opn_por'.",
									itron_strerror(ercd), SERCD(ercd));
	}
	SVC_PERROR(serial_ctl_por(TASK_PORTID,
							(IOCTL_CRLF | IOCTL_FCSND | IOCTL_FCRCV)));

	/*
	 *  ADC�ν����
	 */
	aInit.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
	aInit.Resolution = ADC_RESOLUTION_12B;
//	aInit.ScanConvMode = ADC_SCANMODE_DISABLE;
	aInit.ScanConvMode = ADC_SCANMODE_ENABLE;
#if EXEC_MODE < 3
	aInit.ContinuousConvMode = ADC_CONTINUOUS_ENABLE;
#else
	aInit.ContinuousConvMode = ADC_CONTINUOUS_DISABLE;
#endif
	aInit.DiscontinuousConvMode = ADC_DISCONTINUOUS_DISABLE;
	aInit.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	aInit.ExternalTrigConv = ADC_SOFTWARE_START;
	aInit.DataAlign = ADC_DATAALIGN_RIGHT;
#if EXEC_MODE > 1
	aInit.DMAContinuousRequests = ADC_DMACONTINUOUS_DISABLE;
#else
	aInit.DMAContinuousRequests = ADC_DMACONTINUOUS_ENABLE;
#endif
	aInit.EOCSelection = ADC_EOC_SEQ_DISABLE;

	aInit.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;
//	aInit.SamplingTime = ADC_SAMPLETIME_7CYCLES_5;
	aInit.LowPowerAutoPowerOff = ADC_LOWAUTOPOWEROFF_DISABLE;
	aInit.LowPowerAutoWait = ADC_LOWAUTOWAIT_DISABLE;
	aInit.Overrun = ADC_OVR_DATA_OVERWRITTEN;
	if((hadc = adc_init(ADC1_PORTID, &aInit)) == NULL){
		syslog_0(LOG_NOTICE, "## adc_init ERROR ##");
		slp_tsk();
	}
	hadc->xfercallback = ADC_ConvCpltCallback2;
	hadc->errorcallback = ADC_ErrorCallback;

	/*
	 *  ADC����ͥ�����
	 */
	sConfig.Channel = ADCx_CHANNEL;
	sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;
	sConfig.GpioBase = ADCx_CHANNEL_GPIO_PORT;
	sConfig.GpioPin  = ADCx_CHANNEL_PINNO;
	if((ercd = adc_setupchannel(hadc, &sConfig)) != E_OK){
		syslog_1(LOG_NOTICE, "## adc_setupchannel ERROR(%d) ##", ercd);
		slp_tsk();
	}

	/*
	 *  ����С�����󥹥�����#1
	 */
#if EXEC_MODE == 1
	if((ercd = adc_start_dma(hadc, (uint16_t *)uhADCxConvertedValue, NUM_ADCBUF)) != E_OK){
		syslog_1(LOG_NOTICE, "## adc_start_dma ERROR(%d) ##", ercd);
		slp_tsk();
	}
	memset((void *)uhADCxConvertedValue, 0, NUM_ADCBUF*2);
#elif EXEC_MODE == 2
	if((ercd = adc_start_int(hadc)) != E_OK){
		syslog_1(LOG_NOTICE, "## adc_start_int ERROR(%d) ##", ercd);
		slp_tsk();
	}
	memset((void *)uhADCxConvertedValue, 0, NUM_ADCBUF*2);
#endif
	for(i = 0 ; i < 1000 ; i++){
		get_tim(&tim1);
		/*
		 *  ����С�����󥹥�����#2
		 */
#if EXEC_MODE == 0
		memset((void *)uhADCxConvertedValue, 0, NUM_ADCBUF*2);
		if((ercd = adc_start_dma(hadc, (uint16_t *)uhADCxConvertedValue, NUM_ADCBUF)) != E_OK){
			syslog_1(LOG_NOTICE, "## adc_start_dma ERROR(%d) ##", ercd);
			slp_tsk();
		}
#elif EXEC_MODE == 3
		memset((void *)uhADCxConvertedValue, 0, NUM_ADCBUF*2);
		if((ercd = adc_start_int(hadc)) != E_OK){
			syslog_1(LOG_NOTICE, "## adc_start_int ERROR(%d) ##", ercd);
			slp_tsk();
		}
#endif
		while(hadc->status == ADC_STATUS_BUSY){
			twai_sem(ADCDMA_SEM, 10);
		}
		get_tim(&tim2);

		/*
		 *  ����С�����󥨥��#2
		 */
#if EXEC_MODE == 0
		ercd = adc_end_dma(hadc);
		if(ercd != E_OK){
			syslog_1(LOG_NOTICE, "## adc_end_dma ERROR(%d) ##", ercd);
			slp_tsk();
		}
#elif EXEC_MODE == 2
		uhADCxConvertedValue[0] = adc_getvalue(hadc);
#elif EXEC_MODE == 3
		uhADCxConvertedValue[0] = adc_getvalue(hadc);
		ercd = adc_end_int(hadc);
		if(ercd != E_OK){
			syslog_1(LOG_NOTICE, "## adc_end_int ERROR(%d) ##", ercd);
			slp_tsk();
		}
#endif
		syslog_5(LOG_NOTICE, "## valuet(%d) (%d)(%d)(%d)(%d) ##", tim2-tim1, uhADCxConvertedValue[0], uhADCxConvertedValue[1], uhADCxConvertedValue[2], uhADCxConvertedValue[3]);
		dly_tsk(100);
	}
	/*
	 *  ����С�����󥨥��#1
	 */
#if EXEC_MODE == 1
	ercd = adc_end_dma(hadc);
	if(ercd != E_OK){
		syslog_1(LOG_NOTICE, "## adc_end_dma ERROR(%d) ##", ercd);
		slp_tsk();
	}
#elif EXEC_MODE == 2
	ercd = adc_end_int(hadc);
	if(ercd != E_OK){
		syslog_1(LOG_NOTICE, "## adc_end_int ERROR(%d) ##", ercd);
		slp_tsk();
	}
#endif
	syslog_2(LOG_NOTICE, "## STOP[%08x] (%d) ##", &uhADCxConvertedValue, hadc->status);
	ercd = adc_deinit(hadc);
	syslog_1(LOG_NOTICE, "## adc_deinit(%d) ##", ercd);
	slp_tsk();
//	SVC_PERROR(ext_ker());
}
