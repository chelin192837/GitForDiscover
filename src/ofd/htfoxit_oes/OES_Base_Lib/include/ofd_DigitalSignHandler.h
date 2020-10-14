#ifndef     __OFD_DIGITAL_SIGN_HANDLER_HEADER__
#define     __OFD_DIGITAL_SIGN_HANDLER_HEADER__

class COFD_BaseHandler;

/** @brief  Used to sign data.*/
class COFD_DigitalSignHandler : public COFD_BaseHandler
{
public:
    static COFD_DigitalSignHandler* Create(char* pbData, FX_DWORD cbData, std::string PFX_PWD, std::string strFiller = std::string("sha256RSA"));
    virtual ~COFD_DigitalSignHandler(){}

public:

    virtual bool						IsValid(){ return false; }

    virtual bool						Sign(unsigned char* pbToBeSign, FX_DWORD cbToBeSign,
                                               unsigned char* pbSignedData, FX_DWORD* pcbSignedData){ return false; }

    virtual void						GetSignAlgorithm(std::string& algorithm){}

    virtual void                      Release(){ delete this; }
};

class COFD_SHA256DigitalSignHandler : public COFD_DigitalSignHandler
{
public:
    static COFD_SHA256DigitalSignHandler* Create(char* pbData, FX_DWORD cbData, std::string PFX_PWD);
    virtual ~COFD_SHA256DigitalSignHandler();

public:

    virtual bool						IsValid(){ return m_bValid; }

    virtual bool						Sign(unsigned char* pbToBeSign, FX_DWORD cbToBeSign,
                                               unsigned char* pbSignedData, FX_DWORD* pcbSignedData);

    virtual void						GetSignAlgorithm(std::string& algorithm);

    virtual void                      Release(){ delete this; }

private:
    COFD_SHA256DigitalSignHandler();
    COFD_SHA256DigitalSignHandler(COFD_SHA256DigitalSignHandler&){}
    bool Initialize(char* pbData, FX_DWORD cbData, std::string PFX_PWD);

private:

    bool						  m_bValid;
    std::string               m_strPasswd;
    FX_DWORD             m_cbData;
    char*                      m_pbData;
};

#endif //__OFD_DIGITAL_SIGN_HANDLER_HEADER__
