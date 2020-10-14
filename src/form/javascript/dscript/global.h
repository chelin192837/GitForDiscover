#ifndef _GLOBAL_H_
#define _GLOBAL_H_

class CFXJS_GlobalData;

struct js_global_data
{
	js_global_data()
	{
		nType = 0;
		dData = 0;
		bData = FALSE;
		sData = "";
		pData = NULL;
		bPersistent = FALSE;
		bDeleted = FALSE;
	}

	int					nType; //0:int 1:bool 2:string 3:obj
	double				dData;
	bool				bData;
	CFX_ByteString		sData;
	JSObject*			pData;
	bool				bPersistent;
	bool				bDeleted;
};

class jglobal_alternate : public CFXJS_EmbedObj
{
public:
	jglobal_alternate(CFXJS_Object* pJSObject);
	virtual ~jglobal_alternate();

public:
	FX_BOOL						setPersistent(OBJ_METHOD_PARAMS);

public:
	FX_BOOL						QueryProperty(FX_LPCWSTR propname);
	FX_BOOL						DoProperty(IDS_Context* cc, FX_LPCWSTR propname, CFXJS_PropValue & vp, JS_ErrorString & sError);
	FX_BOOL						DelProperty(IDS_Context* cc, FX_LPCWSTR propname, JS_ErrorString & sError);

	void						Initial(CFXJS_Context* pJSContext);

private:
	void						UpdateGlobalPersistentVariables();
	void						CommitGlobalPersisitentVariables();
	void						DestroyGlobalPersisitentVariables();
	FX_BOOL						SetGlobalVariables(FX_LPCSTR propname, int nType, 
									double dData, bool bData, const CFX_ByteString& sData, JSObject* pData, bool bDefaultPersistent);

	void						ObjectToArray(Dobject* pObj, CFXJS_GlobalVariableArray& array);
	void						PutObjectProperty(DFxObj*, CJS_KeyValue* pData);

private:
	CFX_MapByteStringToPtr		m_mapGlobal;
	CFX_WideString				m_sFilePath;
	CFXJS_GlobalData*				m_pGlobalData;
	CFXJS_Context*				m_pJSContext;
};


class CFXJS_Global : public CFXJS_Object
{
public:
	CFXJS_Global(JSFXObject* pObject) : CFXJS_Object(pObject) {};
	virtual ~CFXJS_Global(void){};

	virtual FX_BOOL	InitInstance(IDS_Context* cc);	

	DECLARE_SPECIAL_JS_CLASS(CFXJS_Global);

	JS_SPECIAL_STATIC_METHOD(setPersistent, jglobal_alternate, global);

};

#endif //_GLOBAL_H_
