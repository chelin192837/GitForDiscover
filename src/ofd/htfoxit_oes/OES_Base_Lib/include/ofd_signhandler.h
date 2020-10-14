#ifndef     __OFD_PFXSIGNHANDLER_HEADER__
#define     __OFD_PFXSIGNHANDLER_HEADER__

class COFD_BaseHandler;

/** @brief  Used to sign data.*/
class COFD_OESSignHandler : public COFD_BaseHandler
{
public:
    static COFD_OESSignHandler* Create(char* pbData, FX_DWORD cbData, std::string PFX_PWD, std::string strDigestMetohd = std::string("sha256"));
    virtual ~COFD_OESSignHandler();

public:

    bool						IsValid(){ return m_bValid; }

    bool						Sign(unsigned char* pbToBeSign, FX_DWORD cbToBeSign,
                                       unsigned char* pbSignedData, FX_DWORD* pcbSignedData);

	void						GetSignCertData(std::string& csCertData);

	void						GetSignAlgorithm(std::string& csCertData);

    void                      Release(){ delete this; }

protected:
    COFD_OESSignHandler();
    COFD_OESSignHandler(COFD_OESSignHandler&){}
    bool Initialize(char* pbData, FX_DWORD cbData, std::string PFX_PWD, std::string strDigestMetohd);

private:

    bool						  m_bValid;
    std::string               m_strPasswd;
    FX_DWORD             m_cbData;
    char*                      m_pbData;
    std::string               m_strDigestMethod;
};

class COFD_OESSignHandlerEx : public COFD_OESSignHandler
{
public:
    static COFD_OESSignHandlerEx* Create(char* pbData, FX_DWORD cbData, std::string PFX_PWD, std::string strDigestMetohd = std::string("sha256"));
    static COFD_OESSignHandlerEx* Create(IHTFX_SignatureCallback* handler, std::string strDigestMetohd = std::string("sha256"));
    
    
public:
    bool						Sign(unsigned char* pbToBeSign, FX_DWORD cbToBeSign,
                                     unsigned char* pbSignedData, FX_DWORD* pcbSignedData);
    
    void						GetSignCertData(std::string& csCertData);
    
    void						GetSignAlgorithm(std::string& csCertData);
    
protected:
    COFD_OESSignHandlerEx();
    bool InitializeEx(IHTFX_SignatureCallback* handler, std::string strDigestMetohd);

protected:
    IHTFX_SignatureCallback*           m_ExtHandler;
};


#endif
