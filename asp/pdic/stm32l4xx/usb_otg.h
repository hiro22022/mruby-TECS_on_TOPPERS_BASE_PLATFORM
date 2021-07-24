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
 *  $Id: usb_otg.h 2416 2016-07-12 18:11:28Z roi $
 */

/*
 *	USB OTG�ǥХ����ɥ饤�ФΥإå��ե�����
 */

#ifndef _USB_OTG_H_
#define _USB_OTG_H_


/*
 *  USB�ݡ������
 */
#define USB1_PORTID             1
#define USB2_PORTID             2
#define NUM_USBPORT             2

/*
 *  USB OTG�⡼��
 */
#define USB_OTG_MODE_DEVICE     0
#define USB_OTG_MODE_HOST       1
#define USB_OTG_MODE_DRD        2

/*
 *  USB PHY���
 */
#define USB_PHY_ULPI            1
#define USB_PHY_EMBEDDED        2

/*
 *  ����ɥݥ���ȥ�����
 */
#define EP_TYPE_CTRL            0
#define EP_TYPE_ISOC            1
#define EP_TYPE_BULK            2
#define EP_TYPE_INTR            3
#define EP_TYPE_MSK             3

/*
 *  USB���ԡ���
 */
#define USB_SPEED_HIGH          0
#define USB_SPEED_HIGH_IN_FULL  1
#define USB_SPEED_LOW           2
#define USB_SPEED_FULL          3


#define HC_PID_DATA0            0
#define HC_PID_DATA2            1
#define HC_PID_DATA1            2
#define HC_PID_SETUP            3

/*
 *  URB�������
 */ 
#define URB_IDLE                0
#define URB_DONE                1
#define URB_NOTREADY            2
#define URB_NYET                3
#define URB_ERROR               4
#define URB_STALL               5

/*
 *  HOST�����ͥ�������
 */
#define USBO_HC_IDLE            0
#define USBO_HC_XFRC            1
#define USBO_HC_HALTED          2
#define USBO_HC_NAK             3
#define USBO_HC_NYET            4
#define USBO_HC_STALL           5
#define USBO_HC_XACTERR         6
#define USBO_HC_BBLERR          7
#define USBO_HC_DATATGLERR      8
#define USBO_HC_FAIL            16

/*
 *  LPM�����ƥ��֥ѥ�᡼��
 */
#define PCD_LPM_L0_ACTIVE       0x00		/* on */
#define PCD_LPM_L1_ACTIVE       0x01		/* LPM L1 sleep */
#define PCD_LPM_L2              0x02		/* suspend */
#define PCD_LPM_L3              0x03		/* off */

/*
 *  ����ɥݥ���ȡ��ۥ��ȥ���ͥ�κ����
 */
#define MAX_EPS_CHANNELS        16

/*
 *  �ǥХ������ơ���
 */
#define DWC2_L0                 0	/* On state */
#define DWC2_L1                 1	/* LPM sleep state */
#define DWC2_L2                 2	/* USB suspend state */
#define DWC2_L3                 3	/* Off state */


/*
 *  USB OTG�ϡ��ɥ���������
 */
typedef struct
{
	unsigned en_multiple_tx_fifo:1;
	unsigned host_rx_fifo_size:16;
	unsigned max_transfer_size:26;
	unsigned max_packet_count:11;
	unsigned total_fifo_size:16;
}USB_OTG_HwParam_t;

/*
 *  USB OTG����������
 */
typedef struct
{
    uint32_t                usb_otg_mode;			/* USB OTG�⡼�� */
    uint32_t                dev_endpoints;			/* �ǥХ�������ɥݥ���ȿ�(1-15) */
    uint32_t                host_channels;			/* �ۥ��ȥ���ͥ��(1-15) */
    uint32_t                speed;					/* USB�������ԡ��� */
    uint32_t                dma_enable;				/* USB-DMA��ǽ��ͭ��̵������(0 or 1) */
    uint32_t                phy_itface;				/* PHY���󥿡��ե��������� */
    uint32_t                sof_enable;				/* SOF����ߤ�ͭ��̵������(0 or 1) */
    uint32_t                low_power_enable;		/* LOW POWER�⡼��ͭ��̵������(0 or 1) */
    uint32_t                lpm_enable;				/* Link Power Managementͭ��̵������(0 or 1) */
    uint32_t                vbus_sensing_enable;	/* VBUS����ͭ��̵������(0 or 1) */
    uint32_t                use_dedicated_ep1;		/* ����ɥݥ����1���ѳ����ͭ��̵������(0 or 1) */
    uint32_t                use_external_vbus;		/* ����VBUSͭ��̵������(0 or 1) */
}USB_OTG_Init_t;

/*
 *  ����ɥݥ�������
 */
typedef struct
{
    uint8_t                 num;					/* ����ɥݥ�����ֹ�(0-14) */
    uint8_t                 is_in : 1;				/* ����ɥݥ����OUT/IN(0 or 1) */
    uint8_t                 is_stall : 1;			/* ����ɥݥ���ȥ����������(0 or 1) */
    uint8_t                 type : 2;				/* ����ɥݥ���ȥ����� */
    uint8_t                 data_pid_start : 1;		/* ����ǡ���PID(0 or 1) */
    uint8_t                 even_odd_frame : 1;		/* �ե졼��ѥ�ƥ�(0 or 1) */
    uint16_t                tx_fifo_num;			/* ž��FIFO�ֹ�(1-15) */
    uint16_t                maxpacket;				/* ����ɥݥ���Ⱥ���ѥ��åȥ�����(0-64KB) */
    uint8_t                 *xfer_buff;				/* ž���Хåե��ݥ��󥿡� */
    uint32_t                dma_addr;				/* DMAž�����ɥ쥹(32bit���饤��) */
    uint32_t                xfer_len;				/* ���ߤ�ž���� */
    uint32_t                xfer_count;				/* ����ž��������� */
}USB_OTG_EPTypeDef;

/*
 *  �ۥ��ȥ��饹���
 */
typedef struct
{
    uint8_t                 dev_addr;				/* USB�ǥХ������ɥ쥹(1-255) */
    uint8_t                 ch_num;					/* Host����ͥ��ֹ�(1-15) */
    uint8_t                 ep_num;					/* ����ɥݥ�����ֹ�(1-15) */
	uint8_t                 ep_is_in;				/* ����ɥݥ���ȤΥǥ��쥯�����(0 or 1) */
	uint8_t                 speed;					/* USB Host���ԡ��� */
	uint8_t                 do_ping;				/* HS�⡼�ɤ�PING�ץ�ȥ����ͭ��̵������(0 or 1) */
	uint8_t                 process_ping;			/* PING�ץ�ȥ���¹���ե饰(1�Ǽ¹���) */
	uint8_t                 ep_type;				/* ����ɥݥ���ȥ�����(0-3) */
	uint16_t                max_packet;				/* ����ѥ��åȥ�����(64KB����) */
	uint8_t                 data_pid;				/* �������ǡ���PID. */
	uint8_t                 *xfer_buff;				/* ž���Хåե��ؤΥݥ��� */
	uint32_t                xfer_len;				/* ���ߤ�ž���� */
	uint32_t                xfer_count;				/* ����ž�������� */
	uint8_t                 toggle_in;				/* IN transfer current toggle flag. */
	uint8_t                 toggle_out;				/* OUT transfer current toggle flag */
	uint32_t                dma_addr;				/* DMAž�����ɥ쥹(4�Х��ȥ��饤��) */
	uint32_t                err_count;				/* Host����ͥ륨�顼�����󥿡� */
	volatile uint8_t        urb_state;				/* URB���ơ��� */
	volatile uint8_t        state;					/* Host����ͥ륹�ơ��� */
}USB_OTG_HCTypeDef;


#ifndef USBD_HS_TRDT_VALUE
 #define USBD_HS_TRDT_VALUE           9U
#endif /* USBD_HS_TRDT_VALUE */


#define USBOTG_BASE(a)    ((uint32_t)(a))
#define USBPGC_BASE(a)    ((uint32_t)(a)+USB_OTG_PCGCCTL_BASE)
#define USBPRT_BASE(a)    ((uint32_t)(a)+USB_OTG_HOST_PORT_BASE)
#define USBD_BASE(a)      ((uint32_t)(a)+USB_OTG_DEVICE_BASE)
#define USBIEP_BASE(a, i) ((uint32_t)(a)+USB_OTG_IN_ENDPOINT_BASE+((i)*USBO_EP_REG_SIZE))
#define USBOEP_BASE(a, i) ((uint32_t)(a)+USB_OTG_OUT_ENDPOINT_BASE+((i)*USBO_EP_REG_SIZE))
#define USBD_FIFO(a, i)   ((uint32_t)(a)+USB_OTG_FIFO_BASE+((i)*USB_OTG_FIFO_SIZE))
#define USBH_BASE(a)      ((uint32_t)(a)+USB_OTG_HOST_BASE)
#define USBHC_BASE(a, i)  ((uint32_t)(a)+USB_OTG_HOST_CHANNEL_BASE+((i)*USBO_HOST_CHANNEL_REG_SIZE))


#ifndef TOPPERS_MACRO_ONLY


/*
 *   USB_OTG�ǥХ����ϥ�ɥ����
 */
typedef struct _USB_OTG_Handle_t USB_OTG_Handle_t;
struct _USB_OTG_Handle_t
{
	uint32_t              base;			/* USB OTG �쥸�����١������ɥ쥹 */
	USB_OTG_Init_t        Init;			/* USB OTG ������� */
	USB_OTG_HCTypeDef     hc[MAX_EPS_CHANNELS];			/* HOST ����ͥ�ǡ��� */
	USB_OTG_EPTypeDef     IN_ep[MAX_EPS_CHANNELS-1];	/* IN ����ɥݥ���ȥǡ��� */
	USB_OTG_EPTypeDef     OUT_ep[MAX_EPS_CHANNELS-1];	/* OUT ����ɥݥ���ȥǡ��� */
	volatile uint32_t     Setup[12];					/* Setup �ѥ��åȥХåե� */
	uint8_t               lx_state;						/* lx state */
	uint8_t               enabled;						/* device enabled */
	uint8_t               connected;					/* device ��³�� */
	uint8_t               dedicated_fifos;				/* device dedicated fifo mode */
	uint32_t              resetbit;						/* usb reset bit */
	uint32_t              fifo_mem;						/* fifo memory������ */
	uint32_t              pcd_rx_fifo_sz;				/* pcd rx fifo������ */
	uint32_t              pcd_np_g_tx_fifo_sz;			/* pcd g tx fifo������ */
	uint32_t              pcd_tx_fifo_sz[MAX_EPS_CHANNELS];	/* pcd tx fifo�ơ��֥� */
#ifdef USB_OTG_GLPMCFG_LPMEN
	volatile uint32_t     BESL;			/* BESL��¸�ΰ� */
	volatile uint8_t      lpm_state;	/* LPM ���� */
	uint8_t               lpm_active;	/* Link Power Managementͭ��̵������(0 or 1) */
#endif /* USB_OTG_GLPMCFG_LPMEN */
	USB_OTG_HwParam_t     hw_params;					/* �ϡ��ɥ����������� */
	void                  (*hostsofcallback)(USB_OTG_Handle_t * hhcd);		/* sof������Хå��ؿ� */
 	void                  (*hostconnectcallback)(USB_OTG_Handle_t * hhcd);	/* connect������Хå��ؿ� */
 	void                  (*hostdisconnectcallback)(USB_OTG_Handle_t * hhcd);	/* disconnect������Хå��ؿ� */
 	void                  (*hostchangeurbcallback)(USB_OTG_Handle_t * hhcd, uint8_t num, uint8_t state);	/* changeurb������Хå��ؿ� */
	void                  (*devsetupstagecallback)(USB_OTG_Handle_t * hhcd);
	void                  (*devdataoutstagecallback)(USB_OTG_Handle_t * hhcd, uint8_t epnum);
	void                  (*devdatainstagecallback)(USB_OTG_Handle_t * hhcd, uint8_t epnum);
	void                  (*devsofcallback)(USB_OTG_Handle_t * hhcd);
	void                  (*devresetcallback)(USB_OTG_Handle_t * hhcd);
	void                  (*devsuspendcallback)(USB_OTG_Handle_t * hhcd);
	void                  (*devresumecallback)(USB_OTG_Handle_t * hhcd);
	void                  (*devisooutcallback)(USB_OTG_Handle_t * hhcd, uint8_t epnum);
	void                  (*devisoincallback)(USB_OTG_Handle_t * hhcd, uint8_t epnum);
	void                  (*devconnectcallback)(USB_OTG_Handle_t * hhcd);
	void                  (*devdisconnectcallback)(USB_OTG_Handle_t * hhcd);
	void                  (*devlpmcallback)(USB_OTG_Handle_t * hhcd, uint8_t msg);
	void                  *pHost;		/* Pointer Host Stack Handler    */
	void                  *pDev;		/* Pointer Device Stack Handler */
};

extern USB_OTG_Handle_t *usbo_init(ID portid, USB_OTG_Init_t *pini);
extern ER usbo_deinit(USB_OTG_Handle_t *husb);
extern ER usbo_devinit(USB_OTG_Handle_t *husb);
extern ER usbo_coreinit(USB_OTG_Handle_t *husb);
extern ER usbo_setupint(USB_OTG_Handle_t *husb);
extern ER usbo_enableglobalint(USB_OTG_Handle_t *husb);
extern ER usbo_disableglobalint(USB_OTG_Handle_t *husb);
extern ER usbo_setcurrentmode(USB_OTG_Handle_t *husb);
extern ER usbo_flushRxFifo(USB_OTG_Handle_t *husb);
extern ER usbo_flushTxFifo(USB_OTG_Handle_t *husb, uint32_t num);
extern ER usbo_initFiFo(USB_OTG_Handle_t *husb);
extern ER usbo_activateEndpoint(USB_OTG_Handle_t *husb, USB_OTG_EPTypeDef *ep);
extern ER usbo_deactivateEndpoint(USB_OTG_Handle_t *husb, USB_OTG_EPTypeDef *ep);
extern ER usbo_epstartxfer(USB_OTG_Handle_t *husb, USB_OTG_EPTypeDef *ep);
extern ER usbo_ep0startxfer(USB_OTG_Handle_t *husb, USB_OTG_EPTypeDef *ep);
extern ER usbo_epsetStall(USB_OTG_Handle_t *husb, USB_OTG_EPTypeDef *ep);
extern ER usbo_epclearStall(USB_OTG_Handle_t *husb, USB_OTG_EPTypeDef *ep);
extern ER usbo_setDevAddress(USB_OTG_Handle_t *husb, uint8_t address);
extern ER usbo_devconnect(USB_OTG_Handle_t *husb);
extern ER usbo_devdisconnect(USB_OTG_Handle_t *husb);
extern ER usbo_stopdevice(USB_OTG_Handle_t *husb);
extern ER usbo_ep0_outstart(USB_OTG_Handle_t *husb, uint8_t *psetup);
extern uint8_t usbo_getDevSpeed(USB_OTG_Handle_t *husb);
extern ER usbo_init_lpm(USB_OTG_Handle_t *husb);

extern ER usbo_hostinit(USB_OTG_Handle_t *husb);
extern ER usbo_starthost(USB_OTG_Handle_t *husb);
extern ER usbo_resetport(USB_OTG_Handle_t *husb);
extern ER usbo_drivevbus(USB_OTG_Handle_t *husb, uint8_t state);
extern void usbo_driveextvbus(USB_OTG_Handle_t *husb, uint8_t state);
extern uint32_t usbo_gethostspeed(USB_OTG_Handle_t *husb);
extern uint32_t usbo_getcurrentframe(USB_OTG_Handle_t *husb);
extern ER usbo_hc_init(USB_OTG_Handle_t *husb, uint8_t ch_num, uint8_t epnum);
extern ER usbo_hc_startxfer(USB_OTG_Handle_t *husb, uint8_t ch_num);
extern ER usbo_hc_halt(USB_OTG_Handle_t *husb, uint8_t ch_num);
extern ER usbo_stophost(USB_OTG_Handle_t *husb);

extern void usbo_hcd_irqhandler(USB_OTG_Handle_t *husb);
extern void usbo_pcd_irqhandler(USB_OTG_Handle_t *husb);

extern void usb_otg_isr(intptr_t exinf);
extern void usb_otg_wkup_isr(intptr_t exinf);

extern void	usbh_task(intptr_t exinf);

#endif /* TOPPERS_MACRO_ONLY */

#endif	/* _USB_OTG_H_ */
