#include "js_fxlib.h"
#if defined(_FSDK_JAVASCRIPT_) || defined(_FSDK_ALL_)
#include "js_module.h"
#include "js_globaldata.h"
#include "js_runtime.h"

CFJS_LTModule::CFJS_LTModule()
{
	m_nGlobalDataCount = 0;
	m_pGlobalData = NULL;
	m_bInitial = FALSE;
}

CFJS_LTModule::~CFJS_LTModule()
{
//	FSCRT_LOCKOBJECT(lock, &m_lock);

	if (m_pGlobalData)
	{
		delete m_pGlobalData;
		m_pGlobalData = NULL;
	}

	if (/*IsAvailable() && */m_threadID2Runtime.GetCount())
	{
		FX_POSITION pos = m_threadID2Runtime.GetStartPosition();
		while(pos)
		{
			FX_DWORD dwThreadId = 0;
			CFXJS_Runtime* pRuntime = NULL;
			m_threadID2Runtime.GetNextAssoc(pos, (void*&)dwThreadId, (void*&)pRuntime);
			DS_ReleaseRuntime((IDS_Runtime*)pRuntime, dwThreadId);
			delete pRuntime;
		}
		DS_Release();
	}
}

FS_RESULT CFJS_LTModule::GetJSRuntime(IJS_Runtime*& pRuntime)
{
//	FSCRT_LOCKOBJECT(lock, &m_lock);

	FX_DWORD id = DS_GetCurrentThreadID();

	IJS_Runtime* pRunTime = NULL;
	if(!m_threadID2Runtime.Lookup((void*)id, (void*&)pRunTime))
	{
		m_bInitial = TRUE;
		ST_GetJSRuntime(pRuntime);
		m_threadID2Runtime.SetAt((void*)id, pRunTime);
	}
	return FSCRT_ERRCODE_SUCCESS;
}

FS_RESULT CFJS_LTModule::DeleteJSRuntime(IJS_Runtime* pRuntime)
{
// 	CFS_LockObject lock(&m_lockRuntime);
// 	if(pRuntime)
// 		delete (CJS_Runtime*)pRuntime;
	return FSCRT_ERRCODE_SUCCESS;
}

FS_RESULT CFJS_LTModule::ST_GetJSRuntime(IJS_Runtime*& pRuntime)
{
//	FSCRT_Setjmp
	//Modified for multi-thread.
	DS_Initial();
//	pRuntime = new CFXJS_Runtime(FSCRT_GetLTAppHandler());
//	if (!pRuntime) return FSCRT_ERRCODE_ST_OOM;
	return FSCRT_ERRCODE_SUCCESS;
}

CFXJS_GlobalData*	CFJS_LTModule::NewGlobalData()
{
//	FSCRT_LOCKOBJECT(lock, &m_lock);

	if (m_pGlobalData)
	{
		m_nGlobalDataCount++;
		return m_pGlobalData;
	}
	else
	{
		m_nGlobalDataCount = 1;
//		m_pGlobalData = new CFXJS_GlobalData(FSCRT_GetLTAppHandler());
		return m_pGlobalData;
	}
}

void CFJS_LTModule::ReleaseGlobalData()
{
//	FSCRT_LOCKOBJECT(lock, &m_lock);

	m_nGlobalDataCount--;

	if (m_nGlobalDataCount < 1)
	{
		delete m_pGlobalData;
		m_pGlobalData = NULL;
	}
}

FS_RESULT CFJS_LTModule::_Recover() 
{
	return TRUE;
}

void CFJS_LTModule::_Clear() 
{

}

FS_RESULT FJS_GetEnviroment(CFJS_LTModule*& pJSEnv)
{
// 	FSCRT_LOCKOBJECT(lock, &m_lock);
// 
// 	CHECK_FSLTSDKMgr
// 
// 	CFS_LTEnvironment* pEnv = FS_GetLTEnvironment();
// 	FXSYS_assert(pEnv != NULL);
// 
// 	pJSEnv = (CFJS_LTModule*)pEnv->GetModule(FS_LTENV_MODULE_JS);
// 	if (!pJSEnv) return FS_ERRCODE_INVALIDMODULE;
	return FSCRT_ERRCODE_SUCCESS;
}

#endif