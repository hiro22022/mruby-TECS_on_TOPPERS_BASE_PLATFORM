/*
 *  TOPPERS/ASP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Advanced Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2004-2012 by Embedded and Real-Time Systems Laboratory
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
 *  $Id: hid_keyboard.c 2416 2017-05-28 11:13:03Z roi $
 */
/* Copyright (c) 2010-2011 mbed.org, MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 * and associated documentation files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/* 
 *  USB HOST KEYBOARD����������
 */

#include <kernel.h>
#include <t_syslog.h>
#include <t_stdlib.h>
#include <sil.h>
#include "syssvc/serial.h"
#include "syssvc/syslog.h"
#include "device.h"
#include "hid_appli.h"

static const uint8_t keymap[4][60] = {
	{
		0,   0,   0,   0,   'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h',	/* 000 - 011 */
		'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't',	/* 012 - 023 */
		'u', 'v', 'w', 'x', 'y', 'z', '1', '2', '3', '4', '5', '6',	/* 024 - 035 */
		'7', '8', '9', '0', 10,  27,  8,   9,   ' ', '-', '=', '[',	/* 036 - 047 */
		']', '\\','#', ';', '\'', 0,  ',', '.', '/', 0,   0,   0	/* 048 - 059 */
    },

    /* CTRL MODIFIER */
	{
		0,   0,   0,   0,   1,   2,   3,   4,   5,   6,   7,   8,	/* 000 - 011 */
		9,   10,  11,  12,  13,  14,  15,  16,  17,  18,  18,  19,	/* 012 - 023 */
		20,  21,  22,  23,  24,  25,  0,   0,   0,   0,   0,   0,	/* 024 - 035 */
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	/* 036 - 047 */
		0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0	/* 048 - 059 */
	},

	/* SHIFT MODIFIER */
	{
		0,   0,   0,   0,   'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',	/* 000 - 011 */
		'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',	/* 012 - 023 */
		'U', 'V', 'W', 'X', 'Y', 'Z', '!', '@', '#', '$', '%', '^',	/* 024 - 035 */
		'&', '*', '(', ')', 0,   0,   0,   0,   0,   0,   '+', '{',	/* 036 - 047 */
		'}', '|', '~', ':', '"', 0,   '<', '>', '?', 0,   0,   0	/* 048 - 059 */
	},

	/* ALT MODIFIER */
	{
		0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	/* 000 - 011 */
		0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	/* 012 - 023 */
		0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	/* 024 - 035 */
 		0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	/* 036 - 047 */
		0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0	/* 048 - 059 */
	}
};

/*
 *  HID KEYBOARD�����
 *  parameter1:  phandle  HID�ϥ�ɥ�
 *  return       TUSBH_ERCODE
 */
TUSBH_ERCODE
HidKeyboardInit(void *phandle)
{
	syslog_0(LOG_NOTICE, "## USBH_HID_KeybdInit ##");
	return TUSBH_E_OK;
}

/*
 *  HID KEYBOARD�������
 *  parameter1:  phandle  HID�ϥ�ɥ�
 *  parameter2:  pinfo    KEYBOARD�������꥿���פؤΥݥ���
 *  parameter3:  buffer   USB HOST����Υǡ���
 *  parameter4:  length   USB HOST�ǡ���Ĺ
 *  return       TUSBH_ERCODE
 */
TUSBH_ERCODE
HidGetKeyboardInfo(void *phandle, KEYBOARD_Info_t *pinfo, uint8_t *buffer, uint16_t length)
{
	uint8_t i;

	if(length > 0){
		pinfo->lctrl = (buffer[0] >> 0) & 1;
		pinfo->lshift = (buffer[0] >> 1) & 1;
		pinfo->lalt = (buffer[0] >> 2) & 1;
		pinfo->lgui = (buffer[0] >> 3) & 1;
		pinfo->rctrl = (buffer[0] >> 4) & 1;
		pinfo->rshift = (buffer[0] >> 5) & 1;
		pinfo->ralt = (buffer[0] >> 6) & 1;
		pinfo->rgui = (buffer[0] >> 7) & 1;

		for(i = 0 ; i < sizeof(pinfo->keys) ; i++)
			pinfo->keys[i]=(uint32_t)buffer[i+2];
		return TUSBH_E_OK;
	}
	else
		return TUSBH_E_TMOUT;
}

/*
 *  HID ASCII-CODE����
 *  parameter1:  phandle  HID�ϥ�ɥ�
 *  parameter2:  pinfo    KEYBOARD�������꥿���פؤΥݥ���
 *  return       ASCII CODE
 */
uint8_t
HidGetASCIICode(KEYBOARD_Info_t *pinfo)
{
	uint8_t   index = 0;

	if(pinfo->lshift == 1 || pinfo->rshift == 1)
		index = 2;
	else if(pinfo->lctrl == 1 || pinfo->rctrl == 1)
		index = 1;
	else if(pinfo->lalt == 1 || pinfo->ralt == 1)
		index = 3;
	if(pinfo->keys[0] < 60)
		return keymap[index][pinfo->keys[0]];
	else
		return 0;
}

