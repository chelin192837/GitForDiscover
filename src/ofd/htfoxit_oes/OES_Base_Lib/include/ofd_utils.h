#ifndef     __OFD_UTILS_HEADER__
#define     __OFD_UTILS_HEADER__

#define				FX_ENCRYPT_ARCFOUR_KEY			"htfoxit_Sign_Libary_t"


template<typename T>
struct auto_ptr_release
{
    T* _MyPtr;
    auto_ptr_release( T* ptr)
    {
        _MyPtr = ptr;
    }

    ~auto_ptr_release()
    {
        if(_MyPtr)
            _MyPtr->Release();
    }
};

template<typename p, typename T>
struct auto_ptr_free
{
    T _MyData;
    p _MyFreeProc;
    auto_ptr_free(p proc, T data)
    {
        _MyFreeProc = proc;
        _MyData = data;
    }

    ~auto_ptr_free()
    {
        _MyFreeProc(_MyData);
    }
};

extern void releaseSealTag(SESeal_t seal);
extern void releaseSealPtr(SESeal_t* pSeal);

extern void releaseToSignTag(TBS_Sign_t seal);
extern void releaseToSignPtr(TBS_Sign_t* pToSign);

extern void releaseSignatureTag(SES_Signature_t signature);
extern void releaseSignaturePtr(SES_Signature_t* pSignature);

typedef void (*FreeSealPtrProc)(SESeal_t*);
typedef void (*FreeSignPtrProc)(SES_Signature_t*);

typedef void (*FreeSealTagProc)(SESeal_t);
typedef void (*FreeSignTagProc)(SES_Signature_t);

typedef auto_ptr_free<FreeSealTagProc, SESeal_t> _auto_seal_tag_releaser;
typedef auto_ptr_free<FreeSignTagProc, SES_Signature_t> _auto_sign_tag_releaser;
typedef auto_ptr_free<FreeSealPtrProc, SESeal_t*> _auto_seal_ptr_releaser;
typedef auto_ptr_free<FreeSignPtrProc, SES_Signature_t*> _auto_sign_ptr_releaser;

#define auto_seal_tag_releaser(var)         _auto_seal_tag_releaser _unique_tag_##var_(releaseSealTag, var);
#define auto_sign_tag_releaser(var)         _auto_sign_tag_releaser _unique_tag_##var_(releaseSignatureTag, var);
#define auto_seal_ptr_releaser(var)         _auto_seal_ptr_releaser _unique_ptr_##var_(releaseSealPtr, var);
#define auto_sign_ptr_releaser(var)         _auto_sign_ptr_releaser _unique_ptr_##var_(releaseSignaturePtr, var);

class COFD_Utils
{
public:

    /**@brief       Get SHA256 degist from a seed. */
    static std::string          GetSeedSHA256();

    /**@brief       Get SHA256 degist and reverse it from a seed. */
    static std::string          GetSeedSHA256Reverse();

    /**@ brief      Get usage from certification.
    *
    * return         return true if sucess, othewise return false.
    */
    static bool					IsCertHasSignUsage(X509* x509);

    /**@ brief      Get usage from the memory Data of pfx file.
    *
    * return         return true if sucess, othewise return false.
    */
    static bool                IsCertHasSignUsage(std::string strPFXData, std::string strPassword);

    /**@ brief      Get the Issue of the certification. */
    static std::string        GetCertIssue(std::string strPFXFile);

    /**@ brief      Get the subject name of the certification. */
    static std::string        GetSubjectName(X509* cert);

    /**@ brief      initialize  openssl algorithm. */
    static void                 InitOpenSSL();

    /**@brief       Encode a buffer into Hex format. */
    static void					HexEncode(const char* pBuf, FX_DWORD dwBufLen, char* pOutBuf);

    /**@brief       Decode a buffer from Hex format. */
    static void					HexDecode(const char* pBuf, FX_DWORD dwBufLen, char* pOutBuf);

    /**@brief       Encode a buffer into Base64 format. */
	static void					Base64_Encode(const unsigned char* Data, int dataLen, unsigned char* pOutData, int* piOutDataLen);

    /**@brief       Decode a buffer from Base64 format. */
	static void					Base64_Decode(const unsigned char* Data, int dataLen, unsigned char* pOutData, int* piOutDataLen);

    /**@brief       Get the time struct from a UTC time string. */
    static bool					GetTime_t(std::string strUTCTime, time_t* t);

	//////////////////////////////////////////////////////////////////////////
    //@return					Format the time struct to string(UTF8): yyyy-MM-dd HH:mm:ss
	static std::string			FormatLocalTimeString(time_t* t);
	static std::string			FormatLocalTimeString(std::string strUTCTime);

};

#endif
