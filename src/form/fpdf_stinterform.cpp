#include "../inc/fpdf_define.h"
#include "fpdf_stinterform.h"
#include "fpdf_stactionhandler.h"
#include "../widget/ifpdf_widget.h"

CFSCRT_STPDFInterForm::CFSCRT_STPDFInterForm(FSForm* form)
{
	m_pLTPDFForm = form;
	m_pActionHandler = NULL;
	m_nCalculate = 0;
	m_bCalculateEnabled = TRUE;
}

CFSCRT_STPDFInterForm::~CFSCRT_STPDFInterForm()
{
	if(m_pInterForm) delete m_pInterForm;
}

FS_RESULT CFSCRT_STPDFInterForm::InitPDFInterForm(CPDF_Document* pPDFDoc)
{
	if (!pPDFDoc) return FSCRT_ERRCODE_ERROR;
	
	CPDF_Dictionary* pRoot = pPDFDoc->GetRoot();
	if (!pRoot) return FSCRT_ERRCODE_ERROR;
	
	CPDF_Dictionary* pAcroForm = pRoot->GetDict(FX_BSTRC("AcroForm"));
	if (!pAcroForm)
	{
		pAcroForm = FX_NEW CPDF_Dictionary;
		if (!pAcroForm) return FSCRT_ERRCODE_ST_OOM;

		pRoot->SetAt("AcroForm", pAcroForm, pPDFDoc);

		m_pInterForm = FX_NEW CPDF_InterForm(pPDFDoc, false);
		if (!m_pInterForm) return FSCRT_ERRCODE_ST_OOM;

		FixPageFields();
	}
	else
	{
		m_pInterForm = FX_NEW CPDF_InterForm(pPDFDoc, false);
		if (!m_pInterForm) return FSCRT_ERRCODE_ST_OOM;
	}

	m_pInterForm->SetFormNotify(this);
	m_pActionHandler = NULL;

	return FSCRT_ERRCODE_SUCCESS;
}

/* ----------------------------- CPDF_FormNotify ----------------------------- */

int	CFSCRT_STPDFInterForm::BeforeValueChange(const CPDF_FormField* pField, CFX_WideString& csValue)
{
	CPDF_FormField* pFormField = (CPDF_FormField*)pField;

	int nType = pFormField->GetFieldType();
	if (nType == FIELDTYPE_COMBOBOX || nType == FIELDTYPE_TEXTFIELD)
	{
		FSPDF_FieldActionParam param;
		param.sValue = csValue;
		param.bWillCommit = TRUE;

		FX_BOOL bRC = TRUE;
		OnKeyStrokeCommit(pFormField, param, bRC);
		if (bRC) 
		{
			OnValidate(pFormField, param, bRC);
			if (bRC)
				return 1;
			else
				return -1;
		}
		else
			return -1;
	}
	
	return 0;
}

int	CFSCRT_STPDFInterForm::AfterValueChange(const CPDF_FormField* pField)
{
	CPDF_FormField* pFormField = (CPDF_FormField*)pField;

	int nType = pFormField->GetFieldType();

	if (nType == FIELDTYPE_COMBOBOX || nType == FIELDTYPE_TEXTFIELD)
	{
		OnCalculate(pFormField);
		FX_BOOL bFormated = FALSE;
		CFX_WideString sValue = OnFormat(pFormField, 0, bFormated);
		
		ResetFieldAppearance(pFormField, bFormated ? (FX_LPCWSTR)sValue : NULL);
	
 		UpdateField(pFormField);
	}

	 return 0;//m_pLTPDFForm->ST_AddFieldValueForRecoverring((CPDF_FormField*)pField);
}

int	CFSCRT_STPDFInterForm::BeforeSelectionChange(const CPDF_FormField* pField, CFX_WideString& csValue)
{
	CPDF_FormField* pFormField = (CPDF_FormField*)pField;

	int nType = pFormField->GetFieldType();
	if (nType == FIELDTYPE_LISTBOX)
	{
		FSPDF_FieldActionParam param;
		param.sValue = csValue;
		param.bWillCommit = TRUE;
		FX_BOOL bRC = TRUE;

		OnKeyStrokeCommit(pFormField, param, bRC);
		if (bRC) 
		{
			OnValidate(pFormField, param, bRC);
			if (bRC)
				return 1;
			else
				return -1;
		}
		else
			return -1;
	}
	else
		return 0;
}

int	CFSCRT_STPDFInterForm::AfterSelectionChange(const CPDF_FormField* pField)
{
	CPDF_FormField* pFormField = (CPDF_FormField*)pField;

	//SynchronizeField(pFormField, FALSE);

	int nType = pFormField->GetFieldType();

	if (nType == FIELDTYPE_LISTBOX)
	{
		this->OnCalculate(pFormField);
		this->ResetFieldAppearance(pFormField, NULL);
		this->UpdateField(pFormField);
	}

	return 0;
}

int	CFSCRT_STPDFInterForm::AfterCheckedStatusChange(const CPDF_FormField* pField, const CFX_ByteArray& statusArray)
{
	CPDF_FormField* pFormField = (CPDF_FormField*)pField;

	//SynchronizeField(pFormField, FALSE);

	int nType = pFormField->GetFieldType();

	if (nType == FIELDTYPE_CHECKBOX || nType == FIELDTYPE_RADIOBUTTON)
	{
		this->OnCalculate(pFormField);
		this->ResetFieldAppearance(pFormField, NULL);
		this->UpdateField(pFormField);
	}

	return 0;
}

int	CFSCRT_STPDFInterForm::BeforeFormReset(const CPDF_InterForm* pForm)
{
 	m_nCalculate++;
// 	SynchronizeDefaultValue();
// 
// 	if (m_pFormNotify)
// 		m_pFormNotify->BeforeFormReset(this);
	return 0;
}

int	CFSCRT_STPDFInterForm::AfterFormReset(const CPDF_InterForm* pForm)
{
	m_nCalculate--;
	this->OnCalculate(NULL);
// 
// 	if (m_pFormNotify)
// 		m_pFormNotify->AfterFormReset(this);

	return 0;
}

int	CFSCRT_STPDFInterForm::BeforeFormImportData(const CPDF_InterForm* pForm)
{
	m_nCalculate++;
	return 0;
}

int	CFSCRT_STPDFInterForm::AfterFormImportData(const CPDF_InterForm* pForm)
{
 	m_nCalculate--;
 	this->OnCalculate(NULL);
	return 0;
}

void CFSCRT_STPDFInterForm::OnKeyStrokeCommit(CPDF_FormField* pFormField, FSPDF_FieldActionParam& param, FX_BOOL& bRC)
{
	ASSERT(pFormField != NULL);

	CPDF_AAction aAction = pFormField->GetAdditionalAction();
	if (aAction != NULL && aAction.ActionExist(CPDF_AAction::KeyStroke)) 
	{
		CPDF_Action action = aAction.GetAction(CPDF_AAction::KeyStroke);
		if (!m_pActionHandler && m_pLTPDFForm && ((FSFormImp*)m_pLTPDFForm)->GetFormFiller())
		{
			m_pActionHandler = ((FSFormImp*)m_pLTPDFForm)->GetFormFiller()->GetActionHandler();
		}
		if (action && m_pActionHandler)
		{		
			m_pActionHandler->DoAction_FieldJavaScript(action, CPDF_AAction::KeyStroke, 
				((FSFormImp*)m_pLTPDFForm)->GetPDFDoc(), pFormField, param);
			bRC = param.bRC;
		}
	}
}

void CFSCRT_STPDFInterForm::OnValidate(CPDF_FormField* pFormField, FSPDF_FieldActionParam& param, FX_BOOL& bRC)
{
	ASSERT(pFormField != NULL);

	CPDF_AAction aAction = pFormField->GetAdditionalAction();
	if (aAction != NULL && aAction.ActionExist(CPDF_AAction::Validate)) 
	{
		CPDF_Action action = aAction.GetAction(CPDF_AAction::Validate);
		if (!m_pActionHandler && m_pLTPDFForm && ((FSFormImp*)m_pLTPDFForm)->GetFormFiller())
		{
			m_pActionHandler = ((FSFormImp*)m_pLTPDFForm)->GetFormFiller()->GetActionHandler();
		}
		if (action && m_pActionHandler)
		{		
			m_pActionHandler->DoAction_FieldJavaScript(action, CPDF_AAction::Validate, ((FSFormImp*)m_pLTPDFForm)->GetPDFDoc(), pFormField, param);
			bRC = param.bRC;
		}
	}
}

CFX_WideString		CFSCRT_STPDFInterForm::OnFormat(CPDF_FormField* pFormField, int nCommitKey, FX_BOOL& bFormated,FX_BOOL* bFormatAction)
{
	CFX_WideString sValue = pFormField->GetValue();

	FSPDFDocImp* pLTPDFDoc = (FSPDFDocImp*)((FSFormImp*)m_pLTPDFForm)->GetPDFDoc();
	//FSPDFDoc* pLTPDFDoc = NULL;
	//m_pLTPDFForm->GetDocument(&pLTPDFDoc);

	IJS_Runtime* pRuntime = pLTPDFDoc->GetJsRuntime();
	if(!pRuntime) return sValue;

	pRuntime->SetReaderDocument(pLTPDFDoc);

	if (pFormField->GetFieldType() == FIELDTYPE_COMBOBOX)
	{
		if (pFormField->CountSelectedItems() > 0)
		{
			int index = pFormField->GetSelectedIndex(0);
			if (index >= 0)
				sValue = pFormField->GetOptionLabel(index);
		}
	}

	bFormated = FALSE;

	CPDF_AAction aAction = pFormField->GetAdditionalAction();
	if (aAction != NULL && aAction.ActionExist(CPDF_AAction::Format)) 
	{
		CPDF_Action action = aAction.GetAction(CPDF_AAction::Format);
		if (action)
		{	
			if (bFormatAction)
			{
				*bFormatAction = TRUE;
			}
			CFX_WideString script = action.GetJavaScript();
			if (!script.IsEmpty())
			{
				CFX_WideString Value = sValue;

				IJS_Context* pContext = pRuntime->NewContext();
				ASSERT(pContext != NULL);

				pContext->OnField_Format(nCommitKey, pFormField, Value, TRUE);

				CFX_WideString sInfo;
				FX_BOOL bRet = pContext->RunScript(script, sInfo);
				pRuntime->ReleaseContext(pContext);

				if (bRet)
				{
					sValue = Value;
					bFormated = TRUE;
				}
			}
		}
	}
	return sValue;
}

class CFSCRT_AutoInt
{
public:
	CFSCRT_AutoInt(FX_DWORD* pCount)
	{
		m_pData = pCount;
		(*m_pData)++;
	}
	~CFSCRT_AutoInt()
	{
		(*m_pData)--;
	}
protected:
	FX_DWORD* m_pData;
};

void CFSCRT_STPDFInterForm::OnCalculate(CPDF_FormField* pFormField)
{
	if (m_nCalculate > 0) return;

	CFSCRT_AutoInt autoCalc(&m_nCalculate);

	if (!IsCalculateEnabled()) return;
	
	FSPDFDocImp* pLTPDFDoc = (FSPDFDocImp*)((FSFormImp*)m_pLTPDFForm)->GetPDFDoc();
	//FSPDFDoc* pLTPDFDoc = NULL;
	//m_pLTPDFForm->GetDocument(&pLTPDFDoc);

	if (IJS_Runtime* pRuntime = pLTPDFDoc->GetJsRuntime())
	{
		pRuntime->SetReaderDocument(pLTPDFDoc);

		int nSize = m_pInterForm->CountFieldsInCalculationOrder();
		for (int i=0; i<nSize; i++)
		{
			if (CPDF_FormField* pField = m_pInterForm->GetFieldInCalculationOrder(i))
			{
				int nType = pField->GetFieldType();
				if (nType == FIELDTYPE_COMBOBOX || nType == FIELDTYPE_TEXTFIELD)
				{
					CPDF_AAction aAction = pField->GetAdditionalAction();
					if (aAction && aAction.ActionExist(CPDF_AAction::Calculate))
					{
						CPDF_Action action = aAction.GetAction(CPDF_AAction::Calculate);
						if (action)
						{
							CFX_WideString csJS = action.GetJavaScript();
							if (!csJS.IsEmpty())
							{
								IJS_Context* pContext = pRuntime->NewContext();
								ASSERT(pContext != NULL);

								CFX_WideString sOldValue = pField->GetValue();
								CFX_WideString sValue = sOldValue;
								FX_BOOL bRC = TRUE;
								pContext->OnField_Calculate(pFormField, pField, sValue, bRC);

								CFX_WideString sInfo;
								FX_BOOL bRet = pContext->RunScript(csJS, sInfo);
								pRuntime->ReleaseContext(pContext);

								if (bRet)
								{
									if (bRC)
									{
										if (sValue.Compare(sOldValue) != 0)
											pField->SetValue(sValue, TRUE);
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

void CFSCRT_STPDFInterForm::EnableCalculate(FX_BOOL bEnabled)
{
	m_bCalculateEnabled = bEnabled;
}

FX_BOOL CFSCRT_STPDFInterForm::IsCalculateEnabled()
{
	return m_bCalculateEnabled;
}

FX_BOOL CFSCRT_STPDFInterForm::ExportFieldsToFDFTextBuf(const CFX_PtrArray& fields,FX_BOOL bIncludeOrExclude, CFX_ByteTextBuf& textBuf)
{
	ASSERT(m_pInterForm != NULL);
	FSFormFillerImp* pFormFiller = ((FSFormImp*)m_pLTPDFForm)->GetFormFiller();

	CFX_WideString wsFilePath;

	//FSCRT_BSTR filePath;
	FS_RESULT ret = FSCRT_ERRCODE_SUCCESS;
	/*
	FSCRT_BStr_Init(&filePath);
	FSPDFDoc* pLTPDFDoc = NULL;
	m_pLTPDFForm->GetDocument(&pLTPDFDoc);
	pFormFiller->m_pJSActionHandle->GetFilePath((FSCRT_DOCUMENT)pLTPDFDoc, &filePath);
	wsFilePath = CFX_WideString::FromUTF8(filePath.str, filePath.len);
	FSCRT_BStr_Clear(&filePath);

	CFDF_Document* pFDF = m_pInterForm->ExportToFDF(wsFilePath,(CFX_PtrArray&)fields, bIncludeOrExclude);
	if (!pFDF) return FALSE;
	FX_BOOL bRet = pFDF->WriteBuf(textBuf); // = FALSE;//
	delete pFDF;
	*/
	return TRUE;
}

FX_BOOL CFSCRT_STPDFInterForm::FDFToURLEncodedData(FX_LPBYTE& pBuf, FX_STRSIZE& nBufSize)
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

			fdfEncodedData = fdfEncodedData<<name_b.GetBuffer(name_b.GetLength());
			name_b.ReleaseBuffer();
			fdfEncodedData = fdfEncodedData<<"=";
			fdfEncodedData = fdfEncodedData<<csValue_b.GetBuffer(csValue_b.GetLength());
			csValue_b.ReleaseBuffer();
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
FX_BOOL CFSCRT_STPDFInterForm::SubmitForm(const CFX_WideString& sDestination, FX_BOOL bUrlEncoded)
{
	if (sDestination.IsEmpty()) return FALSE;

	FSFormFillerImp* pFormFiller = ((FSFormImp*)m_pLTPDFForm)->GetFormFiller();
/*
	CFX_WideString wsPDFFilePath;

	FSCRT_BSTR filePath;
	FS_RESULT ret = FSCRT_ERRCODE_SUCCESS;
	FSCRT_BStr_Init(&filePath);
	FSPDFDoc* pLTPDFDoc = NULL;
	m_pLTPDFForm->GetDocument(&pLTPDFDoc);
	pFormFiller->m_pJSActionHandle->GetFilePath((FSCRT_DOCUMENT)pLTPDFDoc, &filePath);
	wsPDFFilePath = CFX_WideString::FromUTF8(filePath.str, filePath.len);
	FSCRT_BStr_Clear(&filePath);


	if(NULL == m_pInterForm) return FALSE;
	CFDF_Document* pFDFDoc = m_pInterForm->ExportToFDF(wsPDFFilePath);
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

	CFX_ByteString utf8Dest = sDestination.UTF8Encode();
	const FSCRT_BSTR dest = {(FX_LPSTR)(FX_LPCSTR)utf8Dest, (FX_DWORD)utf8Dest.GetLength()};
	pLTPDFDoc = NULL;
	m_pLTPDFForm->GetDocument(&pLTPDFDoc);
	pFormFiller->m_pJSActionHandle->SubmitForm((FSCRT_DOCUMENT)pLTPDFDoc, pBuffer, nBufSize, &dest);

	if (bUrlEncoded && pBuffer)
	{
		FX_Free(pBuffer);
		pBuffer = NULL;	
	}
	*/
	return TRUE;
}

FX_BOOL CFSCRT_STPDFInterForm::SubmitFields(const CFX_WideString& csDestination, const CFX_PtrArray& fields,
	FX_BOOL bIncludeOrExclude, FX_BOOL bUrlEncoded)
{
	// 	CFX_WideString csFile = GetTemporaryFileName(L".FDF");
	// 	if (!ExportFieldsToFDFFile(csFile, fields, bIncludeOrExclude))
	// 	{
	// 	//	DeleteFile(csFile);
	// 		return FALSE;
	// 	}

	CFX_ByteTextBuf textBuf;
	ExportFieldsToFDFTextBuf(fields, bIncludeOrExclude, textBuf);

	FX_LPBYTE pBuffer = textBuf.GetBuffer();
	FX_STRSIZE nBufSize = textBuf.GetLength();

	if (bUrlEncoded)
	{
		if(!FDFToURLEncodedData(pBuffer, nBufSize))
			return FALSE;
	}
/*
	FSFormFillerImp* pFormFiller = m_pLTPDFForm->GetFormFiller();

	FS_RESULT ret = FSCRT_ERRCODE_SUCCESS;
	CFX_ByteString utf8Dest = csDestination.UTF8Encode();
	const FSCRT_BSTR dest = {(FX_LPSTR)(FX_LPCSTR)utf8Dest, (FX_DWORD)utf8Dest.GetLength()};
	FSPDFDoc* pLTPDFDoc = NULL;
	m_pLTPDFForm->GetDocument(&pLTPDFDoc);
	pFormFiller->m_pJSActionHandle->SubmitForm((FSCRT_DOCUMENT)pLTPDFDoc, pBuffer, nBufSize, &dest);

	if (bUrlEncoded && pBuffer)
	{
		FX_Free(pBuffer);
		pBuffer = NULL;	
	}
*/
	return TRUE;
}


FX_BOOL	CFSCRT_STPDFInterForm::DoAction_SubmitForm(const CPDF_Action& action)
{
	ASSERT(action != NULL);
	ASSERT(m_pInterForm != NULL);

	CFX_WideString sDestination = action.GetFilePath();
	if (sDestination.IsEmpty()) return FALSE;

	CPDF_Dictionary* pActionDict = action;
	if (pActionDict->KeyExist("Fields"))
	{
		CPDF_ActionFields af = action.GetWidgets();
		FX_DWORD dwFlags = action.GetFlags();
		
		CFX_PtrArray fieldObjects;
		af.GetAllFields(fieldObjects);
		CFX_PtrArray fields;
		GetFieldFromObjects(fieldObjects, fields);
		
		if (fields.GetSize() != 0)
		{
			FX_BOOL bIncludeOrExclude = !(dwFlags & 0x01);
			if (CPDF_FormField* pFormField = m_pInterForm->CheckRequiredFields(&fields, bIncludeOrExclude))
			{
				/*
				HWND hWnd = NULL;
				CReader_PageView *pPageView = this->m_pDocEx->GetCurrentPageView();
				if (pPageView != NULL) hWnd = pPageView->GetPageViewWnd();
				CFX_WideString csError, csTitle, csInfo;
				csTitle = CReader_Language::LoadString(IDS_FIELDREQUIRED_TITILE);
				csError = CReader_Language::LoadString(IDS_FIELDREQUIRED_INFO);
				CFX_WideString csWFieldName = pField->GetFullName();
			//	CFX_WideString csBFieldName = csWFieldName;
				csInfo.Format(csError, (FX_LPCWSTR)csWFieldName);
				MessageBox(hWnd, csInfo, csTitle, MB_OK);
				*/
				return FALSE;
			}
			return SubmitFields(sDestination, fields, bIncludeOrExclude, FALSE);
		}
		else
		{
			if (CPDF_FormField* pFormField = m_pInterForm->CheckRequiredFields())
			{
				return FALSE;
			}

			return SubmitForm(sDestination, FALSE);
		}
	}
	else
	{
		if (CPDF_FormField* pFormField = m_pInterForm->CheckRequiredFields())
		{
			return FALSE;
		}

		return SubmitForm(sDestination, FALSE);
	}
}

FX_BOOL	CFSCRT_STPDFInterForm::DoAction_ResetForm(const CPDF_Action& action)
{
	ASSERT(action != NULL);

	CPDF_Dictionary* pActionDict = action;

	if (pActionDict->KeyExist("Fields"))
	{
		CPDF_ActionFields af = action.GetWidgets();
		FX_DWORD dwFlags = action.GetFlags();

		CFX_PtrArray fieldObjects;
		af.GetAllFields(fieldObjects);
		CFX_PtrArray fields;
		GetFieldFromObjects(fieldObjects, fields);

		return m_pInterForm->ResetForm(fields, !(dwFlags & 0x01), TRUE);
	}
	else
	{
		return m_pInterForm->ResetForm(TRUE);
	}
}

void CFSCRT_STPDFInterForm::GetFieldFromObjects(const CFX_PtrArray& objects, CFX_PtrArray& fields)
{
	int iCount = objects.GetSize();
	for (int i = 0; i < iCount; i ++)
	{
		CPDF_Object* pObject = (CPDF_Object*)objects[i];
		if (pObject == NULL) continue;

		int iType = pObject->GetType();
		if (iType == PDFOBJ_STRING)
		{
			CFX_WideString csName = pObject->GetUnicodeText();
			CPDF_FormField* pField = m_pInterForm->GetField(0, csName);
			if (pField != NULL)
				fields.Add(pField);
		}
		else if (iType == PDFOBJ_DICTIONARY)
		{
			if (m_pInterForm->IsValidFormField(pObject))
				fields.Add(pObject);
		}
	}
}

void CFSCRT_STPDFInterForm::UpdateField(CPDF_FormField* pFormField)
{
	ASSERT(pFormField != NULL);

	for (int i=0,sz=pFormField->CountControls(); i<sz; i++)
	{
		CPDF_FormControl* pFormCtrl = pFormField->GetControl(i);
		ASSERT(pFormCtrl != NULL);

		if(FSFormControl* pWidget = GetLTFormControl(pFormCtrl))
		{
			FSFormFillerImp* pFormFiller = ((FSFormImp*)m_pLTPDFForm)->GetFormFiller();
			if(!pFormFiller) return;
			IFSPDF_AnnotHandler* pWidgetHandler = pFormFiller->GetWidgetHandler();

			FSPDFDoc* pLTPDFDoc = ((FSFormImp*)m_pLTPDFForm)->GetPDFDoc();
			//m_pLTPDFForm->GetDocument(&pLTPDFDoc);

			FSPDFPage* pPageView = pWidget->getPage();

			FX_RECT rcBBox;
			pWidgetHandler->GetViewBBox(pPageView, pWidget, rcBBox);
			FSFormFillerAssist* pFormFillerAssist = pFormFiller->GetFormFillerAssist();
			if(pFormFillerAssist)
			{
				FSRectF fBBox = {(FX_FLOAT)rcBBox.left, (FX_FLOAT)rcBBox.top, (FX_FLOAT)rcBBox.right, (FX_FLOAT)rcBBox.bottom};
				pFormFillerAssist->refresh(pPageView, &fBBox);
			}
		}
	}
}

void CFSCRT_STPDFInterForm::ResetFieldAppearance(CPDF_FormField* pFormField,FX_LPCWSTR sValue)
{
	for(int i=0; i<pFormField->CountControls(); i++)
	{
		CPDF_FormControl* pControl = pFormField->GetControl(i);
		IFSPDF_Widget::ResetAppearance(pControl, sValue);
		
		if(FSFormControl* pWidget = GetLTFormControl(pControl))
			((FSFormControlImp*)pWidget)->clearCachedAppearance();
	}
}

int CFSCRT_STPDFInterForm::GetPageIndexByAnnotDict(CPDF_Document* pDocument, CPDF_Dictionary* pAnnotDict) const
{
	ASSERT(pDocument != NULL);
	ASSERT(pAnnotDict != NULL);

	for (int i=0,sz=pDocument->GetPageCount(); i<sz; i++)
	{
		if (CPDF_Dictionary* pPageDict = pDocument->GetPage(i))
		{			
			if (CPDF_Array* pAnnots = pPageDict->GetArray("Annots"))
			{
				for (int j=0,jsz=pAnnots->GetCount(); j<jsz; j++)
				{
					CPDF_Object* pDict = pAnnots->GetElementValue(j);
					if (pAnnotDict == pDict)
					{
						return i;
					}
				}
			}
		}
	}

	return -1;
}

FSFormControl* CFSCRT_STPDFInterForm::GetLTFormControl(CPDF_FormControl* pControl) const
{	
	ASSERT(m_pInterForm != NULL);
	ASSERT(pControl != NULL);

	FSFormControl* pWidget = NULL;
	m_Map.Lookup(pControl, pWidget);

	if (pWidget) return pWidget;

	CPDF_Dictionary* pControlDict = pControl->GetWidget();
	ASSERT(pControlDict != NULL);

	FSPDFDoc* pLTPDFDoc = ((FSFormImp*)m_pLTPDFForm)->GetPDFDoc();
	//m_pLTPDFForm->GetDocument(&pLTPDFDoc);

	ASSERT(pLTPDFDoc != NULL);
	CPDF_Document* pDocument = ((FSPDFDocImp*)pLTPDFDoc)->getPDFDocument();

	FSPDFPage* pPage = NULL;

	if (CPDF_Dictionary* pPageDict = pControlDict->GetDict("P"))
	{
		int nPageIndex = pDocument->GetPageIndex(pPageDict->GetObjNum());
		if (nPageIndex >= 0)
		{
			pPage = pLTPDFDoc->getPage(nPageIndex);
		}
	}

	if (!pPage) 
	{
		int nPageIndex = GetPageIndexByAnnotDict(pDocument, pControlDict);
		if (nPageIndex >= 0)
		{
			pPage = pLTPDFDoc->getPage(nPageIndex);
		}
	}

	if (pPage)
	{
		FSFormControl* pLTControl = ((FSPDFPageImp*)pPage)->getControlByDict(pControlDict);
		return pLTControl;
	}

	return NULL;

}

//currently, we don't know the fields in CPDF_InterForm growing linear. If so it's unnecessary to retrieve whole fields array.
FS_RESULT CFSCRT_STPDFInterForm::FixPageFields(CPDF_Dictionary* pPageDict)
{
	CPDF_Document* pPDFDoc = m_pInterForm->GetDocument();
	CPDF_Dictionary*pAcroForm = pPDFDoc->GetRoot()->GetDict("AcroForm");

	if (pPageDict == NULL)
	{
		int pageCount = pPDFDoc->GetPageCount();

		for (int i = 0; i < pageCount; i++)
		{
			CPDF_Dictionary* pPageDict = pPDFDoc->GetPage(i);
			CPDF_Page page;
			page.Load(pPDFDoc, pPageDict);
			m_pInterForm->FixPageFields(&page);
		}
	}
	else{
		CPDF_Page page;
		page.Load(pPDFDoc, pPageDict);
		m_pInterForm->FixPageFields(&page);
	}

	CPDF_Array* pFields = FX_NEW CPDF_Array;
	if (!pFields) return FSCRT_ERRCODE_ST_OOM;

	pAcroForm->SetAt("Fields", pFields, pPDFDoc);
	for (FX_DWORD i=0; i<m_pInterForm->CountFields(); i++)
	{
		CPDF_FormField* pField = m_pInterForm->GetField(i);
		if (NULL == pField)
			continue;

		FX_DWORD  dwObjNum = pField->GetFieldDict()->GetObjNum();
		pFields->AddReference(pPDFDoc, dwObjNum);
	}
	return FSCRT_ERRCODE_SUCCESS;
}
