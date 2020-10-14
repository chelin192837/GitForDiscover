#ifndef _JS_RUNTIME_H_
#define _JS_RUNTIME_H_

// class IJS_RuntimeAcc
// {
// public:
// 	virtual void*				GetGlobalData() = 0;
// 	virtual void				SetGlobalData(void* pData) = 0;
// 	virtual int					DefineObj(const wchar_t* sObjName, DSOBJTYPE eObjType, LP_CONSTRUCTOR pConstructor,
// 		LP_DESTRUCTOR pDestructor, unsigned bApplyNew) = 0;
// 	virtual FX_BOOL				DefineObjConst(int nObjDefnID, const wchar_t* sConstName, Value * pDefault) = 0;
// 	virtual FX_BOOL				DefineObjProperty(int nObjDefnID, const wchar_t* sPropName, 
// 		LP_PROP_GET pPropGet, LP_PROP_PUT pPropPut) = 0;
// 	virtual FX_BOOL				DefineObjAllProperties(int nObjDefnID, LP_PROP_QUERY pPropQuery, 
// 		LP_PROP_GET pPropGet, LP_PROP_PUT pPropPut, LP_PROP_DEL pPropDel) = 0;
// 	virtual FX_BOOL				DefineObjMethod(int nObjDefnID, const wchar_t* sMethodName, 
// 		LP_METHOD_CALL pMethodCall, unsigned nParamNum) = 0;
// 	virtual FX_BOOL				DefineGlobalConst(const wchar_t* sConstName, Value* pDefault) = 0;
// 	virtual FX_BOOL				DefineGlobalMethod(FX_LPCWSTR sMethodName, 
// 		LP_METHOD_CALL pMethodCall, unsigned nParamNum) = 0;
// 	virtual FX_INT32			GetObjDefnID(const wchar_t* pObjName) = 0;
// 	virtual FX_BOOL				SetThisObj(FX_INT32 nThisObjID) = 0;
// 	virtual FX_BOOL				SetGlobalObj(FX_INT32 nGlobalObjID) = 0;
// 	virtual Dobject*			NewDynamicObj(IJS_Context* pJSContext, FX_INT32 nObjDefnID) = 0;
// 	virtual Dobject*			NewGlobalObject(IJS_Context* pJSContext, FX_INT32 nObjDefnID) = 0;
// 
// 	virtual FX_BOOL				RunScript(IJS_Context* pContext, const CFX_WideString& script, 
// 		CFX_WideString& info, Value*& pRet, Dobject* pVariables) = 0;
// 
// 	static IJS_Context*			GetContext(IDS_Context* pContext);
// 	static IDS_Context*			GetDSContext(IJS_Context* pJSContext);
// 	static VALUETYPE			GetValueType(Value* p);
// };

class CJS_FieldEvent
{
public:
	CFX_WideString		sTargetName;
	JS_EVENT_T			eEventType;
	CJS_FieldEvent*		pNext;
};

class CFXJS_Runtime : public IJS_Runtime, public IDS_Runtime
{
public:
	CFXJS_Runtime(CFSCRT_LTAppHandler* pApp);
	virtual ~CFXJS_Runtime();

	
	//IDS_Runtime
	virtual void*		GetGlobalData();
	virtual bool		IsThisAsGlobal();

	//IJS_Runtime
	virtual IJS_Context*					NewContext();
	virtual void							ReleaseContext(IJS_Context * pContext);
	virtual void							SetReaderDocument(FSPDFDoc* pReaderDoc);
	virtual	FSPDFDoc*						GetReaderDocument(){return m_pDocument;};

	virtual void							GetObjectNames(CFX_WideStringArray& array);
	virtual void							GetObjectConsts(const CFX_WideString& swObjName, CFX_WideStringArray& array);
	virtual void							GetObjectProps(const CFX_WideString& swObjName, CFX_WideStringArray& array);
	virtual void							GetObjectMethods(const CFX_WideString& swObjName, CFX_WideStringArray& array);

	CFSCRT_LTAppHandler*						GetReaderApp(){return m_pApp;}

	FX_BOOL									InitJSObjects();

	FX_BOOL									AddEventToLoop(const CFX_WideString& sTargetName, JS_EVENT_T eEventType);
	void									RemoveEventInLoop(const CFX_WideString& sTargetName, JS_EVENT_T eEventType);
	void									RemoveEventsInLoop(CJS_FieldEvent* pStart);

	void									BeginBlock(){m_bBlocking = TRUE;}
	void									EndBlock(){m_bBlocking = FALSE;}
	FX_BOOL									IsBlocking(){return m_bBlocking;}

	void									SetGlobalData(void* pData);

		

protected:
	CFX_ArrayTemplate<CFXJS_Context *>		m_ContextArray;
	CFSCRT_LTAppHandler*						m_pApp;
	FSPDFDoc*						m_pDocument;
	FX_BOOL									m_bBlocking;
	CJS_FieldEvent*							m_pFieldEventPath;
	void*									m_pGlobalData;
};

#endif //_JS_RUNTIME_H_

