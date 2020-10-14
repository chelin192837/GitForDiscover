#ifndef _FS_OFDINFOCOVER_H_
#define _FS_OFDINFOCOVER_H_

class CFX_OFDInfoCover;
class CFX_OFDTextInfoCover;
class CFX_OFDImageInfoCover;
class CFX_OFDInfoReCover;
class CFX_OFDCoverInfoXml;



class CFX_OFDInfoCover : public CFX_Object
{
public:
	CFX_OFDInfoCover(IOFD_WriteDocument* pWriteDocument, IOFD_Page*	pPage);
	~CFX_OFDInfoCover();

	void ObjInfoCover(CFX_RectF& rect, CFX_ByteString& bsXmlContent);

protected:
	void PageObjCollect(CFX_RectF& rect);

	void TraversalEachTextObj(CFX_RectF& rect);
	void TraversalEachImageObj(CFX_RectF& rect);

protected:
	IOFD_Page*				m_pPage;
	CFX_PtrList*			m_pTextObjList;
	CFX_PtrList*			m_pImgObjList;

	CFX_OFDTextInfoCover*	m_pTextInfoCover;
	CFX_OFDImageInfoCover*	m_pImageInfoCover;
	CFX_OFDCoverInfoXml*	m_pInfoXml;
};


const FX_WCHAR m_wTextReplace = 'X';

typedef struct _TEXTREPLACE : public CFX_Object
{
public:
	_TEXTREPLACE()
	{
		nPieceIndexStart = -1;
		nCodePosStart = -1;
		nPieceIndexEnd = -1;
		nCodePosEnd = -1;
	}

	COFD_TextObject*	pTextObj;
	FX_INT32			nPieceIndexStart;
	FX_INT32			nCodePosStart;
	FX_INT32			nPieceIndexEnd;
	FX_INT32			nCodePosEnd;
}TEXTREPLACE, *pTEXTREPLACE;

class CFX_OFDTextInfoCover : public CFX_Object
{
public:
	CFX_OFDTextInfoCover(IOFD_Page*	pPage, CFX_OFDCoverInfoXml*	pInfoXml);
	~CFX_OFDTextInfoCover();

	void ObjFullContained(const COFD_ContentObject* pObj);

	void ObjArrangePattern(const COFD_ContentObject* pObj, CFX_RectF& userRect, CFX_RectF& boundaryRect);

protected:
	FX_INT32 ObjArrageeManner(const COFD_ContentObject* pObj);

	void Horizon_LeftToRight(COFD_TextObject* pTextObj, CFX_RectF& boundaryRect, CFX_RectF& intersectRect);
	void Vertical_LeftToRight(COFD_TextObject* pTextObj, CFX_RectF& boundaryRect, CFX_RectF& intersectRect);

	void judge_Horizon_LeftToRight(COFD_TextObject* pTextObj, CFX_RectF& boundaryRect, CFX_RectF& intersectRect, TEXTREPLACE* pReplace);
	void judge_Vertical_LeftToRight(COFD_TextObject* pTextObj, CFX_RectF& boundaryRect, CFX_RectF& intersectRect, TEXTREPLACE* pReplace);

	void PieceList_Start_Horizon_LeftToRight(COFD_TextObject* pTextObj, CFX_RectF& boundaryRect, FX_FLOAT fPointX, TEXTREPLACE* pReplace);
	void PieceList_End_Horizon_LeftToRight(COFD_TextObject* pTextObj, CFX_RectF& boundaryRect, FX_FLOAT fPointX, TEXTREPLACE* pReplace);

	void PieceList_Start_Vertical_LeftToRight(COFD_TextObject* pTextObj, CFX_RectF& boundaryRect, FX_FLOAT fPointY, TEXTREPLACE* pReplace);
	void PieceList_End_Vertical_LeftToRight(COFD_TextObject* pTextObj, CFX_RectF& boundaryRect, FX_FLOAT fPointY, TEXTREPLACE* pReplace);

	void ObjTextReplace(COFD_TextObject* pTextObj, TEXTREPLACE* pReplace);
	void ObjTextReplace(const COFD_TextPiece* pPiece, FX_INT32 nPieceIndex, FX_INT32 nCodeIndex, FX_INT32 nCodeCount);

protected:
	IOFD_Page*				m_pPage;
	CFX_OFDCoverInfoXml*	m_pInfoXml;
};

class CFX_OFDImageInfoCover : public CFX_Object
{
public:
	CFX_OFDImageInfoCover(IOFD_WriteDocument* pWriteDocument, IOFD_Page* pPage, CFX_OFDCoverInfoXml* pInfoXml);
	~CFX_OFDImageInfoCover();

	void ObjFullContained(const COFD_ContentObject* pObj);

	void ObjIntersectDealWith(const COFD_ContentObject* pObj, CFX_RectF& userRect, CFX_RectF& boundaryRect);

protected:
	FX_BOOL CreateBlurImage(COFD_ImageObject* pImageObj, FX_BOOL bFull, CFX_Rect rect);
	void Base64Image(IOFD_FileStream* pStream, CFX_WideString wsSuffix);
	CFX_DIBitmap* OFD_LoadImage(IFX_FileRead* pFileRead);
	CFX_DIBitmap* OFD_LoadImage(const CFX_WideString& wsPath);
	CFX_DIBitmap* OFD_LoadImage(FX_LPCBYTE pBuf, FX_INT32 nSize);

	FX_BOOL	CreatePngFile(CFX_DIBitmap* pBitmap);
	FX_BOOL	CreateJpgFile(CFX_DIBitmap* pBitmap);
	FX_BOOL	CreateBmpFile(CFX_DIBitmap* pBitmap);
	FX_BOOL	CreateGifFile(CFX_DIBitmap* pBitmap);
	FX_BOOL	CreateTifFile(CFX_DIBitmap* pBitmap);

	FX_BOOL	CreateImgFile(CFX_DIBitmap* pBitmap, CFX_WideString wsSuffix);

	FX_BOOL GetFileName(CFX_WideString wsFilePathFullName, CFX_WideString& wsFileName);
	FX_BOOL GetFileSuffix(CFX_WideString wsFilePathFullName, CFX_WideString& wsSuffix);

	FX_BOOL DoGaussBluror(CFX_DIBitmap* pBitMap, CFX_Rect rect, FX_DWORD dwHorzBlur, FX_DWORD dwVertBlur);

	void OFDImgReplace(COFD_ImageObject* pImageObj);

protected:
	IOFD_WriteDocument*		m_pWriteDocument;
	IOFD_Page*				m_pPage;
	CFX_OFDCoverInfoXml*	m_pInfoXml;

	IFX_FileRead*			m_pBlurFileRead;
};


#define _XML_BASEINFO_OBJTYPE_		0
#define _XML_BASEINFO_PAGEID_		1
#define _XML_BASEINFO_OBJID_		2

// Text Obj 
#define _XML_BASEINFO_PIECESTART_	3
#define _XML_BASEINFO_PIECEEND_		4
#define _XML_BASEINFO_COUNT_		5			// xml文件里，除了 Codes 实际值，其它项的个数

// Image Obj
#define _XML_BASEINFO_IMGTYPE_		3
#define _XML_BASEINFO_STREAMEIMAGE_	4


class CFX_OFDInfoReCover : public CFX_Object
{
public:
	CFX_OFDInfoReCover(IOFD_WriteDocument* pWriteDocument);
	~CFX_OFDInfoReCover();

	void RecoverObj(CFX_ByteString bsXmlContent);
	
protected:
	void RecoverObj(CXML_Element* pElement);

	void RecoverObjText(CXML_Element* pElement);
	void RecoverObjImg(CXML_Element* pElement);

	const COFD_ContentObject* GetOfdObject(IOFD_Page* pPage, FX_INT32 nObjID);

	void ObjTextReCover(IOFD_Page* pPage, COFD_TextObject* pTextObj, CXML_Element* pElement);
	
	void ObjTextReCover(const COFD_TextPiece* pPiece, FX_INT32 nPieceIndex, FX_INT32 nXmlIndex, CXML_Element* pElement);

	void OFDImgReplace(COFD_ImageObject* pImageObj, CXML_Element* pElement);
	IFX_FileRead* GetImgReadFromXml(CXML_Element* pElement, CFX_WideString& wsImgType);

protected:
	IOFD_WriteDocument*		m_pWriteDocument;
	CFX_OFDCoverInfoXml*	m_pInfoXml;
};


#define _OFDCOVER_NAMESPACE_	"OFD:CoverInfo"

class CFX_OFDCoverInfoXml : public CFX_Object
{
public:
	CFX_OFDCoverInfoXml();
	~CFX_OFDCoverInfoXml();

	FX_BOOL Init(CFX_WideString wsXmlFile);
	FX_BOOL Initialize(CFX_ByteString bsXmlContent);

	void InitObjInfo();

	void SetObjType(FX_INT32 nType);

	void SetPageID(FX_INT32 nPageID);
	void SetObjID(FX_INT32 nObjID);

	// Write Text Obj
	void SetPieceStartIndex(FX_INT32 nStartIndex);
	void SetPieceEndIndex(FX_INT32 nEndIndex);

	void SetPieceStartCodesPos(FX_INT32 nPos);					// 不使用这些，涉及到piece 的，就将整个piece的codes保存
	void SetPieceEndCodesPos(FX_INT32 nPos);

	void SetCodesContents(CFX_WideString wsCodes);				// 如果是多个piece的codes，则多次调用

	// Write Image Obj
	void SetImgObjType(CFX_WideString wsType);
	void SetImgStream(FX_LPWSTR pwContents);

	void EndObjInfo();

	void OutputXmlFile();
	void OutputXmlStream(CFX_ByteString& bsContent);

	// Read Obj
	FX_INT32 GetElementCount();

	CXML_Element* GetChildElement(FX_INT32 nIndex);

	FX_INT32 GetAttrValueCount(CXML_Element* pChild);

	CFX_WideString GetAttrValue(CXML_Element* pChild, FX_INT32 nIndex);

protected:
	CFX_ByteString GetNameSpace();

protected:
	CXML_Element*		m_pElement;
	CXML_Element*		m_pObjElement;
	CFX_WideString		m_wsXmlFile;

	CFX_ByteString		m_bsContent;
	FX_INT32			m_nIndex;
};

#endif //_FS_OFDINFOCOVER_H_