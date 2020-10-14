#ifndef _JS_RESMGR_H_
#define _JS_RESMGR_H_

class CJS_ResProv : public CFX_ResourceProvider
{
public:
	CJS_ResProv(){};
	virtual ~CJS_ResProv(){};

	virtual FX_DWORD *		   GetResInfo();
	virtual FX_DWORD           CountResInfos();
	virtual BYTE *             GetResData();
	virtual FX_DWORD           CountResDatas();
};

static FX_HGLOBAL JS_LoadDialog(CDialog & dlg, UINT ID, CWnd* pParent)
{
	FX_HGLOBAL handle = NULL;

	CJS_ResProv resProvider;
	CFX_ResourceMgr * pResMgr = CFX_ResourceMgr::CreateResourceMgr(&resProvider);
	if (pResMgr) handle = pResMgr->LoadDialog(dlg, ID, pParent);
	pResMgr->Release();

	return handle;
};

static FX_BOOL JS_CreateDialog(CDialog & dlg, UINT ID, CWnd* pParent)
{
	FX_BOOL bRet = FALSE;

	CJS_ResProv resProvider;
	CFX_ResourceMgr * pResMgr = CFX_ResourceMgr::CreateResourceMgr(&resProvider);
	if (pResMgr) bRet = pResMgr->CreateDialogEx(dlg, ID, pParent);
	pResMgr->Release();

	return bRet;
};

static CFX_WideString JS_LoadString(CReader_App* pApp, UINT ID)
{
	CFX_WideString csStr;
	CJS_ResProv resProvider;
	CFX_ResourceMgr * pResMgr = CFX_ResourceMgr::CreateResourceMgr(&resProvider);
	if(pResMgr && pApp)csStr = pApp->LoadString(pResMgr,ID);
	pResMgr->Release();

	return csStr;
};

#endif //_JS_RESMGR_H_
