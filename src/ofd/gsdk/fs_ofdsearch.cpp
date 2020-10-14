#include "fxlib.h"
#include "include/fs_ofdsearch.h"

//
CFS_OFDSearchResult::~CFS_OFDSearchResult()
{
	int nCount = m_ArrRects.GetSize();

	m_ArrRects.RemoveAll();
}

//
CFS_OFDSearch::CFS_OFDSearch()
	: m_nKey(0)
{
	m_pResult = FX_NEW CFX_MapPtrTemplate<FX_INT32, CFS_OFDSearchResult*>;
}

//
CFS_OFDSearch::~CFS_OFDSearch()
{
	if (!m_pResult)
	{
		FX_POSITION pos = m_pResult->GetStartPosition();

		FX_INT32 key = 0;
		CFS_OFDSearchResult* pVal = 0;

		while (pos)
		{
			m_pResult->GetNextAssoc(pos, key, pVal);

			delete pVal;
		}

		delete m_pResult;
		m_pResult = NULL;
	}
}

//
void CFS_OFDSearch::AddResult(CFS_OFDSearchResult* pValue)
{
	if (!m_pResult)
		return;

	m_pResult->SetAt(m_nKey, pValue);

	m_nKey++;
}

//
FX_INT32 CFS_OFDSearch::GetCount()
{
	return m_nKey;
}

//
CFS_OFDSearchResult* CFS_OFDSearch::GetSingleResult(FX_INT32 nIndex)
{
	if (!m_pResult || nIndex < 0)
		return NULL;

	CFS_OFDSearchResult* pResult = NULL;
	m_pResult->Lookup(nIndex, pResult);

	return pResult;
}
