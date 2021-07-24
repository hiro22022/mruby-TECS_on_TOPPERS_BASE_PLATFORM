/*
 *  TOPPERS/ASP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Advanced Standard Profile Kernel
 * 
 *  Copyright (C) 2008-2011 by Embedded and Real-Time Systems Laboratory
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
 *  Copyright (C) 2015-2017 by TOPPERS PROJECT Educational Working Group.
 * 
 *  ��L���쌠�҂́C�ȉ���(1)�`(4)�̏����𖞂����ꍇ�Ɍ���C�{�\�t�g�E�F
 *  �A�i�{�\�t�g�E�F�A�����ς������̂��܂ށD�ȉ������j���g�p�E�����E��
 *  �ρE�Ĕz�z�i�ȉ��C���p�ƌĂԁj���邱�Ƃ𖳏��ŋ�������D
 *  (1) �{�\�t�g�E�F�A���\�[�X�R�[�h�̌`�ŗ��p����ꍇ�ɂ́C��L�̒���
 *      ���\���C���̗��p��������щ��L�̖��ۏ؋K�肪�C���̂܂܂̌`�Ń\�[
 *      �X�R�[�h���Ɋ܂܂�Ă��邱�ƁD
 *  (2) �{�\�t�g�E�F�A���C���C�u�����`���ȂǁC���̃\�t�g�E�F�A�J���Ɏg
 *      �p�ł���`�ōĔz�z����ꍇ�ɂ́C�Ĕz�z�ɔ����h�L�������g�i���p
 *      �҃}�j���A���Ȃǁj�ɁC��L�̒��쌠�\���C���̗��p��������щ��L
 *      �̖��ۏ؋K����f�ڂ��邱�ƁD
 *  (3) �{�\�t�g�E�F�A���C�@��ɑg�ݍ��ނȂǁC���̃\�t�g�E�F�A�J���Ɏg
 *      �p�ł��Ȃ��`�ōĔz�z����ꍇ�ɂ́C���̂����ꂩ�̏����𖞂�����
 *      �ƁD
 *    (a) �Ĕz�z�ɔ����h�L�������g�i���p�҃}�j���A���Ȃǁj�ɁC��L�̒�
 *        �쌠�\���C���̗��p��������щ��L�̖��ۏ؋K����f�ڂ��邱�ƁD
 *    (b) �Ĕz�z�̌`�Ԃ��C�ʂɒ�߂���@�ɂ���āCTOPPERS�v���W�F�N�g��
 *        �񍐂��邱�ƁD
 *  (4) �{�\�t�g�E�F�A�̗��p�ɂ�蒼�ړI�܂��͊ԐړI�ɐ����邢���Ȃ鑹
 *      �Q������C��L���쌠�҂����TOPPERS�v���W�F�N�g��Ɛӂ��邱�ƁD
 *      �܂��C�{�\�t�g�E�F�A�̃��[�U�܂��̓G���h���[�U����̂����Ȃ闝
 *      �R�Ɋ�Â�����������C��L���쌠�҂����TOPPERS�v���W�F�N�g��
 *      �Ɛӂ��邱�ƁD
 * 
 *  �{�\�t�g�E�F�A�́C���ۏ؂Œ񋟂���Ă�����̂ł���D��L���쌠�҂�
 *  ���TOPPERS�v���W�F�N�g�́C�{�\�t�g�E�F�A�Ɋւ��āC����̎g�p�ړI
 *  �ɑ΂���K�������܂߂āC�����Ȃ�ۏ؂��s��Ȃ��D�܂��C�{�\�t�g�E�F
 *  �A�̗��p�ɂ�蒼�ړI�܂��͊ԐړI�ɐ����������Ȃ鑹�Q�Ɋւ��Ă��C��
 *  �̐ӔC�𕉂�Ȃ��D
 * 
 *  @(#) $Id: ltdc.h 698 2017-02-24 18:25:32Z roi $
 */
/*
 * STM32F746 LTDC�p�f�o�C�X�h���C�o�̊O���錾
 */

#ifndef _LTDC_H_
#define _LTDC_H_

#ifdef __cplusplus
 extern "C" {
#endif

#define MAX_LAYER  2

/*
 *  LTDC_HS_POLARITY LTDC HS POLARITY
 */
#define LTDC_HSPOLARITY_AL       0x00000000		/* Horizontal Synchronization is active low. */
#define LTDC_HSPOLARITY_AH       LTDC_GCR_HSPOL	/* Horizontal Synchronization is active high. */

/*
 *  LTDC_VS_POLARITY LTDC VS POLARITY
 */
#define LTDC_VSPOLARITY_AL       0x00000000		/* Vertical Synchronization is active low. */
#define LTDC_VSPOLARITY_AH       LTDC_GCR_VSPOL	/* Vertical Synchronization is active high. */

/*
 *  LTDC_DE_POLARITY LTDC DE POLARITY
 */
#define LTDC_DEPOLARITY_AL       0x00000000		/* Data Enable, is active low. */
#define LTDC_DEPOLARITY_AH       LTDC_GCR_DEPOL	/* Data Enable, is active high. */

/*
 *  LTDC_BlendingFactor1 LTDC Blending Factor1
 */
#define LTDC_BLENDING_FACTOR1_CA    0x00000400	/* Blending factor : Cte Alpha */
#define LTDC_BLENDING_FACTOR1_PAxCA 0x00000600	/* Blending factor : Cte Alpha x Pixel Alpha*/

/*
 *  LTDC_BlendingFactor2 LTDC Blending Factor2
 */
#define LTDC_BLENDING_FACTOR2_CA    0x00000005	/* Blending factor : Cte Alpha */
#define LTDC_BLENDING_FACTOR2_PAxCA 0x00000007	/* Blending factor : Cte Alpha x Pixel Alpha*/

/*
 *  LTDC_PC_POLARITY LTDC PC POLARITY
 */
#define LTDC_PCPOLARITY_IPC      0x00000000		/* input pixel clock. */
#define LTDC_PCPOLARITY_IIPC     LTDC_GCR_PCPOL	/* inverted input pixel clock. */

/*
 *  LTDC�s�N�Z���t�H�[�}�b�g��`
 */
#define LTDC_PIXEL_FORMAT_ARGB8888  0x00000000	/* ARGB8888 LTDC pixel format */
#define LTDC_PIXEL_FORMAT_RGB888    0x00000001	/* RGB888 LTDC pixel format   */
#define LTDC_PIXEL_FORMAT_RGB565    0x00000002	/* RGB565 LTDC pixel format   */
#define LTDC_PIXEL_FORMAT_ARGB1555  0x00000003	/* ARGB1555 LTDC pixel format */
#define LTDC_PIXEL_FORMAT_ARGB4444  0x00000004	/* ARGB4444 LTDC pixel format */
#define LTDC_PIXEL_FORMAT_L8        0x00000005	/* L8 LTDC pixel format       */
#define LTDC_PIXEL_FORMAT_AL44      0x00000006	/* AL44 LTDC pixel format     */
#define LTDC_PIXEL_FORMAT_AL88      0x00000007	/* AL88 LTDC pixel format     */

/*
 *  LTDC��Ԓ�`
 */
#define LTDC_STATE_RESET         0x00000000		/* LTDC ���Z�b�g��� */
#define LTDC_STATE_READY         0x00000001		/* LTDC ���f�B��� */
#define LTDC_STATE_BUSY          0x00000002		/* LTDC �r�W�B��� */
#define LTDC_STATE_ERROR         0x00010000		/* LTDC �G���[��� */

/*
 *  LTDC�G���[��`
 */
#define LTDC_ERROR_NONE          0x00000000		/* LTDC �G���[�Ȃ� */
#define LTDC_ERROR_TE            0x00000001		/* LTDC �]���G���[ */
#define LTDC_ERROR_FU            0x00000002		/* LTDC FIFO�A���_�[���� */


/*
 *  LTDC�J���[�\���̒�`
 */
typedef struct
{
	uint8_t	    Blue;			/* Configures the blue value. */
	uint8_t     Green;			/* Configures the green value. */
	uint8_t     Red;			/* Configures the red value. */
	uint8_t     Reserved;		/* Reserved 0xFF */
} Color_t;

/*
 *  LTDC�������\���̒�`
 */
typedef struct
{
	uint32_t    HSPolarity;		/* configures the horizontal synchronization polarity. */
	uint32_t    VSPolarity;		/* configures the vertical synchronization polarity. */
	uint32_t    DEPolarity;		/* configures the data enable polarity. */
	uint32_t    PCPolarity;		/* configures the pixel clock polarity. */
	uint32_t    HorizontalSync;	/* configures the number of Horizontal synchronization width. */
	uint32_t    VerticalSync;	/* configures the number of Vertical synchronization height. */
	uint32_t    AccumulatedHBP;	/* configures the accumulated horizontal back porch width. */
	uint32_t    AccumulatedVBP;	/* configures the accumulated vertical back porch height. */
	uint32_t    AccumulatedActiveW;	/* configures the accumulated active width. */
	uint32_t    AccumulatedActiveH;	/* configures the accumulated active height. */
	uint32_t    TotalWidth;		/* configures the total width. */
	uint32_t    TotalHeigh;		/* configures the total height. */
	Color_t     Backcolor;		/* configures the background color. */
	uint32_t    pllsain;		/* configures the PLLSAIN */
	uint32_t    pllsair;		/* configures the PLLSAIR */
	uint32_t    saidivr;		/* configures the PLLSAIDIVR */
} LTDC_Init_t;

/*
 *  LTDC���C���[�\���̒�`
 */
typedef struct
{
	uint32_t    WindowX0;		/* Configures the Window Horizontal Start Position. */
	uint32_t    WindowX1;		/* Configures the Window Horizontal Stop Position. */
	uint32_t    WindowY0;		/* Configures the Window vertical Start Position. */
	uint32_t    WindowY1;		/* Configures the Window vertical Stop Position. */
	uint32_t    PixelFormat;	/* Specifies the pixel format. */
	uint32_t    Alpha;			/* Specifies the constant alpha used for blending. */
	uint32_t    Alpha0;			/* Configures the default alpha value. */
	uint32_t    BlendingFactor1;/* Select the blending factor 1. */
	uint32_t    BlendingFactor2;/* Select the blending factor 2. */
	uint32_t    FBStartAdress;	/* Configures the color frame buffer address */
	uint32_t    ImageWidth;		/* Configures the color frame buffer line length. */
	uint32_t    ImageHeight;	/* Specifies the number of line in frame buffer. */
	Color_t     Backcolor;		/* Configures the layer background color. */
} LTDC_LayerCfg_t;

/*
 *  LTDC�n���h���\���̒�`
 */
typedef struct _LTDC_Handle_t LTDC_Handle_t;
struct _LTDC_Handle_t {
	uint32_t           base;				/* LTDC �x�[�X���W�X�^ */
	LTDC_Init_t        Init;				/* LTDC parameters */
	LTDC_LayerCfg_t LayerCfg[MAX_LAYER];	/* LTDC Layers parameters */
	void               (*linecallback)(LTDC_Handle_t * hltdc);	/* ���C���R�[���o�b�N�֐� */
	void               (*reloadcallback)(LTDC_Handle_t * hltdc);/* �����[�h�R�[���o�b�N�֐� */
	void               (*errorcallback)(LTDC_Handle_t * hltdc);	/* �G���[�R�[���o�b�N�֐� */
	volatile uint32_t  state;				/* LTDC ���s��� */
	volatile uint32_t  errorcode;			/* LTDC �G���[�R�[�h */
};

extern ER ltdc_init(LTDC_Handle_t *phandle);
extern ER ltdc_configlayer(LTDC_Handle_t *hltdc, LTDC_LayerCfg_t *pLayerCfg, uint32_t LayerIdx);
extern ER ltdc_setwindowsize(LTDC_Handle_t *hltdc, uint32_t XSize, uint32_t YSize, uint32_t LayerIdx);
extern ER ltdc_setwindowposition(LTDC_Handle_t *hltdc, uint32_t X0, uint32_t Y0, uint32_t LayerIdx);
extern ER ltdc_setalpha(LTDC_Handle_t *hltdc, uint32_t Alpha, uint32_t LayerIdx);
extern ER ltdc_setaddress(LTDC_Handle_t *hltdc, uint32_t Address, uint32_t LayerIdx);
extern ER ltdc_configcolorkeying(LTDC_Handle_t *hltdc, uint32_t RGBValue, uint32_t LayerIdx);
extern ER ltdc_enablecolorkeying(LTDC_Handle_t *hltdc, uint32_t LayerIdx);
extern ER ltdc_disablecolorkeying(LTDC_Handle_t *hltdc, uint32_t LayerIdx);
extern ER ltdc_lineevent(LTDC_Handle_t *phandle, uint32_t Line);
extern ER ltdc_reload(LTDC_Handle_t *phandle, uint32_t ReloadType);
extern void ltdc_irqhandler(LTDC_Handle_t *phandle);

#ifdef __cplusplus
}
#endif

#endif

