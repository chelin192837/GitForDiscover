#ifndef _JS_OBJECT_H_
#define _JS_OBJECT_H_

class CFXJS_Object;
class CFXJS_Timer;
class CFXJS_Context;
class CFXJS_Runtime;

class CFXJS_EmbedObj : public CFX_Object
{
public:
	CFXJS_EmbedObj(CFXJS_Object* pJSObject);
	virtual ~CFXJS_EmbedObj();

	virtual void				TimerProc(CFXJS_Timer* pTimer){};

	CFXJS_Timer*					BeginTimer(CFSCRT_LTAppHandler * pApp, FX_DWORD nElapse);
	void						EndTimer(CFXJS_Timer* pTimer);

	CFXJS_Object*					GetJSObject(){return m_pJSObject;};
	operator					CFXJS_Object* (){return m_pJSObject;};

	void						Alert(CFXJS_Context* pContext, FX_LPCWSTR swMsg);
	FX_BOOL						IsSafeMode(IDS_Context* cc);

protected:

	CFXJS_Object*					m_pJSObject;
};

class CFXJS_Object : public CFX_Object
{
public:
	CFXJS_Object(JSFXObject* pObject);
	virtual ~CFXJS_Object(void);
	
	virtual FX_BOOL				IsType(FX_LPCSTR sClassName){return TRUE;};
	virtual CFX_ByteString		GetClassName(){return "";};

	virtual FX_BOOL				InitInstance(IDS_Context* cc){return TRUE;};
	virtual FX_BOOL				ExitInstance(){return TRUE;};

	operator					JSFXObject* (){return m_pObject;};
	operator					CFXJS_EmbedObj* (){return m_pEmbedObj;};

	void						SetEmbedObject(CFXJS_EmbedObj* pObj){m_pEmbedObj = pObj;};
	CFXJS_EmbedObj *				GetEmbedObject(){return m_pEmbedObj;};

	static void					Alert(CFXJS_Context* pContext, FX_LPCWSTR swMsg);

protected:
	CFXJS_EmbedObj *				m_pEmbedObj;
	JSFXObject *				m_pObject;
};

struct JS_TIMER_MAP
{
	FX_DWORD nID;
	CFXJS_Timer * pTimer;
};

typedef CFX_ArrayTemplate<JS_TIMER_MAP*>	CTimerMapArray;

struct JS_TIMER_MAPARRAY
{
public:
	JS_TIMER_MAPARRAY()
	{
	}

	~JS_TIMER_MAPARRAY()
	{
		Reset();
	}

	void Reset()
	{
		for (int i=0,sz=m_Array.GetSize(); i<sz; i++)
			delete m_Array.GetAt(i);

		m_Array.RemoveAll();
	}

	void SetAt(FX_DWORD nIndex,CFXJS_Timer * pTimer)
	{
		int i = Find(nIndex);

		if (i>=0)
		{
			if (JS_TIMER_MAP * pMap = m_Array.GetAt(i))
				pMap->pTimer = pTimer;
		}
		else
		{
			if (JS_TIMER_MAP * pMap = new JS_TIMER_MAP)
			{
				pMap->nID = nIndex;
				pMap->pTimer = pTimer;
				m_Array.Add(pMap);
			}
		}
	}

	CFXJS_Timer * GetAt(FX_DWORD nIndex)
	{
		int i = Find(nIndex);

		if (i>=0)
		{
			if (JS_TIMER_MAP * pMap = m_Array.GetAt(i))
				return pMap->pTimer;
		}
		return NULL;
	}

	void RemoveAt(FX_DWORD nIndex)
	{
		int i = Find(nIndex);

		if (i>=0)
		{
			delete m_Array.GetAt(i);
			m_Array.RemoveAt(i);
		}
		//To prevent potential fake memory leak reported by vc6.
		if(m_Array.GetSize() == 0)
			m_Array.RemoveAll();
	}

	int Find(FX_DWORD nIndex)
	{		
		for (int i=0,sz=m_Array.GetSize(); i<sz; i++)
		{			
			if (JS_TIMER_MAP * pMap = m_Array.GetAt(i))
			{
				if (pMap->nID == nIndex)
					return i;
			}
		}

		return -1;
	}

	CTimerMapArray		m_Array;
};

static JS_TIMER_MAPARRAY	m_sTimeMap;

class CFXJS_Timer
{
public:
	CFXJS_Timer(CFXJS_EmbedObj* pObj, CFSCRT_LTAppHandler* pApp): m_pEmbedObj(pObj), 
		m_nTimerID(0), 
		m_bProcessing(FALSE),
		m_dwStartTime(0),
		m_dwTimeOut(0),
		m_dwElapse(0),
		m_pRuntime(NULL),
		m_nType(0),
		m_pApp(pApp)
	{
	}

	virtual ~CFXJS_Timer()
	{
		KillJSTimer();
	}

public:
	FX_DWORD SetJSTimer(FX_DWORD nElapse);
	void KillJSTimer();

	void SetType(int nType)
	{
		m_nType = nType;
	}

	int GetType() const
	{
		return m_nType;
	}

	void SetStartTime(FX_DWORD dwStartTime)
	{
		m_dwStartTime = dwStartTime;
	}

	FX_DWORD GetStartTime() const
	{
		return m_dwStartTime;
	}

	void SetTimeOut(FX_DWORD dwTimeOut)
	{
		m_dwTimeOut = dwTimeOut;
	}

	FX_DWORD GetTimeOut() const
	{
		return m_dwTimeOut;
	}

	void SetRuntime(CFXJS_Runtime* pRuntime)
	{
		m_pRuntime = pRuntime;
	}

	CFXJS_Runtime* GetRuntime() const
	{
		return m_pRuntime;
	}

	void SetJScript(const CFX_WideString& script)
	{
		m_swJScript = script;
	}

	CFX_WideString GetJScript() const
	{
		return m_swJScript;
	}

	static void TimerProc(FS_INT32 idEvent)
	{
		if (CFXJS_Timer * pTimer = m_sTimeMap.GetAt(idEvent))
		{
			if (!pTimer->m_bProcessing)
			{
				pTimer->m_bProcessing = TRUE;
				if (pTimer->m_pEmbedObj) pTimer->m_pEmbedObj->TimerProc(pTimer);
				pTimer->m_bProcessing = FALSE;
			}
			else
			{
				//	TRACE(L"BUSY!\n");
			}
		}
	};

private:
	FX_DWORD							m_nTimerID;	
	CFXJS_EmbedObj*					m_pEmbedObj;
	FX_BOOL							m_bProcessing;

	//data
	FX_DWORD							m_dwStartTime;
	FX_DWORD							m_dwTimeOut;
	FX_DWORD						m_dwElapse;
	CFXJS_Runtime*					m_pRuntime;
	CFX_WideString					m_swJScript;
	int								m_nType; //0:Interval; 1:TimeOut

	CFSCRT_LTAppHandler*			m_pApp;
};


#endif //_JS_OBJECT_H_
