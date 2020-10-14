#ifndef     __OFD_DEFINE_HEADER__
#define     __OFD_DEFINE_HEADER__

/************************************************************************/
/*                                                              OES_PROVIDER                                                 					*/
/************************************************************************/
#define OES_PROVIDER_NAME						"htfoxit_eseal"
#define OES_PROVIDER_COMPANY					"htfoxit"
#define OES_PROVIDER_VERSION					"1"

/************************************************************************/
/*                                                          		OFD_ESEAL                                                                      */
/************************************************************************/

//OFD_ESEAL_HEADER
#define		OES_ESEAL_HEADER_ID					"ES"
#define		OES_ESEAL_HEADER_VID				"HTFoxit"
#define		OES_ESEAL_HEADER_VERSION			1

//OFD_ESEAL_TYPE
#define		OES_ESEAL_PROPS_TYPE_COMPANY		0x01
#define		OES_ESEAL_PROPS_TYPE_PERSONAL		0x02

#define		OES_ESEAL_PROPS_CERT_INFO			"x.509"

/************************************************************************/
/*                                                                 	OFD_SIGN                                                                    */
/************************************************************************/
#define		OES_SIGN_TOSIGN_VERSION				0x0a
#define		OES_SIGN_ALGORITHM_SHA1RSA          "1.2.840.113549.1.1.5"
#define		OES_SIGN_ALGORITHM_SHA256RSA		"1.2.840.113549.1.1.11"

/************************************************************************/
/*                                                                     OFD_DIGEST                                                               */
/************************************************************************/
#define     OES_DIGEST_ALGORITHM_SHA1           "1.3.14.3.2.26"   /*szOID_OIWSEC_sha1*/
#define		OES_DIGEST_ALGORITHM_SHA256			"2.16.840.1.101.3.4.2.1" /*szOID_NIST_sha256*/

/************************************************************************/
/*                                                                                                                                                        */
/************************************************************************/
#define		OES_LIBRARY_SEALS_NODE				_T("oes_seals")
#define		OES_LIBRARY_SEAL_NODE				_T("oes_seal")

#define		OES_LIBRARY_SEAL_ID					_T("oes_id") 
#define		OES_LIBRARY_SEAL_UID				_T("oes_uid")
#define		OES_LIBRARY_SEAL_NAME				_T("oes_name")
#define		OES_LIBRARY_SEAL_PATH				_T("oes_dir")

/************************************************************************/
/*                                                                     OES_ERROR                                                                */
/************************************************************************/
#define		err_t								unsigned long

#ifndef		OES_OK
#define		OES_OK								0
#endif

#define		OES_ERROR_OK						OES_OK

#define		OES_ERROR_BUFFER_NOT_ENUGH			0x01		//缓冲区空间不足。
#define		OES_ERROR_INVALID_PTR_NULL			0x02		//错误的空指针。
#define		OES_ERROR_READ_DATA_DENIED			0x03		//数据读取失败。

#define		OES_ERROR_SEALLIST_EMPTY			0x04		//印章列表为空。
#define		OES_ERROR_SEAL_INVALID_ID			0x05		//错误的标识符，印章库中无对应的印章。
#define		OES_ERROR_SEAL_INVALID_DATA			0x06		//印章数据错误。
#define		OES_ERROR_SEAL_NOT_OFFICIAL			0x07		//指定的印章无效。

#define		OES_ERROR_DIGEST_ERROR				0x08		//无效的算法。
#define		OES_ERROR_DIGEST_NOTSUPPORT			0x09		//摘要算法不支持。

#define		OES_ERROR_SIGN_ERROR_PASSWORD		0x10		//私钥保护密码错误。
#define		OES_ERROR_SIGN_ERROR_KEY			0x11		//错误的私钥。
#define		OES_ERROR_SIGN_TOBESIGN_DATA		0x12		//待签数据错误。
#define		OES_ERROR_SIGN_BAD_PARAM			0x13		//参数异常，签名失败。

#define		OES_ERROR_VERIFY_FAILED				0x14		//验证失败。
#define		OES_ERROR_VERIFY_SIGNDATA			0x15		//签名数据错误。

/************************************************************************/
/*                                                                      Operators                                                                   */
/************************************************************************/

template<typename obj_t, typename str_t >
void str2Obj(str_t& str, obj_t& obj)		
{	
	if (str.length() != 0)					
	{
		obj.size = str.length();		
		obj.buf = new uint8_t[obj.size];
        memset(obj.buf, 0, obj.size);

        memcpy(obj.buf, str.data(), obj.size);
	}																	
}

template<typename obj_t, typename str_t >
void obj2str(obj_t& obj, str_t& str)
{
	str.assign((char*)obj.buf, obj.size);
}

template<typename obj_t, typename str_t >
int	obj_equalto_str(obj_t& obj, str_t& str)
{
    str_t strTemp;
    strTemp.assign((char*)obj.buf, obj.size);

	int nRet = memcmp(str.data(), obj.buf, obj.size);

	return nRet == 0;
}

#define			str2IA5(str, id)				str2Obj(str, id)
#define			str2ID(str, id)					str2Obj(str, id)
#define			str2BIT(str, bit)				str2Obj(str, bit)
#define			str2OES(str, oes)				str2Obj(str, oes)
#define			str2UTCTime(str, time)			str2Obj(str, time)

template<typename obj_x, typename obj_y >
void membufcpy(obj_x& dest, obj_y& src)
{
    if (src.size > 0)
    {
        dest.size = src.size;
        dest.buf = new uint8_t[dest.size];
        memcpy(dest.buf, src.buf, src.size);
    }
}

template<typename T>
void memdelete(T& var)
{
    if (var.buf)
    {
        delete[] var.buf;
        var.buf = NULL;
    }
}

#define			memfree(var)\
{\
	if (##var)\
	{\
		free(##var);\
        (##var) = NULL;\
	}\
}

#define         SAFE_DELETE(ptr)\
    do \
    { \
        if (ptr)\
        {\
            delete ptr; \
        } \
        ptr = NULL; \
    }while(false)

#define        SAFE_DELETE_BLOCK(ptr)\
    do \
    { \
        if (ptr)\
        {\
            delete[] ptr; \
        } \
        ptr = NULL; \
    }while(false)

#endif
