#ifndef _OFD_BARCODE_R_H_
#define _OFD_BARCODE_R_H_

#include "../include/ofd_base_r.h"

#define OFD_BCFORMAT_UNSPECIFY		-1
#define OFD_BCBCFORMAT_CODE_39		1
#define OFD_BCBCFORMAT_CODE_128		3
#define OFD_BCBCFORMAT_EAN_8		6
#define OFD_BCBCFORMAT_UPC_A		7
#define OFD_BCBCFORMAT_EAN_13		8
#define OFD_BCBCFORMAT_ITF			9
#define OFD_BCBCFORMAT_QR_CODE		15

/**
 * @brief 条形码编码。
 *
 * @param[in]	lpcwContent			被编码成条形码的字符串。
 * @param[in]	bcFormat			条形码格式。
 * @param[in]	imgFile				条形码文件。
 * @param[in]	width				条形码单元模块的宽度。
 * @param[in]	height				条形码单元模块的高度。
 * @param[in]	Password			密码（只针对二维码有效）	
 * @return 成功返0，失败返回其它。
 *				
 */
int OFD_Barcode_Encode(OFD_LPCWSTR lpcwContent, int bcFormat, OFD_LPCWSTR imgFile, int width, int height, OFD_LPCWSTR Password);

/**
 * @brief 条形码解码。
 *
 * @param[in]	imgFile				条形码文件。
 * @param[in]	bcFormat			条形码格式。
 * @param[in]	wContent			解码后的内容。
 * @param[in]	Password			密码（只针对二维码有效）
 * @return 成功返回0，失败返回其它。
 *				
 */
int OFD_Barcode_Decode(OFD_LPCWSTR imgFile, int bcFormat, OFD_WSTR* wContent,  OFD_LPCWSTR Password);

#endif //_OFD_BARCODE_R_H_

