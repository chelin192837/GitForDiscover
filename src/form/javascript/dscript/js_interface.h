#ifndef _IJAVASCRIPT_H_
#define _IJAVASCRIPT_H_

class IJS_Context;
class IJS_Runtime;
class FSAnnot;
class CFSCRT_LTAppHandler;

#include "js_module.h"

class IJS_Context  
{
public:
	virtual FX_BOOL				Compile(const CFX_WideString& script, CFX_WideString& info) = 0;
	virtual FX_BOOL				RunScript(const CFX_WideString& script, CFX_WideString& info) = 0;

	virtual void				OnApp_Init() = 0;

	virtual void				OnDoc_Open(FSPDFDoc* pDoc, const CFX_WideString& strTargetName) = 0;
	virtual void				OnDoc_WillPrint(FSPDFDoc* pDoc) = 0;
	virtual void				OnDoc_DidPrint(FSPDFDoc* pDoc) = 0;
	virtual void				OnDoc_WillSave(FSPDFDoc* pDoc) = 0;
	virtual void				OnDoc_DidSave(FSPDFDoc* pDoc) = 0;
	virtual void				OnDoc_WillClose(FSPDFDoc* pDoc) = 0;

	virtual void				OnPage_Open(FSPDFDoc* pTarget) = 0;
	virtual void				OnPage_Close(FSPDFDoc* pTarget) = 0;
	virtual void				OnPage_InView(FSPDFDoc* pTarget) = 0;
	virtual void				OnPage_OutView(FSPDFDoc* pTarget) = 0;
	
	virtual void				OnField_MouseDown(FX_BOOL bModifier, FX_BOOL bShift, CPDF_FormField* pTarget) = 0;
	virtual void				OnField_MouseEnter(FX_BOOL bModifier, FX_BOOL bShift, CPDF_FormField* pTarget) = 0;
	virtual void				OnField_MouseExit(FX_BOOL bModifier, FX_BOOL bShift, CPDF_FormField* pTarget) = 0;
	virtual void				OnField_MouseUp(FX_BOOL bModifier, FX_BOOL bShift, CPDF_FormField* pTarget) = 0;
	virtual void				OnField_Focus(FX_BOOL bModifier, FX_BOOL bShift, CPDF_FormField* pTarget, const CFX_WideString& Value) = 0;
	virtual void				OnField_Blur(FX_BOOL bModifier, FX_BOOL bShift, CPDF_FormField* pTarget, const CFX_WideString& Value) = 0;

	virtual void				OnField_Calculate(CPDF_FormField* pSource, CPDF_FormField* pTarget, CFX_WideString& Value, FX_BOOL& bRc) = 0;
	virtual void				OnField_Format(int nCommitKey, CPDF_FormField* pTarget, CFX_WideString& Value, FX_BOOL bWillCommit) = 0;
	virtual void				OnField_Keystroke(int nCommitKey, CFX_WideString& strChange, const CFX_WideString& strChangeEx,
									FX_BOOL KeyDown, FX_BOOL bModifier, int &nSelEnd,int &nSelStart, FX_BOOL bShift,
									CPDF_FormField* pTarget, CFX_WideString& Value, FX_BOOL bWillCommit, 
									FX_BOOL bFieldFull, FX_BOOL &bRc) = 0;
	virtual void				OnField_Validate(CFX_WideString& strChange, const CFX_WideString& strChangeEx, FX_BOOL bKeyDown,
									FX_BOOL bModifier, FX_BOOL bShift, CPDF_FormField* pTarget, CFX_WideString& Value, FX_BOOL& bRc) = 0;

	virtual void				OnScreen_Focus(FX_BOOL bModifier, FX_BOOL bShift, FSAnnot* pScreen) = 0;
	virtual void				OnScreen_Blur(FX_BOOL bModifier, FX_BOOL bShift, FSAnnot* pScreen) = 0;
	virtual void				OnScreen_Open(FX_BOOL bModifier, FX_BOOL bShift, FSAnnot* pScreen) = 0;
	virtual void				OnScreen_Close(FX_BOOL bModifier, FX_BOOL bShift, FSAnnot* pScreen) = 0;
	virtual void				OnScreen_MouseDown(FX_BOOL bModifier, FX_BOOL bShift, FSAnnot* pScreen) = 0;
	virtual void				OnScreen_MouseUp(FX_BOOL bModifier, FX_BOOL bShift, FSAnnot* pScreen) = 0;
	virtual void				OnScreen_MouseEnter(FX_BOOL bModifier, FX_BOOL bShift, FSAnnot* pScreen) = 0;
	virtual void				OnScreen_MouseExit(FX_BOOL bModifier, FX_BOOL bShift, FSAnnot* pScreen) = 0;
	virtual void				OnScreen_InView(FX_BOOL bModifier, FX_BOOL bShift, FSAnnot* pScreen) = 0;
	virtual void				OnScreen_OutView(FX_BOOL bModifier, FX_BOOL bShift, FSAnnot* pScreen) = 0;

	virtual void				OnBookmark_MouseUp(CPDF_Bookmark* pBookMark) = 0;
	virtual void				OnLink_MouseUp(FSPDFDoc* pTarget) = 0;

	virtual void				OnMenu_Exec(FSPDFDoc* pTarget, const CFX_WideString& strTargetName) = 0;
	virtual void				OnBatchExec(FSPDFDoc* pTarget) = 0;
	virtual void				OnConsole_Exec() = 0;
	virtual void				OnExternal_Exec() = 0;

	virtual void				EnableMessageBox(FX_BOOL bEnable) = 0;

	virtual IJS_Runtime*		GetRuntime() = 0;

	virtual void				SetAttachedData(void* pData) = 0;
	virtual void*				GetAttachedData() const = 0;
};

class IJS_Runtime
{
public:
	virtual IJS_Context*		NewContext() = 0;
	virtual void				ReleaseContext(IJS_Context * pContext) = 0;

	virtual void				SetReaderDocument(FSPDFDoc* pReaderDoc) = 0;
	virtual	FSPDFDoc*	GetReaderDocument() = 0;	

	virtual void				GetObjectNames(CFX_WideStringArray& array) = 0;
	virtual void				GetObjectConsts(const CFX_WideString& swObjName, CFX_WideStringArray& array) = 0;
	virtual void				GetObjectProps(const CFX_WideString& swObjName, CFX_WideStringArray& array) = 0;
	virtual void				GetObjectMethods(const CFX_WideString& swObjName, CFX_WideStringArray& array) = 0;

	virtual FX_BOOL				IsBlocking() = 0;
};

class CFXJS_GlobalData;

class CFXJS_RuntimeFactory : public CFX_Object
{
public:
	CFXJS_RuntimeFactory();
protected:
	~CFXJS_RuntimeFactory();
public:
	void							Release();
	
	IJS_Runtime*					GetJSRuntime(CFSCRT_LTAppHandler* pApp, FX_BOOL bNew = TRUE);
	void							DeleteJSRuntime();

	CFXJS_GlobalData*					NewGlobalData(CFSCRT_LTAppHandler* pApp);
	void							ReleaseGlobalData();

	IJS_Runtime*					NewJSRuntime(CFSCRT_LTAppHandler* pApp);

	FX_BOOL							IsJSInitiated();
private:

	int								m_nRef;
	CFXJS_GlobalData*				m_pGlobalData;
	FX_INT32						m_nGlobalDataCount;

	CFX_MapPtrToPtr					m_threadID2Runtime;

	FSCRT_DEFINELOCK(m_lock)
};

#endif //_IJAVASCRIPT_H_

