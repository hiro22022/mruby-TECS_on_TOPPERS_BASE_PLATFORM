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
 *  @(#) $Id: ltdc.c 698 2017-02-24 18:26:43Z roi $
 */
/*
 * STM32F746-LTDC�ѥǥХ����ɥ饤��
 */
#include "kernel_impl.h"
#include <t_syslog.h>
#include <t_stdlib.h>
#include <sil.h>
#include <target_syssvc.h>
#include "kernel_cfg.h"
#include "device.h"
#include "ltdc.h"


/*
 *  SIL�ؿ��Υޥ������
 */
#define sil_orw_mem(a, b)		sil_wrw_mem((a), sil_rew_mem(a) | (b))
#define sil_andw_mem(a, b)		sil_wrw_mem((a), sil_rew_mem(a) & ~(b))
#define sil_modw_mem(a, b, c)	sil_wrw_mem((a), (sil_rew_mem(a) & (~b)) | (c))

/*
 *  AF 9������������
 */
#define GPIO_AF9_CAN1           0x09	/* CAN1 Alternate Function mapping    */
#define GPIO_AF9_CAN2           0x09	/* CAN2 Alternate Function mapping    */
#define GPIO_AF9_TIM12          0x09	/* TIM12 Alternate Function mapping   */
#define GPIO_AF9_TIM13          0x09	/* TIM13 Alternate Function mapping   */
#define GPIO_AF9_TIM14          0x09	/* TIM14 Alternate Function mapping   */
#define GPIO_AF9_QUADSPI        0x09	/* QUADSPI Alternate Function mapping */
#define GPIO_AF9_LTDC           0x09	/* LCD-TFT Alternate Function mapping */

/*
 *  AF 14������������
 */
#define GPIO_AF14_LTDC          0x0E	/* LCD-TFT alternate function�ޥå��� */

#define PLLSAI_TIMEOUT_VALUE   (100*1000)

#define RCC_PLLSAICFGR_PLLSAIQ_POS    24
#define RCC_PLLSAICFGR_PLLSAIP_POS    16

#ifdef TOPPERS_STM32F7_DISCOVERY
static const GPIO_Init_Table ltdc_gpio_table[] = {
	{TADR_GPIOG_BASE, (GPIO_PIN_12) },
	{TADR_GPIOE_BASE, (GPIO_PIN_4)  },
	{TADR_GPIOI_BASE, (GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15 ) },
	{TADR_GPIOJ_BASE, (GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | \
			  GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | \
			  GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15) },
	{TADR_GPIOK_BASE, (GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7) }
};

#define NUM_LTDC_GPIO_ITEM (sizeof(ltdc_gpio_table)/sizeof(GPIO_Init_Table))

/*
 *  LTDC GPIO�ν����
 */
static void
ltdc_gpio_init(void)
{
	GPIO_Init_t GPIO_Init_Data;
	int i, pin;
	volatile uint32_t tmp;

	/* LTDC����å����� */
	sil_orw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_APB2ENR), RCC_APB2ENR_LTDCEN);
	tmp = sil_rew_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_APB2ENR));

	/* LTDC��GPIO�Υ���å����� */
	sil_orw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_AHB1ENR),
			  RCC_AHB1ENR_GPIOEEN | RCC_AHB1ENR_GPIOGEN
			| RCC_AHB1ENR_GPIOIEN | RCC_AHB1ENR_GPIOJEN
			| RCC_AHB1ENR_GPIOKEN);
	tmp = sil_rew_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_AHB1ENR));
	(void)(tmp);

	/* LTDC��GPIO�ν�����ѥ�᡼������ */
	GPIO_Init_Data.mode      = GPIO_MODE_AF;
	GPIO_Init_Data.pull      = GPIO_NOPULL;
	GPIO_Init_Data.otype     = GPIO_OTYPE_PP;
	GPIO_Init_Data.speed     = GPIO_SPEED_FAST;
	GPIO_Init_Data.alternate = GPIO_AF9_LTDC;

	for(i = 0 ; i < NUM_LTDC_GPIO_ITEM ; i++){
		for(pin = 0 ; pin < 16 ; pin++){
			if((ltdc_gpio_table[i].pinmap & 1<<pin) != 0)
				gpio_setup(ltdc_gpio_table[i].base, &GPIO_Init_Data, pin);
		}
		GPIO_Init_Data.alternate = GPIO_AF14_LTDC;
	}

	/* LCDɽ����GPIO������ */
	GPIO_Init_Data.mode      = GPIO_MODE_OUTPUT;
	gpio_setup(TADR_GPIOI_BASE, &GPIO_Init_Data, PINPOSITION12);
	gpio_setup(TADR_GPIOK_BASE, &GPIO_Init_Data, PINPOSITION3);

	/* ɽ������LCD_DISP�ԥ�򥢥����� */
	sil_wrw_mem((uint32_t *)(TADR_GPIOI_BASE+TOFF_GPIO_BSRR), GPIO_PIN_12);

	/* �Хå��饤��LCD_BL_CTRL�ԥ�򥢥����� */
	sil_wrw_mem((uint32_t *)(TADR_GPIOK_BASE+TOFF_GPIO_BSRR), GPIO_PIN_3);
}
#endif

/*
 *  LTDC�Υ쥤�䡼�Υϡ��ɥ�����������ꤹ��
 *  parameter1 phandle   LTDC�ν�����깽¤�ΤؤΥݥ���
 *  parameter2 pLayerCfg �쥤�䡼�ν�����깽¤�ΤΥݥ���
 *  parameter3 LaerIdx   �쥤�䡼�ֹ�
 *  return               ����ʤ��E_OK
 */
static void
LTDC_SetConfig(LTDC_Handle_t *phandle, LTDC_LayerCfg_t *pLayerCfg, uint32_t LayerIdx)
{
	uint32_t layer = TADR_LTDC_LAYER1+LayerIdx*LTDC_WINDOW_SIZE;
	uint32_t bpcr  = sil_rew_mem((uint32_t *)(phandle->base+TOFF_LTDC_BPCR));
	uint32_t tmp = 0;

	/* ��ʿ�����������ȥ��ȥåװ������� */
	tmp  = (pLayerCfg->WindowX1 + ((bpcr & LTDC_BPCR_AHBP) >> 16)) << 16;
	tmp |= (pLayerCfg->WindowX0 + ((bpcr & LTDC_BPCR_AHBP) >> 16) + 1);
	sil_wrw_mem((uint32_t *)(layer+TOFF_LTDCW_WHPCR), tmp);

	/* ��ľ�����������ȥ��ȥåװ������� */
	tmp  = (pLayerCfg->WindowY1 + (bpcr & LTDC_BPCR_AVBP)) << 16;
	tmp |= (pLayerCfg->WindowY0 + (bpcr & LTDC_BPCR_AVBP) + 1);
	sil_wrw_mem((uint32_t *)(layer+TOFF_LTDCW_WVPCR), tmp);

	/* �ԥ�����ե����ޥåȤλ��� */
	sil_modw_mem((uint32_t *)(layer+TOFF_LTDCW_PFCR), LTDC_PFCR_PF, pLayerCfg->PixelFormat);

	/* �ǥե���ȤΥ��顼�ͤ����� */
	tmp  = pLayerCfg->Backcolor.Blue;
	tmp |= pLayerCfg->Backcolor.Green << 8;
	tmp |= pLayerCfg->Backcolor.Red << 16;
	tmp |= pLayerCfg->Alpha0 << 24;
	sil_wrw_mem((uint32_t *)(layer+TOFF_LTDCW_DCCR), tmp);

	/* ����ե��ͤ����� */
	sil_wrw_mem((uint32_t *)(layer+TOFF_LTDCW_CACR), pLayerCfg->Alpha);

	/* �֥��ǥ��󥰥ե������������� */
	sil_wrw_mem((uint32_t *)(layer+TOFF_LTDCW_BFCR), (pLayerCfg->BlendingFactor1 | pLayerCfg->BlendingFactor2));

	/* �ե졼��Хåե����ɥ쥹������ */
	sil_wrw_mem((uint32_t *)(layer+TOFF_LTDCW_CFBAR), pLayerCfg->FBStartAdress);

	/* �Х�����Υ��顼�ե졼��ԥå������� */
	if(pLayerCfg->PixelFormat == LTDC_PIXEL_FORMAT_ARGB8888)
		tmp = 4;
	else if (pLayerCfg->PixelFormat == LTDC_PIXEL_FORMAT_RGB888)
		tmp = 3;
	else if((pLayerCfg->PixelFormat == LTDC_PIXEL_FORMAT_ARGB4444) ||
		(pLayerCfg->PixelFormat == LTDC_PIXEL_FORMAT_RGB565)   ||
		  (pLayerCfg->PixelFormat == LTDC_PIXEL_FORMAT_ARGB1555) ||
		    (pLayerCfg->PixelFormat == LTDC_PIXEL_FORMAT_AL88))
		tmp = 2;
	else
		tmp = 1;
	sil_wrw_mem((uint32_t *)(layer+TOFF_LTDCW_CFBLR), (((pLayerCfg->ImageWidth * tmp) << 16) | (((pLayerCfg->WindowX1 - pLayerCfg->WindowX0) * tmp)  + 3)));

	/* �ե졼��Хåե��饤��������� */
	sil_wrw_mem((uint32_t *)(layer+TOFF_LTDCW_CFBLNR), pLayerCfg->ImageHeight);

	/* LTDC�쥤�䡼����� */
	sil_orw_mem((uint32_t *)(layer+TOFF_LTDCW_CR), LTDC_CR_LEN);
}

/*
 *  LTDC�Υ�����ɥ��Υ���ե��ͤ����ꤹ��
 *  parameter1 phandle   LTDC�ν�����깽¤�ΤؤΥݥ���
 *  parameter2 Alpha     ������ɥ�����ե���
 *  parameter3 LaerIdx   �쥤�䡼�ֹ�
 *  return               ����ʤ��E_OK
 */
ER
ltdc_setalpha(LTDC_Handle_t *phandle, uint32_t Alpha, uint32_t LayerIdx)
{
	LTDC_LayerCfg_t *pLayerCfg;

	if(LayerIdx >= MAX_LAYER)
		return E_PAR;
	if(Alpha > 511)
		return E_PAR;

	/* �쥤�䡼����ե�����졼������ΰ�Υݥ��󥿼��� */
	pLayerCfg = &phandle->LayerCfg[LayerIdx];
	/* ������ɥ�����ե������� */
	pLayerCfg->Alpha = Alpha;
	/* LTDC�쥤�䡼���� */
	LTDC_SetConfig(phandle, pLayerCfg, LayerIdx);
	/* ����� */
	sil_wrw_mem((uint32_t *)(phandle->base+TOFF_LTDC_SRCR), LTDC_SRCR_IMR);
	return E_OK;
}

/*
 *  LTDC�Υե졼��Хåե����ɥ쥹�����ꤹ��
 *  parameter1 phandle   LTDC�ν�����깽¤�ΤؤΥݥ���
 *  parameter2 Alpha     ���ɥ쥹��
 *  parameter3 LaerIdx   �쥤�䡼�ֹ�
 *  return               ����ʤ��E_OK
 */
ER
ltdc_setaddress(LTDC_Handle_t *phandle, uint32_t Address, uint32_t LayerIdx)
{
	LTDC_LayerCfg_t *pLayerCfg;

	if(LayerIdx >= MAX_LAYER)
		return E_PAR;

	/* �쥤�䡼����ե�����졼������ΰ�Υݥ��󥿼��� */
	pLayerCfg = &phandle->LayerCfg[LayerIdx];
	/* �ե졼��Хåե����ɥ쥹���� */
	pLayerCfg->FBStartAdress = Address;
	/* LTDC�쥤�䡼���� */
	LTDC_SetConfig(phandle, pLayerCfg, LayerIdx);
	/* ����� */
	sil_wrw_mem((uint32_t *)(phandle->base+TOFF_LTDC_SRCR), LTDC_SRCR_IMR);
	return E_OK;
}

/*
 *  LTDC�Υ�����ɥ����������֤����ꤹ��
 *  parameter1 phandle   LTDC�ν�����깽¤�ΤؤΥݥ���
 *  parameter2 XSize     ������ɥ�X������
 *  parameter3 YSize     ������ɥ�Y������
 *  parameter4 LaerIdx   �쥤�䡼�ֹ�
 *  return               ����ʤ��E_OK
 */
ER
ltdc_setwindowsize(LTDC_Handle_t *phandle, uint32_t XSize, uint32_t YSize, uint32_t LayerIdx)
{
	LTDC_LayerCfg_t *pLayerCfg;

	/* �쥤�䡼����ե�����졼������ΰ�Υݥ��󥿼��� */
	pLayerCfg = &phandle->LayerCfg[LayerIdx];
	if(LayerIdx >= MAX_LAYER)
		return E_PAR;
	if(XSize > 8191 || YSize > 8191)
		return E_PAR;

	/* ������ɥ�X�������� */
	pLayerCfg->WindowX0 = 0;
	pLayerCfg->WindowX1 = XSize + pLayerCfg->WindowX0;
	/* ������ɥ�Y�������� */
	pLayerCfg->WindowY0 = 0;
	pLayerCfg->WindowY1 = YSize + pLayerCfg->WindowY0;
	/* ������ɥ�X���������� */
	pLayerCfg->ImageWidth = XSize;
	/* ������ɥ�Y������ */
	pLayerCfg->ImageHeight = YSize;
	/* LTDC�쥤�䡼���� */
	LTDC_SetConfig(phandle, pLayerCfg, LayerIdx);
	/* ����� */
	sil_wrw_mem((uint32_t *)(phandle->base+TOFF_LTDC_SRCR), LTDC_SRCR_IMR);
	return E_OK;
}

/*
 *  LTDC�Υ�����ɥ����֤����ꤹ��
 *  parameter1 phandle   LTDC�ν�����깽¤�ΤؤΥݥ���
 *  parameter2 X0        ������ɥ�X����
 *  parameter3 Y0        ������ɥ�Y����
 *  parameter4 LaerIdx   �쥤�䡼�ֹ�
 *  return               ����ʤ��E_OK
 */
ER
ltdc_setwindowposition(LTDC_Handle_t *phandle, uint32_t X0, uint32_t Y0, uint32_t LayerIdx)
{
	LTDC_LayerCfg_t *pLayerCfg;

	/* �쥤�䡼����ե�����졼������ΰ�Υݥ��󥿼��� */
	pLayerCfg = &phandle->LayerCfg[LayerIdx];
	if(LayerIdx >= MAX_LAYER)
		return E_PAR;
	if(X0 > 4095 || (X0+pLayerCfg->ImageWidth) > 65535)
		return E_PAR;
	if(Y0 > 4095 || (Y0+pLayerCfg->ImageHeight) > 65535)
		return E_PAR;

	/* ������ɥ�X�������� */
	pLayerCfg->WindowX0 = X0;
	pLayerCfg->WindowX1 = X0 + pLayerCfg->ImageWidth;
	/* ������ɥ�Y�������� */
	pLayerCfg->WindowY0 = Y0;
	pLayerCfg->WindowY1 = Y0 + pLayerCfg->ImageHeight;
	/* LTDC�쥤�䡼���� */
	LTDC_SetConfig(phandle, pLayerCfg, LayerIdx);
	/* ����� */
	sil_wrw_mem((uint32_t *)(phandle->base+TOFF_LTDC_SRCR), LTDC_SRCR_IMR);
	return E_OK;
}

/*
 *  ���顼�����󥰤ν�������Ԥ�
 *  parameter1 phandle   LTDC�ν�����깽¤�ΤؤΥݥ���
 *  parameter2 RGBvalue  ���顼���������ֹ�
 *  parameter3 LaerIdx   �쥤�䡼�ֹ�
 *  return               ����ʤ��E_OK
 */
ER
ltdc_configcolorkeying(LTDC_Handle_t *phandle, uint32_t RGBValue, uint32_t LayerIdx)
{
	uint32_t layer = TADR_LTDC_LAYER1+LayerIdx*LTDC_WINDOW_SIZE;

	if(LayerIdx >= MAX_LAYER)
		return E_PAR;

	/* ���顼���������ֹ����� */
	sil_andw_mem((uint32_t *)(layer+TOFF_LTDCW_CKCR), (LTDC_CKCR_CKBLUE | LTDC_CKCR_CKGREEN | LTDC_CKCR_CKRED));
	sil_wrw_mem((uint32_t *)(layer+TOFF_LTDCW_CKCR), RGBValue);
	/* ����� */
	sil_wrw_mem((uint32_t *)(phandle->base+TOFF_LTDC_SRCR), LTDC_SRCR_IMR);
	return E_OK;
}

/*
 *  ���顼�����󥰤�ͭ���ˤ���
 *  parameter1 phandle   LTDC�ν�����깽¤�ΤؤΥݥ���
 *  parameter2 LaerIdx   �쥤�䡼�ֹ�
 *  return               ����ʤ��E_OK
 */
ER
ltdc_enablecolorkeying(LTDC_Handle_t *phandle, uint32_t LayerIdx)
{
	if(LayerIdx >= MAX_LAYER)
		return E_PAR;

	/* COLKEN�ӥåȤ򥪥� */
	sil_orw_mem((uint32_t *)(TADR_LTDC_LAYER1+LayerIdx*LTDC_WINDOW_SIZE+TOFF_LTDCW_CR), LTDC_CR_COLKEN);
	/* ����� */
	sil_wrw_mem((uint32_t *)(phandle->base+TOFF_LTDC_SRCR), LTDC_SRCR_IMR);
	return E_OK;
}

/*
 *  ���顼�����󥰤�̵���ˤ���
 *  parameter1 phandle   LTDC�ν�����깽¤�ΤؤΥݥ���
 *  parameter2 LaerIdx   �쥤�䡼�ֹ�
 *  return               ����ʤ��E_OK
 */
ER
ltdc_disablecolorkeying(LTDC_Handle_t *phandle, uint32_t LayerIdx)
{
	if(LayerIdx >= MAX_LAYER)
		return E_PAR;

	/* COLKEN�ӥåȤ򥪥� */
	sil_andw_mem((uint32_t *)(TADR_LTDC_LAYER1+LayerIdx*LTDC_WINDOW_SIZE+TOFF_LTDCW_CR), LTDC_CR_COLKEN);
	/* ����� */
	sil_wrw_mem((uint32_t *)(phandle->base+TOFF_LTDC_SRCR), LTDC_SRCR_IMR);
	return E_OK;
}

/*
 *  LTDC�Υ쥤�䡼����򤪤��ʤ�
 *  parameter1 phandle   LTDC�ν�����깽¤�ΤؤΥݥ���
 *  patameter2 pLayerCfg �쥤�䡼���깽¤�ΤؤΥݥ���
 *  parameter3 LaerIdx   �쥤�䡼�ֹ�
 *  return               ����ʤ��E_OK
 */
ER
ltdc_configlayer(LTDC_Handle_t *phandle, LTDC_LayerCfg_t *pLayerCfg, uint32_t LayerIdx)
{
	if(LayerIdx >= MAX_LAYER)
		return E_PAR;
	/* �쥤�䡼����ե����졼���������򥳥ԡ� */
	phandle->LayerCfg[LayerIdx] = *pLayerCfg;
	/* LTDC�쥤�䡼���� */
	LTDC_SetConfig(phandle, pLayerCfg, LayerIdx);
	/* ����� */
	sil_wrw_mem((uint32_t *)(phandle->base+TOFF_LTDC_SRCR), LTDC_SRCR_IMR);
	return E_OK;
}

/*
 *  LTDC�饤�󥤥٥������
 *  parameter1 phandle   LTDC�ν�����깽¤�ΤؤΥݥ���
 *  patameter2 Line      �饤�����
 *  return               ����ʤ��E_OK
 */
ER
ltdc_lineevent(LTDC_Handle_t *phandle, uint32_t Line)
{
	/* Change LTDC peripheral state */
	phandle->state = LTDC_STATE_BUSY;

	/* Enable the Line interrupt */
	sil_orw_mem((uint32_t *)(phandle->base+TOFF_LTDC_IER), LTDC_IER_LIE);

	/* Sets the Line Interrupt position */
	sil_wrw_mem((uint32_t *)(phandle->base+TOFF_LTDC_LIPCR), Line);

	/* Change the LTDC state*/
	phandle->state = LTDC_STATE_READY;
	return E_OK;
}

/*
 *  LTDC�쥸�������������
 *  parameter1 phandle    LTDC�ν�����깽¤�ΤؤΥݥ���
 *  patameter2 ReloadType ����ɥ�����
 *  return                ����ʤ��E_OK
 */
ER
ltdc_reload(LTDC_Handle_t *phandle, uint32_t ReloadType)
{
	/* Change LTDC peripheral state */
	phandle->state = LTDC_STATE_BUSY;

	/* Enable the Reload interrupt */  
	sil_orw_mem((uint32_t *)(phandle->base+TOFF_LTDC_IER), LTDC_IER_RRIE);

	/* Apply Reload type */
	sil_wrw_mem((uint32_t *)(phandle->base+TOFF_LTDC_SRCR), ReloadType);

	/* Change the LTDC state*/
	phandle->state = LTDC_STATE_READY;
	return E_OK;
}

/*
 *  LTDC�ν�������Ԥ�
 *  parameter1 phandle  LTDC�Υϥ�ɥ�ؤΥݥ���
 *  return              ����ʤ�E_OK
 */
ER
ltdc_init(LTDC_Handle_t *phandle)
{
	uint32_t timecount = 0;
	uint32_t tmpreg0 = 0;
	uint32_t tmpreg1 = 0;

	/*
	 * PLLSAI����å����
     */
	sil_andw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CR), RCC_CR_PLLSAION);

    timecount = 0;
	while((sil_rew_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CR)) & (RCC_CR_PLLSAIRDY)) == RCC_CR_PLLSAIRDY){
		if( timecount > PLLSAI_TIMEOUT_VALUE){
			return E_TMOUT;	/* �����ॢ����ȯ�� */
		}
		sil_dly_nse(1000);
		timecount++;
	}

	/*
	 *  PLLSAIP/PLLSAIQ����
	 */
	tmpreg0 = ((sil_rew_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_PLLSAICFGR)) & RCC_PLLSAICFGR_PLLSAIQ) >> RCC_PLLSAICFGR_PLLSAIQ_POS);
	tmpreg1 = ((sil_rew_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_PLLSAICFGR)) & RCC_PLLSAICFGR_PLLSAIP) >> RCC_PLLSAICFGR_PLLSAIP_POS);
	sil_wrw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_PLLSAICFGR), (phandle->Init.pllsain << 6) | (tmpreg1 << 16) | (tmpreg0 << 24) | (phandle->Init.pllsair << 28));
	sil_modw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_DCKCFGR1), RCC_DCKCFGR1_PLLSAIDIVR, phandle->Init.saidivr);

    /*
	 *  PLLSAI����å�����
	 */
	sil_orw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CR), RCC_CR_PLLSAION);

    /*
	 *  PLLSAI��ǥ��Ԥ�
	 */
	timecount = 0;
	while((sil_rew_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CR)) & (RCC_CR_PLLSAIRDY)) != RCC_CR_PLLSAIRDY){
		if( timecount > PLLSAI_TIMEOUT_VALUE){
			return E_TMOUT;	/* �����ॢ����ȯ�� */
		}
		sil_dly_nse(1000);
		timecount++;
	}

#ifdef TOPPERS_STM32F7_DISCOVERY
	/*
	 *  LTDC�ν������
	 */
	/* GPIO���� */
	ltdc_gpio_init();
#endif

	/* HS/VS/DE/PC�ζ������� */
	sil_modw_mem((uint32_t *)(phandle->base+TOFF_LTDC_GCR), (LTDC_GCR_HSPOL | LTDC_GCR_VSPOL | LTDC_GCR_DEPOL | LTDC_GCR_PCPOL),
				(phandle->Init.HSPolarity | phandle->Init.VSPolarity | phandle->Init.DEPolarity | phandle->Init.PCPolarity));
	/* ƱĴ���������� */
	sil_modw_mem((uint32_t *)(phandle->base+TOFF_LTDC_SSCR), (LTDC_SSCR_VSH | LTDC_SSCR_HSW),
				((phandle->Init.HorizontalSync << 16) | phandle->Init.VerticalSync));
	/* �Хå��ݡ������� */
	sil_modw_mem((uint32_t *)(phandle->base+TOFF_LTDC_BPCR), (LTDC_BPCR_AVBP | LTDC_BPCR_AHBP),
				((phandle->Init.AccumulatedHBP << 16) | phandle->Init.AccumulatedVBP));
	/* �����ƥ��������� */
	sil_modw_mem((uint32_t *)(phandle->base+TOFF_LTDC_AWCR), (LTDC_AWCR_AAH | LTDC_AWCR_AAW),
				((phandle->Init.AccumulatedActiveW << 16) | phandle->Init.AccumulatedActiveH));
	/* �ȡ����������� */
	sil_modw_mem((uint32_t *)(phandle->base+TOFF_LTDC_TWCR), (LTDC_TWCR_TOTALH | LTDC_TWCR_TOTALW),
				((phandle->Init.TotalWidth << 16) | phandle->Init.TotalHeigh));
	/* �Хå����饦��ɥ��顼���� */
	sil_modw_mem((uint32_t *)(phandle->base+TOFF_LTDC_BCCR), (LTDC_BCCR_BCBLUE | LTDC_BCCR_BCGREEN | LTDC_BCCR_BCRED),
				((phandle->Init.Backcolor.Red << 16) | (phandle->Init.Backcolor.Green << 8) | phandle->Init.Backcolor.Blue));
	/* ���顼�ꥻ�å� */
	sil_orw_mem((uint32_t *)(phandle->base+TOFF_LTDC_IER), (LTDC_IER_TERRIE | LTDC_IER_FUIE));
	/* LTDC�¹����� */
	sil_orw_mem((uint32_t *)(phandle->base+TOFF_LTDC_GCR), LTDC_GCR_LTDCEN);
	phandle->state = LTDC_STATE_READY;
	return E_OK;
}

/*
 *  LTDC����ߥϥ�ɥ�
 */
void
ltdc_irqhandler(LTDC_Handle_t *phandle)
{
	uint32_t isr = sil_rew_mem((uint32_t *)(phandle->base+TOFF_LTDC_ISR));

	/*
	 *  ����ߥޥ����ȥ��ꥢ
	 */
	sil_andw_mem((uint32_t *)(phandle->base+TOFF_LTDC_IER), isr);
	sil_wrw_mem((uint32_t *)(phandle->base+TOFF_LTDC_ICR), isr);

	/*
	 *  ž�����顼�����å�
	 */
	if((isr & LTDC_ISR_TERRIF) != 0){
		/* Update error code */
		phandle->errorcode |= LTDC_ERROR_TE;
		phandle->state      = LTDC_STATE_ERROR;
		if(phandle->errorcallback != NULL)
			phandle->errorcallback(phandle);
	}

	/*
	 *  FIFO���������󥨥顼�����å�
	 */
	if((isr & LTDC_ISR_FUIF) != 0){
		/* Update error code */
		phandle->errorcode |= LTDC_ERROR_FU;
		phandle->state      = LTDC_STATE_ERROR;
		if(phandle->errorcallback != NULL)
			phandle->errorcallback(phandle);
	}

	/*
	 *  �饤�󥤥٥�ȥ����å�
	 */
	if((isr & LTDC_IER_LIE) != 0){
		/* Change LTDC state */
		phandle->state = LTDC_STATE_READY;
		if(phandle->linecallback != NULL)
			phandle->linecallback(phandle);
	}

	/*
	 * ����ɥ��٥�ȥ����å�
	 */
	if((isr & LTDC_ISR_RRIF) != 0){
		/* Change LTDC state */
		phandle->state = LTDC_STATE_READY;
		if(phandle->reloadcallback != NULL)
			phandle->reloadcallback(phandle);
	}
}

