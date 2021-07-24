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
 *  @(#) $Id: dsc.c 698 2017-02-24 18:26:43Z roi $
 */
/*
 * STM32F746-DSI�ѥǥХ����ɥ饤��
 */

#include <kernel.h>
#include <t_syslog.h>
#include <t_stdlib.h>
#include "syssvc/serial.h"
#include "syssvc/syslog.h"
#include <target_syssvc.h>
#include "ltdc.h"
#include "dsi.h"

#define sil_orw_mem(a, b)		sil_wrw_mem((a), sil_rew_mem(a) | (b))
#define sil_andw_mem(a, b)		sil_wrw_mem((a), sil_rew_mem(a) & ~(b))
#define sil_modw_mem(a, b, c)	sil_wrw_mem((a), (sil_rew_mem(a) & (~b)) | (c))


#define DSI_TIMEOUT_VALUE  1000			/* 1�� */

#define DSI_MAX_RTPKT_SIZE 0x00000037	/* ����꥿����ѥ��åȥ����� */

#define DSI_CMCR_MASK     (DSI_CMCR_GSW0TX | DSI_CMCR_GSW1TX | DSI_CMCR_GSW2TX |\
                           DSI_CMCR_GSR0TX | DSI_CMCR_GSR1TX | DSI_CMCR_GSR2TX |\
                           DSI_CMCR_GLWTX  | DSI_CMCR_DSW0TX | DSI_CMCR_DSW1TX |\
                           DSI_CMCR_DSR0TX | DSI_CMCR_DLWTX  | DSI_CMCR_MRDPS)
/*
 *  DSI�ν�������Ԥ�
 *  parameter1 hdsi     DSI�ϥ�ɥ�ؤΥݥ���
 *  return              ����ʤ�E_OK
 */
ER
dsi_init(DSI_Handle_t *hdsi)
{
	uint32_t tick = 0;
	uint32_t unitIntervalx4 = 0;
	uint32_t temp;

	if(hdsi == NULL)
		return E_PAR;

	hdsi->state = DSI_STATE_BUSY;

	/*
	 *  DSI PLL����
	 */
	sil_orw_mem((uint32_t *)(hdsi->base+TOFF_DSI_WRPCR), DSI_WRPCR_REGEN);

    /*
	 *  ��ǥ��Ԥ�
	 */
	while((sil_rew_mem((uint32_t *)(hdsi->base+TOFF_DSI_WISR)) & DSI_WISR_RRS) == 0){
		if(tick > DSI_TIMEOUT_VALUE)
			return E_TMOUT;
		tick++;
		dly_tsk(1);
	}

    /*
	 *  PLL��ʬ������
	 */
	temp  = sil_rew_mem((uint32_t *)(hdsi->base+TOFF_DSI_WRPCR));
	temp &= ~(DSI_WRPCR_PLL_NDIV | DSI_WRPCR_PLL_IDF | DSI_WRPCR_PLL_ODF);
	temp |= ((hdsi->Init.pllndiv)<<2) | ((hdsi->Init.pllidf)<<11) | ((hdsi->Init.pllodf)<<16);
	sil_wrw_mem((uint32_t *)(hdsi->base+TOFF_DSI_WRPCR), temp);

	/*
	 *  DSI PLL���͡��֥롢��ǥ��Ԥ�
	 */
    tick = 0;
	sil_orw_mem((uint32_t *)(hdsi->base+TOFF_DSI_WRPCR), DSI_WRPCR_PLLEN);
	while((sil_rew_mem((uint32_t *)(hdsi->base+TOFF_DSI_WISR)) & DSI_WISR_PLLLS) == 0){
		if(tick > DSI_TIMEOUT_VALUE)
			return E_TMOUT;
		tick++;
		dly_tsk(1);
	}

    /*
	 *  D-PHY����å����ǥ����륤�͡��֥�
	 */
	sil_orw_mem((uint32_t *)(hdsi->base+TOFF_DSI_PCTLR), (DSI_PCTLR_CKE | DSI_PCTLR_DEN));

    /*
	 *  ����å��졼��Υ���ե�����졼�����
	 */
	temp  = sil_rew_mem((uint32_t *)(hdsi->base+TOFF_DSI_CLCR));
	temp &= ~(DSI_CLCR_DPCC | DSI_CLCR_ACR);
	temp |= (DSI_CLCR_DPCC | hdsi->Init.AutomaticClockLaneControl);
	sil_wrw_mem((uint32_t *)(hdsi->base+TOFF_DSI_CLCR), temp);

    /*
	 *  �ǡ����졼��Υ���ե�����졼�����
	 */
	sil_modw_mem((uint32_t *)(hdsi->base+TOFF_DSI_PCONFR), DSI_PCONFR_NL, hdsi->Init.NumberOfLanes);

	/*
	 *  DSI����å��ѥ�᡼������
	 */
	sil_modw_mem((uint32_t *)(hdsi->base+TOFF_DSI_CCR), DSI_CCR_TXECKDIV, hdsi->Init.TXEscapeCkdiv);

	/*
	 *  �ϥ����ԡ��ɥ⡼������
	 */
	temp = (hdsi->Init.pllidf > 0) ? hdsi->Init.pllidf : 1;
	unitIntervalx4 = (4000000 * temp * (1 << hdsi->Init.pllodf)) / ((HSE_VALUE/1000) * hdsi->Init.pllndiv);
	sil_modw_mem((uint32_t *)(hdsi->base+TOFF_DSI_WPCR0), DSI_WPCR0_UIX4, unitIntervalx4);

    /*
	 *  ���顼�����̵����
	 */
	sil_wrw_mem((uint32_t *)(hdsi->base+TOFF_DSI_IER0), 0);
	sil_wrw_mem((uint32_t *)(hdsi->base+TOFF_DSI_IER1), 0);

    /*
	 *  ���顼����ꥻ�åȡ���ǥ�����
	 */
    hdsi->errorcode = DSI_ERROR_NONE;
	hdsi->state = DSI_STATE_READY;
	return E_OK;
}

/*
 *  DSI����ͭ���������Ԥ�
 *  parameter1 hdsi     DSI�ϥ�ɥ�ؤΥݥ���
 *  return              ����ʤ�E_OK
 */
ER
dsi_deinit(DSI_Handle_t *hdsi)
{
	if(hdsi == NULL)
		return E_PAR;

	hdsi->state = DSI_STATE_BUSY;

	/*
	 *  DSI WRAPPER̵����
	 */
	sil_andw_mem((uint32_t *)(hdsi->base+TOFF_DSI_WCR), DSI_WCR_DSIEN);

	/*
	 *  DSI�⥸�塼��̵����
	 */
	sil_andw_mem((uint32_t *)(hdsi->base+TOFF_DSI_CR), DSI_CR_EN);

	/*
	 *  D-PHY����å����ǥ�����̵����
	 */
	sil_andw_mem((uint32_t *)(hdsi->base+TOFF_DSI_PCTLR), (DSI_PCTLR_CKE | DSI_PCTLR_DEN));

	/*
	 *  DSI PLL����
	 */
	sil_andw_mem((uint32_t *)(hdsi->base+TOFF_DSI_WRPCR), DSI_WRPCR_PLLEN);

	/*
	 *  �쥮��졼��̵����
	 */
	sil_andw_mem((uint32_t *)(hdsi->base+TOFF_DSI_WRPCR), DSI_WRPCR_REGEN);

    /*
	 *  ���顼����ꥻ�åȡ��ꥻ�å�����
	 */
	hdsi->errorcode = DSI_ERROR_NONE;
	hdsi->state = DSI_STATE_RESET;
	return E_OK;
}


/*
 *  DSI�ӥǥ�����ե�����졼���������
 *  parameter1 hdsi     DSI�ϥ�ɥ�ؤΥݥ���
 *  parameter2 pini     DSI�ӥǥ�����ե�����졼���������ؤΥݥ���
 *  return              ����ʤ�E_OK
 */
ER
dsi_configvideo(DSI_Handle_t *hdsi, DSI_VideoConfig_t *pini)
{
	uint32_t temp;

	if(hdsi == NULL || pini == NULL)
		return E_PAR;
	if(pini->ColorCoding == DSI_RGB666 && (pini->LooselyPacked & ~DSI_LCOLCR_LPE) != 0)
		return E_PAR;

	/*
	 *  �ӥǥ��⡼������
	 */
	sil_andw_mem((uint32_t *)(hdsi->base+TOFF_DSI_MCR), DSI_MCR_CMDM);
	sil_andw_mem((uint32_t *)(hdsi->base+TOFF_DSI_WCFGR), DSI_WCFGR_DSIM);

	/*
	 *  �ӥǥ��⡼������
	 */
	sil_modw_mem((uint32_t *)(hdsi->base+TOFF_DSI_VMCR), DSI_VMCR_VMT, pini->Mode);

	/*
	 *  �ӥǥ��ѥ��åȥ���������
	 */
	sil_modw_mem((uint32_t *)(hdsi->base+TOFF_DSI_VPCR), DSI_VPCR_VPSIZE, pini->PacketSize);

	/*
	 *  ������ֹ�����
	 */
	sil_modw_mem((uint32_t *)(hdsi->base+TOFF_DSI_VCCR), DSI_VCCR_NUMC, pini->NumberOfChunks);

	/*
	 *  �ʥ�ѥ��åȥ���������
	 */
	sil_modw_mem((uint32_t *)(hdsi->base+TOFF_DSI_VNPCR), DSI_VNPCR_NPSIZE, pini->NullPacketSize);

	/*
	 *  �С���������ͥ�����
	 */
	sil_modw_mem((uint32_t *)(hdsi->base+TOFF_DSI_LVCIDR), DSI_LVCIDR_VCID, pini->VirtualChannelID);

	/*
	 *  ����ȥ��륷���ʥ�ζ�������
	 */
	temp  = sil_rew_mem((uint32_t *)(hdsi->base+TOFF_DSI_LPCR));
	temp &= ~(DSI_LPCR_DEP | DSI_LPCR_VSP | DSI_LPCR_HSP);
	temp |= (pini->DEPolarity | pini->VSPolarity | pini->HSPolarity);
	sil_wrw_mem((uint32_t *)(hdsi->base+TOFF_DSI_LPCR), temp);

	/*
	 *  ���顼�����ǥ�������
	 */
	sil_modw_mem((uint32_t *)(hdsi->base+TOFF_DSI_LCOLCR), DSI_LCOLCR_COLC, pini->ColorCoding);
	sil_modw_mem((uint32_t *)(hdsi->base+TOFF_DSI_WCFGR), DSI_WCFGR_COLMUX, ((pini->ColorCoding)<<1));

	/*
	 *  RGB666�� loosely packed variants����
	 */
	if(pini->ColorCoding == DSI_RGB666){
		sil_modw_mem((uint32_t *)(hdsi->base+TOFF_DSI_LCOLCR), DSI_LCOLCR_LPE, pini->LooselyPacked);
	}

	/*
	 *  Horizontal Synchronization Active (HSA)����å�����
	 */
	sil_modw_mem((uint32_t *)(hdsi->base+TOFF_DSI_VHSACR), DSI_VHSACR_HSA, pini->HorizontalSyncActive);

	/*
	 *  Horizontal Back Porch (HBP)����å�����
	 */
	sil_modw_mem((uint32_t *)(hdsi->base+TOFF_DSI_VHBPCR), DSI_VHBPCR_HBP, pini->HorizontalBackPorch);

	/*
	 * total line time (HLINE=HSA+HBP+HACT+HFP)����å�����
	 */
	sil_modw_mem((uint32_t *)(hdsi->base+TOFF_DSI_VLCR), DSI_VLCR_HLINE, pini->HorizontalLine);

	/*
	 *  Vertical Synchronization Active (VSA)����å�����
	 */
	sil_modw_mem((uint32_t *)(hdsi->base+TOFF_DSI_VVSACR), DSI_VVSACR_VSA, pini->VerticalSyncActive);

	/*
	 *  Vertical Back Porch (VBP)����å�����
	 */
	sil_modw_mem((uint32_t *)(hdsi->base+TOFF_DSI_VVBPCR), DSI_VVBPCR_VBP, pini->VerticalBackPorch);

	/*
	 *  Vertical Front Porch (VFP)����å�����
	 */
	sil_modw_mem((uint32_t *)(hdsi->base+TOFF_DSI_VVFPCR), DSI_VVFPCR_VFP, pini->VerticalFrontPorch);

	/*
	 *  Vertical Active period����
	 */
	sil_modw_mem((uint32_t *)(hdsi->base+TOFF_DSI_VVACR), DSI_VVACR_VA, pini->VerticalActive);

	/*
	 *  Command transmission mode�⡼������
	 */
	sil_modw_mem((uint32_t *)(hdsi->base+TOFF_DSI_VMCR), DSI_VMCR_LPCE, pini->LPCommandEnable);

	/*
	 *  Low power ����ѥ��åȥ���������
	 */
	sil_modw_mem((uint32_t *)(hdsi->base+TOFF_DSI_LPMCR), DSI_LPMCR_LPSIZE, ((pini->LPLargestPacketSize)<<16));

	/*
	 *  Low power VACT ����ѥ��åȥ���������
	 */
	sil_modw_mem((uint32_t *)(hdsi->base+TOFF_DSI_LPMCR), DSI_LPMCR_VLPSIZE, pini->LPVACTLargestPacketSize);

	/*
	 *  LP transition in HFP period����
	 */
	sil_modw_mem((uint32_t *)(hdsi->base+TOFF_DSI_VMCR), DSI_VMCR_LPHFPE, pini->LPHorizontalFrontPorchEnable);

	/*
	 *  LP transition in HBP period����
	 */
	sil_modw_mem((uint32_t *)(hdsi->base+TOFF_DSI_VMCR), DSI_VMCR_LPHBPE, pini->LPHorizontalBackPorchEnable);

	/*
	 *  LP transition in VACT period����
	 */
	sil_modw_mem((uint32_t *)(hdsi->base+TOFF_DSI_VMCR), DSI_VMCR_LPVAE, pini->LPVerticalActiveEnable);

	/*
	 *  LP transition in VFP period����
	 */
	sil_modw_mem((uint32_t *)(hdsi->base+TOFF_DSI_VMCR), DSI_VMCR_LPVFPE, pini->LPVerticalFrontPorchEnable);

	/*
	 *  LP transition in VBP period����
	 */
	sil_modw_mem((uint32_t *)(hdsi->base+TOFF_DSI_VMCR), DSI_VMCR_LPVBPE, pini->LPVerticalBackPorchEnable);

	/*
	 *  LP transition in vertical sync period����
	 */
	sil_modw_mem((uint32_t *)(hdsi->base+TOFF_DSI_VMCR), DSI_VMCR_LPVSAE, pini->LPVerticalSyncActiveEnable);

	/*
	 *  request for an acknowledge response����
	 */
	sil_modw_mem((uint32_t *)(hdsi->base+TOFF_DSI_VMCR), DSI_VMCR_FBTAAE, pini->FrameBTAAcknowledgeEnable);
	return E_OK;
}

/*
 *  LTDC�ν�����ѥ�᡼����DSI VIDEO�⡼�ɤ�������
 *  parameter1 hltdc     LTDC�ν�����깽¤�ΤؤΥݥ���
 *  parameter2 pini      DSI VIDEO�⡼�����깽¤�ΤؤΥݥ���
 *  return               ����ʤ��E_OK
 */
ER
dci_configltdc(LTDC_Handle_t* hltdc, DSI_VideoConfig_t *pini)
{
	if(hltdc == NULL || pini == NULL)
		return E_PAR;

	/*
	 *  LTDC�Ѷ�������
	 */
	hltdc->Init.DEPolarity = (pini->DEPolarity == DSI_DATA_ENABLE_ACTIVE_HIGH) ? LTDC_DEPOLARITY_AL : LTDC_DEPOLARITY_AH;
	hltdc->Init.VSPolarity = (pini->VSPolarity == DSI_VSYNC_ACTIVE_HIGH) ? LTDC_VSPOLARITY_AH : LTDC_VSPOLARITY_AL;
	hltdc->Init.HSPolarity = (pini->HSPolarity == DSI_HSYNC_ACTIVE_HIGH) ? LTDC_HSPOLARITY_AH : LTDC_HSPOLARITY_AL;

	/*
	 *  LTDC�ѥС�����륿���ߥ󥰥ѥ�᡼������
	 */
	hltdc->Init.VerticalSync       = pini->VerticalSyncActive - 1;
	hltdc->Init.AccumulatedVBP     = pini->VerticalSyncActive + pini->VerticalBackPorch - 1;
	hltdc->Init.AccumulatedActiveH = pini->VerticalSyncActive + pini->VerticalBackPorch + pini->VerticalActive - 1;
	hltdc->Init.TotalHeigh         = pini->VerticalSyncActive + pini->VerticalBackPorch + pini->VerticalActive + pini->VerticalFrontPorch - 1;
	return E_OK;
}

/*
 *  Ŭ�祳�ޥ�ɥ⡼������
 *  parameter1 hdsi      DSI�ϥ�ɥ�ؤΥݥ���
 *  parameter2 pcfg      ���ޥ����������ؤΥݥ���
 *  parameter3 actactive ž��ACKͭ��̵������(DSI_ACTIVE/DSI_INACTIVE)
 *  return               ����ʤ��E_OK
 */
ER
dsi_configadaptedCmdMode(DSI_Handle_t *hdsi, DSI_CommandConfig_t *pcfg, uint32_t ackactive)
{
	uint32_t temp;

	if(hdsi == NULL || pcfg == NULL)
		return E_PAR;
	/*
	 *  ���ޥ�ɥ⡼������
	 */
	sil_orw_mem((uint32_t *)(hdsi->base+TOFF_DSI_MCR), DSI_MCR_CMDM);
	sil_andw_mem((uint32_t *)(hdsi->base+TOFF_DSI_WCFGR), DSI_WCFGR_DSIM);
	sil_orw_mem((uint32_t *)(hdsi->base+TOFF_DSI_WCFGR), DSI_WCFGR_DSIM);

	/*
	 *  �С���������ͥ�����
	 */
	sil_modw_mem((uint32_t *)(hdsi->base+TOFF_DSI_LVCIDR), DSI_LVCIDR_VCID, pcfg->VirtualChannelID);

	/*
	 *  ���������楷���ʥ�����
	 */
	temp  = sil_rew_mem((uint32_t *)(hdsi->base+TOFF_DSI_LPCR));
	temp &= ~(DSI_LPCR_DEP | DSI_LPCR_VSP | DSI_LPCR_HSP);
	temp |= (pcfg->DEPolarity | pcfg->VSPolarity | pcfg->HSPolarity);
	sil_wrw_mem((uint32_t *)(hdsi->base+TOFF_DSI_LPCR), temp);

	/*
	 *  ���顼�����ǥ�������
	 */
	sil_modw_mem((uint32_t *)(hdsi->base+TOFF_DSI_LCOLCR), DSI_LCOLCR_COLC, pcfg->ColorCoding);
	sil_modw_mem((uint32_t *)(hdsi->base+TOFF_DSI_WCFGR), DSI_WCFGR_COLMUX, ((pcfg->ColorCoding)<<1));

	/*
	 *  ���祳�ޥ�ɥ���������
	 */
	sil_modw_mem((uint32_t *)(hdsi->base+TOFF_DSI_LCCR), DSI_LCCR_CMDSIZE, pcfg->CommandSize);

	/*
	 *  �ƥ�����󥰥��ե����ȥ���������������ե�å���⡼������
	 */
	temp  = sil_rew_mem((uint32_t *)(hdsi->base+TOFF_DSI_WCFGR));
	temp &= ~(DSI_WCFGR_TESRC | DSI_WCFGR_TEPOL | DSI_WCFGR_AR | DSI_WCFGR_VSPOL);
	temp |= (pcfg->TearingEffectSource | pcfg->TearingEffectPolarity | pcfg->AutomaticRefresh | pcfg->VSyncPol);
	sil_wrw_mem((uint32_t *)(hdsi->base+TOFF_DSI_WCFGR), temp);

	/*
	 *  ž��ACK����
	 */
	sil_modw_mem((uint32_t *)(hdsi->base+TOFF_DSI_CMCR), DSI_CMCR_TEARE, (ackactive & 1));

 	/*
	 * ���������
	 */
	sil_orw_mem((uint32_t *)(hdsi->base+TOFF_DSI_WIER), (DSI_WIER_TEIE | DSI_WIER_ERIE));
	return E_OK;
}

/*
 *  ���ޥ��ž���⡼������
 *  parameter1 hdsi     DSI�ϥ�ɥ�ؤΥݥ���
 *  parameter2 lpcmode  ���ޥ�ɥ⡼������ӥå���
 *  parameter3 ackactive ACK�Υ����ƥ�������(DSI_ACTIVE/DSI_INACTIVE)
 *  return               ����ʤ��E_OK
 */
ER
dsi_configcommand(DSI_Handle_t *hdsi, uint32_t lpcmode, uint32_t ackactive)
{
	if(hdsi == NULL)
		return E_PAR;
	/*
	 *  ���ޥ��ž���⡼������
	 */
	sil_andw_mem((uint32_t *)(hdsi->base+TOFF_DSI_CMCR), DSI_CMCR_MASK);
	sil_orw_mem((uint32_t *)(hdsi->base+TOFF_DSI_CMCR), (lpcmode & DSI_CMCR_MASK));

	/*
	 *  �ѥ��å�ACK����
	 */
	sil_modw_mem((uint32_t *)(hdsi->base+TOFF_DSI_CMCR), DSI_CMCR_ARE, (ackactive<<1));
	return E_OK;
}

/*
 *  DSI PHY�����ߥ�����
 *  parameter1 hdsi    DSI�ϥ�ɥ�ؤΥݥ���
 *  parameter2 ptime   
  * @brief  Configure the DSI PHY timer parameters
  * @param  hdsi: pointer to a DSI_Handle_t structure that contains
  *               the configuration information for the DSI.
  * @param  ptime: DSI_PHY_Time_t structure that contains
  *                    the DSI PHY timing parameters
  * @retval HAL status
  */
ER
dsi_configPhyTimer(DSI_Handle_t *hdsi, DSI_PHY_Time_t *ptime)
{
	uint32_t maxTime, temp;

	maxTime = (ptime->ClockLaneLP2HSTime > ptime->ClockLaneHS2LPTime)? ptime->ClockLaneLP2HSTime: ptime->ClockLaneHS2LPTime;

	/*
	 *  ����å��졼�󥿥��ޡ�����
	 */
	temp  = sil_rew_mem((uint32_t *)(hdsi->base+TOFF_DSI_CLTCR));
	temp &= ~(DSI_CLTCR_LP2HS_TIME | DSI_CLTCR_HS2LP_TIME);
	temp |= (maxTime | ((maxTime)<<16));
	sil_wrw_mem((uint32_t *)(hdsi->base+TOFF_DSI_CLTCR), temp);

	/*
	 *  �ǡ����졼�󥿥��ޡ�����
	 */
	temp  = sil_rew_mem((uint32_t *)(hdsi->base+TOFF_DSI_DLTCR));
	temp &= ~(DSI_DLTCR_MRD_TIME | DSI_DLTCR_LP2HS_TIME | DSI_DLTCR_HS2LP_TIME);
	temp |= (ptime->DataLaneMaxReadTime | ((ptime->DataLaneLP2HSTime)<<16) | ((ptime->DataLaneHS2LPTime)<<24));
	sil_wrw_mem((uint32_t *)(hdsi->base+TOFF_DSI_DLTCR), temp);

	/*
	 *  ��߾��֤θ�� HSž���Ԥ���������
	 */
	sil_modw_mem((uint32_t *)(hdsi->base+TOFF_DSI_PCONFR), DSI_PCONFR_SW_TIME, ptime->StopWaitTime<<8);
	return E_OK;
}

/*
 *  DSI HOST�����ॢ��������
 *  parameter1 hdsi    DSI�ϥ�ɥ�ؤΥݥ���
 *  parameter2 ptimeout DSI_HostTimeout_t structure that contains
 *  return             ����ʤ�E_OK
 */
ER
dsi_configHostTimeout(DSI_Handle_t *hdsi, DSI_HostTimeout_t *ptimeout)
{
	/*
	 * timeout clock division factor����
	 */
	sil_modw_mem((uint32_t *)(hdsi->base+TOFF_DSI_CCR), DSI_CCR_TOCKDIV, (ptimeout->TimeoutCkdiv<<8));

	/*
	 *  High-speed transmission timeout����
	 */
	sil_modw_mem((uint32_t *)(hdsi->base+TOFF_DSI_TCCR0), DSI_TCCR0_HSTX_TOCNT, ptimeout->HighSpeedTransmissionTimeout<<16);

	/*
	 *  Low-power reception timeout����
	 */
	sil_modw_mem((uint32_t *)(hdsi->base+TOFF_DSI_TCCR0), DSI_TCCR0_LPRX_TOCNT, ptimeout->LowPowerReceptionTimeout);

	/*
	 *  High-speed read timeout����
	 */
	sil_modw_mem((uint32_t *)(hdsi->base+TOFF_DSI_TCCR1), DSI_TCCR1_HSRD_TOCNT, ptimeout->HighSpeedReadTimeout);

	/*
	 *  Low-power read timeout����
	 */
	sil_modw_mem((uint32_t *)(hdsi->base+TOFF_DSI_TCCR2), DSI_TCCR2_LPRD_TOCNT, ptimeout->LowPowerReadTimeout);

	/*
	 *  High-speed write timeout����
	 */
	sil_modw_mem((uint32_t *)(hdsi->base+TOFF_DSI_TCCR3), DSI_TCCR3_HSWR_TOCNT, ptimeout->HighSpeedWriteTimeout);

	/*
	 *  High-speed write presp mode����
	 */
	sil_modw_mem((uint32_t *)(hdsi->base+TOFF_DSI_TCCR3), DSI_TCCR3_PM, ptimeout->HighSpeedWritePrespMode);

	/*
	 *  Low-speed write timeout����
	 */
	sil_modw_mem((uint32_t *)(hdsi->base+TOFF_DSI_TCCR4), DSI_TCCR4_LPWR_TOCNT, ptimeout->LowPowerWriteTimeout);

	/*
	 *  BTA timeout����
	 */
	sil_modw_mem((uint32_t *)(hdsi->base+TOFF_DSI_TCCR5), DSI_TCCR5_BTA_TOCNT, ptimeout->BTATimeout);
	return E_OK;
}

/*
 *  DSI�⥸�塼��¹Գ���
 *  parameter1 hdsi    DSI�ϥ�ɥ�ؤΥݥ���
 *  return             ����ʤ�E_OK
 */
ER
dsi_start(DSI_Handle_t *hdsi)
{
	/*
	 *  DSI�⥸�塼��ͭ����
	 */
	sil_orw_mem((uint32_t *)(hdsi->base+TOFF_DSI_CR), DSI_CR_EN);

	/*
	 *  DSI WRAPPERͭ����
	 */
	sil_orw_mem((uint32_t *)(hdsi->base+TOFF_DSI_WCR), DSI_WCR_DSIEN);
	return E_OK;
}

/*
 *  DSI�⥸�塼�����
 *  parameter1 hdsi    DSI�ϥ�ɥ�ؤΥݥ���
 *  return             ����ʤ�E_OK
 */
ER
dsi_stop(DSI_Handle_t *hdsi)
{
	/*
	 *  DSI�⥸�塼��̵����
	 */
	sil_andw_mem((uint32_t *)(hdsi->base+TOFF_DSI_CR), DSI_CR_EN);

	/*
	 *  DSI WRAPPER̵����
	 */
	sil_andw_mem((uint32_t *)(hdsi->base+TOFF_DSI_WCR), DSI_WCR_DSIEN);
	return E_OK;
}

/*
 *  ���硼��WRITE���ޥ��
 *  parameter1 hdsi    DSI�ϥ�ɥ�ؤΥݥ���
 *  parameter2 Channel Virtual channel ID.
 *  parameter3 Mode    DSI short packet data type.
 *  parameter4 Param1  DSC command or first generic parameter.
 *  parameter5 Param2  DSC parameter or second generic parameter.
 *  return             ����ʤ�E_OK
 */
ER
dsi_swrite(DSI_Handle_t *hdsi, uint32_t Channel, uint32_t Mode, uint32_t Param1, uint32_t Param2)
{
	uint32_t tick = 0;

	if(hdsi == NULL)
		return E_PAR;
	/*
	 *  �����FIFO����ץƥ��Ԥ�
	 */
	while((sil_rew_mem((uint32_t *)(hdsi->base+TOFF_DSI_GPSR)) & DSI_GPSR_CMDFE) == 0){
		if(tick > DSI_TIMEOUT_VALUE)
			return E_TMOUT;
		tick++;
		dly_tsk(1);
	}

	/*
	 *  ����ߥ��ޥ������
	 */
	sil_wrw_mem((uint32_t *)(hdsi->base+TOFF_DSI_GHCR), (Mode | (Channel<<6) | (Param1<<8) | (Param2<<16)));
	return E_OK;
}

/*
 *  ���WRITE���ޥ��
 *  parameter1 hdsi    DSI�ϥ�ɥ�ؤΥݥ���
 *  parameter2 Channel Virtual channel ID.
 *  parameter3 Mode    DSI long packet data type.
 *  parameter4 NbParams: Number of parameters.
 *  parameter5 Param1: DSC command or first generic parameter.
 *  parameter6 buf: Pointer to parameter values table.
 *  return             ����ʤ�E_OK
 */
ER
dsi_lwrite(DSI_Handle_t *hdsi, uint32_t Channel, uint32_t Mode, uint32_t NbParams, uint32_t Param1, uint8_t* buf)
{
	uint32_t no = 0;
	uint32_t tick = 0;

	if(hdsi == NULL)
		return E_PAR;
	/*
	 *  �����FIFO����ץƥ��Ԥ�
	 */
	while((sil_rew_mem((uint32_t *)(hdsi->base+TOFF_DSI_GPSR)) & DSI_GPSR_CMDFE) == 0){
		if(tick > DSI_TIMEOUT_VALUE)
			return E_TMOUT;
		tick++;
		dly_tsk(1);
	}

	/* Set the DCS code hexadecimal on payload byte 1, and the other parameters on the write FIFO command*/
	while(no < NbParams){
		if(no == 0x00){
			sil_wrw_mem((uint32_t *)(hdsi->base+TOFF_DSI_GPDR), (Param1 | \
                            ((uint32_t)(*(buf + no)) << 8) | \
                            ((uint32_t)(*(buf + no+1))<<16) | \
                            ((uint32_t)(*(buf + no+2))<<24)));
			no += 3;
		}
		else{
			sil_wrw_mem((uint32_t *)(hdsi->base+TOFF_DSI_GPDR), ((uint32_t)(*(buf + no)) | \
                            ((uint32_t)(*(buf + no+1)) << 8) | \
                            ((uint32_t)(*(buf + no+2)) << 16) | \
                            ((uint32_t)(*(buf + no+3)) << 24)));
			no += 4;
		}
	}

	/*
	 *  ����ߥѥ��å�����
	 */
	sil_wrw_mem((uint32_t *)(hdsi->base+TOFF_DSI_GHCR), (Mode | (Channel<<6) | (((NbParams+1)&0x00FF)<<8) | (((NbParams+1)&0xFF00)<<8)));
	return E_OK;
}

/*
 *  DSI���ޥ�ɥ꡼��
 *  parameter1 hdsi    DSI�ϥ�ɥ�ؤΥݥ���
 *  parameter2 Channel Virtual channel ID
 *  parameter3 Array   pointer to a buffer to store the payload of a read back operation.
 *  parameter4 Size    Data size to be read (in byte).
 *  parameter5 Mode    DSI read packet data type.
 *  parameter6 DCSCmd  DCS get/read command.
 *  parameter7 buf     �����ΰ�ؤΥݥ���
 *  return             ����ʤ�E_OK
 */
ER
dsi_read(DSI_Handle_t *hdsi, uint32_t Channel, uint8_t* Array, uint32_t Size, uint32_t Mode, uint32_t DCSCmd, uint8_t* buf)
{
	uint32_t tick = 0;

	if(Size > 2){	/* �꥿����ѥ��åȥ��������� */
		dsi_swrite(hdsi, Channel, DSI_MAX_RTPKT_SIZE, ((Size)&0xFF), (((Size)>>8)&0xFF));
	}

	/*
	 *  READ���ޥ������
	 */
	if(Mode == DSI_DCS_SHORT_PKT_READ){
		sil_wrw_mem((uint32_t *)(hdsi->base+TOFF_DSI_GHCR), (Mode | (Channel<<6) | (DCSCmd<<8) | (0<<16)));
	}
	else if(Mode == DSI_GEN_SHORT_PKT_READ_P0){
		sil_wrw_mem((uint32_t *)(hdsi->base+TOFF_DSI_GHCR), (Mode | (Channel<<6) | (0<<8) | (0<<16)));
	}
	else if(Mode == DSI_GEN_SHORT_PKT_READ_P1){
		sil_wrw_mem((uint32_t *)(hdsi->base+TOFF_DSI_GHCR), (Mode | (Channel<<6) | (buf[0]<<8) | (0<<16)));
	}
	else if(Mode == DSI_GEN_SHORT_PKT_READ_P2){
		sil_wrw_mem((uint32_t *)(hdsi->base+TOFF_DSI_GHCR), (Mode | (Channel<<6) | (buf[0]<<8) | (buf[1]<<16)));
	}
	else{
		return E_PAR;
	}

	/*
	 *  �ɤ߽Ф�FIFO�����Ԥ�
	 */
	while((sil_rew_mem((uint32_t *)(hdsi->base+TOFF_DSI_GPSR)) & DSI_GPSR_PRDFE) != 0){
		/* Check for the Timeout */
		if(tick > DSI_TIMEOUT_VALUE)
			return E_TMOUT;
		tick++;
		dly_tsk(1);
	}

	/*
	 *  ���Х��ȼ�Ф�
	 */
	*((uint32_t *)Array) = sil_rew_mem((uint32_t *)(hdsi->base+TOFF_DSI_GPDR));
	if(Size > 4){
		Size -= 4;
		Array += 4;
	}
	else{
		return E_OK;
	}

	/*
	 *  �Ĥ�Х��ȼ�Ф�
	 */
	tick = 0;
	while(((int)(Size)) > 0){
		if((sil_rew_mem((uint32_t *)(hdsi->base+TOFF_DSI_GPSR)) & DSI_GPSR_PRDFE) == 0){
			*((uint32_t *)Array) = sil_rew_mem((uint32_t *)(hdsi->base+TOFF_DSI_GPDR));
			Size -= 4;
			Array += 4;
		}
		if(tick > DSI_TIMEOUT_VALUE)
			return E_TMOUT;
		tick++;
		dly_tsk(1);
	}
	return E_OK;
}

/*
 *  ULPM(Ultra Low Power Mode����(D-PHY PLL running)
 *  parameter1 hdsi    DSI�ϥ�ɥ�ؤΥݥ���
 *  return             ����ʤ�E_OK
 */
ER
dsi_enterULPMData(DSI_Handle_t *hdsi)
{
	uint32_t tick = 0;
	uint32_t pconfrnl;

	/*
	 *  DSI Host PHY ULPS�Υǡ����졼�󥪥�
	 */
	sil_orw_mem((uint32_t *)(hdsi->base+TOFF_DSI_PUCR), DSI_PUCR_URDL);

 	/*
	 *  D-PHY�����ƥ��֥졼�������Ԥ�
	 */
	pconfrnl = sil_rew_mem((uint32_t *)(hdsi->base+TOFF_DSI_PCONFR)) & DSI_PCONFR_NL;
	if(pconfrnl == DSI_ONE_DATA_LANE){
		while((sil_rew_mem((uint32_t *)(hdsi->base+TOFF_DSI_PSR)) & DSI_PSR_UAN0) != 0){
			if(tick > DSI_TIMEOUT_VALUE)
				return E_TMOUT;
			tick++;
			dly_tsk(1);
		}
	}
	else if(pconfrnl == DSI_TWO_DATA_LANES){
		while((sil_rew_mem((uint32_t *)(hdsi->base+TOFF_DSI_PSR)) & (DSI_PSR_UAN0 | DSI_PSR_UAN1)) != 0){
			if(tick > DSI_TIMEOUT_VALUE)
				return E_TMOUT;
			tick++;
			dly_tsk(1);
		}
	}
	return E_OK;
}

/*
 *  ULPM(Ultra Low Power Mode��λ(D-PHY PLL running)
 *  parameter1 hdsi    DSI�ϥ�ɥ�ؤΥݥ���
 *  return             ����ʤ�E_OK
 */
ER
dsi_exitULPMData(DSI_Handle_t *hdsi)
{
	uint32_t tick = 0;
	uint32_t pconfrnl;

	/*
	 *  DSI Host PHY ULPS�Υǡ����졼��λ
	 */
	sil_orw_mem((uint32_t *)(hdsi->base+TOFF_DSI_PUCR), DSI_PUCR_UEDL);

	/*
	 *  D-PHY�졼�󥢥��ƥ����Ԥ�
	 */
	pconfrnl = sil_rew_mem((uint32_t *)(hdsi->base+TOFF_DSI_PCONFR)) & DSI_PCONFR_NL;
	if(pconfrnl == DSI_ONE_DATA_LANE){
		while((sil_rew_mem((uint32_t *)(hdsi->base+TOFF_DSI_PSR)) & DSI_PSR_UAN0) == 0){
			if(tick > DSI_TIMEOUT_VALUE)
				return E_TMOUT;
			tick++;
			dly_tsk(1);
		}
	}
	else if(pconfrnl == DSI_TWO_DATA_LANES){
		while((sil_rew_mem((uint32_t *)(hdsi->base+TOFF_DSI_PSR)) & (DSI_PSR_UAN0 | DSI_PSR_UAN1)) != (DSI_PSR_UAN0 | DSI_PSR_UAN1)){
			if(tick > DSI_TIMEOUT_VALUE)
				return E_TMOUT;
			tick++;
			dly_tsk(1);
		}
	}

	/*
	 *  DSI Host PHY ULPS�Υ���å��ȥǡ����졼��ӥåȤ򤹤٤ƥ���
	 */
	sil_wrw_mem((uint32_t *)(hdsi->base+TOFF_DSI_PUCR), 0);
	return E_OK;
}

/*
 *  ULPM(Ultra Low Power Mode����(D-PHY PLL turned off)
 *  parameter1 hdsi    DSI�ϥ�ɥ�ؤΥݥ���
 *  return             ����ʤ�E_OK
 */
ER
dsi_enterULPM(DSI_Handle_t *hdsi)
{
	uint32_t tick = 0;
	uint32_t pconfrnl;

	/*
	 *  D-PHY���楪��
	 */
	sil_andw_mem((uint32_t *)(hdsi->base+TOFF_DSI_CLCR), DSI_CLCR_DPCC);

	/*
	 *  D-PHY����Υ���å�����������
	 */
	sil_orw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_DCKCFGR2), RCC_DCKCFGR2_DSISEL);

	/*
	 *  DSI Host PHY ULPS�Υ���å��ȥǡ����졼�󥪥�
	 */
	sil_orw_mem((uint32_t *)(hdsi->base+TOFF_DSI_PUCR), (DSI_PUCR_URCL | DSI_PUCR_URDL));

	/*
	 * �����ƥ��֥졼��λ�Ԥ�
	 */
	pconfrnl = sil_rew_mem((uint32_t *)(hdsi->base+TOFF_DSI_PCONFR)) & DSI_PCONFR_NL;
	if(pconfrnl == DSI_ONE_DATA_LANE){
		while((sil_rew_mem((uint32_t *)(hdsi->base+TOFF_DSI_PSR)) & (DSI_PSR_UAN0 | DSI_PSR_UANC)) != 0){
			if(tick > DSI_TIMEOUT_VALUE)
				return E_TMOUT;
			tick++;
			dly_tsk(1);
		}
	}
	else if(pconfrnl == DSI_TWO_DATA_LANES){
		while((sil_rew_mem((uint32_t *)(hdsi->base+TOFF_DSI_PSR)) & (DSI_PSR_UAN0 | DSI_PSR_UAN1 | DSI_PSR_UANC)) != 0){
			if(tick > DSI_TIMEOUT_VALUE)
				return E_TMOUT;
			tick++;
			dly_tsk(1);
		}
	}

	/*
	 *  DSI PLL����
	 */
	sil_andw_mem((uint32_t *)(hdsi->base+TOFF_DSI_WRPCR), DSI_WRPCR_PLLEN);
	return E_OK;
}

/*
 *  ULPM(Ultra Low Power Mode)��λ(D-PHY PLL turned off)
 *  parameter1 hdsi    DSI�ϥ�ɥ�ؤΥݥ���
 *  return             ����ʤ�E_OK
 */
ER
dsi_exitULPM(DSI_Handle_t *hdsi)
{
	uint32_t tick = 0;
	uint32_t pconfrnl;

	/*
	 *  DSI PLL����
	 */
	sil_orw_mem((uint32_t *)(hdsi->base+TOFF_DSI_WRPCR), DSI_WRPCR_PLLEN);

	/*
	 *  DSI PLL��å��Ԥ�
	 */
	while((sil_rew_mem((uint32_t *)(hdsi->base+TOFF_DSI_WISR)) & DSI_WISR_PLLLS) == 0){
		if(tick > DSI_TIMEOUT_VALUE)
			return E_TMOUT;
		tick++;
		dly_tsk(1);
	}

	/*
	 *  DSI Host PHY ULPS�Υ���å��ȥǡ����졼��λ
	 */
	sil_orw_mem((uint32_t *)(hdsi->base+TOFF_DSI_PUCR), (DSI_PUCR_UECL | DSI_PUCR_UEDL));

	/*
	 * �졼�󥢥��ƥ��֥졼���Ԥ�
	 */
	tick = 0;
	pconfrnl = sil_rew_mem((uint32_t *)(hdsi->base+TOFF_DSI_PCONFR)) & DSI_PCONFR_NL;
	if(pconfrnl == DSI_ONE_DATA_LANE){
		while((sil_rew_mem((uint32_t *)(hdsi->base+TOFF_DSI_PSR)) & (DSI_PSR_UAN0 | DSI_PSR_UANC)) != (DSI_PSR_UAN0 | DSI_PSR_UANC)){
			/* Check for the Timeout */
			if(tick > DSI_TIMEOUT_VALUE)
				return E_TMOUT;
			tick++;
			dly_tsk(1);
		}
	}
	else if(pconfrnl == DSI_TWO_DATA_LANES){
		while((sil_rew_mem((uint32_t *)(hdsi->base+TOFF_DSI_PSR)) & (DSI_PSR_UAN0 | DSI_PSR_UAN1 | DSI_PSR_UANC)) != (DSI_PSR_UAN0 | DSI_PSR_UAN1 | DSI_PSR_UANC)){
			/* Check for the Timeout */
			if(tick > DSI_TIMEOUT_VALUE)
				return E_TMOUT;
			tick++;
			dly_tsk(1);
		}
	}

	/*
	 *  DSI Host PHY ULPS�Υ���å��ȥǡ����졼��ӥåȤ򤹤٤ƥ���
	 */
	sil_wrw_mem((uint32_t *)(hdsi->base+TOFF_DSI_PUCR), 0);

	/*
	 *  D-PHY����Υ���å�����������
	 */
	sil_andw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_DCKCFGR2), RCC_DCKCFGR2_DSISEL);

	/*
	 *  D-PHY���楪����
	 */
	sil_orw_mem((uint32_t *)(hdsi->base+TOFF_DSI_CLCR), DSI_CLCR_DPCC);

	return E_OK;
}

/*
 *  �ƥ��ȥѥ�������������
 *  parameter1 hdsi    DSI�ϥ�ɥ�ؤΥݥ���
 *  parameter2 mode    Pattern generator mode(0:color bars, 1:ber pattern)
 *  parameter3 orient  Pattern generator orientation(0:��ľ,1:��ʿ)
 */
void
dsi_startPatternGenerator(DSI_Handle_t *hdsi, uint32_t mode, uint32_t orient)
{
	mode   &= 1;
	orient &= 1;
	/* Configure pattern generator mode and orientation */
	sil_andw_mem((uint32_t *)(hdsi->base+TOFF_DSI_VMCR), (DSI_VMCR_PGM | DSI_VMCR_PGO));
	sil_orw_mem((uint32_t *)(hdsi->base+TOFF_DSI_VMCR), ((mode<<20) | (orient<<24)));

	/* Enable pattern generator by setting PGE bit */
	sil_orw_mem((uint32_t *)(hdsi->base+TOFF_DSI_VMCR), DSI_VMCR_PGE);
}

/*
 *  �ƥ��ȥѥ�����������λ
 *  parameter1 hdsi    DSI�ϥ�ɥ�ؤΥݥ���
 */
void
dsi_stopPatternGenerator(DSI_Handle_t *hdsi)
{
	sil_andw_mem((uint32_t *)(hdsi->base+TOFF_DSI_VMCR), DSI_VMCR_PGE);
}

/*
 *  ��������饤��ԥ�����
 *  parameter1 hdsi    DSI�ϥ�ɥ�ؤΥݥ���
 *  parameter2 customl ��������졼��
 *  parameter3 lane    �졼������
 *  parameter4 active  DSI_ACTIVE/DSI_INACTIVE
 *  return             ����ʤ�E_OK
 */
ER
dsi_setLanePinsConfiguration(DSI_Handle_t *hdsi, uint32_t customlane, uint32_t lane, uint32_t active)
{
	if(hdsi == NULL)
		return E_PAR;
	active &= 1;
	if(customlane == DSI_SWAP_LANE_PINS){
		if(lane == DSI_CLOCK_LANE)		/* SWAP PINS on clock lane */
			sil_modw_mem((uint32_t *)(hdsi->base+TOFF_DSI_WPCR0), DSI_WPCR0_SWCL, ((uint32_t)active << 6));
		else if(lane == DSI_DATA_LANE0)	/* SWAP PINS on data lane 0 */
			sil_modw_mem((uint32_t *)(hdsi->base+TOFF_DSI_WPCR0), DSI_WPCR0_SWDL0, ((uint32_t)active << 7));
		else if(lane == DSI_DATA_LANE1)	/* SWAP PINS on data lane 1 */
			sil_modw_mem((uint32_t *)(hdsi->base+TOFF_DSI_WPCR0), DSI_WPCR0_SWDL1, ((uint32_t)active << 8));
		else
			return E_PAR;
	}
	else if(customlane == DSI_INVERT_HS_SIGNAL){
		if(lane == DSI_CLOCK_LANE)		/* INVERT HS SIGNAL on clock lane */
			sil_modw_mem((uint32_t *)(hdsi->base+TOFF_DSI_WPCR0), DSI_WPCR0_HSICL, ((uint32_t)active << 9));
		else if(lane == DSI_DATA_LANE0)	/* INVERT HS SIGNAL on data lane 0 */
			sil_modw_mem((uint32_t *)(hdsi->base+TOFF_DSI_WPCR0), DSI_WPCR0_HSIDL0, ((uint32_t)active << 10));
		else if(lane == DSI_DATA_LANE1)	/* INVERT HS SIGNAL on data lane 1 */
			sil_modw_mem((uint32_t *)(hdsi->base+TOFF_DSI_WPCR0), DSI_WPCR0_HSIDL1, ((uint32_t)active << 11));
		else
			return E_PAR;
	}
	else
		return E_PAR;
	return E_OK;
}

/*
 *  PHY�����ߥ�����
 *  parameter1 hdsi    DSI�ϥ�ɥ�ؤΥݥ���
 *  parameter2 timing  ���꥿�����
 *  parameter3 active  DSI_ACTIVE/DSI_INACTIVE
 *  parameter4 value   ������             the configuration information for the DSI.
 *  return             ����ʤ�E_OK
 */
ER
dsi_setPHYTiming(DSI_Handle_t *hdsi, uint32_t timing, uint32_t active, uint32_t Value)
{
	if(hdsi == NULL)
		return E_PAR;
	active &= 1;
	switch(timing){
	case DSI_TCLK_POST:
		sil_modw_mem((uint32_t *)(hdsi->base+TOFF_DSI_WPCR0), DSI_WPCR0_TCLKPOSTEN, (active << 27));
		if(active == DSI_ACTIVE)
			sil_modw_mem((uint32_t *)(hdsi->base+TOFF_DSI_WPCR4), DSI_WPCR4_TCLKPOST, Value & DSI_WPCR4_TCLKPOST);
		break;
	case DSI_TLPX_CLK:
		sil_modw_mem((uint32_t *)(hdsi->base+TOFF_DSI_WPCR0), DSI_WPCR0_TLPXCEN, (active << 26));
		if(active == DSI_ACTIVE)
			sil_modw_mem((uint32_t *)(hdsi->base+TOFF_DSI_WPCR3), DSI_WPCR3_TLPXC, (Value << 24) & DSI_WPCR3_TLPXC);
		break;
	case DSI_THS_EXIT:
		sil_modw_mem((uint32_t *)(hdsi->base+TOFF_DSI_WPCR0), DSI_WPCR0_THSEXITEN, (active << 25));
		if(active == DSI_ACTIVE)
			sil_modw_mem((uint32_t *)(hdsi->base+TOFF_DSI_WPCR3), DSI_WPCR3_THSEXIT, (Value << 16) & DSI_WPCR3_THSEXIT);
		break;
	case DSI_TLPX_DATA:
		sil_modw_mem((uint32_t *)(hdsi->base+TOFF_DSI_WPCR0), DSI_WPCR0_TLPXDEN, (active << 24));
		if(active == DSI_ACTIVE)
			sil_modw_mem((uint32_t *)(hdsi->base+TOFF_DSI_WPCR3), DSI_WPCR3_TLPXD, (Value << 8) & DSI_WPCR3_TLPXD);
		break;
	case DSI_THS_ZERO:
		sil_modw_mem((uint32_t *)(hdsi->base+TOFF_DSI_WPCR0), DSI_WPCR0_THSZEROEN, (active << 23));
		if(active == DSI_ACTIVE)
			sil_modw_mem((uint32_t *)(hdsi->base+TOFF_DSI_WPCR3), DSI_WPCR3_THSZERO, Value & DSI_WPCR3_THSZERO);
		break;
	case DSI_THS_TRAIL:
		sil_modw_mem((uint32_t *)(hdsi->base+TOFF_DSI_WPCR0), DSI_WPCR0_THSTRAILEN, (active << 22));
		if(active == DSI_ACTIVE)
			sil_modw_mem((uint32_t *)(hdsi->base+TOFF_DSI_WPCR2), DSI_WPCR2_THSTRAIL, (Value << 24) & DSI_WPCR2_THSTRAIL);
		break;
	case DSI_THS_PREPARE:
		sil_modw_mem((uint32_t *)(hdsi->base+TOFF_DSI_WPCR0), DSI_WPCR0_THSPREPEN, (active << 21));
		if(active == DSI_ACTIVE)
			sil_modw_mem((uint32_t *)(hdsi->base+TOFF_DSI_WPCR2), DSI_WPCR2_THSPREP, (Value << 16) & DSI_WPCR2_THSPREP);
		break;
	case DSI_TCLK_ZERO:
		sil_modw_mem((uint32_t *)(hdsi->base+TOFF_DSI_WPCR0), DSI_WPCR0_TCLKZEROEN, (active << 20));
		if(active == DSI_ACTIVE)
			sil_modw_mem((uint32_t *)(hdsi->base+TOFF_DSI_WPCR2), DSI_WPCR2_TCLKZERO, (Value << 8) & DSI_WPCR2_TCLKZERO);
		break;
	case DSI_TCLK_PREPARE:
		sil_modw_mem((uint32_t *)(hdsi->base+TOFF_DSI_WPCR0), DSI_WPCR0_TCLKPREPEN, (active << 19));
		if(active == DSI_ACTIVE)
			sil_modw_mem((uint32_t *)(hdsi->base+TOFF_DSI_WPCR2), DSI_WPCR2_TCLKPREP, Value & DSI_WPCR2_TCLKPREP);
		break;
	default:
		return E_PAR;
	}
	return E_OK;
}

/*
 *  ����å�/�ǡ����졼�������
 *  parameter1 hdsi    DSI�ϥ�ɥ�ؤΥݥ���
 *  parameter2 lane    �졼������(DSI_CLOCK_LANE/DSI_DATA_LANES)
 *  parameter3 actine  DSI_ACTIVE/DSI_INACTIVE
 *  return             ����ʤ�E_OK
 */
ER
dsi_forceTXStopMode(DSI_Handle_t *hdsi, uint32_t lane, uint32_t active)
{
	if(hdsi == NULL)
		return E_PAR;
	if(lane == DSI_CLOCK_LANE){
		/*
		 *  ����å��졼�󥹥ȥå�
		 */
		sil_modw_mem((uint32_t *)(hdsi->base+TOFF_DSI_WPCR0), DSI_WPCR0_FTXSMCL, (active << 12));
	}
	else if(lane == DSI_DATA_LANES){
		/*
		 *  �ǡ����졼�󥹥ȥå�
		 */
		sil_modw_mem((uint32_t *)(hdsi->base+TOFF_DSI_WPCR0), DSI_WPCR0_FTXSMDL, (active << 13));
	}
	return E_OK;
}

/*
 *  DSI����ߥϥ�ɥ�
 */
void dsi_irqhandler(DSI_Handle_t *hdsi)
{
	uint32_t isr0 = sil_rew_mem((uint32_t *)(hdsi->base+TOFF_DSI_ISR0));
	uint32_t isr1 = sil_rew_mem((uint32_t *)(hdsi->base+TOFF_DSI_ISR1));

	isr0 &= sil_rew_mem((uint32_t *)(hdsi->base+TOFF_DSI_IER0));
	isr1 &= sil_rew_mem((uint32_t *)(hdsi->base+TOFF_DSI_IER1));

	/*
	 *  �ƥ�����󥰥��ե����ȥ����å�
	 */
	if((sil_rew_mem((uint32_t *)(hdsi->base+TOFF_DSI_WISR)) & DSI_WISR_TEIF) != 0){
		if((sil_rew_mem((uint32_t *)(hdsi->base+TOFF_DSI_WISR)) & DSI_WIER_TEIE) != 0){
			/*
			 *  �ƥ�����󥰥��ե����ȳ���ߥ��ꥢ
			 */
			sil_wrw_mem((uint32_t *)(hdsi->base+TOFF_DSI_WIFCR), DSI_WISR_TEIF);

			/*
			 *  �ƥ�����󥰥��ե����ȥ�����Хå�
			 */
			if(hdsi->teffectcallback != NULL)
				hdsi->teffectcallback(hdsi);
		}
	}

	/*
	 *  ��ե�å��彪λ���٥�ȥ����å�
	 */
	if((sil_rew_mem((uint32_t *)(hdsi->base+TOFF_DSI_WISR)) & DSI_WISR_ERIF) != 0){
		if((sil_rew_mem((uint32_t *)(hdsi->base+TOFF_DSI_WISR)) & DSI_WIER_ERIE) != 0){
			/*
			 *  ��ե�å������ߥ��ꥢ
			 */
			sil_wrw_mem((uint32_t *)(hdsi->base+TOFF_DSI_WIFCR), DSI_WISR_ERIF);

			/*
			 *  ��ե�å��女����Хå�
			 */
			if(hdsi->refreshcallback != NULL)
				hdsi->refreshcallback(hdsi);
		}
	}

	/*
	 *  ���顼�ե饰�����å�
	 */
	if((isr0 & DSI_INT_ACKERR_REQ) != 0)
		hdsi->errorcode |= DSI_ERROR_ACK;
	if((isr0 & DSI_INT_PHYERR_REQ) != 0)
		hdsi->errorcode |= DSI_ERROR_PHY;
	if((isr1 & DSI_ISR1_TOHSTX) != 0)
		hdsi->errorcode |= DSI_ERROR_TX;
	if((isr1 & DSI_ISR1_TOLPRX) != 0)
		hdsi->errorcode |= DSI_ERROR_RX;
	if((isr1 & (DSI_ISR1_ECCSE | DSI_ISR1_ECCME)) != 0)
		hdsi->errorcode |= DSI_ERROR_ECC;
	if((isr1 & DSI_ISR1_CRCE) != 0)
		hdsi->errorcode |= DSI_ERROR_CRC;
	if((isr1 & DSI_ISR1_PSE) != 0)
		hdsi->errorcode |= DSI_ERROR_PSE;
	if((isr1 & DSI_ISR1_EOTPE) != 0)
		hdsi->errorcode |= DSI_ERROR_EOT;
	if((isr1 & DSI_ISR1_LPWRE) != 0)
		hdsi->errorcode |= DSI_ERROR_OVF;
	if((isr1 & DSI_INT_GENERR_REQ) != 0)
		hdsi->errorcode |= DSI_ERROR_GEN;

	/*
	 *  DSI���顼������Хå�
	 */
	if(hdsi->errorcode != DSI_ERROR_NONE){
		if(hdsi->errorcallback != NULL)
			hdsi->errorcallback(hdsi);
	}
}

