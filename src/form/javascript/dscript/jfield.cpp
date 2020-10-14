#include "js_fxlib.h"
#if defined(_FSDK_ALL_) || defined(_FSDK_JAVASCRIPT_)

#include "field.h"
#include "document.h"
#include "color.h"
#include "publicmethods.h"
#include "icon.h"

//for all fields
#define FIELDFLAG_READONLY					1
#define FIELDFLAG_REQUIRED					2
#define FIELDFLAG_NOEXPORT					4
//for text fields
#define FIELDFLAG_MULTILINE					(1<<12)
#define FIELDFLAG_PASSWORD					(1<<13)
#define FIELDFLAG_FILESELECT				(1<<20)
#define FIELDFLAG_DONOTSPELLCHECK			(1<<22)
#define FIELDFLAG_DONOTSCROLL				(1<<23)
#define FIELDFLAG_COMB						(1<<24)
#define FIELDFLAG_RICHTEXT					(1<<25)
//for button fileds
#define FIELDFLAG_NOTOGGLETOOFF				(1<<14)
#define FIELDFLAG_RADIO						(1<<15)
#define FIELDFLAG_PUSHBUTTON				(1<<16)
#define FIELDFLAG_RADIOSINUNISON			(1<<25)
//for choice fields
#define FIELDFLAG_COMBO						(1<<17)
#define FIELDFLAG_EDIT						(1<<18)
#define FIELDFLAG_SORT						(1<<19)
#define FIELDFLAG_MULTISELECT				(1<<21)
#ifndef FIELDFLAG_DONOTSPELLCHECK
#define FIELDFLAG_DONOTSPELLCHECK			(1<<22)
#endif
#define FIELDFLAG_COMMITONSELCHANGE			(1<<26)

/* ---------------------- Field ---------------------- */

BEGIN_JS_STATIC_CONST(CFXJS_Field)
END_JS_STATIC_CONST()

BEGIN_JS_STATIC_PROP(CFXJS_Field)
	JS_STATIC_PROP_ENTRY(alignment)
	JS_STATIC_PROP_ENTRY(borderStyle)
	JS_STATIC_PROP_ENTRY(buttonAlignX)
	JS_STATIC_PROP_ENTRY(buttonAlignY)
	JS_STATIC_PROP_ENTRY(buttonFitBounds)
	JS_STATIC_PROP_ENTRY(buttonPosition)
	JS_STATIC_PROP_ENTRY(buttonScaleHow)
	JS_STATIC_PROP_ENTRY(buttonScaleWhen)
	JS_STATIC_PROP_ENTRY(calcOrderIndex)
	JS_STATIC_PROP_ENTRY(charLimit)
	JS_STATIC_PROP_ENTRY(comb)
	JS_STATIC_PROP_ENTRY(commitOnSelChange)
	JS_STATIC_PROP_ENTRY(currentValueIndices)
	JS_STATIC_PROP_ENTRY(defaultStyle)
	JS_STATIC_PROP_ENTRY(defaultValue)
	JS_STATIC_PROP_ENTRY(doNotScroll)
	JS_STATIC_PROP_ENTRY(doNotSpellCheck)
	JS_STATIC_PROP_ENTRY(delay)
	JS_STATIC_PROP_ENTRY(display)
	JS_STATIC_PROP_ENTRY(doc)
	JS_STATIC_PROP_ENTRY(editable)
	JS_STATIC_PROP_ENTRY(exportValues)
	JS_STATIC_PROP_ENTRY(hidden)
	JS_STATIC_PROP_ENTRY(fileSelect)
	JS_STATIC_PROP_ENTRY(fillColor)
	JS_STATIC_PROP_ENTRY(lineWidth)
	JS_STATIC_PROP_ENTRY(highlight)
	JS_STATIC_PROP_ENTRY(multiline)
	JS_STATIC_PROP_ENTRY(multipleSelection)
	JS_STATIC_PROP_ENTRY(name)
	JS_STATIC_PROP_ENTRY(numItems)
	JS_STATIC_PROP_ENTRY(page)
	JS_STATIC_PROP_ENTRY(password)
	JS_STATIC_PROP_ENTRY(print)
	JS_STATIC_PROP_ENTRY(radiosInUnison)
	JS_STATIC_PROP_ENTRY(readonly)
	JS_STATIC_PROP_ENTRY(rect)
	JS_STATIC_PROP_ENTRY(required)
	JS_STATIC_PROP_ENTRY(richText)
	JS_STATIC_PROP_ENTRY(richValue)
	JS_STATIC_PROP_ENTRY(rotation)
	JS_STATIC_PROP_ENTRY(strokeColor)
	JS_STATIC_PROP_ENTRY(style)
	JS_STATIC_PROP_ENTRY(submitName)
	JS_STATIC_PROP_ENTRY(textColor)
	JS_STATIC_PROP_ENTRY(textFont)
	JS_STATIC_PROP_ENTRY(textSize)
	JS_STATIC_PROP_ENTRY(type)
	JS_STATIC_PROP_ENTRY(userName)
	JS_STATIC_PROP_ENTRY(value)
	JS_STATIC_PROP_ENTRY(valueAsString)
	JS_STATIC_PROP_ENTRY(source)
END_JS_STATIC_PROP()

BEGIN_JS_STATIC_METHOD(CFXJS_Field)
	JS_STATIC_METHOD_ENTRY(browseForFileToSubmit,      0)
	JS_STATIC_METHOD_ENTRY(buttonGetCaption,           1)
	JS_STATIC_METHOD_ENTRY(buttonGetIcon,              1)
	JS_STATIC_METHOD_ENTRY(buttonImportIcon,           0)
	JS_STATIC_METHOD_ENTRY(buttonSetCaption,           2)
	JS_STATIC_METHOD_ENTRY(buttonSetIcon,              2)
	JS_STATIC_METHOD_ENTRY(checkThisBox,               2)
	JS_STATIC_METHOD_ENTRY(clearItems,                 0)
	JS_STATIC_METHOD_ENTRY(defaultIsChecked,           2)
	JS_STATIC_METHOD_ENTRY(deleteItemAt,               1)
	JS_STATIC_METHOD_ENTRY(getArray ,                  0)
	JS_STATIC_METHOD_ENTRY(getItemAt,                  0)
	JS_STATIC_METHOD_ENTRY(getLock,                    0)
	JS_STATIC_METHOD_ENTRY(insertItemAt,               0)
	JS_STATIC_METHOD_ENTRY(isBoxChecked,               1)
	JS_STATIC_METHOD_ENTRY(isDefaultChecked,           1)
	JS_STATIC_METHOD_ENTRY(setAction,                  2)
	JS_STATIC_METHOD_ENTRY(setFocus,                   0)
	JS_STATIC_METHOD_ENTRY(setItems,                   1)
	JS_STATIC_METHOD_ENTRY(setLock,                    0)
	JS_STATIC_METHOD_ENTRY(signatureGetModifications,  0)
	JS_STATIC_METHOD_ENTRY(signatureGetSeedValue,      0)
	JS_STATIC_METHOD_ENTRY(signatureInfo,              0)
	JS_STATIC_METHOD_ENTRY(signatureSetSeedValue,      0)
	JS_STATIC_METHOD_ENTRY(signatureSign,              0)
	JS_STATIC_METHOD_ENTRY(signatureValidate,          0)
END_JS_STATIC_METHOD()

IMPLEMENT_JS_CLASS(CFXJS_Field, JField, Field)

JField::JField(CFXJS_Object* pJSObject): CFXJS_EmbedObj(pJSObject),
	m_pJSDoc(NULL),
	m_pDocument(NULL),
	m_nFormControlIndex(-1),
	m_bCanSet(FALSE),
	m_bDelay(FALSE)
{
}

JField::~JField()
{
}

extern FX_INT32 find_last_of(const CFX_WideString& wsString, FX_WCHAR wch);
void JField::ParseFieldName(const CFX_WideString &strFieldNameParsed,CFX_WideString &strFieldName,FX_INT32 & iControlNo)
{
	FX_INT32 iStart = find_last_of(strFieldNameParsed, (FX_WCHAR)L'.');
	if (iStart == -1)
	{
		strFieldName = strFieldNameParsed;
		iControlNo = -1;
		return;
	}
	CFX_WideString suffixal = strFieldNameParsed.Mid(iStart+1);
	iControlNo = FXSYS_wtoi((FX_LPCWSTR)suffixal);
	if (iControlNo == 0)
	{
		FX_INT32 iStart;
		while((iStart = find_last_of(suffixal, (FX_WCHAR)L' ')) != -1)
		{
			suffixal.Delete(iStart,1);
		}

		if (suffixal.Compare(L"0") != 0)
		{
			strFieldName = strFieldNameParsed;
			iControlNo = -1;
			return;
		}

	}
	strFieldName = strFieldNameParsed.Mid(0,iStart);    
}

FX_BOOL JField::AttachField(JDocument* pDocument, const CFX_WideString& csFieldName)
{
	ASSERT(pDocument != NULL);
	m_pJSDoc = pDocument;

	m_pDocument = (FSPDFDocImp*)pDocument->GetReaderDoc();
	ASSERT(m_pDocument != NULL);
	CPDF_Document* pDoc = m_pDocument->getPDFDocument();
	ASSERT(pDoc != NULL);
	FX_DWORD dwPermissions = pDoc->GetUserPermissions();
	m_bCanSet = (dwPermissions & FPDFPERM_MODIFY) || (dwPermissions & FPDFPERM_ANNOT_FORM) 
		|| (dwPermissions & FPDFPERM_FILL_FORM);

	FSFormImp* pInterForm= (FSFormImp*)m_pDocument->GetForm();
	ASSERT(pInterForm != NULL);
	CPDF_InterForm* pPDFForm = pInterForm->ST_GetSTInterForm()->m_pInterForm;
	ASSERT(pPDFForm != NULL);

	CFX_WideString swFieldNameTemp = csFieldName;
	swFieldNameTemp.Replace((FX_LPCWSTR)L"..", (FX_LPCWSTR)L".");

	if (pPDFForm->CountFields(swFieldNameTemp) <= 0)
	{
		CFX_WideString strFieldName;
		FX_INT32 iControlNo = -1;
		ParseFieldName((FX_LPCWSTR)swFieldNameTemp, strFieldName, iControlNo);
		if (iControlNo == -1) return FALSE;
		
		m_FieldName = strFieldName;
		m_nFormControlIndex = iControlNo;
		return TRUE;
	}

	m_FieldName = swFieldNameTemp;
	m_nFormControlIndex = -1;

	return TRUE;
}

void JField::GetFormFields(FSPDFDoc* pDocument, const CFX_WideString& csFieldName, CFX_PtrArray& FieldArray)
{
	ASSERT(pDocument != NULL);
	FSFormImp* pInterForm= (FSFormImp*)((FSPDFDocImp*)pDocument)->GetForm();
	ASSERT(pInterForm != NULL);
	CPDF_InterForm* pPDFForm = pInterForm->ST_GetSTInterForm()->m_pInterForm;
	ASSERT(pPDFForm != NULL);
	ASSERT(FieldArray.GetSize() == 0);

	for (FX_INT32 i=0,sz=pPDFForm->CountFields(csFieldName); i<sz; i++)
	{
		if (CPDF_FormField* pFormField = pPDFForm->GetField(i, csFieldName))
			FieldArray.Add((void*)pFormField);
	}
}

void JField::GetFormFields(const CFX_WideString& csFieldName, CFX_PtrArray& FieldArray)
{
	ASSERT(m_pDocument != NULL);

	JField::GetFormFields(m_pDocument, csFieldName, FieldArray);
}

void JField::UpdateFormField(FSPDFDoc* pDocument, CPDF_FormField* pFormField, 
							FX_BOOL bChangeMark, FX_BOOL bResetAP, FX_BOOL bRefresh)
{
	ASSERT(pDocument != NULL);
	ASSERT(pFormField != NULL);

	FSFormImp* pInterForm= (FSFormImp*)((FSPDFDocImp*)pDocument)->GetForm();
	ASSERT(pInterForm != NULL);
	CPDF_InterForm* pPDFForm = pInterForm->ST_GetSTInterForm()->m_pInterForm;
	ASSERT(pPDFForm != NULL);

	//CFX_PtrArray widgets;
	//pInterForm->GetWidgets(pFormField, widgets);

	if (bResetAP)
	{
		FX_INT32 nFieldType = pFormField->GetFieldType();
		if (nFieldType == FIELDTYPE_COMBOBOX || nFieldType == FIELDTYPE_TEXTFIELD)
		{
			CFSCRT_STPDFInterForm* pSTForm = pInterForm->ST_GetSTInterForm();
			if(!pSTForm) return;

			FX_BOOL bFormated = FALSE;
			CFX_WideString wsValue = pSTForm->OnFormat(pFormField, 0, bFormated);
			pSTForm->ResetFieldAppearance(pFormField, wsValue);
// 			for (FX_INT32 i=0,sz=widgets.GetSize(); i<sz; i++)
// 			{
// 				FSFormControl* pWidget = (FSFormControl*)widgets.GetAt(i);
// 				ASSERT(pWidget != NULL);
// 
// 				FX_BOOL bFormated = FALSE;
// 				//CFX_WideString sValue = pWidget->OnFormat(0, bFormated);
// 				//if (bFormated)
// 				//	pWidget->ResetAppearance(sValue, FALSE);
// 				//else
// 				//	pWidget->ResetAppearance(NULL, FALSE);
// 			}
		}
		else
		{
			CFSCRT_STPDFInterForm* pSTForm = pInterForm->ST_GetSTInterForm();
			if(!pSTForm) return;

			pSTForm->ResetFieldAppearance(pFormField, NULL);
// 			for (FX_INT32 i=0,sz=widgets.GetSize(); i<sz; i++)
// 			{
// 				FSFormControl* pWidget = (FSFormControl*)widgets.GetAt(i);
// 				ASSERT(pWidget != NULL);
// 
// 				//pWidget->ResetAppearance(NULL, FALSE);
// 			}
		}
	}

	if (bRefresh)
	{
		CFSCRT_STPDFInterForm* pSTForm = pInterForm->ST_GetSTInterForm();
		if(!pSTForm) return;

		pSTForm->UpdateField(pFormField);
// 		for (FX_INT32 i=0,sz=widgets.GetSize(); i<sz; i++)
// 		{
// 			FSFormControl* pWidget = (FSFormControl*)widgets.GetAt(i);
// 			ASSERT(pWidget != NULL);
// 			
// 			CFSCRT_LTPage* pPage = pWidget->GetPage();
// 			ASSERT(pPage != NULL);
// 			//pPage->UpdateAllViews(NULL, pWidget);
// 		}
	}		
	
	if (bChangeMark)
		JS_SetChangeMark(pDocument,TRUE);
}

void JField::UpdateFormControl(FSPDFDoc* pDocument, CPDF_FormControl* pFormControl, 
							FX_BOOL bChangeMark, FX_BOOL bResetAP, FX_BOOL bRefresh)
{
	ASSERT(pDocument != NULL);
	ASSERT(pFormControl != NULL);

	FSFormImp* pInterForm= (FSFormImp*)((FSPDFDocImp*)pDocument)->GetForm();
	ASSERT(pInterForm != NULL);

	CFSCRT_STPDFInterForm* pSTInterForm = pInterForm->ST_GetSTInterForm();
	ASSERT(pSTInterForm != NULL);

	FSFormControlImp* pWidget = (FSFormControlImp*)pSTInterForm->GetLTFormControl(pFormControl);
	CPDF_FormField* pFormField = pFormControl->GetField();
	if (pWidget)
	{
		if (bResetAP)
		{
			FX_INT32 nFieldType = pFormField->GetFieldType();
			if (nFieldType == FIELDTYPE_COMBOBOX || nFieldType == FIELDTYPE_TEXTFIELD)
			{
				FX_BOOL bFormated = FALSE;
				CFX_WideString sValue = pSTInterForm->OnFormat(pFormField, 0, bFormated);
				if (bFormated)
					pWidget->ResetAppearance(sValue);
				else
					pWidget->ResetAppearance(NULL);
			}
			else
			{
				pWidget->ResetAppearance(NULL);
			}
		}

		if (bRefresh)
		{
// 			CPDFSDK_InterForm * pInterForm = pWidget->GetInterForm();
// 			FSPDFDoc* pDoc = pInterForm->GetDocument();
// 			ASSERT(pDoc != NULL);
// 			pDoc->UpdateAllViews(NULL, pWidget);
			pSTInterForm->UpdateField(pFormField);
		}

	}

	if (bChangeMark)
		JS_SetChangeMark(pDocument,TRUE);
}

FSFormControl* JField::GetWidget(FSPDFDoc* pDocument, CPDF_FormControl* pFormControl)
{
	ASSERT(pDocument != NULL);
	ASSERT(pFormControl != NULL);

	FSFormImp* pInterForm = ((FSPDFDocImp*)pDocument)->GetForm();
	ASSERT(pInterForm != NULL);

	//return pInterForm->GetWidget(pFormControl);
	return NULL;
}

FX_BOOL JField::ValueIsOccur(CPDF_FormField* pFormField, CFX_WideString csOptLabel)
{
	ASSERT(pFormField != NULL);

	for (FX_INT32 i=0,sz = pFormField->CountOptions(); i < sz; i++)
	{
		if (csOptLabel.Compare(pFormField->GetOptionLabel(i)) == 0)
			return TRUE;
	}

	return FALSE;
}

CPDF_FormControl* JField::GetSmartFieldControl(CPDF_FormField* pFormField)
{
	ASSERT(pFormField != NULL);
	if (m_nFormControlIndex<0)
		return pFormField->GetControl(0);
	else
		return pFormField->GetControl(m_nFormControlIndex);
}

// Helper Functions
FX_BOOL JField::JS_SetChangeMark(FSPDFDoc* pDoc,FX_BOOL bChanged)
{
	FSFormFillerImp* pFormFiller = ((FSPDFDocImp*)pDoc)->GetForm()->GetFormFiller();
// 	CFSCRT_LTAppHandler* pApp = FSCRT_GetLTAppHandler();
// 	if (!pApp) return FALSE;

// 	FSCRT_VAR params = {FSCRT_VT_BOOL, 0, 0};
// 	FSCRT_Var_SetBooleanValue(params, bChanged);
// 	FSCRT_VAR retVal = {FSCRT_VT_INT32, 0, 0};
// 	FSCRT_Var_SetInt32Value(retVal, 0);

	FS_RESULT ret = FSCRT_ERRCODE_SUCCESS;
#if defined(_FSDK_OOM_) && !defined(_FXSDK_EDIT_OOM_RECOVER_) && !defined(_FXSDK_CUSTOM_OOM_RECOVER_)
	//FSCRT_GACHECKOOMSTATE_SETMODIFYFLAG(pDoc, TRUE);
#endif
    if(pFormFiller)
        pFormFiller->m_pJSActionHandle->SetChangeMark((FSPDFDoc*)pDoc, bChanged);
	//FS_RESULT ret = pApp->CallFunction(NULL, FSCRT_APPFUNC_SETCHANGEMARK, &params, 1, &retVal);
	return (ret == FSCRT_ERRCODE_SUCCESS) ? TRUE : FALSE;
}

/* ---------------------------------------- property ---------------------------------------- */

FX_BOOL JField::alignment(OBJ_PROP_PARAMS)
{
	ASSERT(m_pDocument != NULL);

	if (vp.IsSetting())
	{
		if (!m_bCanSet) return FALSE;

		CFX_ByteString alignStr;
		vp >> alignStr;

		if (m_bDelay)
		{
			AddDelay_String(FP_ALIGNMENT, alignStr);
		}
		else
		{
			JField::SetAlignment(m_pDocument, m_FieldName, m_nFormControlIndex, alignStr);
		}
	}
	else
	{
		CFX_PtrArray FieldArray;
		GetFormFields(m_FieldName, FieldArray);
		if (FieldArray.GetSize() <= 0) return FALSE;

		CPDF_FormField* pFormField = (CPDF_FormField*)FieldArray.ElementAt(0);
		ASSERT(pFormField != NULL);

		if (pFormField->GetFieldType() != FIELDTYPE_TEXTFIELD)
			return FALSE;

		CPDF_FormControl* pFormControl = GetSmartFieldControl(pFormField);
		if (!pFormControl)return FALSE;

		switch (pFormControl->GetControlAlignment())
		{
			case 1:
				vp << (FX_LPCWSTR)L"center";
				break;
			case 0:
				vp << (FX_LPCWSTR)L"left";
				break;
			case 2:
				vp << (FX_LPCWSTR)L"right";
				break;
			default:
				vp << (FX_LPCWSTR)L"";
		}
	}

	return TRUE;
}

void JField::SetAlignment(FSPDFDoc* pDocument, const CFX_WideString& swFieldName, FX_INT32 nControlIndex, 
						 const CFX_ByteString& string)
{
	ASSERT(pDocument != NULL);

	FX_INT32 nAlignment = 0;

	if (string == "center")
		nAlignment = 1;
	else if (string=="left")
		nAlignment = 0;
	else if (string=="right")
		nAlignment = 2;
	else
		return;

	CFX_PtrArray FieldArray;
	GetFormFields(pDocument, swFieldName, FieldArray);

	for (FX_INT32 i=0,isz = FieldArray.GetSize(); i<isz; i++)
	{
		CPDF_FormField* pFormField = (CPDF_FormField*)FieldArray.ElementAt(i);
		ASSERT(pFormField != NULL);

		if (pFormField->GetFieldType() == FIELDTYPE_TEXTFIELD)
		{
			if (nControlIndex < 0)
			{
				FX_BOOL bSet = FALSE;
				for (FX_INT32 i=0, sz=pFormField->CountControls(); i<sz; i++)
				{
					CPDF_FormControl* pFormControl = pFormField->GetControl(i);
					ASSERT(pFormControl != NULL);
					
					if (nAlignment != pFormControl->GetControlAlignment())
					{
						pFormControl->SetControlAlignment(nAlignment);
						bSet = TRUE;
					}
				}
				if (bSet) UpdateFormField(pDocument, pFormField, TRUE, TRUE, TRUE);
			}
			else
			{
				CPDF_FormControl* pFormControl = pFormField->GetControl(nControlIndex);
				if (pFormControl)
				{
					if (nAlignment != pFormControl->GetControlAlignment())
					{
						pFormControl->SetControlAlignment(nAlignment);
						UpdateFormControl(pDocument, pFormControl, TRUE, TRUE, TRUE);
					}
				}
			}
		}
	}
}

FX_BOOL JField::borderStyle(OBJ_PROP_PARAMS)
{	
	ASSERT(m_pDocument != NULL);

	if (vp.IsSetting())
	{
		if (!m_bCanSet) return FALSE;

		CFX_ByteString strType = "";
		vp >> strType;

		if (m_bDelay)
		{
			AddDelay_String(FP_BORDERSTYLE, strType);
		}
		else
		{
			JField::SetBorderStyle(m_pDocument, m_FieldName, m_nFormControlIndex, strType);
		}
	}
	else
	{
		CFX_PtrArray FieldArray;
		GetFormFields(m_FieldName, FieldArray);
		if (FieldArray.GetSize() <= 0) return FALSE;

		CPDF_FormField* pFormField = (CPDF_FormField*)FieldArray.ElementAt(0);
		if (!pFormField) return FALSE;

		CPDF_FormControl* pFormControl = GetSmartFieldControl(pFormField);
		ASSERT(pFormControl != NULL);
		CPDF_Dictionary* pControlDict = pFormControl->GetWidget();
		ASSERT(pControlDict != NULL);

		CPDF_Dictionary* pBS = pControlDict->GetDict(FX_BSTRC("BS"));
		if (pBS)
		{
			CFX_ByteString bsBorder = pBS->GetString(FX_BSTRC("S"));
			if (bsBorder.GetLength() != 1)
			{
				vp << (FX_LPCWSTR)L"solid";
				return TRUE;
			}

			FX_BYTE single = bsBorder[0];
			switch (single)
			{
				case 'S':
					vp << (FX_LPCWSTR)L"solid";
					break;
				case 'D':
					vp << (FX_LPCWSTR)L"dashed";
					break;
				case 'B':
					vp << (FX_LPCWSTR)L"beveled";
					break;
				case 'I':
					vp << (FX_LPCWSTR)L"inset";
					break;
				case 'U':
					vp << (FX_LPCWSTR)L"underline";
					break;
				default:
					vp << (FX_LPCWSTR)L"";
					break;
			}
			return TRUE;
		}
		CPDF_Array* pBorder = pControlDict->GetArray(FX_BSTRC("Border"));
		if (pBorder)
		{
			if (pBorder->GetCount() > 3) 
			{ 
				CPDF_Array* pDP = pBorder->GetArray(3) ; 
				if (pDP != NULL && pDP->GetCount() > 0)
				{
					vp << (FX_LPCWSTR)L"dashed";
					return TRUE;
				}
			}
		}

		vp << (FX_LPCWSTR)L"solid";
		return TRUE;
	}

	return TRUE;
}

void JField::SetBorderStyle(FSPDFDoc* pDocument, const CFX_WideString& swFieldName, FX_INT32 nControlIndex, 
						   const CFX_ByteString& string)
{
	ASSERT(pDocument != NULL);

	FX_INT32 nBorderStyle = 0;

// 	if (string == "solid")
// 		nBorderStyle = BBS_SOLID;
// 	else if (string == "beveled")
// 		nBorderStyle = BBS_BEVELED;
// 	else if (string == "dashed")
// 		nBorderStyle = BBS_DASH;
// 	else if (string == "inset")
// 		nBorderStyle = BBS_INSET;
// 	else if (string == "underline")
// 		nBorderStyle = BBS_UNDERLINE;
// 	else return;

	CFX_PtrArray FieldArray;
	GetFormFields(pDocument, swFieldName, FieldArray);

	for (FX_INT32 i=0,isz=FieldArray.GetSize(); i<isz; i++)
	{
		CPDF_FormField* pFormField = (CPDF_FormField*)FieldArray.ElementAt(i);
		ASSERT(pFormField != NULL);

		if (nControlIndex < 0)
		{
			FX_BOOL bSet = FALSE;
			for (FX_INT32 j=0,jsz = pFormField->CountControls(); j<jsz; j++)
			{
				if (FSFormControl* pWidget = GetWidget(pDocument, pFormField->GetControl(j)))
				{
// 					if (pWidget->GetBorderStyle() != nBorderStyle)
// 					{
// 						pWidget->SetBorderStyle(nBorderStyle);
// 						bSet = TRUE;
// 					}
				}
			}
			if (bSet) UpdateFormField(pDocument, pFormField, TRUE, TRUE, TRUE);
		}
		else
		{
			if (CPDF_FormControl* pFormControl = pFormField->GetControl(nControlIndex))
			{
				if (FSFormControl* pWidget = GetWidget(pDocument, pFormControl))
				{
// 					if (pWidget->GetBorderStyle() != nBorderStyle)
// 					{
// 						pWidget->SetBorderStyle(nBorderStyle);
// 						UpdateFormControl(pDocument, pFormControl, TRUE, TRUE, TRUE);
// 					}
				}
			}
		}
	}
}

FX_BOOL JField::buttonAlignX(OBJ_PROP_PARAMS)
{
	ASSERT(m_pDocument != NULL);

	if (vp.IsSetting())
	{
		if (!m_bCanSet) return FALSE;

		FX_INT32 nVP;
		vp >> nVP;

		if (m_bDelay)
		{
			AddDelay_Int(FP_BUTTONALIGNX, nVP);
		}
		else
		{
			JField::SetButtonAlignX(m_pDocument, m_FieldName, m_nFormControlIndex, nVP);
		}
	}
	else
	{		
		CFX_PtrArray FieldArray;
		GetFormFields(m_FieldName,FieldArray);
		if (FieldArray.GetSize() <= 0) return FALSE;

		CPDF_FormField* pFormField = (CPDF_FormField*)FieldArray.ElementAt(0);
		ASSERT(pFormField != NULL);

		if (pFormField->GetFieldType() != FIELDTYPE_PUSHBUTTON)
			return FALSE;

		CPDF_FormControl* pFormControl = GetSmartFieldControl(pFormField);
		if (!pFormControl)return FALSE;

		CPDF_IconFit IconFit = pFormControl->GetIconFit();

		FX_FLOAT fLeft,fBottom;
		IconFit.GetIconPosition(fLeft,fBottom);

		vp << (FX_INT32)fLeft;
	}

	return TRUE;
}

void JField::SetButtonAlignX(FSPDFDoc* pDocument, const CFX_WideString& swFieldName, FX_INT32 nControlIndex, FX_INT32 number)
{
	ASSERT(pDocument != NULL);

	CFX_PtrArray FieldArray;
	GetFormFields(pDocument, swFieldName, FieldArray);
	
	for (FX_INT32 i=0,isz=FieldArray.GetSize(); i<isz; i++)
	{
		CPDF_FormField* pFormField = (CPDF_FormField*)FieldArray.ElementAt(i);
		ASSERT(pFormField != NULL);

		if (pFormField->GetFieldType() == FIELDTYPE_PUSHBUTTON)
		{
			if (nControlIndex < 0)
			{
				FX_BOOL bSet = FALSE;
				for (FX_INT32 i=0,sz = pFormField->CountControls(); i<sz; i++)
				{
					CPDF_FormControl* pFormControl = pFormField->GetControl(i);
					ASSERT(pFormControl != NULL);
				
					CPDF_IconFit IconFit = pFormControl->GetIconFit();

					FX_FLOAT fLeft,fBottom;
					IconFit.GetIconPosition(fLeft,fBottom);
					
					if (fLeft != (FX_FLOAT)number)
					{
						IconFit.SetIconPosition((FX_FLOAT)number,fBottom);
						pFormControl->SetIconFit(IconFit);
						bSet = TRUE;
					}
				}
				if (bSet) UpdateFormField(pDocument, pFormField, TRUE, TRUE, TRUE);
			}
			else
			{
				if (CPDF_FormControl* pFormControl = pFormField->GetControl(nControlIndex))
				{
					CPDF_IconFit IconFit = pFormControl->GetIconFit();

					FX_FLOAT fLeft,fBottom;
					IconFit.GetIconPosition(fLeft, fBottom);
					
					if (fLeft != (FX_FLOAT)number)
					{
						IconFit.SetIconPosition((FX_FLOAT)number, fBottom);
						pFormControl->SetIconFit(IconFit);
						UpdateFormControl(pDocument, pFormControl, TRUE, TRUE, TRUE);
					}
				}
			}
		}
	}
}

FX_BOOL JField::buttonAlignY(OBJ_PROP_PARAMS)
{
	ASSERT(m_pDocument != NULL);

	if (vp.IsSetting())
	{
		if (!m_bCanSet) return FALSE;

		FX_INT32 nVP;
		vp >> nVP;

		if (m_bDelay)
		{
			AddDelay_Int(FP_BUTTONALIGNY, nVP);
		}
		else
		{
			JField::SetButtonAlignY(m_pDocument, m_FieldName, m_nFormControlIndex, nVP);
		}
	}
	else
	{
		CFX_PtrArray FieldArray;
		GetFormFields(m_FieldName,FieldArray);
		if (FieldArray.GetSize() <= 0) return FALSE;

		CPDF_FormField* pFormField = (CPDF_FormField*)FieldArray.ElementAt(0);
		ASSERT(pFormField != NULL);

		if (pFormField->GetFieldType() != FIELDTYPE_PUSHBUTTON)
			return FALSE;

		CPDF_FormControl* pFormControl = GetSmartFieldControl(pFormField);
		if (!pFormControl)return FALSE;

		CPDF_IconFit IconFit = pFormControl->GetIconFit();

		FX_FLOAT fLeft,fBottom;
		IconFit.GetIconPosition(fLeft,fBottom);

		vp <<  (FX_INT32)fBottom;
	}

	return TRUE;
}

void JField::SetButtonAlignY(FSPDFDoc* pDocument, const CFX_WideString& swFieldName, FX_INT32 nControlIndex, FX_INT32 number)
{
	ASSERT(pDocument != NULL);

	CFX_PtrArray FieldArray;
	GetFormFields(pDocument, swFieldName, FieldArray);

	for (FX_INT32 i=0,isz=FieldArray.GetSize(); i<isz; i++)
	{
		CPDF_FormField* pFormField = (CPDF_FormField*)FieldArray.ElementAt(i);
		ASSERT(pFormField != NULL);

		if (pFormField->GetFieldType() == FIELDTYPE_PUSHBUTTON)
		{
			if (nControlIndex < 0)
			{
				FX_BOOL bSet = FALSE;
				for (FX_INT32 i=0,sz = pFormField->CountControls(); i<sz; i++)
				{
					CPDF_FormControl* pFormControl = pFormField->GetControl(i);
					ASSERT(pFormControl != NULL);
					
					CPDF_IconFit IconFit = pFormControl->GetIconFit();

					FX_FLOAT fLeft,fBottom;
					IconFit.GetIconPosition(fLeft,fBottom);

					if (fBottom != (FX_FLOAT)number)
					{
						IconFit.SetIconPosition(fLeft, (FX_FLOAT)number);
						pFormControl->SetIconFit(IconFit);
						bSet = TRUE;
					}
				}

				if (bSet) UpdateFormField(pDocument, pFormField, TRUE, TRUE, TRUE);
			}
			else
			{
				if (CPDF_FormControl* pFormControl = pFormField->GetControl(nControlIndex))
				{
					CPDF_IconFit IconFit = pFormControl->GetIconFit();

					FX_FLOAT fLeft,fBottom;
					IconFit.GetIconPosition(fLeft,fBottom);

					if (fBottom != (FX_FLOAT)number)
					{
						IconFit.SetIconPosition(fLeft, (FX_FLOAT)number);
						pFormControl->SetIconFit(IconFit);
						UpdateFormControl(pDocument, pFormControl, TRUE, TRUE, TRUE);
					}
				}
			}
		}
	}
}

FX_BOOL JField::buttonFitBounds(OBJ_PROP_PARAMS)
{
	ASSERT(m_pDocument != NULL);

	if (vp.IsSetting())
	{
		if (!m_bCanSet) return FALSE;

		bool bVP;
		vp >> bVP;

		if (m_bDelay)
		{
			AddDelay_Bool(FP_BUTTONFITBOUNDS, bVP);
		}
		else
		{
			JField::SetButtonFitBounds(m_pDocument, m_FieldName, m_nFormControlIndex, bVP);
		}
	}
	else
	{
		CFX_PtrArray FieldArray;
		GetFormFields(m_FieldName,FieldArray);
		if (FieldArray.GetSize() <= 0) return FALSE;

		CPDF_FormField* pFormField = (CPDF_FormField*)FieldArray.ElementAt(0);
		ASSERT(pFormField != NULL);

		if (pFormField->GetFieldType() != FIELDTYPE_PUSHBUTTON)
			return FALSE;

		CPDF_FormControl* pFormControl = GetSmartFieldControl(pFormField);
		if (!pFormControl)return FALSE;

		CPDF_IconFit IconFit = pFormControl->GetIconFit();
		vp << IconFit.GetFittingBounds();		
	}

	return TRUE;
}

void JField::SetButtonFitBounds(FSPDFDoc* pDocument, const CFX_WideString& swFieldName, FX_INT32 nControlIndex, bool b)
{
	ASSERT(pDocument != NULL);

	CFX_PtrArray FieldArray;
	GetFormFields(pDocument, swFieldName, FieldArray);

	for (FX_INT32 i=0,isz=FieldArray.GetSize(); i<isz; i++)
	{
		CPDF_FormField* pFormField = (CPDF_FormField*)FieldArray.ElementAt(i);
		ASSERT(pFormField != NULL);

		if (pFormField->GetFieldType() == FIELDTYPE_PUSHBUTTON)
		{
			if (nControlIndex < 0)
			{
				FX_BOOL bSet = FALSE;

				for (FX_INT32 i=0,sz = pFormField->CountControls(); i<sz; i++)
				{
					CPDF_FormControl* pFormControl = pFormField->GetControl(i);
					ASSERT(pFormControl != NULL);
										
					CPDF_IconFit IconFit = pFormControl->GetIconFit();
					if (IconFit.GetFittingBounds() != (FX_BOOL)b)
					{
						IconFit.SetFittingBounds(b);
						pFormControl->SetIconFit(IconFit);
						bSet = TRUE;
					}
				}

				if (bSet) UpdateFormField(pDocument, pFormField, TRUE, TRUE, TRUE);
			}
			else
			{
				if (CPDF_FormControl* pFormControl = pFormField->GetControl(nControlIndex))
				{				
					CPDF_IconFit IconFit = pFormControl->GetIconFit();
					if (IconFit.GetFittingBounds() != (FX_BOOL)b)
					{
						IconFit.SetFittingBounds(b);
						pFormControl->SetIconFit(IconFit);
						UpdateFormControl(pDocument, pFormControl, TRUE, TRUE, TRUE);
					}
				}
			}
		}
	}
}

FX_BOOL JField::buttonPosition(OBJ_PROP_PARAMS)
{
	ASSERT(m_pDocument != NULL);

	if (vp.IsSetting())
	{
		if (!m_bCanSet) return FALSE;

		FX_INT32 nVP;
		vp >> nVP;

		if (m_bDelay)
		{
			AddDelay_Int(FP_BUTTONPOSITION, nVP);
		}
		else
		{
			JField::SetButtonPosition(m_pDocument, m_FieldName, m_nFormControlIndex, nVP);
		}
	}
	else
	{
		CFX_PtrArray FieldArray;
		GetFormFields(m_FieldName,FieldArray);
		if (FieldArray.GetSize() <= 0) return FALSE;

		CPDF_FormField* pFormField = (CPDF_FormField*)FieldArray.ElementAt(0);
		ASSERT(pFormField != NULL);

		if (pFormField->GetFieldType() != FIELDTYPE_PUSHBUTTON)
			return FALSE;

		CPDF_FormControl* pFormControl = GetSmartFieldControl(pFormField);
		if (!pFormControl)return FALSE;

		vp <<  pFormControl->GetTextPosition();
	}
	return TRUE;
}

void JField::SetButtonPosition(FSPDFDoc* pDocument, const CFX_WideString& swFieldName, FX_INT32 nControlIndex, FX_INT32 number)
{
	ASSERT(pDocument != NULL);

	CFX_PtrArray FieldArray;
	GetFormFields(pDocument, swFieldName, FieldArray);

	for (FX_INT32 i=0,isz=FieldArray.GetSize(); i<isz; i++)
	{
		CPDF_FormField* pFormField = (CPDF_FormField*)FieldArray.ElementAt(i);
		ASSERT(pFormField != NULL);

		if (pFormField->GetFieldType() == FIELDTYPE_PUSHBUTTON)
		{
			if (nControlIndex < 0)
			{
				FX_BOOL bSet = FALSE;
				for (FX_INT32 i=0,sz = pFormField->CountControls(); i<sz; i++)
				{
					CPDF_FormControl* pFormControl = pFormField->GetControl(i);
					ASSERT(pFormControl != NULL);
					
					if (number != pFormControl->GetTextPosition())
					{
						pFormControl->SetTextPosition(number);
						bSet = TRUE;
					}
					
				}
				if (bSet) UpdateFormField(pDocument, pFormField, TRUE, TRUE, TRUE);
			}
			else
			{
				if (CPDF_FormControl* pFormControl = pFormField->GetControl(nControlIndex))
				{		
					if (number != pFormControl->GetTextPosition())
					{
						pFormControl->SetTextPosition(number);
						UpdateFormControl(pDocument, pFormControl, TRUE, TRUE, TRUE);
					}
				}
			}
		}
	}
}

FX_BOOL JField::buttonScaleHow(OBJ_PROP_PARAMS)
{
	ASSERT(m_pDocument != NULL);

	if (vp.IsSetting())
	{
		if (!m_bCanSet) return FALSE;

		FX_INT32 nVP;
		vp >> nVP;

		if (m_bDelay)
		{
			AddDelay_Int(FP_BUTTONSCALEHOW, nVP);
		}
		else
		{
			JField::SetButtonScaleHow(m_pDocument, m_FieldName, m_nFormControlIndex, nVP);
		}
	}
	else
	{
		CFX_PtrArray FieldArray;
		GetFormFields(m_FieldName, FieldArray);
		if (FieldArray.GetSize() <= 0) return FALSE;

		CPDF_FormField* pFormField = (CPDF_FormField*)FieldArray.ElementAt(0);
		ASSERT(pFormField != NULL);

		if (pFormField->GetFieldType() != FIELDTYPE_PUSHBUTTON)
			return FALSE;

		CPDF_FormControl* pFormControl = GetSmartFieldControl(pFormField);
		if (!pFormControl)return FALSE;

		CPDF_IconFit IconFit = pFormControl->GetIconFit();
		if (IconFit.IsProportionalScale())
			vp << (FX_INT32)0;
		else
			vp << (FX_INT32)1;
	}

	return TRUE;
}

void JField::SetButtonScaleHow(FSPDFDoc* pDocument, const CFX_WideString& swFieldName, FX_INT32 nControlIndex, FX_INT32 number)
{
	ASSERT(pDocument != NULL);

	CFX_PtrArray FieldArray;
	GetFormFields(pDocument, swFieldName, FieldArray);

	FX_BOOL bProportional = (number == 0);

	for (FX_INT32 i=0,isz=FieldArray.GetSize(); i<isz; i++)
	{
		CPDF_FormField* pFormField = (CPDF_FormField*)FieldArray.ElementAt(i);
		ASSERT(pFormField != NULL);

		if (pFormField->GetFieldType() == FIELDTYPE_PUSHBUTTON)
		{
			if (nControlIndex < 0)
			{
				FX_BOOL bSet = FALSE;
				for (FX_INT32 i=0,sz=pFormField->CountControls(); i<sz; i++)
				{
					CPDF_FormControl* pFormControl = pFormField->GetControl(i);
					ASSERT(pFormControl != NULL);

					CPDF_IconFit IconFit = pFormControl->GetIconFit();

					if (IconFit.IsProportionalScale() != bProportional)
					{
						if (bProportional)
							IconFit.ProportionalScale(TRUE);
						else
							IconFit.ProportionalScale(FALSE);
						pFormControl->SetIconFit(IconFit);
						bSet = TRUE;
					}
				}
				if (bSet) UpdateFormField(pDocument, pFormField, TRUE, TRUE, TRUE);
			}
			else
			{
				if (CPDF_FormControl* pFormControl = pFormField->GetControl(nControlIndex))
				{
					CPDF_IconFit IconFit = pFormControl->GetIconFit();

					if (IconFit.IsProportionalScale() != bProportional)
					{
						if (bProportional)
							IconFit.ProportionalScale();
						else
							IconFit.ProportionalScale(FALSE);
						pFormControl->SetIconFit(IconFit);

						UpdateFormControl(pDocument, pFormControl, TRUE, TRUE, TRUE);
					}
				}
			}
		}
	}
}

FX_BOOL JField::buttonScaleWhen(OBJ_PROP_PARAMS)
{
	ASSERT(m_pDocument != NULL);

	if (vp.IsSetting())
	{
		if (!m_bCanSet) return FALSE;

		FX_INT32 nVP;
		vp >> nVP;

		if (m_bDelay)
		{
			AddDelay_Int(FP_BUTTONSCALEWHEN, nVP);
		}
		else
		{
			JField::SetButtonScaleWhen(m_pDocument, m_FieldName, m_nFormControlIndex, nVP);
		}
	}
	else
	{
		CFX_PtrArray FieldArray;
		GetFormFields(m_FieldName,FieldArray);
		if (FieldArray.GetSize() <= 0) return FALSE;

		CPDF_FormField* pFormField = (CPDF_FormField*) FieldArray.ElementAt(0);
		ASSERT(pFormField != NULL);

		if (pFormField->GetFieldType() != FIELDTYPE_PUSHBUTTON)
			return FALSE;

		CPDF_FormControl* pFormControl = GetSmartFieldControl(pFormField);
		if (!pFormControl) return FALSE;

		CPDF_IconFit IconFit = pFormControl->GetIconFit();
		FX_INT32 ScaleM = IconFit.GetScaleMethod();
		switch (ScaleM)
		{
			case CPDF_IconFit::Always :
				vp <<  (FX_INT32) CPDF_IconFit::Always;
				break;
			case CPDF_IconFit::Bigger :
				vp <<  (FX_INT32) CPDF_IconFit::Bigger;
				break;
			case CPDF_IconFit::Never :
				vp <<  (FX_INT32) CPDF_IconFit::Never;
				break;
			case CPDF_IconFit::Smaller :
				vp <<  (FX_INT32) CPDF_IconFit::Smaller;
				break;
		}
	}

	return TRUE;
}

void JField::SetButtonScaleWhen(FSPDFDoc* pDocument, const CFX_WideString& swFieldName, FX_INT32 nControlIndex, FX_INT32 number)
{
	ASSERT(pDocument != NULL);

	CFX_PtrArray FieldArray;
	GetFormFields(pDocument, swFieldName, FieldArray);

	for (FX_INT32 i=0,isz=FieldArray.GetSize(); i<isz; i++)
	{
		CPDF_FormField* pFormField = (CPDF_FormField*)FieldArray.ElementAt(i);
		ASSERT(pFormField != NULL);

		if (pFormField->GetFieldType() == FIELDTYPE_PUSHBUTTON)
		{
			if (nControlIndex < 0)
			{
				FX_BOOL bSet = FALSE;
				for (FX_INT32 i=0, sz=pFormField->CountControls(); i<sz; i++)				
				{
					CPDF_FormControl* pFormControl = pFormField->GetControl(i);
					ASSERT(pFormControl != NULL);

					CPDF_IconFit IconFit = pFormControl->GetIconFit();
					if (IconFit.GetScaleMethod() != (CPDF_IconFit::ScaleMethod)number)
					{
						IconFit.SetScaleMethod((CPDF_IconFit::ScaleMethod)number);
						pFormControl->SetIconFit(IconFit);
						bSet = TRUE;
					}
				}		
				if (bSet) UpdateFormField(pDocument, pFormField, TRUE, TRUE, TRUE);
			}
			else
			{
				if (CPDF_FormControl* pFormControl = pFormField->GetControl(nControlIndex))
				{
					CPDF_IconFit IconFit = pFormControl->GetIconFit();
					if (IconFit.GetScaleMethod() != (CPDF_IconFit::ScaleMethod)number)
					{
						IconFit.SetScaleMethod((CPDF_IconFit::ScaleMethod)number);
						pFormControl->SetIconFit(IconFit);

						UpdateFormControl(pDocument, pFormControl, TRUE, TRUE, TRUE);
					}
				}
			}
		}
	}
}

FX_BOOL JField::calcOrderIndex(OBJ_PROP_PARAMS)
{
	ASSERT(m_pDocument != NULL);

	if (vp.IsSetting())
	{	
		if (!m_bCanSet) return FALSE;

		FX_INT32 nVP;
		vp >> nVP;

		if (m_bDelay)
		{
			AddDelay_Int(FP_CALCORDERINDEX, nVP);
		}
		else
		{
			JField::SetCalcOrderIndex(m_pDocument, m_FieldName, m_nFormControlIndex, nVP);
		}
	}
	else
	{
		CFX_PtrArray FieldArray;
		GetFormFields(m_FieldName, FieldArray);
		if (FieldArray.GetSize() <= 0) return FALSE;

		CPDF_FormField* pFormField = (CPDF_FormField*)FieldArray.ElementAt(0);
		ASSERT(pFormField != NULL);

		if (pFormField->GetFieldType() != FIELDTYPE_COMBOBOX && pFormField->GetFieldType() != FIELDTYPE_TEXTFIELD)
			return FALSE;

		FSFormImp* pRDInterForm = m_pDocument->GetForm();
		ASSERT(pRDInterForm != NULL);

		CPDF_InterForm* pInterForm = pRDInterForm->ST_GetSTInterForm()->m_pInterForm;
		ASSERT(pInterForm != NULL);

		vp << (FX_INT32)pInterForm->FindFieldInCalculationOrder(pFormField);
	}

	return TRUE;
}

void JField::SetCalcOrderIndex(FSPDFDoc* pDocument, const CFX_WideString& swFieldName, FX_INT32 nControlIndex, FX_INT32 number)
{
	ASSERT(pDocument != NULL);

	CFX_PtrArray FieldArray;
	GetFormFields(pDocument, swFieldName, FieldArray);

	FSFormImp* pRInterForm = ((FSPDFDocImp*)pDocument)->GetForm();
	ASSERT(pRInterForm != NULL);

	CPDF_InterForm* pInterForm = pRInterForm->ST_GetSTInterForm()->m_pInterForm;
	ASSERT(pInterForm != NULL);

	for (FX_INT32 i=0,isz=FieldArray.GetSize(); i<isz; i++)
	{
		CPDF_FormField* pFormField = (CPDF_FormField*)FieldArray.ElementAt(i);
		ASSERT(pFormField != NULL);

		if (pFormField->GetFieldType() == FIELDTYPE_COMBOBOX || pFormField->GetFieldType() == FIELDTYPE_TEXTFIELD)
		{
			FX_INT32 iIndex = pInterForm->FindFieldInCalculationOrder(pFormField);
			
			if (iIndex != number)
			{
				if (iIndex == -1 && number > 0 )
					pInterForm->InsertFieldInCalculationOrder(pFormField, number);
				else if (number > 0 && iIndex > 0)
					pInterForm->MoveFieldInCalculationOrder(pFormField, number);

				UpdateFormField(pDocument, pFormField, TRUE, TRUE, TRUE);
			}
		}
	}
}

FX_BOOL JField::charLimit(OBJ_PROP_PARAMS)
{
	ASSERT(m_pDocument != NULL);

	if (vp.IsSetting())
	{
		if (!m_bCanSet) return FALSE;

		FX_INT32 nVP;
		vp >> nVP;

		if (m_bDelay)
		{
			AddDelay_Int(FP_CHARLIMIT, nVP);
		}
		else
		{
			JField::SetCharLimit(m_pDocument, m_FieldName, m_nFormControlIndex, nVP);
		}
	}
	else
	{
		CFX_PtrArray FieldArray;
		GetFormFields(m_FieldName, FieldArray);
		if (FieldArray.GetSize() <= 0) return FALSE;

		CPDF_FormField* pFormField = (CPDF_FormField*)FieldArray.ElementAt(0);
		ASSERT(pFormField != NULL);

		if (pFormField->GetFieldType() != FIELDTYPE_TEXTFIELD)
			return FALSE;

		vp << (FX_INT32)pFormField->GetMaxLen();
	}
	return TRUE;
}

void JField::SetCharLimit(FSPDFDoc* pDocument, const CFX_WideString& swFieldName, FX_INT32 nControlIndex, FX_INT32 number)
{
	ASSERT(pDocument != NULL);

	CFX_PtrArray FieldArray;
	GetFormFields(pDocument, swFieldName, FieldArray);

	for (FX_INT32 i=0,isz=FieldArray.GetSize(); i<isz; i++)
	{
		CPDF_FormField* pFormField = (CPDF_FormField*)FieldArray.ElementAt(i);
		ASSERT(pFormField != NULL);

		if (pFormField->GetFieldType() == FIELDTYPE_TEXTFIELD)
		{
			if (number != pFormField->GetMaxLen())
			{
				pFormField->SetMaxLen(number);
				UpdateFormField(pDocument, pFormField, TRUE, TRUE, TRUE);
			}
		}
	}
}

FX_BOOL JField::comb(OBJ_PROP_PARAMS)
{
	ASSERT(m_pDocument != NULL);

	if (vp.IsSetting())
	{
		if (!m_bCanSet) return FALSE;

		bool bVP;
		vp >> bVP;

		if (m_bDelay)
		{
			AddDelay_Bool(FP_COMB, bVP);
		}
		else
		{
			JField::SetComb(m_pDocument, m_FieldName, m_nFormControlIndex, bVP);
		}
	}
	else
	{	
		CFX_PtrArray FieldArray;
		GetFormFields(m_FieldName,FieldArray);
		if (FieldArray.GetSize() <= 0) return FALSE;

		CPDF_FormField* pFormField = (CPDF_FormField*)FieldArray.ElementAt(0);
		ASSERT(pFormField != NULL);

		if (pFormField->GetFieldType() != FIELDTYPE_TEXTFIELD)
			return FALSE;

// 		if (pFormField->GetFieldFlags() & FIELDFLAG_COMB)
// 			vp << true;
// 		else
// 			vp << false;
	}

	return TRUE;
}

void JField::SetComb(FSPDFDoc* pDocument, const CFX_WideString& swFieldName, FX_INT32 nControlIndex, bool b)
{
	ASSERT(pDocument != NULL);

	CFX_PtrArray FieldArray;
	GetFormFields(pDocument, swFieldName, FieldArray);

	for (FX_INT32 i=0,isz=FieldArray.GetSize(); i<isz; i++)
	{
		CPDF_FormField* pFormField = (CPDF_FormField*)FieldArray.ElementAt(i);
		ASSERT(pFormField != NULL);

		if (pFormField->GetFieldType() == FIELDTYPE_TEXTFIELD)
		{
			FX_DWORD dwFlag = pFormField->GetFieldFlags();

			if (b)
			{
				dwFlag |= FIELDFLAG_DONOTSCROLL;
				dwFlag |= FIELDFLAG_DONOTSPELLCHECK;
				dwFlag |= FIELDFLAG_COMB;
			}
			else
			{
				dwFlag &= ~FIELDFLAG_COMB;
			}
			
			if (dwFlag != pFormField->GetFieldFlags())
			{
				pFormField->SetFieldFlags(dwFlag);
				UpdateFormField(pDocument, pFormField, TRUE, TRUE, TRUE);
			}
		}
	}
}

FX_BOOL JField::commitOnSelChange(OBJ_PROP_PARAMS)
{
	ASSERT(m_pDocument != NULL);

	if (vp.IsSetting())
	{
		if (!m_bCanSet) return FALSE;

		bool bVP;
		vp >> bVP;

		if (m_bDelay)
		{
			AddDelay_Bool(FP_COMMITONSELCHANGE, bVP);
		}
		else
		{
			JField::SetCommitOnSelChange(m_pDocument, m_FieldName, m_nFormControlIndex, bVP);
		}
	}
	else
	{
		CFX_PtrArray FieldArray;
		GetFormFields(m_FieldName,FieldArray);
		if (FieldArray.GetSize() <= 0) return FALSE;

		CPDF_FormField* pFormField = (CPDF_FormField*)FieldArray.ElementAt(0);
		ASSERT(pFormField != NULL);

		if (pFormField->GetFieldType() != FIELDTYPE_COMBOBOX && pFormField->GetFieldType() != FIELDTYPE_LISTBOX)
			return FALSE;

		if (pFormField->GetFieldFlags() & FIELDFLAG_COMMITONSELCHANGE)
			vp << true;
		else
			vp << false;
	}

	return TRUE;
}

void JField::SetCommitOnSelChange(FSPDFDoc* pDocument, const CFX_WideString& swFieldName, FX_INT32 nControlIndex, bool b)
{
	ASSERT(pDocument != NULL);

	CFX_PtrArray FieldArray;
	GetFormFields(pDocument, swFieldName, FieldArray);

	for (FX_INT32 i=0,isz=FieldArray.GetSize(); i<isz; i++)
	{
		CPDF_FormField* pFormField = (CPDF_FormField*)FieldArray.ElementAt(i);
		ASSERT(pFormField != NULL);

		if (pFormField->GetFieldType() == FIELDTYPE_COMBOBOX || pFormField->GetFieldType() == FIELDTYPE_LISTBOX)
		{
			FX_DWORD dwFlag = pFormField->GetFieldFlags();

			if (b)
				dwFlag |= FIELDFLAG_COMMITONSELCHANGE;
			else
				dwFlag &= ~FIELDFLAG_COMMITONSELCHANGE;
			
			if (dwFlag != pFormField->GetFieldFlags())
			{
				pFormField->SetFieldFlags(dwFlag);
				UpdateFormField(pDocument, pFormField, TRUE, TRUE, TRUE);
			}
		}
	}
}

FX_BOOL JField::currentValueIndices(OBJ_PROP_PARAMS)
{
	ASSERT(m_pDocument != NULL);

	if (vp.IsSetting())
	{
		if (!m_bCanSet) return FALSE;

		CFX_DWordArray array;

		if (vp.GetType() == VT_number)
		{
			FX_INT32 iSelecting = 0;
			vp >> iSelecting;
			array.Add(iSelecting);
		}
		else if (vp.IsArrayObject())
		{
			CFXJS_Array SelArray;
			CFXJS_Value SelValue;
			FX_INT32 iSelecting;
			vp >> SelArray;
			for (FX_INT32 i=0,sz=SelArray.GetLength(); i<sz; i++)
			{
				SelArray.GetElement(i,SelValue);
				iSelecting = (FX_INT32)SelValue;
				array.Add(iSelecting);
			}
		}
		
		if (m_bDelay)
		{
			AddDelay_WordArray(FP_CURRENTVALUEINDICES, array);
		}
		else
		{
			JField::SetCurrentValueIndices(m_pDocument, m_FieldName, m_nFormControlIndex, array);
		}
	}
	else
	{
		CFX_PtrArray FieldArray;
		GetFormFields(m_FieldName,FieldArray);
		if (FieldArray.GetSize() <= 0) return FALSE;

		CPDF_FormField* pFormField = (CPDF_FormField*)FieldArray.ElementAt(0);
		ASSERT(pFormField != NULL);

		if (pFormField->GetFieldType() != FIELDTYPE_COMBOBOX && pFormField->GetFieldType() != FIELDTYPE_LISTBOX)
			return FALSE;

		if (pFormField->CountSelectedItems() == 1)
			vp << pFormField->GetSelectedIndex(0);
		else if (pFormField->CountSelectedItems() > 1)
		{
			CFXJS_Array SelArray;
			for (FX_INT32 i=0,sz=pFormField->CountSelectedItems(); i<sz; i++)
			{
				SelArray.SetElement(i, pFormField->GetSelectedIndex(i));
			}
			vp << SelArray;
		}
		else
			vp << -1;
	}

	return TRUE;
}

void JField::SetCurrentValueIndices(FSPDFDoc* pDocument, const CFX_WideString& swFieldName, FX_INT32 nControlIndex, 
								   const CFX_DWordArray& array)
{
	ASSERT(pDocument != NULL);

	CFX_PtrArray FieldArray;
	GetFormFields(pDocument, swFieldName, FieldArray);

	for (FX_INT32 i=0,isz=FieldArray.GetSize(); i<isz; i++)
	{
		CPDF_FormField* pFormField = (CPDF_FormField*)FieldArray.ElementAt(i);
		ASSERT(pFormField != NULL);

		FX_INT32 nFieldType = pFormField->GetFieldType();
		if (nFieldType == FIELDTYPE_COMBOBOX || nFieldType == FIELDTYPE_LISTBOX)
		{
			FX_DWORD dwFieldFlags = pFormField->GetFieldFlags();
			pFormField->ClearSelection(TRUE);

			for (FX_INT32 i=0,sz=array.GetSize(); i<sz; i++)
			{
				if (i>0 && !(dwFieldFlags & (1<<21)))
				{
					break;
				}

				FX_INT32 iSelecting = (FX_INT32)array.GetAt(i);
				if (iSelecting < pFormField->CountOptions() && !pFormField->IsItemSelected(iSelecting))
					pFormField->SetItemSelection(iSelecting, TRUE);

			}
			UpdateFormField(pDocument, pFormField, TRUE, TRUE, TRUE);
		}
	}
}

FX_BOOL JField::defaultStyle(OBJ_PROP_PARAMS)
{
	// MQG sError = JSGetStringFromID(IDS_STRING_NOTSUPPORT);
	return FALSE;

	if (vp.IsSetting())
	{
		if (!m_bCanSet) return FALSE;

		;		
	}
	else
	{
		;
	}
	return TRUE;
}

void JField::SetDefaultStyle(FSPDFDoc* pDocument, const CFX_WideString& swFieldName, FX_INT32 nControlIndex)
{
}

FX_BOOL JField::defaultValue(OBJ_PROP_PARAMS)
{
	ASSERT(m_pDocument != NULL);

	if (vp.IsSetting())
	{
		if (!m_bCanSet) return FALSE;

		CFX_WideString WideStr;
		vp >> WideStr; 

		if (m_bDelay)
		{
			AddDelay_WideString(FP_DEFAULTVALUE, WideStr);
		}
		else
		{
			JField::SetDefaultValue(m_pDocument, m_FieldName, m_nFormControlIndex, WideStr);
		}
	}
	else
	{
		CFX_PtrArray FieldArray;
		GetFormFields(m_FieldName,FieldArray);
		if (FieldArray.GetSize() <= 0) return FALSE;

		CPDF_FormField* pFormField = (CPDF_FormField*)FieldArray.ElementAt(0);
		ASSERT(pFormField != NULL);

		if (pFormField->GetFieldType() == FIELDTYPE_PUSHBUTTON || 
			pFormField->GetFieldType() == FIELDTYPE_SIGNATURE)
			return FALSE;

		vp << pFormField->GetDefaultValue();
	}
	return TRUE;
}

void JField::SetDefaultValue(FSPDFDoc* pDocument, const CFX_WideString& swFieldName, FX_INT32 nControlIndex,
							const CFX_WideString& string)
{
	ASSERT(pDocument != NULL);

	CFX_PtrArray FieldArray;
	GetFormFields(pDocument, swFieldName, FieldArray);

	for (FX_INT32 i=0,isz=FieldArray.GetSize(); i<isz; i++)
	{
		CPDF_FormField* pFormField = (CPDF_FormField*)FieldArray.ElementAt(i);
		ASSERT(pFormField != NULL);

		FX_INT32 nFieldType = pFormField->GetFieldType();
		if (nFieldType == FIELDTYPE_CHECKBOX || nFieldType == FIELDTYPE_RADIOBUTTON
			|| nFieldType == FIELDTYPE_COMBOBOX || nFieldType == FIELDTYPE_LISTBOX
			|| nFieldType == FIELDTYPE_TEXTFIELD)
		{
			if (string != pFormField->GetDefaultValue())
			{
				pFormField->SetDefaultValue(string);

				CFX_WideString sOldValue = pFormField->GetValue();
				if (sOldValue == L"" || sOldValue == string)
					pFormField->SetValue(string, TRUE);

				UpdateFormField(pDocument, pFormField, TRUE, TRUE, TRUE);
			}
		}
	}
}

FX_BOOL JField::doNotScroll(OBJ_PROP_PARAMS)
{
	ASSERT(m_pDocument != NULL);

	if (vp.IsSetting())
	{
		if (!m_bCanSet) return FALSE;

		bool bVP;
		vp >> bVP;

		if (m_bDelay)
		{
			AddDelay_Bool(FP_DONOTSCROLL, bVP);
		}
		else
		{
			JField::SetDoNotScroll(m_pDocument, m_FieldName, m_nFormControlIndex, bVP);
		}
	}
	else
	{
		CFX_PtrArray FieldArray;
		GetFormFields(m_FieldName,FieldArray);
		if (FieldArray.GetSize() <= 0) return FALSE;

		CPDF_FormField* pFormField = (CPDF_FormField*)FieldArray.ElementAt(0);
		ASSERT(pFormField != NULL);

		if (pFormField->GetFieldType() != FIELDTYPE_TEXTFIELD)
			return FALSE;

		if (pFormField->GetFieldFlags() & FIELDFLAG_DONOTSCROLL)
			vp << true;
		else
			vp << false;
	}

	return TRUE;
}

void JField::SetDoNotScroll(FSPDFDoc* pDocument, const CFX_WideString& swFieldName, FX_INT32 nControlIndex, bool b)
{
	ASSERT(pDocument != NULL);

	CFX_PtrArray FieldArray;
	GetFormFields(pDocument, swFieldName, FieldArray);

	for (FX_INT32 i=0,isz=FieldArray.GetSize(); i<isz; i++)
	{
		CPDF_FormField* pFormField = (CPDF_FormField*)FieldArray.ElementAt(i);
		ASSERT(pFormField != NULL);

		if (pFormField->GetFieldType() == FIELDTYPE_TEXTFIELD)
		{
			FX_DWORD dwFlag = pFormField->GetFieldFlags();

			if (b)
				dwFlag |= FIELDFLAG_DONOTSCROLL;
			else
				dwFlag &= ~FIELDFLAG_DONOTSCROLL;

			if (dwFlag != pFormField->GetFieldFlags())
			{
				pFormField->SetFieldFlags(dwFlag);
				UpdateFormField(pDocument, pFormField, TRUE, TRUE, TRUE);
			}
		}
	}
}

FX_BOOL JField::doNotSpellCheck(OBJ_PROP_PARAMS)
{
	ASSERT(m_pDocument != NULL);

	if (vp.IsSetting())
	{
		if (!m_bCanSet) return FALSE;

		bool bVP;
		vp >> bVP;

		CFX_PtrArray FieldArray;
		GetFormFields(m_FieldName,FieldArray);

		for (FX_INT32 i=0,isz=FieldArray.GetSize(); i<isz; i++)
		{
			CPDF_FormField* pFormField = (CPDF_FormField*)FieldArray.ElementAt(i);
			ASSERT(pFormField != NULL);

			FX_INT32 nFieldType = pFormField->GetFieldType();
			if (nFieldType == FIELDTYPE_TEXTFIELD || nFieldType == FIELDTYPE_COMBOBOX)
			{
				FX_DWORD dwFlag = pFormField->GetFieldFlags();

				if (bVP)
					pFormField->SetFieldFlags(dwFlag | FIELDFLAG_DONOTSPELLCHECK);
				else
					pFormField->SetFieldFlags(dwFlag & ~FIELDFLAG_DONOTSPELLCHECK);
				
				UpdateFormField(m_pDocument, pFormField, TRUE, FALSE, FALSE);
			}
		}
	}
	else
	{
		CFX_PtrArray FieldArray;
		GetFormFields(m_FieldName,FieldArray);
		if (FieldArray.GetSize() <= 0) return FALSE;

		CPDF_FormField* pFormField = (CPDF_FormField*)FieldArray.ElementAt(0);
		ASSERT(pFormField != NULL);

		if (pFormField->GetFieldType() != FIELDTYPE_TEXTFIELD && 
			pFormField->GetFieldType() != FIELDTYPE_COMBOBOX)
			return FALSE;

		if (pFormField->GetFieldFlags() & FIELDFLAG_DONOTSPELLCHECK)
			vp << true;
		else
			vp << false;
	}

	return TRUE;
}

void JField::SetDelay(FX_BOOL bDelay)
{
	m_bDelay = bDelay;

	if (!m_bDelay)
	{
		if (m_pJSDoc)
			m_pJSDoc->DoFieldDelay(m_FieldName, m_nFormControlIndex);
	}
}

FX_BOOL JField::delay(OBJ_PROP_PARAMS)
{
	if (vp.IsSetting())
	{
		if (!m_bCanSet) return FALSE;
		
		bool bVP;
		vp >> bVP;

		SetDelay(bVP);
	}
	else
	{
		vp << m_bDelay;
	}
	return TRUE;
}

FX_BOOL JField::display(OBJ_PROP_PARAMS)
{
	ASSERT(m_pDocument != NULL);

	if (vp.IsSetting())
	{
		if (!m_bCanSet) return FALSE;

		FX_INT32 nVP;
		vp >> nVP;	

		if (m_bDelay)
		{
			AddDelay_Int(FP_DISPLAY, nVP);
		}
		else
		{
			JField::SetDisplay(m_pDocument, m_FieldName, m_nFormControlIndex, nVP);
		}
	}
	else
	{
		CFX_PtrArray FieldArray;
		GetFormFields(m_FieldName,FieldArray);
		if (FieldArray.GetSize() <= 0) return FALSE;

		CPDF_FormField* pFormField = (CPDF_FormField*)FieldArray.ElementAt(0);
		ASSERT(pFormField != NULL);

		FSFormImp* pInterForm = (FSFormImp*)m_pDocument->GetForm();
		ASSERT(pInterForm != NULL);

// 		FSFormControl* pWidget = pInterForm->GetWidget(GetSmartFieldControl(pFormField));
// 		if (!pWidget)return FALSE;
// 
// 		FX_DWORD dwFlag = pWidget->GetFlags();
// 
// 		if (ANNOTFLAG_INVISIBLE & dwFlag || ANNOTFLAG_HIDDEN & dwFlag) 
// 		{
// 			vp << (FX_INT32)1;
// 		}
// 		else 
// 		{
// 			if (ANNOTFLAG_PRINT & dwFlag)
// 			{
// 				if (ANNOTFLAG_NOVIEW & dwFlag)
// 				{
// 					vp << (FX_INT32)3;
// 				}
// 				else
// 				{
// 					vp << (FX_INT32)0;
// 				}
// 			}
// 			else
// 			{
// 				vp << (FX_INT32)2;
// 			}				
// 		}
 	}

	return TRUE;
}

void JField::SetDisplay(FSPDFDoc* pDocument, const CFX_WideString& swFieldName, FX_INT32 nControlIndex, FX_INT32 number)
{
	ASSERT(pDocument != NULL);

	FSFormImp* pInterForm = ((FSPDFDocImp*)pDocument)->GetForm();
	ASSERT(pInterForm != NULL);

	CFX_PtrArray FieldArray;
	GetFormFields(pDocument, swFieldName, FieldArray);

	for (FX_INT32 i=0,isz=FieldArray.GetSize(); i<isz; i++)
	{
		CPDF_FormField* pFormField = (CPDF_FormField*)FieldArray.ElementAt(i);
		ASSERT(pFormField != NULL);

		if (nControlIndex < 0)
		{
			FX_BOOL bSet = FALSE;
			for (FX_INT32 j=0,jsz = pFormField->CountControls(); j<jsz; j++)
			{
				CPDF_FormControl* pFormControl = pFormField->GetControl(j);
				ASSERT(pFormControl != NULL);

				if (CPDF_FormControl* pControl = pFormField->GetControl(j))
				{					
					FX_DWORD dwFlag = pControl->GetWidget()->GetInteger(FX_BSTRC("F"));
					switch (number)
					{
					case 0:
						dwFlag &= (~ANNOTFLAG_INVISIBLE);
						dwFlag &= (~ANNOTFLAG_HIDDEN);
						dwFlag &= (~ANNOTFLAG_NOVIEW);
						dwFlag |= ANNOTFLAG_PRINT;							
						break;
					case 1:
						dwFlag &= (~ANNOTFLAG_INVISIBLE);
						dwFlag &= (~ANNOTFLAG_NOVIEW);
						dwFlag |= (ANNOTFLAG_HIDDEN | ANNOTFLAG_PRINT);
						break;
					case 2:
						dwFlag &= (~ANNOTFLAG_INVISIBLE);
						dwFlag &= (~ANNOTFLAG_PRINT);
						dwFlag &= (~ANNOTFLAG_HIDDEN);
						dwFlag &= (~ANNOTFLAG_NOVIEW);
						break;
					case 3:
						dwFlag |= ANNOTFLAG_NOVIEW;
						dwFlag |= ANNOTFLAG_PRINT;
						dwFlag &= (~ANNOTFLAG_HIDDEN);
						break;
					}	

					if (dwFlag != pControl->GetWidget()->GetInteger(FX_BSTRC("F")))
					{
						pControl->GetWidget()->SetAtInteger(FX_BSTRC("F"),dwFlag);	
						bSet = TRUE;
					}
				}
			}		
			
			if (bSet) UpdateFormField(pDocument, pFormField, TRUE, FALSE, TRUE);
		}
		else
		{
			if (CPDF_FormControl* pFormControl = pFormField->GetControl(nControlIndex))
			{
				FX_DWORD dwFlag = pFormControl->GetWidget()->GetInteger(FX_BSTRC("F"));

				switch (number)
				{
				case 0:
					dwFlag &= (~ANNOTFLAG_INVISIBLE);
					dwFlag &= (~ANNOTFLAG_HIDDEN);
					dwFlag &= (~ANNOTFLAG_NOVIEW);
					dwFlag |= ANNOTFLAG_PRINT;							
					break;
				case 1:
					dwFlag &= (~ANNOTFLAG_INVISIBLE);
					dwFlag &= (~ANNOTFLAG_NOVIEW);
					dwFlag |= (ANNOTFLAG_HIDDEN | ANNOTFLAG_PRINT);
					break;
				case 2:
					dwFlag &= (~ANNOTFLAG_INVISIBLE);
					dwFlag &= (~ANNOTFLAG_PRINT);
					dwFlag &= (~ANNOTFLAG_HIDDEN);
					dwFlag &= (~ANNOTFLAG_NOVIEW);
					break;
				case 3:
					dwFlag |= ANNOTFLAG_NOVIEW;
					dwFlag |= ANNOTFLAG_PRINT;
					dwFlag &= (~ANNOTFLAG_HIDDEN);
					break;
				}	

				if (dwFlag != pFormControl->GetWidget()->GetInteger(FX_BSTRC("F")))
				{
					pFormControl->GetWidget()->SetAtInteger(FX_BSTRC("F"),dwFlag);	
					UpdateFormControl(pDocument, pFormControl, TRUE, FALSE, TRUE);	
				}
				
			}
		}
	}
}

FX_BOOL JField::doc(OBJ_PROP_PARAMS)
{
	ASSERT(m_pJSDoc != NULL);

	if (!vp.IsGetting())return FALSE;

	vp << (CFXJS_Object*)(*m_pJSDoc);

	return TRUE;
}

FX_BOOL JField::editable(OBJ_PROP_PARAMS)
{
	ASSERT(m_pDocument != NULL);

	if (vp.IsSetting())
	{
		if (!m_bCanSet) return FALSE;

		bool bVP;
		vp >> bVP;

		CFX_PtrArray FieldArray;
		GetFormFields(m_FieldName, FieldArray);

		for (FX_INT32 i=0,isz=FieldArray.GetSize(); i<isz; i++)
		{
			CPDF_FormField* pFormField = (CPDF_FormField*)FieldArray.ElementAt(i);
			ASSERT(pFormField != NULL);

			if (pFormField->GetFieldType() != FIELDTYPE_COMBOBOX)
				continue;

			FX_DWORD dwFlag = pFormField->GetFieldFlags();

			if (bVP)
				dwFlag |= FIELDFLAG_EDIT;
			else
				dwFlag &= ~FIELDFLAG_EDIT;

			if (dwFlag != pFormField->GetFieldFlags())
			{
				pFormField->SetFieldFlags(dwFlag);
				UpdateFormField(m_pDocument, pFormField, TRUE, FALSE, TRUE);
			}
		}
	}
	else
	{
		CFX_PtrArray FieldArray;
		GetFormFields(m_FieldName, FieldArray);
		if (FieldArray.GetSize() <= 0) return FALSE;

		CPDF_FormField* pFormField = (CPDF_FormField*)FieldArray.ElementAt(0);
		ASSERT(pFormField != NULL);

		if (pFormField->GetFieldType() != FIELDTYPE_COMBOBOX)
			return FALSE;

		if (pFormField->GetFieldFlags() & FIELDFLAG_EDIT)
			vp << true;
		else
			vp << false;
	}

	return TRUE;
}

FX_BOOL JField::exportValues(OBJ_PROP_PARAMS)
{
	ASSERT(m_pDocument != NULL);

	CFX_PtrArray FieldArray;
	GetFormFields(m_FieldName,FieldArray);
	if (FieldArray.GetSize() <= 0) return FALSE;

	CPDF_FormField* pFormField = (CPDF_FormField*)FieldArray.ElementAt(0);
	ASSERT(pFormField != NULL);

	if (pFormField->GetFieldType() != FIELDTYPE_CHECKBOX && 
		pFormField->GetFieldType() != FIELDTYPE_RADIOBUTTON)
		return FALSE;

	if (vp.IsSetting())
	{	
		if (!m_bCanSet) return FALSE;
		if (!vp.IsArrayObject())return FALSE;

		CFXJS_Array ExportValusArray;
		vp >> ExportValusArray;

		if (m_nFormControlIndex < 0)
		{
			for (FX_INT32 i=0,sz=pFormField->CountControls(); i<sz; i++)
			{
				CPDF_FormControl* pFormControl = pFormField->GetControl(i);
				ASSERT(pFormControl != NULL);

				CFXJS_Value ExportValue;
				ExportValusArray.GetElement(i, ExportValue);
				CFX_WideString csOptValue = (FX_LPCWSTR)ExportValue;
				pFormControl->SetExportValue(csOptValue,TRUE);
			}
			UpdateFormField(m_pDocument, pFormField, TRUE, FALSE, TRUE);
		}
		else
		{
			CPDF_FormControl* pFormControl = pFormField->GetControl(m_nFormControlIndex);
			if (!pFormControl) return FALSE;

			CFXJS_Value ExportValue;
			ExportValusArray.GetElement(0, ExportValue);
			CFX_WideString csOptValue = (FX_LPCWSTR)ExportValue;
			pFormControl->SetExportValue(csOptValue,TRUE);

			UpdateFormControl(m_pDocument, pFormControl, TRUE, FALSE, TRUE);
		}
	}
	else
	{
		CFXJS_Array ExportValusArray;

		if (m_nFormControlIndex < 0)
		{
			for (FX_INT32 i=0,sz=pFormField->CountControls(); i<sz; i++)
			{
				CPDF_FormControl* pFormControl = pFormField->GetControl(i);
				ASSERT(pFormControl != NULL);

				ExportValusArray.SetElement(i, (FX_LPCWSTR)pFormControl->GetExportValue());
			}
		}
		else
		{
			CPDF_FormControl* pFormControl = pFormField->GetControl(m_nFormControlIndex);
			if (!pFormControl) return FALSE;

			ExportValusArray.SetElement(0, (FX_LPCWSTR)pFormControl->GetExportValue());
		}

		vp << ExportValusArray;
	}

	return TRUE;
}

FX_BOOL JField::fileSelect(OBJ_PROP_PARAMS)
{
	ASSERT(m_pDocument != NULL);

	CFX_PtrArray FieldArray;
	GetFormFields(m_FieldName, FieldArray);
	if (FieldArray.GetSize() <= 0) return FALSE;

	CPDF_FormField* pFormField = (CPDF_FormField*)FieldArray.ElementAt(0);
	ASSERT(pFormField != NULL);

	if (pFormField->GetFieldType() != FIELDTYPE_TEXTFIELD)
		return FALSE;

	if (vp.IsSetting())
	{
		if (!m_bCanSet) return FALSE;

		bool bVP;
		vp >> bVP;

		FX_DWORD dwFlag = pFormField->GetFieldFlags();

		if (bVP)
			dwFlag |= FIELDFLAG_FILESELECT;
		else
			dwFlag &= ~FIELDFLAG_FILESELECT;

		if (dwFlag != pFormField->GetFieldFlags())
		{		
			pFormField->SetFieldFlags(dwFlag);
			UpdateFormField(m_pDocument, pFormField, TRUE, FALSE, TRUE);
		}
	}
	else
	{
		if (pFormField->GetFieldFlags() & FIELDFLAG_FILESELECT)
			vp << true;
		else
			vp << false;
	}

	return TRUE;
}

FX_BOOL JField::fillColor(OBJ_PROP_PARAMS)
{
	ASSERT(m_pDocument != NULL);

	CFXJS_Array crArray;

	CFX_PtrArray FieldArray;
	GetFormFields(m_FieldName,FieldArray);
	if (FieldArray.GetSize() <= 0) return FALSE;

	if (vp.IsSetting())
	{
		if (!m_bCanSet) return FALSE;
		if (!vp.IsArrayObject()) return FALSE;

		vp >> crArray;

		CPWL_Color color;
		jcolor::ConvertArrayToPWLColor(crArray, color);

		if (m_bDelay)
		{
			AddDelay_Color(FP_FILLCOLOR, color);
		}
		else
		{
			JField::SetFillColor(m_pDocument, m_FieldName, m_nFormControlIndex, color);
		}
	}
	else
	{
		CPDF_FormField* pFormField = (CPDF_FormField*)FieldArray.ElementAt(0);
		ASSERT(pFormField != NULL);

		CPDF_FormControl* pFormControl = GetSmartFieldControl(pFormField);
		if (!pFormControl)return FALSE;

		FX_INT32 iColorType;
		pFormControl->GetBackgroundColor(iColorType);

		CPWL_Color color;

		if (iColorType == COLORTYPE_TRANSPARENT)
		{
			color = CPWL_Color(COLORTYPE_TRANSPARENT);
		}
		else if (iColorType == COLORTYPE_GRAY)
		{
			color = CPWL_Color(COLORTYPE_GRAY, pFormControl->GetOriginalBackgroundColor(0));
		}
		else if (iColorType == COLORTYPE_RGB)
		{
			color = CPWL_Color(COLORTYPE_RGB, pFormControl->GetOriginalBackgroundColor(0),
				pFormControl->GetOriginalBackgroundColor(1),
				pFormControl->GetOriginalBackgroundColor(2));
		}
		else if (iColorType == COLORTYPE_CMYK)
		{
			color = CPWL_Color(COLORTYPE_CMYK, pFormControl->GetOriginalBackgroundColor(0),
				pFormControl->GetOriginalBackgroundColor(1),
				pFormControl->GetOriginalBackgroundColor(2),
				pFormControl->GetOriginalBackgroundColor(3));
		}
		else
			return FALSE;

		jcolor::ConvertPWLColorToArray(color, crArray);
        vp  <<  crArray;
	}

	return TRUE;
}

void JField::SetFillColor(FSPDFDoc* pDocument, const CFX_WideString& swFieldName, FX_INT32 nControlIndex, const CPWL_Color& color)
{
	ASSERT(pDocument != NULL);

	CFX_PtrArray FieldArray;
	GetFormFields(pDocument, swFieldName, FieldArray);

	FX_ARGB nNewColor = CPWL_Utils::PWLColorToFXColor(color);
	FX_INT32 nNewColorType = color.nColorType;

	for (FX_INT32 i=0,isz=FieldArray.GetSize(); i<isz; i++)
	{
		CPDF_FormField* pFormField = (CPDF_FormField*)FieldArray.ElementAt(i);
		ASSERT(pFormField != NULL);

		for (FX_INT32 i=0,isz = FieldArray.GetSize(); i<isz; i++)
		{
			CPDF_FormField* pFormField = (CPDF_FormField*)FieldArray.ElementAt(i);
			ASSERT(pFormField != NULL);

			if (nControlIndex < 0)
			{
				FX_BOOL bSet = FALSE;
				for (FX_INT32 j=0,jsz = pFormField->CountControls(); j<jsz; j++)
				{
					CPDF_FormControl* pFormControl = pFormField->GetControl(j);
					ASSERT(pFormControl != NULL);

					FX_INT32 nOldColorType = 0;
					FX_ARGB nOldColor = pFormControl->GetBackgroundColor(nOldColorType);

					if (nOldColor != nNewColor || nOldColorType != nNewColorType)
					{
						pFormControl->SetBackgroundColor(nNewColorType, nNewColor);
						bSet = TRUE;
					}
				}

				if (bSet)
					UpdateFormField(pDocument, pFormField, TRUE, TRUE, TRUE);
			}
			else
			{
				CPDF_FormControl* pFormControl = pFormField->GetControl(nControlIndex);
				if (pFormControl)
				{
					FX_INT32 nOldColorType = 0;
					FX_ARGB nOldColor = pFormControl->GetBackgroundColor(nOldColorType);
					if (nOldColor != nNewColor || nOldColorType != nNewColorType)
					{
						pFormControl->SetBackgroundColor(nNewColorType, nNewColor);
						UpdateFormControl(pDocument, pFormControl, TRUE, TRUE, TRUE);
					}
				}
			}
		}
	}
}

FX_BOOL JField::hidden(OBJ_PROP_PARAMS)
{
	ASSERT(m_pDocument != NULL);

	if (vp.IsSetting())
	{
		if (!m_bCanSet) return FALSE;

		bool bVP;
		vp >> bVP;

		if (m_bDelay)
		{
			AddDelay_Bool(FP_HIDDEN, bVP);
		}
		else
		{
			JField::SetHidden(m_pDocument, m_FieldName, m_nFormControlIndex, bVP);
		}
	}
	else
	{
		CFX_PtrArray FieldArray;
		GetFormFields(m_FieldName,FieldArray);
		if (FieldArray.GetSize() <= 0) return FALSE;

		CPDF_FormField* pFormField = (CPDF_FormField*)FieldArray.ElementAt(0);
		ASSERT(pFormField != NULL);

		FSFormImp* pInterForm = (FSFormImp*)m_pDocument->GetForm();
		ASSERT(pInterForm != NULL);

		CPDF_FormControl* pFormControl = GetSmartFieldControl(pFormField);
		if (!pFormControl) return FALSE;

		FX_DWORD dwFlags = pFormControl->GetWidget()->GetInteger(FX_BSTRC("F"));

		if (ANNOTFLAG_INVISIBLE & dwFlags || ANNOTFLAG_HIDDEN & dwFlags) 
		{
			vp << true;
		}
		else 
			vp << false;
	}

	return TRUE;
}

void JField::SetHidden(FSPDFDoc* pDocument, const CFX_WideString& swFieldName, FX_INT32 nControlIndex, bool b)
{
	ASSERT(pDocument != NULL);

	FSFormImp* pInterForm = ((FSPDFDocImp*)pDocument)->GetForm();
	ASSERT(pInterForm != NULL);

	CFX_PtrArray FieldArray;
	GetFormFields(pDocument, swFieldName, FieldArray);

	for (FX_INT32 i=0,isz=FieldArray.GetSize(); i<isz; i++)
	{
		CPDF_FormField* pFormField = (CPDF_FormField*)FieldArray.ElementAt(i);
		ASSERT(pFormField != NULL);

		if (nControlIndex < 0)
		{
			FX_BOOL bSet = FALSE;
			for (FX_INT32 j=0,jsz = pFormField->CountControls(); j<jsz; j++)
			{
				if (CPDF_FormControl* pControl = pFormField->GetControl(j))
				{					
					FX_DWORD dwFlags = pControl->GetWidget()->GetInteger(FX_BSTRC("F"));
					
					if (b)
					{
						dwFlags &= (~ANNOTFLAG_INVISIBLE);
						dwFlags &= (~ANNOTFLAG_NOVIEW);
						dwFlags |= (ANNOTFLAG_HIDDEN | ANNOTFLAG_PRINT);
					}
					else
					{
						dwFlags &= (~ANNOTFLAG_INVISIBLE);
						dwFlags &= (~ANNOTFLAG_HIDDEN);
						dwFlags &= (~ANNOTFLAG_NOVIEW);
						dwFlags |= ANNOTFLAG_PRINT;	
					}

					if (dwFlags != pControl->GetWidget()->GetInteger(FX_BSTRC("F")))
					{
						pControl->GetWidget()->SetAtInteger(FX_BSTRC("F"),dwFlags);	
						bSet = TRUE;
					}
				}
			}

			if (bSet)
				UpdateFormField(pDocument, pFormField, TRUE, FALSE, TRUE);	
		}
		else
		{
			if (CPDF_FormControl* pFormControl = pFormField->GetControl(nControlIndex))
			{
				FX_DWORD dwFlags = pFormControl->GetWidget()->GetInteger(FX_BSTRC("F"));
					
				if (b)
				{
					dwFlags &= (~ANNOTFLAG_INVISIBLE);
					dwFlags &= (~ANNOTFLAG_NOVIEW);
					dwFlags |= (ANNOTFLAG_HIDDEN | ANNOTFLAG_PRINT);
				}
				else
				{
					dwFlags &= (~ANNOTFLAG_INVISIBLE);
					dwFlags &= (~ANNOTFLAG_HIDDEN);
					dwFlags &= (~ANNOTFLAG_NOVIEW);
					dwFlags |= ANNOTFLAG_PRINT;	
				}

				if (dwFlags != pFormControl->GetWidget()->GetInteger(FX_BSTRC("F")))
				{
					pFormControl->GetWidget()->SetAtInteger(FX_BSTRC("F"),dwFlags);	
					UpdateFormControl(pDocument, pFormControl, TRUE, FALSE, TRUE);	
				}
			}
		}
	}
}

FX_BOOL JField::highlight(OBJ_PROP_PARAMS)
{
	ASSERT(m_pDocument != NULL);

	if (vp.IsSetting())
	{
		if (!m_bCanSet) return FALSE;

		CFX_ByteString strMode;
		vp >> strMode;

		if (m_bDelay)
		{
			AddDelay_String(FP_HIGHLIGHT, strMode);
		}
		else
		{
			JField::SetHighlight(m_pDocument, m_FieldName, m_nFormControlIndex, strMode);
		}
	}
	else
	{
		CFX_PtrArray FieldArray;
		GetFormFields(m_FieldName,FieldArray);
		if (FieldArray.GetSize() <= 0) return FALSE;

		CPDF_FormField* pFormField = (CPDF_FormField*)FieldArray.ElementAt(0);
		ASSERT(pFormField != NULL);

		if (pFormField->GetFieldType() != FIELDTYPE_PUSHBUTTON)
			return FALSE;

		CPDF_FormControl* pFormControl = GetSmartFieldControl(pFormField);
		if (!pFormControl) return FALSE;

		FX_INT32 eHM = pFormControl->GetHighlightingMode();
		switch (eHM)
		{
		case CPDF_FormControl::None:
			vp  <<  (FX_LPCWSTR)L"none";
			break;
		case CPDF_FormControl::Push:
			vp  <<  (FX_LPCWSTR)L"push";
			break;
		case CPDF_FormControl::Invert:
			vp  <<  (FX_LPCWSTR)L"invert";
			break;
		case CPDF_FormControl::Outline:
			vp  <<  (FX_LPCWSTR)L"outline";
			break;
		case CPDF_FormControl::Toggle:
			 vp  <<  (FX_LPCWSTR)L"toggle";
			 break;
		}
	}

	return TRUE;
}

void JField::SetHighlight(FSPDFDoc* pDocument, const CFX_WideString& swFieldName, FX_INT32 nControlIndex, const CFX_ByteString& string)
{
	ASSERT(pDocument != NULL);

	CFX_PtrArray FieldArray;
	GetFormFields(pDocument, swFieldName, FieldArray);

	for (FX_INT32 i=0,isz=FieldArray.GetSize(); i<isz; i++)
	{
		CPDF_FormField* pFormField = (CPDF_FormField*)FieldArray.ElementAt(i);
		ASSERT(pFormField != NULL);

		if (pFormField->GetFieldType() == FIELDTYPE_PUSHBUTTON)
		{
			CPDF_FormControl::HighlightingMode eHM ;

			if (string =="none")
				eHM = CPDF_FormControl::None;
			else if (string =="invert")
				eHM = CPDF_FormControl::Invert;
			else if (string =="push")
				eHM = CPDF_FormControl::Push;
			else if (string =="outline")
				eHM = CPDF_FormControl::Outline;
			else if (string =="toggle")
				eHM = CPDF_FormControl::Toggle;
			else
				return;

			if (nControlIndex < 0)
			{
				FX_BOOL bSet = FALSE;
				for (FX_INT32 i=0,sz = pFormField->CountControls(); i<sz; i++)
				{
					CPDF_FormControl* pFormControl = pFormField->GetControl(i);
					ASSERT(pFormControl != NULL);

					if (eHM != pFormControl->GetHighlightingMode())
					{
						pFormControl->SetHighlightingMode(eHM);
						bSet = TRUE;
					}
				}
				if (bSet) 
					UpdateFormField(pDocument, pFormField, TRUE, TRUE, TRUE);
			}
			else
			{
				if (CPDF_FormControl* pFormControl = pFormField->GetControl(nControlIndex))
				{
					if (eHM != pFormControl->GetHighlightingMode())
					{
						pFormControl->SetHighlightingMode(eHM);
						UpdateFormControl(pDocument, pFormControl, TRUE, TRUE, TRUE);
					}
				}
			}
		}
	}
}

FX_BOOL JField::lineWidth(OBJ_PROP_PARAMS)
{
	ASSERT(m_pDocument != NULL);

	if (vp.IsSetting())
	{
		if (!m_bCanSet) return FALSE;

		FX_INT32 iWidth;
		vp >> iWidth;

		if (m_bDelay)
		{
			AddDelay_Int(FP_LINEWIDTH, iWidth);
		}
		else
		{
			JField::SetLineWidth(m_pDocument, m_FieldName, m_nFormControlIndex, iWidth);
		}
	}
	else
	{
		CFX_PtrArray FieldArray;
		GetFormFields(m_FieldName,FieldArray);
		if (FieldArray.GetSize() <= 0) return FALSE;

		CPDF_FormField* pFormField = (CPDF_FormField*)FieldArray.ElementAt(0);
		ASSERT(pFormField != NULL);

		CPDF_FormControl* pFormControl = GetSmartFieldControl(pFormField);
		if (!pFormControl) return FALSE;

		FSFormImp* pInterForm = (FSFormImp*)m_pDocument->GetForm();
		ASSERT(pInterForm != NULL);

// 		FSFormControl* pWidget = pInterForm->GetWidget(pFormField->GetControl(0));
// 		if (!pWidget) return FALSE;
// 
// 		vp << (FX_INT32)pWidget->GetBorderWidth();
	}

	return TRUE;
}

void JField::SetLineWidth(FSPDFDoc* pDocument, const CFX_WideString& swFieldName, FX_INT32 nControlIndex, FX_INT32 number)
{
	ASSERT(pDocument != NULL);

	FSFormImp* pInterForm = ((FSPDFDocImp*)pDocument)->GetForm();
	ASSERT(pInterForm != NULL);

	CFX_PtrArray FieldArray;
	GetFormFields(pDocument, swFieldName, FieldArray);

	for (FX_INT32 i=0,isz=FieldArray.GetSize(); i<isz; i++)
	{
		CPDF_FormField* pFormField = (CPDF_FormField*)FieldArray.ElementAt(i);
		ASSERT(pFormField != NULL);

		if (nControlIndex < 0)
		{
			FX_BOOL bSet = FALSE;
			for (FX_INT32 j=0,jsz=pFormField->CountControls(); j<jsz; j++)
			{
				CPDF_FormControl* pFormControl = pFormField->GetControl(j);
				ASSERT(pFormControl != NULL);
				
// 				if (FSFormControl* pWidget = pInterForm->GetWidget(pFormControl))
// 				{
// 					if (number != pWidget->GetBorderWidth())
// 					{
// 						pWidget->SetBorderWidth(number);
// 						bSet = TRUE;
// 					}
// 				}
			}
			if (bSet) UpdateFormField(pDocument, pFormField, TRUE, TRUE, TRUE);
		}
		else
		{
			if (CPDF_FormControl* pFormControl = pFormField->GetControl(nControlIndex))
			{
// 				if (FSFormControl* pWidget = pInterForm->GetWidget(pFormControl))
// 				{
// 					if (number != pWidget->GetBorderWidth())
// 					{
// 						pWidget->SetBorderWidth(number);
// 						UpdateFormControl(pDocument, pFormControl, TRUE, TRUE, TRUE);
// 					}
// 				}
			}
		}
	}
}

FX_BOOL JField::multiline(OBJ_PROP_PARAMS)
{
	ASSERT(m_pDocument != NULL);

	if (vp.IsSetting())
	{
		if (!m_bCanSet) return FALSE;

		bool bVP;
		vp >> bVP;

		if (m_bDelay)
		{
			AddDelay_Bool(FP_MULTILINE, bVP);
		}
		else
		{
			JField::SetMultiline(m_pDocument, m_FieldName, m_nFormControlIndex, bVP);
		}
	}
	else
	{
		CFX_PtrArray FieldArray;
		GetFormFields(m_FieldName, FieldArray);
		if (FieldArray.GetSize() <= 0) return FALSE;

		CPDF_FormField* pFormField = (CPDF_FormField*)FieldArray.ElementAt(0);
		ASSERT(pFormField != NULL);

		if (pFormField->GetFieldType() != FIELDTYPE_TEXTFIELD)
			return FALSE;

		if (pFormField->GetFieldFlags() & FIELDFLAG_MULTILINE)
			vp << true;
		else
			vp << false;
	}

	return TRUE;
}

void JField::SetMultiline(FSPDFDoc* pDocument, const CFX_WideString& swFieldName, FX_INT32 nControlIndex, bool b)
{
	ASSERT(pDocument != NULL);

	CFX_PtrArray FieldArray;
	GetFormFields(pDocument, swFieldName, FieldArray);

	for (FX_INT32 i=0,isz=FieldArray.GetSize(); i<isz; i++)
	{
		CPDF_FormField* pFormField = (CPDF_FormField*)FieldArray.ElementAt(i);
		ASSERT(pFormField != NULL);

		if (pFormField->GetFieldType() == FIELDTYPE_TEXTFIELD)
		{
			FX_DWORD dwFlags = pFormField->GetFieldFlags();

			if (b)
				dwFlags |= FIELDFLAG_MULTILINE;
			else
				dwFlags &= ~FIELDFLAG_MULTILINE;

			if (dwFlags != pFormField->GetFieldFlags())
			{
				pFormField->SetFieldFlags(dwFlags);
				UpdateFormField(pDocument, pFormField, TRUE, TRUE, TRUE);
			}
		}
	}
}

FX_BOOL JField::multipleSelection(OBJ_PROP_PARAMS)
{
	ASSERT(m_pDocument != NULL);

	if (vp.IsSetting())
	{
		if (!m_bCanSet) return FALSE;

		bool bVP;
		vp >> bVP;

		if (m_bDelay)
		{
			AddDelay_Bool(FP_MULTIPLESELECTION, bVP);
		}
		else
		{
			JField::SetMultipleSelection(m_pDocument, m_FieldName, m_nFormControlIndex, bVP);
		}
	}
	else
	{
		CFX_PtrArray FieldArray;
		GetFormFields(m_FieldName,FieldArray);
		if (FieldArray.GetSize() <= 0) return FALSE;

		CPDF_FormField* pFormField = (CPDF_FormField*)FieldArray.ElementAt(0);
		ASSERT(pFormField != NULL);

		if (pFormField->GetFieldType() != FIELDTYPE_LISTBOX)
			return FALSE;

		if (pFormField->GetFieldFlags() & FIELDFLAG_MULTISELECT)
			vp << true;
		else
			vp << false;
	}

	return TRUE;
}

void JField::SetMultipleSelection(FSPDFDoc* pDocument, const CFX_WideString& swFieldName, FX_INT32 nControlIndex, bool b)
{
	ASSERT(pDocument != NULL);

	CFX_PtrArray FieldArray;
	GetFormFields(pDocument, swFieldName, FieldArray);

	for (FX_INT32 i=0,isz=FieldArray.GetSize(); i<isz; i++)
	{
		CPDF_FormField* pFormField = (CPDF_FormField*)FieldArray.ElementAt(i);
		ASSERT(pFormField != NULL);

		if (pFormField->GetFieldType() == FIELDTYPE_TEXTFIELD)
		{
			FX_DWORD dwFlags = pFormField->GetFieldFlags();
		
			if (b)
				dwFlags |= FIELDFLAG_MULTISELECT;
			else
				dwFlags &= ~FIELDFLAG_MULTISELECT;

			if (dwFlags != pFormField->GetFieldFlags())
			{
				pFormField->SetFieldFlags(dwFlags);
				UpdateFormField(pDocument, pFormField, TRUE, TRUE, TRUE);
			}
		}
	}
}

FX_BOOL JField::name(OBJ_PROP_PARAMS)
{
	if (!vp.IsGetting()) return FALSE;

	CFX_PtrArray FieldArray;
	GetFormFields(m_FieldName, FieldArray);
	if (FieldArray.GetSize() <= 0) return FALSE;

   	vp << m_FieldName;

	return TRUE;
}

FX_BOOL JField::numItems(OBJ_PROP_PARAMS)
{	
	CFX_PtrArray FieldArray;
	GetFormFields(m_FieldName, FieldArray);
	if (FieldArray.GetSize() <= 0) return FALSE;

	CPDF_FormField* pFormField = (CPDF_FormField*)FieldArray.ElementAt(0);
	ASSERT(pFormField != NULL);

	if (pFormField->GetFieldType() != FIELDTYPE_COMBOBOX &&
		pFormField->GetFieldType() != FIELDTYPE_LISTBOX)
		return FALSE;

	if (!vp.IsGetting()) return FALSE;

	vp << (FX_INT32)pFormField->CountOptions();

	return TRUE;
}

FX_BOOL JField::page(OBJ_PROP_PARAMS)
{
	if (!vp.IsGetting()) return FALSE;

	CFX_PtrArray FieldArray;
	GetFormFields(m_FieldName, FieldArray);
	if (FieldArray.GetSize() <= 0) return FALSE;

	CPDF_FormField* pFormField = (CPDF_FormField*)FieldArray.ElementAt(0);
	if (!pFormField) return FALSE;

	ASSERT(m_pDocument != NULL);

	FSFormImp* pInterForm = (FSFormImp*)m_pDocument->GetForm();
	ASSERT(pInterForm != NULL);

	CFX_PtrArray widgetArray;
	//pInterForm->GetWidgets(pFormField, widgetArray);

	if (widgetArray.GetSize() > 0)
	{
		CFXJS_Array PageArray;

		for (FX_INT32 i=0,sz=widgetArray.GetSize(); i<sz; i++)
		{
			FSFormControlImp* pWidget = (FSFormControlImp*)widgetArray.GetAt(i);
			ASSERT(pWidget != NULL);
			FX_INT32 nPageIndex = 0;
			pWidget->GetPageIndex(&nPageIndex);
			PageArray.SetElement(i, nPageIndex);
		}

		vp << PageArray;
	}
	else
	{
		vp << (FX_INT32) -1;
	}

	return TRUE;
}

FX_BOOL JField::password(OBJ_PROP_PARAMS)
{
	ASSERT(m_pDocument != NULL);

	if (vp.IsSetting())
	{
		if (!m_bCanSet) return FALSE;

		bool bVP;
		vp >> bVP;

		if (m_bDelay)
		{
			AddDelay_Bool(FP_PASSWORD, bVP);
		}
		else
		{
			JField::SetPassword(m_pDocument, m_FieldName, m_nFormControlIndex, bVP);
		}	
	}
	else
	{
		CFX_PtrArray FieldArray;
		GetFormFields(m_FieldName,FieldArray);
		if (FieldArray.GetSize() <= 0) return FALSE;

		CPDF_FormField* pFormField = (CPDF_FormField*)FieldArray.ElementAt(0);
		ASSERT(pFormField != NULL);

		if (pFormField->GetFieldType() != FIELDTYPE_TEXTFIELD)
			return FALSE;

		if (pFormField->GetFieldFlags() & FIELDFLAG_PASSWORD)
			vp << true;
		else
			vp << false;
	}

	return TRUE;
}

void JField::SetPassword(FSPDFDoc* pDocument, const CFX_WideString& swFieldName, FX_INT32 nControlIndex, bool b)
{
	ASSERT(pDocument != NULL);

	CFX_PtrArray FieldArray;
	GetFormFields(pDocument, swFieldName, FieldArray);

	for (FX_INT32 i=0,isz=FieldArray.GetSize(); i<isz; i++)
	{
		CPDF_FormField* pFormField = (CPDF_FormField*)FieldArray.ElementAt(i);
		ASSERT(pFormField != NULL);

		if (pFormField->GetFieldType() == FIELDTYPE_TEXTFIELD)
		{
			FX_DWORD dwFlags = pFormField->GetFieldFlags();

			if (b)
				dwFlags |= FIELDFLAG_PASSWORD;
			else
				dwFlags &= ~FIELDFLAG_PASSWORD;
			
			if (dwFlags != pFormField->GetFieldFlags())
			{
				pFormField->SetFieldFlags(dwFlags);
				UpdateFormField(pDocument, pFormField, TRUE, TRUE, TRUE);
			}
		}
	}
}

FX_BOOL JField::print(OBJ_PROP_PARAMS)
{
	ASSERT(m_pDocument != NULL);

	CFX_PtrArray FieldArray;
	GetFormFields(m_FieldName, FieldArray);
	if (FieldArray.GetSize() <= 0) return FALSE;

	if (vp.IsSetting())//setting
	{
		if (!m_bCanSet) return FALSE;

		bool bVP;
		vp >> bVP;

		for (FX_INT32 i=0,isz=FieldArray.GetSize(); i<isz; i++)
		{
			CPDF_FormField* pFormField = (CPDF_FormField*)FieldArray.ElementAt(i);
			ASSERT(pFormField != NULL);

			if (m_nFormControlIndex < 0)//if don't specify a control, we will set all controls' flag.
			{
				FX_BOOL bSet = FALSE;
				for (FX_INT32 j=0,jsz = pFormField->CountControls(); j<jsz; j++)
				{

					if (CPDF_FormControl* pFormControl = pFormField->GetControl(j))
					{
						FS_INT32 dwFlags = -1, dwOldFlag;
						dwFlags = pFormControl->GetWidget()->GetInteger(FX_BSTRC("F"));
						dwOldFlag = dwFlags;

						if (bVP)
							dwFlags |= ANNOTFLAG_PRINT;
						else
							dwFlags &= ~ANNOTFLAG_PRINT;

						if (dwFlags != dwOldFlag)
						{
							pFormControl->GetWidget()->SetAtInteger(FX_BSTRC("F"), dwFlags);
							bSet = TRUE;
						}
					}
				}

				if (bSet)
					UpdateFormField(m_pDocument, pFormField, TRUE, FALSE, TRUE);
			}
			else//specify a control
			{
				if (CPDF_FormControl* pFormControl = pFormField->GetControl(m_nFormControlIndex))
				{
					if (pFormControl->GetWidget())
					{
						FS_INT32 dwFlags = -1, dwOldFlag;
						dwFlags = pFormControl->GetWidget()->GetInteger(FX_BSTRC("F"));
						dwOldFlag = dwFlags;

						if (bVP)
							dwFlags |= ANNOTFLAG_PRINT;
						else
							dwFlags &= ~ANNOTFLAG_PRINT;

						if (dwFlags != dwOldFlag)
						{
							pFormControl->GetWidget()->SetAtInteger(FX_BSTRC("F"), dwFlags);
							UpdateFormControl(m_pDocument, pFormField->GetControl(m_nFormControlIndex), TRUE, FALSE, TRUE);
						}
					}
				}
			}
		}
	}
	else//getting
	{
		CPDF_FormField* pFormField = (CPDF_FormField*)FieldArray.ElementAt(0);
		ASSERT(pFormField != NULL);

		FS_INT32 nControlIndex = m_nFormControlIndex < 0?0:m_nFormControlIndex;
		CPDF_FormControl* pFormControl = pFormField->GetControl(nControlIndex);
		if (!pFormControl) return FALSE;

		FS_INT32 dwFlags = 0;
		dwFlags = pFormControl->GetWidget()->GetInteger(FX_BSTRC("F"));

		if ( dwFlags & ANNOTFLAG_PRINT)
			vp << true;
		else
			vp << false;
	}

	return TRUE;
}

FX_BOOL JField::radiosInUnison(OBJ_PROP_PARAMS)
{
	ASSERT(m_pDocument != NULL);

	CFX_PtrArray FieldArray;
	GetFormFields(m_FieldName,FieldArray);
	if (FieldArray.GetSize() <= 0) return FALSE;

	if (vp.IsSetting())
	{
		if (!m_bCanSet) return FALSE;

		bool bVP;
		vp >> bVP;

		for (FX_INT32 i=0,isz=FieldArray.GetSize(); i<isz; i++)
		{
			CPDF_FormField* pFormField = (CPDF_FormField*)FieldArray.ElementAt(i);
			ASSERT(pFormField != NULL);

			if (pFormField->GetFieldType() == FIELDTYPE_RADIOBUTTON)
			{
				FX_DWORD dwFlags = pFormField->GetFieldFlags();

				if (bVP)
					dwFlags |= FIELDFLAG_RADIOSINUNISON;
				else
					dwFlags &= ~FIELDFLAG_RADIOSINUNISON;
				
				if (dwFlags != pFormField->GetFieldFlags())
				{
					pFormField->SetFieldFlags(dwFlags);
					pFormField->UpdateUnisonStatus(TRUE);
					UpdateFormField(m_pDocument, pFormField, TRUE, FALSE, TRUE);
				}
			}
		}
	}
	else
	{
		CPDF_FormField* pFormField = (CPDF_FormField*)FieldArray.ElementAt(0);
		ASSERT(pFormField != NULL);

		if (pFormField->GetFieldType() != FIELDTYPE_RADIOBUTTON)
			return FALSE;

		if (pFormField->GetFieldFlags() & FIELDFLAG_RADIOSINUNISON)
			vp << true;
		else
			vp << false;
	}

	return TRUE;
}

FX_BOOL JField::readonly(OBJ_PROP_PARAMS)
{
	ASSERT(m_pDocument != NULL);

	CFX_PtrArray FieldArray;
	GetFormFields(m_FieldName,FieldArray);
	if (FieldArray.GetSize() <= 0) return FALSE;

	if (vp.IsSetting())
	{
		if (!m_bCanSet) return FALSE;

		bool bVP;
		vp >> bVP;

		for (FX_INT32 i=0,isz=FieldArray.GetSize(); i<isz; i++)
		{
			CPDF_FormField* pFormField = (CPDF_FormField*)FieldArray.ElementAt(i);
			ASSERT(pFormField != NULL);

			FX_DWORD dwFlags = pFormField->GetFieldFlags();

			if (bVP)
				dwFlags |= FORMFIELD_READONLY;
			else
				dwFlags &= ~FORMFIELD_READONLY;

			if (dwFlags != pFormField->GetFieldFlags())
			{
				pFormField->SetFieldFlags(dwFlags);
				UpdateFormField(m_pDocument, pFormField, TRUE, FALSE, TRUE);
			}
		}
	}
	else
	{
		CPDF_FormField* pFormField = (CPDF_FormField*)FieldArray.ElementAt(0);
		ASSERT(pFormField != NULL);

		if (pFormField->GetFieldFlags() & FORMFIELD_READONLY)
			vp << true;
		else
			vp << false;
	}

	return TRUE;
}

FX_BOOL JField::rect(OBJ_PROP_PARAMS)
{
	ASSERT(m_pDocument != NULL);

	if (vp.IsSetting())
	{
		if (!m_bCanSet) return FALSE;
		if (!vp.IsArrayObject())return FALSE;

		CFXJS_Array rcArray;
		vp >> rcArray;
		CFXJS_Value Upper_Leftx, Upper_Lefty, Lower_Rightx, Lower_Righty;
		rcArray.GetElement(0, Upper_Leftx);
		rcArray.GetElement(1, Upper_Lefty);
		rcArray.GetElement(2, Lower_Rightx);
		rcArray.GetElement(3, Lower_Righty);

		FX_FLOAT pArray[4] = {0.0f,0.0f,0.0f,0.0f};
		pArray[0] = (FX_FLOAT)(FX_INT32)Upper_Leftx;
		pArray[1] = (FX_FLOAT)(FX_INT32)Lower_Righty;
		pArray[2] = (FX_FLOAT)(FX_INT32)Lower_Rightx;
		pArray[3] = (FX_FLOAT)(FX_INT32)Upper_Lefty;

		CPDF_Rect crRect(pArray);

		if (m_bDelay)
		{
			AddDelay_Rect(FP_RECT, crRect);
		}
		else
		{
			JField::SetRect(m_pDocument, m_FieldName, m_nFormControlIndex, crRect);
		}	
	}
	else
	{
		CFX_PtrArray FieldArray;
		GetFormFields(m_FieldName,FieldArray);
		if (FieldArray.GetSize() <= 0) return FALSE;

		CPDF_FormField* pFormField = (CPDF_FormField*)FieldArray.ElementAt(0);
		ASSERT(pFormField != NULL);

		FSFormImp* pInterForm = (FSFormImp*)m_pDocument->GetForm();
		ASSERT(pInterForm != NULL);

		CFSCRT_STPDFInterForm* pSTInterForm = pInterForm->ST_GetSTInterForm();
		ASSERT(pSTInterForm != NULL);

		FSFormControlImp* pWidget = (FSFormControlImp*)pSTInterForm->GetLTFormControl(GetSmartFieldControl(pFormField));
		if (!pWidget) return FALSE;

		CPDF_Rect crRect;
		pWidget->ST_GetRect(crRect);
		CFXJS_Value Upper_Leftx,Upper_Lefty,Lower_Rightx,Lower_Righty;
		Upper_Leftx = (FX_INT32)crRect.left;
		Upper_Lefty = (FX_INT32)crRect.top;
		Lower_Rightx = (FX_INT32)crRect.right;
		Lower_Righty = (FX_INT32)crRect.bottom;

		CFXJS_Array rcArray;
		rcArray.SetElement(0,Upper_Leftx);
		rcArray.SetElement(1,Upper_Lefty);
		rcArray.SetElement(2,Lower_Rightx);
		rcArray.SetElement(3,Lower_Righty);

		vp  <<  rcArray;			
	}

	return TRUE;
}

void JField::SetRect(FSPDFDoc* pDocument, const CFX_WideString& swFieldName, FX_INT32 nControlIndex, const CPDF_Rect& rect)
{
	ASSERT(pDocument != NULL);

	FSFormImp* pInterForm = ((FSPDFDocImp*)pDocument)->GetForm();
	ASSERT(pInterForm != NULL);

	CFX_PtrArray FieldArray;
	GetFormFields(pDocument, swFieldName, FieldArray);

	for (FX_INT32 i=0,isz=FieldArray.GetSize(); i<isz; i++)
	{
		CPDF_FormField* pFormField = (CPDF_FormField*)FieldArray.ElementAt(i);
		ASSERT(pFormField != NULL);

		if (nControlIndex < 0)
		{
			FX_BOOL bSet = FALSE;
			for (FX_INT32 i=0, sz=pFormField->CountControls(); i<sz; i++)
			{
				CPDF_FormControl* pFormControl = pFormField->GetControl(i);
				ASSERT(pFormControl != NULL);

				if (FSFormControlImp* pWidget = NULL/*pInterForm->GetWidget(pFormControl)*/)
				{
					
					CPDF_Rect crRect = rect;

					CPDF_Page* pPDFPage = pWidget->GetPDFPage();
					ASSERT(pPDFPage != NULL);

					crRect.Intersect(pPDFPage->GetPageBBox());

					if (!crRect.IsEmpty())
					{
						CPDF_Rect rcOld;
						pWidget->ST_GetRect(rcOld);
						if (crRect.left != rcOld.left ||
							crRect.right != rcOld.right ||
							crRect.top != rcOld.top ||
							crRect.bottom != rcOld.bottom)
						{
							CPDF_Rect rcRectNew;
							rcRectNew.top = crRect.top;
							rcRectNew.bottom = crRect.bottom;
							rcRectNew.left = crRect.left;
							rcRectNew.right = crRect.right;
							pWidget->ST_SetRect(rcRectNew);
							bSet = TRUE;
						}
					}
				}
			}

			if (bSet) UpdateFormField(pDocument, pFormField, TRUE, TRUE, TRUE);
		}
		else
		{
			if (CPDF_FormControl* pFormControl = pFormField->GetControl(nControlIndex))
			{
				if (FSFormControlImp* pWidget = NULL/*pInterForm->GetWidget(pFormControl)*/)
				{
					CPDF_Rect crRect = rect;
					
					CPDF_Page* pPDFPage = pWidget->GetPDFPage();
					ASSERT(pPDFPage != NULL);

					crRect.Intersect(pPDFPage->GetPageBBox());

					if (!crRect.IsEmpty())
					{
						CPDF_Rect rcOld;
						pWidget->ST_GetRect(rcOld);
						if (crRect.left != rcOld.left ||
							crRect.right != rcOld.right ||
							crRect.top != rcOld.top ||
							crRect.bottom != rcOld.bottom)
						{
							CPDF_Rect rcRectNew;
							rcRectNew.top = crRect.top;
							rcRectNew.bottom = crRect.bottom;
							rcRectNew.left = crRect.left;
							rcRectNew.right = crRect.right;
							pWidget->ST_SetRect(rcRectNew);
							UpdateFormControl(pDocument, pFormControl, TRUE, TRUE, TRUE);
						}
					}
				}
			}
		}
	}
}

FX_BOOL JField::required(OBJ_PROP_PARAMS)
{
	ASSERT(m_pDocument != NULL);

	CFX_PtrArray FieldArray;
	GetFormFields(m_FieldName,FieldArray);
	if (FieldArray.GetSize() <= 0) return FALSE;

	if (vp.IsSetting())
	{
		if (!m_bCanSet) return FALSE;


		bool bVP;
		vp >> bVP;

		for (FX_INT32 i=0,isz=FieldArray.GetSize(); i<isz; i++)
		{
			CPDF_FormField* pFormField = (CPDF_FormField*)FieldArray.ElementAt(i);
			ASSERT(pFormField != NULL);

			FX_DWORD dwFlags = pFormField->GetFieldFlags();
			if (bVP)
				dwFlags |= FIELDFLAG_REQUIRED;
			else
				dwFlags &= ~FIELDFLAG_REQUIRED;

			if (dwFlags != pFormField->GetFieldFlags())
			{
				pFormField->SetFieldFlags(dwFlags);
				UpdateFormField(m_pDocument, pFormField, TRUE, FALSE, TRUE);
			}
		}
	}
	else
	{
		CPDF_FormField* pFormField = (CPDF_FormField*)FieldArray.ElementAt(0);
		ASSERT(pFormField != NULL);

		if (pFormField->GetFieldType() == FIELDTYPE_PUSHBUTTON)
			return FALSE;

		if (pFormField->GetFieldFlags() & FIELDFLAG_REQUIRED)
			vp << true;
		else
			vp << false;
	}

	return TRUE;
}

FX_BOOL JField::richText(OBJ_PROP_PARAMS)
{
	ASSERT(m_pDocument != NULL);

	if (vp.IsSetting())
	{
		if (!m_bCanSet) return FALSE;

		bool bVP;
		vp >> bVP;

		if (m_bDelay)
		{
			AddDelay_Bool(FP_RICHTEXT, bVP);
		}
		else
		{
			JField::SetRichText(m_pDocument, m_FieldName, m_nFormControlIndex, bVP);
		}	
	}
	else
	{
		CFX_PtrArray FieldArray;
		GetFormFields(m_FieldName,FieldArray);
		if (FieldArray.GetSize() <= 0) return FALSE;

		CPDF_FormField* pFormField = (CPDF_FormField*)FieldArray.ElementAt(0);
		ASSERT(pFormField != NULL);

		if (pFormField->GetFieldType() != FIELDTYPE_TEXTFIELD)
			return FALSE;

		if (pFormField->GetFieldFlags() & FIELDFLAG_RICHTEXT)
			vp << true;
		else
			vp << false;
	}

	return TRUE;
}

void JField::SetRichText(FSPDFDoc* pDocument, const CFX_WideString& swFieldName, FX_INT32 nControlIndex, bool b)
{
	ASSERT(pDocument != NULL);

	CFX_PtrArray FieldArray;
	GetFormFields(pDocument, swFieldName, FieldArray);

	for (FX_INT32 i=0,isz=FieldArray.GetSize(); i<isz; i++)
	{
		CPDF_FormField* pFormField = (CPDF_FormField*)FieldArray.ElementAt(i);
		ASSERT(pFormField != NULL);

		if (pFormField->GetFieldType() == FIELDTYPE_TEXTFIELD)
		{
			FX_DWORD dwFlags = pFormField->GetFieldFlags();

			if (b)
				dwFlags |= FIELDFLAG_RICHTEXT;
			else
				dwFlags &= ~FIELDFLAG_RICHTEXT;
			
			if (dwFlags != pFormField->GetFieldFlags())
			{
				pFormField->SetFieldFlags(dwFlags);
				UpdateFormField(pDocument, pFormField, TRUE, TRUE, TRUE);
			}
		}
	}
}

FX_BOOL JField::richValue(OBJ_PROP_PARAMS)
{
	return TRUE;
	if (vp.IsSetting())
	{
		if (!m_bCanSet) return FALSE;
		;
	}
	else
	{
		;
	}
	return TRUE;
}

void JField::SetRichValue(FSPDFDoc* pDocument, const CFX_WideString& swFieldName, FX_INT32 nControlIndex)
{
}

FX_BOOL JField::rotation(OBJ_PROP_PARAMS)
{
	ASSERT(m_pDocument != NULL);

	if (vp.IsSetting())
	{
		if (!m_bCanSet) return FALSE;

		FX_INT32 nVP;
		vp >> nVP;

		if (m_bDelay)
		{
			AddDelay_Int(FP_ROTATION, nVP);
		}
		else
		{
			JField::SetRotation(m_pDocument, m_FieldName, m_nFormControlIndex, nVP);
		}	
	}
	else
	{
		CFX_PtrArray FieldArray;
		GetFormFields(m_FieldName,FieldArray);
		if (FieldArray.GetSize() <= 0) return FALSE;

		CPDF_FormField* pFormField = (CPDF_FormField*)FieldArray.ElementAt(0);
		ASSERT(pFormField != NULL);

		CPDF_FormControl* pFormControl = GetSmartFieldControl(pFormField);
		if (!pFormControl)return FALSE;

		vp << (FX_INT32)pFormControl->GetRotation();
	}

	return TRUE;
}

void JField::SetRotation(FSPDFDoc* pDocument, const CFX_WideString& swFieldName, FX_INT32 nControlIndex, FX_INT32 number)
{
	ASSERT(pDocument != NULL);

	CFX_PtrArray FieldArray;
	GetFormFields(pDocument, swFieldName, FieldArray);

	for (FX_INT32 i=0,isz=FieldArray.GetSize(); i<isz; i++)
	{
		CPDF_FormField* pFormField = (CPDF_FormField*)FieldArray.ElementAt(i);
		ASSERT(pFormField != NULL);

		if (pFormField->GetFieldType() == FIELDTYPE_TEXTFIELD)
		{
			if (nControlIndex < 0)
			{
				FX_BOOL bSet = FALSE;
				for (FX_INT32 i=0, sz=pFormField->CountControls(); i<sz; i++)
				{
					CPDF_FormControl* pFormControl = pFormField->GetControl(i);
					ASSERT(pFormControl != NULL);

					if (number != pFormControl->GetRotation())
					{
						pFormControl->SetRotation(number);
						bSet = TRUE;
					}
				}
				if (bSet) UpdateFormField(pDocument, pFormField, TRUE, TRUE, TRUE);
			}
			else
			{
				if (CPDF_FormControl* pFormControl = pFormField->GetControl(nControlIndex))
				{
					if (number != pFormControl->GetRotation())
					{
						pFormControl->SetRotation(number);
						UpdateFormControl(pDocument, pFormControl, TRUE, TRUE, TRUE);
					}
				}
			}
		}
	}
}

FX_BOOL JField::strokeColor(OBJ_PROP_PARAMS)
{
	ASSERT(m_pDocument != NULL);

	if (vp.IsSetting())
	{
		if (!m_bCanSet) return FALSE;

		if (!vp.IsArrayObject())return FALSE;

		CFXJS_Array crArray;
		vp >> crArray;

		CPWL_Color color;
		jcolor::ConvertArrayToPWLColor(crArray, color);

		if (m_bDelay)
		{
			AddDelay_Color(FP_STROKECOLOR, color);
		}
		else
		{
			JField::SetStrokeColor(m_pDocument, m_FieldName, m_nFormControlIndex, color);
		}	
	}
	else
	{
		CFX_PtrArray FieldArray;
		GetFormFields(m_FieldName,FieldArray);
		if (FieldArray.GetSize() <= 0) return FALSE;

		CPDF_FormField* pFormField = (CPDF_FormField*)FieldArray.ElementAt(0);
		ASSERT(pFormField != NULL);

		CPDF_FormControl* pFormControl = GetSmartFieldControl(pFormField);
		if (!pFormControl)return FALSE;

		FX_INT32 iColorType;
		pFormControl->GetBorderColor(iColorType);

		CPWL_Color color;

		if (iColorType == COLORTYPE_TRANSPARENT)
		{
			color = CPWL_Color(COLORTYPE_TRANSPARENT);
		}
		else if (iColorType == COLORTYPE_GRAY)
		{
			color = CPWL_Color(COLORTYPE_GRAY, pFormControl->GetOriginalBorderColor(0));
		}
		else if (iColorType == COLORTYPE_RGB)
		{
			color = CPWL_Color(COLORTYPE_RGB, pFormControl->GetOriginalBorderColor(0),
				pFormControl->GetOriginalBorderColor(1),
				pFormControl->GetOriginalBorderColor(2));
		}
		else if (iColorType == COLORTYPE_CMYK)
		{
			color = CPWL_Color(COLORTYPE_CMYK, pFormControl->GetOriginalBorderColor(0),
				pFormControl->GetOriginalBorderColor(1),
				pFormControl->GetOriginalBorderColor(2),
				pFormControl->GetOriginalBorderColor(3));
		}
		else
			return FALSE;

		CFXJS_Array crArray;
		jcolor::ConvertPWLColorToArray(color, crArray);
        vp  <<  crArray;
	}

	return TRUE;
}

void JField::SetStrokeColor(FSPDFDoc* pDocument, const CFX_WideString& swFieldName, FX_INT32 nControlIndex, const CPWL_Color& color)
{
	ASSERT(pDocument != NULL);

	CFX_PtrArray FieldArray;
	GetFormFields(pDocument, swFieldName, FieldArray);

	FX_ARGB nNewColor = CPWL_Utils::PWLColorToFXColor(color);
	FX_INT32 nNewColorType = color.nColorType;

	for (FX_INT32 i=0,isz=FieldArray.GetSize(); i<isz; i++)
	{
		CPDF_FormField* pFormField = (CPDF_FormField*)FieldArray.ElementAt(i);
		ASSERT(pFormField != NULL);

		if (nControlIndex < 0)
		{
			FX_BOOL bSet = FALSE;
			for (FX_INT32 j=0,jsz = pFormField->CountControls(); j<jsz; j++)
			{
				CPDF_FormControl* pFormControl = pFormField->GetControl(j);
				ASSERT(pFormControl != NULL);

				FX_INT32 nOldColorType = 0;
				FX_ARGB nOldColor = pFormControl->GetBorderColor(nOldColorType);

				if (nOldColor != nNewColor || nOldColorType != nNewColorType)
				{
					pFormControl->SetBorderColor(nNewColorType, nNewColor);
					bSet = TRUE;
				}
			}
			if (bSet) UpdateFormField(pDocument, pFormField, TRUE, TRUE, TRUE);
		}
		else
		{
			if (CPDF_FormControl* pFormControl = pFormField->GetControl(nControlIndex))
			{
				FX_INT32 nOldColorType = 0;
				FX_ARGB nOldColor = pFormControl->GetBorderColor(nOldColorType);

				if (nOldColor != nNewColor || nOldColorType != nNewColorType)
				{
					pFormControl->SetBorderColor(nNewColorType, nNewColor);
					UpdateFormControl(pDocument, pFormControl, TRUE, TRUE, TRUE);
				}
			}
		}
	}
}

FX_BOOL JField::style(OBJ_PROP_PARAMS)
{
	ASSERT(m_pDocument != NULL);

	if (vp.IsSetting())
	{
		if (!m_bCanSet) return FALSE;

		CFX_ByteString csBCaption;
		vp >> csBCaption;

		if (m_bDelay)
		{
			AddDelay_String(FP_STYLE, csBCaption);
		}
		else
		{
			JField::SetStyle(m_pDocument, m_FieldName, m_nFormControlIndex, csBCaption);
		}	
	}
	else
	{
		CFX_PtrArray FieldArray;
		GetFormFields(m_FieldName,FieldArray);
		if (FieldArray.GetSize() <= 0) return FALSE;

		CPDF_FormField* pFormField = (CPDF_FormField*)FieldArray.ElementAt(0);
		ASSERT(pFormField != NULL);

		if (pFormField->GetFieldType() != FIELDTYPE_RADIOBUTTON && 
			pFormField->GetFieldType() != FIELDTYPE_CHECKBOX)
			return FALSE;

		CPDF_FormControl* pFormControl = GetSmartFieldControl(pFormField);
		if (!pFormControl) return FALSE;

		CFX_WideString csWCaption = pFormControl->GetNormalCaption();
		CFX_ByteString csBCaption;

		switch (csWCaption[0])
		{
			case L'l':
				csBCaption = "circle";
				break;
			case L'8':
				csBCaption = "cross";
				break;
			case L'u':
				csBCaption = "diamond";
				break;
			case L'n':
				csBCaption = "square";
				break;
			case L'H':
				csBCaption = "star";
				break;
			default: //L'4'
				csBCaption = "check";
				break;
		}
		vp << csBCaption;
	}

	return TRUE;
}

void JField::SetStyle(FSPDFDoc* pDocument, const CFX_WideString& swFieldName, FX_INT32 nControlIndex, 
					 const CFX_ByteString& string)
{
	ASSERT(pDocument != NULL);

	CFX_WideString csWCaption;

	if (string == "circle")
		csWCaption = L"l";
	else if (string == "cross")
		csWCaption = L"8";
	else if (string == "diamond")
		csWCaption = L"u";
	else if (string == "square")
		csWCaption = L"n";
	else if (string == "star")
		csWCaption = L"H";
	else
		csWCaption = L"4";

	CFX_PtrArray FieldArray;
	GetFormFields(pDocument, swFieldName, FieldArray);

	for (FX_INT32 i=0,isz=FieldArray.GetSize(); i<isz; i++)
	{
		CPDF_FormField* pFormField = (CPDF_FormField*)FieldArray.ElementAt(i);
		ASSERT(pFormField != NULL);

		if (nControlIndex < 0)
		{
			FX_BOOL bSet = FALSE;
			for (FX_INT32 i=0, sz=pFormField->CountControls(); i<sz; i++)
			{
				CPDF_FormControl* pFormControl = pFormField->GetControl(i);
				ASSERT(pFormControl != NULL);

				if (csWCaption != pFormControl->GetNormalCaption())
				{
					pFormControl->SetNormalCaption(csWCaption);
					bSet = TRUE;
				}
			}
			if (bSet) UpdateFormField(pDocument, pFormField, TRUE, TRUE, TRUE);
		}
		else
		{
			if (CPDF_FormControl* pFormControl = pFormField->GetControl(nControlIndex))
			{
				if (csWCaption != pFormControl->GetNormalCaption())
				{
					pFormControl->SetNormalCaption(csWCaption);
					UpdateFormControl(pDocument, pFormControl, TRUE, TRUE, TRUE);
				}
			}
		}
	}
}

FX_BOOL JField::submitName(OBJ_PROP_PARAMS)
{
	return TRUE;
}

FX_BOOL JField::textColor(OBJ_PROP_PARAMS)
{
	ASSERT(m_pDocument != NULL);

	if (vp.IsSetting())
	{
		if (!m_bCanSet) return FALSE;

		CFXJS_Array crArray;
		if (!vp.IsArrayObject())return FALSE;
		vp >> crArray;

		CPWL_Color color;
		jcolor::ConvertArrayToPWLColor(crArray, color);

		if (m_bDelay)
		{
			AddDelay_Color(FP_TEXTCOLOR, color);
		}
		else
		{
			JField::SetTextColor(m_pDocument, m_FieldName, m_nFormControlIndex, color);
		}	
	}
	else
	{
		CFX_PtrArray FieldArray;
		GetFormFields(m_FieldName,FieldArray);
		if (FieldArray.GetSize() <= 0) return FALSE;

		CPDF_FormField* pFormField = (CPDF_FormField*)FieldArray.ElementAt(0);
		ASSERT(pFormField != NULL);

		CPDF_FormControl* pFormControl = GetSmartFieldControl(pFormField);
		if (!pFormControl)return FALSE;
		
		FX_INT32 iColorType;
		FX_ARGB color;
		CPDF_DefaultAppearance FieldAppearance = pFormControl->GetDefaultAppearance();
		FieldAppearance.GetColor(color, iColorType);
		FX_INT32 a,r,g,b;
		ArgbDecode(color, a, r, g, b);

		CPWL_Color crRet = CPWL_Color(COLORTYPE_RGB, r / 255.0f,
				g / 255.0f,
				b / 255.0f);

		if (iColorType == COLORTYPE_TRANSPARENT)
			crRet = CPWL_Color(COLORTYPE_TRANSPARENT);

		CFXJS_Array crArray;
		jcolor::ConvertPWLColorToArray(crRet, crArray);
        vp  <<  crArray;		
	}

	return TRUE;
}

void JField::SetTextColor(FSPDFDoc* pDocument, const CFX_WideString& swFieldName, FX_INT32 nControlIndex, const CPWL_Color& color)
{
	ASSERT(pDocument != NULL);

	CFX_PtrArray FieldArray;
	GetFormFields(pDocument, swFieldName, FieldArray);

	for (FX_INT32 i=0,isz=FieldArray.GetSize(); i<isz; i++)
	{
		CPDF_FormField* pFormField = (CPDF_FormField*)FieldArray.ElementAt(i);
		ASSERT(pFormField != NULL);

		FX_ARGB newcolor = CPWL_Utils::PWLColorToFXColor(color);
		FX_INT32 newcolortype = color.nColorType;

		if (nControlIndex < 0)
		{
			FX_BOOL bSet = FALSE;

			for (FX_INT32 j=0,jsz = pFormField->CountControls(); j<jsz; j++)
			{
				CPDF_FormControl* pFormControl = pFormField->GetControl(j);
				ASSERT(pFormControl != NULL);

				CPDF_DefaultAppearance FieldAppearance = pFormControl->GetDefaultAppearance();

				FX_INT32 oldcolortype;
				FX_ARGB oldcolor;
				FieldAppearance.GetColor(oldcolor, oldcolortype);
				if (oldcolor != newcolor || oldcolortype != newcolortype)
				{
					FieldAppearance.SetColor(newcolor, newcolortype);
					pFormControl->SetDefaultAppearance(FieldAppearance);
					bSet = TRUE;
				}
			}

			if (bSet) UpdateFormField(pDocument, pFormField, TRUE, TRUE, TRUE);
		}
		else
		{
			if (CPDF_FormControl* pFormControl = pFormField->GetControl(nControlIndex))
			{
				CPDF_DefaultAppearance FieldAppearance = pFormControl->GetDefaultAppearance();
				FX_INT32 oldcolortype;
				FX_ARGB oldcolor;
				FieldAppearance.GetColor(oldcolor, oldcolortype);

				if (oldcolor != newcolor || oldcolortype != newcolortype)
				{
					FieldAppearance.SetColor(CPWL_Utils::PWLColorToFXColor(color), color.nColorType);
					pFormControl->SetDefaultAppearance(FieldAppearance);

					UpdateFormControl(pDocument, pFormControl, TRUE, TRUE, TRUE);
				}
			}
		}
	}			
}

FX_BOOL JField::textFont(OBJ_PROP_PARAMS)
{
	ASSERT(m_pDocument != NULL);

	if (vp.IsSetting())
	{
		if (!m_bCanSet) return FALSE;

		CFX_ByteString csFontName;
		vp >> csFontName;
		if (csFontName.IsEmpty()) return FALSE;

		if (m_bDelay)
		{
			AddDelay_String(FP_TEXTFONT, csFontName);
		}
		else
		{
			JField::SetTextFont(m_pDocument, m_FieldName, m_nFormControlIndex, csFontName);
		}	
	}
	else
	{
		CFX_PtrArray FieldArray;
		GetFormFields(m_FieldName,FieldArray);
		if (FieldArray.GetSize() <= 0) return FALSE;

		CPDF_FormField* pFormField = (CPDF_FormField*)FieldArray.ElementAt(0);
		ASSERT(pFormField != NULL);

		CPDF_FormControl* pFormControl = GetSmartFieldControl(pFormField);
		if (!pFormControl)return FALSE;

		FX_INT32 nFieldType = pFormField->GetFieldType();

		if (nFieldType == FIELDTYPE_PUSHBUTTON || 
			nFieldType == FIELDTYPE_COMBOBOX || 
			nFieldType == FIELDTYPE_LISTBOX ||
			nFieldType == FIELDTYPE_TEXTFIELD)
		{
			CPDF_Font * pFont = pFormControl->GetDefaultControlFont();
			if (!pFont) return FALSE;

			vp << pFont->GetBaseFont();
		}
		else
			return FALSE;
	}

	return TRUE;
}

void JField::SetTextFont(FSPDFDoc* pDocument, const CFX_WideString& swFieldName, FX_INT32 nControlIndex, const CFX_ByteString& string)
{
	ASSERT(pDocument != NULL);

	CFX_PtrArray FieldArray;
	GetFormFields(pDocument, swFieldName, FieldArray);

	CFX_ByteString csFontName = string;
	CFX_ByteString csFontNameTag;

	for (FX_INT32 i=0,isz=FieldArray.GetSize(); i<isz; i++)
	{
		CPDF_FormField* pFormField = (CPDF_FormField*)FieldArray.ElementAt(i);
		ASSERT(pFormField != NULL);

		FX_INT32 nFieldType = pFormField->GetFieldType();

		if (nFieldType == FIELDTYPE_PUSHBUTTON || 
			nFieldType == FIELDTYPE_COMBOBOX || 
			nFieldType == FIELDTYPE_LISTBOX ||
			nFieldType == FIELDTYPE_TEXTFIELD)
		{
			CPDF_InterForm* pInterForm = pFormField->GetInterForm();
			ASSERT(pInterForm != NULL);

			if (nControlIndex < 0)
			{
				FX_BOOL bSet = FALSE;
				for (FX_INT32 j=0,jsz = pFormField->CountControls(); j<jsz; j++)
				{
					CPDF_FormControl* pFormControl = pFormField->GetControl(j);
					ASSERT(pFormControl != NULL);

					CFX_ByteString sOldFont;
					if (CPDF_Font* pOldFont = pFormControl->GetDefaultControlFont())
						sOldFont = pOldFont->GetBaseFont();

					if (sOldFont != csFontName)
					{
						CPDF_Font* pFont = NULL; //pFormControl->GetDefaultControlFont();

						if ( pInterForm->FindFormFont(csFontName, pFont, csFontNameTag) )
							pFormControl->SetDefaultControlFont(pFont);

						else
						{
							pFont = pInterForm->AddStandardFont(((FSPDFDocImp*)pDocument)->getPDFDocument(), csFontName);
							if (pFont == NULL)
							{
								pFont = pInterForm->AddStandardFont(((FSPDFDocImp*)pDocument)->getPDFDocument(), FX_BSTRC("Helvetica"));
								csFontName = FX_BSTRC("Helvetica");
							}

							if (pFont != NULL)
							{
								pInterForm->AddFormFont(pFont, csFontName);
								pFormControl->SetDefaultControlFont(pFont);
							}
						}

						bSet = TRUE;
					}
				}
				if (bSet) UpdateFormField(pDocument, pFormField, TRUE, TRUE, TRUE);
			}
			else
			{
				if (CPDF_FormControl* pFormControl = pFormField->GetControl(nControlIndex))
				{
					CFX_ByteString sOldFont;
					if (CPDF_Font* pOldFont = pFormControl->GetDefaultControlFont())
						sOldFont = pOldFont->GetBaseFont();

					if (sOldFont != csFontName)
					{
						CPDF_Font* pFont = NULL; //pFormControl->GetDefaultControlFont();

						if ( pInterForm->FindFormFont(csFontName, pFont, csFontNameTag) )
							pFormControl->SetDefaultControlFont(pFont);
						else
						{
							pFont = pInterForm->AddStandardFont(((FSPDFDocImp*)pDocument)->getPDFDocument(), csFontName);
							if (pFont == NULL)
							{
								pFont = pInterForm->AddStandardFont(((FSPDFDocImp*)pDocument)->getPDFDocument(), FX_BSTRC("Helvetica"));
								csFontName = FX_BSTRC("Helvetica");
							}
							if (pFont != NULL)
							{
								pInterForm->AddFormFont(pFont, csFontName);
								pFormControl->SetDefaultControlFont(pFont);
							}
						}
						UpdateFormControl(pDocument, pFormControl, TRUE, TRUE, TRUE);
					}
				}
			}
		}
	}
}

FX_BOOL JField::textSize(OBJ_PROP_PARAMS)
{
	ASSERT(m_pDocument != NULL);

	if (vp.IsSetting())
	{
		if (!m_bCanSet) return FALSE;

		FX_INT32 nVP;
		vp >> nVP;

		if (m_bDelay)
		{
			AddDelay_Int(FP_TEXTSIZE, nVP);
		}
		else
		{
			JField::SetTextSize(m_pDocument, m_FieldName, m_nFormControlIndex, nVP);
		}	
	}
	else
	{
		CFX_PtrArray FieldArray;
		GetFormFields(m_FieldName,FieldArray);
		if (FieldArray.GetSize() <= 0) return FALSE;

		CPDF_FormField* pFormField = (CPDF_FormField*)FieldArray.ElementAt(0);
		ASSERT(pFormField != NULL);

		CPDF_FormControl* pFormControl = GetSmartFieldControl(pFormField);
		if (!pFormControl)return FALSE;

		CPDF_DefaultAppearance FieldAppearance = pFormControl->GetDefaultAppearance();

		CFX_ByteString csFontNameTag;
		FX_FLOAT fFontSize;
		FieldAppearance.GetFont(csFontNameTag,fFontSize);

		vp << (FX_INT32)fFontSize;
	}

	return TRUE;
}

void JField::SetTextSize(FSPDFDoc* pDocument, const CFX_WideString& swFieldName, FX_INT32 nControlIndex, FX_INT32 number)
{
	ASSERT(pDocument != NULL);

	CFX_PtrArray FieldArray;
	GetFormFields(pDocument, swFieldName, FieldArray);

	CFX_ByteString csFontNameTag;
	FX_FLOAT fFontSize;

	for (FX_INT32 i=0,isz=FieldArray.GetSize(); i<isz; i++)
	{
		CPDF_FormField* pFormField = (CPDF_FormField*)FieldArray.ElementAt(i);
		ASSERT(pFormField != NULL);

		if (nControlIndex < 0)
		{
			FX_BOOL bSet = FALSE;
			for (FX_INT32 j=0,jsz = pFormField->CountControls(); j<jsz; j++)
			{
				CPDF_FormControl* pFormControl = pFormField->GetControl(j);
				ASSERT(pFormControl != NULL);

				CPDF_DefaultAppearance FieldAppearance = pFormControl->GetDefaultAppearance();
				FieldAppearance.GetFont(csFontNameTag, fFontSize);
				if (fFontSize != number)
				{
					FieldAppearance.SetFont(csFontNameTag, (FX_FLOAT)number);
					pFormControl->SetDefaultAppearance(FieldAppearance);
					bSet = TRUE;
				}
			}

			if (bSet) UpdateFormField(pDocument, pFormField, TRUE, TRUE, TRUE);
		}
		else
		{
			if (CPDF_FormControl* pFormControl = pFormField->GetControl(nControlIndex))
			{
				CPDF_DefaultAppearance FieldAppearance = pFormControl->GetDefaultAppearance();
				FieldAppearance.GetFont(csFontNameTag, fFontSize);
				if (fFontSize != number)
				{
					FieldAppearance.SetFont(csFontNameTag, (FX_FLOAT)number);
					pFormControl->SetDefaultAppearance(FieldAppearance);
					UpdateFormControl(pDocument, pFormControl, TRUE, TRUE, TRUE);
				}			
			}
		}
	}
}

FX_BOOL JField::type(OBJ_PROP_PARAMS)
{
	ASSERT(m_pDocument != NULL);

	if (!vp.IsGetting()) return FALSE;

	CFX_PtrArray FieldArray;
	GetFormFields(m_FieldName,FieldArray);
	if (FieldArray.GetSize() <= 0) return FALSE;

 	CPDF_FormField* pFormField = (CPDF_FormField*)FieldArray.ElementAt(0);
	ASSERT(pFormField != NULL);

	switch (pFormField->GetFieldType())
	{
		case FIELDTYPE_UNKNOWN:
			vp << (FX_LPCWSTR)L"unknown";
			break;
		case FIELDTYPE_PUSHBUTTON:
			vp << (FX_LPCWSTR)L"button";
			break;
		case FIELDTYPE_CHECKBOX:
			vp << (FX_LPCWSTR)L"checkbox";
			break;
		case FIELDTYPE_RADIOBUTTON:
			vp << (FX_LPCWSTR)L"radiobutton";
			break;
		case FIELDTYPE_COMBOBOX:
			vp << (FX_LPCWSTR)L"combobox";
			break;
		case FIELDTYPE_LISTBOX:
			vp << (FX_LPCWSTR)L"listbox";
			break;
		case FIELDTYPE_TEXTFIELD:
			vp << (FX_LPCWSTR)L"text";
			break;
		case FIELDTYPE_SIGNATURE:
			vp << (FX_LPCWSTR)L"signature";
			break;
		default :
			vp << (FX_LPCWSTR)L"unknown";
			break;
	}

	return TRUE;
}

FX_BOOL JField::userName(OBJ_PROP_PARAMS)
{
	ASSERT(m_pDocument != NULL);

  	if (vp.IsSetting())
	{
		if (!m_bCanSet) return FALSE;

		CFX_WideString swName;
		vp >> swName;

		if (m_bDelay)
		{
			AddDelay_WideString(FP_USERNAME, swName);
		}
		else
		{
			JField::SetUserName(m_pDocument, m_FieldName, m_nFormControlIndex, swName);
		}	
	}
	else
	{
		CFX_PtrArray FieldArray;
		GetFormFields(m_FieldName,FieldArray);
		if (FieldArray.GetSize() <= 0) return FALSE;

 		CPDF_FormField* pFormField = (CPDF_FormField*)FieldArray.ElementAt(0);
		ASSERT(pFormField != NULL);

		vp << (CFX_WideString)pFormField->GetAlternateName();
	}

	return TRUE;
}

void JField::SetUserName(FSPDFDoc* pDocument, const CFX_WideString& swFieldName, FX_INT32 nControlIndex, const CFX_WideString& string)
{
	ASSERT(pDocument != NULL);

	CFX_PtrArray FieldArray;
	GetFormFields(pDocument, swFieldName, FieldArray);

	for (FX_INT32 i=0,isz=FieldArray.GetSize(); i<isz; i++)
	{
		CPDF_FormField* pFormField = (CPDF_FormField*)FieldArray.ElementAt(i);
		ASSERT(pFormField != NULL);

		if (string != pFormField->GetAlternateName())
		{
			pFormField->SetAlternateName(string);
			UpdateFormField(pDocument, pFormField, TRUE, TRUE, TRUE);
		}
	}
}

FX_BOOL JField::value(OBJ_PROP_PARAMS)
{
	ASSERT(m_pDocument != NULL);

	if (vp.IsSetting())
	{		
		if (!m_bCanSet) return FALSE;

		CJS_WideStringArray strArray;

		if (vp.IsArrayObject())
		{
			CFXJS_Array ValueArray;
			vp.ConvertToArray(ValueArray);
			for (FX_INT32 i = 0,sz = ValueArray.GetLength(); i < sz; i++)
			{
				CFXJS_Value ElementValue;
				ValueArray.GetElement(i, ElementValue);
				strArray.Add((FX_LPCWSTR)ElementValue);
			}
		}
		else
		{
			CFX_WideString swValue;
			vp >> swValue;

			strArray.Add(swValue);
		}

		if (m_bDelay)
		{
			AddDelay_WideStringArray(FP_VALUE, strArray);
		}
		else
		{
			JField::SetValue(m_pDocument, m_FieldName, m_nFormControlIndex, strArray);
		}
	}
	else
	{
		CFX_PtrArray FieldArray;
		GetFormFields(m_FieldName,FieldArray);
		if (FieldArray.GetSize() <= 0) return FALSE;

 		CPDF_FormField* pFormField = (CPDF_FormField*)FieldArray.ElementAt(0);
		ASSERT(pFormField != NULL);

		FX_INT32 nFieldType = pFormField->GetFieldType();

		switch (pFormField->GetFieldType())
		{
		case FIELDTYPE_PUSHBUTTON:
			return FALSE;
		case FIELDTYPE_COMBOBOX:
		case FIELDTYPE_TEXTFIELD:
			{
				CFX_WideString swValue = pFormField->GetValue();

				double dRet;
				FX_BOOL bDot;
				if (CFXJS_PublicMethods::ConvertStringToNumber(swValue,dRet,bDot))
				{
					if (bDot)
						vp << dRet;
					else
						vp << dRet;
				}
				else
					vp << swValue;
			}
			break;
		case FIELDTYPE_LISTBOX:
			{
				if (pFormField->CountSelectedItems() > 1)
				{
					CFXJS_Array ValueArray;
					CFXJS_Value ElementValue;
					FX_INT32 iIndex;
					for (FX_INT32 i = 0, sz = pFormField->CountSelectedItems(); i < sz; i++)
					{
						iIndex = pFormField->GetSelectedIndex(i);
						ElementValue = pFormField->GetOptionValue(iIndex);
						if (FXSYS_wcslen((wchar_t*)(FX_WCHAR*)ElementValue) == 0)
							ElementValue = pFormField->GetOptionLabel(iIndex);
						ValueArray.SetElement(i, ElementValue);
					}
					vp << ValueArray;
				}
				else
				{
					CFX_WideString swValue = pFormField->GetValue();
				
					double dRet;
					FX_BOOL bDot;
					if (CFXJS_PublicMethods::ConvertStringToNumber(swValue,dRet,bDot))
					{
						if (bDot)
							vp << dRet;
						else
							vp << dRet;
					}
					else
						vp << swValue;	
				}
			}
			break;
		case FIELDTYPE_CHECKBOX:
		case FIELDTYPE_RADIOBUTTON:
			{
				FX_BOOL bFind = FALSE;
				for (FX_INT32 i = 0 , sz = pFormField->CountControls(); i < sz; i++)
				{
					if (pFormField->GetControl(i)->IsChecked())
					{
						CFX_WideString swValue = pFormField->GetControl(i)->GetExportValue();
						
						double dRet;
						FX_BOOL bDot;
						if (CFXJS_PublicMethods::ConvertStringToNumber(swValue,dRet,bDot))
						{
							if (bDot)
								vp << dRet;
							else
								vp << dRet;
						}
						else
							vp << swValue;

						bFind = TRUE;
						break;
					}
					else
						continue;
				}
				if (!bFind)
					vp << (FX_LPCWSTR)L"Off";					
			}
			break;
		default:
			vp << pFormField->GetValue();
			break;
		}
	}

	return TRUE;
}

void JField::SetValue(FSPDFDoc* pDocument, const CFX_WideString& swFieldName, 
					 FX_INT32 nControlIndex, const CJS_WideStringArray& strArray)
{
	ASSERT(pDocument != NULL);

	if (strArray.GetSize() < 1) return;

	CFX_PtrArray FieldArray;
	GetFormFields(pDocument, swFieldName, FieldArray);

	//SDK would update the AP automatically,so disable the updating from FXCORE.
	//FSPDF_UPDATEAP_START(TRUE)
	for (FX_INT32 i=0,isz=FieldArray.GetSize(); i<isz; i++)
	{
		CPDF_FormField* pFormField = (CPDF_FormField*)FieldArray.ElementAt(i);
		ASSERT(pFormField != NULL);

		if (pFormField->GetFullName().Compare(swFieldName) != 0)
			continue;

		switch (pFormField->GetFieldType())
		{
		case FIELDTYPE_TEXTFIELD:
		case FIELDTYPE_COMBOBOX:
			if (pFormField->GetValue() != strArray.GetAt(0))
			{
				CFX_WideString WideString = strArray.GetAt(0);
				
				pFormField->SetValue(strArray.GetAt(0), TRUE);	
				UpdateFormField(pDocument, pFormField, TRUE, FALSE, TRUE);
			}
			break;
		case FIELDTYPE_CHECKBOX: //mantis: 0004493
		case FIELDTYPE_RADIOBUTTON:
			{
				if (pFormField->GetValue() != strArray.GetAt(0))
				{
					pFormField->SetValue(strArray.GetAt(0), TRUE);	
					UpdateFormField(pDocument, pFormField, TRUE, FALSE, TRUE);
				}
			}
			break;
		case FIELDTYPE_LISTBOX:
			{
				FX_BOOL bModified = FALSE;

				for (FX_INT32 i=0,sz=strArray.GetSize(); i<sz; i++)
				{
					FX_INT32 iIndex = pFormField->FindOption(strArray.GetAt(i));

					if (!pFormField->IsItemSelected(iIndex))
					{
						bModified = TRUE;
						break;
					}
				}

				if (bModified)
				{
					pFormField->ClearSelection(TRUE);
					for (FX_INT32 i=0,sz=strArray.GetSize(); i<sz; i++)
					{
						FX_INT32 iIndex = pFormField->FindOption(strArray.GetAt(i));
						pFormField->SetItemSelection(iIndex, TRUE, TRUE);
					}

					UpdateFormField(pDocument, pFormField, TRUE, FALSE, TRUE);
				}
			}
			break;
		default:				
			break;
		}
	}
//	FSPDF_UPDATEAP_END(TRUE)
}

FX_BOOL JField::valueAsString(OBJ_PROP_PARAMS)
{
	ASSERT(m_pDocument != NULL);

	if (!vp.IsGetting()) return FALSE;

	CFX_PtrArray FieldArray;
	GetFormFields(m_FieldName,FieldArray);
	if (FieldArray.GetSize() <= 0) return FALSE;

   	CPDF_FormField* pFormField = (CPDF_FormField*)FieldArray.ElementAt(0);
	ASSERT(pFormField != NULL);

	if (pFormField->GetFieldType() == FIELDTYPE_PUSHBUTTON)
		return FALSE;

	if (pFormField->GetFieldType() == FIELDTYPE_CHECKBOX)
	{
		if (pFormField->GetControl(0)->IsChecked())
			vp << (FX_LPCWSTR)L"Yes";
		else
			vp << (FX_LPCWSTR)L"Off";
	}
// 	else if (pFormField->GetFieldType() == FIELDTYPE_RADIOBUTTON && !(pFormField->GetFieldFlags() & FIELDFLAG_RADIOSINUNISON))
// 	{
// 		for (FX_INT32 i=0, sz=pFormField->CountControls(); i<sz; i++)
// 		{
// 			if (pFormField->GetControl(i)->IsChecked())
// 			{
// 				vp << (FX_LPCWSTR)pFormField->GetControl(i)->GetExportValue();
// 				break;
// 			}
// 			else
// 				vp << (FX_LPCWSTR)L"Off";
// 		}
// 	}
	else if (pFormField->GetFieldType() == FIELDTYPE_LISTBOX && (pFormField->CountSelectedItems() > 1))
	{
		vp << (FX_LPCWSTR)L"";
	}
	else
		vp << (FX_LPCWSTR)pFormField->GetValue();

	return TRUE;
}
// Helper Functions
FX_BOOL JField::JS_fieldBrowse(CFX_WideString& wsFilePath)
{
	FSFormFillerImp* pFormFiller = m_pDocument->GetForm()->GetFormFiller();
    if(!pFormFiller) return FALSE;
// 	CFSCRT_LTAppHandler* pApp = FSCRT_GetLTAppHandler();
// 	if (!pApp) return TRUE;
#if 0
	FSCRT_BSTR sPath;
	FSCRT_BStr_Init(&sPath);
// 	FSCRT_VAR inParams[3] = {{FSCRT_VT_BOOL, 0, 0}, {FSCRT_VT_LPVOID, 0, 0},{FSCRT_VT_LPVOID, 0, 0}};
// 	FSCRT_Var_SetBooleanValue(inParams[0], FALSE)
// 	FSCRT_Var_SetVoidPtrValue(inParams[1], &sFilenameInit)
// 	FSCRT_Var_SetVoidPtrValue(inParams[2], &sPath)
// 	FSCRT_VAR retVal = {FSCRT_VT_INT32, 0, 0};
// 	FSCRT_Var_SetInt32Value(retVal, 0)
	//typedef FS_INT32	(*FSCRT_LPAPPFunc_BrowseFile)(FS_BOOL bSave, const FSCRT_BSTR* initFileName, FSCRT_BSTR* path);
	FS_RESULT ret = pFormFiller->m_pJSActionHandle->BrowseFile(&sPath);
	//FS_RESULT ret = pApp->CallFunction(NULL, FSCRT_APPFUNC_BROWSEFILE, inParams, 3, &retVal);
	FSCRT_ST_FSUTF8ToFXWStr(&sPath, wsFilePath);

	FSCRT_BStr_Clear(&sPath);
	return (FSCRT_ERRCODE_SUCCESS == ret)? TRUE : FALSE;
#else
	return TRUE;
#endif
}

/* --------------------------------- methods --------------------------------- */

FX_BOOL JField::browseForFileToSubmit(OBJ_METHOD_PARAMS)
{
	ASSERT(m_pDocument != NULL);

	CFX_PtrArray FieldArray;
	GetFormFields(m_FieldName, FieldArray);
	if (FieldArray.GetSize() <= 0) return FALSE;

	CPDF_FormField* pFormField = (CPDF_FormField*)FieldArray.ElementAt(0);
	ASSERT(pFormField != NULL);

// 	CFSCRT_LTAppHandler* pApp = FSCRT_GetLTAppHandler();
// 	if (!pApp) return TRUE;

	if (/*(pFormField->GetFieldFlags() & FIELDFLAG_FILESELECT) && */
		(pFormField->GetFieldType() == FIELDTYPE_TEXTFIELD))
	{		
		CFX_WideString wsFileName;
		JS_fieldBrowse(wsFileName);
		if(!wsFileName.IsEmpty())
		{
 			pFormField->SetValue(wsFileName);
 			UpdateFormField(m_pDocument, pFormField, TRUE, TRUE, TRUE);
         }
	}
	else 
		return FALSE;

	return TRUE;
}


FX_BOOL JField::buttonGetCaption(OBJ_METHOD_PARAMS)
{
	ASSERT(m_pDocument != NULL);

	FX_INT32 nface = 0;
	FX_INT32 iSize = params.size();
	if ( iSize >= 1)
		nface = (FX_INT32) params[0];

	CFX_PtrArray FieldArray;
	GetFormFields(m_FieldName,FieldArray);
	if (FieldArray.GetSize() <= 0) return FALSE;

	CPDF_FormField* pFormField = (CPDF_FormField*)FieldArray.ElementAt(0);
	ASSERT(pFormField != NULL);
	
	if (pFormField->GetFieldType() != FIELDTYPE_PUSHBUTTON)
		return FALSE;

	CPDF_FormControl* pFormControl = GetSmartFieldControl(pFormField);
	if (!pFormControl)return FALSE;
	
	if (nface == 0)
		vRet = pFormControl->GetNormalCaption();
	else if (nface == 1)
		vRet = pFormControl->GetDownCaption();
	else if (nface == 2)
		vRet = pFormControl->GetRolloverCaption();
	else
		return FALSE;

	return TRUE;
}

//#pragma warning(disable: 4800)

FX_BOOL JField::buttonGetIcon(OBJ_METHOD_PARAMS)
{
	ASSERT(m_pDocument != NULL);

	FX_INT32 nface = 0;
	FX_INT32 iSize = params.size();
	if ( iSize >= 1)
		nface = (FX_INT32) params[0];
	
	CFX_PtrArray FieldArray;
	GetFormFields(m_FieldName,FieldArray);
	if (FieldArray.GetSize() <= 0) return FALSE;

	CPDF_FormField* pFormField = (CPDF_FormField*)FieldArray.ElementAt(0);
	ASSERT(pFormField != NULL);
	
	if (pFormField->GetFieldType() != FIELDTYPE_PUSHBUTTON)
		return FALSE;

	CPDF_FormControl* pFormControl = pFormField->GetControl(0);//GetSmartFieldControl(pFormField);
	if (!pFormControl)return FALSE;

	CFXJS_Context* pContext = (CFXJS_Context*)cc;
	ASSERT(pContext != NULL);

	CFXJS_Runtime* pRuntime = pContext->GetJSRuntime();
	if(!pRuntime) return TRUE;
	
	JSFXObject* pObj = DS_NewFxDynamicObj(pRuntime, pContext, DS_GetObjDefnID(pRuntime, L"Icon"));
	ASSERT(pObj != NULL);
	
	CFXJS_Icon* pJS_Icon = (CFXJS_Icon*)DS_GetPrivate(pObj);
	ASSERT(pJS_Icon != NULL);

	JIcon* pIcon = (JIcon*)pJS_Icon->GetEmbedObject();
	ASSERT(pIcon != NULL);

	CPDF_Stream* pIconStream = NULL;
	if (nface == 0)
		pIconStream = pFormControl->GetNormalIcon();
	else if (nface == 1)
		pIconStream = pFormControl->GetDownIcon();
	else if (nface == 2)
		pIconStream = pFormControl->GetRolloverIcon();
	else
		return FALSE;

	pIcon->SetStream(pIconStream);
	vRet = pJS_Icon;

	return TRUE;
}

//#pragma warning(default: 4800)

FX_BOOL JField::buttonImportIcon(OBJ_METHOD_PARAMS)
{
#if 0  
	ASSERT(m_pDocument != NULL);

	CFX_PtrArray FieldArray;
	GetFormFields(m_FieldName,FieldArray);
	if (FieldArray.GetSize() <= 0) return FALSE;

	CPDF_FormField* pFormField = (CPDF_FormField*)FieldArray.ElementAt(0);
	if (!pFormField)return FALSE;

	CPDFDoc_Environment* pEnv = m_pDocument->GetEnv();
	ASSERT(pEnv);

	CFX_WideString sIconFileName = pEnv->JS_fieldBrowse();
	if (sIconFileName.IsEmpty()) 
	{
		vRet = 1;
		return TRUE;
	}

	FSFormImp* pInterForm = (FSFormImp*)m_pDocument->GetInterForm();
	ASSERT(pInterForm != NULL);

	CPDF_Stream* pStream = pInterForm->LoadImageFromFile(sIconFileName);
	if (!pStream) 
	{
		vRet = -1;
		return TRUE;
	}

	CPDF_FormControl* pFormControl = GetSmartFieldControl(pFormField);
	if (!pFormControl)return FALSE;

	pFormControl->SetNormalIcon(pStream);
	UpdateFormControl(m_pDocument, pFormControl, TRUE, TRUE, TRUE);

	vRet = 0;
#endif // 0
	return TRUE;
}

FX_BOOL JField::buttonSetCaption(OBJ_METHOD_PARAMS)
{
	ASSERT(m_pDocument != NULL);

	if (!m_bCanSet) return FALSE;

	CFX_WideString csCaption;

	FX_INT32 iSize = params.size();
	if ( iSize >= 1)
	{
		csCaption = params[0];
	}
	FX_INT32 nface =0;
	if ( iSize >= 2)
		nface = (FX_INT32) params[1];

	CFX_PtrArray FieldArray;
	GetFormFields(m_FieldName,FieldArray);
	if (FieldArray.GetSize() <= 0) return FALSE;

	CPDF_FormField* pFormField = (CPDF_FormField*)FieldArray.ElementAt(0);
	ASSERT(pFormField != NULL);
	
	if (pFormField->GetFieldType() != FIELDTYPE_PUSHBUTTON)
		return FALSE;
	
	if (m_nFormControlIndex < 0)
	{
		for (FX_INT32 i = 0 , sz = pFormField->CountControls(); i < sz; i++)
		{
			CPDF_FormControl* pFormControl = pFormField->GetControl(i);
			ASSERT(pFormControl != NULL);

			switch (nface)
			{
			case 0:
				pFormControl->SetNormalCaption(csCaption);
				break;
			case 1:
				pFormControl->SetDownCaption(csCaption);
				break;
			case 2:
				pFormControl->SetRolloverCaption(csCaption);
				break;
			}
		}
		UpdateFormField(m_pDocument, pFormField, TRUE, TRUE, TRUE);
	}
	else
	{
		CPDF_FormControl* pFormControl = pFormField->GetControl(m_nFormControlIndex);
		if (!pFormControl)return FALSE;

		switch (nface)
		{
		case 0:
			pFormControl->SetNormalCaption(csCaption);
			break;
		case 1:
			pFormControl->SetDownCaption(csCaption);
			break;
		case 2:
			pFormControl->SetRolloverCaption(csCaption);
			break;
		}
		UpdateFormControl(m_pDocument, pFormControl, TRUE, TRUE, TRUE);
	}

	return TRUE;
}

FX_BOOL JField::buttonSetIcon(OBJ_METHOD_PARAMS)
{
	ASSERT(m_pDocument != NULL);
	CFXJS_Context* pContext = (CFXJS_Context*)cc;
	ASSERT(pContext != NULL);
	CFXJS_Runtime* pRuntime = pContext->GetJSRuntime();
	if(!pRuntime) return TRUE;

	if (!m_bCanSet) return FALSE;
	
	if(params.size() == 0)
		return FALSE;

	if(params[0].GetType() != VT_fxobject)
		return FALSE;

	CFXJS_Value Oicon = params[0];
	JSFXObject* pJSIcon = (JSFXObject*)Oicon;
	if (!pJSIcon) return TRUE;
	if (DS_GetObjDefnID(pJSIcon) != DS_GetObjDefnID(pRuntime, L"Icon")) return FALSE;

	CFXJS_EmbedObj* pIcon = ((CFXJS_Object*)params[0])->GetEmbedObject();
	if (!pIcon)return FALSE;

	FX_INT32 nface = 0;
	FX_INT32 iSize = params.size();
	if ( iSize >= 2)
		nface = (FX_INT32) params[1];

	CFX_PtrArray FieldArray;
	GetFormFields(m_FieldName,FieldArray);
	if (FieldArray.GetSize() <= 0) return FALSE;

	CPDF_FormField* pFormField = (CPDF_FormField*)FieldArray.ElementAt(0);
	ASSERT(pFormField != NULL);
	
	if (pFormField->GetFieldType() != FIELDTYPE_PUSHBUTTON)
		return FALSE;	

	if (m_nFormControlIndex < 0)
	{
		for (FX_INT32 i=0,sz=pFormField->CountControls(); i<sz; i++)
		{
			CPDF_FormControl* pFormControl = pFormField->GetControl(i);
			ASSERT(pFormControl != NULL);

			switch (nface)
			{
			case 0:
				pFormControl->SetNormalIcon(((JIcon*)pIcon)->GetStream());
				break;
			case 1:
				pFormControl->SetDownIcon(((JIcon*)pIcon)->GetStream());
				break;
			case 2:
				pFormControl->SetRolloverIcon(((JIcon*)pIcon)->GetStream());
				break;
			}
		}
		UpdateFormField(m_pDocument, pFormField, TRUE, TRUE, TRUE);
	}
	else
	{
		CPDF_FormControl* pFormControl = pFormField->GetControl(m_nFormControlIndex);
		if (!pFormControl)return FALSE;

		switch (nface)
		{
		case 0:
			pFormControl->SetNormalIcon(((JIcon*)pIcon)->GetStream());
			break;
		case 1:
			pFormControl->SetDownIcon(((JIcon*)pIcon)->GetStream());
			break;
		case 2:
			pFormControl->SetRolloverIcon(((JIcon*)pIcon)->GetStream());
			break;
		}
		UpdateFormControl(m_pDocument, pFormControl, TRUE, TRUE, TRUE);
	}

	return TRUE;
}

FX_BOOL JField::checkThisBox(OBJ_METHOD_PARAMS)
{
	ASSERT(m_pDocument != NULL);

	if (!m_bCanSet) return FALSE;

	FX_INT32 iSize = params.size();
	FX_INT32 nWidget = -1;
	if ( iSize >= 1)
		nWidget= (FX_INT32) params[0];
	else
		return FALSE;
	FX_BOOL bCheckit = TRUE;
	if ( iSize >= 2)
		bCheckit = params[1];


	CFX_PtrArray FieldArray;
	GetFormFields(m_FieldName,FieldArray);
	if (FieldArray.GetSize() <= 0) return FALSE;

	CPDF_FormField* pFormField = (CPDF_FormField*)FieldArray.ElementAt(0);
	ASSERT(pFormField != NULL);
	
	if (pFormField->GetFieldType() != FIELDTYPE_CHECKBOX && pFormField->GetFieldType() != FIELDTYPE_RADIOBUTTON)
		return FALSE;	
	if(nWidget <0 || nWidget >= pFormField->CountControls())
		return FALSE;
	if (pFormField->GetFieldType() == FIELDTYPE_RADIOBUTTON)
		pFormField->CheckControl(nWidget, bCheckit, TRUE);
	else
		pFormField->CheckControl(nWidget, bCheckit, TRUE);

	UpdateFormField(m_pDocument, pFormField, TRUE, TRUE, TRUE);

	return TRUE;
}

FX_BOOL JField::clearItems(OBJ_METHOD_PARAMS)
{
	ASSERT(m_pDocument != NULL);

	if (!m_bCanSet) return FALSE;

	CFX_PtrArray FieldArray;
	GetFormFields(m_FieldName,FieldArray);
	if (FieldArray.GetSize() <= 0) return FALSE;

	CPDF_FormField* pFormField = (CPDF_FormField*)FieldArray.ElementAt(0);
	ASSERT(pFormField != NULL);

	if ((pFormField->GetFieldType() == FIELDTYPE_LISTBOX)
		|| (pFormField->GetFieldType() == FIELDTYPE_COMBOBOX))
	{
		pFormField->ClearOptions(TRUE);
		UpdateFormField(m_pDocument, pFormField, TRUE, TRUE, TRUE);
	}
	else
		return FALSE;

	return TRUE;
}

FX_BOOL JField::defaultIsChecked(OBJ_METHOD_PARAMS)
{
	ASSERT(m_pDocument != NULL);

	if (!m_bCanSet) return FALSE;

	FX_INT32 iSize = params.size();
	FX_INT32 nWidget = -1;
	if ( iSize >= 1)
		nWidget= (FX_INT32) params[0];
	else
		return FALSE;
	FX_BOOL bIsDefaultChecked = TRUE;
	if ( iSize >= 2)
		bIsDefaultChecked =  params[1];

	CFX_PtrArray FieldArray;
	GetFormFields(m_FieldName,FieldArray);
	if (FieldArray.GetSize() <= 0) return FALSE;

	CPDF_FormField* pFormField = (CPDF_FormField*)FieldArray.ElementAt(0);
	ASSERT(pFormField != NULL);

	if(nWidget <0 || nWidget >= pFormField->CountControls())
	{
		vRet = FALSE;
		return FALSE;
	}
	if ((pFormField->GetFieldType() == FIELDTYPE_CHECKBOX)
		|| (pFormField->GetFieldType() == FIELDTYPE_RADIOBUTTON))
	{
		pFormField->DefaultCheckControl(nWidget, bIsDefaultChecked);
		pFormField->CheckControl(nWidget,bIsDefaultChecked,TRUE);
		
		UpdateFormField(m_pDocument, pFormField, TRUE, TRUE, TRUE);

		vRet = TRUE;
	}
	else
		vRet = FALSE;

	return TRUE;
}

FX_BOOL JField::deleteItemAt(OBJ_METHOD_PARAMS)
{
	ASSERT(m_pDocument != NULL);

	if (!m_bCanSet) return FALSE;

	FX_INT32 nIdx = 0;
	FX_INT32 iSize = params.size();
	FX_BOOL boptional = FALSE;
	if ( iSize >= 1)
	{
		nIdx = (FX_INT32) params[0];
		boptional = TRUE;
	}

	CFX_PtrArray FieldArray;
	GetFormFields(m_FieldName,FieldArray);
	if (FieldArray.GetSize() <= 0) return FALSE;

	CPDF_FormField* pFormField = (CPDF_FormField*)FieldArray.ElementAt(0);
	ASSERT(pFormField != NULL);

	if ((pFormField->GetFieldType() == FIELDTYPE_LISTBOX)
		|| (pFormField->GetFieldType() == FIELDTYPE_COMBOBOX))
	{
		if (boptional)
			pFormField->DeleteOption(nIdx, TRUE);
		else
		{
			for (FX_INT32 i=0,sz = pFormField->CountOptions(); i< sz; i++)
			{
				if (pFormField->IsItemSelected(i))
				{
					pFormField->DeleteOption(i, TRUE);
					break;
				}
				else
					continue;
			}
		}
		UpdateFormField(m_pDocument, pFormField, TRUE, TRUE, TRUE);
	}
	else
		return FALSE;

	return TRUE;
}

FX_INT32 FXJS_COMPARESTRING(CFX_WideString* ps1, CFX_WideString* ps2)
{
	ASSERT(ps1 != NULL);
	ASSERT(ps2 != NULL);

	return ps1->Compare(*ps2);
}


FX_BOOL JField::getArray(OBJ_METHOD_PARAMS)
{
	ASSERT(m_pDocument != NULL);

	CFX_PtrArray FieldArray;
	GetFormFields(m_FieldName,FieldArray);
	if (FieldArray.GetSize() <= 0) return FALSE;

	CGW_ArrayTemplate<CFX_WideString*> swSort;

	for (FX_INT32 i=0,sz=FieldArray.GetSize(); i<sz; i++)
	{
		CPDF_FormField* pFormField = (CPDF_FormField*)FieldArray.ElementAt(i);
		ASSERT(pFormField != NULL);

		swSort.Add(new CFX_WideString(pFormField->GetFullName()));
		
	}
	swSort.Sort(FXJS_COMPARESTRING);

	CFXJS_Context* pContext = (CFXJS_Context*)cc;
	ASSERT(pContext != NULL);
	CFXJS_Runtime* pRuntime = pContext->GetJSRuntime();
	if(!pRuntime) return TRUE;

	CFXJS_Array FormFieldArray;
	for (FX_INT32 j=0,jsz = swSort.GetSize(); j<jsz; j++)
	{
		CFX_WideString* pStr = swSort.GetAt(j);

		JSFXObject* pObj = DS_NewFxDynamicObj(pRuntime, pContext, DS_GetObjDefnID(pRuntime, L"Field"));
		ASSERT(pObj != NULL);

		CFXJS_Field* pJSField = (CFXJS_Field*)DS_GetPrivate(pObj);
		ASSERT(pJSField != NULL);

		JField* pField = (JField*)pJSField->GetEmbedObject(); 
		ASSERT(pField != NULL);

		pField->AttachField(this->m_pJSDoc, *pStr);
	
		CFXJS_Value FormFieldValue;
		FormFieldValue = pJSField;
		FormFieldArray.SetElement(j, FormFieldValue);

		delete pStr;
	}

	vRet = FormFieldArray;
	swSort.RemoveAll();
	return TRUE;
}
	
FX_BOOL JField::getItemAt(OBJ_METHOD_PARAMS)
{
	ASSERT(m_pDocument != NULL);

	FX_INT32 nIdx;
	if (params.size() >=1)
		nIdx = (FX_INT32) params[0];
	FX_BOOL bExport = TRUE;
	FX_INT32 iSize = params.size();
	if ( iSize >= 2)
	{
		bExport =(FX_BOOL) params[1];
	}

	CFX_PtrArray FieldArray;
	GetFormFields(m_FieldName,FieldArray);
	if (FieldArray.GetSize() <= 0) return FALSE;

	CPDF_FormField* pFormField = (CPDF_FormField*)FieldArray.ElementAt(0);
	ASSERT(pFormField != NULL);

	if ((pFormField->GetFieldType() == FIELDTYPE_LISTBOX)
		|| (pFormField->GetFieldType() == FIELDTYPE_COMBOBOX))
	{
		if (nIdx == -1 || nIdx > pFormField->CountOptions())
			nIdx = pFormField->CountOptions() -1;
		if (bExport)
		{
			CFX_WideString strval = pFormField->GetOptionValue(nIdx);
			if (strval.IsEmpty())
				vRet = pFormField->GetOptionLabel(nIdx);
			else
				vRet = strval;
		}
		else
			vRet = pFormField->GetOptionLabel(nIdx);
	}
	else
		return FALSE;

	return TRUE;
}

FX_BOOL JField::getLock(OBJ_METHOD_PARAMS)
{
	return FALSE;
}

FX_BOOL JField::insertItemAt(OBJ_METHOD_PARAMS)
{
	ASSERT(m_pDocument != NULL);

	if (!m_bCanSet) return FALSE;

	CFX_WideString cName;
	if (params.size() >=1 )
		cName = params[0];
	CFX_WideString cExport,cLabel;
	FX_INT32 nIdx = 0;
	FX_BOOL bOptionalExport = FALSE;
    FX_BOOL bOptionalIdx = FALSE;
	if (params.size() >= 2)
	{
		bOptionalExport = TRUE;
		cExport = params[1];
	}
	if (params.size() >= 3)
	{
		bOptionalIdx = TRUE;
		nIdx = (FX_INT32) params[2];
	}

	CFX_PtrArray FieldArray;
	GetFormFields(m_FieldName,FieldArray);
	if (FieldArray.GetSize() <= 0) return FALSE;

	CPDF_FormField* pFormField = (CPDF_FormField*)FieldArray.ElementAt(0);
	ASSERT(pFormField != NULL);

	if ((pFormField->GetFieldType() == FIELDTYPE_LISTBOX)
		|| (pFormField->GetFieldType() == FIELDTYPE_COMBOBOX))
	{
		if (this->ValueIsOccur(pFormField, cName))
			return TRUE;
		if (bOptionalExport)
		{
			pFormField->InsertOption(cName, nIdx,TRUE);
			pFormField->SetOptionValue(nIdx, cExport, TRUE);
			pFormField->SetValue(cName);
		}
		else
		{
			pFormField->InsertOption(cName, nIdx, TRUE);
			pFormField->SetOptionValue(nIdx, cName, TRUE);
			pFormField->SetValue(cName);
		}

    	UpdateFormField(m_pDocument, pFormField, TRUE, TRUE, TRUE);
	}
	else
		return FALSE;

	return TRUE;
}

FX_BOOL JField::isBoxChecked(OBJ_METHOD_PARAMS)
{
	ASSERT(m_pDocument != NULL);

	FX_INT32 nIndex = -1;
	if (params.size() >=1)
		nIndex = (FX_INT32) params[0];

	CFX_PtrArray FieldArray;
	GetFormFields(m_FieldName,FieldArray);
	if (FieldArray.GetSize() <= 0) return FALSE;

	CPDF_FormField* pFormField = (CPDF_FormField*)FieldArray.ElementAt(0);
	ASSERT(pFormField != NULL);

	if(nIndex <0 || nIndex >= pFormField->CountControls())
	{
		vRet = FALSE;
		return FALSE;
	}

	if ((pFormField->GetFieldType() == FIELDTYPE_CHECKBOX)
		|| (pFormField->GetFieldType() == FIELDTYPE_RADIOBUTTON))
	{
		if (pFormField->GetControl(nIndex)->IsChecked() !=0 )
			vRet = TRUE;
		else
			vRet = FALSE;
	}
	else
		vRet = FALSE;

	return TRUE;
}

FX_BOOL JField::isDefaultChecked(OBJ_METHOD_PARAMS)
{
	ASSERT(m_pDocument != NULL);

	FX_INT32 nIndex = -1;
	if (params.size() >=1)
		nIndex = (FX_INT32) params[0];

	CFX_PtrArray FieldArray;
	GetFormFields(m_FieldName,FieldArray);
	if (FieldArray.GetSize() <= 0) return FALSE;

	CPDF_FormField* pFormField = (CPDF_FormField*)FieldArray.ElementAt(0);
	ASSERT(pFormField != NULL);

	if(nIndex <0 || nIndex >= pFormField->CountControls())
	{
		vRet = FALSE;
		return FALSE;
	}
	if ((pFormField->GetFieldType() == FIELDTYPE_CHECKBOX)
		|| (pFormField->GetFieldType() == FIELDTYPE_RADIOBUTTON))
	{
		if (pFormField->GetControl(nIndex)->IsDefaultChecked() != 0)
			vRet = TRUE;
		else
			vRet = FALSE;
	}
	else
		vRet = FALSE;

	return TRUE;
}

FX_BOOL JField::setAction(OBJ_METHOD_PARAMS)
{
	ASSERT(m_pDocument != NULL);

	if (!m_bCanSet) return FALSE;

	CPDF_Action  JSAction;
	CPDF_AAction aa ;
	CFX_ByteString cTrigger,cScript;
	if (params.size() >=2)
	{
		cTrigger = params[0];
		cScript =  params[1];
	}
	else
		return FALSE;
	
	CFX_PtrArray FieldArray;
	GetFormFields(m_FieldName,FieldArray);
	if (FieldArray.GetSize() <= 0) return FALSE;

	for (FX_INT32 i=0,isz = FieldArray.GetSize(); i<isz; i++)
	{
		CPDF_FormField* pFormField = (CPDF_FormField*)FieldArray.ElementAt(i);
		ASSERT(pFormField != NULL);

		CPDF_AAction::AActionType eType = CPDF_AAction::CursorEnter;
		if (cTrigger == "MouseEnter")
			eType = CPDF_AAction::CursorEnter;
		else if (cTrigger == "MouseExit")
			eType = CPDF_AAction::CursorExit;
		else if (cTrigger == "MouseUp")
			eType = CPDF_AAction::ButtonUp;
		else if (cTrigger == "MouseDown")
			eType = CPDF_AAction::ButtonDown;
		else if (cTrigger == "OnFocus")
			eType = CPDF_AAction::GetFocus;
		else if (cTrigger == "OnBlur")
			eType = CPDF_AAction::LoseFocus;
		else if (cTrigger == "Keystroke")
			eType = CPDF_AAction::KeyStroke;
		else if (cTrigger == "Validate")
			eType = CPDF_AAction::Validate;
		else if (cTrigger == "Calculate")
			eType = CPDF_AAction::Calculate;
		else if (cTrigger == "Format")
				eType = CPDF_AAction::Format;
		else
			return FALSE;
		switch (eType)
		{
			case CPDF_AAction::CursorEnter:
			case CPDF_AAction::ButtonUp:
			case CPDF_AAction::ButtonDown:
			case CPDF_AAction::CursorExit:
			case CPDF_AAction::GetFocus:
			case CPDF_AAction::LoseFocus:
				{
					if (m_nFormControlIndex < 0)
					{
						for (FX_INT32 j=0,jsz = pFormField->CountControls(); j<jsz; j++)
						{
							CPDF_FormControl* pFormControl = pFormField->GetControl(j);
							ASSERT(pFormControl != NULL);

							CPDF_Document* pPDFDoc = m_pDocument->getPDFDocument();
							ASSERT(pPDFDoc != NULL);
							JSAction = CPDF_Action::CreateAction(pPDFDoc,CPDF_Action::JavaScript);
							JSAction.SetJavaScript(pPDFDoc, cScript);
							aa = pFormControl->GetAdditionalAction();
							aa.SetAction(m_pDocument->getPDFDocument(),eType, JSAction);
							pFormControl->SetAdditionalAction(aa);
						}
					}
					else
					{
						CPDF_FormControl* pFormControl = pFormField->GetControl(m_nFormControlIndex);
						if (!pFormControl)return FALSE;

						CPDF_Document* pPDFDoc = m_pDocument->getPDFDocument();
						ASSERT(pPDFDoc != NULL);

						JSAction = CPDF_Action::CreateAction(pPDFDoc,CPDF_Action::JavaScript);
						JSAction.SetJavaScript(pPDFDoc, cScript);
						aa = pFormControl->GetAdditionalAction();
						aa.SetAction(m_pDocument->getPDFDocument(),eType, JSAction);
						pFormControl->SetAdditionalAction(aa);
					}

				}
				break;
			case CPDF_AAction::KeyStroke:
			case CPDF_AAction::Validate:
			case CPDF_AAction::Calculate:
			case CPDF_AAction::Format:
				{
					CPDF_Document* pPDFDoc = m_pDocument->getPDFDocument();
					ASSERT(pPDFDoc != NULL);

					JSAction = CPDF_Action::CreateAction(pPDFDoc,CPDF_Action::JavaScript);
					JSAction.SetJavaScript(pPDFDoc, cScript);
					aa = pFormField->GetAdditionalAction();
					aa.SetAction(m_pDocument->getPDFDocument(), eType, JSAction);
					pFormField->SetAdditionalAction(aa);
				}
				break;
			default:
				return FALSE;
		}

		UpdateFormField(m_pDocument, pFormField, TRUE, TRUE, TRUE);
	}

	return TRUE;
}

FX_BOOL JField::JS_GetCurPageIndex(FX_INT32& nPageIndex)
{
	CFS_Library* pLib = CFS_Library::get();
	if (!pLib) return FALSE;
	FSActionHandler* pActionHandler = pLib->getActionHandler();
	if (!pActionHandler) return FALSE;
	nPageIndex = pActionHandler->getCurrentPage((FSPDFDoc*)m_pDocument);
 	return TRUE;
}

FX_BOOL JField::setFocus(OBJ_METHOD_PARAMS)
{
	ASSERT(m_pDocument != NULL);

	CFX_PtrArray FieldArray;
	GetFormFields(m_FieldName,FieldArray);
	if (FieldArray.GetSize() <= 0) return FALSE;

	CPDF_FormField* pFormField = (CPDF_FormField*)FieldArray.ElementAt(0);
	ASSERT(pFormField != NULL);

	FX_INT32 nCount = pFormField->CountControls();

	if (nCount < 1) return FALSE;

	FSFormImp* pInterForm = (FSFormImp*)m_pDocument->GetForm();
	ASSERT(pInterForm != NULL);

	FSFormControl* pWidget = NULL;
	if (nCount == 1)
	{	
		//pWidget = pInterForm->GetWidget(pFormField->GetControl(0));
	}
	else
	{
		FX_INT32 nCurPageIndex = 0;
		if(!JS_GetCurPageIndex(nCurPageIndex)) return FALSE;
		//FSPDFPage* pCurLTPage = NULL;
		//m_pDocument->GetPage(nCurPageIndex, &pCurLTPage);
		//CPDF_Page* pPage = pCurLTPage->m_pSTPage;
		//if(!pPage) return FALSE;
// 		if (CPDFSDK_PageView* pCurPageView = m_pDocument->GetPageView(pPage))
// 		{
// 			for (FX_INT32 i=0; i<nCount; i++)
// 			{
// 				if (FSFormControl* pTempWidget =  pInterForm->GetWidget(pFormField->GetControl(i)))
// 				{				
// 					if (pTempWidget->GetPDFPage() == pCurPageView->GetPDFPage())
// 					{
// 						pWidget = pTempWidget;
// 						break;
// 					}
// 				}
// 			}
// 		}
	}

	if (pWidget)
	{
// 		CReader_Page* pPage = pWidget->GetPage();
// 		ASSERT(pPage != NULL);
// 
// 		if (CReader_PageView* pPageView = pDocView->GetPageView(pPage->GetPageIndex()))
// 		{	
// 			pWidget->GotoPageView(pPageView);
// 		}
// 
// 		m_pDocument->SetFocusAnnot(pWidget);
	}

	return TRUE;
}

FX_BOOL JField::setItems(OBJ_METHOD_PARAMS)
{
	ASSERT(m_pDocument != NULL);

	if (!m_bCanSet) return FALSE;

	CFXJS_Array oArray;
	CFXJS_Value ElementValue;
	if (params.size() >= 1)
	{
		if (params[0].IsArrayObject())
			params[0].ConvertToArray(oArray);
	}
	else
		return FALSE;

	CFX_PtrArray FieldArray;
	GetFormFields(m_FieldName,FieldArray);
	if (FieldArray.GetSize() <= 0) return FALSE;

	CPDF_FormField* pFormField = (CPDF_FormField*)FieldArray.ElementAt(0);
	ASSERT(pFormField != NULL);

	if ((pFormField->GetFieldType() == FIELDTYPE_LISTBOX)
		|| (pFormField->GetFieldType() == FIELDTYPE_COMBOBOX))
	{
		for (FX_INT32 i = 0,sz = oArray.GetLength(); i < sz; i++)
		{
			oArray.GetElement(i, ElementValue);
			if (ElementValue.IsArrayObject())
			{
				CFXJS_Array ElementArray;
				CFXJS_Value UserValue;
				CFXJS_Value ExportValue;
				CFX_WideString csOptLabel;
				CFX_WideString csOptValue;
				ElementValue.ConvertToArray(ElementArray);
				ElementArray.GetElement(0,UserValue);
				ElementArray.GetElement(1,ExportValue);
				csOptLabel = UserValue;
				csOptValue = ExportValue;
				if (this->ValueIsOccur(pFormField, csOptLabel))
					continue;
				pFormField->InsertOption(csOptLabel, i);
				pFormField->SetOptionLabel(i, csOptLabel, TRUE);
				pFormField->SetOptionValue(i, csOptValue, TRUE);
			}
			else
			{
				CFX_WideString csOptLabel;
				csOptLabel = ElementValue;
				if (this->ValueIsOccur(pFormField, csOptLabel))
					continue;
				pFormField->InsertOption(csOptLabel, i);
				pFormField->SetOptionLabel(i, csOptLabel, TRUE);
				pFormField->SetOptionValue(i, csOptLabel, TRUE);
			}
		}

		pFormField->SetItemSelection(0,TRUE,TRUE);
		UpdateFormField(m_pDocument, pFormField, TRUE, TRUE, TRUE);
	}
	else 
		return FALSE;

	return TRUE;
}

FX_BOOL JField::setLock(OBJ_METHOD_PARAMS)
{
	return FALSE;
}

FX_BOOL JField::signatureGetModifications(OBJ_METHOD_PARAMS)
{
	return FALSE;
}

FX_BOOL JField::signatureGetSeedValue(OBJ_METHOD_PARAMS)
{
	return FALSE;
}

FX_BOOL JField::signatureInfo(OBJ_METHOD_PARAMS)
{
	return FALSE;
}

FX_BOOL JField::signatureSetSeedValue(OBJ_METHOD_PARAMS)
{
	return FALSE;
}

FX_BOOL JField::signatureSign(OBJ_METHOD_PARAMS)
{
	return FALSE;
}

FX_BOOL JField::signatureValidate(OBJ_METHOD_PARAMS)
{
	return FALSE;
}

FX_BOOL JField::source(OBJ_PROP_PARAMS)
{
	if (vp.IsGetting())
	{
		vp << (CFXJS_Object*)NULL;
	}

	return TRUE;
}

/////////////////////////////////////////// delay /////////////////////////////////////////////

void JField::AddDelay_Int(enum FIELD_PROP prop, FX_INT32 n)
{
	ASSERT(m_pJSDoc != NULL);

	CJS_DelayData* pNewData = new CJS_DelayData;
	pNewData->sFieldName = m_FieldName;
	pNewData->nControlIndex = m_nFormControlIndex;
	pNewData->eProp = prop;
	pNewData->num = n;

	m_pJSDoc->AddDelayData(pNewData);
}

void JField::AddDelay_Bool(enum FIELD_PROP prop,bool b)
{
	ASSERT(m_pJSDoc != NULL);

	CJS_DelayData* pNewData = new CJS_DelayData;
	pNewData->sFieldName = m_FieldName;
	pNewData->nControlIndex = m_nFormControlIndex;
	pNewData->eProp = prop;
	pNewData->b = b;

	m_pJSDoc->AddDelayData(pNewData);
}

void JField::AddDelay_String(enum FIELD_PROP prop, const CFX_ByteString& string)
{
	ASSERT(m_pJSDoc != NULL);

	CJS_DelayData* pNewData = new CJS_DelayData;
	pNewData->sFieldName = m_FieldName;
	pNewData->nControlIndex = m_nFormControlIndex;
	pNewData->eProp = prop;
	pNewData->string = string;

	m_pJSDoc->AddDelayData(pNewData);
}

void JField::AddDelay_WideString(enum FIELD_PROP prop, const CFX_WideString& string)
{
	ASSERT(m_pJSDoc != NULL);

	CJS_DelayData* pNewData = new CJS_DelayData;
	pNewData->sFieldName = m_FieldName;
	pNewData->nControlIndex = m_nFormControlIndex;
	pNewData->eProp = prop;
	pNewData->widestring = string;

	m_pJSDoc->AddDelayData(pNewData);
}

void JField::AddDelay_Rect(enum FIELD_PROP prop, const CPDF_Rect& rect)
{
	ASSERT(m_pJSDoc != NULL);

	CJS_DelayData* pNewData = new CJS_DelayData;
	pNewData->sFieldName = m_FieldName;
	pNewData->nControlIndex = m_nFormControlIndex;
	pNewData->eProp = prop;
	pNewData->rect = rect;

	m_pJSDoc->AddDelayData(pNewData);
}

void JField::AddDelay_Color(enum FIELD_PROP prop, const CPWL_Color& color)
{
	ASSERT(m_pJSDoc != NULL);

	CJS_DelayData* pNewData = new CJS_DelayData;
	pNewData->sFieldName = m_FieldName;
	pNewData->nControlIndex = m_nFormControlIndex;
	pNewData->eProp = prop;
	pNewData->color = color;

	m_pJSDoc->AddDelayData(pNewData);
}

void JField::AddDelay_WordArray(enum FIELD_PROP prop, const CFX_DWordArray& array)
{
	ASSERT(m_pJSDoc != NULL);

	CJS_DelayData* pNewData = new CJS_DelayData;
	pNewData->sFieldName = m_FieldName;
	pNewData->nControlIndex = m_nFormControlIndex;
	pNewData->eProp = prop;

	for (FX_INT32 i=0,sz=array.GetSize(); i<sz; i++)
		pNewData->wordarray.Add(array.GetAt(i));

	m_pJSDoc->AddDelayData(pNewData);
}

void JField::AddDelay_WideStringArray(enum FIELD_PROP prop, const CJS_WideStringArray& array)
{
	ASSERT(m_pJSDoc != NULL);

	CJS_DelayData* pNewData = new CJS_DelayData;
	pNewData->sFieldName = m_FieldName;
	pNewData->nControlIndex = m_nFormControlIndex;
	pNewData->eProp = prop;
	for (FX_INT32 i=0,sz=array.GetSize(); i<sz; i++)
		pNewData->widestringarray.Add(array.GetAt(i));

	m_pJSDoc->AddDelayData(pNewData);
}

void JField::DoDelay(FSPDFDoc* pDocument, CJS_DelayData* pData)
{
	ASSERT(pDocument != NULL);
	ASSERT(pData != NULL);

	switch (pData->eProp)
	{
	case FP_ALIGNMENT:
		JField::SetAlignment(pDocument, pData->sFieldName, pData->nControlIndex, pData->string);
		break;
	case FP_BORDERSTYLE:
		JField::SetBorderStyle(pDocument, pData->sFieldName, pData->nControlIndex, pData->string);
		break;
	case FP_BUTTONALIGNX:
		JField::SetButtonAlignX(pDocument, pData->sFieldName, pData->nControlIndex, pData->num);
		break;
	case FP_BUTTONALIGNY:
		JField::SetButtonAlignY(pDocument, pData->sFieldName, pData->nControlIndex, pData->num);
		break;
	case FP_BUTTONFITBOUNDS:
		JField::SetButtonFitBounds(pDocument, pData->sFieldName, pData->nControlIndex, pData->b);
		break;
	case FP_BUTTONPOSITION:
		JField::SetButtonPosition(pDocument, pData->sFieldName, pData->nControlIndex, pData->num);
		break;
	case FP_BUTTONSCALEHOW:
		JField::SetButtonScaleHow(pDocument, pData->sFieldName, pData->nControlIndex, pData->num);
		break;
	case FP_BUTTONSCALEWHEN:
		JField::SetButtonScaleWhen(pDocument, pData->sFieldName, pData->nControlIndex, pData->num);
		break;
	case FP_CALCORDERINDEX:
		JField::SetCalcOrderIndex(pDocument, pData->sFieldName, pData->nControlIndex, pData->num);
		break;
	case FP_CHARLIMIT:
		JField::SetCharLimit(pDocument, pData->sFieldName, pData->nControlIndex, pData->num);
		break;
	case FP_COMB:
		JField::SetComb(pDocument, pData->sFieldName, pData->nControlIndex, pData->b);
		break;
	case FP_COMMITONSELCHANGE:
		JField::SetCommitOnSelChange(pDocument, pData->sFieldName, pData->nControlIndex, pData->b);
		break;
	case FP_CURRENTVALUEINDICES:
		JField::SetCurrentValueIndices(pDocument, pData->sFieldName, pData->nControlIndex, pData->wordarray);
		break;
	case FP_DEFAULTVALUE:
		JField::SetDefaultValue(pDocument, pData->sFieldName, pData->nControlIndex, pData->widestring);
		break;
	case FP_DONOTSCROLL:
		JField::SetDoNotScroll(pDocument, pData->sFieldName, pData->nControlIndex, pData->b);
		break;
	case FP_DISPLAY:
		JField::SetDisplay(pDocument, pData->sFieldName, pData->nControlIndex, pData->num);
		break;
	case FP_FILLCOLOR:
		JField::SetFillColor(pDocument, pData->sFieldName, pData->nControlIndex, pData->color);
		break;
	case FP_HIDDEN:
		JField::SetHidden(pDocument, pData->sFieldName, pData->nControlIndex, pData->b);
		break;
	case FP_HIGHLIGHT:
		JField::SetHighlight(pDocument, pData->sFieldName, pData->nControlIndex, pData->string);
		break;
	case FP_LINEWIDTH:
		JField::SetLineWidth(pDocument, pData->sFieldName, pData->nControlIndex, pData->num);
		break;
	case FP_MULTILINE:
		JField::SetMultiline(pDocument, pData->sFieldName, pData->nControlIndex, pData->b);
		break;
	case FP_MULTIPLESELECTION:
		JField::SetMultipleSelection(pDocument, pData->sFieldName, pData->nControlIndex, pData->b);
		break;
	case FP_PASSWORD:
		JField::SetPassword(pDocument, pData->sFieldName, pData->nControlIndex, pData->b);
		break;
	case FP_RECT:
		JField::SetRect(pDocument, pData->sFieldName, pData->nControlIndex, pData->rect);
		break;
	case FP_RICHTEXT:
		JField::SetRichText(pDocument, pData->sFieldName, pData->nControlIndex, pData->b);
		break;
	case FP_RICHVALUE:
		break;
	case FP_ROTATION:
		JField::SetRotation(pDocument, pData->sFieldName, pData->nControlIndex, pData->num);
		break;
	case FP_STROKECOLOR:
		JField::SetStrokeColor(pDocument, pData->sFieldName, pData->nControlIndex, pData->color);
		break;
	case FP_STYLE:
		JField::SetStyle(pDocument, pData->sFieldName, pData->nControlIndex, pData->string);
		break;
	case FP_TEXTCOLOR:
		JField::SetTextColor(pDocument, pData->sFieldName, pData->nControlIndex, pData->color);
		break;
	case FP_TEXTFONT:
		JField::SetTextFont(pDocument, pData->sFieldName, pData->nControlIndex, pData->string);
		break;
	case FP_TEXTSIZE:
		JField::SetTextSize(pDocument, pData->sFieldName, pData->nControlIndex, pData->num);
		break;
	case FP_USERNAME:
		JField::SetUserName(pDocument, pData->sFieldName, pData->nControlIndex, pData->widestring);
		break;
	case FP_VALUE:
		JField::SetValue(pDocument, pData->sFieldName, pData->nControlIndex, pData->widestringarray);
		break;
	}
}

FX_BOOL JField::JS_GetPageRotation(FSPDFPage* pPage, FX_INT32& nPageRotation)
{
// 	CFSCRT_LTAppHandler* pApp = FSCRT_GetLTAppHandler();
// 	if (!pApp) return FALSE;
	FSPDFDocImp* pDoc = (FSPDFDocImp*)pPage->getDocument();
	FSFormFillerImp* pFormFiller = pDoc->GetForm()->GetFormFiller();

// 	FSCRT_VAR params = {FSCRT_VT_LPVOID, 0, 0};
// 	FSCRT_Var_SetVoidPtrValue(params, NULL);
// 	FSCRT_VAR retVal = {FSCRT_VT_INT32, 0, 0};
// 	FSCRT_Var_SetInt32Value(retVal, 0);

	CFS_Library* pLib = CFS_Library::get();
	if (!pLib) return FALSE;
	FSActionHandler* pActionHandler = pLib->getActionHandler();
	if (!pActionHandler) return FALSE;
	nPageRotation = pActionHandler->getPageRotation((FSPDFDoc*)pDoc, pPage->getIndex());
	return TRUE;
}

FX_BOOL JField::JS_GetPage(FSPDFDoc* pDoc, FX_INT32 pageIndex, FSPDFPage** ppPage)
{
//	FSFormFillerImp* pFormFiller = m_pDocument->GetForm()->GetFormFiller();
// 	CFSCRT_LTAppHandler* pApp = FSCRT_GetLTAppHandler();
// 	if (!pApp) return FALSE;

// 	FSCRT_VAR params = {FSCRT_VT_INT32, 0, 0};
// 	FSCRT_Var_SetInt32Value(params, 0);
// 	FSCRT_VAR retVal = {FSCRT_VT_LPVOID, 0, 0};
// 	FSCRT_Var_SetVoidPtrValue(retVal, NULL);
//	FSCRT_StartCallBackState();
	*ppPage = pDoc->getPage(pageIndex);
	return TRUE;
//	FS_RESULT ret = pDoc->GetPage(pageIndex, ppPage);
//In case of OOM, we need to make sure that at least the short term page is available.
//	if(FSCRT_ERRCODE_SUCCESS == ret) (*ppPage)->ST_Load(); 
//	FSCRT_EndCallBackState();
//	FSCRT_SetCallBackErrorCode(ret);
//	FS_RESULT ret = pApp->CallFunction(NULL, FSCRT_APPFUNC_GETPAGEROTATION, &params, 1, &retVal);
//	*ppPage = (FSPDFPage*)retVal.var.voidPtrVar;
//	return (ret == FSCRT_ERRCODE_SUCCESS) ? TRUE : FALSE;
}

#define JS_FIELD_MINWIDTH	1
#define JS_FIELD_MINHEIGHT	1

void JField::AddField(FSPDFDoc* pDocument, FX_INT32 nPageIndex, FX_INT32 nFieldType,
							const CFX_WideString& sName, const CPDF_Rect& rcCoords)
{
	ASSERT(pDocument != NULL);

	CPDF_Rect rcCoor = rcCoords;

	if (abs(rcCoor.right - rcCoor.left) < JS_FIELD_MINWIDTH)
		rcCoor.right = rcCoor.left + JS_FIELD_MINWIDTH;
	if (abs(rcCoor.top - rcCoor.bottom) < JS_FIELD_MINHEIGHT)
		rcCoor.top = rcCoor.bottom + JS_FIELD_MINHEIGHT;

	FSPDFPageImp* pPage = NULL;
	JS_GetPage(pDocument, nPageIndex, (FSPDFPage**)&pPage);
	if(!pPage)
		return;

	FSFormImp* pInterForm = ((FSPDFDocImp*)pDocument)->GetForm();
	ASSERT(pInterForm != NULL);

	CPDF_InterForm* pPDFForm = pInterForm->ST_GetSTInterForm()->m_pInterForm;
	ASSERT(pPDFForm != NULL);

    CFX_WideString strName2(sName);
	if (!pPDFForm->ValidateFieldName(strName2, nFieldType))
	{
		return;
	}
    CPDF_FormControl* pFormControl = pPDFForm->CreateControl(strName2, nFieldType);
	if (!pFormControl) 
	{
		return;
	}

	FX_INT32 nPageRotate = pPage->getRotation() * 90;
	
	FX_INT32 nDocRotate = 0;
	JS_GetPageRotation(pPage, nDocRotate);

 	FX_INT32 nRotate = nDocRotate * 90 + nPageRotate;
	pFormControl->SetRotation(nRotate);

	CPDF_Rect rcPage = pPage->getPage()->GetPageBBox();

	CPDF_Rect rcRotate;

	switch (nRotate % 360)
	{
	case 0: //
		rcRotate = rcCoor;
		break;
	case 90: //
		rcRotate.left = rcPage.Width() - rcCoor.top;
		rcRotate.right = rcPage.Width() - rcCoor.bottom;
		rcRotate.top = rcCoor.left + rcCoor.Width();
		rcRotate.bottom = rcCoor.left;
		break;
	case 180:
		rcRotate.left = rcPage.Width() - (rcCoor.left + rcCoor.Width());
		rcRotate.right = rcPage.Width() - rcCoor.left;
		rcRotate.top = rcPage.Height() - rcCoor.bottom;
		rcRotate.bottom = rcPage.Height() - rcCoor.top;
		break;
	case 270:
		rcRotate.left = rcCoor.bottom;
		rcRotate.right = rcCoor.top;
		rcRotate.top = rcPage.Height() - rcCoor.left;
		rcRotate.bottom = rcPage.Height() - rcCoor.right;
		break;
	}

	FSRectF tempRectf = {(FS_FLOAT)rcRotate.left, (FS_FLOAT)rcRotate.bottom, (FS_FLOAT)rcRotate.right, (FS_FLOAT)rcRotate.top};
	FSFormControlImp* pWidget = (FSFormControlImp*)pPage->addControl(pFormControl, tempRectf);
	if(!pWidget) return;
	pWidget->loadFormControl(pInterForm);

	FSBorderInfo borderInfo;
	borderInfo.style = FSAnnot::e_borderStyleSolid;
	borderInfo.width = 1;
	borderInfo.cloudIntensity = -1;
	pWidget->setBorderInfo(borderInfo);
	pWidget->setFlags(4);

	switch (nFieldType)
	{
	case FIELDTYPE_CHECKBOX:
		{
			CFX_WideString csCaption = L"4";
			pWidget->GetCPDFFormControl()->SetNormalCaption(csCaption);
			CPDF_FormField *pField = pWidget->GetCPDFFormControl()->GetField();
			ASSERT(pField);
			FX_INT32 iIndex = pField->GetControlIndex(pWidget->GetCPDFFormControl());
			pField->CheckControl(iIndex,FALSE);
		}
		break;
	case FIELDTYPE_RADIOBUTTON:
		{
			CFX_WideString csCaption = L"l";
			pWidget->GetCPDFFormControl()->SetNormalCaption(csCaption);
			CPDF_FormField *pField = pWidget->GetCPDFFormControl()->GetField();
			ASSERT(pField);
			FX_INT32 iIndex = pField->GetControlIndex(pWidget->GetCPDFFormControl());
			pField->CheckControl(iIndex,FALSE);
		}
		break;
	}

	pWidget->ResetAppearance(NULL);

}

#endif //defined(_FSDK_ALL_) || defined(_FSDK_JAVASCRIPT_)
