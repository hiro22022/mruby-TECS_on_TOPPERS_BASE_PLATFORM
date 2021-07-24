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
 *  @(#) $Id: device.c 698 2017-06-25 15:28:07Z roi $
 */
/*
 * STM32L0xx�ѥǥХ����ɥ饤��
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

#if !defined(HSE_VALUE)
#define HSE_VALUE               8000000		/* Value of the External oscillator in Hz */
#endif /* HSE_VALUE */
#if !defined(HSI_VALUE)
#define HSI_VALUE               16000000	/* Value of the Internal oscillator in Hz*/
#endif /* HSI_VALUE */

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
void (*exti0_func[NUM_EXTI0_FUNC])(void) = { 0 };
void (*exti2_func[NUM_EXTI2_FUNC])(void) = { 0 };
void (*exti4_func[NUM_EXTI4_FUNC])(void) = { 0 };


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
	0,							/* index 5 */
	0,							/* index 6 */
	TADR_GPIOH_BASE				/* index 7 */
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
	sil_modw_mem((uint32_t *)(base+TOFF_GPIO_MODER), (GPIO_MODER_MODER2 << (pin * 2)), ((init->mode & GPIO_MODE) << (pin * 2)));

	/*  ���ϥ⡼������ */
	if(init->mode == GPIO_MODE_OUTPUT || init->mode == GPIO_MODE_AF){
		sil_modw_mem((uint32_t *)(base+TOFF_GPIO_OSPEEDR), (GPIO_OSPEEDER_OSPEEDR2 << (pin * 2)), (init->speed << (pin * 2)));
		sil_modw_mem((uint32_t *)(base+TOFF_GPIO_OTYPER), (GPIO_OTYPER_OT << pin), (init->otype << pin));
	}

	/*  �ץ륢�åס��ץ���������� */
	sil_modw_mem((uint32_t *)(base+TOFF_GPIO_PUPDR), (GPIO_PUPDR_PUPDR2 << (pin * 2)), (init->pull << (pin * 2)));

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


static const uint8_t PLLMulTable[9] = {3U, 4U, 6U, 8U, 12U, 16U, 24U, 32U, 48U};

/*
 *  �����ƥ९��å����֤�
 *  return SYSCLK(Hz)
 */
uint32_t
get_sysclock(uint8_t *sws)
{
	uint32_t cfgr = 0, pllm = 0, plld = 0, pllvco = 0, msiclkrange = 0;
	uint32_t sysclockfreq = 0;

	cfgr = sil_rew_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CFGR));

	/*
	 *  SYSCLK����������Ф�
	 */
	switch (cfgr & RCC_CFGR_SWS){
    case RCC_CFGR_SWS_HSI:	/* HSI used as system clock source */
		if((sil_rew_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CR)) & RCC_CR_HSIDIVF) != 0)
			sysclockfreq =  (HSI_VALUE >> 2);
		else
			sysclockfreq =  HSI_VALUE;
		break;
	case RCC_CFGR_SWS_HSE:	/* HSE used as system clock */
		sysclockfreq = HSE_VALUE;
		break;
	case RCC_CFGR_SWS_PLL:	/* PLL used as system clock */
		pllm = PLLMulTable[(uint32_t)(cfgr & RCC_CFGR_PLLMUL) >> 18];
		plld = ((uint32_t)(cfgr & RCC_CFGR_PLLDIV) >> 22) + 1;
		if((sil_rew_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CFGR)) & RCC_CFGR_PLLSRC) != RCC_CFGR_PLLSRC_HSI)
			pllvco = (HSE_VALUE * pllm) / plld;	/* HSE used as PLL clock source */
		else{
			if((sil_rew_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CR)) & RCC_CR_HSIDIVF) != 0)
				pllvco = ((HSI_VALUE >> 2) * pllm) / plld;
			else
				pllvco = (HSI_VALUE * pllm) / plld;
		}
		sysclockfreq = pllvco;
		break;
	case RCC_CFGR_SWS_MSI:	/* MSI used as system clock source */
	default: /* MSI used as system clock */
		msiclkrange = (sil_rew_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_ICSCR)) & RCC_ICSCR_MSIRANGE ) >> 13;
		sysclockfreq = (32768 * (1 << (msiclkrange + 1)));
		break;
	}
	if(sws != NULL)
		*sws = (cfgr & RCC_CFGR_SWS) >> 2;
	return sysclockfreq;
}


/*
 *  DMAC������ؿ�
 */
#define NUM_CHDMA       7

#define DMA_TRS_TIMEOUT 2000	/* 2�� */

static const uint32_t dma_base[NUM_CHDMA] = {
	TADR_DMA1_CH1_BASE,
	TADR_DMA1_CH2_BASE,
	TADR_DMA1_CH3_BASE,
	TADR_DMA1_CH4_BASE,
	TADR_DMA1_CH5_BASE,
	TADR_DMA1_CH6_BASE,
	TADR_DMA1_CH7_BASE
};

static DMA_Handle_t *pDmaHandler[NUM_CHDMA];

/*
 *  CHANNEL DMA������ؿ�
 *  parameter1  hdma: DMA�ϥ�ɥ�ؤΥݥ���
 *  return ER��
 */
ER
dma_init(DMA_Handle_t *hdma)
{
	uint32_t tmp = 0;
	uint32_t i, index;

	/* �ѥ�᡼�������å� */
	if(hdma == NULL)
		return E_PAR;

	for(i = 0 ; i < NUM_CHDMA ; i++){
		if(dma_base[i] == hdma->cbase)
			break;
	}
	if(i == NUM_CHDMA)
		return E_PAR;
	hdma->chid = i;
	pDmaHandler[i] = hdma;
	hdma->base = hdma->cbase & ~0x3FF;
	index = i * 4;

	/* DMA�ꥯ���������� */
	sil_modw_mem((uint32_t *)(hdma->base+TOFF_DMA_CSELR), (0xF << index), (hdma->Init.Request << index));

	/* DMA-CCR�쥸�������� */
	tmp = sil_rew_mem((uint32_t *)(hdma->cbase+TOFF_DMACH_CCR));

	/* DMA�⡼�ɥӥåȤ�ƥ�ݥ�ꥣ�˥��ꥢ */
	tmp &= ((uint32_t)~(DMA_CCR_DIR  | DMA_CCR_MEM2MEM | DMA_CCR_PINC  | \
                        DMA_CCR_MINC | DMA_CCR_PSIZE   | DMA_CCR_MSIZE | \
                        DMA_CCR_CIRC | DMA_CCR_PL));

	/* DMA�⡼�ɤ�ƥ�ݥ�ꥣ������ */
	tmp |=   hdma->Init.Direction        |
          hdma->Init.PeriphInc           | hdma->Init.MemInc           |
          hdma->Init.PeriphDataAlignment | hdma->Init.MemDataAlignment |
          hdma->Init.Mode                | hdma->Init.Priority;

	/* DMA�⡼������ */
	sil_wrw_mem((uint32_t *)(hdma->cbase+TOFF_DMACH_CCR), tmp);

	/* ���顼���֤򥯥ꥢ */
	hdma->ErrorCode = DMA_ERROR_NONE;
	return E_OK;
}

/*
 *  CHANNEL DMA��λ�ؿ�
 *  parameter1  hdma: DMA�ϥ�ɥ�ؤΥݥ���
 *  return ER��
 */
ER
dma_deinit(DMA_Handle_t *hdma)
{
	int i, index;

	/* �ѥ�᡼�������å� */
	if(hdma == NULL)
		return E_PAR;

	for(i = 0 ; i < NUM_CHDMA ; i++){
		if(dma_base[i] == hdma->cbase)
			break;
	}
	if(i == NUM_CHDMA)
		return E_PAR;
	hdma->chid = i;
	pDmaHandler[i] = NULL;
	index = i * 4;

	/* DMA���͡��֥� */
	sil_andw_mem((uint32_t *)(hdma->cbase+TOFF_DMACH_CCR), DMA_CCR_EN);
	sil_wrw_mem((uint32_t *)(hdma->cbase+TOFF_DMACH_CCR), 0);
	sil_wrw_mem((uint32_t *)(hdma->cbase+TOFF_DMACH_CNDTR), 0);
	sil_wrw_mem((uint32_t *)(hdma->cbase+TOFF_DMACH_CPAR), 0);
	sil_wrw_mem((uint32_t *)(hdma->cbase+TOFF_DMACH_CMAR), 0);

	/* �ƥե饰�򥯥ꥢ */
	sil_wrw_mem((uint32_t *)(hdma->base+TOFF_DMA_IFCR), (DMA_ISR_GIF << index));
	sil_wrw_mem((uint32_t *)(hdma->base+TOFF_DMA_IFCR), (DMA_IFCR_CTCIF << index));
	sil_wrw_mem((uint32_t *)(hdma->base+TOFF_DMA_IFCR), (DMA_IFCR_CHTIF << index));
	sil_wrw_mem((uint32_t *)(hdma->base+TOFF_DMA_IFCR), (DMA_IFCR_CTEIF << index));

	/* DMA�ꥯ�����ȥ��ꥢ */
	sil_andw_mem((uint32_t *)(hdma->base+TOFF_DMA_CSELR), (0xF << index));

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
	sil_andw_mem((uint32_t *)(hdma->cbase+TOFF_DMACH_CCR), DMA_CCR_EN);
	hdma->status = DMA_STATUS_BUSY;

	/* �ǡ���Ĺ���� */
	sil_wrw_mem((uint32_t *)(hdma->cbase+TOFF_DMACH_CNDTR), DataLength);

	if((hdma->Init.Direction) == DMA_MEMORY_TO_PERIPH){
		/* ���꤫��ǥХ��� */
		sil_wrw_mem((uint32_t *)(hdma->cbase+TOFF_DMACH_CPAR), DstAddress);
		sil_wrw_mem((uint32_t *)(hdma->cbase+TOFF_DMACH_CMAR), SrcAddress);
	}
	else{	/* �ǥХ���������� */
		sil_wrw_mem((uint32_t *)(hdma->cbase+TOFF_DMACH_CPAR), SrcAddress);
		sil_wrw_mem((uint32_t *)(hdma->cbase+TOFF_DMACH_CMAR), DstAddress);
	}

	/* ����ߥ��͡��֥� */
	sil_orw_mem((uint32_t *)(hdma->cbase+TOFF_DMACH_CCR), (DMA_CCR_TCIE | DMA_CCR_HTIE | DMA_CCR_TEIE));

	/* DMA���� */
	sil_orw_mem((uint32_t *)(hdma->cbase+TOFF_DMACH_CCR), DMA_CCR_EN);

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
	sil_andw_mem((uint32_t *)(hdma->cbase+TOFF_DMACH_CCR), DMA_CCR_EN);

	/* DMA����Ԥ� */
	while((sil_rew_mem((uint32_t *)(hdma->cbase+TOFF_DMACH_CCR)) & DMA_CCR_EN) != 0){
	    /* Check for the Timeout */
		if(tick > DMA_TRS_TIMEOUT){
			/* �����ॢ���ȥ��顼���� */
			hdma->ErrorCode |= DMA_ERROR_TIMEOUT;
			return E_TMOUT;
		}
    	dly_tsk(1);
	}
	hdma->status = DMA_ERROR_NONE;
	return E_OK;
}

/*
 *  STREAM DMA����߽����ؿ�
 *  parameter1  hdma: DMA�ϥ�ɥ�ؤΥݥ���
 */
void
dma_inthandler(DMA_Handle_t *hdma)
{
	uint32_t index = hdma->chid * 4;

	/*
	 *  ž�����顼����
	 */
	if((sil_rew_mem((uint32_t *)(hdma->base+TOFF_DMA_ISR)) & (DMA_ISR_TEIF << index)) != 0){
		if((sil_rew_mem((uint32_t *)(hdma->cbase+TOFF_DMACH_CCR)) & DMA_CCR_TEIE) != 0){
			/* ž�����顼����ߥ��ꥢ */
			sil_andw_mem((uint32_t *)(hdma->cbase+TOFF_DMACH_CCR), DMA_CCR_TEIE);
			/* ž�����顼�ե饰�򥯥ꥢ */
			sil_wrw_mem((uint32_t *)(hdma->base+TOFF_DMA_IFCR), (DMA_IFCR_CTEIF << index));
			/* ž�����顼���֤����� */
			hdma->ErrorCode |= DMA_ERROR_TE;
			hdma->status = DMA_STATUS_READY_ERROR;
			/* ���顼������Хå� */
			if(hdma->errorcallback != NULL)
				hdma->errorcallback(hdma);
		}
	}

	/*
	 *  �ϡ���ž����λ����
	 */
	if((sil_rew_mem((uint32_t *)(hdma->base+TOFF_DMA_ISR)) & (DMA_ISR_HTIF << index)) != 0){
		if((sil_rew_mem((uint32_t *)(hdma->cbase+TOFF_DMACH_CCR)) & DMA_CCR_HTIE) != 0){
			if((sil_rew_mem((uint32_t *)(hdma->cbase+TOFF_DMACH_CCR)) & DMA_CCR_CIRC) == 0){
				/* �ϡ���ž������ߤ򥯥ꥢ */
				sil_andw_mem((uint32_t *)(hdma->cbase+TOFF_DMACH_CCR), DMA_CCR_HTIE);
			}
			/* �ϡ���ž����λ�ե饰�򥯥ꥢ */
			sil_wrw_mem((uint32_t *)(hdma->base+TOFF_DMA_IFCR), (DMA_IFCR_CHTIF << index));
			/* �ϡ���ž�����ơ��������ѹ� */
			hdma->status = DMA_STATUS_READY_HMEM0;
			/* �ϡ���ž����λ������Хå� */
			if(hdma->xferhalfcallback != NULL)
				hdma->xferhalfcallback(hdma);
		}
	}

	/*
	 *  ž����λ����
	 */
	if((sil_rew_mem((uint32_t *)(hdma->base+TOFF_DMA_ISR)) & (DMA_ISR_TCIF << index)) != 0){
		if((sil_rew_mem((uint32_t *)(hdma->cbase+TOFF_DMACH_CCR)) & DMA_CCR_TCIE) != 0){
			if((sil_rew_mem((uint32_t *)(hdma->cbase+TOFF_DMACH_CCR)) & DMA_CCR_CIRC) == 0){
				/* ž������ߤ򥯥ꥢ */
				sil_andw_mem((uint32_t *)(hdma->cbase+TOFF_DMACH_CCR), DMA_CCR_TCIE);
			}
			/* ž����λ�ե饰�򥯥ꥢ */
			sil_wrw_mem((uint32_t *)(hdma->base+TOFF_DMA_IFCR), (DMA_IFCR_CTCIF << index));

			/* ���ơ�������λ�� */
			hdma->ErrorCode = DMA_ERROR_NONE;
			hdma->status = DMA_STATUS_READY_MEM0;
			/* ž����λ������Хå� */
			if(hdma->xfercallback != NULL)
				hdma->xfercallback(hdma);
		}
	}
}

/*
 *  STREAM DMA ����ߥ����ӥ��롼����
 */
void
channel_dma_isr(intptr_t exinf)
{
	uint32_t index = (uint32_t)exinf;

	if(index == IRQ_VECTOR_DMA1_CH1){
		if(pDmaHandler[0] != NULL)
			dma_inthandler(pDmaHandler[0]);
	}
	else if(index == IRQ_VECTOR_DMA1_CH_LOW){
		if(pDmaHandler[1] != NULL)
			dma_inthandler(pDmaHandler[1]);
		if(pDmaHandler[2] != NULL)
			dma_inthandler(pDmaHandler[2]);
	}
	else if(index == IRQ_VECTOR_DMA1_CH_HIGH){
		if(pDmaHandler[3] != NULL)
			dma_inthandler(pDmaHandler[3]);
		if(pDmaHandler[4] != NULL)
			dma_inthandler(pDmaHandler[4]);
		if(pDmaHandler[5] != NULL)
			dma_inthandler(pDmaHandler[5]);
		if(pDmaHandler[6] != NULL)
			dma_inthandler(pDmaHandler[6]);
	}
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

#if defined(TOPPERS_STM32LORA_DISCOVERY)
static const gio_conftype led_confdata[] = {
	{ RCC_IOPENR_IOPBEN, TADR_GPIOB_BASE, PINPOSITION5, GPIO_NOPULL},
	{ RCC_IOPENR_IOPAEN, TADR_GPIOA_BASE, PINPOSITION5, GPIO_NOPULL},
	{ RCC_IOPENR_IOPBEN, TADR_GPIOB_BASE, PINPOSITION6, GPIO_NOPULL},
	{ RCC_IOPENR_IOPBEN, TADR_GPIOB_BASE, PINPOSITION7, GPIO_NOPULL}
};
#else
static const gio_conftype led_confdata[] = {
	{ RCC_IOPENR_IOPAEN, TADR_GPIOA_BASE, PINPOSITION5, GPIO_PULLUP}
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
		sil_orw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_IOPENR), pled->clkbit);
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


#if defined(TOPPERS_STM32LORA_DISCOVERY)
#define TADR_PSW_BASE   TADR_GPIOB_BASE
#define PSWGIOPEN       RCC_IOPENR_GPIOBEN
#define	PSW_PIN         PINPOSITION2
#else
#define TADR_PSW_BASE   TADR_GPIOC_BASE
#define PSWGIOPEN       RCC_IOPENR_GPIOCEN
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
	sil_orw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_IOPENR), PSWGIOPEN);
	temp = sil_rew_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_AHBENR));
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
	uint32_t i;
	for(i = 0 ; i < NUM_EXTI0_FUNC ; i++){
		if((istatus & (1<<(i+EXTI0_BASENO))) != 0){
			if(exti0_func[i] != NULL)
				(exti0_func[i])();
		}
	}
    sil_wrw_mem((uint32_t *)(TADR_EXTI_BASE+TOFF_EXTI_PR), istatus);
}

/*
 * EXTI2����ߥϥ�ɥ�
 */
void
exti2_handler(void)
{
	uint32_t istatus = sil_rew_mem((uint32_t *)(TADR_EXTI_BASE+TOFF_EXTI_PR));
	uint32_t i;
	for(i = 0 ; i < NUM_EXTI2_FUNC ; i++){
		if((istatus & (1<<(i+EXTI2_BASENO))) != 0){
			if(exti2_func[i] != NULL)
				(exti2_func[i])();
		}
	}
    sil_wrw_mem((uint32_t *)(TADR_EXTI_BASE+TOFF_EXTI_PR), istatus);
}

/*
 * EXTI4-10����ߥϥ�ɥ�
 */
void
exti4_handler(void)
{
	uint32_t istatus = sil_rew_mem((uint32_t *)(TADR_EXTI_BASE+TOFF_EXTI_PR));
	uint32_t i;
	for(i = 0 ; i < NUM_EXTI4_FUNC ; i++){
		if((istatus & (1<<(i+EXTI4_BASENO))) != 0){
			if(exti4_func[i] != NULL)
				(exti4_func[i])();
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

