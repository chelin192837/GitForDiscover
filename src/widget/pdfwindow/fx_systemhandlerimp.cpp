#include "../../inc/fpdf_define.h"

#include "include/pdfwindow/pdfwindow.h"
#include "include/pdfwindow/pwl_wnd.h"
#include "include/pdfwindow/pwl_button.h"
#include "include/pdfwindow/pwl_utils.h"
#include "include/fx_systemhandler.h"
#include "include/fx_systemhandlerimp.h"
//TODO: wait for /form
#include "../../form/formfiller/ffl_widget.h"

#define  _PDFFORM_NOTBUILD_

#if defined(_FSDK_ALL_) || defined(_PDFFORMFILLER_) /*|| defined(_FSDK_PDFFORM_R_)*/

static int CharSet2CP(int charset)
{
	int ret = FSCRT_UnicodeMapper::GetCodePageFromCharset(charset);
	if(ret < 0)
		return 0;
	return ret;

}

FX_SystemHandlerImp::FX_SystemHandlerImp(CFSPDF_WidgetAnnotHandler* pHandler)
	: m_pHandler(pHandler)
{
	
}

FX_SystemHandlerImp::~FX_SystemHandlerImp()
{

}


void FX_SystemHandlerImp::Release()
{
	delete this;
}

//CFSCRT_LTPDFDocument* FX_SystemHandlerImp::GetDocument()
FSPDFDocImp* FX_SystemHandlerImp::GetDocument()
{
#ifndef _PDFFORM_NOTBUILD_ 
	CFSPDF_ViewContainer* pContainer = NULL;
	m_pHandler->GetViewContainer(&pContainer);
	FXSYS_assert(pContainer);
	return pContainer->GetDocument();
#endif
	return NULL;
}

void FX_SystemHandlerImp::InvalidateRect(FX_HWND hWnd, FX_RECT rect)
{
	//TODO: wait for /form
////#ifndef _PDFFORM_NOTBUILD_ 
//	// convert
	CFFL_Widget* pWidget = (CFFL_Widget*)hWnd;
	CPDF_Rect widgetRect(rect);
	CPDF_Rect pdfRect = pWidget->WidgetRectToFFL(pWidget->GetCurrentMatrix(), widgetRect);
	pWidget->InvalidateRect(pdfRect.left, pdfRect.top, pdfRect.right, pdfRect.bottom);
////#endif
}

FX_BOOL	FX_SystemHandlerImp::IsSelectionImplemented()
{
	return FALSE;
}


void	FX_SystemHandlerImp::OutputSelectedRect(void* pFormFiller, CPDF_Rect&rect)
{
	if(!pFormFiller) return;
	//TODO: wait for /form
	//CPDFAnnot_Base* pAnnot = (CPDFAnnot_Base*)pFormFiller;
	//FX_INT32 pageIndex = pAnnot->GetPageIndex();

	//FSFormFillerImp* pFXFormFiller = NULL;
	//m_pHandler->GetFormFiller(&pFXFormFiller);
	//if(!pFXFormFiller) return;

	//FS_INT32 timerID = 0;
	//FSCRT_RECTF pdfRect = {rect.left, rect.bottom, rect.right, rect.top};
	//pFXFormFiller->m_formInterWindowless.OutputSelectedRect(pageIndex, &pdfRect);
}

CFX_WideString	FX_SystemHandlerImp::GetClipboardText(FX_HWND hWnd)
{ 
	CFX_WideString clip;
	return clip;
}

FX_SYSTEMTIME	FX_SystemHandlerImp::GetLocalTime()
{

	FX_SYSTEMTIME time;
	FXSYS_memset(&time,0,sizeof(FX_SYSTEMTIME));
	return time;
}

CFX_ByteString	FX_SystemHandlerImp::GetNativeTrueTypeFont(FX_INT32 nCharset)
{
	return "Arial";
}

FX_BOOL	FX_SystemHandlerImp::FindNativeTrueTypeFont(FX_INT32 nCharset, CFX_ByteString sFontFaceName)
{
	CFX_Font font;
	if (font.LoadSubst(sFontFaceName, 1, 0, 0, 0, CharSet2CP(nCharset), FALSE))
	{
		return TRUE;
	}
	return FALSE;
}

CPDF_Font*	FX_SystemHandlerImp::AddNativeTrueTypeFontToPDF(CPDF_Document* pDoc, CFX_ByteString sFontFaceName, FX_BYTE nCharset)
{
	CFX_Font* pFXFont = FX_NEW CFX_Font;
	if(pFXFont->LoadSubst(sFontFaceName, 1, 0, 0, 0, CharSet2CP(nCharset), FALSE))
	{
		//	m_GeFonts.Add(pFXFont);
		CPDF_Font* pdfFont = pDoc->AddFont(pFXFont, nCharset, FALSE);
		delete pFXFont;
		return pdfFont;
	}
	if (pFXFont) delete pFXFont;
	return NULL;
}


FX_INT32	FX_SystemHandlerImp::SetTimer(FX_INT32 uElapse, TimerCallback lpTimerFunc)
{
	FSFormFillerImp* pFormFiller = NULL;
	FS_RESULT ret = m_pHandler->GetFormFiller(&pFormFiller);
	FSFormFillerAssist* pFormAssist = pFormFiller->GetFormFillerAssist();
	FS_INT32 timeID = 0;
	if(pFormAssist)
		pFormAssist->setTimer(uElapse, (FS_CALLBACK_TIMER)lpTimerFunc, &timeID);
	return timeID;

//	FS_INT32 timerID = 0;
//	pFormFiller->m_formInterWindowless.SetTimer(uElapse, lpTimerFunc, &timerID);
//	return timerID;
//// 	CFSCRT_LTAppHandler* pAppHandler = FSCRT_GetLTAppHandler();
//// 	if(!pAppHandler) return 0;
//// 
//// 	FSCRT_VAR params[2] = {{FSCRT_VT_INT32}, {FSCRT_VT_LPVOID}};
//// 	params[0].var.int32Var = uElapse;
//// 	params[1].var.voidPtrVar = (FS_LPVOID)lpTimerFunc;
//// 	FSCRT_VAR retvalue;
//// 	FSCRT_GetLTAppHandler()->CallFunction((FSCRT_DOCUMENT)GetDocument(), FSCRT_APPFUNC_SETTIMER, params, 2, &retvalue);
//// 	return retvalue.var.int32Var;
}

void FX_SystemHandlerImp::SetCursor(FX_INT32 nCursorType)
{
//	pFormFiller->m_formInterWindowless.SetCursor(nCursorType);
//// 	CFSCRT_LTAppHandler* pAppHandler = FSCRT_GetLTAppHandler();
//// 	if(!pAppHandler) return;
//// 
//// 	FSCRT_VAR params[1] = {{FSCRT_VT_INT32}};
//// 	params[0].var.int32Var = nCursorType;
//// 	FSCRT_VAR retvalue;
//// 	FSCRT_GetLTAppHandler()->CallFunction((FSCRT_DOCUMENT)GetDocument(), FSCRT_APPFUNC_SETCURSOR, params, 1, &retvalue);
//// 	return;
}

void	FX_SystemHandlerImp::KillTimer(FX_INT32 nID)
{

	FSFormFillerImp* pFormFiller = NULL;
	FS_RESULT ret = m_pHandler->GetFormFiller(&pFormFiller);
	FSFormFillerAssist* pFormAssist = pFormFiller->GetFormFillerAssist();
	FS_INT32 timeID = 0;
	if(pFormAssist)
		pFormAssist->killTimer(nID);
	return;

//// 	CFSCRT_LTAppHandler* pAppHandler = FSCRT_GetLTAppHandler();
//// 	if(!pAppHandler) return;
//// 
//// 	FSCRT_VAR params[1] = {{FSCRT_VT_INT32}};
//// 	params[0].var.int32Var = nID;
//// 	FSCRT_VAR retvalue;
//// 	FSCRT_GetLTAppHandler()->CallFunction((FSCRT_DOCUMENT)GetDocument(), FSCRT_APPFUNC_KILLTIMER, params, 1, &retvalue);

}

void	FX_SystemHandlerImp::ClientToScreen(FX_HWND hWnd, FX_INT32& x, FX_INT32& y)
{

}
void	FX_SystemHandlerImp::ScreenToClient(FX_HWND hWnd, FX_INT32& x, FX_INT32& y)
{

}

#endif
