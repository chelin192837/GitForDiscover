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
 * @brief ��������롣
 *
 * @param[in]	lpcwContent			���������������ַ�����
 * @param[in]	bcFormat			�������ʽ��
 * @param[in]	imgFile				�������ļ���
 * @param[in]	width				�����뵥Ԫģ��Ŀ�ȡ�
 * @param[in]	height				�����뵥Ԫģ��ĸ߶ȡ�
 * @param[in]	Password			���루ֻ��Զ�ά����Ч��	
 * @return �ɹ���0��ʧ�ܷ���������
 *				
 */
int OFD_Barcode_Encode(OFD_LPCWSTR lpcwContent, int bcFormat, OFD_LPCWSTR imgFile, int width, int height, OFD_LPCWSTR Password);

/**
 * @brief ��������롣
 *
 * @param[in]	imgFile				�������ļ���
 * @param[in]	bcFormat			�������ʽ��
 * @param[in]	wContent			���������ݡ�
 * @param[in]	Password			���루ֻ��Զ�ά����Ч��
 * @return �ɹ�����0��ʧ�ܷ���������
 *				
 */
int OFD_Barcode_Decode(OFD_LPCWSTR imgFile, int bcFormat, OFD_WSTR* wContent,  OFD_LPCWSTR Password);

#endif //_OFD_BARCODE_R_H_

