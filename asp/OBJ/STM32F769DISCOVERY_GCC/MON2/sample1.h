/*
 *  TOPPERS/ASP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Advanced Standard Profile Kernel
 *
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2004-2010 by Embedded and Real-Time Systems Laboratory
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
 *  $Id: sample1.h 2416 2012-09-07 08:06:20Z ertl-hiro $
 */

/*
 *		����ץ�ץ����(1)�Υإå��ե�����
 */

/*
 *  �������åȰ�¸�����
 */
#include "target_test.h"

#include "device.h"
#include "i2c.h"

/*
 *  �ƥ�������ͥ���٤����
 */

#define MAIN_PRIORITY	5		/* �ᥤ�󥿥�����ͥ���� */
								/* HIGH_PRIORITY���⤯���뤳�� */

#define HIGH_PRIORITY	9		/* �¹Լ¹Ԥ���륿������ͥ���� */
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

#ifndef LOOP_REF
#define LOOP_REF		ULONG_C(1000000)	/* ®�ٷ�¬�ѤΥ롼�ײ�� */
#endif /* LOOP_REF */

#define INHNO_LTDC      IRQ_VECTOR_LTDC
#define INTNO_LTDC      IRQ_VECTOR_LTDC
#define INTPRI_LTDC     -7        /* �����ͥ���� */
#define INTATR_LTDC     0         /* �����°�� */

#define INHNO_DSI       IRQ_VECTOR_DSI
#define INTNO_DSI       IRQ_VECTOR_DSI
#define INTPRI_DSI      -7        /* �����ͥ���� */
#define INTATR_DSI      0         /* �����°�� */

#define INHNO_I2C1EV    IRQ_VECTOR_I2C1_EV	/* ����ߥϥ�ɥ��ֹ� */
#define INTNO_I2C1EV    IRQ_VECTOR_I2C1_EV	/* ������ֹ� */
#define INTPRI_I2C1EV   -5				/* �����ͥ���� */
#define INTATR_I2C1EV   0				/* �����°�� */

#define INHNO_I2C1ER    IRQ_VECTOR_I2C1_ER	/* ����ߥϥ�ɥ��ֹ� */
#define INTNO_I2C1ER    IRQ_VECTOR_I2C1_ER	/* ������ֹ� */
#define INTPRI_I2C1ER   -5				/* �����ͥ���� */
#define INTATR_I2C1ER   0				/* �����°�� */

#define INHNO_I2C4EV    IRQ_VECTOR_I2C4_EV	/* ����ߥϥ�ɥ��ֹ� */
#define INTNO_I2C4EV    IRQ_VECTOR_I2C4_EV	/* ������ֹ� */
#define INTPRI_I2C4EV   -5				/* �����ͥ���� */
#define INTATR_I2C4EV   0				/* �����°�� */

#define INHNO_I2C4ER    IRQ_VECTOR_I2C4_ER	/* ����ߥϥ�ɥ��ֹ� */
#define INTNO_I2C4ER    IRQ_VECTOR_I2C4_ER	/* ������ֹ� */
#define INTPRI_I2C4ER   -5				/* �����ͥ���� */
#define INTATR_I2C4ER   0				/* �����°�� */

#define INHNO_DFSDM1F0  IRQ_DFSDM1_FLT0	/* ����ߥϥ�ɥ��ֹ� */
#define INTNO_DFSDM1F0  IRQ_DFSDM1_FLT0	/* ������ֹ� */
#define INTPRI_DFSDM1F0 -10				/* �����ͥ���� */
#define INTATR_DFSDM1F0 0				/* �����°�� */

#define INHNO_DFSDM1F1  IRQ_DFSDM1_FLT1	/* ����ߥϥ�ɥ��ֹ� */
#define INTNO_DFSDM1F1  IRQ_DFSDM1_FLT1	/* ������ֹ� */
#define INTPRI_DFSDM1F1 -10				/* �����ͥ���� */
#define INTATR_DFSDM1F1 0				/* �����°�� */

#define INHNO_DFSDM1F2  IRQ_DFSDM1_FLT2	/* ����ߥϥ�ɥ��ֹ� */
#define INTNO_DFSDM1F2  IRQ_DFSDM1_FLT2	/* ������ֹ� */
#define INTPRI_DFSDM1F2 -10				/* �����ͥ���� */
#define INTATR_DFSDM1F2 0				/* �����°�� */

#define INHNO_DFSDM1F3  IRQ_DFSDM1_FLT3	/* ����ߥϥ�ɥ��ֹ� */
#define INTNO_DFSDM1F3  IRQ_DFSDM1_FLT3	/* ������ֹ� */
#define INTPRI_DFSDM1F3 -10				/* �����ͥ���� */
#define INTATR_DFSDM1F3 0				/* �����°�� */


#define INHNO_DMAS21    IRQ_VECTOR_DMA2_STREAM1	/* ����ߥϥ�ɥ��ֹ� */
#define INTNO_DMAS21    IRQ_VECTOR_DMA2_STREAM1	/* ������ֹ� */
#define INTPRI_DMAS21   -4			/* �����ͥ���� */
#define INTATR_DMAS21   0				/* �����°�� */

#define INHNO_DMAS23    IRQ_VECTOR_DMA2_STREAM3	/* ����ߥϥ�ɥ��ֹ� */
#define INTNO_DMAS23    IRQ_VECTOR_DMA2_STREAM3	/* ������ֹ� */
#define INTPRI_DMAS23   -4			/* �����ͥ���� */
#define INTATR_DMAS23   0				/* �����°�� */

#define INHNO_DMAS24    IRQ_VECTOR_DMA2_STREAM4	/* ����ߥϥ�ɥ��ֹ� */
#define INTNO_DMAS24    IRQ_VECTOR_DMA2_STREAM4	/* ������ֹ� */
#define INTPRI_DMAS24   -4			/* �����ͥ���� */
#define INTATR_DMAS24   0				/* �����°�� */

#define INHNO_DMAS26    IRQ_VECTOR_DMA2_STREAM6	/* ����ߥϥ�ɥ��ֹ� */
#define INTNO_DMAS26    IRQ_VECTOR_DMA2_STREAM6	/* ������ֹ� */
#define INTPRI_DMAS26   -4			/* �����ͥ���� */
#define INTATR_DMAS26   0				/* �����°�� */

#define INHNO_DMAS27    IRQ_VECTOR_DMA2_STREAM7	/* ����ߥϥ�ɥ��ֹ� */
#define INTNO_DMAS27    IRQ_VECTOR_DMA2_STREAM7	/* ������ֹ� */
#define INTPRI_DMAS27   -4			/* �����ͥ���� */
#define INTATR_DMAS27   0				/* �����°�� */


typedef struct
{
  void   (*DemoFunc)(void);
  uint8_t DemoName[50]; 
  uint32_t DemoIndex;
}BSP_DemoTypedef;

typedef enum {
  AUDIO_ERROR_NONE = 0,
  AUDIO_ERROR_NOTREADY,
  AUDIO_ERROR_IO,
  AUDIO_ERROR_EOF,
}AUDIO_ErrorTypeDef;

#define COUNT_OF_EXAMPLE(x)    (sizeof(x)/sizeof(BSP_DemoTypedef))

#ifdef USE_FULL_ASSERT
/* Assert activated */
#define BSP_TEST_APPLI_ASSERT(__error_condition__)    do { if(__error_condition__) \
                                                           {  while(1);  \
                                                           } \
                                                          } while(0)
#else
/* Assert not activated : macro has no effect */
#define BSP_TEST_APPLI_ASSERT(__error_condition__)    do { if(__error_condition__) \
                                                           {  ;  \
                                                           } \
                                                         } while(0)
#endif /* USE_FULL_ASSERT */

/* The Audio file is flashed with ST-Link Utility @ flash address =  AUDIO_SRC_FILE_ADDRESS */
#define AUDIO_SRC_FILE_ADDRESS       0x08080000   /* Audio file address in flash */
#define AUDIO_FILE_SIZE              0x80000

/*
 *  �ؿ��Υץ�ȥ��������
 */
#ifndef TOPPERS_MACRO_ONLY

extern void	task(intptr_t exinf);
extern void	main_task(intptr_t exinf);
extern void	tex_routine(TEXPTN texptn, intptr_t exinf);
#ifdef CPUEXC1
extern void	cpuexc_handler(void *p_excinf);
#endif /* CPUEXC1 */
extern void	cyclic_handler(intptr_t exinf);
extern void	alarm_handler(intptr_t exinf);
extern void sw_handle(void);
extern void ltdc_handler(void);
extern void dsi_handler(void);
extern void dfsdm1fl0_handler(void);
extern void dfsdm1fl1_handler(void);
extern void dfsdm1fl2_handler(void);
extern void dfsdm1fl3_handler(void);

/* Exported variables --------------------------------------------------------*/
extern const unsigned char stlogo[];
extern uint32_t SdmmcTest;
extern uint32_t SdramTest;
extern uint8_t  key_value;
extern char     cbuff[260];

/* Exported functions ------------------------------------------------------- */
uint8_t TouchScreen_GetTouchPosition(void);
void Touchscreen_DrawBackground_Circles(uint8_t state);
void     Touchscreen_demo1 (void);
void     Touchscreen_demo2 (void);
void AudioPlay_demo (void);
void AudioLoopback_demo (void);
uint8_t AUDIO_Process(void);
void LCD_demo (void);
void SD_demo (void);
void QSPI_demo (void);
void EEPROM_demo (void);
uint8_t CheckForUserInput(void);
void Toggle_Leds(void);
void Error_Handler(void);
void SDRAM_demo(void);
void SDRAM_DMA_demo (void);
void Log_demo(void);


#endif /* TOPPERS_MACRO_ONLY */
