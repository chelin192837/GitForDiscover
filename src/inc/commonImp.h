#ifndef _COMMON_IMP_H_
#define _COMMON_IMP_H_

#include "../../include/common.h"
#include "../fxcore.h"

class FSPDFDocImp;

struct FONT_PROPERTIES
{
	FS_DWORD fontStyle;
	FS_INT32 weight;
	FS_INT32 charset;
	union
	{
		FS_INT32 faceIndex;
		FS_INT32 fontID;
	};
	//union
	//{
	//	FSCRT_BSTR fontName;
	//	FSCRT_FILE fontFile;
	//};
};

#define FSCRT_FONTLOADTYPE_UNKNOWN		0
#define FSCRT_FONTLOADTYPE_ATTRIBUTE	1
#define FSCRT_FONTLOADTYPE_STANDARD		2
#define FSCRT_FONTLOADTYPE_FILE			3
#define FSCRT_FONTLOADTYPE_EXTEND		4
#define FSCRT_FONTLOADTYPE_FROMPDFFONT	5

class FSFontImp: public FSFont, public CFX_Object
{
public:
	virtual void release();
	virtual FSString getName();
	FS_STANDARDFONTID getFontID();
	
	FSFontImp();
	~FSFontImp();
	FSFontImp* retain();

	FS_BOOL initFromAttribute(const char* fontName, FS_DWORD fontStyles, FS_INT32 weight, FS_FONTCHARSET charset);
	FS_BOOL initFromStandardID(FS_STANDARDFONTID fontID);
	FS_BOOL initFromPDFFont(FSPDFDocImp* pDoc, CPDF_Font* pPDFFont);
	FS_BOOL initMap();
	CFX_WideString getPsName();
	IFX_FontEx* getFontEx() { return m_pFontEx; }
	CPDF_Font*	toPDFFont(CPDF_Document* pdfDoc);

	CPDF_Dictionary* getPDFFontDict(FSPDFDocImp* pDoc);
	FS_BOOL removeMapRecord(FSPDFDocImp* pDoc);
	FS_BOOL	isMapEmpty();
private:
	FSCRT_DEFINELOCK(m_lock);

	FS_BOOL			m_bOwnFont;
	IFX_FontEx*		m_pFontEx;

	FONT_PROPERTIES		m_pro;
	FS_INT32			m_iLoadType;

	FS_INT32			m_nCountRef;

	FSCRT_DEFINELOCK(m_lockMap);
	//<FSPDFDocImp*, font dict>
	CFX_MapPtrToPtr*	m_pMapDoc2PDFFont;
};

class FSPDFPathImp : public FSPDFPath, public CFX_Object
{
public:
	virtual void release();
	virtual void clear();
	virtual FS_BOOL removePoint(FS_INT32 index);
	virtual FS_INT32 getPointCount();
	virtual FS_BOOL	setPoint(FS_INT32 index, FSPointF point, FS_PATHPOINTTYPE pointType);
	virtual FSPointF getPoint(FS_INT32 index);
	virtual FS_PATHPOINTTYPE getPointType(FS_INT32 index);
	virtual FS_BOOL	moveTo(FSPointF point);
	virtual FS_BOOL lineTo(FSPointF point);
	virtual FS_BOOL cubicBezierTo(FSPointF point1, FSPointF point2, FSPointF point3);
	virtual FS_BOOL closeFigure();
	virtual ~FSPDFPathImp();

	FSPDFPathImp();
	FS_BOOL init();
	FSPDFPathImp*	retain();
	FS_BOOL	setPointCount(FS_INT32 pointCount);
private:
	CFX_PathData*	m_pPath;

	FSCRT_DEFINELOCK(m_lock);
	FS_INT32		m_nCountRef;
};

class FSFileSpecImp: public FSFileSpec,  public CFX_Object
{
public:
	FSFileSpecImp(FSPDFDoc* pdfDoc, CPDF_Dictionary* pDict);
	FS_BOOL create();
	void release();
	virtual ~FSFileSpecImp() {}
	virtual FSString getFileName();
	virtual void setFileName(const char* fileName);
	virtual FS_INT64 getFileSize();
	virtual FSFileRead* getFileData();
	virtual FS_BOOL embed(const char* filePath);
	virtual FS_BOOL isEmbedded();
	virtual FSString getDescription();
	virtual void setDescription(const char* description);
	virtual FSDateTime getCreationDateTime();
	virtual void setCreationDateTime(const FSDateTime* dateTime);
	virtual FSDateTime getModifiedDateTime();
	virtual void setModifiedDateTime(const FSDateTime* dateTime); 
	virtual FSString getChecksum();
	virtual void setChecksum(const char* checksum);

	FS_RESULT getParamDict(CPDF_Dictionary*& pDict);
	CPDF_Dictionary* getDict() {return m_pAttachmentDict;}
	FSPDFDoc* getDoc() {return m_pDoc;}
private:
	FSPDFDoc* m_pDoc;
	CPDF_Dictionary* m_pAttachmentDict;
};


#endif //_COMMON_IMP_H_
