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
 * @brief OFD文件命名空间获取
 *
 * @param[in]	lpwszFileName		文件名。
 * @return 成功返回命名空间句柄，错误返回NULL。
 *
 */
 OFD_NAMESPACE OFD_Package_NameSpaceVerify(OFD_LPCWSTR lpwszFileName);

 /**
 * @brief OFD文件命名空间释放
 *
 * @param[in]	hNameSpace		命名空间句柄。
 * @return。
 *
 */
 void OFD_Package_NameSpaceRelease(OFD_NAMESPACE hNameSpace);

 /**
 * @brief OFD文件命名空间数量
 *
 * @param[in]	hNameSpace		命名空间句柄。
 * @return 成功命名空间数量。
 *
 */
 int OFD_Package_CountNameSpace(OFD_NAMESPACE hNameSpace);

  /**
 * @brief OFD文件命名空间数量
 *
 * @param[in]	hNameSpace		命名空间句柄。
 * @param[in]	nIndex			索引值。
 * @param[out]	wsTagName		命名空间标记名称。
 * @param[out]	wsPath			命名空间路径。
 * @param[out]	error			错误值。
 * @return。
 *
 */
 void OFD_Package_GetNameSpaceInfo(OFD_NAMESPACE hNameSpace, int nIndex, OFD_WSTR* wsTagName, OFD_WSTR* wsPath, OFD_DWORD* error);

/**
 * @brief OFD文件规范性校验
 *
 * @param[in]	hPackage		包句柄。
 * @return 成功错误描述信息个数，失败返回-1。
 *
 */
 OFD_PACKAGE OFD_Package_LoadFromVerify(OFD_LPCWSTR lpwszFileName);

/**
 * @brief OFD文件规范性校验
 *
 * @param[in]	hPackage		包句柄。
 * @return 无。
 *
 */
void	OFD_Package_Verify(OFD_PACKAGE hPackage);

/**
 * @brief OFD文件规范性校验
 *
 * @param[in]	hPackage		包句柄。
 * @return 错误信息个数。
 *
 */
int		OFD_Package_Verify_CountErrorInfo(OFD_PACKAGE hPackage);

/**
 * @brief OFD文件规范性校验
 *
 * @param[in]	hPackage		包句柄。
 * @return 错误信息个数。
 *
 */
int		OFD_Package_Verify_CountErrorInfoEx(OFD_PACKAGE hPackage);

/**
 * @brief 获取OFD规范错误描述信息句柄
 *
 * @param[in]	hPackage		包句柄。
 * @param[in]	index			错误序号
 * @return 成功返回OFD规范错误描述信息句柄，失败返回NULL。
 *
 */
OFD_ERRORINFO OFD_Package_Verify_GetErrorInfo(OFD_PACKAGE hPackage, int index);

/**
 * @brief 获取OFD规范错误描述信息句柄
 *
 * @param[in]	hPackage		包句柄。
 * @param[in]	index			错误序号
 * @return 成功返回OFD规范错误描述信息句柄，失败返回NULL。
 *
 */
OFD_ERRORINFO OFD_Package_Verify_GetErrorInfoEx(OFD_PACKAGE hPackage, int index);

/**
 * @brief OFD文件规范性校验
 *
 * @param[in]	hPackage		包句柄。
 * @return OFD规范xml和xsd对应信息个数。
 *
 */
int		OFD_Package_Verify_CountXmlXsdInfo(OFD_PACKAGE hPackage);

/**
 * @brief 获取OFD规范错误描述信息句柄
 *
 * @param[in]	hPackage		包句柄。
 * @param[in]	index			错误序号
 * @return 成功返回OFD规范xml和xsd对应信息句柄，失败返回NULL。
 *
 */
OFD_XMLXSDINFO OFD_Package_Verify_GetXmlXsdInfo(OFD_PACKAGE hPackage, int index);

/**
 * @brief 获取OFD规范错误信息的类型
 *
 * @param[in]	hErrorInfo		OFD规范错误描述信息句柄。
 * @return 成功错误描述信息标号，失败返回-1。
 *
 */
int	OFD_ErrorInfo_GetType(OFD_ERRORINFO hErrorInfo);

/**
 * @brief 获取OFD规范错误信息的类型
 *
 * @param[in]	hErrorInfo		OFD规范错误描述信息句柄。
 * @return 成功错误描述信息标号，失败返回-1。
 *
 */
int	OFD_ErrorInfoEx_GetType(OFD_ERRORINFO hErrorInfo);

/**
 * @brief OFD文件规范性校验
 *
 * @param[in]	hErrorInfo		OFD规范错误描述信息句柄。
 * @param[out]	filePath		错误信息文件路径。
 * @return 无。
 *
 */
void OFD_ErrorInfo_GetFilePath(OFD_ERRORINFO hErrorInfo,OFD_WSTR* filePath);

/**
 * @brief OFD文件规范性校验
 *
 * @param[in]	hErrorInfo		OFD规范错误描述信息句柄。
 * @param[out]	filePath		错误信息文件路径。
 * @return 无。
 *
 */
void OFD_ErrorInfoEx_GetFilePath(OFD_ERRORINFO hErrorInfo,OFD_BSTR* filePath);

/**
 * @brief OFD文件规范性校验
 *
 * @param[in]	hErrorInfo		OFD规范错误描述信息句柄。
 * @param[out]	xPath			错误信息节点路径。
 * @return 无。
 *
 */
void OFD_ErrorInfo_GetXPath(OFD_ERRORINFO hErrorInfo,OFD_WSTR* xPath);

/**
 * @brief OFD文件规范性校验
 *
 * @param[in]	hErrorInfo		OFD规范错误描述信息句柄。
 * @param[out]	xPath			错误信息节点路径。
 * @return 无。
 *
 */
void OFD_ErrorInfoEx_GetXPath(OFD_ERRORINFO hErrorInfo,OFD_BSTR* xPath);

/**
 * @brief OFD文件规范性校验
 *
 * @param[in]	hErrorInfo		OFD规范错误描述信息句柄。
 * @param[out]	tag				错误信息节点。
 * @return 无。
 *
 */
void OFD_ErrorInfo_GetTag(OFD_ERRORINFO hErrorInfo,OFD_WSTR* tag);

/**
 * @brief OFD文件规范性校验
 *
 * @param[in]	hErrorInfo		OFD规范错误描述信息句柄。
 * @param[out]	tagName			错误信息节点。
 * @return 无。
 *
 */
void OFD_ErrorInfoEx_GetTagName(OFD_ERRORINFO hErrorInfo,OFD_BSTR* tagName);

/**
 * @brief OFD文件规范性校验
 *
 * @param[in]	hErrorInfo		OFD规范错误描述信息句柄。
 * @param[out]	value			错误信息节点值。
 * @return 无。
 *
 */
void OFD_ErrorInfo_GetValue(OFD_ERRORINFO hErrorInfo, OFD_WSTR* value);

/**
 * @brief OFD文件规范性校验
 *
 * @param[in]	hErrorInfo		OFD规范错误描述信息句柄。
 * @param[out]	xmlInfo			错误信息节点值。
 * @return 无。
 *
 */
void OFD_ErrorInfoEx_GetXmlInfo(OFD_ERRORINFO hErrorInfo,OFD_BSTR* xmlInfo);

/**
 * @brief OFD文件规范性校验
 *
 * @param[in]	hErrorInfo		OFD规范错误描述信息句柄。
 * @param[out]	Description		错误描述信息。
 * @return 无。
 *
 */
void OFD_ErrorInfo_GetDescription(OFD_ERRORINFO hErrorInfo, OFD_WSTR* Description);

/**
 * @brief OFD文件规范性校验
 *
 * @param[in]	hErrorInfo		OFD规范错误描述信息句柄。
 * @param[out]	desc			错误描述信息。
 * @return 无。
 *
 */
void OFD_ErrorInfoEx_GetDesc(OFD_ERRORINFO hErrorInfo,OFD_BSTR* desc);

/**
 * @brief OFD文件规范性校验
 *
 * @param[in]	hXmlXsdInfo		OFD规范错误描述信息句柄。
 * @param[out]	xmlPath			xml路径。
 * @return 无。
 *
 */
void OFD_XmlXsdInfo_GetXmlPath(OFD_XMLXSDINFO hXmlXsdInfo, OFD_WSTR* xmlPath);

/**
 * @brief OFD文件规范性校验
 *
 * @param[in]	hXmlXsdInfo		OFD规范错误描述信息句柄。
 * @param[out]	xmlContent		xml字节流。
 * @param[out]	xmlSize			xml字节数。
 * @return 无。
 *
 */
OFD_BOOL OFD_XmlXsdInfo_GetXmlContent(OFD_XMLXSDINFO hXmlXsdInfo, OFD_LPBYTE xmlContent, int* xmlSize);

/**
 * @brief OFD文件规范性校验
 *
 * @param[in]	hXmlXsdInfo		OFD规范错误描述信息句柄。
 * @param[out]	xsdName			xsd名称。
 * @return 无。
 *
 */
void OFD_XmlXsdInfo_GetXsdName(OFD_XMLXSDINFO hXmlXsdInfo, OFD_WSTR* xsdName);

#ifdef __cplusplus
}
#endif

#endif //_OFD_VERIFY_R_H_