
	TOPPERS/ASP Kernel��Release 1.9.5��STM32M7
		Toyohashi Open Platform for Embedded Real-Time Systems/
		Advanced Standard Profile Kernel

TOPPERS/ASP STM32M7�ϰʲ��Σ��ĤΥܡ��ɤ�GCC�γ�ȯ�Ķ����б����ޤ���
�ܥѥå������ϸ��̥ѥå������Ǥ��뤿�ᡢTOPPERS/ASP�����ͥ륿�����å�
���¸���ѥå�����asp-1.9.3.tar.gz���Ȥ߹�碌�ƻ��Ѥ��Ƥ���������
(1)STM�� STM32F746 Discovery�ܡ���
   Chip:STM32F746NGH6
(2)STM�� STM32F746 nucleo-144�ܡ���
   Chip:STM32F746ZGT6
(3)STM�� STM32F767 nucleo-144�ܡ���
   Chip:STM32F767ZT6
(4)STM�� STM32F769 Discovery�ܡ���
   Chip:STM32F769NIH6
(5)STM�� STM32F723 Discovery�ܡ���
   Chip:STM32F723IEK6
(6)STM�� STM32H743 nucleo-144�ܡ���
   Chip:STM32H743ZIT6

ASP�μ¹Է��֤ϰʲ��Σ��Ĥ򥵥ݡ��Ȥ��ޤ���
�¹Է��֤ϡ�����ѥ�������ѿ�DBGENV��������ѹ����Ǥ��ޤ���
Makefile��������ѹ����Ǥ��ޤ���

(1)RAM�¹ԡ�ROM��˥�(rommon)�ǵ�ư�����ܡ��ɤˡ�UART���Ѥ���
ASP�μ¹Է���(srec)���������ɤ��Ƽ¹Ԥ������
rommon��FLASH ROM����ߥե������tools/rommon���֤��Ƥ���ޤ���
DBGENV�����ꤵ��ʤ���硢�ޤ���RAM������ξ�硢
���η��֤Υӥ�ɤ�Ԥ��ޤ���

(2)ROM�¹ԡ�FLASH ROM�˽񤭹���Ǽ¹Ԥ������
DBGENV��ROM������ξ�硢���η��֤Υӥ�ɤ�Ԥ��ޤ���


�ڥǥ��쥯�ȥ깽����

arch/arm_m_gcc/common
	cortex-m�Υ������
arch/arm_m_gcc/stm32f7xx
	stm32f7xx��chip��¸��
arch/arm_m_gcc/stm32h7xx
	stm32h7xx��chip��¸��
arch/gcc
	gcc�δĶ���
target/stm32f7discovery_gcc
	STM32F746 Discovery�ܡ��ɤΥ������åȰ�¸��
target/stm32f769discovery_gcc
	STM32F769 Discovery�ܡ��ɤΥ������åȰ�¸��
target/stm32f746nucleo144_gcc
	STM32F746 nucleo-144�ܡ��ɤΥ������åȰ�¸��
target/stm32f767nucleo144_gcc
	STM32F767 nucleo-144�ܡ��ɤΥ������åȰ�¸��
target/stm32f723discovery_gcc
	STM32F723 Discovery�ܡ��ɤΥ������åȰ�¸��
target/stm32h743nucleo144_gcc
	STM32H743 nucleo-144�ܡ��ɤΥ������åȰ�¸��
tools/rommon
	ROM��˥���UART���������ROM��˥��񤭹��ߥХ��ʥ�ե�����
	ROM��˥��Υ�����
	STM32F46-Discovery�Ѥ�SDCARD-PLAER2,3�γ�ȯ�Ķ���
	STM32F746 nucleo-144��arduino��ư�����Ķ���
	asp��TOPPERS BASE PLATFORM(ST)���Ȥ߹�碌�뤳�Ȥǹ��ۤǤ��ޤ�
	������TOPPERS���饳��ƥ�Ĵ��ã�������
	STM32F4-Discovery�ܡ����Ԥˤ����ۤ��ޤ���

1.9.x �С����������
	2016ǯ07��23��	Release	1.9.2		�ǽ�Υ�꡼��
	2017ǯ05��01��	Release	1.9.3		STM32F767/STM32F769 Discovery�ܡ����б�
	2018ǯ11��05��	Release 1.9.4		STM32F723 Discovery�ܡ����б�
	2019ǯ07��24��	Release 1.9.5		STM32H743 Nucleo�ܡ����б�
