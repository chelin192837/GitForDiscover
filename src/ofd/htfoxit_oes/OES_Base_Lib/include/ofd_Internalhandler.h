#ifndef         __OFD_PFXHANDLER_HEADER__
#define         __OFD_PFXHANDLER_HEADER__

class COFD_InternalHandler
{
public:
    static COFD_InternalHandler*	Create();
    virtual ~COFD_InternalHandler();
	
public:
    COFD_OESSignHandler*				GetSignHandler(){ return m_pSignHandler; }
    COFD_OESVerifyHandler*		    GetVerifyHandler(){ return m_pVerifyHandler; }
    void                                      Release();

private:
    COFD_InternalHandler();
    bool	Initialize(char* pbData, FX_DWORD cbData, std::string PFX_PWD);
    bool  CreateAlgorithmHandler(char* pbData, FX_DWORD cbData,
                                                 std::string pstrPassword);

private:
    COFD_OESSignHandler*             m_pSignHandler;
    COFD_OESVerifyHandler*			m_pVerifyHandler;
};


#endif
