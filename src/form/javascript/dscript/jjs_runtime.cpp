#include "js_fxlib.h"
#if defined(_FSDK_ALL_) || defined(_FSDK_JAVASCRIPT_)

// #include "Document.h"
// #include "app.h"
// //#include "Bookmark.h"
// //#include "Collab.h"
// #include "color.h"
// #include "Consts.h"
// //#include "Cursor.h"
// //#include "Data.h"
// //#include "DataBase.h"
// #include "event.h"
// //#include "FDF.h"
// #include "Field.h"
// //#include "FullScreen.h"
// #include "Icon.h"
// //#include "identity.h"
// //#include "OCG.h"
// #include "PublicMethods.h"
// #include "report.h"
// #include "util.h"
// #include "JS_GlobalData.h"
// #include "global.h"
// #include "console.h"
// //#include "Annotation.h"
// //#include "Link.h"
// //#include "Media.h"

#include "js_globaldata.h"
#include "consts.h"
#include "app.h"
#include "color.h"
#include "console.h"
#include "document.h"
#include "event.h"
#include "field.h"
#include "global.h"
#include "icon.h"
#include "util.h"
#include "publicmethods.h"
#include "identity.h"
#include "collab.h"
#include "media.h"

#if _FX_OS_ == _FX_LINUX_DESKTOP_ || _FX_OS_ == _FX_MACOSX_ || _FX_OS_ == _FX_IOS_ || _FX_OS_ == _FX_ANDROID_
#include <unistd.h>
#include <pthread.h>

// Key identifying the thread-specific data
static pthread_key_t ds_key;

/* "Once" variable ensuring that the key for threads will be allocated
 * exactly once.
 */
static pthread_once_t ds_alloc_key_once = PTHREAD_ONCE_INIT;

static void ds_alloc_destroy_gc(void * param)
{
	if(param)
		((CFXJS_RuntimeFactory*)param)->DeleteJSRuntime();
}

static void ds_alloc_key()
{
	pthread_key_create(&ds_key, ds_alloc_destroy_gc);
}


#endif

CFXJS_RuntimeFactory::CFXJS_RuntimeFactory():m_nRef(0),m_pGlobalData(NULL),m_nGlobalDataCount(0) 
{
#if _FX_OS_ == _FX_LINUX_DESKTOP_ || _FX_OS_ == _FX_MACOSX_ || _FX_OS_ == _FX_IOS_ || _FX_OS_ == _FX_ANDROID_
	pthread_once(&ds_alloc_key_once, ds_alloc_key);
#endif
}

CFXJS_RuntimeFactory::~CFXJS_RuntimeFactory()
{
	if (m_pGlobalData)
	{
		delete m_pGlobalData;
		m_pGlobalData = NULL;
	}
	DeleteJSRuntime();
}

IJS_Runtime* CFXJS_RuntimeFactory::NewJSRuntime(CFSCRT_LTAppHandler* pApp)
{
	FSCRT_LOCKOBJECT(lock, &m_lock)
//Dscript is multi-thread implemented.
#if _FX_OS_ == _FX_LINUX_DESKTOP_ || _FX_OS_ == _FX_MACOSX_ || _FX_OS_ == _FX_IOS_ || _FX_OS_ == _FX_ANDROID_
	pthread_setspecific(ds_key, (void *) this);	
#endif
	DS_Initial();	
	return new CFXJS_Runtime(pApp);
}

void CFXJS_RuntimeFactory::Release()
{	
	if(m_threadID2Runtime.GetCount())
	{
		//Alway release the runtime of current thread, do not release it cross threads,the behavior is undefined.
		DeleteJSRuntime();
		DS_Reset();

		//to do.Should be implemented as atom manipulation.
		FX_POSITION pos = m_threadID2Runtime.GetStartPosition();
		while(pos)
		{
			FX_DWORD* dwThreadId = NULL;
			CFXJS_Runtime* pRuntime = NULL;
			m_threadID2Runtime.GetNextAssoc(pos, (void*&)dwThreadId, (void*&)pRuntime);
			delete pRuntime;
		}
		m_threadID2Runtime.RemoveAll();
		delete this;
	}
	else
		delete this;
}

void CFXJS_RuntimeFactory::DeleteJSRuntime()
{
	FSCRT_LOCKOBJECT(lock, &m_lock)
	FX_DWORD id = DS_GetCurrentThreadID();

	CFXJS_Runtime* pRuntime = NULL;
	if(!m_threadID2Runtime.Lookup((void*)id, (void*&)pRuntime))
		return;
	DS_ReleaseRuntime((IDS_Runtime*)pRuntime);
	DS_Release();
	delete pRuntime;
	m_threadID2Runtime.RemoveKey((void*)id);
}


IJS_Runtime* CFXJS_RuntimeFactory::GetJSRuntime(CFSCRT_LTAppHandler* pApp, FX_BOOL bNew)
{
	FSCRT_LOCKOBJECT(lock, &m_lock)

	FX_DWORD id = DS_GetCurrentThreadID();
	 
	IJS_Runtime* pRunTime = NULL;
	if(!m_threadID2Runtime.Lookup((void*)id, (void*&)pRunTime) && bNew)
	{
		//64 is the internal limit for the threads count of dscript.
		if(m_threadID2Runtime.GetCount()>=64) return NULL;

		pRunTime = NewJSRuntime(pApp);
		m_threadID2Runtime.SetAt((void*)id, pRunTime);
	}
	return pRunTime;
}

FX_BOOL CFXJS_RuntimeFactory::IsJSInitiated()
{
	return GetJSRuntime(NULL, FALSE) != NULL;
}

CFXJS_GlobalData*	CFXJS_RuntimeFactory::NewGlobalData(CFSCRT_LTAppHandler* pApp)
{
	FSCRT_LOCKOBJECT(lock, &m_lock)

	if (m_pGlobalData)
	{
		m_nGlobalDataCount++;
		return m_pGlobalData;
	}
	else
	{
		m_nGlobalDataCount = 1;
		m_pGlobalData = new CFXJS_GlobalData(pApp);
		return m_pGlobalData;
	}
}

void CFXJS_RuntimeFactory::ReleaseGlobalData()
{
	FSCRT_LOCKOBJECT(lock, &m_lock)

	m_nGlobalDataCount--;
	
	if (m_nGlobalDataCount <= 0)
	{
 		delete m_pGlobalData;
 		m_pGlobalData = NULL;
	}
}

/* ------------------------------ CJS_Runtime ------------------------------ */

CFXJS_Runtime::CFXJS_Runtime(CFSCRT_LTAppHandler * pApp) : 
	m_pApp(pApp),
	m_pDocument(NULL),
	m_pFieldEventPath(NULL),
	m_bBlocking(FALSE),
	m_pGlobalData(NULL)
{
	DS_InitialRuntime(this);
	InitJSObjects();
}

CFXJS_Runtime::~CFXJS_Runtime()
{
	for (int i=0, sz=m_ContextArray.GetSize(); i<sz; i++)
		delete m_ContextArray.GetAt(i);

	m_ContextArray.RemoveAll();

	//DS_ReleaseRuntime(this, dwThreadID);

	RemoveEventsInLoop(m_pFieldEventPath);

	m_pApp = NULL;
	m_pDocument = NULL;
	m_pFieldEventPath = NULL;
}

void CFXJS_Runtime::SetGlobalData(void* pData)
{
	m_pGlobalData = pData;
}

void* CFXJS_Runtime::GetGlobalData()
{
	return m_pGlobalData;
}

bool CFXJS_Runtime::IsThisAsGlobal()
{
	return true;
}

FX_BOOL CFXJS_Runtime::InitJSObjects()
{
	//0 - 8
	if (CFXJS_Border::Init(this, DS_STATIC) < 0) return FALSE;
	if (CFXJS_Display::Init(this, DS_STATIC) < 0) return FALSE;
	if (CFXJS_Font::Init(this, DS_STATIC) < 0) return FALSE;
	if (CFXJS_Highlight::Init(this, DS_STATIC) < 0) return FALSE;
	if (CFXJS_Position::Init(this, DS_STATIC) < 0) return FALSE;
	if (CFXJS_ScaleHow::Init(this, DS_STATIC) < 0) return FALSE;
	if (CFXJS_ScaleWhen::Init(this, DS_STATIC) < 0) return FALSE;
	if (CFXJS_Style::Init(this, DS_STATIC) < 0) return FALSE;	
	if (CFXJS_Zoomtype::Init(this, DS_STATIC) < 0) return FALSE;	

	//9 - 20
//	if (CJS_ADBC::Init(this, DS_STATIC) < 0) return FALSE;
//	if (CJS_Annotation::Init(this, DS_DYNAMIC) < 0) return FALSE;
	if (CFXJS_App::Init(this, DS_STATIC) < 0) return FALSE;
//	if (CJS_app_media::Init(this, DS_DYNAMIC) < 0) return FALSE;
//	if (CJS_Bookmark::Init(this, DS_DYNAMIC) < 0) return FALSE;
	if (CFXJS_Collab::Init(this, DS_STATIC) < 0) return FALSE;
	if (CFXJS_Color::Init(this, DS_STATIC) < 0) return FALSE;   
//	if (CJS_Column::Init(this, DS_DYNAMIC) < 0) return FALSE;
//	if (CJS_ColumnInfo::Init(this, DS_DYNAMIC) < 0) return FALSE;
//	if (CJS_Connection::Init(this, DS_DYNAMIC) < 0) return FALSE;
	if (CFXJS_Console::Init(this, DS_STATIC) < 0) return FALSE;
//	if (CJS_cursor::Init(this, DS_STATIC) < 0) return FALSE;

	//21 - 27
//	if (CJS_Data::Init(this, DS_DYNAMIC) < 0) return FALSE;
//	if (CJS_DataSourceInfo::Init(this, DS_DYNAMIC) < 0) return FALSE;
	if (CFXJS_Document::Init(this, DS_DYNAMIC) < 0) return FALSE;  
	if (CFXJS_doc_media::Init(this, DS_DYNAMIC) < 0) return FALSE;
	if (CFXJS_Event::Init(this, DS_STATIC) < 0) return FALSE;		
//	if (CJS_FDF::Init(this, DS_STATIC) < 0) return FALSE;
	if (CFXJS_Field::Init(this, DS_DYNAMIC) < 0) return FALSE;    

	//28 - 44
//	if (CJS_FullScreen::Init(this, DS_DYNAMIC) < 0) return FALSE;
	if (CFXJS_Global::Init(this, DS_STATIC) < 0) return FALSE;		
	if (CFXJS_Icon::Init(this, DS_DYNAMIC) < 0) return FALSE;
	if (CFXJS_identity::Init(this, DS_STATIC) < 0) return FALSE;
//	if (CJS_Link::Init(this, DS_DYNAMIC) < 0) return FALSE;
//	if (CJS_MediaData::Init(this, DS_DYNAMIC) < 0) return FALSE;
//	if (CJS_MediaPlayer::Init(this, DS_DYNAMIC) < 0) return FALSE;	
//	if (CJS_MediaSettings::Init(this, DS_DYNAMIC) < 0) return FALSE;
//	if (CJS_OCG::Init(this, DS_DYNAMIC) < 0) return FALSE;
//	if (CJS_PlayerInfoList::Init(this, DS_DYNAMIC) < 0) return FALSE;
//	if (CJS_Rendition::Init(this, DS_DYNAMIC) < 0) return FALSE;
//	if (CJS_Row::Init(this, DS_DYNAMIC) < 0) return FALSE;
//	if (CJS_ScreenAnnot::Init(this, DS_DYNAMIC) < 0) return FALSE;
//	if (CJS_Statement::Init(this, DS_DYNAMIC) < 0) return FALSE;
//	if (CJS_TableInfo::Init(this, DS_DYNAMIC) < 0) return FALSE;
	if (CFXJS_Util::Init(this, DS_STATIC) < 0) return FALSE;

	if (CFXJS_PublicMethods::Init(this) < 0) return FALSE;
	if (CFXJS_GlobalConsts::Init(this) < 0) return FALSE;
	if (CFXJS_GlobalArrays::Init(this) < 0) return FALSE;

	if (CFXJS_TimerObj::Init(this, DS_DYNAMIC) < 0) return FALSE;
	if (CFXJS_PrintParamsObj::Init(this, DS_DYNAMIC) <0) return FALSE;

	return TRUE;
}

IJS_Context* CFXJS_Runtime::NewContext()
{
	CFXJS_Context * p = new CFXJS_Context(this);
	m_ContextArray.Add(p);
	return p;
}

void CFXJS_Runtime::ReleaseContext(IJS_Context * pContext)
{
	CFXJS_Context* pJSContext = (CFXJS_Context*)pContext;

	for (int i=0, sz=m_ContextArray.GetSize(); i<sz; i++)
	{
		if (pJSContext == m_ContextArray.GetAt(i))
		{
			delete pJSContext;
			m_ContextArray.RemoveAt(i);
			break;
		}
	}
}

void CFXJS_Runtime::SetReaderDocument(FSPDFDoc* pReaderDoc)
{
	if (m_pDocument != pReaderDoc)
	{
		m_pDocument = pReaderDoc;

		if (pReaderDoc)
		{
			DFxObj* pThis = NULL;
			if(IsThisAsGlobal())
				pThis = DS_GetGlobalObj(this, true);
			else
				pThis = DS_GetThisObj(this);

			if (pThis)
			{
				if (DS_GetObjDefnID(pThis) == DS_GetObjDefnID(this, L"Document"))
				{
					if (CFXJS_Document* pJSDocument = (CFXJS_Document*)DS_GetPrivate(pThis))
					{
						if (JDocument * pDocument = (JDocument*)pJSDocument->GetEmbedObject())
							pDocument->AttachDoc(pReaderDoc);
					}
				}
			}
			DS_SetThisObj(this, DS_GetObjDefnID(this, L"Document"));
		}
		else
		{
			DS_SetThisObj(this, DS_GetObjDefnID(this, L"app"));
		}
	}
}

FX_BOOL	CFXJS_Runtime::AddEventToLoop(const CFX_WideString& sTargetName, JS_EVENT_T eEventType)
{
	if (m_pFieldEventPath == NULL)
	{
		m_pFieldEventPath = new CJS_FieldEvent;
		m_pFieldEventPath->sTargetName = sTargetName;
		m_pFieldEventPath->eEventType = eEventType;
		m_pFieldEventPath->pNext = NULL;

		return TRUE;
	}

	//to search
	CJS_FieldEvent* p = m_pFieldEventPath;
	CJS_FieldEvent* pLast = m_pFieldEventPath;
	while (p)
	{
		if (p->eEventType == eEventType && p->sTargetName == sTargetName)
			return FALSE;

		pLast = p;
		p = p->pNext;
	}

	//to add
	CJS_FieldEvent* pNew = new CJS_FieldEvent;
	pNew->sTargetName = sTargetName;
	pNew->eEventType = eEventType;
	pNew->pNext = NULL;

	pLast->pNext = pNew;

	return TRUE;
}

void CFXJS_Runtime::RemoveEventInLoop(const CFX_WideString& sTargetName, JS_EVENT_T eEventType)
{
	FX_BOOL bFind = FALSE;

	CJS_FieldEvent* p = m_pFieldEventPath;
	CJS_FieldEvent* pLast = NULL;
	while (p)
	{
		if (p->eEventType == eEventType && p->sTargetName == sTargetName)
		{
			bFind = TRUE;
			break;
		}

		pLast = p;
		p = p->pNext;
	}

	if (bFind)
	{
		RemoveEventsInLoop(p);

		if (p == m_pFieldEventPath)
			m_pFieldEventPath = NULL;

		if (pLast)
			pLast->pNext = NULL;
	}
}

void CFXJS_Runtime::RemoveEventsInLoop(CJS_FieldEvent* pStart)
{
	CJS_FieldEvent* p = pStart;

	while (p)
	{
		CJS_FieldEvent* pOld = p;
		p = pOld->pNext;

		delete pOld;
	}
}

CFX_WideString FXChangeObjName(const CFX_WideString& str)
{
	CFX_WideString sRet = str;
	sRet.Replace((FX_LPCWSTR)L"_", (FX_LPCWSTR)L".");
	return sRet;
}

void CFXJS_Runtime::GetObjectNames(CFX_WideStringArray& array)
{
	array.RemoveAll();

	array.Add(CFXJS_Border::m_pClassName);
	array.Add(CFXJS_Display::m_pClassName);
	array.Add(CFXJS_Font::m_pClassName);
	array.Add(CFXJS_Highlight::m_pClassName);
	array.Add(CFXJS_Position::m_pClassName);
	array.Add(CFXJS_ScaleHow::m_pClassName);
	array.Add(CFXJS_ScaleWhen::m_pClassName);
	array.Add(CFXJS_Style::m_pClassName);
	array.Add(CFXJS_Zoomtype::m_pClassName);

//	array.Add(CJS_ADBC::m_pClassName);
//	array.Add(CJS_Annotation::m_pClassName);
	array.Add(CFXJS_App::m_pClassName);
//	array.Add(ChangeObjName(CJS_app_media::m_pClassName));
//	array.Add(CJS_Bookmark::m_pClassName);
	array.Add(CFXJS_Collab::m_pClassName);
/*	array.Add(CJS_Color::m_pClassName);	*/
//	array.Add(CJS_Column::m_pClassName);
//	array.Add(CJS_ColumnInfo::m_pClassName);
//	array.Add(CJS_Connection::m_pClassName);
//	array.Add(CJS_Console::m_pClassName);
//	array.Add(CJS_cursor::m_pClassName);
//	array.Add(CJS_Data::m_pClassName);
//	array.Add(CJS_DataSourceInfo::m_pClassName);
	array.Add((FX_LPCWSTR)"this"); //CJS_Document::m_pClassName);
	array.Add(FXChangeObjName(CFXJS_doc_media::m_pClassName));
	array.Add(CFXJS_Event::m_pClassName);	
//	array.Add(CJS_FDF::m_pClassName);
/*	array.Add(CJS_Field::m_pClassName);	*/
//	array.Add(CJS_FullScreen::m_pClassName);
	array.Add(CFXJS_Global::m_pClassName);	
	array.Add(CFXJS_identity::m_pClassName);
//	array.Add(CJS_Link::m_pClassName);
//	array.Add(CJS_MediaData::m_pClassName);
//	array.Add(CJS_MediaPlayer::m_pClassName);
//	array.Add(CJS_MediaSettings::m_pClassName);
//	array.Add(CJS_OCG::m_pClassName);
//	array.Add(CJS_PlayerInfoList::m_pClassName);
//	array.Add(CJS_Rendition::m_pClassName);
//	array.Add(CJS_Row::m_pClassName);
//	array.Add(CJS_ScreenAnnot::m_pClassName);
//	array.Add(CJS_Statement::m_pClassName);
//	array.Add(CJS_TableInfo::m_pClassName);
	array.Add(CFXJS_Util::m_pClassName);
}

void CFXJS_Runtime::GetObjectConsts(const CFX_WideString& sObjName, CFX_WideStringArray& array)
{
	JSConstSpec* pConsts = NULL;
	int nSize = 0;

	if (sObjName == CFXJS_Border::m_pClassName)
		CFXJS_Border::GetConsts(pConsts, nSize);
	else if (sObjName == CFXJS_Display::m_pClassName)
		CFXJS_Display::GetConsts(pConsts, nSize);
	else if (sObjName == CFXJS_Font::m_pClassName)
		CFXJS_Font::GetConsts(pConsts, nSize);
	else if (sObjName == CFXJS_Highlight::m_pClassName)
		CFXJS_Highlight::GetConsts(pConsts, nSize);
	else if (sObjName == CFXJS_Position::m_pClassName)
		CFXJS_Position::GetConsts(pConsts, nSize);
	else if (sObjName == CFXJS_ScaleHow::m_pClassName)
		CFXJS_ScaleHow::GetConsts(pConsts, nSize);
	else if (sObjName == CFXJS_ScaleWhen::m_pClassName)
		CFXJS_ScaleWhen::GetConsts(pConsts, nSize);
	else if (sObjName == CFXJS_Style::m_pClassName)
		CFXJS_Style::GetConsts(pConsts, nSize);
	else if (sObjName == CFXJS_Zoomtype::m_pClassName)
		CFXJS_Zoomtype::GetConsts(pConsts, nSize);

// 	else if (sObjName == CJS_ADBC::m_pClassName)
// 		CJS_ADBC::GetConsts(pConsts, nSize);
// 	else if (sObjName == CJS_Annotation::m_pClassName)
// 		CJS_Annotation::GetConsts(pConsts, nSize);
	else if (sObjName == CFXJS_App::m_pClassName)
		CFXJS_App::GetConsts(pConsts, nSize);
// 	else if (sObjName == ChangeObjName(CJS_app_media::m_pClassName))
// 		CJS_app_media::GetConsts(pConsts, nSize);
// 	else if (sObjName == CJS_Bookmark::m_pClassName)
// 		CJS_Bookmark::GetConsts(pConsts, nSize);
	else if (sObjName == CFXJS_Collab::m_pClassName)
		CFXJS_Collab::GetConsts(pConsts, nSize);
	else if (sObjName == CFXJS_Color::m_pClassName)
		CFXJS_Color::GetConsts(pConsts, nSize);	
// 	else if (sObjName == CJS_Column::m_pClassName)
// 		CJS_Column::GetConsts(pConsts, nSize);
// 	else if (sObjName == CJS_ColumnInfo::m_pClassName)
// 		CJS_ColumnInfo::GetConsts(pConsts, nSize);
// 	else if (sObjName == CJS_Connection::m_pClassName)
// 		CJS_Connection::GetConsts(pConsts, nSize);
// 	else if (sObjName == CJS_Console::m_pClassName)
// 		CJS_Console::GetConsts(pConsts, nSize);
// 	else if (sObjName == CJS_cursor::m_pClassName)
// 		CJS_cursor::GetConsts(pConsts, nSize);
// 	else if (sObjName == CJS_Data::m_pClassName)
// 		CJS_Data::GetConsts(pConsts, nSize);
// 	else if (sObjName == CJS_DataSourceInfo::m_pClassName)
// 		CJS_DataSourceInfo::GetConsts(pConsts, nSize);
	else if (sObjName == L"this") 
	{
		if (GetReaderDocument())
			CFXJS_Document::GetConsts(pConsts, nSize);
		else 
			CFXJS_App::GetConsts(pConsts, nSize);
	}
	else if (sObjName == FXChangeObjName(CFXJS_doc_media::m_pClassName))
		CFXJS_doc_media::GetConsts(pConsts, nSize);
	if (sObjName == CFXJS_Event::m_pClassName)
		CFXJS_Event::GetConsts(pConsts, nSize);	
// 	else if (sObjName == CJS_FDF::m_pClassName)
// 		CJS_FDF::GetConsts(pConsts, nSize);
	else if (sObjName == CFXJS_Field::m_pClassName)
		CFXJS_Field::GetConsts(pConsts, nSize);	
// 	else if (sObjName == CJS_FullScreen::m_pClassName)
// 		CJS_FullScreen::GetConsts(pConsts, nSize);
	else if (sObjName == CFXJS_Global::m_pClassName)
		CFXJS_Global::GetConsts(pConsts, nSize);	
	else if (sObjName == CFXJS_identity::m_pClassName)
		CFXJS_identity::GetConsts(pConsts, nSize);
// 	else if (sObjName == CJS_Link::m_pClassName)
// 		CJS_Link::GetConsts(pConsts, nSize);
// 	else if (sObjName == CJS_MediaData::m_pClassName)
// 		CJS_MediaData::GetConsts(pConsts, nSize);
// 	else if (sObjName == CJS_MediaPlayer::m_pClassName)
// 		CJS_MediaPlayer::GetConsts(pConsts, nSize);
// 	else if (sObjName == CJS_MediaSettings::m_pClassName)
// 		CJS_MediaSettings::GetConsts(pConsts, nSize);
// 	else if (sObjName == CJS_OCG::m_pClassName)
// 		CJS_OCG::GetConsts(pConsts, nSize);
// 	else if (sObjName == CJS_PlayerInfoList::m_pClassName)
// 		CJS_PlayerInfoList::GetConsts(pConsts, nSize);
// 	else if (sObjName == CJS_Rendition::m_pClassName)
// 		CJS_Rendition::GetConsts(pConsts, nSize);
// 	else if (sObjName == CJS_Row::m_pClassName)
// 		CJS_Row::GetConsts(pConsts, nSize);
// 	else if (sObjName == CJS_ScreenAnnot::m_pClassName)
// 		CJS_ScreenAnnot::GetConsts(pConsts, nSize);
// 	else if (sObjName == CJS_Statement::m_pClassName)
// 		CJS_Statement::GetConsts(pConsts, nSize);
// 	else if (sObjName == CJS_TableInfo::m_pClassName)
// 		CJS_TableInfo::GetConsts(pConsts, nSize);
	else if (sObjName == CFXJS_Util::m_pClassName)
		CFXJS_Util::GetConsts(pConsts, nSize);

	for (int i=0; i<nSize; i++)
		array.Add(pConsts[i].pName);
}

void CFXJS_Runtime::GetObjectProps(const CFX_WideString& sObjName, CFX_WideStringArray& array)
{
	JSPropertySpec* pProperties = NULL;
	int nSize = 0;

// 	if (sObjName == CJS_ADBC::m_pClassName)
// 		CJS_ADBC::GetProperties(pProperties, nSize);
// 	else if (sObjName == CJS_Annotation::m_pClassName)
// 		CJS_Annotation::GetProperties(pProperties, nSize);
 	if (sObjName == CFXJS_App::m_pClassName)
		CFXJS_App::GetProperties(pProperties, nSize);
// 	else if (sObjName == ChangeObjName(CJS_app_media::m_pClassName))
// 		CJS_app_media::GetProperties(pProperties, nSize);
// 	else if (sObjName == CJS_Bookmark::m_pClassName)
// 		CJS_Bookmark::GetProperties(pProperties, nSize);
	else if (sObjName == CFXJS_Collab::m_pClassName)
		CFXJS_Collab::GetProperties(pProperties, nSize);
	else if (sObjName == CFXJS_Color::m_pClassName)
		CFXJS_Color::GetProperties(pProperties, nSize); 
// 	else if (sObjName == CJS_Column::m_pClassName)
// 		CJS_Column::GetProperties(pProperties, nSize);
// 	else if (sObjName == CJS_ColumnInfo::m_pClassName)
// 		CJS_ColumnInfo::GetProperties(pProperties, nSize);
// 	else if (sObjName == CJS_Connection::m_pClassName)
// 		CJS_Connection::GetProperties(pProperties, nSize);
// 	else if (sObjName == CJS_Console::m_pClassName)
// 		CJS_Console::GetProperties(pProperties, nSize);
// 	else if (sObjName == CJS_cursor::m_pClassName)
// 		CJS_cursor::GetProperties(pProperties, nSize);
// 	else if (sObjName == CJS_Data::m_pClassName)
// 		CJS_Data::GetProperties(pProperties, nSize);
// 	else if (sObjName == CJS_DataSourceInfo::m_pClassName)
// 		CJS_DataSourceInfo::GetProperties(pProperties, nSize);
	else if (sObjName == L"this")
	{
		if (GetReaderDocument())
			CFXJS_Document::GetProperties(pProperties, nSize);
		else	
			CFXJS_App::GetProperties(pProperties, nSize);
	}
	else if (sObjName == FXChangeObjName(CFXJS_doc_media::m_pClassName))
		CFXJS_doc_media::GetProperties(pProperties, nSize);
	else if (sObjName == CFXJS_Event::m_pClassName)
		CFXJS_Event::GetProperties(pProperties, nSize);	
// 	else if (sObjName == CJS_FDF::m_pClassName)
// 		CJS_FDF::GetProperties(pProperties, nSize);
	else if (sObjName == CFXJS_Field::m_pClassName)
		CFXJS_Field::GetProperties(pProperties, nSize);	
// 	else if (sObjName == CJS_FullScreen::m_pClassName)
// 		CJS_FullScreen::GetProperties(pProperties, nSize);
	else if (sObjName == CFXJS_Global::m_pClassName)
		CFXJS_Global::GetProperties(pProperties, nSize);	
	else if (sObjName == CFXJS_identity::m_pClassName)
		CFXJS_identity::GetProperties(pProperties, nSize);
// 	else if (sObjName == CJS_Link::m_pClassName)
// 		CJS_Link::GetProperties(pProperties, nSize);
// 	else if (sObjName == CJS_MediaData::m_pClassName)
// 		CJS_MediaData::GetProperties(pProperties, nSize);
// 	else if (sObjName == CJS_MediaPlayer::m_pClassName)
// 		CJS_MediaPlayer::GetProperties(pProperties, nSize);
// 	else if (sObjName == CJS_MediaSettings::m_pClassName)
// 		CJS_MediaSettings::GetProperties(pProperties, nSize);
// 	else if (sObjName == CJS_OCG::m_pClassName)
// 		CJS_OCG::GetProperties(pProperties, nSize);
// 	else if (sObjName == CJS_PlayerInfoList::m_pClassName)
// 		CJS_PlayerInfoList::GetProperties(pProperties, nSize);
// 	else if (sObjName == CJS_Rendition::m_pClassName)
// 		CJS_Rendition::GetProperties(pProperties, nSize);
// 	else if (sObjName == CJS_Row::m_pClassName)
// 		CJS_Row::GetProperties(pProperties, nSize);
// 	else if (sObjName == CJS_ScreenAnnot::m_pClassName)
// 		CJS_ScreenAnnot::GetProperties(pProperties, nSize);
// 	else if (sObjName == CJS_Statement::m_pClassName)
// 		CJS_Statement::GetProperties(pProperties, nSize);
// 	else if (sObjName == CJS_TableInfo::m_pClassName)
// 		CJS_TableInfo::GetProperties(pProperties, nSize);
	else if (sObjName == CFXJS_Util::m_pClassName)
		CFXJS_Util::GetProperties(pProperties, nSize);

	for (int i=0; i<nSize; i++)
		array.Add(pProperties[i].pName);
}

void CFXJS_Runtime::GetObjectMethods(const CFX_WideString& sObjName, CFX_WideStringArray& array)
{
	JSMethodSpec* pMethods = NULL;
	int nSize = 0;

// 	if (sObjName == CJS_ADBC::m_pClassName)
// 		CJS_ADBC::GetMethods(pMethods, nSize);
// 	else if (sObjName == CJS_Annotation::m_pClassName)
// 		CJS_Annotation::GetMethods(pMethods, nSize);
 	 if (sObjName == CFXJS_App::m_pClassName)
		CFXJS_App::GetMethods(pMethods, nSize);
// 	else if (sObjName == ChangeObjName(CJS_app_media::m_pClassName))
// 		CJS_app_media::GetMethods(pMethods, nSize);
// 	else if (sObjName == CJS_Bookmark::m_pClassName)
// 		CJS_Bookmark::GetMethods(pMethods, nSize);
	else if (sObjName == CFXJS_Collab::m_pClassName)
		CFXJS_Collab::GetMethods(pMethods, nSize);
	else if (sObjName == CFXJS_Color::m_pClassName)
		CFXJS_Color::GetMethods(pMethods, nSize);	
// 	else if (sObjName == CJS_Column::m_pClassName)
// 		CJS_Column::GetMethods(pMethods, nSize);
// 	else if (sObjName == CJS_ColumnInfo::m_pClassName)
// 		CJS_ColumnInfo::GetMethods(pMethods, nSize);
// 	else if (sObjName == CJS_Connection::m_pClassName)
// 		CJS_Connection::GetMethods(pMethods, nSize);
// 	else if (sObjName == CJS_Console::m_pClassName)
// 		CJS_Console::GetMethods(pMethods, nSize);
// 	else if (sObjName == CJS_cursor::m_pClassName)
// 		CJS_cursor::GetMethods(pMethods, nSize);
// 	else if (sObjName == CJS_Data::m_pClassName)
// 		CJS_Data::GetMethods(pMethods, nSize);
// 	else if (sObjName == CJS_DataSourceInfo::m_pClassName)
// 		CJS_DataSourceInfo::GetMethods(pMethods, nSize);
	else if (sObjName == L"this") 
	{
		if (GetReaderDocument())
			CFXJS_Document::GetMethods(pMethods, nSize);
		else	
			CFXJS_App::GetMethods(pMethods, nSize);
	}
	else if (sObjName == FXChangeObjName(CFXJS_doc_media::m_pClassName))
		CFXJS_doc_media::GetMethods(pMethods, nSize);
	else if (sObjName == CFXJS_Event::m_pClassName)
		CFXJS_Event::GetMethods(pMethods, nSize);	
// 	else if (sObjName == CJS_FDF::m_pClassName)
// 		CJS_FDF::GetMethods(pMethods, nSize);
	else if (sObjName == CFXJS_Field::m_pClassName)
		CFXJS_Field::GetMethods(pMethods, nSize);	
// 	else if (sObjName == CJS_FullScreen::m_pClassName)
// 		CJS_FullScreen::GetMethods(pMethods, nSize);
	else if (sObjName == CFXJS_Global::m_pClassName)
		CFXJS_Global::GetMethods(pMethods, nSize);	
	else if (sObjName == CFXJS_identity::m_pClassName)
		CFXJS_identity::GetMethods(pMethods, nSize);
// 	else if (sObjName == CJS_Link::m_pClassName)
// 		CJS_Link::GetMethods(pMethods, nSize);
// 	else if (sObjName == CJS_MediaData::m_pClassName)
// 		CJS_MediaData::GetMethods(pMethods, nSize);
// 	else if (sObjName == CJS_MediaPlayer::m_pClassName)
// 		CJS_MediaPlayer::GetMethods(pMethods, nSize);
// 	else if (sObjName == CJS_MediaSettings::m_pClassName)
// 		CJS_MediaSettings::GetMethods(pMethods, nSize);
// 	else if (sObjName == CJS_OCG::m_pClassName)
// 		CJS_OCG::GetMethods(pMethods, nSize);
// 	else if (sObjName == CJS_PlayerInfoList::m_pClassName)
// 		CJS_PlayerInfoList::GetMethods(pMethods, nSize);
// 	else if (sObjName == CJS_Rendition::m_pClassName)
// 		CJS_Rendition::GetMethods(pMethods, nSize);
// 	else if (sObjName == CJS_Row::m_pClassName)
// 		CJS_Row::GetMethods(pMethods, nSize);
// 	else if (sObjName == CJS_ScreenAnnot::m_pClassName)
// 		CJS_ScreenAnnot::GetMethods(pMethods, nSize);
// 	else if (sObjName == CJS_Statement::m_pClassName)
// 		CJS_Statement::GetMethods(pMethods, nSize);
// 	else if (sObjName == CJS_TableInfo::m_pClassName)
// 		CJS_TableInfo::GetMethods(pMethods, nSize);
	else if (sObjName == CFXJS_Util::m_pClassName)
		CFXJS_Util::GetMethods(pMethods, nSize);

	for (int i=0; i<nSize; i++)
		array.Add(pMethods[i].pName);
}

#endif //defined(_FSDK_ALL_) || defined(_FSDK_JAVASCRIPT_)