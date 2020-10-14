#include "fs_pdfformImp.h"
#include "fs_formfillerImp.h"
#include "../inc/fs_pdfpageImp.h"

class FSPDFDoc;
class FSPDFPage;
class FSForm;
class IFSPDF_AnnotHandler;
class CFSCRT_STPDFActionHandler;
class CFXJS_RuntimeFactory;
class IJS_Runtime;
class CFSCRT_JS_ActionHandler;

static CFXJS_RuntimeFactory* s_pJSRuntimeFactory = NULL;

CFXJS_RuntimeFactory* FSCRT_GetJSRuntimeFactory()
{	
#if defined(ENABLE_JSENGINE)
	if(!s_pJSRuntimeFactory)
		s_pJSRuntimeFactory = FX_NEW CFXJS_RuntimeFactory();
	return s_pJSRuntimeFactory;
#else
	return NULL;
#endif
}

CFSCRT_LTWindowslessHanler::CFSCRT_LTWindowslessHanler(FSFormFillerImp*)
{
}

FS_RESULT CFSCRT_LTWindowslessHanler::ReleaseHandler()
{
	return 0;
}

FS_RESULT CFSCRT_LTWindowslessHanler::OutputSelectedRect(FS_INT32 pageIndex, FSRectF* pdfRect)
{
	return 0;
}

FS_RESULT CFSCRT_LTWindowslessHanler::SetCursor(FS_INT32 nCursorType)
{
	return 0;
}

FS_RESULT CFSCRT_LTWindowslessHanler::SetTimer(FS_INT32 uElapse, FS_CALLBACK_TIMER lpTimerFunc, FS_INT32* timeID)
{
	return 0;
}

FS_RESULT CFSCRT_LTWindowslessHanler::KillTimer(FS_INT32 timerID)
{
	return 0;
}

FS_RESULT   GotFocusOnControl(FSFormControl* control, const char* fieldValue)
{
	return 0;
}

FS_RESULT   LostFocusOnControl(FSFormControl* control, const char* fieldValue)
{
	return 0;
}

FSFormFiller* FSFormFiller::create(FSForm* form, FSFormFillerAssist* assist)
{
	return FX_NEW FSFormFillerImp((FSFormImp*)form, assist, NULL);
}

FSFormFillerImp::FSFormFillerImp(FSFormImp* pForm, FSFormFillerAssist* assist, CFSCRT_JS_ActionHandler* pJSActionHandle)
{
	m_pForm = pForm;
	m_isShowHighlight = TRUE;
	m_hightlightColor = 0x80C8D2FF;

	m_pWidgetHandler = NULL;
	m_renderer = NULL;

	m_pCaptureAnnot = NULL;
	m_pFocusAnnot = NULL;
	m_pHoverAnnot = NULL;

	m_pFormFillerAssist = assist;
	if(pForm)
		pForm->SetFormFiller(this);
	
}
FSFormFillerImp::~FSFormFillerImp()
{
	if(m_pWidgetHandler)
		m_pWidgetHandler->Release();
#if defined(ENABLE_JSENGINE)
	if(s_pJSRuntimeFactory)
		s_pJSRuntimeFactory->Release();
#endif
	s_pJSRuntimeFactory = NULL;
}

void FSFormFillerImp::release()
{
	delete this;
}

void FSFormFillerImp::render(FSPDFPage* page, const FSMatrix& matrix, FSRenderer* renderer)
{
	if(!page || !renderer) return;
	m_renderer = renderer;

#if 0
	FS_INT32 count = page->getAnnotCount();
	for(int i=0; i<count; i++)
	{
		FSAnnot* pAnnot = page->getAnnot(i);
		if(!pAnnot || pAnnot->getType() != FSAnnot::e_annotWidget) continue;

		//Exclude the focused annotation to draw at last!
		if(m_pFocusAnnot == pAnnot)
			continue;
		if(pAnnotHandler)
		{
			if(!isLocked)
			{
				isLocked = TRUE;
				FSCRT_LOCK(m_lock);
			}
			FSRectI rcWindow = {0, 0,0,0};
			CFX_Matrix m(matrix.a, matrix.b, matrix.c, matrix.d, matrix.e, matrix.f);
			pAnnotHandler->OnDraw(page, renderer, pAnnot, &m, rcWindow,
				pAnnot->getFlags());
		}
	}
#endif

    
	//Draw focused annotation at last!
    FSCRT_LOCKOBJECT(lock, &m_lock);
	if(m_pFocusAnnot && m_pFocusAnnot->getPage() == page)
	{
		IFSPDF_AnnotHandler* pAnnotHandler = GetWidgetHandler();
		if(pAnnotHandler)
		{
			FSRectI rcWindow = {0, 0,0,0};
			CFX_Matrix m(matrix.a, matrix.b, matrix.c, matrix.d, matrix.e, matrix.f);
			pAnnotHandler->OnDraw(page, renderer, m_pFocusAnnot, &m, rcWindow,
				m_pFocusAnnot->getFlags());
		}
	}

}

FS_BOOL	FSFormFillerImp::onLButtonDown(FSPDFPage* page, FSPointF* point, FS_DWORD flags)
{
	if(page && point)
	{
		FSCRT_LOCKOBJECT(lock, &m_lock);
		CFX_PointF pointF;
		pointF.x = point->x;
		pointF.y = point->y;
		if(0 == ((FSPDFPageImp*)page)->OnLButtonDown(flags, pointF))
			return TRUE;
	}

	return FALSE;
}
FS_BOOL	FSFormFillerImp::onLButtonUp(FSPDFPage* page, FSPointF* point, FS_DWORD flags)
{
	if(page && point)
	{
		FSCRT_LOCKOBJECT(lock, &m_lock);
		CFX_PointF pointF;
		pointF.x = point->x;
		pointF.y = point->y;
		if(0 == ((FSPDFPageImp*)page)->OnLButtonUp(flags, pointF))
			return TRUE;
	}

	return FALSE;
}
FS_BOOL	FSFormFillerImp::onLButtonDoubleClick(FSPDFPage* page, FSPointF* point, FS_DWORD flags)
{
	if(page && point)
	{
		FSCRT_LOCKOBJECT(lock, &m_lock);
		CFX_PointF pointF;
		pointF.x = point->x;
		pointF.y = point->y;
		if(0 == ((FSPDFPageImp*)page)->OnLButtonDbClick(flags, pointF))
			return TRUE;
	}

	return FALSE;
}
FS_BOOL	FSFormFillerImp::onMouseMove(FSPDFPage* page, FSPointF* point, FS_DWORD flags)
{
	if(page && point)
	{
		FSCRT_LOCKOBJECT(lock, &m_lock);
		CFX_PointF pointF;
		pointF.x = point->x;
		pointF.y = point->y;
		if(0 == ((FSPDFPageImp*)page)->OnMouseMove(flags, pointF))
			return TRUE;
	}

	return FALSE;
}
FS_BOOL	FSFormFillerImp::onRButtonDown(FSPDFPage* page, FSPointF* point, FS_DWORD flags)
{
	return FALSE;
}
FS_BOOL	FSFormFillerImp::onRButtonUp(FSPDFPage* page, FSPointF* point, FS_DWORD flags)
{
	return FALSE;
}
FS_BOOL	FSFormFillerImp::onRButtonDoubleClick(FSPDFPage* page, FSPointF* point, FS_DWORD flags)
{
	return FALSE;
}
FS_BOOL	FSFormFillerImp::onKeyDown(FS_DWORD keyCode, FS_DWORD flags)
{
	return TRUE;
}
FS_BOOL	FSFormFillerImp::onKeyUp(FS_DWORD keyCode, FS_DWORD flags)
{
	return TRUE;
}
FS_BOOL	FSFormFillerImp::onChar(FS_DWORD charCode, FS_DWORD flags)
{
	FSCRT_LOCKOBJECT(lock, &m_lock);
	FSAnnot* pAnnot = GetFocusAnnot();
	if(!pAnnot) return FALSE;

	IFSPDF_AnnotHandler* pHandler = GetWidgetHandler();
	if (!pHandler) return FALSE;

	return pHandler->OnChar(pAnnot, charCode, flags) == FSCRT_ERRCODE_SUCCESS;
}

FS_RESULT FSFormFillerImp::GetForm(FSForm** form)
{
	if(!m_pForm || !form) return -1;
	*form = m_pForm;
	return 0;
}

IJS_Runtime* FSFormFillerImp::GetJSRuntime()
{
#if defined(ENABLE_JSENGINE)
	return FSCRT_GetJSRuntimeFactory()->GetJSRuntime(NULL);
#else
	return NULL;
#endif
}

IFSPDF_AnnotHandler* FSFormFillerImp::GetWidgetHandler()
{
	if(!m_pWidgetHandler) 
	{
		m_pWidgetHandler = FX_NEW CFSPDF_WidgetAnnotHandler;
		if(!m_pWidgetHandler) return NULL;
		m_pWidgetHandler->SetFormFiller(this);
	}
	return m_pWidgetHandler;
}

CFSCRT_STPDFActionHandler* FSFormFillerImp::GetActionHandler()
{
	if(m_pForm)
	{
		FSPDFDocImp* docImp = (FSPDFDocImp*)m_pForm->GetPDFDoc();
		if(docImp)
			return docImp->GetActionHandler();
	}
	return NULL;
}

FS_RESULT FSFormFillerImp::ST_GetWidgetFlag(FSAnnot* pAnnot, FS_INT32* flag)
{
	CPDF_Dictionary* pAnnotDict = (CPDF_Dictionary*)pAnnot->getDict();
	if(!pAnnotDict || FX_BSTRC("Widget") != pAnnotDict->GetString("Subtype"))
		return FSCRT_ERRCODE_ERROR;

	*flag = pAnnotDict->GetInteger("Ff");

	return FSCRT_ERRCODE_SUCCESS;
}

FS_RESULT FSFormFillerImp::ST_IsWidgetReadOnly(FSAnnot* pAnnot, FS_BOOL* bReadOnly)
{
	FS_INT32 nFlag = 0;
	FS_RESULT ret = ST_GetWidgetFlag(pAnnot, &nFlag);
	if (FSCRT_ERRCODE_SUCCESS == ret && nFlag & FORMFIELD_READONLY)
		*bReadOnly = TRUE;
	else
		*bReadOnly = FALSE;

	return ret;
}

FS_RESULT	FSFormFillerImp::SetFocusAnnot(FSAnnot* pFocusAnnot)
{
	FS_RESULT ret = FSCRT_ERRCODE_SUCCESS;
	IFSPDF_AnnotHandler* pHandler = NULL;

	if (pFocusAnnot == m_pFocusAnnot) return FSCRT_ERRCODE_SUCCESS;

	IFSPDF_AnnotHandler* pWidgetHandler = GetWidgetHandler();

	if (m_pFocusAnnot) {
		if (pWidgetHandler) {
			ret = pWidgetHandler->OnKillFocus(m_pFocusAnnot);
			if (ret != FSCRT_ERRCODE_SUCCESS && ret != FSCRT_ERRCODE_UNSUPPORTED) return ret;
		}
	}
	m_pFocusAnnot = pFocusAnnot;
	if(m_pFocusAnnot)
	{
		if (pWidgetHandler) {
			ret = pWidgetHandler->OnSetFocus(m_pFocusAnnot);
			if (ret != FSCRT_ERRCODE_SUCCESS && ret != FSCRT_ERRCODE_UNSUPPORTED) return ret;
		}
	}

	return FSCRT_ERRCODE_SUCCESS;
}
