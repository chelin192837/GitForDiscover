#include "../../inc/fpdf_define.h"
#include "../fpdf_stactionhandler.h"
#include "ffl_widget.h"
#include "../fpdf_ltwidgethandler.h"
#include "../../widget/pdfwindow/include/fx_systemhandler.h"

#ifndef FIELDFLAG_COMMITONSELCHANGE
#define FIELDFLAG_COMMITONSELCHANGE			(1<<26)
#endif

CFFL_Widget::CFFL_Widget(FSFormControl* pControl, CFSPDF_WidgetAnnotHandler* pWidgetHandler)
	: m_pControl(pControl)
	, m_pWidgetHandler(pWidgetHandler)
	, m_bActivated(FALSE)
{

}

CFFL_Widget::~CFFL_Widget()
{
	FX_POSITION pos = m_Map.GetStartPosition();
	while (pos)
	{
		FSPDFPage * pPageView = NULL;
		IFSPDF_Widget* pWidget = NULL;
		m_Map.GetNextAssoc(pos, pPageView, pWidget);

		if (pWidget)
		{
			//TODO: Remove private data
			pWidget->Release();
		}
	}
	m_Map.RemoveAll();
}

IFSPDF_Widget* CFFL_Widget::GetWidget(FSPDFPage* pPageView, FX_BOOL bNewIfNotExist)
{
	if(!pPageView) return NULL;

	IFSPDF_Widget* pWidget = NULL;
	m_Map.Lookup(pPageView, pWidget);

	if(!pWidget && bNewIfNotExist)
	{
		pWidget = InitWidget();
		if(pWidget)
		{
			m_Map.SetAt(pPageView, pWidget);
		}
	}

	return pWidget;
}


void CFFL_Widget::ReleaseWidget(FSPDFPage* pPageView)
{
	IFSPDF_Widget* pWidget = NULL;

	if(!pPageView) return;
	if(m_Map.Lookup(pPageView, pWidget))
	{
		if(pWidget)
			pWidget->Release();
		m_Map.RemoveKey(pPageView);
	}
}

void CFFL_Widget::FillCreateParam(CFSPDF_WidgetProperties& prop)
{
	FXSYS_assert(m_pControl);
	//prop.m_rtWidget;
	{
		CPDF_Rect rectAnnot = ((FSFormControlImp*)m_pControl)->GetCPDFFormControl()->GetRect();

		FX_FLOAT fWidth = rectAnnot.right - rectAnnot.left;
		FX_FLOAT fHeight = rectAnnot.top - rectAnnot.bottom;

		FX_INT32 nRotation = 0;
		((FSFormControlImp*)m_pControl)->GetMKRotation(&nRotation);
		if ((nRotation / 90) & 0x01)
			prop.m_rtWidget = CPDF_Rect(0,0,fHeight,fWidth);
		else
			prop.m_rtWidget = CPDF_Rect(0,0,fWidth,fHeight);
	}

	{
		FS_DWORD fieldFlags = ((FSFormControlImp*)m_pControl)->GetCPDFFormControl()->GetField()->GetFieldFlags();

		if (fieldFlags & FORMFIELD_READONLY)
			prop.m_dwStates |= FPDFW_WGTSTATE_Disabled;
	}

	prop.m_dwStyles = FPDFW_WGTSTYLE_Border;

	//prop.
	FX_ARGB color = 0xFF000000;

	CPDF_FormControl* pControl = ((FSFormControlImp*)m_pControl)->GetCPDFFormControl();
	FXSYS_assert(pControl);

	int iColorType = 0;

	prop.m_sBackgroundColor = pControl->GetBackgroundColor(iColorType);
	if(iColorType==0)
		prop.m_sBackgroundColor = 0; //transparent.

	prop.m_sBorderColor = pControl->GetBorderColor(iColorType);
	if(iColorType==0)
		prop.m_sBorderColor = 0; //transparent.

	CPDF_DefaultAppearance da = pControl->GetDefaultAppearance();

	prop.m_sTextColor = 0;
	if(da.HasColor())
	{
		da.GetColor(color, iColorType);
		if(iColorType)
			prop.m_sTextColor = color;
	}
	prop.m_sTextStrokeColor = 0;
	if(da.HasColor(TRUE))
	{
		da.GetColor(color, iColorType, TRUE);
		if(iColorType)
			prop.m_sTextStrokeColor = color;
	}

	CFX_ByteString csFont = "";
	FX_FLOAT fFontSize = 0.0f;
	da.GetFont(csFont, fFontSize);

	prop.m_fFontSize = fFontSize;

	FSBorderInfo pBorderInfo = m_pControl->getBorderInfo();
	prop.m_dwBorderWidth = pBorderInfo.width;
	prop.m_nBorderStyle = pBorderInfo.style;   //TODO: Define equivalent names for FPDFANNOT_BORDER_Solid, ...
	switch(prop.m_nBorderStyle)
	{
	case FPDFANNOT_BORDER_Solid:
		break;
	case FPDFANNOT_BORDER_Dashed:
		prop.m_nDash = 3;
		prop.m_nDashGap = 3;
		prop.m_nDashPhase = 0;
		break;
	case FPDFANNOT_BORDER_Beveled:
		prop.m_dwBorderWidth *= 2;
		break;
	case FPDFANNOT_BORDER_Inset:
		prop.m_dwBorderWidth *= 2;
		break;
	case FPDFANNOT_BORDER_Underline:
		break;
	}
	//TODO: cp.pSystemHandler = m_pApp->GetSysHandler();

	prop.m_pWidget = this;
	prop.m_pSystemHandler = GetWidgetHandler()->GetSystemHandler();
	FXSYS_assert(prop.m_pSystemHandler);

//	prop.m_pFontMap = prop.m_pSystemHandler->GetFontMap();

	//TODO: prop.m_pAttachedData
}

void CFFL_Widget::OnMouseEnter(FSPDFPage *pPageView)
{
	//implement by subclass
}

void CFFL_Widget::OnMouseExit(FSPDFPage *pPageView)
{
	//implement by subclass
	//TODO: Endtimer
	//TOOD: //RemoveHint
}

FX_BOOL CFFL_Widget::OnLButtonDown (FSPDFPage *pPageView, FS_DWORD nFlags, const CFX_PointF& point)
{
	IFSPDF_Widget* widget = GetWidget(pPageView);
	if(!widget) return FALSE;

	Activate();
	Invalidate(pPageView);

	return widget->OnLButtonDown(nFlags, PDFPointToWidget(pPageView, point));
}

FX_BOOL CFFL_Widget::OnLButtonUp (FSPDFPage *pPageView, FS_DWORD nFlags, const CFX_PointF& point)
{
	IFSPDF_Widget* widget = GetWidget(pPageView, FALSE);
	if(!widget) return FALSE;
	if(GetWidgetHandler()->GetFocusControl() != m_pControl)
		ExitFiller(pPageView, FALSE);

	if (m_pControl && ((FSFormControlImp*)m_pControl)->GetCPDFFormControl())
	{
		CPDF_FormField* pFormField = ((FSFormControlImp*)m_pControl)->GetCPDFFormControl()->GetField();
		if ((pFormField->GetType() == CPDF_FormField::ListBox || pFormField->GetType() == CPDF_FormField::ComboBox) && (pFormField->GetFieldFlags() & FIELDFLAG_COMMITONSELCHANGE))
			CommitData(pPageView, nFlags);
	}

	return widget->OnLButtonUp(nFlags, PDFPointToWidget(pPageView, point));
}

FX_BOOL CFFL_Widget::OnLButtonDblClk (FSPDFPage *pPageView, FS_DWORD nFlags, const CFX_PointF& point)
{
	IFSPDF_Widget* widget = GetWidget(pPageView, FALSE);
	if(!widget) return FALSE;
	return widget->OnLButtonDblClk(nFlags, PDFPointToWidget(pPageView, point));
}

FX_BOOL CFFL_Widget::OnMouseMove (FSPDFPage *pPageView, FS_DWORD nFlags, const CFX_PointF& point)
{
	/* TODO: Phantom code
	if (m_ptOldPos != point)
	{
		EndTimer();
		ASSERT(m_pWidget != NULL);
		m_pWidget->HideHint();
		BeginTimer(FFL_HINT_ELAPSE);
		m_ptOldPos = point;
	}
	*/
	IFSPDF_Widget* widget = GetWidget(pPageView, FALSE);
	if(!widget) return FALSE;
	return widget->OnMouseMove(nFlags, PDFPointToWidget(pPageView, point));
}

FX_BOOL CFFL_Widget::OnMouseWheel (FSPDFPage *pPageView, FS_DWORD nFlags, short zDelta, const CFX_PointF& point)
{
	/* TODO: Phantom code
	EndTimer();
	if (m_pWidget)
	m_pWidget->HideHint();
	*/
	if(!IsActivated()) return FALSE;

	IFSPDF_Widget* widget = GetWidget(pPageView, TRUE);
	if(!widget) return FALSE;
	FX_BOOL bRet = widget->OnMouseWheel(nFlags, zDelta, PDFPointToWidget(pPageView, point));
	/*TODO: Phantom code
	if (bRet)
	{
		BeginTimer(FFL_HINT_ELAPSE);
	}
	*/
	return bRet;
}


FX_BOOL CFFL_Widget::OnRButtonDown (FSPDFPage *pPageView, FS_DWORD nFlags, const CFX_PointF& point)
{
	IFSPDF_Widget* widget = GetWidget(pPageView, TRUE);
	if(!widget) return FALSE;
	return widget->OnRButtonDown(nFlags, PDFPointToWidget(pPageView, point));	
}

FX_BOOL CFFL_Widget::OnRButtonUp (FSPDFPage *pPageView, FS_DWORD nFlags, const CFX_PointF& point)
{
	IFSPDF_Widget* widget = GetWidget(pPageView, FALSE);
	if(!widget) return FALSE;
	return widget->OnRButtonUp(nFlags, PDFPointToWidget(pPageView, point));
}

FX_BOOL CFFL_Widget::OnRButtonDblClk (FSPDFPage *pPageView, FS_DWORD nFlags, const CFX_PointF& point)
{
	if(!IsActivated()) return FALSE;

	IFSPDF_Widget* widget = GetWidget(pPageView, FALSE);
	if(!widget) return FALSE;
	return widget->OnRButtonDblClk(nFlags, PDFPointToWidget(pPageView, point));
}

FX_BOOL CFFL_Widget::OnKeyDown (FS_DWORD nKeyCode, FS_DWORD nFlags)
{
	if(!IsActivated()) return FALSE;

	IFSPDF_Widget* widget = GetWidget(GetCurrentPageView(), FALSE);
	if(!widget) return FALSE;
	return widget->OnKeyDown(nKeyCode, nFlags);

}

FX_BOOL CFFL_Widget::OnKeyUp (FS_DWORD nKeyCode, FS_DWORD nFlags)
{
	if(!IsActivated()) return FALSE;

	IFSPDF_Widget* widget = GetWidget(GetCurrentPageView(), FALSE);
	if(!widget) return FALSE;
	return widget->OnKeyUp(nKeyCode, nFlags);
}

FX_BOOL CFFL_Widget::OnChar (FS_DWORD nKeyCode, FS_DWORD nFlags)
{
	if(!IsActivated()) return FALSE;

	IFSPDF_Widget* widget = GetWidget(GetCurrentPageView(), FALSE);
	if(!widget) return FALSE;
	return widget->OnChar(nKeyCode, nFlags);
}

FX_BOOL CFFL_Widget::OnString (FX_WSTR string,	FS_DWORD nFlags)
{
	if(!IsActivated()) return FALSE;

	IFSPDF_Widget* widget = GetWidget(GetCurrentPageView());
	if(!widget) return FALSE;
	return widget->OnString(string, nFlags);
}


CFX_PointF CFFL_Widget::PDFPointToWidget(FSPDFPage* pPageView, CFX_PointF point) //mostly type conversion
{
	CFX_PointF  pagePoint = point;
	//((FSPDFPageImp*)pPageView)->DeviceToPagePoint(point, pagePoint);
	CPDF_Point pt(FFLPointToWidget(GetCurrentMatrix(), CPDF_Point(pagePoint.x, pagePoint.y)));
	CFX_PointF ret; ret.Set((FX_FLOAT)pt.x, (FX_FLOAT)pt.y);
	return ret;
}

CPDF_Matrix CFFL_Widget::GetCurrentMatrix()
{
	CPDF_Matrix mt;

	FXSYS_assert(m_pControl);

	CPDF_Rect rcDA = ((FSFormControlImp*)m_pControl)->GetCPDFFormControl()->GetRect();

	FS_INT32 rotation;
	((FSFormControlImp*)m_pControl)->GetMKRotation(&rotation);
	switch (rotation)
	{
	default:
	case 0:		
		mt = CPDF_Matrix(1,0,0,1,0,0);
		break;
	case 90:
		mt = CPDF_Matrix(0,1,-1,0,rcDA.right - rcDA.left,0);
		break;
	case 180:
		mt = CPDF_Matrix(-1,0,0,-1,rcDA.right - rcDA.left,rcDA.top - rcDA.bottom);
		break;
	case 270:
		mt = CPDF_Matrix(0,-1,1,0,0,rcDA.top - rcDA.bottom);
		break;
	}
	mt.e += rcDA.left;
	mt.f += rcDA.bottom;

	return mt;
}


FX_BOOL CFFL_Widget::CommitData (FSPDFPage* pPageView, FS_DWORD nFlags)
{
	if (IsDataChanged(pPageView))
	{
		// TODO: Cleanup these 
		//CFFL_IFormFiller* pIFormFiller = CFFL_Module::GetFormFiller(m_pApp);
		//CFFL_IFormFiller* pIFormFiller = m_pApp->GetIFormFiller();//NULL;
		//ASSERT(pIFormFiller != NULL)
			
		FSFormFillerImp* pFormFiller = (FSFormFillerImp*)((FSPDFPageImp*)pPageView)->getFormFiller();
		FSForm* pForm = NULL;
		pFormFiller->GetForm((FSForm**)&pForm);
		CFSCRT_STPDFInterForm* pInterForm = ((FSFormImp*)pForm)->ST_GetSTInterForm();

		FX_BOOL bRC = TRUE;
		FX_BOOL bExit = FALSE;

		CPDF_FormField* pFormField = ((FSFormControlImp*)m_pControl)->GetCPDFFormControl()->GetField();

		FSPDF_FieldActionParam param;
		GetActionData(pPageView, CPDF_AAction::KeyStroke, param);

		
		param.bWillCommit = TRUE;
		pInterForm->OnKeyStrokeCommit(pFormField, param, bRC);
		if (bExit) return TRUE;
		if (!bRC)
		{
		this->ResetPDFWindow(pPageView, FALSE);
		return TRUE;
		}

		GetActionData(pPageView, CPDF_AAction::Validate, param);

		pInterForm->OnValidate(pFormField, param, bRC);
		if (bExit) return TRUE;
		if (!bRC)
		{
		this->ResetPDFWindow(pPageView, FALSE);
		return TRUE;
		}

		CPDF_FormField *pField = ((FSFormControlImp*)m_pControl)->GetCPDFFormControl()->GetField();
		CFX_WideString sOldValue;
		if (pField)
		{
			sOldValue = pField->GetValue();
		}
		
		SaveData(pPageView);
		
		pInterForm->OnCalculate(pFormField);
		if (bExit) return TRUE;

		FX_BOOL bFormat = FALSE;
		FX_BOOL bFormatAction = FALSE;
		CFX_WideString sValue = pInterForm->OnFormat(pFormField, 0, bFormat,&bFormatAction);
		if(!bFormat && bFormatAction)
		{
			IFSPDF_Widget* widget =GetWidget(pPageView, FALSE);
			if (widget)
			{
				widget->SetText(sOldValue);
				SaveData(pPageView);
			}

			if (!sOldValue.IsEmpty())
			{
				pInterForm->OnCalculate(pFormField);
				if (bExit) return TRUE;
				CFX_WideString sValue = pInterForm->OnFormat(pFormField, 0, bFormat);
				UpdateField(pFormField, (FX_LPCWSTR)sValue);
			}
		}
		else
		{
			UpdateField(pFormField, (FX_LPCWSTR)sValue);
		}
	}
	return TRUE;
}

FX_BOOL CFFL_Widget::IsDataChanged (FSPDFPage* pPageView)
{
	return FALSE; //implement in subclass
}

void CFFL_Widget::SaveData (FSPDFPage* pPageView)
{
	return; //implement in subclass
}

FSPDFPage* CFFL_Widget::GetCurrentPageView() const
{
	FSPDFPage *page = m_pControl ? (FSPDFPage*)m_pControl->getPage() : NULL;
	FSPDFDoc *document = page ? (FSPDFDoc*)page->getDocument() : NULL;

	FS_INT32 pageIndex = page->getIndex(); //document->GetPageIndex(page, &pageIndex);

	CFSPDF_WidgetAnnotHandler *pAnnotHandler = GetWidgetHandler();
	//CFSPDF_ViewContainer *pViewContainer; pAnnotHandler->GetViewContainer(&pViewContainer);
	
	//FSPDFPage* pPageView = NULL;
	//document->GetPageView(pageIndex, &pPageView);
	return page;
}

CFSPDF_WidgetAnnotHandler* CFFL_Widget::GetWidgetHandler() const
{
	return m_pWidgetHandler;
}

void CFFL_Widget::CallControlDrawAppearance(FSFormControl* pControl, CFX_RenderDevice* pDevice, const CPDF_Matrix* pUser2Device, CPDF_Annot::AppearanceMode mode, CPDF_Annot::AppearanceMode fallBackMode, const CPDF_RenderOptions* pOptions)
{
	if(pControl && ((FSFormControlImp*)pControl)->ST_IsVisible())
	{
		if(mode != CPDF_Annot::Normal && !CFSPDF_WidgetAnnotHandler::IsWidgetAppearanceValid(pControl, mode))  
			mode = fallBackMode;
		CFSPDF_WidgetAnnotHandler::DrawAppearance(pControl, pDevice, pUser2Device, mode, NULL);
	}
}

void CFFL_Widget::OnDraw(FSPDFPage *pPageView, CFX_RenderDevice* pDevice, CPDF_Matrix* pUser2Device, FX_DWORD dwFlags)
{
	if(IFSPDF_Widget* pWnd = GetWidget(pPageView, FALSE))
	{
		CPDF_Matrix mt = this->GetCurrentMatrix();
		mt.Concat(*pUser2Device);
		pWnd->DrawWidget(pDevice, &mt);
	}
	else
	{
		CallControlDrawAppearance(m_pControl, pDevice, pUser2Device, CPDF_Annot::Normal, CPDF_Annot::Normal, NULL);
	}
}

void CFFL_Widget::OnDrawInactive(FSPDFPage *pPageView, CFX_RenderDevice* pDevice, CPDF_Matrix* pUser2Device, FX_DWORD dwFlags)
{
	FXSYS_assert(m_pControl);

	CallControlDrawAppearance(m_pControl, pDevice, pUser2Device, CPDF_Annot::Normal, CPDF_Annot::Normal, NULL);
}

FX_BOOL CFFL_Widget::OnSetFocus (FS_DWORD nFlag)
{
	FXSYS_assert(m_pControl);
	FSPDFPage* pPageView = GetCurrentPageView();

	CPDF_Rect rcAnnot = ((FSFormControlImp*)m_pControl)->GetCPDFFormControl()->GetRect();

	if(IFSPDF_Widget* pWnd = GetWidget(pPageView, TRUE))
	{
		pWnd->SetFocus();
	}

	Activate();
	Invalidate();

	/*TODO: phantom code
	pDocument->SetCurSelection(FFL_SELECTHANDLER_NAME, pWidget);
	*/

	return TRUE;
}

FX_BOOL CFFL_Widget::OnKillFocus (FS_DWORD nFlag)
{
	if(!IsActivated()) return TRUE;

	FSPDFPage* pPageView = GetCurrentPageView();
	FXSYS_assert(pPageView);

	CommitData(pPageView, nFlag);

	if(IFSPDF_Widget* pWnd = GetWidget(pPageView, TRUE))
	{
		pWnd->KillFocus();
	}

	FXSYS_assert(m_pControl);

	FX_BOOL bDestroyWindow = FALSE;
	switch(((FSFormControlImp*)m_pControl)->GetCPDFFormControl()->GetType())
	{
	case CPDF_FormField::PushButton:
	case CPDF_FormField::RadioButton:
	case CPDF_FormField::CheckBox:
		bDestroyWindow = TRUE;
		break;
	}
	bDestroyWindow = TRUE;   //TODO: Remove this when processing action events.

	ExitFiller(pPageView, bDestroyWindow);
	//bDestroy == TRUE means can't destroy pdfwindow for responsing OnBeforeKeyStroke (from phantom code);
	/*TODO: pDocument->ClearSelection(); (Phantom code, add it to !IsActivated() branch too.)*/

	return TRUE;
}

void CFFL_Widget::ExitFiller(FSPDFPage* pPageView, FX_BOOL bReleaseWidget)
{
	DeActivate();
	Invalidate(pPageView);
	if(bReleaseWidget)
		ReleaseWidget(pPageView);
}

void CFFL_Widget::FormatAndResetAppearance()
{
	FXSYS_assert(m_pControl);

	//TODO: Ask control to reset appearance
	//TODO: 		if (!IsAppearanceValid(pAnnot))
	/*TOOD:				pAnnot->ResetAppearance(NULL, FALSE);
	if (nFieldType == FIELDTYPE_TEXTFIELD || nFieldType == FIELDTYPE_COMBOBOX)
	{
	FX_BOOL bFormated = FALSE;
	CFX_WideString sValue = pWidget->OnFormat(0, bFormated);

	if (bFormated && nFieldType == FIELDTYPE_COMBOBOX)
	{
	pWidget->ResetAppearance(sValue, FALSE);
	}
	}
	*/
	m_pControl->resetAppearanceStream();
}


CPDF_Rect CFFL_Widget::FFLRectToWidget(const CPDF_Matrix& mt, const CPDF_Rect& rect)
{
	CPDF_Matrix rmt;
	rmt.SetReverse(mt);

	CPDF_Rect temp = rect;
	rmt.TransformRect(temp);

	return temp;
}

CPDF_Point CFFL_Widget::FFLPointToWidget(const CPDF_Matrix& mt, const CPDF_Point& point)
{
	CPDF_Matrix rmt;
	rmt.SetReverse(mt);

	CPDF_Point pt = point;
	rmt.Transform(pt.x,pt.y);

	return pt;
}

CPDF_Rect CFFL_Widget::WidgetRectToFFL(const CPDF_Matrix& mt, const CPDF_Rect& rect)
{
	CPDF_Rect temp = rect;
	mt.TransformRect(temp);

	return temp;
}

CPDF_Point CFFL_Widget::WidgetPointtoFFL(const CPDF_Matrix& mt, const CPDF_Point & point)
{
	CPDF_Point pt = point;
	mt.Transform(pt.x,pt.y);

	return pt;
}

CPDF_Rect CFFL_Widget::GetViewBBoxF(FSPDFPage *pPageView)
{
	FXSYS_assert(pPageView);
	FXSYS_assert(m_pControl);

	CPDF_Rect rcAnnot;

	if (IFSPDF_Widget* pWnd = GetWidget(pPageView, FALSE))
	{
		CPDF_Rect rcWindow;
		pWnd->GetWidgetRect(rcWindow);

		rcAnnot = WidgetRectToFFL(GetCurrentMatrix(), rcWindow);
	}
	else
	{
		rcAnnot = ((FSFormControlImp*)m_pControl)->GetCPDFFormControl()->GetRect();
	}

	return rcAnnot;
}

FX_RECT CFFL_Widget::GetViewBBox(FSPDFPage *pPageView)
{
	CPDF_Rect rcWin = GetViewBBoxF(pPageView);

	CPDF_Rect rcFocus = GetFocusBox(pPageView);
	if (!rcFocus.IsEmpty())
		rcWin.Union(rcFocus);

	CPDF_Rect rect = rcWin;
	rect.Inflate(1, 1);

	return rect.GetOutterRect();
}

CPDF_Rect CFFL_Widget::GetFocusBox(FSPDFPage* pPageView)
{
	if (IFSPDF_Widget* pWnd = GetWidget(pPageView, FALSE))
	{	
		//TODO: Verify this.
		CPDF_Rect rcWidgetFocus;
		pWnd->GetFocusRect(rcWidgetFocus);
		CPDF_Rect rcFocus = /*FFLRectToWnd(pPageView, */WidgetRectToFFL(GetCurrentMatrix(), rcWidgetFocus)/*)*/;

		return rcFocus;
	}
	return CPDF_Rect(0,0,0,0);
}


void CFFL_Widget::GetActionData(FSPDFPage* pPageView, CPDF_AAction::AActionType type, FSPDF_FieldActionParam& fa)
{
	//TODO: Check if this is necessary: fa.sValue = m_pControl->GetValue();
}

void CFFL_Widget::SetActionData(FSPDFPage* pPageView, CPDF_AAction::AActionType type, const FSPDF_FieldActionParam& fa)
{
	return;
}

FX_BOOL CFFL_Widget::IsActionDataChanged(CPDF_AAction::AActionType type, const FSPDF_FieldActionParam& faOld, const FSPDF_FieldActionParam& faNew)
{
	return FALSE;
}

void CFFL_Widget::InvalidateRect(FS_FLOAT left, FS_FLOAT top, FS_FLOAT right, FS_FLOAT bottom)
{
	//NOTICE: this is pdf rect, and we'll convert it to device rect here.
	FSPDFPage* pPageView = GetCurrentPageView();
	if(!pPageView) return;
	
	CPDF_Rect pageRect; pageRect.left = left; pageRect.top = top; pageRect.right = right; pageRect.bottom = bottom;
	pageRect.Normalize();

	FSFormFillerImp* pFormFiller = ((FSPDFPageImp*)pPageView)->getFormFiller();
	if(!pFormFiller) return;
	FSFormFillerAssist* pAssist = pFormFiller->GetFormFillerAssist();
	if(!pAssist) return;
	FSRectF rect = {pageRect.left, pageRect.bottom, pageRect.right, pageRect.top};
	pAssist->refresh(pPageView, &rect);
//	CFX_RectF pageRect; pageRect.Set(left, bottom, right - left, top - bottom);
	//CFX_Rect deviceRect;
	//pPageView->PageToDeviceRect(pageRect, deviceRect);

// 	CFSCRT_LTAppHandler* pHandler = FSCRT_GetLTAppHandler();
// 	if(pHandler)
// 	{
//		FSCRT_RECTF rect = {pageRect.left, pageRect.top, pageRect.right, pageRect.bottom}; 
//		FSFormFillerImp* pFormFiller = NULL;
//		FS_RESULT ret = m_pWidgetHandler->GetFormFiller(&pFormFiller);
//		if(FSCRT_ERRCODE_SUCCESS != ret) return;
//		ret = pFormFiller->m_pJSActionHandle->InvalidateRect((FSCRT_DOCUMENT)pPageView->GetDocument(), pPageView->m_page->m_nPageIndex, &rect);
// 		FSCRT_VAR params[2] = {{FSCRT_VT_LPVOID, 0, 0}, {FSCRT_VT_LPVOID, 0, 0}};
// 		CFSCRT_LTPage* pPage = GetControl()->GetPage();
// 		params[0].var.voidPtrVar = (FSCRT_PAGE)pPage;
// 		params[1].var.voidPtrVar = &rect;
// 		FSCRT_VAR retvalue;
// 		FSCRT_GetLTAppHandler()->CallFunction((FSCRT_DOCUMENT)pPage->GetDocument(), FSCRT_APPFUNC_INVALIDATE, params, 2, &retvalue);
//	}
}

void CFFL_Widget::SetChangeMark()
{
// 	CFSCRT_LTAppHandler* pHandler = FSCRT_GetLTAppHandler();
// 	if(pHandler)
// 	{
		FSPDFPage* pPageView = GetCurrentPageView();
		if(!pPageView) return;
		((FSPDFDocImp*)pPageView->getDocument())->setModified();
//		CFSCRT_LTPage* pPage = GetControl()->GetPage();
		//FSCRT_VAR retvalue;
		//FSCRT_GetLTAppHandler()->CallFunction((FSCRT_DOCUMENT)pPage->GetDocument(), FSCRT_APPFUNC_SETCHANGEMARK, NULL, 0, &retvalue);
#if defined(_FSDK_OOM_) && !defined(_FXSDK_EDIT_OOM_RECOVER_) && !defined(_FXSDK_CUSTOM_OOM_RECOVER_)
		//FSCRT_GACHECKOOMSTATE_SETMODIFYFLAG(pPage->GetDocument(), TRUE);
#endif
//		CFSCRT_JS_ActionHandler* formInteraction = pPageView->m_pFormFiller->m_pJSActionHandle;
//		formInteraction->SetChangeMark((FSCRT_DOCUMENT)pPage->GetDocument(), TRUE);
//	}
}


void CFFL_Widget::Invalidate(FSPDFPage* pPageView)
{
	if(!pPageView) pPageView = GetCurrentPageView();

	FX_RECT rect = GetViewBBox(pPageView);
	this->InvalidateRect((FS_FLOAT)rect.left, (FS_FLOAT)rect.top, (FS_FLOAT)rect.right, (FS_FLOAT)rect.bottom);
}

void CFFL_Widget::OnDeSelected()
{
	//Do nothing
}

void CFFL_Widget::OnSelected()
{
	//Do nothing
}


//TODO:
void CFFL_Widget::OnCreate()
{

}

void CFFL_Widget::OnLoad()
{

}

void CFFL_Widget::OnRelease()
{

}

void CFFL_Widget::SaveState(FSPDFPage* pPageView)
{

}

void CFFL_Widget::RestoreState(FSPDFPage* pPageView)
{

}

//Implement IFSPDF_Widget_Notify

void CFFL_Widget::QueryWherePopup(void* pPrivateData, FX_FLOAT fPopupMin,FX_FLOAT fPopupMax, FX_INT32 & nRet, FX_FLOAT & fPopupRet)
{
	//combobox only, will implement it there.
}


void CFFL_Widget::OnBeforeKeyStroke(FX_BOOL bEditOrList, void* pPrivateData, FX_INT32 nKeyCode, CFX_WideString & strChange, const CFX_WideString& strChangeEx, int nSelStart, int nSelEnd,FX_BOOL bKeyDown, FX_BOOL & bRC, FX_BOOL & bExit, FX_DWORD nFlag)
{

// 	if (!m_bNotifying)
// 	{

		if(((FSFormControlImp*)m_pControl)->GetAAction(CPDF_AAction::KeyStroke))
		{
//			m_bNotifying = TRUE;
//			int nAge = pData->pWidget->GetAppearanceAge();
//			int nValueAge = pData->pWidget->GetValueAge();

//			ASSERT(pData->pPageView != NULL);
			FSPDFDoc* pDocument  = NULL;//(FSPDFDoc*)m_pControl->GetPage()->GetDocument();

			FSPDFPage* pPageView = NULL;//pDocument-;
			//pDocument->GetPageView(m_pControl, &pPageView);
			if(!pPageView) return;

			FSPDF_FieldActionParam fa;
			fa.bModifier = 0;//m_pApp->FFI_IsCTRLKeyDown(nFlag);
			fa.bShift = 0;//m_pApp->FFI_IsSHIFTKeyDown(nFlag);
			fa.sChange = strChange;
			fa.sChangeEx = strChangeEx;
			fa.bKeyDown = bKeyDown;
			fa.bWillCommit = FALSE;
			fa.bRC = TRUE;
			fa.nSelStart = nSelStart;
			fa.nSelEnd = nSelEnd;


			GetActionData(pPageView, CPDF_AAction::KeyStroke, fa);
			SaveState(pPageView);

			if (((FSFormControlImp*)m_pControl)->OnAAction(CPDF_AAction::KeyStroke, fa, pPageView))
			{
// 				if (!IsValidAnnot(pData->pPageView, pData->pWidget))
// 				{
// 					bExit = TRUE;
// 					m_bNotifying = FALSE;
// 					return;
// 				}
// 
// 				if (nAge != pData->pWidget->GetAppearanceAge())
// 				{
// 					CPWL_Wnd* pWnd = pFormFiller->ResetPDFWindow(pData->pPageView, nValueAge == pData->pWidget->GetValueAge());
// 					pData = (CFFL_PrivateData*)pWnd->GetAttachedData();
// 					bExit = TRUE;
// 				}

				if (fa.bRC)
				{
					SetActionData(pPageView, CPDF_AAction::KeyStroke, fa);
					bRC = FALSE;
				}
				else
				{
					RestoreState(pPageView);
					bRC = FALSE;
				}

				if (((FSPDFDocImp*)pDocument)->GetFocusAnnot() != m_pControl)
				{
					CommitData(pPageView,nFlag);
					bExit = TRUE;
				}
			}
			else
			{			
// 				if (!IsValidAnnot(pData->pPageView, pData->pWidget))
// 				{
// 					bExit = TRUE;
// 					m_bNotifying = FALSE;
// 					return;
// 				}
			}

	//		m_bNotifying = FALSE;
		}
//	}
}

void CFFL_Widget::OnAfterKeyStroke(FX_BOOL bEditOrList, void* pPrivateData, FX_BOOL& bExit, FX_DWORD nFlag)
{

}

void CFFL_Widget::OnPopupPreOpen(void* pPrivateData, FX_BOOL& bExit)
{

}

void CFFL_Widget::OnPopupPostOpen(void* pPrivateData, FX_BOOL& bExit)
{

}




//
//
//void CFFL_FormFiller::SetWindowRect(CPDFSDK_PageView* pPageView, const CPDF_Rect& rcWindow)
//{
//	if (CPWL_Wnd* pWnd = this->GetPDFWindow(pPageView, FALSE))
//	{
//		pWnd->Move(CPDF_Rect(rcWindow), TRUE, FALSE);
//	}
//}
//
//CPDF_Rect CFFL_FormFiller::GetWindowRect(CPDFSDK_PageView* pPageView)
//{
//	if (CPWL_Wnd* pWnd = this->GetPDFWindow(pPageView, FALSE))
//	{
//		return pWnd->GetWindowRect();
//	}
//
//	return CPDF_Rect(0,0,0,0);
//}
//
//FX_BOOL CFFL_FormFiller::OnLButtonDown(CPDFSDK_PageView *pPageView, CPDFSDK_Annot* pAnnot, FSCRT_UINT nFlags, const CPDF_Point& point)
//{
//	if (CPWL_Wnd * pWnd = GetPDFWindow(pPageView, TRUE))
//	{		
//		m_bValid = TRUE;
//		FX_RECT rect = this->GetViewBBox(pPageView,pAnnot);
//		this->InvalidateRect(rect.left, rect.top, rect.right, rect.bottom);
//
//		//因为PDFWindow模块没有去判断鼠标是否在windowless窗口中，所以在这里判断一下。
//		if(!rect.Contains((int)point.x, (int)point.y))
//			return FALSE;
//
//		//		::InvalidateRect(pPageView->GetPageViewWnd(), &this->GetViewBBox(pPageView, pAnnot), TRUE);
//		return pWnd->OnLButtonDown(WndtoPWL(pPageView, point),nFlags);
//	}
//
//	return FALSE;
//}
//
//FX_BOOL CFFL_FormFiller::OnLButtonUp(CPDFSDK_PageView *pPageView, CPDFSDK_Annot* pAnnot, FSCRT_UINT nFlags, const CPDF_Point& point)
//{
//	if (CPWL_Wnd * pWnd = GetPDFWindow(pPageView, FALSE))
//	{	
//		// 		CReader_Page* pPage = m_pWidget->GetPage();
//		// 		ASSERT(pPage != NULL);
//		// 
//		// 		CPDFSDK_Document* pDocument = pPage->GetDocument();
//		// 		ASSERT(pDocument != NULL);
//		// 
//		// 		if (pDocument->GetFocusAnnot() != m_pWidget)
//		// 		{
//		// 			m_bValid = FALSE;
//		// 			::InvalidateRect(pPageView->GetPageViewWnd(), &this->GetViewBBox(pPageView, pAnnot), TRUE);
//		// 		}
//
//		FX_RECT rcFFL =  this->GetViewBBox(pPageView, pAnnot);
//		this->InvalidateRect(rcFFL.left, rcFFL.top, rcFFL.right, rcFFL.bottom);
//		pWnd->OnLButtonUp(WndtoPWL(pPageView, point),nFlags);
//		return TRUE;
//	}
//
//	return FALSE;
//}
//
//
//CPDF_Matrix CFFL_FormFiller::GetWindowMatrix(void* pAttachedData)
//{
//	if (CFFL_PrivateData* pPrivateData = (CFFL_PrivateData*)pAttachedData)
//	{
//		if (pPrivateData->pPageView)
//		{
//			CPDF_Matrix mtPageView;
//			pPrivateData->pPageView->GetCurrentMatrix(mtPageView);
//			CPDF_Matrix mt = GetCurMatrix();
//			mt.Concat(mtPageView);
//
//			return mt;
//		}
//	}
//	return CPDF_Matrix(1,0,0,1,0,0);
//}
//
//
//CFX_WideString CFFL_FormFiller::LoadPopupMenuString(int nIndex)
//{
//	ASSERT(m_pApp != NULL);
//
//	// 	switch (nIndex)
//	// 	{
//	// 	case 0:
//	// 		return FFL_LoadString(m_pApp, IDS_EDITMENU_UNDO);
//	// 	case 1:
//	// 		return FFL_LoadString(m_pApp, IDS_EDITMENU_REDO);
//	// 	case 2:
//	// 		return FFL_LoadString(m_pApp, IDS_EDITMENU_CUT);
//	// 	case 3:
//	// 		return FFL_LoadString(m_pApp, IDS_EDITMENU_COPY);
//	// 	case 4:
//	// 		return FFL_LoadString(m_pApp, IDS_EDITMENU_PASTE);
//	// 	case 5:
//	// 		return FFL_LoadString(m_pApp, IDS_EDITMENU_DELETE);
//	// 	case 6:
//	// 		return FFL_LoadString(m_pApp, IDS_EDITMENU_SELECTALL);
//	// 	}
//	return L"";
//}
//
//CPDF_Rect CFFL_FormFiller::GetPDFWindowRect() const
//{
//	ASSERT(m_pWidget != NULL);
//
//	CPDF_Rect rectAnnot;
//	m_pWidget->GetPDFAnnot()->GetRect(rectAnnot);
//
//	FX_FLOAT fWidth = rectAnnot.right - rectAnnot.left;
//	FX_FLOAT fHeight = rectAnnot.top - rectAnnot.bottom;
//
//
//	if ((m_pWidget->GetRotate() / 90) & 0x01)
//		return CPDF_Rect(0,0,fHeight,fWidth);
//	else
//		return CPDF_Rect(0,0,fWidth,fHeight);
//}
//
//void CFFL_FormFiller::FFL_FreeData(void* pData)
//{
//	ASSERT(pData != NULL);
//
//	delete (CFFL_PrivateData*)pData;
//}
//void CFFL_FormFiller::GetKeyStrokeData(CPDFSDK_PageView* pPageView, FFL_KeyStrokeData& data)
//{
//}
//
//
//void CFFL_FormFiller::GetActionData(CPDFSDK_PageView* pPageView, CPDF_AAction::AActionType type,
//	PDFSDK_FieldAction& fa)
//{
//	fa.sValue = m_pWidget->GetValue();
//}
//
//void CFFL_FormFiller::SetActionData(CPDFSDK_PageView* pPageView, CPDF_AAction::AActionType type, 
//	const PDFSDK_FieldAction& fa)
//{
//}
//
//FX_BOOL CFFL_FormFiller::IsActionDataChanged(CPDF_AAction::AActionType type, const PDFSDK_FieldAction& faOld, 
//	const PDFSDK_FieldAction& faNew)
//{
//	return FALSE;
//}
//
//void CFFL_FormFiller::SaveState(CPDFSDK_PageView* pPageView)
//{
//}
//
//void CFFL_FormFiller::RestoreState(CPDFSDK_PageView* pPageView)
//{
//}
//
//CPWL_Wnd*  CFFL_FormFiller::ResetPDFWindow(CPDFSDK_PageView* pPageView, FX_BOOL bRestoreValue)
//{
//	return GetPDFWindow(pPageView, FALSE); 
//}
//
//void CFFL_FormFiller::TimerProc()
//{
//	// 	CPDF_Point pt(::GetMessagePos());
//	// 	HWND hMouse = WindowFromPoint(pt);
//	// 
//	// //	if (hMouse == GetCurPageView()->GetPageViewWnd())
//	// 	ScreenToClient(hMouse, &pt);
//	// 
//	// 	ASSERT(m_pWidget != NULL);
//	// 	m_pWidget->ShowHint(hMouse, pt);
//}
//
//IFX_SystemHandler* CFFL_FormFiller::GetSystemHandler() const
//{
//	// 	ASSERT(m_pApp != NULL);
//	// 
//	// 	return ISystemHandle::GetSystemHandler(m_pApp);
//	return m_pApp->GetSysHandler();
//	//	return NULL;
//}
//
//void CFFL_FormFiller::OnKeyStroke(FX_BOOL bKeyDown)
//{
//}
//
//
//FX_BOOL CFFL_FormFiller::CanCopy(CPDFSDK_Document* pDocument)
//{
//	return FALSE;
//}
//
//FX_BOOL CFFL_FormFiller::CanCut(CPDFSDK_Document* pDocument)
//{
//	return FALSE;
//}
//
//FX_BOOL CFFL_FormFiller::CanPaste(CPDFSDK_Document* pDocument)
//{
//	return FALSE;
//}
//
//void CFFL_FormFiller::DoCopy(CPDFSDK_Document* pDocument)
//{
//}
//
//void CFFL_FormFiller::DoCut(CPDFSDK_Document* pDocument)
//{
//}
//
//void CFFL_FormFiller::DoPaste(CPDFSDK_Document* pDocument)
//{
//}
//
//void CFFL_FormFiller::InvalidateRect(FS_FLOAT left, FS_FLOAT top, FS_FLOAT right, FS_FLOAT bottom)
//{
//	CPDF_Page * pPage = m_pWidget->GetPDFPage();
//	m_pApp->FFI_Invalidate((FSPDF_PAGE)pPage, left, top, right, bottom);
//}
//