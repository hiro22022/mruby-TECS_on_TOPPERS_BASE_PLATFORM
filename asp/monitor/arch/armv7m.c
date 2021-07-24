/*
 *  TOPPERS/ASP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Advanced Standard Profile Kernel
 * 
 *  Copyright (C) 2003-2016 by Ryosuke Takeuchi
 *                     GJ Business Division RICOH COMPANY,LTD. JAPAN
 *  Copyright (C) 2017-2018 by TOPPERS PROJECT Educational Working Group.
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
 *  @(#) $Id: armv7.c,v 1.4 2018/05/20 18:50:54 roi Exp $
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
#include "monitor.h"

extern void dispatch_r(void);

#define set_align(a, b)	((ulong_t)a & ~(b-1))

/*
 * �쥸�����ι�¤��
 */
typedef struct t_reg{
	uint32_t r4;
	uint32_t r5;
	uint32_t r6;
	uint32_t r7;
	uint32_t r8;
	uint32_t r9;
	uint32_t r10;
	uint32_t r11;
	uint32_t pc;
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
 * ARMv7�Υ���ޥåԥ�
 */

static T_MEMDEF const memdefine[] = {
#if defined(TOPPERS_CQ_FRK_FM3)
	{0x00000000, 0x000FFFFF,           MEMORY_AREA, MREAD_ONLY },
	{0x1FFF0000, 0x201EFFFF,           MEMORY_AREA, MREAD_WRITE},
	{0x40000000, 0x400FFFFF,           PORT_AREA,   MREAD_WRITE},
#elif defined(TOPPERS_STM32F091_NUCLEO64)	/* ARMV6m */
	{0x08000000, 0x0803FFFF,           MEMORY_AREA, MREAD_ONLY },
	{0x20000000, 0x20007FFF,           MEMORY_AREA, MREAD_WRITE},
	{0x40000000, 0x50060FFF,           PORT_AREA,   MREAD_WRITE},
	{0xA0000000, 0xBFFFFFFF,           PORT_AREA,   MREAD_WRITE},
#elif defined(TOPPERS_STM32L073_NUCLEO64) || defined(TOPPERS_STM32LORA_DISCOVERY)	/* ARMV6m */
	{0x08000000, 0x0802FFFF,           MEMORY_AREA, MREAD_ONLY },
	{0x20000000, 0x20004FFF,           MEMORY_AREA, MREAD_WRITE},
	{0x40000000, 0x50060FFF,           PORT_AREA,   MREAD_WRITE},
	{0xA0000000, 0xBFFFFFFF,           PORT_AREA,   MREAD_WRITE},
#elif defined(TOPPERS_STM32L476_NUCLEO64) || defined(TOPPERS_STM32L476_DISCOVERY)
	{0x08000000, 0x080FFFFF,           MEMORY_AREA, MREAD_ONLY },
	{0x10000000, 0x10007FFF,           MEMORY_AREA, MREAD_WRITE},
	{0x20000000, 0x20017FFF,           MEMORY_AREA, MREAD_WRITE},
	{0x40000000, 0x50060FFF,           PORT_AREA,   MREAD_WRITE},
	{0x90000000, 0xBFFFFFFF,           PORT_AREA,   MREAD_WRITE},
#elif defined(TOPPERS_STM32F4_DISCOVERY) || defined(TOPPERS_STM32_E407)
	{0x08000000, 0x080FFFFF,           MEMORY_AREA, MREAD_ONLY },
	{0x20000000, 0x2001FFFF,           MEMORY_AREA, MREAD_WRITE},
	{0x40000000, 0x50060FFF,           PORT_AREA,   MREAD_WRITE},
	{0xA0000000, 0xBFFFFFFF,           PORT_AREA,   MREAD_WRITE},
#elif defined(TOPPERS_STM32F401_NUCLEO)
	{0x08000000, 0x0807FFFF,           MEMORY_AREA, MREAD_ONLY },
	{0x20000000, 0x20017FFF,           MEMORY_AREA, MREAD_WRITE},
	{0x40000000, 0x50060FFF,           PORT_AREA,   MREAD_WRITE},
	{0xA0000000, 0xBFFFFFFF,           PORT_AREA,   MREAD_WRITE},
#elif defined(TOPPERS_STM32F446_NUCLEO64) || defined(TOPPERS_STM32F446_NUCLEO144)
	{0x08000000, 0x0807FFFF,           MEMORY_AREA, MREAD_ONLY },
	{0x20000000, 0x2001FFFF,           MEMORY_AREA, MREAD_WRITE},
	{0x40000000, 0x50060FFF,           PORT_AREA,   MREAD_WRITE},
	{0xA0000000, 0xBFFFFFFF,           PORT_AREA,   MREAD_WRITE},
#elif defined(TOPPERS_STM32F429_BOARD)
	{0x08000000, 0x080FFFFF,           MEMORY_AREA, MREAD_ONLY },
	{0x20000000, 0x2002FFFF,           MEMORY_AREA, MREAD_WRITE},
	{0x40000000, 0x50060FFF,           PORT_AREA,   MREAD_WRITE},
	{0xA0000000, 0xBFFFFFFF,           PORT_AREA,   MREAD_WRITE},
#elif defined(TOPPERS_STM32F723_DISCOVERY)
	{0x00000000, 0x0003FFFF,           MEMORY_AREA, MREAD_WRITE},
	{0x08000000, 0x0807FFFF,           MEMORY_AREA, MREAD_ONLY },
	{0x20000000, 0x2003FFFF,           MEMORY_AREA, MREAD_WRITE},
	{0x40000000, 0x50060FFF,           PORT_AREA,   MREAD_WRITE},
	{0x90000000, 0xBFFFFFFF,           PORT_AREA,   MREAD_WRITE},
	{0xC0000000, 0xC07FFFFF,           MEMORY_AREA, MREAD_WRITE},
#elif defined(TOPPERS_STM32F7_DISCOVERY) ||defined(TOPPERS_STM32F746_NUCLEO144)
	{0x00000000, 0x0003FFFF,           MEMORY_AREA, MREAD_WRITE},
	{0x08000000, 0x080FFFFF,           MEMORY_AREA, MREAD_ONLY },
	{0x20000000, 0x2004FFFF,           MEMORY_AREA, MREAD_WRITE},
	{0x40000000, 0x50060FFF,           PORT_AREA,   MREAD_WRITE},
	{0x90000000, 0xBFFFFFFF,           PORT_AREA,   MREAD_WRITE},
	{0xC0000000, 0xC07FFFFF,           MEMORY_AREA, MREAD_WRITE},
#elif defined(TOPPERS_STM32F769_DISCOVERY) || defined(TOPPERS_STM32F767_NUCLEO144)
	{0x00000000, 0x0003FFFF,           MEMORY_AREA, MREAD_WRITE},
	{0x08000000, 0x081FFFFF,           MEMORY_AREA, MREAD_ONLY },
	{0x20000000, 0x2007FFFF,           MEMORY_AREA, MREAD_WRITE},
	{0x40000000, 0x50060FFF,           PORT_AREA,   MREAD_WRITE},
	{0x90000000, 0xBFFFFFFF,           PORT_AREA,   MREAD_WRITE},
	{0xC0000000, 0xC07FFFFF,           MEMORY_AREA, MREAD_WRITE},
#else
#error "No support board type in ARMV7-M groups."
#endif
	{0xE0000000, 0xFFFFFFFF,           PORT_AREA,   MREAD_WRITE}
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
			*((UH *)p) = sil_reh_mem((VP)address);
		}
		else if(type == 4){
			len = 4;
			*((UW *)p) = sil_rew_mem((VP)address);
		}
		else{
			len = 1;
			*((UB *)p) = sil_reb_mem((VP)address);
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
			sil_wrh_mem((VP)address, *((UH *)p));
		}
		else if(type == 4){
			len = 4;
			address = set_align(address, len);
			sil_wrw_mem((VP)address, *((UW *)p));
		}
		else{
			len = 1;
			sil_wrb_mem((VP)address, *((UB *)p));
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
 *  �쥸�������Ƥ�ɽ��
 */
void
display_registers(ID tskid)
{
	ER     ercd;
	T_RTST rtst;

	ercd = ref_tst(tskid, &rtst);
	if(ercd == E_OK){
		if(rtst.tskpc == (FP)dispatch_r){
			sreg = *((T_REG *)rtst.tsksp);
			printf(" PC  =%08lx SP =%08x\n", (long)sreg.pc, (UW)((long)rtst.tsksp+sizeof(T_REG)));
			printf(" R4  =%08x R5 =%08x R6  =%08x R7  =%08x\n", sreg.r4, sreg.r5, sreg.r6, sreg.r7);
			printf(" R8  =%08x R9 =%08x R10 =%08x R11 =%08x\n", sreg.r8, sreg.r9, sreg.r10, sreg.r11);
			printf("     %08lx    %04x\n", (long)sreg.pc, *((UH*)(sreg.pc & ~1)));
			return;
		}
		else if(rtst.tskstat == TTS_DMT){
			printf("  wait in activate_r() !!\n");
			return;
		}
	}
	printf("  wait in dispatch() !!\n");
}

