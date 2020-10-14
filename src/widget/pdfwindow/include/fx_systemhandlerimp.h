#ifndef _FX_SYSTEMHANDLER_IMP_H_
#define _FX_SYSTEMHANDLER_IMP_H_

#include "pdfwindow/pdfwindow.h"

class FSPDFDocImp;

class CFFL_Widget;
class CFSPDF_WidgetAnnotHandler;
class FX_SystemHandlerImp : public IFX_SystemHandler , public CFX_Object
{
public:
	FX_SystemHandlerImp(CFSPDF_WidgetAnnotHandler* pHandler);
	~FX_SystemHandlerImp();
	
	virtual void				Release();

	virtual void				InvalidateRect(FX_HWND hWnd, FX_RECT rect);

	virtual void				OutputSelectedRect(void* pFormFiller, CPDF_Rect&rect);

	virtual FX_BOOL				IsSelectionImplemented();

	virtual CFX_WideString		GetClipboardText(FX_HWND hWnd);
	virtual FX_BOOL				SetClipboardText(FX_HWND hWnd, CFX_WideString string){return TRUE;}
	
	virtual void				ClientToScreen(FX_HWND hWnd, FX_INT32& x, FX_INT32& y);
	virtual void				ScreenToClient(FX_HWND hWnd, FX_INT32& x, FX_INT32& y);
	virtual void				SetCursor(FX_INT32 nCursorType);

	virtual FX_HMENU			CreatePopupMenu() {return NULL;}
	virtual FX_BOOL				AppendMenuItem(FX_HMENU hMenu, FX_INT32 nIDNewItem, CFX_WideString string) {return TRUE;}
	virtual FX_BOOL				EnableMenuItem(FX_HMENU hMenu, FX_INT32 nIDItem, FX_BOOL bEnabled){return TRUE;}
	virtual FX_INT32			FXTrackPopupMenu(FX_HMENU hMenu, FX_INT32 x, FX_INT32 y, FX_HWND hParent){return 0;}
	virtual void				DestroyMenu(FX_HMENU hMenu) {}

	virtual CFX_ByteString		GetNativeTrueTypeFont(FX_INT32 nCharset);
	virtual FX_BOOL				FindNativeTrueTypeFont(FX_INT32 nCharset, CFX_ByteString sFontFaceName);
	virtual CPDF_Font*			AddNativeTrueTypeFontToPDF(CPDF_Document* pDoc, CFX_ByteString sFontFaceName, FX_BYTE nCharset); 
	

	virtual FX_INT32			SetTimer(FX_INT32 uElapse, TimerCallback lpTimerFunc);
	virtual void				KillTimer(FX_INT32 nID);


	virtual FX_BOOL				IsSHIFTKeyDown(FX_DWORD nFlag){return FALSE;}
	virtual FX_BOOL				IsCTRLKeyDown(FX_DWORD nFlag) {return FALSE;}
	virtual FX_BOOL				IsALTKeyDown(FX_DWORD nFlag){return FALSE;}
	virtual FX_BOOL				IsINSERTKeyDown(FX_DWORD nFlag) {return FALSE;}

	virtual	FX_SYSTEMTIME		GetLocalTime();


protected:
	//CFSCRT_LTPDFDocument*			GetDocument();
	FSPDFDocImp*				GetDocument();

private:
	CFSPDF_WidgetAnnotHandler*	m_pHandler;
};

#endif 
