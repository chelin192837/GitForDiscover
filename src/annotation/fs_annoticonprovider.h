#ifndef _FS_ANNOTICONPROVIDER_H_
#define _FS_ANNOTICONPROVIDER_H_

#include "../../include/common.h"
#include "../../include/fs_annot.h"

#include "../fxcore.h"

class FSPDFPage;
class FSPDFAnnot;

class CFSPDF_AnnotIconProvider: public CFX_Object
{
public:
	CFSPDF_AnnotIconProvider(CPDF_Document* pPDFDoc);
	~CFSPDF_AnnotIconProvider();

	static FS_BOOL		getDisplaySize(FSM_ANNOTTYPE annotType, FX_BSTR bsIconName, FS_FLOAT& iconWidth, FS_FLOAT& iconHeight);

	FS_BOOL				generateAPStream(FSPDFMarkupImp* pMarkupImp);

private:
	CPDF_Document*		m_pPDFDoc;

	FS_BOOL				isSupportIconProvider(FSM_ANNOTTYPE annotType);
	FS_BOOL				isSupportStdIcon(FSM_ANNOTTYPE annotType) const;

	CFX_ByteString		getAPNameString(FSM_ANNOTTYPE annotType, FX_BSTR bsIconName, FS_ARGB fillColor, FS_ARGB strokeColor) const;
	CPDF_Stream*		findExistingAPStream(const CFX_ByteString &bsAPName) const;
	CPDF_Stream*		createAPFormFromPage(CPDF_Page* pSrcPage);
	void				GetShadingFunctions(CPDF_Stream* pAPStream, CFX_PtrArray &shadings, CFX_PtrArray &functions);
	CPDF_Stream*		createStdFormStream(CPDF_Rect annotRect, FSM_ANNOTTYPE annotType, FX_BSTR bsIconName, FX_ARGB fillColor, FX_ARGB strokeColor = 0);

	CPDF_Stream*		generateDynamicStampAPStream(FSPDFPage* pIconPage, CFX_FloatRect stampAnnotRect);
};

/////////////////////////////////////////////////////////////
// For standard icon provider.
class CFSPDFAnnot_Point : public CPDF_Point
{
public : 
	CFSPDFAnnot_Point() ; 
	CFSPDFAnnot_Point(FX_FLOAT fx, FX_FLOAT fy) ; 
	CFSPDFAnnot_Point(const CFSPDFAnnot_Point& point) ; 
};

class CFSPDFAnnot_PathData : public CFX_Object
{
public : 
	CFSPDFAnnot_PathData() ; 
	CFSPDFAnnot_PathData( const CFSPDFAnnot_Point& point, int nType ) ; 

	CFSPDFAnnot_Point     m_point ; 
	int					m_nType ; 
};

class CFSPDFAnnot_Color : public CFX_Object
{
public:
	CFSPDFAnnot_Color(FX_INT32 type = 0, FX_FLOAT color1 = 0.0f, FX_FLOAT color2 = 0.0f, FX_FLOAT color3 = 0.0f, FX_FLOAT color4 = 0.0f);
	~CFSPDFAnnot_Color(){}

	//void ConvertColorType(int nColorType); 

public:
	// 0 : transparent
	// 1 : gray
	// 2 : rgb
	// 3 : cmyk 
	FX_INT32 m_nColorType; 
	FX_FLOAT m_flColor1; 
	FX_FLOAT m_flColor2; 
	FX_FLOAT m_flColor3; 
	FX_FLOAT m_flColor4; 
};

#define FSPDFANNOT_DEFAULT_BLACKCOLOR	CFSPDFAnnot_Color(COLORTYPE_GRAY, 0)

#ifndef _FSPDF_DEF_MACRO_ANNOT_ICONNAME_
#define _FSPDF_DEF_MACRO_ANNOT_ICONNAME_
/** @brief	Note icon type: Check mark. */
#define FSPDF_ANNOT_ICONNAME_TEXT_CHECK					"Check"
/** @brief	Note icon type: Circle. */
#define FSPDF_ANNOT_ICONNAME_TEXT_CIRCLE				"Circle"
/** @brief	Note icon type: Comment. */
#define FSPDF_ANNOT_ICONNAME_TEXT_COMMENT				"Comment"
/** @brief	Note icon type: Cross. */
#define FSPDF_ANNOT_ICONNAME_TEXT_CROSS					"Cross"
/** @brief	Note icon type: Help. */
#define FSPDF_ANNOT_ICONNAME_TEXT_HELP					"Help"
/** @brief	Note icon type: Insert. */
#define FSPDF_ANNOT_ICONNAME_TEXT_INSERT				"Insert"
/** @brief	Note icon type: Key. */
#define FSPDF_ANNOT_ICONNAME_TEXT_KEY					"Key"
/** @brief	Note icon type: New Paragraph. */
#define FSPDF_ANNOT_ICONNAME_TEXT_NEWPARAGRAPH			"NewParagraph"
/** @brief	Note icon type: Note. */
#define FSPDF_ANNOT_ICONNAME_TEXT_NOTE					"Note"
/** @brief	Note icon type: Paragraph. */
#define FSPDF_ANNOT_ICONNAME_TEXT_PARAGRAPH				"Paragraph"
/** @brief	Note icon type: Right Arrow. */
#define FSPDF_ANNOT_ICONNAME_TEXT_RIGHARROW				"RightArrow"
/** @brief	Note icon type: Right Pointer. */
#define FSPDF_ANNOT_ICONNAME_TEXT_RIGHPOINTER			"RightPointer"
/** @brief	Note icon type: Star. */
#define FSPDF_ANNOT_ICONNAME_TEXT_STAR					"Star"
/** @brief	Note icon type: Up Arrow. */
#define FSPDF_ANNOT_ICONNAME_TEXT_UPARROW				"UpArrow"
/** @brief	Note icon type: Up-left Arrow. */
#define FSPDF_ANNOT_ICONNAME_TEXT_UPLEFTARROW			"UpLeftArrow"

/** @brief	File attachment icon type: Graph. */
#define FSPDF_ANNOT_ICONNAME_FILEATTACH_GRAPH			"Graph"
/** @brief	File attachment icon type: PaperClip. */
#define FSPDF_ANNOT_ICONNAME_FILEATTACH_PAPERCLIP		"Paperclip"
/** @brief	File attachment icon type: PushPin. */
#define FSPDF_ANNOT_ICONNAME_FILEATTACH_PUSHPIN			"PushPin"
/** @brief	File attachment icon type: Tag. */
#define FSPDF_ANNOT_ICONNAME_FILEATTACH_TAG				"Tag"

#define FSPDF_ANNOT_ICONNAME_SIGNATURE_FOXIEFILAG		"FoxitFlag"
#endif //_FSPDF_DEF_MACRO_ANNOT_ICONNAME_

#ifndef _FSPDF_DEF_MACRO_ANNOT_PATHTYPE_
#define _FSPDF_DEF_MACRO_ANNOT_PATHTYPE_

#define	FPDFANNOT_PATHTYPE_PATHDATA		0
#define	FPDFANNOT_PATHTYPE_STREAM		1

#endif //_FSPDF_DEF_MACRO_ANNOT_PATHTYPE_

#ifndef _FSPDF_DEF_MACRO_ANNOT_PATHDATA_TYPE_
#define _FSPDF_DEF_MACRO_ANNOT_PATHDATA_TYPE_

#define FPDFANNOT_PATHDATA_UNKNOW			0
#define FPDFANNOT_PATHDATA_MOVETO			1
#define FPDFANNOT_PATHDATA_LINETO			2
#define FPDFANNOT_PATHDATA_BEZIERTO			3

#endif //_FSPDF_DEF_MACRO_ANNOT_PATHDATA_TYPE_

#ifndef _FSPDF_DEF_MACRO_CONSTANT_
#define _FSPDF_DEF_MACRO_CONSTANT_

//macro  for  constant
#define	 FPDFANNOTUTIL_PI					3.14159265358979f
//#define  FPDFANNOTUTIL_MIN					0.0001f
//#define  FPDFANNOTUTIL_MAXIMUM_DEFLATESIZE	16
#define	 FPDFANNOTUTIL_BEZIER				0.5522847498308f

#endif //_FSPDF_DEF_MACRO_CONSTANT_

class CFSPDF_AnnotStdIconProvider : public CFX_Object
{
public:
	static CFX_ByteString	GetIconAppStream(CFX_ByteString bsAnnotType, CFX_ByteString csIconType, const CPDF_Rect& crRect, 
											const CFSPDFAnnot_Color& crFill, const CFSPDFAnnot_Color& crStroke = FSPDFANNOT_DEFAULT_BLACKCOLOR); 
	static CFX_ByteString	GetColorAppStream( const CFSPDFAnnot_Color& color, const FX_BOOL& bFillOrStroke = TRUE );
	static CFX_ByteString	GetAppStreamFromArray( const CFSPDFAnnot_PathData* pPathData, FX_INT32 nCount );
	static void				GetPathDataFromArray( CFX_PathData& cpPath, const CFSPDFAnnot_PathData* pPathData, FX_INT32 nCount);
	
	//For icon ap of Text annotation.
	static void GetGraphics_Check( CFX_ByteString& csPathData, CFX_PathData& cpPath, const CPDF_Rect& crRect, const FX_INT32 nType ) ; 
	static void GetGraphics_Circle( CFX_ByteString& csPathData, CFX_PathData& cpPath, const CPDF_Rect& crRect, const FX_INT32 nType ) ; 
	static void GetGraphics_Comment( CFX_ByteString& csPathData, CFX_PathData& cpPath, const CPDF_Rect& crRect, const FX_INT32 nType ) ; 
	static void GetGraphics_Cross( CFX_ByteString& csPathData, CFX_PathData& cpPath, const CPDF_Rect& crRect, const FX_INT32 nType ) ; 
	static void GetGraphics_Help( CFX_ByteString& csPathData, CFX_PathData& cpPath, const CPDF_Rect& crRect, const FX_INT32 nType ) ; 
	static void GetGraphics_InsertText( CFX_ByteString& csPathData, CFX_PathData& cpPath, const CPDF_Rect& crRect, const FX_INT32 nType ) ; 
	static void GetGraphics_Key( CFX_ByteString& csPathData, CFX_PathData& cpPath, const CPDF_Rect& crRect, const FX_INT32 nType ) ; 
	static void GetGraphics_NewParagraph( CFX_ByteString& csPathData, CFX_PathData& cpPath, const CPDF_Rect& crRect, const FX_INT32 nType ) ; 
	static void GetGraphics_TextNote( CFX_ByteString& csPathData, CFX_PathData& cpPath, const CPDF_Rect& crRect, const FX_INT32 nType ) ; 
	static void GetGraphics_Paragraph( CFX_ByteString& csPathData, CFX_PathData& cpPath, const CPDF_Rect& crRect, const FX_INT32 nType ) ; 
	static void GetGraphics_RightArrow( CFX_ByteString& csPathData, CFX_PathData& cpPath, const CPDF_Rect& crRect, const FX_INT32 nType ) ; 
	static void GetGraphics_RightPointer( CFX_ByteString& csPathData, CFX_PathData& cpPath, const CPDF_Rect& crRect, const FX_INT32 nType ) ; 
	static void GetGraphics_Star( CFX_ByteString& csPathData, CFX_PathData& cpPath, const CPDF_Rect& crRect, const FX_INT32 nType ) ; 
	static void GetGraphics_UpArrow( CFX_ByteString& csPathData, CFX_PathData& cpPath, const CPDF_Rect& crRect, const FX_INT32 nType ) ; 
	static void GetGraphics_UpLeftArrow( CFX_ByteString& csPathData, CFX_PathData& cpPath, const CPDF_Rect& crRect, const FX_INT32 nType ) ; 

	//For icon ap of FileAttachment annotation
	static void GetGraphics_Graph( CFX_ByteString& csPathData, CFX_PathData& cpPath, const CPDF_Rect& crRect, const FX_INT32 nType ) ; 
	static void GetGraphics_Paperclip( CFX_ByteString& csPathData, CFX_PathData& cpPath, const CPDF_Rect& crRect, const FX_INT32 nType ) ; 
	static void GetGraphics_PushPin( CFX_ByteString& csPathData, CFX_PathData& cpPath, const CPDF_Rect& crRect, const FX_INT32 nType ) ; 
	static void GetGraphics_Tag( CFX_ByteString& csPathData, CFX_PathData& cpPath, const CPDF_Rect& crRect, const FX_INT32 nType ) ; 
	
	//For icon ap of Signature
	static void GetGraphics_FoxitFlag( CFX_ByteString& csPathData, CFX_PathData& cpPath, const CPDF_Rect& crRect, const FX_INT32 nType ) ;
};

#endif // _FS_ANNOTICONPROVIDER_H_