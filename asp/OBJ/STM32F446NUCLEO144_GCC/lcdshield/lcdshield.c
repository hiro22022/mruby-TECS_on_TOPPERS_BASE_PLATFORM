/*
 *  TOPPERS/ASP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Advanced Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2004-2012 by Embedded and Real-Time Systems Laboratory
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
 *  $Id: lcdshield.c 2416 2016-02-12 12:05:32Z roi $
 */

/* 
 *  LCD�����������ץ���������
 */

#include <kernel.h>
#include <t_syslog.h>
#include <t_stdlib.h>
#include <stdio.h>
#include <string.h>
#include <target_syssvc.h>
#include "syssvc/serial.h"
#include "syssvc/syslog.h"
#include "kernel_cfg.h"
#include "device.h"
#include "rtc.h"
#include "spi.h"
#include "adc.h"
#include "spi_driver.h"
#include "adafruit_st7735.h"
#ifdef USE_SDM
#include "storagedevice.h"
#include "sddiskio.h"
#endif
#ifdef USE_MSC_FILE
#include "storagedevice.h"
#include "mscdiskio.h"
#endif
#include "lcdshield.h"
#include "topamelogo.h"

/*
 *  �����ӥ�������Υ��顼�Υ�����
 */
Inline void
svc_perror(const char *file, int_t line, const char *expr, ER ercd)
{
	if (ercd < 0) {
		t_perror(LOG_ERROR, file, line, expr, ercd);
	}
}

#define	SVC_PERROR(expr)	svc_perror(__FILE__, __LINE__, #expr, (expr))
#define ABS(X)  ((X) > 0 ? (X) : -(X))


/*
 *  ���Ѥ����ѿ������
 */
static const Point testPolygon[7] = {
	{0, 0},
	{127, 159},
	{64, 0},
	{64, 159},
	{127, 0},
	{0, 159},
	{0, 0}
};

static const uint32_t pos_tbl[NUM_JSPOSITION-1][2] = {
	{  500, JS_DOWN  },
	{ 1000, JS_RIGHT },
	{ 1500, JS_ON    },
	{ 2500, JS_UP    },
	{ 3800, JS_LEFT  }
};
static const char *pos_str[NUM_JSPOSITION-1] = {
	"      ON      ",
	"      UP      ",
	"     DOWN     ",
	"     LEFT     ",
	"     RIGHT    "
};

#define NUM_ADCBUF   1		/* 96:1ms */
volatile uint32_t uhADCxConvertedValue;

LCD_Handler_t  LcdHandle;
LCD_DrawProp_t DrawProp;

/*
 *  SPI-SDCARD�����å��ѥ���ΰ�
 */
extern uint8_t aTxBuffer[BUFFERSIZE];


#if !defined(USE_SDM) && !defined(USE_MSC_FILE)
uint32_t sdbuffer[514/4];
#endif

#ifdef USE_MSC_FILE
static uint32_t heap_area[2*1024];

uint32_t heap_param[2] = {
	(uint32_t)heap_area,
	(2*4*1024)
};

/*
 *  USB HOST INFORMATION CALLBACK
 *  parameter1:  Host Handler
 *  parameter2:  Device Handler
 *  parameter3:  Host state ID.
 */
static void USBH_UserProcess(USBH_HandleTypeDef *phost, TUSBH_Device_t *pdevice, uint8_t id)
{
	if(pdevice->pClass != NULL){
		syslog_3(LOG_NOTICE, "## CLASS[%s](%d)(%d) ##", pdevice->pClass->Name, pdevice->type, id);
	}
}
#endif

/*
 *  SW1�����
 */
void sw_int(void)
{
	syslog_0(LOG_NOTICE, "## sw_int() ##");
}

#ifdef USE_SDM
/*
 *  ����μ�Ф�
 */
static uint32_t get_fat_time(void)
{
	int    reqtime = 0;
	struct tm2 timedate;

	rtc_get_time(&timedate);
	reqtime  = ((timedate.tm_year-10)<<25)+(timedate.tm_mon<<21)+(timedate.tm_mday<<16);
	reqtime += (timedate.tm_hour<<11)+(timedate.tm_min<<5)+(timedate.tm_sec/2);
	return reqtime;
}
#endif

/*
 *  ʸ������
 *  param1  x0: Line where to display the character shape
 *  param2  y0: Start column address
 *  param3  c: Pointer to the character data
 */
static void
DrawChar(uint16_t x0, uint16_t y0, const uint8_t *c)
{
	LCD_Handler_t *hlcd = DrawProp.hlcd;
	sFONT    *pFont = DrawProp.pFont;
	uint32_t i = 0, j = 0;
	uint16_t height, width;
	uint8_t  offset;
	uint8_t  *pchar;
	uint32_t line;

	height = pFont->Height;
	width  = pFont->Width;
	offset =  8 *((width + 7)/8) -  width ;
	lcd_setAddrWindow(hlcd, x0, y0, x0+width-1, y0+height-1);

	for(i = 0; i < height; i++){
		pchar = ((uint8_t *)c + (width + 7)/8 * i);
		switch(((width + 7)/8)){
		case 1:
			line =  pchar[0];
			break;
		case 2:
			line =  (pchar[0]<< 8) | pchar[1];
			break;
		case 3:
		default:
			line =  (pchar[0]<< 16) | (pchar[1]<< 8) | pchar[2];
			break;
		}

		for(j = 0; j < width; j++){
			if(line & (1 << (width- j + offset- 1))){
				lcd_writedata2(hlcd, DrawProp.TextColor, 1);
			}
			else{
				lcd_writedata2(hlcd, DrawProp.BackColor, 1);
			}
		}
	}
}

/*
 *  ��ʸ������
 *  param1  x0: Start column address
 *  param2  y0: Line where to display the character shape.
 *  param3  Ascii: Character ascii code
 *           This parameter must be a number between Min_Data = 0x20 and Max_Data = 0x7E 
 */
void
lcd_DisplayChar(uint16_t x0, uint16_t y0, uint8_t Ascii)
{
	sFONT    *pFont = DrawProp.pFont;
	DrawChar(x0, y0, &pFont->table[(Ascii-' ') * pFont->Height * ((pFont->Width + 7) / 8)]);
}

/*
 *  ʸ��������
 *  param1  x0: X position (in pixel)
 *  param2  y0: Y position (in pixel)   
 *  param3  Text: Pointer to string to display on LCD
 *  param4  Mode: Display mode
 */
void
lcd_DisplayStringAt(uint16_t x0, uint16_t y0, char *Text, Text_AlignModeTypdef Mode)
{
	sFONT    *pFont = DrawProp.pFont;
	uint16_t ref_column = 1, i = 0;
	uint32_t size = 0, xsize = 0; 
	uint8_t  *ptr = (uint8_t *)Text;

	/* Get the text size */
	while (*ptr++) size ++ ;

	/* Characters number per line */
	xsize = (DrawProp.hlcd->_width/pFont->Width);

	switch (Mode){
	case CENTER_MODE:
		ref_column = x0 + ((xsize - size)* pFont->Width) / 2;
		break;
	case LEFT_MODE:
		ref_column = x0;
		break;
	case RIGHT_MODE:
		ref_column = - x0 + ((xsize - size)*pFont->Width);
		break;
	default:
		ref_column = x0;
		break;
	}

	/* Check that the Start column is located in the screen */
	if ((ref_column < 1) || (ref_column >= 0x8000)){
		ref_column = 1;
	}

	/* Send the string character by character on LCD */
	while ((*Text != 0) & (((DrawProp.hlcd->_width - (i*pFont->Width)) & 0xFFFF) >= pFont->Width)){
		/* Display one character on LCD */
		lcd_DisplayChar(ref_column, y0, *Text);
		/* Decrement the column position by 16 */
		ref_column += pFont->Width;
		/* Point on the next character */
		Text++;
		i++;
	}
}

/*
 *  ����ե��å�ɽ���ƥ���
 */
void
grapics_test(LCD_Handler_t *hlcd)
{
	uint16_t x, y;
	int      i;

	DrawProp.BackColor = ST7735_BLACK;
	lcd_fillScreen(&DrawProp);
	dly_tsk(1000);
	lcd_drawPixel(hlcd, hlcd->_width/2, hlcd->_height/2, ST7735_GREEN);
	dly_tsk(1000);

	lcd_fillScreen(&DrawProp);
	for(y=0; y < hlcd->_height; y+=5) {
		lcd_drawFastHLine(hlcd, 0, y, hlcd->_width, ST7735_RED);
	}
	for (x=0; x < hlcd->_width; x+=5) {
    	lcd_drawFastVLine(hlcd, x, 0, hlcd->_height, ST7735_BLUE);
	}
	dly_tsk(1000);

	lcd_fillScreen(&DrawProp);
	DrawProp.TextColor = ST7735_GREEN;
	lcd_drawPolygon(&DrawProp, (Point *)testPolygon, sizeof(testPolygon)/sizeof(Point));
	dly_tsk(1000);

	lcd_fillScreen(&DrawProp);
	DrawProp.TextColor = ST7735_GREEN;
	for (x=0; x < hlcd->_width; x+=6) {
		lcd_drawRect(&DrawProp, hlcd->_width/2 -x/2, hlcd->_height/2 -x/2 , x, x);
	}
	for(i = 0 ; i < 3 ; i++){
		lcd_invertDisplay(hlcd, true);
		dly_tsk(500);
		lcd_invertDisplay(hlcd, false);
		dly_tsk(500);
	}
	lcd_fillScreen(&DrawProp);
	lcd_drawBitmap(hlcd, 33, 48, (uint8_t *)topamelogo);
	dly_tsk(1000);
}

/*
 *  �ᥤ�󥿥���
 */
void
main_task(intptr_t exinf)
{
#if defined(USE_SDM) || defined(USE_MSC_FILE)
	StorageDevice_t *psdev;
#endif
#ifdef USE_MSC_FILE
	USB_OTG_Init_t  USB_Data_Init;
	USB_OTG_Handle_t *husb;
	USBH_HandleTypeDef *husbh;
#endif
	SPI_Setup_t     SPI_Setup;
	SDCARD_Handler_t *hsd = NULL;
	LCD_Handler_t   *hlcd;
	SPI_Handle_t  *hspi;
#ifndef USE_SDM
	SDCARD_CardInfo_t CardInfo;
#endif
    FLGPTN  flgptn;
	ER_UINT	ercd;
	int sens = 1000;
	int i;

	SVC_PERROR(syslog_msk_log(LOG_UPTO(LOG_INFO), LOG_UPTO(LOG_EMERG)));
	syslog(LOG_NOTICE, "Sample program starts (exinf = %d).", (int_t) exinf);

	/*
	 *  ���ꥢ��ݡ��Ȥν����
	 *
	 *  �����ƥ����������Ʊ�����ꥢ��ݡ��Ȥ�Ȥ����ʤɡ����ꥢ��
	 *  �ݡ��Ȥ������ץ�Ѥߤξ��ˤϤ�����E_OBJ���顼�ˤʤ뤬���پ��
	 *  �ʤ���
	 */
	ercd = serial_opn_por(TASK_PORTID);
	if (ercd < 0 && MERCD(ercd) != E_OBJ) {
		syslog(LOG_ERROR, "%s (%d) reported by `serial_opn_por'.",
									itron_strerror(ercd), SERCD(ercd));
	}
	SVC_PERROR(serial_ctl_por(TASK_PORTID,
							(IOCTL_CRLF | IOCTL_FCSND | IOCTL_FCRCV)));

	syslog_0(LOG_NOTICE, "## START ##");
	SPI_Setup.port    = SPI1_PORTID;
	SPI_Setup.baud    = SPI_BAUDRATEPRESCALER_256;
	SPI_Setup.spi_sem = SPI1DMA_SEM;
	SPI_Setup.cs_pin  = CS_PORT;
	SPI_Setup.rs_pin  = RS_PORT;
	SPI_Setup.rst_pin = RST_PORT;
	SPI_Setup.cs2_pin = CS2_PORT;
	SPI_Setup.otype   = GPIO_OTYPE_PP;
	SPI_Setup.pullmode= GPIO_PULLUP;
	hspi = spi_start(&SPI_Setup);
	sdcard_setspi(SPISDCARD_PORTID, hspi);

	for(i = 0 ; i < 10 ; i++)
		aTxBuffer[i] = 0xFF;
	cs_set(PORT_HIGH);
	cs2_set(PORT_HIGH);
	if(spi_transmit(hspi, (uint8_t*)aTxBuffer, 10) != E_OK){
		/* Transfer error in transmission process */
		syslog_0(LOG_NOTICE, "## call Error_Handler(1) ##");
	}
#if SPI_WAIT_TIME == 0
	spi_wait(hspi, 50);
#endif
	dly_tsk(100);

#ifndef USE_SDM
	if(sdcard_init(SPISDCARD_PORTID)){
		hsd = sdcard_open(SPISDCARD_PORTID);
		syslog_1(LOG_NOTICE, "## card(%d) ##", hsd->cardtype);
		SVC_PERROR(sdcard_checkCID(hsd));
		SVC_PERROR(sdcard_sendCSD(hsd));
		SVC_PERROR(sdcard_getcardinfo(hsd, &CardInfo));
		SVC_PERROR(sdcard_configuration(hsd));
	}
	else
		syslog_0(LOG_NOTICE, "SD-CARD INITIAL ERROR !");
#else
	SDMSence_task(0);
	psdev = SDMGetStorageDevice(SDCARD_DEVNO);
	hsd = (SDCARD_Handler_t *)psdev->_sdev_local[1];
	SDeviceHead._get_datetime = get_fat_time;
#endif
	if(hsd != NULL)
		hsd->spi_lock = SPI1LOCK_SEM;
	spi_end(hspi);
	dly_tsk(100);
	SPI_Setup.baud = SPI_BAUDRATEPRESCALER_64;
	hspi = spi_start(&SPI_Setup);
#ifndef USE_SDM
	if(hsd != NULL && hsd->hspi != NULL){
		unsigned char *p;
		int j, bpb;
		int SecPerCls, Rsv, NumFat, Fsize, RootDirC, RootSec;
		for(i = 0 ; i < 1 ; i++){
			ercd = sdcard_blockread(hsd, sdbuffer, i*512, 512, 1);
			syslog_3(LOG_NOTICE, "## ercd(%d) i(%d) sdbuffer[%08x] ##", ercd, i, sdbuffer);
			dly_tsk(300);
			p = (unsigned char *)sdbuffer;
			if(i == 0){
				if(p[0x36] == 'F' && p[0x37] == 'A' && p[0x38] == 'T')
					bpb = 0;
				else{
					bpb = p[0x1c6];
					bpb |= p[0x1c7]<<8;
					bpb |= p[0x1c8]<<16;
					bpb |= p[0x1c9]<<24;
				}
			}
			printf("\nsec(%d) ", i);
			for(j = 0 ; j < 512 ; j++){
				if((j % 16) == 0)
					printf("\n%03x ", j);
				printf("%02x ", p[j]);
			}
			printf("\n");
		}
		syslog_1(LOG_NOTICE, "## bpb(%08x) ##", bpb);
		if(bpb != 0){
			ercd = sdcard_blockread(hsd, sdbuffer, bpb*512, 512, 1);
			syslog_3(LOG_NOTICE, "## ercd(%d) i(%d) sdbuffer[%08x] ##", ercd, bpb, sdbuffer);
			dly_tsk(300);
			printf("\nsec(%d) ", bpb);
			for(j = 0 ; j < 512 ; j++){
				if((j % 16) == 0)
					printf("\n%03x ", j);
				printf("%02x ", p[j]);
			}
			printf("\n");
		}
		p = (unsigned char *)sdbuffer;
		SecPerCls = p[13];
		Rsv = p[14] + (p[15] << 8);
		NumFat = p[16];
		RootDirC = p[17] + (p[18]<<8);
		Fsize = p[22] + (p[23] << 8);
		syslog_4(LOG_NOTICE, "## SecPerCls(%d) Rsv(%d) NumFat(%d) RootDirC(%d) ##", SecPerCls, Rsv, NumFat, RootDirC);
		RootSec = bpb + Rsv + (NumFat * Fsize);
		syslog_2(LOG_NOTICE, "## Fsize(%d) RootSec(%d) ##", Fsize, RootSec);

		for(i = 0 ; i < 2 ; i++){
			ercd = sdcard_blockread(hsd, sdbuffer, (i+RootSec)*512, 512, 1);
			syslog_3(LOG_NOTICE, "## ercd(%d) i(%d) sdbuffer[%08x] ##", ercd, (i+RootSec), sdbuffer);
			dly_tsk(300);
			p = (unsigned char *)sdbuffer;
			printf("\nsec(%d) ", (i+RootSec));
			for(j = 0 ; j < 512 ; j++){
				if((j % 16) == 0)
					printf("\n%03x ", j);
				printf("%02x ", p[j]);
			}
			printf("\n");
		}
	}
#endif
	(void)(hsd);

#ifdef USE_MSC_FILE
	/* Init USB OTG Hard */
	/* Set the LL Driver parameters */
    USB_Data_Init.usb_otg_mode = USB_OTG_MODE_HOST;
	USB_Data_Init.host_channels = 11;	/* HOST */
	USB_Data_Init.dma_enable = 0;
	USB_Data_Init.low_power_enable = 0;
	USB_Data_Init.phy_itface = USB_PHY_EMBEDDED; 
	USB_Data_Init.sof_enable = 0;
	USB_Data_Init.speed = USB_SPEED_FULL;
	USB_Data_Init.vbus_sensing_enable = 0;	/* HOST */
	USB_Data_Init.lpm_enable = 0;
	USB_Data_Init.use_external_vbus = 0;
	husb = usbo_init(USB1_PORTID, &USB_Data_Init);

	/*
	 *  USB HOST�ߥɥ륦��������
	 */
	husbh = (USBH_HandleTypeDef *)malloc(sizeof(USBH_HandleTypeDef));
	husbh->pSysData = husb;
	tusbhInit(husbh, USBH_UserProcess, 0);
	psdev = SDMGetStorageDevice(MSC_DEVNO);
	psdev->_sdev_local[1] = husbh;

	/*
	 *  USB HOST��������
	 */
	tusbhStart(husbh);
#endif


	hlcd = &LcdHandle;
	hlcd->hspi = hspi;
	hlcd->spi_lock= SPI1LOCK_SEM;
	hlcd->cs_pin  = CS_PORT;
	hlcd->rs_pin  = RS_PORT;
	hlcd->rst_pin = RST_PORT;
	hlcd->cs2_pin = CS2_PORT;
	DrawProp.hlcd = hlcd;
	lcd_initR(hlcd, INITR_BLACKTAB);
	DrawProp.BackColor = ST7735_BLACK;
	DrawProp.TextColor = ST7735_WHITE;
	grapics_test(hlcd);
#if 1	/* LOCAL DEFINE */
	DrawProp.TextColor = ST7735_BLACK;
	DrawProp.BackColor = ST7735_WHITE;
	lcd_fillScreen(&DrawProp);
	lcd_fillRect(hlcd, 0, 0, hlcd->_width, hlcd->_height/3, ST7735_BLUE);
	DrawProp.TextColor = ST7735_WHITE;
	DrawProp.BackColor = ST7735_BLUE;
	DrawProp.pFont = &Font20;
	lcd_DisplayStringAt(0, 20, "TBP TEST", CENTER_MODE);

	DrawProp.TextColor = ST7735_BLACK;
	DrawProp.BackColor = ST7735_WHITE;
	DrawProp.pFont = &Font8;
	lcd_DisplayStringAt(0, 80, "Helle World !", CENTER_MODE);
	DrawProp.pFont = &Font12;
	lcd_DisplayStringAt(0, 100, "Helle World !", CENTER_MODE);
	DrawProp.pFont = &Font16;
	lcd_DisplayStringAt(0, 120, "Helle World !", CENTER_MODE);

	DrawProp.TextColor = ST7735_RED;
	for(i = 20 ; i < 40 ; i++){
		lcd_DrawCircle(&DrawProp, hlcd->_width/2, hlcd->_height/2+30, i);
		if(i == 25)
			i += 10;
	}
#endif	/* LOCAL DEFINE */
	syslog_0(LOG_NOTICE, "LCD TEST END");
	while(sens > 0){
		wai_flg(SW_FLG, JS_MASK, TWF_ORW, &flgptn);
		syslog_1(LOG_NOTICE, "## JS EVENT[%04x] ##", flgptn);
		for(i = 0 ; i < NUM_JSPOSITION ; i++){
			if(flgptn & (1<<i))
				break;
		}
		if(i == NUM_JSPOSITION)
			i = 0;
#if 1	/* LOCAL DEFINE */
		DrawProp.pFont = &Font12;
		if(i == 0 || i == NUM_JSPOSITION)
			lcd_DisplayStringAt(0, 100, "Helle World !", CENTER_MODE);
		else
			lcd_DisplayStringAt(0, 100, (char *)pos_str[i-1], CENTER_MODE);
#else	/* LOCAL DEFINE */
		syslog_1(LOG_NOTICE, "## event(%d) ##", i);
#endif	/* LOCAL DEFINE */
		sens--;
	}
#ifdef USE_SDM
	syslog_0(LOG_NOTICE, "## STOP ##");
	slp_tsk();
#endif
	spi_end(hspi);

	syslog(LOG_NOTICE, "Sample program ends.");
//	SVC_PERROR(ext_ker());
}


/*
 *  ADCž����λ������Хå��ؿ�
 */
static void HAL_ADC_ConvCpltCallback2(ADC_Handle_t* hadc)
{
	isig_sem(ADCDMA_SEM);
}

/*
 *  ���祤���ƥ��å��ƻ륿����
 */
void stick_task(intptr_t exinf)
{
	ADC_Init_t     aInit;
	ADC_ChannelConf_t sConfig;
	ADC_Handle_t *hadc;
	uint32_t event = 0;
	int i;

	/*
	 *  ADC3�����
	 */
	aInit.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
	aInit.Resolution = ADC_RESOLUTION_12B;
	aInit.ScanConvMode = ADC_SCANMODE_DISABLE;
//	aInit.ContinuousConvMode = ADC_CONTINUOUS_ENABLE;
	aInit.ContinuousConvMode = ADC_CONTINUOUS_DISABLE;
	aInit.DiscontinuousConvMode = ADC_DISCONTINUOUS_DISABLE;
	aInit.NumDiscConversion = 0;
	aInit.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	aInit.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T1_CC1;
	aInit.DataAlign = ADC_DATAALIGN_RIGHT;
	aInit.NumConversion = 1;
	aInit.DMAContinuousRequests = ADC_DMACONTINUOUS_ENABLE;
	aInit.EOCSelection = ADC_EOC_SEQ_DISABLE;

	if((hadc = adc_init(ADC3_PORTID, &aInit)) == NULL){
		/* Initialization Error */
		syslog_0(LOG_NOTICE, "## adc_init ERROR ##");
	}
	hadc->xfercallback = HAL_ADC_ConvCpltCallback2;

	/*
	 *  ADC����ͥ�����
	 */
	sConfig.Channel = ADCx_CHANNEL;
	sConfig.Rank = 1;
//  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
	sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
	sConfig.GpioBase = ADCx_CHANNEL_GPIO_PORT;
	sConfig.GpioPin  = ADCx_CHANNEL_PINNO;

	if(adc_setupchannel(hadc, &sConfig) != E_OK){
		/* Channel Configuration Error */
		syslog_0(LOG_NOTICE, "## adc_setupchannel ERROR ##");
	}

	for(;;){
		uhADCxConvertedValue = 0;
		/*
		 *  ADC DMA��������
		 */
		if(adc_start_dma(hadc, (uint32_t*)&uhADCxConvertedValue, 1) != E_OK){
			/* Start Conversation Error */
			syslog_0(LOG_NOTICE, "## adc_start_dma ERROR ##");
		}
		while(hadc->status == ADC_STATUS_BUSY){
 			twai_sem(ADCDMA_SEM, 10);
		}
		adc_end_dma(hadc);
		for(i = 0 ; i < (NUM_JSPOSITION-1) ; i++){
			if(uhADCxConvertedValue < pos_tbl[i][0]){
				event = pos_tbl[i][1];
				set_flg(SW_FLG, event);
				break;
			}
		}
		if(i == 5 && event != JS_OFF){
			event = JS_OFF;
			set_flg(SW_FLG, JS_OFF);
		}
  		dly_tsk(50);
	}
	syslog_2(LOG_NOTICE, "## STOP[%08x] (%d) ##", &uhADCxConvertedValue, hadc->status);
	slp_tsk();
}

