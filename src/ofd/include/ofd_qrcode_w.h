#ifndef _FS_QRCODE_H_
#define _FS_QRCODE_H_

#include <string>
#include "../3rd/qrcode/qrencode.h"
#include "../3rd/libpng/include/png.h"
#include "ofd_base_r.h"

using namespace std;

class IFX_FileWrite;

#define FS_QR_OK		0
#define FS_QR_FAIL		1
#define FS_QR_PARAM		2

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 
 *
 * @param[in]	
 * @return 
 *				
 */
bool FS_QR_ConvertStr2Img(OFD_LPCWSTR pSrcString,OFD_LPCWSTR pDestFilePath,int iPixelSize=3,QRecLevel enumLevel=QR_ECLEVEL_L,
	int iMargin = 0,int iDpi = 72,int iEight = 0,int iMicro = 0,QRencodeMode enumEncodeMode = QR_MODE_8,int iCasesensitive = 1);

bool FS_QR_ConvertStr2ImgData(OFD_LPCWSTR pSrcString,IFX_FileWrite* pFileWrite,int iPixelSize=3,QRecLevel enumLevel=QR_ECLEVEL_L,
	int iMargin = 0,int iDpi = 72,int iEight = 0,int iMicro = 0,QRencodeMode enumEncodeMode = QR_MODE_8,int iCasesensitive = 1);

#ifdef __cplusplus
}
#endif

#endif //_FS_QRCODE_H_
