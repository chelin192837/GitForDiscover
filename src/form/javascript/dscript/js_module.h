#ifndef _JS_MODULE_H_
#define _JS_MODULE_H_

class CFXJS_GlobalData;
class IJS_Runtime;

class CFJS_LTModule
{
public:
	CFJS_LTModule();
	~CFJS_LTModule();

	virtual void					Release() {delete this;}

	//get current thead runtime
	FS_RESULT						GetJSRuntime(IJS_Runtime*& pRuntime);
	FS_RESULT						DeleteJSRuntime(IJS_Runtime* pRuntime);

	CFXJS_GlobalData*					NewGlobalData();
	void							ReleaseGlobalData();

protected:
	virtual FS_RESULT	_Recover() ;
	virtual void		_Clear() ;

	FS_RESULT	ST_GetJSRuntime(IJS_Runtime*& pRuntime);

	FX_BOOL							m_bInitial;
	CFXJS_GlobalData*					m_pGlobalData;
	FX_INT32						m_nGlobalDataCount;
	CFX_MapPtrToPtr					m_threadID2Runtime;
};

FS_RESULT FJS_GetEnviroment(CFJS_LTModule*& pJSEnv);

#endif //_JS_MODULE_H_

