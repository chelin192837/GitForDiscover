#ifndef _FS_OFDSEARCH_H_
#define _FS_OFDSEARCH_H_


struct SERCH_RECT{
    float		x;			//��ʼ�㣨���ϵ㣩x����
    float		y;			//��ʼ�㣨���ϵ㣩y����
	float		width;		//���
	float		height;		//�߶�
};

/**���ֲ������ݽ��*/
class CFS_OFDSearchResult
{
public:
	CFS_OFDSearchResult()	{}
	~CFS_OFDSearchResult();

	int						nDocIndex;		//�ĵ�����
	int						nPageID;		//ҳ��ID
	int						nPageIndex;		//ҳ������

	CFX_PtrList				m_Rects;		//ÿ���ַ��ľ�������

	CFX_RectArray			m_ArrRects;
};

class CFS_OFDSearch : public CFX_Object
{
public:
	CFS_OFDSearch();

	~CFS_OFDSearch();

	void		AddResult(CFS_OFDSearchResult* pValue);
	FX_INT32	GetCount();
	CFS_OFDSearchResult*	GetSingleResult(FX_INT32 nIndex);

protected:
	CFX_MapPtrTemplate<FX_INT32, CFS_OFDSearchResult*>* m_pResult;

	FX_INT32				m_nKey;
};

#endif //_FS_OFDSEARCH_H_
