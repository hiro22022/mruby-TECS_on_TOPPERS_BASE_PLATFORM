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
 *  @(#) $Id: usb_device.c 698 2017-11-07 22:49:28Z roi $
 */
/*
 * STM USB DEVICE�p�f�o�C�X�h���C�o
 */

#include <kernel.h>
#include <t_syslog.h>
#include <t_stdlib.h>
#include <string.h>
#include <sil.h>
#include "t_syslog.h"
#include "device.h"
#include "stm32l0xx.h"
#include "usb_device.h"

/*
 *  SIL�֐��̃}�N����`
 */
#define sil_orw_mem(a, b)		sil_wrw_mem((a), sil_rew_mem(a) | (b))
#define sil_andw_mem(a, b)		sil_wrw_mem((a), sil_rew_mem(a) & ~(b))
#define sil_orh_mem(a, b)		sil_wrh_mem((a), sil_reh_mem(a) | (b))
#define sil_andh_mem(a, b)		sil_wrh_mem((a), sil_reh_mem(a) & ~(b))

/*
 *  USB�|�[�gID����Ǘ��u���b�N�����o�����߂̃}�N��
 */
#define INDEX_USB(usbid)        ((uint_t)((usbid) - 1))


#define BTABLE_ADDRESS                  (0x000U)  

#define EP0                             ((uint8_t)0)

#define USBD_DEFAULT_IMASK (USB_CNTR_CTRM  | USB_CNTR_WKUPM | USB_CNTR_SUSPM | USB_CNTR_ERRM \
							| USB_CNTR_ESOFM | USB_CNTR_RESETM)
#define USBD_DTORR_VALUE   (USB_EP_CTR_RX | USB_EP_CTR_TX | USB_EP_DTOG_RX)
#define USBD_DTORT_VALUE   (USB_EP_CTR_RX | USB_EP_CTR_TX | USB_EP_DTOG_TX)

#define USB_OTG_FS_WAKEUP_EXTI_LINE     (EXTI_IMR_MR18)	/* External interrupt line 18 Connected to the USB FS EXTI Line */


/*
 *  �G���h�|�C���g�E�A�h���X��o���}�N��
 */
#define GET_ENDPOINT_ADDR(b, bEpNum)            ((b)+TOFF_USB_EP0R + (bEpNum) * 4)
#define GET_ENDPOINT_TX_ADDRESS(b, bEpNum)      ((uint16_t *)((sil_reh_mem((uint16_t *)((b)+TOFF_USB_BTABLE))+bEpNum*8U)+  ((b) + 0x400U)))
#define GET_ENDPOINT_TX_CNT_ADDRESS(b, bEpNum)  ((uint16_t *)((sil_reh_mem((uint16_t *)((b)+TOFF_USB_BTABLE))+bEpNum*8U+2U)+  ((b) + 0x400U)))
#define GET_ENDPOINT_RX_ADDRESS(b, bEpNum)      ((uint16_t *)((sil_reh_mem((uint16_t *)((b)+TOFF_USB_BTABLE))+bEpNum*8U+4U)+  ((b) + 0x400U)))
#define GET_ENDPOINT_RX_CNT_ADDRESS(b, bEpNum)  ((uint16_t *)((sil_reh_mem((uint16_t *)((b)+TOFF_USB_BTABLE))+bEpNum*8U+6U)+  ((b) + 0x400U)))


static const uint16_t ep_type_table[4] = {USB_EP_CONTROL, USB_EP_ISOCHRONOUS, USB_EP_BULK, USB_EP_INTERRUPT};
static USB_DEV_Handle_t Husb[NUM_USBPORT];

/*
 *  �_�u���o�b�t�@�E���[�U�[�o�b�t�@���
 */
static void
usbd_freeuserbuffer(volatile uint16_t *pendp, uint8_t is_in)
{
	if(is_in)	/* IN �_�u���E�o�b�t�@�G���h�|�C���g */
		*pendp = (*pendp & USB_EPREG_MASK) | USBD_DTORR_VALUE;
	else	/* OUT �_�u���E�o�b�t�@�G���h�|�C���g */
		*pendp = (*pendp & USB_EPREG_MASK) | USBD_DTORT_VALUE;
}

/*
 *  ���MDTOG�g�O��
 */
static void
usbd_toggle_txdtog(volatile uint16_t *pendp, uint16_t valid)
{
	uint16_t value = *pendp & USB_EPTX_DTOGMASK;
	if((USB_EPTX_DTOG1 & valid) != 0)	/* 1st�r�b�g�g�O�� ? */
		value ^= USB_EPTX_DTOG1;
	if((USB_EPTX_DTOG2 & valid) != 0)	/* 2nd�r�b�g�g�O�� ?  */
		value ^= USB_EPTX_DTOG2;
	*pendp = (value | USB_EP_CTR_RX | USB_EP_CTR_TX);
}

/*
 *  ��MDTOG�g�O��
 */
static void
usbd_toggle_rxdtog(volatile uint16_t *pendp, uint16_t valid)
{
	uint16_t value = *pendp & USB_EPRX_DTOGMASK;
	if((USB_EPRX_DTOG1 & valid) != 0)	/* 1st�r�b�g�g�O�� ? */
		value ^= USB_EPRX_DTOG1;
	if((USB_EPRX_DTOG2 & valid) != 0)	/* 2nd�r�b�g�g�O�� ?  */
		value ^= USB_EPRX_DTOG2;
	*pendp = (value | USB_EP_CTR_RX | USB_EP_CTR_TX);
}

/*
 *  ��M�o�b�t�@�J�E���^�ݒ�
 */
static void
usbd_calc_blk(uint16_t *dwReg, uint16_t len)
{
	uint16_t wNBlocks;

	if(len > 62){
		wNBlocks = len >> 5;
		if((len & 0x1f) == 0)
			wNBlocks--;
		*dwReg = (uint16_t)((wNBlocks << 10U) | 0x8000U);
	}
	else{
		wNBlocks = len >> 1;
		if((len & 0x1) != 0)
			wNBlocks++;
	    *dwReg = (uint16_t)(wNBlocks << 10U);
	}
}

/*
 *  PMA�������ւ̃f�[�^������
 */
static void
usbd_writePMA(uint32_t base, uint8_t *src, uint16_t addr, uint16_t len)
{
	uint32_t n = (len + 1) >> 1;
	uint32_t i;
	uint16_t *p = (uint16_t *)(addr + base + 0x400U);

	for(i = 0; i < n ; i++, src += 2)
		*p++ = src[0] | (src[1] << 8);
}

/*
 *  PMA����������̃f�[�^�ǂݏo��
 */
static void
usbd_readPMA(uint32_t base, uint8_t *dst, uint16_t wPMABufAddr, uint16_t len)
{
	uint32_t n = (len + 1) >> 1;
	uint32_t i;
	uint16_t *p, tmp;
	p = (uint16_t *)(wPMABufAddr + base + 0x400U);
	for(i = 0 ; i < n ; i++){
		tmp = *p++;
		*dst++ = tmp & 0xff;
		*dst++ = tmp >> 8;
	}
}


/*
 *  USB DEVICE�n�[�h�E�F�A������
 *  parameter1 portid USB�|�[�gID
 *  parameter2 pini   �������\���̂ւ̃|�C���^
 *  return     NULL�łȂ����USB�n���h���ւ̃|�C���^
 */
USB_DEV_Handle_t *
usbd_init(int portid, USB_DEV_Init_t *pini)
{
	GPIO_Init_t GPIO_Init_Data;
	USB_DEV_Handle_t *husb;
	uint32_t         no;
	uint16_t         imask = USBD_DEFAULT_IMASK;

	if(portid < USB1_PORTID || portid > NUM_USBPORT || pini == NULL)
		return NULL;

	no = INDEX_USB(portid);
	husb = &Husb[no];
	memset(husb, 0, sizeof(USB_DEV_Handle_t));

	/*
	 *  GPIOA�N���b�N����
	 */
	sil_orw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_IOPENR), RCC_IOPENR_GPIOAEN);

	/*
	 *  USB DM/DP�s���ݒ�
	 */
	GPIO_Init_Data.mode      = GPIO_MODE_INPUT;
	/* �v���A�b�v �v���_�E���ݒ� */
	GPIO_Init_Data.pull      = GPIO_NOPULL;
	/* �o�̓��[�h�ݒ� */
	GPIO_Init_Data.otype     = GPIO_OTYPE_PP;
	/* �X�s�[�h�ݒ� */
	GPIO_Init_Data.speed     = GPIO_SPEED_HIGH;
	gpio_setup(TADR_GPIOA_BASE, &GPIO_Init_Data, 11);
	gpio_setup(TADR_GPIOA_BASE, &GPIO_Init_Data, 12);

	/*
	 *  USB DEVICE�N���b�N����
	 */
	sil_orw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_APB1ENR), RCC_APB1ENR_USBEN);

	memcpy(&husb->Init, pini, sizeof(USB_DEV_Init_t));
	if(pini->low_power_enable == 1){	/* Enable EXTI Line 18 for USB wakeup */
		sil_wrw_mem((uint32_t *)(TADR_EXTI_BASE+TOFF_EXTI_PR), USB_OTG_FS_WAKEUP_EXTI_LINE);
		sil_andw_mem((uint32_t *)(TADR_EXTI_BASE+TOFF_EXTI_FTSR), USB_OTG_FS_WAKEUP_EXTI_LINE);
		sil_orw_mem((uint32_t *)(TADR_EXTI_BASE+TOFF_EXTI_RTSR), USB_OTG_FS_WAKEUP_EXTI_LINE);
		sil_orw_mem((uint32_t *)(TADR_EXTI_BASE+TOFF_EXTI_IMR), USB_OTG_FS_WAKEUP_EXTI_LINE);
	}

	husb->base = TADR_USB_BASE;

	/*
	 *  �f�o�C�X������
	 */
	sil_wrh_mem((uint16_t *)(husb->base+TOFF_USB_CNTR), USB_CNTR_FRES);
	sil_wrh_mem((uint16_t *)(husb->base+TOFF_USB_CNTR), 0x0000);
	sil_wrh_mem((uint16_t *)(husb->base+TOFF_USB_ISTR), 0x0000);
	sil_wrh_mem((uint16_t *)(husb->base+TOFF_USB_BTABLE), BTABLE_ADDRESS);

	/*
	 *  �����ݐݒ�
	 */
	if(pini->sof_enable == 1)
		imask |= USB_CNTR_SOFM;
	sil_wrh_mem((uint16_t *)(husb->base+TOFF_USB_CNTR), USBD_DEFAULT_IMASK);

	husb->usb_address = 0;
	return husb;
}

/*
 *  USB DEVICE�n�[�h�E�F�A��~
 *  parameter1 husb  USB�n���h���ւ̃|�C���^
 *  return     ���E_OK
 */
ER
usbd_deinit(USB_DEV_Handle_t *husb)
{
	/*
	 *  �n���h���̊m�F
	 */
	if(husb == NULL){
		return E_PAR;
	}

	/*
	 *  USB FS �N���b�N��~
	 */
	sil_andw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_APB1ENR), RCC_APB1ENR_USBEN);
	return E_OK;
}


/*
 *  �f�o�C�X�R�l�N�g
 *  parameter1 husb  USB�n���h���ւ̃|�C���^
 *  return     ���E_OK
 */
ER
usbd_devconnect(USB_DEV_Handle_t *husb)
{
	sil_wrh_mem((uint16_t *)(husb->base+TOFF_USB_BCDR), USB_BCDR_DPPU);
	return E_OK;
}

/*
 *  �f�o�C�X�f�B�X�R�l�N�g
 *  parameter1 husb  USB�n���h���ւ̃|�C���^
 *  return     ���E_OK
 */
ER
usbd_devdisconnect(USB_DEV_Handle_t *husb)
{
	/*
	 *  USB DEVICE���Z�b�g
	 */
	sil_wrh_mem((uint16_t *)(husb->base+TOFF_USB_CNTR), USB_CNTR_FRES);

	/*
	 *  �����݃N���A
	 */
	sil_wrh_mem((uint16_t *)(husb->base+TOFF_USB_ISTR), 0);

	/*
	 *  POWER DOWN
	 */
	sil_wrh_mem((uint16_t *)(husb->base+TOFF_USB_CNTR), (USB_CNTR_FRES | USB_CNTR_PDWN));
	return E_OK;
}

/*
 *  �G���h�|�C���g�p�����݃n���h��
 */
static ER
usbd_endpoint_handler(USB_DEV_Handle_t *husb)
{
	USB_DEV_EPTypeDef *ep;
	uint8_t  num;
	volatile uint16_t istr;
	volatile uint16_t *pEndP;

	while(((istr = sil_reh_mem((uint16_t *)(husb->base+TOFF_USB_ISTR))) & USB_ISTR_CTR) != 0){
		/*
		 *  �G���h�|�C���g�ԍ��擾
		 */
		if((num = (uint8_t)(istr & USB_ISTR_EP_ID)) == 0){	/* EP0 */
			pEndP = (volatile uint16_t *)GET_ENDPOINT_ADDR(husb->base, EP0);
			if((istr & USB_ISTR_DIR) == 0){	/* ep0_in */
				*pEndP = (*pEndP & ((~USB_EP_CTR_TX) /*0xFF7FU*/ & USB_EPREG_MASK));
				ep = &husb->IN_ep[0];
				ep->xfer_count = ((uint16_t)(*GET_ENDPOINT_TX_CNT_ADDRESS(husb->base, ep->num)) & 0x3ffU);
				ep->xfer_buff += ep->xfer_count;

				/* TX COMPLETE */
				if(husb->devdatainstagecallback != NULL)
					husb->devdatainstagecallback(husb, 0U);

				if(husb->usb_address > 0 && ep->xfer_len == 0){
					sil_wrh_mem((uint16_t *)(husb->base+TOFF_USB_DADDR), (husb->usb_address | USB_DADDR_EF));
					husb->usb_address = 0;
				}

			}
			else{	/* ep0_out */
				ep = &husb->OUT_ep[0];
				if((*pEndP & USB_EP_SETUP) != 0){	/* setup��M */
					ep->xfer_count = ((uint16_t)(*GET_ENDPOINT_RX_CNT_ADDRESS(husb->base, ep->num)) & 0x3ffU);
					usbd_readPMA(husb->base, (uint8_t*)husb->Setup ,ep->pmaadress , ep->xfer_count);
					*pEndP = (*pEndP & ((~USB_EP_CTR_RX) & USB_EPREG_MASK));

					/*
					 *  �Z�b�g�A�b�v�X�e�[�W�R�[���o�b�N
					 */
					if(husb->devsetupstagecallback != NULL)
						husb->devsetupstagecallback(husb);
				}
				else if((*pEndP & USB_EP_CTR_RX) != 0){	/* data��M */
					*pEndP = (*pEndP & ((~USB_EP_CTR_RX) & USB_EPREG_MASK));
					/*
					 *  ��M�f�[�^�擾
					 */
					ep->xfer_count = ((uint16_t)(*GET_ENDPOINT_RX_CNT_ADDRESS(husb->base, ep->num)) & 0x3ffU);
					if(ep->xfer_count != 0){
						usbd_readPMA(husb->base, ep->xfer_buff, ep->pmaadress, ep->xfer_count);
						ep->xfer_buff += ep->xfer_count;
					}

					/*
					 *  DATA OUT�X�e�[�W�R�[���o�b�N
					 */
					if(husb->devdataoutstagecallback != NULL)
						husb->devdataoutstagecallback(husb, 0);
					usbd_calc_blk(GET_ENDPOINT_RX_CNT_ADDRESS(husb->base, ep->num), ep->maxpacket);
					usbd_toggle_rxdtog(pEndP, USB_EP_RX_VALID);
				}
			}
		}
		else{	/* epn */
			uint16_t count=0;

			/*
			 *  �G���h�|�C���g�A�h���X�擾
			 */
			pEndP = (volatile uint16_t *)GET_ENDPOINT_ADDR(husb->base, num);
			if((*pEndP & USB_EP_CTR_RX) != 0){	/* epn ��M */
				ep = &husb->OUT_ep[num];
				/*
				 *  �����݃N���A
				 */
				*pEndP = (*pEndP & ((~USB_EP_CTR_RX) & USB_EPREG_MASK));

				/* OUT double Buffering*/
				if(ep->doublebuffer == 0){
					count = ((uint16_t)(*GET_ENDPOINT_RX_CNT_ADDRESS(husb->base, ep->num)) & 0x3ffU);
					if(count != 0)
						usbd_readPMA(husb->base, ep->xfer_buff, ep->pmaadress, count);
				}
				else{
					pEndP = (volatile uint16_t *)GET_ENDPOINT_ADDR(husb->base, ep->num);
					if((*pEndP & USB_EP_DTOG_RX) != 0){	/* BUF0�ǂݏo�� */
						count = ((uint16_t)(*GET_ENDPOINT_TX_CNT_ADDRESS(husb->base, ep->num)) & 0x3ffU);
						if(count != 0)
							usbd_readPMA(husb->base, ep->xfer_buff, ep->pmaaddr0, count);
					}
					else{	/* BUF1�ǂݏo�� */
						count = ((uint16_t)(*GET_ENDPOINT_RX_CNT_ADDRESS(husb->base, ep->num)) & 0x3ffU);
						if(count != 0)
							usbd_readPMA(husb->base, ep->xfer_buff, ep->pmaaddr1, count);
					}
					usbd_freeuserbuffer(pEndP, 0);
				}
				/*multi-packet on the NON control OUT endpoint*/
				ep->xfer_count += count;
				ep->xfer_buff  += count;

				if(ep->xfer_len == 0 || count < ep->maxpacket){
					/*
					 * ��M�I��
					 */
					if(husb->devdataoutstagecallback != NULL)
						husb->devdataoutstagecallback(husb, ep->num);
				}
				else{	/* ��M�ݒ� */
					ep->xfer_count = 0;
					ep->is_in = 0;
					usbd_epstartreceive(husb, ep);
				}
			}

			if((*pEndP & USB_EP_CTR_TX) != 0){	/* epn���M */
				ep = &husb->IN_ep[num];
				/*
				 * �����݃N���A
				 */
				*pEndP = (*pEndP & ((~USB_EP_CTR_TX) & USB_EPREG_MASK));

				if(ep->doublebuffer == 0){	/* �V���O���E�o�b�t�@ */
					ep->xfer_count = ((uint16_t)(*GET_ENDPOINT_TX_CNT_ADDRESS(husb->base, ep->num)) & 0x3ffU);
					if(ep->xfer_count != 0)
						usbd_writePMA(husb->base, ep->xfer_buff, ep->pmaadress, ep->xfer_count);
				}
				else{	/* �_�u���E�o�b�t�@ */
					if((*pEndP & USB_EP_DTOG_TX) != 0){	/* BUF0�ݒ� */
						ep->xfer_count = ((uint16_t)(*GET_ENDPOINT_TX_CNT_ADDRESS(husb->base, ep->num)) & 0x3ffU);
						if(ep->xfer_count != 0)
							usbd_writePMA(husb->base, ep->xfer_buff, ep->pmaaddr0, ep->xfer_count);
					}
					else{	/* BUF1�ݒ� */
						ep->xfer_count = ((uint16_t)(*GET_ENDPOINT_RX_CNT_ADDRESS(husb->base, ep->num)) & 0x3ffU);
						if(ep->xfer_count != 0)
							usbd_writePMA(husb->base, ep->xfer_buff, ep->pmaaddr1, ep->xfer_count);
					}
					usbd_freeuserbuffer(pEndP, 1);
				}
				/*
				 * ���M�����擾
				 */
				ep->xfer_count = ((uint16_t)(*GET_ENDPOINT_TX_CNT_ADDRESS(husb->base, ep->num)) & 0x3ffU);
				ep->xfer_buff += ep->xfer_count;
				if(ep->xfer_len == 0){	/* �]���I�� ? */
					if(husb->devdatainstagecallback != NULL)
						husb->devdatainstagecallback(husb, ep->num);
				}
				else{	/* ���̃f�[�^��ݒ� */
					ep->xfer_count = 0U;
					ep->is_in = 1;
					usbd_epstartsend(husb, ep);
				}
			}
		}
	}
	return E_OK;
}

/*
 *  USB-DEVICE�����݃T�[�r�X���[�`��
 */
void
usb_device_isr(intptr_t exinf)
{
	USB_DEV_Handle_t *husb = &Husb[INDEX_USB((uint32_t)exinf)];
	uint32_t mask = 0;
	uint32_t isr = sil_reh_mem((uint16_t *)(husb->base+TOFF_USB_ISTR));

	if((isr & USB_ISTR_CTR) != 0)	/* ENDPOINT������ */
		usbd_endpoint_handler(husb);

	isr = sil_reh_mem((uint16_t *)(husb->base+TOFF_USB_ISTR));
	if((isr & USB_ISTR_RESET) != 0){	/* RESET������ */
		sil_andh_mem((uint16_t *)(husb->base+TOFF_USB_ISTR), USB_ISTR_RESET);
		if(husb->devresetcallback != NULL)
			husb->devresetcallback(husb);
		usbd_setDevAddress(husb, 0U);
	}

	if((isr & USB_ISTR_PMAOVR) != 0){
		sil_andh_mem((uint16_t *)(husb->base+TOFF_USB_ISTR), USB_ISTR_PMAOVR);
	}

	if((isr & USB_ISTR_ERR) != 0){	/* ERROR������ */
		sil_andh_mem((uint16_t *)(husb->base+TOFF_USB_ISTR), USB_ISTR_ERR);
	}

	if((isr & USB_ISTR_WKUP) != 0){	/* RESUME������ */
		sil_andh_mem((uint16_t *)(husb->base+TOFF_USB_CNTR), USB_CNTR_LPMODE);

		/*
		 * USB DEVICE�����ݐݒ�
		 */
		mask = USBD_DEFAULT_IMASK;
		if(husb->Init.sof_enable == 1)
			mask |= USB_CNTR_SOFM;

		if(husb->Init.lpm_enable == 1){	/* LPM�L���ݒ� */
			mask |= USB_CNTR_L1REQM;
			husb->lpm_active = 1;
			husb->lpm_state = PCD_LPM_L0_ACTIVE;

			sil_orh_mem((uint16_t *)(husb->base+TOFF_USB_LPMCSR), USB_LPMCSR_LMPEN);
			sil_orh_mem((uint16_t *)(husb->base+TOFF_USB_LPMCSR), USB_LPMCSR_LPMACK);
		}
		sil_wrh_mem((uint16_t *)(husb->base+TOFF_USB_CNTR), mask);

		if(husb->devresumecallback != NULL)
			husb->devresumecallback(husb);

		sil_andh_mem((uint16_t *)(husb->base+TOFF_USB_ISTR), USB_ISTR_WKUP);
	}

	if((isr & USB_ISTR_SUSP) != 0){	/* SUSPEND������ */
		sil_andh_mem((uint16_t *)(husb->base+TOFF_USB_ISTR), USB_ISTR_SUSP);

		/* Force low-power mode in the macrocell */
		sil_orh_mem((uint16_t *)(husb->base+TOFF_USB_CNTR), USB_CNTR_FSUSP);
		sil_orh_mem((uint16_t *)(husb->base+TOFF_USB_CNTR), USB_CNTR_LPMODE);

		if((isr & USB_ISTR_WKUP) == 0){
			if(husb->devsuspendcallback != NULL)
				husb->devsuspendcallback(husb);
		}
	}
  
	if((isr & USB_ISTR_L1REQ) != 0){	/* LPM������ */
		sil_andh_mem((uint16_t *)(husb->base+TOFF_USB_ISTR), USB_ISTR_L1REQ);
		if( husb->lpm_state == PCD_LPM_L0_ACTIVE){
			/*
			 *  LPM�T�X�y���h�ݒ�
			 */
			sil_orh_mem((uint16_t *)(husb->base+TOFF_USB_CNTR), USB_CNTR_LPMODE);
			sil_orh_mem((uint16_t *)(husb->base+TOFF_USB_CNTR), USB_CNTR_FSUSP);

			husb->lpm_state = PCD_LPM_L1_ACTIVE;
			husb->BESL = (sil_reh_mem((uint16_t *)(husb->base+TOFF_USB_LPMCSR)) & USB_LPMCSR_BESL) >> 2;
			if(husb->devlpmcallback != NULL)
				husb->devlpmcallback(husb, PCD_LPM_L1_ACTIVE);
		}
		else{
			if(husb->devsuspendcallback != NULL)
				husb->devsuspendcallback(husb);
		}
	}

	if((isr & USB_ISTR_SOF) != 0){	/* SOF������ */
		sil_andh_mem((uint16_t *)(husb->base+TOFF_USB_ISTR), USB_ISTR_SOF);
		if(husb->devsofcallback != NULL)
			husb->devsofcallback(husb);
	}

	if((isr & USB_ISTR_ESOF) != 0){	/* ESOF������ */
		sil_andh_mem((uint16_t *)(husb->base+TOFF_USB_ISTR), USB_ISTR_ESOF);
	}
}


/*
 *  USB�Ƀf�o�C�X�A�h���X��ݒ�
 *  parameter1 husb  USB�n���h���ւ̃|�C���^
 *  parameter2 addree �f�o�C�X�A�h���X(0-255)
 *  return     ���펞E_OK
 */
ER
usbd_setDevAddress(USB_DEV_Handle_t *husb, uint8_t address)
{
	if(husb == NULL)
		return E_PAR;
	if(address == 0)	/* �A�h���X�[���Ȃ�C�l�[�u���ɐݒ� */
		sil_wrh_mem((uint16_t *)(husb->base+TOFF_USB_DADDR), USB_DADDR_EF);
	else
		husb->usb_address = address;
	return E_OK;
}

/*
 *  �A�N�e�B�u�G���h�|�C���g�ݒ�
 *  parameter1 husb  USB�n���h���ւ̃|�C���^
 *  parameter2 ep    �G���h�|�C���g�\���̂ւ̃|�C���^
 *  return     ���펞E_OK
 */
ER
usbd_activateEndpoint(USB_DEV_Handle_t *husb, USB_DEV_EPTypeDef *ep)
{
	uint16_t type = USB_EP_CONTROL;
	volatile uint16_t *pEndP;

	if(husb == NULL || ep == NULL)
		return E_PAR;
	pEndP = (volatile uint16_t *)GET_ENDPOINT_ADDR(husb->base, ep->num);

	/*
	 * �G���h�|�C���g�ݒ�
	 */
	type  = ep_type_table[ep->type & 3];
	type |= USB_EP_CTR_RX | USB_EP_CTR_TX | ep->num;
	*pEndP = (*pEndP & (~USB_EP_T_FIELD & USB_EPREG_MASK)) | type;

	if(ep->doublebuffer == 0){	/* �V���O���E�o�b�t�@�ݒ� */
		if(ep->is_in){	/* ���M�o�b�t�@�E�A�h���X�ݒ� */
			*GET_ENDPOINT_TX_ADDRESS(husb->base, ep->num) = ep->pmaadress & ~1;
			/*
			 * TX DTOG�N���A
			 */
			if((*pEndP & USB_EP_DTOG_TX) != 0)
				*pEndP = (*pEndP & USB_EPREG_MASK) | USBD_DTORT_VALUE;

			/*
			 *  ���MNAK�ݒ�
			 */
			usbd_toggle_txdtog(pEndP, USB_EP_TX_NAK);
		}
		else{	/* ��M�o�b�t�@�ݒ� */
			*GET_ENDPOINT_RX_ADDRESS(husb->base, ep->num) = ep->pmaadress & ~1;
			usbd_calc_blk(GET_ENDPOINT_RX_CNT_ADDRESS(husb->base, ep->num), ep->maxpacket);

			/*
			 * RX DTOG�N���A
			 */
			if((*pEndP & USB_EP_DTOG_RX) != 0)
				*pEndP = (*pEndP & USB_EPREG_MASK) | USBD_DTORR_VALUE;
			/*
			 *  ��M�ݒ�
			 */
			usbd_toggle_rxdtog(pEndP, USB_EP_RX_VALID);
		}
	}
	else{	/* �_�u���E�o�b�t�@�ݒ� */
		*pEndP = ((*pEndP & USB_EPREG_MASK) | (USB_EP_CTR_RX | USB_EP_CTR_TX | USB_EP_KIND));

		/*
		 * �o�b�t�@�E�A�h���X�ݒ�
		 */
		*GET_ENDPOINT_TX_ADDRESS(husb->base, ep->num) = ep->pmaaddr0 & ~1;
		*GET_ENDPOINT_RX_ADDRESS(husb->base, ep->num) = ep->pmaaddr1 & ~1;

		if(ep->is_in){	/* IN�G���h�|�C���g�ݒ� */
			/*
			 *  IN/OUT�p�̃f�[�^�g�O���ݒ�N���A
			 */
			if((*pEndP & USB_EP_DTOG_RX) != 0)
				*pEndP = (*pEndP & USB_EPREG_MASK) | USBD_DTORR_VALUE;
			if((*pEndP & USB_EP_DTOG_TX) != 0)
				*pEndP = (*pEndP & USB_EPREG_MASK) | USBD_DTORT_VALUE;
			*pEndP = (*pEndP & USB_EPREG_MASK) | USBD_DTORR_VALUE;

			usbd_toggle_txdtog(pEndP, USB_EP_TX_DIS);
			usbd_toggle_rxdtog(pEndP, USB_EP_RX_DIS);
		}
		else{	/* OUT�G���h�|�C���g�ݒ� */
			/*
			 *  IN/OUT�p�̃f�[�^�g�O���ݒ�N���A
			 */
			if((*pEndP & USB_EP_DTOG_RX) != 0)
				*pEndP = (*pEndP & USB_EPREG_MASK) | USBD_DTORR_VALUE;
			if((*pEndP & USB_EP_DTOG_TX) != 0)
				*pEndP = (*pEndP & USB_EPREG_MASK) | USBD_DTORT_VALUE;
			*pEndP = (*pEndP & USB_EPREG_MASK) | USBD_DTORT_VALUE;

			usbd_toggle_rxdtog(pEndP, USB_EP_RX_VALID);
			usbd_toggle_txdtog(pEndP, USB_EP_TX_DIS);
		}
	}
	return E_OK;
}


/*
 *  �f�B�X�A�N�e�B�u�G���h�|�C���g�ݒ�
 *  parameter1 husb  USB�n���h���ւ̃|�C���^
 *  parameter2 ep    �G���h�|�C���g�\���̂ւ̃|�C���^
 *  return     ���펞E_OK
 */
ER
usbd_deactivateEndpoint(USB_DEV_Handle_t *husb, USB_DEV_EPTypeDef *ep)
{
	volatile uint16_t *pEndP;

	if(husb == NULL || ep == NULL)
		return E_PAR;
	pEndP = (volatile uint16_t *)GET_ENDPOINT_ADDR(husb->base, ep->num);
	if(ep->doublebuffer == 0){
		if(ep->is_in){
			/*
			 * TX DTOG�N���A
			 */
			if((*pEndP & USB_EP_DTOG_TX) != 0)
				*pEndP = (*pEndP & USB_EPREG_MASK) | USBD_DTORT_VALUE;
			/*
			 * TX�G���h�|�C���g�𖳌���
			 */
			usbd_toggle_txdtog(pEndP, USB_EP_TX_DIS);
		}
		else{
			/*
			 * RX DTOG�N���A
			 */
			if((*pEndP & USB_EP_DTOG_RX) != 0)
				*pEndP = (*pEndP & USB_EPREG_MASK) | USBD_DTORR_VALUE;
			/*
			 * RX�G���h�|�C���g�𖳌���
			 */
			usbd_toggle_rxdtog(pEndP, USB_EP_RX_DIS);
		}
	}
	else{	/* �_�u���E�o�b�t�@�ݒ� */
		if(ep->is_in){	/* IN endpoint */
			/*
			 *  IN/OUT�f�[�^�g�O���r�b�g���N���A
			 */
			if((*pEndP & USB_EP_DTOG_RX) != 0)
				*pEndP = (*pEndP & USB_EPREG_MASK) | USBD_DTORR_VALUE;
			if((*pEndP & USB_EP_DTOG_TX) != 0)
				*pEndP = (*pEndP & USB_EPREG_MASK) | USBD_DTORT_VALUE;
			*pEndP = (*pEndP & USB_EPREG_MASK) | USBD_DTORR_VALUE;

			/*
			 * �G���h�|�C���g�𖳌���
			 */
			usbd_toggle_txdtog(pEndP, USB_EP_TX_DIS);
			usbd_toggle_rxdtog(pEndP, USB_EP_RX_DIS);
		}
		else{	/* OUT endpoint */
			/*
			 *  IN/OUT�f�[�^�g�O���r�b�g���N���A
			 */
			if((*pEndP & USB_EP_DTOG_RX) != 0)
				*pEndP = (*pEndP & USB_EPREG_MASK) | USBD_DTORR_VALUE;
			if((*pEndP & USB_EP_DTOG_TX) != 0)
				*pEndP = (*pEndP & USB_EPREG_MASK) | USBD_DTORT_VALUE;
			*pEndP = (*pEndP & USB_EPREG_MASK) | USBD_DTORT_VALUE;

			/*
			 * �G���h�|�C���g�𖳌���
			 */
			usbd_toggle_rxdtog(pEndP, USB_EP_RX_DIS);
			usbd_toggle_txdtog(pEndP, USB_EP_TX_DIS);
		}
	}
	return E_OK;
}


/*
 *  ��M�J�n�ݒ�
 *  parameter1 husb  USB�n���h���ւ̃|�C���^
 *  parameter2 ep    �G���h�|�C���g�\���̂ւ̃|�C���^
 *  return     ���펞E_OK
 */
ER
usbd_epstartreceive(USB_DEV_Handle_t *husb, USB_DEV_EPTypeDef *ep)
{
	uint32_t len;
	volatile uint16_t *pEndP;

	if(husb == NULL || ep == NULL)
		return E_PAR;
	pEndP = (volatile uint16_t *)GET_ENDPOINT_ADDR(husb->base, ep->num);
	/*
	 * �}���`�E�p�P�b�g����
	 */
	if(ep->xfer_len > ep->maxpacket){
		len = ep->maxpacket;
		ep->xfer_len -= len;
	}
	else{
		len = ep->xfer_len;
		ep->xfer_len = 0;
	}

	if(ep->doublebuffer == 0)	/* �V���O���E�o�b�t�@��M�J�E���g�ݒ� */
		usbd_calc_blk(GET_ENDPOINT_RX_CNT_ADDRESS(husb->base, ep->num), len);
	else{	/* �_�u���E�o�b�t�@��M�J�E���g�ݒ� */
		if(ep->is_in){	/* IN endpoint */
			*GET_ENDPOINT_TX_CNT_ADDRESS(husb->base, ep->num) = (uint32_t)len;
			*GET_ENDPOINT_RX_CNT_ADDRESS(husb->base, ep->num) = (uint32_t)len;
		}
		else{	/* OUT endpoint */
			usbd_calc_blk(GET_ENDPOINT_TX_CNT_ADDRESS(husb->base, ep->num), len);
			usbd_calc_blk(GET_ENDPOINT_RX_CNT_ADDRESS(husb->base, ep->num), len);
		}

	}
	usbd_toggle_rxdtog(pEndP, USB_EP_RX_VALID);
	return E_OK;
}

/*
 *  ���M�J�n�ݒ�
 *  parameter1 husb  USB�n���h���ւ̃|�C���^
 *  parameter2 ep    �G���h�|�C���g�\���̂ւ̃|�C���^
 *  return     ���펞E_OK
 */
ER
usbd_epstartsend(USB_DEV_Handle_t *husb, USB_DEV_EPTypeDef *ep)
{
	uint16_t pmabuffer = 0U;
	uint32_t len;
	volatile uint16_t *pEndP;

	if(husb == NULL || ep == NULL)
		return E_PAR;
	pEndP = (volatile uint16_t *)GET_ENDPOINT_ADDR(husb->base, ep->num);

	/*
	 * �}���`�E�p�P�b�g����
	 */
	if(ep->xfer_len > ep->maxpacket){
		len = ep->maxpacket;
		ep->xfer_len -= len;
	}
	else{
		len = ep->xfer_len;
		ep->xfer_len = 0;
	}

	if(ep->doublebuffer == 0){	/* �V���O���E�o�b�t�@�f�[�^������ */
		usbd_writePMA(husb->base, ep->xfer_buff, ep->pmaadress, len);
		*GET_ENDPOINT_TX_CNT_ADDRESS(husb->base, ep->num) = len;
	}
	else{	/* �_�u���E�o�b�t�@�f�[�^������ */
		if(ep->is_in){	/* IN endpoint */
			*GET_ENDPOINT_TX_CNT_ADDRESS(husb->base, ep->num) = (uint32_t)len;
			*GET_ENDPOINT_RX_CNT_ADDRESS(husb->base, ep->num) = (uint32_t)len;
		}
		else{	/* OUT endpoint */
			usbd_calc_blk(GET_ENDPOINT_TX_CNT_ADDRESS(husb->base, ep->num), len);
			usbd_calc_blk(GET_ENDPOINT_RX_CNT_ADDRESS(husb->base, ep->num), len);
		}

		/*
		 * �f�[�^�ݒ�
		 */
		if((*pEndP  & USB_EP_DTOG_TX) != 0)
			pmabuffer = ep->pmaaddr1;
		else
			pmabuffer = ep->pmaaddr0;

		usbd_writePMA(husb->base, ep->xfer_buff, pmabuffer, len);
		usbd_freeuserbuffer(pEndP, ep->is_in);
	}
	usbd_toggle_txdtog(pEndP, USB_EP_TX_VALID);
	return E_OK;
}

/*
 *  �G���h�|�C���g�ɃX�e�[����Ԃ�ݒ�
 *  parameter1 husb  USB�n���h���ւ̃|�C���^
 *  parameter2 ep    �G���h�|�C���g�\���̂ւ̃|�C���^
 *  return     ���펞E_OK
 */
ER
usbd_epsetStall(USB_DEV_Handle_t *husb, USB_DEV_EPTypeDef *ep)
{
	volatile uint16_t *pEndP;

	if(husb == NULL || ep == NULL)
		return E_PAR;
	pEndP = (volatile uint16_t *)GET_ENDPOINT_ADDR(husb->base, ep->num);
	if(ep->num == 0){
		usbd_toggle_rxdtog(pEndP, USB_EP_RX_STALL);
		usbd_toggle_txdtog(pEndP, USB_EP_TX_STALL);
	}
	else{
		if(ep->is_in)
			usbd_toggle_txdtog(pEndP, USB_EP_TX_STALL);
		else
			usbd_toggle_rxdtog(pEndP, USB_EP_RX_STALL);
	}
	return E_OK;
}

/*
 *  �G���h�|�C���g�̃X�e�[����Ԃ��N���A
 *  parameter1 husb  USB�n���h���ւ̃|�C���^
 *  parameter2 ep    �G���h�|�C���g�\���̂ւ̃|�C���^
 *  return     ���펞E_OK
 */
ER
usbd_epclearStall(USB_DEV_Handle_t *husb, USB_DEV_EPTypeDef *ep)
{
	volatile uint16_t *pEndP;

	if(husb == NULL || ep == NULL)
		return E_PAR;
	pEndP = (volatile uint16_t *)GET_ENDPOINT_ADDR(husb->base, ep->num);
	if(ep->is_in){
		/*
		 * TX DTOG�N���A
		 */
		if((*pEndP & USB_EP_DTOG_TX) != 0)
			*pEndP = (*pEndP & USB_EPREG_MASK) | USBD_DTORT_VALUE;
		usbd_toggle_txdtog(pEndP, USB_EP_TX_VALID);
	}
	else{
		/*
		 * RX DTOG�N���A
		 */
		if((*pEndP & USB_EP_DTOG_RX) != 0)
			*pEndP = (*pEndP & USB_EPREG_MASK) | USBD_DTORR_VALUE;
		usbd_toggle_rxdtog(pEndP, USB_EP_RX_VALID);
	}
	return E_OK;
}


/*
 *  PMA��ݒ肷��
 *  parameter1 husb      USB�n���h���ւ̃|�C���^
 *  parameter2 ep_addr   EP�A�h���X
 *  parameter3 kind      EP���(Signle/Double)
 *  parameter4 phmaddess PMA���̃X�^�[�g�ʒu
 *  return     ���E_OK
 */
ER
usbd_setupPMA(USB_DEV_Handle_t *husb, uint16_t ep_addr, uint16_t kind, uint32_t pmaadress)
{
	USB_DEV_EPTypeDef *ep;

	/* initialize ep structure*/
	if((ep_addr & 0x80) == 0x80)
		ep = &husb->IN_ep[ep_addr & 0x7F];
	else
		ep = &husb->OUT_ep[ep_addr];

	if(kind == PCD_SNG_BUF){	/* �V���O���E�o�b�t�@�ݒ� */
		ep->doublebuffer = 0;
		ep->pmaadress = (uint16_t)pmaadress;
	}
	else{	/* �_�u���E�o�b�t�@�ݒ� */
		ep->doublebuffer = 1;
		ep->pmaaddr0 =  pmaadress & 0xFFFF;
		ep->pmaaddr1 =  pmaadress >> 16;
	}
	return E_OK;
}

/*
 *  LPM�����ݒ�
 */
ER
usbd_init_lpm(USB_DEV_Handle_t *husb)
{
	if(husb->Init.lpm_enable == 1){
		husb->lpm_active = 1;
		husb->lpm_state = PCD_LPM_L0_ACTIVE;
		sil_orh_mem((uint16_t *)(husb->base+TOFF_USB_LPMCSR), USB_LPMCSR_LMPEN);
		sil_orh_mem((uint16_t *)(husb->base+TOFF_USB_LPMCSR), USB_LPMCSR_LPMACK);
	}
	return E_OK;
}

/*
 *  BCD�����ݒ�
 */
ER
usbd_init_BCD(USB_DEV_Handle_t *husb)
{
	if(husb->Init.battery_charging_enable == 1){
		husb->battery_charging_active = 1;
		sil_orh_mem((uint16_t *)(husb->base+TOFF_USB_BCDR), USB_BCDR_BCDEN);
		sil_orh_mem((uint16_t *)(husb->base+TOFF_USB_BCDR), USB_BCDR_DCDEN);
	}
	return E_OK;
}

/*
 *  BCD�e�X�g�^�X�N
 *  parameter1 exinf     USBID
 */
void
usbd_bcd_task(intptr_t exinf)
{
	USB_DEV_Handle_t *husb = &Husb[INDEX_USB((uint32_t)exinf)];
	uint32_t tick = 0;

 	/*
	 *  DETECT FLAG�Z�b�g�҂�
	 */
	while((sil_reh_mem((uint16_t *)(husb->base+TOFF_USB_BCDR)) & USB_BCDR_DCDET) == 0){
		if(tick++  > 1000){
			if(husb->devbcdcallback != NULL)
				husb->devbcdcallback(husb, USBD_BCD_ERROR);
			return;
		}
		dly_tsk(1);
	}
	dly_tsk(300);

	/*
	 * DETECT FLAG���|�[�g
	 */
	if((sil_reh_mem((uint16_t *)(husb->base+TOFF_USB_BCDR)) & USB_BCDR_DCDET) != 0){
		if(husb->devbcdcallback != NULL)
			husb->devbcdcallback(husb, USBD_BCD_CONTACT_DETECTION);
	}
	/*
	 * 1ST: STANDARD DOWNSTREAM PORT�`�F�b�N
	 */
	sil_andh_mem((uint16_t *)(husb->base+TOFF_USB_BCDR), USB_BCDR_DCDEN);
	sil_orh_mem((uint16_t *)(husb->base+TOFF_USB_BCDR), USB_BCDR_PDEN);
	dly_tsk(300);

	if((sil_reh_mem((uint16_t *)(husb->base+TOFF_USB_BCDR)) & USB_BCDR_PDET) != 0){
		/*
		 * 2ND: CHARGING OR DETECTED CHARGINE PORT�`�F�b�N
		 */
		sil_andh_mem((uint16_t *)(husb->base+TOFF_USB_BCDR), USB_BCDR_PDEN);
		sil_orh_mem((uint16_t *)(husb->base+TOFF_USB_BCDR), USB_BCDR_SDEN);
		dly_tsk(300);

		if((sil_reh_mem((uint16_t *)(husb->base+TOFF_USB_BCDR)) & USB_BCDR_SDET) != 0){
			/* DOWNSTREAM PORT DCP */
			if(husb->devbcdcallback != NULL)
				husb->devbcdcallback(husb, USBD_BCD_DEDICATED_CHARGING_PORT);
		}
		else{	/* CHARGINE DOWNSTREAM PORT CDP */
			if(husb->devbcdcallback != NULL)
				husb->devbcdcallback(husb, USBD_BCD_CHARGING_DOWNSTREAM_PORT);
		}
	}
	else{	/* Standard DOWNSTREAM PORT */
		if(husb->devbcdcallback != NULL)
			husb->devbcdcallback(husb, USBD_BCD_STD_DOWNSTREAM_PORT);
	}
	/*
	 * BATTERY SENSE�I��
	 */
	if(husb->devbcdcallback != NULL)
		husb->devbcdcallback(husb, USBD_BCD_DISCOVERY_COMPLETED);
}

