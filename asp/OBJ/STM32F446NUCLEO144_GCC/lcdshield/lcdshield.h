/*
 *  TOPPERS/ASP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Advanced Standard Profile Kernel
 *
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2004-2010 by Embedded and Real-Time Systems Laboratory
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
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
 *  $Id: lcdshield.h 2416 2012-09-07 08:06:20Z ertl-hiro $
 */

/*
 *  LCD�����������ץ����Υإå��ե�����
 */

/*
 *  �������åȰ�¸�����
 */
#include "target_test.h"
#include "usb_otg.h"
#include "tusbh_msc.h"
#include "fonts.h"

/*
 *  �ƥ�������ͥ���٤����
 */

#define MAIN_PRIORITY	9		/* �ᥤ�󥿥�����ͥ���� */
								/* HIGH_PRIORITY���⤯���뤳�� */

#define HIGH_PRIORITY	6		/* �¹Լ¹Ԥ���륿������ͥ���� */
#define MID_PRIORITY	10
#define LOW_PRIORITY	11

/*
 *  �������åȤ˰�¸�����ǽ���Τ�����������
 */

#ifndef TASK_PORTID
#define	TASK_PORTID		1			/* ʸ�����Ϥ��륷�ꥢ��ݡ���ID */
#endif /* TASK_PORTID */

#ifndef STACK_SIZE
#define	STACK_SIZE		4096		/* �������Υ����å������� */
#endif /* STACK_SIZE */

#define SPISDCARD_PORTID    0

#define INHNO_SPI   IRQ_VECTOR_SPI1	/* ����ߥϥ�ɥ��ֹ� */
#define INTNO_SPI   IRQ_VECTOR_SPI1	/* ������ֹ� */
#define INTPRI_SPI  -4          /* �����ͥ���� */
#define INTATR_SPI  TA_EDGE     /* �����°�� */

#define INHNO_DMARX   IRQ_VECTOR_DMA2_STREAM2	/* ����ߥϥ�ɥ��ֹ� */
#define INTNO_DMARX   IRQ_VECTOR_DMA2_STREAM2	/* ������ֹ� */
#define INTPRI_DMARX  -4          /* �����ͥ���� */
#define INTATR_DMARX  TA_EDGE     /* �����°�� */

#define INHNO_DMATX   IRQ_VECTOR_DMA2_STREAM3	/* ����ߥϥ�ɥ��ֹ� */
#define INTNO_DMATX   IRQ_VECTOR_DMA2_STREAM3	/* ������ֹ� */
#define INTPRI_DMATX  -4          /* �����ͥ���� */
#define INTATR_DMATX  TA_EDGE     /* �����°�� */

#define INHNO_ADC     IRQ_VECTOR_ADC	/* ����ߥϥ�ɥ��ֹ� */
#define INTNO_ADC     IRQ_VECTOR_ADC	/* ������ֹ� */
#define INTPRI_ADC    -5          /* �����ͥ���� */
#define INTATR_ADC    0           /* �����°�� */

#define INHNO_DMAADC  IRQ_VECTOR_DMA2_STREAM0	/* ����ߥϥ�ɥ��ֹ� */
#define INTNO_DMAADC  IRQ_VECTOR_DMA2_STREAM0	/* ������ֹ� */
#define INTPRI_DMAADC -4          /* �����ͥ���� */
#define INTATR_DMAADC TA_EDGE     /* �����°�� */

#define INHNO_USBFS   IRQ_VECTOR_OTG_FS	/* ����ߥϥ�ɥ��ֹ� */
#define INTNO_USBFS   IRQ_VECTOR_OTG_FS	/* ������ֹ� */
#define INTPRI_USBFS  -5			/* �����ͥ���� */
#define INTATR_USBFS  0				/* �����°�� */

#define INHNO_FSWKUP  IRQ_VECTOR_OTG_FS_WKUP	/* ����ߥϥ�ɥ��ֹ� */
#define INTNO_FSWKUP  IRQ_VECTOR_OTG_FS_WKUP	/* ������ֹ� */
#define INTPRI_FSWKUP -10			/* �����ͥ���� */
#define INTATR_FSWKUP 0				/* �����°�� */

/* Definition for ADCx's Channel */
#define ADCx_CHANNEL                    ADC_CHANNEL_9

/* Definition for ADCx Channel Pin */
#define ADCx_CHANNEL_GPIO_PORT          TADR_GPIOF_BASE
#define ADCx_CHANNEL_PINNO              3

#define NUM_USBH_EVT1   64
#define NUM_USBH_EVT2   16

#define NUM_JSPOSITION 6

#define JS_OFF      0x01
#define JS_ON       0x02
#define JS_UP       0x04
#define JS_DOWN     0x08
#define JS_LEFT     0x10
#define JS_RIGHT    0x20
#define JS_MASK     0x3F

#define CS_PORT     10
#define RST_PORT    9
#define RS_PORT     8
#define CS2_PORT    4

#define cs_set(sw)  digitalWrite(CS_PORT, sw)
#define rs_set(sw)  digitalWrite(RS_PORT, sw)
#define rst_set(sw) digitalWrite(RST_PORT, sw)
#define cs2_set(sw) digitalWrite(CS2_PORT, sw)

#ifndef TOPPERS_MACRO_ONLY

/*
 *  �饤��⡼�����
 */
typedef enum
{
	CENTER_MODE             = 0x01,    /* Center mode */
	RIGHT_MODE              = 0x02,    /* Right mode  */
	LEFT_MODE               = 0x03     /* Left mode   */
}Text_AlignModeTypdef;

/*
 *  �ҡ����ΰ������
 */
extern uint32_t heap_param[2];

/*
 *  �ؿ��Υץ�ȥ��������
 */
extern void lcd_DisplayChar(uint16_t x0, uint16_t y0, uint8_t Ascii);
extern void lcd_DisplayStringAt(uint16_t x0, uint16_t y0, char *Text, Text_AlignModeTypdef Mode);

extern void	main_task(intptr_t exinf);
extern void stick_task(intptr_t exinf);

extern void heap_init(intptr_t exinf);
extern void sw_int(void);
extern void device_info_init(intptr_t exinf);


#endif /* TOPPERS_MACRO_ONLY */
