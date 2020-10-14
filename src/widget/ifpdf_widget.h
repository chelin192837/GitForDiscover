#ifndef _IFSPDF_WIDGET_H
#define _IFSPDF_WIDGET_H

class FSAnnot;

//*****************************************************************************
//* Widget common styles
//*****************************************************************************
#define FPDFW_WGTSTYLE_Border			(1L << 2)	//border
#define FPDFW_WGTSTYLE_HScroll			(1L << 9)	//horizontal scroll bar
#define FPDFW_WGTSTYLE_VScroll			(1L << 11)	//vertical scroll bar
#define FPDFW_WGTSTYLE_LeftScrollbar	(1L << 12)	//vertical scroll bar shows in left side

#define FPDFW_WGTSTYLE_LeftAlignment	(0L << 13)	//left alignment
#define FPDFW_WGTSTYLE_CenterAlignment	(1L << 13)	//center alignment horizontally
#define FPDFW_WGTSTYLE_RightAlignment	(2L << 13)	//right alignment
#define FPDFW_WGTSTYLE_TopAlignment		(0L << 15)	//top alignment
#define FPDFW_WGTSTYLE_VCenterAlignment	(1L << 15)	//center alignment vertically
#define FPDFW_WGTSTYLE_BottomAlignment	(2L << 15)	//bottom alignment

#define FPDFW_WGTSTYLE_RTLReading		(1L << 17)	//right-to-left reading order
#define FPDFW_WGTSTYLE_RTLLayout		(1L << 18)	//right-to-left layout

#define FPDFW_WGTSTYLE_EdgeNone			(0L << 19)	//no edge
#define FPDFW_WGTSTYLE_EdgeFlat			(1L << 19)	//flat edge
#define FPDFW_WGTSTYLE_EdgeRaised		(2L << 19)	//raised edge
#define FPDFW_WGTSTYLE_EdgeSunken		(3L << 19)	//sunken edge
#define FPDFW_WGTSTYLE_EdgeMask			(3L << 19)	//edge mask

#define FPDFW_WGTSTYLE_Shadow			(3L << 21)	//shadow

#define FPDFW_WGTSTYLE_Group			(1L << 22)	//specifies the first widget of a group of widgets 
#define FPDFW_WGTSTYLE_TabStop			(1L << 23)	//Specifies a widget that can receive the keyboard focus when the user presses the TAB key


//*****************************************************************************
//* Widget common states
//*****************************************************************************
#define FPDFW_WGTSTATE_Disabled			(1L << 2)	//disabled
#define FPDFW_WGTSTATE_Focused			(1L << 4)	//focused
#define FPDFW_WGTSTATE_Invisible		(1L << 5)	//invisible

#define FPDFW_WGTSTATE_MAX				(6)			//user definition scope

//*****************************************************************************
//* Test mouse position
//*****************************************************************************
#define FPDFW_WGTHITTEST_Unknown		0
#define FPDFW_WGTHITTEST_Client			1
#define FPDFW_WGTHITTEST_Left			2
#define FPDFW_WGTHITTEST_Top			3
#define FPDFW_WGTHITTEST_Right			4
#define FPDFW_WGTHITTEST_Bottom			5
#define FPDFW_WGTHITTEST_LeftTop		6
#define FPDFW_WGTHITTEST_RightTop		7
#define FPDFW_WGTHITTEST_LeftBottom		8
#define FPDFW_WGTHITTEST_RightBottom	9
#define FPDFW_WGTHITTEST_Icon			10
#define FPDFW_WGTHITTEST_Titlebar		11
#define FPDFW_WGTHITTEST_MinBox			12
#define FPDFW_WGTHITTEST_MaxBox			13
#define FPDFW_WGTHITTEST_CloseBox		14
#define FPDFW_WGTHITTEST_HScrollBar		15
#define FPDFW_WGTHITTEST_VScrollBar		16
#define FPDFW_WGTHITTEST_Border			17
#define FPDFW_WGTHITTEST_Edge			18
#define FPDFW_WGTHITTEST_Edit			19
#define FPDFW_WGTHITTEST_Transparent	20

#define FPDFW_WGTHITTEST_MAX			21


// Pointer to a FX_INT32.
typedef FX_INT32	FPDFW_ERR;
#define FPDFW_ERR_Succeeded				0
#define FPDFW_ERR_Indefinite			-1

//class CFFL_Widget;
//class CPWL_FontMap;
class CFSCRT_LTPDFAnnot;
//*****************************************************************************
//* 
//*****************************************************************************
class CFSPDF_WidgetProperties : public CFX_Object
{
public:
	CFSPDF_WidgetProperties()
		: m_rtWidget(0, 0, 0, 0)
		, m_dwStyles(0)
		, m_dwStyleExes(0)
		, m_dwStates(0)
		, m_sBackgroundColor(0x00000000)
		, m_nBorderStyle(0)
		, m_dwBorderWidth(1)
		, m_sBorderColor(0x00000000)
		, m_sTextColor(0x00000000)
		, m_sTextStrokeColor(0x00000000)
		, m_nTransparency(255)
		, m_fFontSize(9.0f)
		, m_fFontName("")
		, m_nDash(3)
		, m_nDashGap(0)
		, m_nDashPhase(0)
		, m_pAttachedData(NULL)
		, m_pWidget(NULL)
		, m_pSystemHandler(NULL)
	{
	}
    
    ~CFSPDF_WidgetProperties();

	CFX_FloatRect				m_rtWidget;				//required	
	FX_DWORD					m_dwStyles;
	FX_DWORD					m_dwStyleExes;
	FX_DWORD					m_dwStates;
	FX_ARGB						m_sBackgroundColor;		//optional
	FX_INT32					m_nBorderStyle;			//optional
	FX_INT32					m_dwBorderWidth;			//optional
	FX_ARGB						m_sBorderColor;			//optional
	FX_ARGB						m_sTextColor;				//optional
	FX_ARGB						m_sTextStrokeColor;		//optional
	FX_INT32					m_nTransparency;			//optional
	FX_FLOAT					m_fFontSize;				//optional
	CFX_ByteString				m_fFontName;
	FX_INT32					m_nDash;
	FX_INT32					m_nDashGap;
	FX_INT32					m_nDashPhase;
	void*						m_pAttachedData;			//optional
	CFFL_Widget*				m_pWidget;
	IFX_SystemHandler*			m_pSystemHandler;
	IFX_Edit_FontMap*			m_pFontMap;
};

class IFSPDF_Widget
{
public:
	virtual void				Release();

	virtual FPDFW_ERR			Initialize(FSAnnot* pAnnot, CFSPDF_WidgetProperties* pProperties = NULL);
	virtual FPDFW_ERR			Finalize();
	
	virtual FPDFW_ERR			GetWidgetRect(CFX_FloatRect &rect);
	virtual FPDFW_ERR			SetWidgetRect(const CFX_FloatRect &rect);
	virtual FPDFW_ERR			GetClientRect(CFX_FloatRect &rect);
	virtual FX_DWORD			GetStyles();
	virtual FPDFW_ERR			ModifyStyles(FX_DWORD dwStylesAdded, FX_DWORD dwStylesRemoved);
	virtual FX_DWORD			GetStylesEx();
	virtual FPDFW_ERR			ModifyStylesEx(FX_DWORD dwStylesExAdded, FX_DWORD dwStylesExRemoved);
	virtual FX_DWORD			GetStates();
	virtual FPDFW_ERR			SetStates(FX_DWORD dwStates, FX_BOOL bSet = TRUE);
	virtual FPDFW_ERR			SetText(const CFX_WideString &wsText);

	virtual FPDFW_ERR			Update();
	virtual FPDFW_ERR			LockUpdate();
	virtual FPDFW_ERR			UnlockUpdate();
		
	virtual FX_DWORD			HitTest(FX_FLOAT fx, FX_FLOAT fy);

	virtual FPDFW_ERR			DrawWidget(CFX_RenderDevice *pDevice, const CFX_Matrix *pMatrix = NULL);

	virtual FX_BOOL				OnKeyDown(FS_DWORD nKeyCode, FS_DWORD nFlags);
	virtual FX_BOOL				OnKeyUp(FS_DWORD nKeyCode, FS_DWORD nFlags);
	virtual FX_BOOL				OnChar(FS_DWORD nKeyCode, FS_DWORD nFlags);
	virtual FX_BOOL				OnString(FX_WSTR string, FS_DWORD nFlags);

	virtual FX_BOOL				OnLButtonDown(FS_DWORD nFlags,const CFX_PointF& point);
	virtual FX_BOOL				OnLButtonUp(FS_DWORD nFlags, const CFX_PointF& point);
	virtual FX_BOOL				OnLButtonDblClk(FS_DWORD nFlags, const CFX_PointF& point);
	virtual FX_BOOL				OnMouseMove(FS_DWORD nFlags, const CFX_PointF& point);
	virtual FX_BOOL				OnRButtonDown(FS_DWORD nFlags, const CFX_PointF& point);
	virtual FX_BOOL				OnRButtonUp(FS_DWORD nFlags, const CFX_PointF& point);
	virtual FX_BOOL				OnRButtonDblClk(FS_DWORD nFlags, const CFX_PointF& point);
	virtual FX_BOOL				OnMouseWheel(FS_DWORD nFlags, short zDelta, const CFX_PointF& point);
	virtual FX_BOOL				OnMouseHover(const CFX_PointF& point);

	virtual void				SetFocus();
	virtual void				KillFocus();
	virtual FPDFW_ERR			GetFocusRect(CFX_FloatRect &rect);

	virtual CFX_ByteString		GetAppearanceStream();

	static FPDFW_ERR			ResetAppearance(CPDF_FormControl* pControl,FX_LPCWSTR sValue);	

protected:
	FX_LPVOID	m_pData;

	IFSPDF_Widget() {}
	~IFSPDF_Widget() {}
};

//*****************************************************************************
//* Push button widget
//*****************************************************************************
class IFSPDF_PushButton : public IFSPDF_Widget
{
public:
	static IFSPDF_PushButton* Create();
};

//*****************************************************************************
//* Check box widget
//*****************************************************************************
class IFSPDF_CheckBox : public IFSPDF_Widget
{
public:
	static IFSPDF_CheckBox* Create();

	//0 Button state is unchecked. 
	//1 Button state is checked. 
	//2 Button state is indeterminate (applies only if the button has the 3STATE style). 
    FX_INT32	GetCheckState();
	FPDFW_ERR 	SetCheckState(FX_INT32 iCheck);
};

//*****************************************************************************
//* Radio button widget
//*****************************************************************************
class IFSPDF_RadioButton : public IFSPDF_Widget
{
public:
	static IFSPDF_RadioButton* Create();

	//0 Button state is unchecked. 
	//1 Button state is checked. 
	FX_INT32	GetCheckState();
	FPDFW_ERR	SetCheckState(FX_INT32 iCheck);
};

//*****************************************************************************
//* Edit extend style
//*****************************************************************************
#define FPDFW_STYLEEXT_EDT_ReadOnly			(1L << 0)	//prevents the user from typing or editing text in the edit control.
#define FPDFW_STYLEEXT_EDT_MultiLine		(1L << 1)	//designates a multiline edit control
#define FPDFW_STYLEEXT_EDT_WantReturn		(1L << 2)	//specifies that a carriage return be inserted when the user presses the enter key while entering text into a multiline edit control.
#define FPDFW_STYLEEXT_EDT_NoHideSel		(1L << 3)	//the selected text is inverted, even if the control does not have the focus.
#define FPDFW_STYLEEXT_EDT_AutoHScroll		(1L << 4)	//automatically scrolls text to the right by when the user types a character at the end of the line
#define FPDFW_STYLEEXT_EDT_AutoVScroll		(1L << 5)	//automatically scrolls text up one page when the user presses the enter key on the last line.
#define FPDFW_STYLEEXT_EDT_NoRedoUndo		(1L << 6)	//
#define FPDFW_STYLEEXT_EDT_Validate			(1L << 7)
#define FPDFW_STYLEEXT_EDT_Password			(1L << 8)
#define FPDFW_STYLEEXT_EDT_Number			(1L << 9)
#define	FPDFW_STYLEEXT_EDT_HSelfAdaption    (1L << 10)
#define	FPDFW_STYLEEXT_EDT_VSelfAdaption    (1L << 11)
#define FPDFW_STYLEEXT_EDT_AutoFontSize		(1L << 12)
#define FPDFW_STYLEEXT_EDT_RichEdit			(1L << 13)

//Layout
#define FPDFW_STYLEEXT_EDT_VerticalLayout	(1L << 14)	//use vertical layout mode
#define FPDFW_STYLEEXT_EDT_VerticalChars	(1L << 15)	//use vertical character controls
#define FPDFW_STYLEEXT_EDT_ReverseLine		(1L << 16)	//reverse line direction, use right-to-left direction for horizontal line (or bottom-to-top direction for vertical line)
#define FPDFW_STYLEEXT_EDT_ArabicShapes		(1L << 17)	//apply arabic transform rules, for digital characters, comma, and dot
#define FPDFW_STYLEEXT_EDT_ExpandTab		(1L << 18)	//expand tab character
#define FPDFW_STYLEEXT_EDT_CombText			(1L << 19)	//lay out as identical character space

//Alignment
#define FPDFW_STYLEEXT_EDT_HNear			(0L << 20)	//left or near alignment
#define FPDFW_STYLEEXT_EDT_HCenter			(1L << 20)	//center alignment
#define FPDFW_STYLEEXT_EDT_HFar				(2L	<< 20)	//right or far alignment
#define FPDFW_STYLEEXT_EDT_VNear			(0L << 22)
#define FPDFW_STYLEEXT_EDT_VCenter			(1L << 22)
#define FPDFW_STYLEEXT_EDT_VFar				(2L << 22)
#define FPDFW_STYLEEXT_EDT_Justified		(1L << 24)	//justifying alignment
#define FPDFW_STYLEEXT_EDT_Distributed		(2L << 24)	//distributed alignment

#define FPDFW_STYLEEXT_EDT_HAlignMask			(3L	<< 20)
#define FPDFW_STYLEEXT_EDT_VAlignMask			(3L	<< 22)
#define FPDFW_STYLEEXT_EDT_HAlignModeMask		(3L	<< 24)

class IFSPDF_Widget_Notify
{
public:
	virtual void	QueryWherePopup(void* pPrivateData, FX_FLOAT fPopupMin,FX_FLOAT fPopupMax, FX_INT32 & nRet, FX_FLOAT & fPopupRet) = 0; //nRet: (0:bottom 1:top)
	virtual void	OnBeforeKeyStroke(FX_BOOL bEditOrList, void* pPrivateData, FX_INT32 nKeyCode, CFX_WideString & strChange, 
						const CFX_WideString& strChangeEx, int nSelStart, int nSelEnd,FX_BOOL bKeyDown, FX_BOOL & bRC, FX_BOOL & bExit, FX_DWORD nFlag) = 0;
	virtual void	OnAfterKeyStroke(FX_BOOL bEditOrList, void* pPrivateData, FX_BOOL& bExit, FX_DWORD nFlag) = 0;
	virtual void	OnPopupPreOpen(void* pPrivateData, FX_BOOL& bExit) = 0;
	virtual void	OnPopupPostOpen(void* pPrivateData, FX_BOOL& bExit) = 0;
};

//*****************************************************************************
//* Edit widget
//*****************************************************************************
class IFSPDF_Edit : public IFSPDF_Widget
{
public:
	static IFSPDF_Edit* Create();

	//Set text by wide string
	FPDFW_ERR		SetText(const CFX_WideString &wsText);
	//Get the text total length
	FX_INT32		GetTextLength() const;
	//Get text by range. nCount = -1 means the range from nStart to the text end.
	FPDFW_ERR		GetText(CFX_WideString &wsText, FX_INT32 nStart = 0, FX_INT32 nCount = -1) const;
	//Clear text
	FPDFW_ERR		ClearText();

	FPDFW_ERR		AddSelRange(FX_INT32 nStart, FX_INT32 nCount = -1);
	FX_INT32		CountSelRanges();
	FX_INT32		GetSelRange(FX_INT32& nStart, FX_INT32 &nEnd);
	FX_INT32		SetSelRange(FX_INT32 nStart, FX_INT32 nEnd);
	FPDFW_ERR		ClearSelections();

	//if set the count of charArray , then all words is shown in equal space.
	FPDFW_ERR		SetCharArray(FX_INT32 nCharArray);
	//Get the the maximum number of characters
	FX_INT32		GetLimit();
	//Set the the maximum number of characters
	FPDFW_ERR		SetLimit(FX_INT32 nLimit);

	FPDFW_ERR		SetAliasChar(FX_WCHAR wAlias); 

	//Format
	FPDFW_ERR		SetFormatString(const CFX_WideString &wsFormat);
	 
	//These four function followed can be undo/redo
	//Insert text
	FPDFW_ERR		Insert(FX_INT32 nStart, FX_LPCWSTR lpText, FX_INT32 nLen);
	//Delete all selections
	FPDFW_ERR		DeleteSelections();
	//Delete text range
	FPDFW_ERR		DeleteRange(FX_INT32 nStart, FX_INT32 nCount = -1);
	//
	FPDFW_ERR		ReplaceSelections(const CFX_WideStringC &wsReplace);
	//Replace text range
	FPDFW_ERR		Replace(FX_INT32 nStart, FX_INT32 nLen, const CFX_WideStringC &wsReplace);
	 
	//Copy/Cut/Paste
	FPDFW_ERR		DoClipboard(FX_INT32 iCmd);/* 1 Copy; 2 Cut; 3 Paste*/
	 
	//Redo/Undo
	FX_BOOL			Redo(FX_BSTR bsRecord);
	FX_BOOL			Undo(FX_BSTR bsRecord);

	// Horizontal scale of all characters
	FPDFW_ERR		SetHorzScale(FX_INT32 nHorzScale);
	// Char space
	FPDFW_ERR		SetCharSpace(FX_FLOAT fCharSpace);
	// Line leading
	FPDFW_ERR		SetLineLeading(FX_FLOAT fLineLeading);
	
	//Get content rect
	FPDFW_ERR		GetContentRect(CFX_FloatRect &rect);

	//Get text ap stream
	CFX_ByteString	GetTextAppearanceStream(const CPDF_Point & ptOffset);

	void			SetFillerNotify(IFSPDF_Widget_Notify* pNotify);
	void			SetTextMatrix(CPDF_Matrix& mtText);

	void			SetRichText(FX_LPCWSTR xml, FX_LPCWSTR ds = NULL, FX_BOOL bAutoSetAgin = TRUE);
};



//*****************************************************************************
//* ListBox extend style
//*****************************************************************************
#define	FPDFW_STYLEEXT_LTB_MultiSelection		(1L << 0)
#define	FPDFW_STYLEEXT_LTB_Sort					(1L << 1)
#define	FPDFW_STYLEEXT_LTB_ShowScrollBarAlways	(1L << 2)
#define	FPDFW_STYLEEXT_LTB_MultiColumn			(1L << 3)
#define	FPDFW_STYLEEXT_LTB_LeftAlign			(0L << 4)
#define	FPDFW_STYLEEXT_LTB_CenterAlign			(1L << 4)
#define	FPDFW_STYLEEXT_LTB_RightAlign			(2L << 4)
#define	FPDFW_STYLEEXT_LTB_MultiLine			(1L << 6)
#define	FPDFW_STYLEEXT_LTB_OwnerDraw			(1L << 7)
#define	FPDFW_STYLEEXT_LTB_Icon                 (1L << 8)
#define	FPDFW_STYLEEXT_LTB_Check                (1L << 9)

#define FPDFW_STYLEEXT_LTB_AlignMask			(3L << 4)

//*****************************************************************************
//* ListBox item state
//*****************************************************************************
#define FPDFW_ITEMSTATE_LTB_Selected		(1L << 0)
#define FPDFW_ITEMSTATE_LTB_Focused			(1L << 1)
#define FPDFW_ITEMSTATE_LTB_Checked			(1L << 2)

//*****************************************************************************
//*  ListBox widget
//*****************************************************************************
FX_DEFINEHANDLE(FPDFW_HLISTITEM)

class IFSPDF_ListBox : public IFSPDF_Widget
{
public:
	static IFSPDF_ListBox* Create();

	FPDFW_HLISTITEM	AddString(FX_WSTR wsAdd, FX_BOOL bSelect = FALSE);
	FX_BOOL			DeleteString(FPDFW_HLISTITEM hItem);
	FX_BOOL			DeleteAll();

	FX_INT32		CountSelItems();
	FPDFW_HLISTITEM	GetSelItem(FX_INT32 nIndexSel);
	FPDFW_ERR		SetSelItem(FPDFW_HLISTITEM hItem, FX_BOOL bSelect = TRUE);
	FPDFW_ERR		GetItemText(FPDFW_HLISTITEM hItem, CFX_WideString &wsText);
	FPDFW_ERR		GetScrollPos(FX_FLOAT &fPos, FX_BOOL bVert = TRUE);
	FPDFW_ERR		SetItemHeight(FX_FLOAT fItemHeight);

	FPDFW_HLISTITEM	GetFocusItem();
	FPDFW_ERR		SetFocusItem(FPDFW_HLISTITEM hItem);	

	FX_INT32		CountItems();
	FPDFW_HLISTITEM	GetItem(FX_INT32 nIndex);
	FX_INT32		GetItemIndex(FPDFW_HLISTITEM hItem);
	FPDFW_ERR		SetItemString(FPDFW_HLISTITEM hItem, FX_WSTR wsText);
	FPDFW_ERR		GetItemString(FPDFW_HLISTITEM hItem, CFX_WideString &wsText);

	FPDFW_ERR		SetItemData(FPDFW_HLISTITEM hItem, FX_LPVOID pData);
	FX_LPVOID		GetItemData(FPDFW_HLISTITEM hItem);

	FPDFW_HLISTITEM GetTopVisibleItem();
	FPDFW_ERR		SetTopVisibleItem(FPDFW_HLISTITEM hItem);
	
	FPDFW_HLISTITEM	GetItemAtPoint(FX_FLOAT fx, FX_FLOAT fy);
	FX_DWORD		GetItemStates(FPDFW_HLISTITEM hItem);

	void			SetFillerNotify(IFSPDF_Widget_Notify* pNotify);

};

//*****************************************************************************
//* ComboBox extended style
//*****************************************************************************
#define FPDFW_STYLEEXT_CMB_DropList			(0L << 0)
#define FPDFW_STYLEEXT_CMB_DropDown			(1L << 0)
#define FPDFW_STYLEEXT_CMB_Sort				(1L << 1)
#define FPDFW_STYLEEXT_CMB_ListDrag			(1L << 2)
#define FPDFW_STYLEEXT_CMB_OwnerDraw			(1L << 3)

//Edit alignment
#define FPDFW_STYLEEXT_CMB_EditHNear		(0L << 4)	//left or near alignment
#define FPDFW_STYLEEXT_CMB_EditHCenter		(1L << 4)	//center alignment
#define FPDFW_STYLEEXT_CMB_EditHFar			(2L	<< 4)	//right or far alignment
#define FPDFW_STYLEEXT_CMB_EditVNear		(0L << 6)
#define FPDFW_STYLEEXT_CMB_EditVCenter		(1L << 6)
#define FPDFW_STYLEEXT_CMB_EditVFar			(2L	<< 6)
#define FPDFW_STYLEEXT_CMB_EditJustified	(1L << 8)	//justifying alignment
#define FPDFW_STYLEEXT_CMB_EditDistributed	(2L << 8)	//distributed alignment

#define FPDFW_STYLEEXT_CMB_EditHAlignMask		(3L << 4)
#define FPDFW_STYLEEXT_CMB_EditVAlignMask		(3L << 6)
#define FPDFW_STYLEEXT_CMB_EditHAlignModeMask	(3L << 8)

//List item alignment
#define	FPDFW_STYLEEXT_CMB_ListItemLeftAlign	(0L << 10)
#define	FPDFW_STYLEEXT_CMB_ListItemCenterAlign	(1L << 10)
#define	FPDFW_STYLEEXT_CMB_ListItemRightAlign	(2L << 10)
#define	FPDFW_STYLEEXT_CMB_ListItemText         (0L << 12)
#define	FPDFW_STYLEEXT_CMB_ListItemIconText		(1L << 12)

#define FPDFW_STYLEEXT_CMB_ListItemAlignMask	(3L << 12)
//*****************************************************************************
//* ComboBox
//*****************************************************************************
class IFSPDF_ComboBox : public IFSPDF_Widget
{
public:
	static IFSPDF_ComboBox* Create();

	FX_INT32	AddString(FX_WSTR wsText);
	FX_INT32	AddString(FX_WSTR wsText, CFX_DIBitmap *pIcon);
	FX_INT32	RemoveAt(FX_INT32 iIndex);
	FX_INT32    RemoveAll();
	FX_INT32	CountItems();
	FPDFW_ERR	GetTextByIndex(FX_INT32 iIndex, CFX_WideString &wsText);

	FX_INT32	GetCurSel();
	FPDFW_ERR	SetCurSel(FX_INT32 iSel);

	FPDFW_ERR	SetText(const CFX_WideString &wsText);
	FX_INT32	GetEditTextLength() const;
	FPDFW_ERR	GetEditText(CFX_WideString &wsText, FX_INT32 nStart = 0, FX_INT32 nCount = -1) const;

	FPDFW_ERR	AddEditSelRange(FX_INT32 nStart, FX_INT32 nCount = -1);
	FX_INT32	CountEditSelRanges();
	FX_INT32	GetEditSelRange(FX_INT32 nIndex, FX_INT32 &nStart);
	FPDFW_ERR	ClearEditSelections();

	FX_INT32	GetEditLimit();
	FPDFW_ERR	SetEditLimit(FX_INT32 nLimit);
	FPDFW_ERR	EditDoClipboard(FX_INT32 iCmd);/* 1 Copy; 2 Cut; 3 Paste*/
	FX_BOOL		EditRedo(FX_BSTR bsRecord);
	FX_BOOL		EditUndo(FX_BSTR bsRecord);
	FPDFW_ERR	SetMaxListHeight(FX_FLOAT fMaxHeight);

	FPDFW_ERR	SetItemData(FX_INT32 iIndex, FX_LPVOID pData);
	FX_LPVOID	GetItemData(FX_INT32 iIndex);

	void		SetFillerNotify(IFSPDF_Widget_Notify* pNotify);

	FPDFW_ERR		SetSelRange(FX_INT32 nStart, FX_INT32 nEnd);
	FPDFW_ERR		ReplaceSel(const CFX_WideStringC &wsReplace);
};

/************************************************************************/
/* Label                                                                */
/************************************************************************/
class IFSPDF_Label : public IFSPDF_Widget
{
public:
	static IFSPDF_Label*	Create();
	// set text
	FPDFW_ERR		SetText(const CFX_WideString wsText);
};

/************************************************************************/
/* Image                                                                */
/************************************************************************/
class IFSPDF_Image : public IFSPDF_Widget
{
public:
	static IFSPDF_Image*	Create();
	// set image source
	FPDFW_ERR		SetImageStream(CPDF_Stream * pStream);
};

/************************************************************************/
/* IFSPDF_Signature                                                       */
/************************************************************************/
class IFSPDF_Signature : public IFSPDF_Widget
{
public:
	static IFSPDF_Signature*	Create();

	FPDFW_ERR		SetShowText(FX_BOOL bShowText);
	FPDFW_ERR		SetShowImage(FX_BOOL bShowImage);

	FPDFW_ERR		SetText(const CFX_WideString text);
	FPDFW_ERR		SetImageStream(CPDF_Stream* pStream, FX_LPCSTR sImageAlias);
	FPDFW_ERR		SetDescription(const CFX_WideString description);
};

/************************************************************************/
/* IFSPDF_SystemHandler                                                       */
/************************************************************************/

typedef void					(*TimerCallback)(FX_INT32 idEvent);

class IFSPDF_SystemHandler
{
public:
	virtual void				Release() = 0;
	virtual void				InvalidateRect(FS_LPVOID hWnd, FX_RECT rect) = 0;

	virtual CFX_WideString		GetClipboardText(FS_LPVOID hWnd) = 0;
	virtual FX_BOOL				SetClipboardText(FS_LPVOID hWnd, CFX_WideString string) = 0;
	
	virtual void				SetCursor(FX_INT32 nCursorType) = 0;
	
	virtual FX_INT32			SetTimer(FX_INT32 uElapse, TimerCallback lpTimerFunc) = 0;
	virtual void				KillTimer(FX_INT32 nID) = 0;

};
#endif //_IFSPDF_WIDGET_H



