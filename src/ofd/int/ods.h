
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the ODS_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// ODS_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifndef _ODS_H_
#define _ODS_H_

#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)

#ifndef ODS_CALL
#define ODS_CALL __stdcall
#endif

#ifndef ODS_API
#ifdef ODS_EXPORTS
#define ODS_API __declspec(dllexport)
#else
#define ODS_API __declspec(dllimport)
#endif
#endif

#else

#ifndef ODS_CALL
#ifdef _MIPS_ARCH
#define ODS_CALL
#else
#define ODS_CALL __attribute__((stdcall))
#endif
#endif

#ifndef ODS_API
#ifdef ODS_EXPORTS
#define ODS_API __attribute__((visibility("default")))
#else
#define ODS_API
#endif
#endif

#endif

#define ODS_OK						0x00000000	//成功



#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 返回数字签名算法提供者信息 [Required]
 * @param[out]     puchName      名称（UTF-8编码），当为NULL时，通过piNameLen给出长度
 * @param[out/in]  piNameLen     名称长度(有效字符长度，不包括'\0')
 * @param[out]     puchCompany   公司名称（UTF-8编码），当为NULL时，通过piCompanyLen给出长度
 * @param[out/in]  piCompanyLen  公司名称长度(有效字符长度，不包括'\0')
 * @param[out]     puchVersion   版本（UTF-8编码），当为NULL时，通过piVersionLen给出长度
 * @param[out/in]  piVersionLen  版本长度(有效字符长度，不包括'\0')
 * @param[out]     puchExtend    扩展信息（UTF-8编码），当为NULL时，通过piExtendLen给出长度
 * @param[out/in]  piExtendLen   扩展信息长度(有效字符长度，不包括'\0')
 * @return 调用成功返回ODS_OK，否则是错误代码
 */
ODS_API int ODS_CALL    ODS_GetProviderInfo(unsigned char* puchName, int* piNameLen,
                                           unsigned char* puchCompany, int* piCompanyLen,
                                           unsigned char* puchVersion, int* piVersionLen,
                                           unsigned char* puchExtend, int* piExtendLen);

/**
 * @brief 获取签名时间（时间戳或明文形式） [Required]
 * @param[out]     puchSignDateTime          签名时间（字符时用UTF-8编码，形如yyyy-MM-dd hh:mm:ss,时间戳时二进制值），当为NULL时，通过piSignDateTimeLen给出长度
 * @param[out/in]  piSignDateTimeLen         签名时间长度
 * @return 调用成功返回ODS_OK，否则是错误代码
 */
ODS_API int ODS_CALL    ODS_GetSignDateTime(unsigned char* puchSignDateTime,int* piSignDateTimeLen);

/**
 * @brief 获取签名算法标识 [Required]
 * @param[out]     puchSignMethod            签名算法（UTF-8编码），当为NULL时，通过piSignMethodLen给出长度
 * @param[out/in]  piSignMethodLen           签名算法长度
 * @return 调用成功返回ODS_OK，否则是错误代码
 */
ODS_API int ODS_CALL    ODS_GetSignMethod(unsigned char* puchSignMethod,int* piSignMethodLen);

/**
 * @brief 获取摘要算法标识 [Required]
 * @param[out]     puchDigestMethod          摘要算法（UTF-8编码），当为NULL时，通过piDigestMethodLen给出长度
 * @param[out/in]  piDigestMethodLen         摘要算法长度
 * @return 调用成功返回ODS_OK，否则是错误代码
 */
ODS_API int ODS_CALL    ODS_GetDigestMethod(unsigned char* puchDigestMethod,int* piDigestMethodLen);

/**
 * @brief 计算摘要  [Required]
 * @param[in]      puchData                  待摘要的数据
 * @param[in]      iDataLen                  待摘要的数据长度
 * @param[in]      puchDigestMethod          摘要算法
 * @param[in]      iDigestMethodLen          摘要算法长度
 * @param[out]     puchDigestValue           摘要值，当为NULL时，通过piDigestValueLen给出长度
 * @param[out/in]  piDigestValueLen          摘要值长度
 * @return 调用成功返回ODS_OK，否则是错误代码，可调用ODS_GetErrMessage()获取详细信息。
 */
ODS_API unsigned long ODS_CALL    ODS_Digest(unsigned char* puchData,int iDataLen,
                                unsigned char* puchDigestMethod,int iDigestMethodLen,
                                unsigned char* puchDigestValue,int* piDigestValueLen);

/**
 * @brief 计算签名
 * @param[in]      puchId	               签名者标识
 * @param[in]      iIdLen	               签名者标识长度
 * @param[in]      puchDigestData          摘要数据
 * @param[in]      iDigestDataLen          摘要数据长度
 * @param[in]      puchSignMethod          签名算法
 * @param[in]      iSignMethodLen          签名算法长度
 * @param[in]      puchSignDateTime        签名时间
 * @param[in]      iSignDateTimeLen        签名时间长度
 * @param[out]     puchSignValue           签名值，当为NULL时，通过piSignValueLen给出长度
 * @param[out/in]  piSignValueLen          签名值长度
 * @return 调用成功返回ODS_OK，否则是错误代码。
 */
ODS_API unsigned long ODS_CALL    ODS_Sign(unsigned char* puchId,int iIdLen,
                                unsigned char* puchDigestData,int iDigestDataLen,
                                unsigned char* puchSignMethod,int iSignMethodLen,
                                unsigned char* puchSignDateTime,int iSignDateTimeLen,
                                unsigned char* puchSignValue,int* piSignValueLen);

/**
 * @brief 验签实现  [Required]
 * @param[in]  puchDigestValue         摘要值
 * @param[in]  piDigestValueLen        摘要值长度
 * @param[in]  puchSignMethod          签名算法
 * @param[in]  iSignMethodLen          签名算法长度
 * @param[in]  puchSignDateTime        签名时间
 * @param[in]  piSignDateTimeLen       签名时间长度
 * @param[in]  puchSignValue           签名值
 * @param[in]  iSignValueLen           签名值长度
 * @return 调用成功返回ODS_OK，否则是错误代码。
 */
ODS_API unsigned long ODS_CALL    ODS_Verify(unsigned char* puchDigestData,int iDigestDataLen,
                                    unsigned char* puchSignMethod,int iSignMethodLen,
                                    unsigned char* puchSignDateTime,int iSignDateTimeLen,
                                    unsigned char* puchSignValue,int iSignValueLen);

#ifdef __cplusplus
}
#endif

#endif


