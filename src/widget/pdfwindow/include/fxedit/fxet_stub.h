#ifndef _FXET_STUB_H_
#define _FXET_STUB_H_

#include "../fsdkdefine.h"
#include "../fx_systemhandler.h"
#ifdef FX_READER_DLL
	#ifdef _DEBUG
		#pragma comment(lib, "X:/pdf/fxcore/Lib/dbg_w32_vc6/fxcoredll[dbg,w32,vc6].lib")
		#pragma comment(lib, "X:/pdf/fxcore/Lib/dbg_w32_vc6/fpdfdocdll[dbg,w32,vc6].lib")
	#else
		#pragma comment(lib, "X:/pdf/fxcore/Lib/rel_w32_vc6/fxcoredll[rel,w32,vc6].lib")
		#pragma comment(lib, "X:/pdf/fxcore/Lib/rel_w32_vc6/fpdfdocdll[rel,w32,vc6].lib")
	#endif
#endif

#endif 

