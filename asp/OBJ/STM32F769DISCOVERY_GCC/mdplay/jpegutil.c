/*
 *  TOPPERS/ASP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Advanced Standard Profile Kernel
 * 
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
 *  $Id: jpegutil.c 2416 2016-03-05 12:23:35Z roi $
 */

/* 
 *  JPEG�ѥ桼�ƥ���ƥ�
 */

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <kernel.h>
#include <t_syslog.h>
#include <t_stdlib.h>
#include "integer.h"
#include "mdcard_play.h"
#include "cdjpeg.h"		/* Common decls for cjpeg/djpeg applications */
#include "jversion.h"		/* for version message */


static const char * const cdjpeg_message_table[] = {
#include "cderror.h"
  NULL
};

static const char * progname;	/* program name for error messages */
static MEMDEV outmdev;
static FILE   outmfile;

void
exit(int __status)
{
	ID tid;

	get_tid(&tid);
	syslog_2(LOG_ERROR, "exit(%d) task(%d) !", __status, tid);
	slp_tsk();
}

void
abort(void)
{
	ID tid;

	get_tid(&tid);
	syslog_1(LOG_ERROR, "abort task(%d) !", tid);
	slp_tsk();
}


/*
 *  ����ե������ѣ��Х��ȼ�����
 */
static
int func_in(void* mfile)
{
	MEMDEV *mdev = (MEMDEV *)(((FILE*)mfile)->_dev);
	int val = -1;

	if(mdev->fsize > mdev->csize){
		val = mdev->head[mdev->csize++];
	}
	return val;
}

/*
 *  ����ե������ѥǡ���������
 */
static
int func_ins(void* mfile, unsigned int size, char *p)
{
	MEMDEV *mdev = (MEMDEV *)(((FILE*)mfile)->_dev);
	int val = 0;

	if(mdev->fsize >= (mdev->csize+size)){
		BCOPY(&mdev->head[mdev->csize], p, size);
		mdev->csize += size;
		val = size;
	}
	return val;
}

/*
 *  ����ե������ѣ��Х��Ƚ����
 */
static
void func_out(void* mfile, int val)
{
	MEMDEV *mdev = (MEMDEV *)(((FILE*)mfile)->_dev);

	if(mdev->fsize > mdev->csize){
		mdev->head[mdev->csize++] = val;
	}
}

/*
 *  ����ե������ѥǡ��������
 */
static
int func_outs(void* mfile, unsigned int size, char *p)
{
	MEMDEV *mdev = (MEMDEV *)(((FILE*)mfile)->_dev);
	int val = 0;

	if(mdev->fsize >= (mdev->csize+size)){
		BCOPY(p, &mdev->head[mdev->csize], size);
		mdev->csize += size;
		val = size;
	}
	return val;
}

/*
 *  ����ե������ѥե�å���
 */
static
int func_flush(struct __msFILE *file)
{
	return 0;
}

/*
 *  ���ꥳ�ԡ�
 */
void
bcopy2(uint8_t *s, uint8_t *d, int len)
{
	int i;
	for(i = 0 ; i < len ; i++)
		*d++ = *s++;
}

/*
 *  JPEG���᡼���Ѵ��ץ����
 */
static
void jcopy(uint8_t *s, uint8_t *d, int len)
{
	int i;
	for(i = 0 ; i < len ; i++){
		*(d + 2) = *s;
		*d = *(s + 2);
		*(d + 1) = *(s + 1);
		d += 3;
		s += 3;
	}
}

/*
 *  ����ե�����ϥ�ɥ�����
 */
MEMDEV *
setup_memory_file(FILE *st, uint8_t *addr, uint32_t size)
{
	outmdev.head  = addr;
	outmdev.csize = 0;
	outmdev.fsize = size;

	st->_flags = 0;
	st->_dev = (void *)&outmdev;
	st->_func_in   = func_in;
	st->_func_ins  = func_ins;
	st->_func_out  = func_out;
	st->_func_outs = func_outs;
	st->_func_flush = func_flush;
	return &outmdev;
}

/*
 * Marker processor for COM and interesting APPn markers.
 * This replaces the library's built-in processor, which just skips the marker.
 * We want to print out the marker as text, to the extent possible.
 * Note this code relies on a non-suspending data source.
 */

LOCAL(unsigned int)
jpeg_getc (j_decompress_ptr cinfo)
/* Read next byte */
{
	struct jpeg_source_mgr * datasrc = cinfo->src;

	if (datasrc->bytes_in_buffer == 0) {
		if (! (*datasrc->fill_input_buffer) (cinfo))
			ERREXIT(cinfo, JERR_CANT_SUSPEND);
	}
	datasrc->bytes_in_buffer--;
	return GETJOCTET(*datasrc->next_input_byte++);
}


METHODDEF(boolean)
print_text_marker (j_decompress_ptr cinfo)
{
	boolean traceit = (cinfo->err->trace_level >= 1);
	INT32 length;
	unsigned int ch;
	unsigned int lastch = 0;
	char *outstr;
	unsigned int i = 0;

	length = jpeg_getc(cinfo) << 8;
	length += jpeg_getc(cinfo);
	outstr = malloc(length);
	length -= 2;			/* discount the length word itself */

	if (traceit) {
		if (cinfo->unread_marker == JPEG_COM)
			syslog_1(LOG_ERROR, "Comment, length %ld:", (long) length);
		else			/* assume it is an APPn otherwise */
			syslog_2(LOG_ERROR, "APP%d, length %ld:",
				cinfo->unread_marker - JPEG_APP0, (long) length);
	}

	while (--length >= 0) {
		ch = jpeg_getc(cinfo);
		if (traceit) {
      /* Emit the character in a readable form.
       * Nonprintables are converted to \nnn form,
       * while \ is converted to \\.
       * Newlines in CR, CR/LF, or LF form will be printed as one newline.
       */
			if (ch == '\r') {
				outstr[i] = 0;
				syslog_1(LOG_NOTICE, "%s", outstr);
				i = 0;
			} else if (ch == '\n') {
				if (lastch != '\r'){
					outstr[i] = 0;
					syslog_1(LOG_NOTICE, "%s", outstr);
					i = 0;
				}
			} else if (ch == '\\') {
				outstr[i++] = '\\';
				outstr[i++] = '\\';
			} else if (ch >= ' ' && ch < 0x7f) {
				outstr[i++] = ch;
			} else {
				sprintf(&outstr[i], "\\%03o", ch);
				i += 4;
			}
			lastch = ch;
		}
	}

	if(traceit && i > 0){
		outstr[i] = 0;
		syslog_1(LOG_NOTICE, "%s", outstr);
	}
	dly_tsk(200);
	free(outstr);
	return TRUE;
}

/*
 *  JPEG����PPM�ؤ��Ѵ�
 */
int
jpeg2ppm(const char *infile, uint8_t *buffer, uint32_t size)
{
#ifndef NOTUSE_JPEG
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;
	djpeg_dest_ptr dest_mgr = NULL;
	FILE * input_file;
	MEMDEV *mdev;
	JDIMENSION num_scanlines;

	progname = "djpeg";		/* in case C library doesn't provide it */
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);
	/* Add some application-specific error messages (from cderror.h) */
	jerr.addon_message_table = cdjpeg_message_table;
	jerr.first_addon_message = JMSG_FIRSTADDONCODE;
	jerr.last_addon_message = JMSG_LASTADDONCODE;
	/* Insert custom marker processor for COM and APP12.
	 * APP12 is used by some digital camera makers for textual info,
	 * so we provide the ability to display it as text.
	 * If you like, additional APPn marker types can be selected for display,
	 * but don't try to override APP0 or APP14 this way (see libjpeg.doc).
	 */
	jpeg_set_marker_processor(&cinfo, JPEG_COM, print_text_marker);
	jpeg_set_marker_processor(&cinfo, JPEG_APP0+12, print_text_marker);
	if ((input_file = fopen(infile, READ_BINARY)) == NULL) {
		syslog_2(LOG_ERROR, "%s: can't open %s", progname, infile);
		return 0;
	}
	/* Specify data source for decompression */
	jpeg_stdio_src(&cinfo, input_file);
	/* Read file header, set default decompression parameters */
	(void) jpeg_read_header(&cinfo, TRUE);
	dest_mgr = jinit_write_ppm(&cinfo);
	mdev = setup_memory_file(&outmfile, (uint8_t *)buffer, size);
	dest_mgr->output_file = &outmfile;
	/* Start decompressor */
	(void) jpeg_start_decompress(&cinfo);

	/* Write output file header */
	(*dest_mgr->start_output) (&cinfo, dest_mgr);
	/* Process data */
	while (cinfo.output_scanline < cinfo.output_height) {
		num_scanlines = jpeg_read_scanlines(&cinfo, dest_mgr->buffer,
				dest_mgr->buffer_height);
		(*dest_mgr->put_pixel_rows) (&cinfo, dest_mgr, num_scanlines);
	}
	/* Finish decompression and release memory.
	 * I must do it in this order because output module has allocated memory
	 * of lifespan JPOOL_IMAGE; it needs to finish before releasing memory.
	 */
	(*dest_mgr->finish_output) (&cinfo, dest_mgr);
	(void) jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);

	/* Close files, if we opened them */
	fclose(input_file);
	return mdev->csize;
#else
	return 0;
#endif
}

/*
 *  PPM����RGB�ؤ��Ѵ�
 */
int
ppm2rgb(uint8_t *rgbaddr, int *pwidth, int *pheight)
{
	char *p6addr = (char *)rgbaddr + BMP_HEAD_SIZE - P6_HEAD_SIZE;
	uint8_t *laddr, *p1, *p2;
	int id, height, width, lcount, i;

	if(rgbaddr == NULL)
		return 0;
	sscanf(p6addr+1, "%d %d %d", &id, &width, &height);
	if(id != 6)
		return 0;
	bzero(rgbaddr, BMP_HEAD_SIZE-P6_HEAD_SIZE);
	rgbaddr[10] = BMP_HEAD_SIZE;
	rgbaddr[18] = width & 0xff;
	rgbaddr[19] = width >> 8;
	rgbaddr[22] = height & 0xff;
	rgbaddr[23] = height >> 8;
	rgbaddr[28] = 0x18;

	laddr = (uint8_t *)malloc(width*3);
	lcount = height / 2;
	for(i = 0 ; i < lcount ; i++){
		p1 = rgbaddr+BMP_HEAD_SIZE + (i * width * 3);
		p2 = rgbaddr+BMP_HEAD_SIZE + ((height-i-1) * width * 3);
		jcopy(p1, laddr, width);
		jcopy(p2, p1, width);
		BCOPY(laddr, p2, width*3);
	}
	if((height & 1) != 0){
		p1 = rgbaddr+BMP_HEAD_SIZE + (i * width * 3);
		jcopy(p1, laddr, width);
		BCOPY(laddr, p1, width*3);
	}
	free(laddr);
	*pwidth = width;
	*pheight= height;

	return width * height * 3 + BMP_HEAD_SIZE;
}

