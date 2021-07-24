/*
 *  TOPPERS/ASP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Advanced Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
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
 *  $Id: usb_otg.c 2416 2016-11-26 20:15:20Z ertl-hiro $
 */

/* 
 *  USB_OTG�ɥ饤�Ф�����
 */

#include "kernel_impl.h"
#include <t_syslog.h>
#include <t_stdlib.h>
#include <string.h>
#include <sil.h>
#include "syssvc/serial.h"
#include "syssvc/syslog.h"
#include "kernel_cfg.h"
#include <target_syssvc.h>
#include "device.h"
#include "usb_otg.h"

#define sil_orw_mem(a, b)		sil_wrw_mem((a), sil_rew_mem(a) | (b))
#define sil_andw_mem(a, b)		sil_wrw_mem((a), sil_rew_mem(a) & ~(b))
#define sil_andxw_mem(a, b)		sil_wrw_mem((a), sil_rew_mem(a) & (b))
#define sil_modw_mem(a, b, c)	sil_wrw_mem((a), (sil_rew_mem(a) & (~b)) | (c))

#define get_usbmode(a)          (sil_rew_mem((uint32_t *)(USBOTG_BASE((a))+TOFF_USBO_GINTSTS)) & 1)
#define get_int_status(a, b)    (sil_rew_mem((uint32_t *)(USBOTG_BASE((a))+TOFF_USBO_GINTSTS)) & (b))
#define hc_int_status(a, b, c)  (sil_rew_mem((uint32_t *)(USBHC_BASE((a), (b))+TOFF_USBHC_HCINT)) & (c))
#define get_haint_status(a)     (sil_rew_mem((uint32_t *)(USBH_BASE((a))+TOFF_USBH_HAINT)) & 0xFFFF)
#define clear_host_int(a, b)    sil_wrw_mem((uint32_t *)(USBOTG_BASE((a))+TOFF_USBO_GINTSTS), (b))
#define clear_dev_int(a, b)		sil_andxw_mem((uint32_t *)(USBOTG_BASE((a))+TOFF_USBO_GINTSTS), (b))
#define clear_hc_int(a, b, c)   sil_wrw_mem((uint32_t *)(USBHC_BASE((a), (b))+TOFF_USBHC_HCINT), (c))
#define clear_outep_int(a, b, c) sil_wrw_mem((uint32_t *)(USBOEP_BASE((a), (b))+TOFF_USBEO_DOEPINT), (c))
#define clear_inep_int(a, b, c) sil_wrw_mem((uint32_t *)(USBIEP_BASE((a), (b))+TOFF_USBEI_DIEPINT), (c))
#define get_inep_int_status(a)  ((sil_rew_mem((uint32_t *)(USBD_BASE((a))+TOFF_USBD_DAINT)) \
								& sil_rew_mem((uint32_t *)(USBD_BASE((a))+TOFF_USBD_DAINTMSK))) & 0xFFFF)
#define get_outep_int_status(a) (((sil_rew_mem((uint32_t *)(USBD_BASE((a))+TOFF_USBD_DAINT)) \
								& sil_rew_mem((uint32_t *)(USBD_BASE((a))+TOFF_USBD_DAINTMSK))) & 0xFFFF0000) >> 16)

#define get_packet_data(s)      ((s[0] & 0xff) | ((s[1] & 0xff)<<8) | ((s[2] & 0xff)<<16) | ((s[3] & 0xff)<<24))

/*
 *  USB�� AF10���
 */
#define GPIO_AF10_OTG_FS        ((uint8_t)0x0AU)  /* OTG_FS Alternate Function mapping */
#define GPIO_AF10_OTG_HS        ((uint8_t)0x0AU)  /* OTG_HS Alternate Function mapping */

/*
 *  �ǥե����FIFO����
 */
#define DEFAULT_FS_GRXFSIZ      0x80
#define DEFAULT_HS_GRXFSIZ      0x200
#define DEFAULT_FS_TXFIFO0_SIZ  0x60		/* 0x40 */
#define DEFAULT_HS_TXFIFO0_SIZ  0x100
#define DEFAULT_FS_TXFIFO1_SIZ  0x80		/* 0x40 */
#define DEFAULT_HS_TXFIFO1_SIZ  0xE0


/*
 *  �ѥ��åȥ��������
 */
#define USB_OTG_HS_MAX_PACKET_SIZE           512
#define USB_OTG_FS_MAX_PACKET_SIZE           64
#define USB_OTG_MAX_EP0_SIZE                 64

/*
 *  PHY���ȿ����
 */
#define DSTS_ENUMSPD_HS_PHY_30MHZ_OR_60MHZ     (0 << 1)
#define DSTS_ENUMSPD_FS_PHY_30MHZ_OR_60MHZ     (1 << 1)
#define DSTS_ENUMSPD_LS_PHY_6MHZ               (2 << 1)
#define DSTS_ENUMSPD_FS_PHY_48MHZ              (3 << 1)

/*
 * �ե졼�।�󥿡��Х����
 */
#define DCFG_FRAME_INTERVAL_80                 0
#define DCFG_FRAME_INTERVAL_85                 1
#define DCFG_FRAME_INTERVAL_90                 2
#define DCFG_FRAME_INTERVAL_95                 3

/*
 *  �ѥ��åȥ��ơ��������
 */
#define STS_GOUT_NAK                           1
#define STS_DATA_UPDT                          2
#define STS_XFER_COMP                          3
#define STS_SETUP_COMP                         4
#define STS_SETUP_UPDT                         6

/*
 *  PHY���ԡ������
 */
#define HPRT0_PRTSPD_HIGH_SPEED                0
#define HPRT0_PRTSPD_FULL_SPEED                1
#define HPRT0_PRTSPD_LOW_SPEED                 2

/*
 *  �ۥ��ȥ��饹����ɥݥ���ȥ��������
 */
#define HCCHAR_CTRL                            0
#define HCCHAR_ISOC                            1
#define HCCHAR_BULK                            2
#define HCCHAR_INTR                            3


#define GRXSTS_PKTSTS_IN                       2
#define GRXSTS_PKTSTS_IN_XFER_COMP             3
#define GRXSTS_PKTSTS_DATA_TOGGLE_ERR          5
#define GRXSTS_PKTSTS_CH_HALTED                7

#define RESET_TIMEOUT           (1000*100)		/* 1ms */
#define HALT_TIMEOUT            (10*100)		/* 0.01ms */
#if defined(TOPPERS_STM32L476_DISCOVERY)
#define USB_OTG_FS_WAKEUP_EXTI_LINE         0x00020000	/* External interrupt line 17 Connected to the USB EXTI Line */
#else
#define USB_OTG_FS_WAKEUP_EXTI_LINE         0x00040000  /* External interrupt line 18 Connected to the USB FS EXTI Line */
#endif

#define GHWCFG3_XFER_SIZE_CNTR_WIDTH           0x0000000f
#define GHWCFG3_PACKET_SIZE_CNTR_WIDTH         0x00000070
#define GHWCFG3_I2C                            0x00000100
#define GHWCFG3_DFIFO_DEPTH                    0xffff0000
#define GRXFSIZ_DEPTH                          0x0000ffff

/*
 *  USB�ݡ���ID��������֥�å�����Ф�����Υޥ���
 */
#define INDEX_USB(usbid)        ((uint_t)((usbid) - 1))

#ifndef TADR_USB_OTGHS_BASE
#define NUM_USBHANDLE   1
#else
#define NUM_USBHANDLE   NUM_USBPORT
#endif

#if !defined(_STM32F7XX_H_)
#define flushinvalidatedcache_by_addr(a, b)
#endif

static USB_OTG_Handle_t Husb[NUM_USBPORT];

extern ER sysclock_config(int mode);

/*
 *  ɬ�פʤ鶯���⡼�ɤ����ꤹ��
 *  �����⡼�ɤʤ�true���֤�
 */
static bool_t
usbo_force_mode_if_needed(USB_OTG_Handle_t *husb, bool_t host)
{
	uint32_t otgmode = get_usbmode(husb->base);
	uint32_t reqmode = husb->Init.usb_otg_mode;
	uint32_t gusbcfg;
	uint32_t set, clear;

	if (host && (otgmode == USB_OTG_MODE_HOST || reqmode == USB_OTG_MODE_DEVICE))
		return false;
	else if (!host && (otgmode == USB_OTG_MODE_DEVICE || reqmode == USB_OTG_MODE_HOST))
		return false;

	gusbcfg = sil_rew_mem((uint32_t *)(USBOTG_BASE(husb->base)+TOFF_USBO_GUSBCFG));
	set = host ? USB_OTG_GUSBCFG_FHMOD : USB_OTG_GUSBCFG_FDMOD;
	clear = host ? USB_OTG_GUSBCFG_FDMOD : USB_OTG_GUSBCFG_FHMOD;
	gusbcfg &= ~clear;
	gusbcfg |= set;
	sil_wrw_mem((uint32_t *)(USBOTG_BASE(husb->base)+TOFF_USBO_GUSBCFG), gusbcfg);

	dly_tsk(25);
	return true;
}

/*
 *  �����⡼�ɤ򥯥ꥢ����
 */
static void
usbo_clear_force_mode(USB_OTG_Handle_t *husb)
{
	uint32_t gusbcfg;

	gusbcfg = sil_rew_mem((uint32_t *)(USBOTG_BASE(husb->base)+TOFF_USBO_GUSBCFG));
	gusbcfg &= ~USB_OTG_GUSBCFG_FHMOD;
	gusbcfg &= ~USB_OTG_GUSBCFG_FDMOD;
	sil_wrw_mem((uint32_t *)(USBOTG_BASE(husb->base)+TOFF_USBO_GUSBCFG), gusbcfg);

	/*
	 *  �����ѹ��ˤ���Ԥ�
	 */
	dly_tsk(25);
}

/*
 *  USB-OTG�⡼�ɥ����å�
 */
static void
usbo_get_hwparam(USB_OTG_Handle_t *husb)
{
	USB_OTG_HwParam_t *hw = &husb->hw_params;
	uint32_t hwcfg3, grxfsiz;
	unsigned int width;

#ifdef TOFF_USBO_GHWCFG3
	hwcfg3 = sil_rew_mem((uint32_t *)(husb->base+TOFF_USBO_GHWCFG3));
#else
	hwcfg3 = 0x200d1e8;
#endif
	grxfsiz = sil_rew_mem((uint32_t *)(husb->base+TOFF_USBO_GRXFSIZ));
	syslog_2(LOG_NOTICE, "hwcfg3=%08x grxfsiz=%08x", hwcfg3, grxfsiz);

	hw->en_multiple_tx_fifo = 0;
	width = hwcfg3 & GHWCFG3_XFER_SIZE_CNTR_WIDTH;
	hw->max_transfer_size = (1 << (width + 11)) - 1;
	width = (hwcfg3 & GHWCFG3_PACKET_SIZE_CNTR_WIDTH) >> 4;
	hw->max_packet_count = (1 << (width + 4)) - 1;
	hw->total_fifo_size = (hwcfg3 & GHWCFG3_DFIFO_DEPTH) >> 16;
	hw->host_rx_fifo_size = grxfsiz & GRXFSIZ_DEPTH;

	syslog_1(LOG_NOTICE, "  max_transfer_size=%d", hw->max_transfer_size);
	syslog_1(LOG_NOTICE, "  max_packet_count=%d", hw->max_packet_count);
	syslog_1(LOG_NOTICE, "  en_multiple_tx_fifo=%d", hw->en_multiple_tx_fifo);
	syslog_1(LOG_NOTICE, "  total_fifo_size=%d", hw->total_fifo_size);
	syslog_1(LOG_NOTICE, "  host_rx_fifo_size=%d", hw->host_rx_fifo_size);
}

/*
 *  USB OTG �ʥ��ͥ⡼������
 */
static ER
usbo_exit_hibernation(USB_OTG_Handle_t *husb, bool_t restore)
{
	/*
	 *  ��⡼�ȥ����������åץ����ʥ륯�ꥢ
	 */
	sil_andw_mem((uint32_t *)(USBD_BASE(husb->base)+TOFF_USBD_DCTL), USB_OTG_DCTL_RWUSIG);
	return E_OK;
}

/*
 *  USB OTG �ʥ��ͥ⡼�ɰܹ�
 */
static ER
usbo_enter_hibernation(USB_OTG_Handle_t *husb)
{
	sil_orw_mem((uint32_t *)(USBPGC_BASE(husb->base)), USB_OTG_PCGCCTL_STOPCLK);
	/*Enter in STOP mode */
	if(husb->Init.low_power_enable){
		/* Set SLEEPDEEP bit and SleepOnExit of Cortex System Control Register */
		sil_orw_mem((uint32_t *)(TADR_SCB_BASE+TOFF_SCB_SCR), (SCB_SCR_SLEEPDEEP_Msk | SCB_SCR_SLEEPONEXIT_Msk));
	}
	return E_OK;
}

/*
 *  USB_OTG�ϡ��ɥ����������
 *  parameter1 portid USB�ݡ���ID
 *  parameter2 pini   �������¤�ΤؤΥݥ���
 *  return     NULL�Ǥʤ����USB�ϥ�ɥ�ؤΥݥ���
 */
USB_OTG_Handle_t *
usbo_init(ID portid, USB_OTG_Init_t *pini)
{
	GPIO_Init_t GPIO_Init_Data;
	USB_OTG_Handle_t *husb;
	uint32_t base;
	uint32_t no;
	volatile uint32_t tmpreg = 0x00U;

	if(portid < USB1_PORTID || portid > NUM_USBHANDLE)
		return NULL;

	no = INDEX_USB(portid);
	husb = &Husb[no];
	memcpy(&husb->Init, pini, sizeof(USB_OTG_Init_t));

	if(portid == USB1_PORTID){	/* USB OTG FS�ݡ��� */
		husb->base = TADR_USB_OTGFS_BASE;
		/*
		 *  GPIO/USB�ݡ��ȥ���å���ư
		 */
#if defined(TOPPERS_STM32L476_DISCOVERY)
		sil_orw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_AHB2ENR), RCC_AHB2ENR_GPIOAEN);
#else
		sil_orw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_AHB1ENR), RCC_AHB1ENR_GPIOAEN);
#if defined(TOPPERS_STM32F7_DISCOVERY)
		sil_orw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_AHB1ENR), RCC_AHB1ENR_GPIODEN);
#elif defined(TOPPERS_STM32F4_DISCOVERY)
		sil_orw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_AHB1ENR), RCC_AHB1ENR_GPIOCEN);
#else
		sil_orw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_AHB1ENR), RCC_AHB1ENR_GPIOGEN);
#endif
#endif

		/*USB DM and DP */
		GPIO_Init_Data.mode      = GPIO_MODE_AF;
		GPIO_Init_Data.pull      = GPIO_NOPULL;
		GPIO_Init_Data.otype     = GPIO_OTYPE_PP;
		GPIO_Init_Data.speed     = GPIO_SPEED_HIGH;
		GPIO_Init_Data.alternate = GPIO_AF10_OTG_FS;
		gpio_setup(TADR_GPIOA_BASE, &GPIO_Init_Data, 11);
		gpio_setup(TADR_GPIOA_BASE, &GPIO_Init_Data, 12);

		/*USB ID */
		GPIO_Init_Data.mode      = GPIO_MODE_AF;
		GPIO_Init_Data.pull      = GPIO_PULLUP;
		GPIO_Init_Data.otype     = GPIO_OTYPE_OD;
		GPIO_Init_Data.speed     = GPIO_SPEED_HIGH;
		GPIO_Init_Data.alternate = GPIO_AF10_OTG_FS;
		gpio_setup(TADR_GPIOA_BASE, &GPIO_Init_Data, 10);

#if !defined(TOPPERS_STM32F7_DISCOVERY)
		/* Configure VBUS Pin(DEVICE) */
		GPIO_Init_Data.mode      = GPIO_MODE_INPUT;
		GPIO_Init_Data.pull      = GPIO_NOPULL;
		gpio_setup(TADR_GPIOA_BASE, &GPIO_Init_Data, 9);
#endif

		/* Configure POWER_SWITCH IO pin(HOST) */
		GPIO_Init_Data.mode      = GPIO_MODE_OUTPUT;
		GPIO_Init_Data.pull      = GPIO_NOPULL;
		GPIO_Init_Data.otype     = GPIO_OTYPE_PP;
		GPIO_Init_Data.speed     = GPIO_SPEED_HIGH;
#if defined(TOPPERS_STM32F7_DISCOVERY)
		gpio_setup(TADR_GPIOD_BASE, &GPIO_Init_Data, 5);
#elif defined(TOPPERS_STM32F4_DISCOVERY)
		gpio_setup(TADR_GPIOC_BASE, &GPIO_Init_Data, 0);
#elif !defined(TOPPERS_STM32L476_DISCOVERY)
		gpio_setup(TADR_GPIOG_BASE, &GPIO_Init_Data, 6);
#endif

		/*
		 *  USB HS����å�����
		 */
		sil_orw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_AHB2ENR), RCC_AHB2ENR_OTGFSEN);
		sil_orw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_APB2ENR), RCC_APB2ENR_SYSCFGEN);
		tmpreg = sil_rew_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_APB2ENR));

		/*
		 *  FIFO������
		 */
		usbo_get_hwparam(husb);
		husb->fifo_mem            = husb->hw_params.total_fifo_size;
		husb->pcd_rx_fifo_sz      = DEFAULT_FS_GRXFSIZ;
		husb->pcd_np_g_tx_fifo_sz = DEFAULT_FS_TXFIFO0_SIZ;
		husb->pcd_tx_fifo_sz[1]   = DEFAULT_FS_TXFIFO1_SIZ;
	}
#ifdef TADR_USB_OTGHS_BASE
	else{				/* USB OTG HS�ݡ��� */
		husb->base = TADR_USB_OTGHS_BASE;
		/*
		 *  GPIO/USB�ݡ��ȥ���å���ư
		 */
		sil_orw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_AHB1ENR), RCC_AHB1ENR_GPIOAEN);
		sil_orw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_AHB1ENR), RCC_AHB1ENR_GPIOBEN);
		sil_orw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_AHB1ENR), RCC_AHB1ENR_GPIOCEN);
		sil_orw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_AHB1ENR), RCC_AHB1ENR_GPIOHEN);
#if defined(TOPPERS_STM32F769_DISCOVERY)
		sil_orw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_AHB1ENR), RCC_AHB1ENR_GPIOIEN);
#endif

		/* CLK */
		GPIO_Init_Data.mode      = GPIO_MODE_AF;
		GPIO_Init_Data.pull      = GPIO_NOPULL;
		GPIO_Init_Data.otype     = GPIO_OTYPE_PP;
		GPIO_Init_Data.speed     = GPIO_SPEED_HIGH;
		GPIO_Init_Data.alternate = GPIO_AF10_OTG_HS;
		gpio_setup(TADR_GPIOA_BASE, &GPIO_Init_Data, 5);

		/* D0 */
		gpio_setup(TADR_GPIOA_BASE, &GPIO_Init_Data, 3);

		/* D1 D2 D3 D4 D5 D6 D7 */
		gpio_setup(TADR_GPIOB_BASE, &GPIO_Init_Data, 0);
		gpio_setup(TADR_GPIOB_BASE, &GPIO_Init_Data, 1);
		gpio_setup(TADR_GPIOB_BASE, &GPIO_Init_Data, 5);
		gpio_setup(TADR_GPIOB_BASE, &GPIO_Init_Data, 10);
		gpio_setup(TADR_GPIOB_BASE, &GPIO_Init_Data, 11);
		gpio_setup(TADR_GPIOB_BASE, &GPIO_Init_Data, 12);
		gpio_setup(TADR_GPIOB_BASE, &GPIO_Init_Data, 13);

		/* STP */
		gpio_setup(TADR_GPIOC_BASE, &GPIO_Init_Data, 0);

		/* NXT */
		gpio_setup(TADR_GPIOH_BASE, &GPIO_Init_Data, 4);

		/* DIR */
#if defined(TOPPERS_STM32F769_DISCOVERY)
		gpio_setup(TADR_GPIOI_BASE, &GPIO_Init_Data, 11);
#else
		gpio_setup(TADR_GPIOC_BASE, &GPIO_Init_Data, 2);
#endif

		/*
		 *  USB HS-PHY����å�����
		 */
		sil_orw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_AHB1ENR), RCC_AHB1ENR_OTGHSULPIEN);
		tmpreg = sil_rew_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_AHB1ENR));

		/*
		 *  USB HS����å�����
		 */
		sil_orw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_AHB1ENR), RCC_AHB1ENR_OTGHSEN);
		tmpreg = sil_rew_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_APB1ENR));

		/*
		 *  FIFO������
		 */
		usbo_get_hwparam(husb);
		husb->fifo_mem            = husb->hw_params.total_fifo_size;
		husb->pcd_rx_fifo_sz      = DEFAULT_HS_GRXFSIZ;
		husb->pcd_np_g_tx_fifo_sz = DEFAULT_HS_TXFIFO0_SIZ;
		husb->pcd_tx_fifo_sz[1]   = DEFAULT_HS_TXFIFO1_SIZ;
	}
#endif
	((void)(tmpreg));

	/* DEVICE */
	if(husb->Init.low_power_enable == 1){
		/*
		 *  WAKEUP���������
		 */
		sil_wrw_mem((uint32_t *)(TADR_EXTI_BASE+TOFF_EXTI_PR), USB_OTG_FS_WAKEUP_EXTI_LINE);
		sil_andw_mem((uint32_t *)(TADR_EXTI_BASE+TOFF_EXTI_FTSR), USB_OTG_FS_WAKEUP_EXTI_LINE);
		sil_orw_mem((uint32_t *)(TADR_EXTI_BASE+TOFF_EXTI_RTSR), USB_OTG_FS_WAKEUP_EXTI_LINE);
		sil_orw_mem((uint32_t *)(TADR_EXTI_BASE+TOFF_EXTI_IMR), USB_OTG_FS_WAKEUP_EXTI_LINE);
	}

	base = husb->base;
	/*
	 *  USB OTG�����Х����߶ػ�
	 */
	usbo_disableglobalint(husb);

	/*
	 *  USB OTG�����������
	 */
	usbo_coreinit(husb);

	/*
	 *  USB OTG�⡼������
	 */
	usbo_setcurrentmode(husb);

	/*
	 * VBUS Sensing B��ͭ������
	 */
#ifdef USB_OTG_GCCFG_VBDEN
	sil_orw_mem((uint32_t *)(USBOTG_BASE(base)+TOFF_USBO_GCCFG), USB_OTG_GCCFG_VBDEN);
	if(husb->Init.usb_otg_mode != USB_OTG_MODE_HOST){
		if(husb->Init.vbus_sensing_enable == 0){
			sil_andw_mem((uint32_t *)(USBOTG_BASE(base)+TOFF_USBO_GCCFG), USB_OTG_GCCFG_VBDEN);
			sil_orw_mem((uint32_t *)(USBOTG_BASE(base)+TOFF_USBO_GOTGCTL), USB_OTG_GOTGCTL_BVALOEN);
			sil_orw_mem((uint32_t *)(USBOTG_BASE(base)+TOFF_USBO_GOTGCTL), USB_OTG_GOTGCTL_BVALOVAL);
		}
	}
#else
	if(husb->Init.usb_otg_mode != USB_OTG_MODE_HOST){
		sil_orw_mem((uint32_t *)(USBOTG_BASE(base)+TOFF_USBO_GCCFG), USB_OTG_GCCFG_VBUSBSEN);
		if(husb->Init.vbus_sensing_enable == 0){
			sil_orw_mem((uint32_t *)(USBOTG_BASE(base)+TOFF_USBO_GCCFG), USB_OTG_GCCFG_NOVBUSSENS);
		}
	}
	else{
		sil_andw_mem((uint32_t *)(USBOTG_BASE(base)+TOFF_USBO_GCCFG), USB_OTG_GCCFG_VBUSASEN);
		sil_andw_mem((uint32_t *)(USBOTG_BASE(base)+TOFF_USBO_GCCFG), USB_OTG_GCCFG_VBUSBSEN);
		sil_orw_mem((uint32_t *)(USBOTG_BASE(base)+TOFF_USBO_GCCFG), USB_OTG_GCCFG_NOVBUSSENS);
	}
#endif

	/*
	 *  �ꥹ������PHY����å�
	 */
	sil_wrw_mem((uint32_t *)(USBPGC_BASE(base)), 0);

	/*
	 *  GINT�Υ�����ޥ�������
	 */
	sil_wrw_mem((uint32_t *)(USBOTG_BASE(base)+TOFF_USBO_GINTMSK), 0);

	/*
	 *  GINT���������ߥ��ꥢ
	 */
	sil_wrw_mem((uint32_t *)(USBOTG_BASE(base)+TOFF_USBO_GINTSTS), 0xFFFFFFFF);
	return husb;
}

/*
 *  USB OTG�ϡ��ɥ��������
 *  parameter1 husb  USB�ϥ�ɥ�ؤΥݥ���
 *  return     ���E_OK
 */
ER
usbo_deinit(USB_OTG_Handle_t *husb)
{
	sil_andw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_AHB2ENR), RCC_AHB2ENR_OTGFSEN);
//	sil_andw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_APB2ENR), RCC_APB2ENR_SYSCFGEN);
	return E_OK;
}

/*
 *  USB OTG���������
 *  parameter1 husb  USB�ϥ�ɥ�ؤΥݥ���
 *  return     ���E_OK
 */
ER
usbo_setupint(USB_OTG_Handle_t *husb)
{
	uint32_t base = husb->base;
	uint32_t intmsk = USB_OTG_GINTMSK_PXFRM_IISOOXFRM | USB_OTG_GINTMSK_WUIM;

	/*
	 *  ����߶ػ�
	 */
	sil_wrw_mem((uint32_t *)(USBOTG_BASE(base)+TOFF_USBO_GINTMSK), 0);

	/*
	 *  ����ߥڥ�ǥ��󥰥��ꥢ
	 */
	sil_wrw_mem((uint32_t *)(USBOTG_BASE(base)+TOFF_USBO_GINTSTS), 0xFFFFFFFF);

	/*
	 *  DMA����ߵ���
	 */
	if(husb->Init.dma_enable == 0){
		intmsk |= USB_OTG_GINTMSK_RXFLVLM;
	}

	/*
	 *  USB HOST����ߵ���
	 */
	if(husb->Init.usb_otg_mode != USB_OTG_MODE_DEVICE){
		intmsk |= (USB_OTG_GINTMSK_PRTIM | USB_OTG_GINTMSK_HCIM |\
				   USB_OTG_GINTMSK_SOFM  | USB_OTG_GINTSTS_DISCINT);
	}
	/*
	 *  USB DEVICE����ߵ���
	 */
	if(husb->Init.usb_otg_mode != USB_OTG_MODE_HOST){
		intmsk |= (USB_OTG_GINTMSK_USBSUSPM | USB_OTG_GINTMSK_USBRST |\
                    USB_OTG_GINTMSK_ENUMDNEM | USB_OTG_GINTMSK_IEPINT |\
                    USB_OTG_GINTMSK_OEPINT   | USB_OTG_GINTMSK_IISOIXFRM);
	}

	/*
	 *  SOF����ߵ�������
	 */
	if(husb->Init.sof_enable){
		intmsk |= USB_OTG_GINTMSK_SOFM;
	}

	/*
	 *  SENSE ENABLE����ߵ�������
	 */
	if(husb->Init.vbus_sensing_enable == 1){
		intmsk |= (USB_OTG_GINTMSK_SRQIM | USB_OTG_GINTMSK_OTGINT);
	}
	sil_wrw_mem((uint32_t *)(USBOTG_BASE(base)+TOFF_USBO_GINTMSK), intmsk);
	return E_OK;
}

/*
 *  USB�������ꥻ�å�
 *  parameter1 husb  USB���å��١������ɥ쥹
 *  return     ���ｪλ��E_OK �����ॢ���ȡ�E_TMOUT
 */
static ER
USB_CoreReset(uint32_t  base)
{
	uint32_t tick = 0;

	/*
	 *  AHB�ޥ�������IDLE���ơ����Ԥ�
	 */
	do{
		if(++tick > RESET_TIMEOUT)
			return E_TMOUT;
		sil_dly_nse(10*1000);
	}
	while ((sil_rew_mem((uint32_t *)(USBOTG_BASE(base)+TOFF_USBO_GRSTCTL)) & USB_OTG_GRSTCTL_AHBIDL) == 0);

	/*
	 *  ���������եȥꥻ�å��׵�
	 */
	tick = 0;
	sil_orw_mem((uint32_t *)(USBOTG_BASE(base)+TOFF_USBO_GRSTCTL), USB_OTG_GRSTCTL_CSRST);
	do{
		if(++tick > RESET_TIMEOUT)
			return E_TMOUT;
		sil_dly_nse(10*1000);
	}
	while ((sil_rew_mem((uint32_t *)(USBOTG_BASE(base)+TOFF_USBO_GRSTCTL)) & USB_OTG_GRSTCTL_CSRST) != 0);
	return E_OK;
}

/*
 *  �ѥ��åȥǡ����񤭹���
 *  parameter1 base  USB OTG�쥸�����١������ɥ쥹
 *  parameter2 src   �񤭹��ߥǡ����Υݥ���
 *  parameter3 ch_ep_num ����ɥݥ���ȡ��ޤ��ϡ�����ͥ��ֹ�
 *  parameter4 len   �ǡ���Ĺ
 *  parameter5 dma   DMA�⡼��(0:none-DMA��1:DMA)
 */
static void
usbo_writePacket(uint32_t base, uint8_t *src, uint8_t ch_ep_num, uint16_t len, uint8_t dma)
{
	uint32_t count32b = 0 , i = 0;

	if(dma == 0U){
		count32b =  (len + 3) / 4;
		for(i = 0 ; i < count32b; i++, src += 4){
			sil_wrw_mem((uint32_t *)(USBD_FIFO(base, ch_ep_num)), get_packet_data(src));
		}
	}
}

/*
 *  �ѥ��åȥǡ����ɤ߹���
 *  parameter1 base USB OTG�쥸�����١������ɥ쥹
 *  parameter2 dest �ɤ߹��ߥǡ����Υݥ���
 *  parameter3 len  �ǡ���Ĺ
 */
static void *
usbo_readPacket(uint32_t base, uint8_t *dest, uint16_t len)
{
	uint32_t i=0U;
	uint32_t count32b = (len + 3U) / 4U;
	uint32_t data;

	for(i = 0 ; i < count32b; i++, dest += 4){
		data = sil_rew_mem((uint32_t *)(USBD_FIFO(base, 0)));
		dest[0] = (data & 0xff);
		dest[1] = (data >> 8) & 0xff;
		dest[2] = (data >> 16) & 0xff;
		dest[3] = (data >> 24) & 0xff;
	}
	return ((void *)dest);
}

/*
 *  USB HOST�Υ����ȥե졼���ֹ����Ф�
 *  parameter1 USB�ϥ�ɥ�ؤΥݥ���
 *  return     �ϥ�ɥ��ֹ�
*/
uint32_t
usbo_getcurrentframe(USB_OTG_Handle_t *husb)
{
	return (sil_rew_mem((uint32_t *)(USBH_BASE(husb->base)+TOFF_USBH_HFNUM)) & USB_OTG_HFNUM_FRNUM);
}

/*
 *  USB OTG�ε�ǽ�����Ԥ�
 *  parameter1 husb  USB�ϥ�ɥ�ؤΥݥ���
 *     usb_otg_mode�����ꡧUSB_OTG_MODE_DEVICE(USB DEVICE����)
 *                         USB_OTG_MODE_HOST(USB HOST����)
 *                         USB_OTG_MODE_DRD(�ǥ奢��⡼��)
 *  return     ���E_OK
 */
ER
usbo_setcurrentmode(USB_OTG_Handle_t *husb)
{
	uint32_t base = husb->base;
	uint32_t gusbcfg = sil_rew_mem((uint32_t *)(USBOTG_BASE(base)+TOFF_USBO_GUSBCFG));

	gusbcfg &= ~(USB_OTG_GUSBCFG_FHMOD | USB_OTG_GUSBCFG_FDMOD);

	if(husb->Init.usb_otg_mode == USB_OTG_MODE_HOST){
		gusbcfg |= USB_OTG_GUSBCFG_FHMOD;
	}
	else if(husb->Init.usb_otg_mode == USB_OTG_MODE_DEVICE){
		gusbcfg |= USB_OTG_GUSBCFG_FDMOD;
	}
	sil_wrw_mem((uint32_t *)(USBOTG_BASE(base)+TOFF_USBO_GUSBCFG), gusbcfg);
	dly_tsk(50);
	return E_OK;
}

/*
 *  USB�����ν����
 *  parameter1 husb  USB�ϥ�ɥ�ؤΥݥ���
 *  return     ���E_OK
 */
ER
usbo_coreinit(USB_OTG_Handle_t *husb)
{
	uint32_t base = husb->base;

	if(husb->Init.phy_itface == USB_PHY_ULPI){	/* ULPI-PHY select */
		sil_andw_mem((uint32_t *)(USBOTG_BASE(base)+TOFF_USBO_GCCFG), USB_OTG_GCCFG_PWRDWN);

		/*
		 *  ULPI-PHY�����
		 */
		sil_andw_mem((uint32_t *)(USBOTG_BASE(base)+TOFF_USBO_GUSBCFG), (USB_OTG_GUSBCFG_TSDPS | USB_OTG_GUSBCFG_ULPIFSLS | USB_OTG_GUSBCFG_PHYSEL));

		/*
		 *  VBUS����������
		 */
		sil_andw_mem((uint32_t *)(USBOTG_BASE(base)+TOFF_USBO_GUSBCFG), (USB_OTG_GUSBCFG_ULPIEVBUSD | USB_OTG_GUSBCFG_ULPIEVBUSI));
		if(husb->Init.use_external_vbus == 1){
			sil_orw_mem((uint32_t *)(USBOTG_BASE(base)+TOFF_USBO_GUSBCFG), USB_OTG_GUSBCFG_ULPIEVBUSD);
		}
		/*
		 *  �����ꥻ�å�
		 */
		USB_CoreReset(base); 
	}
	else{	/* Embedded PHY select */
		/*
		 *  Embedded FS-PHY����
		 */
		sil_orw_mem((uint32_t *)(USBOTG_BASE(base)+TOFF_USBO_GUSBCFG), USB_OTG_GUSBCFG_PHYSEL);

		/*
		 *  �����ꥻ�å�
		 */
		USB_CoreReset(base);

		/*
		 *  �Ÿ���������
		 */
		sil_wrw_mem((uint32_t *)(USBOTG_BASE(base)+TOFF_USBO_GCCFG), USB_OTG_GCCFG_PWRDWN);
	}
 
	/*
	 *  DMA�⡼������
	 */
	if(husb->Init.dma_enable == 1){
		sil_orw_mem((uint32_t *)(USBOTG_BASE(base)+TOFF_USBO_GAHBCFG), USB_OTG_GAHBCFG_DMAEN);
	}
	return E_OK;
}

/*
 *  USB�����Х����ߵ���
 *  parameter1 husb  USB�ϥ�ɥ�ؤΥݥ���
 *  return     ���E_OK
 */
ER
usbo_enableglobalint(USB_OTG_Handle_t *husb)
{
	sil_orw_mem((uint32_t *)(USBOTG_BASE(husb->base)+TOFF_USBO_GAHBCFG), USB_OTG_GAHBCFG_GINT);
	return E_OK;
}

/*
 *  USB�����Х����߶ػ�
 *  parameter1 husb  USB�ϥ�ɥ�ؤΥݥ���
 *  return     ���E_OK
 */
ER
usbo_disableglobalint(USB_OTG_Handle_t *husb)
{
	sil_andw_mem((uint32_t *)(USBOTG_BASE(husb->base)+TOFF_USBO_GAHBCFG), USB_OTG_GAHBCFG_GINT);
	return E_OK;
}

/*
 *  ����FIFO�Υե�å���
 *  parameter1 husb  USB�ϥ�ɥ�ؤΥݥ���
 *  return     ���ｪλ��E_OK�������ॢ���ȡ�E_TMOUT
 */
ER
usbo_flushTxFifo(USB_OTG_Handle_t *husb, uint32_t num)
{
	uint32_t base = husb->base;
	uint32_t tick = 0;

	sil_wrw_mem((uint32_t *)(USBOTG_BASE(base)+TOFF_USBO_GRSTCTL), (USB_OTG_GRSTCTL_TXFFLSH |(uint32_t)(num << 6)));
	do{
		if(++tick > RESET_TIMEOUT)
			return E_TMOUT;
		sil_dly_nse(10*1000);
	}while ((sil_rew_mem((uint32_t *)(USBOTG_BASE(base)+TOFF_USBO_GRSTCTL)) & USB_OTG_GRSTCTL_TXFFLSH) != 0);
	return E_OK;
}

/*
 *  ����FIFO�Υե�å���
 *  parameter1 husb  USB�ϥ�ɥ�ؤΥݥ���
 *  return     ���ｪλ��E_OK�������ॢ���ȡ�E_TMOUT
 */
ER
usbo_flushRxFifo(USB_OTG_Handle_t *husb)
{
	uint32_t base = husb->base;
	uint32_t tick = 0;

	sil_wrw_mem((uint32_t *)(USBOTG_BASE(base)+TOFF_USBO_GRSTCTL), USB_OTG_GRSTCTL_RXFFLSH);
	do{
		if(++tick > RESET_TIMEOUT)
			return E_TMOUT;
		sil_dly_nse(10*1000);
	}while((sil_rew_mem((uint32_t *)(USBOTG_BASE(base)+TOFF_USBO_GRSTCTL)) & USB_OTG_GRSTCTL_RXFFLSH) != 0);

#ifdef USB_OTG_GCCFG_VBDEN
	sil_orw_mem((uint32_t *)(USBOTG_BASE(base)+TOFF_USBO_GCCFG), USB_OTG_GCCFG_VBDEN);
#endif
	return E_OK;
}

/*
 *  FIFO�ν����
 *  parameter1 husb  USB�ϥ�ɥ�ؤΥݥ���
 *  return     ���E_OK
 */
ER
usbo_initFiFo(USB_OTG_Handle_t *husb)
{
	unsigned int ep;
	unsigned int addr;
	uint32_t base = husb->base;
	uint32_t val;

	/*
	 *  RX/NPTX FIFO����������
	 */
	sil_wrw_mem((uint32_t *)(USBOTG_BASE(base)+TOFF_USBO_GRXFSIZ), husb->pcd_np_g_tx_fifo_sz);
	sil_wrw_mem((uint32_t *)(USBOTG_BASE(base)+TOFF_USBO_DIEPTXF0_HNPTXFSIZ),
			(husb->pcd_np_g_tx_fifo_sz<<16) | husb->pcd_rx_fifo_sz);

	/*
	 *  ���٤Ƥ�TX FIFO����
	 */
	addr = husb->pcd_rx_fifo_sz + husb->pcd_np_g_tx_fifo_sz;
	for(ep = 1; ep < MAX_EPS_CHANNELS; ep++) {
		if(husb->pcd_tx_fifo_sz[ep] == 0){
			sil_wrw_mem((uint32_t *)(USBOTG_BASE(base)+TOFF_USBO_DIEPTXF+(ep-1)*4), 0);
			continue;
		}
		val = addr | husb->pcd_tx_fifo_sz[ep] << 16;
		if((addr + husb->pcd_tx_fifo_sz[ep]) > husb->fifo_mem)
			syslog_1(LOG_ERROR, "insufficient fifo memory(%d)", ep);
		addr += husb->pcd_tx_fifo_sz[ep];
		sil_wrw_mem((uint32_t *)(USBOTG_BASE(base)+TOFF_USBO_DIEPTXF+(ep-1)*4), val);
	}

	/*
	 * FIFO�ե�å������
	 */
	usbo_flushTxFifo(husb, 0x10);
	usbo_flushRxFifo(husb);
	return E_OK;
}

/*******************************************************************************
                       USB HOST�ؿ�
*******************************************************************************/
/*
 *  USB HOST�����
 *  parameter1 husb  USB�ϥ�ɥ�ؤΥݥ���
 *  return     ���E_OK
 */
ER
usbo_hostinit(USB_OTG_Handle_t *husb)
{
	uint32_t base = husb->base;
	bool_t forced;
	uint32_t val, i;

	/* Disable the FS/LS support mode only */
	if((husb->Init.speed == USB_SPEED_FULL) && (base != TADR_USB_OTGFS_BASE)){
		sil_orw_mem((uint32_t *)(USBH_BASE(base)+TOFF_USBH_HCFG), USB_OTG_HCFG_FSLSS);
	}
	else{
		sil_andw_mem((uint32_t *)(USBH_BASE(base)+TOFF_USBH_HCFG), USB_OTG_HCFG_FSLSS);
	}

	/*
	 *  HOST RX-FIFO����������
	 */
	forced = usbo_force_mode_if_needed(husb, true);
	sil_wrw_mem((uint32_t *)(USBOTG_BASE(base)+TOFF_USBO_GRXFSIZ), husb->pcd_np_g_tx_fifo_sz);
	sil_wrw_mem((uint32_t *)(USBOTG_BASE(base)+TOFF_USBO_DIEPTXF0_HNPTXFSIZ),
			(husb->pcd_np_g_tx_fifo_sz<<16) | husb->pcd_rx_fifo_sz);
	val = (husb->pcd_rx_fifo_sz + husb->pcd_np_g_tx_fifo_sz) | husb->pcd_tx_fifo_sz[1] << 16;
	sil_wrw_mem((uint32_t *)(USBOTG_BASE(base)+TOFF_USBO_HPTXFSIZ), val);

	/*
	 *  FIFO�ե�å���
	 */
	usbo_flushTxFifo(husb, 0x10U); /* all Tx FIFOs */
	usbo_flushRxFifo(husb);
	if(forced)
		usbo_clear_force_mode(husb);

	/*
	 * ����ͥ����ߥ��ꥢ�ȥޥ���
	 */
	for (i = 0 ; i < husb->Init.host_channels; i++){
		sil_wrw_mem((uint32_t *)(USBHC_BASE(base, i)+TOFF_USBHC_HCINT), 0xFFFFFFFFU);
		sil_wrw_mem((uint32_t *)(USBHC_BASE(base, i)+TOFF_USBHC_HCINTMSK), 0U);
	}

	/*
	 *  L3���ơ�������
	 */
	husb->lx_state = DWC2_L0;

	/*
	 *  HOST VBUS����
	 */
	usbo_drivevbus(husb, 1);
	dly_tsk(200U);
	usbo_setupint(husb);
	return E_OK;
}

/*
 *  HOST���������׵�
 *  parameter1 husb  USB�ϥ�ɥ�ؤΥݥ���
 *  return     ���E_OK
 */
ER
usbo_starthost(USB_OTG_Handle_t *husb)
{
	if(husb->Init.usb_otg_mode == USB_OTG_MODE_DRD
			&& husb->lx_state != DWC2_L0){
		usbo_hostinit(husb);
		dly_tsk(25);
		usbo_devdisconnect(husb);
		dly_tsk(3);
		usbo_devconnect(husb);
	}
	husb->lx_state = DWC2_L0;
	return E_OK;
}

/*
 *  HOST��������׵�
 *  parameter1 husb  USB�ϥ�ɥ�ؤΥݥ���
 *  return     ���E_OK
 */
ER
usbo_stophost(USB_OTG_Handle_t *husb)
{
	uint32_t base = husb->base;
	uint32_t tick = 0;
	int      i;

	usbo_disableglobalint(husb);

	/*
	 *  FIFO�ե�å���
	 */
	if(husb->Init.usb_otg_mode == USB_OTG_MODE_HOST){
		usbo_flushTxFifo(husb, 0x10U);
		usbo_flushRxFifo(husb);
	}

	/*
	 *  HOST����ͥ륭�塼���ꥢ
	 */
	for(i = 0 ; i <= 15 ; i++){
		sil_modw_mem((uint32_t *)(USBHC_BASE(base, i)+TOFF_USBHC_HCCHAR), (USB_OTG_HCCHAR_CHENA|USB_OTG_HCCHAR_EPDIR), USB_OTG_HCCHAR_CHDIS);
	}

	/*
	 *  HOST����ͥ�HALT
	 */
	for(i = 0, tick = 0 ; i <= 15 ; i++){
		sil_modw_mem((uint32_t *)(USBHC_BASE(base, i)+TOFF_USBHC_HCCHAR), USB_OTG_HCCHAR_EPDIR, (USB_OTG_HCCHAR_CHDIS|USB_OTG_HCCHAR_CHENA));
		do{
			if(++tick > HALT_TIMEOUT)
				break;
			sil_dly_nse(10*1000);
		}while((sil_rew_mem((uint32_t *)(USBHC_BASE(base, i)+TOFF_USBHC_HCCHAR)) & USB_OTG_HCCHAR_CHENA) != 0);
	}

	/*
	 *  USB�ۥ����Ѥγ���ߥ��ꥢ
	 */
	sil_wrw_mem((uint32_t *)(USBH_BASE(base)+TOFF_USBH_HAINT), 0xFFFFFFFFU);
	sil_wrw_mem((uint32_t *)(USBOTG_BASE(base)+TOFF_USBO_GINTSTS), 0xFFFFFFFFU);
	husb->lx_state = DWC2_L3;
	return E_OK;
}

/*
 *  HOST����ͥ�ν����
 *  parameter1 husb   USB�ϥ�ɥ�ؤΥݥ���
 *  parameter2 ch_num ����ͥ��ֹ�(1-15)
 *  parameter3 epnum  ����ɥݥ�����ֹ�(1-15)
 *  return     ���E_OK
 */
ER
usbo_hc_init(USB_OTG_Handle_t *husb, uint8_t ch_num, uint8_t epnum)
{
	uint32_t base = husb->base;
	uint8_t  address = husb->hc[ch_num].dev_addr;
	uint8_t  speed   = husb->hc[ch_num].speed;
	uint8_t  ep_type = husb->hc[ch_num].ep_type;
	uint16_t mps     = husb->hc[ch_num].max_packet;
	uint32_t hcint   = 0;

	/*
	 *  ����ͥ�γ���ߥ��ꥢ
	 */
	sil_wrw_mem((uint32_t *)(USBHC_BASE(base, ch_num)+TOFF_USBHC_HCINT), 0xFFFFFFFFU);

	/*
	 *  ����ɥݥ���ȷ��˽��������
	 */
	switch (ep_type) {
	case EP_TYPE_CTRL:
	case EP_TYPE_BULK:
		hcint = USB_OTG_HCINTMSK_XFRCM  | USB_OTG_HCINTMSK_STALLM |\
                USB_OTG_HCINTMSK_TXERRM | USB_OTG_HCINTMSK_DTERRM |\
                USB_OTG_HCINTMSK_AHBERR | USB_OTG_HCINTMSK_NAKM;

		if(epnum & 0x80U){
			hcint |= USB_OTG_HCINTMSK_BBERRM;
		}
		else{
			if(base != TADR_USB_OTGFS_BASE){
				hcint |= USB_OTG_HCINTMSK_NYET | USB_OTG_HCINTMSK_ACKM;
			}
		}
		break;

	case EP_TYPE_INTR:
		hcint = USB_OTG_HCINTMSK_XFRCM  | USB_OTG_HCINTMSK_STALLM |\
                USB_OTG_HCINTMSK_TXERRM | USB_OTG_HCINTMSK_DTERRM |\
                USB_OTG_HCINTMSK_NAKM   | USB_OTG_HCINTMSK_AHBERR | USB_OTG_HCINTMSK_FRMORM;

		if(epnum & 0x80)
			hcint |= USB_OTG_HCINTMSK_BBERRM;
		break;
	case EP_TYPE_ISOC:
			hcint = USB_OTG_HCINTMSK_XFRCM  | USB_OTG_HCINTMSK_ACKM |\
                    USB_OTG_HCINTMSK_AHBERR | USB_OTG_HCINTMSK_FRMORM;

		if(epnum & 0x80)
			hcint |= USB_OTG_HCINTMSK_TXERRM | USB_OTG_HCINTMSK_BBERRM;
		break;
	}
	sil_wrw_mem((uint32_t *)(USBHC_BASE(base, ch_num)+TOFF_USBHC_HCINTMSK), hcint);

	/*
	 *  HOST����ͥ����ߥޥ������
	 */
	sil_orw_mem((uint32_t *)(USBH_BASE(base)+TOFF_USBH_HAINTMSK), (1 << ch_num));

	/*
	 *  HOST����ͥ����ߵ���
	 */
	sil_orw_mem((uint32_t *)(USBOTG_BASE(base)+TOFF_USBO_GINTMSK), USB_OTG_GINTMSK_HCIM);

	/*
	 *  HCCHAR�쥸��������
	 */
	sil_wrw_mem((uint32_t *)(USBHC_BASE(base, ch_num)+TOFF_USBHC_HCCHAR), (((address << 22U) & USB_OTG_HCCHAR_DAD)  |\
                             (((epnum & 0x7FU)<< 11U) & USB_OTG_HCCHAR_EPNUM)|\
                             ((((epnum & 0x80U) == 0x80U)<< 15U) & USB_OTG_HCCHAR_EPDIR)|\
                             (((speed == USB_SPEED_LOW)<< 17U) & USB_OTG_HCCHAR_LSDEV)|\
                             ((ep_type << 18U) & USB_OTG_HCCHAR_EPTYP)|\
                             (mps & USB_OTG_HCCHAR_MPSIZ)));

	if(ep_type == EP_TYPE_INTR){
		sil_orw_mem((uint32_t *)(USBHC_BASE(base, ch_num)+TOFF_USBHC_HCCHAR), USB_OTG_HCCHAR_ODDFRM);
	}
	return E_OK; 
}

/*
 *  HOST�������ԡ��ɤ���Ф�
 *  parameter1 husb  USB�ϥ�ɥ�ؤΥݥ���
 *  return     speed USB_SPEED_HIGH(High speed)
 *                   USB_SPEED_FULL(Full speed)
 *                   USB_SPEED_LOW(Low speed)
 */
uint32_t
usbo_gethostspeed(USB_OTG_Handle_t *husb)
{
	volatile uint32_t hprt0;

	hprt0 = sil_rew_mem((uint32_t*)(USBPRT_BASE(husb->base)));
	return ((hprt0 & USB_OTG_HPRT_PSPD) >> 17);
}

/*
 *  HOST����ͥ������׵�
 *  parameter1 husb  USB�ϥ�ɥ�ؤΥݥ���
 *  parameter2 ch_num ����ͥ��ֹ�(1-15)
 *  return     ���E_OK
 */
ER
usbo_hc_startxfer(USB_OTG_Handle_t *husb, uint8_t ch_num)
{
	USB_OTG_HCTypeDef *hc = &husb->hc[ch_num];
	uint32_t base = husb->base;
	uint8_t  is_oddframe = 0U; 
	uint16_t len_words = 0U;   
	uint16_t num_packets = 0U;
	uint16_t max_hc_pkt_count = 256U;

	if((base != TADR_USB_OTGFS_BASE) && (hc->speed == USB_SPEED_HIGH)){
		if((husb->Init.dma_enable == 0) && (hc->do_ping == 1)){
			/*
			 * Do Ping
			 */
			sil_wrw_mem((uint32_t *)(USBHC_BASE(base, hc->ch_num)+TOFF_USBHC_HCTSIZ), ((1 << 19) & USB_OTG_HCTSIZ_PKTCNT) | USB_OTG_HCTSIZ_DOPING);
			sil_modw_mem((uint32_t *)(USBHC_BASE(base, hc->ch_num)+TOFF_USBHC_HCCHAR), USB_OTG_HCCHAR_CHDIS, USB_OTG_HCCHAR_CHENA);
			return E_OK;
		}
		else if(husb->Init.dma_enable == 1){
			sil_andw_mem((uint32_t *)(USBHC_BASE(base, hc->ch_num)+TOFF_USBHC_HCINTMSK), (USB_OTG_HCINTMSK_NYET | USB_OTG_HCINTMSK_ACKM));
			hc->do_ping = 0U;
		}
	}

	/*
	 *  �ѥ��åȿ���ž��Ĺ�η׻�
	 */
	if(hc->xfer_len > 0){
		num_packets = (hc->xfer_len + hc->max_packet - 1U) / hc->max_packet;

		if(num_packets > max_hc_pkt_count){
			num_packets = max_hc_pkt_count;
			hc->xfer_len = num_packets * hc->max_packet;
		}
	}
	else{
		num_packets = 1;
	}
	if(hc->ep_is_in){
		hc->xfer_len = num_packets * hc->max_packet;
	}

	/*
	 *  HCTSIZ�쥸��������
	 */
	sil_wrw_mem((uint32_t *)(USBHC_BASE(base, hc->ch_num)+TOFF_USBHC_HCTSIZ), (((hc->xfer_len) & USB_OTG_HCTSIZ_XFRSIZ)) |\
		((num_packets << 19U) & USB_OTG_HCTSIZ_PKTCNT) | (((hc->data_pid) << 29U) & USB_OTG_HCTSIZ_DPID));

	if(husb->Init.dma_enable){
		flushinvalidatedcache_by_addr(((uint8_t *)hc->xfer_buff), hc->xfer_len);
		sil_wrw_mem((uint32_t *)(USBHC_BASE(base, hc->ch_num)+TOFF_USBHC_HCDMA), (uint32_t)hc->xfer_buff);
	}

	/*
	 *  EVEN/ODD�ѥ��å�����
	 */
	is_oddframe = (sil_rew_mem((uint32_t *)(USBH_BASE(base)+TOFF_USBH_HFNUM)) & 0x01U) ? 0U : 1U;
	sil_andw_mem((uint32_t *)(USBHC_BASE(base, hc->ch_num)+TOFF_USBHC_HCCHAR), USB_OTG_HCCHAR_ODDFRM);
	sil_orw_mem((uint32_t *)(USBHC_BASE(base, hc->ch_num)+TOFF_USBHC_HCCHAR), (is_oddframe << 29U));

	/*
	 *  HOST����ͥ��ͭ����
	 */
	sil_modw_mem((uint32_t *)(USBHC_BASE(base, hc->ch_num)+TOFF_USBHC_HCCHAR), USB_OTG_HCCHAR_CHDIS, USB_OTG_HCCHAR_CHENA);

	if(husb->Init.dma_enable == 0){	/* none DMA�⡼�� */
		if((hc->ep_is_in == 0U) && (hc->xfer_len > 0U)){
			switch(hc->ep_type) {
			case EP_TYPE_CTRL:	/* NON PERIODICž�� */
			case EP_TYPE_BULK:
				len_words = (hc->xfer_len + 3) / 4;
				/*
				 *  FIFOĹ��Ķ�����硢NPTXFEMPTY����ߤ�����
				 */
				if(len_words > (sil_rew_mem((uint32_t *)(USBOTG_BASE(base)+TOFF_USBO_HNPTXSTS)) & 0xFFFFU)){
					sil_orw_mem((uint32_t *)(USBOTG_BASE(base)+TOFF_USBO_GINTMSK), USB_OTG_GINTMSK_NPTXFEM);
				}
				break;
			case EP_TYPE_INTR:	/* PERIODIC ž�� */
			case EP_TYPE_ISOC:
				len_words = (hc->xfer_len + 3) / 4;
				/*
				 *  FIFOĹ��Ķ�����硢PTXFEMPTY����ߤ�����
				 */
				if(len_words > (sil_rew_mem((uint32_t *)(USBH_BASE(base)+TOFF_USBH_HPTXSTS)) & 0xFFFFU)){ /* split the transfer */
					sil_orw_mem((uint32_t *)(USBOTG_BASE(base)+TOFF_USBO_GINTMSK), USB_OTG_GINTMSK_PTXFEM);
				}
				break;
			default:
				break;
			}

			/*
			 *  �ѥ��åȽ񤭹���
			 */
			usbo_writePacket(base, hc->xfer_buff, hc->ch_num, hc->xfer_len, 0);
		}
	}
	return E_OK;
}

/*
 *  HOST����ͥ��HALT����
 *  parameter1 husb  USB�ϥ�ɥ�ؤΥݥ���
 *  parameter2 ����ͥ��ֹ�(1-15)
 *  return     ���E_OK
 */
ER
usbo_hc_halt(USB_OTG_Handle_t *husb, uint8_t ch_num)
{
	uint32_t base = husb->base;
	uint32_t tick = 0;

	/* HCCHAR����ȥ���⡼�� */
	if((((sil_rew_mem((uint32_t *)(USBHC_BASE(base, ch_num)+TOFF_USBHC_HCCHAR)) & USB_OTG_HCCHAR_EPTYP) >> 18) == HCCHAR_CTRL)
			|| ((((sil_rew_mem((uint32_t *)(USBHC_BASE(base, ch_num)+TOFF_USBHC_HCCHAR)) & USB_OTG_HCCHAR_EPTYP) >> 18) == HCCHAR_BULK))){
		sil_orw_mem((uint32_t *)(USBHC_BASE(base, ch_num)+TOFF_USBHC_HCCHAR), USB_OTG_HCCHAR_CHDIS);
		if((sil_rew_mem((uint32_t *)(USBOTG_BASE(base)+TOFF_USBO_HNPTXSTS)) & 0xFF0000U) == 0){
			sil_andw_mem((uint32_t *)(USBHC_BASE(base, ch_num)+TOFF_USBHC_HCCHAR), USB_OTG_HCCHAR_CHENA);
			sil_orw_mem((uint32_t *)(USBHC_BASE(base, ch_num)+TOFF_USBHC_HCCHAR), USB_OTG_HCCHAR_CHENA);
			do{
				if(++tick > HALT_TIMEOUT)
					break;
				sil_dly_nse(10*1000);
			}while((sil_rew_mem((uint32_t *)(USBHC_BASE(base, ch_num)+TOFF_USBHC_HCCHAR)) & USB_OTG_HCCHAR_CHENA) != 0);
		}
		else{
			sil_orw_mem((uint32_t *)(USBHC_BASE(base, ch_num)+TOFF_USBHC_HCCHAR), USB_OTG_HCCHAR_CHENA);
		}
	}
	else{	/* ����¾ */
		sil_orw_mem((uint32_t *)(USBHC_BASE(base, ch_num)+TOFF_USBHC_HCCHAR), USB_OTG_HCCHAR_CHDIS);
		if((sil_rew_mem((uint32_t *)(USBH_BASE(base)+TOFF_USBH_HPTXSTS)) & 0xFFFF) == 0){
			sil_andw_mem((uint32_t *)(USBHC_BASE(base, ch_num)+TOFF_USBHC_HCCHAR), USB_OTG_HCCHAR_CHENA);
			sil_orw_mem((uint32_t *)(USBHC_BASE(base, ch_num)+TOFF_USBHC_HCCHAR), USB_OTG_HCCHAR_CHENA);
			do{
				if(++tick > HALT_TIMEOUT) 
					break;
				sil_dly_nse(10*1000);
			}while((sil_rew_mem((uint32_t *)(USBHC_BASE(base, ch_num)+TOFF_USBHC_HCCHAR)) & USB_OTG_HCCHAR_CHENA) != 0);
		}
		else{
			sil_orw_mem((uint32_t *)(USBHC_BASE(base, ch_num)+TOFF_USBHC_HCCHAR), USB_OTG_HCCHAR_CHENA);
		}
	}
	return E_OK;
}

/*
 *  USB OTG HOST�ݡ��ȥꥻ�å�
 *  parameter1 husb  USB�ϥ�ɥ�ؤΥݥ���
 *  return     ���E_OK
 */
ER
usbo_resetport(USB_OTG_Handle_t *husb)
{
	uint32_t base = husb->base;
	volatile uint32_t hprt0;

	hprt0 = sil_rew_mem((uint32_t*)(USBPRT_BASE(base)));
	hprt0 &= ~(USB_OTG_HPRT_PENA | USB_OTG_HPRT_PCDET | USB_OTG_HPRT_PENCHNG | USB_OTG_HPRT_POCCHNG);

	sil_wrw_mem((uint32_t*)(USBPRT_BASE(base)), (USB_OTG_HPRT_PRST | hprt0));
	dly_tsk (10);				/* See Note #1 */
	sil_wrw_mem((uint32_t*)(USBPRT_BASE(base)), (~(USB_OTG_HPRT_PRST) & hprt0));
	return E_OK;
}

/*
 *  USB HOST VBUS��ͭ����̵������
 *  parameter1 husb  USB�ϥ�ɥ�ؤΥݥ���
 *  parameter2 state 1:VBUS-ACTIVE 0:VBUS-INACTIVE
 *  return     ���E_OK
 */
ER
usbo_drivevbus(USB_OTG_Handle_t *husb, uint8_t state)
{
	uint32_t base = husb->base;
	volatile uint32_t hprt0;

	hprt0 = sil_rew_mem((uint32_t*)(USBPRT_BASE(base)));
	hprt0 &= ~(USB_OTG_HPRT_PENA | USB_OTG_HPRT_PCDET | USB_OTG_HPRT_PENCHNG | USB_OTG_HPRT_POCCHNG);

	if(((hprt0 & USB_OTG_HPRT_PPWR) == 0) && (state == 1)){
		sil_wrw_mem((uint32_t*)(USBPRT_BASE(base)), (USB_OTG_HPRT_PPWR | hprt0));
	}
	if(((hprt0 & USB_OTG_HPRT_PPWR) != 0) && (state == 0)){
		sil_wrw_mem((uint32_t*)(USBPRT_BASE(base)), ((~USB_OTG_HPRT_PPWR) & hprt0));
	}
	return E_OK; 
}

/*
 *  HOST EXTERNAL VBUS��ͭ����̵������
 *  parameter1 husb  USB�ϥ�ɥ�ؤΥݥ���
 *  parameter2 state 0:VBUS-ACTIVE 1:VBUS-INACTIVE
 */
void
usbo_driveextvbus(USB_OTG_Handle_t *husb, uint8_t state)
{
#if defined(TOPPERS_STM32F7_DISCOVERY)
	if(husb->base == TADR_USB_OTGFS_BASE){
		if(state == 0)
			sil_wrw_mem((uint32_t *)(TADR_GPIOD_BASE+TOFF_GPIO_BSRR), (1<<(5+16)));
		else
			sil_wrw_mem((uint32_t *)(TADR_GPIOD_BASE+TOFF_GPIO_BSRR), (1<<(5)));
	}
#elif defined(TOPPERS_STM32F746_NUCLEO144) || defined(TOPPERS_STM32F767_NUCLEO144)
 	if(state == 0)
		sil_wrw_mem((uint32_t *)(TADR_GPIOG_BASE+TOFF_GPIO_BSRR), (1<<(6+16)));
	else
		sil_wrw_mem((uint32_t *)(TADR_GPIOG_BASE+TOFF_GPIO_BSRR), (1<<(6)));
#elif defined(TOPPERS_STM32F4_DISCOVERY)
	if(state == 1)
		sil_wrh_mem((uint16_t *)(TADR_GPIOC_BASE+TOFF_GPIO_BSRRH), (1<<(0)));
	else
		sil_wrh_mem((uint16_t *)(TADR_GPIOC_BASE+TOFF_GPIO_BSRRL), (1<<(0)));
#elif !defined(TOPPERS_STM32F769_DISCOVERY) && !defined(TOPPERS_STM32L476_DISCOVERY)
	if(state == 0)
		sil_wrh_mem((uint16_t *)(TADR_GPIOG_BASE+TOFF_GPIO_BSRRH), (1<<(6)));
	else
		sil_wrh_mem((uint16_t *)(TADR_GPIOG_BASE+TOFF_GPIO_BSRRL), (1<<(6)));
#endif
}

/*
 *  FSLSPClkSel����
 *  parameter1 husb  USB�ϥ�ɥ�ؤΥݥ���
 *  parameter2 freq ����å����ȿ�(48MHz,6MHz�Τ�ͭ��)
 */
static void
usbo_initFSLSPClkSel(USB_OTG_Handle_t *husb, uint32_t freq)
{
	uint32_t base = husb->base;
	uint32_t hcfg = sil_rew_mem((uint32_t *)(USBH_BASE(base)+TOFF_USBH_HCFG));
	hcfg &= ~USB_OTG_HCFG_FSLSPCS;
	if(freq == 48000)
		hcfg |= USB_OTG_HCFG_FSLSPCS_0;
	else if(freq == 6000)
		hcfg |= USB_OTG_HCFG_FSLSPCS_1;
	sil_wrw_mem((uint32_t *)(USBH_BASE(base)+TOFF_USBH_HCFG), hcfg);

	if(freq == 48000 || freq ==  6000){
		sil_wrw_mem((uint32_t *)(USBH_BASE(base)+TOFF_USBH_HFIR), freq);
	}
}

/*
 *  HOST����ͥ������׵�
 *  parameter1 husb  USB�ϥ�ɥ�ؤΥݥ���
 *  parameter2 chnum ����ͥ��ֹ�(1-15)
 */
static void
usbo_hc_in_irqhandler(USB_OTG_Handle_t *husb, uint8_t chnum)
{
    USB_OTG_HCTypeDef *hc = &husb->hc[chnum];
	uint32_t base = husb->base;

	if(hc_int_status(base, chnum, USB_OTG_HCINT_AHBERR) != 0){
		clear_hc_int(base, chnum, USB_OTG_HCINT_AHBERR);
		sil_orw_mem((uint32_t *)(USBHC_BASE(base, chnum)+TOFF_USBHC_HCINTMSK), USB_OTG_HCINTMSK_CHHM);
	}
	else if(hc_int_status(base, chnum, USB_OTG_HCINT_ACK) != 0){
		clear_hc_int(base, chnum, USB_OTG_HCINT_ACK);
	}
	else if(hc_int_status(base, chnum, USB_OTG_HCINT_STALL) != 0){
		sil_orw_mem((uint32_t *)(USBHC_BASE(base, chnum)+TOFF_USBHC_HCINTMSK), USB_OTG_HCINTMSK_CHHM);
		hc->state = USBO_HC_STALL;
		usbo_hc_halt(husb, chnum);
		clear_hc_int(base, chnum, USB_OTG_HCINT_NAK);
		clear_hc_int(base, chnum, USB_OTG_HCINT_STALL);
	}
	else if(hc_int_status(base, chnum, USB_OTG_HCINT_DTERR) != 0){
		sil_orw_mem((uint32_t *)(USBHC_BASE(base, chnum)+TOFF_USBHC_HCINTMSK), USB_OTG_HCINTMSK_CHHM);
		usbo_hc_halt(husb, chnum);
		clear_hc_int(base, chnum, USB_OTG_HCINT_NAK);
		hc->state = USBO_HC_DATATGLERR;
		clear_hc_int(base, chnum, USB_OTG_HCINT_DTERR);
	}

	if(hc_int_status(base, chnum, USB_OTG_HCINT_FRMOR) != 0){
		sil_orw_mem((uint32_t *)(USBHC_BASE(base, chnum)+TOFF_USBHC_HCINTMSK), USB_OTG_HCINTMSK_CHHM);
		usbo_hc_halt(husb, chnum);
		clear_hc_int(base, chnum, USB_OTG_HCINT_FRMOR);
	}
	else if(hc_int_status(base, chnum, USB_OTG_HCINT_XFRC) != 0){
		if (husb->Init.dma_enable){
			hc->xfer_count = hc->xfer_len - (sil_rew_mem((uint32_t *)(USBHC_BASE(base, chnum)+TOFF_USBHC_HCTSIZ)) & USB_OTG_HCTSIZ_XFRSIZ);
		}

		hc->state = USBO_HC_XFRC;
		hc->err_count = 0;
		clear_hc_int(base, chnum, USB_OTG_HCINT_XFRC);

		if((hc->ep_type == EP_TYPE_CTRL) || (hc->ep_type == EP_TYPE_BULK)){
			sil_orw_mem((uint32_t *)(USBHC_BASE(base, chnum)+TOFF_USBHC_HCINTMSK), USB_OTG_HCINTMSK_CHHM);
			usbo_hc_halt(husb, chnum);
			clear_hc_int(base, chnum, USB_OTG_HCINT_NAK);
		}
		else if(hc->ep_type == EP_TYPE_INTR){
			sil_orw_mem((uint32_t *)(USBHC_BASE(base, chnum)+TOFF_USBHC_HCCHAR), USB_OTG_HCCHAR_ODDFRM);
			husb->hc[chnum].urb_state = URB_DONE;
			if(husb->hostchangeurbcallback != NULL)
				husb->hostchangeurbcallback(husb, chnum, husb->hc[chnum].urb_state);
		}
		hc->toggle_in ^= 1U;
	}
	else if(hc_int_status(base, chnum, USB_OTG_HCINT_CHH) != 0){
		sil_andw_mem((uint32_t *)(USBHC_BASE(base, chnum)+TOFF_USBHC_HCINTMSK), USB_OTG_HCINTMSK_CHHM);
		if(hc->state == USBO_HC_XFRC){
			hc->urb_state  = URB_DONE;
		}
		else if(hc->state == USBO_HC_STALL){
			hc->urb_state  = URB_STALL;
		}
		else if((hc->state == USBO_HC_XACTERR) || (hc->state == USBO_HC_DATATGLERR)){
			if(hc->err_count++ > 3U){
				hc->err_count = 0;
				hc->urb_state = URB_ERROR;
			}
			else{
				hc->urb_state = URB_NOTREADY;
			}

			/*
			 *  ����ͥ�κƥ����ƥ���
			 */
			sil_modw_mem((uint32_t *)(USBHC_BASE(base, chnum)+TOFF_USBHC_HCCHAR), USB_OTG_HCCHAR_CHDIS, USB_OTG_HCCHAR_CHENA);
		}
		clear_hc_int(base, chnum, USB_OTG_HCINT_CHH);
		if(husb->hostchangeurbcallback != NULL)
			husb->hostchangeurbcallback(husb, chnum, husb->hc[chnum].urb_state);
	}
	else if(hc_int_status(base, chnum, USB_OTG_HCINT_TXERR) != 0){
		sil_orw_mem((uint32_t *)(USBHC_BASE(base, chnum)+TOFF_USBHC_HCINTMSK), USB_OTG_HCINTMSK_CHHM);
		hc->err_count++;
		hc->state = USBO_HC_XACTERR;
#if 1	/* ROI DEBUG */
		syslog_1(LOG_NOTICE, "## SET USBO_HC_XACTERR1 [%08x] ##", sil_rew_mem((uint32_t *)(USBHC_BASE(base, chnum)+TOFF_USBHC_HCINT)));
#endif	/* ROI DEBUG */
		usbo_hc_halt(husb, chnum);
		clear_hc_int(base, chnum, USB_OTG_HCINT_TXERR);
	}
	else if(hc_int_status(base, chnum, USB_OTG_HCINT_NAK) != 0){
		if(hc->ep_type == EP_TYPE_INTR){
			sil_orw_mem((uint32_t *)(USBHC_BASE(base, chnum)+TOFF_USBHC_HCINTMSK), USB_OTG_HCINTMSK_CHHM);
			usbo_hc_halt(husb, chnum);
		}
		else if((hc->ep_type == EP_TYPE_CTRL) || (hc->ep_type == EP_TYPE_BULK)){
			/*
			 *  ����ͥ�κƥ����ƥ���
			 */
			sil_modw_mem((uint32_t *)(USBHC_BASE(base, chnum)+TOFF_USBHC_HCCHAR), USB_OTG_HCCHAR_CHDIS, USB_OTG_HCCHAR_CHENA);
		}
		hc->state = USBO_HC_NAK;
		clear_hc_int(base, chnum, USB_OTG_HCINT_NAK);
	}
}

/*
 *  HOST����ͥ�OUT������׵�
 *  parameter1 husb  USB�ϥ�ɥ�ؤΥݥ���
 *  parameter2 chnum ����ͥ��ֹ�(1-15)
 */
static void
usbo_hc_out_irqhandler(USB_OTG_Handle_t *husb, uint8_t chnum)
{
    USB_OTG_HCTypeDef *hc = &husb->hc[chnum];
	uint32_t base = husb->base;

	if(hc_int_status(base, chnum, USB_OTG_HCINT_AHBERR) != 0){
		clear_hc_int(base, chnum, USB_OTG_HCINT_AHBERR);
		sil_orw_mem((uint32_t *)(USBHC_BASE(base, chnum)+TOFF_USBHC_HCINTMSK), USB_OTG_HCINTMSK_CHHM);
	}
	else if(hc_int_status(base, chnum, USB_OTG_HCINT_ACK) != 0){
		clear_hc_int(base, chnum, USB_OTG_HCINT_ACK);
		if(hc->do_ping == 1U){
			hc->state = USBO_HC_NYET;
			sil_orw_mem((uint32_t *)(USBHC_BASE(base, chnum)+TOFF_USBHC_HCINTMSK), USB_OTG_HCINTMSK_CHHM);
			usbo_hc_halt(husb, chnum);
			hc->urb_state  = URB_NOTREADY;
		}
	}
	else if(hc_int_status(base, chnum, USB_OTG_HCINT_NYET) != 0){
		hc->state = USBO_HC_NYET;
		hc->err_count= 0;
		sil_orw_mem((uint32_t *)(USBHC_BASE(base, chnum)+TOFF_USBHC_HCINTMSK), USB_OTG_HCINTMSK_CHHM);
		usbo_hc_halt(husb, chnum);
		clear_hc_int(base, chnum, USB_OTG_HCINT_NYET);
	}
	else if(hc_int_status(base, chnum, USB_OTG_HCINT_FRMOR) != 0){
		sil_orw_mem((uint32_t *)(USBHC_BASE(base, chnum)+TOFF_USBHC_HCINTMSK), USB_OTG_HCINTMSK_CHHM);
		usbo_hc_halt(husb, chnum);
		clear_hc_int(base, chnum, USB_OTG_HCINT_FRMOR);
	}
	else if(hc_int_status(base, chnum, USB_OTG_HCINT_XFRC) != 0){
		hc->err_count = 0;
		sil_orw_mem((uint32_t *)(USBHC_BASE(base, chnum)+TOFF_USBHC_HCINTMSK), USB_OTG_HCINTMSK_CHHM);
		usbo_hc_halt(husb, chnum);
		clear_hc_int(base, chnum, USB_OTG_HCINT_XFRC);
		hc->state = USBO_HC_XFRC;
	}
	else if(hc_int_status(base, chnum, USB_OTG_HCINT_STALL) != 0){
		clear_hc_int(base, chnum, USB_OTG_HCINT_STALL);
		sil_orw_mem((uint32_t *)(USBHC_BASE(base, chnum)+TOFF_USBHC_HCINTMSK), USB_OTG_HCINTMSK_CHHM);
		usbo_hc_halt(husb, chnum);
		hc->state = USBO_HC_STALL;
	}
	else if(hc_int_status(base, chnum, USB_OTG_HCINT_NAK) != 0){
		hc->err_count = 0;
		sil_orw_mem((uint32_t *)(USBHC_BASE(base, chnum)+TOFF_USBHC_HCINTMSK), USB_OTG_HCINTMSK_CHHM);
		usbo_hc_halt(husb, chnum);
		hc->state = USBO_HC_NAK;
		clear_hc_int(base, chnum, USB_OTG_HCINT_NAK);
	}
	else if(hc_int_status(base, chnum,USB_OTG_HCINT_TXERR) != 0){
		sil_orw_mem((uint32_t *)(USBHC_BASE(base, chnum)+TOFF_USBHC_HCINTMSK), USB_OTG_HCINTMSK_CHHM);
		usbo_hc_halt(husb, chnum);
		hc->state = USBO_HC_XACTERR;
#if 1	/* ROI DEBUG */
    syslog_1(LOG_NOTICE, "## SET USBO_HC_XACTERR2 [%08x] ##", sil_rew_mem((uint32_t *)(USBHC_BASE(base, chnum)+TOFF_USBHC_HCINT)));
#endif	/* ROI DEBUG */
		clear_hc_int(base, chnum, USB_OTG_HCINT_TXERR);
	}
	else if(hc_int_status(base, chnum, USB_OTG_HCINT_DTERR) != 0){
		sil_orw_mem((uint32_t *)(USBHC_BASE(base, chnum)+TOFF_USBHC_HCINTMSK), USB_OTG_HCINTMSK_CHHM);
 		usbo_hc_halt(husb, chnum);
		clear_hc_int(base, chnum, USB_OTG_HCINT_NAK);
		clear_hc_int(base, chnum, USB_OTG_HCINT_DTERR);
		hc->state = USBO_HC_DATATGLERR;
	}
	else if(hc_int_status(base, chnum, USB_OTG_HCINT_CHH) != 0){
		sil_andw_mem((uint32_t *)(USBHC_BASE(base, chnum)+TOFF_USBHC_HCINTMSK), USB_OTG_HCINTMSK_CHHM);
		if(hc->state == USBO_HC_XFRC){
			hc->urb_state  = URB_DONE;
			if(hc->ep_type == EP_TYPE_BULK){
				hc->toggle_out ^= 1U; 
			}
		}
		else if (hc->state == USBO_HC_NAK){
			hc->urb_state  = URB_NOTREADY;
		}
		else if (hc->state == USBO_HC_NYET){
			hc->urb_state  = URB_NOTREADY;
			hc->do_ping = 0U;
		}
		else if (hc->state == USBO_HC_STALL){
			hc->urb_state  = URB_STALL;
		}
		else if((hc->state == USBO_HC_XACTERR) || (hc->state == USBO_HC_DATATGLERR)){
			if(hc->err_count++ > 3){
				hc->err_count = 0;
				hc->urb_state = URB_ERROR;
			}
			else{
				hc->urb_state = URB_NOTREADY;
			}

			/* re-activate the channel  */
			sil_modw_mem((uint32_t *)(USBHC_BASE(base, chnum)+TOFF_USBHC_HCCHAR), USB_OTG_HCCHAR_CHDIS, USB_OTG_HCCHAR_CHENA);
		}
		clear_hc_int(base, chnum, USB_OTG_HCINT_CHH);
		if(husb->hostchangeurbcallback != NULL)
			husb->hostchangeurbcallback(husb, chnum, husb->hc[chnum].urb_state);
	}
}

/*
 *  RX QUEUE LEVEL������׵�
 *  parameter1 husb  USB�ϥ�ɥ�ؤΥݥ���
 */
static void
usbo_rxqlvl_irqhandler(USB_OTG_Handle_t *husb)
{
	uint32_t base = husb->base;
	uint8_t  channelnum = 0U;
	uint32_t pktsts, pktcnt;
	uint32_t temp = 0U;

	temp = sil_rew_mem((uint32_t *)(USBOTG_BASE(base)+TOFF_USBO_GRXSTSP));
	channelnum = temp &  USB_OTG_GRXSTSP_EPNUM;  
	pktsts = (temp &  USB_OTG_GRXSTSP_PKTSTS) >> 17U;
	pktcnt = (temp &  USB_OTG_GRXSTSP_BCNT) >> 4U;

	switch(pktsts){
	case GRXSTS_PKTSTS_IN:
		/*
		 *  HOST�Хåե��ѥ��å��ɤ߹���
		 */
		if((pktcnt > 0) && (husb->hc[channelnum].xfer_buff != (void  *)0U)){
			usbo_readPacket(husb->base, husb->hc[channelnum].xfer_buff, pktcnt);

			/*
			 *  �ޥ���ǡ�������
			 */
			husb->hc[channelnum].xfer_buff += pktcnt;
			husb->hc[channelnum].xfer_count  += pktcnt;

			if((sil_rew_mem((uint32_t *)(USBHC_BASE(base, channelnum)+TOFF_USBHC_HCTSIZ)) & USB_OTG_HCTSIZ_PKTCNT) > 0){
				sil_modw_mem((uint32_t *)(USBHC_BASE(base, channelnum)+TOFF_USBHC_HCCHAR), USB_OTG_HCCHAR_CHDIS, USB_OTG_HCCHAR_CHENA);
				husb->hc[channelnum].toggle_in ^= 1U;
			}
		}
		break;

	case GRXSTS_PKTSTS_DATA_TOGGLE_ERR:
	case GRXSTS_PKTSTS_IN_XFER_COMP:
	case GRXSTS_PKTSTS_CH_HALTED:
	default:
		break;
	}
}

/*
 *  HOST�ݡ��ȳ�����׵�
 *  parameter1 husb  USB�ϥ�ɥ�ؤΥݥ���
 */
static void
usbo_port_irqhandler(USB_OTG_Handle_t *husb)
{
	uint32_t base = husb->base;
	volatile uint32_t hprt0, hprt0_dup;

	hprt0 = sil_rew_mem((uint32_t*)(USBPRT_BASE(base)));
	hprt0_dup = hprt0 & ~USB_OTG_HPRT_PENA;

	/*
	 *  �ݡ��ȥ��ͥ��ȥ����å�
	 */
	if((hprt0 & USB_OTG_HPRT_PCDET) != 0){
		if((hprt0 & USB_OTG_HPRT_PCSTS) == USB_OTG_HPRT_PCSTS){
			sil_andw_mem((uint32_t *)(USBOTG_BASE(base)+TOFF_USBO_GINTMSK), USB_OTG_GINTSTS_DISCINT);
			if(husb->hostconnectcallback != NULL)
				husb->hostconnectcallback(husb);
		}
	}

	/*
	 *  �ݡ��ȥ��͡��֥��Ѳ������å�
	 */
	if((hprt0 & USB_OTG_HPRT_PENCHNG) != 0){
		if((hprt0 & USB_OTG_HPRT_PENA) != 0){
			if(husb->Init.phy_itface  == USB_PHY_EMBEDDED){
				if((hprt0 & USB_OTG_HPRT_PSPD) == (HPRT0_PRTSPD_LOW_SPEED << 17)){
					usbo_initFSLSPClkSel(husb, 6000);
				}
				else{
					usbo_initFSLSPClkSel(husb, 48000);
				}
			}
			else{
				if(husb->Init.speed == USB_SPEED_FULL){
					sil_wrw_mem((uint32_t *)(USBH_BASE(base)+TOFF_USBH_HFIR), 60000U);
				}
			}

			if(husb->hostconnectcallback != NULL)
				husb->hostconnectcallback(husb);

			if(husb->Init.speed == USB_SPEED_HIGH){
				sil_orw_mem((uint32_t *)(USBOTG_BASE(base)+TOFF_USBO_GINTMSK), USB_OTG_GINTSTS_DISCINT);
			}
		}
		else{
			/*
			 *  HPRT�Υ��꡼��
			 */
			sil_andw_mem((uint32_t*)(USBPRT_BASE(base)), (USB_OTG_HPRT_PENA | USB_OTG_HPRT_PCDET |\
				USB_OTG_HPRT_PENCHNG | USB_OTG_HPRT_POCCHNG ));
			sil_orw_mem((uint32_t *)(USBOTG_BASE(base)+TOFF_USBO_GINTMSK), USB_OTG_GINTSTS_DISCINT);
		}
	}

	/*
	 *  �ݡ��ȳ���ߥ��ꥢ
	 */
	sil_wrw_mem((uint32_t*)(USBPRT_BASE(base)), hprt0_dup);
}

/*
 *  HOST����ߥϥ�ɥ�
 *  parameter1 husb  USB�ϥ�ɥ�ؤΥݥ���
 */
void
usbo_hcd_irqhandler(USB_OTG_Handle_t *husb)
{
	uint32_t base = husb->base;
	uint32_t msk  = sil_rew_mem((uint32_t *)(USBOTG_BASE(base)+TOFF_USBO_GINTMSK));
	uint32_t i = 0, int_status = 0;

	/*
	 *  ���Ĥ��ʤ��װ��γ���ߤ�̵�뤹��
	 */
	if(get_int_status(base, msk) == 0){
		uint32_t intsts = sil_rew_mem((uint32_t *)(USBOTG_BASE(base)+TOFF_USBO_GINTSTS));
		syslog_1(LOG_EMERG, "usbo_hcd_irqhandler Error Intrrupt[%08x] !", intsts);
		clear_host_int(base, intsts);
		return;
	}

	if((get_int_status(base, msk) & USB_OTG_GINTSTS_PXFR_INCOMPISOOUT) != 0){
		/*
		 *  ̤�׵������װ����ꥢ
		 */
		clear_host_int(base, USB_OTG_GINTSTS_PXFR_INCOMPISOOUT);
	}
	if((get_int_status(base, msk) & USB_OTG_GINTSTS_IISOIXFR) != 0){
		/*
		 *  ̤�׵������װ����ꥢ
		 */
		clear_host_int(base, USB_OTG_GINTSTS_IISOIXFR);
	}

	if((get_int_status(base, msk) & USB_OTG_GINTSTS_PTXFE) != 0){
		/*
		 *  ̤�׵������װ����ꥢ
		 */
		clear_host_int(base, USB_OTG_GINTSTS_PTXFE);
	}
    if((get_int_status(base, msk) & USB_OTG_GINTSTS_MMIS) != 0){
		/*
		 *  ̤�׵������װ����ꥢ
		 */
		clear_host_int(base, USB_OTG_GINTSTS_MMIS);
    }
    /*
	 *  HOST�ǥ������ͥ��ȳ����
	 */
	if((get_int_status(base, msk) & USB_OTG_GINTSTS_DISCINT) != 0){
		/*
		 *  HPRT�Υ��꡼�󥢥å�
		 */
		sil_andw_mem((uint32_t*)(USBPRT_BASE(base)), (USB_OTG_HPRT_PENA | USB_OTG_HPRT_PCDET |\
			USB_OTG_HPRT_PENCHNG | USB_OTG_HPRT_POCCHNG ));
		if(husb->hostdisconnectcallback != NULL)
			husb->hostdisconnectcallback(husb);
		usbo_initFSLSPClkSel(husb, 48000);
		clear_host_int(base, USB_OTG_GINTSTS_DISCINT);
	}
	/*
	 *  HOST�ݡ��ȳ����
	 */
	if((get_int_status(base, msk) & USB_OTG_GINTSTS_HPRTINT) != 0){
		syslog_0(LOG_NOTICE, "## usbo_hcd_irqhandler USB_OTG_GINTSTS_HPRTINT ##");
		usbo_port_irqhandler(husb);
		if(husb->Init.usb_otg_mode == USB_OTG_MODE_DRD)
			sil_dly_nse(10*1000*1000);	/* ��ա��ǥХ�����ǥ��ޤǤ��Ԥ�(����ߤǤ����ԤƤʤ�) */
	}
	/*
	 *  Host SOF�����
	 */
    if((get_int_status(base, msk) & USB_OTG_GINTSTS_SOF) != 0){
		if(husb->hostsofcallback != NULL)
			husb->hostsofcallback(husb);
		clear_host_int(base, USB_OTG_GINTSTS_SOF);
	}
	/*
	 *  HOST����ͥ�����
	 */
	if((get_int_status(base, msk) & USB_OTG_GINTSTS_HCINT) != 0){
		int_status = get_haint_status(base);
		for(i = 0 ; i < husb->Init.host_channels; i++){
			if(int_status & (1 << i)){
				if((sil_rew_mem((uint32_t *)(USBHC_BASE(husb->base, i)+TOFF_USBHC_HCCHAR)) &  USB_OTG_HCCHAR_EPDIR) != 0)
					usbo_hc_in_irqhandler(husb, i);
				else
					usbo_hc_out_irqhandler(husb, i);
			}
		}
		clear_host_int(base, USB_OTG_GINTSTS_HCINT);
	}
	/*
	 *  RX���塼��٥�����
	 */
	if((get_int_status(base, msk) & USB_OTG_GINTSTS_RXFLVL) != 0){
		sil_andw_mem((uint32_t *)(USBOTG_BASE(base)+TOFF_USBO_GINTMSK), USB_OTG_GINTSTS_RXFLVL);
		usbo_rxqlvl_irqhandler(husb);
		sil_orw_mem((uint32_t *)(USBOTG_BASE(base)+TOFF_USBO_GINTMSK), USB_OTG_GINTSTS_RXFLVL);
	}
}

/*******************************************************************************
                       USB DEVICE�ؿ�
*******************************************************************************/
/*
 *  USB DEVICE���ԡ�������
 *  parameter1 base  USB OTG�쥸�����١������ɥ쥹
 *  parameter2 speed device speed USB_SPEED_HIGH(High speed mode)
 *                                USB_SPEED_HIGH_IN_FULL(High speed core in Full Speed mode)
 *                                USB_SPEED_FULL(Full speed mode)
 *                                USB_SPEED_LOW(Low speed mode)
 */
static void
usbo_setDevSpeed(uint32_t base, uint8_t speed)
{
	sil_orw_mem((uint32_t *)(USBD_BASE(base)+TOFF_USBD_DCFG), speed);
}

/*
 *  �ǥХ������ͥ���
 *  parameter1 husb  USB�ϥ�ɥ�ؤΥݥ���
 *  return     ���E_OK
 */
ER
usbo_devconnect(USB_OTG_Handle_t *husb)
{
	sil_andw_mem((uint32_t *)(USBD_BASE(husb->base)+TOFF_USBD_DCTL), USB_OTG_DCTL_SDIS);
//	dly_tsk(3);
	return E_OK;
}

/*
 *  �ǥХ����ǥ������ͥ���
 *  parameter1 husb  USB�ϥ�ɥ�ؤΥݥ���
 *  return     ���E_OK
 */
ER
usbo_devdisconnect(USB_OTG_Handle_t *husb)
{
	sil_orw_mem((uint32_t *)(USBD_BASE(husb->base)+TOFF_USBD_DCTL), USB_OTG_DCTL_SDIS);
//	dly_tsk(3);
	return E_OK; 
}

/*
 *  �����ƥ��֥���ɥݥ��������
 *  parameter1 husb  USB�ϥ�ɥ�ؤΥݥ���
 *  parameter2 ep    ����ɥݥ���ȹ�¤�ΤؤΥݥ���
 *  return     ���E_OK
 */
ER
usbo_activateEndpoint(USB_OTG_Handle_t *husb, USB_OTG_EPTypeDef *ep)
{
	uint32_t base = husb->base;

	if(ep->is_in == 1){
		sil_orw_mem((uint32_t *)(USBD_BASE(base)+TOFF_USBD_DAINTMSK), USB_OTG_DAINTMSK_IEPM & (1 << (ep->num)));
		if(((sil_rew_mem((uint32_t *)(USBIEP_BASE(base, ep->num)+TOFF_USBEI_DIEPCTL))) & USB_OTG_DIEPCTL_USBAEP) == 0){
			sil_orw_mem((uint32_t *)(USBIEP_BASE(base, ep->num)+TOFF_USBEI_DIEPCTL), ((ep->maxpacket & USB_OTG_DIEPCTL_MPSIZ ) | (ep->type << 18U) |\
				((ep->num) << 22U) | (USB_OTG_DIEPCTL_SD0PID_SEVNFRM) | (USB_OTG_DIEPCTL_USBAEP))); 
		}
	}
	else{
		sil_orw_mem((uint32_t *)(USBD_BASE(base)+TOFF_USBD_DAINTMSK), USB_OTG_DAINTMSK_OEPM & ((1 << (ep->num)) << 16));
		if(((sil_rew_mem((uint32_t *)(USBOEP_BASE(base, ep->num)+TOFF_USBEO_DOEPCTL))) & USB_OTG_DIEPCTL_USBAEP) == 0){
			sil_orw_mem((uint32_t *)(USBOEP_BASE(base, ep->num)+TOFF_USBEO_DOEPCTL), ((ep->maxpacket & USB_OTG_DOEPCTL_MPSIZ ) | (ep->type << 18U) |\
				(USB_OTG_DIEPCTL_SD0PID_SEVNFRM)| (USB_OTG_DOEPCTL_USBAEP)));
		}
	}
	return E_OK;
}

/*
 *  �ǥ��������ƥ��֥���ɥݥ��������
 *  parameter1 husb  USB�ϥ�ɥ�ؤΥݥ���
 *  parameter2 ep    ����ɥݥ���ȹ�¤�ΤؤΥݥ���
 *  return     ���E_OK
 */
ER
usbo_deactivateEndpoint(USB_OTG_Handle_t *husb, USB_OTG_EPTypeDef *ep)
{
	uint32_t base = husb->base;

	if(ep->is_in == 1){
		sil_andw_mem((uint32_t *)(USBD_BASE(base)+TOFF_USBD_DEACHMSK), (USB_OTG_DAINTMSK_IEPM & (1 << (ep->num))));
		sil_andw_mem((uint32_t *)(USBD_BASE(base)+TOFF_USBD_DAINTMSK), (USB_OTG_DAINTMSK_IEPM & (1 << (ep->num))));
		sil_andw_mem((uint32_t *)(USBIEP_BASE(base, ep->num)+TOFF_USBEI_DIEPCTL), USB_OTG_DIEPCTL_USBAEP);
	}
	else{
		sil_andw_mem((uint32_t *)(USBD_BASE(base)+TOFF_USBD_DEACHMSK), (USB_OTG_DAINTMSK_OEPM & ((1 << (ep->num)) << 16)));
		sil_andw_mem((uint32_t *)(USBD_BASE(base)+TOFF_USBD_DAINTMSK), (USB_OTG_DAINTMSK_OEPM & ((1 << (ep->num)) << 16)));
		sil_andw_mem((uint32_t *)(USBOEP_BASE(base, ep->num)+TOFF_USBEO_DOEPCTL), USB_OTG_DOEPCTL_USBAEP);
	}
	return E_OK;
}

/*
 *  ����ɥݥ���Ȥ˥��ơ�����֤�����
 *  parameter1 husb  USB�ϥ�ɥ�ؤΥݥ���
 *  parameter2 ep    ����ɥݥ���ȹ�¤�ΤؤΥݥ���
 *  return     ���E_OK
 */
ER
usbo_epsetStall(USB_OTG_Handle_t *husb, USB_OTG_EPTypeDef *ep)
{
	uint32_t base = husb->base;

	if(ep->is_in == 1){
		if((sil_rew_mem((uint32_t *)(USBIEP_BASE(base, ep->num)+TOFF_USBEI_DIEPCTL)) & USB_OTG_DIEPCTL_EPENA) == 0){
			sil_andw_mem((uint32_t *)(USBIEP_BASE(base, ep->num)+TOFF_USBEI_DIEPCTL), USB_OTG_DIEPCTL_EPDIS);
		}
		sil_orw_mem((uint32_t *)(USBIEP_BASE(base, ep->num)+TOFF_USBEI_DIEPCTL), USB_OTG_DIEPCTL_STALL);
	}
	else{
		if((sil_rew_mem((uint32_t *)(USBOEP_BASE(base, ep->num)+TOFF_USBEO_DOEPCTL)) & USB_OTG_DOEPCTL_EPENA) == 0){
			sil_andw_mem((uint32_t *)(USBOEP_BASE(base, ep->num)+TOFF_USBEO_DOEPCTL), USB_OTG_DOEPCTL_EPDIS);
		}
		sil_orw_mem((uint32_t *)(USBOEP_BASE(base, ep->num)+TOFF_USBEO_DOEPCTL), USB_OTG_DOEPCTL_STALL);
	}
	return E_OK;
}

/*
 *  ����ɥݥ���ȤΥ��ơ�����֤򥯥ꥢ
 *  parameter1 husb  USB�ϥ�ɥ�ؤΥݥ���
 *  parameter2 ep    ����ɥݥ���ȹ�¤�ΤؤΥݥ���
 *  return     ���E_OK
 *
 */
ER
usbo_epclearStall(USB_OTG_Handle_t *husb, USB_OTG_EPTypeDef *ep)
{
	uint32_t base = husb->base;

	if(ep->is_in == 1){
		sil_andw_mem((uint32_t *)(USBIEP_BASE(base, ep->num)+TOFF_USBEI_DIEPCTL), USB_OTG_DIEPCTL_STALL);
		if(ep->type == EP_TYPE_INTR || ep->type == EP_TYPE_BULK){
			sil_orw_mem((uint32_t *)(USBIEP_BASE(base, ep->num)+TOFF_USBEI_DIEPCTL), USB_OTG_DIEPCTL_SD0PID_SEVNFRM);	/* DATA0 */
		}
	}
	else{
		sil_andw_mem((uint32_t *)(USBOEP_BASE(base, ep->num)+TOFF_USBEO_DOEPCTL), USB_OTG_DOEPCTL_STALL);
		if(ep->type == EP_TYPE_INTR || ep->type == EP_TYPE_BULK){
			sil_orw_mem((uint32_t *)(USBOEP_BASE(base, ep->num)+TOFF_USBEO_DOEPCTL), USB_OTG_DOEPCTL_SD0PID_SEVNFRM);	/* DATA0 */
		}
	}
	return E_OK;
}

/**
 *  USB�˥ǥХ������ɥ쥹������
 *  parameter1 husb  USB�ϥ�ɥ�ؤΥݥ���
 *  parameter2 addree �ǥХ������ɥ쥹(0-255)
 *  return     ���E_OK
 */
ER
usbo_setDevAddress(USB_OTG_Handle_t *husb, uint8_t address)
{
	husb->connected = 1;
	sil_andw_mem((uint32_t *)(USBD_BASE(husb->base)+TOFF_USBD_DCFG), USB_OTG_DCFG_DAD);
	sil_orw_mem((uint32_t *)(USBD_BASE(husb->base)+TOFF_USBD_DCFG), ((address << 4) & USB_OTG_DCFG_DAD));
	return E_OK;
}

/*
 *  USB�ǥХ������ԡ��ɤ���Ф�
 *  parameter1 husb  USB�ϥ�ɥ�ؤΥݥ���
 *  return     speed �ǥХ������ԡ��� USB_SPEED_HIGH: High speed mode
 *                                    USB_SPEED_FULL: Full speed mode
 *                                    USB_SPEED_LOW: Low speed mode
 */
uint8_t
usbo_getDevSpeed(USB_OTG_Handle_t *husb)
{
	uint8_t  speed = 0;
	uint32_t status = sil_rew_mem((uint32_t *)(USBD_BASE(husb->base)+TOFF_USBD_DSTS));

	if((status & USB_OTG_DSTS_ENUMSPD) == DSTS_ENUMSPD_HS_PHY_30MHZ_OR_60MHZ){
		speed = USB_SPEED_HIGH;
	}
	else if(((status & USB_OTG_DSTS_ENUMSPD) == DSTS_ENUMSPD_FS_PHY_30MHZ_OR_60MHZ) || ((status & USB_OTG_DSTS_ENUMSPD) == DSTS_ENUMSPD_FS_PHY_48MHZ)){
		speed = USB_SPEED_FULL;
	}
	else if((status & USB_OTG_DSTS_ENUMSPD) == DSTS_ENUMSPD_LS_PHY_6MHZ){
		speed = USB_SPEED_LOW;
	}
	return speed;
}

/*
 *  EP0-OUT��������
 *  parameter1 husb  USB�ϥ�ɥ�ؤΥݥ���
 *  parameter2 psetup ���åȥ��åץѥ��åȥǡ����ؤΥݥ���
 *  return     ���E_OK
 */
ER
usbo_ep0_outstart(USB_OTG_Handle_t *husb, uint8_t *psetup)
{
	uint32_t base = husb->base;

	sil_wrw_mem((uint32_t *)(USBOEP_BASE(base, 0)+TOFF_USBEO_DOEPTSIZ), 0);
	sil_orw_mem((uint32_t *)(USBOEP_BASE(base, 0)+TOFF_USBEO_DOEPTSIZ), (USB_OTG_DOEPTSIZ_PKTCNT & (1U << 19U)));
	sil_orw_mem((uint32_t *)(USBOEP_BASE(base, 0)+TOFF_USBEO_DOEPTSIZ), (3U * 8U));
	sil_orw_mem((uint32_t *)(USBOEP_BASE(base, 0)+TOFF_USBEO_DOEPTSIZ), USB_OTG_DOEPTSIZ_STUPCNT);

	if(husb->Init.dma_enable == 1){
		flushinvalidatedcache_by_addr(psetup, 64);
		sil_wrw_mem((uint32_t *)(USBOEP_BASE(base, 0)+TOFF_USBEO_DOEPDMA), (uint32_t)psetup);
 		/*
		 *  EP0ͭ����
		 */
		sil_wrw_mem((uint32_t *)(USBOEP_BASE(base, 0)+TOFF_USBEO_DOEPCTL), 0x80008000);
	}
	return E_OK;
}

/*
 *  EP0ž����������
 *  parameter1 husb  USB�ϥ�ɥ�ؤΥݥ���
 *  parameter2 ep    ����ɥݥ���ȹ�¤�ΤؤΥݥ���
 *  return     ���E_OK
 */
ER
usbo_ep0startxfer(USB_OTG_Handle_t *husb, USB_OTG_EPTypeDef *ep)
{
	uint32_t base   = husb->base;
	uint8_t  epnum  = ep->num;

	if(ep->is_in == 1){	/* IN����ɥݥ���� */
		if(ep->xfer_len == 0){	/* ZERO-LENGTH�ѥ��å� */
			sil_andw_mem((uint32_t *)(USBIEP_BASE(base, epnum)+TOFF_USBEI_DIEPTSIZ), USB_OTG_DIEPTSIZ_PKTCNT);
			sil_orw_mem((uint32_t *)(USBIEP_BASE(base, epnum)+TOFF_USBEI_DIEPTSIZ), (USB_OTG_DIEPTSIZ_PKTCNT & (1 << 19)));
			sil_andw_mem((uint32_t *)(USBIEP_BASE(base, epnum)+TOFF_USBEI_DIEPTSIZ), USB_OTG_DIEPTSIZ_XFRSIZ);
		}
		else{	/* �̾�ѥ��å� */
			/*
			 *  �ѥ��åȿ��ȥǡ���Ĺ��ʬ��������(EP0�ʤΤǥѥ��åȿ���1)
			 */
			sil_andw_mem((uint32_t *)(USBIEP_BASE(base, epnum)+TOFF_USBEI_DIEPTSIZ), USB_OTG_DIEPTSIZ_XFRSIZ);
			sil_andw_mem((uint32_t *)(USBIEP_BASE(base, epnum)+TOFF_USBEI_DIEPTSIZ), USB_OTG_DIEPTSIZ_PKTCNT);
			if(ep->xfer_len > ep->maxpacket){
				ep->xfer_len = ep->maxpacket;
			}
			sil_orw_mem((uint32_t *)(USBIEP_BASE(base, epnum)+TOFF_USBEI_DIEPTSIZ), (USB_OTG_DIEPTSIZ_PKTCNT & (1 << 19)));
			sil_orw_mem((uint32_t *)(USBIEP_BASE(base, epnum)+TOFF_USBEI_DIEPTSIZ), (USB_OTG_DIEPTSIZ_XFRSIZ & ep->xfer_len));
		}

		if(husb->Init.dma_enable == 1){
			flushinvalidatedcache_by_addr(((uint8_t *)ep->dma_addr), ep->xfer_len);
			sil_wrw_mem((uint32_t *)(USBIEP_BASE(base, epnum)+TOFF_USBEI_DIEPDMA), ep->dma_addr);
		}
		else{
			/*
			 *  DMA�⡼�ɤǤʤ����ϡ�EMPTY����ߤ�����
			 */
			if(ep->xfer_len > 0){
				sil_orw_mem((uint32_t *)(USBD_BASE(base)+TOFF_USBD_DIEPEMPMSK), (1 << (ep->num)));
			}
		}
		/*
		 *  ���ꥢNAK������ɥݥ���ȥ��͡��֥�
		 */
		sil_orw_mem((uint32_t *)(USBIEP_BASE(base, epnum)+TOFF_USBEI_DIEPCTL), (USB_OTG_DIEPCTL_CNAK | USB_OTG_DIEPCTL_EPENA));
	}
	else{	 /* OUT����ɥݥ���� */
		/*
		 *  �ѥ��åȿ��ȥǡ���Ĺ��ʬ��������(EP0�ʤΤǥѥ��åȿ���1)
		 */
		sil_andw_mem((uint32_t *)(USBOEP_BASE(base, epnum)+TOFF_USBEO_DOEPTSIZ), USB_OTG_DOEPTSIZ_XFRSIZ);
		sil_andw_mem((uint32_t *)(USBOEP_BASE(base, epnum)+TOFF_USBEO_DOEPTSIZ), USB_OTG_DOEPTSIZ_PKTCNT);

		if(ep->xfer_len > 0U){
			ep->xfer_len = ep->maxpacket;
		}

		sil_orw_mem((uint32_t *)(USBOEP_BASE(base, epnum)+TOFF_USBEO_DOEPTSIZ), (USB_OTG_DOEPTSIZ_PKTCNT & (1 << 19)));
		sil_orw_mem((uint32_t *)(USBOEP_BASE(base, epnum)+TOFF_USBEO_DOEPTSIZ), (USB_OTG_DOEPTSIZ_XFRSIZ & (ep->maxpacket)));

		if(husb->Init.dma_enable == 1){
			flushinvalidatedcache_by_addr(ep->xfer_buff, ep->xfer_len);
			sil_wrw_mem((uint32_t *)(USBOEP_BASE(base, epnum)+TOFF_USBEO_DOEPDMA), (uint32_t)(ep->xfer_buff));
		}
		/*
		 *  ���ꥢNAK������ɥݥ���ȥ��͡��֥�
		 */
		sil_orw_mem((uint32_t *)(USBOEP_BASE(base, epnum)+TOFF_USBEO_DOEPCTL), (USB_OTG_DOEPCTL_CNAK | USB_OTG_DOEPCTL_EPENA));
	}
	return E_OK;
}

/*
 *  EPnž����������
 *  parameter1 husb  USB�ϥ�ɥ�ؤΥݥ���
 *  parameter2 ep    ����ɥݥ���ȹ�¤�ΤؤΥݥ���
 *  return     ���E_OK
 */
ER
usbo_epstartxfer(USB_OTG_Handle_t *husb, USB_OTG_EPTypeDef *ep)
{
	uint32_t base   = husb->base;
	uint16_t pktcnt = 0;
	uint8_t  epnum  = ep->num;

	if(ep->is_in == 1){	/* IN����ɥݥ���� */
		if(ep->xfer_len == 0){	/* ZERO-LENGTH�ѥ��å� */
			sil_andw_mem((uint32_t *)(USBIEP_BASE(base, epnum)+TOFF_USBEI_DIEPTSIZ), USB_OTG_DIEPTSIZ_PKTCNT);
			sil_orw_mem((uint32_t *)(USBIEP_BASE(base, epnum)+TOFF_USBEI_DIEPTSIZ), (USB_OTG_DIEPTSIZ_PKTCNT & (1 << 19)));
			sil_andw_mem((uint32_t *)(USBIEP_BASE(base, epnum)+TOFF_USBEI_DIEPTSIZ), USB_OTG_DIEPTSIZ_XFRSIZ);
		}
		else{	/* �̾�ѥ��å� */
			/*
			 *  �ѥ��åȿ��ȥǡ���Ĺ��ʬ��������
			 */
			sil_andw_mem((uint32_t *)(USBIEP_BASE(base, epnum)+TOFF_USBEI_DIEPTSIZ), USB_OTG_DIEPTSIZ_XFRSIZ);
			sil_andw_mem((uint32_t *)(USBIEP_BASE(base, epnum)+TOFF_USBEI_DIEPTSIZ), USB_OTG_DIEPTSIZ_PKTCNT);
			sil_orw_mem((uint32_t *)(USBIEP_BASE(base, epnum)+TOFF_USBEI_DIEPTSIZ), (USB_OTG_DIEPTSIZ_PKTCNT & (((ep->xfer_len + ep->maxpacket -1)/ ep->maxpacket) << 19)));
			sil_orw_mem((uint32_t *)(USBIEP_BASE(base, epnum)+TOFF_USBEI_DIEPTSIZ), (USB_OTG_DIEPTSIZ_XFRSIZ & ep->xfer_len));

			if(ep->type == EP_TYPE_ISOC){
				sil_andw_mem((uint32_t *)(USBIEP_BASE(base, epnum)+TOFF_USBEI_DIEPTSIZ), USB_OTG_DIEPTSIZ_MULCNT);
				sil_orw_mem((uint32_t *)(USBIEP_BASE(base, epnum)+TOFF_USBEI_DIEPTSIZ), (USB_OTG_DIEPTSIZ_MULCNT & (1 << 29)));
			}
		}

		if(husb->Init.dma_enable == 1){
			flushinvalidatedcache_by_addr(((uint8_t *)ep->dma_addr), ep->xfer_len);
			sil_wrw_mem((uint32_t *)(USBIEP_BASE(base, epnum)+TOFF_USBEI_DIEPDMA), ep->dma_addr);
		}
		else{
			/*
			 *  EP��ISOC�Ǥʤ���硢�����ǡ�������ʤ��EMPTY����ߤ�����
			 */
			if(ep->type != EP_TYPE_ISOC && ep->xfer_len > 0){
				sil_orw_mem((uint32_t *)(USBD_BASE(base)+TOFF_USBD_DIEPEMPMSK), (1 << (epnum)));
			}
		}

		if(ep->type == EP_TYPE_ISOC){
			if((sil_rew_mem((uint32_t *)(USBD_BASE(base)+TOFF_USBD_DSTS)) & (1 << 8)) == 0){
				sil_orw_mem((uint32_t *)(USBIEP_BASE(base, epnum)+TOFF_USBEI_DIEPCTL), USB_OTG_DIEPCTL_SODDFRM);
			}
			else{
				sil_orw_mem((uint32_t *)(USBIEP_BASE(base, epnum)+TOFF_USBEI_DIEPCTL), USB_OTG_DIEPCTL_SD0PID_SEVNFRM);
			}
		}
		/*
		 *  ���ꥢNAK������ɥݥ���ȥ��͡��֥�
		 */
		sil_orw_mem((uint32_t *)(USBIEP_BASE(base, epnum)+TOFF_USBEI_DIEPCTL), (USB_OTG_DIEPCTL_CNAK | USB_OTG_DIEPCTL_EPENA));

		if(ep->type == EP_TYPE_ISOC){
			usbo_writePacket(base, ep->xfer_buff, epnum, ep->xfer_len, husb->Init.dma_enable);
		}
	}
	else{	 /* OUT����ɥݥ���� */
		/*
		 *  �ѥ��åȿ��ȥǡ���Ĺ��ʬ��������
		 */
		sil_andw_mem((uint32_t *)(USBOEP_BASE(base, epnum)+TOFF_USBEO_DOEPTSIZ), USB_OTG_DOEPTSIZ_XFRSIZ);
		sil_andw_mem((uint32_t *)(USBOEP_BASE(base, epnum)+TOFF_USBEO_DOEPTSIZ), USB_OTG_DOEPTSIZ_PKTCNT);

		if(ep->xfer_len == 0){	/* ZERO-LENGTH�ѥ��å� */
			sil_orw_mem((uint32_t *)(USBOEP_BASE(base, epnum)+TOFF_USBEO_DOEPTSIZ), (USB_OTG_DOEPTSIZ_XFRSIZ & ep->maxpacket));
			sil_orw_mem((uint32_t *)(USBOEP_BASE(base, epnum)+TOFF_USBEO_DOEPTSIZ), (USB_OTG_DOEPTSIZ_PKTCNT & (1 << 19)));
		}
		else{	/* �̾�ѥ��å� */
			pktcnt = (ep->xfer_len + ep->maxpacket - 1)/ ep->maxpacket; 
			sil_orw_mem((uint32_t *)(USBOEP_BASE(base, epnum)+TOFF_USBEO_DOEPTSIZ), (USB_OTG_DOEPTSIZ_PKTCNT & (pktcnt << 19)));
			sil_orw_mem((uint32_t *)(USBOEP_BASE(base, epnum)+TOFF_USBEO_DOEPTSIZ), (USB_OTG_DOEPTSIZ_XFRSIZ & (ep->maxpacket * pktcnt)));
		}

		if(husb->Init.dma_enable == 1){
			flushinvalidatedcache_by_addr(ep->xfer_buff, ep->xfer_len);
			sil_wrw_mem((uint32_t *)(USBOEP_BASE(base, epnum)+TOFF_USBEO_DOEPDMA), (uint32_t)(ep->xfer_buff));
		}

		if(ep->type == EP_TYPE_ISOC){
			if((sil_rew_mem((uint32_t *)(USBD_BASE(base)+TOFF_USBD_DSTS)) & (1 << 8)) == 0){
				sil_orw_mem((uint32_t *)(USBOEP_BASE(base, epnum)+TOFF_USBEO_DOEPCTL), USB_OTG_DOEPCTL_SODDFRM);
			}
			else{
				sil_orw_mem((uint32_t *)(USBOEP_BASE(base, epnum)+TOFF_USBEO_DOEPCTL), USB_OTG_DOEPCTL_SD0PID_SEVNFRM);
			}
		}
		/*
		 *  ���ꥢNAK������ɥݥ���ȥ��͡��֥�
		 */
		sil_orw_mem((uint32_t *)(USBOEP_BASE(base, epnum)+TOFF_USBEO_DOEPCTL), (USB_OTG_DOEPCTL_CNAK | USB_OTG_DOEPCTL_EPENA));
	}
	return E_OK;
}

/*
 *  USB�ǥХ��������
 *  parameter1 husb  USB�ϥ�ɥ�ؤΥݥ���
 *  return     ���E_OK
 */
ER
usbo_devinit(USB_OTG_Handle_t *husb)
{
	uint32_t base = husb->base;
	bool_t   forced;
	uint32_t i = 0U;

	/* Device mode configuration */
	sil_orw_mem((uint32_t *)(USBD_BASE(base)+TOFF_USBD_DCFG), DCFG_FRAME_INTERVAL_80);

	if(husb->Init.phy_itface == USB_PHY_ULPI){	/* ULIP-PHY���� */
		if(husb->Init.speed == USB_SPEED_HIGH){
			/*
			 *  �ϥ����ԡ��ɸ���
			 */
			usbo_setDevSpeed(base, USB_SPEED_HIGH);
		}
		else{
			/*
			 * �ϥ����ե�����⡼��
			 */
			usbo_setDevSpeed(base, USB_SPEED_HIGH_IN_FULL);
		}
	}
	else{	/* EMBEDDED-PHY���� */
		/*
		 *  ��˥ե륹�ԡ���
		 */
		usbo_setDevSpeed(base, USB_SPEED_FULL);
	}

	/*
	 *  �ǥХ����˴ط����롢���٤Ƥγ���ߤ򥯥ꥢ����
	 */
	sil_wrw_mem((uint32_t *)(USBD_BASE(base)+TOFF_USBD_DIEPMSK),
			(USB_OTG_DIEPMSK_TOM | USB_OTG_DIEPMSK_EPDM | USB_OTG_DIEPMSK_XFRCM));
	sil_wrw_mem((uint32_t *)(USBD_BASE(base)+TOFF_USBD_DOEPMSK),
			(USB_OTG_DOEPMSK_STUPM | USB_OTG_DOEPMSK_EPDM | USB_OTG_DOEPMSK_XFRCM));
	sil_wrw_mem((uint32_t *)(USBD_BASE(base)+TOFF_USBD_DAINT), 0xFFFFFFFF);
	sil_wrw_mem((uint32_t *)(USBD_BASE(base)+TOFF_USBD_DAINTMSK), 0);
	for(i = 0 ; i < husb->Init.dev_endpoints ; i++){
		if((sil_rew_mem((uint32_t *)(USBIEP_BASE(base, i)+TOFF_USBEI_DIEPCTL)) & USB_OTG_DIEPCTL_EPENA) != 0){
			sil_wrw_mem((uint32_t *)(USBIEP_BASE(base, i)+TOFF_USBEI_DIEPCTL), (USB_OTG_DIEPCTL_EPDIS | USB_OTG_DIEPCTL_SNAK));
		}
		else{
			sil_wrw_mem((uint32_t *)(USBIEP_BASE(base, i)+TOFF_USBEI_DIEPCTL), 0);
		}
		sil_wrw_mem((uint32_t *)(USBIEP_BASE(base, i)+TOFF_USBEI_DIEPTSIZ), 0);
		sil_wrw_mem((uint32_t *)(USBIEP_BASE(base, i)+TOFF_USBEI_DIEPINT), 0xFF);
	}

	for (i = 0U; i < husb->Init.dev_endpoints; i++){
		if((sil_rew_mem((uint32_t *)(USBOEP_BASE(base, i)+TOFF_USBEO_DOEPCTL)) & USB_OTG_DOEPCTL_EPENA) != 0){
			sil_wrw_mem((uint32_t *)(USBOEP_BASE(base, i)+TOFF_USBEO_DOEPCTL), (USB_OTG_DOEPCTL_EPDIS | USB_OTG_DOEPCTL_SNAK));
		}
		else{
			sil_wrw_mem((uint32_t *)(USBOEP_BASE(base, i)+TOFF_USBEO_DOEPCTL), 0);
		}
		sil_wrw_mem((uint32_t *)(USBOEP_BASE(base, i)+TOFF_USBEO_DOEPTSIZ), 0);
		sil_wrw_mem((uint32_t *)(USBOEP_BASE(base, i)+TOFF_USBEO_DOEPINT), 0xFF);
	}

	usbo_devdisconnect(husb);
	forced = usbo_force_mode_if_needed(husb, false);
	usbo_initFiFo(husb);
	if(forced)
		usbo_clear_force_mode(husb);

	if(husb->Init.dma_enable == 1){
		/*
		 *  DMA�⡼�ɤʤ饹��å���ۡ���ɥѥ�᡼��������
		 */
		sil_wrw_mem((uint32_t *)(USBD_BASE(base)+TOFF_USBD_DTHRCTL), (USB_OTG_DTHRCTL_TXTHRLEN_6 | USB_OTG_DTHRCTL_RXTHRLEN_6));
		sil_orw_mem((uint32_t *)(USBD_BASE(base)+TOFF_USBD_DTHRCTL), (USB_OTG_DTHRCTL_RXTHREN | USB_OTG_DTHRCTL_ISOTHREN | USB_OTG_DTHRCTL_NONISOTHREN));
		i = sil_rew_mem((uint32_t *)(USBD_BASE(base)+TOFF_USBD_DTHRCTL));
	}
	if(husb->Init.usb_otg_mode == USB_OTG_MODE_DEVICE)
		usbo_setupint(husb);
	return E_OK;
}

/*
 *  USB�ǥХ������
 *  parameter1 husb  USB�ϥ�ɥ�ؤΥݥ���
 *  return     ���E_OK
 */
ER
usbo_stopdevice(USB_OTG_Handle_t *husb)
{
	uint32_t base = husb->base;
	uint32_t i;

	/*
	 *  ����ɥݥ���Ȥγ���ߥ��ꥢ
	 */
	for(i = 0 ; i < 15 ; i++){
		sil_wrw_mem((uint32_t *)(USBIEP_BASE(base, i)+TOFF_USBEI_DIEPINT), 0xFF);
		sil_wrw_mem((uint32_t *)(USBOEP_BASE(base, i)+TOFF_USBEO_DOEPINT), 0xFF);
	}
	sil_wrw_mem((uint32_t *)(USBD_BASE(base)+TOFF_USBD_DAINT), 0xFFFFFFFF);

	/*
	 *  ����ɥݥ���Ȥγ��������
	 */
	sil_wrw_mem((uint32_t *)(USBD_BASE(base)+TOFF_USBD_DIEPMSK), 0);
	sil_wrw_mem((uint32_t *)(USBD_BASE(base)+TOFF_USBD_DOEPMSK), 0);
	sil_wrw_mem((uint32_t *)(USBD_BASE(base)+TOFF_USBD_DAINTMSK), 0);

	/*
	 *  FIFO�ե�å���
	 */
	usbo_flushRxFifo(husb);
	usbo_flushTxFifo(husb,  0x10);
	return E_OK;
}

/*
 *  LPM�������
 */
ER
usbo_init_lpm(USB_OTG_Handle_t *husb)
{
#ifdef USB_OTG_GLPMCFG_LPMEN
	uint32_t base = husb->base;

	if(husb->Init.lpm_enable == 1){
		husb->lpm_active = 1;
		husb->lpm_state = PCD_LPM_L0_ACTIVE;
		sil_orw_mem((uint32_t *)(USBOTG_BASE(base)+TOFF_USBO_GINTMSK), USB_OTG_GINTMSK_LPMINTM);
		sil_orw_mem((uint32_t *)(USBOTG_BASE(base)+TOFF_USBO_GLPMCFG), (USB_OTG_GLPMCFG_LPMEN | USB_OTG_GLPMCFG_LPMACK | USB_OTG_GLPMCFG_ENBESL));
	}
#endif /* USB_OTG_GLPMCFG_LPMEN */
	return E_OK;
}

/*
 *  USB EP-IN������װ�����
 *  parameter1 base USB OTG�쥸�����١������ɥ쥹
 *  parameter2 epnum ����ɥݥ�����ֹ�
 *  return     EP-IN������װ�
 */
static uint32_t
usbo_devInEPInterrupt(uint32_t base , uint8_t epnum)
{
	uint32_t msk, emp;

	msk = sil_rew_mem((uint32_t *)(USBD_BASE(base)+TOFF_USBD_DIEPMSK));
	emp = sil_rew_mem((uint32_t *)(USBD_BASE(base)+TOFF_USBD_DIEPEMPMSK));
	msk |= ((emp >> epnum) & 0x1) << 7;
	return sil_rew_mem((uint32_t *)(USBIEP_BASE(base, epnum)+TOFF_USBEI_DIEPINT)) & msk;
}

/*
 *  USB EP-OUT������װ�����
 *  parameter1 base USB OTG�쥸�����١������ɥ쥹
 *  parameter2 epnum ����ɥݥ�����ֹ�
 *  return     EP-OUT������װ�
 */
static uint32_t
usbo_devOutEPInterrupt(uint32_t base, uint8_t epnum)
{
	uint32_t v;
	v = sil_rew_mem((uint32_t *)(USBOEP_BASE(base, epnum)+TOFF_USBEO_DOEPINT));
	return v & sil_rew_mem((uint32_t *)(USBD_BASE(base)+TOFF_USBD_DOEPMSK));
}


/*
 *  ���Υѥ��åȤ�����FIFO������
 *  parameter1 husb  USB�ϥ�ɥ�ؤΥݥ���
 *  parameter2 epnum ����ɥݥ�����ֹ�
 *  return     ���E_OK
 */
static ER
usbo_writeemptyTxfifo(USB_OTG_Handle_t *husb, uint32_t epnum)
{
	uint32_t  base = husb->base;
	USB_OTG_EPTypeDef *ep;
	int32_t len = 0U;
	uint32_t len32b;

	ep = &husb->IN_ep[epnum];
	len = ep->xfer_len - ep->xfer_count;

	if(len > ep->maxpacket){
		len = ep->maxpacket;
	}
	len32b = (len + 3) / 4;

	while((sil_rew_mem((uint32_t *)(USBIEP_BASE(base, epnum)+TOFF_USBEI_DTXFSTS)) & USB_OTG_DTXFSTS_INEPTFSAV) > len32b
          && ep->xfer_count < ep->xfer_len && ep->xfer_len != 0){
		/*
		 * ����FIFO�ؤΥǡ����񤭹���
		 */
		len = ep->xfer_len - ep->xfer_count;
		if(len > ep->maxpacket){
			len = ep->maxpacket;
		}
		len32b = (len + 3) / 4;
		usbo_writePacket(base, ep->xfer_buff, epnum, len, husb->Init.dma_enable); 
		ep->xfer_buff  += len;
		ep->xfer_count += len;
	}

	if(len <= 0){	/* �Ǹ�Υѥ��åȤʤ��EMPTY����ߤ�ޥ��� */
		sil_andw_mem((uint32_t *)(USBD_BASE(base)+TOFF_USBD_DIEPEMPMSK), (1 << epnum));
	}
	return E_OK;
}

/*
 *  DEVICE����ߥϥ�ɥ�
 *  parameter1 husb  USB�ϥ�ɥ�ؤΥݥ���
 */
void
usbo_pcd_irqhandler(USB_OTG_Handle_t *husb)
{
	USB_OTG_EPTypeDef *ep;
	uint32_t base = husb->base;
	uint32_t msk  = sil_rew_mem((uint32_t *)(USBOTG_BASE(base)+TOFF_USBO_GINTMSK));
	uint32_t i = 0U, ep_intr = 0U, epint = 0U, epnum = 0U;
	uint32_t temp = 0;

	/*
	 *  ����ߥ��顼�����å�
	 */
	if((get_int_status(base, msk) == 0)){
		uint32_t intsts = sil_rew_mem((uint32_t *)(USBOTG_BASE(base)+TOFF_USBO_GINTSTS));
		syslog_1(LOG_EMERG, "usbo_pcd_irqhandler Error Intrrupt[%08x] !", intsts);
		clear_dev_int(base, intsts);
		return;
	}

    if((get_int_status(base, msk) & USB_OTG_GINTSTS_MMIS) != 0){
		/*
		 *  ���顼������װ����ꥢ
		 */
		clear_dev_int(base, USB_OTG_GINTSTS_MMIS);
	}
	if((get_int_status(base, msk) & USB_OTG_GINTSTS_OEPINT) != 0){
		/*
		 *  EP-OUT�����
		 */
		ep_intr = get_outep_int_status(base);
		for(epnum = 0 ; epnum < 16 ; epnum++){
			if((ep_intr & (1<<epnum)) != 0){
				epint = usbo_devOutEPInterrupt(base, epnum);
				if((epint & USB_OTG_DOEPINT_XFRC) == USB_OTG_DOEPINT_XFRC){
					clear_outep_int(base, epnum, USB_OTG_DOEPINT_XFRC);
					if(husb->Init.dma_enable == 1U){
						husb->OUT_ep[epnum].xfer_count = husb->OUT_ep[epnum].maxpacket- (sil_rew_mem((uint32_t *)(USBOEP_BASE(base, epnum)+TOFF_USBEO_DOEPTSIZ)) & USB_OTG_DOEPTSIZ_XFRSIZ); 
						husb->OUT_ep[epnum].xfer_buff += husb->OUT_ep[epnum].maxpacket;
					}
					if(husb->devdataoutstagecallback != NULL)
						husb->devdataoutstagecallback(husb, epnum);
					if(husb->Init.dma_enable == 1 && epnum == 0 && husb->OUT_ep[epnum].xfer_len == 0){
						/*
						 *  DMA�⡼��EP0 ZERO LENGTH�ѥ��å�����
						 */
						usbo_ep0_outstart(husb, (uint8_t *)husb->Setup);
					}
				}
				if((epint & USB_OTG_DOEPINT_STUP) != 0){
					/*
					 *  SETUP�ѥ��å�����
					 */
					if(husb->devsetupstagecallback != NULL)
						husb->devsetupstagecallback(husb);
					clear_outep_int(base, epnum, USB_OTG_DOEPINT_STUP);
				}
				if((epint & USB_OTG_DOEPINT_OTEPDIS) != 0){
					clear_outep_int(base, epnum, USB_OTG_DOEPINT_OTEPDIS);
				}
#ifdef USB_OTG_DOEPINT_OTEPSPR 
				/*
				 *  ���ơ������ե��������������
				 */
				if((epint & USB_OTG_DOEPINT_OTEPSPR) != 0){
					clear_outep_int(base, epnum, USB_OTG_DOEPINT_OTEPSPR);
				}
#endif /* USB_OTG_DOEPINT_OTEPSPR */
			}
		}
	}
	if((get_int_status(base, msk) & USB_OTG_GINTSTS_IEPINT) != 0){
		/*
		 *  EP-IN�����
		 */
		ep_intr = get_inep_int_status(base);
		for(epnum = 0 ; epnum < 16 ; epnum++){
			if((ep_intr & (1<<epnum)) != 0){ /* In ITR */
				epint = usbo_devInEPInterrupt(base, epnum);
				if((epint & USB_OTG_DIEPINT_XFRC) != 0){
					sil_andw_mem((uint32_t *)(USBD_BASE(base)+TOFF_USBD_DIEPEMPMSK), (1<<epnum));
					clear_inep_int(base, epnum, USB_OTG_DIEPINT_XFRC);
					if(husb->Init.dma_enable == 1){
						husb->IN_ep[epnum].xfer_buff += husb->IN_ep[epnum].maxpacket; 
					}
					if(husb->devdatainstagecallback != NULL)
						husb->devdatainstagecallback(husb, epnum);
					if(husb->Init.dma_enable == 1 && epnum == 0 && husb->IN_ep[epnum].xfer_len == 0){
						/*
						 *  DMA�⡼��EP0 ZERO LENGTH�ѥ��å�����
						 */
						usbo_ep0_outstart(husb, (uint8_t *)husb->Setup);
					}
				}
				if((epint & USB_OTG_DIEPINT_TOC) != 0){
					clear_inep_int(base, epnum, USB_OTG_DIEPINT_TOC);
				}
				if((epint & USB_OTG_DIEPINT_ITTXFE) != 0){
					clear_inep_int(base, epnum, USB_OTG_DIEPINT_ITTXFE);
				}
				if((epint & USB_OTG_DIEPINT_INEPNE) != 0){
					clear_inep_int(base, epnum, USB_OTG_DIEPINT_INEPNE);
				}
				if((epint & USB_OTG_DIEPINT_EPDISD) != 0){
					clear_inep_int(base, epnum, USB_OTG_DIEPINT_EPDISD);
				}
				if((epint & USB_OTG_DIEPINT_TXFE) != 0){
					usbo_writeemptyTxfifo(husb , epnum);
				}
			}
		}
	}
	/*
	 *  RESUME�����
	 */
	if((get_int_status(base, msk) & USB_OTG_GINTSTS_WKUINT) != 0){
		usbo_exit_hibernation(husb, true);		/* �ʥ������� */
#ifdef USB_OTG_GLPMCFG_LPMEN
		if(husb->lpm_state == PCD_LPM_L1_ACTIVE){
			husb->lpm_state = PCD_LPM_L0_ACTIVE;
			if(husb->devlpmcallback != NULL)
				husb->devlpmcallback(husb, PCD_LPM_L0_ACTIVE);
		}
		else
#endif /* USB_OTG_GLPMCFG_LPMEN */
		{
			if(husb->devresumecallback != NULL)
				husb->devresumecallback(husb);
		}
		husb->lx_state = DWC2_L0;
		clear_dev_int(base, USB_OTG_GINTSTS_WKUINT);
	}
    /*
	 *  SUSPEND�����
	 */
    if((get_int_status(base, msk) & USB_OTG_GINTSTS_USBSUSP) != 0){
		if((sil_rew_mem((uint32_t *)(USBD_BASE(base)+TOFF_USBD_DSTS)) & USB_OTG_DSTS_SUSPSTS) != 0){
			usbo_enter_hibernation(husb);		/* �ʥ��Ͱܹ� */
			if(husb->devsuspendcallback != NULL && husb->connected)
				husb->devsuspendcallback(husb);
			if(husb->Init.usb_otg_mode == USB_OTG_MODE_DRD && husb->hostdisconnectcallback != NULL && husb->connected){
				syslog_1(LOG_NOTICE, "usbo_pcd_irqhandler drd mode disconnect lx_state(%d) !", husb->lx_state);
				husb->connected = 0;
				husb->hostdisconnectcallback(husb);
			}
			husb->lx_state = DWC2_L2;
		}
		clear_dev_int(base, USB_OTG_GINTSTS_USBSUSP);
	}
#ifdef USB_OTG_GLPMCFG_LPMEN
    /*
	 *  LPM�����
	 */
	if((get_int_status(base, msk) & USB_OTG_GINTSTS_LPMINT) != 0){
		clear_dev_int(base, USB_OTG_GINTSTS_LPMINT);
		if(husb->lpm_state == PCD_LPM_L0_ACTIVE){
			husb->lpm_state = PCD_LPM_L1_ACTIVE;
			husb->BESL = (sil_rew_mem((uint32_t *)(USBOTG_BASE(base)+TOFF_USBO_GLPMCFG)) & USB_OTG_GLPMCFG_BESL) >>2 ;
			if(husb->devlpmcallback != NULL)
				husb->devlpmcallback(husb, PCD_LPM_L1_ACTIVE);
			husb->lx_state = DWC2_L1;
		}
		else{
			if(husb->devsuspendcallback != NULL)
				husb->devsuspendcallback(husb);
			husb->lx_state = DWC2_L2;
		}
	}
#endif /* USB_OTG_GLPMCFG_LPMEN */
	/*
	 * USB-RESET�����
	 */
	if((get_int_status(base, msk) & USB_OTG_GINTSTS_USBRST) != 0){
		sil_andw_mem((uint32_t *)(USBD_BASE(base)+TOFF_USBD_DCTL), USB_OTG_DCTL_RWUSIG);
		usbo_flushTxFifo(husb, 0U);
		for(i = 0; i < husb->Init.dev_endpoints ; i++){
			sil_wrw_mem((uint32_t *)(USBIEP_BASE(base, i)+TOFF_USBEI_DIEPINT), 0xFF);
			sil_wrw_mem((uint32_t *)(USBOEP_BASE(base, 0)+TOFF_USBEO_DOEPINT), 0xFF);
		}
		sil_wrw_mem((uint32_t *)(USBD_BASE(base)+TOFF_USBD_DAINT), 0xFFFFFFFF);
		sil_orw_mem((uint32_t *)(USBD_BASE(base)+TOFF_USBD_DAINTMSK), 0x10001);
		if(husb->Init.use_dedicated_ep1){
			sil_orw_mem((uint32_t *)(USBD_BASE(base)+TOFF_USBD_DOUTEP1MSK), (USB_OTG_DOEPMSK_STUPM | USB_OTG_DOEPMSK_XFRCM | USB_OTG_DOEPMSK_EPDM));
			sil_orw_mem((uint32_t *)(USBD_BASE(base)+TOFF_USBD_DINEP1MSK), (USB_OTG_DIEPMSK_TOM | USB_OTG_DIEPMSK_XFRCM | USB_OTG_DIEPMSK_EPDM));
		}
		else{
#ifdef USB_OTG_DOEPINT_OTEPSPR
			sil_orw_mem((uint32_t *)(USBD_BASE(base)+TOFF_USBD_DOEPMSK), (USB_OTG_DOEPMSK_STUPM | USB_OTG_DOEPMSK_XFRCM | USB_OTG_DOEPMSK_EPDM | USB_OTG_DOEPMSK_OTEPSPRM));
#else
			sil_orw_mem((uint32_t *)(USBD_BASE(base)+TOFF_USBD_DOEPMSK), (USB_OTG_DOEPMSK_STUPM | USB_OTG_DOEPMSK_XFRCM | USB_OTG_DOEPMSK_EPDM));
#endif /* USB_OTG_DOEPINT_OTEPSPR */
			sil_orw_mem((uint32_t *)(USBD_BASE(base)+TOFF_USBD_DIEPMSK), (USB_OTG_DIEPMSK_TOM | USB_OTG_DIEPMSK_XFRCM | USB_OTG_DIEPMSK_EPDM));
		}
		/*
		 *  �ǥե���ȥ��ɥ쥹�򥼥������
		 */
		sil_andw_mem((uint32_t *)(USBD_BASE(base)+TOFF_USBD_DCFG), USB_OTG_DCFG_DAD);
		/*
		 *  SETUP�ѥ��åȼ�������
		 */
		usbo_ep0_outstart(husb, (uint8_t *)husb->Setup);
		clear_dev_int(base, USB_OTG_GINTSTS_USBRST);
	}
	/*
	 *  ENUMERATION�ϥ�ɥ뽪λ�����
	 */
	if((get_int_status(base, msk) & USB_OTG_GINTSTS_ENUMDNE) != 0){
		/*
		 *  USB�����ƥ��١��ȥ��åȥ��å�
		 */
		sil_andw_mem((uint32_t *)(USBIEP_BASE(base, 0)+TOFF_USBEI_DIEPCTL), USB_OTG_DIEPCTL_MPSIZ);
		if((sil_rew_mem((uint32_t *)(USBD_BASE(base)+TOFF_USBD_DSTS)) & USB_OTG_DSTS_ENUMSPD) == DSTS_ENUMSPD_LS_PHY_6MHZ){
			sil_orw_mem((uint32_t *)(USBIEP_BASE(base, 0)+TOFF_USBEI_DIEPCTL), 3);
		}
		sil_orw_mem((uint32_t *)(USBD_BASE(base)+TOFF_USBD_DCTL), USB_OTG_DCTL_CGINAK);

		sil_andw_mem((uint32_t *)(USBOTG_BASE(base)+TOFF_USBO_GUSBCFG), USB_OTG_GUSBCFG_TRDT);
		if(usbo_getDevSpeed(husb) == USB_SPEED_HIGH){
			husb->Init.speed     = USB_SPEED_HIGH;
			sil_orw_mem((uint32_t *)(USBOTG_BASE(base)+TOFF_USBO_GUSBCFG), ((USBD_HS_TRDT_VALUE << 10) & USB_OTG_GUSBCFG_TRDT));
		}
		else{
			husb->Init.speed     = USB_SPEED_FULL;

        /*
		 * USBTRDT������ϡ�AHB��������(SysFreHCLK)�˽���
		 */
#if (SysFreHCLK < 15000000)		/* under 15MHz */
			temp = 0x0F;
#elif (SysFreHCLK >= 15000000) && (SysFreHCLK < 16000000)	/* 15Mhz-16MHz */
			temp = 0x0E;
#elif (SysFreHCLK >= 16000000) && (SysFreHCLK < 17200000)	/* 16MHz-17.2MHz */
			temp = 0x0D;
#elif (SysFreHCLK >= 17200000) && (SysFreHCLK < 18500000)	/* 17.2MHz-18.5MHz */
			temp = 0x0C;
#elif (SysFreHCLK >= 18500000) && (SysFreHCLK < 20000000)	/* 18.5MHz-20MHz */
			temp = 0x0B;
#elif (SysFreHCLK >= 20000000) && (SysFreHCLK < 21800000)	/* 20MHz-21.8MHz */
			temp = 0x0A;
#elif (SysFreHCLK >= 21800000) && (SysFreHCLK < 24000000)	/* 21.8MHz-24MHz */
			temp = 0x09;
#elif (SysFreHCLK >= 24000000) && (SysFreHCLK < 27700000)	/* 24MHz-27.7MHz */
			temp = 0x08;
#elif (SysFreHCLK >= 27700000) && (SysFreHCLK < 32000000)	/* 27.7MHz-32MHz */
			temp = 0x07;
#else							/* over 32MHz */
			temp = 0x06;
#endif
			sil_orw_mem((uint32_t *)(USBOTG_BASE(base)+TOFF_USBO_GUSBCFG), ((temp << 10) & USB_OTG_GUSBCFG_TRDT));
		}
		if(husb->devresetcallback != NULL)
			husb->devresetcallback(husb);
		clear_dev_int(base, USB_OTG_GINTSTS_ENUMDNE);
	}
    /*
	 *  RXQLEVEL�����
	 */
	if((get_int_status(base, msk) & USB_OTG_GINTSTS_RXFLVL) != 0){
		sil_andw_mem((uint32_t *)(USBOTG_BASE(base)+TOFF_USBO_GINTMSK), USB_OTG_GINTSTS_RXFLVL);
		temp = sil_rew_mem((uint32_t *)(USBOTG_BASE(base)+TOFF_USBO_GRXSTSP));
		ep = &husb->OUT_ep[temp & USB_OTG_GRXSTSP_EPNUM];
		if(((temp & USB_OTG_GRXSTSP_PKTSTS) >> 17U) ==  STS_DATA_UPDT){
			if((temp & USB_OTG_GRXSTSP_BCNT) != 0){
				usbo_readPacket(base, ep->xfer_buff, (temp & USB_OTG_GRXSTSP_BCNT) >> 4U);
				ep->xfer_buff += (temp & USB_OTG_GRXSTSP_BCNT) >> 4U;
				ep->xfer_count += (temp & USB_OTG_GRXSTSP_BCNT) >> 4U;
			}
		}
		else if(((temp & USB_OTG_GRXSTSP_PKTSTS) >> 17U) ==  STS_SETUP_UPDT){
			usbo_readPacket(base, (uint8_t *)husb->Setup, 8U);
			ep->xfer_count += (temp & USB_OTG_GRXSTSP_BCNT) >> 4U;
		}
		sil_orw_mem((uint32_t *)(USBOTG_BASE(base)+TOFF_USBO_GINTMSK), USB_OTG_GINTSTS_RXFLVL);
	}
    /*
	 *  DEVICE-SOF�����
	 */
	if((get_int_status(base, msk) & USB_OTG_GINTSTS_SOF) != 0){
		if(husb->devsofcallback != NULL)
			husb->devsofcallback(husb);
		clear_dev_int(base, USB_OTG_GINTSTS_SOF);
	}
    /*
	 *  ISO IN̤��������
	 */
    if((get_int_status(base, msk) & USB_OTG_GINTSTS_IISOIXFR) != 0){
		if(husb->devisoincallback != NULL)
			husb->devisoincallback(husb, epnum);
		clear_dev_int(base, USB_OTG_GINTSTS_IISOIXFR);
    }
	/*
	 *  ISO OUT̤��������
	 */
	if((get_int_status(base, msk) & USB_OTG_GINTSTS_PXFR_INCOMPISOOUT) != 0){
		if(husb->devisooutcallback != NULL)
			husb->devisooutcallback(husb, epnum);
		clear_dev_int(base, USB_OTG_GINTSTS_PXFR_INCOMPISOOUT);
	}
    /*
	 *  ���ͥ�����󥤥٥�ȳ����
	 */
    if((get_int_status(base, msk) & USB_OTG_GINTSTS_SRQINT) != 0){
		if(husb->devconnectcallback != NULL)
			husb->devconnectcallback(husb);
		clear_dev_int(base, USB_OTG_GINTSTS_SRQINT);
	}
    /*
	 *  �ǥ������ͥ�����󥤥٥�ȳ����
	 */
    if((get_int_status(base, msk) & USB_OTG_GINTSTS_OTGINT) != 0){
		temp = sil_rew_mem((uint32_t *)(USBOTG_BASE(base)+TOFF_USBO_GOTGINT));
		if((temp & USB_OTG_GOTGINT_SEDET) == USB_OTG_GOTGINT_SEDET){
			husb->connected = 0;
			if(husb->devdisconnectcallback != NULL)
				husb->devdisconnectcallback(husb);
		}
		sil_orw_mem((uint32_t *)(USBOTG_BASE(base)+TOFF_USBO_GOTGINT), temp);
	}
}

/*
 *  USB-OTG����ߥ����ӥ��롼����
 */
void
usb_otg_isr(intptr_t exinf)
{
	USB_OTG_Handle_t *husb = &Husb[INDEX_USB((uint32_t)exinf)];
	if(get_usbmode(husb->base) == USB_OTG_MODE_DEVICE){
		usbo_pcd_irqhandler(husb);
	}
	else{
		usbo_hcd_irqhandler(husb);
	}
}

/*
 *  USB-OTG WAKEUP����ߥ����ӥ��롼����
 */
void usb_otg_wkup_isr(intptr_t exinf)
{
	USB_OTG_Handle_t *husb = &Husb[INDEX_USB((uint32_t)exinf)];

	if(husb->Init.low_power_enable){
		/*
		 *  Cortex SLEEPDEEP�ӥåȥꥻ�å�
		 */
		sil_andw_mem((uint32_t *)(TADR_SCB_BASE+TOFF_SCB_SCR), (SCB_SCR_SLEEPDEEP_Msk | SCB_SCR_SLEEPONEXIT_Msk));
#if !defined(TOPPERS_STM32F4_DISCOVERY)
		/*
		 *  ��ư����å�������
		 */
		sysclock_config(0);
#endif
	}

	/* ungate PHY clock */
	sil_andw_mem((uint32_t *)(USBPGC_BASE(husb->base)), USB_OTG_PCGCCTL_STOPCLK);

	/*
	 *  EXTI WAKEUP�ڥ�ǥ��󥰥ӥåȥ��ꥢ
	 */
	sil_wrw_mem((uint32_t *)(TADR_EXTI_BASE+TOFF_EXTI_PR), USB_OTG_FS_WAKEUP_EXTI_LINE);
}

