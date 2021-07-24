/*
 *  TOPPERS/ASP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Advanced Standard Profile Kernel
 * 
 *  Copyright (C) 2008-2011 by Embedded and Real-Time Systems Laboratory
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
 *  Copyright (C) 2015-2017 by TOPPERS PROJECT Educational Working Group.
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
 *  @(#) $Id: dsi.h 698 2017-03-04 17:30:50Z roi $
 */
/*
 * 
 *  STM32F7xx DSI�ǥХ����ɥ饤�Фγ������
 *
 */

#ifndef _DSI_H_
#define _DSI_H_

#include "ltdc.h"

#ifdef __cplusplus
 extern "C" {
#endif


/*
 *  DSI ACTIVE/INACTIVE����
 */
#define DSI_INACTIVE              0
#define DSI_ACTIVE                1

/*
 *  DSI_VideoConfig_t����
 */
/*  ColorCoding���� */
#define DSI_RGB565               0x00000000	/* RGB565 color mode configuration */
#define DSI_RGB666               0x00000003	/* RGB666 color mode configuration */
#define DSI_RGB888               0x00000005

/*  Mode���� */
#define DSI_VID_MODE_NB_PULSES   0x00000000
#define DSI_VID_MODE_NB_EVENTS   0x00000001
#define DSI_VID_MODE_BURST       0x00000002

/*  LPCommandEnable���� */
#define DSI_LP_COMMAND_DISABLE   0x00000000
#define DSI_LP_COMMAND_ENABLE    DSI_VMCR_LPCE

/*  LPHorizontalFrontPorchEnable���� */
#define DSI_LP_HFP_DISABLE       0x00000000
#define DSI_LP_HFP_ENABLE        DSI_VMCR_LPHFPE

/*  LPHorizontalBackPorchEnable���� */
#define DSI_LP_HBP_DISABLE       0x00000000
#define DSI_LP_HBP_ENABLE        DSI_VMCR_LPHBPE

/*  LPVerticalActiveEnable���� */
#define DSI_LP_VACT_DISABLE      0x00000000
#define DSI_LP_VACT_ENABLE       DSI_VMCR_LPVAE

/*  LPVerticalFrontPorchEnable���� */
#define DSI_LP_VFP_DISABLE       0x00000000
#define DSI_LP_VFP_ENABLE        DSI_VMCR_LPVFPE

/*  LPVerticalBackPorchEnable���� */
#define DSI_LP_VBP_DISABLE       0x00000000
#define DSI_LP_VBP_ENABLE        DSI_VMCR_LPVBPE

/*  LPVerticalSyncActiveEnable���� */
#define DSI_LP_VSYNC_DISABLE     0x00000000
#define DSI_LP_VSYNC_ENABLE      DSI_VMCR_LPVSAE

/*  FrameBTAAcknowledgeEnable���� */
#define DSI_FBTAA_DISABLE        0x00000000
#define DSI_FBTAA_ENABLE         DSI_VMCR_FBTAAE

/*  LooselyPacked���� */
#define DSI_LOOSELY_PACKED_DISABLE 0x00000000
#define DSI_LOOSELY_PACKED_ENABLE  DSI_LCOLCR_LPE

/*  HSPolarity���� */
#define DSI_HSYNC_ACTIVE_HIGH    0x00000000
#define DSI_HSYNC_ACTIVE_LOW     DSI_LPCR_HSP

/*  VSPolarity���� */
#define DSI_VSYNC_ACTIVE_HIGH    0x00000000
#define DSI_VSYNC_ACTIVE_LOW     DSI_LPCR_VSP

/*  DEPolarity���� */
#define DSI_DATA_ENABLE_ACTIVE_HIGH 0x00000000
#define DSI_DATA_ENABLE_ACTIVE_LOW  DSI_LPCR_DEP


/*
 *  DSI_CommandConfig_t����
 */
/*  TearingEffectSource���� */
#define DSI_TE_DSILINK           0x00000000
#define DSI_TE_EXTERNAL          DSI_WCFGR_TESRC

/*  TearingEffectPolarity���� */
#define DSI_TE_RISING_EDGE       0x00000000
#define DSI_TE_FALLING_EDGE      DSI_WCFGR_TEPOL

/*  VSyncPol���� */
#define DSI_VSYNC_FALLING        0x00000000
#define DSI_VSYNC_RISING         DSI_WCFGR_VSPOL

/*  AutomaticRefresh���� */
#define DSI_AR_DISABLE           0x00000000
#define DSI_AR_ENABLE            DSI_WCFGR_AR

/*
 *  LP COMMAND MODE(dsi_configcommand)����ӥå����
 */
/*  DSI LP LPGen Short Write NoP */
#define DSI_LP_GSW0P_DISABLE     0x00000000
#define DSI_LP_GSW0P_ENABLE      DSI_CMCR_GSW0TX

/*  DSI LP LPGen Short Write OneP */
#define DSI_LP_GSW1P_DISABLE     0x00000000
#define DSI_LP_GSW1P_ENABLE      DSI_CMCR_GSW1TX

/*  DSI LP LPGen Short Write TwoP */
#define DSI_LP_GSW2P_DISABLE     0x00000000
#define DSI_LP_GSW2P_ENABLE      DSI_CMCR_GSW2TX

/*  DSI LP LPGen Short Read NoP */
#define DSI_LP_GSR0P_DISABLE     0x00000000
#define DSI_LP_GSR0P_ENABLE      DSI_CMCR_GSR0TX

/*  DSI LP LPGen Short Read OneP */
#define DSI_LP_GSR1P_DISABLE     0x00000000
#define DSI_LP_GSR1P_ENABLE      DSI_CMCR_GSR1TX

/*  DSI LP LPGen Short Read TwoP */
#define DSI_LP_GSR2P_DISABLE     0x00000000
#define DSI_LP_GSR2P_ENABLE      DSI_CMCR_GSR2TX

/*  DSI LP LPGen LongWrite */
#define DSI_LP_GLW_DISABLE       0x00000000
#define DSI_LP_GLW_ENABLE        DSI_CMCR_GLWTX

/*  DSI LP LPDcs Short Write NoP */
#define DSI_LP_DSW0P_DISABLE     0x00000000
#define DSI_LP_DSW0P_ENABLE      DSI_CMCR_DSW0TX

/*  DSI LP LPDcs Short Write OneP */
#define DSI_LP_DSW1P_DISABLE     0x00000000
#define DSI_LP_DSW1P_ENABLE      DSI_CMCR_DSW1TX

/*  DSI LP LPDcs Short Read NoP */
#define DSI_LP_DSR0P_DISABLE     0x00000000
#define DSI_LP_DSR0P_ENABLE      DSI_CMCR_DSR0TX

/*  DSI LP LPDcs Long Write */
#define DSI_LP_DLW_DISABLE       0x00000000
#define DSI_LP_DLW_ENABLE        DSI_CMCR_DLWTX

/*  DSI LP LPMax Read Packet */
#define DSI_LP_MRDP_DISABLE      0x00000000
#define DSI_LP_MRDP_ENABLE       DSI_CMCR_MRDPS

/*
 *  DSI_HostTimeout_t����
 */
/*  HighSpeedWritePrespMode���� */
#define DSI_HS_PM_DISABLE        0x00000000
#define DSI_HS_PM_ENABLE         DSI_TCCR3_PM

/*
 *  DSI_Init_t����
 */
/*  AutomaticClockLaneControl���� */
#define DSI_AUTO_CLK_LANE_CTRL_DISABLE 0x00000000
#define DSI_AUTO_CLK_LANE_CTRL_ENABLE  DSI_CLCR_ACR

/*  NumberOfLanes��� */
#define DSI_ONE_DATA_LANE        0
#define DSI_TWO_DATA_LANES       1

/*  pllidf���� */
#define DSI_PLL_IN_DIV1          0x00000001
#define DSI_PLL_IN_DIV2          0x00000002
#define DSI_PLL_IN_DIV3          0x00000003
#define DSI_PLL_IN_DIV4          0x00000004
#define DSI_PLL_IN_DIV5          0x00000005
#define DSI_PLL_IN_DIV6          0x00000006
#define DSI_PLL_IN_DIV7          0x00000007

/*  pllodf���� */
#define DSI_PLL_OUT_DIV1         0x00000000
#define DSI_PLL_OUT_DIV2         0x00000001
#define DSI_PLL_OUT_DIV4         0x00000002
#define DSI_PLL_OUT_DIV8         0x00000003


/*
 *  DSI���硼��WRITE���ޥ�ɥ⡼�����(Mode)
 */
#define DSI_DCS_SHORT_PKT_WRITE_P0 0x00000005	/* DCS short write, no parameters */
#define DSI_DCS_SHORT_PKT_WRITE_P1 0x00000015	/* DCS short write, one parameter */
#define DSI_GEN_SHORT_PKT_WRITE_P0 0x00000003	/* Generic short write, no parameters */
#define DSI_GEN_SHORT_PKT_WRITE_P1 0x00000013	/* Generic short write, one parameter */
#define DSI_GEN_SHORT_PKT_WRITE_P2 0x00000023	/* Generic short write, two parameters */

/*
 *  DSI���WRITE���ޥ�ɥ⡼�����(Mode)
 */
#define DSI_DCS_LONG_PKT_WRITE    0x00000039	/* DCS long write     */
#define DSI_GEN_LONG_PKT_WRITE    0x00000029	/* Generic long write */

/*
 *  DSI���ޥ��READ���ޥ�ɥ⡼�����(Mode)
 */
#define DSI_DCS_SHORT_PKT_READ    0x00000006	/* DCS short read */
#define DSI_GEN_SHORT_PKT_READ_P0 0x00000004	/* Generic short read, no parameters  */
#define DSI_GEN_SHORT_PKT_READ_P1 0x00000014	/* Generic short read, one parameter  */
#define DSI_GEN_SHORT_PKT_READ_P2 0x00000024	/* Generic short read, two parameters */

/*
 *  DSI DCS���ޥ�ɥ⡼�����
 */
#define DSI_ENTER_IDLE_MODE       0x39
#define DSI_ENTER_INVERT_MODE     0x21
#define DSI_ENTER_NORMAL_MODE     0x13
#define DSI_ENTER_PARTIAL_MODE    0x12
#define DSI_ENTER_SLEEP_MODE      0x10
#define DSI_EXIT_IDLE_MODE        0x38
#define DSI_EXIT_INVERT_MODE      0x20
#define DSI_EXIT_SLEEP_MODE       0x11
#define DSI_GET_3D_CONTROL        0x3F
#define DSI_GET_ADDRESS_MODE      0x0B
#define DSI_GET_BLUE_CHANNEL      0x08
#define DSI_GET_DIAGNOSTIC_RESULT 0x0F
#define DSI_GET_DISPLAY_MODE      0x0D
#define DSI_GET_GREEN_CHANNEL     0x07
#define DSI_GET_PIXEL_FORMAT      0x0C
#define DSI_GET_POWER_MODE        0x0A
#define DSI_GET_RED_CHANNEL       0x06
#define DSI_GET_SCANLINE          0x45
#define DSI_GET_SIGNAL_MODE       0x0E
#define DSI_NOP                   0x00
#define DSI_READ_DDB_CONTINUE     0xA8
#define DSI_READ_DDB_START        0xA1
#define DSI_READ_MEMORY_CONTINUE  0x3E
#define DSI_READ_MEMORY_START     0x2E
#define DSI_SET_3D_CONTROL        0x3D
#define DSI_SET_ADDRESS_MODE      0x36
#define DSI_SET_COLUMN_ADDRESS    0x2A
#define DSI_SET_DISPLAY_OFF       0x28
#define DSI_SET_DISPLAY_ON        0x29
#define DSI_SET_GAMMA_CURVE       0x26
#define DSI_SET_PAGE_ADDRESS      0x2B
#define DSI_SET_PARTIAL_COLUMNS   0x31
#define DSI_SET_PARTIAL_ROWS      0x30
#define DSI_SET_PIXEL_FORMAT      0x3A
#define DSI_SET_SCROLL_AREA       0x33
#define DSI_SET_SCROLL_START      0x37
#define DSI_SET_TEAR_OFF          0x34
#define DSI_SET_TEAR_ON           0x35
#define DSI_SET_TEAR_SCANLINE     0x44
#define DSI_SET_VSYNC_TIMING      0x40
#define DSI_SOFT_RESET            0x01
#define DSI_WRITE_LUT             0x2D
#define DSI_WRITE_MEMORY_CONTINUE 0x3C
#define DSI_WRITE_MEMORY_START    0x2C


/*
 *  DSI��������졼�����(customlane)
 */
#define DSI_SWAP_LANE_PINS       0x00000000
#define DSI_INVERT_HS_SIGNAL     0x00000001

/*
 *  DSI�졼�󥿥��ߥ����(lane)
 */
#define DSI_CLOCK_LANE           0x00000000
#define DSI_DATA_LANES           0x00000001
#define DSI_DATA_LANE0           0x00000001
#define DSI_DATA_LANE1           0x00000002

/*
 *  DSI PHY�����ߥ����(timing)
 */
#define DSI_TCLK_POST            0x00000000
#define DSI_TLPX_CLK             0x00000001
#define DSI_THS_EXIT             0x00000002
#define DSI_TLPX_DATA            0x00000003
#define DSI_THS_ZERO             0x00000004
#define DSI_THS_TRAIL            0x00000005
#define DSI_THS_PREPARE          0x00000006
#define DSI_TCLK_ZERO            0x00000007
#define DSI_TCLK_PREPARE         0x00000008

/*
 *  DSI���顼���
 */
#define DSI_ERROR_NONE           0x00000000		/* ���顼�ʤ� */
#define DSI_ERROR_ACK            0x00000001		/* ACK���顼 */
#define DSI_ERROR_PHY            0x00000002		/* PHY���顼 */
#define DSI_ERROR_TX             0x00000004		/* �������顼 */
#define DSI_ERROR_RX             0x00000008		/* �������顼 */
#define DSI_ERROR_ECC            0x00000010		/* ECC���顼 */
#define DSI_ERROR_CRC            0x00000020		/* CRC���顼 */
#define DSI_ERROR_PSE            0x00000040		/* �ѥ��åȥ��������顼 */
#define DSI_ERROR_EOT            0x00000080		/* EOT���顼 */
#define DSI_ERROR_OVF            0x00000100		/* FIFO�����С��ե����顼 */
#define DSI_ERROR_GEN            0x00000200		/* �����ͥ�å�FIFO���顼 */

/*
 *  DSI�������
 */
#define DSI_STATE_RESET          0x00000000		/* DSI �ꥻ�åȾ��� */
#define DSI_STATE_READY          0x00000001		/* DSI ��ǥ����� */
#define DSI_STATE_BUSY           0x00000002		/* DSI �ӥ������� */
#define DSI_STATE_ERROR          0x00010000		/* DSI ���顼���� */


/*
 *  DSI������������
 */
typedef struct
{
	uint32_t           AutomaticClockLaneControl;	/* Automatic clock lane control */
	uint32_t           TXEscapeCkdiv;				/* TX Escape clock division */
	uint32_t           NumberOfLanes;				/* Number of lanes */
    uint32_t           pllndiv;						/* PLL Loop Division Factor */
    uint32_t           pllidf;						/* PLL Input Division Factor */
	uint32_t           pllodf;						/* PLL Output Division Factor */
} DSI_Init_t;

/*
 *  DSI�ӥǥ�����ե�����졼���������
 */
typedef struct 
{
	uint32_t           VirtualChannelID;			/* Virtual channel ID */
	uint32_t           ColorCoding;					/* Color coding for LTDC interface */
	uint32_t           LooselyPacked;				/* Enable or disable loosely packed stream */
	uint32_t           Mode;						/* Video mode type */
	uint32_t           PacketSize;					/* Video packet size */
	uint32_t           NumberOfChunks;				/* Number of chunks */
	uint32_t           NullPacketSize;				/* Null packet size */
	uint32_t           HSPolarity;					/* HSYNC pin polarity */
	uint32_t           VSPolarity;					/* VSYNC pin polarity */
	uint32_t           DEPolarity;					/* Data Enable pin polarity */
	uint32_t           HorizontalSyncActive;		/* Horizontal synchronism active duration */
	uint32_t           HorizontalBackPorch;			/* Horizontal back-porch duration */
	uint32_t           HorizontalLine;				/* Horizontal line duration */
	uint32_t           VerticalSyncActive;			/* Vertical synchronism active duration */
	uint32_t           VerticalBackPorch;			/* Vertical back-porch duration */
	uint32_t           VerticalFrontPorch;			/* Vertical front-porch duration */
	uint32_t           VerticalActive;				/* Vertical active duration */
	uint32_t           LPCommandEnable;				/* Low-power command enable */
	uint32_t           LPLargestPacketSize;			/* the low power largest packet size that can fit in a line during VSA, VBP and VFP regions */
	uint32_t           LPVACTLargestPacketSize;		/* the low power largest packet size that can fit in a line during VACT region */
	uint32_t           LPHorizontalFrontPorchEnable;	/* Low-power horizontal front-porch enable */
	uint32_t           LPHorizontalBackPorchEnable;	/* Low-power horizontal back-porch enable */
	uint32_t           LPVerticalActiveEnable;		/* Low-power vertical active enable */
	uint32_t           LPVerticalFrontPorchEnable;	/* Low-power vertical front-porch enable */
	uint32_t           LPVerticalBackPorchEnable;	/* Low-power vertical back-porch enable */
	uint32_t           LPVerticalSyncActiveEnable;	/* Low-power vertical sync active enable */
	uint32_t           FrameBTAAcknowledgeEnable;	/* Frame bus-turn-around acknowledge enable */
} DSI_VideoConfig_t;

/*
 *  DSI ���ޥ�ɥ���ե�����졼��������
 */
typedef struct
{
	uint32_t           VirtualChannelID;			/* Virtual channel ID */
	uint32_t           ColorCoding;					/* Color coding for LTDC interface */
	uint32_t           CommandSize;					/* Maximum allowed size for an LTDC write memory command, measured in pixels */
	uint32_t           TearingEffectSource;			/* Tearing effect source */
	uint32_t           TearingEffectPolarity;		/* Tearing effect pin polarity */
	uint32_t           HSPolarity;					/* HSYNC pin polarity */
	uint32_t           VSPolarity;					/* VSYNC pin polarity */
	uint32_t           DEPolarity;					/* Data Enable pin polarity */
	uint32_t           VSyncPol;					/* VSync edge on which the LTDC is halted */
	uint32_t           AutomaticRefresh;			/* Automatic refresh mode */
} DSI_CommandConfig_t;

/*
 *  DSI PHY �����ߥ����
 */
typedef struct
{
	uint32_t           ClockLaneHS2LPTime;			/* The maximum time that the D-PHY clock lane takes to go from high-speed to low-power transmission */
	uint32_t           ClockLaneLP2HSTime;			/* The maximum time that the D-PHY clock lane takes to go from low-power to high-speed transmission */
	uint32_t           DataLaneHS2LPTime;			/* The maximum time that the D-PHY data lanes takes to go from high-speed to low-power transmission */
	uint32_t           DataLaneLP2HSTime;			/* The maximum time that the D-PHY data lanes takes to go from low-power to high-speed transmission */
	uint32_t           DataLaneMaxReadTime;			/* The maximum time required to perform a read command */
	uint32_t           StopWaitTime;				/* The minimum wait period to request a High-Speed transmission after the Stop state */
} DSI_PHY_Time_t;

/*
 *  DSI HOST�����ॢ�������
 */
typedef struct
{
	uint32_t           TimeoutCkdiv;				/* Time-out clock division */
	uint32_t           HighSpeedTransmissionTimeout;	/* High-speed transmission time-out */
	uint32_t           LowPowerReceptionTimeout;	/* Low-power reception time-out */
	uint32_t           HighSpeedReadTimeout;		/* High-speed read time-out */
	uint32_t           LowPowerReadTimeout;			/* Low-power read time-out */
	uint32_t           HighSpeedWriteTimeout;		/* High-speed write time-out */
	uint32_t           HighSpeedWritePrespMode;		/* High-speed write presp mode */
	uint32_t           LowPowerWriteTimeout;		/* Low-speed write time-out */
	uint32_t           BTATimeout;					/* BTA time-out */
  
} DSI_HostTimeout_t;

/**
  * @brief  DSI Handle Structure definition
  */
typedef struct DSI_Handle_s DSI_Handle_t;
struct DSI_Handle_s
{
	uint32_t           base;			/* DSI �١����쥸���� */
    DSI_Init_t         Init;			/* DSI required parameters    */
	void               (*teffectcallback)(DSI_Handle_t * hdsi);	/* TearEffct������Хå��ؿ� */
	void               (*refreshcallback)(DSI_Handle_t * hdsi);	/* ��ե�å��彪λ������Хå��ؿ� */
	void               (*errorcallback)(DSI_Handle_t * hdsi);	/* ���顼������Хå��ؿ� */
	volatile uint32_t  state;		  	/* DSI �¹Ծ��� */
	volatile uint32_t  errorcode;		/* DSI Error code */
};

extern ER dsi_init(DSI_Handle_t *hdsi);
extern ER dsi_deinit(DSI_Handle_t *hdsi);
extern ER dsi_configvideo(DSI_Handle_t *hdsi, DSI_VideoConfig_t *pini);
extern ER dci_configltdc(LTDC_Handle_t* hltdc, DSI_VideoConfig_t *pini);
extern ER dsi_configadaptedCmdMode(DSI_Handle_t *hdsi, DSI_CommandConfig_t *pcfg, uint32_t ackactive);
extern ER dsi_configcommand(DSI_Handle_t *hdsi, uint32_t lpcmode, uint32_t ackactive);
extern ER dsi_configPhyTimer(DSI_Handle_t *hdsi, DSI_PHY_Time_t *ptime);
extern ER dsi_configHostTimeout(DSI_Handle_t *hdsi, DSI_HostTimeout_t *ptimeout);
extern ER dsi_start(DSI_Handle_t *hdsi);
extern ER dsi_stop(DSI_Handle_t *hdsi);
extern ER dsi_swrite(DSI_Handle_t *hdsi, uint32_t Channel, uint32_t Mode, uint32_t Param1, uint32_t Param2);
extern ER dsi_lwrite(DSI_Handle_t *hdsi, uint32_t Channel, uint32_t Mode, uint32_t NbParams, uint32_t Param1, uint8_t* buf);
extern ER dsi_read(DSI_Handle_t *hdsi, uint32_t Channel, uint8_t* Array, uint32_t Size, uint32_t Mode, uint32_t DCSCmd, uint8_t* buf);
extern ER dsi_enterULPMData(DSI_Handle_t *hdsi);
extern ER dsi_exitULPMData(DSI_Handle_t *hdsi);
extern ER dsi_enterULPM(DSI_Handle_t *hdsi);
extern ER dsi_exitULPM(DSI_Handle_t *hdsi);
extern void dsi_startPatternGenerator(DSI_Handle_t *hdsi, uint32_t Mode, uint32_t Orientation);
extern void dsi_stopPatternGenerator(DSI_Handle_t *hdsi);
extern ER dsi_setLanePinsConfiguration(DSI_Handle_t *hdsi, uint32_t CustomLane, uint32_t Lane, uint32_t active);
extern ER dsi_setPHYTiming(DSI_Handle_t *hdsi, uint32_t Timing, uint32_t active, uint32_t Value);
extern ER dsi_forceTXStopMode(DSI_Handle_t *hdsi, uint32_t Lane, uint32_t active);

extern void dsi_irqhandler(DSI_Handle_t *hdsi);

/*
 *  SLEW RATE High-Speed Transmission Slew Rate Control on Clock Lane����
 */
#define dsi_setSRateHSTXClock(h, v) sil_wrw_mem((uint32_t *)((h)->base+TOFF_DSI_WPCR1), \
									((sil_rew_mem((uint32_t *)((h)->base+TOFF_DSI_WPCR1)) & ~DSI_WPCR1_HSTXSRCCL) |((v)<<16)))

/*
 *  SLEW RATE High-Speed Transmission Slew Rate Control on Data Lanes����
 */
#define dsi_setSRateHSTXLane(h, v)  sil_wrw_mem((uint32_t *)((h)->base+TOFF_DSI_WPCR1), \
									((sil_rew_mem((uint32_t *)((h)->base+TOFF_DSI_WPCR1)) & ~DSI_WPCR1_HSTXSRCDL) |((v)<<18)))

/* 
 *  SLEW RATE Low-Power transmission Slew Rate Compensation on Clock Lane����
 */
#define dsi_setSRateLPTXClock(h, v) sil_wrw_mem((uint32_t *)((h)->base+TOFF_DSI_WPCR1), \
									((sil_rew_mem((uint32_t *)((h)->base+TOFF_DSI_WPCR1)) & ~DSI_WPCR1_LPSRCCL) |((v)<<6)))

/*
 *  SLEW RATE Low-Power transmission Slew Rate Compensation on Data Lanes����
 */
#define dsi_setSRateLPTXLane(h, v)  sil_wrw_mem((uint32_t *)((h)->base+TOFF_DSI_WPCR1), \
									((sil_rew_mem((uint32_t *)((h)->base+TOFF_DSI_WPCR1)) & ~DSI_WPCR1_LPSRCDL) |((v)<<8)))

/*
 *  DEALY�����ߥ� High-Speed Transmission Delay on Clock Lane����
 */
#define dsi_setHSDealyClock(h, v)   sil_wrw_mem((uint32_t *)((h)->base+TOFF_DSI_WPCR1), \
									((sil_rew_mem((uint32_t *)((h)->base+TOFF_DSI_WPCR1)) & ~DSI_WPCR1_HSTXDCL) |(v)))

/*
 *  DEAY�����ߥ� High-Speed Transmission Delay on Data Lanes����
 */
#define dsi_setHSDealyLane(h, v)    sil_wrw_mem((uint32_t *)((h)->base+TOFF_DSI_WPCR1), \
									((sil_rew_mem((uint32_t *)((h)->base+TOFF_DSI_WPCR1)) & ~DSI_WPCR1_HSTXDDL) |((v)<<2)))


/*
 *  Configure the Generic interface read-back Virtual Channel ID.
 */
#define dsi_setGenericVCID(h, v)	sil_wrw_mem((uint32_t *)((h)->base+TOFF_DSI_GVCIDR), \
									((sil_rew_mem((uint32_t *)((h)->base+TOFF_DSI_GVCIDR)) & ~DSI_GVCIDR_VCID) |(v)))

/*
 *  Configure the flow control parameters
 */
#define dsi_confFlowControl(h, v)	sil_wrw_mem((uint32_t *)((h)->base+TOFF_DSI_PCR), \
									((sil_rew_mem((uint32_t *)((h)->base+TOFF_DSI_PCR)) & ~DSI_FLOW_CONTROL_ALL) |(v)))

/*
 *  Refresh the display in command mode
 */
#define dsi_refresh(h)				sil_wrw_mem((uint32_t *)((h)->base+TOFF_DSI_WCR), \
									(sil_rew_mem((uint32_t *)((h)->base+TOFF_DSI_WCR)) | DSI_WCR_LTDCEN))

/*
 *  Controls the display color mode in Video mode
 */
#define dsi_colorMode(h, v)			sil_wrw_mem((uint32_t *)((h)->base+TOFF_DSI_WCR), \
									((sil_rew_mem((uint32_t *)((h)->base+TOFF_DSI_WCR)) & ~DSI_WCR_COLM) |(v)))

/*
 *  Control the display shutdown in Video mode
 */
#define dsi_shutdown(h, v)			sil_wrw_mem((uint32_t *)((h)->base+TOFF_DSI_WCR), \
									((sil_rew_mem((uint32_t *)((h)->base+TOFF_DSI_WCR)) & ~DSI_WCR_SHTDN) |((v)<<1)))

/*
 *  Low-Power Reception Filter Tuning
 */
#define dsi_setLowPowRXFilter(h, v)	sil_wrw_mem((uint32_t *)((h)->base+TOFF_DSI_WPCR1), \
									((sil_rew_mem((uint32_t *)((h)->base+TOFF_DSI_WPCR1)) & ~DSI_WPCR1_LPRXFT) |((v)<<25)))

/*
 *  Activate an additional current path on all lanes to meet the SDDTx parameter
 */
#define dsi_setSDD(h, v)			sil_wrw_mem((uint32_t *)((h)->base+TOFF_DSI_WPCR1), \
									((sil_rew_mem((uint32_t *)((h)->base+TOFF_DSI_WPCR1)) & ~DSI_WPCR1_SDDC) |((v)<<12)))

/*
 *  Forces LP Receiver in Low-Power Mode
 */
#define dsi_forceRXLowPower(h, v)	sil_wrw_mem((uint32_t *)((h)->base+TOFF_DSI_WPCR1), \
									((sil_rew_mem((uint32_t *)((h)->base+TOFF_DSI_WPCR1)) & ~DSI_WPCR1_FLPRXLPM) |((v)<<22)))

/*
 *  Force Data Lanes in RX Mode after a BTA
 */
#define dsi_forceDataLanesRX(h, v)	sil_wrw_mem((uint32_t *)((h)->base+TOFF_DSI_WPCR0), \
									((sil_rew_mem((uint32_t *)((h)->base+TOFF_DSI_WPCR0)) & ~DSI_WPCR0_TDDL) |((v)<<16)))

/*
 *  Enable a pull-down on the lanes to prevent from floating states when unused
 */
#define dsi_setPullDown(h, v)		sil_wrw_mem((uint32_t *)((h)->base+TOFF_DSI_WPCR0), \
									((sil_rew_mem((uint32_t *)((h)->base+TOFF_DSI_WPCR0)) & ~DSI_WPCR0_PDEN) |((v)<<18)))

/*
 *  Switch off the contention detection on data lanes
 */
#define dsi_setContDetectOff(h, v)	sil_wrw_mem((uint32_t *)((h)->base+TOFF_DSI_WPCR0), \
									((sil_rew_mem((uint32_t *)((h)->base+TOFF_DSI_WPCR0)) & ~DSI_WPCR0_CDOFFDL) |((v)<<14)))

/*
 *  ���顼���������
 */
#define DSI_INT_ACKERR_REQ (DSI_ISR0_AE0 | DSI_ISR0_AE1 | DSI_ISR0_AE2 | DSI_ISR0_AE3 | \
                            DSI_ISR0_AE4 | DSI_ISR0_AE5 | DSI_ISR0_AE6 | DSI_ISR0_AE7 | \
                            DSI_ISR0_AE8 | DSI_ISR0_AE9 | DSI_ISR0_AE10 | DSI_ISR0_AE11 | \
                            DSI_ISR0_AE12 | DSI_ISR0_AE13 | DSI_ISR0_AE14 | DSI_ISR0_AE15)
#define DSI_INT_PHYERR_REQ (DSI_ISR0_PE0 | DSI_ISR0_PE1 | DSI_ISR0_PE2 | DSI_ISR0_PE3 | DSI_ISR0_PE4)
#define DSI_INT_GENERR_REQ (DSI_ISR1_GCWRE | DSI_ISR1_GPWRE | DSI_ISR1_GPTXE | DSI_ISR1_GPRDE | DSI_ISR1_GPRXE)

/*
 *  ACK���顼������׵�
 */
#define dsi_int_ackerr(h)			sil_wrw_mem((uint32_t *)((h)->base+TOFF_DSI_IER0), \
									(sil_rew_mem((uint32_t *)((h)->base+TOFF_DSI_IER0)) | DSI_INT_ACKERR_REQ))

/*
 *  PHY���顼������׵�
 */
#define dsi_int_phyerr(h)			sil_wrw_mem((uint32_t *)((h)->base+TOFF_DSI_IER0), \
									(sil_rew_mem((uint32_t *)((h)->base+TOFF_DSI_IER0)) | DSI_INT_PHYERR_REQ))

/*
 *  TX���顼������׵�
 */
#define dsi_int_txerr(h)			sil_wrw_mem((uint32_t *)((h)->base+TOFF_DSI_IER1), \
									(sil_rew_mem((uint32_t *)((h)->base+TOFF_DSI_IER1)) | DSI_ISR1_TOHSTX))

/*
 *  RX���顼������׵�
 */
#define dsi_int_rxerr(h)			sil_wrw_mem((uint32_t *)((h)->base+TOFF_DSI_IER1), \
									(sil_rew_mem((uint32_t *)((h)->base+TOFF_DSI_IER1)) | DSI_ISR1_TOLPRX))
/*
 *  ECC���顼������׵�
 */
#define dsi_int_eccerr(h)			sil_wrw_mem((uint32_t *)((h)->base+TOFF_DSI_IER1), \
									(sil_rew_mem((uint32_t *)((h)->base+TOFF_DSI_IER1)) | (DSI_ISR1_ECCSE | DSI_ISR1_ECCME)))

/*
 *  CRC���顼������׵�
 */
#define dsi_int_crcerr(h)			sil_wrw_mem((uint32_t *)((h)->base+TOFF_DSI_IER1), \
									(sil_rew_mem((uint32_t *)((h)->base+TOFF_DSI_IER1)) | DSI_ISR1_CRCE))

/*
 *  PSE���顼������׵�
 */
#define dsi_int_pseerr(h)			sil_wrw_mem((uint32_t *)((h)->base+TOFF_DSI_IER1), \
									(sil_rew_mem((uint32_t *)((h)->base+TOFF_DSI_IER1)) | DSI_ISR1_PSE))

/*
 *  EOT���顼������׵�
 */
#define dsi_int_eoterr(h)			sil_wrw_mem((uint32_t *)((h)->base+TOFF_DSI_IER1), \
									(sil_rew_mem((uint32_t *)((h)->base+TOFF_DSI_IER1)) | DSI_ISR1_EOTPE))

/*
 *  VOF���顼������׵�
 */
#define dsi_int_voferr(h)			sil_wrw_mem((uint32_t *)((h)->base+TOFF_DSI_IER1), \
									(sil_rew_mem((uint32_t *)((h)->base+TOFF_DSI_IER1)) | DSI_ISR1_LPWRE))

/*
 *  GEN���顼������׵�
 */
#define dsi_int_generr(h)			sil_wrw_mem((uint32_t *)((h)->base+TOFF_DSI_IER1), \
									(sil_rew_mem((uint32_t *)((h)->base+TOFF_DSI_IER1)) | DSI_INT_GENERR_REQ))

#ifdef __cplusplus
}
#endif

#endif	/* _DSI_H_ */

