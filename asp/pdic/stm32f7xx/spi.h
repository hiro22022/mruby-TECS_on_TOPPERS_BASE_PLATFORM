/*
 *  TOPPERS/ASP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Advanced Standard Profile Kernel
 * 
 *  Copyright (C) 2008-2011 by Embedded and Real-Time Systems Laboratory
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
 *  Copyright (C) 2015-2016 by TOPPERS PROJECT Educational Working Group.
 * 
 *  ��L���쌠�҂́C�ȉ���(1)�`(4)�̏����𖞂����ꍇ�Ɍ���C�{�\�t�g�E�F
 *  �A�i�{�\�t�g�E�F�A�����ς������̂��܂ށD�ȉ������j���g�p�E�����E��
 *  �ρE�Ĕz�z�i�ȉ��C���p�ƌĂԁj���邱�Ƃ𖳏��ŋ�������D
 *  (1) �{�\�t�g�E�F�A���\�[�X�R�[�h�̌`�ŗ��p����ꍇ�ɂ́C��L�̒���
 *      ���\���C���̗��p��������щ��L�̖��ۏ؋K�肪�C���̂܂܂̌`�Ń\�[
 *      �X�R�[�h���Ɋ܂܂�Ă��邱�ƁD
 *  (2) �{�\�t�g�E�F�A���C���C�u�����`���ȂǁC���̃\�t�g�E�F�A�J���Ɏg
 *      �p�ł���`�ōĔz�z����ꍇ�ɂ́C�Ĕz�z�ɔ����h�L�������g�i���p
 *      �҃}�j���A���Ȃǁj�ɁC��L�̒��쌠�\���C���̗��p��������щ��L
 *      �̖��ۏ؋K����f�ڂ��邱�ƁD
 *  (3) �{�\�t�g�E�F�A���C�@��ɑg�ݍ��ނȂǁC���̃\�t�g�E�F�A�J���Ɏg
 *      �p�ł��Ȃ��`�ōĔz�z����ꍇ�ɂ́C���̂����ꂩ�̏����𖞂�����
 *      �ƁD
 *    (a) �Ĕz�z�ɔ����h�L�������g�i���p�҃}�j���A���Ȃǁj�ɁC��L�̒�
 *        �쌠�\���C���̗��p��������щ��L�̖��ۏ؋K����f�ڂ��邱�ƁD
 *    (b) �Ĕz�z�̌`�Ԃ��C�ʂɒ�߂���@�ɂ���āCTOPPERS�v���W�F�N�g��
 *        �񍐂��邱�ƁD
 *  (4) �{�\�t�g�E�F�A�̗��p�ɂ�蒼�ړI�܂��͊ԐړI�ɐ����邢���Ȃ鑹
 *      �Q������C��L���쌠�҂����TOPPERS�v���W�F�N�g��Ɛӂ��邱�ƁD
 *      �܂��C�{�\�t�g�E�F�A�̃��[�U�܂��̓G���h���[�U����̂����Ȃ闝
 *      �R�Ɋ�Â�����������C��L���쌠�҂����TOPPERS�v���W�F�N�g��
 *      �Ɛӂ��邱�ƁD
 * 
 *  �{�\�t�g�E�F�A�́C���ۏ؂Œ񋟂���Ă�����̂ł���D��L���쌠�҂�
 *  ���TOPPERS�v���W�F�N�g�́C�{�\�t�g�E�F�A�Ɋւ��āC����̎g�p�ړI
 *  �ɑ΂���K�������܂߂āC�����Ȃ�ۏ؂��s��Ȃ��D�܂��C�{�\�t�g�E�F
 *  �A�̗��p�ɂ�蒼�ړI�܂��͊ԐړI�ɐ����������Ȃ鑹�Q�Ɋւ��Ă��C��
 *  �̐ӔC�𕉂�Ȃ��D
 * 
 *  @(#) $Id: spi.h 698 2016-02-01 11:00:32Z roi $
 */
/*
 * 
 *  STM32F4XX SPI�f�o�C�X�h���C�o�̊O���錾
 *
 */

#ifndef _SPI_H_
#define _SPI_H_

#ifdef __cplusplus
 extern "C" {
#endif

/*
 *  SPI�|�[�g��`
 */
#define SPI1_PORTID             1
#define SPI2_PORTID             2
#define NUM_SPIPORT             2

/*
 *  SPI��Ԓ�`
 */
#define SPI_STATUS_RESET        0x00	/* SPI���g�p��� */
#define SPI_STATUS_READY        0x01	/* SPI���f�B��� */
#define SPI_STATUS_ERROR        0x02	/* SPI�G���[��� */
#define SPI_STATUS_BUSY         0x04	/* SPI������ */

/*
 *  SPI�G���[��`
 */ 
#define SPI_ERROR_NONE          0x00000000	/* No error */
#define SPI_ERROR_MODF          0x00000001	/* MODF error */
#define SPI_ERROR_CRC           0x00000002	/* CRC error */
#define SPI_ERROR_OVR           0x00000004	/* OVR error */
#define SPI_ERROR_FRE           0x00000008	/* FRE error */
#define SPI_ERROR_DMA           0x00000010	/* DMA transfer error */
#define SPI_ERROR_TIMEOUT       0x00000020

/*
 *  SPI�]�����[�h
 */
#define SPI_XMODE_TX            0x00000000	/* ���M���[�h */
#define SPI_XMODE_RX            0x00000001	/* ��M���[�h */
#define SPI_XMODE_TXRX          0x00000002	/* ����M���[�h */

/*
 *  SPI���[�h��`
 */
#define SPI_MODE_SLAVE          0x00000000
#define SPI_MODE_MASTER         (SPI_CR1_MSTR | SPI_CR1_SSI)


/*
 *  SPI�f�[�^�T�C�Y��`
 */
#define SPI_DATASIZE_4BIT       0x00000300
#define SPI_DATASIZE_5BIT       0x00000400
#define SPI_DATASIZE_6BIT       0x00000500
#define SPI_DATASIZE_7BIT       0x00000600
#define SPI_DATASIZE_8BIT       0x00000700
#define SPI_DATASIZE_9BIT       0x00000800
#define SPI_DATASIZE_10BIT      0x00000900
#define SPI_DATASIZE_11BIT      0x00000A00
#define SPI_DATASIZE_12BIT      0x00000B00
#define SPI_DATASIZE_13BIT      0x00000C00
#define SPI_DATASIZE_14BIT      0x00000D00
#define SPI_DATASIZE_15BIT      0x00000E00
#define SPI_DATASIZE_16BIT      0x00000F00

/*
 *  SPI�]���N���b�N�ɐ���`
 */
#define SPI_POLARITY_LOW        0x00000000
#define SPI_POLARITY_HIGH       SPI_CR1_CPOL

/*
 *  SPI�N���b�N�ʑ���`
 */
#define SPI_PHASE_1EDGE         0x00000000
#define SPI_PHASE_2EDGE         SPI_CR1_CPHA

/*
 *  SPI NSS��`
 */
#define SPI_NSS_SOFT            SPI_CR1_SSM
#define SPI_NSS_HARD_INPUT      0x00000000
#define SPI_NSS_HARD_OUTPUT     0x00040000

/*
 *  SPI MSB/LSB��`
 */
#define SPI_DATA_MSB            0x00000000
#define SPI_DATA_LSB            SPI_CR1_LSBFIRST

/*
 *  SPI�v���X�P�[���ݒ�
 */
#define SPI_BAUDRATEPRESCALER_2   0x00000000
#define SPI_BAUDRATEPRESCALER_4   0x00000008
#define SPI_BAUDRATEPRESCALER_8   0x00000010
#define SPI_BAUDRATEPRESCALER_16  0x00000018
#define SPI_BAUDRATEPRESCALER_32  0x00000020
#define SPI_BAUDRATEPRESCALER_64  0x00000028
#define SPI_BAUDRATEPRESCALER_128 0x00000030
#define SPI_BAUDRATEPRESCALER_256 0x00000038

/*
 *  CRC����`
 */
#define SPI_CRC_LENGTH_DATASIZE   0x00000000
#define SPI_CRC_LENGTH_8BIT       0x00000001
#define SPI_CRC_LENGTH_16BIT      0x00000002

/*
 *  SPI TI���[�h�ݒ��`
 */
#define SPI_TIMODE_DISABLE      0x00000000
#define SPI_TIMODE_ENABLE       SPI_CR2_FRF

/*
 *  SPI���C���ݒ�
 */
#define SPI_DIRECTION_2LINES             ((uint32_t)0x00000000)
#define SPI_DIRECTION_2LINES_RXONLY      SPI_CR1_RXONLY
#define SPI_DIRECTION_1LINE              SPI_CR1_BIDIMODE

/*
 *  SPI CRC�ݒ��`
 */
#define SPI_CRC_DISABLE         0x00000000
#define SPI_CRC_ENABLE          SPI_CR1_CRCEN

/*
 *  SPI�n�[�h�E�F�A�ݒ�\����
 */

typedef struct _SPI_PortControlBlock{
	uint32_t              base;
	uint32_t              gioclockbase;
	uint32_t              gioclockbit;
	uint32_t              gioclockbit2;
	uint32_t              spiclockbase;
	uint32_t              spiclockbit;
	uint32_t              dmaclockbase;
	uint32_t              dmaclockbit;

	uint32_t              giobase1;
	uint32_t              giobase2;
	uint32_t              giobase3;
	uint8_t               sckpin;
	uint8_t               misopin;
	uint8_t               mosipin;
	uint8_t               apvalue;
	uint32_t              dmatxchannel;
	uint32_t              dmatxbase;
	uint32_t              dmarxchannel;
	uint32_t              dmarxbase;
} SPI_PortControlBlock;

/*
 *  SPI �ݒ菉���ݒ�\����
 */
typedef struct
{
	uint32_t              Mode;				/* SPI�}�X�^�[�X���[�u�ݒ� */
	uint32_t              Direction;		/* SPI�]������ */
    uint32_t              DataSize;			/* SPI�]���f�[�^�T�C�Y */
	uint32_t              CLKPolarity;		/* SPI�]���N���b�N�̋ɐ� */
	uint32_t              CLKPhase;			/* SPI�N���b�N�ʑ� */
	uint32_t              NSS;				/* SPI NSS */
	uint32_t              Prescaler;		/* SPI�N���b�N�����ݒ� */
	uint32_t              SignBit;			/* SPI MSB/LSB�ݒ� */
	uint32_t              TIMode;			/* SPI TI���[�h */
	uint32_t              CRC;				/* SPI CRC���Z�ݒ� */
	uint32_t              CRCPolynomial;	/* SPI CRC�������ݒ� */
	uint32_t              CRCLength;		/* SPI CRC�� */
	int                   semid;			/* SPI �ʐM�p�Z�}�t�H�l */
	int                   semlock;			/* SPI ���b�N�Z�}�t�H�l */
}SPI_Init_t;

/*
 *  SPI�n���h��
 */
typedef struct _SPI_Handle_t
{
	uint32_t              base;				/* SPI registers base address */
	SPI_Init_t            Init;				/* SPI communication parameters */
	uint8_t               *pTxBuffPtr;		/* Pointer to SPI Tx transfer Buffer */
	uint16_t              TxXferSize;		/* SPI Tx transfer size */
	uint16_t              TxXferCount;		/* SPI Tx Transfer Counter */
	uint8_t               *pRxBuffPtr;		/* Pointer to SPI Rx transfer Buffer */
	uint16_t              RxXferSize;		/* SPI Rx transfer size */
	uint16_t              RxXferCount;		/* SPI Rx Transfer Counter */
	DMA_Handle_t          *hdmatx;			/* SPI Tx DMA handle parameters */
	DMA_Handle_t          *hdmarx;			/* SPI Rx DMA handle parameters */
	uint32_t              xmode;
	volatile uint32_t     status;			/* SPI communication state */
	volatile uint32_t     ErrorCode;		/* SPI Error code */
}SPI_Handle_t;



extern SPI_Handle_t *spi_init(ID port, SPI_Init_t *spii);
extern ER spi_deinit(SPI_Handle_t *hspi);
extern ER spi_reset(SPI_Handle_t *hspi);
extern ER spi_transmit(SPI_Handle_t *hspi, uint8_t *pdata, uint16_t length);
extern ER spi_receive(SPI_Handle_t *hspi, uint8_t *pdata, uint16_t length);
extern ER spi_transrecv(SPI_Handle_t *hspi, uint8_t *ptxData, uint8_t *prxData, uint16_t length);
extern ER spi_wait(SPI_Handle_t *hspi, uint32_t timeout);
extern void spi_handler(SPI_Handle_t *hspi);
extern void spi_isr(intptr_t exinf);

#ifdef __cplusplus
}
#endif

#endif	/* _SPI_H_ */

