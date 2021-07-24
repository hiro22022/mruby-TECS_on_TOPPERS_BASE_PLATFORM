/*
 *  TOPPERS/ASP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Advanced Standard Profile Kernel
 * 
 *  Copyright (C) 2008-2011 by Embedded and Real-Time Systems Laboratory
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
 *  Copyright (C) 2015      by 3rd Designing Center
 *              Imageing System Development Division RICOH COMPANY, LTD.
 *  Copyright (C) 2016-2019 by TOPPERS PROJECT Educational Working Group.
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
 *  @(#) $Id: chip_serial.c 699 2019-07-26 21:59:22Z roi $
 */

/*
 *  ���ꥢ��I/O�ǥХ�����SIO�˥ɥ饤�С�stm32f7xx�ѡ�
 */

#include <kernel.h>
#include <t_syslog.h>
#include "target_stddef.h"
#include "target_serial.h"
#include "target_syssvc.h"

/*
 * �쥸����������
 */
#define PORT2SIOPID(x)	((x) + 1)
#define INDEX_PORT(x)	((x) - 1)
#define GET_SIOPCB(x)	(&siopcb_table[INDEX_PORT(x)])

/*
 *  GPIO�⡼�ɥѥ�᡼��
 */
#define GPIO_MODE_INPUT     0x00000000	/* Input Floating Mode */
#define GPIO_MODE_OUTPUT    0x00000001	/* Output Mode */
#define GPIO_MODE_AF        0x00000002	/* Alternate Function Mode */
#define GPIO_MODE_ANALOG    0x00000003	/* Analog Mode  */

/*
 *  GPIO�����ȥץå�����ѥ�᡼��
 */
#define GPIO_OTYPE_PP       0x0
#define GPIO_OTYPE_OD       0x1

/*
 *  GPIO�����ȥץåȺ�����ȿ��ѥ�᡼��
 */
#define GPIO_SPEED_LOW      0x00000000	/* Low speed     */
#define GPIO_SPEED_MEDIUM   0x00000001	/* Medium speed  */
#define GPIO_SPEED_FAST     0x00000002	/* Fast speed    */
#define GPIO_SPEED_HIGH     0x00000003	/* High speed    */

/*
 *  GPIO�ץ륢�åץ�����ѥ�᡼��
 */
#define GPIO_NOPULL         0x00000000	/* No Pull-up or Pull-down activation  */
#define GPIO_PULLUP         0x00000001	/* Pull-up activation                  */
#define GPIO_PULLDOWN       0x00000002	/* Pull-down activation                */

/*
 *  CLOCK������
 */
#define SOURCE_PCLK2        0
#define SOURCE_SYSCLK       1
#define SOURCE_HSI          2
#define SOURCE_LSE          3

/*
 *  �ӥå�Ĺ�ѥ�᡼��
 */
#define USART_WordLength_8b 0x0000
#define USART_WordLength_9b USART_CR1_M

/*
 *  ���ȥåץӥåȥѥ�᡼��
 */ 
#define USART_StopBits_1    0x0000
#define USART_StopBits_0_5  USART_CR2_STOP_0
#define USART_StopBits_2    USART_CR2_STOP_1
#define USART_StopBits_1_5  USART_CR2_STOP

/*
 *  �ѥ�ƥ��ѥ�᡼��
 */
#define USART_Parity_No     0x0000
#define USART_Parity_Even   USART_CR1_PCE
#define USART_Parity_Odd    (USART_CR1_PCE | USART_CR1_PS) 

/*
 * �ϡ��ɥ������ե�����ѥ�᡼��
 */
#define USART_HardwareFlowControl_None       0x0000
#define USART_HardwareFlowControl_RTS        USART_CR3_RTSE
#define USART_HardwareFlowControl_CTS        USART_CR3_CTSE
#define USART_HardwareFlowControl_RTS_CTS    (USART_CR3_RTSE | USART_CR3_CTSE)

#define ISR_ACK             (USART_ISR_TEACK | USART_ISR_REACK)
#define CR1_CLEAR_MASK      (USART_CR1_M | USART_CR1_PCE | USART_CR1_PS | USART_CR1_TE | USART_CR1_RE | USART_CR1_OVER8)
#define CR3_CLEAR_MASK      (USART_CR3_RTSE | USART_CR3_CTSE | USART_CR3_ONEBIT)

#define UART_TIMEOUT_VALUE               0x1FFFFFF

/*
 *  ���ꥢ��I/O�ݡ��Ƚ�����֥�å������
 */
typedef struct sio_port_initialization_block {
	uint32_t base;
	INTNO    intno_usart;
	uint32_t clock_shift;
} SIOPINIB;

/*
 *  ����GPIO�ݡ��Ƚ�����֥�å������
 */
typedef struct gpio_port_initialization_block {
	uint32_t clockbase;
	uint32_t clock_set;
	uint32_t portbase;
	uint32_t txport_set;
	uint32_t rxport_set;
	uint32_t resetbase;
	uint32_t reset_set;
	uint32_t gpio_af;

	uint32_t txportbase;
	uint32_t txpinport;
	uint32_t txmode_msk;
	uint32_t txmode_set;
	uint32_t txspeed_msk;
	uint32_t txspeed_set;
	uint32_t txtype_msk;
	uint32_t txtype_set;
	uint32_t txpupd_msk;
	uint32_t txpupd_set;

	uint32_t rxportbase;
	uint32_t rxpinport;
	uint32_t rxmode_msk;
	uint32_t rxmode_set;
	uint32_t rxspeed_msk;
	uint32_t rxspeed_set;
	uint32_t rxtype_msk;
	uint32_t rxtype_set;
	uint32_t rxpupd_msk;
	uint32_t rxpupd_set;
} GPIOINIB;

/*
 *  ���ꥢ��I/O�ݡ��ȴ����֥�å������
 */
struct sio_port_control_block {
	const SIOPINIB  *p_siopinib;  /* ���ꥢ��I/O�ݡ��Ƚ�����֥�å� */
	const GPIOINIB  *p_gpioinib;  /* ����GPIO�ݡ��Ƚ�����֥�å� */
	intptr_t        exinf;        /* ��ĥ���� */
	bool_t          opnflg;       /* �����ץ�Ѥߥե饰 */
};

/*
 * ���ꥢ��I/O�ݡ��Ƚ�����֥�å�
 */
const SIOPINIB siopinib_table[TNUM_SIOP] = {
	{(uint32_t)USART1_BASE, (INTNO)INTNO_SIO1, U1_CLOCK_SHIFT}
#if TNUM_SIOP > 1
   ,{(uint32_t)USART2_BASE, (INTNO)INTNO_SIO2, U2_CLOCK_SHIFT}
#endif
};

/*
 * ����GPIO�ݡ��Ƚ�����֥�å�
 */
const GPIOINIB gpioinib_table[TNUM_SIOP] = {
	{(uint32_t)(TADR_RCC_BASE+TOFF_U1_APBNER), (uint32_t)ENABLE_U1_PORT,
     (uint32_t)(TADR_RCC_BASE+TOFF_RCC_AHB1ENR), (uint32_t)ENABLE_U1T_GPIO, (uint32_t)ENABLE_U1R_GPIO,
     (uint32_t)(TADR_RCC_BASE+TOFF_U1_APBRSTR), (uint32_t)RESET_U1_PORT,
	 (uint32_t)GPIO_U1_AF,
     (uint32_t)TADR_U1T_GPIO_BASE, (uint32_t)TX1_PINPOS,
     (uint32_t)~(GPIO_MODER_MODER2 << (TX1_PINPOS*2)), (uint32_t)(GPIO_MODE_AF << (TX1_PINPOS*2)),
     (uint32_t)~(GPIO_OSPEEDER_OSPEEDR2 << (TX1_PINPOS*2)), (uint32_t)(GPIO_SPEED_HIGH << (TX1_PINPOS*2)),
     (uint32_t)~(GPIO_OTYPER_OT << TX1_PINPOS), (uint32_t)(GPIO_OTYPE_PP << TX1_PINPOS),
     (uint32_t)~(GPIO_PUPDR_PUPDR2 << (TX1_PINPOS*2)), (uint32_t)(GPIO_PULLUP << (TX1_PINPOS*2)),
     (uint32_t)TADR_U1R_GPIO_BASE, (uint32_t)RX1_PINPOS,
     (uint32_t)~(GPIO_MODER_MODER2 << (RX1_PINPOS*2)), (uint32_t)(GPIO_MODE_AF << (RX1_PINPOS*2)),
     (uint32_t)~(GPIO_OSPEEDER_OSPEEDR2 << (RX1_PINPOS*2)), (uint32_t)(GPIO_SPEED_HIGH << (RX1_PINPOS*2)),
     (uint32_t)~(GPIO_OTYPER_OT << RX1_PINPOS), (uint32_t)(GPIO_MODE_AF << RX1_PINPOS),
     (uint32_t)~(GPIO_PUPDR_PUPDR2 << (RX1_PINPOS*2)), (uint32_t)(GPIO_PULLUP << (RX1_PINPOS*2))
    }
#if TNUM_SIOP > 1
   ,{(uint32_t)(TADR_RCC_BASE+TOFF_U2_APBNER), (uint32_t)ENABLE_U2_PORT,
     (uint32_t)(TADR_RCC_BASE+TOFF_RCC_AHB1ENR), (uint32_t)ENABLE_U2T_GPIO, (uint32_t)ENABLE_U2R_GPIO,
     (uint32_t)(TADR_RCC_BASE+TOFF_U2_APBRSTR), (uint32_t)RESET_U2_PORT,
	 (uint32_t)GPIO_U2_AF,
     (uint32_t)TADR_U2T_GPIO_BASE, (uint32_t)TX2_PINPOS,
     (uint32_t)~(GPIO_MODER_MODER2 << (TX2_PINPOS*2)), (uint32_t)(GPIO_MODE_AF << (TX2_PINPOS*2)),
     (uint32_t)~(GPIO_OSPEEDER_OSPEEDR2 << (TX2_PINPOS*2)), (uint32_t)(GPIO_SPEED_HIGH << (TX2_PINPOS*2)),
     (uint32_t)~(GPIO_OTYPER_OT << TX2_PINPOS), (uint32_t)(GPIO_OTYPE_PP << TX2_PINPOS),
     (uint32_t)~(GPIO_PUPDR_PUPDR2 << (TX2_PINPOS*2)), (uint32_t)(GPIO_PULLUP << (TX2_PINPOS*2)),
     (uint32_t)TADR_U2R_GPIO_BASE, (uint32_t)RX2_PINPOS,
     (uint32_t)~(GPIO_MODER_MODER2 << (RX2_PINPOS*2)), (uint32_t)(GPIO_MODE_AF << (RX2_PINPOS*2)),
     (uint32_t)~(GPIO_OSPEEDER_OSPEEDR2 << (RX2_PINPOS*2)), (uint32_t)(GPIO_SPEED_HIGH << (RX2_PINPOS*2)),
     (uint32_t)~(GPIO_OTYPER_OT << RX2_PINPOS), (uint32_t)(GPIO_MODE_AF << RX2_PINPOS),
     (uint32_t)~(GPIO_PUPDR_PUPDR2 << (RX2_PINPOS*2)), (uint32_t)(GPIO_PULLUP << (RX2_PINPOS*2))
    }
#endif
};

/*
 *  ���ꥢ��I/O�ݡ��ȴ����֥�å��Υ��ꥢ
 */
SIOPCB	siopcb_table[TNUM_SIOP];

/*
 *  ���ꥢ��I/O�ݡ���ID��������֥�å�����Ф�����Υޥ���
 */
#define INDEX_SIOP(siopid)	((uint_t)((siopid) - 1))
#define get_siopcb(siopid)	(&(siopcb_table[INDEX_SIOP(siopid)]))

static void setup_gpio_source(uint32_t base, uint8_t source, uint8_t select)
{
	uint32_t tmpreg, regoff;
	regoff = TOFF_GPIO_AFR0+((source>>3) * 4);
	tmpreg = (sil_rew_mem((uint32_t *)(base+regoff)) & ~(0xF << ((source & 0x07) * 4)))
			    | (select << ((source & 0x07) * 4));
	sil_wrw_mem((uint32_t *)(base+regoff), tmpreg);
}

void put_hex(char a, int val)
{
	int i, j;
	target_fput_log(a);
	target_fput_log(' ');
	for(i = 28 ; i >= 0 ; i-= 4){
		j = (val >> i) & 0xf;;
		if(j > 9)
			j += ('A'-10);
		else
			j += '0';
		target_fput_log(j);
	}
	target_fput_log('\n');
}

/*
 *  SIO�ɥ饤�Фν����
 */
void
sio_initialize(intptr_t exinf)
{
	SIOPCB	*p_siopcb;
	uint_t	i;

	/*
	 *  ���ꥢ��I/O�ݡ��ȴ����֥�å��ν����
	 */
	for (p_siopcb = siopcb_table, i = 0; i < TNUM_SIOP; p_siopcb++, i++) {
		p_siopcb->p_siopinib = &(siopinib_table[i]);
		p_siopcb->p_gpioinib = &(gpioinib_table[i]);
		p_siopcb->opnflg = false;
	}
}

/*
 *  ���ꥢ��I/O�ݡ��ȤΥ����ץ�
 */
SIOPCB *
sio_opn_por(ID siopid, intptr_t exinf)
{
	SIOPCB          *p_siopcb;
	const SIOPINIB  *p_siopinib;
	const GPIOINIB  *p_gpioinib;
	bool_t   opnflg;
	ER       ercd;
	uint32_t base, txbase, rxbase, tmp;

	p_siopcb = get_siopcb(siopid);
	p_siopinib = p_siopcb->p_siopinib;
	p_gpioinib = p_siopcb->p_gpioinib;

	/*
	 *  �����ץ󤷤��ݡ��Ȥ����뤫��opnflg���ɤ�Ǥ�����
	 */
	opnflg = p_siopcb->opnflg;

	p_siopcb->exinf = exinf;
	txbase = p_gpioinib->txportbase;
	rxbase = p_gpioinib->rxportbase;
	base   = p_siopinib->base;

	if(txbase == 0 || rxbase == 0)	/* no usart port */
		goto sio_opn_exit;

	/*
	 *  USART���
	 */
	sil_wrw_mem((uint32_t *)(base+TOFF_USART_ICR), 0xFFFFFFFF);
	sil_wrw_mem((uint32_t *)(base+TOFF_USART_CR1), sil_rew_mem((uint32_t *)(base+TOFF_USART_CR1)) & ~USART_CR1_UE);

	/*
	 *  �ϡ��ɥ������ν����
	 */
	if(p_siopinib->clock_shift == 0){	/* USART1�ξ�硢SYSLOCK������ */
		sil_wrw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_DCKCFGR2),
			(sil_rew_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_DCKCFGR2)) & RCC_DCKCFGR2_USART1SEL) | RCC_DCKCFGR2_USART1SEL_0);
	}
	else if(p_siopinib->clock_shift == 10 && siopid == 1){	/* F723/USART6�ξ�硢SYSLOCK������ */
		sil_wrw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_DCKCFGR2),
			(sil_rew_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_DCKCFGR2)) & RCC_DCKCFGR2_USART6SEL) | RCC_DCKCFGR2_USART6SEL_0);
	}
	sil_wrw_mem((uint32_t *)p_gpioinib->clockbase, sil_rew_mem((uint32_t *)p_gpioinib->clockbase) | p_gpioinib->clock_set);
	sil_wrw_mem((uint32_t *)p_gpioinib->portbase, sil_rew_mem((uint32_t *)p_gpioinib->portbase) | p_gpioinib->txport_set);
	sil_wrw_mem((uint32_t *)p_gpioinib->portbase, sil_rew_mem((uint32_t *)p_gpioinib->portbase) | p_gpioinib->rxport_set);
	sil_wrw_mem((uint32_t *)(txbase+TOFF_GPIO_MODER), (sil_rew_mem((uint32_t *)(txbase+TOFF_GPIO_MODER)) & p_gpioinib->txmode_msk) | p_gpioinib->txmode_set);
	sil_wrw_mem((uint32_t *)(txbase+TOFF_GPIO_OSPEEDR), (sil_rew_mem((uint32_t *)(txbase+TOFF_GPIO_OSPEEDR)) & p_gpioinib->txspeed_msk) | p_gpioinib->txspeed_set);
	sil_wrw_mem((uint32_t *)(txbase+TOFF_GPIO_OTYPER), (sil_rew_mem((uint32_t *)(txbase+TOFF_GPIO_OTYPER)) & p_gpioinib->txtype_msk) | p_gpioinib->txtype_set);
	sil_wrw_mem((uint32_t *)(txbase+TOFF_GPIO_PUPDR), (sil_rew_mem((uint32_t *)(txbase+TOFF_GPIO_PUPDR)) & p_gpioinib->txpupd_msk) | p_gpioinib->txpupd_set);
	sil_wrw_mem((uint32_t *)(rxbase+TOFF_GPIO_MODER), (sil_rew_mem((uint32_t *)(rxbase+TOFF_GPIO_MODER)) & p_gpioinib->rxmode_msk) | p_gpioinib->rxmode_set);
	sil_wrw_mem((uint32_t *)(rxbase+TOFF_GPIO_OSPEEDR), (sil_rew_mem((uint32_t *)(rxbase+TOFF_GPIO_OSPEEDR)) & p_gpioinib->rxspeed_msk) | p_gpioinib->rxspeed_set);
	sil_wrw_mem((uint32_t *)(rxbase+TOFF_GPIO_OTYPER), (sil_rew_mem((uint32_t *)(rxbase+TOFF_GPIO_OTYPER)) & p_gpioinib->rxtype_msk) | p_gpioinib->rxtype_set);
	sil_wrw_mem((uint32_t *)(rxbase+TOFF_GPIO_PUPDR), (sil_rew_mem((uint32_t *)(txbase+TOFF_GPIO_PUPDR)) & p_gpioinib->rxpupd_msk) | p_gpioinib->rxpupd_set);
	setup_gpio_source(txbase, p_gpioinib->txpinport, p_gpioinib->gpio_af);
	setup_gpio_source(rxbase, p_gpioinib->rxpinport, p_gpioinib->gpio_af);

	tmp = sil_rew_mem((uint32_t *)(base+TOFF_USART_CR2)) & ~USART_CR2_STOP;
	sil_wrw_mem((uint32_t *)(base+TOFF_USART_CR2), tmp | USART_StopBits_1);
	tmp = sil_rew_mem((uint32_t *)(base+TOFF_USART_CR1)) & ~CR1_CLEAR_MASK;
	sil_wrw_mem((uint32_t *)(base+TOFF_USART_CR1), tmp | USART_WordLength_8b | USART_Parity_No | USART_CR1_RE | USART_CR1_TE);
	tmp = sil_rew_mem((uint32_t *)(base+TOFF_USART_CR3)) & ~CR3_CLEAR_MASK;
	sil_wrw_mem((uint32_t *)(base+TOFF_USART_CR3), tmp | USART_HardwareFlowControl_None);

	switch((sil_rew_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_DCKCFGR2)) >> p_siopinib->clock_shift) & 3){
	case SOURCE_HSI:
		tmp = HSI_VALUE;
		break;
	case SOURCE_SYSCLK:
		tmp = SysFreHCLK;
		break;
	case SOURCE_LSE:
		tmp = LSE_VALUE;
		break;
	case SOURCE_PCLK2:
	default:
		if(p_siopinib->clock_shift == 10)	/* USART6,1�ξ�� PCLK2 */
			tmp = SysFrePCLK2;
		else
			tmp = SysFrePCLK1;
		break;
	}
	sil_wrw_mem((uint32_t *)(base+TOFF_USART_BRR), tmp/BPS_SETTING);
	sil_wrw_mem((uint32_t *)(base+TOFF_USART_CR2), sil_rew_mem((uint32_t *)(base+TOFF_USART_CR2)) & ~(USART_CR2_LINEN | USART_CR2_CLKEN));
	sil_wrw_mem((uint32_t *)(base+TOFF_USART_CR3), sil_rew_mem((uint32_t *)(base+TOFF_USART_CR3)) & ~(USART_CR3_SCEN | USART_CR3_HDSEL | USART_CR3_IREN));

	sil_wrw_mem((uint32_t *)(base+TOFF_USART_CR1), sil_rew_mem((uint32_t *)(base+TOFF_USART_CR1)) | USART_CR1_UE);

	/*
	 * RX/TX ACK�����åȤ����ޤ��Ԥ�
	 */
	tmp = 0;
    while((sil_rew_mem((uint32_t *)(base+TOFF_USART_ISR)) & ISR_ACK) != ISR_ACK){
		/* Check for the Timeout */
		if(tmp >=  UART_TIMEOUT_VALUE){
			/* Disable TXE, RXNE, PE and ERR (Frame error, noise error, overrun error) interrupts for the interrupt process */
			sil_wrw_mem((uint32_t *)(base+TOFF_USART_CR1),
				sil_rew_mem((uint32_t *)(base+TOFF_USART_CR1)) & ~(USART_CR1_TXEIE | USART_CR1_RXNEIE | USART_CR1_PEIE));
			sil_wrw_mem((uint32_t *)(base+TOFF_USART_CR3),
				sil_rew_mem((uint32_t *)(base+TOFF_USART_CR3)) & ~USART_CR3_EIE);
			goto sio_opn_exit;
		}
		sil_dly_nse(1000);
		tmp++;
	}

	sil_wrw_mem((uint32_t *)(base+TOFF_USART_CR1), sil_rew_mem((uint32_t *)(base+TOFF_USART_CR1)) | USART_CR1_PEIE);
	sil_wrw_mem((uint32_t *)(base+TOFF_USART_CR3), sil_rew_mem((uint32_t *)(base+TOFF_USART_CR3)) | USART_CR3_EIE);
	sil_wrw_mem((uint32_t *)(base+TOFF_USART_CR1), sil_rew_mem((uint32_t *)(base+TOFF_USART_CR1)) | USART_CR1_RXNEIE);
	p_siopcb->opnflg = true;

	/*
	 *  ���ꥢ��I/O����ߤΥޥ����������롥
	 */
	if (!opnflg) {
		ercd = ena_int(p_siopinib->intno_usart);
		assert(ercd == E_OK);
	}

sio_opn_exit:;
	return(p_siopcb);
}

/*
 *  ���ꥢ��I/O�ݡ��ȤΥ�����
 */
void
sio_cls_por(SIOPCB *p_siopcb)
{
	const SIOPINIB  *p_siopinib;
	const GPIOINIB  *p_gpioinib;
	uint32_t base, tmp;

	/*
	 *  ���ꥢ��I/O����ߤ�ޥ������롥
	 */
	if ((p_siopcb->opnflg)) {
		dis_int(p_siopcb->p_siopinib->intno_usart);
	}

	/*
	 *  �ϡ��ɥ������Υꥻ�å�
	 */
	p_siopinib = p_siopcb->p_siopinib;
	p_gpioinib = p_siopcb->p_gpioinib;
	base = p_siopinib->base;
	sil_wrw_mem((uint32_t *)(base+TOFF_USART_ICR), 0xFFFFFFFF);
	sil_wrw_mem((uint32_t *)(base+TOFF_USART_CR1), sil_rew_mem((uint32_t *)(base+TOFF_USART_CR1)) & ~USART_CR1_UE);
	sil_wrw_mem((uint32_t *)(base+TOFF_USART_CR1), 0x0);
	sil_wrw_mem((uint32_t *)(base+TOFF_USART_CR2), 0x0);
	sil_wrw_mem((uint32_t *)(base+TOFF_USART_CR3), 0x0);
	tmp = sil_rew_mem((uint32_t *)p_gpioinib->resetbase);
	sil_wrw_mem((uint32_t *)p_gpioinib->resetbase, tmp | p_gpioinib->reset_set);
	sil_wrw_mem((uint32_t *)p_gpioinib->resetbase, tmp & ~(p_gpioinib->reset_set));
	p_siopcb->opnflg = false;
}

/*
 *  SIO�γ���ߥ����ӥ��롼����
 */

Inline bool_t
sio_putready(SIOPCB* p_siopcb)
{
	uint32_t cr1 = sil_rew_mem((uint32_t *)(p_siopcb->p_siopinib->base+TOFF_USART_CR1));
	uint32_t isr = sil_rew_mem((uint32_t *)(p_siopcb->p_siopinib->base+TOFF_USART_ISR));

	if ((cr1 & USART_CR1_TXEIE) != 0 && (isr & USART_ISR_TXE) != 0)
	{
		return 1;
	}
	return 0;
}

Inline bool_t
sio_getready(SIOPCB* p_siopcb)
{
	uint32_t base = p_siopcb->p_siopinib->base;
	uint32_t cr1, cr3, isr;

	cr1 = sil_rew_mem((uint32_t *)(base+TOFF_USART_CR1));
	cr3 = sil_rew_mem((uint32_t *)(base+TOFF_USART_CR3));
	isr = sil_rew_mem((uint32_t *)(base+TOFF_USART_ISR));

	if((isr & USART_ISR_PE) != 0 && (cr3 & USART_CR3_EIE) != 0){
		sil_wrw_mem((uint32_t *)(base+TOFF_USART_ICR), USART_ICR_PECF);
	}
	if((isr & USART_ISR_FE) != 0 && (cr3 & USART_CR3_EIE) != 0){
		sil_wrw_mem((uint32_t *)(base+TOFF_USART_ICR), USART_ICR_FECF);
	}
	if((isr & USART_ISR_NE) != 0 && (cr3 & USART_CR3_EIE) != 0){
		sil_wrw_mem((uint32_t *)(base+TOFF_USART_ICR), USART_ICR_NCF);
	}
	if((isr & USART_ISR_ORE) != 0 && (cr3 & USART_CR3_EIE) != 0){
		sil_wrw_mem((uint32_t *)(base+TOFF_USART_ICR), USART_ICR_ORECF);
	}
	if ((cr1 & USART_CR1_RXNEIE) != 0 && (isr & USART_ISR_RXNE) != 0)
	{
		return 1;
	}
	return 0;
}

void
sio_usart_isr(intptr_t exinf)
{
	SIOPCB *p_siopcb = get_siopcb(exinf);
	if (sio_getready(p_siopcb)) {
		sio_irdy_rcv(p_siopcb->exinf);
	}
	if (sio_putready(p_siopcb)) {
		sio_irdy_snd(p_siopcb->exinf);
	}
}

/*
 *  ���ꥢ��I/O�ݡ��Ȥؤ�ʸ������
 */
bool_t
sio_snd_chr(SIOPCB *p_siopcb, char c)
{
	uint32_t base = p_siopcb->p_siopinib->base;

	if (sio_putready(p_siopcb)) {
		sil_wrw_mem((uint32_t *)(base+TOFF_USART_TDR), (uint32_t)c);
		return true;
	}
	return false;
}

/*
 *  ���ꥢ��I/O�ݡ��Ȥ����ʸ������
 */
int_t
sio_rcv_chr(SIOPCB *p_siopcb)
{
	uint32_t base = p_siopcb->p_siopinib->base;
	int_t c = -1;

	if (sio_getready(p_siopcb)) {
		c = sil_rew_mem((uint32_t *)(base+TOFF_USART_RDR)) & 0xFF;
	}
	return c;
}

/*
 *  ���ꥢ��I/O�ݡ��Ȥ���Υ�����Хå��ε���
 */
void
sio_ena_cbr(SIOPCB *p_siopcb, uint_t cbrtn)
{
	uint32_t base = p_siopcb->p_siopinib->base;

	switch (cbrtn) {
	case SIO_RDY_SND:
		sil_wrw_mem((uint32_t *)(base+TOFF_USART_CR1), sil_rew_mem((uint32_t *)(base+TOFF_USART_CR1)) | USART_CR1_TXEIE);
		break;
	case SIO_RDY_RCV:
		sil_wrw_mem((uint32_t *)(base+TOFF_USART_CR1), sil_rew_mem((uint32_t *)(base+TOFF_USART_CR1)) | USART_CR1_RXNEIE);
		break;
	}
}

/*
 *  ���ꥢ��I/O�ݡ��Ȥ���Υ�����Хå��ζػ�
 */
void
sio_dis_cbr(SIOPCB *p_siopcb, uint_t cbrtn)
{
	uint32_t base = p_siopcb->p_siopinib->base;

	switch (cbrtn) {
	case SIO_RDY_SND:
		sil_wrw_mem((uint32_t *)(base+TOFF_USART_CR1), sil_rew_mem((uint32_t *)(base+TOFF_USART_CR1)) & ~USART_CR1_TXEIE);
		break;
	case SIO_RDY_RCV:
		sil_wrw_mem((uint32_t *)(base+TOFF_USART_CR1), sil_rew_mem((uint32_t *)(base+TOFF_USART_CR1)) & ~USART_CR1_RXNEIE);
		break;
	}
}

/*
 *  1ʸ�����ϡʥݡ���󥰤Ǥν��ϡ�
 */
void sio_pol_snd_chr(int8_t c, ID siopid)
{
	uint32_t base = siopinib_table[INDEX_PORT(siopid)].base;

	sil_wrw_mem((uint32_t *)(base+TOFF_USART_TDR), (uint32_t)c);
	while(0 == (sil_rew_mem((uint32_t *)(base+TOFF_USART_ISR)) & USART_ISR_TXE));

	/*
	 *  ���Ϥ������˽����ޤ��Ԥ�
	 */
	volatile int n = 300000000/BPS_SETTING;
	while(n--);
}

/*
 *  �������åȤΥ��ꥢ������
 */
void chip_uart_init(ID siopid)
{
	const GPIOINIB  *p_gpioinib = &gpioinib_table[INDEX_PORT(siopid)];
	const SIOPINIB  *p_siopinib = &siopinib_table[INDEX_PORT(siopid)];
	uint32_t base, txbase, rxbase, tmp;

	txbase = p_gpioinib->txportbase;
	rxbase = p_gpioinib->rxportbase;
	base = p_siopinib->base;

	sil_wrw_mem((uint32_t *)(base+TOFF_USART_CR1), sil_rew_mem((uint32_t *)(base+TOFF_USART_CR1)) & ~USART_CR1_UE);
	sil_wrw_mem((uint32_t *)(base+TOFF_USART_CR1), 0x0);
	sil_wrw_mem((uint32_t *)(base+TOFF_USART_CR2), 0x0);
	sil_wrw_mem((uint32_t *)(base+TOFF_USART_CR3), 0x0);
	tmp = sil_rew_mem((uint32_t *)p_gpioinib->resetbase);
	sil_wrw_mem((uint32_t *)p_gpioinib->resetbase, tmp | p_gpioinib->reset_set);
	sil_wrw_mem((uint32_t *)p_gpioinib->resetbase, tmp & ~(p_gpioinib->reset_set));

	if(p_siopinib->clock_shift == 0){	/* USART1�ξ�硢SYSLOCK������ */
		sil_wrw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_DCKCFGR2),
			(sil_rew_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_DCKCFGR2)) & RCC_DCKCFGR2_USART1SEL) | RCC_DCKCFGR2_USART1SEL_0);
	}
	else if(p_siopinib->clock_shift == 10){	/* F723/USART6�ξ�硢SYSLOCK������ */
		sil_wrw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_DCKCFGR2),
			(sil_rew_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_DCKCFGR2)) & RCC_DCKCFGR2_USART6SEL) | RCC_DCKCFGR2_USART6SEL_0);
	}

	sil_wrw_mem((uint32_t *)p_gpioinib->clockbase, sil_rew_mem((uint32_t *)p_gpioinib->clockbase) | p_gpioinib->clock_set);
	sil_wrw_mem((uint32_t *)p_gpioinib->portbase, sil_rew_mem((uint32_t *)p_gpioinib->portbase) | p_gpioinib->txport_set);
	sil_wrw_mem((uint32_t *)p_gpioinib->portbase, sil_rew_mem((uint32_t *)p_gpioinib->portbase) | p_gpioinib->rxport_set);
	sil_wrw_mem((uint32_t *)(txbase+TOFF_GPIO_MODER), (sil_rew_mem((uint32_t *)(txbase+TOFF_GPIO_MODER)) & p_gpioinib->txmode_msk) | p_gpioinib->txmode_set);
	sil_wrw_mem((uint32_t *)(txbase+TOFF_GPIO_OSPEEDR), (sil_rew_mem((uint32_t *)(txbase+TOFF_GPIO_OSPEEDR)) & p_gpioinib->txspeed_msk) | p_gpioinib->txspeed_set);
	sil_wrw_mem((uint32_t *)(txbase+TOFF_GPIO_OTYPER), (sil_rew_mem((uint32_t *)(txbase+TOFF_GPIO_OTYPER)) & p_gpioinib->txtype_msk) | p_gpioinib->txtype_set);
	sil_wrw_mem((uint32_t *)(txbase+TOFF_GPIO_PUPDR), (sil_rew_mem((uint32_t *)(txbase+TOFF_GPIO_PUPDR)) & p_gpioinib->txpupd_msk) | p_gpioinib->txpupd_set);
	sil_wrw_mem((uint32_t *)(rxbase+TOFF_GPIO_MODER), (sil_rew_mem((uint32_t *)(rxbase+TOFF_GPIO_MODER)) & p_gpioinib->rxmode_msk) | p_gpioinib->rxmode_set);
	sil_wrw_mem((uint32_t *)(rxbase+TOFF_GPIO_OSPEEDR), (sil_rew_mem((uint32_t *)(rxbase+TOFF_GPIO_OSPEEDR)) & p_gpioinib->rxspeed_msk) | p_gpioinib->rxspeed_set);
	sil_wrw_mem((uint32_t *)(rxbase+TOFF_GPIO_OTYPER), (sil_rew_mem((uint32_t *)(rxbase+TOFF_GPIO_OTYPER)) & p_gpioinib->rxtype_msk) | p_gpioinib->rxtype_set);
	sil_wrw_mem((uint32_t *)(rxbase+TOFF_GPIO_PUPDR), (sil_rew_mem((uint32_t *)(txbase+TOFF_GPIO_PUPDR)) & p_gpioinib->rxpupd_msk) | p_gpioinib->rxpupd_set);
	setup_gpio_source(txbase, p_gpioinib->txpinport, p_gpioinib->gpio_af);
	setup_gpio_source(rxbase, p_gpioinib->rxpinport, p_gpioinib->gpio_af);

	tmp = sil_rew_mem((uint32_t *)(base+TOFF_USART_CR2)) & ~USART_CR2_STOP;
	sil_wrw_mem((uint32_t *)(base+TOFF_USART_CR2), tmp | USART_StopBits_1);
	tmp = sil_rew_mem((uint32_t *)(base+TOFF_USART_CR1)) & ~CR1_CLEAR_MASK;
	sil_wrw_mem((uint32_t *)(base+TOFF_USART_CR1), tmp | USART_WordLength_8b | USART_Parity_No | USART_CR1_RE | USART_CR1_TE);
	tmp = sil_rew_mem((uint32_t *)(base+TOFF_USART_CR3)) & ~CR3_CLEAR_MASK;
	sil_wrw_mem((uint32_t *)(base+TOFF_USART_CR3), tmp | USART_HardwareFlowControl_None);

	switch((sil_rew_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_DCKCFGR2)) >> p_siopinib->clock_shift) & 3){
	case SOURCE_HSI:
		tmp = HSI_VALUE;
		break;
	case SOURCE_SYSCLK:
		tmp = SysFreHCLK;
		break;
	case SOURCE_LSE:
		tmp = LSE_VALUE;
		break;
	case SOURCE_PCLK2:
	default:
		if(p_siopinib->clock_shift == 10)	/* USART6,1�ξ�� PCLK2 */
			tmp = SysFrePCLK2;
		else
			tmp = SysFrePCLK1;
		break;
	}
	sil_wrw_mem((uint32_t *)(base+TOFF_USART_BRR), tmp/BPS_SETTING);
	sil_wrw_mem((uint32_t *)(base+TOFF_USART_CR1), sil_rew_mem((uint32_t *)(base+TOFF_USART_CR1)) | USART_CR1_UE);
}

