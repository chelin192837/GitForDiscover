// #include "../fs_precompilers.h"
// #include "inc/fs_object.h"
// #include "inc/fs_sdkmgr.h"
// #include "inc/fs_stream.h"
#include "inc/fs_log.h"

#define FSCRT_Logger_BUFFER_SIZE (8*1024)

CFSCRT_Logger::CFSCRT_Logger()
: m_pBuffer(NULL)
, m_nLen (FSCRT_Logger_BUFFER_SIZE)
, m_nBufferPos (0)
, m_pLogFile (0)
{
}

CFSCRT_Logger::~CFSCRT_Logger()
{
	FSCRT_LOCKOBJECT(lock, &m_lock)

	if (m_pBuffer) {
		free(m_pBuffer);
	}
	m_pLogFile = NULL;
}

FS_RESULT CFSCRT_Logger::Initialize(CFSCRT_FileStream* pLogFile)
{
	FSCRT_LOCKOBJECT(lock, &m_lock)

	m_pBuffer = (FX_WCHAR*)FX_Alloc(FX_WCHAR, m_nLen);
	if (!m_pBuffer) return FSCRT_ERRCODE_OUTOFMEMORY;
	FXSYS_memset(m_pBuffer, 0, m_nLen);

	m_pLogFile = pLogFile;
	return FSCRT_ERRCODE_SUCCESS;
}


FS_RESULT CFSCRT_Logger::Write(FX_LPCWSTR format, ...)
{
	FSCRT_LOCKOBJECT(lock, &m_lock)

	if (!m_pLogFile) return FSCRT_ERRCODE_ERROR;

	va_list arglist;
	va_start( arglist, format);

	FS_RESULT ret = ST_Write(format, arglist);

	va_end( arglist);
	return ret;
}

FS_RESULT CFSCRT_Logger::ST_Write(FX_LPCWSTR format, va_list arglist)
{

	if (m_nBufferPos > (FSCRT_Logger_BUFFER_SIZE - 1024)) ST_WriteToFile();

#if (_FX_OS_ == _FX_WIN32_DESKTOP_ || _FX_OS_ == _FX_WIN32_MOBILE_ || _FX_OS_ == _FX_WIN64_)
	#if (WINAPI_PARTITION_APP || WINAPI_PARTITION_PC_APP)
		m_nBufferPos += vswprintf_s((LPWSTR)(m_pBuffer + m_nBufferPos), (m_nLen - m_nBufferPos), (LPWSTR)format, arglist);
	#else
		m_nBufferPos += FXSYS_vswprintf((LPWSTR)(m_pBuffer + m_nBufferPos), (LPWSTR)format, arglist);
	#endif
#else
	m_nBufferPos += FXSYS_vswprintf((FX_LPWSTR)(m_pBuffer + m_nBufferPos), m_nLen - m_nBufferPos, (FX_LPCWSTR)format, arglist);
#endif

	return FSCRT_ERRCODE_SUCCESS;
}

FS_RESULT CFSCRT_Logger::ST_WriteToFile()
{
	FX_FILESIZE pos = m_pLogFile->GetPosition();
	m_pLogFile->WriteBlock(m_pBuffer, pos, m_nBufferPos * sizeof(FX_WCHAR));
	FXSYS_memset(m_pBuffer, 0, m_nLen);
	m_nBufferPos = 0;
	return FSCRT_ERRCODE_SUCCESS;
}

FS_RESULT CFSCRT_Logger::Flush()
{
	FSCRT_LOCKOBJECT(lock, &m_lock)

	if (!m_pLogFile) return FSCRT_ERRCODE_ERROR;

	if (m_nBufferPos > 0) ST_WriteToFile();

	FS_BOOL bRet = m_pLogFile->Flush();
	return bRet ? FSCRT_ERRCODE_SUCCESS : FSCRT_ERRCODE_ERROR;
}

FS_RESULT CFSCRT_Logger::Close()
{
	FSCRT_LOCKOBJECT(lock, &m_lock)

	if (!m_pLogFile) return FSCRT_ERRCODE_ERROR;

	Flush();

	m_pLogFile->Release();
	m_pLogFile = NULL;
	return FSCRT_ERRCODE_SUCCESS;
}

void FS_OUTPUT_LOG_W(FX_LPCWSTR Msg, ...)
{
	va_list arglist;
	va_start( arglist, Msg);

	CFSCRT_Logger* pLogger = CFS_Library::getLogger();
	if (pLogger) pLogger->Write(Msg, arglist);

	va_end( arglist);
}

CFSCRT_LogObject::CFSCRT_LogObject(FX_LPCWSTR Msg, ...)
	: m_context(Msg)
{
	va_list arglist;
	va_start( arglist, Msg);

	CFSCRT_Logger* pLogger = CFS_Library::getLogger();
	if (pLogger){
		pLogger->Write(FX_LPCWSTR(L"Enter Function: "));
		pLogger->Write(Msg, arglist);
		pLogger->Write(FX_LPCWSTR(L"\r\n"));
	}

	va_end( arglist);

}
// CFSCRT_LogObject::CFSCRT_LogObject(FX_LPCWSTR context)
// 	: m_context(context)
// {
// 	FXSYS_assert(context != NULL);
// 	CFSCRT_Logger* pLogger = FSCRT_GetLogger();
// 	if (pLogger) pLogger->Write(FX_LPCWSTR(L"Enter Fun %s\r\n"), m_context);
// }

CFSCRT_LogObject::~CFSCRT_LogObject()
{
	CFSCRT_Logger* pLogger = CFS_Library::getLogger();
	if (pLogger) pLogger->Write(FX_LPCWSTR(L"Exit Function: %s\r\n"), m_context);
}
