#include "../fxlib.h"
#include "../include/fs_ofdsdkmgr.h"
#include "../include/fs_ofdsignature.h"
#include "../include/fs_ofdsignaturefun.h"
#include "../include/fs_ofdsignaturemanage.h"
#include "../include/fs_ofdsignaturefactory.h"
#include "../include/fs_ofdsdkmgr.h"
#include "fs_ofdsignaturegmsslfunc.h"

#if (_FX_OS_ == _FX_WIN32_DESKTOP_ || _FX_OS_ == _FX_WIN64_)
#define APPLINK_STDIN   1
#define APPLINK_STDOUT  2
#define APPLINK_STDERR  3
#define APPLINK_FPRINTF 4
#define APPLINK_FGETS   5
#define APPLINK_FREAD   6
#define APPLINK_FWRITE  7
#define APPLINK_FSETMOD 8
#define APPLINK_FEOF    9
#define APPLINK_FCLOSE  10      /* should not be used */

#define APPLINK_FOPEN   11      /* solely for completeness */
#define APPLINK_FSEEK   12
#define APPLINK_FTELL   13
#define APPLINK_FFLUSH  14
#define APPLINK_FERROR  15
#define APPLINK_CLEARERR 16
#define APPLINK_FILENO  17      /* to be used with below */

#define APPLINK_OPEN    18      /* formally can't be used, as flags can vary */
#define APPLINK_READ    19
#define APPLINK_WRITE   20
#define APPLINK_LSEEK   21
#define APPLINK_CLOSE   22
#define APPLINK_MAX     22      /* always same as last macro */

#ifndef APPMACROS_ONLY
# include <stdio.h>
# include <io.h>
# include <fcntl.h>

static void *app_stdin(void)
{
    return stdin;
}

static void *app_stdout(void)
{
    return stdout;
}

static void *app_stderr(void)
{
    return stderr;
}

static int app_feof(FILE *fp)
{
    return feof(fp);
}

static int app_ferror(FILE *fp)
{
    return ferror(fp);
}

static void app_clearerr(FILE *fp)
{
    clearerr(fp);
}

static int app_fileno(FILE *fp)
{
    return _fileno(fp);
}

static int app_fsetmod(FILE *fp, char mod)
{
    return _setmode(_fileno(fp), mod == 'b' ? _O_BINARY : _O_TEXT);
}

#ifdef __cplusplus
extern "C" {
#endif

__declspec(dllexport)
void **
# if defined(__BORLANDC__)
/*
 * __stdcall appears to be the only way to get the name
 * decoration right with Borland C. Otherwise it works
 * purely incidentally, as we pass no parameters.
 */
 __stdcall
# else
 __cdecl
# endif
OPENSSL_Applink(void)
{
    static int once = 1;
    static void *OPENSSL_ApplinkTable[APPLINK_MAX + 1] =
        { (void *)APPLINK_MAX };

    if (once) {
        OPENSSL_ApplinkTable[APPLINK_STDIN] = app_stdin;
        OPENSSL_ApplinkTable[APPLINK_STDOUT] = app_stdout;
        OPENSSL_ApplinkTable[APPLINK_STDERR] = app_stderr;
        OPENSSL_ApplinkTable[APPLINK_FPRINTF] = fprintf;
        OPENSSL_ApplinkTable[APPLINK_FGETS] = fgets;
        OPENSSL_ApplinkTable[APPLINK_FREAD] = fread;
        OPENSSL_ApplinkTable[APPLINK_FWRITE] = fwrite;
        OPENSSL_ApplinkTable[APPLINK_FSETMOD] = app_fsetmod;
        OPENSSL_ApplinkTable[APPLINK_FEOF] = app_feof;
        OPENSSL_ApplinkTable[APPLINK_FCLOSE] = fclose;

        OPENSSL_ApplinkTable[APPLINK_FOPEN] = fopen;
        OPENSSL_ApplinkTable[APPLINK_FSEEK] = fseek;
        OPENSSL_ApplinkTable[APPLINK_FTELL] = ftell;
        OPENSSL_ApplinkTable[APPLINK_FFLUSH] = fflush;
        OPENSSL_ApplinkTable[APPLINK_FERROR] = app_ferror;
        OPENSSL_ApplinkTable[APPLINK_CLEARERR] = app_clearerr;
        OPENSSL_ApplinkTable[APPLINK_FILENO] = app_fileno;

        OPENSSL_ApplinkTable[APPLINK_OPEN] = _open;
        OPENSSL_ApplinkTable[APPLINK_READ] = _read;
        OPENSSL_ApplinkTable[APPLINK_WRITE] = _write;
        OPENSSL_ApplinkTable[APPLINK_LSEEK] = _lseek;
        OPENSSL_ApplinkTable[APPLINK_CLOSE] = _close;

        once = 0;
    }
    return OPENSSL_ApplinkTable;
}

#ifdef __cplusplus
}
#endif
#endif//#ifndef APPMACROS_ONLY
#endif//#if (_FX_OS_ == _FX_WIN32_DESKTOP_ || _FX_OS_ == _FX_WIN64_)

#if (_FX_OS_ != _FX_WIN32_DESKTOP_ && _FX_OS_ != _FX_WIN64_)
#include <stdio.h>
#include <dlfcn.h>
#endif

GmsslFunctions::GmsslFunctions() :m_gmsslHandle(NULL)
{

}

GmsslFunctions::~GmsslFunctions()
{
	if (m_gmsslHandle)
	{
#if (_FX_OS_ == _FX_WIN32_DESKTOP_ || _FX_OS_ == _FX_WIN64_)
		::FreeLibrary(m_gmsslHandle);
#else
		dlclose(m_gmsslHandle);
#endif
		m_gmsslHandle = NULL;
	}
}

FX_BOOL GmsslFunctions::LoadAllFunctions()
{
	//WriteLog_GSDK("LoadAllFunctions ", "m_gmsslHandle : ", (long)m_gmsslHandle);
	if(m_gmsslHandle)
		return TRUE;		// have loaded already.

	CFS_OFDSDKMgr* pMgr = CFS_OFDSDKMgr::Get();
	if(pMgr == NULL)
		return FALSE;

	CFX_WideString wsWorkPath = pMgr->GetWorkPath();
	//CFX_WideString wsWorkPath = L"E:/OFD_GSDK/trunk/fxlib/GmSSL/lib/win_x86/";
	//WriteLog_GSDK("LoadAllFunctions wsWorkPath : ", CFX_ByteString::FromUnicode(wsWorkPath));
	
#if (_FX_OS_ == _FX_WIN32_DESKTOP_ || _FX_OS_ == _FX_WIN64_)
	CFX_WideString wsLibFile = wsWorkPath + L"libeay32_foxit.dll";
	//WriteLog_GSDK("LoadAllFunctions wsLibFile : ", CFX_ByteString::FromUnicode(wsLibFile));
	//m_gmsslHandle = ::LoadLibraryA((FX_LPCSTR)bsSignatureLibPath);
	m_gmsslHandle = ::LoadLibraryW((LPCWSTR)(FX_LPCWSTR(wsLibFile)));
	//WriteLog_GSDK("LoadAllFunctions ", "m_gmsslHandle 2 : ", (long)m_gmsslHandle);
#elif (_FX_OS_ == _FX_MACOSX_)
	m_gmsslHandle = dlopen((FX_LPCSTR)bsSignatureLibPath, RTLD_NOW);
#elif (_FX_OS_ == _FX_LINUX_DESKTOP_)
	CFX_WideString wsLibFile = wsWorkPath + L"libcrypto_foxit.so";
	CFX_ByteString bsSignatureLibPath = CFX_ByteString::FromUnicode(wsLibFile);
	m_gmsslHandle = dlopen((FX_LPCSTR)bsSignatureLibPath, RTLD_NOW);
#else
	m_gmsslHandle = NULL;
#endif
    if (NULL == m_gmsslHandle)
        return FALSE;

	//WriteLog_GSDK("LoadAllFunctions ", "nCount : ", sizeof(g_GmsslpFuncNames) / sizeof(g_GmsslpFuncNames[0]));
	for (int i = 0; i < sizeof(g_GmsslpFuncNames) / sizeof(g_GmsslpFuncNames[0]); i++)
	{
#if (_FX_OS_ == _FX_WIN32_DESKTOP_ || _FX_OS_ == _FX_WIN64_)
		m_Functions[i] = GetProcAddress(m_gmsslHandle, g_GmsslpFuncNames[i]);
#else
		m_Functions[i] = dlsym(m_gmsslHandle, g_GmsslpFuncNames[i]);
#endif
	}
#if (_FX_OS_ == _FX_WIN32_DESKTOP_ || _FX_OS_ == _FX_WIN64_)
	SetApplinkFunc(&OPENSSL_Applink);
#endif

	return TRUE;
}

FX_BOOL GmsslFunctions::IsValid()
{
	return m_gmsslHandle != NULL;
}
