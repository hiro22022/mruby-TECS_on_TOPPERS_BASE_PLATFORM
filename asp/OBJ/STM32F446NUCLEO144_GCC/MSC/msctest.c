/*
 *  TOPPERS/ASP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Advanced Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2004-2012 by Embedded and Real-Time Systems Laboratory
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
 *  Copyright (C) 2017 by TOPPERS PROJECT Educational Working Group.
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
 *  $Id: media.c 2416 2012-09-07 08:06:20Z ertl-hiro $
 */

/* 
 *  MEDIA(MSC)�ƥ��Ȥ�����
 */

#include <kernel.h>
#include <t_syslog.h>
#include <t_stdlib.h>
#include <sil.h>
#include "syssvc/serial.h"
#include "syssvc/syslog.h"
#include "kernel_cfg.h"
#include <target_syssvc.h>
#include "device.h"
#include "storagedevice.h"
#include "fcntl.h"
#include "mscdiskio.h"
#include "monitor.h"
#include "msctest.h"

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

uint8_t ukey;
static uint8_t Buffer[512];
static uint32_t heap_area[4*1024];

uint32_t heap_param[2] = {
	(uint32_t)heap_area,
	(4*4*1024)
};

static int_t msc_read(int argc, char **argv);

/*
 *  MSC���ޥ�ɥơ��֥�
 */
static const COMMAND_INFO msc_command_info[] = {
	{"READ",		msc_read }		/* MSC SECTOR READ */
};

#define NUM_MSC_CMD      (sizeof(msc_command_info)/sizeof(COMMAND_INFO))

static const char msc_name[] = "MSC";
static const char msc_help[] =
"  Msc     READ     [no]\n";

static COMMAND_LINK msc_command_link = {
	NULL,
	NUM_MSC_CMD,
	msc_name,
	NULL,
	msc_help,
	&msc_command_info[0]
};

static int a2i(char *str)
{
	int num = 0;

	while(*str >= '0' && *str <= '9'){
		num = num * 10 + *str++ - '0';
	}
	return num;
}

/*
 *  MSC SECTOR READ
 */
static int_t
msc_read(int argc, char **argv)
{
	StorageDevice_t *psdev;
	USBH_HandleTypeDef *husbh;
	TUSBH_ERCODE       status;
	char               pdrv;
	int arg1, arg2, i;

	if(argc < 2)
		return -1;
	arg1 = a2i(argv[1]);
	arg2 = 1;
	if(arg1 < 0)
		return -1;
	if(argc >= 3)
		arg2 = a2i(argv[2]);

	psdev = SDMGetStorageDevice(MSC_DEVNO);
	husbh = (USBH_HandleTypeDef *)psdev->_sdev_local[1];
	pdrv  = psdev->_sdev_port - MSC_PORTID;
	while(arg2 > 0){
		status = tusbhMscRead(husbh, pdrv, arg1, (uint8_t *)Buffer, 1);
		printf("\nsec(%d) status(%d) !", arg1, status);
		if(status == TUSBH_E_OK){
			for(i = 0 ; i < 512 ; i++){
				if((i % 16) == 0)
					printf("\n%03x ", i);
				printf(" %02x", Buffer[i]);
			}
			printf("\n");
		}
		dly_tsk(100);
		arg1++;
		arg2--;
	}
	return 0;
}

/*
 *  SW1�����
 */
void sw_int(void)
{
	ukey = 1;
	syslog_0(LOG_NOTICE, "## sw_int() ##");
}

/*
 *  USB HOST INFORMATION CALLBACK
 *  parameter1:  Host Handler
 *  parameter2:  Device Handler
 *  parameter3:  Host state ID.
 */
static void USBH_UserProcess(USBH_HandleTypeDef *phost, TUSBH_Device_t *pdevice, uint8_t id)
{
	if(pdevice->pClass != NULL){
		syslog_3(LOG_NOTICE, "## CLASS[%s](%d)(%d) ##", pdevice->pClass->Name, pdevice->type, id);
	}
}

/*
 *  �ᥤ�󥿥���
 */
void main_task(intptr_t exinf)
{
	StorageDevice_t *psdev;
	USB_OTG_Init_t USB_Data_Init;
	ER_UINT	ercd;
	TUSBH_ERCODE result;
	USB_OTG_Handle_t *husb;
	USBH_HandleTypeDef *phusbh;

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

	dly_tsk(2000);

	/*
	 *  USB OTG�ϡ��ɤν����
	 */
    USB_Data_Init.usb_otg_mode = USB_OTG_MODE_HOST;
	USB_Data_Init.host_channels = 11;	/* HOST */
	USB_Data_Init.dma_enable = 0;
	USB_Data_Init.low_power_enable = 0;
	USB_Data_Init.phy_itface = USB_PHY_EMBEDDED; 
	USB_Data_Init.sof_enable = 0;
	USB_Data_Init.speed = USB_SPEED_FULL;
	USB_Data_Init.vbus_sensing_enable = 0;	/* HOST/DEV */
	USB_Data_Init.lpm_enable = 0;
	USB_Data_Init.use_external_vbus = 0;
	husb = usbo_init(USB1_PORTID, &USB_Data_Init);
#if 1	/* ROI DEBUG */
	if(husb == NULL){
		syslog_0(LOG_NOTICE, "## usbo_init error STOP ##");
		slp_tsk();
	}
#endif	/* ROI DEBUG */

	/*
	 *  USB HOST�ߥɥ륦��������
	 */
	phusbh = (USBH_HandleTypeDef *)malloc(sizeof(USBH_HandleTypeDef));
	phusbh->pSysData = husb;
	tusbhInit(phusbh, USBH_UserProcess, 0);

	psdev = SDMGetStorageDevice(MSC_DEVNO);
	psdev->_sdev_local[1] = phusbh;

	/*
	 *  USB HOST��������
	 */
	result = tusbhStart(phusbh);
	syslog_1(LOG_NOTICE, "## tusbhStart result(%d) ##", result);
	setup_command(&msc_command_link);

#if 1	/* ROI DEBUG */
	syslog_0(LOG_NOTICE, "## STOP ##");
	slp_tsk();
#endif	/* ROI DEBUG */

	syslog(LOG_NOTICE, "Sample program ends.");
//	SVC_PERROR(ext_ker());
}

