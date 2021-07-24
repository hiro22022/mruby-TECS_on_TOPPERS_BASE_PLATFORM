/*
 *  TOPPERS/ASP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Advanced Standard Profile Kernel
 * 
 *  Copyright (C) 2003-2015 by Ryosuke Takeuchi
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
 *  @(#) $Id: monitor.h,v 2.0 2015/05/29 17:43:21 roi Exp $
 */

#ifndef _MONITOR_H_
#define	_MONITOR_H_

#ifdef __cplusplus
extern "C" {
#endif

/* 
 *  ��˥�������ץ�ץ����Υإå��ե�����
 */
#include <kernel.h>
#include "syssvc/logtask.h"

/*
 *  ������ͥ���٤�����
 */
#define MONITOR_PRIORITY	4

/*
 *  �������åȤ˰�¸�����ǽ���Τ�����������
 */

#ifndef CONSOLE_PORTID
#ifdef LOGTASK_PORTID
#define	CONSOLE_PORTID		LOGTASK_PORTID	/* ʸ�����Ϥ��륷�ꥢ��ݡ���ID */
#else  /* LOGTASK_PORTID */
#define	CONSOLE_PORTID		1				/* ʸ�����Ϥ��륷�ꥢ��ݡ���ID */
#endif /* LOGTASK_PORTID */
#endif /* TASK_PORTID */

/*
 *  ��˥��Υ�����������
 */
#if defined(M16C)
#define	MONITOR_STACK_SIZE	512		/* �������Υ����å������� */
#elif defined(H8) && !defined(SUPPORT_VLOUME)
#define	MONITOR_STACK_SIZE	768		/* �������Υ����å������� */
#else
#define	MONITOR_STACK_SIZE	2046	/* �������Υ����å������� */
#endif
#define	MAX_COMMAND_LENGTH	80		/* ��˥��κ��祳�ޥ��Ĺ */
#define	NUM_LOG_DISP		3		/* ����ɽ�������ƥ�� */
#ifndef MONITOR_PORTID
#define MONITOR_PORTID      CONSOLE_PORTID
#endif

/*
 *  �С���������
 */
#define	TMONITOR_PRVER	0x1020		/* �����ͥ�ΥС�������ֹ� */

/*
 *  ����������Ƥ����
 */
#define	KEY_BS			(8)			/* �Хå����ڡ��� */
#define	KEY_DEL			(127)		/* ��� */
#define KEY_NL			(10)		/* ����1 */
#define	KEY_CR			(13)		/* ����2 */
#define	KEY_EXT			(1)			/* ��λ */

/*
 *  �ǡ������������
 */
#define	DATA_BYTE		1			/* �Х��ȥǡ����ʣ��Х��ȡ�*/
#define	DATA_HALF		2			/* �ϡ��եǡ����ʣ��Х��ȡ�*/
#define	DATA_WORD		4			/* ��ɥǡ����ʣ��Х��ȡ�*/

/*
 *  �ΰ�°�������
 */
#define	NONE_AREA		0			/* �ΰ�γ�����ƤΤʤ��ΰ� */
#define	PORT_AREA		1			/* �ϡ��ɥ������Υݡ����ΰ� */
#define	MEMORY_AREA		2			/* �����ΰ衡*/

#define	MREAD_ONLY		1			/* �ɤ߹������� */
#define	MWRITE_ONLY		2			/* �񤭹������� */
#define	MREAD_WRITE		(MREAD_ONLY+MWRITE_ONLY)

#ifndef TOPPERS_MACRO_ONLY

/*
 *  ���ޥ�ɥǥ��ѥå��Ѥι�¤�����
 */
typedef struct _COMMAND_INFO {
	const char   *command;			/* ���ޥ��ʸ */
	int_t        (*func)(int argc, char **argv);	/* �¹Դؿ� */
} COMMAND_INFO;

typedef struct _COMMAND_LINK COMMAND_LINK;
struct _COMMAND_LINK {
	COMMAND_LINK *pcnext;
	int          num_command;
	const char   *command;			/* �祳�ޥ��ʸ */
	int_t        (*func)(int argc, char **argv);	/* �¹Դؿ� */
	const char   *help;				/* �إ��ʸ */
    const COMMAND_INFO *pcinfo;
};

/*
 *  ������������
 */
#define putecho(a)      putchar(a)
#define printecho(a)    printf(a)

/*
 *  �ؿ��Υץ�ȥ��������
 */

extern bool_t  need_monitor(void);
extern void	   monitor(intptr_t exinf);
extern int     setup_command(COMMAND_LINK *pcmd);
extern bool_t  monitor_break(void);
extern bool_t  compare_word(const char *s, char *d, int_t mode);
extern char    getMemoryType(ulong_t address, int_t mode);
extern int_t   MemoryRead(ulong_t address, intptr_t p, int_t type);
extern int_t   MemoryWrite(ulong_t address, intptr_t p, int_t type);
extern ulong_t MonAlignAddress(ulong_t address);
extern int_t   MemoryRead(ulong_t address, intptr_t p, int_t type);
extern int_t   MemoryWrite(ulong_t address, intptr_t p, int_t type);
extern ulong_t get_exception_pc(void * p_excinf);
extern void    display_registers(ID tskid);
extern ulong_t display_assembler(ulong_t pc);

/*
 *  ���Υ���饯����Ƚ��
 */
Inline bool_t
test_next_char(char c)
{
	if(c == ' ' || c == '\t')
		return true;
	else
		return false;
}

/*
 *  ʸ���󤫤��������Ф�
 *  ����ͤ�FALSE�ʤ��ͤ�̤����Ǥ��뤳�Ȥ򼨤�
 */
Inline bool_t
get_value(char *s, ulong_t *v, int_t card)
{
	char c;
	int  no = 0;

	*v = 0;
	if(s[no] == 0)
		return false;
	else if(s[no] == ' ' || s[no] == '\t'
               || s[no] == ',' || s[no] == '.'){
		no++;
		return false;
	}
	else{
		while(s[no]){
			c = s[no];
			no++;
			if(c >= '0' && c <= '9')
				c -= '0';
			else if(c >= 'A' && c <= 'Z')
				c -= 'A' - 10;
			else if(c >= 'a' && c <= 'z')
				c -= 'a' - 10;
			else
				break;
			*v = *v * card + c;
		}
		return true;
	}
}

#endif /* TOPPERS_MACRO_ONLY */

#ifdef __cplusplus
}
#endif

#endif /* _MONITOR_H_ */

