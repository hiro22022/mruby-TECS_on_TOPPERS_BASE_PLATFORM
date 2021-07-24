/*
 *  TOPPERS/JSP/ASP/FMP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
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
 *  @(#) $Id: stddev.c,v 1.4 2017/05/27 18:34:04 roi Exp $
 */

/* 
 *  TOPPERS�ѥ�������˥�CPU��¸�ץ���ࡥ
 *
 */

#include <itron.h> 
#include <stdio.h>
#include <string.h>
#include "syssvc/serial.h"
#include "monitor.h"


#ifdef _SMALL_STDIO_
/******************************************************************************
 * �̾���ɸ�������Ϥ�����
 ******************************************************************************/

FILE _iob[4];
static ID   dummyid = CONSOLE_PORTID;
static ID * stdport = &dummyid;

/*
 *  ��˥��Σ�ʸ������ʸ
 */
static int
local_getc(FILE *st)
{
	char buf[2];

	serial_rea_dat(*stdport, buf, 1);
	return (int)buf[0];
}

/*
 *  ��˥���ʸ��������ʸ
 */
static int
local_gets(FILE *st, unsigned int len, char *s)
{
	return serial_rea_dat(*stdport, s, len);
}

/*
 *  ��˥��Σ�ʸ������ʸ
 */
static void
local_putc(FILE *st, int c)
{
	char buf[2];

	buf[0] = c;
	serial_wri_dat(*stdport, buf, 1);
	if(c == '\n'){
		buf[0] = '\r';
		serial_wri_dat(*stdport, buf, 1);
	}
}

/*
 *  ��˥���ʸ�������ʸ
 */
static int
local_puts(FILE *st, unsigned int len, char *s)
{
	return serial_wri_dat(*stdport, s, len);
}

/*
 *  ��˥��Υǡ����ե�å���ʸ
 */
static int
local_flush(FILE *st)
{
	return 0;
}

/*
 *  �������strlen
 */
static int
local_datacount(const char *s)
{
	int i;

	for(i = 0 ; *s != 0 ; i++)
		s++;
	return i;
}

/*
 *  ɸ����ϴؿ�
 */
int fputc(int c, FILE *st)
{
	if(st == NULL)
		st = stdout;
	st->_func_out(st, c);
	return 1;
}

/*
 *  ɸ��ʸ������ϴؿ�
 */
int fputs(const char *s, FILE *st)
{
	int status;

	if(st == NULL)
		st = stdout;
	status = st->_func_outs(st, local_datacount(s), (char *)s);
	st->_func_out(st, '\n');
	return status;
}

/*
 *  ɸ�����ϴؿ�
 */
int fgetc(FILE *st)
{
	if(st == NULL)
		st = stdin;
	return st->_func_in(st);
}

/*
 *  ɸ��ʸ�������ϴؿ�
 */
int fgets(char *buf, int size, FILE *st)
{
	if(st == NULL)
		st = stdin;
	return st->_func_ins(st, size, buf);
}

/*
 *  ɸ��ǡ����ɤ߽Ф��ؿ�
 */
size_t fread(void *buf, size_t size, size_t count, FILE *st)
{
	int result;
	if(st == NULL)
		st = stdin;
	result = st->_func_ins(st, size*count, buf);
	return result / size;
}

/*
 *  ɸ��ǡ����񤭹��ߴؿ�
 */
size_t fwrite(const void *buf, size_t size, size_t count, FILE *st)
{
	int result;
	if(st == NULL)
		st = stdout;
	result = st->_func_outs(st, size*count, (char *)buf);
	return result / size;
}

/*
 *  ɸ��ʸ������ϴؿ�
 */
int puts(const char *s)
{
	int status;

	status = stdout->_func_outs(stdout, local_datacount(s), (char *)s);
	stdout->_func_out(stdout, '\n');
	return status;
}

/*
 *  ��ʸ�����ϥץ����
 */
int putchar(int c)
{
	return fputc(c,stdout);
}

/*
 *  �Хåե��Υե�å���ؿ�
 */
int fflush(FILE *st)
{
	if(st == NULL)
		st = stdout;
	return st->_func_flush(st);
}

/*
 *  ɸ���������ѥ��ȥ꡼����Ф��ؿ�
 */
FILE *_get_stdio(int no)
{
	return &_iob[no];
}

/*
 *  ɸ�������ϥ��ȥ꡼�������ؿ�
 */
int _set_stdio(int no)
{
	if(no >= 0 && no < 3){
		_iob[no]._file       = -1;
		_iob[no]._func_in    = (int (*)(void*))local_getc;
		_iob[no]._func_ins   = (int (*)(void*, unsigned int, char *))local_gets;
		_iob[no]._func_out   = (void (*)(void*, int))local_putc;
		_iob[no]._func_outs  = (int  (*)(void*, unsigned int, char *))local_puts;
		_iob[no]._func_flush = local_flush;
		_iob[no]._dev        = 0;
		return no;
	}
	else
		return -1;
}

/*
 *  ɸ�������Ͻ�����ؿ�
 */
void _setup_stdio(signed int *pport)
{
	int i;

	stdport = pport;
	for(i = 0 ; i < 3 ; i++){
		_set_stdio(i);
	}
}

#endif /* _SMALL_STDIO_ */

