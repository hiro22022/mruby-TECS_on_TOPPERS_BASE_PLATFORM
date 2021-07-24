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
 *  @(#) $Id: tusbh_base.h 698 2017-10-28 10:48:57Z roi $
 */
/*
 *  USB Host Middleware BASE�����
 */

#ifndef _HUSBH_BASE_H_
#define _HUSBH_BASE_H_

#include "tusb_rtos.h"
#include "tusb_types.h"

#ifdef __cplusplus
 extern "C" {
#endif

#ifndef TUSBH_MAX_NUM_ENDPOINTS
#define TUSBH_MAX_NUM_ENDPOINTS           2
#endif
#ifndef TUSBH_MAX_NUM_INTERFACES
#define TUSBH_MAX_NUM_INTERFACES          2
#endif
#ifndef TUSBH_MAX_DATA_BUFFER
#define TUSBH_MAX_DATA_BUFFER             0x200
#endif

/*
 *  STM�桼�������٥��
 */
#define HOST_USER_SELECT_CONFIGURATION    1
#define HOST_USER_CLASS_ACTIVE            2
#define HOST_USER_CLASS_SELECTED          3
#define HOST_USER_CONNECTION              4
#define HOST_USER_DISCONNECTION           5
#define HOST_USER_UNRECOVERED_ERROR       6

/*Standard Feature Selector for clear feature command*/
#define FEATURE_SELECTOR_ENDPOINT         0X00
#define FEATURE_SELECTOR_DEVICE           0X01

/*
 *  SUBMIT������
 */
#define SUBMIT_SETUP                      ((0<<8) | USB_PID_SETUP)
#define SUBMIT_WRITE                      ((0<<8) | USB_PID_DATA)
#define SUBMIT_READ                       ((1<<8) | USB_PID_DATA)

/*
 *  �ѥ���°�����
 */
#define PIPE_ACTIVATE                     0x8000
#define PIPE_ADDRESS                      0x00FF


#define NO_INTERFACE                      0xFF


/*
 *  URB���ơ������
 */
typedef enum {
	TUSBH_URB_IDLE = 0,
	TUSBH_URB_DONE,
	TUSBH_URB_NOTREADY,
	TUSBH_URB_NYET,
	TUSBH_URB_ERROR,
	TUSBH_URB_STALL
} TUSBH_URBState_t;

/*
 *  �ץ������٥��
 */
enum {
	TUSBH_PORT_EVENT = 1,
	TUSBH_URB_EVENT,
	TUSBH_TIME_EVENT,
	TUSBH_CLASS_EVENT,
	TUSBH_RESET_EVENT
};

enum {
	TUSBH_CONNECT_EVENT = 1,
	TUSBH_DISCONNECT_EVENT,
	TUSBH_IDCHANGE_EVENT,
};

/*
 *  �ǥХ����������
 */
enum {
	DEVICE_IDLE = 0,
	DEVICE_ATTACHED,
	DEVICE_DISCONNECTED,
	DEVICE_CHECK_CLASS,
	DEVICE_CLASS,
	DEVICE_SUSPENDED,
	DEVICE_ABORT_STATE,
};

/*
 *  CONTROL�̿�����
 */
enum {
	CONTROL_IDLE = 0,
	CONTROL_SETUP,
	CONTROL_SETUP_WAIT,
	CONTROL_DATA_IN_WAIT,
	CONTROL_DATA_OUT_WAIT,
	CONTROL_STATUS_IN_WAIT,
	CONTROL_STATUS_OUT_WAIT,
	CONTROL_ERROR
};


#define USBH_HandleTypeDef     TUSBH_Handle_t
typedef struct _TUSBH_Handle_t TUSBH_Handle_t;
typedef struct _TUSBH_Class_t  TUSBH_Class_t;
typedef struct _TUSBH_Device_t TUSBH_Device_t;

typedef struct
{
	uint16_t                   attribute;
	uint8_t                    index;
	uint8_t                    type;
} USBH_Pipe_t;

/*
 *  USB�ۥ��ȥ��饹���
 */
struct _TUSBH_Class_t
{
	TUSBH_Class_t              *pNext;
	const char                 *Name;
	uint32_t                   classCode;
	TUSBH_ERCODE               (*Init)(TUSBH_Device_t *pdevice);
	TUSBH_ERCODE               (*DeInit)(TUSBH_Device_t *pdevice);
	TUSBH_ERCODE               (*Process)(TUSBH_Device_t *pdevice, uint8_t *mes);
	void                       *subfunc;
};

/*
 *  USB�ۥ��ȥǥХ����ϥ�ɥ�
 */
struct _TUSBH_Device_t
{
	TUSBH_Device_t             *pNext;
	TUSBH_Handle_t             *pHost;
	TUSBH_Class_t              *pClass;
	uint8_t                    activate;
	uint8_t                    address;
	uint8_t                    speed;
	volatile uint8_t           is_connected;
	uint8_t                    pre_connected;
	uint8_t                    sel_interface;
	uint8_t                    hub;
	uint8_t                    port;
	uint8_t                    idx;
	volatile uint8_t           dstate;
	uint16_t                   timeid;
	uint32_t                   timecount;

	uint8_t                    cntl_pipe_in;
	uint8_t                    cntl_pipe_out;
	uint8_t                    cntl_pipe_size;
	uint8_t                    cntl_errcount;
    uint8_t                    setupPacket[8];
	uint32_t                   dummy[6];	/* dummy for cache */
	uint8_t                    Data[TUSBH_MAX_DATA_BUFFER];
	DeviceDescriptor           DevDesc;
	ConfigurationDescriptor    CfgDesc;
	InterfaceDescriptor        ItfDesc[TUSBH_MAX_NUM_INTERFACES];
	EndpointDescriptor         EpDesc[TUSBH_MAX_NUM_INTERFACES][TUSBH_MAX_NUM_ENDPOINTS];

	uint8_t                    type;
	uint8_t                    hubid;
	volatile uint8_t           cstate;
	uint8_t                    numUnit;
	void                       *pData;
	uint8_t                    *cbuff;
	char                       Manufacturer[TUSBH_STR_LENGTH];
	char                       Prodeuct[TUSBH_STR_LENGTH];
	char                       Serial[TUSBH_STR_LENGTH];
};

/*
 *  USB�ۥ��ȥϥ�ɥ�
 */
struct _TUSBH_Handle_t
{
	TUSBH_Device_t             *pDevice;
	TUSBH_Class_t              *pClass;
	USBH_Pipe_t                pipes[MAX_EPS_PIPES];
	uint32_t                   numPipes;
	void                       *pSysData;
	uint8_t                    *pCfgData;
	void                       (*usrcallback)(TUSBH_Handle_t *pHandle, TUSBH_Device_t *pdevice, uint8_t id);

	uint8_t                    id;
	uint8_t                    numDevice;
	uint8_t                    numClass;
	uint8_t                    numHub;
	ID                         processTaskID;
	ID                         eventTaskID;
	ID                         process_event;
	ID                         connect_event;
};

#include "tusbh_conf.h"

/*
 *  �ؿ��ץ�ȥ��������
 */
TUSBH_ERCODE tusbhInit(TUSBH_Handle_t *phost, void (*pufunc)(TUSBH_Handle_t*, TUSBH_Device_t *, uint8_t ), uint8_t id);
TUSBH_ERCODE tusbhDeInit(TUSBH_Handle_t *phost);
TUSBH_ERCODE tusbhLinkClass(TUSBH_Handle_t *phost, TUSBH_Class_t *pclass);
TUSBH_ERCODE tusbhStart(TUSBH_Handle_t *phost);
TUSBH_ERCODE tusbhStop(TUSBH_Handle_t *phost);

TUSBH_ERCODE tusbhResetHub(TUSBH_Device_t *pdevice);
TUSBH_ERCODE tusbhSelectInterface(TUSBH_Device_t *pdevice, uint8_t interface);
uint8_t      tusbhFindInterface(TUSBH_Device_t *pdevice, uint8_t Class, uint8_t SubClass, uint8_t Protocol);
uint8_t      tusbhGetActiveClass(TUSBH_Device_t *pdevice);
uint8_t      tusbhFindInterfacebynumber(TUSBH_Device_t *pdevice, uint8_t interface, uint8_t altsetno);

TUSBH_ERCODE tusbhControlRequest(TUSBH_Device_t *pdevice, uint8_t *buff, uint8_t type, uint8_t request, uint16_t value, uint16_t index, uint16_t length);
TUSBH_ERCODE tusbhControlWait(TUSBH_Device_t *pdevice, uint8_t *mes);
TUSBH_ERCODE tusbhGetDescriptor(TUSBH_Device_t *pdevice, uint8_t req_type, uint16_t value, uint8_t* buff, uint16_t length);
TUSBH_ERCODE tusbhSetInterface(TUSBH_Device_t *pdevice, uint8_t ep_num, uint8_t interface);
TUSBH_ERCODE tusbhClearFeature(TUSBH_Device_t *pdevice, uint8_t ep_num);

TUSBH_ERCODE tusbhOpenPipe(TUSBH_Device_t *, uint8_t, uint8_t, uint8_t, uint16_t);
TUSBH_ERCODE tusbhClosePipe(TUSBH_Device_t *, uint8_t pipe);
TUSBH_ERCODE tusbhSubmitURB(TUSBH_Device_t *pdevice, uint8_t, uint16_t, uint8_t, uint8_t*, uint16_t);
TUSBH_ERCODE tusbhControlSendSetup(TUSBH_Device_t *pdevice, uint8_t *buff, uint8_t pipe_num);
TUSBH_ERCODE tusbhControlSendData(TUSBH_Device_t *pdevice, uint8_t *buff, uint16_t length, uint8_t pipe_num);
TUSBH_ERCODE tusbhControlReceiveData(TUSBH_Device_t *pdevice, uint8_t* buff, uint16_t length, uint8_t pipe_num);
TUSBH_ERCODE tusbhBulkWrite(TUSBH_Device_t *pdevice, uint8_t *buff, uint16_t length, uint8_t pipe_num);
TUSBH_ERCODE tusbhBulkRead(TUSBH_Device_t *pdevice, uint8_t *buff, uint16_t length, uint8_t pipe_num);
TUSBH_ERCODE tusbhInterruptWrite(TUSBH_Device_t *pdevice, uint8_t *buff, uint8_t length, uint8_t pipe_num);
TUSBH_ERCODE tusbhInterruptRead(TUSBH_Device_t *pdevice, uint8_t *buff, uint8_t length, uint8_t pipe_num);
TUSBH_ERCODE tusbhIsocWrite(TUSBH_Device_t *pdevice, uint8_t *buff, uint32_t length, uint8_t pipe_num);
TUSBH_ERCODE tusbhIsocRead(TUSBH_Device_t *pdevice, uint8_t *buff, uint32_t length, uint8_t pipe_num);

void tusbhProcessTask(intptr_t exinf);
void tusbhEventTask(intptr_t exinf);
void tusbhCyclicHandler(intptr_t exinf);

TUSBH_Device_t *tusbhSearchDevice(TUSBH_Handle_t *phost, uint8_t classCode, uint8_t *plun);
uint8_t tusbhAllocPipe(TUSBH_Handle_t *phost, uint8_t ep_addr);
TUSBH_ERCODE tusbFreePipe(TUSBH_Handle_t *phost, uint8_t no);


#ifdef __cplusplus
}
#endif

#endif /* _HUSBH_BASE_H_ */

