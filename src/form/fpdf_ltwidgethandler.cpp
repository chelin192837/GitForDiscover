#include "../inc/fs_precompilers.h"

#if defined(_FSDK_ALL_) || defined(_PDFFORMFILLER_) /*|| defined(_FSDK_PDFFORM_R_)*/
#include "../inc/fpdf_define.h"
#include "fpdf_ltwidgethandler.h"
#include "formfiller/ffl_widget.h"

#include "formfiller/ffl_button.h"
#include "formfiller/ffl_checkbox.h"
#include "formfiller/ffl_combobox.h"
#include "formfiller/ffl_listbox.h"
#include "formfiller/ffl_pushbutton.h"
#include "formfiller/ffl_radiobutton.h"
#include "formfiller/ffl_textfield.h"
#include "../widget/pdfwindow/include/pdfwindow/ipdfwindow.h"
#include "../widget/pdfwindow/include/fpdf_resetappearance.h"

#include "../widget/pdfwindow/include/fx_systemhandlerimp.h"

FX_BOOL	IsSHIFTKeyDown(FX_DWORD nFlag)
{
	return (nFlag & e_eventFlagShiftKey) != 0;
}
FX_BOOL	IsCTRLKeyDown(FX_DWORD nFlag)
{
	return (nFlag & e_eventFlagControlKey) != 0;
}
FX_BOOL	IsALTKeyDown(FX_DWORD nFlag)
{
	return (nFlag & e_eventFlagAltKey) != 0;
}
FX_BOOL	IsINSERTKeyDown(FX_DWORD nFlag)
{
	return FALSE;
}
#define WIDGETBOOL2RETVAL(x) ((x) ? FSCRT_ERRCODE_SUCCESS : FSCRT_ERRCODE_UNSUPPORTED)

CFSPDF_WidgetAnnotHandler::CFSPDF_WidgetAnnotHandler() 
	: m_bNotifyBlocked(FALSE),m_pFormFiller(NULL), m_pSystemHandler(NULL),m_bNotifying(FALSE)
{
}

CFSPDF_WidgetAnnotHandler::~CFSPDF_WidgetAnnotHandler()
{
	FX_POSITION pos = m_Map.GetStartPosition();
	while (pos)
	{
		FSFormControl* pControl = NULL;
		CFFL_Widget * pWidget = NULL;
		m_Map.GetNextAssoc(pos, pControl, pWidget);
		if(pWidget)
			delete pWidget;
	}
	m_Map.RemoveAll();
	
	if(m_pSystemHandler)
	{
		m_pSystemHandler->Release();
		m_pSystemHandler = NULL;
	}

}

FX_BSTR CFSPDF_WidgetAnnotHandler::GetType()
{
	static const CFX_ByteStringC g_TypeName("Widget");
	return g_TypeName;
}

void CFSPDF_WidgetAnnotHandler::Release()
{
	FXSYS_assert(this);
	delete this;
}

static FX_RECT PDFRectToFXRect(const CPDF_Rect& rect)
{
	FX_RECT fxRect;
	FXSYS_memset(&fxRect,0,sizeof(FX_RECT));

	fxRect.left = (int)rect.left;
	fxRect.right = (int)rect.right;
	fxRect.top = (int)rect.top;
	fxRect.bottom = (int)rect.bottom;
	return fxRect;
}

IFX_SystemHandler* CFSPDF_WidgetAnnotHandler::GetSystemHandler()
{
	if(!m_pSystemHandler) 
	{
		m_pSystemHandler = FX_NEW FX_SystemHandlerImp(this);
	}
	return m_pSystemHandler;
}


FS_RESULT CFSPDF_WidgetAnnotHandler::GetViewBBox(FSPDFPage *pPageView, FSAnnot* pAnnot, FX_RECT& rect)
{
	FXSYS_assert(pAnnot);
	FXSYS_assert(pPageView);
	CPDF_Rect pdfRect(0,0,0,0);

	FSFormControl* pControl = FSANNOT_Util_GetFormControlImp(pAnnot);
    CPDF_FormControl* pFormControl = ((FSFormControlImp*)pControl)->GetCPDFFormControl();
	if(CFFL_Widget* pWidget = GetWidget(pControl))
	{
		pdfRect = pWidget->GetViewBBox(pPageView);

	}
	else
	{
		if (pFormControl)
		{
			CPDF_Rect rcAnnot = pFormControl->GetRect();

			CPDF_Rect rcWin = rcAnnot;
			rcWin.Inflate(1, 1);

			pdfRect = rcWin.GetOutterRect();
		}
		
	}
	rect = PDFRectToFXRect(pdfRect);
	return FSCRT_ERRCODE_SUCCESS;
}

FS_RESULT CFSPDF_WidgetAnnotHandler::OnCreate(FSAnnot* pAnnot) 
{
	FSCRT_LOCKOBJECT(lock, &m_lock)
	FXSYS_assert(pAnnot != NULL);

	FSFormControl* pControl = FSANNOT_Util_GetFormControlImp(pAnnot);
	if(CFFL_Widget* pWidget = GetWidget(pControl, FALSE))
	{
		pWidget->OnCreate();
	}

	return FSCRT_ERRCODE_SUCCESS;
}


FS_RESULT CFSPDF_WidgetAnnotHandler::OnLoad(FSAnnot* pAnnot)
{
	FSCRT_LOCKOBJECT(lock, &m_lock)

	FXSYS_assert(pAnnot != NULL);

	FSFormControl* pControl = (FSFormControl*)FSANNOT_Util_GetFormControlImp(pAnnot);
	if(CFFL_Widget* pWidget = GetWidget(pControl, FALSE))
	{
		pWidget->FormatAndResetAppearance();

		pWidget->OnLoad();
	}
	return FSCRT_ERRCODE_SUCCESS;
}

FS_RESULT CFSPDF_WidgetAnnotHandler::OnRelease (FSAnnot* pAnnot)
{
	FSCRT_LOCKOBJECT(lock, &m_lock)
	FXSYS_assert(pAnnot != NULL);

	FSFormControl* pControl = (FSFormControl*)FSANNOT_Util_GetFormControlImp(pAnnot);
	if(CFFL_Widget* pWidget = GetWidget(pControl, FALSE))
	{
		pWidget->OnRelease();
	}

	ReleaseWidget(pControl);
	return FSCRT_ERRCODE_SUCCESS;
}

// FS_RESULT CFSPDF_WidgetAnnotHandler::OnMouseEnter (FSAnnot* pAnnot, FSPDFPage *pPageView)
// {
// 	FSCRT_LOCKOBJECT(lock, &m_lock)
// 	FSFormControl* pControl = (FSFormControl*)pAnnot;
// 	CFFL_Widget* widget = GetWidget(pControl);
// 	if(!widget) return FSCRT_ERRCODE_UNSUPPORTED;
// 	widget->OnMouseEnter(pPageView);
// 	return FSCRT_ERRCODE_SUCCESS;
// }
// 
// FS_RESULT CFSPDF_WidgetAnnotHandler::OnMouseExit (FSAnnot* pAnnot, FSPDFPage *pPageView)
// {
// 	FSCRT_LOCKOBJECT(lock, &m_lock)
// 	FSFormControl* pControl = (FSFormControl*)pAnnot;
// 	CFFL_Widget* widget = GetWidget(pControl);
// 	if(!widget) return FSCRT_ERRCODE_UNSUPPORTED;
// 	widget->OnMouseExit(pPageView);
// 	return FSCRT_ERRCODE_SUCCESS;
// }


FS_RESULT CFSPDF_WidgetAnnotHandler::OnDeSelected (FSAnnot* pAnnot)
{
	FSCRT_LOCKOBJECT(lock, &m_lock)
	FSFormControl* pControl = (FSFormControl*)FSANNOT_Util_GetFormControlImp(pAnnot);
	CFFL_Widget* widget = GetWidget(pControl);
	if(!widget) return FSCRT_ERRCODE_UNSUPPORTED;
	widget->OnDeSelected();
	return FSCRT_ERRCODE_SUCCESS;
}

FS_RESULT CFSPDF_WidgetAnnotHandler::OnSelected (FSAnnot* pAnnot)
{
	FSCRT_LOCKOBJECT(lock, &m_lock)
	FSFormControl* pControl = (FSFormControl*)FSANNOT_Util_GetFormControlImp(pAnnot);
	CFFL_Widget* widget = GetWidget(pControl);
	if(!widget) return FSCRT_ERRCODE_UNSUPPORTED;
	widget->OnSelected();
	return FSCRT_ERRCODE_SUCCESS;
}

FS_RESULT	CFSPDF_WidgetAnnotHandler::SetFormFiller (FSFormFillerImp* formFiller)
{
	FSCRT_LOCKOBJECT(lock, &m_lock)
	if (!formFiller) return FSCRT_ERRCODE_PARAM;
	m_pFormFiller = formFiller;
	return FSCRT_ERRCODE_SUCCESS;
}

FS_RESULT	CFSPDF_WidgetAnnotHandler::GetFormFiller (FSFormFillerImp** formFiller)
{
	FSCRT_LOCKOBJECT(lock, &m_lock)
	*formFiller = m_pFormFiller;
	return FSCRT_ERRCODE_SUCCESS;
}

FS_RESULT	CFSPDF_WidgetAnnotHandler::useWindowless (FS_BOOL* bUseWindowless) 
{
	*bUseWindowless = FALSE;
	if(m_pFormFiller) 
		*bUseWindowless =(FS_BOOL) m_pFormFiller->useWindowless();
	return FSCRT_ERRCODE_SUCCESS;
}

FSFormControl*	CFSPDF_WidgetAnnotHandler::GetFocusControl()
{
	FSCRT_LOCKOBJECT(lock, &m_lock)
	//CFSPDF_ViewContainerController* pViewContainerController = m_pViewContainer ? m_pViewContainer->m_containerController : NULL;
	
	//FSFormControl* pControl = NULL;
	//if(pViewContainerController)
	//	pViewContainerController->GetFocusAnnot((FSAnnot**)&pControl);
	

	return (FSFormControl*)m_pFormFiller->GetFocusAnnot();
}

void CFSPDF_WidgetAnnotHandler::SetFocusControl(FSFormControl* pControl)
{
	FSCRT_LOCKOBJECT(lock, &m_lock)
// 	CFSPDF_ViewContainerController* pViewContainerController = m_pViewContainer ? m_pViewContainer->m_containerController : NULL;
// 
// 	if(pViewContainerController)
// 		pViewContainerController->SetFocusAnnot(pControl);
	if (!pControl) return;
	if (pControl->getField() || pControl->getField()->getType() == FSFormField::e_formFieldSignature)
		return;
	m_pFormFiller->SetFocusAnnot((FSAnnot*)pControl);
}

FS_RESULT CFSPDF_WidgetAnnotHandler::OnMouseEnter(FSAnnot* pAnnot, FSPDFPage *pPageView)
{
	ASSERT(pAnnot != NULL);
	//ASSERT(pAnnot->m_pSTAnnot->GetAnnotType() == "Widget");
	
	FSFormControlImp* pWidget = (FSFormControlImp*)FSANNOT_Util_GetFormControlImp(pAnnot);
	if (!m_bNotifying)
	{
		if (pWidget->GetAAction(CPDF_AAction::CursorEnter))
		{
			m_bNotifying = TRUE;
			int nAge = pWidget->GetAppearanceAge();
			int nValueAge = pWidget->GetValueAge();

			//pWidget->ClearAppModified();
			
// 			ASSERT(pPageView != NULL);
// 			CPDFSDK_Document* pDocument = pPageView->GetSDKDocument();
// 			ASSERT(pDocument != NULL);
			

			FSPDF_FieldActionParam fa;
			pWidget->OnAAction(CPDF_AAction::CursorEnter, fa, pPageView);
			m_bNotifying = FALSE;
			
			//if ( !IsValidAnnot(pPageView, pAnnot) ) return;
			
			if (nAge != pWidget->GetAppearanceAge())
			{
				if (CFFL_Widget* widget = GetWidget(pWidget))
				{
					widget->ResetPDFWindow(pPageView, nValueAge == pWidget->GetValueAge());
				}
			}
		}
	}
	
	CFFL_Widget* widget = GetWidget(pWidget, TRUE);
	if (widget != NULL)
		widget->OnMouseEnter(pPageView);
	return FSCRT_ERRCODE_SUCCESS;
}

FS_RESULT CFSPDF_WidgetAnnotHandler::OnMouseExit(FSAnnot* pAnnot, FSPDFPage *pPageView)
{
	ASSERT(pAnnot != NULL);
	//ASSERT(pAnnot->m_pSTAnnot->GetAnnotType() == "Widget");
	
	FSFormControlImp* pWidget = (FSFormControlImp*)FSANNOT_Util_GetFormControlImp(pAnnot);
	if (!m_bNotifying)
	{
		if (pWidget->GetAAction(CPDF_AAction::CursorExit))
		{
			m_bNotifying = TRUE;
			int nAge = pWidget->GetAppearanceAge();
			int nValueAge = pWidget->GetValueAge();
			//pWidget->ClearAppModified();
		
			
			FSPDF_FieldActionParam fa;
			pWidget->OnAAction(CPDF_AAction::CursorExit, fa, pPageView);
			m_bNotifying = FALSE;
			
			//if (!IsValidAnnot(pPageView, pAnnot)) return;
			
			//if (pWidget->IsAppModified())
			{
				if (CFFL_Widget* widget = GetWidget(pWidget))
				{
					widget->ResetPDFWindow(pPageView, nValueAge == pWidget->GetValueAge());
				}
			}
		}
	}
	
	CFFL_Widget* widget = GetWidget(pWidget, TRUE);
	if (widget != NULL)
		widget->OnMouseExit(pPageView);
	return FSCRT_ERRCODE_SUCCESS;
}


FS_RESULT CFSPDF_WidgetAnnotHandler::OnLButtonDown (FSAnnot* pAnnot, FSPDFPage *pPageView, FS_DWORD nFlags, const CFX_PointF& point)
{
	FSCRT_LOCKOBJECT(lock, &m_lock)
	FSFormControlImp* pControl = (FSFormControlImp*)FSANNOT_Util_GetFormControlImp(pAnnot);
	if (!pControl) return FSCRT_ERRCODE_ERROR;
	/*TODO: Phantom code, check if this is necessary
	if (pDocument->CountSelection() > 1 || pDocument->GetFocusAnnot() != pAnnot)
	pDocument->ClearAllSelection(); */
	switch(pControl->GetCPDFFormControl()->GetType())
	{
	case CPDF_FormField::PushButton:
	case CPDF_FormField::CheckBox:
	case CPDF_FormField::RadioButton:
		{
			FX_RECT rect; CFX_PointF pagePoint;
			GetViewBBox(pPageView, pAnnot, rect);
			//((FSPDFPageImp*)pPageView)->DeviceToPagePoint(point, pagePoint);
			rect.Normalize();
			if (!rect.Contains((int)point.x, (int)point.y))
				break;
		}
	default:
		SetFocusControl(pControl);
		break;
	}

	// TODO: Phantom version action code, pending for implementation 
	if (!m_bNotifying)
	{
		if (pControl->GetAAction(CPDF_AAction::ButtonDown))
		{
			m_bNotifying = TRUE;
			int nAge = pControl->GetAppearanceAge();
			int nValueAge = pControl->GetValueAge();

			FSPDF_FieldActionParam fa;
			fa.bModifier = IsCTRLKeyDown(nFlags);
			fa.bShift = IsSHIFTKeyDown(nFlags);

			pControl->OnAAction(CPDF_AAction::ButtonDown, fa, pPageView);
			m_bNotifying = FALSE;

			//if (!IsValidAnnot(m_pApp, pDocument, pDocView, pPageView, pAnnot)) return TRUE;

			if (nAge != pControl->GetAppearanceAge())
			{
				if (CFFL_Widget* widget = GetWidget(pControl))
				{
					widget->ResetPDFWindow(pPageView, nValueAge == pControl->GetValueAge());
				}
			}
		}
	}

	//pDocument->SetTopmostAnnot(pAnnot);

	CFFL_Widget* widget = GetWidget(pControl);
	if(!widget) return FSCRT_ERRCODE_UNSUPPORTED;
	return WIDGETBOOL2RETVAL(widget->OnLButtonDown(pPageView, nFlags, point));
}

FS_RESULT CFSPDF_WidgetAnnotHandler::OnLButtonUp (FSAnnot* pAnnot, FSPDFPage *pPageView, FS_DWORD nFlags, const CFX_PointF& point)
{
	FSFormControl* pControl = (FSFormControl*)FSANNOT_Util_GetFormControlImp(pAnnot);


	FS_RESULT bRet;
	
	CFFL_Widget* widget = GetWidget(pControl);
	if(!widget)
		return FSCRT_ERRCODE_UNSUPPORTED;
	else
		bRet = WIDGETBOOL2RETVAL(widget->OnLButtonUp(pPageView, nFlags, point));

	CFSPDF_WidgetAnnotHandler* pWidgetHandler =  widget->GetWidgetHandler();
	FSFormFillerImp* pFormFiller = NULL;
	bRet = pWidgetHandler->GetFormFiller(&pFormFiller);
	if(FSCRT_ERRCODE_SUCCESS != bRet) return bRet;

	// Phantom version action code, pending for implement
	if (pFormFiller->GetFocusAnnot() == pAnnot)
	{
		FX_BOOL bExit = FALSE;
		FX_BOOL bReset = FALSE;
		OnButtonUp(pControl, pPageView, bReset, bExit, nFlags);
		if (bExit) return TRUE;

		//OnClick(pWidget, pPageView, bReset, bExit);
		//if (bExit) return TRUE;
	} 

	return bRet;
}

void	CFSPDF_WidgetAnnotHandler::OnButtonUp(FSFormControl* pWidget, FSPDFPage* pPageView, FX_BOOL& bReset, FX_BOOL& bExit,FS_DWORD nFlag)
{
	ASSERT(pWidget != NULL);
	FSFormControlImp* pWg = (FSFormControlImp*)pWidget;
	if (!m_bNotifying)
	{
		if (pWg->GetAAction(CPDF_AAction::ButtonUp))
		{
			m_bNotifying = TRUE;
			int nAge = pWg->GetAppearanceAge();
			int nValueAge = pWg->GetValueAge();

			ASSERT(pPageView != NULL);
// 			CReader_DocView* pDocView = pPageView->GetDocView();
// 			ASSERT(pDocView != NULL);
// 			CPDFSDK_Document* pDocument = pPageView->GetSDKDocument();
// 			ASSERT(pDocument != NULL);

			FSPDF_FieldActionParam fa;
			fa.bModifier = IsCTRLKeyDown(nFlag);
			fa.bShift = IsSHIFTKeyDown(nFlag);

			pWg->OnAAction(CPDF_AAction::ButtonUp, fa, pPageView);
			m_bNotifying = FALSE;

// 			if (!IsValidAnnot(pPageView, pWidget))
// 			{
// 				bExit = TRUE;
// 				return;
// 			}

			if (nAge != pWg->GetAppearanceAge())
			{
				if (CFFL_Widget* widget = GetWidget(pWidget))
				{
					widget->ResetPDFWindow(pPageView, nValueAge == pWg->GetValueAge());
				}

				bReset = TRUE;
			}
		}
	}
}

FS_RESULT CFSPDF_WidgetAnnotHandler::OnLButtonDblClk (FSAnnot* pAnnot, FSPDFPage *pPageView, FS_DWORD nFlags, const CFX_PointF& point)
{
	FSFormControl* pControl = (FSFormControl*)FSANNOT_Util_GetFormControlImp(pAnnot);
	CFFL_Widget* widget = GetWidget(pControl, FALSE);
	if(!widget) return FSCRT_ERRCODE_UNSUPPORTED;
	return WIDGETBOOL2RETVAL(widget->OnLButtonDblClk(pPageView, nFlags, point));
}

FS_RESULT CFSPDF_WidgetAnnotHandler::OnMouseMove (FSAnnot* pAnnot, FSPDFPage *pPageView, FS_DWORD nFlags, const CFX_PointF& point)
{
	FSFormControl* pControl = (FSFormControl*)FSANNOT_Util_GetFormControlImp(pAnnot);
	CFFL_Widget* widget = GetWidget(pControl, TRUE);
	if(!widget) return FSCRT_ERRCODE_UNSUPPORTED;
	return WIDGETBOOL2RETVAL(widget->OnMouseMove(pPageView, nFlags, point));
}

FS_RESULT CFSPDF_WidgetAnnotHandler::OnMouseWheel (FSAnnot* pAnnot, FSPDFPage *pPageView, FS_DWORD nFlags, FS_INT32 deltaX, FX_INT32 deltaY, const CFX_PointF& point)
{
	FSFormControl* pControl = (FSFormControl*)FSANNOT_Util_GetFormControlImp(pAnnot);
	CFFL_Widget* widget = GetWidget(pControl, FALSE);
	if(!widget) return FSCRT_ERRCODE_UNSUPPORTED;
	return WIDGETBOOL2RETVAL(widget->OnMouseWheel(pPageView, nFlags, deltaY, point));
}

FS_RESULT CFSPDF_WidgetAnnotHandler::OnRButtonDown (FSAnnot* pAnnot, FSPDFPage *pPageView, FS_DWORD nFlags, const CFX_PointF& point)
{
	FSFormControl* pControl = (FSFormControl*)FSANNOT_Util_GetFormControlImp(pAnnot);
	CFFL_Widget* widget = GetWidget(pControl, FALSE);
	if(!widget) return FSCRT_ERRCODE_UNSUPPORTED;
	return WIDGETBOOL2RETVAL(widget->OnRButtonDown(pPageView, nFlags, point));
}

FS_RESULT CFSPDF_WidgetAnnotHandler::OnRButtonUp (FSAnnot* pAnnot, FSPDFPage *pPageView, FS_DWORD nFlags, const CFX_PointF& point)
{
	FSFormControl* pControl = (FSFormControl*)FSANNOT_Util_GetFormControlImp(pAnnot);
	CFFL_Widget* widget = GetWidget(pControl, FALSE);
	if(!widget) return FSCRT_ERRCODE_UNSUPPORTED;
	return WIDGETBOOL2RETVAL(widget->OnRButtonUp(pPageView, nFlags, point));
}

FS_RESULT CFSPDF_WidgetAnnotHandler::OnRButtonDblClk (FSAnnot* pAnnot, FSPDFPage *pPageView, FS_DWORD nFlags, const CFX_PointF& point)
{
	FSFormControl* pControl = (FSFormControl*)FSANNOT_Util_GetFormControlImp(pAnnot);
	CFFL_Widget* widget = GetWidget(pControl);
	if(!widget) return FSCRT_ERRCODE_UNSUPPORTED;
	return WIDGETBOOL2RETVAL(widget->OnRButtonDblClk(pPageView, nFlags, point));
}

FS_RESULT CFSPDF_WidgetAnnotHandler::OnMButtonDown	(FSAnnot* pAnnot, FSPDFPage *pPageView, FS_DWORD nFlags, const CFX_PointF& point)
{
	return FSCRT_ERRCODE_UNSUPPORTED;
}

FS_RESULT CFSPDF_WidgetAnnotHandler::OnMButtonUp		(FSAnnot* pAnnot, FSPDFPage *pPageView, FS_DWORD nFlags, const CFX_PointF& point)
{
	return FSCRT_ERRCODE_UNSUPPORTED;
}

FS_RESULT CFSPDF_WidgetAnnotHandler::OnMButtonDblClk	(FSAnnot* pAnnot, FSPDFPage *pPageView, FS_DWORD nFlags, const CFX_PointF& point)
{
	return FSCRT_ERRCODE_UNSUPPORTED;
}

FS_RESULT CFSPDF_WidgetAnnotHandler::OnSetFocus(FSAnnot* pAnnot)
{
	FSFormControl* pControl = (FSFormControl*)FSANNOT_Util_GetFormControlImp(pAnnot);
	CFFL_Widget* widget = GetWidget(pControl);
	if(!widget) return FSCRT_ERRCODE_UNSUPPORTED;
	FS_RESULT ret = WIDGETBOOL2RETVAL(widget->OnSetFocus(0));

	if(!m_bNotifying)
	{
		FSFormControlImp* pWidget = (FSFormControlImp*)(FSFormControl*)pAnnot;
		if (pWidget->GetAAction(CPDF_AAction::GetFocus))
		{
			m_bNotifying = TRUE;

			FSPDFPage* pPageView = pWidget->GetPageView();
			ASSERT(pPageView != NULL);

			FSPDF_FieldActionParam fa;
			fa.bModifier = 0;
			fa.bShift = 0;

			widget->GetActionData(pPageView, CPDF_AAction::GetFocus, fa);

			pWidget->OnAAction(CPDF_AAction::GetFocus, fa, pPageView);
			m_bNotifying = FALSE;

			//if (!IsValidAnnot(m_pApp, pDocument, pDocView, pPageView, pAnnot)) return FALSE;
		}
	}

	return ret;
}

FS_RESULT CFSPDF_WidgetAnnotHandler::OnKillFocus(FSAnnot* pAnnot)
{
	FSFormControl* pControl = (FSFormControl*)FSANNOT_Util_GetFormControlImp(pAnnot);
	CFFL_Widget* widget = GetWidget(pControl);
	if(!widget) return FSCRT_ERRCODE_UNSUPPORTED;
	FS_RESULT ret = WIDGETBOOL2RETVAL(widget->OnKillFocus(0));

	if(!m_bNotifying)
	{
		FSFormControlImp* pWidget = (FSFormControlImp*)(FSFormControl*)pAnnot;
		if (pWidget->GetAAction(CPDF_AAction::LoseFocus))
		{
			m_bNotifying = TRUE;

			FSPDFPage* pPageView = pWidget->GetPageView();
			ASSERT(pPageView != NULL);

			FSPDF_FieldActionParam fa;
			fa.bModifier = 0;
			fa.bShift = 0;

			widget->GetActionData(pPageView, CPDF_AAction::LoseFocus, fa);

			pWidget->OnAAction(CPDF_AAction::LoseFocus, fa, pPageView);
			m_bNotifying = FALSE;

			//if (!IsValidAnnot(m_pApp, pDocument, pDocView, pPageView, pAnnot)) return FALSE;
		}
	}

	return ret;
}

FS_RESULT CFSPDF_WidgetAnnotHandler::OnKeyDown (FSAnnot* pAnnot, FS_DWORD nKeyCode, FS_DWORD nFlags)
{
	FSFormControl* pControl = (FSFormControl*)FSANNOT_Util_GetFormControlImp(pAnnot);
	CFFL_Widget* widget = GetWidget(pControl, FALSE);
	if(!widget) return FSCRT_ERRCODE_UNSUPPORTED;
	return WIDGETBOOL2RETVAL(widget->OnKeyDown(nKeyCode, nFlags));
}

FS_RESULT CFSPDF_WidgetAnnotHandler::OnKeyUp (FSAnnot* pAnnot, FS_DWORD nKeyCode, FS_DWORD nFlags)
{
	FSFormControl* pControl = (FSFormControl*)FSANNOT_Util_GetFormControlImp(pAnnot);
	CFFL_Widget* widget = GetWidget(pControl, FALSE);
	if(!widget) return FSCRT_ERRCODE_UNSUPPORTED;
	return WIDGETBOOL2RETVAL(widget->OnKeyUp(nKeyCode, nFlags));
}

FS_RESULT CFSPDF_WidgetAnnotHandler::OnChar (FSAnnot* pAnnot, FS_DWORD nKeyCode, FS_DWORD nFlags)
{
	FSFormControl* pControl = (FSFormControl*)FSANNOT_Util_GetFormControlImp(pAnnot);
	CFFL_Widget* widget = GetWidget(pControl, FALSE);
	if(!widget) return FSCRT_ERRCODE_UNSUPPORTED;
	return WIDGETBOOL2RETVAL(widget->OnChar(nKeyCode, nFlags));
}

FS_RESULT CFSPDF_WidgetAnnotHandler::OnString (FSAnnot* pAnnot, FX_WSTR string,	FS_DWORD nFlags)
{
	FSFormControl* pControl = (FSFormControl*)FSANNOT_Util_GetFormControlImp(pAnnot);
	CFFL_Widget* widget = GetWidget(pControl, FALSE);
	if(!widget) return FSCRT_ERRCODE_UNSUPPORTED;
	return WIDGETBOOL2RETVAL(widget->OnString(string, nFlags));
}

CFFL_Widget* CFSPDF_WidgetAnnotHandler::GetWidget(FSFormControl* pControl, FX_BOOL bCreateIfNotExist)
{
	FSCRT_LOCKOBJECT(lock, &m_lock)
	if(!pControl) return NULL;
    CPDF_FormControl* pFormControl = ((FSFormControlImp*)pControl)->GetCPDFFormControl();
	CFFL_Widget* pWidget = NULL;
	m_Map.Lookup(pControl, pWidget);

	if(!pWidget && bCreateIfNotExist)
	{
		if (pFormControl && pFormControl->GetField())
		{
			switch(pFormControl->GetType())
			{
			case CPDF_FormField::PushButton:
				pWidget = FX_NEW CFFL_PushButtonCtrl(pControl, this);
				break;
			case CPDF_FormField::RadioButton:
				pWidget = FX_NEW CFFL_RadioButtonCtrl(pControl, this);
				break;
			case CPDF_FormField::CheckBox:
				pWidget = FX_NEW CFFL_CheckBoxCtrl(pControl, this);
				break;
			case CPDF_FormField::RichText:
			case CPDF_FormField::Text:
				pWidget = FX_NEW CFFL_TextFieldCtrl(pControl, this);
				break;
			case CPDF_FormField::ComboBox:
				pWidget = FX_NEW CFFL_ComboBoxCtrl(pControl, this);
				break;
			case CPDF_FormField::ListBox:
				pWidget = FX_NEW CFFL_ListBoxCtrl(pControl, this);
				break;
			default:
				return NULL;
			}
			if(pWidget)
				m_Map.SetAt(pControl, pWidget);
		}
	}

	return pWidget;
}


void CFSPDF_WidgetAnnotHandler::ReleaseWidget(FSFormControl* pControl)
{
	FSCRT_LOCKOBJECT(lock, &m_lock)
	CFFL_Widget* pWidget = NULL;

	//if(!pWidget) return;
	if(m_Map.Lookup(pControl, pWidget))
	{
		if(pWidget)
			delete pWidget;
		m_Map.RemoveKey(pControl);
	}
}

FS_RESULT CFSPDF_WidgetAnnotHandler::OnDraw(FSPDFPage *pPageView, FSRenderer* renderer, FSAnnot* pAnnot, 
											CPDF_Matrix* pUser2Device, const FSRectI& rcWindow, FS_DWORD dwFlags)
{
    if (!pUser2Device) return FSCRT_ERRCODE_ERROR;
	FSCRT_LOCKOBJECT(lock, &m_lock)
	FXSYS_assert(pAnnot);
    FSFormControlImp* pControl = FSANNOT_Util_GetFormControlImp(pAnnot);
#if 0
	CPDF_FormControl* pFormControl = pControl->GetCPDFFormControl();
	if (pFormControl)
	{
		CPDF_FormField* pField = pFormControl->GetField();
		if(pField->GetType()==CPDF_FormField::PushButton)
			pField = pField;
	}

#endif

#if _FX_OS_ == _FX_WIN32_DESKTOP_ || _FX_OS_ == _FX_WIN64_
	// to check whether the Print bit of annot flag "F" entry is set; if yes, will do not print the annotation
	FS_INT32 nControlFlag = 0;
	FS_RESULT ret = pControl->ST_GetFlags(&nControlFlag);
	//if(FSCRT_ERRCODE_SUCCESS == ret && renderer->m_pSTRenderDevice->GetDeviceClass() == FXDC_PRINTER)
	//{
	//	if (!(nControlFlag & ANNOTFLAG_PRINT)) return FSCRT_ERRCODE_SUCCESS;
	//}

#endif

	CFFL_Widget* widget = GetWidget(pControl, FALSE);

	if (!pControl->ST_IsVisible()) return FSCRT_ERRCODE_SUCCESS;

	CFX_RenderDevice* pDevice = ((FSRendererImp*)renderer)->getRenderDevice();

	if (widget && widget->IsActivated())
	{
		widget->OnDraw(pPageView, pDevice, pUser2Device, dwFlags);

		FSFormControl* pFocusControl = GetFocusControl();
		if(pControl == pFocusControl)
			DrawFocus(widget->GetFocusBox(pPageView), pDevice, pUser2Device, pPageView);
	}
	else
	{
		if(widget)
			widget->OnDrawInactive(pPageView, pDevice, pUser2Device, dwFlags);
		else
			DrawAppearance(pControl, pDevice, pUser2Device, CPDF_Annot::Normal, NULL);

		DrawShadow(pControl, pDevice, pUser2Device, pPageView);
		/* TODO: Investigate on this
		if (pWidget->GetFieldType() == FIELDTYPE_TEXTFIELD)
		{
			CFFL_TextField::DrawScrollMark(pPageView, hDC, pWidget, pDevice, pUser2Device, rcWindow, dwFlags);
		}
		*/

	}
	return FSCRT_ERRCODE_SUCCESS;
}
//copy from phantom, used to draw the drop button of combo box
FX_BOOL	CFSPDF_WidgetAnnotHandler::WidgetDirectionIsRightToLeft(CPDF_Dictionary* pAnnotDict, FX_BOOL& bRTL)
{
	if (NULL == pAnnotDict) return FALSE;
	int nMEOptions = 0;
	if (pAnnotDict->KeyExist("MEOptions"))
	{
		nMEOptions = pAnnotDict->GetInteger("MEOptions",nMEOptions);
	}

	bRTL = nMEOptions;
	return TRUE;
}

void CFSPDF_WidgetAnnotHandler::DrawAppearance(FSFormControl* pControl, CFX_RenderDevice* pDevice, const CPDF_Matrix* pUser2Device, CPDF_Annot::AppearanceMode mode, const CPDF_RenderOptions* pOptions)
{
	FXSYS_assert(pControl);
//	FXSYS_assert(pControl->GetCPDFFormControl());
	FSFormControlImp* pContronImp = (FSFormControlImp*)pControl;
	CPDF_FormControl* pFormControl = pContronImp->GetCPDFFormControl();
    int nFieldType = -1;
	if (pFormControl && pFormControl->GetField())
	{
		nFieldType = pFormControl->GetField()->GetFieldType();
		//if(nFieldType == FIELDTYPE_SIGNATURE) return;
	}else
        return;

	CPDF_Dictionary* pAnnotDict = (CPDF_Dictionary*)pContronImp->getDict();
	CPDF_Page* pPage = pContronImp->GetPDFPage();
	
	if ((nFieldType == FIELDTYPE_CHECKBOX || nFieldType == FIELDTYPE_RADIOBUTTON) &&
		mode == CPDF_Annot::Normal && 
		!IsWidgetAppearanceValid(pControl, CPDF_Annot::Normal))
	{
		CFX_PathData pathData;
		FSRectF rcAnnot = pContronImp->getRect();

		pathData.AppendRect(rcAnnot.left, rcAnnot.bottom,
			rcAnnot.right, rcAnnot.top);

		CFX_GraphStateData gsd;
		gsd.m_LineWidth = 0.0f;

		pDevice->DrawPath(&pathData, pUser2Device, &gsd, 0, 0xFFAAAAAA, FXFILL_ALTERNATE);
	}
	else
	{
		CFX_ByteString cmode;
		switch(mode)
		{
		case CPDF_Annot::Normal:
			cmode = CFX_ByteString("N");
			break;
		case CPDF_Annot::Rollover:
			cmode = CFX_ByteString("R");
			break;
		case CPDF_Annot::Down:
			cmode = CFX_ByteString("D");
			break;

		}

		pContronImp->drawAppearance(pDevice, pUser2Device, cmode, (CPDF_RenderOptions*)pOptions);

		FX_INT32 flags = 0;
		CPDF_FormField *pField = pFormControl->GetField();
		flags = pField->GetFlags();
		if(flags & FORMFIELD_READONLY)
			return;

		flags = pContronImp->getFlags();
		if (flags & FPDFANNOT_FLAG_Hidden || flags & FPDFANNOT_FLAG_NoView) return;

		if (FIELDTYPE_COMBOBOX == nFieldType)
		{
			CPDF_Page* pSTPage = pPage;
			if (!pSTPage)
				return;
			CPDF_RenderContext context;
			context.Create(pSTPage);

			CFSCRT_STPDFResetApperance stAP(pFormControl);
			CPDF_Rect rcButton = stAP.GetClientRect();

			FX_BOOL bRTL = FALSE;
			if (WidgetDirectionIsRightToLeft(pAnnotDict, bRTL) && bRTL)
			{
				rcButton.right = rcButton.left + 13;
				rcButton.Normalize();
			}
			else
			{
				rcButton.left = rcButton.right - 13;
				rcButton.Normalize();
			}

			CFX_ByteString sStream = CPWL_Utils::GetDropButtonAppStream(rcButton);

			CPDF_Matrix matrix;
			matrix.Copy(*pUser2Device);
			FSRectF rcAnnot = pControl->getRect();
			matrix.Translate(rcAnnot.left, rcAnnot.bottom, TRUE);
			matrix.Concat(stAP.GetMatrix(), TRUE);

			context.DrawStream(pDevice, (FX_LPCSTR)sStream , sStream.GetLength(), &matrix/*pUser2Device*/);
		}
	}
}

void CFSPDF_WidgetAnnotHandler::DrawFocus(const CPDF_Rect& rcFocus, CFX_RenderDevice* pDevice, const CPDF_Matrix* pUser2Device, FSPDFPage* pPageView)
{
	if (rcFocus.IsEmpty()) return;

	CFX_PathData path;

	path.SetPointCount(5);
	path.SetPoint(0, rcFocus.left,  rcFocus.top, FXPT_MOVETO);
	path.SetPoint(1, rcFocus.left,  rcFocus.bottom, FXPT_LINETO);
	path.SetPoint(2, rcFocus.right,  rcFocus.bottom, FXPT_LINETO);
	path.SetPoint(3, rcFocus.right,  rcFocus.top, FXPT_LINETO);
	path.SetPoint(4, rcFocus.left,  rcFocus.top, FXPT_LINETO);

	CFX_GraphStateData gsd;
	gsd.SetDashCount(1);				
	gsd.m_DashArray[0] = 1.0f;
	gsd.m_DashPhase = 0;	

	gsd.m_LineWidth = 1.0f;
	pDevice->DrawPath(&path, pUser2Device, &gsd, 0, ArgbEncode(255,0,0,0), FXFILL_ALTERNATE);
}

void CFSPDF_WidgetAnnotHandler::DrawShadow(FSFormControl *pControl, CFX_RenderDevice* pDevice, const CPDF_Matrix* pUser2Device, FSPDFPage* pPageView)
{
	FXSYS_assert(pControl);
    CPDF_FormControl* pFormControl = ((FSFormControlImp*)pControl)->GetCPDFFormControl();
    if(!pFormControl || !pFormControl->GetField())
    {
		return;
    }
	int nFieldFlags = nFieldFlags = pFormControl->GetField()->GetFieldFlags();
	//pControl should be readonly
	FX_BOOL bIsReadOnly = (nFieldFlags & FORMFIELD_READONLY) == FORMFIELD_READONLY;
	if(bIsReadOnly)
		return;

	//pControl filling should be allowed.
	FX_BOOL bFillingAllowed = TRUE;
	if(pFormControl->GetType() != CPDF_FormField::PushButton)
	{
		FSPDFDoc* pDocument = (FSPDFDoc*)((FSFormControlImp*)pControl)->GetPage()->getDocument();
		FXSYS_assert(pDocument);
		FSPDFDocImp* pDoc = (FSPDFDocImp*)pDocument;
		bFillingAllowed = pDoc->getPDFDocument()->GetUserPermissions() & (FSPDF_PERMISSION_MODIFY | FSPDF_PERMISSION_ANNOTFORM | FSPDF_PERMISSION_FILLFORM) ? 1 : 0; 
		//TODO: This check can be done somewhere else?
	}
	if(!bFillingAllowed)
		return;
	
	//pControl should of be need highlight type
	//TODO: interact with formfiller

	int nFieldType = pFormControl->GetType();
	switch(nFieldType)
	{
	case CPDF_FormField::PushButton:
		nFieldType = FSPDF_FORMFIELDTYPE_PUSHBUTTON;
		break;
	case CPDF_FormField::RadioButton:
		nFieldType = FSPDF_FORMFIELDTYPE_RADIOBUTTON;
		break;
	case CPDF_FormField::CheckBox:
		nFieldType = FSPDF_FORMFIELDTYPE_CHECKBOX;
		break;
	case CPDF_FormField::Text:
	case CPDF_FormField::RichText:
		nFieldType = FSPDF_FORMFIELDTYPE_TEXTFIELD;
		break;
	case CPDF_FormField::ListBox:
		nFieldType = FSPDF_FORMFIELDTYPE_LISTBOX;
		break;
	case CPDF_FormField::ComboBox:
		nFieldType = FSPDF_FORMFIELDTYPE_COMBOBOX;
		break;
	default:
		nFieldType = FSPDF_FORMFIELDTYPE_UNKNOWN;
	}

	FSPDFDoc* pDoc = (FSPDFDoc*)((FSFormControlImp*)pControl)->GetPage()->getDocument();
	FSFormFillerImp* pFormFiller =((FSFormImp*)((FSPDFDocImp*)pDoc)->getForm())->GetFormFiller();
	FX_BOOL showHighlight = pFormFiller->m_isShowHighlight;
	if(showHighlight && pFormControl->GetType() != CPDF_FormField::PushButton)
	{
		FS_ARGB color = pFormFiller->m_hightlightColor;
#ifdef _FS_PDFIUM_BUILD_
		color = ArgbEncode((FS_INT32)pFormFiller->GetAlpha(), color);
#endif
		//FSCRT_RECTF rect = {0,0,0,0};
		//pControl->ST_GetRect(&rect);
		CPDF_Rect pdfRect;// = CPDF_Rect(rect.left,rect.bottom, rect.right, rect.top);
		((FSFormControlImp*)pControl)->ST_GetRect(pdfRect);

		pUser2Device->TransformRect(pdfRect);

		FX_RECT rcDev((int)pdfRect.left,(int)pdfRect.top,(int)pdfRect.right,(int)pdfRect.bottom);
		rcDev.Normalize();
		pDevice->FillRect(&rcDev, color);	
	}
}

FX_BOOL CFSPDF_WidgetAnnotHandler::IsWidgetAppearanceValid(FSFormControl* pControl, CPDF_Annot::AppearanceMode mode)
{
	FXSYS_assert(pControl != NULL);
	//FXSYS_assert(pControl->GetCPDFFormControl() != NULL);

	CPDF_Dictionary* pAnnotDict = (CPDF_Dictionary*)pControl->getDict();

	CPDF_Dictionary* pAP = pAnnotDict->GetDict("AP");
	if (pAP == NULL) return FALSE;

	// Choose the right sub-ap
	const FX_CHAR* ap_entry = "N";
	if (mode == CPDF_Annot::Down)
		ap_entry = "D";
	else if (mode == CPDF_Annot::Rollover)
		ap_entry = "R";
	if (!pAP->KeyExist(ap_entry))
		return FALSE;

	// Get the AP stream or subdirectory
	CPDF_Object* psub = pAP->GetElementValue(ap_entry);
	if (psub == NULL) return FALSE;

    CPDF_FormControl* pFormControl = ((FSFormControlImp*)pControl)->GetCPDFFormControl();
    if (!pFormControl || !pFormControl->GetField())
        return FALSE;
	int nFieldType = pFormControl->GetField()->GetFieldType();
	switch (nFieldType)
	{
	case FIELDTYPE_PUSHBUTTON:
	case FIELDTYPE_COMBOBOX:
	case FIELDTYPE_LISTBOX:
	case FIELDTYPE_TEXTFIELD:
	case FIELDTYPE_SIGNATURE:
		return psub->GetType() == PDFOBJ_STREAM;
	case FIELDTYPE_CHECKBOX:
	case FIELDTYPE_RADIOBUTTON:
		if (psub->GetType() == PDFOBJ_DICTIONARY) 
		{
			CPDF_Dictionary* pSubDict = (CPDF_Dictionary*)psub;
			CFSCRT_STPDFResetApperance apperance(pFormControl);
			return pSubDict->GetStream(apperance.GetAppState()) != NULL;
		}
		else
			return FALSE;
		break;
	}

	return TRUE;
}




/*
FX_BOOL CFSPDF_WidgetAnnotHandler::OnKeyDown(FSAnnot* pAnnot, FS_DWORD nKeyCode, FS_DWORD nFlags)
{
	return TRUE;
}

FX_BOOL CFSPDF_WidgetAnnotHandler::OnString(FSAnnot* pAnnot, FX_WSTR string, FS_DWORD nFlags)
{
	return TRUE;
}
*/


/*

FX_BOOL CFSPDF_WidgetAnnotHandler::HitTest(FSPDFPage *pPageView, FSAnnot* pAnnot, const CPDF_Point& point)
{
	FXSYS_assert(pPageView);
	FXSYS_assert(pAnnot);

	CPDF_Rect rect = GetViewBBox(pPageView, pAnnot);
	return rect.Contains(point.x, point.y);
}

*/

//
//FX_BOOL CFFL_IFormFiller::Annot_HitTest(CPDFSDK_PageView* pPageView,CPDFSDK_Annot* pAnnot, CPDF_Point point)
//{
//	CPDF_Rect rc = pAnnot->GetRect();
//	if(rc.Contains(point.x, point.y))
//		return TRUE;
//	return FALSE;
//}
//
//void CFFL_IFormFiller::OnDelete(CPDFSDK_Annot* pAnnot)
//{
//	if (CFFL_FormFiller* pFormFiller = GetFormFiller(pAnnot, FALSE))
//	{
//		pFormFiller->OnDelete(pAnnot);
//	}
//
//	UnRegisterFormFiller(pAnnot);
//}
//


//void CFFL_IFormFiller::OnButtonUp(CPDFSDK_Widget* pWidget, CPDFSDK_PageView* pPageView, FX_BOOL& bReset, FX_BOOL& bExit,FSCRT_UINT nFlag)
//{
//	ASSERT(pWidget != NULL);
//	
//	if (!m_bNotifying)
//	{
//		if (pWidget->GetAAction(CPDF_AAction::ButtonUp))
//		{
//			m_bNotifying = TRUE;
//			int nAge = pWidget->GetAppearanceAge();
//			int nValueAge = pWidget->GetValueAge();
//			
//			ASSERT(pPageView != NULL);
//// 			CReader_DocView* pDocView = pPageView->GetDocView();
//// 			ASSERT(pDocView != NULL);
//			CPDFSDK_Document* pDocument = pPageView->GetSDKDocument();
//			ASSERT(pDocument != NULL);
//			
//			PDFSDK_FieldAction fa;
//			fa.bModifier = m_pApp->FFI_IsCTRLKeyDown(nFlag);
// 			fa.bShift = m_pApp->FFI_IsSHIFTKeyDown(nFlag);
//
//			pWidget->OnAAction(CPDF_AAction::ButtonUp, fa, pPageView);
//			m_bNotifying = FALSE;
//			
//			if (!IsValidAnnot(pPageView, pWidget))
//			{
//				bExit = TRUE;
//				return;
//			}
//			
//			if (nAge != pWidget->GetAppearanceAge())
//			{
//				if (CFFL_FormFiller* pFormFiller = GetFormFiller(pWidget, FALSE))
//				{
//					pFormFiller->ResetPDFWindow(pPageView, nValueAge == pWidget->GetValueAge());
//				}
//				
//				bReset = TRUE;
//			}
//		}
//	}
//}
//
//
//void CFFL_IFormFiller::OnDeSelected(CPDFSDK_Annot* pAnnot)
//{
//	ASSERT(pAnnot != NULL);
//	ASSERT(pAnnot->GetPDFAnnot()->GetSubType() == "Widget");
//
//	if (CFFL_FormFiller* pFormFiller = GetFormFiller(pAnnot, FALSE))
//	{
//		pFormFiller->OnDeSelected(pAnnot);
//	}
//}
//
//void CFFL_IFormFiller::OnSelected(CPDFSDK_Annot* pAnnot)
//{
//	ASSERT(pAnnot != NULL);
//	ASSERT(pAnnot->GetPDFAnnot()->GetSubType() == "Widget");
//
//	if (CFFL_FormFiller* pFormFiller = GetFormFiller(pAnnot, FALSE))
//	{
//		pFormFiller->OnSelected(pAnnot);
//	}
//}
//
//FX_BOOL CFFL_IFormFiller::OnSetFocus(CPDFSDK_Annot* pAnnot,FSCRT_UINT nFlag)
//{
//
//	ASSERT(pAnnot != NULL);
//	ASSERT(pAnnot->GetPDFAnnot()->GetSubType() == "Widget");
//
//	if (!m_bNotifying)
//	{
//		CPDFSDK_Widget* pWidget = (CPDFSDK_Widget*)pAnnot;
// 		if (pWidget->GetAAction(CPDF_AAction::GetFocus))
// 		{
//  			m_bNotifying = TRUE;
//			int nAge = pWidget->GetAppearanceAge();
//			int nValueAge = pWidget->GetValueAge();
// 			pWidget->ClearAppModified();
// 
// 
// 			CPDFSDK_PageView* pPageView = pAnnot->GetPageView();
// 			ASSERT(pPageView != NULL);
// 			
// 			PDFSDK_FieldAction fa;
//			fa.bModifier = m_pApp->FFI_IsCTRLKeyDown(nFlag);
// 			fa.bShift = m_pApp->FFI_IsSHIFTKeyDown(nFlag);
//
// 
// 			CFFL_FormFiller* pFormFiller = GetFormFiller(pWidget, TRUE);
// 			ASSERT(pFormFiller != NULL);
// 			pFormFiller->GetActionData(pPageView, CPDF_AAction::GetFocus, fa);
// 
// 			pWidget->OnAAction(CPDF_AAction::GetFocus, fa, pPageView);
// 			m_bNotifying = FALSE;
// 			
// //			if (!IsValidAnnot(m_pApp, pDocument, pDocView, pPageView, pAnnot)) return FALSE;
// 
// 			if (pWidget->IsAppModified())
// 			{
// 				if (CFFL_FormFiller* pFormFiller = GetFormFiller(pWidget, FALSE))
// 				{
// 					pFormFiller->ResetPDFWindow(pPageView, nValueAge == pWidget->GetValueAge());
// 				}
// 			}
//		}
//	}
//	
//	if (CFFL_FormFiller* pFormFiller = GetFormFiller(pAnnot, TRUE))
//	{
//		if (pFormFiller->OnSetFocus(pAnnot, nFlag))
//		{
//// 			CReader_Page* pPage = pAnnot->GetPage();
//// 			ASSERT(pPage != NULL);
//// 			pPage->UpdateAllViews(NULL, pAnnot);
//// 
//// 			CPDFSDK_Document* pDocument = pPage->GetDocument();
//// 			ASSERT(pDocument != NULL);
//// 
//// 			pDocument->SetTopmostAnnot(pAnnot);
//
//			return TRUE;
//		}
//		else
//			return FALSE;
//	}
//
//	return TRUE;
//}
//
//FX_BOOL CFFL_IFormFiller::OnKillFocus(CPDFSDK_Annot* pAnnot,FSCRT_UINT nFlag)
//{
//	ASSERT(pAnnot != NULL);
//	ASSERT(pAnnot->GetPDFAnnot()->GetSubType() == "Widget");
//
//	if (CFFL_FormFiller* pFormFiller = GetFormFiller(pAnnot, FALSE))
//	{
//		if (pFormFiller->OnKillFocus(pAnnot, nFlag))
//		{
// 			if (!m_bNotifying)
// 			{
// 				CPDFSDK_Widget* pWidget = (CPDFSDK_Widget*)pAnnot;
// 				if (pWidget->GetAAction(CPDF_AAction::LoseFocus))
// 				{
// 					m_bNotifying = TRUE;
// 					pWidget->ClearAppModified();
// 
// 					CPDFSDK_PageView* pPageView = pWidget->GetPageView();
// 					ASSERT(pPageView != NULL);
// 
// 					PDFSDK_FieldAction fa;
//					fa.bModifier = m_pApp->FFI_IsCTRLKeyDown(nFlag);
// 					fa.bShift = m_pApp->FFI_IsSHIFTKeyDown(nFlag);
// 
// 					pFormFiller->GetActionData(pPageView, CPDF_AAction::LoseFocus, fa);
// 
// 					pWidget->OnAAction(CPDF_AAction::LoseFocus, fa, pPageView);
// 					m_bNotifying = FALSE;
// 
// 					//if (!IsValidAnnot(m_pApp, pDocument, pDocView, pPageView, pAnnot)) return FALSE;
// 				}
// 			}
//		}
//		else
//			return FALSE;
//	}
//
//
//	return TRUE;
//}
//
//FX_BOOL CFFL_IFormFiller::IsVisible(CPDFSDK_Widget* pWidget)
//{
//	return pWidget->IsVisible();
//}
//
//FX_BOOL CFFL_IFormFiller::IsReadOnly(CPDFSDK_Widget* pWidget)
//{
//	ASSERT(pWidget != NULL);
//
//	int nFieldFlags = pWidget->GetFieldFlags();
//
//	return (nFieldFlags & FIELDFLAG_READONLY) == FIELDFLAG_READONLY;
//}
//
//FX_BOOL CFFL_IFormFiller::IsFillingAllowed(CPDFSDK_Widget* pWidget)
//{
//	ASSERT(pWidget != NULL);
//
//	if (pWidget->GetFieldType() == FIELDTYPE_PUSHBUTTON)
//		return TRUE;
// 	else
// 	{
// 		CPDF_Page* pPage = pWidget->GetPDFPage();
// 		ASSERT(pPage != NULL);
// 
// 		CPDF_Document* pDocument = pPage->m_pDocument;
// 		ASSERT(pDocument != NULL);
// 
//		FX_DWORD dwPermissions = pDocument->GetUserPermissions();
// 		return (dwPermissions&FPDFPERM_FILL_FORM) || 
// 				(dwPermissions&FPDFPERM_ANNOT_FORM) || 
// 			(dwPermissions&FPDFPERM_MODIFY);
// 	}
//	return TRUE;	
//}
//
//
//void CFFL_IFormFiller::SetFocusAnnotTab(CPDFSDK_Annot* pWidget, FX_BOOL bSameField, FX_BOOL bNext)
//{
//}
//

//
//void CFFL_IFormFiller::OnSetWindowRect(void* pPrivateData, const CPDF_Rect & rcWindow)
//{
//	ASSERT(pPrivateData != NULL);
//
//	CFFL_PrivateData* pData = (CFFL_PrivateData*)pPrivateData;
//
//	if (CFFL_FormFiller* pFormFiller = GetFormFiller(pData->pWidget, TRUE))
//	{
//
//		CPDF_Rect rcOld = pFormFiller->PWLtoFFL(pFormFiller->GetWindowRect(pData->pPageView));
//		CPDF_Rect rcNew = pFormFiller->PWLtoFFL(rcWindow);
//		pFormFiller->SetWindowRect(pData->pPageView, rcWindow);
//
//		CPDF_Rect unRect = rcOld;
//		unRect.Union(rcNew);
//		//FX_RECT rcRect = unRect.GetOutterRect();
//		unRect.left = (FX_FLOAT)(unRect.left - 0.5);
//		unRect.right = (FX_FLOAT)(unRect.right + 0.5);
//		unRect.top = (FX_FLOAT)(unRect.top + 0.5);
//		unRect.bottom = (FX_FLOAT)(unRect.bottom -0.5);
//		m_pApp->FFI_Invalidate((FSPDF_PAGE)pData->pWidget->GetPDFPage(), unRect.left, unRect.top, unRect.right, unRect.bottom);
//	}
//}
//
//void CFFL_IFormFiller::OnKeyStroke(FX_BOOL bEditOrList, void* pPrivateData, FX_INT32 nKeyCode, CFX_WideString& strChange, 
//								   const CFX_WideString& strChangeEx, FX_BOOL bKeyDown, 
//								   FX_BOOL & bRC, FX_BOOL & bExit)
//{
//	ASSERT(pPrivateData != NULL);
//	CFFL_PrivateData* pData = (CFFL_PrivateData*)pPrivateData;
//	ASSERT(pData->pWidget != NULL);
//
//	CFFL_FormFiller* pFormFiller = GetFormFiller(pData->pWidget, FALSE);
//	ASSERT(pFormFiller != NULL);
//
//	pFormFiller->OnKeyStroke(bKeyDown);
//}
//
//void CFFL_IFormFiller::OnKeyStrokeCommit(CPDFSDK_Widget* pWidget, CPDFSDK_PageView* pPageView, FX_BOOL& bRC, FX_BOOL& bExit, FX_DWORD nFlag)
//{
//	if (!m_bNotifying)
//	{
//		ASSERT(pWidget != NULL);
//		if (pWidget->GetAAction(CPDF_AAction::KeyStroke))
//		{
//			m_bNotifying = TRUE;
//			pWidget->ClearAppModified();
//
//			ASSERT(pPageView != NULL);
//			CPDFSDK_Document* pDocument = pPageView->GetSDKDocument();
//			ASSERT(pDocument != NULL);
//
//			PDFSDK_FieldAction fa;
//			fa.bModifier = m_pApp->FFI_IsCTRLKeyDown(nFlag);
// 			fa.bShift = m_pApp->FFI_IsSHIFTKeyDown(nFlag);
//			fa.bWillCommit = TRUE;
//			fa.nCommitKey = GetCommitKey();
//			fa.bKeyDown = GetKeyDown();
//			fa.bRC = TRUE;
//
//			CFFL_FormFiller* pFormFiller = GetFormFiller(pWidget, FALSE);
//			ASSERT(pFormFiller != NULL);
//
//			pFormFiller->GetActionData(pPageView, CPDF_AAction::KeyStroke, fa);
//			pFormFiller->SaveState(pPageView);
//
//			PDFSDK_FieldAction faOld = fa;
//			pWidget->OnAAction(CPDF_AAction::KeyStroke, fa, pPageView);
//
//			bRC = fa.bRC;
//
//			m_bNotifying = FALSE;
//		}
//	}
//}
//
//void CFFL_IFormFiller::OnValidate(CPDFSDK_Widget* pWidget, CPDFSDK_PageView* pPageView, FX_BOOL& bRC, FX_BOOL& bExit, FX_DWORD nFlag)
//{
//	if (!m_bNotifying)
//	{
//		ASSERT(pWidget != NULL);
//		if (pWidget->GetAAction(CPDF_AAction::Validate))
//		{
//			m_bNotifying = TRUE;
//			pWidget->ClearAppModified();
//
//			ASSERT(pPageView != NULL);
//			CPDFSDK_Document* pDocument = pPageView->GetSDKDocument();
//			ASSERT(pDocument != NULL);
//
//			PDFSDK_FieldAction fa;
//			fa.bModifier = m_pApp->FFI_IsCTRLKeyDown(nFlag);
// 			fa.bShift = m_pApp->FFI_IsSHIFTKeyDown(nFlag);
//			fa.bKeyDown = GetKeyDown();
//			fa.bRC = TRUE;
//
//			CFFL_FormFiller* pFormFiller = GetFormFiller(pWidget, FALSE);
//			ASSERT(pFormFiller != NULL);
//
//			pFormFiller->GetActionData(pPageView, CPDF_AAction::Validate, fa);
//			pFormFiller->SaveState(pPageView);
//
//			PDFSDK_FieldAction faOld = fa;
//			pWidget->OnAAction(CPDF_AAction::Validate, fa, pPageView);
//
//			bRC = fa.bRC;
//
//			m_bNotifying = FALSE;
//		}
//	}
//}
//
//void CFFL_IFormFiller::OnCalculate(CPDFSDK_Widget* pWidget, CPDFSDK_PageView* pPageView, FX_BOOL& bExit, FX_DWORD nFlag)
//{
//	if (!m_bNotifying)
//	{
//		ASSERT(pWidget != NULL);
//		ASSERT(pPageView != NULL);
//		CPDFSDK_Document* pDocument = pPageView->GetSDKDocument();
//		ASSERT(pDocument != NULL);
//
//		CPDFSDK_InterForm* pInterForm = (CPDFSDK_InterForm*)pDocument->GetInterForm();
//		ASSERT(pInterForm != NULL);
//
//		pInterForm->OnCalculate(pWidget->GetFormField());
//
////		bExit = !IsValidAnnot(m_pApp, pDocument, pDocView, pPageView, pWidget);
//
//		m_bNotifying = FALSE;
//	}
//}
//
//void CFFL_IFormFiller::OnFormat(CPDFSDK_Widget* pWidget, CPDFSDK_PageView* pPageView, FX_BOOL& bExit, FX_DWORD nFlag)
//{
//	if (!m_bNotifying)
//	{
//		ASSERT(pWidget != NULL);
//		ASSERT(pPageView != NULL);
//// 		CReader_DocView* pDocView = pPageView->GetDocView();
//// 		ASSERT(pDocView != NULL);
//		CPDFSDK_Document* pDocument = pPageView->GetSDKDocument();
//		ASSERT(pDocument != NULL);
//
//		CPDFSDK_InterForm* pInterForm = (CPDFSDK_InterForm*)pDocument->GetInterForm();
//		ASSERT(pInterForm != NULL);
//
//		FX_BOOL bFormated = FALSE;
//		CFX_WideString sValue = pInterForm->OnFormat(pWidget->GetFormField(), GetCommitKey(), bFormated);
//
////		bExit = !IsValidAnnot(m_pApp, pDocument, pDocView, pPageView, pWidget);
//
//		if (bExit) return;
//
//		if (bFormated)
//		{
//			pInterForm->ResetFieldAppearance(pWidget->GetFormField(), sValue, TRUE);
//			pInterForm->UpdateField(pWidget->GetFormField());
//		}
//
//		m_bNotifying = FALSE;
//	}
//}
//
//int CFFL_IFormFiller::GetCommitKey()
//{
////	MSG msg = CFFL_IFormFiller::GetLastMessage();
//
//	int nCommitKey = 0;
//// 	switch (msg.message)
//// 	{
//// 	case WM_LBUTTONDOWN:
//// 	case WM_LBUTTONUP:
//// 		nCommitKey = 1;
//// 		break;
//// 	case WM_KEYDOWN:
//// 		switch (msg.wParam)
//// 		{
//// 		case VK_RETURN:
//// 			nCommitKey = 2;
//// 			break;
//// 		case VK_TAB:
//// 			nCommitKey = 3;
//// 			break;
//// 		}
//// 		break;
//// 	}
//
//	return nCommitKey;
//}
//
//FX_BOOL CFFL_IFormFiller::GetKeyDown()
//{
//	return TRUE;
//}
//
//FX_BOOL CFFL_IFormFiller::IsValidAnnot(CPDFSDK_PageView* pPageView, CPDFSDK_Annot* pAnnot)
//{
//
//	ASSERT(pPageView != NULL);
//	ASSERT(pAnnot != NULL);
//
//	if(pPageView)
//		return pPageView->IsValidAnnot(pAnnot->GetPDFAnnot());
//	else
//		return FALSE;
//}
//
//void CFFL_IFormFiller::OnBeforeKeyStroke(FX_BOOL bEditOrList, void* pPrivateData, FX_INT32 nKeyCode,
//											  CFX_WideString & strChange, const CFX_WideString& strChangeEx, 
//											  int nSelStart, int nSelEnd,
//										FX_BOOL bKeyDown, FX_BOOL & bRC, FX_BOOL & bExit, FX_DWORD nFlag)
//{
//	ASSERT(pPrivateData != NULL);
//	CFFL_PrivateData* pData = (CFFL_PrivateData*)pPrivateData;
//	ASSERT(pData->pWidget != NULL);
//	
//	CFFL_FormFiller* pFormFiller = GetFormFiller(pData->pWidget, FALSE);
//	ASSERT(pFormFiller != NULL);
//	
//	if (!m_bNotifying)
//	{
//		if (pData->pWidget->GetAAction(CPDF_AAction::KeyStroke))
//		{
//			m_bNotifying = TRUE;
//			int nAge = pData->pWidget->GetAppearanceAge();
//			int nValueAge = pData->pWidget->GetValueAge();
//
//			ASSERT(pData->pPageView != NULL);
//			CPDFSDK_Document* pDocument  = pData->pPageView->GetSDKDocument();
//			
//			PDFSDK_FieldAction fa;
// 			fa.bModifier = m_pApp->FFI_IsCTRLKeyDown(nFlag);
// 			fa.bShift = m_pApp->FFI_IsSHIFTKeyDown(nFlag);
//			fa.sChange = strChange;
//			fa.sChangeEx = strChangeEx;
//			fa.bKeyDown = bKeyDown;
//			fa.bWillCommit = FALSE;
//			fa.bRC = TRUE;
//			fa.nSelStart = nSelStart;
//			fa.nSelEnd = nSelEnd;
//
//
//			pFormFiller->GetActionData(pData->pPageView, CPDF_AAction::KeyStroke, fa);
//			pFormFiller->SaveState(pData->pPageView);
//			
//			if (pData->pWidget->OnAAction(CPDF_AAction::KeyStroke, fa, pData->pPageView))
//			{
//				if (!IsValidAnnot(pData->pPageView, pData->pWidget))
//				{
//					bExit = TRUE;
//					m_bNotifying = FALSE;
//					return;
//				}
//				
//				if (nAge != pData->pWidget->GetAppearanceAge())
//				{
//					CPWL_Wnd* pWnd = pFormFiller->ResetPDFWindow(pData->pPageView, nValueAge == pData->pWidget->GetValueAge());
//					pData = (CFFL_PrivateData*)pWnd->GetAttachedData();
//					bExit = TRUE;
//				}
//				
//				if (fa.bRC)
//				{
//					pFormFiller->SetActionData(pData->pPageView, CPDF_AAction::KeyStroke, fa);
//					bRC = FALSE;
//				}
//				else
//				{
//					pFormFiller->RestoreState(pData->pPageView);
//					bRC = FALSE;
//				}
//				
//				if (pDocument->GetFocusAnnot() != pData->pWidget)
//				{
//					pFormFiller->CommitData(pData->pPageView,nFlag);
//					bExit = TRUE;
//				}
//			}
//			else
//			{			
//				if (!IsValidAnnot(pData->pPageView, pData->pWidget))
//				{
//					bExit = TRUE;
//					m_bNotifying = FALSE;
//					return;
//				}
//			}
//			
//			m_bNotifying = FALSE;
//		}
//	}
//}
//
//void CFFL_IFormFiller::OnAfterKeyStroke(FX_BOOL bEditOrList, void* pPrivateData, FX_BOOL & bExit,FX_DWORD nFlag) 
//{
//	ASSERT(pPrivateData != NULL);
//	CFFL_PrivateData* pData = (CFFL_PrivateData*)pPrivateData;
//	ASSERT(pData->pWidget != NULL);
//	
//	CFFL_FormFiller* pFormFiller = GetFormFiller(pData->pWidget, FALSE);
//	ASSERT(pFormFiller != NULL);
//	
//	if (!bEditOrList)
//		pFormFiller->OnKeyStroke(bExit);
//}


#endif  //defined(_FSDK_ALL_) || defined(_PDFFORMFILLER_) 
