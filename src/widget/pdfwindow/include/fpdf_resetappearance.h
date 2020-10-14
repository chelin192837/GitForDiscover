#ifndef _FSCRT_ST_PDF_RESETAPPREANCE_H_
#define _FSCRT_ST_PDF_RESETAPPREANCE_H_

class IFX_Edit;
struct CPWL_Color;
class CFSPDF_WidgetAnnotHandler;

class CFSCRT_STPDFResetApperance
{
	friend class CFSPDF_WidgetAnnotHandler;//some function is used by CFSPDF_WidgetAnnotHandler to draw the drop button of combo box
public:
	CFSCRT_STPDFResetApperance(CPDF_FormControl* control);
	~CFSCRT_STPDFResetApperance();
		
		
	FS_RESULT		ST_ResetAppearance_RadioButton();
	FS_RESULT		ST_ResetAppearance_PushButton(IFX_Edit_FontMap* fontmap);
	FS_RESULT		ST_ResetAppearance_CheckBox();
	FS_RESULT		ST_ResetAppearance_ListBox(IFX_Edit_FontMap* fontmap);
	FS_RESULT		ST_ResetAppearance_ComboBox(IFX_Edit_FontMap* fontmap, FX_LPCWSTR sValue);
	FS_RESULT		ST_ResetAppearance_TextField(IFX_Edit_FontMap* fontmap, FX_LPCWSTR sValue);
	CFX_ByteString  GetAppState();

private:
	CPDF_Rect		GetRotatedRect();
	void			SetAppState(const CFX_ByteString& str);
	
	void			WriteAppearance(const CFX_ByteString& sAPType, const CPDF_Rect& rcBBox, 
								const CPDF_Matrix& matrix, const CFX_ByteString& sContents,
								const CFX_ByteString& sAPState);

	CPDF_Matrix		GetMatrix();
	CPDF_Rect		GetClientRect();
	CPWL_Color		GetTextPWLColor();
	CPDF_Matrix		GetTextPWLMatrix();
	CPWL_Color		GetFillPWLColor() const;
	CPWL_Color		GetBorderPWLColor() const;
	FS_RESULT		WidgetGetBorder(FSBorderInfo* border);
	CFX_ByteString	GetBorderAppStream() ;
	CFX_ByteString	GetBackgroundAppStream() ;


	void			RemoveAppearance(const CFX_ByteString& sAPType);
	void			AddImageToAppearance(const CFX_ByteString& sAPType, CPDF_Stream* pImage);

	FX_FLOAT		SetFontSize(IFX_Edit* pEdit, FX_BOOL isAutoFontSize = FALSE);

private:
	CPDF_FormControl* m_pControl;
};

#endif //_FSCRT_ST_PDF_RESETAPPREANCE_H_


