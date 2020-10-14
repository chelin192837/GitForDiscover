#ifndef _FS_PAUSE_
#define _FS_PAUSE_

#include "fxcore.h"
#include "../include/common.h"

class CFSCRT_Pause : public IFX_Pause, public CFX_Object
{
public:
	virtual FX_BOOL			NeedToPauseNow();

public:
	CFSCRT_Pause(FSPause* pPause) : m_pPause(pPause) {}

public:
	FSPause* m_pPause;
};

class CFSRDK_Pause : public FSPause, public CFX_Object
{
public:
	virtual FS_BOOL		needPauseNow();

public:
	CFSRDK_Pause(IFX_Pause* pPause) : m_pFxPause(pPause) {}

public:
	IFX_Pause* m_pFxPause;
};

#endif