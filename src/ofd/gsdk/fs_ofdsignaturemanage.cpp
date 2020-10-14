#include "fxlib.h"
#include "include/fs_ofdsignaturefactory.h"
#include "include/fs_ofdsignaturemanage.h"
#include "../int/ofd_es.h"
#include "../htfoxit_oes/OES_DLL/include/oes.h"
#include <fstream>

using namespace std;

void FX_WriteLog120(CFX_ByteString logName, CFX_ByteString logValue, int val = 0)
{
	fstream logfile;
	if( !logfile.is_open() )
	{
		//  logfile.open("/sdcard/Fonts/FxLog.txt", ios::app | ios::out | ios::binary );
		logfile.open("/storage/sdcard0/Fx_Gsdk_Log120.txt", ios::app | ios::out | ios::binary );

		if( logfile.fail() )    return;
	}
	logfile.seekg(0, ios::end);
	time_t now;
	struct tm* timenow;
	time(&now);
	timenow = localtime(&now);
	char localTime[32] = {0};
	sprintf(localTime, "%02d-%02d %02d:%02d:%02d ", timenow->tm_mon+1, timenow->tm_mday, timenow->tm_hour, timenow->tm_min, timenow->tm_sec);
    LOGD("%02d-%02d %02d:%02d:%02d ", timenow->tm_mon+1, timenow->tm_mday, timenow->tm_hour, timenow->tm_min, timenow->tm_sec);
	char pVal[32] = {0};
	sprintf(pVal, "%d", val);
	CFX_ByteString strLog = localTime + logName + "  " + logValue+ "  " + pVal + "\r\n";
	logfile.write(FX_LPCSTR(strLog), strLog.GetLength());
	logfile.close();
}



//
CFS_OFDSignatureManage::CFS_OFDSignatureManage()
	: g_sealModule(NULL)
	, m_pOESInterface(NULL)
	, m_pOESInterfaceV0(NULL)
	, m_pOESInterfaceV1(NULL)
	, m_pOESInterfaceV2(NULL)
    , m_pOESInterfaceV3(NULL)
{
}

//
CFS_OFDSignatureManage::~CFS_OFDSignatureManage()
{
	FreeSealModule();

	if(m_pOESInterfaceV0)
		delete m_pOESInterfaceV0;
	m_pOESInterfaceV0 = NULL;

	if(m_pOESInterfaceV1)
		delete m_pOESInterfaceV1;
	m_pOESInterfaceV1 = NULL;

	if(m_pOESInterfaceV2)
		delete m_pOESInterfaceV2;
    m_pOESInterfaceV2 = NULL;

    if(m_pOESInterfaceV3)
        delete m_pOESInterfaceV3;
	m_pOESInterfaceV3 = NULL;
}

//
FX_INT32 CFS_OFDSignatureManage::LoadSealModule(CFX_WideString& wsOESFile, CFX_ByteString bsPin)
{
	FX_WriteLog120("LoadSealModule wsOESFile : ", CFX_ByteString::FromUnicode(wsOESFile));
	int res = 0;
#if _FX_OS_ == _FX_WIN32_DESKTOP_ || _FX_OS_ == _FX_WIN64_
	g_sealModule = (void*)::LoadLibraryEx(wsOESFile,  NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
	//g_sealModule = (void*)::LoadLibrary(wsOESFile);
	WriteLog_GSDK("LoadSealModule ", "g_sealModule : ", (long)g_sealModule);
#elseif _FX_OS_ == _FX_IOS_
    g_sealModule = 1;
#else
	CFX_ByteString bsOESFile = wsOESFile.UTF8Encode();
	g_sealModule = dlopen((FX_LPCSTR)bsOESFile, RTLD_LAZY);
    LOGD("oes handle:%ld", long(g_sealModule));
    LOGD("module open error:%s", dlerror());
	FX_WriteLog120("LoadSealModule wsOESFile : ", "g_sealModule : ", (long)g_sealModule);
#endif
	if (!g_sealModule)
		return OFD_ERRCODE_LoadDLL;
    LOGD("eOESVersion  begin");
	//check version
	OESVERRSION eOESVersion = CheckOESVerion();
	LOGD("eOESVersion:%d", eOESVersion);
	FX_WriteLog120("LoadSealModule ", "eOESVersion : ", eOESVersion);
	switch(eOESVersion)
    {
        case OES_VERSION_0:
            if(!m_pOESInterfaceV0)
                m_pOESInterfaceV0 = FX_NEW CFS_OESInterface_V0;
            m_pOESInterface = m_pOESInterfaceV0;
            break;
        case OES_VERSION_1:
            if(!m_pOESInterfaceV1)
                m_pOESInterfaceV1 = FX_NEW CFS_OESInterface_V1;
            m_pOESInterface = m_pOESInterfaceV1;
            break;
        case OES_VERSION_2:
            if(!m_pOESInterfaceV2)
                m_pOESInterfaceV2 = FX_NEW CFS_OESInterface_V2;
            m_pOESInterface = m_pOESInterfaceV2;
            break;
        case OES_VERSION_3:
//            if(!m_pOESInterfaceV3)
//                m_pOESInterfaceV3 = FX_NEW CFS_OESInterface_V3;
//            m_pOESInterface = m_pOESInterfaceV3;
            break;
    }
	res = m_pOESInterface->LoadInterface(g_sealModule, bsPin);
    LOGD("login res:%d", res);
	FX_WriteLog120("LoadSealModule ", "res : ", res);
	return res;
}

//
void CFS_OFDSignatureManage::FreeSealModule()
{
#if _FX_OS_ == _FX_WIN32_DESKTOP_ || _FX_OS_ == _FX_WIN64_
	if (g_sealModule)
		::FreeLibrary((HINSTANCE)g_sealModule);
#else
	if (g_sealModule)
		dlclose(g_sealModule);
#endif
	g_sealModule = NULL;
}

//
CFS_OESInterface* CFS_OFDSignatureManage::GetOESInterface()
{
	return m_pOESInterface;
}

//
CFS_OESInterface* CFS_OFDSignatureManage::GetOESInterface(OFD_SIGNVERSION eOESVersion)
{
	if(g_sealModule == NULL)
		return NULL;

	switch(eOESVersion)
	{
	case OFD_SIGN_PERIOD_FIRST:
		if(!m_pOESInterfaceV0)
			m_pOESInterfaceV0 = FX_NEW CFS_OESInterface_V0;
		return m_pOESInterfaceV0;
		break;
		//case OES_VERSION_1:
		//if(!m_pOESInterfaceV1)
		//	m_pOESInterfaceV0 = FX_NEW CFS_OESInterface_V1;
		//	return m_pOESInterfaceV1;
		//	break;
	case OFD_SIGN_PERIOD_SECOND:
		if(!m_pOESInterfaceV2)
			m_pOESInterfaceV2 = FX_NEW CFS_OESInterface_V2;
		return m_pOESInterfaceV2;
		break;	
	}

	return NULL;
}

#define _FOXITREADER_OES_MODE_
//
OESVERRSION CFS_OFDSignatureManage::CheckOESVerion()
{
#ifdef _FOXITREADER_OES_MODE_	//�����ж�oes �汾��Ϊ�Ķ����жϷ�ʽ

#if _FX_OS_ == _FX_WIN32_DESKTOP_ || _FX_OS_ == _FX_WIN64_
	/*GetSignImage*/
	//V2 �ӿ�
	Dll_V1_OES_GetSignImage OES_GetSignImage2 = (Dll_V1_OES_GetSignImage)GetProcAddress((HINSTANCE)g_sealModule, "OES_GetSignImage");
	WriteLog_GSDK("CheckOESVerion ", "OES_GetSignImage2 : ", (long)OES_GetSignImage2);

	//V3 �ӿ�
	Dll_V2_OES_GetSignImage OES_GetSignImage3 = (Dll_V2_OES_GetSignImage)GetProcAddress((HINSTANCE)g_sealModule, "OES_GetSignImage");
	WriteLog_GSDK("CheckOESVerion ", "OES_GetSignImage3 : ", (long)OES_GetSignImage3);
	
	/*GetSealImage*/
	//V1 �ӿ�
	Dll_V1_OES_GetSealImage OES_GetSealImage1 = (Dll_V0_OES_GetSealImage)GetProcAddress((HINSTANCE)g_sealModule, "OES_GetSealImage");
	WriteLog_GSDK("CheckOESVerion ", "OES_GetSealImage : ", (long)OES_GetSealImage1);
	//V2 �ӿ�
	Dll_V1_OES_GetSealImage OES_GetSealImage2 = (Dll_V1_OES_GetSealImage)GetProcAddress((HINSTANCE)g_sealModule, "OES_GetSealImage");
	WriteLog_GSDK("CheckOESVerion ", "OES_GetSealImage : ", (long)OES_GetSealImage2);
#elseif _FX_OS_ == _FX_IOS_
    return OES_VERSION_3;
#else
	Dll_V1_OES_GetSignImage OES_GetSignImage2 = (Dll_V1_OES_GetSignImage)dlsym(g_sealModule, "OES_GetSignImage");
	Dll_V2_OES_GetSignImage OES_GetSignImage3 = (Dll_V2_OES_GetSignImage)dlsym(g_sealModule, "OES_GetSignImage");
	Dll_V1_OES_GetSealImage OES_GetSealImage1 = (Dll_V1_OES_GetSealImage)dlsym(g_sealModule, "OES_GetSealImage");
	Dll_V1_OES_GetSealImage OES_GetSealImage2 = (Dll_V1_OES_GetSealImage)dlsym(g_sealModule, "OES_GetSealImage");
#endif

	if(OES_GetSignImage2 != NULL && OES_GetSealImage2 != NULL)
		return OES_VERSION_1;

	if(OES_GetSignImage3 != NULL && OES_GetSealImage2 == NULL)
		return OES_VERSION_2;

	return OES_VERSION_0;

#else	//_FOXITREADER_OES_MODE_	�������ж�oes �汾��Ϊsdk�жϷ�ʽ
#if _FX_OS_ == _FX_WIN32_DESKTOP_ || _FX_OS_ == _FX_WIN64_
	//V3 �ӿ�
	Dll_V2_OES_Login OES_Login_Addr = (Dll_V2_OES_Login)GetProcAddress((HINSTANCE)g_sealModule, "OES_Login");
	WriteLog_GSDK("CheckOESVerion ", "OES_Login_Addr : ", (long)OES_Login_Addr);
	//V2 �ӿ�
	Dll_V1_OES_Session_Open OES_Session_Open_Addr = (Dll_V1_OES_Session_Open)GetProcAddress((HINSTANCE)g_sealModule, "OES_Session_Open");
	WriteLog_GSDK("CheckOESVerion ", "OES_Session_Open_Addr : ", (long)OES_Session_Open_Addr);
#else
	Dll_V2_OES_Login OES_Login_Addr = (Dll_V2_OES_Login)dlsym(g_sealModule, "OES_Login");
	Dll_V1_OES_Session_Open OES_Session_Open_Addr = (Dll_V1_OES_Session_Open)dlsym(g_sealModule, "OES_Session_Open");
#endif
	//return OES_VERSION_2;
	if (OES_Login_Addr)
		return OES_VERSION_2;
	if (OES_Session_Open_Addr)		//�ɴ˽ӿ��Ƿ�������ж�Ϊv1�汾������׼ȷ�ģ�v0�汾Ҳͬ�����
		return OES_VERSION_1;

	//��������ǩ�£�OES_Login_Addr�� OES_Session_Open_Addr ��û�У�������Ҫ return OES_VERSION_2;
	WriteLog_GSDK("CheckOESVerion ", "OES_VERSION_0 ...");

	return OES_VERSION_0;
#endif	//_FOXITREADER_OES_MODE_
}
