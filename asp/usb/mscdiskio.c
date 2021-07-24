/*
 *  TOPPERS/ASP/FMP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Advanced Standard Profile Kernel
 * 
 *  Copyright (C) 2008-2011 by Embedded and Real-Time Systems Laboratory
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
 *  Copyright (C) 2015-2017 by TOPPERS PROJECT Educational Working Group.
 * 
 *  �嵭����Ԥϡ��ʲ��� (1)��(4) �ξ�狼��Free Software Foundation 
 *  �ˤ�äƸ�ɽ����Ƥ��� GNU General Public License �� Version 2 �˵�
 *  �Ҥ���Ƥ���������������˸¤ꡤ�ܥ��եȥ��������ܥ��եȥ�����
 *  ����Ѥ�����Τ�ޤࡥ�ʲ�Ʊ���ˤ���ѡ�ʣ�������ѡ������ۡʰʲ���
 *  ���ѤȸƤ֡ˤ��뤳�Ȥ�̵���ǵ������롥
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
 * 
 *  �ܥ��եȥ������ϡ�̵�ݾڤ��󶡤���Ƥ����ΤǤ��롥�嵭����Ԥ�
 *  ���TOPPERS�ץ������Ȥϡ��ܥ��եȥ������˴ؤ��ơ�����Ŭ�Ѳ�ǽ����
 *  �ޤ�ơ������ʤ��ݾڤ�Ԥ�ʤ����ޤ����ܥ��եȥ����������Ѥˤ��ľ
 *  ��Ū�ޤ��ϴ���Ū�������������ʤ�»���˴ؤ��Ƥ⡤������Ǥ�����ʤ���
 * 
 *  @(#) $Id: mscdickio.c,v 1.1 2017/05/27 13:11:52 roi Exp $
 */

/*
 * �ͣӣ����ѥ��ȥ졼���ؿ��򥵥ݡ��Ȥ���
 * ����Ū�ʥ��ȥ졼���ؿ����Ǥ��ꡢɸ��饤�֥�ꥣ��ʬ���Ƥλ��Ѥ�˾�ޤ�����
 * 1. msc_init                 ���δؿ����ν�����ؿ�(bss����������ξ�礳�δؿ�������)
 * 2. msc_sense                MSC�Υ��󥹤�Ԥ�
 * 3. msc_diskstatus           MSC�Υ��ơ���������:�ʤ�
 * 4. msc_diskread             MSC���ɤ߹���:�ʤ�
 * 5. msc_diskwrite            MSC�ν񤭹���:�ʤ�
 * 6. msc_iocil                MSC��IO����:�ʤ�
 *
 */

#include "kernel_impl.h"
#include <stdlib.h>
#include <string.h>
#include <t_syslog.h>
#include <t_stdlib.h>
#include "fcntl.h"
#include "device.h"
#include "storagedevice.h"
#include "ff.h"
#include "diskio.h"
#include "usb_otg.h"
#include "mscdiskio.h"
#include "tusbh_msc.h"

static int msc_sense(void *psdev, bool_t on);
static int msc_diskstatus(void *psdev);
static int msc_diskread(void *pif, BYTE *Buffer, DWORD SectorNumber, BYTE SectorCount);
static int msc_diskwrite(void *pif, const BYTE *Buffer, DWORD SectorNumber, BYTE SectorCount);
static int msc_diskioctl(void *pif, BYTE Func, void* Buffer);

static const StorageDeviceFunc_t fatfsMscDeviceFunc = {
	msc_sense,
	msc_diskstatus,
	msc_diskstatus,
	(int(*)())msc_diskread,
	(int(*)())msc_diskwrite,
	(int(*)())msc_diskioctl
};

static MSC_Unit_t MSCLUNInfo;
static FATFS ActiveFatFsObj __attribute__ ((aligned (32)));

/*
 *  FatFs��SD�����ɥե�����ɥ饤�Фν����
 */
#if MSC_DEVNO == 0
void sd_init(intptr_t exinf)
#else
void msc_init(intptr_t exinf)
#endif
{
	StorageDevice_t *psdev;

	SDMSetupDevice(MSC_DEVNO, &psdev);
	psdev->pdevf            = (StorageDeviceFunc_t *)&fatfsMscDeviceFunc;
	psdev->pdevff           = (StorageDeviceFileFunc_t *)&fatfsSDeviceFileFunc;
	psdev->_sdev_secsize    = 512;
	psdev->_sdev_port       = MSC_PORTID;
#ifdef SDEV_INSWAIT_TIME
	psdev->_sdev_inswait    = SDEV_INSWAIT_TIME;
#else
	psdev->_sdev_inswait    = 0;
#endif
	psdev->_sdev_attribute |= SDEV_INSERTCHK|SDEV_CHKREMOVE;
	psdev->_sdev_local[0]   = &ActiveFatFsObj;
	psdev->_sdev_local[1]   = NULL;
}

/*
 *  MSC���󥹴ؿ�
 */
static int msc_sense(void *pif, BOOL on)
{
	StorageDevice_t  *psdev = pif;
	TUSBH_Handle_t   *husbh;
	TUSBH_ERCODE     status;
	bool_t   exist;
	char     pdrv;
	int      result = FR_DISK_ERR;

	pdrv  = psdev->_sdev_port - MSC_PORTID;
	husbh = (TUSBH_Handle_t *)psdev->_sdev_local[1];
	if(husbh == NULL)
		exist = 0;
	else
		exist = tusbhMscUnitIsReady(husbh, pdrv);
	if(on && !exist){
		f_mount(psdev->_sdev_devno, 0);
		psdev->_sdev_attribute &= ~SDEV_DEVERROR;
		return TRUE;
	}
	else if(!on && exist){
		psdev->_sdev_instimer += SENSE_TIME;
		status = tusbhMscGetLUNInfo(husbh, pdrv, &MSCLUNInfo);
		if(status != TUSBH_E_OK)
			psdev->_sdev_attribute |= SDEV_DEVERROR;
		else
			psdev->_sdev_maxsec = MSCLUNInfo.num_block;
		if((psdev->_sdev_attribute & SDEV_DEVERROR) == 0)
			result = f_mount(psdev->_sdev_devno, &ActiveFatFsObj);
		if(result != FR_OK)
			psdev->_sdev_attribute |= SDEV_DEVERROR;
		else
			psdev->_sdev_local[0]   = &ActiveFatFsObj;
#if 1
		syslog_3(LOG_NOTICE, "## attr[%04x] max(%d) result(%d) ##", psdev->_sdev_attribute, psdev->_sdev_maxsec, result);
#endif
		return TRUE;
	}
	else
		return FALSE;
}

/*
 *  FatFs��MSC���ơ������ؿ�
 */
static int msc_diskstatus(void *pif)
{
	TUSBH_Handle_t  *husbh;
	StorageDevice_t *psdev = pif;

	if(psdev == NULL)
		return STA_NODISK;
	husbh = (TUSBH_Handle_t *)psdev->_sdev_local[1];
	if((psdev->_sdev_attribute & (SDEV_EMPLOY|SDEV_NOTUSE)) != SDEV_EMPLOY || husbh == NULL)
		return STA_NOINIT;
	else
		return 0;
}

/*
 *  FatFs��MSC�ɤ߹��ߴؿ�
 */
static int msc_diskread(void *pif, BYTE *Buffer, DWORD SectorNumber, BYTE SectorCount)
{
 	StorageDevice_t   *psdev = pif;
	USB_OTG_Handle_t  *husb;
	TUSBH_Handle_t    *husbh;
	TUSBH_ERCODE      status;
	char              pdrv;
	int               retry = 0;
	uint8_t           *buff = NULL;
	unsigned int      align = ((unsigned int)Buffer) & 3;
	BYTE              *buffer = (BYTE *)Buffer;

	husbh = (TUSBH_Handle_t *)psdev->_sdev_local[1];
	husb  = (USB_OTG_Handle_t *)husbh->pSysData;
	pdrv  = psdev->_sdev_port - MSC_PORTID;
	if((psdev->_sdev_attribute & (SDEV_EMPLOY|SDEV_NOTUSE)) != SDEV_EMPLOY || husbh == NULL)
		return RES_ERROR;
	if(align != 0 && husb->Init.dma_enable == 1){
		uint32_t *addr = malloc_cache(psdev->_sdev_secsize*SectorCount);
		if(addr != NULL){
			buff   = buffer;
			buffer = (BYTE *)addr;
		}
	}
	do{
		status = tusbhMscRead(husbh, pdrv, SectorNumber, (uint8_t *)buffer, SectorCount);
		if(status == TUSBH_E_SYS)
			break;
		retry++;
	}while(status != TUSBH_E_OK && retry < RETRY_COUNT);
	if(buff != NULL){
		tusbmemcpy(buff, buffer, psdev->_sdev_secsize*SectorCount);
		free_cache(buffer);
	}
	if(status == TUSBH_E_OK)
		return RES_OK;
	else if(status == TUSBH_E_SYS)
		psdev->_sdev_attribute |= SDEV_DEVERROR;
	return RES_ERROR;
}

/*
 *  FatFs��MSC�񤭹��ߴؿ�
 */
static int msc_diskwrite(void *pif, const BYTE *Buffer, DWORD SectorNumber, BYTE SectorCount)
{
 	StorageDevice_t   *psdev = pif;
	USB_OTG_Handle_t  *husb;
	TUSBH_Handle_t    *husbh;
	TUSBH_ERCODE      status;
	char              pdrv;
	int               retry = 0;
	uint8_t           *buff = NULL;
	unsigned int      align = ((unsigned int)Buffer) & 3;
	BYTE              *buffer = (BYTE *)Buffer;

	husbh = (TUSBH_Handle_t *)psdev->_sdev_local[1];
	husb  = (USB_OTG_Handle_t *)husbh->pSysData;
	pdrv  = psdev->_sdev_port - MSC_PORTID;
	if((psdev->_sdev_attribute & (SDEV_EMPLOY|SDEV_NOTUSE)) != SDEV_EMPLOY || husbh == NULL)
		return RES_ERROR;
	if(align != 0 && husb->Init.dma_enable == 1){
		uint32_t *addr = malloc_cache(psdev->_sdev_secsize*SectorCount);
		if(addr != NULL){
			buff   = (uint8_t *)buffer;
			buffer = (BYTE *)addr;
			tusbmemcpy(buffer, buff, psdev->_sdev_secsize*SectorCount);
		}
	}
	do{
		status = tusbhMscWrite(husbh, pdrv, SectorNumber, (uint8_t *)buffer, SectorCount);
		if(status == TUSBH_E_SYS)
			break;
		retry++;
	}while(status != TUSBH_E_OK && retry < RETRY_COUNT);
	if(buff != NULL)
		free_cache(buffer);
	if(status == TUSBH_E_OK)
		return RES_OK;
	else if(status == TUSBH_E_SYS)
		psdev->_sdev_attribute |= SDEV_DEVERROR;
	return RES_ERROR;
}


/*
 *  FatFs��MSC-DISKIO����ؿ�
 */
static int msc_diskioctl(void *pif, BYTE Func, void* Buffer)
{
	StorageDevice_t *psdev = (StorageDevice_t *)pif;
	DRESULT         result;

	if(psdev == NULL)
		return RES_ERROR;
	if((psdev->_sdev_attribute & (SDEV_EMPLOY|SDEV_NOTUSE)) != SDEV_EMPLOY)
		return RES_ERROR;
	switch(Func){
	case CTRL_SYNC:
		result = RES_OK;			/* no action */
		break;
	case GET_SECTOR_COUNT:
		*((DWORD *)Buffer) = psdev->_sdev_maxsec;
		syslog_2(LOG_NOTICE, "ioctl notuse (%d)(%d) ", (int)Func, psdev->_sdev_maxsec);
		result = RES_OK;
		break;
	case GET_BLOCK_SIZE:
		*((DWORD *)Buffer) = 135;	/* ERASE_BLK */
		syslog_1(LOG_NOTICE, "call disk_ioctl(GET_BLOCK_SIZE, %08x)", (int)(*((DWORD *)Buffer)));
		result = RES_OK;
		break;
	default:
		syslog_2(LOG_NOTICE, "call disk_ioctl(%d, %08x)", (int)psdev->_sdev_devno, (int)Buffer);
		slp_tsk();
		result = RES_PARERR;
		break;
	}
	return result;
}

