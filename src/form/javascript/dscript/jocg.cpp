#if 0
#include "js_fxlib.h"
#if defined(_FSDK_ALL_) || defined(_FSDK_JAVASCRIPT_)

/* ---------------------- OCG ---------------------- */

BEGIN_JS_STATIC_CONST(CJS_OCG)
END_JS_STATIC_CONST()

BEGIN_JS_STATIC_PROP(CJS_OCG)
	JS_STATIC_PROP_ENTRY(constants)
	JS_STATIC_PROP_ENTRY(name)
	JS_STATIC_PROP_ENTRY(state)
END_JS_STATIC_PROP()

BEGIN_JS_STATIC_METHOD(CJS_OCG) 
	JS_STATIC_METHOD_ENTRY(getIntent, 0)
	JS_STATIC_METHOD_ENTRY(setAction, 0)
END_JS_STATIC_METHOD()

IMPLEMENT_JS_CLASS(CJS_OCG,OCG)

IPDF_OCNotify* OCG::s_pNotify = NULL;
	
void CJS_OCGNotify::OnOCGStateChanged(CPDF_Document *pDoc, CPDF_OCGroup ocg, FX_BOOL bVisible)
{
	if (!m_pOcg)return;
	m_pOcg->RunJavaScript(m_pOcg->GetJSRuntime(), m_pOcg->GetJSScript());
}

OCG::OCG(CJS_Object * pJSObject) : CJS_EmbedObj(pJSObject)
{
	m_pDocView = NULL;
	m_pOCGDict = NULL;
	m_swJavaScript = L"";
	m_pRuntime = NULL;
}

OCG::~OCG()
{
	CPDF_OCProperties::RemoveOCNotify((IPDF_OCNotify*)s_pNotify);
	
	if (s_pNotify)
	{
		delete s_pNotify;
		s_pNotify = NULL;
	}
}

void OCG::SetOCGInfo(FSCRT_DOCUMENT pDocument, CPDFSDK_PageView* pDocView, CPDF_Dictionary* pDict)
{
	m_pDocument = pDocument;
	m_pDocView = pDocView;
	m_pOCGDict = pDict;
}

FX_BOOL OCG::constants(OBJ_PROP_PARAMS)
{
	return TRUE;
}

FX_BOOL OCG::state(OBJ_PROP_PARAMS)
{
	if (!m_pOCGDict)return TRUE;
#if 0
	CPDF_OCContext* pContext = m_pDocument->GetOCContext();
	if (!pContext)return FALSE;

	FX_BOOL state = pContext->CheckOCGVisible((CPDF_Dictionary*)m_pOCGDict);
	if (vp.IsGetting())
	{
		vp << state;
		return TRUE;
	}
	else
	{
		FX_BOOL bSetState;
		vp >> bSetState;
		CPDF_OCContext* pOCGContext = m_pDocument->GetOCContext();
		if (!pOCGContext)return FALSE;

 		if (state != bSetState)
 		{
			pOCGContext->SetOCGState(m_pOCGDict, bSetState);
			CFX_PtrArray pages;
			CPDF_Document* pPDFDocument = m_pDocument->GetDocument();
			CPDF_OCProperties* pOCP = m_pDocument->GetOCProperties();

/*			int iVisiblePages = m_pDocView->CountVisiblePages();

			for(int i = 0; i < iVisiblePages; i++)
			{	
				CReader_PageView* pPageView = m_pDocView->GetVisiblePage(i);							
				if (pPageView)
				{
					int iPageIndex = pPageView->GetReaderPage()->GetPageIndex();
					CPDF_Dictionary* pPageDict = pPDFDocument->GetPage(iPageIndex);
					if (pOCP->IsOCGInPage(pPageDict, m_pOCGDict))
					{
						if (!m_pDocView)return FALSE;

						m_pDocView->SetRenderDataChange();
						CRect pageRect = pPageView->GetPageRect();
						::InvalidateRect(pPageView->GetPageViewWnd(), &pageRect, TRUE);
					}
				}
				
			}*/
		}
		return TRUE;
	}
#endif
#if 1
	return TRUE;
#endif

}

FX_BOOL OCG::name(OBJ_PROP_PARAMS)
{
	if (!m_pOCGDict)return FALSE;

	if (vp.IsGetting())
	{
		CFX_WideString cwName;
		CPDF_OCGroup ocg = (CPDF_Dictionary*)m_pOCGDict;
		ocg.GetName(cwName);
		vp << cwName;
		return TRUE;
	}
	else
	{
		CFX_WideString cwName;
		vp >> cwName;
		CPDF_OCGroup ocg = m_pOCGDict;
		ocg.SetName(cwName);
		return TRUE;
	}
}

FX_BOOL OCG::getIntent(OBJ_METHOD_PARAMS)
{
	return TRUE;
}

FX_BOOL OCG::setAction(OBJ_METHOD_PARAMS)
{
	if (params.size() != 1)return FALSE;

	if (params[0].GetType() != VT_string)return FALSE;

	m_swJavaScript = params[0];
#if 0
	CPDF_OCProperties* pOCP = m_pDocument->GetOCProperties();
#endif
	CFXJS_Context * pContext = (CFXJS_Context *)cc;
	if (!pContext)return FALSE;
	
	m_pRuntime = pContext->GetJSRuntime();
	s_pNotify = new CJS_OCGNotify(this);
	CPDF_OCProperties::AddOCNotify((IPDF_OCNotify*)s_pNotify);
//	pOCP->SetOCNotify((IPDF_OCNotify*)s_pNotify);
	return TRUE;
}

void OCG::RunJavaScript(CFXJS_Runtime* pRuntime, CFX_WideString swJavaScript)
{
	if (pRuntime)
	{
		IJS_Context * pContext = pRuntime->NewContext();
		if (pContext)
		{
			pContext->OnExternal_Exec();
			CFX_WideString wtInfo;
			pContext->RunScript(swJavaScript,wtInfo);
			pRuntime->ReleaseContext(pContext);
		}		
	}
}

#endif //defined(_FSDK_ALL_) || defined(_FSDK_JAVASCRIPT_)
#endif