/*
 *  TOPPERS/ASP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Advanced Standard Profile Kernel
 * 
 *  Copyright (C) 2008-2011 by Embedded and Real-Time Systems Laboratory
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
 *  Copyright (C) 2015-2016 by TOPPERS PROJECT Educational Working Group.
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
 *  @(#) $Id: i2c.h 698 2016-02-01 11:00:32Z roi $
 */
/*
 * 
 *  STM32F7xx I2C�ǥХ����ɥ饤�Фγ������
 *
 */

#ifndef _I2C_H_
#define _I2C_H_

#ifdef __cplusplus
 extern "C" {
#endif


/*
 *  I2C�ݡ������
 */
#define I2C1_PORTID             1
#define I2C2_PORTID             2
#define I2C3_PORTID             3
#define I2C4_PORTID             4
#define NUM_I2CPORT             4

/*
 *  I2C���ɥ�å��󥰥⡼��
 */
#define I2C_ADDRESSINGMODE_7BIT  0x00000001
#define I2C_ADDRESSINGMODE_10BIT 0x00000002

/*
 *  I2C�ǥ奢�륢�ɥ�å��󥰥⡼��
 */
#define I2C_DUALADDRESS_DISABLE 0x00000000
#define I2C_DUALADDRESS_ENABLE  I2C_OAR2_OA2EN

/*
 *  I2C�����ͥ�륳���륢�ɥ�å��󥰥⡼��
 */
#define I2C_GENERALCALL_DISABLE 0x00000000
#define I2C_GENERALCALL_ENABLE  I2C_CR1_GCEN

/*
 *  I2C�Υ��ȥ�å��⡼��
 */
#define I2C_NOSTRETCH_DISABLE   0x00000000
#define I2C_NOSTRETCH_ENABLE    I2C_CR1_NOSTRETCH

/*
 *  I2C���ꥢ�ɥ쥹�⡼��
 */
#define I2C_MEMADD_SIZE_8BIT    0x00000001
#define I2C_MEMADD_SIZE_16BIT   0x00000002

/*
 *  I2C�ϡ��ɥ��������깽¤��
 */

typedef struct _I2C_PortControlBlock{
	uint32_t              base;
	uint32_t              gioclockbase;
	uint32_t              gioclockbit1;
	uint32_t              gioclockbit2;
	uint32_t              i2cclockbase;
	uint32_t              i2cclockbit;
	uint32_t              i2cresetbase;
	uint32_t              i2cresetbit;

	uint32_t              giobase1;
	uint32_t              giobase2;
	uint8_t               sclpin;
	uint8_t               sclaf;
	uint8_t               sdapin;
	uint8_t               sdaaf;
} I2C_PortControlBlock;

/*
 *  I2C����ե奮��졼�����¤�����
 */
typedef struct
{
	uint32_t              Timing;			/* I2C I2C_TIMINGR�� */
	uint32_t              OwnAddress1;		/* I2C �ǽ�ΥǥХ����μ�ʬ�Υ��ɥ쥹 */
	uint32_t              AddressingMode;	/* I2C ���ɥ�å��󥰥⡼�� */
	uint32_t              DualAddressMode;	/* I2C ���ɥ쥹�ǥ奢��⡼�� */
	uint32_t              OwnAddress2;		/* I2C �����ܤΥǥХ����μ�ʬ�Υ��ɥ쥹 */
	uint32_t              OwnAddress2Masks;	/* I2C �����ܤΥǥХ������ɥ쥹�Υޥ����� */
	uint32_t              GeneralCallMode;	/* I2C �����ͥ�롦�����롦�⡼�� */
	uint32_t              NoStretchMode;	/* I2C �Υ󥹥ȥ�å��⡼�� */
	int                   semid;			/* I2C �̿��ѥ��ޥե��� */
	int                   semlock;			/* I2C ��å����ޥե��� */
}I2C_Init_t;

/*
 *  I2C�������
 */
#define I2C_STATE_RESET     0x00		/* I2C �ꥻ�åȾ��� */
#define I2C_STATE_READY     0x01		/* I2C ��ǥ����� */
#define I2C_STATE_BUSY      0x02		/* I2C �ӥ������� */
#define I2C_STATE_MASTER_BUSY_TX 0x12	/* I2C �ޥ����������� */
#define I2C_STATE_MASTER_BUSY_RX 0x22	/* I2C �ޥ����������� */
#define I2C_STATE_SLAVE_BUSY_TX  0x32	/* I2C ���졼�������� */
#define I2C_STATE_SLAVE_BUSY_RX  0x42	/* I2C ���졼�ּ����� */

/*
 *  I2C���顼���
 */
#define I2C_ERROR_NONE         0x00000000	/* No error              */
#define I2C_ERROR_BERR         0x00000001	/* BERR error            */
#define I2C_ERROR_ARLO         0x00000002	/* ARLO error            */
#define I2C_ERROR_AF           0x00000004	/* ACKF error            */
#define I2C_ERROR_OVR          0x00000008	/* OVR error             */
#define I2C_ERROR_DMA          0x00000010	/* DMA transfer error    */
#define I2C_ERROR_TIMEOUT      0x00000020	/* Timeout error         */
#define I2C_ERROR_SIZE         0x00000040	/* Size Management error */

/*
 *  I2C�ϥ�ɥ����
 */
typedef struct _I2C_Handle_t I2C_Handle_t;
struct _I2C_Handle_t {
	uint32_t                   base;		/* IP�١������ɥ쥹 */
	I2C_Init_t                 Init;		/* I2C�������ѥ�᡼�� */
	int                        i2cid;		/* I2C ID */
	uint8_t                    *pBuffPtr;	/* I2C �̿��Хåե��ؤΥݥ��� */
	uint16_t                   XferSize;	/* I2C �̿������� */
	volatile uint16_t          XferCount;	/* I2C �̿������� */
	void               (*writecallback)(I2C_Handle_t * hi2c);	/* ������λ������Хå��ؿ� */
	void               (*readcallback)(I2C_Handle_t * hi2c);	/* ������λ������Хå��ؿ� */
	void               (*errorcallback)(I2C_Handle_t * hi2c);	/* ���顼������Хå��ؿ� */
	volatile uint32_t          State;		/* I2C �¹Ծ��� */
	volatile uint32_t          ErrorCode;	/* I2C Error code */
};

extern I2C_Handle_t *i2c_init(ID port, I2C_Init_t *pini);
extern ER i2c_deinit (I2C_Handle_t *hi2c);
extern ER i2c_slaveread(I2C_Handle_t *hi2c, uint8_t *pData, uint16_t Size);
extern ER i2c_slavewrite(I2C_Handle_t *hi2c, uint8_t *pData, uint16_t Size);
extern ER i2c_memread(I2C_Handle_t *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout);
extern ER i2c_memwrite(I2C_Handle_t *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout);
extern ER i2c_ready(I2C_Handle_t *pi2chandler, uint16_t DevAddress, uint32_t Trials, uint32_t Timeout);
extern void i2c_ev_handler(I2C_Handle_t *pi2chandler);
extern void i2c_er_handler(I2C_Handle_t *pi2chandler);
extern void i2c_ev_isr(intptr_t exinf);
extern void i2c_er_isr(intptr_t exinf);


#ifdef __cplusplus
}
#endif

#endif	/* _I2C_H_ */

