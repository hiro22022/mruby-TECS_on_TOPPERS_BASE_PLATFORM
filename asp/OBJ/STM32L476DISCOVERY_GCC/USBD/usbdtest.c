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
 *  $Id: usbdtest.c 2416 2017-05-28 11:02:20Z roi $
 */

/* 
 *  USB DEVICE�ƥ��Ȥ�����
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

#include "usb_otg.h"
#include "usbdtest.h"

#include "tusbd_base.h"
#include "tusbd_device.h" 

/*
 *  SIL�ؿ��Υޥ������
 */
#define sil_orw_mem(a, b)		sil_wrw_mem((a), sil_rew_mem(a) | (b))

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

#define CURSOR_STEP     5

USBD_HandleTypeDef USBD_Device;
uint8_t ukey;

uint32_t heap_param[2] = {
	(uint32_t)SRAM2_BASE,
	SRAM2_SIZE
};

/*
 *  SW1�����
 */
void sw_int(void)
{
	ukey = 1;
	syslog_0(LOG_NOTICE, "## sw_int() ##");
}

#if USB_DEVICE_ID == 0

uint8_t HID_Buffer[4];

/*
 *  HID MOUSE DEVICE CLASS REQUEST
 *  parameter1:  pbuf  �ǡ�������Хåե�
 */
static void GetPointerData(uint8_t *pbuf)
{
  static int8_t cnt = 0;
  int8_t  x = 0, y = 0 ;
  
  if(cnt++ > 0)
  {
    x = CURSOR_STEP;
  }
  else
  {
    x = -CURSOR_STEP;
  }
  pbuf[0]  = 0;
  pbuf[1] += x;
  pbuf[2] += y;
  pbuf[3]  = 0;
}
#else

#define APP_TX_DATA_SIZE   2048

#define FLG_CDC_SET_LINE_CODING         0x0001
#define FLG_CDC_GET_LINE_CODING         0x0002
#define FLG_CDC_SET_CONTROL_LINE_STATE  0x0004
#define FLG_CDC_SEND_BREAK              0x0008
#define FLG_CDC_RECEIVE_DATA           	0x0010
#define FLG_CDC_SEND_FLUSH              0x0100

static uint8_t    txBuffer[APP_TX_DATA_SIZE] __attribute__ ((aligned (32)));/* Received Data over UART (CDC interface) are stored in this buffer */
static uint8_t    *pTransRxBuffer;
static uint16_t   TransRxLength;
static uint16_t   TransRxCount;


Line_Coding_t LineCoding =
{
	115200, /* baud rate*/
	0x00,   /* stop bits-1*/
	0x00,   /* parity - none*/
	0x08    /* nb. of bits 8*/
};

/*
 *  CDC DEVICE CLASS REQUEST
 *  parameter1:  hcdc   CDC�ϥ�ɥ�
 *  parameter2:  cmd    �׵ᥳ�ޥ��
 *  parameter3:  length �ǡ���Ĺ
 */
static void CdcDeviceRequest(TUSBD_CDC_Handle_t *hcdc, uint8_t cmd, uint32_t length)
{
	switch (cmd){
	case CDC_SEND_ENCAPSULATED_COMMAND:
		syslog_2(LOG_NOTICE, "## CdcDeviceRequest CDC_SEND_ENCAPSULATED_COMMAND hcdc[%08x] length(%d) ##", hcdc, length);
		break;

	case CDC_GET_ENCAPSULATED_RESPONSE:
		syslog_2(LOG_NOTICE, "## CdcDeviceRequest CDC_GET_ENCAPSULATED_RESPONSE hcdc[%08x] length(%d) ##", hcdc, length);
		break;

	case CDC_SET_COMM_FEATURE:
		syslog_2(LOG_NOTICE, "## CdcDeviceRequest CDC_SET_COMM_FEATURE hcdc[%08x] length(%d) ##", hcdc, length);
		break;

	case CDC_GET_COMM_FEATURE:
		syslog_2(LOG_NOTICE, "## CDC_GET_COMM_FEATURE hcdc[%08x] length(%d) ##", hcdc, length);
		break;

	case CDC_CLEAR_COMM_FEATURE:
		syslog_2(LOG_NOTICE, "## CDC_CLEAR_COMM_FEATURE hcdc[%08x] length(%d) ##", hcdc, length);
		break;

	case CDC_SET_LINE_CODING:
		memcpy(&LineCoding, hcdc->cmddata, sizeof(Line_Coding_t));
		iset_flg(USBCDC_FLG, FLG_CDC_SET_LINE_CODING);
		break;

	case CDC_GET_LINE_CODING:
		memcpy(hcdc->cmddata, &LineCoding, sizeof(Line_Coding_t));
		break;

	case CDC_SET_CONTROL_LINE_STATE:
		iset_flg(USBCDC_FLG, FLG_CDC_SET_CONTROL_LINE_STATE);
		break;

	case CDC_SEND_BREAK:
		iset_flg(USBCDC_FLG, FLG_CDC_SEND_BREAK);
		break;

	case CDC_RECEIVED:
		pTransRxBuffer = hcdc->rxdata;
		TransRxLength = length;
		TransRxCount  = 0;
		iset_flg(USBCDC_FLG, FLG_CDC_RECEIVE_DATA);
		break;

	default:
		syslog_3(LOG_NOTICE, "## CdcDeviceRequest cmd[%02x] hcdc[%08x] length(%d) ##", cmd, hcdc, length);
		break;
	}
}
#endif


/*
 *  �ᥤ�󥿥���
 */
void main_task(intptr_t exinf)
{
	USB_OTG_Init_t USB_Data_Init;
	USB_OTG_Handle_t *pusb;
	ER_UINT	ercd;
	USBD_StatusTypeDef result;
	uint32_t tmp;

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

	/*
	 *  USB OTG�����
	 */
	/* Enable Power Clock*/
	sil_orw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_APB1ENR1), RCC_APB1ENR1_PWREN);
	tmp = sil_rew_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_APB1ENR1));

	/* Enable VddUSB */
	sil_orw_mem((uint32_t *)(TADR_PWR_BASE+TOFF_PWR_CR2), PWR_CR2_USV);

    USB_Data_Init.usb_otg_mode = USB_OTG_MODE_DEVICE;
	USB_Data_Init.dev_endpoints = 4;		/* DEV */
	USB_Data_Init.use_dedicated_ep1 = 0;	/* DEV */
	USB_Data_Init.dma_enable = 0;
//  USB_Data_Init.low_power_enable = 1;
	USB_Data_Init.low_power_enable = 0;
	USB_Data_Init.phy_itface = USB_PHY_EMBEDDED;
	USB_Data_Init.sof_enable = 0;
	USB_Data_Init.speed = USB_SPEED_FULL;
	USB_Data_Init.vbus_sensing_enable = 0;	/* HOST/DEV */
	USB_Data_Init.lpm_enable = 0;
	USB_Data_Init.use_external_vbus = 0;
	pusb = usbo_init(USB1_PORTID, &USB_Data_Init);

	/*
	 *  USB DEVICE�ߥɥ륦���������
	 */
	USBD_Device.pSysData = pusb;
	result = tusbdInit(&USBD_Device, 0);
	syslog_1(LOG_NOTICE, "## USBD_Init result(%d) ##", result);
	MakeUsbDescriptor(&USBD_Device);

#if USB_DEVICE_ID != 0
	USBD_Device.pUsrData = CdcDeviceRequest;
#endif

	/*
	 *  USB DEVICE��������
	 */
	result = tusbdStart(&USBD_Device);
	syslog_1(LOG_NOTICE, "## USBD_Start result(%d) ##", result);

#if 0	/* ROI DEBUG */
	while (1){
		dly_tsk(1000 /*200*/);
		GetPointerData(HID_Buffer);
		USBD_HID_SendReport(&USBD_Device, HID_Buffer, 4);
		syslog_1(LOG_NOTICE, "## dev_state(%d) ##", USBD_Device.dev_state);
	}
#else	/* ROI DEBUG */
	act_tsk(USBD_TASK);
#endif	/* ROI DEBUG */

#if 1	/* ROI DEBUG */
	syslog_0(LOG_NOTICE, "## STOP ##");
	slp_tsk();
#endif	/* ROI DEBUG */

	syslog(LOG_NOTICE, "Sample program ends.");
//	SVC_PERROR(ext_ker());
}


void usbd_task(intptr_t exinf)
{
	FLGPTN crcflag;
	ER ercd;

	while (1){
		ercd = twai_flg(USBCDC_FLG, 0xffff, TWF_ORW, &crcflag, 1000);
#if USB_DEVICE_ID == 0
		GetPointerData(HID_Buffer);
		if(USBD_HID_SendReport(&USBD_Device, HID_Buffer, 4) == TUSBD_E_OK)
			memset(&HID_Buffer, 0, sizeof(HID_Buffer));
#else
		if(ercd == E_OK){
			if((crcflag & FLG_CDC_RECEIVE_DATA) != 0 && TransRxLength > TransRxCount){
				pTransRxBuffer[TransRxLength] = 0;
				syslog_2(LOG_NOTICE, "## revlen(%d)[%s] ##", TransRxLength, pTransRxBuffer);
				memcpy(txBuffer, pTransRxBuffer, TransRxLength);
				tusbdCdcStartTransmit(&USBD_Device, txBuffer, TransRxLength);
				TransRxLength = 0;
				tusbdCdcSetReceivePacket(&USBD_Device);
			}
		}
#endif
		syslog_1(LOG_NOTICE, "## dev_state(%d) ##", USBD_Device.dev_state);
	}
}

