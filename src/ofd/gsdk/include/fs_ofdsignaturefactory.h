#ifndef _FS_OFDSIGNATUREFACTORY_H_
#define _FS_OFDSIGNATUREFACTORY_H_


class CFS_OESInterface;
class CFS_OESInterface_V0;
class CFS_OESInterface_V1;
class CFS_OESInterface_V2;

// OES Erorr
#define OES_OK						0x00000000	//�ɹ�
#define OES_PIN						0x01111111  //��Ҫ��¼��־

#define OES_SEALIMAGE_FLAG_DISPLAY		0x00000000	//��ʾ
#define OES_SEALIMAGE_FLAG_PRINT		0x00000001	//��ӡ
#define OES_SEALIMAGE_FLAG_PREVIEW		0x00000002	//��ӡԤ��

#if defined(_WIN32) || defined(_WIN64)


/* �Ķ�������OES�汾�жϵķ�ʽ
 *
 *								GetSignImage			GetSealImage
 * һ�ڣ�CFS_OESInterface_V0,	��						��
 * ���ڣ�CFS_OESInterface_V1,	��						��
 * ���ڣ�CFS_OESInterface_V2,	��						��
 *
*/
//˵����OES�ӿ��������汾��ͨ������Ϊ V1��V2��V3����sdk�ڲ����ֱ��Ӧ CFS_OESInterface_V0��CFS_OESInterface_V1��CFS_OESInterface_V2
/*********************** ���¼����ӿ� V1��V2��V3 ���� ******************************/

/** T.2
 * @brief ��ȡӡ���б�ӿڣ���ӡģͼ��У��֤����������ݣ�  [Required]
 * @param[out]     puchSealListData            ӡ���б����ݣ��ַ�����UTF-8���룩
 *                                             ���� ID1\0Name1\0ID2\0Name2\0\0  (��\0�ָ�\0\0�������ַ���)
 * @param[out/in]  piSealListDataLen           ӡ���б����ݳ���
 * @return ���óɹ�����OES_OK����Ҫ��¼����OES_PIN�������Ǵ�����룬�ɵ���OES_GetErrMessage()��ȡ��ϸ��Ϣ
 */
typedef int (__stdcall *Dll_OES_GetSealList)(unsigned char* puchSealListData, int* piSealListDataLen);

/** T.1
 * @brief ��ȡǩ������ṩ����Ϣ [Required]
 * @param[out]     puchName      ���ƣ��ַ�����UTF-8���룩����ΪNULLʱ��ͨ��piNameLen��������
 * @param[out/in]  piNameLen     ���Ƴ���
 * @param[out]     puchCompany   ��˾���ƣ��ַ�����UTF-8���룩����ΪNULLʱ��ͨ��piCompanyLen��������
 * @param[out/in]  piCompanyLen  ��˾���Ƴ���
 * @param[out]     puchVersion   �汾���ַ�����UTF-8���룩����ΪNULLʱ��ͨ��piVersionLen��������
 * @param[out/in]  piVersionLen  �汾����
 * @param[out]     puchExtend    ��չ��Ϣ�������ƣ�����ΪNULLʱ��ͨ��piExtendLen��������
 * @param[out/in]  piExtendLen   ��չ��Ϣ����
 * @return ���óɹ�����OES_OK����Ҫ��¼����OES_PIN�������Ǵ�����룬�ɵ���OES_GetErrMessage()��ȡ��ϸ��Ϣ
 */
typedef int (__stdcall *Dll_OES_GetProviderInfo)(unsigned char* puchName, int* piNameLen,
	unsigned char* puchCompany, int* piCompanyLen,
	unsigned char* puchVersion, int* piVersionLen,
	unsigned char* puchExtend, int* piExtendLen);

/** T.4 ��ȡժҪ�㷨��ʶ�ӿ�
 * @brief ��ȡժҪ�㷨��ʶ [Required]
 * @param[out]     puchDigestMethod           ժҪ�㷨(�ַ�����UTF-8����)����ΪNULLʱ��ͨ��piDigestMethodLen��������
 * @param[out/in]  piDigestMethodLen          ժҪ�㷨����
 * @return ���óɹ�����OES_OK����Ҫ��¼����OES_PIN�������Ǵ�����룬�ɵ���OES_GetErrMessage()��ȡ��ϸ��Ϣ
 */
typedef int (__stdcall *Dll_OES_GetDigestMethod)(unsigned char* puchDigestMethod, int* piDigestMethodLen);

/** T.5 �������ժҪ�ӿ�
 * @brief ����ժҪ�ӿ�  [Required]
 * @param[in]      puchData                  ��ժҪ������
 * @param[in]      iDataLen                  ��ժҪ�����ݳ���
 * @param[in]      puchDigestMethod          ժҪ�㷨(�ַ�����UTF-8����)
 * @param[in]      iDigestMethodLen          ժҪ�㷨����
 * @param[out]     puchDigestValue           ժҪֵ����ΪNULLʱ��ͨ��piDigestValueLen��������
 * @param[out/in]  piDigestValueLen          ժҪֵ����
 * @return ���óɹ�����OES_OK����Ҫ��¼����OES_PIN�������Ǵ�����룬�ɵ���OES_GetErrMessage()��ȡ��ϸ��Ϣ
 */
typedef int (__stdcall *Dll_OES_Digest)(unsigned char* puchData, int iDataLen,
	unsigned char* puchDigestMethod, int iDigestMethodLen,
	unsigned char* puchDigestValue, int* piDigestValueLen);

/**T.16
 * @brief ȡ�����´�����Ϣ
 * @param[in]      iErrCode                �������
 * @param[out]     puchErrMessage          ������Ϣ����ΪNULLʱ��ͨ��piErrMessageLen��������(utf8����)
 * @param[out/in]  piErrMessageLen         ������Ϣ����
 * @return ���óɹ�����OES_OK��������ζ�Ÿô������޷�����
*/
typedef int (__stdcall *Dll_OES_GetErrMessage)(unsigned long errCode,
	unsigned char* puchErrMessage,int* piErrMessageLen);

/* puchSealData��iSealDataLen V1��V2 Ϊӡ�����ݣ�V3Ϊǩ������ */
/**  T.11
 * @brief ��ȡ����ӡ�µ�ӡģ�ṹ  [Required]
 * @param[in]      puchSealData			  ǩ������  (����12.23�������۽��,��ǩ�������н���ӡ��ӡģ�ṹ)
 * @param[in]      iSealDataLen           ǩ�������ݳ���
 * @param[out]     puchSealId             ӡ�±�ʶ(�ַ�����UTF-8����)����ΪNULLʱ��ͨ��piSealIdLen�������ȣ�����׼�Ϲ̶�ΪES��
 * @param[out/in]  piSealIdLen            ӡ�±�ʶ����
 * @param[out]     puchVersion            ͷ��Ϣ-�汾(�ַ�����UTF-8����)����ΪNULLʱ��ͨ��piVersionLen��������
 * @param[out/in]  piVersionLen           ͷ��Ϣ-�汾���ݳ���
 * @param[out]     puchVenderId           ͷ��Ϣ-���̱�ʶ(�ַ�����UTF-8����)����ΪNULLʱ��ͨ��piVenderIdLen��������
 * @param[out/in]  piVenderIdLen          ͷ��Ϣ-���̱�ʶ����
 * @param[out]     puchSealType           ӡ����Ϣ-ӡ������(�ַ�����UTF-8����)����ΪNULLʱ��ͨ��piSealTypeLen��������
 * @param[out/in]  piSealTypeLen          ӡ����Ϣ-ӡ�����ͳ���
 * @param[out]     puchSealName           ӡ����Ϣ-ӡ������(�ַ�����UTF-8����)����ΪNULLʱ��ͨ��piSealNameLen��������
 * @param[out/in]  piSealNameLen          ӡ����Ϣ-ӡ�����Ƴ���
 * @param[out]     puchValidStart         ӡ����Ϣ-��Ч��ʼʱ��(�ַ�����UTF-8����)����ΪNULLʱ��ͨ��piValidStartLen��������
 * @param[out/in]  piValidStartLen        ӡ����Ϣ-��Ч��ʼʱ�䳤��
 * @param[out]     puchValidEnd           ӡ����Ϣ-��Ч����ʱ��(�ַ�����UTF-8����)����ΪNULLʱ��ͨ��piValidEndLen��������
 * @param[out/in]  piValidEndLen          ӡ����Ϣ-��Ч��������
 * @param[out]     puchSignedDate         ӡ����Ϣ-��������(�ַ�����UTF-8����)����ΪNULLʱ��ͨ��piSignedDateLen��������
 * @param[out/in]  piSignedDateLen        ӡ����Ϣ-�������ڳ���
 * @param[out]     puchSignerName         ǩ����Ϣ-������(�ַ�����UTF-8����)����ΪNULLʱ��ͨ��piSignerNameLen��������
 * @param[out/in]  piSignerNameLen        ǩ����Ϣ-�����˳���
 * @return ���óɹ�����OES_OK����Ҫ��¼����OES_PIN�������Ǵ������,ͨ��OES_GetErrMessage�ӿڻ�ȡ��ϸ������Ϣ
 */
typedef int (__stdcall *Dll__GetSealInfo)(unsigned char* puchSealData,int iSealDataLen,
                                        unsigned char* puchSealId,int* piSealIdLen,
                                        unsigned char* puchVersion,int* piVersionLen,
                                        unsigned char* puchVenderId,int* piVenderIdLen,
                                        unsigned char* puchSealType,int* piSealTypeLen,
                                        unsigned char* puchSealName,int* piSealNameLen,
                                        unsigned char* puchCertInfo,int* piCertInfoLen,
                                        unsigned char* puchValidStart,int* piValidStartLen,
                                        unsigned char* puchValidEnd,int* piValidEndLen,
                                        unsigned char* puchSignedDate,int* piSignedDateLen,
                                        unsigned char* puchSignerName,int* piSignerNameLen,
                                        unsigned char* puchSignMethod,int* piSignMethodLen);

/*********************** OES Common Interface Function End   ***********************/

/*********************** OES Update Interface Function Start ***********************/
/*Dll_OES_Sign V1��V2�ӿ�*/
/**
 * @brief �������ǩ��,�������ǰ��Ҫ�������룬���ʵ������Ҫ�ṩ�������  [Required]
 * @param[in]      puchSealId              ӡ�±�ʶ
 * @param[in]      iSealIdLen              ӡ�±�ʶ����
 * @param[in]      puchDocProperty         �ĵ���Ϣ���̶�ΪSignature.xml�ľ���·��
 * @param[in]      iDocPropertyLen         �ĵ���Ϣ����
 * @param[in]      puchDigestData          ժҪ����
 * @param[in]      iDigestDataLen          ժҪ���ݳ���
 * @param[in]      puchSignMethod          ǩ���㷨
 * @param[in]      iSignMethodLen          ǩ���㷨����
 * @param[in]      puchSignDateTime        ǩ��ʱ��
 * @param[in]      iSignDateTimeLen        ǩ��ʱ�䳤��
 * @param[out]     puchSignValue           ǩ��ֵ�����Ϲ��ܱ�׼������ΪNULLʱ��ͨ��piSignValueLen��������
 * @param[out/in]  piSignValueLen          ǩ��ֵ����
 * @return ���óɹ�����OES_OK�������Ǵ�����룬�ɵ���OES_GetErrMessage()��ȡ��ϸ��Ϣ��
 */
typedef int (__stdcall *Dll_OES_Sign)(unsigned char* puchSealId,int iSealIdLen,
	unsigned char* puchDocProperty,int iDocPropertyLen,
	unsigned char* puchDigestData,int iDigestLen,
	unsigned char* puchSignMethod,int iSignMethodLen,
	unsigned char* puchSignDateTime,int iSignDateTimeLen,
	unsigned char* puchSignValue,int* piSignValueLen);

/*Dll_OES_Sign V3�ӿ�,��Σ�ȥ��puchSignDateTime�����Ӧ�ĳ���*/
/** T.9 ǩ�½ӿ�
 * @brief �û��ṩ��ǩ��ʵ��  [Required]
 * @param[in]      puchSealId              ӡ�±�ʶ���ַ�����UTF-8���룩
 * @param[in]      iSealIdLen              ӡ�±�ʶ����
 * @param[in]      puchDocProperty         �ĵ���Ϣ���ַ�����UTF-8���룩,�̶�ΪSignature.xml������
 * @param[in]      iDocPropertyLen         �ĵ���Ϣ����
 * @param[in]	   puchDigestData		   ժҪ���ݣ������ƣ�
 * @param[in]	   iDigestDataLen		   ժҪ���ݳ���
 * @param[out]     puchSignedValue		   ǩ�����ݣ���ΪNULLʱ��ͨ��piSignValueLen��������
 * @param[out/in]  piSignedValueLen        ǩ�����ݳ���
 * @return ���óɹ�����OES_OK����Ҫ��¼����OES_PIN�������Ǵ������,�ڷ�������ʱ����ϸ�Ĵ�����Ϣ��Ҫ����OES_GetErrMessage()��
 */
typedef int (__stdcall *Dll_V2_OES_Sign)(unsigned char* puchSealId,int iSealIdLen,
	unsigned char* puchDocProperty,int iDocPropertyLen,
	unsigned char* puchDigestData,int iDigestDataLen,
	unsigned char* puchSignedValue,int* piSignedValueLen);

/*Dll_OES_Verify V1��V2�ӿ�*/
/**
 * @brief ������ǩʵ��  [Required]
 * @param[in]  puchSealData            ӡ������
 * @param[in]  iSealDataLen            ӡ�����ݳ���
 * @param[in]  puchDocProperty         �ĵ���Ϣ���̶�ΪSignature.xml�ľ���·��
 * @param[in]  iDocPropertyLen         �ĵ���Ϣ����
 * @param[in]  puchSignMethod          ǩ���㷨
 * @param[in]  iSignMethodLen          ǩ���㷨����
 * @param[in]  puchSignDateTime        ǩ��ʱ��
 * @param[in]  piSignDateTimeLen       ǩ��ʱ�䳤��
 * @param[in]  puchSignValue           ǩ��ֵ
 * @param[in]  iSignValueLen           ǩ��ֵ����
 * @param[in]  iOnline                 �Ƿ�������֤
 * @return ���óɹ�����OES_OK�������Ǵ�����룬����ֵ�������Σ�ӡ�±�����֤�����ӡ��������֤������ĵ���֤�������
 *         �ڷ�������ʱ����ϸ�Ĵ�����Ϣ��Ҫ����OES_GetErrMessage()��
 */
typedef int (__stdcall *Dll_OES_Verify)(unsigned char* puchSealData,int iSealDataLen,
	unsigned char* puchDocProperty,int iDocPropertyLen,
	unsigned char* puchDigestData,int iDigestLen,
	unsigned char* puchSignMethod,int iSignMethodLen,
	unsigned char* puchSignDateTime,int iSignDateTimeLen,
	unsigned char* puchSignValue,int iSignValueLen,
	int bOnline);

/*Dll_V2_OES_Verify V3�ӿ�,���,ȥ��puchSignDateTime�����Ӧ�ĳ���*/
/** T.10 ��ǩ�ӿ�
 * @brief �û��ṩ������ʵ��  [Required]
 * @param[in]  puchSignedValue         ǩ������(ǩ�������к���ӡ������)
 * @param[in]  iSignedValueLen         ǩ�����ݳ���
 * @param[in]  puchDocProperty         �ĵ���Ϣ(�ַ�����UTF-8����)���̶�ΪSignature.xml������
 * @param[in]  iDocPropertyLen         �ĵ���Ϣ����
 * @param[in]  puchDigestData		   ժҪ����
 * @param[in]  iDigestDataLen		   ժҪ���ݳ���
 * @param[in]  iOnline                 �Ƿ�������֤ (1:������֤��0��������֤)
 * @return ���óɹ�����OES_OK����Ҫ��¼����OES_PIN�������Ǵ�����룬ͨ������OES_GetErrMessage()��ȡ��ϸ�Ĵ�����Ϣ��
 */
typedef int (__stdcall *Dll_V2_OES_Verify)(unsigned char* puchSignedValue,int iSignedValueLen,/*ǩ������*/
	unsigned char* puchDocProperty,int iDocPropertyLen,
	unsigned char* puchDigestData,int iDigestDataLen,
	int iOnline);


/*********************** OES Specific Interface Function Start ********************/
/*Dll_OES_GetSeal V1��V2�д˽ӿڣ�V3�޴˽ӿ�*/
/**
 * @brief ��ȡ����ǩ�µ�ӡģ�ṹ����ӡģͼ��У��֤�����������,���Ϲ��ܱ�׼��  [Required]
 * @param[in]      puchSealId              ӡ�±�ʶ�����ƣ��ַ�����
 * @param[in]      iSealIdLen              ӡ�±�ʶ�����Ƴ���
 * @param[out]     puchSealData            ӡ�����ݣ����Ϲ��ܱ�׼������ΪNULLʱ��ͨ��piSealListDataLen��������
 * @param[out/in]  piSealDataLen           ӡ�����ݳ���
 * @return ���óɹ�����OES_OK�������Ǵ������
 */
typedef int (__stdcall *Dll_OES_GetSeal)(unsigned char* puchSealIdOrName, int iSealIdOrNameLen,
	unsigned char* puchSealData, int* piSealDataLen);

/*Dll_OES_GetSignMethod V1��V2�д˽ӿڣ�V3�޴˽ӿ�*/
/**
 * @brief ��ȡǩ���㷨��ʶ [Required]
 * @param[out]     puchSignMethod            ǩ���㷨��UTF-8���룩����ΪNULLʱ��ͨ��piSignMethodLen��������
 * @param[out/in]  piSignMethodLen           ǩ���㷨����
 * @return ���óɹ�����OES_OK�������Ǵ������
 */
typedef int (__stdcall *Dll_OES_GetSignMethod)(unsigned char* puchSignMethod, int* piSignMethodLen);

/*Dll_OES_GetSignDateTime V1��V2�д˽ӿڡ�V3�޴˽ӿ�,ǩ��ʱ����ǩ������ڲ�ȷ��������Ҫ������*/
/**
 * @brief ��ȡǩ��ʱ�䣨ʱ�����������ʽ�� [Required]
 * @param[out]     puchSignDateTime          ǩ��ʱ�䣨�ַ�ʱ��UTF-8���룬����yyyy-MM-dd hh:mm:ss,ʱ���ʱ������ֵ������ΪNULLʱ��ͨ��piSignDateTimeLen��������
 * @param[out/in]  piSignDateTimeLen         ǩ��ʱ�䳤��
 * @return ���óɹ�����OES_OK�������Ǵ������
 */
typedef int (__stdcall *Dll_OES_GetSignDateTime)(unsigned char* puchSignDateTime, int* piSignDateTimeLen);

//ע�����ӿ�
/*V1�ӿ�����*/
/**
 * @brief �ӵ���ӡ�������л�ȡӡ��ͼ�񼰳ߴ�  [Optional]
 * @param[in]      puchSealData             ӡ�����ݣ����Ϲ��ܱ�׼��
 * @param[in]      iSealDataLen             ӡ�����ݳ���
 * @param[in]      iRenderFlag              ������;��ǣ�0��ʾӡ���б�������ʾ��ӡ��ͼ��
 * @param[out]     puchSealImage            ӡ��ͼ�����ݣ���ΪNULLʱ��ͨ��piSealImageLen��������
 * @param[out/in]  piSealImageLen           ӡ��ͼ�����ݳ���
 * @param[out/in]  piSealWidth              ӡ�¿�ȣ���λmm��
 * @param[out/in]  piSealHeight             ӡ�¸߶ȣ���λmm��
 * @return ���óɹ�����OES_OK�������Ǵ������
 */
typedef int (__stdcall *Dll_V0_OES_GetSealImage)(unsigned char* puchSealData,int iSealDataLen,int iRenderFlag, 
	unsigned char* puchSealImage,int* piSealImageLen,
	int* piSealWidth,int* piSealHeight);

// Foxit soft sign
typedef void (__stdcall *Dll_V0_OES_SetPasswd)(unsigned long nLen,unsigned char* passwd);

/*V2�ӿ�����*/
/**
 * @brief �ӵ���ӡ�������л�ȡӡ��ͼ�񼰳ߴ�  [Optional]
 * @param[in]      puchSealData             ӡ�����ݣ����Ϲ��ܱ�׼��
 * @param[in]      iSealDataLen             ӡ�����ݳ���
 * @param[in]      iRenderFlag              ������;��ǣ�0��ʾӡ���б�������ʾ��ӡ��ͼ��
 * @param[out]     puchSealImage            ӡ��ͼ�����ݣ���ΪNULLʱ��ͨ��piSealImageLen��������
 * @param[out/in]  piSealImageLen           ӡ��ͼ�����ݳ���
 * @param[out/in]  piSealWidth              ӡ�¿�ȣ���λmm��
 * @param[out/in]  piSealHeight             ӡ�¸߶ȣ���λmm��
 * @return ���óɹ�����OES_OK�������Ǵ������
 */
typedef int (__stdcall *Dll_V1_OES_GetSealImage)(unsigned char* puchSealData,int iSealDataLen,int iRenderFlag, 
	unsigned char* puchSealImage,int* piSealImageLen,
	int* piSealWidth,int* piSealHeight);

/*V3�ӿ�����*/
/** T.3
 * @brief ����ӡ��ID��ȡ����ӡ���е�ͼƬ����  [Required]
 * @param[in]		puchSealId				ӡ�±�ʶ���ַ�����
 * @param[in]		iSealIdLen				ӡ�±�ʶ����
 * @param[out]		puchPictureData			ӡ��ͼ�����ݣ���ΪNULLʱ��ͨ��piPictureDataLen��������
 * @param[out/in]	piPictureDataLen		ӡ��ͼ�����ݳ���
 * @param[out]		puchPictureType			ͼ������(�ַ���,UTF-8����)����ΪNULLʱ��ͨ��piPictureTypeLen��������
 * @param[out/in]	piPictureTypeLen	    ͼ�����ͳ���
 * @param[out]		piPictureWidth			ӡ��ͼ���ȣ���λΪ����(mm)
 * @param[out]		piPictureHeight			ӡ��ͼ��߶ȣ���λΪ����(mm)
 * @return ���óɹ�����OES_OK����Ҫ��¼����OES_PIN�������Ǵ�����룬�ɵ���OES_GetErrMessage()��ȡ��ϸ��Ϣ
 */
typedef int (__stdcall *Dll_V2_OES_GetSealImage)(unsigned char* puchSealId,int iSealIdLen,
	unsigned char* puchPictureData,int* piPictureDataLen,
	unsigned char* puchPictureType,int* piPictureTypeLen,
	int* piPictureWidth,int* piPictureHeight);

/*V2�д˽ӿ�*/
typedef int (__stdcall *Dll_V1_OES_Session_Open)(unsigned int* piHandler);

/*V3�д˽ӿ�*/
/** T.14
*@brief ��½�������,��½��ɽ��������ӿڵ��ã���Ӧ�ó����ṩpin�������
*@param[in]puchPIN,�������PIN��(�ַ�����UTF-8����)
*@param[in]iPINLen PIN�볤��
*@return ���óɹ�����OES_OK�������Ǵ����룬ͨ��OES_GetErrMessage�ӿڻ�ȡ��ϸ������Ϣ
**/
typedef int (__stdcall *Dll_V2_OES_Login)(unsigned char* puchPIN,int iPINLen);

/*V3�д˽ӿ�*/
/**T.15
*@brief �ǳ��������
*@return ���óɹ�����OES_OK�������Ǵ����룬ͨ��OES_GetErrMessage�ӿڻ�ȡ��ϸ������Ϣ
**/
typedef int (__stdcall *Dll_V2_OES_Logout)();

/*V2�д˽ӿ�*/
/**  T.13
 * @brief ����ǩ�����ݻ�ȡ���ڵ���ӡ�µ�ӡģ�ṹ  [Required]
 * @param[in]	   puchSignedValue        ǩ������
 * @param[in]	   iSignedValueLen        ǩ�����ݳ���
 * @param[out]	   puchPictureData        ӡ��ͼ�����ݣ���ΪNULLʱ��ͨ��piPictureDataLen��������
 * @param[out]     piPictureDataLen		  ӡ��ͼ�����ݳ���
 * @param[out]	   puchPictureType		  ӡ��ͼ������(UTF-8����)����ΪNULLʱ��ͨ��piPictureTypeLen��������
 * @param[out]	   piPictureTypeLen	      ӡ��ͼ�����ͳ���
 * @param[out]	   piPictureWidth		  ӡ��ͼ���ȣ���λΪ����(mm)
 * @param[out]	   piPictureHeight		  ӡ��ͼ��߶ȣ���λΪ����(mm)
 * @return ���óɹ�����OES_OK����Ҫ��¼����OES_PIN�������Ǵ������,ͨ��OES_GetErrMessage�ӿڻ�ȡ��ϸ������Ϣ
 */
typedef int (__stdcall *Dll_V1_OES_GetSignImage)(unsigned char* puchSignedValue,int iSignedValueLen,
	unsigned char* puchPictureData,int* piPictureDataLen,
	unsigned char* puchPictureType,int* piPictureTypeLen,
	int* piPictureWidth,int* piPictureHeight);

/*V3�д˽ӿ�*/
/**  T.13
 * @brief ����ǩ�����ݻ�ȡ���ڵ���ӡ�µ�ӡģ�ṹ  [Required]
 * @param[in]	   puchSignedValue        ǩ������
 * @param[in]	   iSignedValueLen        ǩ�����ݳ���
 * @param[out]	   puchPictureData        ӡ��ͼ�����ݣ���ΪNULLʱ��ͨ��piPictureDataLen��������
 * @param[out]     piPictureDataLen		  ӡ��ͼ�����ݳ���
 * @param[out]	   puchPictureType		  ӡ��ͼ������(UTF-8����)����ΪNULLʱ��ͨ��piPictureTypeLen��������
 * @param[out]	   piPictureTypeLen	      ӡ��ͼ�����ͳ���
 * @param[out]	   piPictureWidth		  ӡ��ͼ���ȣ���λΪ����(mm)
 * @param[out]	   piPictureHeight		  ӡ��ͼ��߶ȣ���λΪ����(mm)
 * @return ���óɹ�����OES_OK����Ҫ��¼����OES_PIN�������Ǵ������,ͨ��OES_GetErrMessage�ӿڻ�ȡ��ϸ������Ϣ
 */
typedef int (__stdcall *Dll_V2_OES_GetSignImage)(unsigned char* puchSignedValue,int iSignedValueLen,
	unsigned char* puchPictureData,int* piPictureDataLen,
	unsigned char* puchPictureType,int* piPictureTypeLen,
	int* piPictureWidth,int* piPictureHeight);


/*V3�д˽ӿ�*/
/** T.12
(����12.23�������۽�����˽ӿڵ�����7��������ȥ��ӡ�����ݺ�ǩ���㷨��ʶ��
 * @brief ��ȡ����ǩ�����ݽṹ  [Required]
 * @param[in]		puchSignedValue         ǩ������
 * @param[in]		iSignedValueLen         ǩ�����ݳ���
 * @param[out]		puchVersion				�汾�ţ��ַ���,UTF-8���룩
 * @param[out/in]	piVersionLen		    �汾�ų���
 * @param[out]		puchDateTime			ʱ����Ϣ��ǩ��ʱ�䣩���ַ���,UTF-8���룩
 * @param[out/in]	piDateTimeLen			ʱ����Ϣ����
 * @param[out]		puchDigestData			ԭ���Ӵ�ֵ�������ƣ�
 * @param[out/in]	piDigestDataLen			ԭ���Ӵ�ֵ����
 * @param[out]		puchPropertyInfo		ԭ�����ԣ��ַ���,UTF-8���룩
 * @param[out/in]	piPropertyInfoLen	    ԭ�����Գ���
 * @param[out]		puchSignerCert			ǩ����֤�飨der��ʽ�������ƣ�
 * @param[out/in]	piSignerCertLen		    ǩ����֤�鳤��
 * @param[out]		puchSignature			ǩ��ֵ�������ƣ�
 * @param[out/in]	piSignatureLen	        ǩ��ֵ����
 * @return ���óɹ�����OES_OK����Ҫ��¼����OES_PIN�������Ǵ������,ͨ��OES_GetErrMessage�ӿڻ�ȡ��ϸ������Ϣ
**/
typedef  int (__stdcall *DLL_V2_OES_GetSignInfo)(unsigned char* puchSignedValue,int iSignedValueLen,
                                      unsigned char* puchVersion  ,int* piVersionLen,
                                      unsigned char* puchDateTime ,int* piDateTimeLen,
                                      unsigned char* puchDigestData ,int* piDigestDataLen,
                                      unsigned char* puchPropertyInfo ,int* piPropertyInfoLen,
                                      unsigned char* puchSignerCert ,int* piSignerCertLen,
                                      unsigned char* puchSignature ,int* piSignatureLen);

/*********************** OES Specific Interface Function End **********************/

#else
// Linux
#include<dlfcn.h>

#define OES_HANDLE                    void*

/*********************** OES Common Interface Function Start ***********************/
typedef int (__attribute__((stdcall)) *Dll_OES_GetSealList)(unsigned char* puchSealListData, int* piSealListDataLen);

typedef int (__attribute__((stdcall)) *Dll_OES_GetProviderInfo)(unsigned char* puchName, int* piNameLen,
	unsigned char* puchCompany, int* piCompanyLen,
	unsigned char* puchVersion, int* piVersionLen,
	unsigned char* puchExtend, int* piExtendLen);

typedef int (__attribute__((stdcall)) *Dll_OES_GetDigestMethod)(unsigned char* puchDigestMethod, int* piDigestMethodLen);

typedef int (__attribute__((stdcall)) *Dll_OES_Digest)(unsigned char* puchData, int iDataLen,
	unsigned char* puchDigestMethod, int iDigestMethodLen,
	unsigned char* puchDigestValue, int* piDigestValueLen);

typedef int (__attribute__((stdcall)) *Dll_OES_GetErrMessage)(unsigned long errCode,
	unsigned char* puchErrMessage,int* piErrMessageLen);
/*********************** OES Common Interface Function End   ***********************/

/*********************** OES Update Interface Function Start ***********************/
typedef int (__attribute__((stdcall)) *Dll_OES_Sign)(unsigned char* puchSealId,int iSealIdLen,
	unsigned char* puchDocProperty,int iDocPropertyLen,
	unsigned char* puchDigestData,int iDigestLen,
	unsigned char* puchSignMethod,int iSignMethodLen,
	unsigned char* puchSignDateTime,int iSignDateTimeLen,
	unsigned char* puchSignValue,int* piSignValueLen);

typedef int (__attribute__((stdcall)) *Dll_V2_OES_Sign)(unsigned char* puchSealId,int iSealIdLen,
	unsigned char* puchDocProperty,int iDocPropertyLen,
	unsigned char* puchDigestData,int iDigestDataLen,
	unsigned char* puchSignedValue,int* piSignedValueLen);

typedef int (__attribute__((stdcall)) *Dll_OES_Verify)(unsigned char* puchSealData,int iSealDataLen,
	unsigned char* puchDocProperty,int iDocPropertyLen,
	unsigned char* puchDigestData,int iDigestLen,
	unsigned char* puchSignMethod,int iSignMethodLen,
	unsigned char* puchSignDateTime,int iSignDateTimeLen,
	unsigned char* puchSignValue,int iSignValueLen,
	int bOnline);

typedef int (__attribute__((stdcall)) *Dll_V2_OES_Verify)(unsigned char* puchSignedValue,int iSignedValueLen,/*ǩ������*/
	unsigned char* puchDocProperty,int iDocPropertyLen,
	unsigned char* puchDigestData,int iDigestDataLen,
	int iOnline);
/*********************** OES Update Interface Function End   ***********************/

/*********************************** Added by xhj **********************************/
typedef int (__attribute__((stdcall)) *Dll__GetSealInfo)(unsigned char* puchSealData,int iSealDataLen,
														unsigned char* puchSealId,int* piSealIdLen,
                                        				unsigned char* puchVersion,int* piVersionLen,
														unsigned char* puchVenderId,int* piVenderIdLen,
														unsigned char* puchSealType,int* piSealTypeLen,
														unsigned char* puchSealName,int* piSealNameLen,
														unsigned char* puchCertInfo,int* piCertInfoLen,
														unsigned char* puchValidStart,int* piValidStartLen,
														unsigned char* puchValidEnd,int* piValidEndLen,
														unsigned char* puchSignedDate,int* piSignedDateLen,
														unsigned char* puchSignerName,int* piSignerNameLen,
														unsigned char* puchSignMethod,int* piSignMethodLen);
/*********************************** Added by xhj **********************************/

/*********************** OES Specific Interface Function Start ********************/
typedef int (__attribute__((stdcall)) *Dll_OES_GetSeal)(unsigned char* puchSealIdOrName, int iSealIdOrNameLen,
	unsigned char* puchSealData, int* piSealDataLen);

typedef int (__attribute__((stdcall)) *Dll_OES_GetSignMethod)(unsigned char* puchSignMethod, int* piSignMethodLen);

typedef int (__attribute__((stdcall)) *Dll_OES_GetSignDateTime)(unsigned char* puchSignDateTime, int* piSignDateTimeLen);

//V0
typedef int (__attribute__((stdcall)) *Dll_V0_OES_GetSealImage)(unsigned char* puchSealData,int iSealDataLen,int iRenderFlag, 
	unsigned char* puchSealImage,int* piSealImageLen,
	int* piSealWidth,int* piSealHeight);

//V0 Foxit soft sign
typedef void (__attribute__((stdcall)) *Dll_V0_OES_SetPasswd)(unsigned long nLen,unsigned char* passwd);

//V1
typedef int (__attribute__((stdcall)) *Dll_V1_OES_GetSealImage)(unsigned char* puchSealData,int iSealDataLen,int iRenderFlag, 
	unsigned char* puchSealImage,int* piSealImageLen,
	int* piSealWidth,int* piSealHeight);
//V2
typedef int (__attribute__((stdcall)) *Dll_V2_OES_GetSealImage)(unsigned char* puchSealId,int iSealIdLen,
	unsigned char* puchPictureData,int* piPictureDataLen,
	unsigned char* puchPictureType,int* piPictureTypeLen,
	int* piPictureWidth,int* piPictureHeight);

//V1
typedef int (__attribute__((stdcall)) *Dll_V1_OES_Session_Open)(unsigned int* piHandler);

//V2
typedef int (__attribute__((stdcall)) *Dll_V2_OES_Login)(unsigned char* puchPIN,int iPINLen);

typedef int (__attribute__((stdcall)) *Dll_V2_OES_Logout)();

//V2
typedef int (__attribute__((stdcall)) *Dll_V1_OES_GetSignImage)(unsigned char* puchSignedValue,int iSignedValueLen,
	unsigned char* puchPictureData,int* piPictureDataLen,
	unsigned char* puchPictureType,int* piPictureTypeLen,
	int* piPictureWidth,int* piPictureHeight);

//V3
typedef int (__attribute__((stdcall)) *Dll_V2_OES_GetSignImage)(unsigned char* puchSignedValue,int iSignedValueLen,
	unsigned char* puchPictureData,int* piPictureDataLen,
	unsigned char* puchPictureType,int* piPictureTypeLen,
	int* piPictureWidth,int* piPictureHeight);

//V3
typedef  int (__attribute__((stdcall)) *DLL_V2_OES_GetSignInfo)(unsigned char* puchSignedValue,int iSignedValueLen,
    unsigned char* puchVersion  ,int* piVersionLen,
    unsigned char* puchDateTime ,int* piDateTimeLen,
    unsigned char* puchDigestData ,int* piDigestDataLen,
    unsigned char* puchPropertyInfo ,int* piPropertyInfoLen,
    unsigned char* puchSignerCert ,int* piSignerCertLen,
    unsigned char* puchSignature ,int* piSignatureLen);





//V4
typedef  int (__attribute__((stdcall)) *DLL_V3_OESV4_GetProviderInfo)(unsigned char* puchName, int* piNameLen,
unsigned char* puchCompany, int* piCompanyLen,
unsigned char* puchVersion, int* piVersionLen,
unsigned char* puchExtend, int* piExtendLen);


typedef  int (__attribute__((stdcall)) * DLL_V3_OESV4_GetProtocolVersion)(unsigned char* puchVersion, int* piVersionLen);

typedef  int (__attribute__((stdcall)) * DLL_V3_OESV4_CloseSession)(OES_HANDLE iSession);

typedef  int (__attribute__((stdcall)) * DLL_V3_OESV4_GetSealList)(OES_HANDLE iSession,unsigned char* puchSealList,int* piSealListLen);

typedef  int (__attribute__((stdcall)) * DLL_V3_OESV4_OESV4_GetCertList)(OES_HANDLE iSession,unsigned char* puchCertList,int* piCertListLen);

typedef  int (__attribute__((stdcall)) * DLL_V3_OESV4_OESV4_GetSealImageById)(OES_HANDLE iSession,unsigned char* puchSealId,int iSealIdLen,
unsigned char* puchImageData,int* piImageDataLen,
unsigned char* puchImageType,int* piImageTypeLen,
int* piSealWidth,int* piSealHeight);

typedef  int (__attribute__((stdcall)) * DLL_V3_OESV4_OESV4_GetSealInfoById)(OES_HANDLE iSession,
unsigned char* puchSealId,int iSealIdLen,
unsigned char* puchVersion,int* piVersionLen,
unsigned char* puchVenderId,int* piVenderIdLen,
unsigned char* puchSealType,int* piSealTypeLen,
unsigned char* puchSealName,int* piSealNameLen,
unsigned char* puchCertList,int* piCertListLen,
unsigned char* puchSignedDate,int* piSignedDateLen,
unsigned char* puchValidStart,int* piValidStartLen,
unsigned char* puchValidEnd,int* piValidEndLen,
unsigned char* puchSignerName,int* piSignerNameLen,
unsigned char* puchSignMethod,int* piSignMethodLen,
unsigned char* puchSignature,int* piSignatureLen);


typedef  int (__attribute__((stdcall)) * DLL_V3_OESV4_OESV4_GetCertById)(OES_HANDLE iSession,
unsigned char* puchCertId,int iCertIdLen,
unsigned char* puchCertData,int* piCertDataLen);


typedef  int (__attribute__((stdcall)) * DLL_V3_OESV4_OESV4_GetSignImage)(OES_HANDLE iSession,
unsigned char* puchSignedValue,int iSignedValueLen,
unsigned char* puchExtendParam,int iExtendParamLen,
unsigned char* puchPictureData,int* piPictureDataLen,
unsigned char* puchPictureType,int* piPictureTypeLen,
int* piPictureWidth,int* piPictureHeight);


typedef  int (__attribute__((stdcall)) * DLL_V3_OESV4_OESV4_GetSealImage)(OES_HANDLE iSession,
unsigned char* puchSealData,int iSealDataLen,
unsigned char* puchExtendParam,int iExtendParamLen,
unsigned char* puchSealPicture,int* piSealPictureLen,
unsigned char* puchPictureType,int* piPictureTypeLen,
int* piPictureWidth,int* piPictureHeight);


typedef  int (__attribute__((stdcall)) * DLL_V3_OESV4_OESV4_GetSignInfo)(OES_HANDLE iSession,
unsigned char* puchSignedValue,int iSignedValueLen,
unsigned char* puchSignVersion,int* piSignVersionLen,
unsigned char* puchSealId,int* piSealIdLen,
unsigned char* puchSealVersion,int* piSealVersionLen,
unsigned char* puchVenderId,int* piVenderIdLen,
unsigned char* puchSealType,int* piSealTypeLen,
unsigned char* puchSealName,int* piSealNameLen,
unsigned char* puchCertInfo,int* piCertInfoLen,
unsigned char* puchValidStart,int* piValidStartLen,
unsigned char* puchValidEnd,int* piValidEndLen,
unsigned char* puchSignedDate,int* piSignedDateLen,
unsigned char* puchSignerName,int* piSignerNameLen,
unsigned char* puchSealSignMethod,int* piSealSignMethodLen,
unsigned char* puchSealSignature,int* piSealSignatureLen,
unsigned char* puchTimeInfo,int* piTimeInfoLen,
unsigned char* puchDataHash,int* piDataHashLen,
unsigned char* puchPropertyInfo,int* piPropertyLen,
unsigned char* puchCert,int* piCertLen,
unsigned char* puchSignMethod,int* piSignMethodLen,
unsigned char* puchSignature,int* piSignatureLen);


typedef  int (__attribute__((stdcall)) * DLL_V3_OESV4_OESV4_GetRawSignInfo)(OES_HANDLE iSession,
unsigned char* puchSignedValue,int iSignedValueLen,
unsigned char* puchVersion,int* piVersionLen,
unsigned char* puchTimeInfo,int* piTimeInfoLen,
unsigned char* puchDataHash,int* piDataHashLen,
unsigned char* puchPropertyInfo,int* piPropertyLen,
unsigned char* puchCert,int* piCertLen,
unsigned char* puchSignMethod,int* piSignMethodLen,
unsigned char* puchSignature,int* piSignatureLen);


typedef  int (__attribute__((stdcall)) * DLL_V3_OESV4_OESV4_GetDigestMethod)(OES_HANDLE iSession,
unsigned char* puchDigestMethod,int* piDigestMethodLen);


typedef  unsigned long (__attribute__((stdcall)) * DLL_V3_OESV4_OESV4_Digest)(OES_HANDLE iSession,
unsigned char* puchData,int iDataLen,
unsigned char* puchDigestMethod,int iDigestMethodLen,
unsigned char* puchDigestValue,int* piDigestValueLen);


typedef  int (__attribute__((stdcall)) * DLL_V3_OESV4_OESV4_Digest_Init)(OES_HANDLE iSession,unsigned char* puchDigestMethod,int iDigestMethodLen,OES_HANDLE *piHash);


typedef  int (__attribute__((stdcall)) * DLL_V3_OESV4_OESV4_Digest_Update)(OES_HANDLE iSession,OES_HANDLE iHash,unsigned char* puchData,int iDataLen);


typedef  int (__attribute__((stdcall)) * DLL_V3_OESV4_OESV4_Digest_Final)(OES_HANDLE iSession,OES_HANDLE iHash,unsigned char* puchDigestValue,int* piDigestValueLen);





typedef  int (__attribute__((stdcall)) * DLL_V3_OESV4_OESV4_Digest_Final)(OES_HANDLE iSession,OES_HANDLE iHash,unsigned char* puchDigestValue,int* piDigestValueLen);


typedef  unsigned long (__attribute__((stdcall)) * DLL_V3_OESV4_OESV4_Sign)(OES_HANDLE iSession,
unsigned char* puchSealId, int iSealIdLen,
unsigned char* puchDocProperty,int iDocPropertyLen,
unsigned char* puchDigestData,int iDigestDataLen,
unsigned char* puchSignValue,int* piSignValueLen);


typedef  unsigned long (__attribute__((stdcall)) * DLL_V3_OESV4_OESV4_RawSign)(OES_HANDLE iSession,
unsigned char* puchCertId, int iCertIdLen,
unsigned char* puchDocProperty,int iDocPropertyLen,
unsigned char* puchDigestData,int iDigestDataLen,
unsigned char* puchSignValue,int* piSignValueLen);



typedef  unsigned long (__attribute__((stdcall)) * DLL_V3_OESV4_OESV4_Verify)(OES_HANDLE iSession,
unsigned char* puchDigestData,int iDigestDataLen,
unsigned char* puchSignValue,int iSignValueLen,
int iOnline);



typedef  int (__attribute__((stdcall)) * DLL_V3_OESV4_OESV4_GetErrMessage)(OES_HANDLE iSession,
unsigned long errCode,
unsigned char* puchErrMessage,int* piErrMessageLen);


typedef  int (__attribute__((stdcall)) * DLL_V3_OESV4_OESV4_Login)(OES_HANDLE iSession,
unsigned char* puchPIN,int iPINLen);


typedef  int (__attribute__((stdcall)) * DLL_V3_OESV4_OESV4_Logout)(OES_HANDLE iSession);


typedef  int (__attribute__((stdcall)) * DLL_V3_OESV4_OESV4_GetSignMethod)(OES_HANDLE iSession,
unsigned char* puchSignMethod,int* piSignMethodLen);



typedef  int (__attribute__((stdcall)) * DLL_V3_OESV4_OESV4_GetDateTime)(OES_HANDLE iSession,
unsigned char* puchSignDateTime,int* piSignDateTimeLen);


typedef  int (__attribute__((stdcall)) * DLL_V3_OESV4_OESV4_GetParamNameList)(OES_HANDLE iSession,
unsigned char* puchParamName,int* piParamNameLen);


typedef  int (__attribute__((stdcall)) * DLL_V3_OESV4_OESV4_SetExtendParam)(OES_HANDLE iSession,
unsigned char* puchParamName,int iParamNameLen,
unsigned char* puchParamValue,int iParamValueLen);


typedef  int (__attribute__((stdcall)) * DLL_V3_OESV4_OESV4_GetTimeStamp)(OES_HANDLE iSession,unsigned char* puchContent,int iContentLen,
unsigned char* puchTimeStamp,int* piTimeStampLen);


typedef  int (__attribute__((stdcall)) * DLL_V3_OESV4_OESV4_VerifyTimeStamp)(OES_HANDLE iSession,unsigned char* puchContent,int iContentLen,
unsigned char* puchTimeStamp,int piTimeStampLen);


typedef  int (__attribute__((stdcall)) * DLL_V3_OESV4_OESV4_GetTimeStampInfo)(OES_HANDLE iSession,unsigned char* puchTimeStamp,int iTimeStampLen,
unsigned char* puchContent,int* piContentLen,
unsigned char* puchDateTime,int* piDateTimeLen,
unsigned char* puchCert,int* piCertLen);













/*********************** OES Specific Interface Function End **********************/

#endif

//V1 V2 V3 �汾 OES�� ���к����ӿ�
class CFS_OESInterface : public CFX_Object
{
public:
	CFS_OESInterface();
	virtual ~CFS_OESInterface();

	// ���¼����ӿڣ�ΪV1 V2 V3 �����汾���еĽӿڣ�����Ҳһ��
	FX_INT32 GetSealList(FX_BYTE* lpStampList, FX_INT32& StampListLen);
	FX_INT32 GetProviderInfo(FX_BYTE* lpName, FX_INT32& iNameLen, FX_BYTE* lpCompany, FX_INT32& iCompanyLen,
		FX_BYTE* lpVersion, FX_INT32& iVersionLen, FX_BYTE* lpExtend, FX_INT32& iExtendLen);
	FX_INT32 GetDigestMethod(FX_BYTE* lpDigestMethod, FX_INT32& iDigestMethodLen);
	FX_INT32 Digest(FX_BYTE* plData, FX_INT32 iDataLen, FX_BYTE* plDigestMethod, FX_INT32 iDigestMethodLen,
		FX_BYTE* plDigestData, FX_INT32& iDigestDataLen);
	FX_INT32 GetErrMessage(FX_UINT32 uiErrCode, FX_BYTE* lpErrMessage, FX_INT32& iErrMessageLen);
	FX_INT32 GetSealInfo(FX_BYTE* pSealData, FX_INT32 iSealDataLen,
                        FX_BYTE* lpSealId, FX_INT32& iSealIdLen,
                        FX_BYTE* lpVersion, FX_INT32& iVersionLen,
                        FX_BYTE* lpVenderId, FX_INT32& iVenderIdLen,
                        FX_BYTE* lpSealType, FX_INT32& iSealTypeLen,
                        FX_BYTE* lpSealName, FX_INT32& iSealNameLen,
                        FX_BYTE* lpCertInfo, FX_INT32& iCertInfoLen,
                        FX_BYTE* lpValidStart, FX_INT32& iValidStartLen,
                        FX_BYTE* lpValidEnd, FX_INT32& iValidEndLen,
                        FX_BYTE* lpSignedDate, FX_INT32& iSignedDateLen,
                        FX_BYTE* lpSignerName, FX_INT32& iSignerNameLen,
                        FX_BYTE* lpSignMethod, FX_INT32& iSignMethodLen);

	// ���������ӿڣ�ΪV1 V2 �����汾���еĽӿڣ�����Ҳһ�£�V3�ӿ���˲�����ͬ
	virtual FX_INT32 Sign(FX_BYTE* lpSealId, FX_INT32 iSealIdLen, FX_BYTE* lpDocProperty, FX_INT32 iDocPropertyLen,
		FX_BYTE* lpDigestData, FX_INT32 iDigestDataLen, FX_BYTE* lpSignMethod, FX_INT32 iSignMethodLen, 
		FX_BYTE* lpSignDateTime,FX_INT32 iSignDateTimeLen, FX_BYTE* lpSignValue,FX_INT32& iSignValueLen);
	virtual FX_INT32 Verify(FX_BYTE* lpSealData, FX_INT32 iSealDataLen, FX_BYTE* lpDocProperty, FX_INT32 iDocPropertyLen, 
		FX_BYTE* lpDigestData, FX_INT32 iDigestDataLen, FX_BYTE* lpSignMethod, FX_INT32 iSignMethodLen, 
		FX_BYTE* lpSignDateTime,FX_INT32 iSignDateTimeLen, FX_BYTE* lpSignValue, FX_INT32 iSignValueLen, 
		FX_INT32 iOnline);

	// ���������ӿڣ�ΪV1 V2 �����汾���еĽӿڣ�����Ҳһ�£�V3û�д˽ӿ�
	FX_BOOL IsEmptyGetSeal();
	FX_INT32 GetSeal(const CFX_ByteString& bsSealID, FX_BYTE* lpSealData, FX_INT32& iSealDataLen);

	FX_BOOL IsEmptyGetSignMethod();
	FX_INT32 GetSignMethod(FX_BYTE* lpSignMethod, FX_INT32& iSignMethodLen);
	FX_BOOL IsEmptyGetSignDataTime();
	FX_INT32 GetSignDataTime(FX_BYTE* lpDateTime, FX_INT32& iDateTimeLen);
	
	// ����Ϊ����ӿں���
	virtual FX_INT32 LoadInterface(void* g_sealModule, CFX_ByteString bsPin) = 0;
	virtual FX_INT32 GetSealImage(const CFX_ByteString& bsSealData/*V0/V1 in*/, FX_INT32 iRenderFlag/*V1 in*/, 
		const CFX_ByteString& bsSealId/*V2 in*/, FX_BYTE* lpPictureData, FX_INT32& iPictureDataLen, 
		FX_BYTE* lpPictureType, FX_INT32& iPictureTypeLen, FX_INT32& PictureWidth, FX_INT32& PictureHeight) = 0;
	virtual FX_INT32 GetSignImage(FX_BYTE* lpData, FX_INT32 iDataLen, FX_BYTE* lpPictureData, FX_INT32& iPictureDataLen, 
		FX_BYTE* lpPictureType, FX_INT32& iPictureTypeLen) = 0;

	// added by xhj ,20200408
	virtual FX_INT32 GetOESVerion() = 0;

protected:
	Dll_OES_GetSealList g_OES_GetSealList;
	Dll_OES_GetProviderInfo g_OES_GetProviderInfo;
	Dll_OES_GetDigestMethod g_OES_GetDigestMethod;
	Dll_OES_Digest	g_OES_Digest;
	Dll_OES_GetErrMessage g_OES_GetErrMessage;

	Dll_OES_Sign g_OES_Sign;
	Dll_OES_Verify g_OES_Verify;

	Dll_OES_GetSeal g_OES_GetSeal;
	Dll__GetSealInfo	g_OES_GetSealInfo;
	Dll_OES_GetSignMethod	g_OES_GetSignMethod;
	Dll_OES_GetSignDateTime g_OES_GetSignDataTime;
};

//V1 �汾 OES��
class CFS_OESInterface_V0 : public CFS_OESInterface 
{
public:
	CFS_OESInterface_V0();
	virtual ~CFS_OESInterface_V0();
	
	//��̬����V1�汾OES�������еĽӿ�
	FX_INT32 LoadInterface(void* g_sealModule, CFX_ByteString bsPin);

	//V1�汾OES��δ�ṩ���������ӿ�
	FX_INT32 GetSealImage(const CFX_ByteString& bsSealData/*V0/V1 in*/, FX_INT32 iRenderFlag/*V1 in*/, 
		const CFX_ByteString& bsSealId/*V2 in*/, FX_BYTE* lpPictureData, FX_INT32& iPictureDataLen, 
		FX_BYTE* lpPictureType, FX_INT32& iPictureTypeLen, FX_INT32& PictureWidth, FX_INT32& PictureHeight);
	FX_INT32 GetSignImage(FX_BYTE* lpSealData, FX_INT32 iSealDataLen, FX_BYTE* lpPictureData, FX_INT32& iPictureDataLen, 
		FX_BYTE* lpPictureType, FX_INT32& iPictureTypeLen);

	FX_INT32 GetOESVerion()	{ return 1; }

protected:
	Dll_V0_OES_GetSealImage g_V0_OES_GetSealImage;
	Dll_V0_OES_SetPasswd	g_V0_OES_SetPassWord;
};

//V2 �汾 OES��
class CFS_OESInterface_V1 : public CFS_OESInterface 
{
public:
	CFS_OESInterface_V1();
	virtual ~CFS_OESInterface_V1();

	//��̬����V2�汾OES�������еĽӿ�
	FX_INT32 LoadInterface(void* g_sealModule, CFX_ByteString bsPin);

	FX_INT32 GetSealImage(const CFX_ByteString& bsSealData/*V0/V1 in*/, FX_INT32 iRenderFlag/*V1 in*/, 
		const CFX_ByteString& bsSealId/*V2 in*/, FX_BYTE* lpPictureData, FX_INT32& iPictureDataLen, 
		FX_BYTE* lpPictureType, FX_INT32& iPictureTypeLen, FX_INT32& PictureWidth, FX_INT32& PictureHeight);

	//V2�汾OES��δ�ṩ���½ӿ�
	FX_INT32 GetSignImage(FX_BYTE* lpSealData, FX_INT32 iSealDataLen, FX_BYTE* lpPictureData, FX_INT32& iPictureDataLen, 
		FX_BYTE* lpPictureType, FX_INT32& iPictureTypeLen);

	FX_INT32 GetOESVerion()	{ return 2; }

protected:
	FX_INT32 Login(FX_BYTE* lpPinData, FX_INT32 iPinDataLen);

protected:
	Dll_V1_OES_GetSealImage g_V1_OES_GetSealImage;
	Dll_V1_OES_GetSignImage	g_V1_OES_GetSignImage;

	Dll_V2_OES_Login g_V2_OES_Login;
	Dll_V2_OES_Logout g_V2_OES_Logout;
};

//V3 �汾 OES��
class CFS_OESInterface_V2 : public CFS_OESInterface 
{
public:
	CFS_OESInterface_V2();
	virtual ~CFS_OESInterface_V2();
	
	// ���������ӿڣ�ΪV3�ӿڣ���V1 V2 �����汾�Ľӿڲ�����һ��
	FX_INT32 Sign(FX_BYTE* lpSealId, FX_INT32 iSealIdLen, FX_BYTE* lpDocProperty, FX_INT32 iDocPropertyLen,
		FX_BYTE* lpDigestData, FX_INT32 iDigestDataLen, FX_BYTE* lpSignMethod, FX_INT32 iSignMethodLen, 
		FX_BYTE* lpSignDateTime,FX_INT32 iSignDateTimeLen, FX_BYTE* lpSignValue,FX_INT32& iSignValueLen);
	FX_INT32 Verify(FX_BYTE* lpSealData, FX_INT32 iSealDataLen, FX_BYTE* lpDocProperty, FX_INT32 iDocPropertyLen, 
		FX_BYTE* lpDigestData, FX_INT32 iDigestDataLen, FX_BYTE* lpSignMethod, FX_INT32 iSignMethodLen, 
		FX_BYTE* lpSignDateTime,FX_INT32 iSignDateTimeLen, FX_BYTE* lpSignValue, FX_INT32 iSignValueLen, 
		FX_INT32 iOnline);

	//��̬����V3�汾OES�������еĽӿ�
	FX_INT32 LoadInterface(void* g_sealModule, CFX_ByteString bsPin);

	//GetSealImage V3�ӿڣ�V1û�У�V2�е���V3������һ��
	FX_INT32 GetSealImage(const CFX_ByteString& bsSealData/*V0/V1 in*/, FX_INT32 iRenderFlag/*V1 in*/, 
		const CFX_ByteString& bsSealId/*V2 in*/, FX_BYTE* lpPictureData, FX_INT32& iPictureDataLen, 
		FX_BYTE* lpPictureType, FX_INT32& iPictureTypeLen, FX_INT32& PictureWidth, FX_INT32& PictureHeight);

	//���������ӿ� V3�У�V1��V2û��
	FX_INT32 GetSignImage(FX_BYTE* lpSignData, FX_INT32 iSignDataLen, FX_BYTE* lpPictureData, FX_INT32& iPictureDataLen, 
		FX_BYTE* lpPictureType, FX_INT32& iPictureTypeLen);
	FX_INT32 GetSignInfo(FX_BYTE* puchSignedValue,int iSignedValueLen,
						FX_BYTE* puchVersion, FX_INT32& iVersionLen,
						FX_BYTE* puchDateTime, FX_INT32& iDateTimeLen,
						FX_BYTE* puchDigestData, FX_INT32& iDigestDataLen,
						FX_BYTE* puchPropertyInfo, FX_INT32& iPropertyInfoLen,
						FX_BYTE* puchSignerCert, FX_INT32& iSignerCertLen,
                        FX_BYTE* puchSignature, FX_INT32& iSignatureLen);

	FX_INT32 GetOESVerion()	{ return 3; }

protected:
	//V3�ӿڣ�V1��V2û��
	FX_INT32 Login(FX_BYTE* lpPinData, FX_INT32 iPinDataLen);

protected:
	Dll_V2_OES_Sign g_V2_OES_Sign;
	Dll_V2_OES_Verify g_V2_OES_Verify;
	Dll_V2_OES_GetSealImage g_V2_OES_GetSealImage;
	Dll_V2_OES_GetSignImage g_V2_OES_GetSignImage;
	Dll_V2_OES_Login g_V2_OES_Login;
	Dll_V2_OES_Logout g_V2_OES_Logout;
	DLL_V2_OES_GetSignInfo g_V2_OES_GetSignInfo;
};




//V3 �汾 OES��
class CFS_OESInterface_V3 : public CFS_OESInterface
{
public:
    CFS_OESInterface_V3();
    virtual ~CFS_OESInterface_V3();
    
    // ���������ӿڣ�ΪV3�ӿڣ���V1 V2 �����汾�Ľӿڲ�����һ��
    FX_INT32 Sign(FX_BYTE* lpSealId, FX_INT32 iSealIdLen, FX_BYTE* lpDocProperty, FX_INT32 iDocPropertyLen,
        FX_BYTE* lpDigestData, FX_INT32 iDigestDataLen, FX_BYTE* lpSignMethod, FX_INT32 iSignMethodLen,
        FX_BYTE* lpSignDateTime,FX_INT32 iSignDateTimeLen, FX_BYTE* lpSignValue,FX_INT32& iSignValueLen);
    FX_INT32 Verify(FX_BYTE* lpSealData, FX_INT32 iSealDataLen, FX_BYTE* lpDocProperty, FX_INT32 iDocPropertyLen,
        FX_BYTE* lpDigestData, FX_INT32 iDigestDataLen, FX_BYTE* lpSignMethod, FX_INT32 iSignMethodLen,
        FX_BYTE* lpSignDateTime,FX_INT32 iSignDateTimeLen, FX_BYTE* lpSignValue, FX_INT32 iSignValueLen,
        FX_INT32 iOnline);

    //��̬����V3�汾OES�������еĽӿ�
    FX_INT32 LoadInterface(void* g_sealModule, CFX_ByteString bsPin);

    //GetSealImage V3�ӿڣ�V1û�У�V2�е���V3������һ��
    FX_INT32 GetSealImage(const CFX_ByteString& bsSealData/*V0/V1 in*/, FX_INT32 iRenderFlag/*V1 in*/,
        const CFX_ByteString& bsSealId/*V2 in*/, FX_BYTE* lpPictureData, FX_INT32& iPictureDataLen,
        FX_BYTE* lpPictureType, FX_INT32& iPictureTypeLen, FX_INT32& PictureWidth, FX_INT32& PictureHeight);

    //���������ӿ� V3�У�V1��V2û��
    FX_INT32 GetSignImage(FX_BYTE* lpSignData, FX_INT32 iSignDataLen, FX_BYTE* lpPictureData, FX_INT32& iPictureDataLen,
        FX_BYTE* lpPictureType, FX_INT32& iPictureTypeLen);
    FX_INT32 GetSignInfo(FX_BYTE* puchSignedValue,int iSignedValueLen,
                        FX_BYTE* puchVersion, FX_INT32& iVersionLen,
                        FX_BYTE* puchDateTime, FX_INT32& iDateTimeLen,
                        FX_BYTE* puchDigestData, FX_INT32& iDigestDataLen,
                        FX_BYTE* puchPropertyInfo, FX_INT32& iPropertyInfoLen,
                        FX_BYTE* puchSignerCert, FX_INT32& iSignerCertLen,
                        FX_BYTE* puchSignature, FX_INT32& iSignatureLen);

    FX_INT32 GetOESVerion()    { return 4; }

protected:
    //V3�ӿڣ�V1��V2û��
    FX_INT32 Login(FX_BYTE* lpPinData, FX_INT32 iPinDataLen);

protected:
    
    DLL_V3_OESV4_GetProviderInfo g_V3_OESV4_GetProviderInfo;
    DLL_V3_OESV4_GetProtocolVersion g_V3_OESV4_GetProtocolVersion;
    
    DLL_V3_OESV4_CloseSession g_V3_OESV4_CloseSession;
    DLL_V3_OESV4_GetSealList g_V3_OESV4_GetSealList;
    DLL_V3_OESV4_OESV4_GetCertList g_V3_OESV4_GetCertList;
    DLL_V3_OESV4_OESV4_GetSealImageById g_V3_OESV4_GetSealImageById;
    DLL_V3_OESV4_OESV4_GetSealInfoById g_V3_OESV4_GetSealInfoById;
    
    DLL_V3_OESV4_OESV4_GetCertById g_V3_OESV4_GetCertById;
    DLL_V3_OESV4_OESV4_GetSignImage g_V3_OESV4_GetSignImage;
    DLL_V3_OESV4_OESV4_GetSealImage g_V3_OESV4_GetSealImage;
    DLL_V3_OESV4_OESV4_GetSignInfo g_V3_OESV4_GetSignInfo;
    DLL_V3_OESV4_OESV4_GetRawSignInfo g_V3_OESV4_GetRawSignInfo;
    
    DLL_V3_OESV4_OESV4_GetDigestMethod g_V3_OESV4_GetDigestMethod;
    DLL_V3_OESV4_OESV4_Digest g_V3_OESV4_Digest;
    DLL_V3_OESV4_OESV4_Digest_Init g_V3_OESV4_Digest_Init;
    DLL_V3_OESV4_OESV4_Digest_Update g_V3_OESV4_Digest_Update;
    DLL_V3_OESV4_OESV4_Digest_Final g_V3_OESV4_Digest_Final;
    
    DLL_V3_OESV4_OESV4_Sign g_V3_OESV4_Sign;
    DLL_V3_OESV4_OESV4_RawSign g_V3_OESV4_RawSign;
    DLL_V3_OESV4_OESV4_Verify g_V3_OESV4_Verify;
    DLL_V3_OESV4_OESV4_GetErrMessage g_V3_OESV4_GetErrMessage;
    DLL_V3_OESV4_OESV4_Login g_V3_OESV4_Login;
  
    DLL_V3_OESV4_OESV4_Logout g_V3_OESV4_Logout;
    DLL_V3_OESV4_OESV4_GetSignMethod g_V3_OESV4_GetSignMethod;
    DLL_V3_OESV4_OESV4_GetDateTime g_V3_OESV4_GetDateTime;
    DLL_V3_OESV4_OESV4_GetParamNameList g_V3_OESV4_GetParamNameList;
    DLL_V3_OESV4_OESV4_SetExtendParam g_V3_OESV4_SetExtendParam;
   
    DLL_V3_OESV4_OESV4_GetTimeStamp g_V3_OESV4_GetTimeStamp;
    DLL_V3_OESV4_OESV4_VerifyTimeStamp g_V3_OESV4_VerifyTimeStamp;
    DLL_V3_OESV4_OESV4_GetTimeStampInfo g_V3_OESV4_GetTimeStampInfo;
    
    OES_HANDLE *g_piSession;
};
#endif //_FS_OFDSIGNATUREFACTORY_H_
