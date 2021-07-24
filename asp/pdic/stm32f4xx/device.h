/*
 *  TOPPERS/ASP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Advanced Standard Profile Kernel
 * 
 *  Copyright (C) 2008-2011 by Embedded and Real-Time Systems Laboratory
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
 *  Copyright (C) 2015-2016 by TOPPERS PROJECT Educational Working Group.
 * 
 *  ��L���쌠�҂́C�ȉ���(1)�`(4)�̏����𖞂����ꍇ�Ɍ���C�{�\�t�g�E�F
 *  �A�i�{�\�t�g�E�F�A�����ς������̂��܂ށD�ȉ������j���g�p�E�����E��
 *  �ρE�Ĕz�z�i�ȉ��C���p�ƌĂԁj���邱�Ƃ𖳏��ŋ�������D
 *  (1) �{�\�t�g�E�F�A���\�[�X�R�[�h�̌`�ŗ��p����ꍇ�ɂ́C��L�̒���
 *      ���\���C���̗��p��������щ��L�̖��ۏ؋K�肪�C���̂܂܂̌`�Ń\�[
 *      �X�R�[�h���Ɋ܂܂�Ă��邱�ƁD
 *  (2) �{�\�t�g�E�F�A���C���C�u�����`���ȂǁC���̃\�t�g�E�F�A�J���Ɏg
 *      �p�ł���`�ōĔz�z����ꍇ�ɂ́C�Ĕz�z�ɔ����h�L�������g�i���p
 *      �҃}�j���A���Ȃǁj�ɁC��L�̒��쌠�\���C���̗��p��������щ��L
 *      �̖��ۏ؋K����f�ڂ��邱�ƁD
 *  (3) �{�\�t�g�E�F�A���C�@��ɑg�ݍ��ނȂǁC���̃\�t�g�E�F�A�J���Ɏg
 *      �p�ł��Ȃ��`�ōĔz�z����ꍇ�ɂ́C���̂����ꂩ�̏����𖞂�����
 *      �ƁD
 *    (a) �Ĕz�z�ɔ����h�L�������g�i���p�҃}�j���A���Ȃǁj�ɁC��L�̒�
 *        �쌠�\���C���̗��p��������щ��L�̖��ۏ؋K����f�ڂ��邱�ƁD
 *    (b) �Ĕz�z�̌`�Ԃ��C�ʂɒ�߂���@�ɂ���āCTOPPERS�v���W�F�N�g��
 *        �񍐂��邱�ƁD
 *  (4) �{�\�t�g�E�F�A�̗��p�ɂ�蒼�ړI�܂��͊ԐړI�ɐ����邢���Ȃ鑹
 *      �Q������C��L���쌠�҂����TOPPERS�v���W�F�N�g��Ɛӂ��邱�ƁD
 *      �܂��C�{�\�t�g�E�F�A�̃��[�U�܂��̓G���h���[�U����̂����Ȃ闝
 *      �R�Ɋ�Â�����������C��L���쌠�҂����TOPPERS�v���W�F�N�g��
 *      �Ɛӂ��邱�ƁD
 * 
 *  �{�\�t�g�E�F�A�́C���ۏ؂Œ񋟂���Ă�����̂ł���D��L���쌠�҂�
 *  ���TOPPERS�v���W�F�N�g�́C�{�\�t�g�E�F�A�Ɋւ��āC����̎g�p�ړI
 *  �ɑ΂���K�������܂߂āC�����Ȃ�ۏ؂��s��Ȃ��D�܂��C�{�\�t�g�E�F
 *  �A�̗��p�ɂ�蒼�ړI�܂��͊ԐړI�ɐ����������Ȃ鑹�Q�Ɋւ��Ă��C��
 *  �̐ӔC�𕉂�Ȃ��D
 * 
 *  @(#) $Id: device.h 698 2017-11-03 12:49:28Z roi $
 */
/*
 * 
 *  STM32F4XX�p�f�o�C�X�h���C�o�̊O���錾
 *
 */

#ifndef _DEVICE_H_
#define _DEVICE_H_

#ifdef __cplusplus
 extern "C" {
#endif

/*
 *  �o�[�W�������
 */
#define	TPLATFORM_PRVER 0x1030		/* �v���b�g�t�H�[���̃o�[�W�����ԍ� */

/*
 * �s���ݒ�
 */

#define PINPOSITION0    0
#define PINPOSITION1    1
#define PINPOSITION2    2
#define PINPOSITION3    3
#define PINPOSITION4    4
#define PINPOSITION5    5
#define PINPOSITION6    6
#define PINPOSITION7    7
#define PINPOSITION8    8
#define PINPOSITION9    9
#define PINPOSITION10   10
#define PINPOSITION11   11
#define PINPOSITION12   12
#define PINPOSITION13	13
#define PINPOSITION14	14
#define PINPOSITION15	15

#define GPIO_PIN_0      (1<<PINPOSITION0)
#define GPIO_PIN_1      (1<<PINPOSITION1)
#define GPIO_PIN_2      (1<<PINPOSITION2)
#define GPIO_PIN_3      (1<<PINPOSITION3)
#define GPIO_PIN_4      (1<<PINPOSITION4)
#define GPIO_PIN_5      (1<<PINPOSITION5)
#define GPIO_PIN_6      (1<<PINPOSITION6)
#define GPIO_PIN_7      (1<<PINPOSITION7)
#define GPIO_PIN_8      (1<<PINPOSITION8)
#define GPIO_PIN_9      (1<<PINPOSITION9)
#define GPIO_PIN_10     (1<<PINPOSITION10)
#define GPIO_PIN_11     (1<<PINPOSITION11)
#define GPIO_PIN_12     (1<<PINPOSITION12)
#define GPIO_PIN_13     (1<<PINPOSITION13)
#define GPIO_PIN_14     (1<<PINPOSITION14)
#define GPIO_PIN_15     (1<<PINPOSITION15)

/*
 *  GPIO���[�h�p�����[�^
 */
#define GPIO_MODE_INPUT     0x00000000	/* Input Floating Mode */
#define GPIO_MODE_OUTPUT    0x00000001	/* Output Mode */
#define GPIO_MODE_AF        0x00000002	/* Alternate Function Mode */
#define GPIO_MODE_ANALOG    0x00000003	/* Analog Mode  */

/*
 *  GPIO-EXTI���[�h�p�����[�^
 */
#define GPIO_MODE_IT_RISING             0x10110000	/* External Interrupt Mode with Rising edge trigger detection */
#define GPIO_MODE_IT_FALLING            0x10210000	/* External Interrupt Mode with Falling edge trigger detection */
#define GPIO_MODE_IT_RISING_FALLING     0x10310000	/* External Interrupt Mode with Rising/Falling edge trigger detection */
#define GPIO_MODE_EVT_RISING            0x10120000	/* External Event Mode with Rising edge trigger detection */
#define GPIO_MODE_EVT_FALLING           0x10220000	/* External Event Mode with Falling edge trigger detection */
#define GPIO_MODE_EVT_RISING_FALLING    0x10320000	/* External Event Mode with Rising/Falling edge trigger detection */

/*
 *  GPIO�A�E�g�v�b�g�ݒ�p�����[�^
 */
#define GPIO_OTYPE_PP       0x0
#define GPIO_OTYPE_OD       0x1

/*
 *  GPIO�A�E�g�v�b�g�ő���g���p�����[�^
 */
#define GPIO_SPEED_LOW      0x00000000	/* Low speed     */
#define GPIO_SPEED_MEDIUM   0x00000001	/* Medium speed  */
#define GPIO_SPEED_FAST     0x00000002	/* Fast speed    */
#define GPIO_SPEED_HIGH     0x00000003	/* High speed    */

/*
 *  GPIO�v���A�b�v�_�E���p�����[�^
 */
#define GPIO_NOPULL         0x00000000	/* No Pull-up or Pull-down activation  */
#define GPIO_PULLUP         0x00000001	/* Pull-up activation                  */
#define GPIO_PULLDOWN       0x00000002	/* Pull-down activation                */

/*
 *  GPIO�������ݒ�
 */
typedef struct
{
    uint32_t    mode;		/* specifies the operating mode for the selected pins. */
	uint32_t    pull;		/* specifies the Pull-up or Pull-Down */
	uint32_t    otype;		/* output type */
	uint32_t    speed;		/* speed for the selected pins. */
	uint32_t    alternate;	/* alternate for the selected pins. */
}GPIO_Init_t;

typedef struct {
	uint32_t    base;
	uint32_t    pinmap;
} GPIO_Init_Table;

extern void gpio_setup(uint32_t base, GPIO_Init_t *init, uint32_t pin);

/*
 *  DMA�X�e�[�^�X��`
 */
#define DMA_STATUS_BUSY         0x00000001	/* BUSY */
#define DMA_STATUS_READY_HMEM0  0x00000002	/* DMA Mem0 Half process success */
#define DMA_STATUS_READY_HMEM1  0x00000004	/* DMA Mem1 Half process success */
#define DMA_STATUS_READY_MEM0   0x00000008	/* DMA Mem0 process success      */
#define DMA_STATUS_READY_ERROR  0x00000100	/* DMA Error end */

/*
 *  DMA�G���[��`
 */ 
#define DMA_ERROR_NONE          0x00000000	/* No error */
#define DMA_ERROR_TE            0x00000001	/* Transfer error */
#define DMA_ERROR_FE            0x00000002	/* FIFO error */
#define DMA_ERROR_DME           0x00000004	/* Direct Mode error */
#define DMA_ERROR_TIMEOUT       0x00000020	/* Timeout error */

/*
 *  DMA�`���l����`
 */
#define DMA_CHANNEL_0           0x00000000	/* DMA Channel 0 */
#define DMA_CHANNEL_1           0x02000000	/* DMA Channel 1 */
#define DMA_CHANNEL_2           0x04000000	/* DMA Channel 2 */
#define DMA_CHANNEL_3           0x06000000	/* DMA Channel 3 */
#define DMA_CHANNEL_4           0x08000000	/* DMA Channel 4 */
#define DMA_CHANNEL_5           0x0A000000	/* DMA Channel 5 */
#define DMA_CHANNEL_6           0x0C000000	/* DMA Channel 6 */
#define DMA_CHANNEL_7           0x0E000000	/* DMA Channel 7 */

/*
 *  DMA�X�g���[��ID��`
 */
#define DMA1STM0_SID            (0)
#define DMA1STM1_SID            (1)
#define DMA1STM2_SID            (2)
#define DMA1STM3_SID            (3)
#define DMA1STM4_SID            (4)
#define DMA1STM5_SID            (5)
#define DMA1STM6_SID            (6)
#define DMA1STM7_SID            (7)
#define DMA2STM0_SID            (8+0)
#define DMA2STM1_SID            (8+1)
#define DMA2STM2_SID            (8+2)
#define DMA2STM3_SID            (8+3)
#define DMA2STM4_SID            (8+4)
#define DMA2STM5_SID            (8+5)
#define DMA2STM6_SID            (8+6)
#define DMA2STM7_SID            (8+7)

/*
 *  DMA�]��������`
 */
#define DMA_PERIPH_TO_MEMORY    0x00000000		/* Peripheral to memory direction */
#define DMA_MEMORY_TO_PERIPH    DMA_SxCR_DIR_0	/* Memory to peripheral direction */
#define DMA_MEMORY_TO_MEMORY    DMA_SxCR_DIR_1	/* Memory to memory direction     */

/*
 *  DMA�y���t�F�����������[�h��`
 */
#define DMA_PINC_ENABLE         DMA_SxCR_PINC	/* Peripheral increment mode enable  */
#define DMA_PINC_DISABLE        0x00000000		/* Peripheral increment mode disable */

/*
 *  DMA�������������[�h��`
 */ 
#define DMA_MINC_ENABLE         DMA_SxCR_MINC	/* Memory increment mode enable  */
#define DMA_MINC_DISABLE        0x00000000		/* Memory increment mode disable */

/*
 *  DMA�y���t�F�����E�f�[�^�E�T�C�Y��`
 */
#define DMA_PDATAALIGN_BYTE     0x00000000		/* Peripheral data alignment: Byte     */
#define DMA_PDATAALIGN_HALFWORD DMA_SxCR_PSIZE_0	/* Peripheral data alignment: HalfWord */
#define DMA_PDATAALIGN_WORD     DMA_SxCR_PSIZE_1	/* Peripheral data alignment: Word     */

/*
 *  DMA�������E�f�[�^�E�T�C�Y��`
 */
#define DMA_MDATAALIGN_BYTE     0x00000000		/* Memory data alignment: Byte     */
#define DMA_MDATAALIGN_HALFWORD DMA_SxCR_MSIZE_0	/* Memory data alignment: HalfWord */
#define DMA_MDATAALIGN_WORD     DMA_SxCR_MSIZE_1	/* Memory data alignment: Word     */

/*
 *  DMA���[�h��`
 */
#define DMA_NORMAL              0x00000000		/* Normal mode                  */
#define DMA_CIRCULAR            DMA_SxCR_CIRC	/* Circular mode                */
#define DMA_PFCTRL              DMA_SxCR_PFCTRL	/* Peripheral flow control mode */

/*
 *  DMA�D��x���x����`
 */
#define DMA_PRIORITY_LOW        0x00000000		/* Priority level: Low       */
#define DMA_PRIORITY_MEDIUM     DMA_SxCR_PL_0	/* Priority level: Medium    */
#define DMA_PRIORITY_HIGH       DMA_SxCR_PL_1	/* Priority level: High      */
#define DMA_PRIORITY_VERY_HIGH  DMA_SxCR_PL		/* Priority level: Very High */

/*
 *  DMA FIFO���[�h��`
 */
#define DMA_FIFOMODE_DISABLE    0x00000000		/* FIFO mode disable */
#define DMA_FIFOMODE_ENABLE     DMA_SxFCR_DMDIS	/* FIFO mode enable  */

/*
 *  DMA FIFO�X���b�V���z�[���h���x����`
 */
#define DMA_FIFO_THRESHOLD_1QUARTERFULL  0x00000000			/* FIFO threshold 1 quart full configuration  */
#define DMA_FIFO_THRESHOLD_HALFFULL      DMA_SxFCR_FTH_0	/* FIFO threshold half full configuration     */
#define DMA_FIFO_THRESHOLD_3QUARTERSFULL DMA_SxFCR_FTH_1	/* FIFO threshold 3 quarts full configuration */
#define DMA_FIFO_THRESHOLD_FULL          DMA_SxFCR_FTH		/* FIFO threshold full configuration          */

/*
 *  DMA�������o�[�X�g��`
 */
#define DMA_MBURST_SINGLE       0x00000000
#define DMA_MBURST_INC4         DMA_SxCR_MBURST_0
#define DMA_MBURST_INC8         DMA_SxCR_MBURST_1
#define DMA_MBURST_INC16        DMA_SxCR_MBURST

/*
 *  DMA�y���t�F�����o�[�X�g��`
 */
#define DMA_PBURST_SINGLE       0x00000000
#define DMA_PBURST_INC4         DMA_SxCR_PBURST_0
#define DMA_PBURST_INC8         DMA_SxCR_PBURST_1
#define DMA_PBURST_INC16        DMA_SxCR_PBURST

/*
 *  DMA�������\���̒�`
 */
typedef struct
{
	uint32_t              Channel;		/* Specifies the channel used for the specified stream. */
	uint32_t              Direction;	/* Specifies if the data will be transferred from memory to peripheral */
	uint32_t              PeriphInc;	/* Specifies whether the Peripheral address register should be incremented or not. */
	uint32_t              MemInc;		/* Specifies whether the memory address register should be incremented or not. */
	uint32_t              PeriphDataAlignment;	/* Specifies the Peripheral data width. */
	uint32_t              MemDataAlignment;		/* Specifies the Memory data width. */
	uint32_t              Mode;			/* Specifies the operation mode of the DMAy Streamx. */
	uint32_t              Priority;		/* Specifies the software priority for the DMAy Streamx. */
	uint32_t              FIFOMode;		/* Specifies if the FIFO mode or Direct mode will be used for the specified stream. */
	uint32_t              FIFOThreshold;		/* Specifies the FIFO threshold level. */
	uint32_t              MemBurst;		/* Specifies the Burst transfer configuration for the memory transfers. */
	uint32_t              PeriphBurst;	/* Specifies the Burst transfer configuration for the peripheral transfers. */
}DMA_Init_t;

/*
 *  DMA�n���h���\���̒�`
 */
typedef struct __DMA_Handle_t DMA_Handle_t;
struct __DMA_Handle_t
{
	uint32_t              base;
	DMA_Init_t            Init;			/* DMA communication parameters */
	uint32_t              sdid;			/* stream dma id */
	volatile uint32_t     status;		/* DMA status */
	void                  (*xfercallback)(DMA_Handle_t * hdma);		/* DMA transfer complete callback */
	void                  (*xferhalfcallback)(DMA_Handle_t * hdma);	/* DMA Half transfer complete callback */
	void                  (*xferm1callback)(DMA_Handle_t * hdma);	/* DMA transfer complete Memory1 callback */
	void                  (*errorcallback)(DMA_Handle_t * hdma);	/* DMA transfer error callback */
	volatile uint32_t     ErrorCode;	/* DMA Error code */
	void                  *localdata;	/* DMA local data */
};

extern ER dma_init(DMA_Handle_t *hdma);
extern ER dma_deinit(DMA_Handle_t *hdma);
extern ER dma_start(DMA_Handle_t *hdma, uint32_t SrcAddress, uint32_t DstAddress, uint32_t DataLength);
extern ER dma_end(DMA_Handle_t *hdma);
extern void dma_inthandler(DMA_Handle_t *hdma);
extern void stream_dma_isr(intptr_t exinf);


/*
 * �|�[�g��LED�ڑ��r�b�g
 */
#if defined(TOPPERS_STM32F401_NUCLEO) || defined(TOPPERS_STM32F446_NUCLEO64)
#if defined(PSHIELD)
#define LEDN3           PINPOSITION6
#define LEDN4           PINPOSITION7
#else
#define LEDN3           PINPOSITION5
#define LEDN4           PINPOSITION12
#endif
#elif defined(TOPPERS_STM32F446_NUCLEO144)
#define LEDN3           PINPOSITION0
#define LEDN4           PINPOSITION7
#else
#define LEDN3           PINPOSITION13
#define LEDN4           PINPOSITION12
#endif
#define LEDN5           PINPOSITION15
#define LEDN6           PINPOSITION14

/*
 * LED�ڑ��r�b�g
 */
#define LED01           (1<<LEDN3)
#define LED02           (1<<LEDN4)
#if defined(TOPPERS_STM32F4_DISCOVERY)
#define LED03           (1<<LEDN5)
#define LED04           (1<<LEDN6)
#elif defined(TOPPERS_STM32F446_NUCLEO144)
#define LED03           (1<<LEDN6)
#define LED04           0
#else
#define LED03           0
#define LED04           0
#endif

#define LED_MASK        (LED01 | LED02 | LED03 | LED04)

/*
 * LED�ƃX�C�b�`�̏�ԃ}�N��
 */
#define ON              1     /* LED��X�C�b�`ON���             */
#define OFF             0     /* LED��X�C�b�`OFF���            */

/*
 * DIPSW�ݒ�
 */
#define DSW1             0x01
#define DSW2             0x02
#define DSW3             0x04
#define DSW4             0x08

/*
 *  LED�ڑ��|�[�g������
 */ 
extern void led_init(intptr_t exinf);

/*
 *  LED�ڑ��|�[�g�ǂݏo��
 */
uint_t led_in(void);

/*
 *  LED�ڑ��|�[�g��������
 */ 
void led_out(unsigned short led_data);

/*
 * LED�ƃX�C�b�`�̌ʐݒ�E�ǂݍ��݊֐��Q
 */
extern void set_led_state(unsigned short led, unsigned char state);


#if defined(TOPPERS_STM32F401_NUCLEO) || defined(TOPPERS_STM32F446_NUCLEO64)
#if defined(PSHIELD)
#define INHNO_SW1   IRQ_VECTOR_EXTI4	/* �����݃n���h���ԍ� */
#define INTNO_SW1   IRQ_VECTOR_EXTI4	/* �����ݔԍ� */
#define NUM_EXTI_FUNC  1
#define EXTI_BASENO    4
#else
#define INHNO_SW1   IRQ_VECTOR_EXTI15_10 /* �����݃n���h���ԍ� */
#define INTNO_SW1   IRQ_VECTOR_EXTI15_10 /* �����ݔԍ� */
#define NUM_EXTI_FUNC  6
#define EXTI_BASENO    10
#endif
#elif defined(TOPPERS_STM32F446_NUCLEO144)
#define INHNO_SW1   IRQ_VECTOR_EXTI15_10 /* �����݃n���h���ԍ� */
#define INTNO_SW1   IRQ_VECTOR_EXTI15_10 /* �����ݔԍ� */
#define NUM_EXTI_FUNC  6
#define EXTI_BASENO    10
#else
#define INHNO_SW1   IRQ_VECTOR_EXTI0	/* �����݃n���h���ԍ� */
#define INTNO_SW1   IRQ_VECTOR_EXTI0	/* �����ݔԍ� */
#define NUM_EXTI_FUNC  1
#define EXTI_BASENO    0
#endif
#define INTPRI_SW1  -4          /* �����ݗD��x */
#define INTATR_SW1  TA_EDGE     /* �����ݑ��� */

/*
 * PSW�ڑ��r�b�g
 */
#define PSW1             0x00000001
#define PSW_MASK         (PSW1)
#define PSW_CHK_INTERVAL 10


/*
 * PUSH�X�C�b�`�ڑ��|�[�g������
 */
extern void switch_push_init(intptr_t exinf);


/*
 * PUSH�X�C�b�`�����݃N���A
 */
void switch_push_clear(void);

/*
 * PUSH�X�C�b�`�R�[���o�b�N�֐��ݒ�
 */
void setup_sw_func(intptr_t exinf);

/*
 * PUSH�X�C�b�`�����݊֌W
 */
void sw_dev_int(void);

/*
 *  DIPSW�̏�����
 */
extern void (*exti_func[NUM_EXTI_FUNC])(void);

extern void switch_dip_init(intptr_t exinf);
/*
 * DIPSW�̎�o��
 */
uint_t switch_dip_sense(void);


extern void device_info_init(intptr_t exinf);

extern uint_t dipsw_value;

#ifdef __cplusplus
}
#endif

#endif

