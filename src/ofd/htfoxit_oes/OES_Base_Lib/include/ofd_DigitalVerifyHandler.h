#ifndef     __OFD_DIGITAL_VERIFY_HANDLER_HEADER__
#define     __OFD_DIGITAL_VERIFY_HANDLER_HEADER__

class COFD_BaseHandler;

/** @brief 用来验证数字签名(PKCS#7)。*/
class COFD_DigitalVerifyHandler : public COFD_BaseHandler
{
public:
    /** @brief Create from cer file*/
    static COFD_DigitalVerifyHandler* Create();

    virtual ~COFD_DigitalVerifyHandler();

public:
    bool						IsValid(){ return true; }

    bool                      GetSigner(unsigned char* pbSignedValue, FX_DWORD cbSignedValue,
                                               char* pbSigner, FX_DWORD* cbSigner);

    bool						Verify(unsigned char* pbToBeSign, FX_DWORD cbToBeSign,
                                         unsigned char* pbSignedData, FX_DWORD cbSignedData);

    void                      Release(){ delete this; }

private:
    COFD_DigitalVerifyHandler(COFD_DigitalVerifyHandler&){}
    COFD_DigitalVerifyHandler();
};

#endif
