/*
 *  TOPPERS/ASP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Advanced Standard Profile Kernel
 *
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2004-2010 by Embedded and Real-Time Systems Laboratory
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
 *  $Id: spi_driver.h 2416 2016-02-12 10:32:30Z roi $
 */

/*
 *	SPI�ɥ饤�ФΥإå��ե�����
 */

#ifndef _SPI_DRIVER_H_
#define _SPI_DRIVER_H_

#ifdef __cplusplus
 extern "C" {
#endif

#include "device.h"
#include "spi.h"
#include "pinmode.h"

/*
 *  SD�����ɼ������
 */
#define SD_CARD_V11         0
#define SD_CARD_V20         1
#define SD_CARD_HC          2
#define MMC_CARD			3
#define SD_IO_CARD          4
#define HS_MM_CARD          5
#define SD_IO_COMBO_CARD    6
#define MMC_CARD_HC         7

#define PORT_HIGH           1
#define PORT_LOW            0

#define SDMODE_PROTECT      (1<<5)
#define BUFFERSIZE          140


#ifndef TOPPERS_MACRO_ONLY

/*
 *  SPI SETUP��¤�����
 */
typedef struct {
	int                     port;
	uint32_t                baud;
	ID                      spi_sem;
	uint8_t                 cs_pin;
	uint8_t                 rs_pin;
	uint8_t                 rst_pin;
	uint8_t                 cs2_pin;
	uint32_t                otype;
	uint32_t                pullmode;
} SPI_Setup_t;

/*
 *  SDCARD�ϥ�ɥ鹽¤�����
 */
typedef struct {
	SPI_Handle_t            *hspi;		/* spi�ϥ�ɥ� */
	uint32_t                RetryCount;	/* ��ȥ饤��� */
	uint32_t                cardtype;	/* �����ɥ����� */
	uint32_t                RCA;		/* RCA�� */
	uint8_t                 CSD[16];	/* CSD�� */
	uint8_t                 CID[16];	/* CID�� */
	ID                      spi_lock;
	volatile uint32_t       status;		/* ž������ */
	uint32_t                read_count;
	uint32_t                write_count;
	uint32_t                read_retry_count;
	uint32_t                write_retry_count;
	uint32_t                read_error;
	uint32_t                write_error;
}SDCARD_Handler_t;

/*
 *  SDCARD����¤��
 */
typedef struct {
	uint64_t                capacity;	/* ����(�Х���) */
	uint32_t                blocksize;	/* �֥�å������� */
	uint32_t                maxsector;	/* ���祻������ */
	uint16_t                RCA;		/* SD RCA */
	uint8_t                 cardtype;	/* �����ɼ��� */
	uint8_t                 status;		/* ���ơ����� */
}SDCARD_CardInfo_t;


/*
 *  �ؿ��Υץ�ȥ��������
 */

extern SPI_Handle_t *spi_start(SPI_Setup_t *psps);
extern void spi_end(SPI_Handle_t *hspi);

extern void sdcard_setspi(ID portid, SPI_Handle_t *hspi);
extern bool_t sdcard_init(ID portid);
extern SDCARD_Handler_t *sdcard_open(int id);
extern ER sdcard_close(SDCARD_Handler_t *hsd);
extern ER sdcard_getcardinfo(SDCARD_Handler_t *hsd, SDCARD_CardInfo_t *pCardInfo);
extern ER sdcard_checkCID(SDCARD_Handler_t *hsd);
extern ER sdcard_sendCSD(SDCARD_Handler_t *hsd);
extern ER sdcard_select_card(SDCARD_Handler_t *hsd, uint64_t addr);
extern ER sdcard_configuration(SDCARD_Handler_t *hsd);
extern ER sdcard_blockread(SDCARD_Handler_t *hsd, uint32_t *pbuf, uint64_t ReadAddr, uint32_t blocksize, uint32_t num);
extern ER sdcard_blockwrite(SDCARD_Handler_t *hsd, uint32_t *pbuf, uint64_t WriteAddr, uint32_t blocksize, uint32_t num);
extern ER sdcard_wait_transfar(SDCARD_Handler_t *hsd, uint32_t Timeout);
extern ER sdcard_dummy(SDCARD_Handler_t *hsd);

#endif /* TOPPERS_MACRO_ONLY */

#ifdef __cplusplus
}
#endif

#endif	/* _SPI_DRIVER_H_ */

