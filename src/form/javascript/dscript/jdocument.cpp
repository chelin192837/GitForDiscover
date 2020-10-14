#include "js_fxlib.h"
#if defined(_FSDK_ALL_) || defined(_FSDK_JAVASCRIPT_)

#include "document.h"
#include "app.h"
#include "field.h"
#include "bookmark.h"
#include "icon.h"
#include "collab.h"
#include "field.h"
#include "annotation.h"
#include "link.h"
#include "media.h"
#include "ocg.h"

BEGIN_JS_STATIC_CONST(CFXJS_PrintParamsObj)
END_JS_STATIC_CONST()

BEGIN_JS_STATIC_PROP(CFXJS_PrintParamsObj)
END_JS_STATIC_PROP()

BEGIN_JS_STATIC_METHOD(CFXJS_PrintParamsObj)
END_JS_STATIC_METHOD()

IMPLEMENT_JS_CLASS(CFXJS_PrintParamsObj, FXPrintParamsObj,PrintParamsObj)

FXPrintParamsObj::FXPrintParamsObj(CFXJS_Object* pJSObject)
: CFXJS_EmbedObj(pJSObject)
{
	bUI = TRUE;
	nStart = 0;
	nEnd = 0;
	bSilent = FALSE;
	bShrinkToFit = FALSE;
	bPrintAsImage = FALSE;
	bReverse = FALSE;
	bAnnotations = TRUE;
}

//*****************************************************************************
//* CJS_Document
//*****************************************************************************

#define MINWIDTH  5.0f
#define MINHEIGHT 5.0f

BEGIN_JS_STATIC_CONST(CFXJS_Document)
END_JS_STATIC_CONST()

BEGIN_JS_STATIC_PROP(CFXJS_Document)
	JS_STATIC_PROP_ENTRY(ADBE)
	JS_STATIC_PROP_ENTRY(author)
	JS_STATIC_PROP_ENTRY(baseURL)
	JS_STATIC_PROP_ENTRY(bookmarkRoot)
	JS_STATIC_PROP_ENTRY(calculate)
	JS_STATIC_PROP_ENTRY(Collab)
	JS_STATIC_PROP_ENTRY(creationDate)
	JS_STATIC_PROP_ENTRY(creator)
	JS_STATIC_PROP_ENTRY(delay)
	JS_STATIC_PROP_ENTRY(dirty)
	JS_STATIC_PROP_ENTRY(documentFileName)
	JS_STATIC_PROP_ENTRY(external)
	JS_STATIC_PROP_ENTRY(filesize)
	JS_STATIC_PROP_ENTRY(icons)
	JS_STATIC_PROP_ENTRY(info)   
	JS_STATIC_PROP_ENTRY(keywords)
	JS_STATIC_PROP_ENTRY(layout)
	JS_STATIC_PROP_ENTRY(media)
	JS_STATIC_PROP_ENTRY(modDate)
	JS_STATIC_PROP_ENTRY(mouseX)
	JS_STATIC_PROP_ENTRY(mouseY)
	JS_STATIC_PROP_ENTRY(numFields)
	JS_STATIC_PROP_ENTRY(numPages)
	JS_STATIC_PROP_ENTRY(pageNum)   
	JS_STATIC_PROP_ENTRY(pageWindowRect)
	JS_STATIC_PROP_ENTRY(path)
	JS_STATIC_PROP_ENTRY(producer)
	JS_STATIC_PROP_ENTRY(subject)
	JS_STATIC_PROP_ENTRY(title)
	JS_STATIC_PROP_ENTRY(zoom)
	JS_STATIC_PROP_ENTRY(zoomType)
END_JS_STATIC_PROP()

BEGIN_JS_STATIC_METHOD(CFXJS_Document)
	JS_STATIC_METHOD_ENTRY(addAnnot,0)
	JS_STATIC_METHOD_ENTRY(addField, 4)
	JS_STATIC_METHOD_ENTRY(addLink, 0)
	JS_STATIC_METHOD_ENTRY(addIcon, 0)
	JS_STATIC_METHOD_ENTRY(calculateNow, 0)
	JS_STATIC_METHOD_ENTRY(closeDoc, 0)
	JS_STATIC_METHOD_ENTRY(createDataObject, 0)
	JS_STATIC_METHOD_ENTRY(deletePages, 2)
	JS_STATIC_METHOD_ENTRY(exportAsText, 3)
	JS_STATIC_METHOD_ENTRY(exportAsFDF, 6)
	JS_STATIC_METHOD_ENTRY(exportAsXFDF, 5)
	JS_STATIC_METHOD_ENTRY(extractPages, 3)
	JS_STATIC_METHOD_ENTRY(getAnnot, 0)
	JS_STATIC_METHOD_ENTRY(getAnnots, 2)
	JS_STATIC_METHOD_ENTRY(getAnnot3D, 2)
	JS_STATIC_METHOD_ENTRY(getAnnots3D, 1)
	JS_STATIC_METHOD_ENTRY(getField, 1)
	JS_STATIC_METHOD_ENTRY(getIcon, 0)
	JS_STATIC_METHOD_ENTRY(getLinks, 0)
	JS_STATIC_METHOD_ENTRY(getNthFieldName, 1)
	JS_STATIC_METHOD_ENTRY(getOCGs, 0)
	JS_STATIC_METHOD_ENTRY(getPageBox, 0)
	JS_STATIC_METHOD_ENTRY(getPageNthWord, 3)
	JS_STATIC_METHOD_ENTRY(getPageNthWordQuads, 2)
	JS_STATIC_METHOD_ENTRY(getPageNumWords, 1)
	JS_STATIC_METHOD_ENTRY(getPrintParams, 0)
	JS_STATIC_METHOD_ENTRY(getURL, 2)
	JS_STATIC_METHOD_ENTRY(importAnFDF, 1)
	JS_STATIC_METHOD_ENTRY(importAnXFDF, 1)
	JS_STATIC_METHOD_ENTRY(importTextData, 2)
	JS_STATIC_METHOD_ENTRY(insertPages, 4)
	JS_STATIC_METHOD_ENTRY(mailForm, 6)
	JS_STATIC_METHOD_ENTRY(print, 9)
	JS_STATIC_METHOD_ENTRY(removeField, 1)
	JS_STATIC_METHOD_ENTRY(replacePages, 4)
	JS_STATIC_METHOD_ENTRY(resetForm, 1)
	JS_STATIC_METHOD_ENTRY(removeIcon, 0)
	JS_STATIC_METHOD_ENTRY(saveAs, 5)
	JS_STATIC_METHOD_ENTRY(submitForm, 23)
	JS_STATIC_METHOD_ENTRY(mailDoc, 0)		
END_JS_STATIC_METHOD()

IMPLEMENT_JS_CLASS(CFXJS_Document, JDocument, Document)

FX_BOOL	CFXJS_Document::InitInstance(IDS_Context* cc)
{
	CFXJS_Context* pContext = (CFXJS_Context*)cc;
	ASSERT(pContext != NULL);

	JDocument* pDoc = (JDocument*)GetEmbedObject();
	ASSERT(pDoc != NULL);
	
	pDoc->AttachDoc(pContext->GetReaderDocument());
	return TRUE;
};

//*****************************************************************************
//* Document
//*****************************************************************************

JDocument::JDocument(CFXJS_Object* pJSObject) : CFXJS_EmbedObj(pJSObject),
	m_cwBaseURL(L""),
	m_pIconTree(NULL),
	m_pDocument(NULL),
	m_bDelay(FALSE)
{
}

JDocument::~JDocument()
{
	if (m_pIconTree)
	{
		m_pIconTree->DeleteIconTree();
		delete m_pIconTree;
		m_pIconTree = NULL;
	}
	for (int i=0; i<m_DelayData.GetSize(); i++)
	{
		if (CJS_DelayData* pData = m_DelayData.GetAt(i))
		{
			delete pData;
			pData = NULL;
			m_DelayData.SetAt(i, NULL);
			
		}
	}

// 	for (int j=0;j<m_DelayAnnotData.GetSize();j++)
// 	{
// 		if (CJS_AnnotObj* pData = m_DelayAnnotData.GetAt(j))
// 		{
// 			if(pData->dashArray)
// 			{
// 				delete pData->dashArray;
// 				pData->dashArray = NULL;
// 			}
// 			if(pData->pointsArray)
// 			{
// 				delete pData->pointsArray;
// 				pData->pointsArray = NULL;
// 			}
// 			if(pData->quadsArray)
// 			{
// 				delete pData->quadsArray;
// 				pData->quadsArray = NULL;
// 			}
// 			if(pData->gesturesArray)
// 			{
// 				delete pData->gesturesArray;
// 				pData->gesturesArray = NULL;
// 			}
// 			if(pData->verticesArray)
// 			{
// 				delete pData->verticesArray;
// 				pData->verticesArray = NULL;
// 			}
// 			delete pData;
// 			m_DelayAnnotData.SetAt(i, NULL);
// 			
// 		}
// 	}
	m_DelayData.RemoveAll();
	m_DelayAnnotData.RemoveAll();
}

//the total number of fileds in document.
FX_BOOL JDocument::numFields(OBJ_PROP_PARAMS)
{
	if (!vp.IsGetting()) return FALSE;

	ASSERT(m_pDocument != NULL);

   	FSFormImp* pForm = m_pDocument->GetForm();
	ASSERT(pForm != NULL);

	CPDF_InterForm *pPDFForm = pForm->ST_GetSTInterForm()->m_pInterForm;
	ASSERT(pPDFForm != NULL);

	vp << (int)pPDFForm->CountFields();

	return TRUE;
}

FX_BOOL JDocument::dirty(OBJ_PROP_PARAMS)
{
	FSFormFillerImp* pFormFiller = m_pDocument->GetForm()->GetFormFiller();
// 	CFSCRT_LTAppHandler* pApp = FSCRT_GetLTAppHandler();
// 	if (!pApp) return TRUE;

	ASSERT(m_pDocument != NULL);

	if (vp.IsGetting())
	{
// 		FSCRT_VAR params = {FSCRT_VT_LPBOOL, 0, 0};
// 		FSCRT_Var_SetBooleanPtrValue(params, NULL);
// 		FSCRT_VAR retVal = {FSCRT_VT_INT32, 0, 0};
// 		FSCRT_Var_SetInt32Value(retVal, 0);
// 
// 		FS_RESULT ret = pApp->CallFunction(NULL, FSCRT_APPFUNC_GETCHANGEMARK, &params, 1, &retVal);
// 		if (ret != FSCRT_ERRCODE_SUCCESS) return FALSE;
// 
// 		vp << *params.var.boolPtrVar;
		FS_BOOL changeMark = FALSE;
        if(pFormFiller)
            pFormFiller->m_pJSActionHandle->GetChangeMark((FSPDFDoc*)m_pDocument, &changeMark);
		vp << changeMark;
		
	}
	else
	{
		FX_BOOL bChanged = FALSE;
		vp >> bChanged;

		JS_SetChangeMark(bChanged);
	}

	return TRUE;
}

FX_BOOL JDocument::ADBE(OBJ_PROP_PARAMS)
{
	ASSERT(m_pDocument != NULL);

	if (vp.IsGetting())
	{
		vp.SetNull();
	}
	else
	{
	}

	return TRUE;
}

FX_BOOL JDocument::pageNum(OBJ_PROP_PARAMS)
{
	FSFormFillerImp* pFormFiller = m_pDocument->GetForm()->GetFormFiller();
// 	CFSCRT_LTAppHandler* pApp = FSCRT_GetLTAppHandler();
// 	if (!pApp) return FALSE;

	ASSERT(m_pDocument != NULL);

	CFS_Library* pLib = CFS_Library::get();
	if (!pLib) return FALSE;
	FSActionHandler* pActionHandler = pLib->getActionHandler();
	if (!pActionHandler) return FALSE;

	if (vp.IsGetting())
	{
// 		FSCRT_VAR params = {FSCRT_VT_LPINT32, 0, 0};
// 		FSCRT_Var_SetInt32PtrValue(params, NULL);
// 		FSCRT_VAR retVal = {FSCRT_VT_INT32, 0, 0};
// 		FSCRT_Var_SetInt32Value(retVal, 0);
// 
// 		FS_RESULT ret = pApp->CallFunction(NULL, FSCRT_APPFUNC_GETCURPAGEINDEX, &params, 1, &retVal);
// 		if (ret != FSCRT_ERRCODE_SUCCESS) return FALSE;
		FS_INT32 pageIndex = pActionHandler->getCurrentPage((FSPDFDoc*)m_pDocument);
 		vp << pageIndex;
	}
	else
	{
		FX_INT32 pageIndex=0;
		vp >> pageIndex;

// 		FSCRT_VAR params = {FSCRT_VT_INT32, 0, 0};
// 		FSCRT_Var_SetInt32Value(params, pageIndex);
// 		FSCRT_VAR retVal = {FSCRT_VT_INT32, 0, 0};
// 		FSCRT_Var_SetInt32Value(retVal, 0);

//		FS_RESULT ret = pApp->CallFunction(NULL, FSCRT_APPFUNC_SETCURPAGEINDEX, &params, 1, &retVal);
//		FSCRT_StartCallBackState();
		pActionHandler->setCurrentPage((FSPDFDoc*)m_pDocument, pageIndex);
	}

 	return TRUE;
}

FX_BOOL JDocument::ParserParams(JSObject* pObj,CFXJS_AnnotObj& annotobj)
{
	return TRUE;
}

FX_BOOL JDocument::addAnnot(OBJ_METHOD_PARAMS)
{
 	return TRUE;
}

FX_BOOL JDocument::addField(OBJ_METHOD_PARAMS)
{
	ASSERT(m_pDocument != NULL);

	CPDF_Document* pDoc = m_pDocument->getPDFDocument();
	ASSERT(pDoc != NULL);

	FX_DWORD dwPermissions = pDoc->GetUserPermissions();
	if (!((dwPermissions & FPDFPERM_MODIFY) || (dwPermissions & FPDFPERM_ANNOT_FORM)))
		return FALSE;

	FX_INT32 iSize = params.size();
	if (iSize < 4) return FALSE;

	CFX_WideString sName = (FX_LPCWSTR)params[0];
	CFX_WideString sType = (FX_LPCWSTR)params[1];
	FX_INT32 nPageNo = (int)params[2];

	CFXJS_Array aCoords;
	aCoords.Attach(params[3]);
	if(aCoords.GetLength() < 4)
 		return FALSE;

	CPDF_Rect rcCoords;
	CFXJS_Value valCoord;
	aCoords.GetElement(0, valCoord);
	rcCoords.left = valCoord;
	aCoords.GetElement(1, valCoord);
	rcCoords.top = valCoord;
	aCoords.GetElement(2, valCoord);
	rcCoords.right = valCoord;
	aCoords.GetElement(3, valCoord);
	rcCoords.bottom = valCoord;
	rcCoords.Normalize();

	FX_INT32 nFieldType = FIELDTYPE_UNKNOWN;
	if (sType == L"text")
	{
		nFieldType = FIELDTYPE_TEXTFIELD;
	}
	else if (sType == L"button")
	{
		nFieldType = FIELDTYPE_PUSHBUTTON;
	}
	else if (sType == L"combobox")
	{
		nFieldType = FIELDTYPE_COMBOBOX;
	}
	else if (sType == L"listbox")
	{
		nFieldType = FIELDTYPE_LISTBOX;
	}
	else if (sType == L"checkbox")
	{
		nFieldType = FIELDTYPE_CHECKBOX;
	}
	else if (sType == L"radiobutton")
	{
		nFieldType = FIELDTYPE_RADIOBUTTON;
	}
	else if (sType == L"signature")
	{
		//nFieldType = FIELDTYPE_SIGNATURE;
	}

	if (nFieldType == FIELDTYPE_UNKNOWN) return FALSE;

	JField::AddField(m_pDocument, nPageNo, nFieldType, sName, rcCoords);
	
	CFXJS_Context* pContext = (CFXJS_Context*)cc;
	ASSERT(pContext != NULL);

	CFXJS_Runtime* pRuntime = pContext->GetJSRuntime();
	if(!pRuntime) return TRUE;

	JSFXObject * pFieldObj = DS_NewFxDynamicObj(pRuntime, pContext, DS_GetObjDefnID(pRuntime, L"Field"));
	ASSERT(pFieldObj != NULL);
	CFXJS_Field * pJSField = (CFXJS_Field*)DS_GetPrivate(pFieldObj);
	ASSERT(pJSField != NULL);
	JField* pField = (JField *)pJSField->GetEmbedObject();
	ASSERT(pField != NULL);

	pField->AttachField(this, sName);

	vRet = pJSField;
	return TRUE;
}

//exports form fields as a tab-delimited text file to a local hard disk.
//comment: need reader support
//note : watch the third parameter:cPath, for what case it can be safely saved?
//int CPDFSDK_InterForm::ExportAsText(FX_BOOL bNoPassword,StringArray aFields,String cPath);
//return value, int the index of the parameters illegal, the index is based on 1.

FX_BOOL JDocument::exportAsText(OBJ_METHOD_PARAMS)
{
	if (IsSafeMode(cc)) return TRUE;
	return TRUE;
}

void MakePathDirArray(const CFX_WideString& sPathDir, CFX_WideStringArray& sDirArray)
{
	CFX_WideString dir = sPathDir;

	for (int i = 0, sz = dir.GetLength(), prev = 0; i < sz; i++)
	{
		FX_WCHAR ch = dir.GetAt(i);
		if (ch == L'\\' || ch == L'/')
		{
			sDirArray.Add(dir.Mid(prev+1, i - prev));
			prev = i;
		}
	}
}

FX_INT32 GetLastIndex(CFX_WideString& path, FX_WCHAR ch = L'/')
{
	for (int i = path.GetLength(); i > -1; i--)
	{
		if (path.GetAt(i) == ch)
		{
			return i;
		}
	}
	return -1;
}

CFX_WideString MakeRelativePath(const CFX_WideString& sPathFrom, const CFX_WideString& sPathTo)
{
	CFX_WideString sRet = sPathTo;

	CFX_WideString sPathFromTemp = sPathFrom;
	CFX_WideString sPathToTemp = sPathTo;

	sPathFromTemp.TrimLeft();
	sPathToTemp.TrimLeft();

	if (sPathFromTemp[0] == sPathToTemp[0])
	{
		sPathFromTemp.Replace((FX_LPCWSTR)L"\\", (FX_LPCWSTR)L"/");
		sPathToTemp.Replace((FX_LPCWSTR)L"\\", (FX_LPCWSTR)L"/");

		CFX_WideString sPathFromDir = sPathFromTemp.Left(GetLastIndex(sPathFromTemp, L'/'));
		CFX_WideString sPathToDir = sPathToTemp.Left(GetLastIndex(sPathToTemp, L'/'));

		if (sPathFromDir.CompareNoCase(sPathToDir) == 0)
		{
			if (sPathFromDir.IsEmpty() && sPathToDir.IsEmpty())
			{
				return sRet;
			}

			sRet = sPathToTemp.Right(sPathToTemp.GetLength() - GetLastIndex(sPathToTemp) - 1);
		}
		else
		{
			CFX_WideStringArray sPathDirArrayFrom;
			MakePathDirArray(sPathFromDir, sPathDirArrayFrom);

			CFX_WideStringArray sPathDirArrayTo;
			MakePathDirArray(sPathToDir, sPathDirArrayTo);

			sRet.Empty();

			FX_BOOL bSame = TRUE;

			for (FX_INT32 i=0; i<sPathDirArrayFrom.GetSize(); i++)
			{
				if (i >= sPathDirArrayTo.GetSize())
				{
					sRet = L"../" + sRet;
				}
				else
				{
					if (!bSame)
					{
						//For mantis 0064193, modify by xiaolong_fan
						sRet = L"../" + sRet;

						sRet += sPathDirArrayTo[i] + L"/";
					}
					else
					{
						if (sPathDirArrayFrom[i].CompareNoCase(sPathDirArrayTo[i]) == 0)
						{						
						}
						else
						{
							sRet += L"../" + sPathDirArrayTo[i] + L"/";
							bSame = FALSE;
						}
					}
				}
			}

			if (sPathDirArrayFrom.GetSize() < sPathDirArrayTo.GetSize())
			{
				for (FX_INT32 j=sPathDirArrayFrom.GetSize(); j<sPathDirArrayTo.GetSize(); j++)
					sRet += sPathDirArrayTo[j] + L"/";
			}

			sRet += sPathToTemp.Right(sPathToTemp.GetLength() - GetLastIndex(sPathToTemp) - 1);
		}
	}

	return sRet;
}
//exports form fields as a fdf file to the local hard drive
//comment: need reader supports
//note:the last parameter hasn't been confirmed.because the previous one blocks the way.
//int FSPDFDoc::ExportAsFDF(FX_BOOL bAllFields,BOOL bNoPassword,StringArray aFields,FX_BOOL bFlags,String cPath,FX_BOOL bAnnotations);

FX_BOOL JDocument::exportAsFDF(OBJ_METHOD_PARAMS)
{
	if (IsSafeMode(cc)) return TRUE;

	FSFormFillerImp* pFormFiller = m_pDocument->GetForm()->GetFormFiller();
    if(!pFormFiller) return FALSE;
// 	CFSCRT_LTAppHandler* pApp = FSCRT_GetLTAppHandler();
// 	if (!pApp) return TRUE;

	ASSERT(m_pDocument != NULL);
	CPDF_Document* pDoc = m_pDocument->getPDFDocument();
	ASSERT(pDoc != NULL);
	FX_DWORD dwPermissions = pDoc->GetUserPermissions();
	if (!(dwPermissions & FPDFPERM_EXTRACT_ACCESS)) return FALSE;

	FX_BOOL bAllFields = params.size() > 0 ? (FX_BOOL)params[0] : FALSE;
	FX_BOOL bNoPassWord = params.size() > 1 ? (FX_BOOL)params[1] : TRUE;
	FX_BOOL bWhole = params.size() > 2 ? (params[2].GetType() == VT_null) : TRUE;
	CFXJS_Array arrayFileds;
	if (!bWhole)
		arrayFileds.Attach(params[2]);
	FX_BOOL bFlags = params.size() > 3 ? (FX_BOOL)params[3] : FALSE;
    CFX_WideString swFilePath = params.size() > 4 ? (FX_LPCWSTR)params[4] : (FX_LPCWSTR)L"";

	if (swFilePath.IsEmpty())
	{
#if 0
		FSCRT_BSTR bsPath;
		FSCRT_BStr_Init(&bsPath);
// 		FSCRT_VAR inParams[3] = {{FSCRT_VT_BOOL, 0, 0}, {FSCRT_VT_LPVOID, 0, 0},{FSCRT_VT_LPVOID, 0, 0}};
// 		FSCRT_Var_SetBooleanValue(inParams[0], TRUE)
// 		FSCRT_Var_SetVoidPtrValue(inParams[1], &bsFileName)
// 		FSCRT_Var_SetVoidPtrValue(inParams[2], &bsPath)
// 		FSCRT_VAR retVal = {FSCRT_VT_INT32, 0, 0};
// 		FSCRT_Var_SetInt32Value(retVal, 0)
// 		//typedef FS_INT32	(*FSCRT_LPAPPFunc_BrowseFile)(FS_BOOL bSave, const FSCRT_BSTR* initFileName, FSCRT_BSTR* path);
// 		FS_RESULT ret = pApp->CallFunction(NULL, FSCRT_APPFUNC_BROWSEFILE, inParams, 3, &retVal);
		FS_RESULT ret = pFormFiller->m_pJSActionHandle->BrowseFile(&bsPath);
		if (ret != FSCRT_ERRCODE_SUCCESS || FSCRT_BStr_IsEmpty(&bsPath))
		{
			FSCRT_BStr_Clear(&bsPath);
			return TRUE;
		}

		FSCRT_ST_FSUTF8ToFXWStr(&bsPath, swFilePath);
		FSCRT_BStr_Clear(&bsPath);
#endif
	}
	else
	{
		swFilePath = japp::PDFPathToSysPath(swFilePath);
	}

	swFilePath.TrimRight();
	CFX_WideString expectedExt1 = L".fdf";
	CFX_WideString expectedExt2 = L".FDF";
	CFX_WideString externtion = swFilePath.Right(expectedExt1.GetLength());
	if(!externtion.Equal(expectedExt1) && !externtion.Equal(expectedExt2) )
	{
		sError = L"The path doesn't have a .fdf extension";
		//Alert((CFXJS_Context*)cc, sError);
		return FALSE;
	}
	/* why?
	if (!(strType == L"Batch" || strType == L"Console" || strType == L"Menu") && !swFilePath.IsEmpty())
		return FALSE;
		*/
    
	//m_pDocument->SetFocusAnnot(NULL);
   
    FSFormImp* pInterForm= (FSFormImp*)m_pDocument->GetForm();
	ASSERT(pInterForm != NULL);
	CPDF_InterForm* pPDFForm = pInterForm->ST_GetSTInterForm()->m_pInterForm;
	ASSERT(pPDFForm != NULL);

	CFX_PtrArray aFields;
	if (bWhole)
	{
		for (FX_INT32 j=0,jsz=pPDFForm->CountFields(); j<jsz; j++)
		{
			aFields.Add(pPDFForm->GetField(j));
		}
	}
	else
	{
		for (FX_INT32 i=0,isz=arrayFileds.GetLength(); i<isz; i++)
		{
			CFXJS_Value valName;
			arrayFileds.GetElement(i,valName);
			CFX_WideString swName = (FX_LPCWSTR)valName;

			for (int j=0, jsz=pPDFForm->CountFields(swName); j<jsz; j++)
			{
				aFields.Add(pPDFForm->GetField(j, swName));
			}
		}
	}

	CFX_PtrArray fields;
	for (int i=0,sz=aFields.GetSize(); i<sz; i++)
	{
	    CPDF_FormField* pField = (CPDF_FormField*)aFields[i];
		
		if (!bAllFields)
			if (pField->GetValue() == L"")
				continue;

		if (bNoPassWord)
			if (pField->GetFieldFlags() & 0x2000)
				continue;

        fields.Add((void*)pField);
	}    

	FS_BOOL isAbsolutePath = FALSE;
#if _FXM_PLATFORM_ == _FXM_PLATFORM_WINDOWS_
	if (swFilePath.GetAt(1) == L':' && (swFilePath.GetAt(0) > L'a'&& swFilePath.GetAt(0) < L'z' || swFilePath.GetAt(0) > L'A'&& swFilePath.GetAt(0) < L'Z'))
		isAbsolutePath = TRUE;
#else
	if (swFilePath.GetAt(0) == L'/')
		isAbsolutePath = TRUE;
#endif

	CFX_WideString folderPath;


	CFX_WideString wsDocPath;

	JS_docGetPath(wsDocPath);
	if(!isAbsolutePath && !wsDocPath.IsEmpty())
	{
		folderPath = wsDocPath.Mid(0,GetLastIndex(wsDocPath)+1);
	}
	CFX_WideString path = folderPath+ swFilePath;

	CFDF_Document* pFDF = pPDFForm->ExportToFDF(MakeRelativePath(path, wsDocPath),(CFX_PtrArray&)fields, TRUE);
	if (!pFDF) return TRUE;
	FX_BOOL bRet = pFDF->WriteFile(path.UTF8Encode());
	if(!bRet)
	{
		sError = L"Can't create file: ";
		sError += path;
		Alert((CFXJS_Context*)cc, sError);
		return FALSE;
	}
	delete pFDF;

	return bRet;

}

//exports form fields an XFDF file to the local hard drive
//comment: need reder supports
//note:the last parameter can't be test
//int FSPDFDoc::ExportAsXFDF(FX_BOOL bAllFields,FX_BOOL  bNoPassWord,StringArray aFields,String cPath,FX_BOOL bAnnoatations);

FX_BOOL JDocument::exportAsXFDF(OBJ_METHOD_PARAMS)
{
	if (IsSafeMode(cc)) return TRUE;

	ASSERT(m_pDocument != NULL);
	CPDF_Document* pDoc = m_pDocument->getPDFDocument();
	ASSERT(pDoc != NULL);
	FX_DWORD dwPermissions = pDoc->GetUserPermissions();
	if (!(dwPermissions & FPDFPERM_EXTRACT_ACCESS)) return FALSE;

	return TRUE;
}

//Maps a field object in PDF document to a JavaScript variable
//comment:
//note: the paremter cName, this is clue how to treat if the cName is not a valiable filed name in this document

FX_BOOL JDocument::getField(OBJ_METHOD_PARAMS)
{
	ASSERT(m_pDocument != NULL);

	if (params.size() < 1) return FALSE;

	CFX_WideString wideName = (FX_LPCWSTR)params[0];

	FSFormImp* pInterForm= (FSFormImp*)m_pDocument->GetForm();
	ASSERT(pInterForm != NULL);
	CPDF_InterForm* pPDFForm = pInterForm->ST_GetSTInterForm()->m_pInterForm;
	ASSERT(pPDFForm != NULL);

	if (pPDFForm->CountFields(wideName) <= 0) 
	{
		vRet.SetNull();
		return TRUE;
	}

	CFXJS_Context* pContext = (CFXJS_Context*)cc;
	ASSERT(pContext != NULL);
	CFXJS_Runtime* pRuntime = pContext->GetJSRuntime();
	if(!pRuntime) return TRUE;

	JSFXObject * pFieldObj = DS_NewFxDynamicObj(pRuntime, pContext, DS_GetObjDefnID(pRuntime, L"Field"));
	ASSERT(pFieldObj != NULL);

	CFXJS_Field * pJSField = (CFXJS_Field*)DS_GetPrivate(pFieldObj);
	ASSERT(pJSField != NULL);

	JField * pField = (JField *)pJSField->GetEmbedObject(); 
	ASSERT(pField != NULL);

	pField->AttachField(this, wideName);
	vRet = pJSField;

	return TRUE;
}

//Gets the name of the nth field in the document 
//comment:
//note: the parameter nIndex, if it is not available

FX_BOOL JDocument::getNthFieldName(OBJ_METHOD_PARAMS)
{
	ASSERT(m_pDocument != NULL);

	int nIndex = params.size() > 0 ? (int)params[0] : -1;
	if (nIndex == -1) return FALSE;

	FSFormImp* pInterForm= (FSFormImp*)m_pDocument->GetForm();
	ASSERT(pInterForm != NULL);
	CPDF_InterForm* pPDFForm = pInterForm->ST_GetSTInterForm()->m_pInterForm;
	ASSERT(pPDFForm != NULL);

	CPDF_FormField* pField = pPDFForm->GetField(nIndex);
	if (!pField)
		return FALSE;

	vRet = pField->GetFullName();
	return TRUE;	
}

//imports the specified fdf file.
//comments: need reader suppport
//note:once the cpath is illigl  then a file dialog box pops up in order to ask user to chooose the file
//int FSPDFDoc::importAnFDF(String cPath);

FX_BOOL JDocument::importAnFDF(OBJ_METHOD_PARAMS)
{
	if (IsSafeMode(cc)) return TRUE;
	ASSERT(m_pDocument != NULL);
	CPDF_Document* pDoc = m_pDocument->getPDFDocument();
	ASSERT(pDoc != NULL);

	FX_DWORD dwPermissions = pDoc->GetUserPermissions();
	if (!((dwPermissions & FPDFPERM_MODIFY) || (dwPermissions & FPDFPERM_ANNOT_FORM) 
		|| (dwPermissions & FPDFPERM_FILL_FORM)))
		return FALSE;

	FSFormFillerImp* pFormFiller = m_pDocument->GetForm()->GetFormFiller();
    if(!pFormFiller) return FALSE;
// 	CFSCRT_LTAppHandler* pApp = FSCRT_GetLTAppHandler();
// 	if (!pApp) return TRUE;

	CFX_WideString swPath;
	if (params.size() > 0)
		swPath = params[0];
    
	if (swPath.IsEmpty())
	{
#if 0
		FSCRT_BSTR bsFileName;
		FSCRT_BStr_Init(&bsFileName);
		FSCRT_BSTR bsPath;
		FSCRT_BStr_Init(&bsPath);
// 		FSCRT_VAR inParams[3] = {{FSCRT_VT_BOOL, 0, 0}, {FSCRT_VT_LPVOID, 0, 0},{FSCRT_VT_LPVOID, 0, 0}};
// 		FSCRT_Var_SetBooleanValue(inParams[0], TRUE)
// 		FSCRT_Var_SetVoidPtrValue(inParams[1], &bsFileName)
// 		FSCRT_Var_SetVoidPtrValue(inParams[2], &bsPath)
// 		FSCRT_VAR retVal = {FSCRT_VT_INT32, 0, 0};
// 		FSCRT_Var_SetInt32Value(retVal, 0)
// 		//typedef FS_INT32	(*FSCRT_LPAPPFunc_BrowseFile)(FS_BOOL bSave, const FSCRT_BSTR* initFileName, FSCRT_BSTR* path);
// 		FS_RESULT ret = pApp->CallFunction(NULL, FSCRT_APPFUNC_BROWSEFILE, inParams, 3, &retVal);
		FS_RESULT ret = pFormFiller->m_pJSActionHandle->BrowseFile(&bsPath);
		if (ret != FSCRT_ERRCODE_SUCCESS || FSCRT_BStr_IsEmpty(&bsPath))
		{
			FSCRT_BStr_Clear(&bsFileName);
			FSCRT_BStr_Clear(&bsPath);
			return TRUE;
		}

		FSCRT_ST_FSUTF8ToFXWStr(&bsPath, swPath);
		FSCRT_BStr_Clear(&bsFileName);
		FSCRT_BStr_Clear(&bsPath);
#endif
	}
	else
	{
		swPath = japp::PDFPathToSysPath(swPath);
	}

	//m_pDocument->SetFocusAnnot(NULL);

	FSFormImp* pInterForm= (FSFormImp*)m_pDocument->GetForm();
	ASSERT(pInterForm != NULL);
	CPDF_InterForm* pPDFForm = pInterForm->ST_GetSTInterForm()->m_pInterForm;
	ASSERT(pPDFForm != NULL);

	CFDF_Document* pFDF = CFDF_Document::ParseFile(swPath.UTF8Encode());
	FX_BOOL bRet = pPDFForm->ImportFromFDF(pFDF, TRUE);
	delete pFDF;
	if (!bRet)  return FALSE;
	
	JS_SetChangeMark(TRUE);

	return TRUE;
}

//imports and specified XFDF file containing XML form data
//comment: need reader supports
//note: same as up
//int FSPDFDoc::importAnFDF(String cPath)

FX_BOOL JDocument::importAnXFDF(OBJ_METHOD_PARAMS)
{
	if (IsSafeMode(cc)) return TRUE;

	ASSERT(m_pDocument != NULL);
	CPDF_Document* pDoc = m_pDocument->getPDFDocument();
	ASSERT(pDoc != NULL);

	FX_DWORD dwPermissions = pDoc->GetUserPermissions();
	if (!((dwPermissions & FPDFPERM_MODIFY) || (dwPermissions & FPDFPERM_ANNOT_FORM) 
		|| (dwPermissions & FPDFPERM_FILL_FORM)))
		return FALSE;

	return TRUE;
}

//imports and specified text file 
//commnet: need reader supports
//note: same as up,when nRow is not rational,adobe is dumb for it.
//int FSPDFDoc::importTextData(String cPath,int nRow);

FX_BOOL JDocument::importTextData(OBJ_METHOD_PARAMS)
{
	if (IsSafeMode(cc)) return TRUE;
	ASSERT(m_pDocument != NULL);
	CPDF_Document* pDoc = m_pDocument->getPDFDocument();
	ASSERT(pDoc != NULL);

	FX_DWORD dwPermissions = pDoc->GetUserPermissions();
	if (!((dwPermissions & FPDFPERM_MODIFY) || (dwPermissions & FPDFPERM_ANNOT_FORM) 
		|| (dwPermissions & FPDFPERM_FILL_FORM)))
		return FALSE;

	return TRUE;
}

//exports the form data and mails the resulting fdf file as an attachment to all recipients.
//comment: need reader supports
//note:
//int FSPDFDoc::mailForm(FX_BOOL bUI,String cto,string ccc,string cbcc,string cSubject,string cms);

FX_BOOL JDocument::mailForm(OBJ_METHOD_PARAMS)
{
	ASSERT(m_pDocument != NULL);
	CPDF_Document* pDoc = m_pDocument->getPDFDocument();
	ASSERT(pDoc != NULL);

	FX_DWORD dwPermissions = pDoc->GetUserPermissions();
	if (!(dwPermissions & FPDFPERM_EXTRACT_ACCESS)) return FALSE;

	FSFormFillerImp* pFormFiller = m_pDocument->GetForm()->GetFormFiller();
    if(!pFormFiller) return FALSE;
// 	CFSCRT_LTAppHandler* pApp = FSCRT_GetLTAppHandler();
// 	if (!pApp) return TRUE;

	FX_INT32 iLength = params.size();

	FX_BOOL bUI = iLength > 0 ? (FX_BOOL)params[0] : TRUE;
	CFX_WideString cTo = iLength > 1 ? (FX_LPCWSTR)params[1] : (FX_LPCWSTR)L"";
	CFX_WideString cCc = iLength > 2 ? (FX_LPCWSTR)params[2] : (FX_LPCWSTR)L"";
	CFX_WideString cBcc = iLength > 3 ? (FX_LPCWSTR)params[3] : (FX_LPCWSTR)L"";
	CFX_WideString cSubject = iLength > 4 ? (FX_LPCWSTR)params[4] : (FX_LPCWSTR)L"";
	CFX_WideString cMsg = iLength > 5 ? (FX_LPCWSTR)params[5] : (FX_LPCWSTR)L"";

	FSFormImp* pInterForm= (FSFormImp*)m_pDocument->GetForm();
	ASSERT(pInterForm != NULL);
	CPDF_InterForm* pPDFForm = pInterForm->ST_GetSTInterForm()->m_pInterForm;
	ASSERT(pPDFForm != NULL);

	CFX_WideString wsDocPath;
	JS_docGetPath(wsDocPath);
	CFDF_Document* pFDF = pPDFForm->ExportToFDF(wsDocPath);
	if (!pFDF) return FALSE;
	CFX_ByteTextBuf textBuf;
	FX_BOOL bRet = pFDF->WriteBuf(textBuf);
	delete pFDF;
	if (!bRet) return FALSE;

	CFXJS_Context* pContext = (CFXJS_Context*)cc;
	ASSERT(pContext != NULL);
	CFXJS_Runtime* pRuntime = pContext->GetJSRuntime();
	if(!pRuntime) return TRUE;

	pRuntime->BeginBlock();
	JS_docmailForm(textBuf.GetBuffer(), textBuf.GetLength(), bUI, (FX_LPCWSTR)cTo, (FX_LPCWSTR)cSubject, (FX_LPCWSTR)cCc, (FX_LPCWSTR)cBcc, (FX_LPCWSTR)cMsg);
	pRuntime->EndBlock();
 	return TRUE;
}

FX_BOOL JDocument::print(OBJ_METHOD_PARAMS)
{
	CFXJS_Context* pContext = (CFXJS_Context*)cc;
	ASSERT(pContext != NULL);
	CFXJS_Runtime* pRuntime = pContext->GetJSRuntime();
	if(!pRuntime) return TRUE;

	FX_BOOL bUI = TRUE;
	int nStart = 0;
	int nEnd = 0;
	FX_BOOL bSilent = FALSE;
	FX_BOOL bShrinkToFit = FALSE;
	FX_BOOL bPrintAsImage = FALSE;
	FX_BOOL bReverse = FALSE;
	FX_BOOL bAnnotations = FALSE;


	int nlength = params.size();
	if(nlength ==9)
	{
		if (params[8].GetType() == VT_fxobject)
		{
			if (JSFXObject* pObj = (JSFXObject*)params[8])
			{
				if (DS_GetObjDefnID(pObj) == DS_GetObjDefnID(pRuntime, L"PrintParamsObj"))
				{
					if (CFXJS_Object* pJSObj = (CFXJS_Object*)params[8])
					{
						if (FXPrintParamsObj* pprintparamsObj = (FXPrintParamsObj*)pJSObj->GetEmbedObject())
						{
							bUI = pprintparamsObj->bUI;
							nStart = pprintparamsObj->nStart;
							nEnd = pprintparamsObj->nEnd;
							bSilent = pprintparamsObj->bSilent;
							bShrinkToFit = pprintparamsObj->bShrinkToFit;
							bPrintAsImage = pprintparamsObj->bPrintAsImage;
							bReverse = pprintparamsObj->bReverse;
							bAnnotations = pprintparamsObj->bAnnotations;
						}
					}
				}
			}	
		}
	}
	else
	{
		if(nlength >= 1)
			bUI = params[0];
		if(nlength >= 2)
			nStart = (int)params[1];
		if(nlength >= 3)
			nEnd = (int)params[2];
		if(nlength >= 4)
			bSilent = params[3];
		if(nlength >= 5)
			bShrinkToFit = params[4];
		if(nlength >= 6)
			bPrintAsImage = params[5];
		if(nlength >= 7)
			bReverse = params[6];
		if(nlength >= 8)
			bAnnotations = params[7];
	}

	ASSERT(m_pDocument != NULL);

	FSFormFillerImp* pFormFiller = m_pDocument->GetForm()->GetFormFiller();
    if(!pFormFiller) return FALSE;
	pFormFiller->m_pJSActionHandle->Print((FSPDFDoc*)m_pDocument, bUI, nStart, nEnd, bSilent, bShrinkToFit, bPrintAsImage, bReverse, bAnnotations);

	return TRUE;
}

void deleteAnnot(CPDF_FormField* pField, FS_INT32 nControlIndex, CPDF_Document* pDoc, FSPDFDoc* pDocument, FSFormFillerImp* pFormFiller)
{
	CPDF_FormControl* pControl = pField->GetControl(nControlIndex);
	CPDF_Dictionary* pControlDict = pControl->GetWidget();
	ASSERT(pControlDict != NULL);

	FS_INT32 nPageIndex = -1;
	FSPDFPage* pPage = NULL;

	if (CPDF_Dictionary* pPageDict = pControlDict->GetDict("P"))
		nPageIndex = pDoc->GetPageIndex(pPageDict->GetObjNum());
	else 
	{
		for (int i=0,sz=pDoc->GetPageCount(); i<sz; i++)
		{
			if (CPDF_Dictionary* pPageDict = pDoc->GetPage(i))
			{			
				if (CPDF_Array* pAnnots = pPageDict->GetArray("Annots"))
				{
					for (int j=0,jsz=pAnnots->GetCount(); j<jsz; j++)
					{
						CPDF_Object* pDict = pAnnots->GetElementValue(j);
						if (pControlDict == pDict)
						{
							nPageIndex = i;
							break;
						}
					}
				}
			}
			if (nPageIndex != -1)break;
		}
	}

	if (nPageIndex >= 0)
	{
		pPage = pDocument->getPage(nPageIndex);
		CFX_FloatRect annotRect = pControl->GetRect();
		FSRectF rtAnnotRect = {annotRect.left, annotRect.top, annotRect.right, annotRect.bottom};
		((FSPDFPageImp*)pPage)->removeAnnotByObjNum(pControlDict->GetObjNum());
        
        if(pFormFiller)
        {
            FSFormFillerAssist* pFormFillAssist = pFormFiller->GetFormFillerAssist();
            if(pFormFillAssist)
                pFormFillAssist->refresh(pPage, &rtAnnotRect);
        }
	}

}

//removes the specified field from the document.
//comment:
//note: if the filed name is not retional, adobe is dumb for it.

FX_BOOL JDocument::removeField(OBJ_METHOD_PARAMS)
{
	ASSERT(m_pDocument != NULL);
	CPDF_Document* pDoc = m_pDocument->getPDFDocument();
	ASSERT(pDoc != NULL);

	FX_DWORD dwPermissions = pDoc->GetUserPermissions();
	if (!((dwPermissions & FPDFPERM_MODIFY) || (dwPermissions & FPDFPERM_ANNOT_FORM)))
		return FALSE;

	if (params.size() < 1)
		return TRUE;

	CFX_WideString sFieldName = (FX_LPCWSTR)params[0];

	FSFormImp* pInterForm= (FSFormImp*)m_pDocument->GetForm();
	FSFormFillerImp* pFormFiller = pInterForm->GetFormFiller();
    if(!pFormFiller) return FALSE;

	ASSERT(pInterForm != NULL);
	CPDF_InterForm* pPDFForm = pInterForm->ST_GetSTInterForm()->m_pInterForm;
	ASSERT(pPDFForm != NULL);

	CFX_PtrArray widgets;

	FX_INT32 nFieldNum = pPDFForm->CountFields(sFieldName);

	if (nFieldNum > 0)
	{
		for (FX_INT32 nFieldIndex=nFieldNum - 1; nFieldIndex > -1; nFieldIndex --)
		{
			CPDF_FormField* pField = pPDFForm->GetField(nFieldIndex, sFieldName);

			FX_INT32 nControlNum = pField->CountControls();
			for (FX_INT32 nControlIndex=nControlNum-1; nControlIndex>-1; nControlIndex--)
				deleteAnnot(pField, nControlIndex, pDoc, m_pDocument, pFormFiller);
			pPDFForm->DeleteField(pField);
		}
	}

	return TRUE;
}

//reset filed values within a document.
//comment:
//note: if the fields names r not rational, aodbe is dumb for it.

FX_BOOL JDocument::resetForm(OBJ_METHOD_PARAMS)
{
	ASSERT(m_pDocument != NULL);

	CPDF_Document* pDoc = m_pDocument->getPDFDocument();
	ASSERT(pDoc != NULL);

	FX_DWORD dwPermissions = pDoc->GetUserPermissions();
	if (!((dwPermissions & FPDFPERM_MODIFY) || (dwPermissions & FPDFPERM_ANNOT_FORM) 
		|| (dwPermissions & FPDFPERM_FILL_FORM))) return FALSE;

	FSFormFillerImp* pFormFiller = m_pDocument->GetForm()->GetFormFiller();
    if(!pFormFiller) return FALSE;
// 	CFSCRT_LTAppHandler* pApp = FSCRT_GetLTAppHandler();
// 	if (!pApp) return TRUE;

	FSFormImp* pInterForm= (FSFormImp*)m_pDocument->GetForm();
	ASSERT(pInterForm != NULL);
	CPDF_InterForm* pPDFForm = pInterForm->ST_GetSTInterForm()->m_pInterForm;
	ASSERT(pPDFForm != NULL);

	CFXJS_Array aName;
	if (params.size() > 0)
	{
		switch (params[0].GetType())
		{
		default:
			aName.Attach(params[0]);
			break;
		case VT_string:
			aName.SetElement(0,params[0]);
			break;
		}

		CFX_PtrArray aFields;

		for (int i=0,isz=aName.GetLength(); i<isz; i++)
		{
			CFXJS_Value valElement;
			aName.GetElement(i,valElement);
			CFX_WideString swVal = (FX_LPCWSTR)valElement;	
			
			for (int j=0,jsz=pPDFForm->CountFields(swVal); j<jsz; j++)
			{
				aFields.Add((void*)pPDFForm->GetField(j,swVal));
			}		
		}

		if (aFields.GetSize() > 0)
		{
 			pPDFForm->ResetForm(aFields, TRUE, TRUE);
			JS_SetChangeMark(TRUE);
		}
	}
	else
	{
 		pPDFForm->ResetForm(TRUE);
		JS_SetChangeMark(TRUE);
	}

	return TRUE;
}


FX_BOOL JDocument::saveAs(OBJ_METHOD_PARAMS)
{
	if (IsSafeMode(cc)) return TRUE;
	ASSERT(m_pDocument != NULL);

	CFX_WideString sPath, sConvID, sFS;
	bool bCopy = false;
	bool bPrompt = false;

	switch (params.size())
	{
	case 5:
		bPrompt = params[4];
	case 4:
		bCopy = params[3];
	case 3:
		sFS = params[2];
	case 2:
		sConvID = params[1];
	case 1:
		sPath = params[0];
		break;
	}

	sPath = japp::PDFPathToSysPath(sPath);

	//m_pDocument->SaveAs(sPath);

	return TRUE;
}
//************************************************************************
//*  Submit Form Data                                                                   
//************************************************************************
void JDocument::JS_docGetPath(CFX_WideString& wsDocPath)
{
	FSFormFillerImp* pFormFiller = m_pDocument->GetForm()->GetFormFiller();
    if(!pFormFiller) return;
// 	CFSCRT_LTAppHandler* pApp = FSCRT_GetLTAppHandler();
// 	if (!pApp) return;
#if 0
	FSCRT_BSTR bsDocPath;
	FSCRT_BStr_Init(&bsDocPath);
// 	FSCRT_VAR inParam = {FSCRT_VT_LPVOID, 0, 0};
// 	FSCRT_Var_SetVoidPtrValue(inParam, &bsDocPath);
// 	FSCRT_VAR retVal = {FSCRT_VT_INT32, 0, 0};
// 	FSCRT_Var_SetInt32Value(retVal, 0);
// 	//typedef FS_INT32	(*FSCRT_LPAPPFunc_GetDocPath)(const FSCRT_BSTR* filePath);
// 	pApp->CallFunction(NULL, FSCRT_APPFUNC_GETFILEPATH, &inParam, 1, &retVal);
	pFormFiller->m_pJSActionHandle->GetFilePath((FSPDFDoc*)m_pDocument, &bsDocPath);

	FSCRT_ST_FSUTF8ToFXWStr(&bsDocPath, wsDocPath);
	FSCRT_BStr_Clear(&bsDocPath);
#endif
}

FX_BOOL JDocument::JS_docSubmitForm(void* formData, FX_INT32 length, FX_LPCWSTR URL)
{
#if 0
	FSFormFillerImp* pFormFiller = m_pDocument->GetForm()->GetFormFiller();
// 	CFSCRT_LTAppHandler* pApp = FSCRT_GetLTAppHandler();
// 	if (!pApp) return FALSE;

	CFX_WideString wsURL(URL);
	CFX_ByteString utfURL = wsURL.UTF8Encode();
	FSCRT_BSTR bsURL = { (FS_LPSTR)(FX_LPCSTR)(utfURL), (FS_DWORD)utfURL.GetLength()};
// 	FSCRT_VAR inParams[3] = {{FSCRT_VT_LPVOID, 0, 0}, {FSCRT_VT_DWORD, 0, 0},{FSCRT_VT_LPVOID, 0, 0}};
// 	FSCRT_Var_SetVoidPtrValue(inParams[0], formData);
// 	FSCRT_Var_SetUInt32Value(inParams[1], length);
// 	FSCRT_Var_SetVoidPtrValue(inParams[2], &bsURL);
// 	FSCRT_VAR retVal = {FSCRT_VT_EMPTY, 0, 0};
	//typedef void		(*FSCRT_LPAPPFunc_SubmitForm)(FS_LPVOID formData, FS_DWORD length, const FSCRT_BSTR* url);
	//pApp->CallFunction(NULL, FSCRT_APPFUNC_SUBMITFORM, inParams, 3, &retVal);
	pFormFiller->m_pJSActionHandle->SubmitForm((FSPDFDoc*)m_pDocument, formData, length, &bsURL);
	//FSCRT_BStr_Clear(&bsURL);
#endif
	return TRUE;
}

FX_BOOL JDocument::FDFToURLEncodedData(FX_LPBYTE& pBuf, FX_STRSIZE& nBufSize)
{
	CFDF_Document* pFDF = CFDF_Document::ParseMemory(pBuf, nBufSize);
	if (pFDF)
	{
		CPDF_Dictionary* pMainDict = pFDF->GetRoot()->GetDict("FDF");
		if (pMainDict == NULL) return FALSE;

		// Get fields
		CPDF_Array* pFields = pMainDict->GetArray("Fields");
		if (pFields == NULL) return FALSE;

		CFX_ByteTextBuf fdfEncodedData;

		for (FX_DWORD i = 0; i < pFields->GetCount(); i ++) 
		{
			CPDF_Dictionary* pField = pFields->GetDict(i);
			if (pField == NULL) continue;
			CFX_WideString name;
			name = pField->GetUnicodeText("T");
			CFX_ByteString name_b = CFX_ByteString::FromUnicode(name);
			CFX_ByteString csBValue = pField->GetString("V");
			CFX_WideString csWValue = PDF_DecodeText(csBValue);
			CFX_ByteString csValue_b = CFX_ByteString::FromUnicode(csWValue);

			fdfEncodedData = fdfEncodedData<<(FX_LPCSTR)name_b;
			fdfEncodedData = fdfEncodedData<<"=";
			fdfEncodedData = fdfEncodedData<<(FX_LPCSTR)csValue_b;
			if(i != pFields->GetCount()-1)
				fdfEncodedData = fdfEncodedData<<"&";
		}

		nBufSize = fdfEncodedData.GetLength();
		pBuf = FX_Alloc(FX_BYTE, nBufSize);
		if(!pBuf)
			return FALSE;
		FXSYS_memcpy(pBuf, fdfEncodedData.GetBuffer(), nBufSize);

	}
	return TRUE;
}

FX_BOOL JDocument::ExportFieldsToFDFTextBuf(const CFX_PtrArray& fields,FX_BOOL bIncludeOrExclude, CFX_ByteTextBuf& textBuf)
{
	FSFormImp* pInterForm = (FSFormImp*)m_pDocument->GetForm();
	ASSERT(pInterForm != NULL);
	CPDF_InterForm* pPDFForm = pInterForm->ST_GetSTInterForm()->m_pInterForm;
	ASSERT(pPDFForm != NULL);

	CFX_WideString wsDocPath;
	JS_docGetPath(wsDocPath);
	CFDF_Document* pFDF = pPDFForm->ExportToFDF(wsDocPath,(CFX_PtrArray&)fields, bIncludeOrExclude);
	if (!pFDF) return FALSE;
	FX_BOOL bRet = pFDF->WriteBuf(textBuf);
	delete pFDF;

	return bRet;
}

FX_BOOL JDocument::SubmitFields(const CFX_WideString& csDestination, const CFX_PtrArray& fields,
	FX_BOOL bIncludeOrExclude, FX_BOOL bUrlEncoded)
{
	CFX_ByteTextBuf textBuf;
	ExportFieldsToFDFTextBuf(fields, bIncludeOrExclude, textBuf);

	FX_LPBYTE pBuffer = textBuf.GetBuffer();
	FX_STRSIZE nBufSize = textBuf.GetLength();

	if (bUrlEncoded)
	{
		if(!FDFToURLEncodedData(pBuffer, nBufSize))
			return FALSE;
	}

	JS_docSubmitForm(pBuffer, nBufSize, (FX_LPCWSTR)csDestination);

	if (bUrlEncoded && pBuffer)
	{
		FX_Free(pBuffer);
		pBuffer = NULL;	
	}

	return TRUE;
}

FX_BOOL JDocument::SubmitForm(const CFX_WideString& sDestination, FX_BOOL bUrlEncoded)
{
	if (sDestination.IsEmpty()) return FALSE;
// 	CFSCRT_LTAppHandler* pApp = FSCRT_GetLTAppHandler();
// 	if (!pApp) return FALSE;
	FSFormFillerImp* pFormFiller = m_pDocument->GetForm()->GetFormFiller();
    if(!pFormFiller) return FALSE;

	FSFormImp* pInterForm= (FSFormImp*)m_pDocument->GetForm();
	ASSERT(pInterForm != NULL);
	CPDF_InterForm* pPDFForm = pInterForm->ST_GetSTInterForm()->m_pInterForm;
	ASSERT(pPDFForm != NULL);

	CFX_WideString wsDocPath;
	JS_docGetPath(wsDocPath);

	CFDF_Document* pFDFDoc = pPDFForm->ExportToFDF(wsDocPath);
	if (NULL == pFDFDoc) return FALSE;

	CFX_ByteTextBuf FdfBuffer;
	FX_BOOL bRet = pFDFDoc->WriteBuf(FdfBuffer);
	delete pFDFDoc;
	if (!bRet) return FALSE;

	FX_LPBYTE pBuffer = FdfBuffer.GetBuffer();
	FX_STRSIZE nBufSize = FdfBuffer.GetLength();

	if (bUrlEncoded)
	{
		if(!FDFToURLEncodedData(pBuffer, nBufSize))
			return FALSE;
	}

	JS_docSubmitForm(pBuffer, nBufSize, (FX_LPCWSTR)sDestination);

	if (bUrlEncoded && pBuffer)
	{
		FX_Free(pBuffer);
		pBuffer = NULL;	
	}

	return TRUE;
}

//参数众多，需要进一步支持！
FX_BOOL JDocument::submitForm(OBJ_METHOD_PARAMS)
{
	ASSERT(m_pDocument != NULL);

	FX_INT32 nSize = params.size();
	if (nSize < 1) return FALSE;

	CFX_WideString strURL;
	FX_BOOL bFDF = TRUE;
	FX_BOOL bEmpty = FALSE;
	CFXJS_Array aFields;

	CFXJS_Value v = params[0];
	if (v.GetType() == VT_string)
	{
		strURL = (FX_LPCWSTR)params[0];
		if (nSize > 1)
			bFDF = params[1];
		if (nSize > 2)
			bEmpty = params[2];
		if (nSize > 3)
			aFields.Attach(params[3]);
	}
	else if (v.GetType() == VT_object)
	{
		JSObject* pObj = (JSObject*)params[0];
		Value* pValue = DS_GetObjectElement(pObj, L"cURL");
		if (pValue)
			strURL = CFXJS_Value(pValue,GET_VALUE_TYPE(pValue));
		if (pValue = DS_GetObjectElement(pObj, L"bFDF"))
			bFDF = CFXJS_Value(pValue, GET_VALUE_TYPE(pValue));
		if (pValue = DS_GetObjectElement(pObj, L"bEmpty"))
			bEmpty = CFXJS_Value(pValue, GET_VALUE_TYPE(pValue));
		if (pValue = DS_GetObjectElement(pObj,L"aFields"))
			aFields.Attach(CFXJS_Value(pValue, GET_VALUE_TYPE(pValue)));
	}		

	FSFormImp* pInterForm= (FSFormImp*)m_pDocument->GetForm();
	ASSERT(pInterForm != NULL);
	CPDF_InterForm* pPDFForm = pInterForm->ST_GetSTInterForm()->m_pInterForm;
	ASSERT(pPDFForm != NULL);

	FX_BOOL bAll = (aFields.GetLength() == 0);

	if (bAll && bEmpty)
	{
		CFXJS_Context* pContext = (CFXJS_Context*)cc;
		ASSERT(pContext != NULL);
		CFXJS_Runtime* pRuntime = pContext->GetJSRuntime();
		if(!pRuntime) return TRUE;

		FX_BOOL bSucceed = FALSE;
		if (CPDF_FormField* pFormField = pPDFForm->CheckRequiredFields())
		{
			pRuntime->BeginBlock();
			bSucceed = SubmitForm(strURL, FALSE);
			pRuntime->EndBlock();
		}
		return TRUE;
	}
	else
	{	
		CFX_PtrArray fieldObjects;

		for (int i=0,sz=aFields.GetLength(); i<sz; i++)
		{
			CFXJS_Value valName;
			aFields.GetElement(i, valName);
			CFX_WideString sName = (FX_LPCWSTR)valName;

			for (int j=0, jsz=pPDFForm->CountFields(sName); j<jsz; j++)
			{
				CPDF_FormField* pField = pPDFForm->GetField(j, sName);
				if (!bEmpty && pField->GetValue().IsEmpty())
					continue;

				fieldObjects.Add(pField);
			}
		}

		CFXJS_Context* pContext = (CFXJS_Context*)cc;
		ASSERT(pContext != NULL);
		CFXJS_Runtime* pRuntime = pContext->GetJSRuntime();
		if(!pRuntime) return TRUE;

		FX_BOOL bSucceed = FALSE;
		//It looks like we don't need to check this, phantom and adobe would still trigger the submit action in this case.
		//if (CPDF_FormField* pFormField = pPDFForm->CheckRequiredFields(&fieldObjects, TRUE))
		{
			pRuntime->BeginBlock();
			FX_BOOL bSucceed = SubmitFields(strURL, fieldObjects, TRUE, !bFDF);
			pRuntime->EndBlock();
		}

		return TRUE;
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////////

void JDocument::AttachDoc(FSPDFDoc* pDoc)
{
	m_pDocument = (FSPDFDocImp*)pDoc;
}

FSPDFDoc* JDocument::GetReaderDoc()
{
	return m_pDocument;
}

FX_BOOL JDocument::ExtractFileName(FSPDFDoc* pDoc,CFX_ByteString &strFileName)
{
	return FALSE;
}

FX_BOOL JDocument::ExtractFolderName(FSPDFDoc* pDoc,CFX_ByteString &strFolderName)
{
	return FALSE;
}

FX_BOOL JDocument::bookmarkRoot(OBJ_PROP_PARAMS)
{
	return TRUE;
}

//************************************************************************
//*  mail Doc                                                                   
//************************************************************************
FX_BOOL JDocument::JS_docmailForm(void* mailData, FX_INT32 length, FS_BOOL bUI,FX_LPCWSTR To, FX_LPCWSTR Subject, 
	FX_LPCWSTR CC, FX_LPCWSTR BCC, FX_LPCWSTR Msg)
{
// 	CFSCRT_LTAppHandler* pApp = FSCRT_GetLTAppHandler();
// 	if (!pApp) return FALSE;
	FSFormFillerImp* pFormFiller = m_pDocument->GetForm()->GetFormFiller();
    if(!pFormFiller) return FALSE;

	CFX_WideString cTo(To);
	CFX_WideString cCc(CC);
	CFX_WideString cBcc(BCC);
	CFX_WideString cSubject(Subject);
	CFX_WideString cMsg(Msg);

	CFX_ByteString utfTo = cTo.UTF8Encode();
	CFX_ByteString utfcCc = cCc.UTF8Encode();
	CFX_ByteString utfcBcc = cBcc.UTF8Encode();
	CFX_ByteString utfcSubject = cSubject.UTF8Encode();
	CFX_ByteString utfMsg = cMsg.UTF8Encode();
#if 0
	FSCRT_BSTR sTo = {(FS_LPSTR)(FX_LPCSTR)utfTo, (FS_DWORD)utfTo.GetLength()};
	FSCRT_BSTR sCc = {(FS_LPSTR)(FX_LPCSTR)utfcCc, (FS_DWORD)utfcCc.GetLength()};
	FSCRT_BSTR sBcc = { (FS_LPSTR)(FX_LPCSTR)utfcBcc, (FS_DWORD)utfcBcc.GetLength()};
	FSCRT_BSTR sSubject = { (FS_LPSTR)(FX_LPCSTR)utfcSubject, (FS_DWORD)utfcSubject.GetLength()};
	FSCRT_BSTR sMsg = { (FS_LPSTR)(FX_LPCSTR)utfMsg, (FS_DWORD)utfMsg.GetLength()};
// 	FSCRT_VAR inParams[8] = {{FSCRT_VT_LPVOID, 0, 0}, {FSCRT_VT_LPVOID, 0, 0},{FSCRT_VT_LPVOID, 0, 0},{FSCRT_VT_LPVOID, 0, 0},{FSCRT_VT_LPVOID, 0, 0},{FSCRT_VT_LPVOID, 0, NULL},
// 	{FSCRT_VT_DWORD, 0, 0}, {FSCRT_VT_BOOL, 0, bUI}};
// 	FSCRT_Var_SetVoidPtrValue(inParams[0], &sTo);
// 	FSCRT_Var_SetVoidPtrValue(inParams[1], &sCc);
// 	FSCRT_Var_SetVoidPtrValue(inParams[2], &sBcc);
// 	FSCRT_Var_SetVoidPtrValue(inParams[3], &sSubject);
// 	FSCRT_Var_SetVoidPtrValue(inParams[4], &sMsg);
// 	FSCRT_Var_SetVoidPtrValue(inParams[5], mailData);
// 	FSCRT_Var_SetUInt32Value(inParams[6], length);
// 	FSCRT_Var_SetBooleanValue(inParams[7], bUI);
// 	FSCRT_VAR retVal2 = {FSCRT_VT_EMPTY, 0, 0};
// 	//void (*FSCRT_LPAPPFunc_Mail)(const FSCRT_BSTR* to, const FSCRT_BSTR* cc, const FSCRT_BSTR* bcc, const FSCRT_BSTR* subject, const FSCRT_BSTR* msg, FS_LPCVOID attachData, FS_DWORD attachLen, FS_BOOL displayUI);
// 	FS_RESULT ret = pApp->CallFunction(NULL, FSCRT_APPFUNC_MAIL, inParams, 8, &retVal2);

	FS_RESULT ret = pFormFiller->m_pJSActionHandle->Mail((FSPDFDoc*)m_pDocument, (FX_LPBYTE)mailData, length, bUI, &sTo, &sSubject, &sCc, &sBcc, &sMsg, mailData == NULL ? FSPDF_MAILTYPE_DOC : FSPDF_MAILTYPE_FORM);
	return (FSCRT_ERRCODE_SUCCESS != ret)? FALSE : TRUE;
#else
	return TRUE;
#endif
}

FX_BOOL JDocument::mailDoc(OBJ_METHOD_PARAMS)
{
	ASSERT(m_pDocument != NULL);

	FX_BOOL bUI = TRUE;
	CFX_WideString cTo = L"";
	CFX_WideString cCc = L"";
	CFX_WideString cBcc = L"";
	CFX_WideString cSubject = L"";
	CFX_WideString cMsg = L"";
	

	bUI = params.size()>=1?static_cast<FX_BOOL>(params[0]):TRUE;
	cTo = params.size()>=2?(const wchar_t*)(FX_LPCWSTR)params[1]:L"";
	cCc = params.size()>=3?(const wchar_t*)(FX_LPCWSTR)params[2]:L"";
	cBcc = params.size()>=4?(const wchar_t*)(FX_LPCWSTR)params[3]:L"";
	cSubject = params.size()>=5?(const wchar_t*)(FX_LPCWSTR)params[4]:L"";
	cMsg = params.size()>=6?(const wchar_t*)(FX_LPCWSTR)params[5]:L"";
	
	if(params.size()>=1 && params[0].GetType() == VT_object)
	{
		JSObject * pObj = (JSObject *)params[0];

		if (Value * pValue = DS_GetObjectElement(pObj, L"bUI"))
			bUI = (int)CFXJS_Value(pValue,GET_VALUE_TYPE(pValue));

		if (Value * pValue = DS_GetObjectElement(pObj, L"cTo"))
			cTo = (FX_LPCWSTR)CFXJS_Value(pValue,GET_VALUE_TYPE(pValue));

		if (Value * pValue = DS_GetObjectElement(pObj, L"cCc"))
			cCc = (FX_LPCWSTR)CFXJS_Value(pValue,GET_VALUE_TYPE(pValue));

		if (Value * pValue = DS_GetObjectElement(pObj, L"cBcc"))
			cBcc = (FX_LPCWSTR)CFXJS_Value(pValue,GET_VALUE_TYPE(pValue));

		if (Value * pValue = DS_GetObjectElement(pObj, L"cSubject"))
			cSubject = (FX_LPCWSTR)CFXJS_Value(pValue,GET_VALUE_TYPE(pValue));

		if (Value * pValue = DS_GetObjectElement(pObj, L"cMsg"))
			cMsg = (FX_LPCWSTR)CFXJS_Value(pValue,GET_VALUE_TYPE(pValue));
	
	}

	CFXJS_Context* pContext = (CFXJS_Context*)cc;
	ASSERT(pContext != NULL);
	CFXJS_Runtime* pRuntime = pContext->GetJSRuntime();
	if(!pRuntime) return TRUE;

	pRuntime->BeginBlock();
 	JS_docmailForm(NULL, 0, bUI, (FX_LPCWSTR)cTo, (FX_LPCWSTR)cSubject, (FX_LPCWSTR)cCc, (FX_LPCWSTR)cBcc, (FX_LPCWSTR)cMsg);
	pRuntime->EndBlock();

	return TRUE;
}

FX_BOOL JDocument::JS_SetChangeMark(FX_BOOL bChanged)
{
	FSFormFillerImp* pFormFiller = m_pDocument->GetForm()->GetFormFiller();
    if(!pFormFiller) return FALSE;
// 	CFSCRT_LTAppHandler* pApp = FSCRT_GetLTAppHandler();
// 	if (!pApp) return FALSE;

// 	FSCRT_VAR params = {FSCRT_VT_BOOL, 0, 0};
// 	FSCRT_Var_SetBooleanValue(params, bChanged);
// 	FSCRT_VAR retVal = {FSCRT_VT_INT32, 0, 0};
// 	FSCRT_Var_SetInt32Value(retVal, 0);

	FS_RESULT ret = FSCRT_ERRCODE_SUCCESS;
#if defined(_FSDK_OOM_) && !defined(_FXSDK_EDIT_OOM_RECOVER_) && !defined(_FXSDK_CUSTOM_OOM_RECOVER_)
	//FSCRT_GACHECKOOMSTATE_SETMODIFYFLAG(m_pDocument, TRUE);
#endif
	pFormFiller->m_pJSActionHandle->SetChangeMark((FSPDFDoc*)m_pDocument, bChanged);
//	FS_RESULT ret = pApp->CallFunction(NULL, FSCRT_APPFUNC_SETCHANGEMARK, &params, 1, &retVal);
	if (ret != FSCRT_ERRCODE_SUCCESS) return FALSE;
	return TRUE;
}

FX_BOOL JDocument::author(OBJ_PROP_PARAMS)
{
	ASSERT(m_pDocument != NULL);

	CPDF_Dictionary* pDictionary = m_pDocument->getPDFDocument()->GetInfo();
	if (!pDictionary)return FALSE;

	if (vp.IsGetting())
	{
		vp << pDictionary->GetUnicodeText("Author");
		return TRUE;
	}
	else
	{
		CPDF_Document* pDoc = m_pDocument->getPDFDocument();
		ASSERT(pDoc != NULL);
		FX_DWORD dwPermissions = pDoc->GetUserPermissions();
		if (!(dwPermissions & FPDFPERM_MODIFY)) return FALSE;

		CFX_WideString csAuthor;
		vp >> csAuthor;
		pDictionary->SetAtString("Author", PDF_EncodeText(csAuthor));
		JS_SetChangeMark(TRUE);
		return TRUE;
	}
}

FX_BOOL JDocument::info(OBJ_PROP_PARAMS)
{
	ASSERT(m_pDocument != NULL);

	CPDF_Dictionary* pDictionary = m_pDocument->getPDFDocument()->GetInfo();
	if (!pDictionary)return FALSE;

	CFX_WideString cwAuthor			= pDictionary->GetUnicodeText("Author");
	CFX_WideString cwTitle			= pDictionary->GetUnicodeText("Title");
	CFX_WideString cwSubject		= pDictionary->GetUnicodeText("Subject");
	CFX_WideString cwKeywords		= pDictionary->GetUnicodeText("Keywords");
	CFX_WideString cwCreator		= pDictionary->GetUnicodeText("Creator");
	CFX_WideString cwProducer		= pDictionary->GetUnicodeText("Producer");
	CFX_WideString cwCreationDate	= pDictionary->GetUnicodeText("CreationDate");
	CFX_WideString cwModDate		= pDictionary->GetUnicodeText("ModDate");
	CFX_WideString cwTrapped		= pDictionary->GetUnicodeText("Trapped");

	if (!vp.IsSetting())
	{
		CFXJS_Context* pContext = (CFXJS_Context *)cc;
		CFXJS_Runtime* pRuntime = pContext->GetJSRuntime();
		if(!pRuntime) return TRUE;
		JSFXObject * pObj = DS_NewFxDynamicObj(pRuntime, pContext, -1);

		DS_PutObjectString(pObj, L"Author", cwAuthor);
		DS_PutObjectString(pObj, L"Title", cwTitle);
		DS_PutObjectString(pObj, L"Subject", cwSubject);
		DS_PutObjectString(pObj, L"Keywords", cwKeywords);
		DS_PutObjectString(pObj, L"Creator", cwCreator);
		DS_PutObjectString(pObj, L"Producer", cwProducer);
		DS_PutObjectString(pObj, L"CreationDate", cwCreationDate);
		DS_PutObjectString(pObj, L"ModDate", cwModDate);
		DS_PutObjectString(pObj, L"Trapped", cwTrapped);
	
		vp << pObj;
		return TRUE;
	}
	else
	{
		return TRUE;
	}
}

FX_BOOL JDocument::creationDate(OBJ_PROP_PARAMS)
{
	ASSERT(m_pDocument != NULL);

	CPDF_Dictionary* pDictionary = m_pDocument->getPDFDocument()->GetInfo();
	if (!pDictionary)return FALSE;

	if (vp.IsGetting())
	{
		vp << pDictionary->GetUnicodeText("CreationDate");
		return TRUE;
	}
	else
	{
		CPDF_Document* pDoc = m_pDocument->getPDFDocument();
		ASSERT(pDoc != NULL);
		FX_DWORD dwPermissions = pDoc->GetUserPermissions();
		if (!(dwPermissions & FPDFPERM_MODIFY)) return FALSE;

		CFX_WideString csCreationDate;
		vp >> csCreationDate;
		pDictionary->SetAtString("CreationDate", PDF_EncodeText(csCreationDate));
		JS_SetChangeMark(TRUE);

		return TRUE;
	}
}

FX_BOOL JDocument::creator(OBJ_PROP_PARAMS)
{
	ASSERT(m_pDocument != NULL);

	CPDF_Dictionary* pDictionary = m_pDocument->getPDFDocument()->GetInfo();
	if (!pDictionary)return FALSE;

	if (vp.IsGetting())
	{
		vp << pDictionary->GetUnicodeText("Creator");
		return TRUE;
	}
	else
	{
		CPDF_Document* pDoc = m_pDocument->getPDFDocument();
		ASSERT(pDoc != NULL);
		FX_DWORD dwPermissions = pDoc->GetUserPermissions();
		if (!(dwPermissions & FPDFPERM_MODIFY)) return FALSE;

		CFX_WideString csCreator;
		vp >> csCreator;
		pDictionary->SetAtString("Creator", PDF_EncodeText(csCreator));
		JS_SetChangeMark(TRUE);
		return TRUE;
	}
}

FX_BOOL JDocument::delay(OBJ_PROP_PARAMS)
{
	if (vp.IsGetting())
	{
		vp << m_bDelay;
		return TRUE;
	}
	else
	{
		ASSERT(m_pDocument != NULL);
		CPDF_Document* pDoc = m_pDocument->getPDFDocument();
		ASSERT(pDoc != NULL);
		FX_DWORD dwPermissions = pDoc->GetUserPermissions();
		if (!(dwPermissions & FPDFPERM_MODIFY)) return FALSE;

		FX_BOOL b = FALSE;
		vp >> b;

		m_bDelay = b;

		if (m_bDelay) 
		{
			for (int i=0,sz=m_DelayData.GetSize(); i<sz; i++)
				delete m_DelayData.GetAt(i);

			m_DelayData.RemoveAll();
		}
		else
		{
			for (int i=0,sz=m_DelayData.GetSize(); i<sz; i++)
			{
				if (CJS_DelayData* pData = m_DelayData.GetAt(i))
				{
					JField::DoDelay(m_pDocument, pData);
					delete m_DelayData.GetAt(i);
				}
			}
			m_DelayData.RemoveAll();
		}

		return TRUE;
	}
}

FX_BOOL JDocument::keywords(OBJ_PROP_PARAMS)
{
	ASSERT(m_pDocument != NULL);

	CPDF_Dictionary* pDictionary = m_pDocument->getPDFDocument()->GetInfo();
	if (!pDictionary)return FALSE;

	if (vp.IsGetting())
	{
		vp << pDictionary->GetUnicodeText("Keywords");
		return TRUE;
	}
	else
	{
		CPDF_Document* pDoc = m_pDocument->getPDFDocument();
		ASSERT(pDoc != NULL);
		FX_DWORD dwPermissions = pDoc->GetUserPermissions();
		if (!(dwPermissions & FPDFPERM_MODIFY)) return FALSE;

		CFX_WideString csKeywords;
		vp >> csKeywords;
		pDictionary->SetAtString("Keywords", PDF_EncodeText(csKeywords));
		JS_SetChangeMark(TRUE);
		return TRUE;
	}
}

FX_BOOL JDocument::modDate(OBJ_PROP_PARAMS)
{
	ASSERT(m_pDocument != NULL);

	CPDF_Dictionary* pDictionary = m_pDocument->getPDFDocument()->GetInfo();
	if (!pDictionary)return FALSE;

	if (vp.IsGetting())
	{
		vp << pDictionary->GetUnicodeText("ModDate");
		return TRUE;
	}
	else
	{
		CPDF_Document* pDoc = m_pDocument->getPDFDocument();
		ASSERT(pDoc != NULL);
		FX_DWORD dwPermissions = pDoc->GetUserPermissions();
		if (!(dwPermissions & FPDFPERM_MODIFY)) return FALSE;

		CFX_WideString csmodDate;
		vp >> csmodDate;
		pDictionary->SetAtString("ModDate", PDF_EncodeText(csmodDate));
		JS_SetChangeMark(TRUE);
		return TRUE;
	}
}

FX_BOOL JDocument::producer(OBJ_PROP_PARAMS)
{
	ASSERT(m_pDocument != NULL);

	CPDF_Dictionary* pDictionary = m_pDocument->getPDFDocument()->GetInfo();
	if (!pDictionary)return FALSE;

	if (vp.IsGetting())
	{
		vp << pDictionary->GetUnicodeText("Producer");
		return TRUE;
	}
	else
	{
		CPDF_Document* pDoc = m_pDocument->getPDFDocument();
		ASSERT(pDoc != NULL);
		FX_DWORD dwPermissions = pDoc->GetUserPermissions();
		if (!(dwPermissions & FPDFPERM_MODIFY)) return FALSE;

		CFX_WideString csproducer;
		vp >> csproducer;
		pDictionary->SetAtString("Producer", PDF_EncodeText(csproducer));
		JS_SetChangeMark(TRUE);
		return TRUE;
	}
}

FX_BOOL JDocument::subject(OBJ_PROP_PARAMS)
{
	ASSERT(m_pDocument != NULL);

	CPDF_Dictionary* pDictionary = m_pDocument->getPDFDocument()->GetInfo();
	if (!pDictionary)return FALSE;

	if (vp.IsGetting())
	{
		vp << pDictionary->GetUnicodeText("Subject");
		return TRUE;
	}
	else
	{
		CPDF_Document* pDoc = m_pDocument->getPDFDocument();
		ASSERT(pDoc != NULL);
		FX_DWORD dwPermissions = pDoc->GetUserPermissions();
		if (!(dwPermissions & FPDFPERM_MODIFY)) return FALSE;

		CFX_WideString cssubject;
		vp >> cssubject;
		pDictionary->SetAtString("Subject", PDF_EncodeText(cssubject));
		JS_SetChangeMark(TRUE);
		return TRUE;
	}
}

FX_BOOL JDocument::title(OBJ_PROP_PARAMS)
{
	ASSERT(m_pDocument != NULL);

	CPDF_Dictionary* pDictionary = m_pDocument->getPDFDocument()->GetInfo();
	if (!pDictionary)return FALSE;

	if (vp.IsGetting())
	{
		vp << pDictionary->GetUnicodeText("Title");
		return TRUE;
	}
	else
	{
		CPDF_Document* pDoc = m_pDocument->getPDFDocument();
		ASSERT(pDoc != NULL);
		FX_DWORD dwPermissions = pDoc->GetUserPermissions();
		if (!(dwPermissions & FPDFPERM_MODIFY)) return FALSE;

		CFX_WideString cstitle;
		vp >> cstitle;
		pDictionary->SetAtString("Title", PDF_EncodeText(cstitle));
		JS_SetChangeMark(TRUE);
		return TRUE;
	}
}

FX_BOOL JDocument::numPages(OBJ_PROP_PARAMS)
{
	if (vp.IsGetting())
	{
		ASSERT(m_pDocument->getPDFDocument() != NULL);
		vp << m_pDocument->getPDFDocument()->GetPageCount();
		return TRUE;
	}
	else
	{
		return FALSE;
	}
	return TRUE;
}

FX_BOOL JDocument::external(OBJ_PROP_PARAMS)
{
	//In Chrome case,should always return true.
	if(vp.IsGetting())
		vp << TRUE;
	return TRUE;
}

FX_BOOL JDocument::filesize(OBJ_PROP_PARAMS)
{
	if (!vp.IsGetting())return FALSE;

	ASSERT(m_pDocument != NULL);

	CFX_WideString wsDocPath;
	JS_docGetPath(wsDocPath);
	if (wsDocPath.IsEmpty() == FALSE)
	{
		CFX_ByteString bsStr = CFX_ByteString::FromUnicode(wsDocPath);
		FILE * pFile = NULL;
#if (WINAPI_PARTITION_APP || WINAPI_PARTITION_PC_APP)
		fopen_s( &pFile, bsStr.GetBuffer( bsStr.GetLength() ), "rb" );
#else
		pFile = fopen( bsStr.GetBuffer( bsStr.GetLength() ), "rb" );
#endif
		if ( pFile )
		{
			fseek( pFile, 0, SEEK_END );
			long lSize = ftell( pFile );
			fclose( pFile );
			pFile = NULL;

			vp << (FX_INT32)(lSize);
			return TRUE;
		}
	}

	vp << 0;
	return TRUE;
}

FX_BOOL JDocument::mouseX(OBJ_PROP_PARAMS)
{
 	return TRUE;	
}

FX_BOOL JDocument::mouseY(OBJ_PROP_PARAMS)
{
 	return TRUE;
}

FX_BOOL JDocument::baseURL(OBJ_PROP_PARAMS)
{
	if (vp.IsGetting())
	{
		vp << m_cwBaseURL;
		return TRUE;
	}
	else
	{
		vp >> m_cwBaseURL;
		return TRUE;
	}
}

FX_BOOL JDocument::calculate(OBJ_PROP_PARAMS)
{
	ASSERT(m_pDocument != NULL);

	FSFormImp* pInterForm= (FSFormImp*)m_pDocument->GetForm();
	ASSERT(pInterForm != NULL);

	if (vp.IsGetting())
	{
		if (pInterForm->IsCalculateEnabled())
			vp << true;
		else
			vp << false;
	}
	else
	{
		bool bCalculate;
		vp >> bCalculate;

		pInterForm->EnableCalculate(bCalculate);
	}

	return TRUE;
}

FX_BOOL JDocument::documentFileName(OBJ_PROP_PARAMS)
{
	if (!vp.IsGetting())
		return FALSE;

	CFX_WideString wsFilePath;
	JS_docGetPath(wsFilePath);

	FX_INT32 i = wsFilePath.GetLength() - 1;
	for ( ; i >= 0; i-- )
	{
		if ( wsFilePath.GetAt( i ) == L'\\' || wsFilePath.GetAt( i ) == L'/' )
			break;
	}
	if ( i >= 0 && i < wsFilePath.GetLength() - 1 )
	{
		vp << ( wsFilePath.GetBuffer( wsFilePath.GetLength() ) + i + 1 );
	}else{
		vp << (FX_LPCWSTR)L"";
	}
	return TRUE;
}

CFX_WideString JDocument::ReversalStr(CFX_WideString cbFrom)
{
	wchar_t* pFrom = NULL;
	int iLenth = cbFrom.GetLength();
	wchar_t* pResult = (wchar_t*)malloc((iLenth+1) * sizeof(wchar_t));
	memset(pResult, 0, (iLenth+1));
	pFrom = (wchar_t*)(FX_LPCWSTR)cbFrom;

	for (int i = 0; i < iLenth; i++)
	{
		pResult[i] = *(pFrom + iLenth - i - 1);
	}
	CFX_WideString cbRet = CFX_WideString(pResult);
	free(pResult);
	pResult = NULL;
	return cbRet;
}

CFX_WideString JDocument::CutString(CFX_WideString cbFrom)
{
	wchar_t* pFrom = NULL;
	int iLenth = cbFrom.GetLength();
	wchar_t* pResult = (wchar_t*)malloc((iLenth+1) * sizeof(wchar_t));
	memset(pResult, 0, (iLenth+1));
	pFrom = (wchar_t*)(FX_LPCWSTR)cbFrom;

	for (int i = 0; i < iLenth; i++)
	{
		if (pFrom[i] == L'\\' || pFrom[i] == L'/')
			break;
		pResult[i] = pFrom[i];
	}

	CFX_WideString cbRet = CFX_WideString(pResult);
	free(pResult);
	pResult = NULL;
	return cbRet;
}

FX_BOOL JDocument::path(OBJ_PROP_PARAMS)
{
	if (!vp.IsGetting()) return FALSE;

	CFX_WideString wsFilePath;
	JS_docGetPath(wsFilePath);
	vp << japp::SysPathToPDFPath(wsFilePath);

	return TRUE;
}

FX_BOOL JDocument::pageWindowRect(OBJ_PROP_PARAMS)
{
// 	if (!vp.IsGetting())
// 		return FALSE;
// 
// 	CReader_DocView* pDocView = m_pDocument->GetCurrentDocView();
// 	if (!pDocView) return FALSE;
// 
// 	CReader_PageView* pPageView = pDocView->GetCurrentPageView();
// 	if (!pPageView) return FALSE;
// 	
// 	CPDF_Rect rect;
// 	HWND hWnd = pPageView->GetPageViewWnd();
// 	GetWindowRect(hWnd, &rect);
// 	
// 	CJS_Array RetArray;
// 	RetArray.SetElement(0, rect.left);
// 	RetArray.SetElement(1, rect.top);
// 	RetArray.SetElement(2, rect.right);
// 	RetArray.SetElement(3, rect.bottom);
// 
// 	vp << RetArray;
	return TRUE;
}

FX_BOOL JDocument::layout(OBJ_PROP_PARAMS)
{	
/*	CPDF_TVView *pTVView = _gAfxGetTVView();
	if (!pTVView)return FALSE;

	CPDF_TVPreview* pTVPreview = pTVView->GetPreviewWnd();
	if (!pTVPreview)return FALSE;

	int LayoutModel = 0;
	BOOL bFacing = FALSE;
	CFX_WideString cwModel = L"";
	CFX_ByteString cbModel = L"";
	vp >> cwModel;

	cbModel = CFX_ByteString::FromUnicode(cwModel);
	int iLenth = cbModel.GetLength();
	char* pModel = cbModel.GetBuffer(iLenth);
	
	if (memcmp(pModel, "SinglePage", iLenth) == 0)
	{
		LayoutModel = 0;
		bFacing = FALSE;
	}
	else if (memcmp(pModel, "OneColumn", iLenth) == 0)
	{
		LayoutModel = 1;
		bFacing = FALSE;
	}
	else if (memcmp(pModel, "TwoColumnLeft", iLenth) == 0)
	{
		LayoutModel = 1;
		bFacing = TRUE;
	}
	else if (memcmp(pModel, "TwoColumnRight", iLenth) == 0)
	{
		LayoutModel = 1;
		bFacing = TRUE;						
	}
	else if (memcmp(pModel, "TwoPageLeft", iLenth) == 0)
	{
		LayoutModel = 0;
		bFacing = TRUE;
	}
	else if (memcmp(pModel, "TwoPageRight", iLenth) == 0)
	{
		LayoutModel = 0;
		bFacing = TRUE;
	}

	if (vp.IsSetting())
	{	
		pTVPreview->SetLayoutShowMode(LayoutModel, bFacing);
		cbModel.ReleaseBuffer();
		return TRUE;
	}
	else
	{
		cbModel.ReleaseBuffer();
		return TRUE;
	}
*/
	return TRUE;
}

FX_BOOL JDocument::addLink(OBJ_METHOD_PARAMS)
{
// 	ASSERT(m_pDocument != NULL);
// 
// 	if (!(m_pDocument->GetPermissions(FPDFPERM_MODIFY) || 
// 		m_pDocument->GetPermissions(FPDFPERM_ANNOT_FORM))) return FALSE;
// 
// 	if (params.size() != 2)
// 		return FALSE;
// 
// 	CFX_FloatRect cfLinkRect;
// 
// 	if (params[0].GetType() != VT_number || params[1].GetType() != VT_object)
// 		return FALSE;
// 
// 	int nPage = params[0];
// 	CJS_Value vRect = params[1];
// 
// 	CJS_Array aRect;
// 	vRect.ConvertToArray(aRect);
// 	if (aRect.GetLength() != 4)
// 		return FALSE;
// 
// 	CJS_Value vElement[4];
// 	for (int i = 0; i < 4; i++)
// 		aRect.GetElement(i, vElement[i]);
// 
// 	cfLinkRect.left = (FX_FLOAT)vElement[0];
// 	cfLinkRect.top = (FX_FLOAT)vElement[1];
// 	cfLinkRect.right = (FX_FLOAT)vElement[2];
// 	cfLinkRect.bottom = (FX_FLOAT)vElement[3];
// 
// 	CReader_Page* pPage = m_pDocument->GetPage(nPage);
// 	if (!pPage) return FALSE;
// 
// 	CBA_Annot* pNewAnnot = (CBA_Annot*)pPage->AddAnnot("Link", cfLinkRect);
// 	ASSERT(pNewAnnot != NULL);
// 
// 	CPDF_Dictionary* pDict = pNewAnnot->GetPDFAnnot()->m_pAnnotDict;
// 	ASSERT(pDict != NULL);
// 
// 	if (!pDict->KeyExist("BS"))
// 	{
// 		CPDF_Dictionary* pBorderStyle = CPDF_Dictionary::Create();
// 		ASSERT(pBorderStyle != NULL);
// 
// 		pBorderStyle->SetAtInteger("W", 0);
// 		pBorderStyle->SetAtName("S", "S");
// 		pBorderStyle->SetAtName("Type", "Border");
// 		pDict->SetAt("BS", pBorderStyle);
// 	}
// 	else
// 	{
// 		CPDF_Dictionary* pBorderStyle = pDict->GetDict("BS");
// 		if (!pBorderStyle) return FALSE;
// 
// 		pBorderStyle->SetAtInteger("W", 0);
// 	}
// 
// 	CPDF_Array* pSetColor = CPDF_Array::Create();
// 	ASSERT(pSetColor != NULL);
// 
// 	for (int j = 0; j < 3; j++)
// 		pSetColor->AddNumber((float)0);
// 
// 	pDict->SetAt("C", pSetColor);
// 
// 	pNewAnnot->ResetAppearance();
// 	pPage->UpdateAllViews(NULL, pNewAnnot);
// 
// 	CJS_Context* pContext = (CJS_Context *)cc;
// 	CJS_Runtime* pRuntime = pContext->GetJSRuntime();
// 
// 	JSFXObject * pObj = DS_NewFxDynamicObj(pRuntime, pContext, DS_GetObjDefnID(pRuntime, L"Link"));
// 	if (!pObj) return FALSE;
// 		
// 	CJS_Link * pJS_FS = (CJS_Link *)DS_GetPrivate(pObj);
// 	if (!pJS_FS) return FALSE;
// 
// 	Link* pLink = (Link*)(pJS_FS->GetEmbedObject());
// 	if (!pLink)return FALSE;
// 
// 	pLink->SetAnnot(pNewAnnot);
// 	vRet = pJS_FS;
// 
 	return TRUE;
}

FX_BOOL JDocument::closeDoc(OBJ_METHOD_PARAMS)
{
// 	ASSERT(m_pDocument != NULL);
// 
// //	CReader_AppProvider* pAppProvider = CReader_AppProvider::CreateAppProvider();
// //	pAppProvider->CloseDocument(m_pDocument);
// 
// 	//MQG Added
// 	CPDFDoc_Environment* pEnv = m_pDocument->GetEnv();
// 	ASSERT(pEnv != NULL);
// 
// //	pEnv->CloseDocument(m_pDocument, TRUE);

	return TRUE;
}

FX_BOOL JDocument::getPageBox(OBJ_METHOD_PARAMS)
{
	ASSERT(m_pDocument != NULL);

	CFX_WideString cwBox = params.size() > 0 ? (CFX_WideString)(FX_LPCWSTR)params[0] : L"CropBox";
	FX_INT32 nPageIndex = params.size() > 1 ? (FX_INT32)params[1] : 0;

	FX_INT32 nPageCount = 0;
	m_pDocument->GetPageCount(&nPageCount);
	if (nPageIndex > nPageCount) return FALSE;

	FSPDFPage* pPage = NULL;
	m_pDocument->GetPage(nPageIndex, &pPage);
	if (!pPage)return FALSE;
//In case of OOM.
	//pPage->ST_Load();
	CPDF_Page* pPDFPage = ((FSPDFPageImp*)pPage)->getPage();
	if (!pPDFPage)return FALSE;

	CFX_FloatRect rcBBox = pPDFPage->GetPageBBox();
	CPDF_Rect rcWin;
	CFXJS_Array aBBox;

	aBBox.SetElement(0, rcBBox.left);
	aBBox.SetElement(1, rcBBox.top);
	aBBox.SetElement(2, rcBBox.right);
	aBBox.SetElement(3, rcBBox.bottom);

	vRet = aBBox;
 	return TRUE;
}


FX_BOOL JDocument::getAnnot(OBJ_METHOD_PARAMS)
{
// 	ASSERT(m_pDocument != NULL);
// 
// 	if (params.size() != 2) return FALSE;
// 	if (params[0].GetType() != VT_number || params[1].GetType() != VT_string)
// 		return FALSE;
// 
// 	int nPage = params[0];
// 	CFX_WideString cwAnnotName = params[1];
// 
// 	CReader_Page* pPage = m_pDocument->GetPage(nPage);
// 	if (!pPage) return FALSE;
// 
// 	int nAnnotCounts = pPage->CountAnnots();
// 
// 	CPDFSDK_Annot* pAnnot = NULL;
// 	FX_BOOL bFind = FALSE;
// 
// 	for (int i=0; i<nAnnotCounts; i++)
// 	{
// 		pAnnot = pPage->GetAnnot(i);
// 		ASSERT(pAnnot != NULL);
// 		CPDF_Dictionary* pDict = pAnnot->GetPDFAnnot()->m_pAnnotDict;
// 
// 		CFX_WideString cwName = pDict->GetUnicodeText("NM");
// 		if (cwName == cwAnnotName)
// 		{
// 			bFind = TRUE;
// 			break;
// 		}
// 	}
// 
// 	if (!bFind)
// 	{
// 		vRet = NULL;
// 		return TRUE;
// 	}
// 
// 	CJS_Context* pContext = (CJS_Context*)cc;
// 	ASSERT(pContext != NULL);
// 
// 	CJS_Runtime* pRuntime = pContext->GetJSRuntime();
// 	ASSERT(pRuntime != NULL);
// 
// 	JSFXObject* pObj = DS_NewFxDynamicObj(pRuntime, pContext, DS_GetObjDefnID(pRuntime, L"Annotation"));
// 	ASSERT(pObj != NULL);
// 		
// 	CJS_Annotation* pJS_Annot = (CJS_Annotation*)DS_GetPrivate(pObj);
// 	ASSERT(pJS_Annot != NULL);
// 
// 	Annotation* pAnnotation = (Annotation*)(pJS_Annot->GetEmbedObject());
// 	if (!pAnnotation)return FALSE;
// 
// 	pAnnotation->Initial(this, pAnnot);
// 	vRet = pJS_Annot;
 	return TRUE;
}

FX_BOOL JDocument::getAnnots(OBJ_METHOD_PARAMS)
{
	vRet.SetNull();
	return TRUE;
}

FX_BOOL JDocument::getAnnot3D(OBJ_METHOD_PARAMS)
{
	vRet.SetNull();
	return TRUE;
}

FX_BOOL JDocument::getAnnots3D(OBJ_METHOD_PARAMS)
{
	vRet = VT_undefined;
	return TRUE;
}

FX_BOOL JDocument::getOCGs(OBJ_METHOD_PARAMS)
{
	ASSERT(m_pDocument != NULL);

	FX_INT32 iPage = -1;
	if (params.size() == 1)
	{
		if (params[0].GetType() == VT_number)
			iPage = params[0];
	}
	
	FX_INT32 nPageCount = -1;
	m_pDocument->GetPageCount(&nPageCount);/*CountDocViews()*/;
	FX_INT32 iOCGCounts = 0;
	CFX_PtrArray ocgs;
	CFXJS_Array OCGArray;
	CPDF_Document* pPDFDocument = m_pDocument->getPDFDocument();
	if (!pPDFDocument)return FALSE;

	CPDF_OCProperties ocgProperties(pPDFDocument);
	//CPDFSDK_PageView* pDocView = NULL;
	
	if (iPage >= 0 && iPage < nPageCount)
	{
		//pDocView = m_pDocument->GetPageView( m_pDocument->GetPage(iPage), FALSE );
		iOCGCounts = ocgProperties.GetOCGroups(ocgs, iPage);
	}
	else
	{
		//pDocView = m_pDocument->GetCurrentView();
		iOCGCounts = ocgProperties.GetOCGroups(ocgs);
	}

	//if (!pDocView) return FALSE;

	//CPDF_OCContext* pOCGContenx = m_pDocument->GetOCContext();

	CFXJS_Context* pContext = (CFXJS_Context *)cc;
	CFXJS_Runtime* pRuntime = pContext->GetJSRuntime();
	if(!pRuntime) return TRUE;
	for (int i = 0; i < iOCGCounts; i++)
	{		
		JSFXObject* pObj = DS_NewFxDynamicObj(pRuntime, pContext, DS_GetObjDefnID(pRuntime, L"OCG"));
		ASSERT(pObj != NULL);
			
		CJS_OCG* pJS_OCG = (CJS_OCG*)DS_GetPrivate(pObj);
		/*ASSERT(pJS_OCG != NULL);

		OCG* pOCG = (OCG*)(pJS_OCG->GetEmbedObject());
		ASSERT(pOCG != NULL);

		pOCG->SetOCGInfo(m_pDocument, pDocView, (CPDF_Dictionary*)ocgs[i]);
		OCGArray.SetElement(i, CJS_Value(pJS_OCG));*/
	}

	vRet = OCGArray;			
	return TRUE;
}

FX_BOOL JDocument::getLinks(OBJ_METHOD_PARAMS)
{
// 	if (params.size() != 2) return FALSE;
// 	if (params[0].GetType() != VT_number || params[1].GetType() != VT_object)
// 		return FALSE;
// 
// 	int iPage = params[0];
// 
// 	CJS_Value vRect[4];
// 	vRect[0] = params[1];
// 
// 	CJS_Array RectArray;
// 	vRect[0].ConvertToArray(RectArray);
// 	if (RectArray.GetLength() != 4)
// 		return FALSE;
// 
// 	for (int i=0; i<4; i++)
// 	{
// 		RectArray.GetElement(i, vRect[i]);
// 	}
// 
// 	CFX_FloatRect Rect;
// 	Rect.left = vRect[0]; 
// 	Rect.top = vRect[1];
// 	Rect.right = vRect[2];
// 	Rect.bottom = vRect[3];
// 
// 	ASSERT(m_pDocument != NULL);
// 
// 	int iPageCount = m_pDocument->GetPageCount();
// 	if (iPage < 0 || iPage > iPageCount)
// 		return FALSE;
// 
// 	CPDFSDK_PageView* pPage = m_pDocument->GetPageView( m_pDocument->GetPage(iPage), FALSE );
// 	ASSERT(pPage != NULL);
// 
// 	int nLinkCount = 0;
// 	CJS_Array LinkAnnots;
// 
// 	CJS_Context* pContext = (CJS_Context *)cc;
// 	ASSERT(pContext != NULL);
// 
// 	CJS_Runtime* pRuntime = pContext->GetJSRuntime();
// 	ASSERT(pRuntime != NULL);
// 
// 	int iAnnotCount = pPage->CountAnnots();
// 	for (int j = 0; j < iAnnotCount; j++)
// 	{
// 		CPDFSDK_Annot* pAnnot = (CPDFSDK_Annot*)pPage->GetAnnot(j);
// 		CFX_ByteString cbAnnotType = pAnnot->GetType();
// 		if (cbAnnotType != "Link")
// 			continue;
// 
// 		CPDF_Rect AnnotRect = pAnnot->GetRect();
// 
// 		if (!IsEnclosedInRect(Rect, AnnotRect))
// 			continue;
// 
// 		JSFXObject* pObj = DS_NewFxDynamicObj(pRuntime, pContext, DS_GetObjDefnID(pRuntime, L"Link"));
// 		ASSERT(pObj != NULL);
// 			
// 		CJS_Link* pJSObj = (CJS_Link*)DS_GetPrivate(pObj);
// 		ASSERT(pJSObj != NULL);
// 
// 		Link* pLink = (Link*)(pJSObj->GetEmbedObject());
// 		ASSERT(pLink != NULL);
// 
// //		pLink->SetAnnot(pAnnot);
// 
// 		LinkAnnots.SetElement(nLinkCount++, CJS_Value(pJSObj));
// 	}
// 
// 	vRet = LinkAnnots;

	return TRUE;
}

bool JDocument::IsEnclosedInRect(CFX_FloatRect rect, CFX_FloatRect LinkRect)
{
	if (rect.left <= LinkRect.left
	  && rect.top <= LinkRect.top
	  && rect.right >= LinkRect.right
	  && rect.bottom >= LinkRect.bottom)
		return true;
	else
		return false;
}

void JIconTree::InsertIconElement(JIconElement* pNewIcon)
{
	if (!pNewIcon)return;

	if (m_pHead == NULL && m_pEnd == NULL)
	{
		m_pHead = m_pEnd = pNewIcon;
		m_iLength++;
	}
	else
	{
		m_pEnd->NextIcon = pNewIcon;
		m_pEnd = pNewIcon;
		m_iLength++;
	}
}

void JIconTree::DeleteIconTree()
{
	if (!m_pHead || !m_pEnd)return;
	
	JIconElement* pTemp = NULL;
	while(m_pEnd != m_pHead)
	{
		pTemp = m_pHead;
		m_pHead = m_pHead->NextIcon;
		delete pTemp;
	}

	delete m_pEnd;
	m_pHead = NULL;
	m_pEnd = NULL;
}

int JIconTree::GetLength()
{
	return m_iLength;
}

JIconElement* JIconTree::operator [](int iIndex)
{
	if (iIndex >= 0 && iIndex <= m_iLength)
	{
		JIconElement* pTemp = m_pHead;
		for (int i = 0; i < iIndex; i++)
		{
			pTemp = pTemp->NextIcon;
		}
		return pTemp;
	}
	else
		return NULL;
}

void JIconTree::DeleteIconElement(CFX_WideString swIconName)
{
	JIconElement* pTemp = m_pHead;
	int iLoopCount = m_iLength; 
	for (int i = 0; i < iLoopCount - 1; i++)
	{
		if (pTemp == m_pEnd)
			break;
	
		if (m_pHead->IconName == swIconName)
		{
			m_pHead = m_pHead->NextIcon;
			delete pTemp;
			m_iLength--;
			pTemp = m_pHead;
		}
		if (pTemp->NextIcon->IconName == swIconName)
		{
			if (pTemp->NextIcon == m_pEnd)
			{
				m_pEnd = pTemp;
				delete pTemp->NextIcon;
				m_iLength--;
				pTemp->NextIcon = NULL;
			}
			else
			{
				JIconElement* pElement = pTemp->NextIcon;
				pTemp->NextIcon = pTemp->NextIcon->NextIcon;
				delete pElement;
				m_iLength--;
				pElement = NULL;
			}

			continue;
		}

		pTemp = pTemp->NextIcon;
	}
}

FX_BOOL JDocument::addIcon(OBJ_METHOD_PARAMS)
{
	if (params.size() != 2)return FALSE;

	CFXJS_Context* pContext = (CFXJS_Context*)cc;
	ASSERT(pContext != NULL);
	CFXJS_Runtime* pRuntime = pContext->GetJSRuntime();
	if(!pRuntime) return TRUE;

	CFX_WideString swIconName = (FX_LPCWSTR)params[0];
	
	JSFXObject* pJSIcon = (JSFXObject*)params[1];
	if (!pJSIcon)return FALSE;
	if (DS_GetObjDefnID(pJSIcon) != DS_GetObjDefnID(pRuntime, L"Icon")) return FALSE;

	CFXJS_EmbedObj* pEmbedObj = ((CFXJS_Object*)params[1])->GetEmbedObject();
	if (!pEmbedObj)return FALSE;
	JIcon* pIcon = (JIcon*)pEmbedObj;

	if (!m_pIconTree)
		m_pIconTree = new JIconTree();

	JIconElement* pNewIcon = new JIconElement();
	pNewIcon->IconName = swIconName;
	pNewIcon->NextIcon = NULL;
	pNewIcon->IconStream = pIcon;
	m_pIconTree->InsertIconElement(pNewIcon);
	return TRUE;
}

FX_BOOL JDocument::icons(OBJ_PROP_PARAMS)
{
	if (vp.IsSetting())
		return FALSE;

	if (!m_pIconTree)
	{
		vp.SetNull();
		return TRUE;
	}

	CFXJS_Array Icons;
	JIconElement* pIconElement = NULL;
	int iIconTreeLength = m_pIconTree->GetLength();

	CFXJS_Context* pContext = (CFXJS_Context *)cc;
	CFXJS_Runtime* pRuntime = pContext->GetJSRuntime();
	if(!pRuntime) return TRUE;
	for (int i = 0; i < iIconTreeLength; i++)
	{
		pIconElement = (*m_pIconTree)[i];
		
		JSFXObject * pObj = DS_NewFxDynamicObj(pRuntime, pContext, DS_GetObjDefnID(pRuntime, L"Icon"));
		if (!pObj) return FALSE;
				
		CFXJS_Icon * pJS_Icon = (CFXJS_Icon *)DS_GetPrivate(pObj);
		if (!pJS_Icon) return FALSE;

		JIcon* pIcon = (JIcon*)pJS_Icon->GetEmbedObject();
		if (!pIcon)return FALSE;

		pIcon->SetStream(pIconElement->IconStream->GetStream());
		pIcon->SetIconName(pIconElement->IconName);
		Icons.SetElement(i, CFXJS_Value(pJS_Icon));
	}

	vp << Icons;
	return TRUE;
}

FX_BOOL JDocument::getIcon(OBJ_METHOD_PARAMS)
{
	if (params.size() != 1)return FALSE;
	if(!m_pIconTree)
		return FALSE;
	CFX_WideString swIconName = (FX_LPCWSTR)params[0];
	int iIconCounts = m_pIconTree->GetLength();

	CFXJS_Context* pContext = (CFXJS_Context *)cc;
	CFXJS_Runtime* pRuntime = pContext->GetJSRuntime();
	if(!pRuntime) return TRUE;
	for (int i = 0; i < iIconCounts; i++)
	{
		if ((*m_pIconTree)[i]->IconName == swIconName)
		{
			JIcon* pRetIcon = (*m_pIconTree)[i]->IconStream;
				
			JSFXObject * pObj = DS_NewFxDynamicObj(pRuntime, pContext, DS_GetObjDefnID(pRuntime, L"Icon"));
			if (!pObj) return FALSE;
					
			CFXJS_Icon * pJS_Icon = (CFXJS_Icon *)DS_GetPrivate(pObj);
			if (!pJS_Icon) return FALSE;

			JIcon* pIcon = (JIcon*)pJS_Icon->GetEmbedObject();
			if (!pIcon)return FALSE;

			pIcon->SetIconName(swIconName);
			pIcon->SetStream(pRetIcon->GetStream());
			vRet = pJS_Icon;
			return TRUE;
		}
	}

	return FALSE;
}

FX_BOOL JDocument::removeIcon(OBJ_METHOD_PARAMS)
{
	if (params.size() != 1)return FALSE;
	if(!m_pIconTree)
		return FALSE;
	CFX_WideString swIconName = (FX_LPCWSTR)params[0];
// #ifndef FOXIT_CHROME_BUILD
// 	m_pIconTree->DeleteIconElement(swIconName);
// #endif
	return TRUE;
}

FX_BOOL JDocument::createDataObject(OBJ_METHOD_PARAMS)
{
	if (IsSafeMode(cc)) return TRUE;
	ASSERT(m_pDocument != NULL);

	CFX_WideString swName = L"";
	CFX_ByteString sbName = "";
	CFX_WideString swValue = L"";
	CFX_WideString swMIMEType = L"";
	CFX_WideString swCryptFilter = L"";
	CFX_ByteString sbFileValue = "";
	
	int iParamSize = params.size() < 4 ? params.size():4;
	for (int i = 0; i < iParamSize; i++)
	{
		switch (i)
		{
		case 0:
			swName = params[0];
			break;
		case 1:
			swValue = params[1];
			break;
		case 2:
			swMIMEType = params[2];
			break;
		case 3:
			swCryptFilter = params[3];
			break;
		}
	}

	FILE* pFile = NULL;

	//CFileStatus fileStatus;
	const int BUFSIZE = 17;
	FX_BYTE buf[BUFSIZE];
	FX_BYTE *pBuffer = NULL;
	char* pBuf = NULL;
	int nFileSize = 0;
	sbFileValue = CFX_ByteString::FromUnicode(swValue);
	sbName = CFX_ByteString::FromUnicode(swName);
	int iBufLength = sbFileValue.GetLength();
	pBuf = (char*)malloc(sizeof(char) * iBufLength);
	pBuf = sbFileValue.GetBuffer(iBufLength);

	if (sbName.IsEmpty())
		return FALSE;

	#if (WINAPI_PARTITION_APP || WINAPI_PARTITION_PC_APP)
		fopen_s( &pFile, sbName.GetBuffer(sbName.GetLength()), "wb+" );
	#else
		pFile = FXSYS_fopen( sbName.GetBuffer(sbName.GetLength()), "wb+" );
	#endif
	if ( NULL == pFile )
	{
		return FALSE;
	}

	fwrite( pBuf, sizeof(char), iBufLength, pFile );
	fclose( pFile );
	pFile = NULL;
	
	#if (WINAPI_PARTITION_APP || WINAPI_PARTITION_PC_APP)
		fopen_s( &pFile, sbName.GetBuffer(sbName.GetLength()), "wb+" );
	#else
		pFile = FXSYS_fopen( sbName.GetBuffer(sbName.GetLength()), "rb+" );
	#endif
	fseek( pFile, 0, SEEK_END );
	nFileSize = ftell( pFile );

	pBuffer = new FX_BYTE[nFileSize];
	fseek( pFile, 0, SEEK_SET );
	size_t s = fread( pBuffer, sizeof(char), nFileSize, pFile );
	if(s == 0)
	{
		delete[] pBuffer;
		return FALSE;
	}

	CRYPT_MD5Generate(pBuffer, nFileSize, buf);
	buf[BUFSIZE - 1] = 0;
	CFX_WideString csCheckSum((FX_LPCWSTR)buf, 16);
	delete[] pBuffer;
// 	CFile theFile;
// 	CFileStatus fileStatus;
// 	const int BUFSIZE = 17;
// 	FX_BYTE buf[BUFSIZE];
// 	FX_BYTE *pBuffer = NULL;
// 	char* pBuf = NULL;
// 	int nFileSize = 0;
// 	sbFileValue = CFX_ByteString::FromUnicode(swValue);
// 	int iBufLength = sbFileValue.GetLength();
// 	pBuf = (char*)malloc(sizeof(char) * iBufLength);
// 	pBuf = sbFileValue.GetBuffer(iBufLength);
// 	if (!theFile.Open(swName, CFile::modeWrite | CFile::modeCreate, NULL))
// 		return FALSE;
// 	
// 	theFile.Write(pBuf, iBufLength);
// 	theFile.Close();
// 	
// 	theFile.Open(swName, CFile::modeRead, NULL);
// 	nFileSize = theFile.GetLength();
// 	pBuffer = new FX_BYTE[nFileSize];
// 	theFile.Seek(0, CFile::begin);
// 	theFile.Read(pBuffer, nFileSize);
// 	
// 	SYSTEMTIME CreateDate;
// 	SYSTEMTIME ModDate;
// 	::ZeroMemory(buf, BUFSIZE * sizeof( FX_BYTE ));
// 	theFile.GetStatus(fileStatus);
// 	fileStatus.m_ctime.GetAsSystemTime(CreateDate);
// 	fileStatus.m_mtime.GetAsSystemTime(ModDate);
// 	CRYPT_MD5Generate(pBuffer, nFileSize, buf);
// 	buf[BUFSIZE - 1] = 0;
// 	CFX_WideString csCheckSum((FX_LPCWSTR)buf, 16);

	/* MQG
	CRAO_FileAttachment FileAttach(m_pDocument->GetDocument());
	CPDF_Dictionary* pDictionary = CPDF_Dictionary::Create();
	CRAO_FileSpec FileSpec(pDictionary);

	FileSpec.CreateFileSpecObj(m_pDocument->GetDocument());
	FileSpec.GetFileStream()->InitStream(pBuffer, nFileSize, NULL );
	FileSpec.SetDA_F(PDF_EncodeText(theFile.GetFileName()));
//	FileSpec->SetDA_EmbeddedFileSubtype(GetMIMESubtype(GetFileExt()));		
	FileSpec.SetDA_EmbeddedFileParams((int)fileStatus.m_size, CreateDate, ModDate, csCheckSum );

//	FileSpec.SetDA_EmbeddedFileParams(iBufSize, CreateDate, ModDate, swCryptFilter);
	
//	FileSpec.SetDA_F(PDF_EncodeText(swName));
//	FileSpec.GetFileStream()->InitStream( pBuffer, iBufSize, NULL );

	m_pDocument->SetChangeMark();

	theFile.Close();
	delete []pBuffer;
	free(pBuf);
	*/

	return TRUE;
}

FX_BOOL JDocument::media(OBJ_PROP_PARAMS)
{
// 	ASSERT(m_pDocument != NULL);
// 
// 	CJS_Context* pContext = (CJS_Context *)cc;
// 	CJS_Runtime* pRuntime = pContext->GetJSRuntime();
// 
// 	JSFXObject * pObj = DS_NewFxDynamicObj(pRuntime, pContext, DS_GetObjDefnID(pRuntime, L"doc_media"));
// 	if (!pObj) return FALSE;
// 
// 	CFXJS_doc_media * pJS_doc_media = (CFXJS_doc_media*)DS_GetPrivate(pObj);
// 	if (!pJS_doc_media) return FALSE;
// 
// 	doc_media* pDoc_media = (doc_media*)pJS_doc_media->GetEmbedObject();
// 	if (!pDoc_media)return FALSE;
// //	pDoc_media->SetDocument(m_pDocument);
// 	
// 	vp << pJS_doc_media;
//	return TRUE;

	return TRUE;
}

FX_BOOL JDocument::calculateNow(OBJ_METHOD_PARAMS)
{
	ASSERT(m_pDocument != NULL);

	CPDF_Document* pDoc = m_pDocument->getPDFDocument();
	ASSERT(pDoc != NULL);
	FX_DWORD dwPermissions = pDoc->GetUserPermissions();
	if (!((dwPermissions & FPDFPERM_MODIFY) || (dwPermissions & FPDFPERM_ANNOT_FORM) 
		|| (dwPermissions & FPDFPERM_FILL_FORM)))
		return FALSE;

	FSFormImp* pInterForm= (FSFormImp*)m_pDocument->GetForm();
	ASSERT(pInterForm != NULL);
	CPDF_InterForm* pPDFForm = pInterForm->ST_GetSTInterForm()->m_pInterForm;
	ASSERT(pPDFForm != NULL);

// 	pPDFForm->OnCalculate();
	return TRUE;
}

FX_BOOL JDocument::Collab(OBJ_PROP_PARAMS)
{
	ASSERT(m_pDocument != NULL);
	if(vp.IsGetting())
	{
	CFXJS_Context* pContext = (CFXJS_Context*)cc;
	ASSERT(pContext != NULL);
	CFXJS_Runtime* pRuntime = pContext->GetJSRuntime();
	if(!pRuntime) return TRUE;

	JSFXObject* pObj = DS_NewFxDynamicObj(pRuntime, pContext, DS_GetObjDefnID(pRuntime, L"Collab"));
	ASSERT(pObj != NULL);

	CFXJS_Collab* pJS_Collab = (CFXJS_Collab*)DS_GetPrivate(pObj);
	ASSERT(pJS_Collab != NULL);
		
	vp << pJS_Collab;
	}
	return TRUE;
}

FX_BOOL JDocument::getPageNthWord(OBJ_METHOD_PARAMS)
{
	//if (IsSafeMode(cc)) return TRUE;

	ASSERT(m_pDocument != NULL);
	CPDF_Document* pDoc = m_pDocument->getPDFDocument();
	ASSERT(pDoc != NULL);
	FX_DWORD dwPermissions = pDoc->GetUserPermissions();
	if (!(dwPermissions & FPDFPERM_EXTRACT_ACCESS)) return FALSE;

	int nPageNo = params.GetSize() > 0 ? (int)params[0] : 0;
	int nWordNo = params.GetSize() > 1 ? (int)params[1] : 0;
	bool bStrip = params.GetSize() > 2 ? (bool)params[2] : true;

	if (nPageNo < 0 || nPageNo >= pDoc->GetPageCount())
	{
		//MQG
		//sError = JSGetStringFromID(IDS_STRING_JSPARAMERROR);
		return FALSE;
	}

	CPDF_Dictionary* pPageDict = pDoc->GetPage(nPageNo);
	if (!pPageDict) return FALSE;

	CPDF_Page page;
	page.Load(pDoc, pPageDict);
	page.StartParse();
	page.ParseContent();

	FX_POSITION pos = page.GetFirstObjectPosition();

	int nWords = 0;

	CFX_WideString swRet;

	while (pos)
	{
		if (CPDF_PageObject* pPageObj = page.GetNextObject(pos))
		{
			if (pPageObj->m_Type == PDFPAGE_TEXT)
			{
				int nObjWords = CountWords((CPDF_TextObject*)pPageObj);

				if (nWords + nObjWords >= nWordNo)
				{
					swRet = GetObjWordStr((CPDF_TextObject*)pPageObj, nWordNo - nWords);
					break;
				}

				nWords += nObjWords;
			}
		}
	}

	if (bStrip)
	{
		swRet.TrimLeft();
		swRet.TrimRight();
	}

	vRet = swRet;
	return TRUE;
}

FX_BOOL JDocument::getPageNthWordQuads(OBJ_METHOD_PARAMS)
{
	//if (IsSafeMode(cc)) return TRUE;

	ASSERT(m_pDocument != NULL);
	CPDF_Document* pDoc = m_pDocument->getPDFDocument();
	ASSERT(pDoc != NULL);
	FX_DWORD dwPermissions = pDoc->GetUserPermissions();
	if (!(dwPermissions & FPDFPERM_EXTRACT_ACCESS)) return FALSE;

	return FALSE;
}

FX_BOOL JDocument::getPageNumWords(OBJ_METHOD_PARAMS)
{
	ASSERT(m_pDocument != NULL);
	CPDF_Document* pDoc = m_pDocument->getPDFDocument();
	ASSERT(pDoc != NULL);
	FX_DWORD dwPermissions = pDoc->GetUserPermissions();
	if (!(dwPermissions & FPDFPERM_EXTRACT_ACCESS)) return FALSE;

	FX_INT32 nPageNo = params.GetSize() > 0 ? (int)params[0] : 0;

	if (nPageNo < 0 || nPageNo >= pDoc->GetPageCount())
	{
		//MQG
		//sError = JSGetStringFromID(IDS_STRING_JSPARAMERROR);
		return FALSE;
	}

	CPDF_Dictionary* pPageDict = pDoc->GetPage(nPageNo);
	if (!pPageDict) return FALSE;

	CPDF_Page page;
	page.Load(pDoc, pPageDict);
	page.StartParse();
	page.ParseContent();

	FX_POSITION pos = page.GetFirstObjectPosition();

	int nWords = 0;

	while (pos)
	{
		if (CPDF_PageObject* pPageObj = page.GetNextObject(pos))
		{
			if (pPageObj->m_Type == PDFPAGE_TEXT)
			{
				CPDF_TextObject* pTextObj = (CPDF_TextObject*)pPageObj;
				nWords += CountWords(pTextObj);
			}
		}
	}

	vRet = nWords;

	return TRUE;
}

FX_BOOL JDocument::getPrintParams(OBJ_METHOD_PARAMS)
{
	CFXJS_Context* pContext = (CFXJS_Context*)cc;
	ASSERT(pContext != NULL);
	CFXJS_Runtime* pRuntime = pContext->GetJSRuntime();
	if(!pRuntime) return TRUE;
	JSFXObject* pRetObj = DS_NewFxDynamicObj(pRuntime, pContext, DS_GetObjDefnID(pRuntime, L"PrintParamsObj"));
	ASSERT(pRetObj != NULL);
	//not implemented yet.
	vRet = pRetObj;

	return TRUE;
}

#define ISLATINWORD(u)	(u != 0x20 && u <= 0x28FF)

int	JDocument::CountWords(CPDF_TextObject* pTextObj)
{
	if (!pTextObj) return 0;

	int nWords = 0;

	CPDF_Font* pFont = pTextObj->GetFont();
	if (!pFont) return 0;

	FX_BOOL bIsLatin = FALSE;

	for (int i=0, sz=pTextObj->CountChars(); i<sz; i++)
	{
		FX_DWORD charcode = -1;
		FX_FLOAT kerning;

		pTextObj->GetCharInfo(i, charcode, kerning);
		CFX_WideString swUnicode = pFont->UnicodeFromCharCode(charcode);

		FX_WORD unicode = 0;
		if (swUnicode.GetLength() > 0)
			unicode = swUnicode[0];

		if (ISLATINWORD(unicode) && bIsLatin)
			continue;
		
		bIsLatin = ISLATINWORD(unicode);
		if (unicode != 0x20)
			nWords++;
	}

	return nWords;
}

CFX_WideString JDocument::GetObjWordStr(CPDF_TextObject* pTextObj, int nWordIndex)
{
	ASSERT(pTextObj != NULL);

	CFX_WideString swRet;

	CPDF_Font* pFont = pTextObj->GetFont();
	if (!pFont) return L"";

	int nWords = 0;
	FX_BOOL bIsLatin = FALSE;

	for (int i=0, sz=pTextObj->CountChars(); i<sz; i++)
	{
		FX_DWORD charcode = -1;
		FX_FLOAT kerning;

		pTextObj->GetCharInfo(i, charcode, kerning);
		CFX_WideString swUnicode = pFont->UnicodeFromCharCode(charcode);

		FX_WORD unicode = 0;
		if (swUnicode.GetLength() > 0)
			unicode = swUnicode[0];

		if (ISLATINWORD(unicode) && bIsLatin)
		{
		}
		else
		{		
			bIsLatin = ISLATINWORD(unicode);
			if (unicode != 0x20)
				nWords++;	
		}

		if (nWords-1 == nWordIndex)
			swRet += unicode;
	}

	return swRet;
}

FX_BOOL JDocument::zoom(OBJ_PROP_PARAMS)
{
// 	ASSERT(m_pDocument != NULL);
// 
// 	CPDFSDK_PageView* pDocView = m_pDocument->GetCurrentDocView();
// 	if (!pDocView) return FALSE;
// 	
// 	CPDFSDK_PageView* pPageView = pDocView->GetCurrentPageView();
// 	if (!pPageView) return FALSE;
// 
// 	if (vp.IsGetting())
// 	{
// 		vp << (int)(pPageView->GetPageScale() * 100);
// 	}
// 	else
// 	{
// 		int nZoom = 100;
// 		vp >> nZoom;
// 
// 		PAGESTATE psPageView;
// 		pPageView->GetPageState(psPageView);
// 
// 		psPageView.nFitType = PDFZOOM_XYZ;
// 		psPageView.dest.RemoveAll();
// 		psPageView.dest.Add(-32768);
// 		psPageView.dest.Add(-32768);
// 		psPageView.dest.Add(nZoom / 100.0f);
// 
// 		pPageView->ScalePage(psPageView.nFitType, psPageView.dest);
// 
// 		return TRUE;
// 	}

	return TRUE;
}

/**
(none,	NoVary)
(fitP,	FitPage)
(fitW,	FitWidth)
(fitH,	FitHeight)
(fitV,	FitVisibleWidth)
(pref,	Preferred)
(refW,	ReflowWidth)
*/

FX_BOOL JDocument::zoomType(OBJ_PROP_PARAMS)
{
// 	ASSERT(m_pDocument != NULL);
// 
// 	CReader_DocView* pDocView = m_pDocument->GetCurrentDocView();
// 	if (!pDocView) return FALSE;
// 	
// 	CReader_PageView* pPageView = pDocView->GetCurrentPageView();
// 	if (!pPageView) return FALSE;
// 
// 	if (vp.IsGetting())
// 	{
// 		PAGESTATE ps;
// 		pPageView->GetPageState(ps);
// 
// 		//#define PDFZOOM_XYZ					1			// left, top, zoom
// 		//#define PDFZOOM_FITPAGE				2
// 		//#define PDFZOOM_FITHORZ				3			// top
// 		//#define PDFZOOM_FITVERT				4			// left
// 		//#define PDFZOOM_FITRECT				5			// left, bottom, right, top
// 
// 		CFX_ByteString sZoomType = "NoVary";
// 
// 		switch (ps.nFitType)
// 		{
// 		case PDFZOOM_XYZ:
// 			sZoomType = "Preferred";
// 			break;
// 		case PDFZOOM_FITPAGE:
// 			sZoomType = "FitPage";
// 			break;
// 		case PDFZOOM_FITHORZ:
// 			sZoomType = "FitWidth";
// 			break;
// 		case PDFZOOM_FITVERT:
// 			sZoomType = "FitHeight";
// 			break;
// 		case PDFZOOM_FITRECT:
// 			sZoomType = "Preferred";
// 			break;
// 		}
// 
// 		vp << sZoomType;
// 
// 		ps.dest.RemoveAll();
// 	}
// 	else
// 	{
// 		CFX_ByteString sZoomType = "NoVary";
// 
// 		vp >> sZoomType;
// 
// 		if (sZoomType == "NoVary")
// 		{
// 		}
// 		else if (sZoomType == "FitPage")
// 		{
// 			PAGESTATE ps;
// 			ps.nFitType = PDFZOOM_FITPAGE;
// 			pPageView->ScalePage(ps.nFitType, ps.dest);
// 		}
// 		else if (sZoomType == "FitWidth")
// 		{
// 			PAGESTATE ps;
// 			ps.nFitType = PDFZOOM_FITHORZ;
// 			pPageView->ScalePage(ps.nFitType, ps.dest);
// 		}
// 		else if (sZoomType == "FitHeight")
// 		{
// 			PAGESTATE ps;
// 			ps.nFitType = PDFZOOM_FITVERT;
// 			pPageView->ScalePage(ps.nFitType, ps.dest);
// 		}
// 		else if (sZoomType == "FitVisibleWidth")
// 		{
// 			/*
// 			PAGESTATE ps;
// 			ps.nFitType = PDFZOOM_FITRECT;
// 			pPageView->ScalePage(ps.nFitType, ps.dest);
// 			*/
// 		}
// 		else if (sZoomType == "Preferred")
// 		{
// 		}
// 		else if (sZoomType == "ReflowWidth")
// 		{
// 		}
// 		else
// 			return FALSE;
// 	}

	return TRUE;
}

FX_BOOL JDocument::deletePages(OBJ_METHOD_PARAMS)
{
	CFXJS_Context* pContext = (CFXJS_Context *)cc;
	ASSERT(pContext != NULL);
	
	if (IsSafeMode(cc)) return TRUE;
// 	CFSCRT_LTAppHandler* pApp = FSCRT_GetLTAppHandler();
// 	if (!pApp) return TRUE;
	FSFormFillerImp* pFormFiller = m_pDocument->GetForm()->GetFormFiller();
    if(!pFormFiller) return FALSE;

	ASSERT(m_pDocument != NULL);
	CPDF_Document* pDoc = m_pDocument->getPDFDocument();
	ASSERT(pDoc != NULL);
	FX_DWORD dwPermissions = pDoc->GetUserPermissions();
	if (!((dwPermissions & FPDFPERM_MODIFY) || (dwPermissions & FPDFPERM_ASSEMBLE)))
		return FALSE;

	FX_INT32 iSize = params.size();
	FX_INT32 nStart = 0;
	FX_INT32 nEnd = 0;
	
	if (iSize < 1)
	{
	}
	else if (iSize == 1)
	{
		if (params[0].GetType() == VT_object)
		{
			JSObject * pObj = (JSObject *)params[0];
			if (Value * pValue = DS_GetObjectElement(pObj, L"nStart"))
				nStart = (FX_INT32)CFXJS_Value(pValue,GET_VALUE_TYPE(pValue));

			if (Value * pValue = DS_GetObjectElement(pObj, L"nEnd"))
				nEnd = (FX_INT32)CFXJS_Value(pValue,GET_VALUE_TYPE(pValue));
		}
		else
		{
			nStart = (FX_INT32)params[0];
		}
	}
	else
	{
		nStart = (FX_INT32)params[0];
		nEnd = (FX_INT32)params[1];
	}

	FX_INT32 nTotal = -1;
	m_pDocument->GetPageCount(&nTotal);

	if (nStart < 0)	nStart = 0;
	if (nStart >= nTotal) nStart = nTotal - 1;

	if (nEnd < 0) nEnd = 0;
	if (nEnd >= nTotal) nEnd = nTotal - 1;

	if (nEnd < nStart) nEnd = nStart;

	FX_INT32 nCount = nEnd - nStart + 1;	

// 	return m_pDocument->DeletePages(nStart, nCount);
	return FALSE;
}

FX_BOOL JDocument::extractPages(OBJ_METHOD_PARAMS)
{
	CFXJS_Context* pContext = (CFXJS_Context *)cc;
	ASSERT(pContext != NULL);
	
// 	CFSCRT_LTAppHandler* pApp = FSCRT_GetLTAppHandler();
// 	if (!pApp) return TRUE;

// 	if (IsSafeMode(cc)) return TRUE;
// 
// 	ASSERT(m_pDocument != NULL);
// 	CPDF_Document* pDoc = m_pDocument->getPDFDocument();
// 	ASSERT(pDoc != NULL);
// 	FX_DWORD dwPermissions = pDoc->GetUserPermissions();
// 	if (!(dwPermissions & FPDFPERM_EXTRACT)) return FALSE;
// 
// 	FX_INT32 iSize = params.size();
// 	
// 	FX_INT32 nTotal = -1;
// 	m_pDocument->GetPageCount(&nTotal);
// 	FX_INT32 nStart = 0;
// 	FX_INT32 nEnd = nTotal - 1;
// 
// 	CFX_WideString swFilePath;
// 	
// 	if (iSize < 1)
// 	{
// 	}
// 	else if (iSize == 1)
// 	{
// 		if (params[0].GetType() == VT_object)
// 		{
// 			JSObject * pObj = (JSObject *)params[0];
// 			if (Value * pValue = DS_GetObjectElement(pObj, L"nStart"))
// 				nStart = (FX_INT32)CFXJS_Value(pValue,GET_VALUE_TYPE(pValue));
// 
// 			if (Value * pValue = DS_GetObjectElement(pObj, L"nEnd"))
// 				nEnd = (FX_INT32)CFXJS_Value(pValue,GET_VALUE_TYPE(pValue));
// 
// 			if (Value * pValue = DS_GetObjectElement(pObj, L"cPath"))
// 				swFilePath = (FX_LPCWSTR)CFXJS_Value(pValue,GET_VALUE_TYPE(pValue));
// 		}
// 		else
// 		{
// 			nStart = (FX_INT32)params[0];
// 		}
// 	}
// 	else if (iSize == 2)
// 	{
// 		nStart = (FX_INT32)params[0];
// 		nEnd = (FX_INT32)params[1];
// 	}
// 	else
// 	{
// 		nStart = (FX_INT32)params[0];
// 		nEnd = (FX_INT32)params[1];
// 		swFilePath = (FX_LPCWSTR)params[2];
// 	}

// 	if (nEnd < nStart)
// 		nEnd = nStart;
// 
// 	CPDF_Document *pNewDoc = new CPDF_Document;
// 	pNewDoc->CreateNewDoc();	
// 
// 	CFX_WordArray array;
// 	for (FX_INT32 i=nStart; i<=nEnd; i++)
// 		array.Add(i);

//	m_pDocument->ExtractPages(array, pNewDoc);

//	if (swFilePath.IsEmpty())
//	{
// 		swFilePath = L"temp.pdf";
// 		FSPDFDoc* pReaderDoc = pEnv->OpenMemPDFDoc(pNewDoc, swFilePath);
// 
// 		CJS_Context* pContext = (CJS_Context *)cc;
// 		CJS_Runtime* pRuntime = pContext->GetJSRuntime();
// 	
// 		JSFXObject * pObj = DS_NewFxDynamicObj(pRuntime, pContext, DS_GetObjDefnID(pRuntime, L"Document"));
// 		if (!pObj) return FALSE;
// 		
// 		CJS_Document * pJS_Document = (CJS_Document*)DS_GetPrivate(pObj);
// 		if (!pJS_Document) return FALSE;
// 
// 		Document* pEnbedDoc = (Document*)pJS_Document->GetEmbedObject();
// 		if (!pEnbedDoc)return FALSE;
// 
// 		pEnbedDoc->AttachDoc(pReaderDoc);
// 		pJS_Document->SetEmbedObject((CJS_EmbedObj*)pEnbedDoc);
// 
// 		vRet = pJS_Document;
//	}
//	else
//	{
//		swFilePath = japp::PDFPathToSysPath(swFilePath);
//		CPDF_Creator PDFCreater(pNewDoc);
//		PDFCreater.Create(swFilePath);
//		delete pNewDoc;
//		pEnv->OpenDocument(swFilePath);
//		vRet.SetNull();
//	}

	return TRUE;
}

FX_BOOL JDocument::insertPages(OBJ_METHOD_PARAMS)
{
	CFXJS_Context* pContext = (CFXJS_Context *)cc;
	ASSERT(pContext != NULL);
	
	FSFormFillerImp* pFormFiller = m_pDocument->GetForm()->GetFormFiller();
    if(!pFormFiller) return FALSE;
// 	CFSCRT_LTAppHandler* pApp = FSCRT_GetLTAppHandler();
// 	if (!pApp) return TRUE;

	if (IsSafeMode(cc)) return TRUE;

	ASSERT(m_pDocument != NULL);
	CPDF_Document* pDoc = m_pDocument->getPDFDocument();
	ASSERT(pDoc != NULL);
	FX_DWORD dwPermissions = pDoc->GetUserPermissions();
	if (!((dwPermissions & FPDFPERM_MODIFY) || (dwPermissions & FPDFPERM_ASSEMBLE)))
		return FALSE;

	FX_INT32 iSize = params.size();
	
	FX_INT32 nStart = 0;
	FX_INT32 nEnd = 0;
	FX_INT32 nPage = 0;

	CFX_WideString swFilePath;
	
	if (iSize < 1)
	{
	}
	else if (iSize == 1)
	{
		if (params[0].GetType() == VT_object)
		{
			JSObject * pObj = (JSObject *)params[0];

			if (Value * pValue = DS_GetObjectElement(pObj, L"nPage"))
				nPage = (FX_INT32)CFXJS_Value(pValue,GET_VALUE_TYPE(pValue));

			if (Value * pValue = DS_GetObjectElement(pObj, L"cPath"))
				swFilePath = (FX_LPCWSTR)CFXJS_Value(pValue,GET_VALUE_TYPE(pValue));

			if (Value * pValue = DS_GetObjectElement(pObj, L"nStart"))
				nStart = (FX_INT32)CFXJS_Value(pValue,GET_VALUE_TYPE(pValue));

			if (Value * pValue = DS_GetObjectElement(pObj, L"nEnd"))
				nEnd = (FX_INT32)CFXJS_Value(pValue,GET_VALUE_TYPE(pValue));
		}
		else
		{
			nPage = (FX_INT32)params[0];
		}
	}
	else 
	{
		nPage = (FX_INT32)params[0];

		if (iSize >= 2)
			swFilePath = (FX_LPCWSTR)params[1];

		if (iSize >= 3)
			nStart = (FX_INT32)params[2];

		if (iSize >= 4)
			nEnd = (FX_INT32)params[3];
	}

	nPage++;

	if (nPage < 0)
		nPage = 0;

	FX_INT32 nCount = -1;
	m_pDocument->GetPageCount(&nCount);
	if (nPage > nCount)
		nPage = nCount;

	if (swFilePath.IsEmpty()) return FALSE;

	swFilePath = japp::PDFPathToSysPath(swFilePath);

	CPDF_Parser pdfParser;
	pdfParser.StartParse(swFilePath, FALSE);
	CPDF_Document* pSrcDoc = pdfParser.GetDocument();

	if (!pSrcDoc) 
	{
		pdfParser.CloseParser();
		return FALSE;
	}

	int nTotal = pSrcDoc->GetPageCount();

	if (nStart < 0)	nStart = 0;
	if (nStart >= nTotal) nStart = nTotal - 1;

	if (nEnd < 0) nEnd = 0;
	if (nEnd >= nTotal) nEnd = nTotal - 1;

	if (nEnd < nStart) nEnd = nStart;

	CFX_WordArray array;
	for (FX_INT32 i=nStart; i<=nEnd; i++)
		array.Add(i);

//	m_pDocument->InsertPages(nPage, pSrcDoc, array);

	pdfParser.CloseParser();

	return TRUE;
}

FX_BOOL JDocument::replacePages(OBJ_METHOD_PARAMS)
{
	CFXJS_Context* pContext = (CFXJS_Context *)cc;
	ASSERT(pContext != NULL);
	
// 	CFSCRT_LTAppHandler* pApp = FSCRT_GetLTAppHandler();
// 	if (!pApp) return TRUE;

	if (IsSafeMode(cc)) return TRUE;

	ASSERT(m_pDocument != NULL);
	CPDF_Document* pDoc = m_pDocument->getPDFDocument();
	ASSERT(pDoc != NULL);
	FX_DWORD dwPermissions = pDoc->GetUserPermissions();
	if (!((dwPermissions & FPDFPERM_MODIFY) || (dwPermissions & FPDFPERM_ASSEMBLE)))
		return FALSE;

	FX_INT32 iSize = params.size();
	
	FX_INT32 nStart = -1;
	FX_INT32 nEnd = -1;
	FX_INT32 nPage = 0;

	CFX_WideString swFilePath;
	
	if (iSize < 1)
	{
	}
	else if (iSize == 1)
	{
		if (params[0].GetType() == VT_object)
		{
			JSObject * pObj = (JSObject *)params[0];

			if (Value * pValue = DS_GetObjectElement(pObj, L"nPage"))
				nPage = (FX_INT32)CFXJS_Value(pValue,GET_VALUE_TYPE(pValue));

			if (Value * pValue = DS_GetObjectElement(pObj, L"cPath"))
				swFilePath = (FX_LPCWSTR)CFXJS_Value(pValue,GET_VALUE_TYPE(pValue));

			if (Value * pValue = DS_GetObjectElement(pObj, L"nStart"))
				nStart = (FX_INT32)CFXJS_Value(pValue,GET_VALUE_TYPE(pValue));

			if (Value * pValue = DS_GetObjectElement(pObj, L"nEnd"))
				nEnd = (FX_INT32)CFXJS_Value(pValue,GET_VALUE_TYPE(pValue));
		}
		else
		{
			nPage = (FX_INT32)params[0];
		}
	}
	else 
	{
		nPage = (FX_INT32)params[0];

		if (iSize >= 2)
			swFilePath = (FX_LPCWSTR)params[1];

		if (iSize >= 3)
			nStart = (FX_INT32)params[2];

		if (iSize >= 4)
			nEnd = (FX_INT32)params[3];
	}

	if (nPage < 0)
		nPage = 0;

	FX_INT32 nCount = -1;
	m_pDocument->GetPageCount(&nCount);
	if (nPage > nCount)
		nPage = nCount-1;

	if (swFilePath.IsEmpty()) return FALSE;

	swFilePath = japp::PDFPathToSysPath(swFilePath);

	CPDF_Parser pdfParser;
	pdfParser.StartParse(swFilePath, FALSE);
	CPDF_Document* pSrcDoc = pdfParser.GetDocument();

	if (!pSrcDoc) 
	{
		pdfParser.CloseParser();
		return FALSE;
	}

	int nTotal = pSrcDoc->GetPageCount();

	if (nStart < 0)
	{
		if (nEnd < 0)
		{
			nStart = 0;
			nEnd = nTotal - 1;
		}
		else
		{
			nStart = 0;
		}
	}
	else
	{
		if (nEnd < 0)
		{
			nEnd = nStart;
		}
		else
		{
			if (nStart >= nTotal) nStart = nTotal - 1;
			if (nEnd >= nTotal) nEnd = nTotal - 1;

			if (nEnd < nStart) nEnd = nStart;
		}
	}

	CFX_WordArray array;
	for (FX_INT32 i=nStart; i<=nEnd; i++)
		array.Add(i);

//	m_pDocument->ReplacePages(nPage, pSrcDoc, array);

	pdfParser.CloseParser();

	return TRUE;
}

FX_BOOL JDocument::JS_OpenURL(CFX_WideString& wsURL)
{
	FSFormFillerImp* pFormFiller = m_pDocument->GetForm()->GetFormFiller();
    if(!pFormFiller) return FALSE;
// 	CFSCRT_LTAppHandler* pApp = FSCRT_GetLTAppHandler();
// 	if (!pApp) return FALSE;
#if 0
	FSCRT_BSTR bsDocPath;
	FSCRT_BStr_Init(&bsDocPath);
// 	FSCRT_VAR inParam = {FSCRT_VT_LPVOID, 0, 0};
// 	FSCRT_Var_SetVoidPtrValue(inParam, &bsDocPath);
// 	FSCRT_VAR retVal = {FSCRT_VT_INT32, 0, 0};
// 	FSCRT_Var_SetInt32Value(retVal, 0);
	//typedef FS_INT32	(*FSCRT_LPAPPFunc_OpenURL)(const FSCRT_BSTR* url);
	//FS_RESULT ret = pApp->CallFunction(NULL, FSCRT_APPFUNC_OPENURL, &inParam, 1, &retVal);
	FS_RESULT ret = pFormFiller->m_pJSActionHandle->LaunchURL(&bsDocPath);
	FSCRT_BStr_Clear(&bsDocPath);

	return (FSCRT_ERRCODE_SUCCESS == ret) ? TRUE : FALSE;
#else
	return TRUE;
#endif
}

FX_BOOL JDocument::getURL(OBJ_METHOD_PARAMS)
{
	if (IsSafeMode(cc)) return TRUE;

	CFXJS_Context * pContext = (CFXJS_Context *)cc;
	if (!pContext)return FALSE;

	FSPDFDocImp* pDocument = (FSPDFDocImp*)pContext->GetReaderDocument();
	if (!pDocument)return FALSE;

	FSFormImp *pInterForm = pDocument->GetForm();
	if (!pInterForm) return FALSE;

	CFX_WideString swURL = params.GetSize() > 0 ? (FX_LPCWSTR)params[0] : (FX_LPCWSTR)L"";
	FX_BOOL bAppend = params.GetSize() > 1 ? (bool)params[1] : TRUE;

	CFXJS_Runtime* pRuntime = pContext->GetJSRuntime();
	if(!pRuntime) return TRUE;

// 	CFSCRT_LTAppHandler* pApp = FSCRT_GetLTAppHandler();
// 	if (!pApp) return TRUE;

	pRuntime->BeginBlock();
	FX_BOOL bRet = JS_OpenURL(swURL);
	pRuntime->EndBlock();

	return bRet;
}

void JDocument::AddDelayData(CJS_DelayData* pData)
{
	m_DelayData.Add(pData);
}

void JDocument::DoFieldDelay(const CFX_WideString& sFieldName, int nControlIndex)
{
	CFX_DWordArray DelArray;

	for (int i=0,sz=m_DelayData.GetSize(); i<sz; i++)
	{
		if (CJS_DelayData* pData = m_DelayData.GetAt(i))
		{
			if (!pData->busy && pData->sFieldName == sFieldName && pData->nControlIndex == nControlIndex)
			{
				pData->busy = TRUE;
				JField::DoDelay(m_pDocument, pData);//sometioms this function could be nested, should avoid it.
				pData->busy = FALSE;
				delete pData;
				m_DelayData.SetAt(i, NULL);
				DelArray.Add(i);
			}
		}
	}

	for (int j=DelArray.GetSize()-1; j>=0; j--)
	{
		m_DelayData.RemoveAt(DelArray[j]);
	}
}

void JDocument::AddDelayAnnotData(CFXJS_AnnotObj *pData)
{
	m_DelayAnnotData.Add(pData);
}

void JDocument::DoAnnotDelay()
{
	CFX_DWordArray DelArray;
	
	for (int i=0,sz=m_DelayAnnotData.GetSize(); i<sz; i++)
	{
		if (CFXJS_AnnotObj* pData = m_DelayAnnotData.GetAt(i))
		{
	//		Annotation::DoDelay(m_pDocument,pData);
			if(pData->dashArray)
			{
				delete pData->dashArray;
				pData->dashArray = NULL;
			}
			if(pData->pointsArray)
			{
				delete pData->pointsArray;
				pData->pointsArray = NULL;
			}
			if(pData->quadsArray)
			{
				delete pData->quadsArray;
				pData->quadsArray = NULL;
			}
			if(pData->gesturesArray)
			{
				delete pData->gesturesArray;
				pData->gesturesArray = NULL;
			}
			if(pData->verticesArray)
			{
				delete pData->verticesArray;
				pData->verticesArray = NULL;
			}
			delete pData;
			m_DelayAnnotData.SetAt(i, NULL);
			DelArray.Add(i);
			
		}
	}
	
	for (int j=DelArray.GetSize()-1; j>=0; j--)
	{
		m_DelayData.RemoveAt(DelArray[j]);
	}
}

#endif //defined(_FSDK_ALL_) || defined(_FSDK_JAVASCRIPT_)
