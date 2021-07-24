/*
 *  TOPPERS/ASP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Advanced Standard Profile Kernel
 * 
 *  Copyright (C) 2008-2011 by Embedded and Real-Time Systems Laboratory
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
 *  Copyright (C) 2015-2018 by TOPPERS PROJECT Educational Working Group.
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
 *  @(#) $Id: device.c 698 2018-05-20 20:32:26Z roi $
 */
/*
 * STM32F7xx�ѥǥХ����ɥ饤��
 */
#include "kernel_impl.h"
#include <t_syslog.h>
#include <t_stdlib.h>
#include <sil.h>
#include <target_syssvc.h>
#include "kernel_cfg.h"
#include "device.h"


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

uint_t dipsw_value;
void (*exti0_func[1])(void) = { 0 };
void (*exti15_func[NUM_EXTI15_FUNC])(void) = { 0 };


/*
 *  GPIO�⡼�ɤ��������
 */
#define GPIO_MODE               0x00000003
#define EXTI_MODE               0x10000000
#define GPIO_MODE_IT            0x00010000
#define GPIO_MODE_EVT           0x00020000
#define RISING_EDGE             0x00100000
#define FALLING_EDGE            0x00200000
#define GPIO_OUTPUT_TYPE        0x00000010

static const uint32_t gpio_index[] = {
	TADR_GPIOA_BASE,			/* index 0 */
	TADR_GPIOB_BASE,			/* index 1 */
	TADR_GPIOC_BASE,			/* index 2 */
	TADR_GPIOD_BASE,			/* index 3 */
	TADR_GPIOE_BASE,			/* index 4 */
	TADR_GPIOF_BASE,			/* index 5 */
	TADR_GPIOG_BASE,			/* index 6 */
	TADR_GPIOH_BASE,			/* index 7 */
	TADR_GPIOI_BASE,			/* index 8 */
	TADR_GPIOJ_BASE,			/* index 9 */
	TADR_GPIOK_BASE				/* index 10 */
};

#define NUM_OF_GPIOPORT (sizeof(gpio_index)/sizeof(uint32_t))

/*
 *  GPIO�ν������ؿ�
 */
void
gpio_setup(uint32_t base, GPIO_Init_t *init, uint32_t pin)
{
	uint32_t iocurrent = 1<<pin;
	uint32_t temp = 0x00;
	uint32_t index;

	for(index = 0 ; index < NUM_OF_GPIOPORT ; index++){
		if(gpio_index[index] == base)
			break;
	}
	if(index == NUM_OF_GPIOPORT)
		return;

	/*
	 *  GPIO�⡼������
	 */
	/* ���륿�͡��ȡ��ե��󥯥���󡦥⡼������ */
	temp = sil_rew_mem((uint32_t *)(base+TOFF_GPIO_AFR0+(pin>>3)*4));
	temp &= ~((uint32_t)0xF << ((uint32_t)(pin & (uint32_t)0x07) * 4)) ;
	if(init->mode == GPIO_MODE_AF)
		temp |= ((uint32_t)(init->alternate) << (((uint32_t)pin & (uint32_t)0x07) * 4));
	sil_wrw_mem((uint32_t *)(base+TOFF_GPIO_AFR0+(pin>>3)*4), temp);

	/*  �����ϥ⡼������ */
	sil_modw_mem((uint32_t *)(base+TOFF_GPIO_MODER), (GPIO_MODER_MODER0 << (pin * 2)), ((init->mode & GPIO_MODE) << (pin * 2)));

	/*  ���ϥ⡼������ */
	if(init->mode == GPIO_MODE_OUTPUT || init->mode == GPIO_MODE_AF){
		sil_modw_mem((uint32_t *)(base+TOFF_GPIO_OSPEEDR), (GPIO_OSPEEDER_OSPEEDR0 << (pin * 2)), (init->speed << (pin * 2)));
		sil_modw_mem((uint32_t *)(base+TOFF_GPIO_OTYPER), (GPIO_OTYPER_OT_0 << pin), (init->otype << pin));
	}

	/*  �ץ륢�åס��ץ���������� */
	sil_modw_mem((uint32_t *)(base+TOFF_GPIO_PUPDR), (GPIO_PUPDR_PUPDR0 << (pin * 2)), (init->pull << (pin * 2)));

	/*
	 *  EXTI�⡼������
	 */
	if((init->mode & EXTI_MODE) == EXTI_MODE){
		/*  SYSCFG����å����� */
		sil_orw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_APB2ENR), RCC_APB2ENR_SYSCFGEN);
		temp = sil_rew_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_APB2ENR));

		temp = sil_rew_mem((uint32_t *)(TADR_SYSCFG_BASE+TOFF_SYSCFG_EXTICR0+(pin & 0x0C)));
		temp &= ~(0x0F << (4 * (pin & 0x03)));
		temp |= (index << (4 * (pin & 0x03)));
		sil_wrw_mem((uint32_t *)(TADR_SYSCFG_BASE+TOFF_SYSCFG_EXTICR0+(pin & 0x0C)), temp);

		if((init->mode & GPIO_MODE_IT) == GPIO_MODE_IT)
			sil_orw_mem((uint32_t *)(TADR_EXTI_BASE+TOFF_EXTI_IMR), iocurrent);
		else
			sil_andw_mem((uint32_t *)(TADR_EXTI_BASE+TOFF_EXTI_IMR), iocurrent);

		if((init->mode & GPIO_MODE_EVT) == GPIO_MODE_EVT)
			sil_orw_mem((uint32_t *)(TADR_EXTI_BASE+TOFF_EXTI_EMR), iocurrent);
		else
			sil_andw_mem((uint32_t *)(TADR_EXTI_BASE+TOFF_EXTI_EMR), iocurrent);

		if((init->mode & RISING_EDGE) == RISING_EDGE)
			sil_orw_mem((uint32_t *)(TADR_EXTI_BASE+TOFF_EXTI_RTSR), iocurrent);
		else
			sil_andw_mem((uint32_t *)(TADR_EXTI_BASE+TOFF_EXTI_RTSR), iocurrent);

		if((init->mode & FALLING_EDGE) == FALLING_EDGE)
			sil_orw_mem((uint32_t *)(TADR_EXTI_BASE+TOFF_EXTI_FTSR), iocurrent);
		else
			sil_andw_mem((uint32_t *)(TADR_EXTI_BASE+TOFF_EXTI_FTSR), iocurrent);
	}
}


/*
 *  ���ɥ쥹����D����å��奤��Х�ǡ��ȴؿ�
 *  parameter1  addr    address (aligned to CACHE_LINE_SIZE(32)-byte boundary)
 *  parameter2  dsize   size of memory block (in number of bytes)
 */
void
invalidatedcache_by_addr(uint8_t *addr, int32_t dsize)
{
	int32_t  op_size = dsize + (CACHE_LINE_SIZE-1);
	uint32_t op_addr = (((uint32_t)addr) & ~(CACHE_LINE_SIZE-1));

	asm("dsb 0xF":::"memory");
	while (op_size > 0) {
		sil_wrw_mem((uint32_t *)(TADR_SCB_BASE+TOFF_SCB_DCIMVAC), op_addr);
		op_addr += CACHE_LINE_SIZE;
		op_size -= CACHE_LINE_SIZE;
	}
	asm("dsb 0xF":::"memory");
	asm("isb 0xF":::"memory");
}

/*
 *  ���ɥ쥹����D����å���ե�å���ؿ�
 *  parameter1  addr    address (aligned to CACHE_LINE_SIZE(32)-byte boundary)
 *  parameter2  dsize   size of memory block (in number of bytes)
 */
void flushdcache_by_addr(uint8_t *addr, int32_t dsize)
{
	int32_t  op_size = dsize + (CACHE_LINE_SIZE-1);
	uint32_t op_addr = (((uint32_t)addr) & ~(CACHE_LINE_SIZE-1));

	asm("dsb 0xF":::"memory");
	while (op_size > 0) {
		sil_wrw_mem((uint32_t *)(TADR_SCB_BASE+TOFF_SCB_DCCMVAC), op_addr);
		op_addr += CACHE_LINE_SIZE;
		op_size -= CACHE_LINE_SIZE;
	}
	asm("dsb 0xF":::"memory");
	asm("isb 0xF":::"memory");
}

/*
 *  ���ɥ쥹����D����å���ե�å���&����Х�ǡ��ȴؿ�
 *  parameter1  addr    address (aligned to CACHE_LINE_SIZE(32)-byte boundary)
 *  parameter2  dsize   size of memory block (in number of bytes)
 */
void flushinvalidatedcache_by_addr(uint8_t *addr, int32_t dsize)
{
	int32_t  op_size = dsize + (CACHE_LINE_SIZE-1);
	uint32_t op_addr = (((uint32_t)addr) & ~(CACHE_LINE_SIZE-1));

	asm("dsb 0xF":::"memory");
	while (op_size > 0) {
		sil_wrw_mem((uint32_t *)(TADR_SCB_BASE+TOFF_SCB_DCCIMVAC), op_addr);
		op_addr += CACHE_LINE_SIZE;
		op_size -= CACHE_LINE_SIZE;
	}
	asm("dsb 0xF":::"memory");
	asm("isb 0xF":::"memory");
}


/*
 *  DMAC������ؿ�
 */

#define NUM_STMDMA      16
#define NUM_DMAINDEX    5
#define INDEX_TC        0
#define INDEX_HT        1
#define INDEX_TE        2
#define INDEX_FE        3
#define INDEX_DME       4

#define DMA_TRS_TIMEOUT 2000	/* 2�� */

static const uint32_t dma_base[NUM_STMDMA] = {
	TADR_DMA1_STM0_BASE,
	TADR_DMA1_STM1_BASE,
	TADR_DMA1_STM2_BASE,
	TADR_DMA1_STM3_BASE,
	TADR_DMA1_STM4_BASE,
	TADR_DMA1_STM5_BASE,
	TADR_DMA1_STM6_BASE,
	TADR_DMA1_STM7_BASE,
	TADR_DMA2_STM0_BASE,
	TADR_DMA2_STM1_BASE,
	TADR_DMA2_STM2_BASE,
	TADR_DMA2_STM3_BASE,
	TADR_DMA2_STM4_BASE,
	TADR_DMA2_STM5_BASE,
	TADR_DMA2_STM6_BASE,
	TADR_DMA2_STM7_BASE
};

static const uint32_t dma_flag[NUM_STMDMA][NUM_DMAINDEX] = {
	{	0x00000020, 0x00000010, 0x00000008, 0x00800001, 0x00800004 },
	{	0x00000800, 0x00000400, 0x00000200, 0x00000040, 0x00000100 },
	{	0x00200000, 0x00100000, 0x00080000, 0x00010000, 0x00040000 },
	{	0x08000000, 0x04000000, 0x02000000, 0x00400000, 0x01000000 },
	{	0x00000020, 0x00000010, 0x00000008, 0x00800001, 0x00800004 },
	{	0x00000800, 0x00000400, 0x00000200, 0x00000040, 0x00000100 },
	{	0x00200000, 0x00100000, 0x00080000, 0x00010000, 0x00040000 },
	{	0x08000000, 0x04000000, 0x02000000, 0x00400000, 0x01000000 },
	{	0x00000020, 0x00000010, 0x00000008, 0x00800001, 0x00800004 },
	{	0x00000800, 0x00000400, 0x00000200, 0x00000040, 0x00000100 },
	{	0x00200000, 0x00100000, 0x00080000, 0x00010000, 0x00040000 },
	{	0x08000000, 0x04000000, 0x02000000, 0x00400000, 0x01000000 },
	{	0x00000020, 0x00000010, 0x00000008, 0x00800001, 0x00800004 },
	{	0x00000800, 0x00000400, 0x00000200, 0x00000040, 0x00000100 },
	{	0x00200000, 0x00100000, 0x00080000, 0x00010000, 0x00040000 },
	{	0x08000000, 0x04000000, 0x02000000, 0x00400000, 0x01000000 }
};

static const uint32_t dma_faddr[NUM_STMDMA/4] = {
	(TADR_DMA1_BASE+TOFF_DMAI_LISR),
	(TADR_DMA1_BASE+TOFF_DMAI_HISR),
	(TADR_DMA2_BASE+TOFF_DMAI_LISR),
	(TADR_DMA2_BASE+TOFF_DMAI_HISR)
};

static DMA_Handle_t *pDmaHandler[NUM_STMDMA];


/*
 *  STREAM DMA������ؿ�
 *  parameter1  hdma: DMA�ϥ�ɥ�ؤΥݥ���
 *  return ER��
 */
ER
dma_init(DMA_Handle_t *hdma)
{
	uint32_t tmp = 0;
	uint32_t i;

	/* �ѥ�᡼�������å� */
	if(hdma == NULL)
		return E_PAR;

	for(i = 0 ; i < NUM_STMDMA ; i++){
		if(dma_base[i] == hdma->base)
			break;
	}
	if(i == NUM_STMDMA)
		return E_PAR;
	hdma->sdid = i;
	pDmaHandler[i] = hdma;

	/* DMA-CR�쥸�������� */
	tmp = sil_rew_mem((uint32_t *)(hdma->base+TOFF_DMAS_CR));

	/* DMA�⡼�ɥӥåȤ�ƥ�ݥ�ꥣ�˥��ꥢ */
	tmp &= ((uint32_t)~(DMA_SxCR_CHSEL | DMA_SxCR_MBURST | DMA_SxCR_PBURST | \
                      DMA_SxCR_PL    | DMA_SxCR_MSIZE  | DMA_SxCR_PSIZE  | \
                      DMA_SxCR_MINC  | DMA_SxCR_PINC   | DMA_SxCR_CIRC   | \
                      DMA_SxCR_DIR   | DMA_SxCR_CT     | DMA_SxCR_DBM));

	/* DMA�⡼�ɤ�ƥ�ݥ�ꥣ������ */
	tmp |=  hdma->Init.Channel           | hdma->Init.Direction        |
          hdma->Init.PeriphInc           | hdma->Init.MemInc           |
          hdma->Init.PeriphDataAlignment | hdma->Init.MemDataAlignment |
          hdma->Init.Mode                | hdma->Init.Priority;

	/* FIFO�⡼�ɤʤ�С��������� */
	if(hdma->Init.FIFOMode == DMA_FIFOMODE_ENABLE){
		tmp |=  hdma->Init.MemBurst | hdma->Init.PeriphBurst;
	}

	/* DMA�⡼������ */
	sil_wrw_mem((uint32_t *)(hdma->base+TOFF_DMAS_CR), tmp);

	/* DMA-FCR�쥸��������� */
	tmp = sil_rew_mem((uint32_t *)(hdma->base+TOFF_DMAS_FCR));

	/* FIFO�⡼�ɤ�ƥ�ݥ�ꥣ�˥��ꥢ */
	tmp &= (uint32_t)~(DMA_SxFCR_DMDIS | DMA_SxFCR_FTH);

	/* FIFO�⡼�ɤ�ƥ�ݥ�ꥣ������ */
	tmp |= hdma->Init.FIFOMode;

	/* FIFO�⡼�ɤʤ饹��å���ۡ���ɤ����� */
	if(hdma->Init.FIFOMode == DMA_FIFOMODE_ENABLE){
		tmp |= hdma->Init.FIFOThreshold;
	}

	/* DMA-FCR�쥸���������� */
	sil_wrw_mem((uint32_t *)(hdma->base+TOFF_DMAS_FCR), tmp);

	/* ���顼���֤򥯥ꥢ */
	hdma->ErrorCode = DMA_ERROR_NONE;
	return E_OK;
}

/*
 *  STREAM DMA��λ�ؿ�
 *  parameter1  hdma: DMA�ϥ�ɥ�ؤΥݥ���
 *  return ER��
 */
ER
dma_deinit(DMA_Handle_t *hdma)
{
	int i;

	/* �ѥ�᡼�������å� */
	if(hdma == NULL)
		return E_PAR;

	for(i = 0 ; i < NUM_STMDMA ; i++){
		if(dma_base[i] == hdma->base)
			break;
	}
	if(i == NUM_STMDMA)
		return E_PAR;
	hdma->sdid = i;

	/* DMA���͡��֥� */
	sil_andw_mem((uint32_t *)(hdma->base+TOFF_DMAS_CR), DMA_SxCR_EN);
	sil_wrw_mem((uint32_t *)(hdma->base+TOFF_DMAS_CR), 0);
	sil_wrw_mem((uint32_t *)(hdma->base+TOFF_DMAS_NDTR), 0);
	sil_wrw_mem((uint32_t *)(hdma->base+TOFF_DMAS_PAR), 0);
	sil_wrw_mem((uint32_t *)(hdma->base+TOFF_DMAS_M0AR), 0);
	sil_wrw_mem((uint32_t *)(hdma->base+TOFF_DMAS_M1AR), 0);
	sil_wrw_mem((uint32_t *)(hdma->base+TOFF_DMAS_FCR), 0x00000021);

	/* �ƥե饰�򥯥ꥢ */
	for(i = 0 ; i < NUM_DMAINDEX ; i++){
		sil_wrw_mem((uint32_t *)(dma_faddr[hdma->sdid/4]+8), dma_flag[hdma->sdid][i]);
	}

	/* ���顼���֤򥯥ꥢ */
	hdma->ErrorCode = DMA_ERROR_NONE;
	return E_OK;
}

/*
 *  STREAM DMA���ϴؿ�
 *  parameter1  hdma:       DMA�ϥ�ɥ�ؤΥݥ���
 *  parameter2  SrcAddress: ���������ɥ쥹
 *  parameter3  DstAddress: �ǥ��ƥ��͡�����󥢥ɥ쥹
 *  parameter4  DataLength: ž��Ĺ
 *  return ER��
 */
ER
dma_start(DMA_Handle_t *hdma, uint32_t SrcAddress, uint32_t DstAddress, uint32_t DataLength)
{
	/* �ѥ�᡼�������å� */
	if(hdma == NULL)
		return E_PAR;

	/* DMA��� */
	sil_andw_mem((uint32_t *)(hdma->base+TOFF_DMAS_CR), DMA_SxCR_EN);
	sil_andw_mem((uint32_t *)(hdma->base+TOFF_DMAS_CR), DMA_SxCR_TCIE | DMA_SxCR_HTIE | DMA_SxCR_TEIE | DMA_SxCR_DMEIE);
	sil_andw_mem((uint32_t *)(hdma->base+TOFF_DMAS_FCR), DMA_SxFCR_FEIE);
	hdma->status = DMA_STATUS_BUSY;

	/* DBM�ӥåȥ��ꥢ */
	sil_andw_mem((uint32_t *)(hdma->base+TOFF_DMAS_CR), DMA_SxCR_DBM);
	/* �ǡ���Ĺ���� */
	sil_wrw_mem((uint32_t *)(hdma->base+TOFF_DMAS_NDTR), DataLength);

	if((hdma->Init.Direction) == DMA_MEMORY_TO_PERIPH){
		/* ���꤫��ǥХ��� */
		flushdcache_by_addr((uint8_t *)SrcAddress, DataLength*4);
		sil_wrw_mem((uint32_t *)(hdma->base+TOFF_DMAS_PAR), DstAddress);
		sil_wrw_mem((uint32_t *)(hdma->base+TOFF_DMAS_M0AR), SrcAddress);
	}
	else{	/* �ǥХ���������� */
		flushinvalidatedcache_by_addr((uint8_t *)DstAddress, DataLength*4);
		sil_wrw_mem((uint32_t *)(hdma->base+TOFF_DMAS_PAR), SrcAddress);
		sil_wrw_mem((uint32_t *)(hdma->base+TOFF_DMAS_M0AR), DstAddress);
	}

	/* ����ߥ��͡��֥� */
	sil_orw_mem((uint32_t *)(hdma->base+TOFF_DMAS_CR), DMA_SxCR_TCIE | DMA_SxCR_HTIE | DMA_SxCR_TEIE | DMA_SxCR_DMEIE);
	sil_orw_mem((uint32_t *)(hdma->base+TOFF_DMAS_FCR), DMA_SxFCR_FEIE);
	/* DMA���� */
	sil_orw_mem((uint32_t *)(hdma->base+TOFF_DMAS_CR), DMA_SxCR_EN);

	return E_OK;
}

/*
 *  STREAM DMA��ߴؿ�
 *  parameter1  hdma  : DMA�ϥ�ɥ�ؤΥݥ���
 *  return ER��
 */
ER
dma_end(DMA_Handle_t *hdma)
{
	uint32_t tick = 0;

	/* DMA��� */
	sil_andw_mem((uint32_t *)(hdma->base+TOFF_DMAS_CR), DMA_SxCR_EN);

	/* DMA����Ԥ� */
	while((sil_rew_mem((uint32_t *)(hdma->base+TOFF_DMAS_CR)) & DMA_SxCR_EN) != 0){
	    /* Check for the Timeout */
		if(tick > DMA_TRS_TIMEOUT){
			/* �����ॢ���ȥ��顼���� */
			hdma->ErrorCode |= DMA_ERROR_TIMEOUT;
			return E_TMOUT;
		}
    	dly_tsk(1);
	}
	hdma->status = 0;
	return E_OK;
}

/*
 *  STREAM DMA����߽����ؿ�
 *  parameter1  hdma: DMA�ϥ�ɥ�ؤΥݥ���
 */
void
dma_inthandler(DMA_Handle_t *hdma)
{
	/*
	 *  ž�����顼����
	 */
	if((sil_rew_mem((uint32_t *)(dma_faddr[hdma->sdid/4])) & dma_flag[hdma->sdid][INDEX_TE]) != 0){
		if((sil_rew_mem((uint32_t *)(hdma->base+TOFF_DMAS_CR)) & DMA_SxCR_TEIE) != 0){
			/* ž�����顼����ߥ��ꥢ */
			sil_andw_mem((uint32_t *)(hdma->base+TOFF_DMAS_CR), DMA_SxCR_TEIE);
			/* ž�����顼�ե饰�򥯥ꥢ */
			sil_wrw_mem((uint32_t *)(dma_faddr[hdma->sdid/4]+8), dma_flag[hdma->sdid][INDEX_TE]);
			/* ž�����顼���֤����� */
			hdma->ErrorCode |= DMA_ERROR_TE;
			hdma->status = DMA_STATUS_READY_ERROR;
			/* ���顼������Хå� */
			if(hdma->errorcallback != NULL)
				hdma->errorcallback(hdma);
		}
	}
	/*
	 *  FIFO���顼����
	 */
	if((sil_rew_mem((uint32_t *)(dma_faddr[hdma->sdid/4])) & dma_flag[hdma->sdid][INDEX_FE]) != 0){
	    if((sil_rew_mem((uint32_t *)(hdma->base+TOFF_DMAS_FCR)) & DMA_SxFCR_FEIE) != 0){
			/* FIFO���顼����ߤ򥯥ꥢ */
			sil_andw_mem((uint32_t *)(hdma->base+TOFF_DMAS_FCR), DMA_SxFCR_FEIE);
			/* FIFO���顼�ե饰�򥯥ꥢ */
			sil_wrw_mem((uint32_t *)(dma_faddr[hdma->sdid/4]+8), dma_flag[hdma->sdid][INDEX_FE]);
			/* FIFO���顼������ */
			hdma->ErrorCode |= DMA_ERROR_FE;
			hdma->status = DMA_STATUS_READY_ERROR;
			/* ���顼������Хå� */
			if(hdma->errorcallback != NULL){
				hdma->errorcallback(hdma);
			}
		}
	}
	/*
	 *  �����쥯�ȡ����ꡦ���顼����
	 */
	if((sil_rew_mem((uint32_t *)(dma_faddr[hdma->sdid/4])) & dma_flag[hdma->sdid][INDEX_DME]) != 0){
		if((sil_rew_mem((uint32_t *)(hdma->base+TOFF_DMAS_CR)) & DMA_SxCR_DMEIE) != 0){
			/* DME���顼����ߤ򥯥ꥢ */
			sil_andw_mem((uint32_t *)(hdma->base+TOFF_DMAS_CR), DMA_SxCR_DMEIE);
			/* DME���顼�ե饰�򥯥ꥢ */
			sil_wrw_mem((uint32_t *)(dma_faddr[hdma->sdid/4]+8), dma_flag[hdma->sdid][INDEX_DME]);
			/* DME���顼������ */
			hdma->ErrorCode |= DMA_ERROR_DME;
			hdma->status = DMA_STATUS_READY_ERROR;
			/* ���顼������Хå� */
			if(hdma->errorcallback != NULL)
				hdma->errorcallback(hdma);
		}
	}
	/*
	 *  �ϡ���ž����λ����
	 */
	if((sil_rew_mem((uint32_t *)(dma_faddr[hdma->sdid/4])) & dma_flag[hdma->sdid][INDEX_HT]) != 0){
		if((sil_rew_mem((uint32_t *)(hdma->base+TOFF_DMAS_CR)) & DMA_SxCR_HTIE) != 0){
			/* �ޥ���Хåե��⡼�� */
			if((sil_rew_mem((uint32_t *)(hdma->base+TOFF_DMAS_CR)) & DMA_SxCR_DBM) != 0){
				/* �ϡ���ž���ե饰�򥯥ꥢ */
				sil_wrw_mem((uint32_t *)(dma_faddr[hdma->sdid/4]+8), dma_flag[hdma->sdid][INDEX_HT]);

				/* ���꣰���� */
				if((sil_rew_mem((uint32_t *)(hdma->base+TOFF_DMAS_CR)) & DMA_SxCR_CT) == 0){
					/* �ϡ��ե��꣰�ξ������� */
					hdma->status = DMA_STATUS_READY_HMEM0;
				}
				/* ���꣱���� */
				else if((sil_rew_mem((uint32_t *)(hdma->base+TOFF_DMAS_CR)) & DMA_SxCR_CT) != 0){
					/* �ϡ��ե��꣱�ξ������� */
					hdma->status = DMA_STATUS_READY_HMEM1;
				}
			}
			else{
				if((sil_rew_mem((uint32_t *)(hdma->base+TOFF_DMAS_CR)) & DMA_SxCR_CIRC) == 0){
					/* �ϡ���ž������ߤ򥯥ꥢ */
					sil_andw_mem((uint32_t *)(hdma->base+TOFF_DMAS_CR), DMA_SxCR_HTIE);
				}
				/* �ϡ���ž����λ�ե饰�򥯥ꥢ */
				sil_wrw_mem((uint32_t *)(dma_faddr[hdma->sdid/4]+8), dma_flag[hdma->sdid][INDEX_HT]);
				/* �ϡ���ž�����ơ��������ѹ� */
				hdma->status = DMA_STATUS_READY_HMEM0;
			}
			/* �ϡ���ž����λ������Хå� */
			if(hdma->xferhalfcallback != NULL)
				hdma->xferhalfcallback(hdma);
		}
	}
	/*
	 *  ž����λ����
	 */
	if((sil_rew_mem((uint32_t *)(dma_faddr[hdma->sdid/4])) & dma_flag[hdma->sdid][INDEX_TC]) != 0){
		if((sil_rew_mem((uint32_t *)(hdma->base+TOFF_DMAS_CR)) & DMA_SxCR_TCIE) != 0){
			if((sil_rew_mem((uint32_t *)(hdma->base+TOFF_DMAS_CR)) & DMA_SxCR_DBM) != 0){
				/* ž����λ�ե饰�򥯥ꥢ */
				sil_wrw_mem((uint32_t *)(dma_faddr[hdma->sdid/4]+8), dma_flag[hdma->sdid][INDEX_TC]);
				/* ���꣱���� */
				if((sil_rew_mem((uint32_t *)(hdma->base+TOFF_DMAS_CR)) & DMA_SxCR_CT) == 0){
					/* ����1ž����λ������Хå� */
					if(hdma->xferm1callback != NULL)
						hdma->xferm1callback(hdma);
				}
				/* ���꣰���� */
				else if((sil_rew_mem((uint32_t *)(hdma->base+TOFF_DMAS_CR)) & DMA_SxCR_CT) != 0){
					/* ž����λ������Хå� */
					if(hdma->xfercallback != NULL)
						hdma->xfercallback(hdma);
				}
			}
			else{
				if((sil_rew_mem((uint32_t *)(hdma->base+TOFF_DMAS_CR)) & DMA_SxCR_CIRC) == 0){
					/* ž������ߤ򥯥ꥢ */
					sil_andw_mem((uint32_t *)(hdma->base+TOFF_DMAS_CR), DMA_SxCR_TCIE);
				}
				/* ž����λ�ե饰�򥯥ꥢ */
				sil_wrw_mem((uint32_t *)(dma_faddr[hdma->sdid/4]+8), dma_flag[hdma->sdid][INDEX_TC]);
				/* ���ơ�������λ�� */
				hdma->ErrorCode = DMA_ERROR_NONE;
				hdma->status = DMA_STATUS_READY_MEM0;
				/* ž����λ������Хå� */
				if(hdma->xfercallback != NULL)
					hdma->xfercallback(hdma);
			}
		}
	}
}

/*
 *  STREAM DMA ����ߥ����ӥ��롼����
 */
void
stream_dma_isr(intptr_t exinf)
{
	dma_inthandler(pDmaHandler[(uint32_t)exinf]);
}


static DMA2D_Handle_t *pDma2dHandler;

/*
 *  DMA2D����ߴؿ�
 */
void dma2d_inthandler(DMA2D_Handle_t *hdma2d)
{
	uint32_t isr = sil_rew_mem((uint32_t *)(hdma2d->base+TOFF_DMA2D_ISR));
	/*
	 *  ž�����顼Ƚ��
	 */
	if((isr & DMA2D_ISR_TEIF) != 0){
		if((sil_rew_mem((uint32_t *)(hdma2d->base+TOFF_DMA2D_CR)) & DMA2D_CR_TEIE) != 0){
			/* ž�����顼�����Disable */
			sil_andw_mem((uint32_t *)(hdma2d->base+TOFF_DMA2D_CR), DMA2D_CR_TEIE);
			hdma2d->ErrorCode |= DMA2D_ERROR_TE;	/* ž�����顼���� */
			hdma2d->status = DMA2D_STATUS_ERROR;
			/* ž�����顼����ߥ��ꥢ */
			sil_wrw_mem((uint32_t *)(hdma2d->base+TOFF_DMA2D_IFCR), DMA2D_IFSR_CTEIF);
			/* ž�����顼������Хå� */
			if(hdma2d->errorcallback != NULL)
				hdma2d->errorcallback(hdma2d);
			isig_sem(DMA2DTRNSEM);
		}
	}
	/*
	 *  ����ե�����졼����󥨥顼Ƚ��
	 */
	if((isr & DMA2D_ISR_CEIF) != 0){
		if((sil_rew_mem((uint32_t *)(hdma2d->base+TOFF_DMA2D_CR)) & DMA2D_CR_CEIE) != 0){
			/* ����ե�����졼����󥨥顼Disable */
			sil_andw_mem((uint32_t *)(hdma2d->base+TOFF_DMA2D_CR), DMA2D_CR_CEIE);
			/* ����ե�����졼����󥨥顼����ߥ��ꥢ */
			sil_wrw_mem((uint32_t *)(hdma2d->base+TOFF_DMA2D_IFCR), DMA2D_IFSR_CCEIF);
			hdma2d->ErrorCode |= DMA2D_ERROR_CE;	/* ����ե�����졼����󥨥顼���� */
			hdma2d->status = DMA2D_STATUS_ERROR;
			/* ����ե�����졼����󥨥顼������Хå� */
			if(hdma2d->errorcallback != NULL)
				hdma2d->errorcallback(hdma2d);
			isig_sem(DMA2DTRNSEM);
		}
	}
	/*
	 *  ž����λ�����Ƚ��
	 */
	if((isr & DMA2D_ISR_TCIF) != 0){
		if((sil_rew_mem((uint32_t *)(hdma2d->base+TOFF_DMA2D_CR)) & DMA2D_CR_TCIE) != 0){ 
			/* ž����λ�����Disable */
			sil_andw_mem((uint32_t *)(hdma2d->base+TOFF_DMA2D_CR), DMA2D_CR_TCIE);
			/* ž����λ����ߥ��ꥢ */  
			sil_wrw_mem((uint32_t *)(hdma2d->base+TOFF_DMA2D_IFCR), DMA2D_IFSR_CTCIF);
			hdma2d->ErrorCode = DMA2D_ERROR_NONE;	/* ���顼����ꥻ�å� */
			hdma2d->status = DMA2D_STATUS_TCOMP;
			/* ž����λ������Хå� */
			if(hdma2d->xfercallback != NULL)
				hdma2d->xfercallback(hdma2d);
			isig_sem(DMA2DTRNSEM);
		}
	}
	/*
	 *  CLUT���ǥ���ž�������Ƚ��
	 */
	if((isr & DMA2D_ISR_CTCIF) != 0){
		if((sil_rew_mem((uint32_t *)(hdma2d->base+TOFF_DMA2D_CR)) & DMA2D_CR_CTCIE) != 0){ 
			/* CLUTž����λ�����Disable */
			sil_andw_mem((uint32_t *)(hdma2d->base+TOFF_DMA2D_CR), DMA2D_CR_CTCIE);
			/* ž����λ����ߥ��ꥢ */
			sil_wrw_mem((uint32_t *)(hdma2d->base+TOFF_DMA2D_IFCR), DMA2D_IFSR_CCTCIF);
			hdma2d->ErrorCode = DMA2D_ERROR_NONE;	/* ���顼����ꥻ�å� */
			hdma2d->status = DMA2D_STATUS_CCOMP;
			/* ž����λ������Хå� */
			if(hdma2d->xfercallback != NULL)
				hdma2d->xfercallback(hdma2d);
			isig_sem(DMA2DTRNSEM);
		}
	}
}

/*
 *  DMA2D�ν������
 *  parameter1 hdma2d    DMA2D�ϥ�ɥ�ؤΥݥ���
 *  return               ���ｪλ�ʤ��E_OK
 */
ER
dma2d_init(DMA2D_Handle_t *hdma2d)
{
	volatile uint32_t tmp = 0;
	static bool_t init = false;

	if(hdma2d == NULL)
		return E_PAR;

	wai_sem(DMA2DSEM);	/* ���ޥե���å� */
	pDma2dHandler = hdma2d;
	if(!init){
		/* ����å����� */
		sil_orw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_AHB1ENR), RCC_AHB1ENR_DMA2DEN);
		tmp = sil_rew_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_AHB1ENR));
		(void)(tmp);
		init = true;
	}

	/* DMA2D CR�쥸�������� */
	sil_modw_mem((uint32_t *)(hdma2d->base+TOFF_DMA2D_CR), DMA2D_CR_MODE, hdma2d->Init.Mode);
	/* DMA2D OPFCC�쥸�������� */
	sil_modw_mem((uint32_t *)(hdma2d->base+TOFF_DMA2D_OPFCCR), DMA2D_OPFCCR_CM, hdma2d->Init.ColorMode);
	/* DMA2D OOR�쥸�������� */
	sil_modw_mem((uint32_t *)(hdma2d->base+TOFF_DMA2D_OOR), DMA2D_OOR_LO, hdma2d->Init.OutputOffset);

#if defined(TOPPERS_STM32F767_NUCLEO144) || defined(TOPPERS_STM32F769_DISCOVERY)
	/* DMA2D OPFCCR�쥸���� AI/RBSWAP���� */
	sil_modw_mem((uint32_t *)(hdma2d->base+TOFF_DMA2D_OPFCCR), DMA2D_OPFCCR_AI, hdma2d->Init.AlphaInverted);
	sil_modw_mem((uint32_t *)(hdma2d->base+TOFF_DMA2D_OPFCCR), DMA2D_OPFCCR_RBS, hdma2d->Init.RedBlueSwap);
#endif

	/* ���顼����ꥻ�å� */
	hdma2d->ErrorCode = DMA2D_ERROR_NONE;
	return E_OK;
}

/*
 *  DMA2D�쥤�䡼����
 *  parameter1 hdma2d    DMA2D�ϥ�ɥ�ؤΥݥ���
 *  parameter2 LayerIdx  DMA2D�쥤�䡼����ǥå���
 *  return               ���ｪλ�ʤ��E_OK
 */
ER
dma2d_configlayer(DMA2D_Handle_t *hdma2d, uint32_t LayerIdx)
{
	DMA2D_LayerCfg_t *pLayerCfg = &hdma2d->LayerCfg[LayerIdx];
	uint32_t tmp = 0;

	if(hdma2d->Init.Mode != DMA2D_R2M && hdma2d->Init.Mode != DMA2D_M2M){
		if(pLayerCfg->AlphaMode != DMA2D_NO_MODIF_ALPHA
			&& pLayerCfg->AlphaMode != DMA2D_REPLACE_ALPHA && pLayerCfg->AlphaMode != DMA2D_COMBINE_ALPHA){
			sig_sem(DMA2DSEM);
			return E_PAR;
		}
	}

	/* �쥤�䡼0(background)DMA2D���� */
	if(LayerIdx == 0){
		/* DMA2D BGPFCCR�쥸�������� */
		tmp = sil_rew_mem((uint32_t *)(hdma2d->base+TOFF_DMA2D_BGPFCCR));
		tmp &= (uint32_t)~(DMA2D_BGPFCCR_CM | DMA2D_BGPFCCR_AM | DMA2D_BGPFCCR_ALPHA); 
		if (pLayerCfg->InputColorMode == CM_A4 || pLayerCfg->InputColorMode == CM_A8)
			tmp |= (pLayerCfg->InputColorMode | (pLayerCfg->AlphaMode << 16) | ((pLayerCfg->InputAlpha) & 0xFF000000));
		else
			tmp |= (pLayerCfg->InputColorMode | (pLayerCfg->AlphaMode << 16) | (pLayerCfg->InputAlpha << 24));
		sil_wrw_mem((uint32_t *)(hdma2d->base+TOFF_DMA2D_BGPFCCR), tmp);

		/* DMA2D BGOR�쥸�������� */
		sil_modw_mem((uint32_t *)(hdma2d->base+TOFF_DMA2D_BGOR), DMA2D_BGOR_LO, pLayerCfg->InputOffset);
		/* DMA2D BGCOLR�쥸�������� */
		if ((pLayerCfg->InputColorMode == CM_A4) || (pLayerCfg->InputColorMode == CM_A8)){
			sil_wrw_mem((uint32_t *)(hdma2d->base+TOFF_DMA2D_BGCOLR), (pLayerCfg->InputAlpha & 0x00FFFFFF));
		}
	}
	/* �쥤�䡼1(foreground)DMA2D���� */
	else{
		/* DMA2D FGPFCCR�쥸�������� */
		tmp = sil_rew_mem((uint32_t *)(hdma2d->base+TOFF_DMA2D_FGPFCCR));
		tmp &= (uint32_t)~(DMA2D_FGPFCCR_CM | DMA2D_FGPFCCR_AM | DMA2D_FGPFCCR_ALPHA); 
		if (pLayerCfg->InputColorMode == CM_A4 || pLayerCfg->InputColorMode == CM_A8)
			tmp |= (pLayerCfg->InputColorMode | (pLayerCfg->AlphaMode << 16) | ((pLayerCfg->InputAlpha) & 0xFF000000));
		else
			tmp |= (pLayerCfg->InputColorMode | (pLayerCfg->AlphaMode << 16) | (pLayerCfg->InputAlpha << 24));
		sil_wrw_mem((uint32_t *)(hdma2d->base+TOFF_DMA2D_FGPFCCR), tmp);

		/* DMA2D FGOR�쥸�������� */
		sil_modw_mem((uint32_t *)(hdma2d->base+TOFF_DMA2D_FGOR), DMA2D_FGOR_LO, pLayerCfg->InputOffset);
		/* DMA2D FGCOLR�쥸�������� */
		if ((pLayerCfg->InputColorMode == CM_A4) || (pLayerCfg->InputColorMode == CM_A8)){
			sil_wrw_mem((uint32_t *)(hdma2d->base+TOFF_DMA2D_FGCOLR), (pLayerCfg->InputAlpha & 0x00FFFFFF));
		}
	}
	return E_OK;
}

/*
 *  DMA2Dž����������
 *  parameter1 hdma2d    DMA2D�ϥ�ɥ�ؤΥݥ���
 *  parameter2 pdata     ����������Хåե����ɥ쥹
 *  parameter3 DstAddr   �ǥ��ƥ��͡���������Хåե����ɥ쥹
 *  parameter4 Width     ž���ǡ�����
 *  parameter5 Height    ž���ǡ����⤵
 *  return               ���ｪλ�ʤ��E_OK
 */
ER
dma2d_start(DMA2D_Handle_t *hdma2d, uint32_t pdata, uint32_t DstAddress, uint32_t Width,  uint32_t Height)
{
	if(hdma2d == NULL)
		return E_PAR;
	/* DMA2D Disable */
	sil_andw_mem((uint32_t *)(hdma2d->base+TOFF_DMA2D_CR), DMA2D_CR_START);

	/* DMA2D�ǡ������������� */
	sil_wrw_mem((uint32_t *)(hdma2d->base+TOFF_DMA2D_NLR), (Width << 16) | Height);

	/* DMA2D�Υǥ��ƥ��͡�����󥢥ɥ쥹���� */
	sil_wrw_mem((uint32_t *)(hdma2d->base+TOFF_DMA2D_OMAR), DstAddress);

	/* DMA2D����⡼������ */
	if (hdma2d->Init.Mode == DMA2D_R2M){
		uint32_t tmp = 0;
		uint32_t tmp1 = 0;
		uint32_t tmp2 = 0;
		uint32_t tmp3 = 0;
		uint32_t tmp4 = 0;

		tmp1 = pdata & DMA2D_OCOLR_ALPHA_1;
		tmp2 = pdata & DMA2D_OCOLR_RED_1;
		tmp3 = pdata & DMA2D_OCOLR_GREEN_1;
		tmp4 = pdata & DMA2D_OCOLR_BLUE_1;

		/* Prepare the value to be wrote to the OCOLR register according to the color mode */
		if (hdma2d->Init.ColorMode == DMA2D_ARGB8888){
			tmp = (tmp3 | tmp2 | tmp1| tmp4);
		}
		else if (hdma2d->Init.ColorMode == DMA2D_RGB888){
			tmp = (tmp3 | tmp2 | tmp4);  
		}
		else if (hdma2d->Init.ColorMode == DMA2D_RGB565){
			tmp2 = (tmp2 >> 19);
			tmp3 = (tmp3 >> 10);
			tmp4 = (tmp4 >> 3 );
			tmp  = ((tmp3 << 5) | (tmp2 << 11) | tmp4); 
		}
 		else if (hdma2d->Init.ColorMode == DMA2D_ARGB1555){
			tmp1 = (tmp1 >> 31);
			tmp2 = (tmp2 >> 19);
			tmp3 = (tmp3 >> 11);
			tmp4 = (tmp4 >> 3 );
			tmp  = ((tmp3 << 5) | (tmp2 << 10) | (tmp1 << 15) | tmp4);
		}
		else{ /* DMA2D_CMode = DMA2D_ARGB4444 */
			tmp1 = (tmp1 >> 28);
			tmp2 = (tmp2 >> 20);
			tmp3 = (tmp3 >> 12);
			tmp4 = (tmp4 >> 4 );
			tmp  = ((tmp3 << 4) | (tmp2 << 8) | (tmp1 << 12) | tmp4);
		}
		/* Write to DMA2D OCOLR register */
		sil_wrw_mem((uint32_t *)(hdma2d->base+TOFF_DMA2D_OCOLR), tmp);
	}
	else{ /* M2M, M2M_PFC or M2M_Blending DMA2D Mode */
		/* DMA2D���������ɥ쥹���� */
		sil_wrw_mem((uint32_t *)(hdma2d->base+TOFF_DMA2D_FGMAR), pdata);
	}
	/*
	 *  ž������ߤ����
	 */
	sil_orw_mem((uint32_t *)(hdma2d->base+TOFF_DMA2D_CR), (DMA2D_CR_TEIE | DMA2D_CR_CEIE | DMA2D_CR_TCIE));

	/* DMA2D Enable */
	sil_orw_mem((uint32_t *)(hdma2d->base+TOFF_DMA2D_CR), DMA2D_CR_START);
	hdma2d->status = DMA2D_STATUS_BUSY;
	return E_OK;
}

/*
 *  DMA2Dž���Ԥ�
 *  parameter1 hdma2d    DMA2D�ϥ�ɥ�ؤΥݥ���
 *  parameter2 Timeout   �Ԥ�����(MS)
 *  return               ���ｪλ�ʤ��E_OK
 */
ER
dma2d_waittransfar(DMA2D_Handle_t *hdma2d, uint32_t Timeout)
{
	ER       ercd = E_OK;
	uint32_t time = 0;

	/*
	 *  ž�����꤬������Υݡ�����Ԥ�
	 */
	if((sil_rew_mem((uint32_t *)(hdma2d->base+TOFF_DMA2D_CR)) & DMA2D_CR_START) != 0){
		while(hdma2d->status == DMA2D_STATUS_BUSY){
			if(time > Timeout){	/* �����ॢ���Ȼ��֤�����å� */
				hdma2d->ErrorCode |= DMA2D_ERROR_TIMEOUT;	/* �����ॢ���ȥ��顼���� */
				sig_sem(DMA2DSEM);
				return E_TMOUT;
			}
			twai_sem(DMA2DTRNSEM, 1);
			time++;
		}
		if(hdma2d->status == DMA2D_STATUS_ERROR)
			ercd = E_OBJ;
	}
	/*
	 *  CLUT���ǥ��󥰤ν����Ԥ�
	 */
	time = 0;
	if((sil_rew_mem((uint32_t *)(hdma2d->base+TOFF_DMA2D_FGPFCCR)) & DMA2D_FGPFCCR_START) != 0){
		while(hdma2d->status == DMA2D_STATUS_BUSY){
			if(time > Timeout){	/* �����ॢ���Ȼ��֤�����å� */
				hdma2d->ErrorCode |= DMA2D_ERROR_TIMEOUT;	/* �����ॢ���ȥ��顼���� */
				sig_sem(DMA2DSEM);
				return E_TMOUT;
			}
			twai_sem(DMA2DTRNSEM, 1);
			time++;
		}
		if(hdma2d->status == DMA2D_STATUS_ERROR)
			ercd = E_OBJ;
	}
	sig_sem(DMA2DSEM);	/* ���ޥե������å� */
	return ercd;
}

/*
 *  DMA2D����ߥϥ�ɥ�
 */
void dma2d_handler(void)
{
	dma2d_inthandler(pDma2dHandler);
}


#define RTC_ASYNCH_PREDIV       0x7F		/* LSE�� RTC����å� */
#define RTC_SYNCH_PREDIV        0x00FF

#define RTC_TR_RESERVED_MASK    (RTC_TR_SU | RTC_TR_ST | RTC_TR_MNU | RTC_TR_MNT | \
								 RTC_TR_HU | RTC_TR_HT | RTC_TR_PM)
#define RTC_DR_RESERVED_MASK    (RTC_DR_DU | RTC_DR_DT  | RTC_DR_MU | RTC_DR_MT | \
								 RTC_DR_WDU | RTC_DR_YU | RTC_DR_YT)
#define RTC_INIT_MASK           ((uint32_t)0xFFFFFFFF)  
#define RTC_RSF_MASK            ((uint32_t)0xFFFFFF5F)

#define RTC_TIMEOUT_VALUE       (1000*1000)
#define LSI_TIMEOUT_VALUE       (100*1000)
#define RCC_DBP_TIMEOUT_VALUE   (100*1000)
#define RCC_LSE_TIMEOUT_VALUE   (5000*1000)

#define RTC_EXTI_LINE_WUPTIMER  ((uint32_t)EXTI_IMR_MR22)	/* External interrupt line 22 Connected to the RTC Wake-up event */

uint32_t rtcerrorcode;

static void (*rtcalarmA_callback)(void);	/* DMA transfer complete Memory1 callback */
static void (*rtcalarmB_callback)(void);	/* DMA transfer error callback */
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
	volatile uint32_t tmpreg;

	rtcerrorcode = 0;
	/*
	 *  LSI�򥪥դ���
	 */
	sil_andw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CSR), RCC_CSR_LSION);
	while((sil_rew_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CSR)) & RCC_CSR_LSIRDY) != 0){
		if( tick > LSI_TIMEOUT_VALUE){
			rtcerrorcode |= RTC_ERROR_LSI;
			return;
		}
		sil_dly_nse(1000);
		tick++;
	}

	/*
	 *  LSE�򥪥󤹤�
	 */
    /* Enable Power Clock*/
	sil_orw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_APB1ENR), RCC_APB1ENR_PWREN);
	tmpreg = sil_rew_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_APB1ENR));

	/* Enable write access to Backup domain */
	sil_orw_mem((uint32_t *)(TADR_PWR_BASE+TOFF_PWR_CR1), PWR_CR1_DBP);

	/* Wait for Backup domain Write protection disable */
	tick = 0;
	while((sil_rew_mem((uint32_t *)(TADR_PWR_BASE+TOFF_PWR_CR1)) & PWR_CR1_DBP) == 0){
		if(tick > RCC_DBP_TIMEOUT_VALUE){
			rtcerrorcode |= RTC_ERROR_LSE;
			return;
		}
		sil_dly_nse(1000);
		tick++;
	}
    
    /*
	 *  LSEON / LSEBYP�ꥻ�å�
	 */
	sil_andw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_BDCR), RCC_BDCR_LSEBYP);
	sil_andw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_BDCR), RCC_BDCR_LSEON);

	tick = 0;
	while((sil_rew_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_BDCR)) & RCC_BDCR_LSERDY) != 0){
		if(tick > RCC_LSE_TIMEOUT_VALUE){
			rtcerrorcode |= RTC_ERROR_LSE;
			return;
		}
		sil_dly_nse(1000);
		tick++;
	}
    
    /*
	 *  LSE�������
	 */
	sil_andw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_BDCR), RCC_BDCR_LSEBYP);
	sil_orw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_BDCR), RCC_BDCR_LSEON);

	tick = 0;
	while((sil_rew_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_BDCR)) & RCC_BDCR_LSERDY) == 0){
		if(tick > RCC_LSE_TIMEOUT_VALUE){
			rtcerrorcode |= RTC_ERROR_LSE;
			return;
		}
		sil_dly_nse(1000);
		tick++;
	}

    /*
	 *  RTC����å�����
	 */
	if((sil_rew_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_BDCR)) & RCC_BDCR_RTCSEL) != RCC_BDCR_RTCSEL_0){
		/*
		 *  ����å��Ÿ�����
		 */
		tmpreg = (sil_rew_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_BDCR)) & ~(RCC_BDCR_RTCSEL));

		sil_orw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_BDCR), RCC_BDCR_BDRST);
		sil_andw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_BDCR), RCC_BDCR_BDRST);

		sil_wrw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_BDCR), tmpreg);

		if((tmpreg & RCC_BDCR_LSERDY) != 0){
			tick = 0;
			/* Wait till LSE is ready */  
			while((sil_rew_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_BDCR)) & RCC_BDCR_LSERDY) == 0){
				if(tick > RCC_LSE_TIMEOUT_VALUE){
					rtcerrorcode |= RTC_ERROR_RTC;
					return;
				}
				sil_dly_nse(1000);
				tick++;
			}
		}
		sil_andw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CFGR), RCC_CFGR_RTCPRE);
		sil_orw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_BDCR), RCC_BDCR_RTCSEL_0);
	}

	/*
	 *  RTC�Υ���å�����
	 */
	sil_orw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_BDCR), RCC_BDCR_RTCEN);

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
		sil_andw_mem((uint32_t *)(TADR_RTC_BASE+TOFF_RTC_OR), RTC_OR_ALARMTYPE);

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
	else{
		/*
		  ALARM-B �쥸��������
		 */
		parm->alarm = RTC_ALARM_B;
		tmparm = sil_rew_mem((uint32_t *)(TADR_RTC_BASE+TOFF_RTC_ALRMBR));
		subsecond = sil_rew_mem((uint32_t *)(TADR_RTC_BASE+TOFF_RTC_ALRMBSSR)) & RTC_ALRMBSSR_SS;
	}

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
	uint32_t tmp;

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
	}
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
	}
	/*
	 *  EXTI RTC�ڥ�ǥ��󥰥��ꥢ
	 */
	sil_wrw_mem((uint32_t *)(TADR_EXTI_BASE+TOFF_EXTI_PR), EXTI_PR_PR17);
}

/*
 *  WAKEUP����ߥϥ�ɥ�
 */
void wakeup_handler(void)
{
	uint32_t isr;

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


/*
 *  LED��³�ݡ���
 *
 *  ��ĥI/O�ܡ��ɤ�LED1-3�ϥץ���ޥ֥������ϥݡ���0��
 *  �˥���С�����𤷤���³����Ƥ��롥
 */

typedef struct gio_confstruct
{
	uint32_t    clkbit;
	uint32_t    giobase;
	uint32_t    pinpos;
	uint32_t    pinpp;
} gio_conftype;

#if defined(TOPPERS_STM32F7_DISCOVERY)
static const gio_conftype led_confdata[] = {
	{ RCC_AHB1ENR_GPIOIEN, TADR_GPIOI_BASE, PINPOSITION1, GPIO_PULLUP}
};
#elif defined(TOPPERS_STM32F769_DISCOVERY)
static const gio_conftype led_confdata[] = {
	{ RCC_AHB1ENR_GPIOJEN, TADR_GPIOJ_BASE, PINPOSITION13, GPIO_PULLUP},
	{ RCC_AHB1ENR_GPIOJEN, TADR_GPIOJ_BASE, PINPOSITION5, GPIO_PULLUP}
};
#elif defined(TOPPERS_STM32F723_DISCOVERY)
static const gio_conftype led_confdata[] = {
	{ RCC_AHB1ENR_GPIOAEN, TADR_GPIOA_BASE, PINPOSITION7, GPIO_PULLUP},
	{ RCC_AHB1ENR_GPIOBEN, TADR_GPIOB_BASE, PINPOSITION1, GPIO_PULLUP}
};
#else
static const gio_conftype led_confdata[] = {
	{ RCC_AHB1ENR_GPIOBEN, TADR_GPIOB_BASE, PINPOSITION0, GPIO_PULLUP},
	{ RCC_AHB1ENR_GPIOBEN, TADR_GPIOB_BASE, PINPOSITION7, GPIO_PULLUP},
	{ RCC_AHB1ENR_GPIOBEN, TADR_GPIOB_BASE, PINPOSITION14, GPIO_PULLUP}
};
#endif
#define NUM_LED   (sizeof(led_confdata)/sizeof(gio_conftype))

/*
 *  LED��³�ݡ��Ƚ����
 */ 
void
led_init(intptr_t exinf)
{
	const gio_conftype *pled = &led_confdata[0];
	GPIO_Init_t GPIO_Init_Data;
	uint32_t i;

	for(i = 0 ; i < NUM_LED ; pled++, i++){
		sil_orw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_AHB1ENR), pled->clkbit);
		/* �⡼������ */
		GPIO_Init_Data.mode      = GPIO_MODE_OUTPUT;
		/* �ץ륢�å� �ץ���������� */
		GPIO_Init_Data.pull      = pled->pinpp;
		/* ���ϥ⡼������ */
		GPIO_Init_Data.otype     = GPIO_OTYPE_PP;
        /* ���ԡ������� */
		GPIO_Init_Data.speed     = GPIO_SPEED_HIGH;
		gpio_setup(pled->giobase, &GPIO_Init_Data, pled->pinpos);

		/* ���� */
		sil_wrw_mem((uint32_t *)(pled->giobase+TOFF_GPIO_BSRR), (1 << pled->pinpos)<<16);
	}
}

/*
 *  LED��³�ݡ����ɤ߽Ф�
 */
uint_t
led_in(void)
{
	const gio_conftype *pled = &led_confdata[0];
	uint32_t data, i;

	for(i = 0, data = 0 ; i < NUM_LED ; pled++, i++){
		if((sil_rew_mem((uint32_t *)(pled->giobase+TOFF_GPIO_IDR)) & (1<<(pled->pinpos))) != 0)
			data |= (1<<i);
	}
	return data;
}

/*
 *  LED��³�ݡ��Ƚ񤭹���
 */ 
void
led_out(unsigned int led_data)
{
	const gio_conftype *pled = &led_confdata[0];
	uint32_t reg1, reg2, i;

	/* �����ͤ�LED��³�ӥåȰʳ����ѹ����ʤ� */
	for(i = 0 ; i < NUM_LED ; pled++, i++){
		reg1 = reg2 = 0;
		if((led_data & (1<<i)) != 0)
			reg2 = 1 << pled->pinpos;
		else
			reg1 = 1 << pled->pinpos;

		/* �񤭹��ߥǡ������������ƽ񤭹��� */
		sil_wrw_mem((uint32_t *)(pled->giobase+TOFF_GPIO_BSRR), (reg1<<16) | reg2);
	}
}

/*
 * LED�ȥ����å��θ������ꡦ�ɤ߹��ߴؿ���
 */

/*
 *  LED�ξ�����¸���ѿ�
 */	
unsigned int LedState;


/*
 *  LED��������
 */
void 
set_led_state(unsigned int led, unsigned char state){
	if (state == ON) {
		LedState = LedState | led;
	} else {
		LedState = LedState & ~led;
	}
	led_out(LedState);
}


#if defined(TOPPERS_STM32F7_DISCOVERY)
#define TADR_PSW_BASE   TADR_GPIOI_BASE
#define PSWGIOPEN       RCC_AHB1ENR_GPIOIEN
#define	PSW_PIN         PINPOSITION11
#elif defined(TOPPERS_STM32F769_DISCOVERY) || defined(TOPPERS_STM32F723_DISCOVERY)
#define TADR_PSW_BASE   TADR_GPIOA_BASE
#define PSWGIOPEN       RCC_AHB1ENR_GPIOAEN
#define	PSW_PIN         PINPOSITION0
#else
#define TADR_PSW_BASE   TADR_GPIOC_BASE
#define PSWGIOPEN       RCC_AHB1ENR_GPIOCEN
#define	PSW_PIN         PINPOSITION13
#endif

/*
 * PUSH�����å���³�ݡ��Ƚ����
 */
void
switch_push_init(intptr_t exinf)
{
	GPIO_Init_t GPIO_Init_Data;
	volatile uint32_t temp;

	/* ���ϥݡ��Ȥ����� */
	/* Enable the BUTTON Clock */
	sil_orw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_AHB1ENR), PSWGIOPEN);
	temp = sil_rew_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_AHB1ENR));
	(void)(temp);
	/* �⡼������ */
	GPIO_Init_Data.mode      = GPIO_MODE_IT_FALLING;
	/* �ץ륢�å� �ץ���������� */
	GPIO_Init_Data.pull      = GPIO_NOPULL;
	gpio_setup(TADR_PSW_BASE, &GPIO_Init_Data, PSW_PIN);
}

/*
 * PUSH�����å�������Хå��ؿ�����
 */
void setup_sw_func(intptr_t exinf)
{
	exti_func[PSW_PIN-EXTI_BASENO] = (void (*)(void))exinf;
}

/*
 * EXTI0����ߥϥ�ɥ�
 */
void
exti0_handler(void)
{
	uint32_t istatus = sil_rew_mem((uint32_t *)(TADR_EXTI_BASE+TOFF_EXTI_PR));

	if((istatus & (1<<0)) != 0){
		if(exti0_func[0] != NULL)
			(exti0_func[0])();
	}
    sil_wrw_mem((uint32_t *)(TADR_EXTI_BASE+TOFF_EXTI_PR), istatus);
}

/*
 * EXTI15-10����ߥϥ�ɥ�
 */
void
exti15_handler(void)
{
	/* EXTI line interrupt detected */
	uint32_t istatus = sil_rew_mem((uint32_t *)(TADR_EXTI_BASE+TOFF_EXTI_PR));
	uint32_t i;
	for(i = 0 ; i < NUM_EXTI15_FUNC ; i++){
		if((istatus & (1<<(i+EXTI15_BASENO))) != 0){
			if(exti15_func[i] != NULL)
				(exti15_func[i])();
		}
	}
    sil_wrw_mem((uint32_t *)(TADR_EXTI_BASE+TOFF_EXTI_PR), istatus);
}


/*
 * DIPSW�μ�Ф�
 */
uint_t
switch_dip_sense(void)
{
	return dipsw_value;
}

