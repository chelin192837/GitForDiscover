#ifndef _FR_OFDSIGNATURE_HANDLER_H_
#define _FR_OFDSIGNATURE_HANDLER_H_
#include "../../include/common.h"
#include "../../include/fs_signature.h"
#include "htfoxit_oes/OES_Base_Lib/include/ofd_sign.h"

struct FR_OFDSignatureClientData
{
    void*				pClientData;
    FSSignature*		pFSSig;
    FS_DIGEST_ALGORITHM m_algorithm;
    std::string         pfxpath;
    std::string         password;
};

class FR_OFDSignatureHandler : public IHTFX_SignatureCallback
{
public:
    FR_OFDSignatureHandler(FSSignatureHandler* handler);
    ~FR_OFDSignatureHandler();
    
    // sign
    virtual std::string getDigestMethod();
    virtual std::string getSignMethod();
    virtual std::string getCertData();
    virtual std::string sign(std::string digest);
    
public:
    FSSignatureHandler*         m_SigHandler;
    FR_OFDSignatureClientData   m_signClientData;
    std::string     m_CertData;
    std::string     m_digest;
    std::string     m_SignedData;
};

#endif
