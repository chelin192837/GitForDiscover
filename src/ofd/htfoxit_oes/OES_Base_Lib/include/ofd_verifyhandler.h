#ifndef     __OFD_PFXVERIFYHANDLER_HEADER__
#define     __OFD_PFXVERIFYHANDLER_HEADER__

class COFD_BaseHandler;

/** @brief 用来验证签名。*/
class COFD_OESVerifyHandler : public COFD_BaseHandler
{
public:
    /** @brief Create from pfx file*/
    static COFD_OESVerifyHandler* Create(char* pbPFXData, FX_DWORD cbPFXData, std::string PFX_PWD, std::string strDigestMetohd);

    /** @brief Create from cer file*/
    static COFD_OESVerifyHandler* Create(char* pbCerData, FX_DWORD cbCerData, std::string strDigestMethod);

    virtual ~COFD_OESVerifyHandler();

public:
    bool						IsValid(){ return true; }

    bool						Verify(unsigned char* pbToBeSign, FX_DWORD cbToBeSign,
                                         unsigned char* pbSignedData, FX_DWORD cbSignedData);

    void                      Release(){ delete this; }

private:
    COFD_OESVerifyHandler(COFD_OESVerifyHandler&){}
    COFD_OESVerifyHandler();
    bool    Initialize(char* pbCerData, FX_DWORD cbCerData, std::string strDigestMethod);

public:
    FX_DWORD             m_cbData;
    char*                      m_pbData;
    std::string               m_strDigestMethod;
};

#endif
