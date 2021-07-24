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
 *  $Id: sdcard_play.h 2416 2016-04-24 09:42:51Z roi $
 */

/*
 *		�ӣ�-����� �У̣��٣ţҥץ����Υإå��ե�����
 */

#include <stdio.h>
/*
 *  �������åȰ�¸�����
 */
#include "target_test.h"
#include "storagedevice.h"
#include "device.h"
#include "i2c.h"
#include "sai.h"
#if _DRIVES > 1
#include "usb_otg.h"
#include "tusbh_base.h"
#include "mscdiskio.h"
#endif

/*
 *  �ƥ�������ͥ���٤����
 */
#define AUDIO_PRIORITY  2
#define MAIN_PRIORITY	7		/* �ᥤ�󥿥�����ͥ���� */
								/* HIGH_PRIORITY���⤯���뤳�� */

/*
 *  �������åȤ˰�¸�����ǽ���Τ�����������
 */

#ifndef TASK_PORTID
#define	TASK_PORTID		1			/* ʸ�����Ϥ��륷�ꥢ��ݡ���ID */
#endif /* TASK_PORTID */

#ifndef STACK_SIZE
#define	STACK_SIZE		8192		/* �������Υ����å������� */
#endif /* STACK_SIZE */

#define I2C_PORTID    I2C3_PORTID

#define INHNO_I2CEV   IRQ_VECTOR_I2C3_EV	/* ����ߥϥ�ɥ��ֹ� */
#define INTNO_I2CEV   IRQ_VECTOR_I2C3_EV	/* ������ֹ� */
#define INTPRI_I2CEV  -5			/* �����ͥ���� */
#define INTATR_I2CEV  0				/* �����°�� */

#define INHNO_I2CER   IRQ_VECTOR_I2C3_ER	/* ����ߥϥ�ɥ��ֹ� */
#define INTNO_I2CER   IRQ_VECTOR_I2C3_ER	/* ������ֹ� */
#define INTPRI_I2CER  -5			/* �����ͥ���� */
#define INTATR_I2CER  0				/* �����°�� */

#define INHNO_DMAARX  IRQ_VECTOR_DMA2_STREAM7	/* ����ߥϥ�ɥ��ֹ� */
#define INTNO_DMAARX  IRQ_VECTOR_DMA2_STREAM7	/* ������ֹ� */
#define INTPRI_DMAARX -4			/* �����ͥ���� */
#define INTATR_DMAARX 0				/* �����°�� */

#define INHNO_DMAATX  IRQ_VECTOR_DMA2_STREAM4	/* ����ߥϥ�ɥ��ֹ� */
#define INTNO_DMAATX  IRQ_VECTOR_DMA2_STREAM4	/* ������ֹ� */
#define INTPRI_DMAATX -4			/* �����ͥ���� */
#define INTATR_DMAATX 0				/* �����°�� */

#define INHNO_USBHS   IRQ_VECTOR_OTG_HS	/* ����ߥϥ�ɥ��ֹ� */
#define INTNO_USBHS   IRQ_VECTOR_OTG_HS	/* ������ֹ� */
#define INTPRI_USBHS  -5			/* �����ͥ���� */
#define INTATR_USBHS  0				/* �����°�� */

#define INHNO_HSWKUP  IRQ_VECTOR_OTG_HS_WKUP	/* ����ߥϥ�ɥ��ֹ� */
#define INTNO_HSWKUP  IRQ_VECTOR_OTG_HS_WKUP	/* ������ֹ� */
#define INTPRI_HSWKUP -10			/* �����ͥ���� */
#define INTATR_HSWKUP 0				/* �����°�� */

#define NUM_USBH_EVT1   64
#define NUM_USBH_EVT2   16

#define NUM_FILENAME    256
#define NUM_FILEMAX     256
#define MAX_PATH        256
#define PPM_OUTSIZE     (1024*1024)


#define MUSIC_DATA_SIZE (512*1024)
#define WAV_BUFFER_SIZE (64*1024-4)

#define SET_BUFFER0     (1<<0)
#define SET_BUFFER1     (1<<1)
#define SET_WAV         (1<<4)
#define SET_MP3         (1<<5)
#define SET_BUFFER      (SET_BUFFER0 | SET_BUFFER1)

#define P6_HEAD_SIZE    15
#define BMP_HEAD_SIZE   54

/*
 *  disp_status��������
 */
#define CLOCK_APL       (1<<0)
#define MEDIA_APL       (1<<1)
#define AUDIO_APL       (1<<2)
#define JPEG_APL        (1<<3)
#define ALL_APL         (CLOCK_APL | MEDIA_APL | AUDIO_APL | JPEG_APL)

#define MENU_SHIFT      8
#define ACTSHIFT        16
#define OPTSHIFT        24

/*
 *  ���ڹ�¤�ΤΥ��ơ���������
 */
#define AUDIO_ACTIVE    (1<<0)
#define AUDIO_PHASE     (1<<4)
#define AUDIO_STOP      (1<<5)


#ifndef TOPPERS_MACRO_ONLY

typedef enum {
	AUDIO_OUT_DEVICE_SPEAKER,
	AUDIO_OUT_DEVICE_HEADPHONE,
	AUDIO_OUT_DEVICE_BOTH,
	AUDIO_OUT_DEVICE_AUTO,
	AUDIO_IN_DEVICE,
	AUDIO_IN_DEVICE_DIGITAL_MICROPHONE_1,
	AUDIO_IN_DEVICE_DIGITAL_MICROPHONE_2,
	AUDIO_IN_DEVICE_INPUT_LINE_1,
	AUDIO_IN__DEVICE_INPUT_LINE_2
} AudioDevice;

typedef enum {
	POWERDOWN_HW,
	POWERDOWN_SW
} CodecPowerDown;


/*
 *  ����ǥХ����ι�¤�����
 */
typedef struct {
	uint8_t *head;
	uint32_t csize;
	uint32_t fsize;
} MEMDEV;

#define FILE_ACT              (1<<0)
#define FILE_JPEG             (1<<1)
#define FILE_WAV              (1<<2)
#define FILE_MP3              (1<<3)
#define FILE_EXE              (FILE_JPEG | FILE_WAV | FILE_MP3)

/*
 *  �ե����륤��ե��᡼�����¤��
 */
typedef struct {
	uint32_t devno;
	uint32_t tsize;
	uint32_t fsize;
	uint32_t num_file;
	uint32_t num_jpeg;
	struct dirent2 finfo[NUM_FILEMAX];
} FILE_INFO;

/*
 *  ���ڥե����빽¤��
 */
typedef struct {
	uint32_t       status;
	AUDIO_Handle_t *haudio;
	uint8_t        *maddr;
	uint8_t        *buffer;
	uint8_t        *start;
	FILE           *fileid;
	uint32_t       datasize;
	uint32_t       transize;
	uint32_t       buffsize;
	uint32_t       length;
	uint32_t       playmode;
} MUSIC_INFO;

/*
 *  ����ž����¤��
 */
typedef struct _MUSIC_TRANS MUSIC_TRANS;
struct _MUSIC_TRANS {
	MUSIC_INFO     *minfo;
	void (*func)(MUSIC_TRANS*);
	uint32_t       mcount;
	uint32_t       mhead;
	uint32_t       mtail;
	uint32_t       msize;
	uint32_t       datasize;
	uint32_t       transize;
	uint32_t       samplerate;
	uint32_t       channels;		/* number of channels */
};

/*
 *  WAV RIFF HEADER
 */
typedef struct {
	char     riff[4];
	uint32_t file_size;
	char     id[4];
	char     chank_fmt[4];
	uint32_t chank_size;
	uint16_t formatid;
	uint16_t channels;
	uint32_t audiofreq;
	uint32_t bytepersec;
	uint16_t blocksize;
	uint16_t bitswidth;
} Riff_Header;

/*
 *  WAV DATA HEADER
 */
typedef struct {
	char     id[4];
	uint32_t data_size;
} Data_Header;


#define BCOPY(a, b, c)  bcopy2((uint8_t *)(a), (uint8_t *)(b), c)

/*
 *  �ҡ����ΰ������
 */
extern uint32_t heap_param[2];

/*
 *  �ؿ��Υץ�ȥ��������
 */
extern void	main_task(intptr_t exinf);
extern void	audio_task(intptr_t exinf);
extern void device_info_init(intptr_t exinf);
extern void heap_init(intptr_t exinf);
extern void sw_handle(void);

extern void bcopy2(uint8_t *s, uint8_t *d, int len);
extern MEMDEV *setup_memory_file(FILE *st, uint8_t *addr, uint32_t size);
extern int jpeg2ppm(const char *infile, uint8_t *buffer, uint32_t size);
extern int ppm2rgb(uint8_t *rbgaddr, int *pwidth, int *pheight);

extern MUSIC_INFO *AUDIO_Hard_Init(ID id);
extern ER AUDIO_OUT_Init(AUDIO_Handle_t *haudio, AudioDevice Dev, uint8_t Volume, uint32_t AudioFreq, uint16_t framesize);
extern ER AUDIO_OUT_Play(AUDIO_Handle_t *haudio, uint16_t* pBuffer, uint32_t Size, uint32_t mode);
extern ER BSP_AUDIO_OUT_Pause(AUDIO_Handle_t *haudio);
extern ER BSP_AUDIO_OUT_Resume(AUDIO_Handle_t *haudio);
extern ER AUDIO_OUT_Stop(AUDIO_Handle_t *haudio, CodecPowerDown Option);
extern ER wav_transfar(MUSIC_TRANS *mtrans);
extern ER mp3_decode(MUSIC_TRANS *mtrans);

#endif /* TOPPERS_MACRO_ONLY */
