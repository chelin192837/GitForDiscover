#include "../../inc/fpdf_define.h"
#include "ffl_widget.h"

static int FSPDF_PointerCompare(const void* lhs, const void* rhs)
{
	return *(const FX_INTPTR*)lhs - *(const FX_INTPTR*)rhs;
}

void CFFL_Widget::UpdateField(CPDF_FormField* pFormField, FX_LPCWSTR value, FX_BOOL doResetAppearance, FX_BOOL doInvalidateRect)
{
	if(!pFormField)
	{
		pFormField = ((FSFormControlImp*)GetControl())->GetCPDFFormControl()->GetField();
		FXSYS_assert(pFormField);
	}

	const int numControls = pFormField->CountControls();
	CPDF_FormControl** pFormControlArray = FX_Alloc(CPDF_FormControl*, numControls);

	{
		for (int i = 0, count = pFormField->CountControls(); i < count; i++)
		{
			pFormControlArray[i] = pFormField->GetControl(i);
			FXSYS_assert(pFormControlArray[i]);
		}
	}

	FXSYS_qsort(pFormControlArray, numControls, sizeof(pFormControlArray[0]), FSPDF_PointerCompare);

	if(doResetAppearance)
	{
		for (int i = 0; i < numControls; i++)
		{
			IFSPDF_Widget::ResetAppearance(pFormControlArray[i], value);
		}
	}

	if(doInvalidateRect)
	{
		FSPDFPage* pPageView = GetCurrentPageView();
// 		CFSPDF_ViewContainer* pViewContainer = pPageView->m_viewer;
// 
// 		FS_INT32 pageviewStart, pageviewCount;
// 		pViewContainer->GetVisiblePages(&pageviewStart, &pageviewCount);

		CFSPDF_WidgetAnnotHandler* pWidgetHandler = GetWidgetHandler();

		FSPDFPage* pPage = pPageView;
		FSPDFDocImp* pDoc  = (FSPDFDocImp*)pPage->getDocument();

		CFX_MapPtrToPtr* pageViewMap = pDoc->getPageMap();
		FX_POSITION pos = pageViewMap ? pageViewMap->GetStartPosition() : NULL;
		while(pos)
		{
			void* index; FSPDFPage* page = NULL;
			pageViewMap->GetNextAssoc(pos, (void*&)index, (void*&)page);
			if(!page) continue;

			FS_INT32 pageCtrlCount = ((FSPDFPageImp*)page)->CountFormControls();

			for(FS_INT32 ctrlIndex = 0; ctrlIndex < pageCtrlCount; ctrlIndex++)
			{
				FSFormControl* control = ((FSPDFPageImp*)page)->GetFormControl(ctrlIndex);

				CPDF_FormControl* ctrl = ((FSFormControlImp*)control)->GetCPDFFormControl();
				if(FXSYS_bsearch(&ctrl, pFormControlArray, numControls, sizeof(pFormControlArray[0]), FSPDF_PointerCompare))
				{
					control->resetAppearanceStream();
					pWidgetHandler->GetWidget(control, 1)->Invalidate(page);
				}
			}
		}
	}
	FX_Free(pFormControlArray);
}
