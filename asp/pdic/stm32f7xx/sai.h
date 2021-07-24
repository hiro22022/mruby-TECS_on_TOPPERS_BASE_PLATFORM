/*
 *  TOPPERS/ASP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Advanced Standard Profile Kernel
 * 
 *  Copyright (C) 2008-2011 by Embedded and Real-Time Systems Laboratory
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
 *  @(#) $Id: sai.h 698 2016-03-26 16:37:57Z roi $
 */
/*
 * 
 *  STM32F7xx SAI�ǥХ����ɥ饤�Фγ������
 *
 */

#ifndef _SAI_H_
#define _SAI_H_

#ifdef __cplusplus
 extern "C" {
#endif


/*
 *  AUDIO�ݡ������
 */
#define AUDIO1_PORTID           1
#define AUDIO2_PORTID           2
#define NUM_AUDIOPORT           2

/*
 *  AUDIO�ϡ��ɥ��������깽¤��
 */
typedef struct _AUDIO_PortControlBlock{
	uint32_t                base;
	uint32_t                outgioclockbase;
	uint32_t                outgioclockbit;
	uint32_t                ingioclockbase;
	uint32_t                ingioclockbit;
	uint32_t                intgioclockbase;
	uint32_t                intgioclockbit;
	uint32_t                outdmaclockbase;
	uint32_t                outdmaclockbit;
	uint32_t                indmaclockbase;
	uint32_t                indmaclockbit;
	uint32_t                audioclockbase;
	uint32_t                audioclockbit;

	uint32_t                giofsobase;
	uint32_t                giomcobase;
	uint32_t                giosdibase;
	uint32_t                giointbase;
	uint32_t                outdmabase;
	uint32_t                outdmachannel;
	uint32_t                indmabase;
	uint32_t                indmachannel;
	uint8_t                 outmsckpin;
	uint8_t                 outfspin;
	uint8_t                 outsckin;
	uint8_t                 outsdpin;
	uint8_t                 insdpin;
	uint8_t                 inintpin;
	uint8_t                 outaf;
	uint8_t                 inaf;
} AUDIO_PortControlBlock;

/*
 *  AUDIO�������¤�� 
 */
typedef struct {
	uint32_t                AudioMode;			/* AUDIO�⡼�� */
	uint32_t                Synchro;			/* SAI�֥�å����󥯥��� */
	uint32_t                SynchroExt;			/* SAI�֥�å����󥯥��ĥ�� */
	uint32_t                OutputDrive;		/* ���ϥǥХ��� */
    uint32_t                NoDivider;			/* DIVIDERͭ��/̵�� */
	uint32_t                FIFOThreshold;		/* FIFO����å���ۡ�������� */
	uint32_t                AudioFrequency;		/* AUDIO ����ץ�󥰼��ȿ� */
	uint32_t                Mckdiv;				/* �ޥ���������å�DIVIDER�� */
	uint32_t                MonoStereoMode;		/* ��Υ���/���ƥ쥪�⡼�� */
	uint32_t                CompandingMode;		/* ����ѥ���ǥ��󥰥⡼�� */
	uint32_t                TriState;           /* TRIState�ޥ͡����������� */
	uint32_t                Protocol;			/* SAI BLOCK�ץ�ȥ��� */
	uint32_t                DataSize;			/* SAI BLOCK�ǡ��������� */
	uint32_t                FirstBit;			/* MSB/LSB���� */
	uint32_t                ClockStrobing;		/* ����å������ȥ��ӥ����� */

	/*
	 *  �ե졼��ǡ���
	 */
	uint32_t                FrameLength;		/* �ե졼��Ĺ */
	uint32_t                ActiveFrameLength;	/* �����ƥ��֡��ե졼��Ĺ */
	uint32_t                FSDefinition;		/* �ե졼�ࡦ���󥯥�ʥ���� */
	uint32_t                FSPolarity;         /* FS POLARITY���� */
	uint32_t                FSOffset;           /* FS OFFSET���� */

	/*
	 *  ����å����
	 */
	uint32_t                FirstBitOffset;		/* ����å���κǽ�Υǡ����ӥåȰ��� */
	uint32_t                SlotSize;			/* ����åȥ����� */
	uint32_t                SlotNumber;			/* ����å��ֹ� */
	uint32_t                SlotActive;			/* SLOT�����ƥ������� */
}SAI_Init_t;

/*
 *  AUDIO�ϥ�ɥ����
 */
typedef struct _AUDIO_HandleStruct AUDIO_Handle_t;
struct _AUDIO_HandleStruct
{
	uint32_t                base;			/* SAI�ǥХ����Υ١������ɥ쥹 */
	const AUDIO_PortControlBlock *pcb;		/* �ϡ��ɥ����������¤�ΤؤΥݥ��� */
	SAI_Init_t              OutInit;		/* ���ϥ֥�å�������� */
	SAI_Init_t              InInit;			/* ���ϥ֥�å�������� */
	uint8_t                 *pBuffPtr;		/* �ǡ����ΰ�ؤΥݥ��� */
	uint16_t                XferSize;		/* �ǡ��������� */
	uint16_t                XferCount;		/* ������������� */
	DMA_Handle_t            *hdmatx;		/* ����DMA�ϥ�ɥ�ؤΥݥ��� */
	DMA_Handle_t            *hdmarx;		/* ����DMA�ϥ�ɥ�ؤΥݥ��� */
    void        (*audiomutecallback)(AUDIO_Handle_t *haudio, uint32_t mode); 	/* MUTE������Хå��ؿ� */
    void        (*audioerrorcallback)(AUDIO_Handle_t *haudio, uint32_t mode); 	/* ����ߥ��顼������Хå��ؿ� */
	void        (*transcallback)(AUDIO_Handle_t *haudio);		/* ������λ������Хå��ؿ� */
	void        (*transhalfcallback)(AUDIO_Handle_t *haudio);	/* ������λ������Хå��ؿ� */
	void        (*recevcallback)(AUDIO_Handle_t *haudio);		/* ������λ������Хå��ؿ� */
	void        (*recevhalfcallback)(AUDIO_Handle_t *haudio);	/* ������λ������Хå��ؿ� */
	void        (*errorcallback)(AUDIO_Handle_t *haudio);		/* ���顼������Хå��ؿ� */
    volatile uint32_t       status[2];		/* AUDIO���ơ����� */
    volatile uint32_t       ErrorCode;		/* ���顼������ */
};


/*
 *  AUDIO����⡼�����
 */
#define AUDIO_OUT_BLOCK         0
#define AUDIO_IN_BLOCK          1

/*
 *  AUDIO���ơ��������
 */
#define AUDIO_STATUS_RESET      0x00		/* AUDIO�ǥХ����ꥻ�åȾ��� */
#define AUDIO_STATUS_READY      0x01		/* AUDIO�ǥХ�����ǥ����� */
#define AUDIO_STATUS_BUSY       0x02		/* AUDIO�ǥХ����ӥ��������� */
#define AUDIO_STATUS_BUSY_TX    0x12		/* AUDIO�ǥХ��������ӥ��������� */
#define AUDIO_STATUS_BUSY_RX    0x22		/* AUDIO�ǥХ��������ӥ��������� */
#define AUDIO_STATUS_TIMEOUT    0x03		/* AUDIO�ǥХ��������ॢ���Ⱦ��� */
#define AUDIO_STATUS_ERROR      0x04		/* AUDIO�ǥХ������顼���� */                                                                        

/*
 *  AUDIO���顼���
 */
#define AUDIO_ERROR_NONE        0x00000000		/* ���顼�ʤ� */
#define AUDIO_ERROR_OVRUDR      0x00000001		/* �����С����ޤ��ϥ��������� */
#define AUDIO_ERROR_AFSDET      0x00000004		/* Anticipated Frame synchronisation detection */
#define AUDIO_ERROR_LFSDET      0x00000008		/* Late Frame synchronisation detection        */
#define AUDIO_ERROR_WCKCFG      0x00000010		/* Wrong clock configuration */ 
#define AUDIO_ERROR_TIMEOUT     0x00000040		/* �����ॢ���� */

/*
 *  SAI�֥�å����󥯥��ĥ��(SynchroExt)
 */
#define SAI_SYNCEXT_DISABLE             0x00000000
#define SAI_SYNCEXT_IN_ENABLE           0x00000001
#define SAI_SYNCEXT_OUTBLOCKA_ENABLE    0x00000002
#define SAI_SYNCEXT_OUTBLOCKB_ENABLE    0x00000004

/*
 *  AUDIO�⡼��(AudioMode)
 */
#define SAI_MODEMASTER_TX               0x00000000
#define SAI_MODEMASTER_RX               SAI_xCR1_MODE_0
#define SAI_MODESLAVE_TX                SAI_xCR1_MODE_1
#define SAI_MODESLAVE_RX                (SAI_xCR1_MODE_1 | SAI_xCR1_MODE_0)

/*
 *  SAI BLOCK�ץ�ȥ���(Protocol)
 */
#define SAI_FREE_PROTOCOL               0x00000000
#define SAI_SPDIF_PROTOCOL              SAI_xCR1_PRTCFG_0
#define SAI_AC97_PROTOCOL               SAI_xCR1_PRTCFG_1

/*
 *  SAI BLOCK�ǡ���������(DataSize)
 */
#define SAI_DATASIZE_8                  SAI_xCR1_DS_1
#define SAI_DATASIZE_10                 (SAI_xCR1_DS_1 | SAI_xCR1_DS_0)
#define SAI_DATASIZE_16                 SAI_xCR1_DS_2
#define SAI_DATASIZE_20                 (SAI_xCR1_DS_2 | SAI_xCR1_DS_0)
#define SAI_DATASIZE_24                 (SAI_xCR1_DS_2 | SAI_xCR1_DS_1)
#define SAI_DATASIZE_32                 (SAI_xCR1_DS_2 | SAI_xCR1_DS_1 | SAI_xCR1_DS_0)

/*
 *  MSB/LSB����(FirstBit)
 */
#define SAI_FIRSTBIT_MSB                0x00000000
#define SAI_FIRSTBIT_LSB                SAI_xCR1_LSBFIRST

/*
 *  ����å������ȥ��ӥ�����(ClockStrobing)
 */
#define SAI_CLOCKSTROBING_FALLINGEDGE     ((uint32_t)0x00000000)
#define SAI_CLOCKSTROBING_RISINGEDGE      ((uint32_t)SAI_xCR1_CKSTR)

/*
 *  SAI�֥�å����󥯥���(Synchro)
 */
#define SAI_ASYNCHRONOUS                0x00000000
#define SAI_SYNCHRONOUS                 SAI_xCR1_SYNCEN_0
#define SAI_SYNCHRONOUS_EXT             SAI_xCR1_SYNCEN_1

/*
 *  ���ϥǥХ���(OutputDrive)
 */
#define SAI_OUTPUTDRIVE_DISABLE         0x00000000
#define SAI_OUTPUTDRIVE_ENABLE          SAI_xCR1_OUTDRIV

/*
 *  DIVIDERͭ��/̵��(NoDivider)
 */
#define SAI_MASTERDIVIDER_ENABLE        0x00000000
#define SAI_MASTERDIVIDER_DISABLE       SAI_xCR1_NODIV

/*
 *  �ե졼�ࡦ���󥯥�ʥ����(FSDefinition)
 */
#define SAI_FS_STARTFRAME               0x00000000
#define SAI_FS_CHANNEL_IDENTIFICATION   SAI_xFRCR_FSDEF

/*
 *  FS POLARITY����(FSPolarity)
 */
#define SAI_FS_ACTIVE_LOW               0x00000000
#define SAI_FS_ACTIVE_HIGH              SAI_xFRCR_FSPO

/*
 *  FS OFFSET����(FSOffset)
 */
#define SAI_FS_FIRSTBIT                 0x00000000
#define SAI_FS_BEFOREFIRSTBIT           SAI_xFRCR_FSOFF

/*
 *  SLOT����������(SlotSize)
 */
#define SAI_SLOTSIZE_DATASIZE           0x00000000
#define SAI_SLOTSIZE_16B                SAI_xSLOTR_SLOTSZ_0
#define SAI_SLOTSIZE_32B                SAI_xSLOTR_SLOTSZ_1

/*
 *  SLOT�����ƥ�������(SlotActive)
 */
#define SAI_SLOT_NOTACTIVE              0x00000000
#define SAI_SLOTACTIVE_0                0x00010000
#define SAI_SLOTACTIVE_1                0x00020000
#define SAI_SLOTACTIVE_2                0x00040000
#define SAI_SLOTACTIVE_3                0x00080000
#define SAI_SLOTACTIVE_4                0x00100000
#define SAI_SLOTACTIVE_5                0x00200000
#define SAI_SLOTACTIVE_6                0x00400000
#define SAI_SLOTACTIVE_7                0x00800000
#define SAI_SLOTACTIVE_8                0x01000000
#define SAI_SLOTACTIVE_9                0x02000000
#define SAI_SLOTACTIVE_10               0x04000000
#define SAI_SLOTACTIVE_11               0x08000000
#define SAI_SLOTACTIVE_12               0x10000000
#define SAI_SLOTACTIVE_13               0x20000000
#define SAI_SLOTACTIVE_14               0x40000000
#define SAI_SLOTACTIVE_15               0x80000000
#define SAI_SLOTACTIVE_ALL              0xFFFF0000

/*
 *  ��Υ���/���ƥ쥪�⡼��(MonoStereoMode)
 */
#define SAI_STEREOMODE                  0x00000000
#define SAI_MONOMODE                    SAI_xCR1_MONO

/*
 *  TRIState�ޥ͡�����������(TriState)
 */
#define SAI_OUTPUT_NOTRELEASED          0x00000000
#define SAI_OUTPUT_RELEASED             SAI_xCR2_TRIS

/*
 *  FIFO���쥷��ۡ��������(FIFOThreshold)
 */
#define SAI_FIFOTHRESHOLD_EMPTY         0x00000000
#define SAI_FIFOTHRESHOLD_1QF           SAI_xCR2_FTH_0
#define SAI_FIFOTHRESHOLD_HF            SAI_xCR2_FTH_1
#define SAI_FIFOTHRESHOLD_3QF           (SAI_xCR2_FTH_1 | SAI_xCR2_FTH_0)
#define SAI_FIFOTHRESHOLD_FULL          SAI_xCR2_FTH_2

/*
 *  ����ѥ���ǥ��󥰥⡼������(CompandingMode)
 */
#define SAI_NOCOMPANDING                0x00000000
#define SAI_ULAW_1CPL_COMPANDING        SAI_xCR2_COMP_1
#define SAI_ALAW_1CPL_COMPANDING        (SAI_xCR2_COMP_1 | SAI_xCR2_COMP_0)
#define SAI_ULAW_2CPL_COMPANDING        (SAI_xCR2_COMP_1 | SAI_xCR2_CPL)
#define SAI_ALAW_2CPL_COMPANDING        (SAI_xCR2_COMP_1 | SAI_xCR2_COMP_0 | SAI_xCR2_CPL)


extern AUDIO_Handle_t *audio_init(ID id);
extern void audio_deinit(AUDIO_Handle_t *haudio, uint32_t mode);
extern void audio_clockconfig(AUDIO_Handle_t *haudio, uint32_t AudioFreq, void *Params);

extern ER audio_start(AUDIO_Handle_t *haudio, uint32_t mode);
extern ER audio_end(AUDIO_Handle_t *haudio, uint32_t mode);
extern ER audio_transmit(AUDIO_Handle_t *haudio, uint8_t *pData, uint16_t Size);
extern ER audio_receive(AUDIO_Handle_t *haudio, uint8_t *pData, uint16_t Size);

extern ER audio_dmapause(AUDIO_Handle_t *haudio, uint32_t mode);
extern ER audio_dmaresume(AUDIO_Handle_t *haudio, uint32_t mode);
extern ER audio_dmastop(AUDIO_Handle_t *haudio, uint32_t mode);
extern void audio_enable(AUDIO_Handle_t *haudio, uint32_t mode);
extern ER audio_disable(AUDIO_Handle_t *haudio, uint32_t mode);
extern AUDIO_Handle_t *audio_gethandle(ID id);
extern uint32_t audio_status(AUDIO_Handle_t *haudio, uint32_t mode);
extern void audio_irqhandler(AUDIO_Handle_t *haudio);

#ifdef __cplusplus
}
#endif

#endif	/* _SAI_H_ */

