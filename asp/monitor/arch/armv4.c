/*
 *  TOPPERS/ASP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Advanced Standard Profile Kernel
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
 *  @(#) $Id: armv4.c,v 1.1 2009/08/09 09:44:29 roi Exp $
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

extern void _kernel_break_wait();

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
	uint32_t spsr;
	uint32_t r0;
	uint32_t r1;
	uint32_t r2;
	uint32_t r3;
	uint32_t ip;
	uint32_t lr;
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
 * ARM4�Υ���ޥåԥ�
 */

static T_MEMDEF const memdefine[] = {
#if defined(__AT91SAM7S128__)
	{0x00000000, 0x0001FFFF,           MEMORY_AREA, MREAD_ONLY },
	{RAM_START,  RAM_START+RAM_SIZE-1, MEMORY_AREA, MREAD_WRITE},
	{0xFFFF0000, 0xFFFFFFFF,           PORT_AREA,   MREAD_WRITE}
#elif defined(MINDSTORMSNXT)
	{0x00000000, RAM_SIZE-1,           MEMORY_AREA, MREAD_ONLY },
	{0x001C0000, 0x001FFFFF,           MEMORY_AREA, MREAD_ONLY },
	{RAM_START,  RAM_START+RAM_SIZE-1, MEMORY_AREA, MREAD_WRITE},
	{0xFFFF0000, 0xFFFFFFFF,           PORT_AREA,   MREAD_WRITE}
#elif defined(__LPC2388__)
	{0x00000000, 0x0007FFFF,           MEMORY_AREA, MREAD_ONLY },
	{0x3FFFC000, 0x3FFFFFFF,           PORT_AREA,   MREAD_WRITE},
	{RAM_START,  RAM_START+RAM_SIZE-1, MEMORY_AREA, MREAD_WRITE},
	{0x80000000, 0x8000FFFF,           MEMORY_AREA, MREAD_ONLY },
	{0x81000000, 0x8100FFFF,           MEMORY_AREA, MREAD_ONLY },
	{0xE0000000, 0xE008FFFF,           PORT_AREA,   MREAD_WRITE},
	{0xE01FC000, 0xE01FFFFF,           PORT_AREA,   MREAD_WRITE},
	{0xFFEF0000, 0xFFFFFFFF,           PORT_AREA,   MREAD_WRITE}
#elif (__TARGET_ARCH_ARM == 5)
	{0x01BC0000, 0x01BC0FFF,           MEMORY_AREA, MREAD_WRITE},
	{0x01BC1000, 0x01BC17FF,           PORT_AREA,   MREAD_WRITE},
	{0x01C00000, 0x01C07FFF,           PORT_AREA,   MREAD_WRITE},
	{0x01C08000, 0x01C087FF,           PORT_AREA,   MREAD_WRITE},
	{0x01C10000, 0x01C11FFF,           PORT_AREA,   MREAD_WRITE},
	{0x01C14000, 0x01C14FFF,           PORT_AREA,   MREAD_WRITE},
	{0x01C20000, 0x01C23FFF,           PORT_AREA,   MREAD_WRITE},
	{0x01C40000, 0x01C42FFF,           PORT_AREA,   MREAD_WRITE},
	{0x01D00000, 0x01D02FFF,           PORT_AREA,   MREAD_WRITE},
	{0x01D0C000, 0x01D0DFFF,           PORT_AREA,   MREAD_WRITE},
	{0x01D10000, 0x01D11FFF,           PORT_AREA,   MREAD_WRITE},
	{0x01E00000, 0x01E0FFFF,           PORT_AREA,   MREAD_WRITE},
	{0x01E10000, 0x01E10FFF,           PORT_AREA,   MREAD_WRITE},
	{0x01E13000, 0x01E1BFFF,           PORT_AREA,   MREAD_WRITE},
	{0x01E20000, 0x01E28FFF,           PORT_AREA,   MREAD_WRITE},
	{0x01E2C000, 0x01E2CFFF,           PORT_AREA,   MREAD_WRITE},
	{0x01E30000, 0x01E383FF,           PORT_AREA,   MREAD_WRITE},
	{0x01F00000, 0x01F03FFF,           PORT_AREA,   MREAD_WRITE},
	{0x01F06000, 0x01F08FFF,           PORT_AREA,   MREAD_WRITE},
	{0x01F0C000, 0x01F0EFFF,           PORT_AREA,   MREAD_WRITE},
	{0x01F10000, 0x01F11FFF,           PORT_AREA,   MREAD_WRITE},
	{0x01C40000, 0x01C42FFF,           PORT_AREA,   MREAD_WRITE},
	{0xC0000000, 0xC3FFFFFF,           MEMORY_AREA, MREAD_WRITE},
	{0xFFFD0000, 0xFFFDFFFF,           MEMORY_AREA, MREAD_ONLY },
	{0xFFFEE000, 0xFFFEFFFF,           PORT_AREA,   MREAD_WRITE},
	{0xFFFF0000, 0xFFFF1FFF,           MEMORY_AREA, MREAD_WRITE}
#else
#error "No board type in ARMV4 groups."
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
		if(rtst.tskpc == (FP)_kernel_break_wait){
			sreg = *((T_REG *)rtst.tsksp);
			printf(" PC =%08lx SP =%08x IR =%08x", (long)sreg.pc, (UW)((long)rtst.tsksp+sizeof(T_REG)), sreg.lr);
			printf(" IP =%08x SR=%08x\n", sreg.ip, sreg.spsr);
			printf(" R0 =%08x R1 =%08x R2 =%08x", sreg.r0, sreg.r1, sreg.r2);
			printf(" R3 =%08x R4 =%08x R5 =%08x\n", sreg.r3, sreg.r4, sreg.r5);
			printf(" R6 =%08x R7 =%08x R8 =%08x", sreg.r6, sreg.r7, sreg.r8);
			printf(" R9 =%08x R10=%08x R11=%08x\n", sreg.r9, sreg.r10, sreg.r11);
			printf("     %08lx    %08x\n", (long)sreg.pc, *((UW*)((sreg.pc+3) & ~3)));
			return;
		}
		else if(rtst.tskstat == TTS_DMT){
			printf("  wait in activate_r() !!\n");
			return;
		}
	}
	printf("  wait in dispatch() !!\n");
}

