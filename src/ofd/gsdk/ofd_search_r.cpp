#include "../include/ofd.h"

#include "fxlib.h"
#include "include/fs_ofdcontentobject.h"
#include "include/fs_ofdimageobject.h"
#include "include/fs_ofdtextobject.h"
#include "include/fs_ofdpathobject.h"
#include "include/fs_ofdlayer.h"
#include "include/fs_ofdpage.h"
#include "include/fs_ofddocument.h"
#include "include/fs_ofdsdkmgr.h"
#include "include/fs_ofdsearch.h"

#ifdef __cplusplus
extern "C" {
#endif

//
int	OFD_Search_Count(OFD_SEARCH hSearch)
{
	if (!hSearch)
		return -1;

	CFS_OFDSearch* pSearch = (CFS_OFDSearch*)hSearch;
	return pSearch->GetCount();
}

//
int OFD_Search_GetProperty(OFD_SEARCH hSearch, int index, int flag)
{
	if (!hSearch || index < 0)
		return -1;

	CFS_OFDSearch* pSearch = (CFS_OFDSearch*)hSearch;
	CFS_OFDSearchResult* pResult = pSearch->GetSingleResult(index);
	if (!pResult)
		return -1;

	FX_INT32 nValue = 0;

	switch (flag)
	{
	case 0:
		nValue = pResult->nPageID;
		break;
	case 1:
		nValue = pResult->nDocIndex;
		break;
	case 2:
		nValue = pResult->m_ArrRects.GetSize();
		break;
	case 3:
		nValue = pResult->nPageIndex;
		break;
	}

	return nValue;
}

//
OFD_RECT OFD_Search_GetRect(OFD_SEARCH hSearch, int index, int iRect)
{
	OFD_RECT rect;
	rect.x = rect.y = rect.width = rect.height = 0;

	if (!hSearch || index < 0)
		return rect;

	CFS_OFDSearch* pSearch = (CFS_OFDSearch*)hSearch;
	CFS_OFDSearchResult* pResult = pSearch->GetSingleResult(index);
	if (!pResult || iRect < 0 || (iRect > pResult->m_ArrRects.GetSize() - 1))
		return rect;

	rect.x = pResult->m_ArrRects[iRect].left;
	rect.y = pResult->m_ArrRects[iRect].top;
	rect.width = pResult->m_ArrRects[iRect].Width();
	rect.height = pResult->m_ArrRects[iRect].Height();
	if (rect.width < 0)
		rect.width = - rect.width;
	if (rect.height < 0)
		rect.height = - rect.height;

	return rect;
}

//
void OFD_Search_Release(OFD_SEARCH hSearch)
{
	if (!hSearch)
		return;

	CFS_OFDSearch* pSearch = (CFS_OFDSearch*)hSearch;

	delete pSearch;
}

#ifdef __cplusplus
}
#endif
