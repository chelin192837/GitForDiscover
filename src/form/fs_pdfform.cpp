
#include "../../include/common.h"
#include "../../include/fs_pdfdoc.h"
#include "../../include/fs_pdfform.h"

#include "../fxcore.h"
#include "../fs_util.h"
#include "fs_pdfformImp.h"
#include "../inc/fs_signatureImp.h"
#include "../inc/fs_pdfdocImp.h"


////////////////////////////////////////////////
// Class FSFormImp
FSFormImp::FSFormImp()
	: m_pSTInterForm(NULL)
	, m_pMapFields(NULL)
	, m_pFormFiller(NULL)
	, m_pPDFDoc(NULL)
	, m_bCalculate(NULL)
{
}

FSFormImp::~FSFormImp()
{
	if (m_pMapFields) 
	{
		FX_POSITION pos = m_pMapFields->GetStartPosition();
		while (pos)
		{
			void* pKey = NULL;
			FSFormFieldImp* pField = NULL;
			m_pMapFields->GetNextAssoc(pos, pKey, (void*&)pField);
			if (pField) {
				delete pField;
			}
		}

		m_pMapFields->RemoveAll();
		delete m_pMapFields;
		m_pMapFields = NULL;
	}

	if (m_pSTInterForm)
	{
		delete m_pSTInterForm;
		m_pSTInterForm = NULL;
	}
}

FS_BOOL FSFormImp::FixPageFields(CPDF_Dictionary* pPageDict)
{
	return TRUE;
}

FS_RESULT FSFormImp::ST_LoadFromPDF(FSPDFDoc* pLTPDFDoc)
{
	m_pSTInterForm = FX_NEW CFSCRT_STPDFInterForm(this);
	return m_pSTInterForm->InitPDFInterForm(((FSPDFDocImp*)pLTPDFDoc)->getPDFDocument());
}

FS_INT32 FSFormImp::getFieldCount(const char* filter) const
{
	CFX_WideString wsFilter;
	if (!FSCRT_Util_IsStringEmpty(filter))
		wsFilter = CFX_WideString::FromUTF8(filter, strlen(filter));
	return m_pSTInterForm->m_pInterForm->CountFields(wsFilter);
}
FSFormField* FSFormImp::getField(const char* filter, FS_INT32 index)
{
	if (index < 0)
		return NULL;

	FSFormFieldImp* pFieldImp = NULL;
	CFX_WideString wsFilter;
	if (filter && strlen(filter) > 0)
		wsFilter = CFX_WideString::FromUTF8(filter, strlen(filter));
	CPDF_FormField* pField = m_pSTInterForm->m_pInterForm->GetField(index, wsFilter);
	if (pField)
	{
		if (!m_pMapFields)
		{
			m_pMapFields = FX_NEW CFX_MapPtrToPtr();
			if (!m_pMapFields)
				return NULL;
		}
		else
		{
			if (m_pMapFields->Lookup((void*)pField, (void*&)pFieldImp))
				return (FSFormFieldImp*)pFieldImp;
		}

		pFieldImp = FX_NEW FSFormFieldImp(this, pField);
		m_pMapFields->SetAt((void*)pField, pFieldImp);
		return pFieldImp;
	}

	return pFieldImp;
}

FSFormControl* FSFormImp::getControlAtPos(FSPDFPage* page, FSPointF pos, FS_FLOAT tolerance)
{
	if (!page || !m_pSTInterForm || !m_pSTInterForm->m_pInterForm)
		return NULL;

	FSPDFPageImp* pPageImp = (FSPDFPageImp*)page;
	if (!pPageImp->initAnnotArray())
		return NULL;
	CFX_ArrayTemplate<void*>* pAnnotArray = pPageImp->getAnnotArray();
	FS_INT32 annotCount = pAnnotArray->GetSize();
	for (int i = annotCount - 1; i >= 0; i--) 
	{
		FSAnnot* pAnnot = (FSAnnot*)pAnnotArray->GetAt(i);
		if (pAnnot)
		{
			FSM_ANNOTTYPE type = pAnnot->getType();
			if (FSAnnot::e_annotWidget == type)
			{
				if(m_pFormFiller)
				{
					IFSPDF_AnnotHandler* annotHandler = m_pFormFiller->GetWidgetHandler();
					if (annotHandler)
					{
						FX_RECT rect;
						FS_RESULT ret = annotHandler->GetViewBBox(pPageImp, pAnnot, rect);
						if (FSCRT_ERRCODE_SUCCESS == ret)
						{
							rect.Normalize();	
							if (rect.Contains((int)pos.x, (int)pos.y))
								return (FSFormControl*)pAnnot;
						}
					}
					continue;
				}

				//Note: when tolerance is zero, the initial point might be excluded from the rect, thus the adding and minus.
				//      not sure which two edges to modify, might need to check CFX_FloatRect::Contains code, but this should be a good guess. -crlf
				CFX_FloatRect posRect(pos.x - tolerance, pos.y - tolerance - 1, pos.x + tolerance + 1, pos.y + tolerance);
				CPDF_FormControl* pControl = m_pSTInterForm->m_pInterForm->GetControlByDict((CPDF_Dictionary*)pAnnot->getDict());
				if (pControl)
				{
					CFX_FloatRect rect = pControl->GetRect();
					rect.Intersect(posRect);
					if(!rect.IsEmpty())
					{
						return (FSFormControl*)pAnnot;
					}
				}
			}
		}
	}
	return NULL;
}

FSFormControl* FSFormImp::getControlAtDevicePos(FSPDFPage* page, const FSMatrix* matrix, FSPointF pos, FS_FLOAT tolerance)
{
	if (matrix)
	{
		CFX_Matrix srcM(matrix->a, matrix->b, matrix->c, matrix->d, matrix->e, matrix->f);
		if (!srcM.IsInvertible())
			return NULL;
		CFX_Matrix destM;
		destM.SetReverse(srcM);
		FSMatrix dstMatrix;
		dstMatrix.a = destM.a;
		dstMatrix.b = destM.b;
		dstMatrix.c = destM.c;
		dstMatrix.d = destM.d;
		dstMatrix.e = destM.e;
		dstMatrix.f = destM.f;

		FSPointF dstPos = dstMatrix.transform(pos);
		return getControlAtPos(page, dstPos, tolerance);
	}
	else
		return getControlAtPos(page, pos, tolerance);
}

FSFormFiller* FSFormImp::getFormFiller() const
{
    return m_pFormFiller;
}

FS_BOOL FSFormImp::reset()
{
	if (!m_pSTInterForm || !m_pSTInterForm->m_pInterForm)
		return FALSE;
    if(m_pFormFiller)
        m_pFormFiller->SetFocusAnnot(NULL);
	FS_BOOL bReset = m_pSTInterForm->m_pInterForm->ResetForm(TRUE);
	if (bReset && m_pPDFDoc)
	{
		((FSPDFDocImp*)m_pPDFDoc)->setModified();
	}

	return bReset;
}

CFX_ByteString FSFormImp::MakeXMLTagName(const CFX_ByteString& sAliasName, const CFX_ByteString& sTrueName)
{
	return sAliasName + " " + "xfdf:original=\"" + sTrueName + "\"";
}

FS_BOOL FSFormImp::exportToXML(const char* path)
{
	if (!path || 0 == FXSYS_strlen(path) || !m_pSTInterForm || !m_pSTInterForm->m_pInterForm)
		return FALSE;
    if(m_pFormFiller)
        m_pFormFiller->SetFocusAnnot(NULL);
	CPDF_InterForm* pInterForm = m_pSTInterForm->m_pInterForm;

	int len = FXSYS_strlen(path);
	CFX_ByteString bsPath(path, len);
	if (FALSE == bsPath.Right(4).EqualNoCase(".xml"))
		return FALSE;
	FX_HFILE hFile = FX_File_Open(bsPath, FX_FILEMODE_Truncate);
	if (!hFile)
		return FALSE;

	CFX_ByteTextBuf sContents;
	sContents << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
	sContents << "<fields xmlns:xfdf=\"http://ns.adobe.com/xfdf-transition/\">\n";

	FX_DWORD nSize = pInterForm->CountFields();
	for (int i=0; i<(int)nSize; i++)
	{
		CPDF_FormField* pField = pInterForm->GetField(i);

		if (pField == NULL || pField->GetFieldType() == FSFormField::e_formFieldPushButton)
			continue;

		FX_DWORD dwFlags = pField->GetFieldFlags();
		if (dwFlags & 0x04) continue; //field with NoExport flag
		if ((dwFlags & 0x02) != 0 && pField->GetValue().IsEmpty())
			continue; //field with Required flag
		if ((dwFlags >> 20) & 1) continue; //file selection

		CFX_WideString sAlias;
		CFX_WideString sTrueName = pField->GetFullName();

		switch (pField->GetFieldType())
		{
		case FSFormField::e_formFieldCheckBox:
			sAlias.Format((FX_LPCWSTR)L"CheckBox%d", i);
			break;
		case FSFormField::e_formFieldRadioButton:
			sAlias.Format((FX_LPCWSTR)L"RadioButton%d", i);
			break;
		case FSFormField::e_formFieldTextField:
			sAlias.Format((FX_LPCWSTR)L"TextField%d", i);
			break;
		case FSFormField::e_formFieldListBox:
			sAlias.Format((FX_LPCWSTR)L"ListBox%d", i);
			break;
		case FSFormField::e_formFieldComboBox:
			sAlias.Format((FX_LPCWSTR)L"ComboBox%d", i);
			break;
		default:
			continue;
		}

		sContents << "<" << MakeXMLTagName(FX_UTF8Encode(sAlias), FX_UTF8Encode(sTrueName)) << ">" 
			<< FX_UTF8Encode(pField->GetValue()) 
			<< "</" << FX_UTF8Encode(sAlias) << ">\n";
	}
	sContents << "</fields>\n";

	size_t size = FX_File_Write(hFile, sContents.GetBuffer(), sContents.GetLength());
	FX_File_Close(hFile);

	return size == sContents.GetLength();
}

FS_BOOL FSFormImp::importFromXML(const char* path)
{
	if (!path || 0 == FXSYS_strlen(path) || !m_pSTInterForm || !m_pSTInterForm->m_pInterForm)
		return FALSE;
    if(m_pFormFiller)
        m_pFormFiller->SetFocusAnnot(NULL);
	CPDF_InterForm* pInterForm = m_pSTInterForm->m_pInterForm;

	int len = FXSYS_strlen(path);
	CFX_ByteString bsPath(path, len);
	if (FALSE == bsPath.Right(4).EqualNoCase(".xml"))
		return FALSE;
	FX_HFILE hFile = FX_File_Open(bsPath, FX_FILEMODE_ReadOnly);
	if (!hFile)
		return FALSE;

	FX_FILESIZE size = FX_File_GetSize(hFile);
	CFX_ByteString sFileContent;
	FX_BOOL bRet = FX_File_Read(hFile, (void*)sFileContent.GetBuffer((FX_STRSIZE)size), size);
	sFileContent.ReleaseBuffer();
	if(!bRet)
		return FALSE;
	if (sFileContent.IsEmpty())
		return TRUE;

	CXML_Element* pRoot = CXML_Element::Parse((FX_LPCSTR)sFileContent, sFileContent.GetLength());
	if(!pRoot)
		return FALSE;
	if (!pRoot->GetTagName().EqualNoCase("fields"))
	{
		delete pRoot;
		return FALSE;
	}

	FX_INT32 sz = pRoot->CountChildren();
	for (FX_INT32 i = 0;  i < sz; i++)
	{
		CXML_Element* pFieldNode = pRoot->GetElement(i);
		if (pFieldNode == NULL)
			continue;

		CFX_WideString sFieldName = CFX_WideString::FromLocal(pFieldNode->GetTagName());
		CFX_WideString sTrueName = pFieldNode->GetAttrValue("xfdf:original");

		if (!sTrueName.IsEmpty())
			sFieldName = sTrueName;

		if (CPDF_FormField* pFormField = pInterForm->GetField(0 , sFieldName))
		{
			CFX_WideString sContent = pFieldNode->GetContent(0);
			pFormField->SetValue(pFieldNode->GetContent(0), TRUE);
			FS_INT32 sc = pFormField->CountControls();
			for (FS_INT32 j =0; j < sc; j++)
			{
				CPDF_FormControl* pFormCtrl = pFormField->GetControl(j);
				FPDF_GenerateAP(pInterForm->GetDocument(), pFormCtrl->GetWidget());
			}
		}
	}

	delete pRoot;
	pRoot = NULL;
	sFileContent.ReleaseBuffer();
	if (m_pPDFDoc)
	{
		((FSPDFDocImp*)m_pPDFDoc)->setModified();
	}
	return TRUE;
}

void FSFormImp::EnableCalculate(FX_BOOL bEnable)
{
	//FSCRT_LOCKOBJECT(lock, &m_lock)
	m_bCalculate = bEnable;
}
FX_BOOL FSFormImp::IsCalculateEnabled()
{
	return m_bCalculate;
}
CFSCRT_STPDFInterForm*  FSFormImp::ST_GetSTInterForm()
{
	return m_pSTInterForm;
}
FSFormFillerImp* FSFormImp::GetFormFiller()
{
	return m_pFormFiller;
}

void FSFormImp::SetFormFiller(FSFormFillerImp* pFormFiller)
{
	m_pFormFiller = pFormFiller;
}

FSPDFDoc* FSFormImp::GetPDFDoc()
{
	return m_pPDFDoc;
}

void FSFormImp::SetPDFDoc(FSPDFDoc* pDoc)
{
	m_pPDFDoc = pDoc;
}

CFX_MapPtrToPtr* FSFormImp::GetMapFields()
{
	return m_pMapFields;
}

void FSFormImp::SetMapFields(CFX_MapPtrToPtr* pMapFields)
{
	m_pMapFields = pMapFields;
}

////////////////////////////////////////////////
// Class FSFormFieldImp
FSFormFieldImp::FSFormFieldImp(FSFormImp* pFormImp, CPDF_FormField* pFormField)
	: m_pFormField(pFormField)
	, m_pFormImp(pFormImp)
{
}

FSFormFieldImp::~FSFormFieldImp()
{
}

FSM_FORMFIELDTYPE FSFormFieldImp::getType() const
{
	if (!m_pFormField)
		return FSFormField::e_formFieldUnknownType;
	FS_INT32 type = m_pFormField->GetFieldType();
	if (FSFormField::e_formFieldUnknownType > type || FSFormField::e_formFieldSignature < type)
		type = FSFormField::e_formFieldUnknownType;
	return (FSM_FORMFIELDTYPE)type;
}

FSM_FORMFIELDFLAGS FSFormFieldImp::getFlags() const
{
	if (!m_pFormField)
		return (FSM_FORMFIELDFLAGS)0;

	return (FSM_FORMFIELDFLAGS)m_pFormField->GetFlags();
}

FSString FSFormFieldImp::getName() const
{
	if (!m_pFormField)
		return FSString();
	CFX_WideString wsStr = m_pFormField->GetFullName();
	CFX_ByteString bsByteStringUTF8 = FSCRT_UTF8_FromWideString(wsStr);
	FSString fsRetString(bsByteStringUTF8.GetBuffer(bsByteStringUTF8.GetLength()));
	bsByteStringUTF8.ReleaseBuffer();
	return fsRetString;
}

FSString FSFormFieldImp::getDefaultValue() const
{
	if (!m_pFormField)
		return FSString();
	CFX_WideString wsStr = m_pFormField->GetDefaultValue();
	CFX_ByteString bsByteStringUTF8 = FSCRT_UTF8_FromWideString(wsStr);
	FSString fsRetString(bsByteStringUTF8.GetBuffer(bsByteStringUTF8.GetLength()));
	bsByteStringUTF8.ReleaseBuffer();
	return fsRetString;
}

FSString FSFormFieldImp::getValue() const
{
	if (!m_pFormField)
		return FSString();
	CFX_WideString wsStr = m_pFormField->GetValue();
	CFX_ByteString bsByteStringUTF8 = FSCRT_UTF8_FromWideString(wsStr);
	FSString fsRetString(bsByteStringUTF8.GetBuffer(bsByteStringUTF8.GetLength()));
	bsByteStringUTF8.ReleaseBuffer();
	return fsRetString;
}

FS_INT32 FSFormFieldImp::getControlCount(FSPDFPage* page) const
{
	if (!page || !m_pFormField || !m_pFormField->GetInterForm())
		return 0;

	FS_INT32 retCount = 0;
	FSPDFPageImp* pPageImp = (FSPDFPageImp*)page;
	int count = pPageImp->getAnnotCount();
	for (int i = 0; i < count; i++)
	{
		FSAnnot* annot = pPageImp->getAnnot(i);
		if(!annot || annot->getType() != FSAnnot::e_annotWidget)
			continue;
		FSFormControl* pControl = (FSFormControl*)annot;
		FSFormField* pFormField = pControl->getField();
		CPDF_FormControl* pPDFControl = NULL;
		if(pFormField && pFormField->getType() == e_formFieldSignature)
#if defined(ENABLE_SIGNATURE)
			pPDFControl = ((FSSignatureImp*)(FSSignature*)pControl)->getFormControlImp()->GetCPDFFormControl();
#else
			pPDFControl = NULL;
#endif
		else
			pPDFControl = ((FSFormControlImp*)pControl)->GetCPDFFormControl();

		if(pPDFControl && pPDFControl->GetField() == m_pFormField)
			retCount++;
	}
	return retCount;
}

FSFormControl* FSFormFieldImp::getControl(FSPDFPage* page, FS_INT32 index)
{
	if (0 > index || !page || !m_pFormField || !m_pFormField->GetInterForm())
		return NULL;

	FS_INT32 retCount = 0;
	FSPDFPageImp* pPageImp = (FSPDFPageImp*)page;
	int count = pPageImp->getAnnotCount();
	for (int i = 0; i < count; i++)
	{
		FSAnnot* annot = pPageImp->getAnnot(i);
		if(!annot || annot->getType() != FSAnnot::e_annotWidget)
			continue;
		FSFormControl* pControl = (FSFormControl*)annot;
		FSFormField* pFormField = pControl->getField();
		CPDF_FormControl* pPDFControl = NULL;
		if(pFormField && pFormField->getType() == e_formFieldSignature)
#if defined(ENABLE_SIGNATURE)
			pPDFControl = ((FSSignatureImp*)(FSSignature*)pControl)->getFormControlImp()->GetCPDFFormControl();
#else
			pPDFControl = NULL;
#endif
		else
			pPDFControl = ((FSFormControlImp*)pControl)->GetCPDFFormControl();

		if(pPDFControl && pPDFControl->GetField() == m_pFormField)
		{
			if(index == retCount)
				return pControl;
			retCount++;
		}
	}
	return NULL;
}

////////////////////////////////////////////////
// Class FSFormControlImp
FSFormControlImp::FSFormControlImp(CPDF_Dictionary* pAnnotDict, FSPDFPage* page)
	:m_pFormControl(NULL)
	, m_pFormFieldImp(NULL)
	, m_nAppAge(0)
	, m_nValueAge(0)
{
	init(pAnnotDict, page);
}

FSFormControlImp::FSFormControlImp()
	:m_pFormControl(NULL)
	, m_pFormFieldImp(NULL)
	, m_nAppAge(0)
	, m_nValueAge(0)
{

}

void FSFormControlImp::init(CPDF_Dictionary* pAnnotDict, FSPDFPage* page)
{
	FSPDFAnnotImp::init(pAnnotDict, page);

	FSPDFDoc* pDoc = page->getDocument();
	if(pDoc)
	{
		FSFormImp* pForm = (FSFormImp*)((FSPDFDocImp*)pDoc)->getForm();
		if(pForm && pForm->ST_GetSTInterForm())
			m_pFormControl = pForm->ST_GetSTInterForm()->m_pInterForm->GetControlByDict(pAnnotDict);
	}
}


FSFormControlImp::~FSFormControlImp()
{
}

FSFormField* FSFormControlImp::getField()
{
	if (!m_pFormFieldImp)
	{
        if(!m_pFormControl) return NULL;
		FSPDFDoc* pDoc = getPage()->getDocument();
		if (!pDoc)
			return NULL;
		FSFormImp* pForm = (FSFormImp*)pDoc->getForm();
		if (!pForm)
			return NULL;
		CFX_MapPtrToPtr* pMapFields = pForm->GetMapFields();
		if (!pMapFields)
		{
			pMapFields = FX_NEW CFX_MapPtrToPtr();
			if (!pMapFields)
				return NULL;
			pForm->SetMapFields(pMapFields);
		}

		FSFormFieldImp* pFieldImp = NULL;
		CPDF_FormField* pFormField = m_pFormControl->GetField();
		if (pMapFields->Lookup((void*)pFormField, (void*&)pFieldImp))
			return pFieldImp;

		pFieldImp = FX_NEW FSFormFieldImp(pForm, pFormField);
		pMapFields->SetAt((void*)pFormField, pFieldImp);
		return pFieldImp;
	}

	return m_pFormFieldImp;
}
FS_BOOL FSFormControlImp::move(FSRectF rect)
{
	return FSPDFAnnotImp::moveImp((FSFormControl*)this,rect, TRUE);
}


FS_BOOL FSFormControlImp::resetAppearanceStream()
{
	FSPDFPage* pPage = getPage();
	if(!pPage) return FALSE;
	FSPDFDoc* pDoc = pPage->getDocument();
	if(!pDoc) return FALSE;
	FSFormImp* pForm = (FSFormImp*)pDoc->getForm();
	if(!pForm) return FALSE;
	CFSCRT_STPDFInterForm* pSTInterForm = pForm->ST_GetSTInterForm();
	if(!pSTInterForm) return FALSE;

	FX_BOOL isFormated;
	CFX_WideString cwValue = pSTInterForm->OnFormat(m_pFormControl?m_pFormControl->GetField():NULL, 0, isFormated);
	ResetAppearance(cwValue);
	return TRUE;
}

void FSFormControlImp::loadFormControl(FSForm* form)
{
	if(m_pFormControl)
	{
		CFSCRT_STPDFInterForm* pSTForm = ((FSFormImp*)form)->ST_GetSTInterForm();
		if(pSTForm->m_pInterForm->NeedConstructAP())
		{
			FX_BOOL bFormat = FALSE;
			CFX_WideString sValue = pSTForm->OnFormat(m_pFormControl->GetField(), 0, bFormat);
			if(bFormat)
			{
				ResetAppearance((FX_LPCWSTR)sValue);
			}
			else
				ResetAppearance(NULL);
		}
	}
}

CPDF_FormControl* FSFormControlImp::GetCPDFFormControl()
{
	return m_pFormControl;
}
FSPDFPage* FSFormControlImp::GetPageView()
{
	return getPage();
}
FSPDFPage* FSFormControlImp::GetPage()
{
	return getPage();
}
void FSFormControlImp::GetPageIndex(FX_INT32* nPageIndex)
{
	if (!nPageIndex || !getPage())
		return;

    *nPageIndex = getPage()->getIndex();
}
CPDF_Page* FSFormControlImp::GetPDFPage()
{
    return ((FSPDFPageImp*)getPage())->getPage();
}

FX_BOOL	FSFormControlImp::OnAAction(CPDF_AAction::AActionType type, FSPDF_FieldActionParam& data, FSPDFPage* pPageView)
{
	CPDF_Action action = GetAAction(type);

	if (action && action.GetType() != CPDF_Action::Unknown)
	{
// 		CPDFSDK_Document* pDocument = pPageView->GetSDKDocument();
// 		ASSERT(pDocument != NULL);
// 
// 		CPDFDoc_Environment* pEnv = pDocument->GetEnv();
// 		ASSERT(pEnv != NULL);
// 
// 		CPDFSDK_ActionHandler* pActionHandler = pEnv->GetActionHander();/*(CPDFSDK_ActionHandler*)pApp->GetActionHandler();*/
// 		ASSERT(pActionHandler != NULL);

		FSPDFDoc* pDoc = pPageView->getDocument();
		if(!pDoc) return FALSE;
		FSFormImp* pForm = (FSFormImp*)pDoc->getForm();
		if(!pForm) return FALSE;
		FSFormFillerImp* pFormFiller = (FSFormFillerImp*)pForm->GetFormFiller();

		CFSCRT_STPDFActionHandler* pActionHandler = pFormFiller->GetActionHandler();
		if(pActionHandler)
			return pActionHandler->DoAction_Field(action, type, pDoc, m_pFormControl->GetField(), data);
	}

	return FALSE;
}

CPDF_Action	FSFormControlImp::GetAAction(CPDF_AAction::AActionType eAAT)
{
	switch (eAAT)
	{
	case CPDF_AAction::CursorEnter:
	case CPDF_AAction::CursorExit:
	case CPDF_AAction::ButtonDown:
	case CPDF_AAction::ButtonUp:
	case CPDF_AAction::GetFocus:
	case CPDF_AAction::LoseFocus:
	case CPDF_AAction::PageOpen:
	case CPDF_AAction::PageClose:
	case CPDF_AAction::PageVisible:
	case CPDF_AAction::PageInvisible:
		return  FSPDFAnnotImp::GetAAction(eAAT);
	case CPDF_AAction::KeyStroke:
	case CPDF_AAction::Format:
	case CPDF_AAction::Validate:
	case CPDF_AAction::Calculate:
		{
			CPDF_FormField* pField = m_pFormControl->GetField();
			ASSERT(pField != NULL);

			if (CPDF_AAction aa = pField->GetAdditionalAction())
				return aa.GetAction(eAAT);
			else 
				return FSPDFAnnotImp::GetAAction(eAAT);
		}
	}

	return NULL;
}

FS_RESULT	FSFormControlImp::ST_GetFlags(FS_INT32* flags)
{
	if (!flags)
		return FSCRT_ERRCODE_ERROR;
	*flags = getFlags();
	return FSCRT_ERRCODE_SUCCESS;
}

FS_RESULT	FSFormControlImp::ST_SetFlags(FS_INT32 flags)
{
	setFlags(flags);
	return FSCRT_ERRCODE_SUCCESS;
}

FS_RESULT	FSFormControlImp::ST_GetRect(CPDF_Rect& rect)
{
	FSRectF rectF = getRect();
	rect.left = rectF.left;
	rect.bottom = rectF.bottom;
	rect.right = rectF.right;
	rect.top = rectF.top;
	return FSCRT_ERRCODE_SUCCESS;
}

FS_RESULT	FSFormControlImp::ST_SetRect(CPDF_Rect& rect)
{
	FSRectF rectF;
	rectF.left = rect.left;
	rectF.bottom = rect.bottom;
	rectF.right = rect.right;
	rectF.top = rect.top;
	setRect("Rect", rectF);
	return FSCRT_ERRCODE_SUCCESS;
}

FS_BOOL		FSFormControlImp::ST_IsVisible()
{
	FS_DWORD flags = getFlags();
	return !((flags & ANNOTFLAG_HIDDEN) || (flags & ANNOTFLAG_NOVIEW));
}


FS_RESULT	FSFormControlImp::IsChecked(FS_BOOL* checked, FS_BOOL isDefault)
{
	if (!checked)
		return FSCRT_ERRCODE_ERROR;

	if(isDefault)
		*checked = m_pFormControl->IsDefaultChecked();
	else
		*checked = m_pFormControl->IsChecked();
	return FSCRT_ERRCODE_SUCCESS;
}

FS_RESULT	FSFormControlImp::SetChecked(FS_BOOL checked, FS_BOOL isDefault)
{
	CPDF_FormField::Type type = m_pFormControl->GetField()->GetType();
	if(type != CPDF_FormField::CheckBox && type != CPDF_FormField::RadioButton)
		return FSCRT_ERRCODE_ERROR;

	CPDF_FormField* pField = m_pFormControl->GetField();
	if(pField == NULL) return FSCRT_ERRCODE_ERROR;

	FX_INT32 index = pField->GetControlIndex(m_pFormControl);
	if(isDefault)
	{
		if (m_pFormControl->IsDefaultChecked() == checked)
			return FSCRT_ERRCODE_SUCCESS;
		if(pField->DefaultCheckControl(index, checked))
			return FSCRT_ERRCODE_SUCCESS;
	}
	else
	{
		if(pField->CheckControl(index, checked, TRUE))
			return FSCRT_ERRCODE_SUCCESS;
	}
	return FSCRT_ERRCODE_ERROR;
}

FS_RESULT	FSFormControlImp::GetMKRotation(int* rotation)
{
	if (!rotation)
		return FSCRT_ERRCODE_ERROR;
	*rotation = 0;

	CPDF_Dictionary* pAnnotDict = getPDFDict();
	if (!pAnnotDict)
		return FSCRT_ERRCODE_ERROR;
	CPDF_Dictionary* pMKDict = pAnnotDict->GetDict(FX_BSTRC("MK"));
	if (!pMKDict)
		return FSCRT_ERRCODE_ERROR;
	*rotation = pMKDict->GetInteger(FX_BSTRC("R"));
	return FSCRT_ERRCODE_SUCCESS;
}


void FSFormControlImp::ResetAppearance(CFX_WideString value)
{
	IFSPDF_Widget::ResetAppearance(m_pFormControl, value);
	clearCachedAppearance();
	return;
}

void FSFormControlImp::ResetAppearance(FX_LPCWSTR value)
{
	IFSPDF_Widget::ResetAppearance(m_pFormControl, value);
	clearCachedAppearance();
	return ;
}

FX_BOOL	FSFormControlImp::drawAppearance(CFX_RenderDevice* pDevice, const CFX_Matrix* pUser2Device, FX_BSTR bsMode, CPDF_RenderOptions* pOptions)
{
	return FSPDFAnnotImp::drawAppearance(pDevice, pUser2Device, bsMode, pOptions);
}

FX_BOOL	FSFormControlImp::drawInContext(const CPDF_Page* pPage, const CPDF_RenderContext* pContext, const CFX_AffineMatrix* pUser2Device, CPDF_Annot::AppearanceMode mode)
{
	return FSPDFAnnotImp::drawInContext(pPage, pContext, pUser2Device, mode);
}

void FSFormControlImp::clearCachedAppearance()
{
	return FSPDFAnnotImp::clearCachedAppearance();
}

CPDF_Form*	FSFormControlImp::getAPForm(FX_BSTR mode)
{
	return FSPDFAnnotImp::getAPForm(mode);
}

