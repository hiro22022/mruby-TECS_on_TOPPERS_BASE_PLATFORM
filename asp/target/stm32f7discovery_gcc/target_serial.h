/*
 *  TOPPERS/ASP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Advanced Standard Profile Kernel
 * 
 *  Copyright (C) 2008-2011 by Embedded and Real-Time Systems Laboratory
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
 *  Copyright (C) 2015      by 3rd Designing Center
 *              Imageing System Development Division RICOH COMPANY, LTD.
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
 *    (b) �����ۤη��֤��̤�������ˡ�ˤ�äơ�TOPPERS�ץ��������Ȥ�
 *        ��𤹤뤳�ȡ�
 *  (4) �ܥ��եȥ����������Ѥˤ��ľ��Ū�ޤ��ϴ���Ū�������뤤���ʤ�»
 *      ������⡤�嵭����Ԥ����TOPPERS�ץ��������Ȥ����դ��뤳�ȡ�
 *      �ޤ����ܥ��եȥ������Υ桼���ޤ��ϥ���ɥ桼������Τ����ʤ���
 *      ͳ�˴�Ť����ᤫ��⡤�嵭����Ԥ����TOPPERS�ץ��������Ȥ�
 *      ���դ��뤳�ȡ�
 * 
 *  �ܥ��եȥ������ϡ�̵�ݾڤ��󶡤���Ƥ����ΤǤ��롥�嵭����Ԥ�
 *  ���TOPPERS�ץ��������Ȥϡ��ܥ��եȥ������˴ؤ��ơ�����λ�����Ū
 *  ���Ф���Ŭ������ޤ�ơ������ʤ��ݾڤ�Ԥ�ʤ����ޤ����ܥ��եȥ���
 *  �������Ѥˤ��ľ��Ū�ޤ��ϴ���Ū�������������ʤ�»���˴ؤ��Ƥ⡤��
 *  ����Ǥ�����ʤ���
 * 
 *  @(#) $Id: target_serial.h 698 2015-12-10 14:23:15Z roi $
 */

/*
 *    ���ꥢ��I/O�ǥХ�����SIO�˥ɥ饤��
 */

#ifndef TOPPERS_TARGET_SERIAL_H
#define TOPPERS_TARGET_SERIAL_H

/*
 *  ���åװ�¸�⥸�塼���stm32f7-discovery�ѡ�
 */

/*
 *  SIO��GPIO������
 */
#define TADR_U1T_GPIO_BASE TADR_GPIOA_BASE
#define TADR_U1R_GPIO_BASE TADR_GPIOB_BASE
#define TADR_U2T_GPIO_BASE TADR_GPIOC_BASE
#define TADR_U2R_GPIO_BASE TADR_GPIOC_BASE
#define TOFF_U1_APBNER     TOFF_RCC_APB2ENR
#define TOFF_U2_APBNER     TOFF_RCC_APB2ENR
#define TOFF_U1_APBRSTR    TOFF_RCC_APB2RSTR
#define TOFF_U2_APBRSTR    TOFF_RCC_APB2RSTR
#define GPIO_U1_AF         0x07
#define GPIO_U2_AF         0x08
#define ENABLE_U1_PORT     RCC_APB2ENR_USART1EN
#define ENABLE_U2_PORT     RCC_APB2ENR_USART6EN
#define ENABLE_U1T_GPIO    RCC_AHB1ENR_GPIOAEN
#define ENABLE_U1R_GPIO    RCC_AHB1ENR_GPIOBEN
#define ENABLE_U2T_GPIO    RCC_AHB1ENR_GPIOCEN
#define ENABLE_U2R_GPIO    RCC_AHB1ENR_GPIOCEN
#define RESET_U1_PORT      RCC_APB2RSTR_USART1RST
#define RESET_U2_PORT      RCC_APB2RSTR_USART6RST
#define TX1_PINPOS         9
#define RX1_PINPOS         7
#define TX2_PINPOS         6
#define RX2_PINPOS         7

/*
 *  SIO�Υ١������ɥ쥹
 */
#define USART1_BASE        TADR_USART1_BASE
#define USART2_BASE        TADR_USART6_BASE

/*
 *  CLOCK����������
 */
#define U1_CLOCK_SHIFT     0
#define U2_CLOCK_SHIFT     10

/*
 *  ���ꥢ��I/O�ݡ��ȿ������
 */
#define TNUM_SIOP       2       /* ���ݡ��Ȥ��륷�ꥢ��I/O�ݡ��Ȥο� */

/*
 *  SIO�γ���ߥϥ�ɥ�Υ٥����ֹ�
 */
#define INHNO_SIO1      IRQ_VECTOR_USART1
#define INTNO_SIO1      IRQ_VECTOR_USART1
#define INHNO_SIO2      IRQ_VECTOR_USART6
#define INTNO_SIO2      IRQ_VECTOR_USART6

#define INTPRI_SIO       -3        /* �����ͥ���� */
#define INTATR_SIO       0         /* �����°�� */


#include "arm_m_gcc/stm32f7xx/chip_serial.h"

#endif /* TOPPERS_TARGET_SERIAL_H */