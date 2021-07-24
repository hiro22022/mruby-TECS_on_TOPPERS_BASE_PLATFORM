/*
 *  TOPPERS/ASP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Advanced Standard Profile Kernel
 * 
 *  Copyright (C) 2003-2012 by Ryosuke Takeuchi
 *                     GJ Business Division RICOH COMPANY,LTD. JAPAN
 * 
 *  �嵭����Ԥϡ�Free Software Foundation �ˤ�äƸ�ɽ����Ƥ��� 
 *  GNU General Public License �� Version 2 �˵��Ҥ���Ƥ����狼����
 *  ����(1)��(4)�ξ������������˸¤ꡤ�ܥ��եȥ��������ܥ��եȥ���
 *  ������Ѥ�����Τ�ޤࡥ�ʲ�Ʊ���ˤ���ѡ�ʣ�������ѡ������ۡʰʲ���
 *  ���ѤȸƤ֡ˤ��뤳�Ȥ�̵���ǵ������롥
 *  (1) �ܥ��եȥ������򥽡��������ɤη������Ѥ�����ˤϡ��嵭������
 *      ��ɽ�����������Ѿ�浪��Ӳ�����̵�ݾڵ��꤬�����Τޤޤη��ǥ���
 *      ����������˴ޤޤ�Ƥ��뤳�ȡ�
 *  (2) �ܥ��եȥ�����������Ѳ�ǽ�ʥХ��ʥꥳ���ɡʥ�������֥륪��
 *      �������ȥե������饤�֥��ʤɡˤη������Ѥ�����ˤϡ�����
 *      ��ȼ���ɥ�����ȡ����Ѽԥޥ˥奢��ʤɡˤˡ��嵭�����ɽ����
 *      �������Ѿ�浪��Ӳ�����̵�ݾڵ����Ǻܤ��뤳�ȡ�
 *  (3) �ܥ��եȥ�������������Բ�ǽ�ʥХ��ʥꥳ���ɤη��ޤ��ϵ������
 *      �߹�����������Ѥ�����ˤϡ����Τ����줫�ξ������������ȡ�
 *    (a) ���Ѥ�ȼ���ɥ�����ȡ����Ѽԥޥ˥奢��ʤɡˤˡ��嵭������
 *        ��ɽ�����������Ѿ�浪��Ӳ�����̵�ݾڵ����Ǻܤ��뤳�ȡ�
 *    (b) ���Ѥη��֤��̤�������ˡ�ˤ�äơ��嵭����Ԥ���𤹤�
 *        ���ȡ�
 *  (4) �ܥ��եȥ����������Ѥˤ��ľ��Ū�ޤ��ϴ���Ū�������뤤���ʤ�»
 *      ������⡤�嵭����Ԥ����դ��뤳�ȡ�
 * 
 *  �ܥ��եȥ������ϡ�̵�ݾڤ��󶡤���Ƥ����ΤǤ��롥�嵭����Ԥϡ�
 *  �ܥ��եȥ������˴ؤ��ơ�����Ŭ�Ѳ�ǽ����ޤ�ơ������ʤ��ݾڤ�Ԥ�
 *  �ʤ����ޤ����ܥ��եȥ����������Ѥˤ��ľ��Ū�ޤ��ϴ���Ū����������
 *  ���ʤ�»���˴ؤ��Ƥ⡤������Ǥ�����ʤ���
 * 
 *  @(#) $Id: task_expansion.c,v 1.3 2015/11/07 14:46:20 roi Exp $
 */

/*
 *	TOPPERS/ASP�ѥ�����������ĥ��ǽ
 */

#include "kernel_impl.h"
#include "check.h"
#include "task.h"
#include "task_expansion.h"
#include "kernel_cfg.h"
#include "target_timer.h"

/*
 *  ����ƥ������椫��ζ����Ԥ����֤ؤΰܹ�
 */
ER
isus_tsk(ID tskid)
{
	TCB	*tcb;
	UINT	tstat;
	ER	ercd;

	CHECK_INTCTX_UNL();
	CHECK_TSKID(tskid);
	tcb = get_tcb(tskid);

	i_lock_cpu();
	if (TSTAT_DORMANT(tstat = tcb->tstat)) {
		ercd = E_OBJ;
	}
	else if (TSTAT_RUNNABLE(tstat)) {
		/*
		 *  �¹ԤǤ�����֤��鶯���Ԥ����֤ؤ�����
		 */
		tcb->tstat = TS_SUSPENDED;
		if (make_non_runnable(tcb)) {
			reqflg = TRUE;
		}
		ercd = E_OK;
	}
	else if (TSTAT_SUSPENDED(tstat)) {
		ercd = E_QOVR;
	}
	else {
		/*
		 *  �Ԥ����֤�������Ԥ����֤ؤ�����
		 */
		tcb->tstat |= TS_SUSPENDED;
		ercd = E_OK;
	}
	i_unlock_cpu();

    error_exit:
	return(ercd);
}

/*
 *  ���������ֻ��ȡʴʰ��ǡ�
 */
ER
ref_tst(ID tskid, T_RTST *pk_rtst)
{
	TCB	*tcb;
	UB	tstat;
	ER	ercd;

	CHECK_TSKCTX_UNL();
	CHECK_TSKID_SELF(tskid);
	tcb = get_tcb_self(tskid);

	t_lock_cpu();
	pk_rtst->tskwait  = 0;
	pk_rtst->tskpri   = EXT_TSKPRI(tcb->priority);
#ifdef USE_TSKINICTXB
	pk_rtst->inistk   = (VP)tcb->p_tinib->tskinictxb.stk_bottom;
	pk_rtst->inistksz = tcb->p_tinib->tskinictxb.stksz;
#else
	pk_rtst->inistk   = (VP)tcb->p_tinib->stk;
	pk_rtst->inistksz = tcb->p_tinib->stksz;
#endif

	tstat = tcb->tstat;
	if (TSTAT_RUNNABLE(tstat)) {
		if (tcb == p_runtsk) {
			pk_rtst->tskstat = TTS_RUN;
		}
		else {
			pk_rtst->tskstat = TTS_RDY;
		}
	}
	else if (TSTAT_WAITING(tstat)) {
		if (TSTAT_SUSPENDED(tstat)) {
			pk_rtst->tskstat = TTS_WAS;
		}
		else {
			pk_rtst->tskstat = TTS_WAI;
		}
		if ((tstat & TS_WAIT_MASK) == TS_WAIT_DLY) {
			pk_rtst->tskwait = TTW_DLY;
		}
		else if ((tstat & TS_WAIT_MASK) == TS_WAIT_SLP) {
			pk_rtst->tskwait = TTW_SLP;
		}
		else if ((tstat & TS_WAIT_MASK) == TS_WAIT_RDTQ) {
			pk_rtst->tskwait = TTW_RDTQ;
		}
		else {
			pk_rtst->tskwait = TTW_OTHR;
		}
	}
	else if (TSTAT_SUSPENDED(tstat)) {
		pk_rtst->tskstat = TTS_SUS;
	}
	else {
		pk_rtst->tskstat = TTS_DMT;
	}
	if (pk_rtst->tskstat == TTS_DMT)
		pk_rtst->tskpc = (FP)tcb->p_tinib->task;
	else
		pk_rtst->tskpc = (FP)tcb->tskctxb.pc;
	pk_rtst->tsksp   = (VP)tcb->tskctxb.sp;
	ercd = E_OK;
	t_unlock_cpu();

    error_exit:
	return(ercd);
}

/*
 *  ���������֤Υ��󥰳�Ǽ�ΰ�
 */
static T_TLOG   tsk_log[MAX_TASK_LOG+1];
static uint32_t check_tskid;
static SYSTIM   check_time;
static SYSTIM   pervious_time;

/*
 *  �����ƥ���ּ��Ф��ؿ�
 *  �������Ǥ���ѤǤ������̲��Ǥ���褦��
 *  ����饤��ؿ��Ȥ��롥
 */
Inline SYSTIM get_systime(void)
{
	return next_time;
}

/*
 *  �������μ�¬����¬���Ѥλ���
 *  ���Ф��ؿ���CPU��å����֤ǸƤӽФ���
 */
Inline SYSTIM get_anatime(void)
{
	SYSTIM  time;
#if TIC_DENO != 1
	INT     subtime;
#endif /* TIC_DENO != 1 */
	CLOCK   clock1, clock2;
	BOOL    ireq;

	/*
	 * ���֤μ��Ф��ϥ���ƥ�������Ǥ�Ԥ���Τ�
	 * get_utm��Ʊ���ε��Ҥ򤳤��ˤ�����
	 */
#if TIC_DENO != 1
	subtime = (INT) next_subtime;
#endif /* TIC_DENO != 1 */
	clock1 = target_timer_get_current();
	ireq = target_timer_probe_int();
	clock2 = target_timer_get_current();

	time = get_systime() * ANA_STIC;
#if TIC_DENO != 1
	time += subtime * ANA_STIC / TIC_DENO;
#endif /* TIC_DENO != 1 */
	if (!(ireq) || clock1 > clock2) {
		time -= TIC_NUME * ANA_STIC / TIC_DENO;
	}
	time += clock1 * ANA_STIC / TIMER_CLOCK;
	return time;
}

/*
 *  �������¹��ΰ�����ؿ�
 *  ���δؿ���dispatch�γ��ϻ���CPU��å����֤ǸƤӽФ���
 *  STM32F746�Ϥ��δؿ���ʣ���ˤ���ȥϥ󥰥��å׻��֤����Ϥ������롥
 */
void log_dsp_enter(TCB *runtsk)
{
#if !defined(TOPPERS_ARM_M) || defined(TOPPERS_STM32F4_DISCOVERY)
	SYSTIM time = get_systime();
#endif	/* ROI DEBUG */
	T_TLOG  *t;

	if(check_tskid < MAX_TASK_LOG)
		t = &tsk_log[check_tskid];
	else
		t = &tsk_log[MAX_TASK_LOG];
#if !defined(TOPPERS_ARM_M) || defined(TOPPERS_STM32F4_DISCOVERY)
	t->runtimes += time - check_time;
	check_time = time;
#endif
	if(p_schedtsk == 0){
		check_tskid = 0;
#if defined(TOPPERS_ARM_M) && !defined(TOPPERS_STM32F4_DISCOVERY)
		t->runtimes += 2;
//		check_time  += 2;
#endif
	}
}

/*
 *  �������¹��ΰ�����ؿ�
 *  ���δؿ���dispatch�ν�λ������CPU��å����֤ǸƤӽФ���
 */
void log_dsp_leave(TCB *runtsk)
{
	SYSTIM time = get_anatime();
	T_TLOG  *t;

	if(check_tskid < MAX_TASK_LOG)
		t = &tsk_log[check_tskid];
	else
		t = &tsk_log[MAX_TASK_LOG];
	t->runtimes += time - check_time;

	if(runtsk){
		check_tskid = TSKID(runtsk);
		t = &tsk_log[check_tskid];
		t->runcount++;
	}
	else
		check_tskid = 0;
	check_time = time;
}

/*
 *  �����������ּ��Ф��ؿ�
 */
int_t get_tsklog(T_TPRM * pprm)
{
	INT no;
	INT num_item=0;

	get_tim(&pprm->currtime);
	t_lock_cpu();
	pprm->pervtime = pervious_time;
	pervious_time  = pprm->currtime;
	for(no = 0 ; no <= tmax_tskid && no <= MAX_TASK_LOG ; no++){
		if(no < NUM_LDSP){
			pprm->tlog[no] = tsk_log[no];
			num_item = no;
		}
		else{
			pprm->tlog[1].runcount += tsk_log[no].runcount;
			pprm->tlog[1].runtimes += tsk_log[no].runtimes;
			num_item = NUM_LDSP-1;
		}
		tsk_log[no].runcount = 0;
		tsk_log[no].runtimes = 0;
	}
	t_unlock_cpu();
	return num_item;
}


/*
 *  �ǥХ������ݡ��ȤΥ��������׵�Ƚ���ѥǡ����ΰ�
 */

static T_PCHK port_log[NUM_PCHK];

/*
 *  �ǥХ�������Ԥ����ɤ�����Ƚ��
 */
static uint_t check_device_log(ulong_t address, ulong_t size)
{
	int    i;
	T_PCHK *p;

	for(i = 0 ; i < NUM_PCHK ; i++){
		p = &port_log[i];
		if(p->portaddress >= address && p->portaddress < (address+size) && p->logtype)
			return (UINT)p->logtype;
	}
	return 0;
}

/*
 *  �ǥХ������ݡ��ȤΥ�Ƚ��ǡ����μ��Ф�
 */
T_PCHK *get_device_log(ulong_t no){
	if(no < NUM_PCHK)
		return &port_log[no];
	else
		return 0;
}

/*
 *  �ǥХ����ɤ߽Ф�ɽ��Ƚ��ȥ������ȴؿ�
 */
void
ana_rdv(ulong_t address, ulong_t data, int_t size)
{
	SYSTIM time = get_systime();
	ID     id;
	uint_t logtype = check_device_log(address, size);

	if(logtype == 0)
		return;
	if(sense_context() || !p_runtsk)
		syslog_3(logtype, "Device Read  time=%09d interrupt port=%08x data=0x%x", time, address, data);
	else{
		id = TSKID(p_runtsk);
		switch(size){
		case 4:
			syslog_4(logtype, "Device Read  time=%09d task=%04d port=%08x data=%08x", time, id, address, data);
			break;
		case 2:
			syslog_4(logtype, "Device Read  time=%09d task=%04d port=%08x data=%04x", time, id, address, data);
			break;
		default:
			syslog_4(logtype, "Device Read  time=%09d task=%04d port=%08x data=%02x", time, id, address, data);
			break;
		}
	}
}

/*
 *  �ǥХ����񤭹���ɽ��Ƚ��ȥ������ȴؿ�
 */
void
ana_wdv(ulong_t address, ulong_t data, int_t size)
{
	SYSTIM time = get_systime();
	ID     id;
	uint_t logtype = check_device_log(address, size);

	if(logtype == 0)
		return;
	if(sense_context() || !p_runtsk)
		syslog_3(logtype, "Device Write  time=%09d interrupt port=%08x data=0x%x", time, address, data);
	else{
		id = TSKID(p_runtsk);
		switch(size){
		case 4:
			syslog_4(logtype, "Device Write time=%09d task=%04d port=%08x data=%08x", time, id, address, data);
			break;
		case 2:
			syslog_4(logtype, "Device Write time=%09d task=%04d port=%08x data=%04x", time, id, address, data);
			break;
		default:
			syslog_4(logtype, "Device Write time=%09d task=%04d port=%08x data=%02x", time, id, address, data);
			break;
		}
	}
}

