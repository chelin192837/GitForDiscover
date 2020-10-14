#ifndef _FPDFDEFINE_H_
#define _FPDFDEFINE_H_

#include "../../../fxcore.h"

// We use an array to store all open pages (when we clear caches, we'll look in those pages)
#define MODULE_QUICKDRAW	(FX_LPVOID)2
#define MODULE_RENDER		(FX_LPVOID)3
#define MODULE_TEXTINFO		(FX_LPVOID)4
#define MODULE_SEARCH		(FX_LPVOID)5
#define MODULE_SIGNATURE	(FX_LPVOID)7
/*
#include "fsdkmgr.h"
class CSDK_Mgr;
*/
extern "C"
{
	extern void FX_OUTPUT_LOG_FUNC(const char*, ...);

	extern int FX_GET_TICK_FUNC();
};

// Comment out these definitions since they have been defined in fxcore level.
// If these definitions exist, redefinition warning will occur for Android.
/*#ifdef _FOXIT_DEBUG_
#define FOXIT_DEBUG1(msg) FX_OUTPUT_LOG_FUNC(msg)
#define FOXIT_DEBUG2(msg,para) FX_OUTPUT_LOG_FUNC(msg,para)
#define FOXIT_DEBUG3(msg,para1,para2) FX_OUTPUT_LOG_FUNC(msg,para1,para2)
#define FOXIT_DEBUG4(msg,para1,para2,para3) FX_OUTPUT_LOG_FUNC(msg,para1,para2,para3)
#define FOXIT_DEBUG5(msg,para1,para2,para3,param4) FX_OUTPUT_LOG_FUNC(msg,para1,para2,para3,param4)
#else
#define FOXIT_DEBUG1(msg) 
#define FOXIT_DEBUG2(msg,para)
#define FOXIT_DEBUG3(msg,para1,para2) 
#define FOXIT_DEBUG4(msg,para1,para2,para3)
#define FOXIT_DEBUG5(msg,para1,para2,para3,param4) 

#endif*/

#ifdef _FOXIT_BENCHMARK_

//extern void FX_OUTPUT_LOG_FUNC(const char*, ...);

static int g_Ticks[128], g_CallCount[128];

class _TickLogger
{
public:
	int m_Tick;
	FX_LPCSTR m_ID;
	_TickLogger(FX_LPCSTR id1) 
	{ 
		m_ID = id1; 
		FX_OUTPUT_LOG_FUNC("FPDFEMB TRACE: %s called", m_ID);
		m_Tick = FX_GET_TICK_FUNC(); 
	}
	~_TickLogger() 
	{
		int tick = FX_GET_TICK_FUNC() - m_Tick;
		FX_OUTPUT_LOG_FUNC("FPDFEMB BENCHMARK: %s, %d ticks", m_ID, tick);
	}
};

#define FOXIT_BENCHMARK(id1) _TickLogger tick(id1)
#else
#define FOXIT_BENCHMARK(id1)
#endif

/*
#ifndef _CDictArray_
#define _CDictArray_
typedef CFX_ArrayTemplate<CPDF_Dictionary*> CDictArray;
#endif

#define FSPDF_GETSIGN(a)			((a) > 0 ? 1 : (a) < 0 ? -1 : 0)
#define FSPDF_FLOAT_MIN			0.001f

#define FSPDF_FDIVVAL(a)			( \
	(FXSYS_fabs(a) < FSPDF_FLOAT_MIN) \
	? (FSPDF_GETSIGN(a) >= 0 ? FSPDF_FLOAT_MIN : (-1)*FSPDF_FLOAT_MIN) \
	: (a))
class CSDK_Vector
{
public:
	CSDK_Vector(FX_FLOAT xx = 0.0f, FX_FLOAT yy = 0.0f) 
	{ x = xx; y = yy; }
	
	FX_FLOAT		x;
	FX_FLOAT		y;
	
	void			SetValue(FX_FLOAT xx, FX_FLOAT yy)
	{ 
		x = xx; 
		y = yy; 
	}
	
	FX_FLOAT		Length() const { return x * x + y * y; }
	
	FX_FLOAT		DotProduct(const CSDK_Vector& vector) const
	{ return x * vector.x + y * vector.y; }
	
	friend FX_FLOAT	_Vector_Cosine(const CSDK_Vector& a, const CSDK_Vector& b)
	{
		FX_FLOAT dotProduct = a.DotProduct(b);
		return dotProduct / FSPDF_FDIVVAL(a.Length() * b.Length());
	}
	
	friend FX_FLOAT	_Vector_ArcCosine(const CSDK_Vector& a, const CSDK_Vector& b) 
	{ return FXSYS_acos(_Vector_Cosine(a, b)); }
	
	friend FX_FLOAT	_Vector_SlopeAngle(const CSDK_Vector& a) 
	{ return _Vector_ArcCosine(a, CSDK_Vector(1.0f, 0.0f)); }	
	
	friend CSDK_Vector operator + (const CSDK_Vector& p1, const CSDK_Vector& p2)
	{
		CSDK_Vector p;
		p.x = p1.x + p2.x;
		p.y = p1.y + p2.y;
		return p;
	}
	
	friend CSDK_Vector operator - (const CSDK_Vector& p1, const CSDK_Vector& p2)
	{
		CSDK_Vector p;
		p.x = p1.x - p2.x;
		p.y = p1.y - p2.y;
		return p;
	}
	
	friend CSDK_Vector operator * (const CSDK_Vector& point, float t)
	{
		CSDK_Vector p;
		p.x = point.x * t;
		p.y = point.y * t;
		return p;
	}
	
	friend CSDK_Vector operator / (const CSDK_Vector& p1, const CSDK_Vector& p2)
	{
		CSDK_Vector p;
		p.x = p1.x / FSPDF_FDIVVAL(p2.x);
		p.y = p1.y / FSPDF_FDIVVAL(p2.y);
		return p;
	}
	
	friend CSDK_Vector operator / (const CSDK_Vector& point, float t)
	{
		CSDK_Vector p;
		p.x = point.x / FSPDF_FDIVVAL(t);
		p.y = point.y / FSPDF_FDIVVAL(t);
		return p;
	}
};

int FSCRT_CheckModuleLicense(FS_LPCSTR module_name);

#define FSCRT_CHECKMODULELICENSE(ModuleName) {if (!FSCRT_CheckModuleLicense(ModuleName)) return FSCRT_ERR_INVALID_LICENSE;}
*/


// default is VC10
#ifdef FSPDF_DLL_EXPORTS
	#ifdef	_DEBUG
		#ifdef WIN32
		#pragma comment(lib, "fpdfapi[dbg_x86_vc10].lib")
		#pragma comment(lib, "fpdfdoc[dbg_x86_vc10].lib")
		#pragma comment(lib, "fxcodec[dbg_x86_vc10].lib")
		#pragma comment(lib, "fxcrt[dbg_x86_vc10].lib")
		#pragma comment(lib, "fxhal[dbg_x86_vc10].lib")
		#pragma comment(lib, "fxsslasm[dbg_x86_vc10].lib")
		#pragma comment(lib, "fxge[dbg_x86_vc10].lib")
		#pragma comment(lib, "fpdftext[dbg_x86_vc10].lib")
		#pragma comment(lib, "fdrm[dbg_x86_vc10].lib")
		#pragma comment(lib, "reflow[dbg_x86_vc10].lib")
		#pragma comment(lib, "fxpsi[dbg_x86_vc10].lib")
		#pragma comment(lib, "fxmath[dbg_x86_vc10].lib")
		//#pragma comment(lib,"../../../lib/dbg/x86_vc10/fgsdkcrt[dbg_x86_vc10].lib")
		#endif
	#else
#ifdef WIN32
		#pragma comment(lib, "fxhal[rel_x86_vc10].lib")
		#pragma comment(lib, "fxsslasm[rel_x86_vc10].lib")
		#pragma comment(lib, "fpdfapi[rel_x86_vc10].lib")
		#pragma comment(lib, "fpdfdoc[rel_x86_vc10].lib")
		#pragma comment(lib, "fxcodec[rel_x86_vc10].lib")
		#pragma comment(lib, "fxcrt[rel_x86_vc10].lib")
		#pragma comment(lib, "fxge[rel_x86_vc10].lib")
		#pragma comment(lib, "fpdftext[rel_x86_vc10].lib")
		#pragma comment(lib, "fdrm[rel_x86_vc10].lib")
		#pragma comment(lib, "reflow[rel_x86_vc10].lib")
		#pragma comment(lib, "fxpsi[rel_x86_vc10].lib")
		#pragma comment(lib, "fxmath[rel_x86_vc10].lib")
		//#pragma comment(lib,"../../../lib/dbg/x86_vc10/fgsdkcrt[dbg_x86_vc10].lib")
#endif
	#endif
#endif

#endif

