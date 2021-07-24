/*
 *  TOPPERS/ASP Educative Program
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2003-2008 by Ryosuke Takeuchi
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
 *  @(#) $Id: sh2.c,v 1.4 2009/05/11 16:05:29 roi Exp $
 */

/* 
 *  TOPPERS/ASP�ѥ�������˥�CPU��¸�ץ���ࡥ
 *
 */

#include <itron.h>
#include <sil.h>
#include <stdio.h>
#include "kernel_impl.h"
#include "task.h"
#include "task_expansion.h"
#ifdef SUPPORT_ETHER
#include "netdevice.h"
#endif
#include "monitor.h"

extern void _kernel_break_wait();
#ifdef SUPPORT_ETHER
extern unsigned long vectors[];
#endif

#define set_align(a, b)	((UW)a & ~(b-1))

/*
 * �쥸�����ι�¤��
 */
typedef struct t_reg{
	uint32_t gbr;
	uint32_t macl;
	uint32_t mach;
	uint32_t r14;
	uint32_t r13;
	uint32_t r12;
	uint32_t r11;
	uint32_t r10;
	uint32_t r9;
	uint32_t r8;
	uint32_t pr;
	uint32_t r7;
	uint32_t r6;
	uint32_t r5;
	uint32_t r4;
	uint32_t r3;
	uint32_t r2;
	uint32_t r1;
	uint32_t r0;
	uint32_t pc;
	uint32_t sr;
}T_REG;

/*
 * ����Υޥåԥ������¤��
 */

typedef struct t_memdef{
	uint32_t mstart;
	uint32_t mend;
	uint8_t  mtype;
	uint8_t  mstate;
}T_MEMDEF;

/*
 * SH2�Υ���ޥåԥ�
 */

static T_MEMDEF const memdefine[] = {
#if defined(HSB7616IT)
	{0x00000000, 0x0001FFFF, MEMORY_AREA, MREAD_ONLY },
	{0x02000000, 0x021FFFFF, MEMORY_AREA, MREAD_ONLY },
	{0x04000000, 0x040FFFFF, MEMORY_AREA, MREAD_WRITE},
	{0xFFFFE000, 0xFFFFFFFF, PORT_AREA,   MREAD_WRITE}
#elif defined(MS72060SEP01)
	{0x00000000, 0x001FFFFF, MEMORY_AREA, MREAD_ONLY },
	{0x01000000, 0x01FFFFFF, MEMORY_AREA, MREAD_ONLY },
	{0x0C000000, 0x0FFFFFFF, MEMORY_AREA, MREAD_WRITE},
	{0x10000000, 0xFFF7FFFF, PORT_AREA,   MREAD_WRITE},
	{0xFFF80000, 0xFFF9FFFF, MEMORY_AREA, MREAD_WRITE},
	{0xFFFA0000, 0xFFFFFFFF, PORT_AREA,   MREAD_WRITE}
#elif defined(CRB_H3)
	{0x00000000, 0x0003FFFF, MEMORY_AREA, MREAD_ONLY },
	{0x00200000, 0x0021FFFF, MEMORY_AREA, MREAD_WRITE},
	{0x00220000, 0x0022FFFF, MEMORY_AREA, MREAD_ONLY },
	{0x00230000, 0x0023FFFF, PORT_AREA,   MREAD_WRITE},
	{0xFFF00000, 0xFFFCFFFF, PORT_AREA,   MREAD_WRITE},
	{0xFFFD0000, 0xFFFFFFFF, MEMORY_AREA, MREAD_WRITE}
#else
#error "No board type in SH2 groups."
#endif
};

static T_REG  sreg;

/******************************************************************************
 * �ϡ��ɥ������ݡ���°�����ȴؿ�
 ******************************************************************************/
/*
 *  ���ɥ쥹��������ΰ�°������Ф�
 *  mode=0:�ΰ�η�
 *  mode=1:�ɤ߼��񤭹���°��
 */

char
getMemoryType(ulong_t address, int_t mode)
{
	int_t count = sizeof(memdefine) / sizeof(T_MEMDEF);
	int_t i;

	for(i = 0 ; i < count ; i++){
		if(address >= memdefine[i].mstart && address <= memdefine[i].mend){
			if(mode == 0)
				return memdefine[i].mtype;
			else
				return memdefine[i].mstate;
		}
	}
	return NONE_AREA;
}

/*
 *  ���ɥ쥹���饢�饤���Υ��ɥ쥹����Ф�
 */

ulong_t
MonAlignAddress(ulong_t address)
{
	return address;
}

/******************************************************************************
 * ���ꥢ�������Ѵؿ�
 ******************************************************************************/
/*
 *  �����ΰ���Ф����ɤ߽Ф��ؿ�
 *  �ΰ�Υ����å���Ԥ������顼�ʤ饼����֤�
 */
int_t
MemoryRead(ulong_t address, intptr_t p, int_t type)
{
	int_t len;

	switch(getMemoryType(address, 0)){
	case PORT_AREA:
		if(type == 2){
			len = 2;
#ifdef SUPPORT_ETHER
            if(!NetDeviceRead((unsigned long)address, len, (void *)p))
#endif
			*((UH *)p) = sil_reh_mem((VP)address);
		}
		else if(type == 4){
			len = 4;
#ifdef SUPPORT_ETHER
            if(!NetDeviceRead((unsigned long)address, len, (void *)p))
#endif
			*((UW *)p) = sil_rew_mem((VP)address);
		}
		else{
#ifdef SUPPORT_ETHER
			len = 1;
            if(!NetDeviceRead((unsigned long)address, len, (void *)p))
				*((UB *)p) = sil_reb_mem((VP)address);
#else
			len = 1;
			*((UB *)p) = sil_reb_mem((VP)address);
#endif
		}
		break;
	case MEMORY_AREA:
		if(type == 2){
			len = 2;
			*((UH *)p) = *((UH *)set_align(address, len));
		}
		else if(type == 4){
			len = 4;
			*((UW *)p) = *((UW *)set_align(address, len));
		}
		else{
			len = 1;
			*((UB *)p) = *((UB *)address);
		}
		break;
	default:
		len = 0;
		break;
	}
	return len;
}

/*
 *  �����ΰ���Ф���񤭹��ߴؿ�
 *  �ΰ�Υ����å���Ԥ������顼�ʤ饼����֤�
 */
int_t
MemoryWrite(ulong_t address, intptr_t p, int_t type)
{
	int_t len;

	switch(getMemoryType(address, 0)){
	case PORT_AREA:
		if(type == 2){
			len = 2;
			address = set_align(address, len);
#ifdef SUPPORT_ETHER
            if(!NetDeviceWrite((unsigned long)address, len, (void *)p))
#endif
			sil_wrh_mem((VP)address, *((UH *)p));
		}
		else if(type == 4){
			len = 4;
			address = set_align(address, len);
#ifdef SUPPORT_ETHER
            if(!NetDeviceWrite((unsigned long)address, len, (void *)p))
#endif
			sil_wrw_mem((VP)address, *((UW *)p));
		}
		else{
#ifdef SUPPORT_ETHER
			len = 1;
            if(!NetDeviceWrite((unsigned long)address, len, (void *)p))
				sil_wrb_mem((VP)address, *((UB *)p));
#else
			len = 1;
			sil_wrb_mem((VP)address, *((UB *)p));
#endif
		}
		break;
	case MEMORY_AREA:
		if(getMemoryType(address, 1) == MREAD_ONLY){
			len = 0;
		}
		else if(type == 2){
			len = 2;
			*((UH *)address) = *((UH *)set_align(p, len));
		}
		else if(type == 4){
			len = 4;
			*((UW *)address) = *((UW *)set_align(p, len));
		}
		else{
			len = 1;
			*((UB *)address) = *((UB *)p);
		}
		break;
	default:
		len = 0;
		break;
	}
	return len;
}

/******************************************************************************
 * ��˥��Ѵؿ�
 ******************************************************************************/
/*
 *  �������ץ����ΰ������pc����Ф��ؿ�
 */
ulong_t
get_exception_pc(void * p_excinf)
{
	ulong_t pc;

	pc = *(ulong_t*)((char*)p_excinf-4);
	return pc;
}

/*
 *  �쥸�������Ƥ�ɽ��
 */
void
display_registers(ID tskid)
{
	ER     ercd;
	T_RTST rtst;

	ercd = ref_tst(tskid, &rtst);
	if(ercd == E_OK){
		if(rtst.tskpc == (FP)_kernel_break_wait){
			sreg = *((T_REG *)rtst.tsksp);
			printf(" PC =%08lx SP =%08x PR =%08x", (long)sreg.pc, (UW)((long)rtst.tsksp+sizeof(T_REG)), sreg.pr);
			printf(" SR =%08x MBH=%08x MBL=%08x\n", sreg.sr, sreg.mach, sreg.macl);
			printf(" R0 =%08x R1 =%08x R2 =%08x", sreg.r0, sreg.r1, sreg.r2);
			printf(" R3 =%08x R4 =%08x R5 =%08x\n", sreg.r3, sreg.r4, sreg.r5);
			printf(" R6 =%08x R7 =%08x R8 =%08x", sreg.r6, sreg.r7, sreg.r8);
			printf(" R9 =%08x R10=%08x R11=%08x\n", sreg.r9, sreg.r10, sreg.r11);
			printf(" R12=%08x R13=%08x R14=%08x\n", sreg.r12, sreg.r13, sreg.r14);
			printf("     %08lx    %02x %02x\n", (long)sreg.pc, *((UB*)sreg.pc), *((UB*)(sreg.pc+1)));
			return;
		}
		else if(rtst.tskstat == TTS_DMT){
			printf("  wait in activate_r() !!\n");
			return;
		}
	}
	printf("  wait in dispatch() !!\n");
}

/******************************************************************************
 * ���顼�����Ѵؿ�
 ******************************************************************************/
/*
 * ���쥮��顼�������ץ����
 */
static void
irregular_ext_handler(void *p_excinf, const char *s)
{
	uint32_t pc = get_exception_pc(p_excinf);

	if(runtsk){
		if(exc_sense_context(p_excinf)){
			syslog_3(LOG_EMERG, "Irregular Exception(%s) occured in not task Context pc=0x%x p_excinf=0x%x !", s, pc, p_excinf);
			kernel_exit();
		}
		else{
			syslog_4(LOG_ERROR, "Irregular Exception(%s) occured in tskid=%d pc=0x%x p_excinf=0x%x !", s, TSKID(runtsk), pc, p_excinf);
			isus_tsk(TSKID(runtsk));
		}
	}
	else{
		syslog_3(LOG_EMERG, "Irregular Exception(%s) occured in Idle pc=0x%x p_excinf=0x%x !", s, pc, p_excinf);
		kernel_exit();
	}
}

/*
 *  ��������̿��:General Illegal Instruction
 */
void
irregular_extgii_handler(void * p_excinf)
{
	irregular_ext_handler(p_excinf, "GII");
}

/*
 *  ����å�����̿��:Slot Illegal Instruction
 */
void
irregular_extsii_handler(void * p_excinf)
{
	irregular_ext_handler(p_excinf, "SII");
}

/*
 *  CPU���ɥ쥹���顼:CPU Address Error
 */
void
irregular_extcae_handler(void * p_excinf)
{
	irregular_ext_handler(p_excinf, "CAE");
}

/*
 *  DMA���ɥ쥹���顼:DMA Address Error
 */
void
irregular_extdae_handler(void * p_excinf)
{
	irregular_ext_handler(p_excinf, "DAE");
}

