#ifndef _OFD_VERIFY_R_H_
#define _OFD_VERIFY_R_H_

#include "../include/ofd_base_r.h"

#ifdef __cplusplus
extern "C" {
#endif

enum OFDFILE_VERIFY_ERROR
{
	OFDFILE_VERIFY_UNKNOW = 0,
	OFDFILE_VERIFY_NoError,
	OFDFILE_VERIFY_Pause,
	OFDFILE_VERIFY_Parameter_ERROR,
	OFDFILE_VERIFY_FILE_ERROR,
	OFDFILE_VERIFY_XML_Parser_ERROR,
	OFDFILE_VERIFY_XML_Node_ERROR,
	OFDFILE_VERIFY_XML_TAG_Name_ERROR,
	OFDFILE_VERIFY_XML_TAG_Attr_ERROR,
	OFDFILE_VERIFY_XML_TAG_HasAttr_ERROR,
	OFDFILE_VERIFY_XML_TAG_Content_ERROR,
	OFDFILE_VERIFY_XML_TAG_HasChild_ERROR,
	OFDFILE_VERIFY_XML_DocInfo_Node_ERROR,
	OFDFILE_VERIFY_XML_DocInfo_TAG_Name_ERROR,
	OFDFILE_VERIFY_XML_DocInfo_TAG_Attr_ERROR,
	OFDFILE_VERIFY_XML_DocInfo_TAG_HasAttr_ERROR,
	OFDFILE_VERIFY_XML_DocInfo_TAG_Content_ERROR,
	OFDFILE_VERIFY_XML_DocInfo_HasChild_ERROR,
	OFDFILE_VERIFY_XML_Version_Node_ERROR,
	OFDFILE_VERIFY_XML_Version_TAG_Name_ERROR,
	OFDFILE_VERIFY_XML_Version_TAG_Attr_ERROR,
	OFDFILE_VERIFY_XML_Version_TAG_HasAttr_ERROR,
	OFDFILE_VERIFY_XML_Version_TAG_Content_ERROR,
	OFDFILE_VERIFY_XML_Version_HasChild_ERROR,
	OFDFILE_VERIFY_XML_Outline_Node_ERROR,
	OFDFILE_VERIFY_XML_Outline_TAG_Name_ERROR,
	OFDFILE_VERIFY_XML_Outline_TAG_Attr_ERROR,
	OFDFILE_VERIFY_XML_Outline_TAG_HasAttr_ERROR,
	OFDFILE_VERIFY_XML_Outline_TAG_Content_ERROR,
	OFDFILE_VERIFY_XML_Outline_HasChild_ERROR,
	OFDFILE_VERIFY_XML_Permissions_Node_ERROR,
	OFDFILE_VERIFY_XML_Permissions_TAG_Name_ERROR,
	OFDFILE_VERIFY_XML_Permissions_TAG_Attr_ERROR,
	OFDFILE_VERIFY_XML_Permissions_TAG_HasAttr_ERROR,
	OFDFILE_VERIFY_XML_Permissions_TAG_Content_ERROR,
	OFDFILE_VERIFY_XML_Permissions_HasChild_ERROR,
	OFDFILE_VERIFY_XML_VPreferences_Node_ERROR,
	OFDFILE_VERIFY_XML_VPreferences_TAG_Name_ERROR,
	OFDFILE_VERIFY_XML_VPreferences_TAG_Attr_ERROR,
	OFDFILE_VERIFY_XML_VPreferences_TAG_HasAttr_ERROR,
	OFDFILE_VERIFY_XML_VPreferences_TAG_Content_ERROR,
	OFDFILE_VERIFY_XML_VPreferences_HasChild_ERROR,
	OFDFILE_VERIFY_XML_Attachment_Node_ERROR,
	OFDFILE_VERIFY_XML_Attachment_TAG_Name_ERROR,
	OFDFILE_VERIFY_XML_Attachment_TAG_Attr_ERROR,
	OFDFILE_VERIFY_XML_Attachment_TAG_HasAttr_ERROR,
	OFDFILE_VERIFY_XML_Attachment_TAG_Content_ERROR,
	OFDFILE_VERIFY_XML_Attachment_HasChild_ERROR,
	OFDFILE_VERIFY_XML_Extension_Node_ERROR,
	OFDFILE_VERIFY_XML_Extension_TAG_Name_ERROR,
	OFDFILE_VERIFY_XML_Extension_TAG_Attr_ERROR,
	OFDFILE_VERIFY_XML_Extension_TAG_HasAttr_ERROR,
	OFDFILE_VERIFY_XML_Extension_TAG_Content_ERROR,
	OFDFILE_VERIFY_XML_Extension_HasChild_ERROR,
	OFDFILE_VERIFY_XML_Bookmark_Node_ERROR,
	OFDFILE_VERIFY_XML_Bookmark_TAG_Name_ERROR,
	OFDFILE_VERIFY_XML_Bookmark_TAG_Attr_ERROR,
	OFDFILE_VERIFY_XML_Bookmark_TAG_HasAttr_ERROR,
	OFDFILE_VERIFY_XML_Bookmark_TAG_Content_ERROR,
	OFDFILE_VERIFY_XML_Bookmark_HasChild_ERROR,
	OFDFILE_VERIFY_XML_Page_Node_ERROR,
	OFDFILE_VERIFY_XML_Page_TAG_Name_ERROR,
	OFDFILE_VERIFY_XML_Page_TAG_Attr_ERROR,
	OFDFILE_VERIFY_XML_Page_TAG_HasAttr_ERROR,
	OFDFILE_VERIFY_XML_Page_TAG_Content_ERROR,
	OFDFILE_VERIFY_XML_Page_HasChild_ERROR,
	OFDFILE_VERIFY_XML_PageArea_Node_ERROR,
	OFDFILE_VERIFY_XML_PageArea_TAG_Name_ERROR,
	OFDFILE_VERIFY_XML_PageArea_TAG_Attr_ERROR,
	OFDFILE_VERIFY_XML_PageArea_TAG_HasAttr_ERROR,
	OFDFILE_VERIFY_XML_PageArea_TAG_Content_ERROR,
	OFDFILE_VERIFY_XML_PageArea_HasChild_ERROR,
	OFDFILE_VERIFY_XML_Content_Node_ERROR,
	OFDFILE_VERIFY_XML_Content_TAG_Name_ERROR,
	OFDFILE_VERIFY_XML_Content_TAG_Attr_ERROR,
	OFDFILE_VERIFY_XML_Content_TAG_HasAttr_ERROR,
	OFDFILE_VERIFY_XML_Content_TAG_Content_ERROR,
	OFDFILE_VERIFY_XML_Content_HasChild_ERROR,
	OFDFILE_VERIFY_XML_Layer_Node_ERROR,
	OFDFILE_VERIFY_XML_Layer_TAG_Name_ERROR,
	OFDFILE_VERIFY_XML_Layer_TAG_Attr_ERROR,
	OFDFILE_VERIFY_XML_Layer_TAG_HasAttr_ERROR,
	OFDFILE_VERIFY_XML_Layer_TAG_Content_ERROR,
	OFDFILE_VERIFY_XML_Layer_HasChild_ERROR,
	OFDFILE_VERIFY_XML_Text_Node_ERROR,
	OFDFILE_VERIFY_XML_Text_TAG_Name_ERROR,
	OFDFILE_VERIFY_XML_Text_TAG_Attr_ERROR,
	OFDFILE_VERIFY_XML_Text_TAG_HasAttr_ERROR,
	OFDFILE_VERIFY_XML_Text_TAG_Content_ERROR,
	OFDFILE_VERIFY_XML_Text_HasChild_ERROR,
	OFDFILE_VERIFY_XML_Path_Node_ERROR,
	OFDFILE_VERIFY_XML_Path_TAG_Name_ERROR,
	OFDFILE_VERIFY_XML_Path_TAG_Attr_ERROR,
	OFDFILE_VERIFY_XML_Path_TAG_HasAttr_ERROR,
	OFDFILE_VERIFY_XML_Path_TAG_Content_ERROR,
	OFDFILE_VERIFY_XML_Path_HasChild_ERROR,
	OFDFILE_VERIFY_XML_Image_Node_ERROR,
	OFDFILE_VERIFY_XML_Image_TAG_Name_ERROR,
	OFDFILE_VERIFY_XML_Image_TAG_Attr_ERROR,
	OFDFILE_VERIFY_XML_Image_TAG_HasAttr_ERROR,
	OFDFILE_VERIFY_XML_Image_TAG_Content_ERROR,
	OFDFILE_VERIFY_XML_Image_HasChild_ERROR,
	OFDFILE_VERIFY_XML_Video_Node_ERROR,
	OFDFILE_VERIFY_XML_Video_TAG_Name_ERROR,
	OFDFILE_VERIFY_XML_Video_TAG_Attr_ERROR,
	OFDFILE_VERIFY_XML_Video_TAG_HasAttr_ERROR,
	OFDFILE_VERIFY_XML_Video_TAG_Content_ERROR,
	OFDFILE_VERIFY_XML_Video_HasChild_ERROR,
	OFDFILE_VERIFY_XML_Block_Node_ERROR,
	OFDFILE_VERIFY_XML_Block_TAG_Name_ERROR,
	OFDFILE_VERIFY_XML_Block_TAG_Attr_ERROR,
	OFDFILE_VERIFY_XML_Block_TAG_HasAttr_ERROR,
	OFDFILE_VERIFY_XML_Block_TAG_Content_ERROR,
	OFDFILE_VERIFY_XML_Block_HasChild_ERROR,
	OFDFILE_VERIFY_XML_Composite_Node_ERROR,
	OFDFILE_VERIFY_XML_Composite_TAG_Name_ERROR,
	OFDFILE_VERIFY_XML_Composite_TAG_Attr_ERROR,
	OFDFILE_VERIFY_XML_Composite_TAG_HasAttr_ERROR,
	OFDFILE_VERIFY_XML_Composite_TAG_Content_ERROR,
	OFDFILE_VERIFY_XML_Composite_HasChild_ERROR,
	OFDFILE_VERIFY_XML_Resource_Node_ERROR,
	OFDFILE_VERIFY_XML_Resource_TAG_Name_ERROR,
	OFDFILE_VERIFY_XML_Resource_TAG_Attr_ERROR,
	OFDFILE_VERIFY_XML_Resource_TAG_HasAttr_ERROR,
	OFDFILE_VERIFY_XML_Resource_TAG_Content_ERROR,
	OFDFILE_VERIFY_XML_Resource_HasChild_ERROR,
	OFDFILE_VERIFY_XML_ColorSpace_Node_ERROR,
	OFDFILE_VERIFY_XML_ColorSpace_TAG_Name_ERROR,
	OFDFILE_VERIFY_XML_ColorSpace_TAG_Attr_ERROR,
	OFDFILE_VERIFY_XML_ColorSpace_TAG_HasAttr_ERROR,
	OFDFILE_VERIFY_XML_ColorSpace_TAG_Content_ERROR,
	OFDFILE_VERIFY_XML_ColorSpace_HasChild_ERROR,
	OFDFILE_VERIFY_XML_DrawParam_Node_ERROR,
	OFDFILE_VERIFY_XML_DrawParam_TAG_Name_ERROR,
	OFDFILE_VERIFY_XML_DrawParam_TAG_Attr_ERROR,
	OFDFILE_VERIFY_XML_DrawParam_TAG_HasAttr_ERROR,
	OFDFILE_VERIFY_XML_DrawParam_TAG_Content_ERROR,
	OFDFILE_VERIFY_XML_DrawParam_HasChild_ERROR,
	OFDFILE_VERIFY_XML_Font_Node_ERROR,
	OFDFILE_VERIFY_XML_Font_TAG_Name_ERROR,
	OFDFILE_VERIFY_XML_Font_TAG_Attr_ERROR,
	OFDFILE_VERIFY_XML_Font_TAG_HasAttr_ERROR,
	OFDFILE_VERIFY_XML_Font_TAG_Content_ERROR,
	OFDFILE_VERIFY_XML_Font_HasChild_ERROR,
	OFDFILE_VERIFY_XML_Multimedia_Node_ERROR,
	OFDFILE_VERIFY_XML_Multimedia_TAG_Name_ERROR,
	OFDFILE_VERIFY_XML_Multimedia_TAG_Attr_ERROR,
	OFDFILE_VERIFY_XML_Multimedia_TAG_HasAttr_ERROR,
	OFDFILE_VERIFY_XML_Multimedia_TAG_Content_ERROR,
	OFDFILE_VERIFY_XML_Multimedia_HasChild_ERROR,
	OFDFILE_VERIFY_XML_CompositeUnit_Node_ERROR,
	OFDFILE_VERIFY_XML_CompositeUnit_TAG_Name_ERROR,
	OFDFILE_VERIFY_XML_CompositeUnit_TAG_Attr_ERROR,
	OFDFILE_VERIFY_XML_CompositeUnit_TAG_HasAttr_ERROR,
	OFDFILE_VERIFY_XML_CompositeUnit_TAG_Content_ERROR,
	OFDFILE_VERIFY_XML_CompositeUnit_HasChild_ERROR,
	OFDFILE_VERIFY_XML_Annotation_Node_ERROR,
	OFDFILE_VERIFY_XML_Annotation_TAG_Name_ERROR,
	OFDFILE_VERIFY_XML_Annotation_TAG_Attr_ERROR,
	OFDFILE_VERIFY_XML_Annotation_TAG_HasAttr_ERROR,
	OFDFILE_VERIFY_XML_Annotation_TAG_Content_ERROR,
	OFDFILE_VERIFY_XML_Annotation_HasChild_ERROR,
	OFDFILE_VERIFY_XML_PageAnnot_Node_ERROR,
	OFDFILE_VERIFY_XML_PageAnnot_TAG_Name_ERROR,
	OFDFILE_VERIFY_XML_PageAnnot_TAG_Attr_ERROR,
	OFDFILE_VERIFY_XML_PageAnnot_TAG_HasAttr_ERROR,
	OFDFILE_VERIFY_XML_PageAnnot_TAG_Content_ERROR,
	OFDFILE_VERIFY_XML_PageAnnot_HasChild_ERROR,
	OFDFILE_VERIFY_XML_Color_Node_ERROR,
	OFDFILE_VERIFY_XML_Color_TAG_Name_ERROR,
	OFDFILE_VERIFY_XML_Color_TAG_Attr_ERROR,
	OFDFILE_VERIFY_XML_Color_TAG_HasAttr_ERROR,
	OFDFILE_VERIFY_XML_Color_TAG_Content_ERROR,
	OFDFILE_VERIFY_XML_Color_HasChild_ERROR,
	OFDFILE_VERIFY_XML_Pattern_Node_ERROR,
	OFDFILE_VERIFY_XML_Pattern_TAG_Name_ERROR,
	OFDFILE_VERIFY_XML_Pattern_TAG_Attr_ERROR,
	OFDFILE_VERIFY_XML_Pattern_TAG_HasAttr_ERROR,
	OFDFILE_VERIFY_XML_Pattern_TAG_Content_ERROR,
	OFDFILE_VERIFY_XML_Pattern_HasChild_ERROR,
	OFDFILE_VERIFY_XML_Shading_Node_ERROR,
	OFDFILE_VERIFY_XML_Shading_TAG_Name_ERROR,
	OFDFILE_VERIFY_XML_Shading_TAG_Attr_ERROR,
	OFDFILE_VERIFY_XML_Shading_TAG_HasAttr_ERROR,
	OFDFILE_VERIFY_XML_Shading_TAG_Content_ERROR,
	OFDFILE_VERIFY_XML_Shading_HasChild_ERROR,
	OFDFILE_VERIFY_XML_Action_Node_ERROR,
	OFDFILE_VERIFY_XML_Action_TAG_Name_ERROR,
	OFDFILE_VERIFY_XML_Action_TAG_Attr_ERROR,
	OFDFILE_VERIFY_XML_Action_TAG_HasAttr_ERROR,
	OFDFILE_VERIFY_XML_Action_TAG_Content_ERROR,
	OFDFILE_VERIFY_XML_Action_HasChild_ERROR,
	OFDFILE_VERIFY_XML_Dest_Node_ERROR,
	OFDFILE_VERIFY_XML_Dest_TAG_Name_ERROR,
	OFDFILE_VERIFY_XML_Dest_TAG_Attr_ERROR,
	OFDFILE_VERIFY_XML_Dest_TAG_HasAttr_ERROR,
	OFDFILE_VERIFY_XML_Dest_TAG_Content_ERROR,
	OFDFILE_VERIFY_XML_Dest_HasChild_ERROR,
	OFDFILE_VERIFY_XML_Region_Node_ERROR,
	OFDFILE_VERIFY_XML_Region_TAG_Name_ERROR,
	OFDFILE_VERIFY_XML_Region_TAG_Attr_ERROR,
	OFDFILE_VERIFY_XML_Region_TAG_HasAttr_ERROR,
	OFDFILE_VERIFY_XML_Region_TAG_Content_ERROR,
	OFDFILE_VERIFY_XML_Region_HasChild_ERROR,
	OFDFILE_VERIFY_XML_Signature_Node_ERROR,
	OFDFILE_VERIFY_XML_Signature_TAG_Name_ERROR,
	OFDFILE_VERIFY_XML_Signature_TAG_Attr_ERROR,
	OFDFILE_VERIFY_XML_Signature_TAG_HasAttr_ERROR,
	OFDFILE_VERIFY_XML_Signature_TAG_Content_ERROR,
	OFDFILE_VERIFY_XML_Signature_HasChild_ERROR,
	OFDFILE_VERIFY_XML_SignReference_Node_ERROR,
	OFDFILE_VERIFY_XML_SignReference_TAG_Name_ERROR,
	OFDFILE_VERIFY_XML_SignReference_TAG_Attr_ERROR,
	OFDFILE_VERIFY_XML_SignReference_TAG_HasAttr_ERROR,
	OFDFILE_VERIFY_XML_SignReference_TAG_Content_ERROR,
	OFDFILE_VERIFY_XML_SignReference_HasChild_ERROR,
	OFDFILE_VERIFY_XML_Stamp_Node_ERROR,
	OFDFILE_VERIFY_XML_Stamp_TAG_Name_ERROR,
	OFDFILE_VERIFY_XML_Stamp_TAG_Attr_ERROR,
	OFDFILE_VERIFY_XML_Stamp_TAG_HasAttr_ERROR,
	OFDFILE_VERIFY_XML_Stamp_TAG_Content_ERROR,
	OFDFILE_VERIFY_XML_Stamp_HasChild_ERROR,
	OFDFILE_VERIFY_XML_CustomTag_Node_ERROR,
	OFDFILE_VERIFY_XML_CustomTag_TAG_Name_ERROR,
	OFDFILE_VERIFY_XML_CustomTag_TAG_Attr_ERROR,
	OFDFILE_VERIFY_XML_CustomTag_TAG_HasAttr_ERROR,
	OFDFILE_VERIFY_XML_CustomTag_TAG_Content_ERROR,
	OFDFILE_VERIFY_XML_CustomTag_HasChild_ERROR,
	OFDFILE_VERIFY_XML_OfficeDoc_Node_ERROR,
	OFDFILE_VERIFY_XML_OfficeDoc_TAG_Name_ERROR,
	OFDFILE_VERIFY_XML_OfficeDoc_TAG_Attr_ERROR,
	OFDFILE_VERIFY_XML_OfficeDoc_TAG_HasAttr_ERROR,
	OFDFILE_VERIFY_XML_OfficeDoc_TAG_Content_ERROR,
	OFDFILE_VERIFY_XML_OfficeDoc_HasChild_ERROR,
};

/**
 * @brief OFD�ļ������ռ��ȡ
 *
 * @param[in]	lpwszFileName		�ļ�����
 * @return �ɹ����������ռ��������󷵻�NULL��
 *
 */
 OFD_NAMESPACE OFD_Package_NameSpaceVerify(OFD_LPCWSTR lpwszFileName);

 /**
 * @brief OFD�ļ������ռ��ͷ�
 *
 * @param[in]	hNameSpace		�����ռ�����
 * @return��
 *
 */
 void OFD_Package_NameSpaceRelease(OFD_NAMESPACE hNameSpace);

 /**
 * @brief OFD�ļ������ռ�����
 *
 * @param[in]	hNameSpace		�����ռ�����
 * @return �ɹ������ռ�������
 *
 */
 int OFD_Package_CountNameSpace(OFD_NAMESPACE hNameSpace);

  /**
 * @brief OFD�ļ������ռ�����
 *
 * @param[in]	hNameSpace		�����ռ�����
 * @param[in]	nIndex			����ֵ��
 * @param[out]	wsTagName		�����ռ������ơ�
 * @param[out]	wsPath			�����ռ�·����
 * @param[out]	error			����ֵ��
 * @return��
 *
 */
 void OFD_Package_GetNameSpaceInfo(OFD_NAMESPACE hNameSpace, int nIndex, OFD_WSTR* wsTagName, OFD_WSTR* wsPath, OFD_DWORD* error);

/**
 * @brief OFD�ļ��淶��У��
 *
 * @param[in]	hPackage		�������
 * @return �ɹ�����������Ϣ������ʧ�ܷ���-1��
 *
 */
 OFD_PACKAGE OFD_Package_LoadFromVerify(OFD_LPCWSTR lpwszFileName);

/**
 * @brief OFD�ļ��淶��У��
 *
 * @param[in]	hPackage		�������
 * @return �ޡ�
 *
 */
void	OFD_Package_Verify(OFD_PACKAGE hPackage);

/**
 * @brief OFD�ļ��淶��У��
 *
 * @param[in]	hPackage		�������
 * @return ������Ϣ������
 *
 */
int		OFD_Package_Verify_CountErrorInfo(OFD_PACKAGE hPackage);

/**
 * @brief OFD�ļ��淶��У��
 *
 * @param[in]	hPackage		�������
 * @return ������Ϣ������
 *
 */
int		OFD_Package_Verify_CountErrorInfoEx(OFD_PACKAGE hPackage);

/**
 * @brief ��ȡOFD�淶����������Ϣ���
 *
 * @param[in]	hPackage		�������
 * @param[in]	index			�������
 * @return �ɹ�����OFD�淶����������Ϣ�����ʧ�ܷ���NULL��
 *
 */
OFD_ERRORINFO OFD_Package_Verify_GetErrorInfo(OFD_PACKAGE hPackage, int index);

/**
 * @brief ��ȡOFD�淶����������Ϣ���
 *
 * @param[in]	hPackage		�������
 * @param[in]	index			�������
 * @return �ɹ�����OFD�淶����������Ϣ�����ʧ�ܷ���NULL��
 *
 */
OFD_ERRORINFO OFD_Package_Verify_GetErrorInfoEx(OFD_PACKAGE hPackage, int index);

/**
 * @brief OFD�ļ��淶��У��
 *
 * @param[in]	hPackage		�������
 * @return OFD�淶xml��xsd��Ӧ��Ϣ������
 *
 */
int		OFD_Package_Verify_CountXmlXsdInfo(OFD_PACKAGE hPackage);

/**
 * @brief ��ȡOFD�淶����������Ϣ���
 *
 * @param[in]	hPackage		�������
 * @param[in]	index			�������
 * @return �ɹ�����OFD�淶xml��xsd��Ӧ��Ϣ�����ʧ�ܷ���NULL��
 *
 */
OFD_XMLXSDINFO OFD_Package_Verify_GetXmlXsdInfo(OFD_PACKAGE hPackage, int index);

/**
 * @brief ��ȡOFD�淶������Ϣ������
 *
 * @param[in]	hErrorInfo		OFD�淶����������Ϣ�����
 * @return �ɹ�����������Ϣ��ţ�ʧ�ܷ���-1��
 *
 */
int	OFD_ErrorInfo_GetType(OFD_ERRORINFO hErrorInfo);

/**
 * @brief ��ȡOFD�淶������Ϣ������
 *
 * @param[in]	hErrorInfo		OFD�淶����������Ϣ�����
 * @return �ɹ�����������Ϣ��ţ�ʧ�ܷ���-1��
 *
 */
int	OFD_ErrorInfoEx_GetType(OFD_ERRORINFO hErrorInfo);

/**
 * @brief OFD�ļ��淶��У��
 *
 * @param[in]	hErrorInfo		OFD�淶����������Ϣ�����
 * @param[out]	filePath		������Ϣ�ļ�·����
 * @return �ޡ�
 *
 */
void OFD_ErrorInfo_GetFilePath(OFD_ERRORINFO hErrorInfo,OFD_WSTR* filePath);

/**
 * @brief OFD�ļ��淶��У��
 *
 * @param[in]	hErrorInfo		OFD�淶����������Ϣ�����
 * @param[out]	filePath		������Ϣ�ļ�·����
 * @return �ޡ�
 *
 */
void OFD_ErrorInfoEx_GetFilePath(OFD_ERRORINFO hErrorInfo,OFD_BSTR* filePath);

/**
 * @brief OFD�ļ��淶��У��
 *
 * @param[in]	hErrorInfo		OFD�淶����������Ϣ�����
 * @param[out]	xPath			������Ϣ�ڵ�·����
 * @return �ޡ�
 *
 */
void OFD_ErrorInfo_GetXPath(OFD_ERRORINFO hErrorInfo,OFD_WSTR* xPath);

/**
 * @brief OFD�ļ��淶��У��
 *
 * @param[in]	hErrorInfo		OFD�淶����������Ϣ�����
 * @param[out]	xPath			������Ϣ�ڵ�·����
 * @return �ޡ�
 *
 */
void OFD_ErrorInfoEx_GetXPath(OFD_ERRORINFO hErrorInfo,OFD_BSTR* xPath);

/**
 * @brief OFD�ļ��淶��У��
 *
 * @param[in]	hErrorInfo		OFD�淶����������Ϣ�����
 * @param[out]	tag				������Ϣ�ڵ㡣
 * @return �ޡ�
 *
 */
void OFD_ErrorInfo_GetTag(OFD_ERRORINFO hErrorInfo,OFD_WSTR* tag);

/**
 * @brief OFD�ļ��淶��У��
 *
 * @param[in]	hErrorInfo		OFD�淶����������Ϣ�����
 * @param[out]	tagName			������Ϣ�ڵ㡣
 * @return �ޡ�
 *
 */
void OFD_ErrorInfoEx_GetTagName(OFD_ERRORINFO hErrorInfo,OFD_BSTR* tagName);

/**
 * @brief OFD�ļ��淶��У��
 *
 * @param[in]	hErrorInfo		OFD�淶����������Ϣ�����
 * @param[out]	value			������Ϣ�ڵ�ֵ��
 * @return �ޡ�
 *
 */
void OFD_ErrorInfo_GetValue(OFD_ERRORINFO hErrorInfo, OFD_WSTR* value);

/**
 * @brief OFD�ļ��淶��У��
 *
 * @param[in]	hErrorInfo		OFD�淶����������Ϣ�����
 * @param[out]	xmlInfo			������Ϣ�ڵ�ֵ��
 * @return �ޡ�
 *
 */
void OFD_ErrorInfoEx_GetXmlInfo(OFD_ERRORINFO hErrorInfo,OFD_BSTR* xmlInfo);

/**
 * @brief OFD�ļ��淶��У��
 *
 * @param[in]	hErrorInfo		OFD�淶����������Ϣ�����
 * @param[out]	Description		����������Ϣ��
 * @return �ޡ�
 *
 */
void OFD_ErrorInfo_GetDescription(OFD_ERRORINFO hErrorInfo, OFD_WSTR* Description);

/**
 * @brief OFD�ļ��淶��У��
 *
 * @param[in]	hErrorInfo		OFD�淶����������Ϣ�����
 * @param[out]	desc			����������Ϣ��
 * @return �ޡ�
 *
 */
void OFD_ErrorInfoEx_GetDesc(OFD_ERRORINFO hErrorInfo,OFD_BSTR* desc);

/**
 * @brief OFD�ļ��淶��У��
 *
 * @param[in]	hXmlXsdInfo		OFD�淶����������Ϣ�����
 * @param[out]	xmlPath			xml·����
 * @return �ޡ�
 *
 */
void OFD_XmlXsdInfo_GetXmlPath(OFD_XMLXSDINFO hXmlXsdInfo, OFD_WSTR* xmlPath);

/**
 * @brief OFD�ļ��淶��У��
 *
 * @param[in]	hXmlXsdInfo		OFD�淶����������Ϣ�����
 * @param[out]	xmlContent		xml�ֽ�����
 * @param[out]	xmlSize			xml�ֽ�����
 * @return �ޡ�
 *
 */
OFD_BOOL OFD_XmlXsdInfo_GetXmlContent(OFD_XMLXSDINFO hXmlXsdInfo, OFD_LPBYTE xmlContent, int* xmlSize);

/**
 * @brief OFD�ļ��淶��У��
 *
 * @param[in]	hXmlXsdInfo		OFD�淶����������Ϣ�����
 * @param[out]	xsdName			xsd���ơ�
 * @return �ޡ�
 *
 */
void OFD_XmlXsdInfo_GetXsdName(OFD_XMLXSDINFO hXmlXsdInfo, OFD_WSTR* xsdName);

#ifdef __cplusplus
}
#endif

#endif //_OFD_VERIFY_R_H_