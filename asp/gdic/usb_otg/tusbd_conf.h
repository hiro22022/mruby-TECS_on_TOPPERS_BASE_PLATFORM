/*
 *  TOPPERS BASE PLATFORM MIDDLEWARE
 * 
 *  Copyright (C) 2017-2017 by TOPPERS PROJECT
 *                             Educational Working Group.
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
 *  @(#) $Id: tusbd_conf.h 698 2017-10-05 21:46:02Z roi $
 */
/*
 *  TOPPERS USB DEVICE MODULE CONFIGURATION ���󥯥롼�ɥե�����
 */

#ifndef _TUSBD_CONF_H_
#define _TUSBD_CONF_H_

#include "dwc2_device_driver.h"

#define TUSBD_MAX_NUM_INTERFACES              1
#define TUSBD_MAX_NUM_CONFIGURATION           1
#define TUSBD_NUM_ENDPOINT                    15
#define TUSBD_CONFIG_SELF_POWERED             0x01

#define USBD_StatusTypeDef   TUSBD_ERCODE
#define USBD_HandleTypeDef   TUSBD_Handle_t
#define USBD_Init            tusbdInit
#define USBD_Start           tusbdStart

extern uint8_t defaultDeviceDescriptor[];
extern uint8_t defaultDeviceQualifierDescriptor[];
extern uint8_t defaultLanguageIDDescriptor[];
extern uint8_t defaultManufacturerString[];
extern uint8_t defaultProductString[];
extern uint8_t defaultSerialString[];
extern uint8_t defaultConfigurationString[];
extern uint8_t defaultInterfaceString[];

#define TUSBD_DEVICE_DESCRIPTOR               defaultDeviceDescriptor
#define TUSBD_DEVICE_QUALIFIER_DESCRIPTOR     defaultDeviceQualifierDescriptor
#define TUSBD_LANGUAGEID_DESCRIPTOR           defaultLanguageIDDescriptor
#define TUSBD_MANUFACTURER_STRING             defaultManufacturerString
#define TUSBD_PRODUCT_STRING                  defaultProductString
#define TUSBD_SERIAL_STRING                   defaultSerialString
#define TUSBD_CONFIGURATION_STRING            defaultConfigurationString
#define TUSBD_INTERFACE_STRING                defaultInterfaceString
#define TUSBD_USER_STRING                     NULL

#define tusbdDriverSetUSBAddress(d, a)  usbo_setDevAddress(((d)->pSysData), (a))
#define tusbdDriverGetRxDataSize        dwc2_device_getrxdatasize

#define tusbdDriverInit                 dwc2_device_init
#define tusbdDriverDeInit               dwc2_device_deinit
#define tusbdDriverStart                dwc2_device_start
#define tusbdDriverStop                 dwc2_device_stop
#define tusbdDriverOpenEp               dwc2_device_openep
#define tusbdDriverCloseEp              dwc2_device_closeep
#define tusbdDriverFlushEp              dwc2_device_flushep
#define tusbdDriverStallEp              dwc2_device_stallep
#define tusbdDriverClearStallEp         dwc2_device_clearep
#define tusbdDriverStallConditionEp     dwc2_device_getstallcondition
#define tusbdDriverStartTransmit        dwc2_device_transmitdata
#define tusbdDriverSetupReceive         dwc2_device_startreceive
#define tusbdDriverTestMode             dwc2_device_testmode

#endif /* _TUSBD_CONF_H_ */

