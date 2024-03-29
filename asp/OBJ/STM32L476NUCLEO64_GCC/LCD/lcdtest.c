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
 *  上記著作権者は，以下の(1)〜(4)の条件を満たす場合に限り，本ソフトウェ
 *  ア（本ソフトウェアを改変したものを含む．以下同じ）を使用・複製・改
 *  変・再配布（以下，利用と呼ぶ）することを無償で許諾する．
 *  (1) 本ソフトウェアをソースコードの形で利用する場合には，上記の著作
 *      権表示，この利用条件および下記の無保証規定が，そのままの形でソー
 *      スコード中に含まれていること．
 *  (2) 本ソフトウェアを，ライブラリ形式など，他のソフトウェア開発に使
 *      用できる形で再配布する場合には，再配布に伴うドキュメント（利用
 *      者マニュアルなど）に，上記の著作権表示，この利用条件および下記
 *      の無保証規定を掲載すること．
 *  (3) 本ソフトウェアを，機器に組み込むなど，他のソフトウェア開発に使
 *      用できない形で再配布する場合には，次のいずれかの条件を満たすこ
 *      と．
 *    (a) 再配布に伴うドキュメント（利用者マニュアルなど）に，上記の著
 *        作権表示，この利用条件および下記の無保証規定を掲載すること．
 *    (b) 再配布の形態を，別に定める方法によって，TOPPERSプロジェクトに
 *        報告すること．
 *  (4) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損
 *      害からも，上記著作権者およびTOPPERSプロジェクトを免責すること．
 *      また，本ソフトウェアのユーザまたはエンドユーザからのいかなる理
 *      由に基づく請求からも，上記著作権者およびTOPPERSプロジェクトを
 *      免責すること．
 * 
 *  本ソフトウェアは，無保証で提供されているものである．上記著作権者お
 *  よびTOPPERSプロジェクトは，本ソフトウェアに関して，特定の使用目的
 *  に対する適合性も含めて，いかなる保証も行わない．また，本ソフトウェ
 *  アの利用により直接的または間接的に生じたいかなる損害に関しても，そ
 *  の責任を負わない．
 * 
 *  $Id: lcdtest.c 2416 2016-02-12 12:05:32Z roi $
 */

/* 
 *  LCDシールド制御プログラムの本体
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
#include "spi.h"
#include "adc.h"
#include "pinmode.h"
#include "adafruit_st7735.h"
#include "lcdtest.h"
#include "topamelogo.h"

/*
 *  サービスコールのエラーのログ出力
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
 *  使用する変数の定義
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

#define NUM_ADCBUF   2
volatile uint16_t uhADCxConvertedValue[NUM_ADCBUF];

LCD_Handler_t  LcdHandle;
LCD_DrawProp_t DrawProp;


/*
 *  SW1割込み
 */
void sw_int(int arg)
{
	syslog_1(LOG_NOTICE, "## sw_int(%d) ##", arg);
}

/*
 *  GPIO設定関数
 */
static void
pinMode(uint8_t no, uint32_t mode)
{
	Arduino_PortControlBlock *pgcb = getGpioTable(DIGITAL_PIN, no);
	GPIO_Init_t GPIO_Init_Data;

	if(pgcb == NULL)
		return;
	pinClock(no);
	GPIO_Init_Data.mode  = mode;
	GPIO_Init_Data.pull  = GPIO_PULLUP;
	GPIO_Init_Data.otype = GPIO_OTYPE_PP;
	GPIO_Init_Data.speed = GPIO_SPEED_FAST;
	gpio_setup(pgcb->giobase, &GPIO_Init_Data, pgcb->giopin);
}

/*
 *  文字描画
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
 *  一文字描画
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
 *  文字列描画
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
 *  グラフィック表示テスト
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
 *  メインタスク
 */
void
main_task(intptr_t exinf)
{
	SPI_Init_t spi_initd;
	LCD_Handler_t   *hlcd;
	SPI_Handle_t  *hspi;
    FLGPTN  flgptn;
	ER_UINT	ercd;
	int sens = 1000;
	int i;

	SVC_PERROR(syslog_msk_log(LOG_UPTO(LOG_INFO), LOG_UPTO(LOG_EMERG)));
	syslog(LOG_NOTICE, "Sample program starts (exinf = %d).", (int_t) exinf);

	/*
	 *  シリアルポートの初期化
	 *
	 *  システムログタスクと同じシリアルポートを使う場合など，シリアル
	 *  ポートがオープン済みの場合にはここでE_OBJエラーになるが，支障は
	 *  ない．
	 */
	ercd = serial_opn_por(TASK_PORTID);
	if (ercd < 0 && MERCD(ercd) != E_OBJ) {
		syslog(LOG_ERROR, "%s (%d) reported by `serial_opn_por'.",
									itron_strerror(ercd), SERCD(ercd));
	}
	SVC_PERROR(serial_ctl_por(TASK_PORTID,
							(IOCTL_CRLF | IOCTL_FCSND | IOCTL_FCRCV)));

	syslog_0(LOG_NOTICE, "## START ##");
	/*
	 *  CS PORT(D10-PB6)
	 */
	pinMode(CS_PORT, GPIO_MODE_OUTPUT);
	/*
	 *  RS PORT(D8-PA9)
	 */
	pinMode(RS_PORT, GPIO_MODE_OUTPUT);
	/*
	 *  RST PORT(D9-PC7)
	 */
	pinMode(RST_PORT, GPIO_MODE_OUTPUT);
	/*
	 *  CS2 PORT(D4-PB5)
	 */
	pinMode(CS2_PORT, GPIO_MODE_OUTPUT);

	spi_initd.Prescaler     = SPI_BAUDRATEPRESCALER_8;
	spi_initd.Direction     = SPI_DIRECTION_2LINES;
	spi_initd.CLKPhase      = SPI_PHASE_1EDGE;
	spi_initd.CLKPolarity   = SPI_POLARITY_LOW;
	spi_initd.CRC           = SPI_CRC_DISABLE;
	spi_initd.CRCPolynomial = 7;
	spi_initd.CRCLength     = SPI_CRC_LENGTH_8BIT;
	spi_initd.DataSize      = SPI_DATASIZE_8BIT;
	spi_initd.SignBit       = SPI_DATA_MSB;
	spi_initd.NSS           = SPI_NSS_SOFT;
	spi_initd.TIMode        = SPI_TIMODE_DISABLE;
	spi_initd.Mode = SPI_MODE_MASTER;
	spi_initd.semid = SPI1DMA_SEM;
	spi_initd.semlock = 0;

	if((hspi = spi_init(SPI1_PORTID, &spi_initd)) == NULL){
		syslog_0(LOG_ERROR, "spi_start initialize error !");
		slp_tsk();
	}

	hlcd = &LcdHandle;
	hlcd->hspi = hspi;
	hlcd->spi_lock= 0;
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
	spi_deinit(hspi);

	syslog(LOG_NOTICE, "Sample program ends.");
//	SVC_PERROR(ext_ker());
}


/*
 *  ADC転送終了コールバック関数
 */
static void HAL_ADC_ConvCpltCallback2(ADC_Handle_t* hadc)
{
	isig_sem(ADCDMA_SEM);
}

/*
 *  ジョイスティック監視タスク
 */
void stick_task(intptr_t exinf)
{
	ADC_Init_t     aInit;
	ADC_ChannelConf_t sConfig;
	ADC_Handle_t *hadc;
	uint32_t event = 0;
	ER  ercd;
	int i;

	/*
	 *  ADC初期化
	 */
	aInit.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
	aInit.Resolution = ADC_RESOLUTION_12B;
	aInit.ScanConvMode = ADC_SCANMODE_DISABLE;
//	aInit.ContinuousConvMode = ADC_CONTINUOUS_ENABLE;
	aInit.ContinuousConvMode = ADC_CONTINUOUS_DISABLE;
	aInit.DiscontinuousConvMode = ADC_DISCONTINUOUS_DISABLE;
	aInit.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	aInit.ExternalTrigConv = ADC_SOFTWARE_START;
	aInit.DataAlign = ADC_DATAALIGN_RIGHT;
	aInit.DMAContinuousRequests = ADC_DMACONTINUOUS_ENABLE;
	aInit.NumConversion = 1;
	aInit.NumDiscConversion = 0;
	aInit.EOCSelection = ADC_EOC_SEQ_DISABLE;

	aInit.LowPowerAutoWait = ADC_LOWAUTOWAIT_DISABLE;
	aInit.Overrun = ADC_OVR_DATA_OVERWRITTEN;
	aInit.OversamplingMode = ADC_OVR_SAMPLING_DISABLE;
	aInit.OversamplingRatio = ADC_OVERSAMPLING_RATIO_2;
	aInit.OversamplingRightBitShift = ADC_RIGHTBITSHIFT_NONE;
	aInit.OversamplingTriggeredMode = ADC_TRIGGEREDMODE_SINGLE_TRIGGER;
	if((hadc = adc_init(ADC1_PORTID, &aInit)) == NULL){
		/* ADC初期化エラー */
		syslog_0(LOG_ERROR, "## adc_init ERROR ##");
		slp_tsk();
	}
	hadc->xfercallback = HAL_ADC_ConvCpltCallback2;

	/*
	 *  ADCチャネル初期化
	 */
	sConfig.Channel = ADCx_CHANNEL;
	sConfig.Rank = 1;
	sConfig.SamplingTime = ADC_SAMPLETIME_640CYCLES_5;
	sConfig.SingleDiff   = ADC_SINGLE_ENDED;            /* Single-ended input channel */
	sConfig.OffsetNumber = ADC_OFFSET_NONE;             /* No offset subtraction */ 
	sConfig.Offset = 0;                                 /* Parameter discarded because offset correction is disabled */
	sConfig.GpioBase = ADCx_CHANNEL_GPIO_PORT;
	sConfig.GpioPin  = ADCx_CHANNEL_PINNO;

	if((ercd = adc_setupchannel(hadc, &sConfig)) != E_OK){
		/* ADCチャネル初期化エラー */
		syslog_1(LOG_ERROR, "## adc_setupchannel ERROR(%d) ##", ercd);
		slp_tsk();
	}

	for(;;){
		uhADCxConvertedValue[0] = 0;
		/*
		 *  ADCスタート
		 */
		if((ercd = adc_start_dma(hadc, (uint16_t*)&uhADCxConvertedValue, 1)) != E_OK){
			/* Start Conversation Error */
			syslog_1(LOG_ERROR, "## adc_start_dma ERROR(%d) ##", ercd);
			slp_tsk();
		}
		while(hadc->status == ADC_STATUS_BUSY){
 			twai_sem(ADCDMA_SEM, 10);
		}
		adc_end_dma(hadc);
		for(i = 0 ; i < (NUM_JSPOSITION-1) ; i++){
			if(uhADCxConvertedValue[0] < pos_tbl[i][0]){
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

