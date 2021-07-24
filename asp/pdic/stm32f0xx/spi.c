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
 *  @(#) $Id: spi.c 698 2017-06-29 07:49:53Z roi $
 */
/*
 * 
 *  SPI�ɥ饤�дؿ���
 *
 */
#include "kernel_impl.h"
#include <t_syslog.h>
#include <t_stdlib.h>
#include <string.h>
#include <sil.h>
#include <target_syssvc.h>
#include "device.h"
#include "spi.h"

#define sil_orw_mem(a, b)		sil_wrw_mem((a), sil_rew_mem(a) | (b))
#define sil_andw_mem(a, b)		sil_wrw_mem((a), sil_rew_mem(a) & ~(b))
#define sil_modw_mem(a, b, c)	sil_wrw_mem((a), (sil_rew_mem(a) & (~b)) | (c))

#ifndef SPI_GPIO_PP
#define SPI_GPIO_PP     GPIO_PULLDOWN
#endif
#ifndef SPI_GPIO_SPEED
#define SPI_GPIO_SPEED  GPIO_SPEED_HIGH
#endif

/*
 *  SPIO�ݡ���ID��������֥�å�����Ф�����Υޥ���
 */
#define INDEX_SPI(spiid)        ((uint_t)((spiid) - 1))

#define GPIO_AF0_SPI1          ((uint8_t)0x00)  /* SPI1 Alternate Function mapping        */
#define GPIO_AF0_SPI2          ((uint8_t)0x00)  /* SPI2 Alternate Function mapping        */

#define SPI_TIMEOUT_VALUE  (10*1000)

static const SPI_PortControlBlock spi_pcb[NUM_SPIPORT] = {
  {	TADR_SPI1_BASE,
	(TADR_RCC_BASE+TOFF_RCC_AHBENR), RCC_AHBENR_GPIOAEN, RCC_AHBENR_GPIOAEN,
	(TADR_RCC_BASE+TOFF_RCC_APB2ENR), RCC_APB2ENR_SPI1EN,
	(TADR_RCC_BASE+TOFF_RCC_AHBENR), RCC_AHBENR_DMA1EN,
	TADR_GPIOA_BASE, TADR_GPIOA_BASE, TADR_GPIOA_BASE,
	5, 6, 7, GPIO_AF0_SPI1,
	TADR_DMA1_CH3_BASE, DMA_REQUEST_0,
	TADR_DMA1_CH2_BASE, DMA_REQUEST_0 },

  {	TADR_SPI2_BASE,
	(TADR_RCC_BASE+TOFF_RCC_AHBENR), RCC_AHBENR_GPIOBEN, RCC_AHBENR_GPIOBEN,
	(TADR_RCC_BASE+TOFF_RCC_APB1ENR), RCC_APB1ENR_SPI2EN,
	(TADR_RCC_BASE+TOFF_RCC_AHBENR), RCC_AHBENR_DMA1EN,
	TADR_GPIOB_BASE, TADR_GPIOB_BASE, TADR_GPIOB_BASE,
	13, 14, 15, GPIO_AF0_SPI2,
	TADR_DMA1_CH5_BASE, DMA_REQUEST_0,
	TADR_DMA1_CH4_BASE, DMA_REQUEST_0 },
};

static SPI_Handle_t SpiHandle[NUM_SPIPORT];
static DMA_Handle_t hdma_tx[NUM_SPIPORT];
static DMA_Handle_t hdma_rx[NUM_SPIPORT];


/*
 *  SPI�����Ѳ��Ԥ��ؿ�
 *  parameter1  hspi: SPI�ϥ�ɥ�ؤΥݥ���
 *  parameter2  Flag: �����å���Ԥ�SPI�ե饰
 *  parameter3  Status: �Ԥ�����
 *  parameter4  Timeout: �����ॢ���Ȼ���(�ޥ�������)
 *  return ER������
 */
static ER
spi_waitflag(SPI_Handle_t *hspi, uint32_t Flag, bool_t Status, uint32_t Timeout)
{
  uint32_t tick = 0;

	while(((sil_rew_mem((uint32_t *)(hspi->base+TOFF_SPI_SR)) & Flag) == Flag) == Status){
		if(Timeout == 0 || tick > Timeout){
			/*
			 *  �������ȥ��顼�ե饰��ꥻ�å�
			 */
			sil_andw_mem((uint32_t *)(hspi->base+TOFF_SPI_CR2), (SPI_CR2_TXEIE | SPI_CR2_RXNEIE | SPI_CR2_ERRIE));

			/*
			 *  SPI�����
			 */
			sil_andw_mem((uint32_t *)(hspi->base+TOFF_SPI_CR1), SPI_CR1_SPE);

			/*
			 *  CRC�黻��̵����
			 */
			if(hspi->Init.CRC == SPI_CRC_ENABLE){
				sil_andw_mem((uint32_t *)(hspi->base+TOFF_SPI_CR1), SPI_CR1_CRCEN);
				sil_orw_mem((uint32_t *)(hspi->base+TOFF_SPI_CR1), SPI_CR1_CRCEN);
			}

			hspi->status= SPI_STATUS_READY;
			return E_TMOUT;
		}
		tick++;
		sil_dly_nse(1000);
	}
	return E_OK;
}

/*
 *  SPI DMA����������Хå��ؿ�
 */
static void
spi_dmatransmit_func(DMA_Handle_t *hdma)
{
	SPI_Handle_t* hspi = ( SPI_Handle_t* )hdma->localdata;
	volatile uint32_t tmp;

	if((sil_rew_mem((uint32_t *)(hdma->cbase+TOFF_DMACH_CCR)) & DMA_CCR_CIRC) == 0){
		/*
		 *  TXE�ե饰���å��Ԥ�
		 */
		if(spi_waitflag(hspi, SPI_SR_TXE, false, SPI_TIMEOUT_VALUE) != E_OK){
			hspi->ErrorCode |= SPI_ERROR_TIMEOUT;
		}

		/*
		 *  TX DMA���
		 */
		sil_andw_mem((uint32_t *)(hspi->base+TOFF_SPI_CR2), SPI_CR2_TXDMAEN);

		/*
		 *  SPI BUSY�ꥻ�å��Ԥ�
		 */
		if(spi_waitflag(hspi, SPI_SR_BSY, true, SPI_TIMEOUT_VALUE) != E_OK){
			hspi->ErrorCode |= SPI_ERROR_TIMEOUT;
		}

		hspi->status = SPI_STATUS_READY;
	}

	/*
	 *  2�饤��ž���ǥ����С����ȯ�����Ƥ���ʤ�ꥻ�å�
	 */
	if(hspi->Init.Direction == SPI_DIRECTION_2LINES){
		tmp = sil_rew_mem((uint32_t *)(hspi->base+TOFF_SPI_DR));
		sil_andw_mem((uint32_t *)(hspi->base+TOFF_SPI_SR), SPI_SR_OVR);
		((void)(tmp));
	}
	hspi->TxXferCount += hspi->TxXferSize;

	if(hspi->Init.semid != 0)
		isig_sem(hspi->Init.semid);
}

/*
 *  SPI DMA����������Хå��ؿ�
 */
static void
spi_dmareceive_func(DMA_Handle_t *hdma)
{
	SPI_Handle_t *hspi = (SPI_Handle_t *)hdma->localdata;
	volatile uint32_t tmp;

	if((sil_rew_mem((uint32_t *)(hdma->cbase+TOFF_DMACH_CCR)) & DMA_CCR_CIRC) == 0){
		/*
		 *  RX DMA���
		 */
		sil_andw_mem((uint32_t *)(hspi->base+TOFF_SPI_CR2), (SPI_CR2_RXDMAEN));

		/*
		 *  TX DMA���
		 */
		sil_andw_mem((uint32_t *)(hspi->base+TOFF_SPI_CR2), (SPI_CR2_TXDMAEN));

		/*
		 *  CRC�黻
		 */
		if(hspi->Init.CRC == SPI_CRC_ENABLE){
			/*
			 *  ������λ�Ԥ�
			 */
			if(spi_waitflag(hspi, SPI_SR_RXNE, false, SPI_TIMEOUT_VALUE) != E_OK){
				hspi->ErrorCode |= SPI_ERROR_TIMEOUT;
			}

			/*
			 *  CRC�ɤ߼Τ�
			 */
			tmp = sil_rew_mem((uint32_t *)(hspi->base+TOFF_SPI_DR));
			((void)(tmp));

			/*
			 *  RXNE�ꥻ�å��Ԥ�
			 */
			if(spi_waitflag(hspi, SPI_SR_RXNE, true, SPI_TIMEOUT_VALUE) != E_OK){
				hspi->ErrorCode |= SPI_ERROR_TIMEOUT;
			}

			/*
			 *  CRC���顼�����å�
			 */
			if((sil_rew_mem((uint32_t *)(hspi->base+TOFF_SPI_SR)) & SPI_SR_CRCERR) != 0){
				hspi->ErrorCode |= SPI_ERROR_CRC;
				sil_andw_mem((uint32_t *)(hspi->base+TOFF_SPI_SR), SPI_SR_CRCERR);
			}
		}
		if((hspi->Init.Mode == SPI_MODE_MASTER) &&
			(hspi->Init.Direction == SPI_DIRECTION_1LINE || hspi->Init.Direction == SPI_DIRECTION_2LINES_RXONLY)){
			/*
			 *  SPI�����
			 */
			sil_andw_mem((uint32_t *)(hspi->base+TOFF_SPI_CR1), SPI_CR1_SPE);
		}
		hspi->status = SPI_STATUS_READY;
	}
	hspi->RxXferCount += hspi->RxXferSize;

	if(hspi->Init.semid != 0)
		isig_sem(hspi->Init.semid);
}

/*
 *  SPI DMA������������Хå��ؿ�
 */
static void
spi_dmatransrecv_func(DMA_Handle_t *hdma)
{
	SPI_Handle_t *hspi = (SPI_Handle_t *)hdma->localdata;
	volatile uint32_t tmp;

	if((sil_rew_mem((uint32_t *)(hdma->cbase+TOFF_DMACH_CCR)) & DMA_CCR_CIRC) == 0){
		/*
		 *  CRC�黻
		 */
		if(hspi->Init.CRC == SPI_CRC_ENABLE){
			/*
			 *  ������λ�Ԥ�
			 */
			if(spi_waitflag(hspi, SPI_SR_RXNE, false, SPI_TIMEOUT_VALUE) != E_OK){
				hspi->ErrorCode |= SPI_ERROR_TIMEOUT;
			}
			/*
			 *  CRC�ɤ߼Τ�
			 */
	 		tmp = sil_rew_mem((uint32_t *)(hspi->base+TOFF_SPI_DR));
			((void)(tmp));

			/*
			 *  CRC���顼�����å�
			 */
			if((sil_rew_mem((uint32_t *)(hspi->base+TOFF_SPI_SR)) & SPI_SR_CRCERR) != 0){
				hspi->ErrorCode |= SPI_ERROR_CRC;
				sil_andw_mem((uint32_t *)(hspi->base+TOFF_SPI_SR), SPI_SR_CRCERR);
			}
		}
		/*
		 *  TXE�ե饰���å��Ԥ�
		 */
		if(spi_waitflag(hspi, SPI_SR_TXE, false, SPI_TIMEOUT_VALUE) != E_OK){
			hspi->ErrorCode |= SPI_ERROR_TIMEOUT;
		}

		/*
		 *  TX DMA���
		 */
		sil_andw_mem((uint32_t *)(hspi->base+TOFF_SPI_CR2), (SPI_CR2_TXDMAEN));

		/*
		 *  SPI BUSY��λ�Ԥ�
		 */
		if(spi_waitflag(hspi, SPI_SR_BSY, true, SPI_TIMEOUT_VALUE) != E_OK){
			hspi->ErrorCode |= SPI_ERROR_TIMEOUT;
		}

		/*
		 *  RX DMA���
		 */
		sil_andw_mem((uint32_t *)(hspi->base+TOFF_SPI_CR2), (SPI_CR2_RXDMAEN));

		hspi->status = SPI_STATUS_READY;
	}
	hspi->TxXferCount += hspi->TxXferSize;
	hspi->RxXferCount += hspi->RxXferSize;

	if(hspi->Init.semid != 0)
		isig_sem(hspi->Init.semid);
}

/*
 *  SPI����ž����λ�Ԥ�
 */
ER
spi_inwait(SPI_Handle_t *hspi, uint32_t timeout)
{
	ER ercd = E_OK;
	int tick = timeout;

	while((hspi->status & SPI_STATUS_BUSY) != 0 && tick > 0){
		if(hspi->Init.semid != 0){
	 		ercd = twai_sem(hspi->Init.semid, 50);
		}
		else
			dly_tsk(1);
		tick--;
	}
	dma_end(hspi->hdmarx);
	dma_end(hspi->hdmatx);
	if(hspi->ErrorCode != 0)
		ercd = E_OBJ;
	else if(tick == 0)
		ercd = E_TMOUT;
	return ercd;
}

/*
 *  SPI DMA���顼������Хå��ؿ�
 */
static
void spi_dmaerror_func(DMA_Handle_t *hdma)
{
	SPI_Handle_t *hspi = (SPI_Handle_t *)hdma->localdata;

	hspi->TxXferCount = 0;
	hspi->RxXferCount = 0;
	hspi->status= SPI_STATUS_READY;
	hspi->ErrorCode |= SPI_ERROR_DMA;
	syslog_2(LOG_ERROR, "SPI DMA Error handle[%08x] ErrorCode[%08x] !", hdma, hdma->ErrorCode);
	sil_dly_nse(1000*500);	/* ���顼���ϥ󥰥��åײ����0.5msec�Ԥ� */
}

/*
 *  SPI�������
 *  parameter1  port: SPI�ݡ����ֹ�
 *  parameter2  spii: SPI������깽¤�ΤؤΥݥ���
 *  return SPI�ϥ�ɥ�ؤΥݥ��󥿡�NULL�ǥ��顼
 */
SPI_Handle_t *
spi_init(ID port, SPI_Init_t *spii)
{
	GPIO_Init_t GPIO_Init_Data;
	SPI_Handle_t *hspi;
	const SPI_PortControlBlock *spcb;
	DMA_Handle_t *hdma;
	uint32_t no, frxth;
	volatile uint32_t tmpreg;

	if(port < SPI1_PORTID || port > NUM_SPIPORT)
		return NULL;
	if(spii == NULL)
		return NULL;
	spii->NSSPMode &= SPI_CR2_NSSP;

	no = INDEX_SPI(port);
	hspi = &SpiHandle[no];
	if(hspi->status != SPI_STATUS_RESET)
		return NULL;
	spcb = &spi_pcb[no];
	memcpy(&hspi->Init, spii, sizeof(SPI_Init_t));
	hspi->base = spcb->base;

	/*
	 *  SPI ����å�����
	 */
	sil_orw_mem((uint32_t *)spcb->gioclockbase, spcb->gioclockbit);
	tmpreg = sil_rew_mem((uint32_t *)spcb->gioclockbase);
	sil_orw_mem((uint32_t *)spcb->gioclockbase, spcb->gioclockbit2);
	tmpreg = sil_rew_mem((uint32_t *)spcb->gioclockbase);
	sil_orw_mem((uint32_t *)spcb->spiclockbase, spcb->spiclockbit);
	tmpreg = sil_rew_mem((uint32_t *)spcb->spiclockbase);
	sil_orw_mem((uint32_t *)spcb->dmaclockbase, spcb->dmaclockbit);
	tmpreg = sil_rew_mem((uint32_t *)spcb->dmaclockbase);
    ((void)(tmpreg));
  
	/*
	 *  SPI GPIO�ԥ�����
	 */
	GPIO_Init_Data.mode      = GPIO_MODE_AF;
	GPIO_Init_Data.pull      = SPI_GPIO_PP;
	GPIO_Init_Data.otype     = GPIO_OTYPE_PP;
	GPIO_Init_Data.speed     = SPI_GPIO_SPEED;
	GPIO_Init_Data.alternate = spcb->apvalue;
	gpio_setup(spcb->giobase1, &GPIO_Init_Data, spcb->sckpin);
	gpio_setup(spcb->giobase2, &GPIO_Init_Data, spcb->misopin);
	gpio_setup(spcb->giobase3, &GPIO_Init_Data, spcb->mosipin);

	/*
	 *  SPI�Ѽ���DMP����
	 */
	hdma = &hdma_tx[no];
	hdma->cbase                    = spcb->dmatxchannel;
	hdma->Init.Request             = spcb->dmatxrequest;
	hdma->Init.Direction           = DMA_MEMORY_TO_PERIPH;
	hdma->Init.PeriphInc           = DMA_PINC_DISABLE;
	hdma->Init.MemInc              = DMA_MINC_ENABLE;
	if(spii->DataSize > SPI_DATASIZE_8BIT){
		hdma->Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
		hdma->Init.MemDataAlignment    = DMA_MDATAALIGN_HALFWORD;
	}
	else{
		hdma->Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
		hdma->Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
	}
	hdma->Init.Mode                = DMA_NORMAL;
	hdma->Init.Priority            = DMA_PRIORITY_LOW;

	dma_init(hdma);
	hspi->hdmatx = hdma;
	hdma->localdata                = hspi;

	/*
	 *  SPI�Ѽ���DMP����
	 */
	hdma = &hdma_rx[no];
	hdma->cbase                    = spcb->dmarxchannel;
	hdma->Init.Request             = spcb->dmarxrequest;
	hdma->Init.Direction           = DMA_PERIPH_TO_MEMORY;
	hdma->Init.PeriphInc           = DMA_PINC_DISABLE;
	hdma->Init.MemInc              = DMA_MINC_ENABLE;
	if(spii->DataSize > SPI_DATASIZE_8BIT){
		hdma->Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
		hdma->Init.MemDataAlignment    = DMA_MDATAALIGN_HALFWORD;
	}
	else{
		hdma->Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
		hdma->Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
	}
	hdma->Init.Mode                = DMA_NORMAL;
	hdma->Init.Priority            = DMA_PRIORITY_HIGH;

	dma_init(hdma);
	hspi->hdmarx = hdma;
	hdma->localdata                = hspi;

	/*
	 *  SPI�ǥ������֥�
	 */
	sil_andw_mem((uint32_t *)(hspi->base+TOFF_SPI_CR1), SPI_CR1_SPE);

	/*
	 *  FIFO����å���ۡ��������
	 */
	if(hspi->Init.DataSize > SPI_DATASIZE_8BIT)
		frxth = 0x00000000;
	else
		frxth = SPI_CR2_FRXTH;

	/*
	 *  CRCĹ�κ�����
	 */
	if(hspi->Init.DataSize != SPI_DATASIZE_16BIT && hspi->Init.DataSize != SPI_DATASIZE_8BIT)
		hspi->Init.CRC = SPI_CRC_DISABLE;
	if(hspi->Init.CRCLength == SPI_CRC_LENGTH_DATASIZE){
		if(hspi->Init.DataSize > SPI_DATASIZE_8BIT)
			hspi->Init.CRCLength = SPI_CRC_LENGTH_16BIT;
		else
			hspi->Init.CRCLength = SPI_CRC_LENGTH_8BIT;
	}

	/*
	 *  SPI CR1/CR2�쥸��������
	 */
	sil_wrw_mem((uint32_t *)(hspi->base+TOFF_SPI_CR1), (hspi->Init.Mode | hspi->Init.Direction |
							hspi->Init.CLKPolarity | hspi->Init.CLKPhase | (hspi->Init.NSS & SPI_CR1_SSM) |
							hspi->Init.Prescaler | hspi->Init.SignBit | hspi->Init.CRC));

	/*
	 *  SPI NSS����
	 */
	sil_wrw_mem((uint32_t *)(hspi->base+TOFF_SPI_CR2), (((hspi->Init.NSS >> 16) & SPI_CR2_SSOE) | hspi->Init.TIMode |
							hspi->Init.NSSPMode | hspi->Init.DataSize | frxth));

	/*
	 *  SPI CRC¿�༰����
	 */
	sil_wrw_mem((uint32_t *)(hspi->base+TOFF_SPI_CRCPR), hspi->Init.CRCPolynomial);

	/*
	 *  SPI I2CS�⡼�ɤ�ꥻ�å�
	 */
	sil_andw_mem((uint32_t *)(hspi->base+TOFF_SPI_I2SCFGR), SPI_I2SCFGR_I2SMOD);

	hspi->ErrorCode = SPI_ERROR_NONE;
	hspi->status = SPI_STATUS_READY;
	return hspi;
}

/*
 *  SPI��λ����
 *  parameter1  hspi: SPI�ϥ�ɥ�ؤΥݥ���
 *  return ER������
 */
ER
spi_deinit(SPI_Handle_t *hspi)
{
	if(hspi == NULL)
		return E_PAR;

	dma_deinit(hspi->hdmatx);
	dma_deinit(hspi->hdmarx);

	sil_andw_mem((uint32_t *)(hspi->base+TOFF_SPI_CR1), SPI_CR1_SPE);

	hspi->ErrorCode = SPI_ERROR_NONE;
	hspi->status = SPI_STATUS_RESET;
	return E_OK;
}

/*
 *  SPI�⥸�塼��Υꥻ�å�
 *  parameter1  hspi: SPI�ϥ�ɥ�ؤΥݥ���
 *  return ER������
 */
ER
spi_reset(SPI_Handle_t *hspi)
{
	uint32_t frxth;

	if(hspi == NULL)
		return E_PAR;
	/*
	 *  SPI�ǥ������֥�
	 */
	sil_andw_mem((uint32_t *)(hspi->base+TOFF_SPI_CR1), SPI_CR1_SPE);

	/*
	 *  FIFO����å���ۡ��������
	 */
	if(hspi->Init.DataSize > SPI_DATASIZE_8BIT)
		frxth = 0x00000000;
	else
		frxth = SPI_CR2_FRXTH;

	/*
	 *  SPI CR1/CR2�쥸��������
	 */
	sil_wrw_mem((uint32_t *)(hspi->base+TOFF_SPI_CR1), (hspi->Init.Mode | hspi->Init.Direction |
							hspi->Init.CLKPolarity | hspi->Init.CLKPhase | (hspi->Init.NSS & SPI_CR1_SSM) |
							hspi->Init.Prescaler | hspi->Init.SignBit | hspi->Init.CRC));

	/*
	 *  SPI NSS����
	 */
	sil_wrw_mem((uint32_t *)(hspi->base+TOFF_SPI_CR2), (((hspi->Init.NSS >> 16) & SPI_CR2_SSOE) | hspi->Init.TIMode |
							hspi->Init.NSSPMode | hspi->Init.DataSize | frxth));

	/*
	 *  SPI CRC¿�༰����
	 */
	sil_wrw_mem((uint32_t *)(hspi->base+TOFF_SPI_CRCPR), hspi->Init.CRCPolynomial);

	/*
	 *  SPI I2CS�⡼�ɤ�ꥻ�å�
	 */
	sil_andw_mem((uint32_t *)(hspi->base+TOFF_SPI_I2SCFGR), SPI_I2SCFGR_I2SMOD);

	hspi->ErrorCode = SPI_ERROR_NONE;
	hspi->status = SPI_STATUS_READY;
	return E_OK;
}

/*
 *  SPI�����¹Դؿ�
 *  parameter1  hspi: SPI�ϥ�ɥ�ؤΥݥ���
 *  parameter2  pdata: �����Хåե��ؤΥݥ���
 *  parameter3  length: ����������
 *  return ER������
 */
ER
spi_transmit(SPI_Handle_t *hspi, uint8_t *pdata, uint16_t length)
{
	ER ercd = E_OK;

	if(hspi == NULL || pdata == NULL || length == 0)
		return E_PAR;

	if(hspi->Init.semlock != 0)
		wai_sem(hspi->Init.semlock);
	if(hspi->status != SPI_STATUS_READY){
		if(hspi->Init.semlock != 0)
			sig_sem(hspi->Init.semlock);
		return E_OBJ;
	}

	/* Configure communication */
	hspi->xmode       = SPI_XMODE_TX;
	hspi->status      = SPI_STATUS_BUSY;
	hspi->ErrorCode   = SPI_ERROR_NONE;

	hspi->pTxBuffPtr  = pdata;
	hspi->TxXferSize  = length;
	hspi->TxXferCount = 0;
	hspi->RxXferSize   = 0;
	hspi->RxXferCount  = 0;

    /*
	 *  1�饤���̿�����
	 */
	if(hspi->Init.Direction == SPI_DIRECTION_1LINE){
		sil_orw_mem((uint32_t *)(hspi->base+TOFF_SPI_CR1), SPI_CR1_BIDIOE);
	}

	/*
	 *  CRC�⡼�ɤʤ������ͭ����
	 */
	if(hspi->Init.CRC == SPI_CRC_ENABLE){
		sil_andw_mem((uint32_t *)(hspi->base+TOFF_SPI_CR1), SPI_CR1_CRCEN);
		sil_orw_mem((uint32_t *)(hspi->base+TOFF_SPI_CR1), SPI_CR1_CRCEN);
	}

	/*
	 *  LAST DMA����
	 */
	sil_andw_mem((uint32_t *)(hspi->base+TOFF_SPI_CR2), SPI_CR2_LDMATX);
	if(hspi->Init.DataSize <= SPI_DATASIZE_8BIT && hspi->hdmatx->Init.MemDataAlignment == DMA_MDATAALIGN_HALFWORD){
	    /* Check the even/odd of the data size + crc if enabled */
		if((hspi->TxXferCount & 0x1) == 0){
			sil_andw_mem((uint32_t *)(hspi->base+TOFF_SPI_CR2), SPI_CR2_LDMATX);
			hspi->TxXferCount = (hspi->TxXferCount >> 1);
		}
		else{
			sil_orw_mem((uint32_t *)(hspi->base+TOFF_SPI_CR2), SPI_CR2_LDMATX);
			hspi->TxXferCount = (hspi->TxXferCount >> 1) + 1;
		}
	}

	/*
	 *  ����DMA����ȥ�������
	 */
	hspi->hdmatx->xferhalfcallback = NULL;
	hspi->hdmatx->xfercallback = spi_dmatransmit_func;
	hspi->hdmatx->errorcallback = spi_dmaerror_func;

	/*
	 *  ����DMA����ȥ�������
	 */
	dma_start(hspi->hdmatx, (uint32_t)hspi->pTxBuffPtr, (uint32_t)(hspi->base+TOFF_SPI_DR), hspi->TxXferSize);

	/*
	 *  SPI����DMA����
	 */
	sil_orw_mem((uint32_t *)(hspi->base+TOFF_SPI_CR2), SPI_CR2_TXDMAEN);

	/*  SPIͭ���� */
	if((sil_rew_mem((uint32_t *)(hspi->base+TOFF_SPI_CR1)) & SPI_CR1_SPE) == 0)
		sil_orw_mem((uint32_t *)(hspi->base+TOFF_SPI_CR1), SPI_CR1_SPE);

	/*
	 *  ���顼���������
	 */
	sil_orw_mem((uint32_t *)(hspi->base+TOFF_SPI_CR2), SPI_CR2_ERRIE);

#if SPI_WAIT_TIME != 0
	ercd = spi_inwait(hspi, SPI_WAIT_TIME * length);

	if(hspi->Init.semlock != 0)
		sig_sem(hspi->Init.semlock);
#endif
	return ercd;
}

/*
 *  SPI�����¹Դؿ�
 *  parameter1  hspi: SPI�ϥ�ɥ�ؤΥݥ���
 *  parameter2  pdata: �����Хåե��ؤΥݥ���
 *  parameter3  length: ����������
 *  return ER������
 */
ER
spi_receive(SPI_Handle_t *hspi, uint8_t *pdata, uint16_t length)
{
	ER ercd = E_OK;

	if(hspi == NULL || pdata == NULL || length == 0)
		return E_PAR;

	if(hspi->Init.semlock != 0)
		wai_sem(hspi->Init.semlock);
	if(hspi->status != SPI_STATUS_READY){
		if(hspi->Init.semlock != 0)
			sig_sem(hspi->Init.semlock);
		return E_OBJ;
	}

    /*
	 *  ž�����������
	 */
	hspi->xmode       = SPI_XMODE_RX;
    hspi->status      = SPI_STATUS_BUSY;
    hspi->ErrorCode   = SPI_ERROR_NONE;
    hspi->pRxBuffPtr  = pdata;
    hspi->RxXferSize  = length;
    hspi->RxXferCount = 0;
    hspi->TxXferSize   = 0;
    hspi->TxXferCount  = 0;

    /*
	 *  1�饤���̿�����
	 */
    if(hspi->Init.Direction == SPI_DIRECTION_1LINE){
		sil_andw_mem((uint32_t *)(hspi->base+TOFF_SPI_CR1), SPI_CR1_BIDIOE);
    }
    else if(hspi->Init.Direction == SPI_DIRECTION_2LINES){
	    hspi->status = SPI_STATUS_READY;
		if(hspi->Init.semlock != 0)
			sig_sem(hspi->Init.semlock);
		if(hspi->Init.Mode == SPI_MODE_MASTER){
			/*
			 *  ��������Ԥ�
			 */
			memset(pdata, 0xff, length);
			return spi_transrecv(hspi, pdata, pdata, length);
		}
		else
			return E_PAR;
    }

	/*
	 *  CRC�⡼�ɤʤ������ͭ����
	 */
    if(hspi->Init.CRC == SPI_CRC_ENABLE){
      sil_andw_mem((uint32_t *)(hspi->base+TOFF_SPI_CR1), SPI_CR1_CRCEN);
      sil_orw_mem((uint32_t *)(hspi->base+TOFF_SPI_CR1), SPI_CR1_CRCEN);
    }

	/*
	 *  ����å���ۡ��������
	 */
	sil_andw_mem((uint32_t *)(hspi->base+TOFF_SPI_CR2), SPI_CR2_LDMARX);
	if( hspi->Init.DataSize > SPI_DATASIZE_8BIT){
		sil_andw_mem((uint32_t *)(hspi->base+TOFF_SPI_CR2), SPI_CR2_FRXTH);
	}
	else{
		sil_orw_mem((uint32_t *)(hspi->base+TOFF_SPI_CR2), SPI_CR2_FRXTH);
		if(hspi->hdmarx->Init.MemDataAlignment == DMA_MDATAALIGN_HALFWORD){
			sil_andw_mem((uint32_t *)(hspi->base+TOFF_SPI_CR2), SPI_CR2_FRXTH);
			if((hspi->RxXferCount & 0x1) == 0){
				sil_andw_mem((uint32_t *)(hspi->base+TOFF_SPI_CR2), SPI_CR2_LDMARX);
				hspi->RxXferCount = hspi->RxXferCount >> 1;
			}
			else{
				sil_orw_mem((uint32_t *)(hspi->base+TOFF_SPI_CR2), SPI_CR2_LDMARX);
				hspi->RxXferCount = (hspi->RxXferCount >> 1) + 1;
			}
		}
	}

    /*
	 *  DMA����������Хå��ؿ�����
	 */
	if((hspi->Init.Mode == SPI_MODE_MASTER)
		 && (hspi->Init.Direction == SPI_DIRECTION_1LINE || hspi->Init.Direction == SPI_DIRECTION_2LINES_RXONLY)){
	    hspi->hdmarx->xfercallback = spi_dmareceive_func;
	}
	else{
		hspi->hdmarx->xfercallback = spi_dmatransrecv_func;
	}
    hspi->hdmarx->xferhalfcallback = NULL;
    hspi->hdmarx->errorcallback = spi_dmaerror_func;

	/*
	 *  ����DMA����ȥ�������
	 */
    dma_start(hspi->hdmarx, (uint32_t)(hspi->base+TOFF_SPI_DR), (uint32_t)hspi->pRxBuffPtr, hspi->RxXferSize);

	/*  SPIͭ���� */
	if((sil_rew_mem((uint32_t *)(hspi->base+TOFF_SPI_CR1)) & SPI_CR1_SPE) == 0)
		sil_orw_mem((uint32_t *)(hspi->base+TOFF_SPI_CR1), SPI_CR1_SPE);

	/*
	 *  SPI����DMA����
	 */
	sil_orw_mem((uint32_t *)(hspi->base+TOFF_SPI_CR2), (SPI_CR2_RXDMAEN));

	/*
	 *  ���顼���������
	 */
	sil_orw_mem((uint32_t *)(hspi->base+TOFF_SPI_CR2), SPI_CR2_ERRIE);

#if SPI_WAIT_TIME != 0
	ercd = spi_inwait(hspi, SPI_WAIT_TIME * length);

	if(hspi->Init.semlock != 0)
		sig_sem(hspi->Init.semlock);
#endif
	return ercd;
}

/*
 *  SPI�������¹Դؿ�
 *  parameter1  hspi: SPI�ϥ�ɥ�ؤΥݥ���
 *  parameter2  ptxdata: �����Хåե��ؤΥݥ���
 *  parameter3  prxdata: �����Хåե��ؤΥݥ���
 *  parameter4  length: ������������
 *  return ER������
 */
ER
spi_transrecv(SPI_Handle_t *hspi, uint8_t *ptxdata, uint8_t *prxdata, uint16_t length)
{
	ER ercd = E_OK;

	if(hspi == NULL || prxdata == NULL || length == 0)
		return E_PAR;
	if(hspi->Init.Direction != SPI_DIRECTION_2LINES && hspi->Init.Mode != SPI_MODE_MASTER)
		return E_PAR;

	if(hspi->Init.semlock != 0)
		wai_sem(hspi->Init.semlock);
	if(hspi->status != SPI_STATUS_READY){
		if(hspi->Init.semlock != 0)
			sig_sem(hspi->Init.semlock);
		return E_OBJ;
	}

	hspi->xmode = SPI_XMODE_TXRX;
	hspi->status = SPI_STATUS_BUSY;

	/*
	 *  ž�����������
	 */
	hspi->ErrorCode   = SPI_ERROR_NONE;
	hspi->pTxBuffPtr  = (uint8_t*)ptxdata;
	hspi->TxXferSize  = length;
	hspi->TxXferCount = 0;
	hspi->pRxBuffPtr  = (uint8_t*)prxdata;
	hspi->RxXferSize  = length;
	hspi->RxXferCount = 0;

	/*
	 *  CRC�⡼�ɤʤ������ͭ����
	 */
	if(hspi->Init.CRC == SPI_CRC_ENABLE){
		sil_andw_mem((uint32_t *)(hspi->base+TOFF_SPI_CR1), SPI_CR1_CRCEN);
		sil_orw_mem((uint32_t *)(hspi->base+TOFF_SPI_CR1), SPI_CR1_CRCEN);
	}

    /*
	 *  LAST DMA�ȥ���å���ۡ��������
	 */
	sil_andw_mem((uint32_t *)(hspi->base+TOFF_SPI_CR2), SPI_CR2_LDMATX);
	sil_andw_mem((uint32_t *)(hspi->base+TOFF_SPI_CR2), SPI_CR2_LDMARX);
	if(hspi->Init.DataSize > SPI_DATASIZE_8BIT){
		sil_andw_mem((uint32_t *)(hspi->base+TOFF_SPI_CR2), SPI_CR2_FRXTH);
	}
	else{
		sil_orw_mem((uint32_t *)(hspi->base+TOFF_SPI_CR2), SPI_CR2_FRXTH);
		if(hspi->hdmatx->Init.MemDataAlignment == DMA_MDATAALIGN_HALFWORD){
	        if((hspi->TxXferSize & 0x1) == 0x0 ){
				sil_andw_mem((uint32_t *)(hspi->base+TOFF_SPI_CR2), SPI_CR2_LDMATX);
				hspi->TxXferCount = hspi->TxXferCount >> 1;
			}
			else{
				sil_orw_mem((uint32_t *)(hspi->base+TOFF_SPI_CR2), SPI_CR2_LDMATX);
				hspi->TxXferCount = (hspi->TxXferCount >> 1) + 1;
			}
		}
		if(hspi->hdmarx->Init.MemDataAlignment == DMA_MDATAALIGN_HALFWORD){
			sil_andw_mem((uint32_t *)(hspi->base+TOFF_SPI_CR2), SPI_CR2_FRXTH);
			if((hspi->RxXferCount & 0x1) == 0){
				sil_andw_mem((uint32_t *)(hspi->base+TOFF_SPI_CR2), SPI_CR2_LDMARX);
				hspi->RxXferCount = hspi->RxXferCount >> 1;
			}
			else{
				sil_orw_mem((uint32_t *)(hspi->base+TOFF_SPI_CR2), SPI_CR2_LDMARX);
				hspi->RxXferCount = (hspi->RxXferCount >> 1) + 1;
			}
		}
	}

	/*
	 *  DMA����������Хå��ؿ�����
	 */
	if((hspi->Init.Mode == SPI_MODE_MASTER)
		 && (hspi->Init.Direction == SPI_DIRECTION_1LINE || hspi->Init.Direction == SPI_DIRECTION_2LINES_RXONLY)){
		hspi->hdmarx->xfercallback = spi_dmareceive_func;
	}
	else{
		hspi->hdmarx->xfercallback = spi_dmatransrecv_func;
	}
	hspi->hdmarx->xferhalfcallback = NULL;
	hspi->hdmarx->errorcallback = spi_dmaerror_func;

	/*
	 *  ����DMA����ȥ�������
	 */
	dma_start(hspi->hdmarx, (uint32_t)(hspi->base+TOFF_SPI_DR), (uint32_t)hspi->pRxBuffPtr, hspi->RxXferSize);

	/*
	 *  SPI����DMA����
	 */
	sil_orw_mem((uint32_t *)(hspi->base+TOFF_SPI_CR2), SPI_CR2_RXDMAEN);

	/*
	 *  DMA����������Хå�������
	 */
	hspi->hdmatx->xfercallback = NULL;
	hspi->hdmatx->errorcallback = spi_dmaerror_func;

	/*
	 *  ����DMA����ȥ�������
	 */
	dma_start(hspi->hdmatx, (uint32_t)hspi->pTxBuffPtr, (uint32_t)(hspi->base+TOFF_SPI_DR), hspi->TxXferSize);

	/*
	 *  SPI���顼���������
	 */
	sil_orw_mem((uint32_t *)(hspi->base+TOFF_SPI_CR2), SPI_CR2_ERRIE);

	/*  SPIͭ���� */
	if((sil_rew_mem((uint32_t *)(hspi->base+TOFF_SPI_CR1)) & SPI_CR1_SPE) == 0)
		sil_orw_mem((uint32_t *)(hspi->base+TOFF_SPI_CR1), SPI_CR1_SPE);

	/*
	 *  SPI����DMA����
	 */
	sil_orw_mem((uint32_t *)(hspi->base+TOFF_SPI_CR2), SPI_CR2_TXDMAEN);

#if SPI_WAIT_TIME != 0
	ercd = spi_inwait(hspi, SPI_WAIT_TIME * length);

	if(hspi->Init.semlock != 0)
		sig_sem(hspi->Init.semlock);
#endif
	return ercd;
}

/*
 *  SPIž����λ�Ԥ�
 */
ER
spi_wait(SPI_Handle_t *hspi, uint32_t timeout)
{
	ER ercd = E_OK;

#if SPI_WAIT_TIME == 0
	if(hspi == NULL)
		return E_PAR;
	ercd = spi_inwait(hspi, timeout);
	if(hspi->Init.semlock != 0)
		sig_sem(hspi->Init.semlock);
#endif
	return ercd;
}

/*
 *  SPI����ߥ����ӥ��롼����
 */
void
spi_handler(SPI_Handle_t *hspi)
{
	volatile uint32_t tmp3 = 0;

	/*
	 *  ����ߥ��顼Ƚ��
	 */
	if((sil_rew_mem((uint32_t *)(hspi->base+TOFF_SPI_CR2)) & SPI_CR2_ERRIE) != 0){
		/*
		 *  SPI CRC���顼Ƚ��
		 */
		if((sil_rew_mem((uint32_t *)(hspi->base+TOFF_SPI_SR)) & SPI_SR_CRCERR) != 0){
			hspi->ErrorCode |= SPI_ERROR_CRC;
			sil_andw_mem((uint32_t *)(hspi->base+TOFF_SPI_SR), SPI_SR_CRCERR);
		}
		/*
		 *  SPI �⡼�ɥե����Ƚ��
		 */
		if((sil_rew_mem((uint32_t *)(hspi->base+TOFF_SPI_SR)) & SPI_SR_MODF) != 0){
			hspi->ErrorCode |= SPI_ERROR_MODF;
			sil_andw_mem((uint32_t *)(hspi->base+TOFF_SPI_SR), SPI_SR_MODF);
            sil_andw_mem((uint32_t *)(hspi->base+TOFF_SPI_CR1), SPI_CR1_SPE);
		}
		/*
		 *  SPI �����С����Ƚ��
		 */
		if((sil_rew_mem((uint32_t *)(hspi->base+TOFF_SPI_SR)) & SPI_SR_OVR) != 0){
			if(hspi->xmode != SPI_XMODE_TX){
				hspi->ErrorCode |= SPI_ERROR_OVR;
				tmp3 = sil_rew_mem((uint32_t *)(hspi->base+TOFF_SPI_DR));
				((void)(tmp3));
				sil_andw_mem((uint32_t *)(hspi->base+TOFF_SPI_SR), SPI_SR_OVR);
			}
		}
		/*
		 *  SPI �ե졼�२�顼Ƚ��
		 */
		if((sil_rew_mem((uint32_t *)(hspi->base+TOFF_SPI_SR)) & SPI_SR_FRE) != 0){
			hspi->ErrorCode |= SPI_ERROR_FRE;
			sil_andw_mem((uint32_t *)(hspi->base+TOFF_SPI_SR), SPI_SR_FRE);
		}
		/*
		 *  ���顼ȯ�����ϥ�ǥ����᤹
		 */
		if(hspi->ErrorCode != SPI_ERROR_NONE){
            sil_andw_mem((uint32_t *)(hspi->base+TOFF_SPI_CR1), SPI_CR1_SPE);
			hspi->status = SPI_STATUS_READY;
			syslog_2(LOG_ERROR, "spi hanndler error[%08x] ErrorCode[%08x] !", hspi, hspi->ErrorCode);
		}
	}
}


/*
 *  SPI����ߥ����ӥ��롼����
 */
void spi_isr(intptr_t exinf)
{
  spi_handler(&SpiHandle[INDEX_SPI((uint32_t)exinf)]);
}

