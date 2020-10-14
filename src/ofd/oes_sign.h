#ifndef _OES_SIGN_H_
#define _OES_SIGN_H_
namespace oes_foxit
{
    /**@brief       Get cert data from a pfx file(path)*/
    std::string          GetCertData(CFX_WideString strFilePath, CFX_ByteString strPasswd);
}




#endif


