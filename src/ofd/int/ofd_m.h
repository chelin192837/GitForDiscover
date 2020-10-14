#ifndef _OFD_M_H_
#define _OFD_M_H_

#include "../include/ofd_base_r.h"

// Internal interface

// MSDK
void OFD_Document_ExportOfficeDoc(OFD_DOCUMENT hDocument, OFD_LPCWSTR lpwszFileName);
// MSDK
void OFD_Document_ExportMetadata(OFD_DOCUMENT hDocument, OFD_LPCWSTR lpwszFileName);

// HTKY
// GRAY Print
// Copy number
/*
	1.令
	2.公告
	3.公报
	4.决定
	5.决议
	6.函
	7.意见
	8.批复
	9.报告
	10.纪要
	11.议案
	12.请示
	13.通告
	14.通报
	15.通知
*/

int OFD_Page_Insensitive(OFD_PAGE hPage, int imgType, IFX_MemoryStream* pMem, int dpi, 
	OFD_LPCWSTR copy_num, OFD_LPBYTE barcode_buf, int barcode_bufSize, int barcode_ImgType,
	int barcode_contentlen, int wzh_type);


#endif //_OFD_M_H_
