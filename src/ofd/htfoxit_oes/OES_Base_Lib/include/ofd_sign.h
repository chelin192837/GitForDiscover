/*********************************************************************************
  * @Copyright(C), 2016-2016, HTFoxit
  * @FileName: ofd_sign.h
  * @Author: xiaolong_fan
  * @Version: 1.0
  * @Date: 2016-7-21
  * @Description: sign an ofd document by certification.
  * @Platform: _FXM_PLATFORM_LINUX_
  *
**********************************************************************************/

#ifndef         __INTERNAL_SDK_OFD_SIGN_HEADER__
#define         __INTERNAL_SDK_OFD_SIGN_HEADER__

#ifndef         err_t
#define         err_t                         unsigned long
#endif

#include <string>

namespace oes_foxit
{
    /**@brief       Get cert data from a pfx file(path)*/
    std::string          GetCertData(CFX_WideString strFilePath, CFX_ByteString strPasswd);
    /**@brief       Sign data with a pfx file(path)*/
    std::string          Sign(CFX_WideString strFilePath, CFX_ByteString strPasswd, std::string digest, std::string digestAlg);
}

typedef struct tag_Eseal_Create_Param
{
    /*印章信息*/
    std::string                 strName;         //印章名称
    std::string                 esID;            //电子印章标识，电子印章数据的唯一标识编码

    long                  	 lnWidth;         //印章图片宽度
    long                  	 lnHeight;        //印章图片高度
    std::string                 strPicType;      //印章图片类型
    std::string                 strPicData;      //印章图片数据

    std::string                 strCertData;     //印章证书
    std::string                 strCreateDate;   //印章制作日期
    std::string                 strStartDate;    //印章有效起始日期
    std::string                 strEndDate;      //印章有效终止日期

}HTFX_ESEAL_PARAM;

typedef struct tag_Sign_Create_Param
{
    /*签章信息*/
    std::string                 strSealData;    //印章数据
    std::string                 strDataHash;    //原文杂凑值
    std::string                 strPropInfo;    //原文属性信息
    std::string                 strTimeInfo;    //签章的时间信息
    std::string                 strPFXData;     //签章证书数据
    std::string                 strPassword;    //签章证书密码
}HTFX_SIGN_PARAM;

typedef struct tag_Verify_Param
{
    /*签章信息*/
    std::string                 strSealData;    //印章数据
    std::string                 strDataHash;    //原文杂凑值
    std::string                 strPropInfo;    //原文属性信息
    std::string                 strTimeInfo;    //签章的时间信息
}HTFX_VERIFY_PARAM;

typedef struct tag_Digital_Sign_Param
{
    std::string                 strDataHash;    //原文杂凑值
    std::string                 strPFXData;      //签名证书数据
    std::string                 strPassword;    //签名证书密码
}HTFX_DIGITAL_SIGN_PARAM;

typedef struct tag_Digital_Verify_Param
{
    std::string                 strDataHash;    //原文杂凑值
}HTFX_DIGITAL_VERIFY_PARAM;

/** @brief 用于签章的印章。 */
class IHTFX_Eseal
{
public:

    static IHTFX_Eseal* Create(HTFX_ESEAL_PARAM& param);

public:
    //输出印章数据
    virtual bool                OutputSealData(std::string& pstrData) = 0;

    //释放内存
    virtual void                Release() = 0;
};

class IHTFX_SignatureCallback
{
public:
    virtual std::string getDigestMethod() = 0;
    virtual std::string getSignMethod() = 0;
    virtual std::string getCertData() = 0;
    virtual std::string sign(std::string digest) = 0;
};

/** @brief 签章/验章的句柄。*/
class IHTFX_SignatureHandler
{
public:
    //创建签章句柄。
    static IHTFX_SignatureHandler*      Create();

public:
    // add by gm 20170818, set 3rd party signature handler
    virtual void                Init(IHTFX_SignatureCallback* handler = NULL) = 0;

    //摘要
    virtual err_t               Digest(unsigned char* pbToBeHash, FX_DWORD cbToBeHash,
                                       unsigned char* pbHashedData, int* pcbHashedData) = 0;

    //签名
    virtual err_t               Sign(HTFX_SIGN_PARAM& param, unsigned char* pbSignedValue,  FX_DWORD* cbSignedValue) = 0;

    //验签
    virtual err_t               Verify(HTFX_VERIFY_PARAM& param, unsigned char* pbSignedValue, FX_DWORD cbSignedValue) = 0;

    //签署纯数字签名
    virtual err_t               DigitalSign(HTFX_DIGITAL_SIGN_PARAM& param, unsigned char* pbSignedValue, FX_DWORD* cbSignedValue) = 0;

    //验证纯数字签名
    virtual err_t               DigitalVerify(HTFX_DIGITAL_VERIFY_PARAM& param, unsigned char* pbSignedValue, FX_DWORD cbSignedValue) = 0;

    //获取签名身份
    virtual err_t               DigitalGetSigner(unsigned char* pbSignedValue, FX_DWORD cbSignedValue, char* pbSigner, FX_DWORD* cbSigner) = 0;

    //释放内存
    virtual void                  Release() = 0;
};

#endif //__INTERNAL_SDK_OFD_SIGN_HEADER__

