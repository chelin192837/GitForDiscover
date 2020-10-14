
#include "fxlib.h"
#include "ofd/include/fx_time.h"
#include "include/fs_ofdsignaturefactory.h"
#include "include/fs_ofdsignature.h"
#include "../int/ofd_es.h"
#include "../htfoxit_oes/OES_DLL/include/oes.h"
#include "oes_V4T.h"
//
CFS_OESInterface::CFS_OESInterface()
{
	g_OES_GetSealList = NULL;
	g_OES_GetProviderInfo = NULL;
	g_OES_GetDigestMethod = NULL;
	g_OES_Digest = NULL;
	g_OES_GetErrMessage = NULL;

	g_OES_Sign = NULL;
	g_OES_Verify = NULL;

	g_OES_GetSeal = NULL;
	g_OES_GetSealInfo = NULL;
	g_OES_GetSignMethod = NULL;
	g_OES_GetSignDataTime = NULL;
}

//
CFS_OESInterface::~CFS_OESInterface()
{
    

}

//
FX_INT32 CFS_OESInterface::GetSealList(FX_BYTE* lpStampList, FX_INT32& StampListLen)
{
	if (!g_OES_GetSealList)
		return OFD_ERRCODE_NoSymbol;
	return g_OES_GetSealList(lpStampList, &StampListLen);
}

//
FX_INT32 CFS_OESInterface::GetProviderInfo(FX_BYTE* lpName, FX_INT32& iNameLen, FX_BYTE* lpCompany, FX_INT32& iCompanyLen,
				FX_BYTE* lpVersion, FX_INT32& iVersionLen, FX_BYTE* lpExtend, FX_INT32& iExtendLen)
{
	return g_OES_GetProviderInfo(lpName, &iNameLen, lpCompany, &iCompanyLen, 
		lpVersion, &iVersionLen, lpExtend, &iExtendLen);
	LOGD("GetProviderInfo");
}

//
FX_INT32 CFS_OESInterface::GetDigestMethod(FX_BYTE* lpDigestMethod, FX_INT32& iDigestMethodLen)
{
	return g_OES_GetDigestMethod(lpDigestMethod, &iDigestMethodLen);
}

//
FX_INT32 CFS_OESInterface::Digest(FX_BYTE* plData, FX_INT32 iDataLen, FX_BYTE* plDigestMethod, FX_INT32 iDigestMethodLen,
		FX_BYTE* plDigestData, FX_INT32& iDigestDataLen)
{
	return g_OES_Digest(plData, iDataLen, plDigestMethod, iDigestMethodLen, plDigestData, &iDigestDataLen);
}

//
FX_INT32 CFS_OESInterface::GetErrMessage(FX_UINT32 uiErrCode, FX_BYTE* lpErrMessage, FX_INT32& iErrMessageLen)
{
	return g_OES_GetErrMessage(uiErrCode, lpErrMessage, &iErrMessageLen);
}

//
FX_INT32 CFS_OESInterface::Sign(FX_BYTE* lpSealId, FX_INT32 iSealIdLen, FX_BYTE* lpDocProperty, FX_INT32 iDocPropertyLen,
		FX_BYTE* lpDigestData, FX_INT32 iDigestDataLen, FX_BYTE* lpSignMethod, FX_INT32 iSignMethodLen, 
		FX_BYTE* lpSignDateTime,FX_INT32 iSignDateTimeLen, FX_BYTE* lpSignValue,FX_INT32& iSignValueLen)
{
	return g_OES_Sign(lpSealId, iSealIdLen, lpDocProperty, iDocPropertyLen, 
		lpDigestData, iDigestDataLen, lpSignMethod, iSignMethodLen, 
		lpSignDateTime, iSignDateTimeLen, lpSignValue, &iSignValueLen);
}

//
FX_INT32 CFS_OESInterface::Verify(FX_BYTE* lpSealData, FX_INT32 iSealDataLen, FX_BYTE* lpDocProperty, FX_INT32 iDocPropertyLen, 
		FX_BYTE* lpDigestData, FX_INT32 iDigestDataLen, FX_BYTE* lpSignMethod, FX_INT32 iSignMethodLen, 
		FX_BYTE* lpSignDateTime,FX_INT32 iSignDateTimeLen, FX_BYTE* lpSignValue, FX_INT32 iSignValueLen, 
		FX_INT32 iOnline)
{
	return g_OES_Verify(lpSealData, iSealDataLen, lpDocProperty, iDocPropertyLen, 
		lpDigestData, iDigestDataLen, lpSignMethod, iSignMethodLen, 
		lpSignDateTime, iSignDateTimeLen, lpSignValue, iSignValueLen, 
		iOnline);
}

//
FX_BOOL CFS_OESInterface::IsEmptyGetSeal()
{
	if(!g_OES_GetSeal)
		return TRUE;
	
	return FALSE;
}

//
FX_INT32 CFS_OESInterface::GetSeal(const CFX_ByteString& bsSealID, FX_BYTE* lpSealData, FX_INT32& iSealDataLen)
{
	FX_LPBYTE lpSealID = (FX_LPBYTE)(FX_LPCSTR)bsSealID;
	FX_INT32 iSealIDSize = bsSealID.GetLength();
	
	return g_OES_GetSeal(lpSealID, iSealIDSize, lpSealData, &iSealDataLen);
}

FX_INT32 CFS_OESInterface::GetSealInfo(FX_BYTE* pSealData, FX_INT32 iSealDataLen,
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
										FX_BYTE* lpSignMethod, FX_INT32& iSignMethodLen)
{
	//FX_LPBYTE lpSealData = (FX_LPBYTE)(FX_LPCSTR)bsSeaData;
	//FX_INT32 iSealDataSize = bsSeaData.GetLength();
	if(pSealData == NULL || iSealDataLen <= 0)
		return -1;

	return g_OES_GetSealInfo(pSealData, iSealDataLen
							, lpSealId, &iSealIdLen
							, lpVersion, &iVersionLen
							, lpVenderId, &iVenderIdLen
							, lpSealType, &iSealTypeLen
							, lpSealName, &iSealNameLen
							, lpCertInfo, &iCertInfoLen
							, lpValidStart, &iValidStartLen
							, lpValidEnd, &iValidEndLen
							, lpSignedDate, &iSignedDateLen
							, lpSignerName, &iSignerNameLen
							, lpSignMethod, &iSignMethodLen
							);
}

//
FX_BOOL CFS_OESInterface::IsEmptyGetSignMethod()
{
	if(!g_OES_GetSignMethod)
		return TRUE;
	
	return FALSE;
}

//
FX_INT32 CFS_OESInterface::GetSignMethod(FX_BYTE* lpSignMethod, FX_INT32& iSignMethodLen)
{
	return g_OES_GetSignMethod(lpSignMethod, &iSignMethodLen);
}

FX_BOOL CFS_OESInterface::IsEmptyGetSignDataTime()
{
	if(!g_OES_GetSignDataTime)
		return TRUE;
	
	return FALSE;
}

//
FX_INT32 CFS_OESInterface::GetSignDataTime(FX_BYTE* lpDateTime, FX_INT32& iDateTimeLen)
{
	return g_OES_GetSignDataTime(lpDateTime, &iDateTimeLen);
}

//
CFS_OESInterface_V0::CFS_OESInterface_V0()
{

}

//
CFS_OESInterface_V0::~CFS_OESInterface_V0()
{

}

//
FX_INT32 CFS_OESInterface_V0::LoadInterface(void* g_sealModule, CFX_ByteString bsPin)
{
#if _FX_OS_ == _FX_WIN32_DESKTOP_ || _FX_OS_ == _FX_WIN64_
	g_OES_GetSealList = (Dll_OES_GetSealList)GetProcAddress((HINSTANCE)g_sealModule, "OES_GetSealList");
	g_OES_GetProviderInfo = (Dll_OES_GetProviderInfo)GetProcAddress((HINSTANCE)g_sealModule, "OES_GetProviderInfo");
	g_OES_GetDigestMethod = (Dll_OES_GetDigestMethod)GetProcAddress((HINSTANCE)g_sealModule, "OES_GetDigestMethod");
	g_OES_Digest = (Dll_OES_Digest)GetProcAddress((HINSTANCE)g_sealModule, "OES_Digest");
	g_OES_GetErrMessage = (Dll_OES_GetErrMessage)GetProcAddress((HINSTANCE)g_sealModule, "OES_GetErrMessage");

	g_OES_Sign = (Dll_OES_Sign)GetProcAddress((HINSTANCE)g_sealModule, "OES_Sign");
	g_OES_Verify = (Dll_OES_Verify)GetProcAddress((HINSTANCE)g_sealModule, "OES_Verify");

	g_OES_GetSeal = (Dll_OES_GetSeal)GetProcAddress((HINSTANCE)g_sealModule, "OES_GetSeal");
	g_OES_GetSealInfo = (Dll__GetSealInfo)GetProcAddress((HINSTANCE)g_sealModule, "OES_GetSealInfo");
	g_OES_GetSignMethod = (Dll_OES_GetSignMethod)GetProcAddress((HINSTANCE)g_sealModule, "OES_GetSignMethod");
	g_OES_GetSignDataTime = (Dll_OES_GetSignDateTime)GetProcAddress((HINSTANCE)g_sealModule, "OES_GetSignDateTime");
	//�����Ķ����ж�oes��׼���޸�
	g_V0_OES_GetSealImage = (Dll_V0_OES_GetSealImage)GetProcAddress((HINSTANCE)g_sealModule, "OES_GetSealImage");
	g_V0_OES_SetPassWord = (Dll_V0_OES_SetPasswd)GetProcAddress((HINSTANCE)g_sealModule, "OES_SetPasswd");
#else
	g_OES_GetSealList = (Dll_OES_GetSealList)dlsym(g_sealModule, "OES_GetSealList");
	g_OES_GetProviderInfo = (Dll_OES_GetProviderInfo)dlsym(g_sealModule, "OES_GetProviderInfo");
	g_OES_GetDigestMethod = (Dll_OES_GetDigestMethod)dlsym(g_sealModule, "OES_GetDigestMethod");
	g_OES_Digest = (Dll_OES_Digest)dlsym(g_sealModule, "OES_Digest");
	g_OES_GetErrMessage = (Dll_OES_GetErrMessage)dlsym(g_sealModule, "OES_GetErrMessage");

	g_OES_Sign = (Dll_OES_Sign)dlsym(g_sealModule, "OES_Sign");
	g_OES_Verify = (Dll_OES_Verify)dlsym(g_sealModule, "OES_Verify");

	g_OES_GetSeal = (Dll_OES_GetSeal)dlsym(g_sealModule, "OES_GetSeal");
	g_OES_GetSealInfo = (Dll__GetSealInfo)dlsym(g_sealModule, "OES_GetSealInfo");
	g_OES_GetSignMethod = (Dll_OES_GetSignMethod)dlsym(g_sealModule, "OES_GetSignMethod");
	g_OES_GetSignDataTime = (Dll_OES_GetSignDateTime)dlsym(g_sealModule, "OES_GetSignDateTime");

	g_V0_OES_GetSealImage = (Dll_V0_OES_GetSealImage)dlsym(g_sealModule, "OES_GetSealImage");
	g_V0_OES_SetPassWord = (Dll_V0_OES_SetPasswd)dlsym(g_sealModule, "OES_SetPasswd");
	LOGD("g_OES_GetProviderInfo:%ld", g_OES_GetProviderInfo);
    LOGD("g_V0_OES_SetPassWord:%ld", g_V0_OES_SetPassWord);
#endif
	if (!g_OES_GetSealList)
		return OFD_ERRCODE_NoSymbol;


	//if(g_V0_OES_SetPassWord)
	   // g_V0_OES_SetPassWord(bsPin.GetLength(), (FX_LPBYTE)FX_LPCBYTE(bsPin));

	return 0;
}

//
FX_INT32 CFS_OESInterface_V0::GetSealImage(const CFX_ByteString& bsSealData/*V0/V1 in*/, FX_INT32 iRenderFlag/*V1 in*/, 
		const CFX_ByteString& bsSealId/*V2 in*/, FX_BYTE* lpPictureData, FX_INT32& iPictureDataLen, 
		FX_BYTE* lpPictureType, FX_INT32& iPictureTypeLen, FX_INT32& PictureWidth, FX_INT32& PictureHeight)

{
	int res = OES_OK;
	FX_LPCBYTE plSealData = (FX_LPCBYTE)bsSealData;
	FX_INT32 iSealDataSize = bsSealData.GetLength();
	
	if(FS_Decode_Sealdata(plSealData, iSealDataSize, lpPictureData, iPictureDataLen, lpPictureType, iPictureTypeLen))
		res = OES_OK;
	if(lpPictureData || lpPictureType)
		return res;
	if(FS_Ber_Decode_PicSize(plSealData, iSealDataSize, (long&)PictureWidth, (long&)PictureHeight))
		res = OES_OK;

	return res;
}

//V1�汾û�д˽ӿ�
FX_INT32 CFS_OESInterface_V0::GetSignImage(FX_BYTE* lpSealData, FX_INT32 iSealDataLen, FX_BYTE* lpPictureData, FX_INT32& iPictureDataLen, 
	FX_BYTE* lpPictureType, FX_INT32& iPictureTypeLen)
{
	int res = OES_OK;

	if(FS_Decode_Sealdata(lpSealData, iSealDataLen, lpPictureData, iPictureDataLen, lpPictureType, iPictureTypeLen))
		res = OES_OK;

	return res;
}

//
CFS_OESInterface_V1::CFS_OESInterface_V1()
{
	g_V1_OES_GetSealImage = NULL;
	g_V1_OES_GetSignImage = NULL;
}

//
CFS_OESInterface_V1::~CFS_OESInterface_V1()
{

}

//
FX_INT32 CFS_OESInterface_V1::LoadInterface(void* g_sealModule, CFX_ByteString bsPin)
{
#if _FX_OS_ == _FX_WIN32_DESKTOP_ || _FX_OS_ == _FX_WIN64_
	g_OES_GetSealList = (Dll_OES_GetSealList)GetProcAddress((HINSTANCE)g_sealModule, "OES_GetSealList");
	g_OES_GetProviderInfo = (Dll_OES_GetProviderInfo)GetProcAddress((HINSTANCE)g_sealModule, "OES_GetProviderInfo");
	g_OES_GetDigestMethod = (Dll_OES_GetDigestMethod)GetProcAddress((HINSTANCE)g_sealModule, "OES_GetDigestMethod");
	g_OES_Digest = (Dll_OES_Digest)GetProcAddress((HINSTANCE)g_sealModule, "OES_Digest");
	g_OES_GetErrMessage = (Dll_OES_GetErrMessage)GetProcAddress((HINSTANCE)g_sealModule, "OES_GetErrMessage");

	g_OES_Sign = (Dll_OES_Sign)GetProcAddress((HINSTANCE)g_sealModule, "OES_Sign");
	g_OES_Verify = (Dll_OES_Verify)GetProcAddress((HINSTANCE)g_sealModule, "OES_Verify");

	g_OES_GetSeal = (Dll_OES_GetSeal)GetProcAddress((HINSTANCE)g_sealModule, "OES_GetSeal");
	g_OES_GetSealInfo = (Dll__GetSealInfo)GetProcAddress((HINSTANCE)g_sealModule, "OES_GetSealInfo");
	g_OES_GetSignMethod = (Dll_OES_GetSignMethod)GetProcAddress((HINSTANCE)g_sealModule, "OES_GetSignMethod");
	g_OES_GetSignDataTime = (Dll_OES_GetSignDateTime)GetProcAddress((HINSTANCE)g_sealModule, "OES_GetSignDateTime");

	g_V1_OES_GetSealImage = (Dll_V1_OES_GetSealImage)GetProcAddress((HINSTANCE)g_sealModule, "OES_GetSealImage");
	g_V1_OES_GetSignImage = (Dll_V1_OES_GetSignImage)GetProcAddress((HINSTANCE)g_sealModule, "OES_GetSignImage");

	g_V2_OES_Login = (Dll_V2_OES_Login)GetProcAddress((HINSTANCE)g_sealModule, "OES_Login");
	g_V2_OES_Logout = (Dll_V2_OES_Logout)GetProcAddress((HINSTANCE)g_sealModule, "OES_Logout");
#else
	g_OES_GetSealList = (Dll_OES_GetSealList)dlsym(g_sealModule, "OES_GetSealList");
	g_OES_GetProviderInfo = (Dll_OES_GetProviderInfo)dlsym(g_sealModule, "OES_GetProviderInfo");
	g_OES_GetDigestMethod = (Dll_OES_GetDigestMethod)dlsym(g_sealModule, "OES_GetDigestMethod");
	g_OES_Digest = (Dll_OES_Digest)dlsym(g_sealModule, "OES_Digest");
	g_OES_GetErrMessage = (Dll_OES_GetErrMessage)dlsym(g_sealModule, "OES_GetErrMessage");

	g_OES_Sign = (Dll_OES_Sign)dlsym(g_sealModule, "OES_Sign");
	g_OES_Verify = (Dll_OES_Verify)dlsym(g_sealModule, "OES_Verify");

	g_OES_GetSeal = (Dll_OES_GetSeal)dlsym(g_sealModule, "OES_GetSeal");
	g_OES_GetSealInfo = (Dll__GetSealInfo)dlsym(g_sealModule, "OES_GetSealInfo");
	g_OES_GetSignMethod = (Dll_OES_GetSignMethod)dlsym(g_sealModule, "OES_GetSignMethod");
	g_OES_GetSignDataTime = (Dll_OES_GetSignDateTime)dlsym(g_sealModule, "OES_GetSignDateTime");

	g_V1_OES_GetSealImage = (Dll_V1_OES_GetSealImage)dlsym(g_sealModule, "OES_GetSealImage");
	g_V1_OES_GetSignImage = (Dll_V1_OES_GetSignImage)dlsym(g_sealModule, "OES_GetSignImage");

	g_V2_OES_Login = (Dll_V2_OES_Login)dlsym(g_sealModule, "OES_Login");
	g_V2_OES_Logout = (Dll_V2_OES_Logout)dlsym(g_sealModule, "OES_Logout");
#endif
	if (!g_OES_GetSealList)
		return OFD_ERRCODE_NoSymbol;

	if(!g_V2_OES_Login)
		return OES_PIN;
	int ret = 0;
	if(bsPin.GetLength()!=0)
		ret = Login((FX_LPBYTE)(FX_LPCSTR)bsPin, bsPin.GetLength());
	return ret;
}

//
FX_INT32 CFS_OESInterface_V1::GetSealImage(const CFX_ByteString& bsSealData/*V0/V1 in*/, FX_INT32 iRenderFlag/*V1 in*/, 
		const CFX_ByteString& bsSealId/*V2 in*/, FX_BYTE* lpPictureData, FX_INT32& iPictureDataLen, 
		FX_BYTE* lpPictureType, FX_INT32& iPictureTypeLen, FX_INT32& PictureWidth, FX_INT32& PictureHeight)

{
	FX_LPCBYTE plSealData = (FX_LPCBYTE)bsSealData;
	FX_INT32 iSealDataSize = bsSealData.GetLength();
	LOGD("sealData %d,seal length %d",plSealData, iSealDataSize);
	return g_V1_OES_GetSealImage((FX_LPBYTE)plSealData, iSealDataSize, iRenderFlag, 
		lpPictureData, &iPictureDataLen, &PictureWidth, &PictureHeight);
}

//
FX_INT32 CFS_OESInterface_V1::GetSignImage(FX_BYTE* lpSealData, FX_INT32 iSealDataLen, FX_BYTE* lpPictureData, FX_INT32& iPictureDataLen, 
	FX_BYTE* lpPictureType, FX_INT32& iPictureTypeLen)
{
	int res = OES_OK;
	//int PictureWidth = 0;
	//int PictureHeight = 0;

	//if(!g_V1_OES_GetSealImage)
	//	return OFD_ERRCODE_NoSymbol;
	//res = g_V1_OES_GetSealImage(lpSealData, iSealDataLen, OES_SEALIMAGE_FLAG_DISPLAY, lpPictureData, &iPictureDataLen, &PictureWidth, &PictureHeight)

	if(FS_Decode_Sealdata(lpSealData, iSealDataLen, lpPictureData, iPictureDataLen, lpPictureType, iPictureTypeLen))
		res = OES_OK;

	return res;
}

//
FX_INT32 CFS_OESInterface_V1::Login(FX_BYTE* lpPinData, FX_INT32 iPinDataLen)
{
    FX_INT32 ret = g_V2_OES_Login(lpPinData, iPinDataLen);
    LOGD("login V1 res:%d",ret);
	return ret;
}


//
CFS_OESInterface_V2::CFS_OESInterface_V2()
{
	g_V2_OES_Sign = NULL;
	g_V2_OES_Verify = NULL;
	g_V2_OES_GetSealImage = NULL;
	g_V2_OES_GetSignImage = NULL; 
	g_V2_OES_Login = NULL;
	g_V2_OES_Logout = NULL;
	g_V2_OES_GetSignInfo = NULL;
}

//
CFS_OESInterface_V2::~CFS_OESInterface_V2()
{
	if(g_V2_OES_Logout)
		g_V2_OES_Logout();
}

//
FX_INT32 CFS_OESInterface_V2::LoadInterface(void* g_sealModule, CFX_ByteString bsPin)
{
#if _FX_OS_ == _FX_WIN32_DESKTOP_ || _FX_OS_ == _FX_WIN64_
	g_OES_GetSealList = (Dll_OES_GetSealList)GetProcAddress((HINSTANCE)g_sealModule, "OES_GetSealList");
	g_OES_GetProviderInfo = (Dll_OES_GetProviderInfo)GetProcAddress((HINSTANCE)g_sealModule, "OES_GetProviderInfo");
	g_OES_GetDigestMethod = (Dll_OES_GetDigestMethod)GetProcAddress((HINSTANCE)g_sealModule, "OES_GetDigestMethod");
	g_OES_Digest = (Dll_OES_Digest)GetProcAddress((HINSTANCE)g_sealModule, "OES_Digest");
	g_OES_GetErrMessage = (Dll_OES_GetErrMessage)GetProcAddress((HINSTANCE)g_sealModule, "OES_GetErrMessage");

	g_V2_OES_Sign = (Dll_V2_OES_Sign)GetProcAddress((HINSTANCE)g_sealModule, "OES_Sign");
	g_V2_OES_Verify = (Dll_V2_OES_Verify)GetProcAddress((HINSTANCE)g_sealModule, "OES_Verify");

	g_V2_OES_GetSealImage = (Dll_V2_OES_GetSealImage)GetProcAddress((HINSTANCE)g_sealModule, "OES_GetSealImage");
	g_V2_OES_GetSignImage = (Dll_V2_OES_GetSignImage)GetProcAddress((HINSTANCE)g_sealModule, "OES_GetSignImage");
	g_V2_OES_Login = (Dll_V2_OES_Login)GetProcAddress((HINSTANCE)g_sealModule, "OES_Login");
	g_V2_OES_Logout = (Dll_V2_OES_Logout)GetProcAddress((HINSTANCE)g_sealModule, "OES_Logout");
	g_V2_OES_GetSignInfo = (DLL_V2_OES_GetSignInfo)GetProcAddress((HINSTANCE)g_sealModule, "OES_GetSignInfo");
#else
	g_OES_GetSealList = (Dll_OES_GetSealList)dlsym(g_sealModule, "OES_GetSealList");
	g_OES_GetProviderInfo = (Dll_OES_GetProviderInfo)dlsym(g_sealModule, "OES_GetProviderInfo");
	g_OES_GetDigestMethod = (Dll_OES_GetDigestMethod)dlsym(g_sealModule, "OES_GetDigestMethod");
	g_OES_Digest = (Dll_OES_Digest)dlsym(g_sealModule, "OES_Digest");
	g_OES_GetErrMessage = (Dll_OES_GetErrMessage)dlsym(g_sealModule, "OES_GetErrMessage");

	g_V2_OES_Sign = (Dll_V2_OES_Sign)dlsym(g_sealModule, "OES_Sign");
	g_V2_OES_Verify = (Dll_V2_OES_Verify)dlsym(g_sealModule, "OES_Verify");

	g_V2_OES_GetSealImage = (Dll_V2_OES_GetSealImage)dlsym(g_sealModule, "OES_GetSealImage");
	g_V2_OES_GetSignImage = (Dll_V2_OES_GetSignImage)dlsym(g_sealModule, "OES_GetSignImage");
	g_V2_OES_Login = (Dll_V2_OES_Login)dlsym(g_sealModule, "OES_Login");
	g_V2_OES_Logout = (Dll_V2_OES_Logout)dlsym(g_sealModule, "OES_Logout");
	g_V2_OES_GetSignInfo = (DLL_V2_OES_GetSignInfo)dlsym(g_sealModule, "OES_GetSignInfo");
#endif

	if (!g_OES_GetSealList)
		return OFD_ERRCODE_NoSymbol;

	//��������ǩ��OES g_V2_OES_Login �ӿ�û�У��ʴ���Ҫע�͵���������0
	//return 0;

	if(!g_V2_OES_Login)
		return OES_PIN;

	return Login((FX_LPBYTE)(FX_LPCSTR)bsPin, bsPin.GetLength());
}

//
FX_INT32 CFS_OESInterface_V2::Sign(FX_BYTE* lpSealId, FX_INT32 iSealIdLen, FX_BYTE* lpDocProperty, FX_INT32 iDocPropertyLen,
		FX_BYTE* lpDigestData, FX_INT32 iDigestDataLen, FX_BYTE* lpSignMethod, FX_INT32 iSignMethodLen, 
		FX_BYTE* lpSignDateTime,FX_INT32 iSignDateTimeLen, FX_BYTE* lpSignValue,FX_INT32& iSignValueLen)
{
	return g_V2_OES_Sign(lpSealId, iSealIdLen, lpDocProperty, iDocPropertyLen, 
		lpDigestData, iDigestDataLen, lpSignValue, &iSignValueLen);
}

//
FX_INT32 CFS_OESInterface_V2::Verify(FX_BYTE* lpSealData, FX_INT32 iSealDataLen, FX_BYTE* lpDocProperty, FX_INT32 iDocPropertyLen, 
		FX_BYTE* lpDigestData, FX_INT32 iDigestDataLen, FX_BYTE* lpSignMethod, FX_INT32 iSignMethodLen, 
		FX_BYTE* lpSignDateTime,FX_INT32 iSignDateTimeLen, FX_BYTE* lpSignValue, FX_INT32 iSignValueLen, 
		FX_INT32 iOnline)
{
	return g_V2_OES_Verify(lpSignValue, iSignValueLen, lpDocProperty, iDocPropertyLen, 
		lpDigestData, iDigestDataLen, iOnline);
}

//
FX_INT32 CFS_OESInterface_V2::GetSealImage(const CFX_ByteString& bsSealData/*V0/V1 in*/, FX_INT32 iRenderFlag/*V1 in*/, 
		const CFX_ByteString& bsSealId/*V2 in*/, FX_BYTE* lpPictureData, FX_INT32& iPictureDataLen, 
		FX_BYTE* lpPictureType, FX_INT32& iPictureTypeLen, FX_INT32& PictureWidth, FX_INT32& PictureHeight)

{
	FX_LPCBYTE lpSealId = (FX_LPCBYTE)bsSealId;
	FX_INT32 iSealIdLen = bsSealId.GetLength();

	return g_V2_OES_GetSealImage((FX_LPBYTE)lpSealId, iSealIdLen, lpPictureData, &iPictureDataLen, 
		lpPictureType, &iPictureTypeLen, &PictureWidth, &PictureHeight);
}

//
FX_INT32 CFS_OESInterface_V2::GetSignImage(FX_BYTE* lpSignData, FX_INT32 iSignDataLen, FX_BYTE* lpPictureData, FX_INT32& iPictureDataLen, 
	FX_BYTE* lpPictureType, FX_INT32& iPictureTypeLen)
{
	int PictureWidth = 0;
	int PictureHeight = 0;

	if(!g_V2_OES_GetSignImage)
		return OFD_ERRCODE_NoSymbol;

	return g_V2_OES_GetSignImage(lpSignData, iSignDataLen, lpPictureData, &iPictureDataLen, 
		lpPictureType, &iPictureTypeLen, &PictureWidth, &PictureHeight);
}

//
FX_INT32 CFS_OESInterface_V2::GetSignInfo(FX_BYTE* puchSignedValue,int iSignedValueLen,
								FX_BYTE* puchVersion, FX_INT32& iVersionLen,
								FX_BYTE* puchDateTime, FX_INT32& iDateTimeLen,
								FX_BYTE* puchDigestData, FX_INT32& iDigestDataLen,
								FX_BYTE* puchPropertyInfo, FX_INT32& iPropertyInfoLen,
								FX_BYTE* puchSignerCert, FX_INT32& iSignerCertLen,
								FX_BYTE* puchSignature, FX_INT32& iSignatureLen)
{
	if(puchSignedValue == NULL || iSignedValueLen <= 0)
		return -1;

	if(!g_V2_OES_GetSignInfo)
		return OFD_ERRCODE_NoSymbol;

	return g_V2_OES_GetSignInfo(puchSignedValue,iSignedValueLen,
								puchVersion, &iVersionLen,
								puchDateTime, &iDateTimeLen,
								puchDigestData, &iDigestDataLen,
								puchPropertyInfo, &iPropertyInfoLen,
								puchSignerCert, &iSignerCertLen,
								puchSignature, &iSignatureLen);
}

//
FX_INT32 CFS_OESInterface_V2::Login(FX_BYTE* lpPinData, FX_INT32 iPinDataLen)
{
    FX_INT32 ret = g_V2_OES_Login(lpPinData, iPinDataLen);
    LOGD("login V2 res:%d",ret);
	return ret;
}

//
//CFS_OESInterface_V3::CFS_OESInterface_V3()
//{
//
//
//    g_V3_OESV4_GetProviderInfo = NULL;
//     g_V3_OESV4_GetProtocolVersion =  NULL;
//
//     g_V3_OESV4_CloseSession= NULL;
//     g_V3_OESV4_GetSealList= NULL;
//     g_V3_OESV4_GetCertList=  NULL;
//     g_V3_OESV4_GetSealImageById= NULL;
//     g_V3_OESV4_GetSealInfoById= NULL;
//
//     g_V3_OESV4_GetCertById= NULL;
//     g_V3_OESV4_GetSignImage= NULL;
//     g_V3_OESV4_GetSealImage= NULL;
//     g_V3_OESV4_GetSignInfo= NULL;
//     g_V3_OESV4_GetRawSignInfo= NULL;
//
//     g_V3_OESV4_GetDigestMethod= NULL;
//     g_V3_OESV4_Digest= NULL;
//     g_V3_OESV4_Digest_Init= NULL;
//     g_V3_OESV4_Digest_Update= NULL;
//     g_V3_OESV4_Digest_Final= NULL;
//
//     g_V3_OESV4_Sign= NULL;
//     g_V3_OESV4_RawSign= NULL;
//     g_V3_OESV4_Verify= NULL;
//     g_V3_OESV4_GetErrMessage= NULL;
//     g_V3_OESV4_Login= NULL;
//
//     g_V3_OESV4_Logout= NULL;
//     g_V3_OESV4_GetSignMethod= NULL;
//     g_V3_OESV4_GetDateTime= NULL;
//     g_V3_OESV4_GetParamNameList= NULL;
//     g_V3_OESV4_SetExtendParam= NULL;
//
//     g_V3_OESV4_GetTimeStamp= NULL;
//     g_V3_OESV4_VerifyTimeStamp= NULL;
//     g_V3_OESV4_GetTimeStampInfo= NULL;
//
//
//}
//
//
//CFS_OESInterface_V3::~CFS_OESInterface_V3()
//{
//    if(g_V3_OESV4_Logout)
//        g_V3_OESV4_Logout(g_piSession);
//}
//
//
//FX_INT32 CFS_OESInterface_V3::LoadInterface(void* g_sealModule, CFX_ByteString bsPin)
//{
//#if _FX_OS_ == _FX_WIN32_DESKTOP_ || _FX_OS_ == _FX_WIN64_
//    g_OES_GetSealList = (Dll_OES_GetSealList)GetProcAddress((HINSTANCE)g_sealModule, "OES_GetSealList");
//    g_OES_GetProviderInfo = (Dll_OES_GetProviderInfo)GetProcAddress((HINSTANCE)g_sealModule, "OES_GetProviderInfo");
//    g_OES_GetDigestMethod = (Dll_OES_GetDigestMethod)GetProcAddress((HINSTANCE)g_sealModule, "OES_GetDigestMethod");
//    g_OES_Digest = (Dll_OES_Digest)GetProcAddress((HINSTANCE)g_sealModule, "OES_Digest");
//    g_OES_GetErrMessage = (Dll_OES_GetErrMessage)GetProcAddress((HINSTANCE)g_sealModule, "OES_GetErrMessage");
//
//    g_V3_OES_Sign = (Dll_V3_OES_Sign)GetProcAddress((HINSTANCE)g_sealModule, "OES_Sign");
//    g_V3_OES_Verify = (Dll_V3_OES_Verify)GetProcAddress((HINSTANCE)g_sealModule, "OES_Verify");
//
//    g_V3_OES_GetSealImage = (Dll_V3_OES_GetSealImage)GetProcAddress((HINSTANCE)g_sealModule, "OES_GetSealImage");
//    g_V3_OES_GetSignImage = (Dll_V3_OES_GetSignImage)GetProcAddress((HINSTANCE)g_sealModule, "OES_GetSignImage");
//    g_V3_OES_Login = (Dll_V3_OES_Login)GetProcAddress((HINSTANCE)g_sealModule, "OES_Login");
//    g_V3_OES_Logout = (Dll_V3_OES_Logout)GetProcAddress((HINSTANCE)g_sealModule, "OES_Logout");
//    g_V3_OES_GetSignInfo = (DLL_V3_OES_GetSignInfo)GetProcAddress((HINSTANCE)g_sealModule, "OES_GetSignInfo");
//
//#elif _FX_OS_ == _FX_IOS_
//
//    g_V3_OESV4_GetProviderInfo = OESV4_GetProviderInfo;
//    g_V3_OESV4_GetProtocolVersion = OESV4_GetProtocolVersion;
//
//    g_V3_OESV4_CloseSession=OESV4_CloseSession;
//    g_V3_OESV4_GetSealList=OESV4_GetSealList;
//    g_V3_OESV4_GetCertList= OESV4_GetCertList;
//    g_V3_OESV4_GetSealImageById=OESV4_GetSealImageById;
//    g_V3_OESV4_GetSealInfoById=OESV4_GetSealInfoById;
//
//    g_V3_OESV4_GetCertById=OESV4_GetCertById;
//    g_V3_OESV4_GetSignImage=OESV4_GetSignImage;
//    g_V3_OESV4_GetSealImage=OESV4_GetSealImage;
//    g_V3_OESV4_GetSignInfo=OESV4_GetSignInfo;
//    g_V3_OESV4_GetRawSignInfo=OESV4_GetRawSignInfo;
//
//    g_V3_OESV4_GetDigestMethod=OESV4_GetDigestMethod;
//    g_V3_OESV4_Digest=OESV4_Digest;
//    g_V3_OESV4_Digest_Init=OESV4_Digest_Init;
//    g_V3_OESV4_Digest_Update=OESV4_Digest_Update;
//    g_V3_OESV4_Digest_Final=OESV4_Digest_Final;
//
//    g_V3_OESV4_Sign=OESV4_Sign;
//    g_V3_OESV4_RawSign=OESV4_RawSign;
//    g_V3_OESV4_Verify=OESV4_Verify;
//    g_V3_OESV4_GetErrMessage=OESV4_GetErrMessage;
//    g_V3_OESV4_Login=OESV4_Login;
//
//    g_V3_OESV4_Logout=OESV4_Logout;
//    g_V3_OESV4_GetSignMethod=OESV4_GetSignMethod;
//    g_V3_OESV4_GetDateTime=OESV4_GetDateTime;
//    g_V3_OESV4_GetParamNameList=OESV4_GetParamNameList;
//    g_V3_OESV4_SetExtendParam=OESV4_SetExtendParam;
//
//    g_V3_OESV4_GetTimeStamp=OESV4_GetTimeStamp;
//    g_V3_OESV4_VerifyTimeStamp=OESV4_VerifyTimeStamp;
//    g_V3_OESV4_GetTimeStampInfo=OESV4_GetTimeStampInfo;
//
//#else
//    g_OES_GetSealList = (Dll_OES_GetSealList)dlsym(g_sealModule, "OES_GetSealList");
//    g_OES_GetProviderInfo = (Dll_OES_GetProviderInfo)dlsym(g_sealModule, "OES_GetProviderInfo");
//    g_OES_GetDigestMethod = (Dll_OES_GetDigestMethod)dlsym(g_sealModule, "OES_GetDigestMethod");
//    g_OES_Digest = (Dll_OES_Digest)dlsym(g_sealModule, "OES_Digest");
//    g_OES_GetErrMessage = (Dll_OES_GetErrMessage)dlsym(g_sealModule, "OES_GetErrMessage");
//
//    g_V2_OES_Sign = (Dll_V2_OES_Sign)dlsym(g_sealModule, "OES_Sign");
//    g_V2_OES_Verify = (Dll_V2_OES_Verify)dlsym(g_sealModule, "OES_Verify");
//
//    g_V2_OES_GetSealImage = (Dll_V2_OES_GetSealImage)dlsym(g_sealModule, "OES_GetSealImage");
//    g_V2_OES_GetSignImage = (Dll_V2_OES_GetSignImage)dlsym(g_sealModule, "OES_GetSignImage");
//    g_V2_OES_Login = (Dll_V2_OES_Login)dlsym(g_sealModule, "OES_Login");
//    g_V2_OES_Logout = (Dll_V2_OES_Logout)dlsym(g_sealModule, "OES_Logout");
//    g_V2_OES_GetSignInfo = (DLL_V2_OES_GetSignInfo)dlsym(g_sealModule, "OES_GetSignInfo");
//#endif
//
//    if (!g_OES_GetSealList)
//        return OFD_ERRCODE_NoSymbol;
//
//    //��������ǩ��OES g_V3_OES_Login �ӿ�û�У��ʴ���Ҫע�͵���������0
//    //return 0;
//
//    if(!g_V2_OES_Login)
//        return OES_PIN;
//
//    return Login((FX_LPBYTE)(FX_LPCSTR)bsPin, bsPin.GetLength());
//}
//
////
//FX_INT32 CFS_OESInterface_V3::Sign(FX_BYTE* lpSealId, FX_INT32 iSealIdLen, FX_BYTE* lpDocProperty, FX_INT32 iDocPropertyLen,
//        FX_BYTE* lpDigestData, FX_INT32 iDigestDataLen, FX_BYTE* lpSignMethod, FX_INT32 iSignMethodLen,
//        FX_BYTE* lpSignDateTime,FX_INT32 iSignDateTimeLen, FX_BYTE* lpSignValue,FX_INT32& iSignValueLen)
//{
//    return g_V2_OES_Sign(lpSealId, iSealIdLen, lpDocProperty, iDocPropertyLen,
//        lpDigestData, iDigestDataLen, lpSignValue, &iSignValueLen);
//}
//
////
//FX_INT32 CFS_OESInterface_V3::Verify(FX_BYTE* lpSealData, FX_INT32 iSealDataLen, FX_BYTE* lpDocProperty, FX_INT32 iDocPropertyLen,
//        FX_BYTE* lpDigestData, FX_INT32 iDigestDataLen, FX_BYTE* lpSignMethod, FX_INT32 iSignMethodLen,
//        FX_BYTE* lpSignDateTime,FX_INT32 iSignDateTimeLen, FX_BYTE* lpSignValue, FX_INT32 iSignValueLen,
//        FX_INT32 iOnline)
//{
//    return g_V2_OES_Verify(lpSignValue, iSignValueLen, lpDocProperty, iDocPropertyLen,
//        lpDigestData, iDigestDataLen, iOnline);
//}
//
////
//FX_INT32 CFS_OESInterface_V3::GetSealImage(const CFX_ByteString& bsSealData/*V0/V1 in*/, FX_INT32 iRenderFlag/*V1 in*/,
//        const CFX_ByteString& bsSealId/*V3 in*/, FX_BYTE* lpPictureData, FX_INT32& iPictureDataLen,
//        FX_BYTE* lpPictureType, FX_INT32& iPictureTypeLen, FX_INT32& PictureWidth, FX_INT32& PictureHeight)
//
//{
//    FX_LPCBYTE lpSealId = (FX_LPCBYTE)bsSealId;
//    FX_INT32 iSealIdLen = bsSealId.GetLength();
//
//    return g_V2_OES_GetSealImage((FX_LPBYTE)lpSealId, iSealIdLen, lpPictureData, &iPictureDataLen,
//        lpPictureType, &iPictureTypeLen, &PictureWidth, &PictureHeight);
//}
//
////
//FX_INT32 CFS_OESInterface_V3::GetSignImage(FX_BYTE* lpSignData, FX_INT32 iSignDataLen, FX_BYTE* lpPictureData, FX_INT32& iPictureDataLen,
//    FX_BYTE* lpPictureType, FX_INT32& iPictureTypeLen)
//{
//    int PictureWidth = 0;
//    int PictureHeight = 0;
//
//    if(!g_V2_OES_GetSignImage)
//        return OFD_ERRCODE_NoSymbol;
//
//    return g_V2_OES_GetSignImage(lpSignData, iSignDataLen, lpPictureData, &iPictureDataLen,
//        lpPictureType, &iPictureTypeLen, &PictureWidth, &PictureHeight);
//}
//
////
//FX_INT32 CFS_OESInterface_V3::GetSignInfo(FX_BYTE* puchSignedValue,int iSignedValueLen,
//                                FX_BYTE* puchVersion, FX_INT32& iVersionLen,
//                                FX_BYTE* puchDateTime, FX_INT32& iDateTimeLen,
//                                FX_BYTE* puchDigestData, FX_INT32& iDigestDataLen,
//                                FX_BYTE* puchPropertyInfo, FX_INT32& iPropertyInfoLen,
//                                FX_BYTE* puchSignerCert, FX_INT32& iSignerCertLen,
//                                FX_BYTE* puchSignature, FX_INT32& iSignatureLen)
//{
//    if(puchSignedValue == NULL || iSignedValueLen <= 0)
//        return -1;
//
//    if(!g_V2_OES_GetSignInfo)
//        return OFD_ERRCODE_NoSymbol;
//
//    return g_V2_OES_GetSignInfo(puchSignedValue,iSignedValueLen,
//                                puchVersion, &iVersionLen,
//                                puchDateTime, &iDateTimeLen,
//                                puchDigestData, &iDigestDataLen,
//                                puchPropertyInfo, &iPropertyInfoLen,
//                                puchSignerCert, &iSignerCertLen,
//                                puchSignature, &iSignatureLen);
//}
//
////
//FX_INT32 CFS_OESInterface_V3::Login(FX_BYTE* lpPinData, FX_INT32 iPinDataLen)
//{
//    FX_INT32 ret = g_V2_OES_Login(lpPinData, iPinDataLen);
//    LOGD("login V3 res:%d",ret);
//    return ret;
//}


