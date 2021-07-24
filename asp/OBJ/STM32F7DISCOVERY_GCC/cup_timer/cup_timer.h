/*
 * ���åץ顼��󥿥��ޤΥإå��ե�����
 */

#define STACK_SIZE      2048  /* �������Υ����å������� */
#define DEFAULT_PRIORITY   8  /* ��������ͥ���� */

#define I2C_PORTID    I2C3_PORTID

#define INHNO_I2CEV   IRQ_VECTOR_I2C3_EV	/* ����ߥϥ�ɥ��ֹ� */
#define INTNO_I2CEV   IRQ_VECTOR_I2C3_EV	/* ������ֹ� */
#define INTPRI_I2CEV  -5			/* �����ͥ���� */
#define INTATR_I2CEV  0				/* �����°�� */

#define INHNO_I2CER   IRQ_VECTOR_I2C3_ER	/* ����ߥϥ�ɥ��ֹ� */
#define INTNO_I2CER   IRQ_VECTOR_I2C3_ER	/* ������ֹ� */
#define INTPRI_I2CER  -5			/* �����ͥ���� */
#define INTATR_I2CER  0				/* �����°�� */


#define LED1_BLINK_INTERVAL 1000     /* LED1�����Ǵֳ�   */
#define LED4_BLINK_INTERVAL  250     /* LED4�����Ǵֳ�   */
#define SW_SCAN_INTERVAL      10     /* SW�Υ������ֳ� */
#define BASE_TIME_INTERVAL   250     /* ���ܻ��ֳִ�     */
#define TO_SEC               (1000/BASE_TIME_INTERVAL)

#define INIT_TIME      30     /* �������Ȼ��Υ����ॢ���Ȼ��� */
#define EXTRA_UNIT     30     /* ���ֱ�Ĺñ�� */
#define TIMEOUT_BLINK  60     /* �����ॢ���Ȼ������ǲ�� */
#define ACT_INTERVAL   10     /* ������ư������LED�Υ��󥿡��Х� */
#define ACT_BLINK_TIME  4     /* ������ư������LED�����ǲ�� */


#ifndef TOPPERS_MACRO_ONLY
extern void timer_task(intptr_t exinf);
extern void blink_task(intptr_t exinf);

extern void led1_blink_handler(void);
extern void led4_blink_time_handler(void);
extern void sw_scan_handler(void);
extern void base_time_handler(void);

extern void device_info_init(intptr_t exinf);
extern void sw_handle(void);


#endif /* TOPPERS_MACRO_ONLY */

