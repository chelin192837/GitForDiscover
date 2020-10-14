#include "../include/oes_dll.h"
#include "../include/oes_error.h"

typedef struct tag_err_cuple
{
    FX_DWORD                    id;
    const wchar_t*              msg;
}OFD_ERR_CUPLE;

OFD_ERR_CUPLE g_mapErrMsg[] = 
{
    {OES_ERROR_BUFFER_NOT_ENUGH             ,	_T("缓冲区空间不足。") },
    {OES_ERROR_INVALID_PTR_NULL                ,	_T("错误的空指针。") },
    {OES_ERROR_READ_DATA_DENIED              ,	_T("数据读取失败。") },
    {OES_ERROR_SEALLIST_EMPTY                   ,	_T("印章列表为空。") },
    {OES_ERROR_SEAL_INVALID_ID                   ,	_T("错误的标识符，印章库中无对应的印章。") },
    {OES_ERROR_SEAL_INVALID_DATA              ,	_T("印章数据错误。") },
    {OES_ERROR_SEAL_NOT_OFFICIAL               ,	_T("指定的印章无效。") },
    {OES_ERROR_DIGEST_ERROR                      ,	_T("无效的算法。") },
    {OES_ERROR_DIGEST_NOTSUPPORT            ,	_T("摘要算法不支持。") },
    {OES_ERROR_SIGN_ERROR_PASSWORD      ,	_T("私钥保护密码错误。") },
    {OES_ERROR_SIGN_ERROR_KEY                  ,	_T("错误的私钥。") },
    {OES_ERROR_SIGN_TOBESIGN_DATA           ,	_T("待签数据错误。") },
    {OES_ERROR_SIGN_BAD_PARAM                  ,	_T("参数异常，签名失败。") },
    {OES_ERROR_VERIFY_FAILED                        ,	_T("验章失败, 文档在签章后被修改过。")	 },
    {OES_ERROR_VERIFY_SIGNDATA                   ,	_T("签章数据错误。") }
};

std::string GetErrMsg(unsigned long errCode)
{
    const wchar_t* pszMsg = NULL;
    int nCount = (sizeof(g_mapErrMsg) / sizeof(g_mapErrMsg[0]));
	for (int i = 0; i < nCount; i++)
	{
		OFD_ERR_CUPLE item = g_mapErrMsg[i];
		if (item.id == errCode)
		{
			pszMsg = item.msg;
		}
	}
	
    if (pszMsg == NULL)
	{
		pszMsg = _T("未知错误。");
	}

	size_t nMsgLen = wcslen(pszMsg);
    CFX_WideString wsMsg(pszMsg, nMsgLen);
    CFX_ByteString csMsg = wsMsg.UTF8Encode();

	std::string strErrMsg;
    strErrMsg.assign(csMsg.GetBuffer(csMsg.GetLength()), csMsg.GetLength());
    csMsg.ReleaseBuffer();

	return strErrMsg;
}
