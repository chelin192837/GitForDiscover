#include "js_fxlib.h"
#if defined(_FSDK_ALL_) || defined(_FSDK_JAVASCRIPT_)

#include "js_globaldata.h"
#include "global.h"


/* ---------------------------- global ---------------------------- */

BEGIN_JS_STATIC_CONST(CFXJS_Global)
END_JS_STATIC_CONST()

BEGIN_JS_STATIC_PROP(CFXJS_Global)
END_JS_STATIC_PROP()

BEGIN_JS_STATIC_METHOD(CFXJS_Global)
	JS_STATIC_METHOD_ENTRY(setPersistent, 2)
END_JS_STATIC_METHOD()

IMPLEMENT_SPECIAL_JS_CLASS(CFXJS_Global, jglobal_alternate, global);

FX_BOOL	CFXJS_Global::InitInstance(IDS_Context* cc)
{
	CFXJS_Context* pContext = (CFXJS_Context*)cc;
	ASSERT(pContext != NULL);

	jglobal_alternate* pGlobal = (jglobal_alternate*)GetEmbedObject();
	ASSERT(pGlobal != NULL);
	
	pGlobal->Initial(pContext);
	
	return TRUE;
};

jglobal_alternate::jglobal_alternate(CFXJS_Object* pJSObject)
	: CFXJS_EmbedObj(pJSObject),
	m_pJSContext(NULL)
{
}

jglobal_alternate::~jglobal_alternate(void)
{
	ASSERT(m_pJSContext != NULL);

	CommitGlobalPersisitentVariables();
	DestroyGlobalPersisitentVariables();

// 	CJS_Module* pModule = CJS_Module::GetModule(m_pApp);
// 	ASSERT(pModule != NULL);
	CFXJS_RuntimeFactory* pFactory = FSCRT_GetJSRuntimeFactory();
// 	CJS_RuntimeFactory* pFactory = m_pApp->m_pJSRuntimeFactory;
 	ASSERT(pFactory);
// 
 	pFactory->ReleaseGlobalData();
}
  
void jglobal_alternate::Initial(CFXJS_Context* pJSContext)
{
	m_pJSContext = pJSContext;

	FSPDFDocImp* pDoc = (FSPDFDocImp*)m_pJSContext->GetReaderDocument();
	FSFormFillerImp* pFormFiller = pDoc->GetForm()->GetFormFiller();

// 	CJS_Module* pModule = CJS_Module::GetModule(m_pApp);
// 	ASSERT(pModule != NULL);
 	CFXJS_RuntimeFactory* pFactory = pFormFiller->GetRuntimFactory();
// 	ASSERT(pFactory);
 	m_pGlobalData = pFactory->NewGlobalData(NULL);
	UpdateGlobalPersistentVariables();
}

FX_BOOL	jglobal_alternate::QueryProperty(FX_LPCWSTR propname)
{
	return CFX_WideString(propname) != L"setPersistent";
}

FX_BOOL	jglobal_alternate::DelProperty(IDS_Context* cc, FX_LPCWSTR propname, JS_ErrorString& sError)
{
	js_global_data* pData = NULL;
	CFX_ByteString sPropName = CFX_ByteString::FromUnicode(propname);

	if (m_mapGlobal.Lookup(sPropName, (FX_LPVOID&)pData))
	{
		pData->bDeleted = TRUE;
		return TRUE;
	}

	return FALSE;
}

FX_BOOL jglobal_alternate::DoProperty(IDS_Context* cc, FX_LPCWSTR propname, CFXJS_PropValue& vp, JS_ErrorString& sError)
{
	if (vp.IsSetting())
	{
		CFX_ByteString sPropName = CFX_ByteString::FromUnicode(propname);
		switch (vp.GetType())
		{
		case VT_number:
			{
				double dData;
				vp >> dData;
				return SetGlobalVariables(sPropName, JS_GLOBALDATA_TYPE_NUMBER, dData, false, "", NULL, FALSE);
			}
		case VT_boolean:
			{
				bool bData;
				vp >> bData;
				return SetGlobalVariables(sPropName, JS_GLOBALDATA_TYPE_BOOLEAN, 0, (bool)vp, "", NULL, FALSE);
			}
		case VT_string:
			{
				CFX_ByteString sData;
				vp >> sData;
				return SetGlobalVariables(sPropName, JS_GLOBALDATA_TYPE_STRING, 0, false, sData, NULL, FALSE);
			}
		case VT_object:
			{
				JSObject* pData = (JSObject*)vp;
				if (pData)
					return SetGlobalVariables(sPropName, JS_GLOBALDATA_TYPE_OBJECT, 0, false, "", pData, FALSE);
				else
				{
					if (vp.IsArrayObject())
					{
						CFXJS_Array array;
						vp.ConvertToArray(array);
						return SetGlobalVariables(sPropName, JS_GLOBALDATA_TYPE_OBJECT, 0, false, "", 
							(Dobject*)(Darray*)array, FALSE);
					}
					else
						return FALSE;
				}
			}
		case VT_null:
			{
				return SetGlobalVariables(sPropName, JS_GLOBALDATA_TYPE_NULL, 0, false, "", NULL, FALSE);
			}
		case VT_undefined:
			{
				DelProperty(cc, propname, sError);
				return TRUE;
			}
		}
	}
	else
	{
		js_global_data* pData = NULL;
		CFX_ByteString sPropName = CFX_ByteString::FromUnicode(propname);

		if (m_mapGlobal.Lookup(sPropName, (FX_LPVOID&)pData))
		{
			if (pData)
			{
				if (!pData->bDeleted)
				{
					switch (pData->nType)
					{
					case JS_GLOBALDATA_TYPE_NUMBER:
						vp << pData->dData;
						break;
					case JS_GLOBALDATA_TYPE_BOOLEAN:
						vp << pData->bData;
						break;
					case JS_GLOBALDATA_TYPE_STRING:
						vp << pData->sData;
						break;
					case JS_GLOBALDATA_TYPE_OBJECT:
						vp << pData->pData;
						break;
					case JS_GLOBALDATA_TYPE_NULL:
						vp.SetNull();
						break;
					}
					return TRUE;
				}
				else
				{
					return TRUE;
				}
			}
			else
			{
				vp.SetNull();
				return TRUE;
			}
		}
		else
		{
			vp.SetNull();
			return TRUE;
		}
	}

	return FALSE;
}

FX_BOOL jglobal_alternate::setPersistent(OBJ_METHOD_PARAMS)
{
	if (params.size() != 2)
	{
		//sError = JSGetStringFromID(IDS_STRING_JSPARAMERROR);	
		return FALSE;
	}

	CFX_ByteString sName = params[0];

	js_global_data* pData = NULL;
	if (m_mapGlobal.Lookup(sName, (FX_LPVOID&)pData))
	{
		if (pData && !pData->bDeleted)
		{
			pData->bPersistent = (bool)params[1];
			return TRUE;
		}
	}

	//sError = JSGetStringFromID(IDS_JSPARAM_INCORRECT);	
	return FALSE;
}

void jglobal_alternate::UpdateGlobalPersistentVariables()
{
	ASSERT(m_pGlobalData != NULL);

	for (int i=0,sz=m_pGlobalData->GetSize(); i<sz; i++)
	{
		CJS_GlobalData_Element* pData = m_pGlobalData->GetAt(i);
		ASSERT(pData != NULL);

		switch (pData->data.nType)
		{
		case JS_GLOBALDATA_TYPE_NUMBER:
			this->SetGlobalVariables(pData->data.sKey, JS_GLOBALDATA_TYPE_NUMBER, pData->data.dData, false, "", NULL, pData->bPersistent == 1);
			DS_PutObjectNumber((JSObject*)(JSFXObject*)(*m_pJSObject),
				pData->data.sKey.UTF8Decode(), pData->data.dData);
			break;
		case JS_GLOBALDATA_TYPE_BOOLEAN:
			this->SetGlobalVariables(pData->data.sKey, JS_GLOBALDATA_TYPE_BOOLEAN, 0, (bool)(pData->data.bData == 1), "", NULL, pData->bPersistent == 1);
			DS_PutObjectBoolean((JSObject*)(JSFXObject*)(*m_pJSObject),
				pData->data.sKey.UTF8Decode(), (bool)(pData->data.bData == 1));
			break;
		case JS_GLOBALDATA_TYPE_STRING:
			this->SetGlobalVariables(pData->data.sKey, JS_GLOBALDATA_TYPE_STRING, 0, false, pData->data.sData, NULL, pData->bPersistent == 1);
			DS_PutObjectString((JSObject*)(JSFXObject*)(*m_pJSObject),
				pData->data.sKey.UTF8Decode(), 
				pData->data.sData.UTF8Decode());
			break;
		case JS_GLOBALDATA_TYPE_OBJECT:
			{
				IDS_Runtime* pRuntime = DS_GetRuntime((JSFXObject*)(*m_pJSObject));
				DFxObj* pObj = DS_NewFxDynamicObj(pRuntime, NULL, -1);

				PutObjectProperty(pObj, &pData->data);

				this->SetGlobalVariables(pData->data.sKey, JS_GLOBALDATA_TYPE_OBJECT, 0, false, "", 
					(JSObject*)pObj, pData->bPersistent == 1);
				DS_PutObjectObject((JSObject*)(JSFXObject*)(*m_pJSObject),
					pData->data.sKey.UTF8Decode(), (JSObject*)pObj);
			}
			break;
		case JS_GLOBALDATA_TYPE_NULL:
			this->SetGlobalVariables(pData->data.sKey, JS_GLOBALDATA_TYPE_NULL, 0, false, "", NULL, pData->bPersistent == 1);
			DS_PutObjectNull((JSObject*)(JSFXObject*)(*m_pJSObject),
				pData->data.sKey.UTF8Decode());
			break;
		}
	}
}

void jglobal_alternate::CommitGlobalPersisitentVariables()
{
	ASSERT(m_pGlobalData != NULL);

	FX_POSITION	 pos = m_mapGlobal.GetStartPosition();
	while (pos)
	{
		CFX_ByteString name; 
		js_global_data* pData = NULL;
		m_mapGlobal.GetNextAssoc(pos, name, (FX_LPVOID&)pData);
		
		if (pData)
		{
			if (pData->bDeleted)
			{
				m_pGlobalData->DeleteGlobalVariable(name);
			}
			else
			{
				switch (pData->nType)
				{
				case JS_GLOBALDATA_TYPE_NUMBER:
					m_pGlobalData->SetGlobalVariableNumber(name, pData->dData);
					m_pGlobalData->SetGlobalVariablePersistent(name, pData->bPersistent);
					break;
				case JS_GLOBALDATA_TYPE_BOOLEAN:
					m_pGlobalData->SetGlobalVariableBoolean(name, pData->bData);
					m_pGlobalData->SetGlobalVariablePersistent(name, pData->bPersistent);
					break;
				case JS_GLOBALDATA_TYPE_STRING:
					m_pGlobalData->SetGlobalVariableString(name, pData->sData);
					m_pGlobalData->SetGlobalVariablePersistent(name, pData->bPersistent);
					break;
				case JS_GLOBALDATA_TYPE_OBJECT:
					if (pData->pData)
					{
						CFXJS_GlobalVariableArray array;
						ObjectToArray(pData->pData, array);
						m_pGlobalData->SetGlobalVariableObject(name, array);
						m_pGlobalData->SetGlobalVariablePersistent(name, pData->bPersistent);
					}
					break;
				case JS_GLOBALDATA_TYPE_NULL:
					m_pGlobalData->SetGlobalVariableNull(name);
					m_pGlobalData->SetGlobalVariablePersistent(name, pData->bPersistent);
					break;
				}
			}
		}
	}
}

void jglobal_alternate::ObjectToArray(Dobject* pObj, CFXJS_GlobalVariableArray& array)
{
	ASSERT(pObj != NULL);

	Value** pKeyList = NULL;
	Value** pValueList = NULL;

	int	nObjElements = DS_GetObjectElements(pObj, pKeyList, pValueList);

	if (nObjElements > 0)
	{
		pKeyList = (Value**)new FX_LPVOID[nObjElements];
		pValueList = (Value**)new FX_LPVOID[nObjElements];

		DS_GetObjectElements(pObj, pKeyList, pValueList);

		for (int i=0; i<nObjElements; i++)
		{
			CFX_ByteString sKey = CFXJS_Value(pKeyList[i], GET_VALUE_TYPE(pKeyList[i]));
			VALUETYPE vt = GET_VALUE_TYPE(pValueList[i]);
			switch (vt)
			{
			case VT_number:
				{
					CJS_KeyValue* pObjElement = new CJS_KeyValue;
					pObjElement->nType = JS_GLOBALDATA_TYPE_NUMBER;
					pObjElement->sKey = sKey;
					pObjElement->dData = DS_ToNumber(pValueList[i]);
					array.Add(pObjElement);
				}
				break;
			case VT_boolean:
				{
					CJS_KeyValue* pObjElement = new CJS_KeyValue;
					pObjElement->nType = JS_GLOBALDATA_TYPE_BOOLEAN;
					pObjElement->sKey = sKey;
					pObjElement->dData = DS_ToBoolean(pValueList[i]);
					array.Add(pObjElement);
				}
				break;
			case VT_string:
				{
					CFX_ByteString sValue = CFXJS_Value(pValueList[i], VT_string);
					CJS_KeyValue* pObjElement = new CJS_KeyValue;
					pObjElement->nType = JS_GLOBALDATA_TYPE_STRING;
					pObjElement->sKey = sKey;
					pObjElement->sData = sValue;
					array.Add(pObjElement);
				}
				break;
			case VT_object:
				{
					CJS_KeyValue* pObjElement = new CJS_KeyValue;
					pObjElement->nType = JS_GLOBALDATA_TYPE_OBJECT;
					pObjElement->sKey = sKey;
					ObjectToArray(DS_ToObject(pValueList[i]), pObjElement->objData);
					array.Add(pObjElement);
				}
				break;
			case VT_null:
				{
					CJS_KeyValue* pObjElement = new CJS_KeyValue;
					pObjElement->nType = JS_GLOBALDATA_TYPE_NULL;
					pObjElement->sKey = sKey;
					array.Add(pObjElement);
				}
				break;
			}
		}

		delete []pKeyList;
		delete []pValueList;
	}
}

void jglobal_alternate::PutObjectProperty(DFxObj* pObj, CJS_KeyValue* pData)
{
	ASSERT(pObj != NULL);
	ASSERT(pData != NULL);

	for (int i=0,sz=pData->objData.Count(); i<sz; i++)
	{
		CJS_KeyValue* pObjData = pData->objData.GetAt(i);
		ASSERT(pObjData != NULL);

		switch (pObjData->nType)
		{
		case JS_GLOBALDATA_TYPE_NUMBER:
			DS_PutObjectNumber((JSObject*)pObj, pObjData->sKey.UTF8Decode(), pObjData->dData);
			break;
		case JS_GLOBALDATA_TYPE_BOOLEAN:
			DS_PutObjectBoolean((JSObject*)pObj, pObjData->sKey.UTF8Decode(), (bool)(pObjData->bData == 1));
			break;
		case JS_GLOBALDATA_TYPE_STRING:
			DS_PutObjectString((JSObject*)pObj, pObjData->sKey.UTF8Decode(), pObjData->sData.UTF8Decode());
			break;
		case JS_GLOBALDATA_TYPE_OBJECT:
			{
				IDS_Runtime* pRuntime = DS_GetRuntime((JSFXObject*)(*m_pJSObject));
				DFxObj* pNewObj = DS_NewFxDynamicObj(pRuntime, NULL, -1);
				PutObjectProperty(pNewObj, pObjData);
				DS_PutObjectObject((JSObject*)pObj, pObjData->sKey.UTF8Decode(), (JSObject*)pNewObj);
			}
			break;
		case JS_GLOBALDATA_TYPE_NULL:
			DS_PutObjectNull((JSObject*)pObj, pObjData->sKey.UTF8Decode());
			break;
		}
	}
}

void jglobal_alternate::DestroyGlobalPersisitentVariables()
{
	FX_POSITION	 pos = m_mapGlobal.GetStartPosition();
	while (pos)
	{
		CFX_ByteString name; 
		js_global_data* pData = NULL;
		m_mapGlobal.GetNextAssoc(pos, name, (FX_LPVOID&)pData);
		delete pData;
	}

	m_mapGlobal.RemoveAll();
}

//bPersistent需要使用老的
FX_BOOL jglobal_alternate::SetGlobalVariables(FX_LPCSTR propname, int nType, 
				double dData, bool bData, const CFX_ByteString& sData, JSObject* pData, bool bDefaultPersistent)
{
	if (propname == NULL) return FALSE;

	js_global_data* pTemp = NULL;
	m_mapGlobal.Lookup(propname, (FX_LPVOID&)pTemp);

	if (pTemp)
	{
		if (pTemp->bDeleted || pTemp->nType != nType)
		{
			pTemp->dData = 0;
			pTemp->bData = 0;
			pTemp->sData = "";
			pTemp->pData = NULL;
			pTemp->nType = nType;
		}

		pTemp->bDeleted = FALSE;

		switch (nType)
		{
		case JS_GLOBALDATA_TYPE_NUMBER:
			{
				pTemp->dData = dData;
			}
			break;
		case JS_GLOBALDATA_TYPE_BOOLEAN:
			{
				pTemp->bData = bData;
			}
			break;
		case JS_GLOBALDATA_TYPE_STRING:
			{
				pTemp->sData = sData;
			}
			break;
		case JS_GLOBALDATA_TYPE_OBJECT:
			{
				pTemp->pData = pData;
			}
			break;
		case JS_GLOBALDATA_TYPE_NULL:
			break;
		default:
			return FALSE;
		}	

		return TRUE;
	}

	js_global_data* pNewData = NULL;

	switch (nType)
	{
	case JS_GLOBALDATA_TYPE_NUMBER:
		{
			pNewData = new js_global_data;
			pNewData->nType = JS_GLOBALDATA_TYPE_NUMBER;
			pNewData->dData = dData;
			pNewData->bPersistent = bDefaultPersistent;
		}
		break;
	case JS_GLOBALDATA_TYPE_BOOLEAN:
		{
			pNewData = new js_global_data;
			pNewData->nType = JS_GLOBALDATA_TYPE_BOOLEAN;
			pNewData->bData = bData;
			pNewData->bPersistent = bDefaultPersistent;
		}
		break;
	case JS_GLOBALDATA_TYPE_STRING:
		{
			pNewData = new js_global_data;
			pNewData->nType = JS_GLOBALDATA_TYPE_STRING;
			pNewData->sData = sData;
			pNewData->bPersistent = bDefaultPersistent;
		}
		break;
	case JS_GLOBALDATA_TYPE_OBJECT:
		{
			pNewData = new js_global_data;
			pNewData->nType = JS_GLOBALDATA_TYPE_OBJECT;
			pNewData->pData = pData;
			pNewData->bPersistent = bDefaultPersistent;
		}
		break;
	case JS_GLOBALDATA_TYPE_NULL:
		{
			pNewData = new js_global_data;
			pNewData->nType = JS_GLOBALDATA_TYPE_NULL;
			pNewData->bPersistent = bDefaultPersistent;
		}
		break;
	default:
		return FALSE;
	}	

	m_mapGlobal.SetAt(propname, (FX_LPVOID)pNewData);

	return TRUE;
}

#endif //defined(_FSDK_ALL_) || defined(_FSDK_JAVASCRIPT_)