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
 *  @(#) $Id: rtc.c 698 2017-12-05 18:41:54Z roi $
 */
/*
 *
 * RTC(L0XX)�ѥǥХ������ؿ��������
 *
 */
#include "kernel_impl.h"
#include <t_syslog.h>
#include <t_stdlib.h>
#include <sil.h>
#include <target_syssvc.h>
#include "kernel_cfg.h"
#include "device.h"
#include "rtc.h"

/*
 *  SIL�ؿ��Υޥ������
 */
#define sil_orw_mem(a, b)		sil_wrw_mem((a), sil_rew_mem(a) | (b))
#define sil_andw_mem(a, b)		sil_wrw_mem((a), sil_rew_mem(a) & ~(b))
#define sil_modw_mem(a, b, c)	sil_wrw_mem((a), (sil_rew_mem(a) & (~b)) | (c))

#define byte2bcd(value) ((((value)/10)<<4) | ((value) % 10))
#define bcd2byte(value) ((((value)>>4) * 10) + ((value) & 0xF))

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


#define RCC_RTCCLKSOURCE_LSE    RCC_CSR_RTCSEL_LSE
#define RCC_RTCCLKSOURCE_LSI    RCC_CSR_RTCSEL_LSI


#ifndef RTC_CLOCK_LSE
#define RCC_RTCCLKSOURCE_DEF    RCC_RTCCLKSOURCE_LSI
#else
#define RCC_RTCCLKSOURCE_DEF    RCC_RTCCLKSOURCE_LSE
#endif

#if RCC_RTCCLKSOURCE_DEF == RCC_RTCCLKSOURCE_LSI
#define RTC_ASYNCH_PREDIV       0x7C	/* LSI as RTC clock */
#define RTC_SYNCH_PREDIV        0x0127	/* LSI as RTC clock */
#elif RCC_RTCCLKSOURCE_DEF == RCC_RTCCLKSOURCE_LSE
#define RTC_ASYNCH_PREDIV       0x7F	/* LSE as RTC clock */
#define RTC_SYNCH_PREDIV        0x00FF	/* LSE as RTC clock */
#else
#error "No clok select for RTC."
#endif


#define RTC_TR_RESERVED_MASK    (RTC_TR_SU | RTC_TR_ST | RTC_TR_MNU | RTC_TR_MNT | \
								 RTC_TR_HU | RTC_TR_HT | RTC_TR_PM)
#define RTC_DR_RESERVED_MASK    (RTC_DR_DU | RTC_DR_DT  | RTC_DR_MU | RTC_DR_MT | \
								 RTC_DR_WDU | RTC_DR_YU | RTC_DR_YT)
#define RTC_INIT_MASK           ((uint32_t)0xFFFFFFFF)  
#define RTC_RSF_MASK            ((uint32_t)0xFFFFFF5F)

#define RTC_EXTI_LINE_WUPTIMER  ((uint32_t)EXTI_IMR_MR20)  /* External interrupt line 20 Connected to the RTC Wakeup event */

#define RTC_TIMEOUT_VALUE       (1000*1000)
#define LSI_TIMEOUT_VALUE       (100*1000)
#define HSE_TIMEOUT_VALUE       (100*1000)
#define RCC_DBP_TIMEOUT_VALUE   (100*1000)
#define RCC_LSE_TIMEOUT_VALUE   (5000*1000)

uint32_t rtcerrorcode;

static void (*rtcalarmA_callback)(void);	/* RTC AlamA callback */
#if defined(TOFF_RTC_ALRMBR)
static void (*rtcalarmB_callback)(void);	/* RTC AlamB callback */
#endif
static void (*rtcwakeup_callback)(void);	/* rtc wakeup callback */

/*
 *  RTC����ȥ�⡼������
 */
static ER
rtc_requestInitMode(void)
{
	uint32_t tick = 0;

	/*
	 * ���˥����⡼��Ƚ��
	 */
	if((sil_rew_mem((uint32_t *)(TADR_RTC_BASE+TOFF_RTC_ISR)) & RTC_ISR_INITF) == 0){
	    /*
		 *  ���˥����⡼������
		 */
		sil_wrw_mem((uint32_t *)(TADR_RTC_BASE+TOFF_RTC_ISR), RTC_INIT_MASK);
	    while((sil_rew_mem((uint32_t *)(TADR_RTC_BASE+TOFF_RTC_ISR)) & RTC_ISR_INITF) == 0){
			if(tick > RTC_TIMEOUT_VALUE){
				return E_TMOUT;
			}
		}
		sil_dly_nse(1000);
		tick++;
	}
	return E_OK;
}

/*
 *  RTC�ѽ����
 */
void
rtc_init(intptr_t exinf)
{
	uint32_t tick = 0;
	bool_t   pwrclkchanged = false;
	volatile uint32_t tmp;

	rtcerrorcode = 0;

	if((sil_rew_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_APB1ENR)) & RCC_APB1ENR_PWREN) == 0){
		sil_orw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_APB1ENR), RCC_APB1ENR_PWREN);
		pwrclkchanged = true;
	}

#if RCC_RTCCLKSOURCE_DEF == RCC_RTCCLKSOURCE_LSI
	/*
	 *  LSE DISABLE
	 */
	if((sil_rew_mem((uint32_t *)(TADR_PWR_BASE+TOFF_PWR_CR)) & PWR_CR_DBP) == 0){
		sil_orw_mem((uint32_t *)(TADR_PWR_BASE+TOFF_PWR_CR), PWR_CR_DBP);
		while((sil_rew_mem((uint32_t *)(TADR_PWR_BASE+TOFF_PWR_CR)) & PWR_CR_DBP) == 0){
			if(tick > RCC_DBP_TIMEOUT_VALUE){
				syslog_0(LOG_ERROR, "rtc_init dbp timeout!");
				rtcerrorcode |= RTC_ERROR_RTC;
				return;
			}
			tick++;
			sil_dly_nse(1000);
		}
	}

	sil_andw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CSR), RCC_CSR_LSEON);
	sil_andw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CSR), RCC_CSR_LSEBYP);
	tick = 0;
	while((sil_rew_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CSR)) & RCC_CSR_LSERDY) != 0){
		if(tick > RCC_LSE_TIMEOUT_VALUE){
			syslog_0(LOG_ERROR, "rtc_init LSE timeout!");
			rtcerrorcode |= RTC_ERROR_LSE;
			return;
		}
		tick++;
		sil_dly_nse(1000);
    }

	/*
	 *  LSI ENABLE
	 */
	sil_orw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CSR), RCC_CSR_LSION);

	tick = 0;
	while((sil_rew_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CSR)) & RCC_CSR_LSIRDY) == 0){
		if(tick > RCC_LSE_TIMEOUT_VALUE){
			syslog_0(LOG_ERROR, "rtc_init LSI timeout!");
			rtcerrorcode |= RTC_ERROR_LSI;
			return;
		}
		tick++;
		sil_dly_nse(1000);
	}

#elif RCC_RTCCLKSOURCE_DEF == RCC_RTCCLKSOURCE_LSE
	/*
	 *  LSI DISABLE
	 */
	sil_andw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CSR), RCC_CSR_LSION);

	tick = 0;
	while((sil_rew_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CSR)) & RCC_CSR_LSIRDY) != 0){
		if(tick > RCC_LSE_TIMEOUT_VALUE){
			syslog_0(LOG_ERROR, "rtc_init LSI timeout!");
			rtcerrorcode |= RTC_ERROR_LSI;
			return;
		}
		tick++;
		sil_dly_nse(1000);
	}

	/*
	 *  LSE ENABLE
	 */
	if((sil_rew_mem((uint32_t *)(TADR_PWR_BASE+TOFF_PWR_CR)) & PWR_CR_DBP) == 0){
		sil_orw_mem((uint32_t *)(TADR_PWR_BASE+TOFF_PWR_CR), PWR_CR_DBP);
		while((sil_rew_mem((uint32_t *)(TADR_PWR_BASE+TOFF_PWR_CR)) & PWR_CR_DBP) == 0){
			if(tick > RCC_DBP_TIMEOUT_VALUE){
				syslog_0(LOG_ERROR, "rtc_init dbp timeout!");
				rtcerrorcode |= RTC_ERROR_RTC;
				return;
			}
			tick++;
			sil_dly_nse(1000);
		}
	}

	sil_orw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CSR), RCC_CSR_LSEON);
	tick = 0;
	while((sil_rew_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CSR)) & RCC_CSR_LSERDY) == 0){
		if(tick > RCC_LSE_TIMEOUT_VALUE){
			syslog_0(LOG_ERROR, "rtc_init LSE timeout!");
			rtcerrorcode |= RTC_ERROR_LSE;
			return;
		}
		tick++;
		sil_dly_nse(1000);
    }
#endif

    /*
	 *  ����å�������������
	 */
    tmp = sil_rew_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CSR)) & RCC_CSR_RTCSEL;
    if(tmp != 0 && tmp != RCC_RTCCLKSOURCE_DEF){
		/*
		 *  ����å����������㤦��硢RTC�ꥻ�å�
		 */
		tmp = sil_rew_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CSR)) & ~RCC_CSR_RTCSEL;
		sil_orw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CSR), RCC_CSR_RTCRST);
		sil_andw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CSR), RCC_CSR_RTCRST);
		/*
		 * ����å��������ʤ�������
		 */
		sil_wrw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CSR), tmp);
	}
	sil_modw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CSR), RCC_CSR_RTCSEL, RCC_RTCCLKSOURCE_DEF);

	if(pwrclkchanged)
		sil_andw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_APB1ENR), RCC_APB1ENR_PWREN);

	/*
	 *  RTC�Υ���å�����
	 */
	sil_orw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CSR), RCC_CSR_RTCEN);


	/* �ץ�ƥ�������� */
	sil_wrw_mem((uint32_t *)(TADR_RTC_BASE+TOFF_RTC_WPR), 0xCA);
	sil_wrw_mem((uint32_t *)(TADR_RTC_BASE+TOFF_RTC_WPR), 0x53);

	/*
	 *  RTC�������
	 */
	if(rtc_requestInitMode() != E_OK){
		rtcerrorcode |= RTC_ERROR_RTC;
	    /* �ץ�ƥ���������� */
		sil_wrw_mem((uint32_t *)(TADR_RTC_BASE+TOFF_RTC_WPR), 0xFF);
		return;
	}
	else{
		sil_andw_mem((uint32_t *)(TADR_RTC_BASE+TOFF_RTC_CR), (RTC_CR_FMT | RTC_CR_OSEL | RTC_CR_POL));

		sil_wrw_mem((uint32_t *)(TADR_RTC_BASE+TOFF_RTC_PRER), RTC_SYNCH_PREDIV);
		sil_orw_mem((uint32_t *)(TADR_RTC_BASE+TOFF_RTC_PRER), (RTC_ASYNCH_PREDIV << 16));

		sil_andw_mem((uint32_t *)(TADR_RTC_BASE+TOFF_RTC_ISR), RTC_ISR_INIT);

	    /* �ץ�ƥ���������� */
		sil_wrw_mem((uint32_t *)(TADR_RTC_BASE+TOFF_RTC_WPR), 0xFF);
		return;
	}
}


/*
 *  RTC�λ�������ؿ�
 *
 *  ����������PONIX��tm��¤�Τ���Ѥ���
 *  PONIX�Υ��󥯥롼�ɤ��ʤ������θ����Ʊ����ܤ�tm2��ɥ饤�ФȤ���������롣
 */
ER
rtc_set_time(struct tm2 *pt)
{
	uint32_t timetmp = 0;
	uint32_t datetmp = 0;

	if(pt == NULL)
		return E_PAR;
	if(rtcerrorcode != 0)
		return E_SYS;

	SVC_PERROR(wai_sem(RTCSEM));

	datetmp = (((uint32_t)byte2bcd(pt->tm_year - 30) << 16) |
				((uint32_t)byte2bcd(pt->tm_mon) << 8) |
				((uint32_t)byte2bcd(pt->tm_mday)) |
				((uint32_t)pt->tm_wday << 13));
    timetmp = (uint32_t)(((uint32_t)byte2bcd(pt->tm_hour) << 16) |
				((uint32_t)byte2bcd(pt->tm_min) << 8) |
				((uint32_t)byte2bcd(pt->tm_sec)));

	/* �ץ�ƥ�������� */
	sil_wrw_mem((uint32_t *)(TADR_RTC_BASE+TOFF_RTC_WPR), 0xCA);
	sil_wrw_mem((uint32_t *)(TADR_RTC_BASE+TOFF_RTC_WPR), 0x53);

	/*
	 *  ������⡼������
	 */
	if(rtc_requestInitMode() != E_OK){
		/* �ץ�ƥ���������� */
		sil_wrw_mem((uint32_t *)(TADR_RTC_BASE+TOFF_RTC_WPR), 0xFF);
		SVC_PERROR(sig_sem(RTCSEM));
		return E_TMOUT;
	}
	else{
		/*
		 *  ���ա���������
		 */
		sil_wrw_mem((uint32_t *)(TADR_RTC_BASE+TOFF_RTC_DR), (datetmp & RTC_DR_RESERVED_MASK));
		sil_wrw_mem((uint32_t *)(TADR_RTC_BASE+TOFF_RTC_TR), (timetmp & RTC_TR_RESERVED_MASK));

		/*  ������⡼�ɽ�λ */
		sil_andw_mem((uint32_t *)(TADR_RTC_BASE+TOFF_RTC_CR), RTC_CR_BCK);
		sil_andw_mem((uint32_t *)(TADR_RTC_BASE+TOFF_RTC_ISR), RTC_ISR_INIT);

		/*
		 *  Ʊ������
		 */
		if((sil_rew_mem((uint32_t *)(TADR_RTC_BASE+TOFF_RTC_CR)) & RTC_CR_BYPSHAD) == 0){
			uint32_t tick = 0;

			sil_andw_mem((uint32_t *)(TADR_RTC_BASE+TOFF_RTC_ISR), RTC_RSF_MASK);
    		while((sil_rew_mem((uint32_t *)(TADR_RTC_BASE+TOFF_RTC_ISR)) & RTC_ISR_RSF) == 0){
				if(tick > (RTC_TIMEOUT_VALUE/1000)){
					SVC_PERROR(sig_sem(RTCSEM));
					return E_TMOUT;
				}
				dly_tsk(1);
				tick++;
			}
		}

		/* �ץ�ƥ���������� */
		sil_wrw_mem((uint32_t *)(TADR_RTC_BASE+TOFF_RTC_WPR), 0xFF);
	}
	SVC_PERROR(sig_sem(RTCSEM));
	return E_OK;
}

/*
 *  RTC�λ�����Ф��ؿ�
 *
 *  ����������PONIX��tm��¤�Τ���Ѥ���
 *  PONIX�Υ��󥯥롼�ɤ��ʤ������θ����Ʊ����ܤ�tm2��ɥ饤�ФȤ���������롣
 */
ER
rtc_get_time(struct tm2 *pt)
{
	uint32_t timetmp = 0;
	uint32_t datetmp = 0;

	if(pt == NULL)
		return E_PAR;
	if(rtcerrorcode != 0)
		return E_SYS;

	SVC_PERROR(wai_sem(RTCSEM));
	/*
	 *  �������
	 */
	timetmp = sil_rew_mem((uint32_t *)(TADR_RTC_BASE+TOFF_RTC_TR)) & RTC_TR_RESERVED_MASK;
	pt->tm_hour = (uint8_t)bcd2byte((uint8_t)((timetmp & (RTC_TR_HT | RTC_TR_HU)) >> 16));
	pt->tm_min = (uint8_t)bcd2byte((uint8_t)((timetmp & (RTC_TR_MNT | RTC_TR_MNU)) >>8));
	pt->tm_sec = (uint8_t)bcd2byte((uint8_t)(timetmp & (RTC_TR_ST | RTC_TR_SU)));

	/*
	 *  ���ռ���
	 */
	datetmp = sil_rew_mem((uint32_t *)(TADR_RTC_BASE+TOFF_RTC_DR)) & RTC_DR_RESERVED_MASK;
	pt->tm_year = (uint8_t)bcd2byte((uint8_t)((datetmp & (RTC_DR_YT | RTC_DR_YU)) >> 16)) + 30;
	pt->tm_mon = (uint8_t)bcd2byte((uint8_t)((datetmp & (RTC_DR_MT | RTC_DR_MU)) >> 8));
	pt->tm_mday = (uint8_t)bcd2byte((uint8_t)(datetmp & (RTC_DR_DT | RTC_DR_DU)));
	pt->tm_wday = (uint8_t)((datetmp & (RTC_DR_WDU)) >> 13); 

	SVC_PERROR(sig_sem(RTCSEM));
	return E_OK;
}

/*
 *  RTC���顼������
 *  parameter1 : parm: Pointer to Alarm structure
 *  parameter2 : ptm: Pointer to struct tm2
 *  return ER������
 */
ER
rtc_setalarm(RTC_Alarm_t *parm, struct tm2 *ptm)
{
	uint32_t tick = 0;
	uint32_t tmparm = 0, subsecond = 0;
	uint32_t tmp, day;

	if(parm == NULL || ptm == NULL)
		return E_PAR;
	if(rtcerrorcode != 0)
		return E_SYS;

	if(parm->dayselect == ALARMDAYSEL_DATE)
		day = ptm->tm_mday;
	else
		day = ptm->tm_wday;

	/*
	 *  ALARM-AB����쥸�����ͤ����
	 */
	SVC_PERROR(wai_sem(RTCSEM));
    tmparm = ((byte2bcd(ptm->tm_hour) << 16) | (byte2bcd(ptm->tm_min) << 8) |
              (byte2bcd(ptm->tm_sec)) | (byte2bcd(day) << 24) |
              ((uint32_t)parm->dayselect) | (parm->alarmmask)); 
	subsecond = (uint32_t)(parm->subsecond | parm->subsecondmask);

	/* �ץ�ƥ�������� */
	sil_wrw_mem((uint32_t *)(TADR_RTC_BASE+TOFF_RTC_WPR), 0xCA);
	sil_wrw_mem((uint32_t *)(TADR_RTC_BASE+TOFF_RTC_WPR), 0x53);

	if(parm->alarm == RTC_ALARM_A){
		/*
		 *  ALARM-A���ꡢ�쥸���������
		 */
		sil_andw_mem((uint32_t *)(TADR_RTC_BASE+TOFF_RTC_CR), RTC_CR_ALRAE);
		tmp = sil_rew_mem((uint32_t *)(TADR_RTC_BASE+TOFF_RTC_ISR));
		sil_wrw_mem((uint32_t *)(TADR_RTC_BASE+TOFF_RTC_ISR), (~(RTC_ISR_ALRAF | RTC_ISR_INIT) | tmp));
		while((sil_rew_mem((uint32_t *)(TADR_RTC_BASE+TOFF_RTC_ISR)) & RTC_ISR_ALRAWF) == 0){
			if(tick > (RTC_TIMEOUT_VALUE/1000)){
			    /* �ץ�ƥ���������� */
				sil_wrw_mem((uint32_t *)(TADR_RTC_BASE+TOFF_RTC_WPR), 0xFF);
				SVC_PERROR(sig_sem(RTCSEM));
				return E_TMOUT;
			}
			dly_tsk(1);
			tick++;
		}

		/*
		 *  ALARM-A����
		 */
		sil_wrw_mem((uint32_t *)(TADR_RTC_BASE+TOFF_RTC_ALRMAR), tmparm);
		sil_wrw_mem((uint32_t *)(TADR_RTC_BASE+TOFF_RTC_ALRMASSR), subsecond);
		sil_orw_mem((uint32_t *)(TADR_RTC_BASE+TOFF_RTC_CR), RTC_CR_ALRAE);
		sil_orw_mem((uint32_t *)(TADR_RTC_BASE+TOFF_RTC_CR), RTC_CR_ALRAIE);
		rtcalarmA_callback = parm->callback;
	}
#if defined(TOFF_RTC_ALRMBR)
	else{
		/*
		 *  ALARM-B���ꡢ�쥸���������
		 */
		sil_andw_mem((uint32_t *)(TADR_RTC_BASE+TOFF_RTC_CR), RTC_CR_ALRBE);
		tmp = sil_rew_mem((uint32_t *)(TADR_RTC_BASE+TOFF_RTC_ISR));
		sil_wrw_mem((uint32_t *)(TADR_RTC_BASE+TOFF_RTC_ISR), (~(RTC_ISR_ALRBF | RTC_ISR_INIT) | tmp));
		while((sil_rew_mem((uint32_t *)(TADR_RTC_BASE+TOFF_RTC_ISR)) & RTC_ISR_ALRBWF) == 0){
			if(tick > (RTC_TIMEOUT_VALUE/1000)){
			    /* �ץ�ƥ���������� */
				sil_wrw_mem((uint32_t *)(TADR_RTC_BASE+TOFF_RTC_WPR), 0xFF);
				SVC_PERROR(sig_sem(RTCSEM));
				return E_TMOUT;
			}
			dly_tsk(1);
			tick++;
		}

		/*
		 *  ALARM-B����
		 */
		sil_wrw_mem((uint32_t *)(TADR_RTC_BASE+TOFF_RTC_ALRMBR), tmparm);
		sil_wrw_mem((uint32_t *)(TADR_RTC_BASE+TOFF_RTC_ALRMBSSR), subsecond);
		sil_orw_mem((uint32_t *)(TADR_RTC_BASE+TOFF_RTC_CR), RTC_CR_ALRBE);
		sil_orw_mem((uint32_t *)(TADR_RTC_BASE+TOFF_RTC_CR), RTC_CR_ALRBIE);
		rtcalarmB_callback = parm->callback;
	}
#endif

	/*
	 * RTC ALARM��EXTI����
	 */
	sil_orw_mem((uint32_t *)(TADR_EXTI_BASE+TOFF_EXTI_IMR), EXTI_IMR_MR17);
	sil_orw_mem((uint32_t *)(TADR_EXTI_BASE+TOFF_EXTI_RTSR), EXTI_RTSR_TR17);

    /* �ץ�ƥ���������� */
	sil_wrw_mem((uint32_t *)(TADR_RTC_BASE+TOFF_RTC_WPR), 0xFF);
	SVC_PERROR(sig_sem(RTCSEM));
	return E_OK;
}

/*
 *  RTC���顼�����
 *  parameter1 : Alarm: ���顼������
 *  return ER������
 */
ER
rtc_stopalarm(uint32_t Alarm)
{
	uint32_t tick = 0;

	if(rtcerrorcode != 0)
		return E_SYS;
	SVC_PERROR(wai_sem(RTCSEM));
	/* �ץ�ƥ�������� */
	sil_wrw_mem((uint32_t *)(TADR_RTC_BASE+TOFF_RTC_WPR), 0xCA);
	sil_wrw_mem((uint32_t *)(TADR_RTC_BASE+TOFF_RTC_WPR), 0x53);

	if(Alarm == RTC_ALARM_A){
		/*
		 *  ALARM-A����ߥ��͡��֥���
		 */
		sil_andw_mem((uint32_t *)(TADR_RTC_BASE+TOFF_RTC_CR), RTC_CR_ALRAE);
		sil_andw_mem((uint32_t *)(TADR_RTC_BASE+TOFF_RTC_CR), RTC_CR_ALRAIE);
		while((sil_rew_mem((uint32_t *)(TADR_RTC_BASE+TOFF_RTC_ISR)) & RTC_ISR_ALRAWF) == 0){
			if(tick > (RTC_TIMEOUT_VALUE/1000)){
			    /* �ץ�ƥ���������� */
				sil_wrw_mem((uint32_t *)(TADR_RTC_BASE+TOFF_RTC_WPR), 0xFF);
				SVC_PERROR(sig_sem(RTCSEM));
				return E_TMOUT;
			}
			dly_tsk(1);
			tick++;
		}
		rtcalarmA_callback = NULL;
	}
#if defined(TOFF_RTC_ALRMBR)
	else{
		/*
		 *  ALARM-B����ߥ��͡��֥���
		 */
		sil_andw_mem((uint32_t *)(TADR_RTC_BASE+TOFF_RTC_CR), RTC_CR_ALRBE);
		sil_andw_mem((uint32_t *)(TADR_RTC_BASE+TOFF_RTC_CR), RTC_CR_ALRBIE);
		while((sil_rew_mem((uint32_t *)(TADR_RTC_BASE+TOFF_RTC_ISR)) & RTC_ISR_ALRBWF) == 0){
			if( tick > RTC_TIMEOUT_VALUE){
			    /* �ץ�ƥ���������� */
				sil_wrw_mem((uint32_t *)(TADR_RTC_BASE+TOFF_RTC_WPR), 0xFF);
				SVC_PERROR(sig_sem(RTCSEM));
				return E_TMOUT;
			}
			dly_tsk(1);
			tick++;
		}
		rtcalarmB_callback = NULL;
	}
#endif
    /* �ץ�ƥ���������� */
	sil_wrw_mem((uint32_t *)(TADR_RTC_BASE+TOFF_RTC_WPR), 0xFF);
	SVC_PERROR(sig_sem(RTCSEM));
	return E_OK; 
}

/*
 *  RTC���顼�������ͼ���
 *  parameter1 : parm: Pointer to Alarm structure
 *  parameter2 : ptm: Pointer to struct tm2
 *  return ER������
 */
ER
rtc_getalarm(RTC_Alarm_t *parm, struct tm2 *ptm, uint32_t Alarm)
{
	uint32_t tmparm = 0;
	uint32_t subsecond = 0;

	if(parm == NULL || ptm == NULL)
		return E_PAR;
	if(rtcerrorcode != 0)
		return E_SYS;

	SVC_PERROR(wai_sem(RTCSEM));
	if(Alarm == RTC_ALARM_A){
		/*
		  ALARM-A �쥸��������
		 */
		parm->alarm = RTC_ALARM_A;
		tmparm = sil_rew_mem((uint32_t *)(TADR_RTC_BASE+TOFF_RTC_ALRMAR));
		subsecond = sil_rew_mem((uint32_t *)(TADR_RTC_BASE+TOFF_RTC_ALRMASSR)) & RTC_ALRMASSR_SS;
	}
#if defined(TOFF_RTC_ALRMBR)
	else{
		/*
		  ALARM-B �쥸��������
		 */
		parm->alarm = RTC_ALARM_B;
		tmparm = sil_rew_mem((uint32_t *)(TADR_RTC_BASE+TOFF_RTC_ALRMBR));
		subsecond = sil_rew_mem((uint32_t *)(TADR_RTC_BASE+TOFF_RTC_ALRMBSSR)) & RTC_ALRMBSSR_SS;
	}
#endif

	/*
	 *  �쥸��������ѥ�᡼�����Ѵ�
	 */
	ptm->tm_hour = bcd2byte((uint8_t)((tmparm & (RTC_ALRMAR_HT | RTC_ALRMAR_HU)) >> 16));
	ptm->tm_min  = bcd2byte((uint8_t)((tmparm & (RTC_ALRMAR_MNT | RTC_ALRMAR_MNU)) >> 8));
	ptm->tm_sec = bcd2byte((uint8_t)(tmparm & (RTC_ALRMAR_ST | RTC_ALRMAR_SU)));
	parm->subsecond = (uint32_t) subsecond;
	parm->dayselect = (uint32_t)(tmparm & RTC_ALRMAR_WDSEL);
	if(parm->dayselect == ALARMDAYSEL_DATE)
		ptm->tm_mday = bcd2byte((uint8_t)((tmparm & (RTC_ALRMAR_DT | RTC_ALRMAR_DU)) >> 24));
	else
		ptm->tm_wday = bcd2byte((uint8_t)((tmparm & (RTC_ALRMAR_DT | RTC_ALRMAR_DU)) >> 24));

	parm->alarmmask = (uint32_t)(tmparm & ALARMMASK_ALL);
	SVC_PERROR(sig_sem(RTCSEM));
	return E_OK;
}

/*
 *  RTC WAKEUP�����޽����
 *  parameter1 : parm: Pointer to wakeup structure
 *  parameter2 : func: callback function
 *  return ER������
 */
ER
rtc_wakeup_init(RTC_Wakeup_t *parm, void (*func)(void))
{
	uint32_t prer, isr;

	if(parm == NULL)
		return E_PAR;
	parm->wakeuptimerPrescaler = (4 - (sil_rew_mem((uint32_t *)(TADR_RTC_BASE+TOFF_RTC_CR)) & RTC_CR_WUCKSEL));
	prer = sil_rew_mem((uint32_t *)(TADR_RTC_BASE+TOFF_RTC_PRER));
	parm->asynchPrescaler = ((prer >> 16) & 0x7f) + 1;
	parm->synchPrescaler  = prer & 0xffff;

	/* �ץ�ƥ�������� */
	sil_wrw_mem((uint32_t *)(TADR_RTC_BASE+TOFF_RTC_WPR), 0xCA);
	sil_wrw_mem((uint32_t *)(TADR_RTC_BASE+TOFF_RTC_WPR), 0x53);

	sil_andw_mem((uint32_t *)(TADR_RTC_BASE+TOFF_RTC_CR), RTC_CR_WUTE);
	isr  = sil_rew_mem((uint32_t *)(TADR_RTC_BASE+TOFF_RTC_ISR)) & RTC_ISR_INIT;
	isr |= ~(RTC_ISR_WUTF | RTC_ISR_INIT);
	sil_wrw_mem((uint32_t *)(TADR_RTC_BASE+TOFF_RTC_ISR), isr);
	sil_wrw_mem((uint32_t *)(TADR_EXTI_BASE+TOFF_EXTI_PR), RTC_EXTI_LINE_WUPTIMER);
	sil_orw_mem((uint32_t *)(TADR_RTC_BASE+TOFF_RTC_CR), RTC_CR_WUTIE);
	/*
	 *  EXTI WAKEUPLINE����
	 */
	sil_orw_mem((uint32_t *)(TADR_EXTI_BASE+TOFF_EXTI_RTSR), RTC_EXTI_LINE_WUPTIMER);
	sil_orw_mem((uint32_t *)(TADR_EXTI_BASE+TOFF_EXTI_IMR), RTC_EXTI_LINE_WUPTIMER);

    /* �ץ�ƥ���������� */
	sil_wrw_mem((uint32_t *)(TADR_RTC_BASE+TOFF_RTC_WPR), 0xFF);

	rtcwakeup_callback = func;
	return E_OK;
}

/*
 *  RTC WAKEUP��������
 *  parameter1 : parm: WAKEUP����
 *  return ER������
 */
ER
rtc_setup_wakeuptime(uint32_t time)
{
	uint32_t timeout = LSI_TIMEOUT_VALUE;
	/*
	 *  WAUPTIMER�������
	 */
	if((sil_rew_mem((uint32_t *)(TADR_RTC_BASE+TOFF_RTC_CR)) & RTC_CR_WUTE) != 0){
		while((sil_rew_mem((uint32_t *)(TADR_RTC_BASE+TOFF_RTC_ISR)) & RTC_ISR_WUTWF) == 0){
			timeout--;
			if(timeout == 0)
				return E_TMOUT;
			sil_dly_nse(1000);
		}
	}
	/* �ץ�ƥ�������� */
	sil_wrw_mem((uint32_t *)(TADR_RTC_BASE+TOFF_RTC_WPR), 0xCA);
	sil_wrw_mem((uint32_t *)(TADR_RTC_BASE+TOFF_RTC_WPR), 0x53);

	sil_andw_mem((uint32_t *)(TADR_RTC_BASE+TOFF_RTC_CR), RTC_CR_WUTE);

	if(time != 0 && time < 0xFFFF){
		sil_wrw_mem((uint32_t *)(TADR_EXTI_BASE+TOFF_EXTI_PR), RTC_EXTI_LINE_WUPTIMER);
		sil_wrw_mem((uint32_t *)(TADR_RTC_BASE+TOFF_RTC_WUTR), time);
		sil_orw_mem((uint32_t *)(TADR_RTC_BASE+TOFF_RTC_CR), RTC_CR_WUTE);
	}
    /* �ץ�ƥ���������� */
	sil_wrw_mem((uint32_t *)(TADR_RTC_BASE+TOFF_RTC_WPR), 0xFF);
	return E_OK;
}

/*
 *  SSR�μ��Ф�
 */
uint32_t
rtc_get_ssr(void)
{
	return sil_rew_mem((uint32_t *)(TADR_RTC_BASE+TOFF_RTC_SSR));
}


/*
 *  RTC����ߥϥ�ɥ�
 */
void rtc_handler(void)
{
	uint32_t tmp, isr;

	if((sil_rew_mem((uint32_t *)(TADR_RTC_BASE+TOFF_RTC_ISR)) & RTC_CR_ALRAE) != 0){
		/*
		 * ALARM-A����߳�ǧ
		 */
		if((sil_rew_mem((uint32_t *)(TADR_RTC_BASE+TOFF_RTC_CR)) & RTC_CR_ALRAIE) != 0){
			/*
			 *  ALARM-A������Хå�
			 */
			if(rtcalarmA_callback != NULL)
				rtcalarmA_callback();

			/* �ץ�ƥ�������� */
			sil_wrw_mem((uint32_t *)(TADR_RTC_BASE+TOFF_RTC_WPR), 0xCA);
			sil_wrw_mem((uint32_t *)(TADR_RTC_BASE+TOFF_RTC_WPR), 0x53);
			/*
			 *  ALARM-A����ߥ��ꥢ
			 */
			tmp = sil_rew_mem((uint32_t *)(TADR_RTC_BASE+TOFF_RTC_ISR)) & RTC_ISR_INIT;
			sil_wrw_mem((uint32_t *)(TADR_RTC_BASE+TOFF_RTC_ISR), (~(RTC_ISR_ALRAF | RTC_ISR_INIT) | tmp));
		    /* �ץ�ƥ���������� */
			sil_wrw_mem((uint32_t *)(TADR_RTC_BASE+TOFF_RTC_WPR), 0xFF);
		}

		/*
		 *  EXTI RTC�ڥ�ǥ��󥰥��ꥢ
		 */
		sil_wrw_mem((uint32_t *)(TADR_EXTI_BASE+TOFF_EXTI_PR), EXTI_PR_PR17);
	}
#if defined (RTC_CR_ALRBE)
	if((sil_rew_mem((uint32_t *)(TADR_RTC_BASE+TOFF_RTC_ISR)) & RTC_CR_ALRBE) != 0){
		/*
		 * ALARM-B����߳�ǧ
		 */
		if((sil_rew_mem((uint32_t *)(TADR_RTC_BASE+TOFF_RTC_CR)) & RTC_CR_ALRBIE) != 0){
			/*
			 *  ALARM-B������Хå�
			 */
			if(rtcalarmB_callback != NULL)
				rtcalarmB_callback();

			/* �ץ�ƥ�������� */
			sil_wrw_mem((uint32_t *)(TADR_RTC_BASE+TOFF_RTC_WPR), 0xCA);
			sil_wrw_mem((uint32_t *)(TADR_RTC_BASE+TOFF_RTC_WPR), 0x53);
			/*
			 *  ALARM-B����ߥ��ꥢ
			 */
			tmp = sil_rew_mem((uint32_t *)(TADR_RTC_BASE+TOFF_RTC_ISR)) & RTC_ISR_INIT;
			sil_wrw_mem((uint32_t *)(TADR_RTC_BASE+TOFF_RTC_ISR), (~(RTC_ISR_ALRBF | RTC_ISR_INIT) | tmp));
		    /* �ץ�ƥ���������� */
			sil_wrw_mem((uint32_t *)(TADR_RTC_BASE+TOFF_RTC_WPR), 0xFF);
		}

		/*
		 *  EXTI RTC�ڥ�ǥ��󥰥��ꥢ
		 */
		sil_wrw_mem((uint32_t *)(TADR_EXTI_BASE+TOFF_EXTI_PR), EXTI_PR_PR17);
	}
#endif

	if((sil_rew_mem((uint32_t *)(TADR_RTC_BASE+TOFF_RTC_ISR)) & RTC_ISR_WUTF) != 0){
		/* �ץ�ƥ�������� */
		sil_wrw_mem((uint32_t *)(TADR_RTC_BASE+TOFF_RTC_WPR), 0xCA);
		sil_wrw_mem((uint32_t *)(TADR_RTC_BASE+TOFF_RTC_WPR), 0x53);
		/*
		 *  WAKEUP TIMER���
		 */
		sil_andw_mem((uint32_t *)(TADR_RTC_BASE+TOFF_RTC_CR), RTC_CR_WUTE);

		/*
 		 *  WUTF flag���ꥢ
		 */
		isr  = sil_rew_mem((uint32_t *)(TADR_RTC_BASE+TOFF_RTC_ISR)) & RTC_ISR_INIT;
		isr |= ~(RTC_ISR_WUTF | RTC_ISR_INIT);
		sil_wrw_mem((uint32_t *)(TADR_RTC_BASE+TOFF_RTC_ISR), isr);

	    /* �ץ�ƥ���������� */
		sil_wrw_mem((uint32_t *)(TADR_RTC_BASE+TOFF_RTC_WPR), 0xFF);

		/*
		 *  EXTI�γ���ߥ��ꥢ
		 */
		sil_wrw_mem((uint32_t *)(TADR_EXTI_BASE+TOFF_EXTI_PR), RTC_EXTI_LINE_WUPTIMER);

		/*
		 *  ������Хå��ؿ��¹�
		 */
		if(rtcwakeup_callback != NULL)
			rtcwakeup_callback();
	}
}

