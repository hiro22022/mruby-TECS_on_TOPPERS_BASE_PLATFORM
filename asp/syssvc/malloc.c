/*
 *  TOPPERS/ASP/FMP Kernel
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
 *  @(#) $Id: malloc.c 698 2017-06-02 08:48:16Z roi $
 */
/*
 * 
 *  ���ꥢ���������ǽ
 *
 */
#include <kernel.h>
#include <t_syslog.h>
#include <stdlib.h>
#include <string.h>
#include "kernel_cfg.h"
#include "tlsf.h"

#ifndef CACHE_LINE_SIZE
#define CACHE_LINE_SIZE	    32	/* default cacle line size */
#endif

static void *memory_pool;

/*
 *  �ס�������
 */
void heap_init(intptr_t exinf)
{
	uint32_t *param = (uint32_t *)exinf;
	uint32_t *p;
	int i, size;
	memory_pool = (void*)param[0];
	p = (uint32_t *)param[0];
	size = param[1] / sizeof(uint32_t);
	for(i = 0 ; i < size ; i++)
		*p++ = 0;
	init_memory_pool(param[1], (void*)memory_pool);
}

/*
 *  MALLOC
 */
_PTR	_EXFUN_NOTHROW(malloc,(size_t __size))
{
	void *mempool, *addr;
	mempool = (void *)memory_pool;
	if(mempool == NULL)
		return NULL;
	wai_sem(TLSF_SEM);
	addr = (void*)malloc_ex(__size, mempool);
	sig_sem(TLSF_SEM);
	return addr;
}

/*
 *  CALLOC
 */
_PTR	_EXFUN_NOTHROW(calloc,(size_t __nmemb, size_t __size))
{
	void *mempool, *addr;
	mempool = (void *)memory_pool;
	if(mempool == NULL)
		return NULL;
	wai_sem(TLSF_SEM);
	addr = (void*)malloc_ex(__nmemb * __size, mempool);
	sig_sem(TLSF_SEM);
	memset(addr, 0, __nmemb * __size);
	return addr;
}

/*
 *  FREE
 */
_VOID	_EXFUN_NOTHROW(free,(_PTR ptr))
{
	void *mempool = memory_pool;
	if(mempool != NULL){
		wai_sem(TLSF_SEM);
		free_ex(ptr, mempool);
		sig_sem(TLSF_SEM);
	}
}

/*
 *  CACHE ALINE MALLOC
 */
void *malloc_cache(uint32_t len)
{
	void * addr = malloc(len+CACHE_LINE_SIZE);
	uint32_t *p, uaddr;

	p = addr;
	uaddr = (uint32_t)addr;
	if((uaddr & (CACHE_LINE_SIZE-1)) != 0)
		uaddr &= ~(CACHE_LINE_SIZE-1);
	uaddr += CACHE_LINE_SIZE;
	p = (uint32_t *)uaddr;
	*(p-1) = (uint32_t)addr;
	return p;
}

/*
 *  CACHE ALINE FREE
 */
void free_cache(void *addr)
{
	uint32_t *p = addr;

	addr = (void*)(*(p-1));
	free(addr);
}

