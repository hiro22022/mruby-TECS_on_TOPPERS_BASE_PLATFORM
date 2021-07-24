/*
 *  TOPPERS/ASP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Advanced Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2004-2012 by Embedded and Real-Time Systems Laboratory
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
 *  Copyright (C) 2015-2017 by TOPPERS PROJECT Educational Working Group.
 * 
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
 *  $Id: sd.c 2416 2017-03-29 14:16:20Z roi $
 */
/**
  ******************************************************************************
  * @file    sd.c 
  * @author  MCD Application Team
  * @version V1.2.0
  * @date    30-December-2016 
  * @brief   This example code shows how to use the SD Driver
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include <kernel.h>
#include <t_syslog.h>
#include <t_stdlib.h>
#include <stdio.h>
#include "syssvc/serial.h"
#include "syssvc/syslog.h"
#include "sample1.h"
#include "stm32f769i_discovery_lcd.h"
#include "stm32f769i_discovery_ts.h"
#include "storagedevice.h"
#include "ff.h"
#include "fcntl.h"

#define NUM_FILENAME    256
#define NUM_FILEMAX     256
#define MAX_PATH        256

#define FILE_ACT              (1<<0)
#define FILE_JPEG             (1<<1)
#define FILE_WAV              (1<<2)
#define FILE_MP3              (1<<3)
#define FILE_EXE              (FILE_JPEG | FILE_WAV | FILE_MP3)

/*
 *  �t�@�C���C���t�H���[�V�����\����
 */
typedef struct {
	uint32_t devno;
	uint32_t tsize;
	uint32_t fsize;
	uint32_t num_file;
	uint32_t num_jpeg;
	struct dirent2 finfo[NUM_FILEMAX];
} FILE_INFO;


static char root_dir[] = "0:";
static FILE_INFO  Finfo;
static struct dirent2 finfo;
static volatile int  sdmode = 0;

/* Private function prototypes -----------------------------------------------*/
static void SD_SetHint(void);
/* Private functions ---------------------------------------------------------*/

static void storagedev_notice(void *psdev, bool_t sw)
{
	syslog_3(LOG_NOTICE, "MEDIA[%08x][%s] on/off(%d) !", psdev, root_dir, sw);
	sdmode = 2;
}

/*
 *  �t�@�C���g���q�`�F�b�N
 */
static int
_checkext(const char* fpath, const char* ext)
{
	int i, j;
	char c;
	for(i = j = 0 ; i < MAX_PATH && fpath[i] != 0 ; i++){
		if(fpath[i] == '.'){
			j = i;
		}
	}
	if(j == 0)
		return 0;
	i = j + 1;
	for(j = 0 ; i < MAX_PATH && fpath[i] != 0 && ext[j] != 0 ; i++){
		c = fpath[i];
		if(c >= 'a' && c <= 'z')
			c -= 0x20;
		if(c != ext[j])
			return 0;
		j++;
	}
	return j;
}

/*
 *  �t�@�C�����̎擾
 */
static int_t
volume_dir(char *arg, FILE_INFO *pfinfo)
{
	StorageDevice_t *psdev;
	struct statfs2  status;
	char   *path, *spath, name[16], *p;
	void   *dir;

	name[0] = 0;
	spath = path = arg;
	pfinfo->devno = SDMGetDeviceNo((const char **)&path);
	pfinfo->num_file = 0;
	pfinfo->num_jpeg = 0;
	pfinfo->tsize = 0;
	pfinfo->fsize = 0;
	psdev = SDMGetStorageDevice(pfinfo->devno);
	if(psdev == 0){
		return -1;
	}
	if((psdev->_sdev_attribute & SDEV_DEVERROR) != 0
			|| (psdev->_sdev_attribute & SDEV_EMPLOY) == 0)
		return -1;
	if(psdev->pdevff == 0 || psdev->pdevff->_sdevff_opendir == 0){
		return -1;
	}
	dir = psdev->pdevff->_sdevff_opendir(spath);
	if(dir != NULL){
		if(*spath == 0){
			name[0] = pfinfo->devno + '0';
			name[1] = ':';
			name[2] = '/';
			name[3] = 0;
			spath = name;
		}
		while(psdev->pdevff->_sdevff_readdir(dir, &finfo) != 0 && finfo.d_name[0] != 0 && pfinfo->num_file < NUM_FILEMAX){
			memcpy(&pfinfo->finfo[pfinfo->num_file], &finfo, sizeof(struct dirent2));
			pfinfo->finfo[pfinfo->num_file].d_dummy = FILE_ACT;
			p = pfinfo->finfo[pfinfo->num_file].d_name;
			if(_checkext(p, "JPG") != 0){
				pfinfo->finfo[pfinfo->num_file].d_dummy |= FILE_JPEG;
				pfinfo->num_jpeg++;
			}
			else if(_checkext(p, "WAV") != 0)
				pfinfo->finfo[pfinfo->num_file].d_dummy |= FILE_WAV;
			else if(_checkext(p, "MP3") != 0)
				pfinfo->finfo[pfinfo->num_file].d_dummy |= FILE_MP3;
			pfinfo->num_file++;
		}
		psdev->pdevff->_sdevff_closedir(dir);
		if(psdev->pdevff->_sdevff_statfs != 0
			 && psdev->pdevff->_sdevff_statfs(spath, &status) == 0){
			pfinfo->tsize = status.f_bsize;
			pfinfo->fsize = status.f_bfree;
		}
	}
	else{
		syslog_0(LOG_ERROR, "disk error !");
	}
	return 0;
}

/**
  * @brief  SD Demo
  * @param  None
  * @retval None
  */
void SD_demo (void)
{
  int_t SD_state;
  FILE_INFO  *pFinfo = &Finfo;
  StorageDevice_t *psdev;
  struct dirent2  *pfinfo;
  int y = 100;
  int count, len;

  psdev = SDMGetStorageDevice(0);
  psdev->_sdev_notice = storagedev_notice;
  sdmode = 1;

  while (1)
  {
    if (CheckForUserInput() > 0)
    {
      return;
    }
    if(sdmode != 0){
      SD_SetHint();
      dly_tsk(20);
      SD_state = volume_dir(root_dir, pFinfo);
      syslog_1(LOG_NOTICE, "## SD_state(%d) ##", SD_state);
      sdmode = 0;
      y = 100;
      memset(cbuff, ' ', sizeof(cbuff));
      len = (BSP_LCD_GetXSize()-4) / 7;
      cbuff[len] = 0;
      if(SD_state != 0){
        BSP_LCD_DisplayStringAt(20, y, (uint8_t *)"SD no media !", LEFT_MODE);
      }
      else{
       	for(count = 0 ; count < pFinfo->num_file && y < (BSP_LCD_GetYSize()-30) ; count++){
          int year, no;

          pfinfo = &pFinfo->finfo[count];
          sprintf(cbuff, "%02d %s ", count+1, pfinfo->d_name);
          no = strlen(pfinfo->d_name);
          for(no += 3 ; no < len ; no++){
            if(cbuff[no] < ' ')
              cbuff[no] = ' ';
          }
          no = len - 22;
          year = (pfinfo->d_date>>9);
          if(year >= 20)
            year -= 20;
          else
            year += 80;
          sprintf(&cbuff[no], "%02d/%02d/%02d ", year, (pfinfo->d_date>>5) & 15, pfinfo->d_date & 31);
          no += 9;
          if(pfinfo->d_type & AM_DIR)
            strcpy(&cbuff[no], "<DIR>  ");
          else if(pfinfo->d_type & AM_VOL)
            strcpy(&cbuff[no], "<VOL>  ");
          else{
            sprintf(&cbuff[no], "%d", (int)pfinfo->d_fsize);
          }
          BSP_LCD_DisplayStringAt(20, y, (uint8_t *)cbuff, LEFT_MODE);
          y += 15;
        }
        sprintf(cbuff, "%d free blocks %d bytes in a block", (int)pFinfo->fsize, (int)pFinfo->tsize);
        BSP_LCD_DisplayStringAt(20, y, (uint8_t *)cbuff, LEFT_MODE);
      }
    }
    dly_tsk(100);
  }
}

/**
  * @brief  Display SD Demo Hint
  * @param  None
  * @retval None
  */
static void SD_SetHint(void)
{
  /* Clear the LCD */
  BSP_LCD_Clear(LCD_COLOR_WHITE);

  /* Set LCD Demo description */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), 80);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
  BSP_LCD_SetFont(&Font24);
  BSP_LCD_DisplayStringAt(0, 0, (uint8_t *)"SD FILE", CENTER_MODE);
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_DisplayStringAt(0, 30, (uint8_t *)"This example sd file system", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 45, (uint8_t *)"display sd-card file list and also", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 60, (uint8_t *)"how to detect the presence of the card", CENTER_MODE);

  /* Set the LCD Text Color */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_DrawRect(10, 90, BSP_LCD_GetXSize() - 20, BSP_LCD_GetYSize() - 100);
  BSP_LCD_DrawRect(11, 91, BSP_LCD_GetXSize() - 22, BSP_LCD_GetYSize() - 102);

  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
