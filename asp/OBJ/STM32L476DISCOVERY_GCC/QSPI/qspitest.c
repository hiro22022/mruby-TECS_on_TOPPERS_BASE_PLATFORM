/*
 *  TOPPERS/ASP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Advanced Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2004-2012 by Embedded and Real-Time Systems Laboratory
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
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
 *  $Id: qspitest.c 2416 2012-09-07 08:06:20Z ertl-hiro $
 */

/* 
 *  QSPI�ƥ��ȥץ����
 *  �ץ����γ���:
 *
 *  1)QSPI����ν������
 *  2)�ǽ�Υ�������ERASE��
 *  3)0���Ϥ���256�Х��ȥǡ�������ߡ�
 *  4)DIRECT READ�⡼�ɤ�20������(0x90000000���Ϥǻ��Ȳ�ǽ)��
 *  5)INDIRECT READ��512�Х����ɤ߽Ф���
 *
 *
 */

#include <kernel.h>
#include <t_syslog.h>
#include <t_stdlib.h>
#include <target_syssvc.h>
#include "syssvc/serial.h"
#include "syssvc/syslog.h"
#include "kernel_cfg.h"
#include "qspi.h"
#include "qspitest.h"

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

#define BUFFERSIZE  512

/*
 * MX25L512 Registers  
 */ 
/* Status Register */
#define MX25L512_SR_WIP                      ((uint8_t)0x01)    /* Write in progress */
#define MX25L512_SR_WREN                     ((uint8_t)0x02)    /* Write enable latch */
#define MX25L512_SR_BLOCKPR                  ((uint8_t)0x5C)    /* Block protected against program and erase operations */
#define MX25L512_SR_PRBOTTOM                 ((uint8_t)0x20)    /* Protected memory area defined by BLOCKPR starts from top or bottom */
#define MX25L512_SR_QUADEN                   ((uint8_t)0x40)    /* Quad IO mode enabled if =1 */
#define MX25L512_SR_SRWREN                   ((uint8_t)0x80)    /* Status register write enable/disable */

/* Configuration Register */
#define MX25L512_CR_ODS                      ((uint8_t)0x07)    /* Output driver strength */
#define MX25L512_CR_ODS_30                   ((uint8_t)0x07)    /* Output driver strength 30 ohms (default)*/
#define MX25L512_CR_ODS_15                   ((uint8_t)0x06)    /* Output driver strength 15 ohms */
#define MX25L512_CR_ODS_20                   ((uint8_t)0x05)    /* Output driver strength 20 ohms */
#define MX25L512_CR_ODS_45                   ((uint8_t)0x03)    /* Output driver strength 45 ohms */
#define MX25L512_CR_ODS_60                   ((uint8_t)0x02)    /* Output driver strength 60 ohms */
#define MX25L512_CR_ODS_90                   ((uint8_t)0x01)    /* Output driver strength 90 ohms */
#define MX25L512_CR_TB                       ((uint8_t)0x08)    /* Top/Bottom bit used to configure the block protect area */
#define MX25L512_CR_PBE                      ((uint8_t)0x10)    /* Preamble Bit Enable */
#define MX25L512_CR_4BYTE                    ((uint8_t)0x20)    /* 3-bytes or 4-bytes addressing */
#define MX25L512_CR_NB_DUMMY                 ((uint8_t)0xC0)    /* Number of dummy clock cycles */

#define MX25L512_DUMMY_CYCLES_READ_QUAD      3

static ER qspi_init_N25Q512A13GSF40E(void *h);
static ER qspi_init_N25Q128A13EF840E(void *hqspi);
static ER QSPI_WriteEnable(void *h, uint32_t Timeout);
static ER QSPI_AutoPollingMemReady(void *h, uint32_t Timeout);


const QSPI_Init_t qspi_config_N25Q512A13GSF40E = {
	QSPI_STIG_RDID_JEDECID_MICRON,
	0,
	1,									/* ClockPrescaler */
	16,									/* FifoThreshold */
	QSPI_SAMPLE_SHIFTING_NONE,			/* SampleShifting */
	QSPI_CS_HIGH_TIME_4_CYCLE,			/* ChipSelectHighTime */
	QSPI_CLOCK_MODE_0,					/* ClockMode */
	QSPI_FLASH_ID_1,					/* FlashID */
	QSPI_DUALFLASH_DISABLE,				/* DualFlash */
	QSPI_DDR_MODE_DISABLE,				/* ddrmode */
	QSPI_DDR_HHC_ANALOG_DELAY,			/* ddrholdhalfcycle */
	QSPI_SIOO_INST_EVERY_CMD,			/* sioomode */
	QSPI_ADDRESS_32_BITS,				/* addr_size */
	QSPI_INSTRUCTION_4_LINES,			/* inst_type */
	QSPI_DATA_4_LINES,					/* inst_data_xfer_type */
	QSPI_STIG_OPCODE_4QUAD_READ,		/* read_op_code */
	QSPI_ADDRESS_4_LINES,				/* read_addr_xfer_type */
	QSPI_DATA_4_LINES,					/* read_data_xfer_type */
	10,									/* read_dummy_cycles */
	QSPI_STIG_OPCODE_4BYTE_WRITE,		/* write_op_code */
	QSPI_ADDRESS_4_LINES,				/* write_addr_xfer_type */
	QSPI_DATA_4_LINES,					/* write_data_xfer_type */
	0,									/* write_dummy_cycles */
	1,									/* erase_count */
	{(64*1024)},
	{QSPI_STIG_OPCODE_4BYTE_SEC_ERASE},
	0,									/* erase_sector_idx */
	(1<<26),
	qspi_init_N25Q512A13GSF40E,
	QSPI_WriteEnable,
	QSPI_AutoPollingMemReady,
	"N25Q512A13GSF40E"
};

const QSPI_Init_t qspi_config_N25Q128A13EF840E = {
	QSPI_STIG_RDID_JEDECID_MICRON,
	0,
	1,									/* ClockPrescaler */
	4,									/* FifoThreshold */
	QSPI_SAMPLE_SHIFTING_NONE,			/* SampleShifting */
	QSPI_CS_HIGH_TIME_1_CYCLE,			/* ChipSelectHighTime */
	QSPI_CLOCK_MODE_0,					/* ClockMode */
	0,									/* FlashID */
	QSPI_DUALFLASH_DISABLE,				/* DualFlash */
	QSPI_DDR_MODE_DISABLE,				/* ddrmode */
	QSPI_DDR_HHC_ANALOG_DELAY,			/* ddrholdhalfcycle */
	QSPI_SIOO_INST_EVERY_CMD,			/* sioomode */
	QSPI_ADDRESS_24_BITS,				/* addr_size */
	QSPI_INSTRUCTION_1_LINE,			/* inst_type */
	QSPI_DATA_1_LINE,					/* inst_data_xfer_type */
	QSPI_STIG_OPCODE_FASTREAD_QUAD_IO,	/* read_op_code */
	QSPI_ADDRESS_4_LINES,				/* read_addr_xfer_type */
	QSPI_DATA_4_LINES,					/* read_data_xfer_type */
	10,									/* read_dummy_cycles */
	QSPI_STIG_OPCODE_4BYTE_WRITE,		/* write_op_code */
	QSPI_ADDRESS_4_LINES,				/* write_addr_xfer_type */
	QSPI_DATA_4_LINES,					/* write_data_xfer_type */
	0,									/* write_dummy_cycles */
	1,									/* erase_count */
	{(64*1024)},
	{QSPI_STIG_OPCODE_SEC_ERASE},
	0,									/* erase_sector_idx */
	(1<<23),
	qspi_init_N25Q128A13EF840E,
	QSPI_WriteEnable,
	QSPI_AutoPollingMemReady,
	"N25Q128A13EF840E"
};


uint8_t aBuffer[BUFFERSIZE];

#if 1	/* ROI DEBUG */
void sw_handle(int arg)
{
	syslog_1(LOG_NOTICE, "## sw int(%d) ##", arg);
}
#endif	/* ROI DEBUG */

/*
 *  N25Q128A13EF840E������ؿ�
 */
static ER
qspi_init_N25Q128A13EF840E(void *h)
{
	QSPI_Handle_t  *hqspi = h;
	QSPI_Command_t sCommand;
	uint8_t reg[4];
	ER ercd;

	/*
	 *  VOLATILE CONFIGURATION REGISTER�ɹ���
	 */
	sCommand.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	sCommand.Instruction       = QSPI_STIG_OPCODE_READ_VOL_CFG_REG_CMD;
	sCommand.AddressMode       = QSPI_ADDRESS_NONE;
	sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	sCommand.DataMode          = QSPI_DATA_1_LINE;
	sCommand.DummyCycles       = 0;
	sCommand.NbData            = 1;
	if((ercd = qspi_command(hqspi, &sCommand, hqspi->timeout)) != E_OK)
		return ercd;

	/*
	 *  �쥸��������
	 */
	if((ercd = qspi_receive(hqspi, (uint32_t *)&reg[0], hqspi->timeout)) != E_OK)
		return ercd;

	/*
	 *  ENABLE WRITE�׵�
	 */
	if((ercd = QSPI_WriteEnable(hqspi, hqspi->timeout)) != E_OK)
		return ercd;

	/*
	 *  READ DUMMY CYCLE����
	 */
	sCommand.Instruction = QSPI_STIG_OPCODE_WRITE_VOL_CFG_REG_CMD;
	reg[0] &= ~0xF0;
	reg[0] |= hqspi->Init.read_dummy_cycles << 4;
	if((ercd = qspi_command(hqspi, &sCommand, hqspi->timeout)) != E_OK)
		return ercd;

	return qspi_transmit(hqspi, (uint32_t *)&reg[0], hqspi->timeout);
}

/*
 *  READ DUMMY CYCLE����
 */
static ER
QSPI_DummyCyclesCfg(QSPI_Handle_t *hqspi)
{
	QSPI_Command_t  sCommand;
	uint8_t reg[4], reg2[4];
	ER ercd = E_OK;

	/*
	 *  READ STATUS REGISTER
	 */
	sCommand.InstructionMode   = QSPI_INSTRUCTION_4_LINES;
	sCommand.Instruction       = QSPI_STIG_OPCODE_RD_STAT_REG;
	sCommand.AddressMode       = QSPI_ADDRESS_NONE;
	sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	sCommand.DataMode          = QSPI_DATA_4_LINES;
	sCommand.DummyCycles       = 0;
	sCommand.NbData            = 1;
	if((ercd = qspi_command(hqspi, &sCommand, hqspi->timeout)) != E_OK)
		return ercd;
	if((ercd = qspi_receive(hqspi, (uint32_t *)&(reg[0]), hqspi->timeout)) != E_OK)
		return ercd;

	/*
	 *  READ CONFIGURATION REGISTER
	 */
	sCommand.InstructionMode   = QSPI_INSTRUCTION_4_LINES;
	sCommand.Instruction       = QSPI_STIG_OPCODE_READ_CFG_REG;
	sCommand.AddressMode       = QSPI_ADDRESS_NONE;
	sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	sCommand.DataMode          = QSPI_DATA_4_LINES;
	sCommand.DummyCycles       = 0;
	sCommand.NbData            = 1;
	if((ercd = qspi_command(hqspi, &sCommand, hqspi->timeout)) != E_OK)
		return ercd;
	if((ercd = qspi_receive(hqspi, (uint32_t *)&reg2[0], hqspi->timeout)) != E_OK)
		return ercd;

	/*
	 *  ENABLE WRITE
	 */
	if((ercd = QSPI_WriteEnable(hqspi, hqspi->timeout)) != E_OK)
		return ercd;


	/*
	 *  ���ߡ��������������
	 */
	reg2[0] &= ~MX25L512_CR_NB_DUMMY;
	reg2[0] |= (MX25L512_DUMMY_CYCLES_READ_QUAD << 6);
	reg[1] = reg2[0];

	/*
	 *  UPDATE CONFIGURATION REGISTER ���ߡ��������������
	 */
	sCommand.InstructionMode   = QSPI_INSTRUCTION_4_LINES;
	sCommand.Instruction       = QSPI_STIG_OPCODE_WR_STAT_REG;
	sCommand.AddressMode       = QSPI_ADDRESS_NONE;
	sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	sCommand.DataMode          = QSPI_DATA_4_LINES;
	sCommand.DummyCycles       = 0;
	sCommand.NbData            = 2;
	if((ercd = qspi_command(hqspi, &sCommand, hqspi->timeout)) != E_OK)
		return ercd;
	ercd = qspi_transmit(hqspi, (uint32_t *)&reg[0], hqspi->timeout);

	/*
	 *  STATUS/CONFIGURATION�����Ѥ�40ms�Ԥ�
	 */
	dly_tsk( 40 );

	return ercd;
}

/*
 *  QSPI����ꥻ�å�
 */
static ER
QSPI_ResetMemory(QSPI_Handle_t *hqspi)
{
	QSPI_Command_t     sCommand;
	QSPI_AutoPolling_t sConfig;
	uint32_t           reg;
	ER ercd = E_OK;

	/*
	 *  4LINES��QSPI����ꥻ�åȥ��͡��֥륳�ޥ��
	 */
	sCommand.InstructionMode   = QSPI_INSTRUCTION_4_LINES;
	sCommand.Instruction       = QSPI_STIG_OPCODE_N25Q512A_RESET_EN;
	sCommand.AddressMode       = QSPI_ADDRESS_NONE;
	sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	sCommand.DataMode          = QSPI_DATA_NONE;
	sCommand.DummyCycles       = 0;
	if((ercd = qspi_command(hqspi, &sCommand, hqspi->timeout)) != E_OK)
		return ercd;

	/*
	 *  4LINES��QSPI����ꥻ�å��׵�
	 */
	sCommand.Instruction = QSPI_STIG_OPCODE_N25Q512A_RESET_MEM;
	if((ercd = qspi_command(hqspi, &sCommand, hqspi->timeout)) != E_OK)
		return ercd;

	/*
	 *  1LINE��QSPI����ꥻ�åȥ��͡��֥륳�ޥ��
	 */
	sCommand.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	sCommand.Instruction       = QSPI_STIG_OPCODE_N25Q512A_RESET_EN;
	if((ercd = qspi_command(hqspi, &sCommand, hqspi->timeout)) != E_OK)
		return ercd;

	/*
	 *  1LINE��QSPI����ꥻ�å��׵�
	 */
	sCommand.Instruction = QSPI_STIG_OPCODE_N25Q512A_RESET_MEM;
	if((ercd = qspi_command(hqspi, &sCommand, hqspi->timeout)) != E_OK)
		return E_OK;

	/*
	 *  QSPI����ꥻ�å��Ԥ�������
	 */
	dly_tsk( 1000 );

	/*
	 *  WIP bit=0�����Ԥ�
	 */
	sConfig.Match           = 0;
	sConfig.Mask            = MX25L512_SR_WIP;
	sConfig.MatchMode       = QSPI_MATCH_MODE_AND;
	sConfig.StatusBytesSize = 1;
	sConfig.Interval        = 0x10;
	sConfig.AutomaticStop   = QSPI_AUTOMATIC_STOP_ENABLE;

	sCommand.InstructionMode = QSPI_INSTRUCTION_1_LINE;
	sCommand.Instruction     = QSPI_STIG_OPCODE_RD_STAT_REG;
	sCommand.DataMode        = QSPI_DATA_1_LINE;
	if((ercd = qspi_autopolling(hqspi, &sCommand, &sConfig, hqspi->timeout)) != E_OK)
		return ercd;

	/*
	 *  STATUS REGISTER�ɹ���
	 */
	sCommand.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	sCommand.Instruction       = QSPI_STIG_OPCODE_RD_STAT_REG;
	sCommand.AddressMode       = QSPI_ADDRESS_NONE;
	sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	sCommand.DataMode          = QSPI_DATA_1_LINE;
	sCommand.DummyCycles       = 0;
	sCommand.NbData            = 1;
	if((ercd = qspi_command(hqspi, &sCommand, hqspi->timeout)) != E_OK)
		return ercd;

	/*
	 *  �쥸�����ǡ�������
	 */
	if((ercd = qspi_receive(hqspi, &reg, hqspi->timeout)) != E_OK)
		return ercd;

	/*
	 *  1LINE ENABLE WRITE�׵�
	 */
	sCommand.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	sCommand.Instruction       = QSPI_STIG_OPCODE_WREN;
	sCommand.AddressMode       = QSPI_ADDRESS_NONE;
	sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	sCommand.DataMode          = QSPI_DATA_NONE;
	sCommand.DummyCycles       = 0;
	if((ercd = qspi_command(hqspi, &sCommand, hqspi->timeout)) != E_OK)
		return ercd;

	/*
	 *  ENABLE WRITE�����Ԥ�
	 */
	sConfig.Match           = MX25L512_SR_WREN;
	sConfig.Mask            = MX25L512_SR_WREN;
	sConfig.MatchMode       = QSPI_MATCH_MODE_AND;
	sConfig.StatusBytesSize = 1;
	sConfig.Interval        = 0x10;
	sConfig.AutomaticStop   = QSPI_AUTOMATIC_STOP_ENABLE;

	sCommand.Instruction    = QSPI_STIG_OPCODE_RD_STAT_REG;
	sCommand.DataMode       = QSPI_DATA_1_LINE;
	if((ercd = qspi_autopolling(hqspi, &sCommand, &sConfig, hqspi->timeout)) != E_OK)
		return ercd;

	/*
	 *  QSPI���ꥤ�͡��֥�����
	 */
	reg |= MX25L512_SR_QUADEN;

	/*
	 *  UPDATE STATUS REGISTER
	 */
	sCommand.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	sCommand.Instruction       = QSPI_STIG_OPCODE_WR_STAT_REG;
	sCommand.AddressMode       = QSPI_ADDRESS_NONE;
	sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	sCommand.DataMode          = QSPI_DATA_1_LINE;
	sCommand.DummyCycles       = 0;
	sCommand.NbData            = 1;
	if((ercd = qspi_command(hqspi, &sCommand, hqspi->timeout)) != E_OK)
		return ercd;

	/*
	 *  STATUS REGISTER����
	 */
	ercd = qspi_transmit(hqspi, &reg, hqspi->timeout);

	/*
	 *  ������λ�Ԥ���40ms
	 */
	dly_tsk(40);

	return ercd;
}

/*
 * ��This function put QSPI memory in QPI mode (quad I/O).
 */
static ER
QSPI_EnterMemory_QPI(QSPI_Handle_t *hqspi)
{
	QSPI_Command_t     sCommand;
	QSPI_AutoPolling_t sConfig;
	ER ercd = E_OK;

	/*
	 * QSPI�⡼������
	 */
	sCommand.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	sCommand.Instruction       = QSPI_STIG_OPCODE_ENTER_QUAD_CMD;
	sCommand.AddressMode       = QSPI_ADDRESS_NONE;
	sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	sCommand.DataMode          = QSPI_DATA_NONE;
	sCommand.DummyCycles       = 0;
	if((ercd = qspi_command(hqspi, &sCommand, hqspi->timeout)) != E_OK)
		return ercd;

	/*
	 *  QUADEN bit=1 and WIP bit=0�����Ԥ�
	 */
	sConfig.Match           = MX25L512_SR_QUADEN;
	sConfig.Mask            = MX25L512_SR_QUADEN|MX25L512_SR_WIP;
	sConfig.MatchMode       = QSPI_MATCH_MODE_AND;
	sConfig.StatusBytesSize = 1;
	sConfig.Interval        = 0x10;
	sConfig.AutomaticStop   = QSPI_AUTOMATIC_STOP_ENABLE;

	sCommand.InstructionMode   = QSPI_INSTRUCTION_4_LINES;
	sCommand.Instruction       = QSPI_STIG_OPCODE_RD_STAT_REG;
	sCommand.DataMode          = QSPI_DATA_4_LINES;
	return qspi_autopolling(hqspi, &sCommand, &sConfig, hqspi->timeout);
}

/*
 *  QSPI���� 4�Х��ȥ��ɥ쥹����
 */
static ER
QSPI_EnterFourBytesAddress(QSPI_Handle_t *hqspi)
{
	QSPI_Command_t sCommand;
	ER ercd = E_OK;

	/* Enable write operations */
	if((ercd = QSPI_WriteEnable(hqspi, hqspi->timeout)) != E_OK)
		return ercd;

	/*
	 *  4�Х��ȥ⡼������
	 */
	sCommand.InstructionMode   = QSPI_INSTRUCTION_4_LINES;
	sCommand.Instruction       = QSPI_STIG_OPCODE_ENTER_4BYTE_MODE;
	sCommand.AddressMode       = QSPI_ADDRESS_NONE;
	sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	sCommand.DataMode          = QSPI_DATA_NONE;
	sCommand.DummyCycles       = 0;
	if((ercd = qspi_command(hqspi, &sCommand, hqspi->timeout)) != E_OK)
		return ercd;

	/*
	 *  ���꽪λ�Ԥ�
	 */
	return QSPI_AutoPollingMemReady(hqspi, hqspi->timeout);
}

/*
 *  This function configure the Output driver strength on memory side.
 */
static ER
QSPI_OutDrvStrengthCfg( QSPI_Handle_t *hqspi)
{
	QSPI_Command_t  sCommand;
	uint8_t reg[4], reg2[4];
	ER ercd = E_OK;

	/*
	 *  STATUS REGISTER�ɹ���
	 */
	sCommand.InstructionMode   = QSPI_INSTRUCTION_4_LINES;
	sCommand.Instruction       = QSPI_STIG_OPCODE_RD_STAT_REG;
	sCommand.AddressMode       = QSPI_ADDRESS_NONE;
	sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	sCommand.DataMode          = QSPI_DATA_4_LINES;
	sCommand.DummyCycles       = 0;
	sCommand.NbData            = 1;
	if((ercd = qspi_command(hqspi, &sCommand, hqspi->timeout)) != E_OK)
		return ercd;

	/*
	 *  STATUS REGISTER����
	 */
	if((ercd = qspi_receive(hqspi, (uint32_t *)&reg[0], hqspi->timeout)) != E_OK)
		return ercd;

	/*
	 *  CONFIGURATION REGISTER�ɹ���
	 */
	sCommand.InstructionMode   = QSPI_INSTRUCTION_4_LINES;
	sCommand.Instruction       = QSPI_STIG_OPCODE_READ_CFG_REG;
	sCommand.AddressMode       = QSPI_ADDRESS_NONE;
	sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	sCommand.DataMode          = QSPI_DATA_4_LINES;
	sCommand.DummyCycles       = 0;
	sCommand.NbData            = 1;
	if((ercd = qspi_command(hqspi, &sCommand, hqspi->timeout)) != E_OK)
		return ercd;

	/*
	 *  CONFIGURATION REGISTER����
	 */
	if((ercd = qspi_receive(hqspi, (uint32_t *)&reg2[0], hqspi->timeout)) != E_OK)
		return ercd;

	/*
	 *  ENABLE WRITE����
	 */
	if((ercd = QSPI_WriteEnable(hqspi, hqspi->timeout)) != E_OK)
		return ercd;

	/*
	 *  Set Output Strength of the QSPI memory 15 ohms
	 */
	reg2[0] &= ~MX25L512_CR_ODS;
	reg2[0] |= MX25L512_CR_ODS_15;
	reg[1] = reg2[0];

	/*
	 *  UPDATE CONFIGURATION REGISTER
	 */
	sCommand.InstructionMode   = QSPI_INSTRUCTION_4_LINES;
	sCommand.Instruction       = QSPI_STIG_OPCODE_WR_STAT_REG;
	sCommand.AddressMode       = QSPI_ADDRESS_NONE;
	sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	sCommand.DataMode          = QSPI_DATA_4_LINES;
	sCommand.DummyCycles       = 0;
	sCommand.NbData            = 2;
	if((ercd = qspi_command(hqspi, &sCommand, hqspi->timeout)) != E_OK)
		return ercd;

	/*
	 *  �쥸��������
	 */
	return qspi_transmit(hqspi, (uint32_t *)&reg[0], hqspi->timeout);
}

/*
 *  N25Q512A13GSF40E������ؿ�
 */
static ER
qspi_init_N25Q512A13GSF40E(void *h)
{
	QSPI_Handle_t *hqspi = h;
	ER ercd = E_OK;

	/*
	 *  QSPI memory reset
	 */
	if((ercd = QSPI_ResetMemory(hqspi)) != E_OK)
		return ercd;

	/*
	 *  Put QSPI memory in QPI mode
	 */
	if((ercd =  QSPI_EnterMemory_QPI(hqspi)) !=E_OK )
		return ercd;

	/*
	 *  Set the QSPI memory in 4-bytes address mode
	 */
	if((ercd = QSPI_EnterFourBytesAddress(hqspi)) != E_OK)
		return ercd;

	/*
	 *  Configuration of the dummy cycles on QSPI memory side
	 */
	if((ercd = QSPI_DummyCyclesCfg(hqspi)) != E_OK)
		return ercd;

	/*
	 *  Configuration of the Output driver strength on memory side
	 */
	return QSPI_OutDrvStrengthCfg(hqspi);
}

/*
 *  QSPI WRITE ENABLE����ؿ�
 */
static ER
QSPI_WriteEnable(void *h, uint32_t Timeout)
{
	QSPI_Handle_t      *hqspi = h;
	QSPI_Command_t     sCommand;
	QSPI_AutoPolling_t sConfig;
	ER ercd = E_OK;

	/*
	 *  ENABLE WRITE����
	 */
	sCommand.InstructionMode   = hqspi->Init.inst_type;
	sCommand.Instruction       = QSPI_STIG_OPCODE_WREN;
	sCommand.AddressMode       = QSPI_ADDRESS_NONE;
	sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	sCommand.DataMode          = QSPI_DATA_NONE;
	sCommand.DummyCycles       = 0;
	if((ercd = qspi_command(hqspi, &sCommand, Timeout)) != E_OK)
		return ercd;

	/*
	 *  WRITE ENABLE STATUS�����Ԥ�
	 */
	sConfig.Match           = 0x02;
	sConfig.Mask            = 0x02;
	sConfig.MatchMode       = QSPI_MATCH_MODE_AND;
	sConfig.StatusBytesSize = 1;
	sConfig.Interval        = 0x10;
	sConfig.AutomaticStop   = QSPI_AUTOMATIC_STOP_ENABLE;

	sCommand.Instruction    = QSPI_STIG_OPCODE_RD_STAT_REG;
	sCommand.DataMode       = hqspi->Init.inst_data_xfer_type;
	return qspi_autopolling(hqspi, &sCommand, &sConfig, Timeout);
}

/*
 *  ���������Ԥ��ؿ�
 */
static ER
QSPI_AutoPollingMemReady(void *h, uint32_t Timeout)
{
	QSPI_Handle_t *hqspi = h;
	QSPI_Command_t     sCommand;
	QSPI_AutoPolling_t sConfig;

	/*
	 *  WRITE IN PROGRESS��λ�Ԥ�
	 */
	sCommand.InstructionMode   = hqspi->Init.inst_type;
	sCommand.Instruction       = QSPI_STIG_OPCODE_RD_STAT_REG;
	sCommand.AddressMode       = QSPI_ADDRESS_NONE;
	sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	sCommand.DataMode          = hqspi->Init.inst_data_xfer_type;
	sCommand.DummyCycles       = 0;

	sConfig.Match           = 0x00;
	sConfig.Mask            = 0x01;
	sConfig.MatchMode       = QSPI_MATCH_MODE_AND;
	sConfig.StatusBytesSize = 1;
	sConfig.Interval        = 0x10;
	sConfig.AutomaticStop   = QSPI_AUTOMATIC_STOP_ENABLE;

	return qspi_autopolling(hqspi, &sCommand, &sConfig, Timeout);
}


/*
 *  �ᥤ�󥿥���
 */
void main_task(intptr_t exinf)
{
	QSPI_Handle_t *hqspi;
	ER_UINT	ercd;
	uint32_t i;
#ifdef TOPPERS_STM32F769_DISCOVERY
	const QSPI_Init_t *pini = &qspi_config_N25Q512A13GSF40E;
#else
	const QSPI_Init_t *pini = &qspi_config_N25Q128A13EF840E;
#endif

	SVC_PERROR(syslog_msk_log(LOG_UPTO(LOG_INFO), LOG_UPTO(LOG_EMERG)));
	syslog(LOG_NOTICE, "Sample program starts (exinf = %d).", (int_t) exinf);

	/*
	 *  ���ꥢ��ݡ��Ȥν����
	 *
	 *  �����ƥ����������Ʊ�����ꥢ��ݡ��Ȥ�Ȥ����ʤɡ����ꥢ��
	 *  �ݡ��Ȥ������ץ�Ѥߤξ��ˤϤ�����E_OBJ���顼�ˤʤ뤬���پ��
	 *  �ʤ���
	 */
	ercd = serial_opn_por(TASK_PORTID);
	if (ercd < 0 && MERCD(ercd) != E_OBJ) {
		syslog(LOG_ERROR, "%s (%d) reported by `serial_opn_por'.",
									itron_strerror(ercd), SERCD(ercd));
	}
	SVC_PERROR(serial_ctl_por(TASK_PORTID,
							(IOCTL_CRLF | IOCTL_FCSND | IOCTL_FCRCV)));

	hqspi = qspi_init(QSPI1_PORTID, (QSPI_Init_t *)pini);
	syslog_1(LOG_NOTICE, "## QSPI SIZE[%08x] ##", pini->device_size);
	syslog_1(LOG_NOTICE, "## hqspi[%08x] ##", hqspi);
	if(hqspi == NULL){
		syslog_0(LOG_ERROR, "## QSPI INIT ERROR ##");
		slp_tsk();
	}
	hqspi->semid = QSPI1_SEM;

	for(i = 0 ; i < BUFFERSIZE ; i++)
		aBuffer[i] = i;

//	ercd = qspi_mode_init(hqspi);
//	syslog_1(LOG_NOTICE, "## MODE INIT ercd(%d) ##", ercd);
//	if(ercd != E_OK)
//		slp_tsk();

#if 1
	ercd = qspi_erase_sector(hqspi, 0);
	syslog_1(LOG_NOTICE, "## ERASE ercd(%d) ##", ercd);
	if(ercd != E_OK)
		slp_tsk();
	dly_tsk(100);

	ercd = qspi_write(hqspi, 0, aBuffer, BUFFERSIZE);
	syslog_1(LOG_NOTICE, "## WRITE ercd(%d) ##", ercd);
	if(ercd != E_OK)
		slp_tsk();
	dly_tsk(100);
#endif

#if 1
	ercd = qspi_direct_enable(hqspi);
	syslog_1(LOG_NOTICE, "## ENABLE ercd(%d) ##", ercd);
	dly_tsk(20000);

	ercd = qspi_direct_disable(hqspi);
	syslog_1(LOG_NOTICE, "## DISABLE ercd(%d) ##", ercd);
	if(ercd != E_OK){
		slp_tsk();
	}
	dly_tsk(100);
#endif

	ercd = qspi_read(hqspi, aBuffer, 0x0, BUFFERSIZE);
	syslog_3(LOG_NOTICE, "############## aBuffer[%08x][%08x] ercd(%d) ##", aBuffer, i, ercd);
	syslog_4(LOG_NOTICE, "############## [%02x][%02x][%02x][%02x] ##", aBuffer[0], aBuffer[1], aBuffer[2], aBuffer[3]);
	dly_tsk(100);

	syslog_0(LOG_NOTICE, "## STOP ##");
	slp_tsk();
}
