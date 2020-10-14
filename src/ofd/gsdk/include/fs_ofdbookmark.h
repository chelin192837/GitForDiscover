#ifndef _FS_OFDBOOKMARK_H_
#define _FS_OFDBOOKMARK_H_

class CFS_OFDBookmarks;
class CFS_OFDBookmark;

class CFS_OFDBookmarks : public CFX_Object
{
public:
	CFS_OFDBookmarks();
	~CFS_OFDBookmarks();

	FX_BOOL Create(CFS_OFDDocument* pDocument, IOFD_Bookmarks* pBookmarks = NULL);

	FX_INT32 CountSubBookmark();
	CFS_OFDBookmark* GetSubBookmark(FX_INT32 index);
	CFS_OFDBookmark* AddSubBookmark();
	void DeleteSubBookmark(FX_INT32 index);

	CFS_OFDDocument* GetDocument();
	IOFD_WriteBookmarks* GetWriteBookmarks();

protected:
	CFS_OFDBookmark* FindBookmark_Storage(COFD_Bookmark* pBookmark);

	CFX_PtrList* m_bookmarks;
	CFS_OFDDocument* m_pDocument;
	IOFD_WriteBookmarks* m_pWriteBookmarks;
};

class CFS_OFDBookmark : public CFX_Object
{
public:
	CFS_OFDBookmark(CFS_OFDBookmarks* pBookmarks);
	~CFS_OFDBookmark();

	FX_BOOL Create(COFD_Bookmark* pBookmark = NULL);
	FX_BOOL Create(COFD_WriteBookmark* pBookmark);

	CFX_WideString GetName();
	void GetDest(FX_INT32& iPage, FX_FLOAT& x, FX_FLOAT& y);

	void SetName(const CFX_WideString& wsTitle);
	void SetDest(int iPage, FX_FLOAT x, FX_FLOAT y);

	COFD_Bookmark* GetBookmark();

protected:
	CFS_OFDBookmarks* m_pBookmarks;
	COFD_WriteBookmark* m_pWriteBookmark;
};

#endif //_FS_OFDBOOKMARK_H_
