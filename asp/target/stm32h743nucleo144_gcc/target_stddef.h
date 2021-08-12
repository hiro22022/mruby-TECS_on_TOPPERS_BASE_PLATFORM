/*
 *  TOPPERS Software
 *      Toyohashi Open Platform for Embedded Real-Time Systems
 * 
 *  Copyright (C) 2008-2011 by Embedded and Real-Time Systems Laboratory
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
 *  Copyright (C) 2015-2016 by 3rd Designing Center
 *              Imageing System Development Division RICOH COMPANY, LTD.
 *  Copyright (C) 2017-2019 by TOPPERS PROJECT Educational Working Group.
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
 *  @(#) $Id: target_stddef.h 698 2019-07-23 21:47:27Z roi $
 */

/*
 *  t_stddef.h�Υ������åȰ�¸����stm32h743-nucleo144�ѡ�
 *
 *  ���Υ��󥯥롼�ɥե�����ϡ�t_stddef.h����Ƭ�ǥ��󥯥롼�ɤ���롥
 *  ¾�Υե����뤫���ľ�ܥ��󥯥롼�ɤ��뤳�ȤϤʤ���¾�Υ��󥯥롼��
 *  �ե��������Ω�äƽ�������뤿�ᡤ¾�Υ��󥯥롼�ɥե�����˰�¸��
 *  �ƤϤʤ�ʤ���
 */

#ifndef TOPPERS_TARGET_STDDEF_H
#define TOPPERS_TARGET_STDDEF_H

/*
 *  �������åȤ��̤��뤿��Υޥ��������
 */
#define TOPPERS_STM32H743_NUCLEO144				/* �����ƥ�ά�� */

/*
 *  ��ȯ�Ķ��Ƕ��̤����
 *
 *  ��ȯ�Ķ���stdint.h���Ѱդ���Ƥ�����ˤϡ�TOPPERS_STDINT_TYPE1��
 *  �ޥ��������������stdint.h�򥤥󥯥롼�ɤ���Ф褤��
 */
#define TOPPERS_STDINT_TYPE1
#define TOPPERS_STDFLOAT_TYPE1
#define _SYS__STDINT_H
#define _STDINT_H
#include "gcc/tool_stddef.h"

/*
 *  ������¸�Ƕ��̤����
 */
#include "arm_m_gcc/common/core_stddef.h"


#ifndef TOPPERS_MACRO_ONLY

/*
 *  �����������μ��Ի��μ¹����ǽ�����T.B.D��
 */
Inline void
TOPPERS_assert_abort(void)
{
    
}

#endif /* TOPPERS_MACRO_ONLY */
#endif /* TOPPERS_TARGET_STDDEF_H */