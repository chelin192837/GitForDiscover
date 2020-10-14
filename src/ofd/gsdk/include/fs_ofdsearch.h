#ifndef _FS_OFDSEARCH_H_
#define _FS_OFDSEARCH_H_


struct SERCH_RECT{
    float		x;			//起始点（左上点）x坐标
    float		y;			//起始点（左上点）y坐标
	float		width;		//宽度
	float		height;		//高度
};

/**文字查找内容结果*/
class CFS_OFDSearchResult
{
public:
	CFS_OFDSearchResult()	{}
	~CFS_OFDSearchResult();

	int						nDocIndex;		//文档索引
	int						nPageID;		//页面ID
	int						nPageIndex;		//页面索引

	CFX_PtrList				m_Rects;		//每个字符的矩形区域

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
