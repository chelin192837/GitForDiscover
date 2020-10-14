#include "oes_sign.h"
#include "fr_include.h"

std::string oes_foxit::GetCertData(CFX_WideString strFilePath, CFX_ByteString strPasswd)
{
    std::string strCertData;
    IFX_FileRead* pFileRead = FX_CreateFileRead(strFilePath);
    if (pFileRead)
    {
        int nSize = pFileRead->GetSize();
        char* buf = new char[nSize + 1];
        memset(buf, 0, nSize + 1);
        pFileRead->ReadBlock(buf, nSize);
        pFileRead->Release();

        std::string strPWD;
        strPWD.assign(strPasswd.GetBuffer(strPasswd.GetLength()), strPasswd.GetLength());
        strPasswd.ReleaseBuffer();

        COFD_OESSignHandler* pHandler = COFD_OESSignHandler::Create(buf, nSize, strPWD);
        if(pHandler)
        {
            pHandler->GetSignCertData(strCertData);
        }
    }

    return strCertData;
}