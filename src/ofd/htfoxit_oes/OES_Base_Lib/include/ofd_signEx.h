#ifndef         __OFD_SIGNEX_HEADER__
#define         __OFD_SIGNEX_HEADER__

class IHTFX_Eseal;
class CHTFX_Eseal : public IHTFX_Eseal
{
public:
	static IHTFX_Eseal*			Create(HTFX_ESEAL_PARAM& param);
	virtual ~CHTFX_Eseal();

public:
    virtual bool				SetSignInfo(std::string strCertData,
											std::string strSignAlgorithm, 
											std::string strSignValue);
    virtual bool				OutputSealData(std::string& pstrData);
    virtual bool				OutputSealInfo(std::string* pstrInfo);
	virtual void				Release();

public:
	SESeal_t*					GetSealTag(){ return m_pSeal; }

private:
	CHTFX_Eseal();
    bool						Initialize(HTFX_ESEAL_PARAM& param);

private:
	SESeal_t*					m_pSeal;
};

class CHTFX_SignatureHandler : public IHTFX_SignatureHandler
{
public:
	static CHTFX_SignatureHandler*	Create();
	virtual ~CHTFX_SignatureHandler();

public:

    // by gm
    virtual void                Init(IHTFX_SignatureCallback* handler);
    
	//摘要
    virtual err_t				Digest(unsigned char* pbToBeHash, FX_DWORD cbToBeHash,
									   unsigned char* pbHashedData, int* pcbHashedData);

	//签名
    virtual err_t				Sign(HTFX_SIGN_PARAM& param, unsigned char* pbSignedValue, FX_DWORD* cbSignedValue);

	//验签
    virtual err_t				Verify(HTFX_VERIFY_PARAM& param, unsigned char* pbSignedValue, FX_DWORD cbSignedValue);

    //签署纯数字签名
    virtual err_t            DigitalSign(HTFX_DIGITAL_SIGN_PARAM& param, unsigned char* pbSignedValue, FX_DWORD* cbSignedValue);

    //验证纯数字签名
    virtual err_t            DigitalVerify(HTFX_DIGITAL_VERIFY_PARAM& param, unsigned char* pbSignedValue, FX_DWORD cbSignedValue);

    //获取签名身份
    virtual err_t            DigitalGetSigner(unsigned char* pbSignedValue, FX_DWORD cbSignedValue, char* pbSigner, FX_DWORD* cbSigner);

	//释放内存
	virtual void				Release();

private:
    bool						VerifySeal(std::string& strSealData);

private:
	CHTFX_SignatureHandler();

protected:
    IHTFX_SignatureCallback*           m_ExtHandler;
};

#endif
