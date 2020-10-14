#ifndef _FS_OFDRENDERER_H_
#define _FS_OFDRENDERER_H_

class CFS_OFDRenderEngine;
class CFS_OFDRenderContext;
class CFS_OFDRenderProgress;
class CFX_WindowsDIB;

class CFS_OFDRenderEngine: public CFX_Object
{
public:
	CFS_OFDRenderEngine();
	virtual ~CFS_OFDRenderEngine();

	FX_BOOL InitializeDIB(CFX_DIBitmap* dib);
	FX_BOOL	InitializeHDC(void* dc, int width, int height);
	FX_BOOL	InitializeCairo(void* cr);
	FX_BOOL	InitializeCGContext(void* content, int nDeviceClass);

#if _FX_OS_ == _FX_WIN32_DESKTOP_ || _FX_OS_ == _FX_WIN64_
	HDC GetDC() {return m_hdc;}
#endif
	CFX_WindowsDIB* GetWinDIB() {return m_winDIB;}

protected:
	CFX_WindowsDIB* m_winDIB;

	int m_deviceFlag; // 1 == CFX_FxgeDevice, 2 == CFX_LinuxDevice, 3 == CFX_QuartzDevice

	CFX_RenderDevice* m_fxDev;
	COFD_RenderDevice* m_ofdRenderDev;

#if _FX_OS_ == _FX_WIN32_DESKTOP_ || _FX_OS_ == _FX_WIN64_
	HDC m_hdc;
#endif

	friend class CFS_OFDRenderContext;
};

class CFS_OFDRenderContext: public CFX_Object
{
public:
	CFS_OFDRenderContext();
	virtual ~CFS_OFDRenderContext();

	void SetMatrix(CFX_Matrix mt);
	void SetOption(FX_BOOL bIsPrinting, int colorMode, FX_ARGB backColor, FX_ARGB foreColor, int flags);
	CFS_OFDRenderProgress* StartPage(CFS_OFDPage* page, CFS_OFDRenderEngine* renderer);

	IOFD_Page* GetRenderPage();

protected:
	CFS_OFDRenderEngine* m_renderEngine;
	IOFD_RenderContext* m_pRenderContext;
	COFD_RenderOptions m_renderOptions;
	FX_BOOL m_bSetRenderOptions;
	CFX_Matrix m_matrix;
	CFS_OFDRenderProgress* m_progress;
	CFS_OFDPage* m_ofdPage;

	friend class CFS_OFDRenderProgress;
};

class CFS_OFDRenderProgress: public CFX_Object
{
public:
	CFS_OFDRenderProgress(CFS_OFDRenderContext* pOFDRenderContext);
	virtual ~CFS_OFDRenderProgress();

	FX_BOOL Initialize();

	FX_INT32 Continue();
	FX_INT32 Continue_HighLightAnnots(int blendMode);
	FX_INT32 Continue_CommonAnnots(int blendMode);
	FX_INT32 Continue_Contents();
	FX_INT32 Continue_StampAnnots(FX_BOOL bSealAllGray);

protected:
	CFS_OFDRenderContext* m_pOFDRenderContext;
	IOFD_ProgressiveRenderer* m_pProgressiveRenderer;

	friend class CFS_OFDRenderContext;
};

#endif //_FS_OFDRENDERER_H_
