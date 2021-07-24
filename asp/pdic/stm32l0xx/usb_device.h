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
 *  @(#) $Id: usb_device.h 698 2017-11-07 22:49:05Z roi $
 */
/*
 * STM USB DEVICE�p�f�o�C�X�h���C�o�̊O���錾
 */

#ifndef _USB_DEVICE_H_
#define _USB_DEVICE_H_

#ifdef __cplusplus
 extern "C" {
#endif

/*
 *  USB�|�[�g��`
 */
#define USB1_PORTID             1
#define NUM_USBPORT             1

/*
 *  �G���h�|�C���g�E�f�o�C�X�`���l���̍ő吔�{�P
 */
#define MAX_DEVICE_EPS          8
#define MAX_EPS_CHANNELS        (MAX_DEVICE_EPS+1)

/*
 *  USB PHY��`
 */
#define USB_PHY_ULPI            1
#define USB_PHY_EMBEDDED        2

/*
 *  �G���h�|�C���g�^�C�v
 */
#define EP_TYPE_CTRL            0
#define EP_TYPE_ISOC            1
#define EP_TYPE_BULK            2
#define EP_TYPE_INTR            3
#define EP_TYPE_MSK             3

/*
 *  USB�X�s�[�h
 */
#define USB_SPEED_HIGH          0
#define USB_SPEED_HIGH_IN_FULL  1
#define USB_SPEED_LOW           2
#define USB_SPEED_FULL          3

/*
 *  LPM�A�N�e�B�u�p�����[�^
 */
#define PCD_LPM_L0_ACTIVE       0x00		/* on */
#define PCD_LPM_L1_ACTIVE       0x01		/* LPM L1 sleep */
#define PCD_LPM_L2              0x02		/* suspend */
#define PCD_LPM_L3              0x03		/* off */

/*
 *  �G���h�|�C���g���
 */
#define PCD_SNG_BUF             0U
#define PCD_DBL_BUF             1U

/*
 *  BCD�C�x���g��`
 */
#define USBD_BCD_CONTACT_DETECTION          0
#define USBD_BCD_STD_DOWNSTREAM_PORT        2
#define USBD_BCD_CHARGING_DOWNSTREAM_PORT   3
#define USBD_BCD_DEDICATED_CHARGING_PORT    4
#define USBD_BCD_DISCOVERY_COMPLETED        1
#define USBD_BCD_ERROR                      -1


/*
 *  USB�f�o�C�X�����ݒ��`
 */
typedef struct
{
	uint32_t              dev_endpoints;			/* �f�o�C�X�G���h�|�C���g��(1-8) */
	uint32_t              speed;					/* USB�R�A�X�s�[�h */
	uint32_t              phy_itface;				/* PHY�C���^�[�t�F�C�X�I�� */
	uint32_t              sof_enable;				/* SOF�����݂̗L�������ݒ�(0 or 1) */
    uint32_t              low_power_enable;			/* LOW POWER���[�h�L�������ݒ�(0 or 1) */
    uint32_t              lpm_enable;				/* Link Power Management�L�������ݒ�(0 or 1) */
	uint32_t              battery_charging_enable;	/* �o�b�e���`���[�W�L�������ݒ�(0 or 1) */ 
}USB_DEV_Init_t;

/*
 *  �G���h�|�C���g��`
 */
typedef struct
{
    uint8_t               num;					/* �G���h�|�C���g�ԍ�(0-7) */
	uint8_t               is_in : 1;			/* �G���h�|�C���g�X�`�[�����(0 or 1) */
	uint8_t               is_stall : 1;			/* �G���h�|�C���g�X�`�[�����(0 or 1) */
	uint8_t               type : 2;				/* �G���h�|�C���g�^�C�v */
	uint8_t               doublebuffer : 1;		/* �G���h�|�C���g�F�_�u���o�b�t�@�w�� */
	uint16_t              pmaadress;			/* PMA Address */
	uint16_t              pmaaddr0;				/* PMA Address0 */
	uint16_t              pmaaddr1;				/* PMA Address1 */
    uint16_t              maxpacket;			/* �G���h�|�C���g�ő�p�P�b�g�T�C�Y(0-64KB) */
	uint8_t               *xfer_buff;			/* �]���o�b�t�@�|�C���^�[ */
	uint32_t              xfer_len;				/* ���݂̓]���� */
	uint32_t              xfer_count;			/* �w��]���J�E���g */
}USB_DEV_EPTypeDef;

typedef struct _USB_DEV_Handle USB_DEV_Handle_t;

/*
 *   USB�f�o�C�X�n���h����`
 */
struct _USB_DEV_Handle
{
	uint32_t              base;						/* USB DEVICE ���W�X�^�x�[�X�A�h���X */
	USB_DEV_Init_t        Init;						/* USB DEVICE �����ݒ� */
	USB_DEV_EPTypeDef     IN_ep[MAX_DEVICE_EPS];	/* IN �G���h�|�C���g�f�[�^ */
	USB_DEV_EPTypeDef     OUT_ep[MAX_DEVICE_EPS];	/* OUT �G���h�|�C���g�f�[�^ */
	volatile uint32_t     Setup[12];				/* Setup �p�P�b�g�o�b�t�@ */
	uint32_t              BESL;						/* BESL�ۑ��̈� */
	uint8_t               lpm_state;				/* LPM ��� */
	uint8_t               lpm_active;				/* Link Power Management�L�������ݒ�(0 or 1) */
	uint8_t               battery_charging_active;	/* Enable or disable Battery charging */
	volatile uint8_t      usb_address;				/* USB Address */
	void                  (*devsetupstagecallback)(USB_DEV_Handle_t * hhcd);
	void                  (*devdataoutstagecallback)(USB_DEV_Handle_t * hhcd, uint8_t epnum);
	void                  (*devdatainstagecallback)(USB_DEV_Handle_t * hhcd, uint8_t epnum);
	void                  (*devsofcallback)(USB_DEV_Handle_t * hhcd);
	void                  (*devresetcallback)(USB_DEV_Handle_t * hhcd);
	void                  (*devsuspendcallback)(USB_DEV_Handle_t * hhcd);
	void                  (*devresumecallback)(USB_DEV_Handle_t * hhcd);
	void                  (*devconnectcallback)(USB_DEV_Handle_t * hhcd);					/* */
	void                  (*devdisconnectcallback)(USB_DEV_Handle_t * hhcd);				/* */
	void                  (*devlpmcallback)(USB_DEV_Handle_t * hhcd, uint8_t msg);
	void                  (*devbcdcallback)(USB_DEV_Handle_t * hhcd, int8_t msg);
	void                  *pDev;					/* Pointer Device Stack Handler */
};

extern ER usbd_setupPMA(USB_DEV_Handle_t *husb, uint16_t ep_addr, uint16_t kind, uint32_t pmaadress);
extern ER usbd_init_lpm(USB_DEV_Handle_t *husb);
extern ER usbd_init_BCD(USB_DEV_Handle_t *husb);
extern void usbd_bcd_task(intptr_t exinf);

extern USB_DEV_Handle_t *usbd_init(int id, USB_DEV_Init_t *pini);
extern ER usbd_deinit (USB_DEV_Handle_t *husb);
extern ER usbd_devconnect(USB_DEV_Handle_t *husb);
extern ER usbd_devdisconnect(USB_DEV_Handle_t *husb);
extern ER usbd_setDevAddress(USB_DEV_Handle_t *husb, uint8_t address);
extern ER usbd_activateEndpoint(USB_DEV_Handle_t *husb, USB_DEV_EPTypeDef *ep);
extern ER usbd_deactivateEndpoint(USB_DEV_Handle_t *husb, USB_DEV_EPTypeDef *ep);
extern ER usbd_epstartreceive(USB_DEV_Handle_t *husb, USB_DEV_EPTypeDef *ep);
extern ER usbd_epstartsend(USB_DEV_Handle_t *husb, USB_DEV_EPTypeDef *ep);
extern ER usbd_epsetStall(USB_DEV_Handle_t *husb, USB_DEV_EPTypeDef *ep);
extern ER usbd_epclearStall(USB_DEV_Handle_t *husb, USB_DEV_EPTypeDef *ep);
extern void usb_device_isr(intptr_t exinf);


#ifdef __cplusplus
}
#endif

#endif

