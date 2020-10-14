#ifndef _JAVASCRIPT_H_
#define _JAVASCRIPT_H_

#include "../../../inc/fs_precompilers.h"

#if defined(_FSDK_ALL_) || defined(_FSDK_JAVASCRIPT_)
#include "../../../inc/fpdf_define.h"

#include "../../../../fxlib/dscript/jsapi/dfxapi.h"
#include "../../../widget/pdfwindow/include/pdfwindow/pwl_wnd.h"
#include "../../../widget/pdfwindow/include/pdfwindow/pwl_utils.h"

#include "js_interface.h"
#include "js_define.h"
#include "js_object.h"
#include "js_value.h"
#include "js_resource.h"

#include "js_context.h"
#include "js_eventhandler.h"
#include "js_runtime.h"

#include <string>
// #pragma warning( disable : 4786) 
#include <vector>

template<class TYPE>
class CGW_ArrayTemplate : public CFX_ArrayTemplate<TYPE>
{
public: 
	CGW_ArrayTemplate(){}
	virtual ~CGW_ArrayTemplate(){}

	typedef int (*LP_COMPARE)(TYPE p1, TYPE p2);

	void Sort(LP_COMPARE pCompare, FX_BOOL bAscent = TRUE)
	{
		int nSize = this->GetSize();
		QuickSort(0, nSize -1, bAscent, pCompare);
	}

private:
	void QuickSort(FX_UINT32 nStartPos, FX_UINT32 nStopPos, FX_BOOL bAscend, LP_COMPARE pCompare)
	{
		if (nStartPos >= nStopPos) return;

		if ((nStopPos - nStartPos) == 1)
		{
			TYPE Value1 = this->GetAt(nStartPos);
			TYPE Value2 = this->GetAt(nStopPos);

			int iGreate = (*pCompare)(Value1, Value2);
			if ((bAscend && iGreate > 0) || (!bAscend && iGreate < 0))
			{
				this->SetAt(nStartPos, Value2);
				this->SetAt(nStopPos, Value1);
			}
			return;
		}

		FX_UINT32 m = (nStartPos + nStopPos) / 2;
		FX_UINT32 i = nStartPos;

		TYPE Value = this->GetAt(m);

		while (i < m)
		{
			TYPE temp = this->GetAt(i);

			int iGreate = (*pCompare)(temp, Value);
			if ((bAscend && iGreate > 0) || (!bAscend && iGreate < 0))
			{
				this->InsertAt(m+1, temp);
				this->RemoveAt(i);
				m--;
			}
			else
			{
				i++;
			}
		}

		FX_UINT32 j = nStopPos;

		while (j > m)
		{
			TYPE temp = this->GetAt(j);

			int iGreate = (*pCompare)(temp, Value);
			if ((bAscend && iGreate < 0) || (!bAscend && iGreate > 0))
			{
				this->RemoveAt(j);
				this->InsertAt(m, temp);
				m++;
			}
			else
			{
				j--;
			}
		}

		if (nStartPos < m) QuickSort(nStartPos, m, bAscend, pCompare);
		if (nStopPos > m) QuickSort(m, nStopPos, bAscend, pCompare);
	}
};

#endif //#if defined(_FSDK_ALL_) || defined(_FSDK_JAVASCRIPT_)

#endif //_JAVASCRIPT_H_

