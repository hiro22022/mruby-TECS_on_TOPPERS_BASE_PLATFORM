/*
 *  TOPPERS/ASP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Advanced Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2004-2012 by Embedded and Real-Time Systems Laboratory
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
 *  Copyright (C) 2016-2017 by TOPPERS PROJECT Educational Working Group.
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
 *    (b) �����ۤη��֤��̤�������ˡ�ˤ�äơ�TOPPERS�ץ��������Ȥ�
 *        ��𤹤뤳�ȡ�
 *  (4) �ܥ��եȥ����������Ѥˤ��ľ��Ū�ޤ��ϴ���Ū�������뤤���ʤ�»
 *      ������⡤�嵭����Ԥ����TOPPERS�ץ��������Ȥ����դ��뤳�ȡ�
 *      �ޤ����ܥ��եȥ������Υ桼���ޤ��ϥ���ɥ桼������Τ����ʤ���
 *      ͳ�˴�Ť����ᤫ��⡤�嵭����Ԥ����TOPPERS�ץ��������Ȥ�
 *      ���դ��뤳�ȡ�
 * 
 *  �ܥ��եȥ������ϡ�̵�ݾڤ��󶡤���Ƥ����ΤǤ��롥�嵭����Ԥ�
 *  ���TOPPERS�ץ��������Ȥϡ��ܥ��եȥ������˴ؤ��ơ�����λ�����Ū
 *  ���Ф���Ŭ������ޤ�ơ������ʤ��ݾڤ�Ԥ�ʤ����ޤ����ܥ��եȥ���
 *  �������Ѥˤ��ľ��Ū�ޤ��ϴ���Ū�������������ʤ�»���˴ؤ��Ƥ⡤��
 *  ����Ǥ�����ʤ���
 * 
 *  $Id: usbhtest.c 2416 2017-05-28 11:13:03Z roi $
 */

/* 
 *  USB HOST�ƥ��Ȥ�����
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
#include "usbhtest.h"

/*
 *  �����ӥ�������Υ��顼�Υ�������
 */
Inline void
svc_perror(const char *file, int_t line, const char *expr, ER ercd)
{
	if (ercd < 0) {
		t_perror(LOG_ERROR, file, line, expr, ercd);
	}
}

#define	SVC_PERROR(expr)	svc_perror(__FILE__, __LINE__, #expr, (expr))

USBH_HandleTypeDef hUSBHost;
uint8_t ukey;
uint8_t device_state[16];
Line_Coding_t CDC_Info = {
	115200,
	0,
	0,
	8
};
uint8_t send_buffer[] = "help\r\nhelp\r\n";
uint8_t receive_buffer[512] __attribute__ ((aligned (32)));
uint8_t scratch[512];

static uint32_t heap_area[4*1024];

uint32_t heap_param[2] = {
	(uint32_t)heap_area,
	(4*4*1024)
};

/*
 *  SW1�����
 */
void sw_int(void)
{
	ukey = 1;
	syslog_0(LOG_NOTICE, "## sw_int() ##");
}


/*
 *  USB HOST USER CALLBACK�ؿ�
 */
static void USBH_UserProcess(USBH_HandleTypeDef *phost, TUSBH_Device_t *pdevice, uint8_t id)
{
	device_state[pdevice->type] = id;
	if(pdevice->pClass != NULL){
		syslog_3(LOG_NOTICE, "## CLASS[%s](%d)(%d) ##", pdevice->pClass->Name, pdevice->type, id);
	}
}

/*
 *  MSC SECTOR READ
 */
static void MSC_Read_Operations(void){
	TUSBH_ERCODE  status = TUSBH_E_OK;
	char pdrv = 0;

	if(tusbhMscUnitIsReady (&hUSBHost, pdrv)){
		syslog_0(LOG_NOTICE, "READY");
	}
	else{
		syslog_0(LOG_NOTICE, "NOT READY");
	}
	status = tusbhMscRead(&hUSBHost, pdrv, 0, (uint8_t *)scratch, 1);
	syslog_1(LOG_NOTICE, "## status(%d) ##", status);
}

/*
 *  HID CALLBACK�ؿ�
 */
static void HID_Callback(TUSBH_Device_t *pdevice, uint8_t length, uint8_t *buffer)
{
	void *phandle = pdevice->pData;
	uint8_t type = tusbhHidGetType(pdevice);
	if(length == 0){
		if(type == HID_KEYBOARD_TYPE)
			HidKeyboardInit(phandle);
		else if(type == HID_MOUSE_TYPE)
			HidMouseInit(phandle);
	}
	else{
#if 1	/* ROI DEBUG */
		uint8_t *p = buffer;
		syslog_5(LOG_NOTICE, "## HID_Callback len(%d) [%02x][%02x][%02x][%02x] ##", length, p[0], p[1], p[2], p[3]);
#endif	/* ROI DEBUG */

		if(type == HID_KEYBOARD_TYPE){
			KEYBOARD_Info_t k_info;
			if(HidGetKeyboardInfo(phandle, &k_info, buffer, length) == TUSBH_E_OK){
				char c = HidGetASCIICode(&k_info);
				if(c != 0){
					syslog_1(LOG_NOTICE, "## KEY[%02x] ##", c);
				}
			}
		}
		else if(type == HID_MOUSE_TYPE){
			MOUSE_Info_t m_info;
			if(HidMouseInfo(phandle, &m_info, buffer, length) == TUSBH_E_OK){
				syslog_3(LOG_NOTICE, "button left(%d) right(%d) middle(%d)", m_info.button_left, m_info.button_right, m_info.button_middle);
				syslog_3(LOG_NOTICE, "position x (%d) y    (%d) z     (%d)", m_info.x, m_info.y, m_info.z);
			}
		}
	}
}

/*
 *  CDC CALLBACK�ؿ�
 */
static void CDC_Callback(TUSBH_Device_t *pdevice, uint8_t type)
{
	syslog_2(LOG_NOTICE, "## CDC_Callback[%08x](%d) ##", pdevice, type);
}


/*
 *  �ᥤ�󥿥���
 */
void main_task(intptr_t exinf)
{
	USB_OTG_Init_t USB_Data_Init;
	ER_UINT	ercd;
	TUSBH_ERCODE result;
	USB_OTG_Handle_t *husb;

	SVC_PERROR(syslog_msk_log(LOG_UPTO(LOG_INFO), LOG_UPTO(LOG_EMERG)));
	syslog(LOG_NOTICE, "Sample program starts (exinf = %d).", (int_t) exinf);
	Line_Coding_t CDC_data;
	uint32_t length;
	uint32_t count = 0;

	/*
	 *  ���ꥢ��ݡ��Ȥν����
	 *
	 *  �����ƥ������������Ʊ�����ꥢ��ݡ��Ȥ�Ȥ����ʤɡ����ꥢ��
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

	/*
	 *  USB OTG�����
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
	hUSBHost.pSysData = husb;
	tusbhInit(&hUSBHost, USBH_UserProcess, 0);

	tusbhSetHidCallBack(&hUSBHost, HID_Callback);
	tusbhSetCdcCallBack(&hUSBHost, CDC_Callback);

	/*
	 *  USB HOST��������
	 */
	result = tusbhStart(&hUSBHost);
	syslog_1(LOG_NOTICE, "## tusbhStart result(%d) ##", result);

	/*
	 *  USB HOST���ץ����
	 */
	while (1){
		if(device_state[MSC_CLASS] == HOST_USER_CLASS_ACTIVE){
			syslog_0(LOG_NOTICE, "Press User button to start");
			syslog_0(LOG_NOTICE, "read and write operations");
			/* KEY�Ԥ� */
			ukey = 0;
			while((ukey != 1) && (device_state[MSC_CLASS] != HOST_USER_DISCONNECTION)){
				dly_tsk(1);
			}
			MSC_Read_Operations();
			dly_tsk(400);
		}

		if(device_state[MSC_CLASS] == HOST_USER_DISCONNECTION){
			device_state[MSC_CLASS] = 0;
			syslog_0(LOG_ERROR, "MSC device disconnected!\n");
		}

		/*
		 *  CDC SERIAL����
		 */
		if(device_state[CDC_CLASS] == HOST_USER_CLASS_ACTIVE){
			static bool_t cdc_init = true;
			if(cdc_init){
				syslog_1(LOG_NOTICE, "## device_state(%d) ##", device_state[CDC_CLASS]);
				tusbhCdcSetLineCoding(&hUSBHost, 0, &CDC_Info);
				dly_tsk(100);
				tusbhCdcGetLineCoding(&hUSBHost, 0, &CDC_data);
				result = tusbhCdcSend(&hUSBHost, 0, send_buffer, sizeof(send_buffer));
				syslog_4(LOG_NOTICE, "CDC INFO baud(%d) format(%d) parity(%d) data(%d)", CDC_data.baudrate, CDC_data.stop_bits, CDC_data.parity, CDC_data.data_bits);
				syslog_1(LOG_NOTICE, "## send result(%d) ##", result);
				cdc_init = false;
			}
			length = 512;
			result = tusbhCdcReceive(&hUSBHost, 0, receive_buffer, &length, 500);
			syslog_3(LOG_NOTICE, "## tusbhCdcReceive status(%d) length(%d) buffer[%08x] ##", result, length, receive_buffer);
			if(length > 0){
				syslog_4(LOG_NOTICE, "## 00[%02x] 01[%02x] 02[%02x] 03[%02x] ##", receive_buffer[0], receive_buffer[1], receive_buffer[2], receive_buffer[3]);
				syslog_4(LOG_NOTICE, "## 04[%02x] 05[%02x] 06[%02x] 07[%02x] ##", receive_buffer[4], receive_buffer[5], receive_buffer[6], receive_buffer[7]);
				syslog_4(LOG_NOTICE, "## 08[%02x] 09[%02x] 10[%02x] 11[%02x] ##", receive_buffer[8], receive_buffer[9], receive_buffer[10], receive_buffer[11]);
				syslog_4(LOG_NOTICE, "## 12[%02x] 13[%02x] 14[%02x] 15[%02x] ##", receive_buffer[12], receive_buffer[13], receive_buffer[14], receive_buffer[15]);
			}
			if(length != 0 && count == 0){
				dly_tsk(1000);
				cdc_init = true;
			}
			count++;
		}
#if 1	/* ROI DEBUG */
		/*
		 *  PRINTER����
		 */
		if(device_state[PRT_CLASS] == HOST_USER_CLASS_ACTIVE){
			static bool_t prt_init = true;
			if(prt_init){
				syslog_1(LOG_NOTICE, "## [PRT]device_state(%d) ##", device_state[PRT_CLASS]);
				dly_tsk(1000);
				syslog_0(LOG_NOTICE, "## SEND EOJ ##");
				tusbhPrtEOJ(&hUSBHost, 0, 1, 2, 3);
				tusbhPrtPortID(&hUSBHost, 0, receive_buffer);
				syslog_1(LOG_NOTICE, "## PORTID(%d) ##", receive_buffer[0]);
				prt_init = false;
			}
		}
#endif	/* ROI DEBUG */

		dly_tsk(500);
	}
#if 1	/* ROI DEBUG */
	syslog_0(LOG_NOTICE, "## STOP ##");
	slp_tsk();
#endif	/* ROI DEBUG */

	syslog(LOG_NOTICE, "Sample program ends.");
//	SVC_PERROR(ext_ker());
}
