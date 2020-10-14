#ifndef _FSCRT_LT_LOG_H_
#define _FSCRT_LT_LOG_H_

//#include "fs_object.h"
#include "../fxcore.h"
#include "../fs_file.h"
class CFSCRT_FileStream;


class CFSCRT_Logger;
class CFSCRT_LogObject;

void FS_OUTPUT_LOG_W(FX_LPCWSTR Msg, ...);

#define LOG_BUFFER_MAX_LENGTH			4096
//#define _FSRDK_LOG_
#ifdef _FSRDK_LOG_
//#define FSRDK_LOG_FUNC(parm)					CFSCRT_LogObject _logMsg(##parm);
#define FSRDK_LOG_FUNC							FS_OUTPUT_LOG_W
#define WSTR_FORMAT(str)						(FX_LPCWSTR)L##str
#else
//#define FSRDK_LOG_FUNC(func_name)
#define FSRDK_LOG_FUNC
#define WSTR_FORMAT(str)						(FX_LPCWSTR)L##str
#endif


class CFSCRT_Logger
{
public:
	CFSCRT_Logger();
	~CFSCRT_Logger();

	FS_RESULT				Initialize(CFSCRT_FileStream* pLogFile);

	//When write message great than LOG_BUFFER_MAX_LENGTH, then write to log file.
//	FS_RESULT				Write(FSCRT_BSTR* msg);
	FS_RESULT				Write(FX_LPCWSTR format, ...);
	FS_RESULT				Flush();
	FS_RESULT				Close();

//	IFSCRT_LTExceptionImp*	GetLTExceptionImp() { return m_pException; }
protected:
	FS_RESULT				ST_Write(FX_LPCWSTR format, va_list arglist);
	FS_RESULT				ST_WriteToFile();

	FX_LPWSTR				m_pBuffer;
	FX_INT32				m_nLen;
	FX_INT32				m_nBufferPos;
//	IFSCRT_LTExceptionImp*	m_pException;
	CFSCRT_FileStream*	m_pLogFile;
public:
	FSCRT_DEFINELOCK(m_lock)
};

class CFSCRT_LogObject
{
public:
	//	CFSCRT_LogObject(FX_LPCWSTR context);
	CFSCRT_LogObject(FX_LPCWSTR Msg, ...);
	~CFSCRT_LogObject();

protected:
	FX_LPCWSTR m_context;
};

// class IFSCRT_LTExceptionImp : public CFSCRT_LTObject
// {
// public:
// 	virtual void		Release() = 0;
// 	virtual FS_RESULT	Initialize(CFSCRT_Logger* pLogger) = 0;
// 	
// protected:
// 	CFSCRT_Logger*	m_pLogger;
// };
// 
// FS_RESULT FSCRT_CreateLTExceptionImp(IFSCRT_LTExceptionImp*& pException);

#endif //_FSCRT_LT_LOG_H_