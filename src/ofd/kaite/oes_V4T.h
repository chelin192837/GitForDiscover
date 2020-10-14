/**
 * 文档处理软件（含版式软件、流式软件和CAD软件等）签章接入接口，代号OESV4
 * 调用连接方式：名为OES_HOME的环境变量取值（组件库所在目录，多个时用;分隔）指向的目录，“先到先创建”。
 * 该文件按照GB/T 33481+GM/T 0031、《N网电子印章技术规范》两组标准及其升级后版本(20173596-T-469)的综合要求编写，由电子文件管理推进联盟负责维护和解释。
 * 微信公众号：电子文件管理推进联盟
 * 联盟网站：http://www.edrm.org.cn
 * QQ群：609147128（电子文件管理推进联盟）
 * 维护：54464170@qq.com
 * 版权：电子文件管理推进联盟（EDRMA，2016-2018）拥有。您可以免费使用或获得咨询服务，但用于商业用途时应注明来源。
 */
#ifndef _KSOES_H_
#define _KSOES_H_

#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)

#ifndef OES_CALL
#define OES_CALL __stdcall
#endif

#ifndef OES_API
#ifdef OES_EXPORTS
#define OES_API __declspec(dllexport)
#else
#define OES_API __declspec(dllimport)
#endif
#endif

#else

#ifndef OES_CALL
#ifdef _MIPS_ARCH
#define OES_CALL
#else
#define OES_CALL __attribute__((stdcall))
#endif
#endif

#ifndef OES_API
#ifdef OES_EXPORTS
#define OES_API __attribute__((visibility("default")))
#else
#define OES_API
#endif
#endif

#endif
/**************************2014年7月03日更新**************************/
//    添加 OES_GetSignMethod、OES_GetDigestMethod方法（这两个方法要同步调用）；
//    添加 OES_Digest 方法；
//    更改 OES_Sign方法，增加DocProperty、SignDateTime入参，修改SignedValue出参定义；
//    更改 OES_Verify方法，增加DocProperty入参，调整SignDateTime、SignedValue顺序；
//    只保留OES_OK常量定义，其他错误码可去掉，通过OES_GetErrMessage获得字符解析即可。

/**************************2014年7月27日更新**************************/
//    添加 OES_GetSealImage 方法，增加 OES_SEALIMAGE_FLAG_XXXX 常量定义；
//    对GetProviderInfo中版本返回值做出约定。

/**************************2016年1月19日更新**************************/
//    添加 OES_GetSignImage 方法，签名值也包含图像数据，必要时可从中提取使用。

/**************************2016年8月15日更新**************************/
//    添加 OES_CANCEL，区分使用者主动取消，增加了预定义了错误码段。

/**************************2017年11月1日更新**************************/
//    添加 OES_NEED_PIN和Login和Logout函数，允许向外委托PIN码UI实现；
//    添加 OES_RESERVED_MAX，规定错误码不得使用保留区以下数值，支持后续发展；
//    添加 OES_RawSign方法，支持纯数字签名；
//    添加 OES_GetSignInfo、GetRawSignInfo方法，支持查看签章/名属性；
//    取消 OES_GetSeal、GetSignDateTime，将OES_GetSignMethod改为可选；
//    增加 OES_GetSealImageById、GetSealInfoById支持选章，查看印章属性；
//    增加 三段式摘要接口，约定超过1024K的原文数据应执行三段式摘要。

/**************************2018年5月3日更新**************************/
//    添加 OES_GetProtocolVersion函数，取消GetProviderInfo版本约定；
//    添加 OES_VERIFYINFO等常量，支持查看签章验证过程各步骤详细信息；
//    添加 OES_GetDateTime、GetTimeStamp方法支持时间同步；
//    约定二进制数字和字符串编码长度都按照内存长度处理，对于字符串“JPG”，则返回长度应为3。


/**************************2018年12月20日更新**************************/
//    增加0x00000100段常量；
//    增加SetDocParam、SetUserParam、SetEnvParam三项接口，约定在返回特定协议常量值时分别调用。
//    某些印章厂商要求在文档签署时要求收集文档的密级、当前操作人和操作环境等信息

/**************************2019年9月23日更新**************************/
//    增加0x00000200段常量；
//    增加GetExtendParamList、SetExtendParam两项接口，约定在返回特定常量值时分别调用。
//    某些操作（如签名、签章、验签等）在调用时需要额外设置特定参数，通过该机制请求及响应
//    OES实现者要求额外数据：返回特定协议常量，并准备好响应GetExtendParamList
//    OES调用者响应额外要求：根据参数名称准备好数据后，通过SetExtendParam设置给OES实现者
//    该接口可以取代SetDocParam、SetUserParam、SetEnvParam

/**************************2019年09月30日更新**************************/
//    添加 OES_OpenSession，OES_CloseSession函数，支持多线程调用
//    取消 SetDocParam、SetUserParam、SetEnvParam三项接口，以及对应常数定义
//    约定：签章组件可根据所在的环境对某些信息进行保密处理，例如将需保密的算法标识替换为***
//    为支持在一个库中同时实现v2、v4，对已定义或有变动的接口都增加数字后缀



/*****************************函数返回值常量定义*******************************/
#define OES_OK					        0x00000000	//调用成功

#define OES_NEED_PIN					0x00000001	//需要设备登录密码
#define OES_CANCEL				        0x00000010	//使用者主动取消
#define OES_DEORECATED				    0x00000011	//不响应，请勿调用
#define OES_UNKOWN						0x00000100	//未知，因算法不支持、环境不具备等无法给出判断
#define OES_NEED_PARAM					0x00000200	//需要特外参数（所需的参数名可通过交互GetExtendParamList、SetExtendParam确定）

/*****************************需要额外参数常量，部分名称参考了GB_T 33190、GB_T 33480*************************/
//文档标识元数据，例如文档GUID标识（doc.DocID）GB_T 33190、公文标识（doc.GWBS）GB_T 33480
//密级标识元数据，例如密级和保密期限（doc.MJHBMQX）、定密时间（doc.DMSJ）//GB_T 33480
//紧急程度元数据，例如紧急程度（doc.JJCD）//GB_T 33480
//文档通用元数据，例如标题（doc.Title）、作者（doc.Author）、主题（doc.Subject）//GB_T 33190

//当前用户相关信息，例如用户标识（usr.UserID）、用户名称（usr.UserName）
//用户部门相关信息，例如部门标识（usr.DeptID）、部门名称（usr.DeptName）

//软件环境相关信息，例如软件类型（env.SoftwareType）、版本号（env.Version）
//硬件环境相关信息，例如机器型号（env.MachineType）、机器名（env.MachineName）
//网络环境相关信息，例如IP地址（env.IP）、网卡标识（env.MacAddr）

//计算机文件相关信息，例如文件名称（arc.FileName）、文件大小（arc.FileSize）
//计算机文件相关信息，例如创建时间（arc.CreateTime）、修改时间（arc.ModifyTime）
//OFD文件内部信息，例如签名描述文件路径（ofd.Sign,曾用名Entry），签名个数（ofd.SignCount）
//OFD文件内部信息，例如包内文件的个数（ofd.EntryCount）等
//------!!!注意，不可随意增加其他名称，若有需要，请联系本文件的维护人员----------------------



/*****************************获知签署或验证中的详细信息***************************************/
#define OES_VERIFYINFO				    0x00001000	//用于查询验证的详细信息的常量参数
#define OES_SIGNINFO				    0x00001001	//用于查询签章的详细信息的常量参数
#define OES_RAWSIGNINFO				    0x00001002	//用于查询签名的详细信息的常量参数

#define OES_RESERVED_MAX                0x00001111  //保留区最大值，函数的错误码不得小于此数


/**************************获取印章图像常量定义*******************************/
#define OES_SEALIMAGE_FLAG_DISPLAY		0x00000000	//显示
#define OES_SEALIMAGE_FLAG_PRINT		0x00000001	//打印
#define OES_SEALIMAGE_FLAG_PREVIEW		0x00000002	//打印预览

//句柄定义
#define OES_HANDLE					void*

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 返回签章组件提供者(OES实现厂商)信息接口 [Required]
 * @param[out]     puchName			出参，签章组件名称（UTF-8编码字符串）应取值为“商标+应用领域”，例如“卫士通-国密1”，当为NULL时，通过piNameLen给出长度
 * @param[out/in]  piNameLen		出参/入参，签章组件名称长度
 * @param[out]     puchCompany		出参，签章组件公司名称（UTF-8编码字符串），当为NULL时，通过piCompanyLen给出长度
 * @param[out/in]  piCompanyLen		出参/入参，签章组件公司名称长度
 * @param[out]     puchVersion		出参，签章组件版本（UTF-8编码字符串），当为NULL时，通过piVersionLen给出长度
 * @param[out/in]  piVersionLen		出参/入参，签章组件版本长度
 * @param[out]     puchExtend		出参，签章组件扩展信息（UTF-8编码字符串），当为NULL时，通过piExtendLen给出长度
 * @param[out/in]  piExtendLen		出参/入参，签章组件扩展信息长度
 * @return 调用成功返回OES_OK，一般不返回错误代码。
 */
OES_API int OES_CALL    OESV4_GetProviderInfo(unsigned char* puchName, int* piNameLen,
                                           unsigned char* puchCompany, int* piCompanyLen,
                                           unsigned char* puchVersion, int* piVersionLen,
                                           unsigned char* puchExtend, int* piExtendLen);


/**
 * @brief 返回签章组件的接口协议版本号 [Required]
 * @param[out]     puchVersion   出参，OES接口协议版本（UTF-8编码字符串，固定为“4”），当为NULL时，通过piVersionLen给出长度
 * @param[out/in]  piVersionLen  出参/入参，OES接口协议版本长度
 *                               例如如果要返回“4.0”，则Len应取值为3；
 * @return 调用成功返回OES_OK，一般不返回错误代码。
 */
OES_API int OES_CALL    OESV4_GetProtocolVersion(unsigned char* puchVersion, int* piVersionLen);


/**
 * @brief 开始线程  [Required]
 * @param[out]     piSession  出参，线程句柄
 * @return 调用成功返回OES_OK，一般不返回错误代码。
 */
OES_API int OES_CALL    OESV4_OpenSession(OES_HANDLE *piSession);

/**
 * @brief 结束线程 [Required]
 * @param[in]      iSession  入参，线程句柄
 * @return 调用成功返回OES_OK，一般不返回错误代码。
 */
OES_API int OES_CALL    OESV4_CloseSession(OES_HANDLE iSession);

/**
 * @brief 从签章组件中获取可用的电子印章列表，可用来进行印章名称到标识的转换  [Required]
 * 实现时务必使用实际内容按编码计算后的长度，禁止使用预估长度
 * 后台调用时，不建议调用
 * @param[in]      iSession				   入参，线程句柄
 * @param[out]     puchSealList            出参，印章列表（UTF-8编码字符串），当为NULL时，通过piSealListLen给出长度
 *                                         形如 “ID1\0Name1\0ID2\0Name2\0\0”，注意以两个\0结尾。其中最后一个以外的‘\0’可使用‘;’代替
 * @param[out/in]  piSealListLen           出参/入参，印章列表长度
 * @return 调用成功返回OES_OK，一般不返回错误代码。
 */
OES_API int OES_CALL    OESV4_GetSealList(OES_HANDLE iSession,unsigned char* puchSealList,int* piSealListLen);

/**
 * @brief 从签章组件中获取可用的签署人身份列表，可用来进行签署人名称（CN项）到密钥对标识的转换  [Required]
 * 实现时务必使用实际内容按编码计算后的长度，禁止使用预估长度
 * 后台调用时，不建议调用
 * @param[in]      iSession				   入参，线程句柄
 * @param[out]     puchCertList            出参，签署人列表（UTF-8编码字符串），当为NULL时，通过piCertListLen给出长度
 *                                         形如 ID1\0CN_Name1\0ID2\0CN_Name2\0\0，注意以两个\0结尾。其中最后一个以外的‘\0’可使用‘;’代替
 * @param[out/in]  piCertListLen           出参/入参，签署人列表长度
 * @return 调用成功返回OES_OK，一般不返回错误代码。
 */
OES_API int OES_CALL    OESV4_GetCertList(OES_HANDLE iSession,unsigned char* puchCertList,int* piCertListLen);

/**
 * @brief 从签章组件中获取指定的印章图像及格式和尺寸信息  [Required]
 * @param[in]      iSession				    入参，线程句柄
 * @param[in]      puchSealId               入参，印章标识或名称（UTF-8编码字符串）
 * @param[in]      iSealIdLen               入参，印章标识或名称长度
 * @param[out]     puchImageData            出参，印章图像数据（二进制值），当为NULL时，通过piImageDataLen给出长度
 * @param[out/in]  piImageDataLen           出参/入参，印章图像数据长度
 * @param[out]     puchImageType            出参，印章图像数据格式（UTF-8编码字符串，取值如"GIF"、"BMP"、"JPG"等）
 * @param[out/in]  piImageTypeLen           出参/入参，印章图像数据格式长度，如想返回"GIF"，对应的长度为3
 * @param[out]	   piSealWidth              出参，印章宽度（单位mm）
 * @param[out]     piSealHeight             出参，印章高度（单位mm）
 * @return 调用成功返回OES_OK，否则应进一步判断是否大于OES_RESERVED_MAX，如果是，可通过GetErrMessage查询详细信息
 */
OES_API int OES_CALL    OESV4_GetSealImageById(OES_HANDLE iSession,unsigned char* puchSealId,int iSealIdLen,
	                                        unsigned char* puchImageData,int* piImageDataLen,
											unsigned char* puchImageType,int* piImageTypeLen,
											int* piSealWidth,int* piSealHeight);


/**
 * @brief 从签章组件中获取指定标识的电子印章相关信息  [Required]
 * @param[in]      iSession				  入参，线程句柄
 * @param[in]      puchSealId             入参，电子印章标识（UTF-8编码字符串）
 * @param[in]      iSealIdLen             入参，电子印章标识长度
 * @param[out]     puchID				  出参，头信息-电子印章数据标识（UTF-8编码字符串，固定为"ES"），当为NULL时，通过piIDLen给出长度
 * @param[out/in]  piIDLen				  出参/入参，头信息-电子印章数据标识长度
 * @param[out]     puchVersion            出参，头信息-版本（UTF-8编码字符串），当为NULL时，通过piVersionLen给出长度
 * @param[out/in]  piVersionLen           出参/入参，头信息-版本数据长度
 * @param[out]     puchVenderId           出参，头信息-厂商（UTF-8编码字符串），当为NULL时，通过piVenderIdLen给出长度
 * @param[out/in]  piVenderIdLen          出参/入参，头信息-厂商长度
 * @param[out]     piSealType			  出参，印章信息-印章类型（UTF-8编码字符串，如"1","2"），当为NULL时，通过piSealTypeLen给出长度
 * @param[out/in]  piSealTypeLen          出参/入参，印章信息-印章类型长度
 * @param[out]     puchSealName           出参，印章信息-印章名称（UTF-8编码字符串），当为NULL时，通过piSealNameLen给出长度
 * @param[out/in]  piSealNameLen          出参/入参，印章信息-印章名称长度
 * @param[out]     puchCertList           出参，印章信息-签章人证书或证书摘要值列表（二进制值）。当为NULL时，通过piCertInfoLen给出长度
 * @param[out/in]  piCertListLen          出参/入参，印章信息-签章人证书或证书摘要值列表长度
 * @param[out]     puchSignedDate         出参，印章信息-制作日期（UTF-8编码字符串，如“20180101115959Z”），当为NULL时，通过piSignedDateLen给出长度
 * @param[out/in]  piSignedDateLen        出参/入参，印章信息-制作日期长度
 * @param[out]     puchValidStart         出参，印章信息-有效起始时间（UTF-8编码字符串，如“20180101115959Z”），当为NULL时，通过piValidStartLen给出长度
 * @param[out/in]  piValidStartLen        出参/入参，印章信息-有效起始时间长度
 * @param[out]     puchValidEnd           出参，印章信息-有效结束时间（UTF-8编码字符串，如“20180101115959Z”），当为NULL时，通过piValidEndLen给出长度
 * @param[out/in]  piValidEndLen          出参/入参，印章信息-有效结束长度
 * @param[out]     puchSignerName         出参，签名信息-制章人证书，也可是为空数组（二进制值）。当为NULL时，通过piSignerNameLen给出长度
 * @param[out/in]  piSignerNameLen        出参/入参，签名信息-制章人证书长度
 * @param[out]     puchSignMethod         出参，签名信息-制章签名方法（UTF-8编码字符串，算法OID），当为NULL时，通过piSignMethodLen给出长度
 * @param[out/in]  piSignMethodLen        出参/入参，签名信息-制章签名方法长度
 * @param[out]     puchSignature          出参，签名信息-签名值（二进制值），当为NULL时，通过piSignatureLen给出长度
 * @param[out/in]  piSignatureLen         出参/入参，签名信息-签名值长度
 * @return 调用成功返回OES_OK，否则应进一步判断是否大于OES_RESERVED_MAX，如果是，可通过GetErrMessage查询详细信息
 */
OES_API int OES_CALL    OESV4_GetSealInfoById(OES_HANDLE iSession,
                                        unsigned char* puchSealId,int iSealIdLen,
                                        unsigned char* puchVersion,int* piVersionLen,
                                        unsigned char* puchVenderId,int* piVenderIdLen,
                                        unsigned char* puchSealType,int* piSealTypeLen,
                                        unsigned char* puchSealName,int* piSealNameLen,
                                        unsigned char* puchCertList,int* piCertListLen,
                                        unsigned char* puchSignedDate,int* piSignedDateLen,
										unsigned char* puchValidStart,int* piValidStartLen,
                                        unsigned char* puchValidEnd,int* piValidEndLen,
                                        unsigned char* puchSignerName,int* piSignerNameLen,
                                        unsigned char* puchSignMethod,int* piSignMethodLen,
                                        unsigned char* puchSignature,int* piSignatureLen);

/**
 * @brief 从签章组件中获取指定的公钥证书  [Required]
 * @param[in]      iSession				    入参，线程句柄
 * @param[in]      puchCertId               入参，证书标识或名称（UTF-8编码字符串）
 * @param[in]      iCertIdLen               入参，证书标识或名称长度
 * @param[out]     puchCertData             出参，证书数据（二进制值），当为NULL时，通过piCertDataLen给出长度
 * @param[out/in]  piCertDataLen            出参/入参，证书数据长度
 * @return 调用成功返回OES_OK，否则应进一步判断是否大于OES_RESERVED_MAX，如果是，可通过GetErrMessage查询详细信息
 */
OES_API int OES_CALL    OESV4_GetCertById(OES_HANDLE iSession,
                                        unsigned char* puchCertId,int iCertIdLen,
                                        unsigned char* puchCertData,int* piCertDataLen);



/**
 * @brief 从电子签章数据中获取印章图像及尺寸  [Required]
 * @param[in]      iSession				      入参，线程句柄
 * @param[in]      puchSignedValue            入参，签章数据（二进制值，符合国密标准，见GM/T 0031 6.2节）
 * @param[in]      iSignedValueLen            入参，签章数据长度
 * @param[in]      puchExtendParam            入参，扩展参数（UTF-8编码字符串），例如可取值为{RenderFlag:1;Extend_N:value_N}，注：RenderFlag取值含义见OES_SEALIMAGE_FLAG_XXX常量定义
 *                                            Extend_N留作扩展，如调用双方确需使用该协议，需向联盟备案
 * @param[in]      iExtendParamLen            入参，扩展参数长度
 * @param[out]     puchPictureData            出参，印章图像数据（二进制值），当为NULL时，通过piSealImageLen给出长度
 * @param[out/in]  piPictureDataLen           出参/入参，印章图像数据长度
 * @param[out]     puchPictureType            出参，印章图像数据格式（UTF-8编码字符串，如"GIF"、"BMP"、"JPG"等）
 * @param[out/in]  piPictureTypeLen           出参/入参，印章图像数据格式长度，返回"GIF"时，对应长度为3
 * @param[out]	   piPictureWidth             出参，印章图像宽度（单位mm）
 * @param[out]     piPictureHeight            出参，印章图像高度（单位mm）
 * @return 调用成功返回OES_OK，否则应进一步判断是否大于OES_RESERVED_MAX，如果是，可通过GetErrMessage查询详细信息
 */
OES_API int OES_CALL    OESV4_GetSignImage(OES_HANDLE iSession,
                                        unsigned char* puchSignedValue,int iSignedValueLen,
                                        unsigned char* puchExtendParam,int iExtendParamLen,
                                        unsigned char* puchPictureData,int* piPictureDataLen,
                                        unsigned char* puchPictureType,int* piPictureTypeLen,
										int* piPictureWidth,int* piPictureHeight);
/**
 * @brief 从电子印章数据中代理解析出来印章图像数据（仅用于兼容历史文件）  [Optional]
 * @param[in]      iSession					  入参，线程句柄
 * @param[in]      puchSealData               入参，印章数据（二进制值，符合国密标准，见GM/T 0031 6.1节）
 * @param[in]      iSealDataLen               入参，印章数据长度
 * @param[in]      puchExtendParam            入参，扩展参数（UTF-8编码字符串），例如可取值为{RenderFlag:1;Extend_N:value_N}，注：RenderFlag取值含义见OES_SEALIMAGE_FLAG_XXX常量定义
 * @param[in]      iExtendParamLen            入参，扩展参数长度
 * @param[out]     puchSealPicture            出参，印章图像数据（二进制值），当为NULL时，通过piSealImageLen给出长度
 * @param[out/in]  piSealPictureLen           出参/入参，印章图像数据长度
 * @param[out]     puchPictureType            出参，印章图像数据格式（UTF-8编码字符串，如"GIF"、"BMP"、"JPG"等）
 * @param[out/in]  piPictureTypeLen           出参/入参，印章图像数据格式长度，返回"GIF"时，对应长度为3
 * @param[out]     piPictureWidth             出参，印章图像宽度（单位mm）
 * @param[out]     piPictureHeight            出参，印章图像高度（单位mm）
 * @return 调用成功返回OES_OK，否则应进一步判断是否大于OES_RESERVED_MAX，如果是，可通过GetErrMessage查询详细信息
 */
OES_API int OES_CALL    OESV4_GetSealImage(OES_HANDLE iSession,
                                        unsigned char* puchSealData,int iSealDataLen,
	                                    unsigned char* puchExtendParam,int iExtendParamLen,
										unsigned char* puchSealPicture,int* piSealPictureLen,
										unsigned char* puchPictureType,int* piPictureTypeLen,
										int* piPictureWidth,int* piPictureHeight);

/**
 * @brief 从电子签章数据中获取相关信息  [Required]
 * @param[in]      iSession				    出参/入参，线程句柄
 * @param[in]      puchSignedValue          出参/入参，签章数据（二进制值，符合国密标准，见GM/T0031 6.2节）
 * @param[in]      iSignedValueLen          出参/入参，签章数据长度
 * @param[out]     puchVersion				出参，签章数据版本（UTF-8编码字符串），当为NULL时，通过piVersionLen给出长度
 * @param[out/in]  piVersionLen				出参/入参，签章数据版本长度
 * @param[out]     puchSealId				出参，印章-标识（UTF-8编码字符串）
 * @param[out/in]  iSealIdLen				出参/入参，印章-标识长度
 * @param[out]     puchVersion              出参，印章-印章数据版本（UTF-8编码字符串），当为NULL时，通过piVersionLen给出长度
 * @param[out/in]  piVersionLen             出参/入参，印章-印章数据版本长度
 * @param[out]     puchVenderId             出参，印章-厂商标识（UTF-8编码字符串），当为NULL时，通过piVenderIdLen给出长度
 * @param[out/in]  piVenderIdLen            出参/入参，印章-厂商标识长度
 * @param[out]     puchSealType             出参，印章-类型（UTF-8编码字符串，如"1"、"2"），当为NULL时，通过piSealTypeLen给出长度
 * @param[out/in]  piSealTypeLen            出参/入参，印章-类型长度
 * @param[out]     puchSealName             出参，印章-名称（UTF-8编码字符串），当为NULL时，通过piSealNameLen给出长度
 * @param[out/in]  piSealNameLen            出参/入参，印章-名称长度
 * @param[out]     puchCertInfo             出参，印章-签章人证书或其摘要列表，也可是为空数组（二进制值）。当为NULL时，通过piCertInfoLen给出长度
 * @param[out/in]  piCertInfoLen            出参/入参，印章-签章人证书或其摘要列表长度
 * @param[out]     puchValidStart           出参，印章-有效起始时间（UTF-8编码字符串，如“20180101115959Z”），当为NULL时，通过piValidStartLen给出长度
 * @param[out/in]  piValidStartLen          出参/入参，印章-有效起始时间长度
 * @param[out]     puchValidEnd             出参，印章-有效结束时间（UTF-8编码字符串，如“20180101115959Z”），当为NULL时，通过piValidEndLen给出长度
 * @param[out/in]  piValidEndLen            出参/入参，印章-有效结束长度
 * @param[out]     puchSignedDate           出参，印章-制作日期（UTF-8编码字符串，如“20180101115959Z”），当为NULL时，通过piSignedDateLen给出长度
 * @param[out/in]  piSignedDateLen          出参/入参，印章-制作日期长度
 * @param[out]     puchSignerName           出参，印章-制章人证书，也可是为空数组（二进制值）。当为NULL时，通过piSignerNameLen给出长度
 * @param[out/in]  piSignerNameLen          出参/入参，印章-制章人证书长度
 * @param[out]     puchSealSignMethod       出参，印章-制章签名方法（UTF-8编码字符串,算法OID），当为NULL时，通过piSignMethodLen给出长度
 * @param[out/in]  piSealSignMethodLen      出参/入参，印章-制章签名方法长度
 * @param[out]     puchSealSignature        出参，印章-制章签名值（二进制值），当为NULL时，通过piSignatureLen给出长度
 * @param[out/in]  piSealSignatureLen       出参/入参，印章-制章签名值长度
 * @param[out]     puchTimeInfo				出参，签章时间（UTF-8编码字符串，如“20180101115959Z”），当为NULL时，通过piTimeInfoLen给出长度
 * @param[out/in]  piTimeInfoLen			出参/入参，签章时间长度
 * @param[out]     puchDataHash				出参，原文摘要（二进制值），当为NULL时，通过piDataHashLen给出长度
 * @param[out/in]  piDataHashLen			出参/入参，原文摘要长度
 * @param[out]     puchPropertyInfo			出参，原文属性信息（UTF-8编码字符串），当为NULL时，通过piPropertyInfoLen给出长度
 * @param[out/in]  piPropertyInfoLen		出参/入参，原文属性信息长度
 * @param[out]     puchCert					出参，签章人证书（二进制），当为NULL时，通过piCertLen给出长度
 * @param[out/in]  piCertLen				出参/入参，签章人证书长度
 * @param[out]     puchSignMethod			出参，签名算法（UTF-8编码字符串，算法OID），当为NULL时，通过piSignMethodLen给出长度
 * @param[out/in]  piSignMethodLen			出参/入参，签名算法长度
 * @param[out]     puchSignature            出参，签名值（二进制值），当为NULL时，通过piSignatureLen给出长度
 * @param[out/in]  piSignatureLen           出参/入参，签名值长度			
 * @return 调用成功返回OES_OK，否则应进一步判断是否大于OES_RESERVED_MAX，如果是，可通过GetErrMessage查询详细信息
 */
OES_API int OES_CALL    OESV4_GetSignInfo(OES_HANDLE iSession,
                                        unsigned char* puchSignedValue,int iSignedValueLen,
                                        unsigned char* puchSignVersion,int* piSignVersionLen,
                                        unsigned char* puchSealId,int* piSealIdLen,
                                        unsigned char* puchSealVersion,int* piSealVersionLen,
                                        unsigned char* puchVenderId,int* piVenderIdLen,
                                        unsigned char* puchSealType,int* piSealTypeLen,
                                        unsigned char* puchSealName,int* piSealNameLen,
                                        unsigned char* puchCertInfo,int* piCertInfoLen,
                                        unsigned char* puchValidStart,int* piValidStartLen,
                                        unsigned char* puchValidEnd,int* piValidEndLen,
                                        unsigned char* puchSignedDate,int* piSignedDateLen,
                                        unsigned char* puchSignerName,int* piSignerNameLen,
                                        unsigned char* puchSealSignMethod,int* piSealSignMethodLen,
                                        unsigned char* puchSealSignature,int* piSealSignatureLen,
                                        unsigned char* puchTimeInfo,int* piTimeInfoLen,
                                        unsigned char* puchDataHash,int* piDataHashLen,
                                        unsigned char* puchPropertyInfo,int* piPropertyLen,
                                        unsigned char* puchCert,int* piCertLen,
                                        unsigned char* puchSignMethod,int* piSignMethodLen,
                                        unsigned char* puchSignature,int* piSignatureLen);

/**
 * @brief 从电子签名数据中获取相关信息  [Required]
 * @param[in]      iSession				    入参，线程句柄
 * @param[in]      puchSignedValue          入参，签名数据（二进制值，符合国密标准，见GM/T0010-2012中"8签名数据类型"）
 * @param[in]      iSignedValueLen          入参，签名数据长度
 * @param[out]     puchVersion				出参，签名数据版本，当为NULL时，通过piVersionLen给出长度
 * @param[out/in]  piVersionLen				出参/入参，签名数据版本长度
 * @param[out]     puchTimeInfo				出参，签名时间（UTF-8编码字符串，如“20180101115959Z”），当为NULL时，通过piTimeInfoLen给出长度
 * @param[out/in]  piTimeInfoLen			出参/入参，签名时间长度
 * @param[out]     puchDataHash				出参，原文摘要（二进制），当为NULL时，通过piDataHashLen给出长度
 * @param[out/in]  piDataHashLen			出参/入参，原文摘要长度
 * @param[out]     puchPropertyInfo			出参，原文属性信息（UTF-8编码字符串），当为NULL时，通过piPropertyInfoLen给出长度
 * @param[out/in]  piPropertyInfoLen		出参/入参，原文属性信息长度
 * @param[out]     puchCert					出参，签名人证书（二进制），当为NULL时，通过piCertLen给出长度
 * @param[out/in]  piCertLen				出参/入参，签名人证书长度
 * @param[out]     puchSignMethod			出参，签名算法（UTF-8编码字符串，算法OID），当为NULL时，通过piSignMethodLen给出长度
 * @param[out/in]  piSignMethodLen			出参/入参，签名算法长度
 * @param[out]     puchSignature            出参，签名值（二进制值），当为NULL时，通过piSignatureLen给出长度
 * @param[out/in]  piSignatureLen           出参/入参，签名值长度
 * @return 调用成功返回OES_OK，否则应进一步判断是否大于OES_RESERVED_MAX，如果是，可通过GetErrMessage查询详细信息
 */
OES_API int OES_CALL    OESV4_GetRawSignInfo(OES_HANDLE iSession,
                                        unsigned char* puchSignedValue,int iSignedValueLen,
                                        unsigned char* puchVersion,int* piVersionLen,
                                        unsigned char* puchTimeInfo,int* piTimeInfoLen,
                                        unsigned char* puchDataHash,int* piDataHashLen,
                                        unsigned char* puchPropertyInfo,int* piPropertyLen,
                                        unsigned char* puchCert,int* piCertLen,
                                        unsigned char* puchSignMethod,int* piSignMethodLen,
                                        unsigned char* puchSignature,int* piSignatureLen);

/**
 * @brief 获取摘要算法标识 [Required]
 * @param[in]      iSession				     入参，线程句柄
 * @param[out]     puchDigestMethod          出参，摘要算法（UTF-8编码字符串，算法OID），当为NULL时，通过piDigestMethodLen给出长度
 * @param[out/in]  piDigestMethodLen         出参/入参，摘要算法长度
 * @return 调用成功返回OES_OK，一般不返回错误码
 */
OES_API int OES_CALL    OESV4_GetDigestMethod(OES_HANDLE iSession,
                                        unsigned char* puchDigestMethod,int* piDigestMethodLen);

/**
 * @brief 代理计算摘要  [Required]
 * @param[in]      iSession				     入参，线程句柄
 * @param[in]      puchData                  入参，待摘要的数据（二进制值）
 * @param[in]      iDataLen                  入参，待摘要的数据长度
 * @param[in]      puchDigestMethod          入参，摘要算法（UTF-8编码字符串，算法OID）
 * @param[in]      iDigestMethodLen          入参，摘要算法长度
 * @param[out]     puchDigestValue           出参，摘要值（二进制值），当为NULL时，通过piDigestValueLen给出长度
 * @param[out/in]  piDigestValueLen          出参/入参，摘要值长度
 * @return 调用成功返回OES_OK，否则应进一步判断是否大于OES_RESERVED_MAX，如果是，可通过GetErrMessage查询详细信息
 */
OES_API unsigned long OES_CALL    OESV4_Digest(OES_HANDLE iSession,
                                        unsigned char* puchData,int iDataLen,
                                        unsigned char* puchDigestMethod,int iDigestMethodLen,
                                        unsigned char* puchDigestValue,int* piDigestValueLen);
/**
 * @brief 代理计算摘要_初始化
 * @param[in]      iSession				     入参，线程句柄
 * @param[in]      puchDigestMethod          入参，摘要算法（UTF-8编码字符串，算法OID）
 * @param[in]      iDigestMethodLen          入参，摘要算法长度
 * @param[out]     piHash                    出参，摘要句柄  
 * @return 调用成功返回OES_OK，否则应进一步判断是否大于OES_RESERVED_MAX，如果是，可通过GetErrMessage查询详细信息
 */
//#if defined (__APPLE__)
OES_API int OES_CALL    OESV4_Digest_Init(OES_HANDLE iSession,unsigned char* puchDigestMethod,int iDigestMethodLen,OES_HANDLE *piHash);
//#endif
/**
 * @brief 代理计算摘要_更新数据
 * @param[in]      iSession				     入参，线程句柄
 * @param[in]      iHash        	         入参，摘要句柄
 * @param[in]      puchData                  入参，待摘要的数据（二进制值）
 * @param[in]      iDataLen                  入参，待摘要的数据长度
 * @return 调用成功返回OES_OK，否则应进一步判断是否大于OES_RESERVED_MAX，如果是，可通过GetErrMessage查询详细信息
 */
//#if defined (__APPLE__)
OES_API int OES_CALL    OESV4_Digest_Update(OES_HANDLE iSession,OES_HANDLE iHash,unsigned char* puchData,int iDataLen);
//#endif
/**
 * @brief 代理计算摘要_计算摘要值
 * @param[in]      iSession				     入参，线程句柄
 * @param[in]      iHandler        	         入参，摘要句柄
 * @param[out]     puchDigestValue           出参，摘要值（二进制值），当为NULL时，通过piDigestValueLen给出长度
 * @param[out/in]  piDigestValueLen          出参/入参，摘要值长度
 * @return 调用成功返回OES_OK，否则应进一步判断是否大于OES_RESERVED_MAX，如果是，可通过GetErrMessage查询详细信息
 */
//#if defined (__APPLE__)
OES_API int OES_CALL    OESV4_Digest_Final(OES_HANDLE iSession,OES_HANDLE iHash,unsigned char* puchDigestValue,int* piDigestValueLen);
//#endif
/**
 * @brief 代理计算签名1  [Required]
 * 注意：该方法生成的签章数据的版本号固定为4。
 * 注意：部分调用者ofd.Sign曾写作Entry，不加doc.usr.等前缀，请注意兼容
 * @param[in]      iSession				   入参，线程句柄
 * @param[in]      puchSealId              入参，印章标识（UTF-8编码字符串）
 * @param[in]      iSealIdLen              入参，印章标识长度
 * @param[in]      puchDocProperty         入参，原文属性，一般为文档标识等文档元数据信息（UTF-8编码字符串）
 *                                         例如:阅读器A签名时{"doc.DocID":"xxx";"doc.Title":"yyy";"ofd.Sign":"/Doc_0/Signs/Sign_0/Signature.xml"}
                                           或   阅读器B验证时/Doc_0/Signs/Sign_0/Signature.xml
 * @param[in]      iDocPropertyLen         入参，原文属性长度
 * @param[in]      puchDigestData          入参，摘要数据（二进制值）
 * @param[in]      iDigestDataLen          入参，摘要数据长度
 * @param[out]     puchSignValue           出参，签名值（二进制值，符合国密标准，见GM/T0031 6.2节，或其升级版），当为NULL时，通过piSignValueLen给出长度
 * @param[out/in]  piSignValueLen          出参/入参，签名值长度
 * @return 应判断返回值与常量OES_OK和OES_RESERVED_MAX的关系
 *         如果大于MAX，可调用OES_GetErrMessage()获取详细信息；
 *         如果等于NEED_PARAM，可依次调用GetExtendParamNameList,SetExtendParam设置所需的参数后再次调用；
 *         如果是小于MAX的其他值，可按预定义协议进行操作如提供PIN码或取消操作；
 *         如果等于OES_OK，可调用OES_GetErrMessage(OES_SIGNINFO)获取详细信息。
 */
OES_API unsigned long OES_CALL    OESV4_Sign(OES_HANDLE iSession,
                                        unsigned char* puchSealId, int iSealIdLen,
                                        unsigned char* puchDocProperty,int iDocPropertyLen,
                                        unsigned char* puchDigestData,int iDigestDataLen,
                                        unsigned char* puchSignValue,int* piSignValueLen);


/**
 * @brief 代理计算签名2  [Required]
 * 注意：部分调用者ofd.Sign曾写作Entry，不加doc.usr.等前缀，请注意兼容
 * @param[in]      iSession				 入参，线程句柄
 * @param[in]      puchCertId            入参，证书标识（UTF-8编码字符串）
 * @param[in]      iCertIdLen            入参，证书标识长度
 * @param[in]      puchDocProperty       入参，原文属性，一般为文档标识等文档元数据信息（UTF-8编码字符串）
 *                                         例如:阅读器A签名时{"doc.DocID":"xxx";"doc.Title":"yyy";"ofd.Sign":"/Doc_0/Signs/Sign_0/Signature.xml"}
 *										   或   阅读器B验证时/Doc_0/Signs/Sign_0/Signature.xml
 * @param[in]      iDocPropertyLen       入参，原文属性长度
 * @param[in]      puchDigestData        入参，摘要数据（二进制值）
 * @param[in]      iDigestDataLen        入参，摘要数据长度
 * @param[out]     puchSignValue         出参，签名值（二进制值，符合国密标准，见GB/T 35275-2017签名数据类型），当为NULL时，通过piSignValueLen给出长度
 * @param[out/in]  piSignValueLen        出参/入参，签名值长度
 * @return 应判断返回值与常量OES_OK和OES_RESERVED_MAX的关系
 *         如果大于MAX，可调用OES_GetErrMessage()获取详细信息；
 *         如果等于NEED_PARAM，可依次调用GetExtendParamNameList,SetExtendParam设置所需的参数后再次调用；
 *         如果是小于MAX的其他值，可按预定义协议进行操作如提供PIN码或取消操作；
 *         如果等于OES_OK，可调用OES_GetErrMessage(OES_RAWSIGNINFO)获取详细信息。
 */
OES_API unsigned long OES_CALL    OESV4_RawSign(OES_HANDLE iSession,
                                        unsigned char* puchCertId, int iCertIdLen,
                                        unsigned char* puchDocProperty,int iDocPropertyLen,
                                        unsigned char* puchDigestData,int iDigestDataLen,
                                        unsigned char* puchSignValue,int* piSignValueLen);

/**
 * @brief 代理验签实现  [Required]
 * @param[in]  iSession				    入参，线程句柄
 * @param[in]  puchDigestData           入参，摘要数据（二进制值）
 * @param[in]  iDigestDataLen           入参，摘要数据长度
 * @param[in]  puchSignValue            入参，签章数据（二进制值）
 * @param[in]  iSignValueLen            入参，签章数据长度
 * @param[in]  iOnline                  入参，是否在线验证，1表示在线，2表示离线
 * @return 应判断返回值与常量OES_OK和OES_RESERVED_MAX的关系
 *         返回大于RESERVED_MAX的错误代码时，可调用GetErrMessage()获取详细信息，并提示或记录日志;
 *         返回OES_UNKOWN时，可调用GetMessage(OES_VERIFYINFO)获取详细信息，并提示或记录日志;
 *         返回OES_OK代码时，可调用GetErrMessage(OES_VERIFYINFO)获取详细信息;
 *         返回OES_NEEDPARAM代码时，可依次调用GetExtendParamNameList,SetExtendParam设置所需的参数后再次调用;
 *         返回小于RESERVED_MAX的预定代码时，按照协议执行对应操作。
 */
OES_API unsigned long OES_CALL    OESV4_Verify(OES_HANDLE iSession,
                                        unsigned char* puchDigestData,int iDigestDataLen,
                                        unsigned char* puchSignValue,int iSignValueLen,
                                        int iOnline);


/**
 * @brief 取得错误信息
 * @param[in]      iSession				入参，线程句柄
 * @param[in]      errCode              入参，错误代码，获得于OES_GetErrMessage()以外的任意函数
 * @param[out]     puchErrMessage       出参，错误信息，其中可包含换行符号（UTF-8编码字符串），当为NULL时，通过piErrMessageLen给出长度
 * @param[out/in]  piErrMessageLen      出参/入参，错误信息长度
 * @return  固定返回OES_OK
*/
OES_API int OES_CALL OESV4_GetErrMessage(OES_HANDLE iSession,
                                        unsigned long errCode,
                                        unsigned char* puchErrMessage,int* piErrMessageLen);




/*******************************实现者可以不提供Pin码框,由调用者（阅读器或SDK）提供，避免OES带窗体库************************/
/** 
* @brief 登陆签章客户端（UKey或其他），密码收集由接口调用方（如版式软件）提供输入框
* @param[in]        iSession			  入参，线程句柄
* @param[in]		puchPIN				  入参，密码组件PIN码(UTF-8编码字符串)
* @param[in]		iPINLen				  入参，PIN码长度
* @return 调用成功返回OES_OK，否则应进一步判断是否大于OES_RESERVED_MAX，如果是，可通过GetErrMessage查询详细信息
**/
OES_API  int OES_CALL OESV4_Login(OES_HANDLE iSession,
                                        unsigned char* puchPIN,int iPINLen);

/**
* @brief 登出签章客户端（UKey或其他）
* @param[in]      iSession				  入参，线程句柄
* @return 调用成功返回OES_OK，否则应进一步判断是否大于OES_RESERVED_MAX，如果是，可通过GetErrMessage查询详细信息
**/
OES_API  int OES_CALL OESV4_Logout(OES_HANDLE iSession);



/**
 * @brief 获取签名算法标识 [Optional]
 * @param[in]      iSession				  入参，线程句柄
 * @param[out]     puchSignMethod         出参，签名算法（UTF-8编码字符串）。当为NULL时，通过piSignMethodLen给出长度
 * @param[out/in]  piSignMethodLen        出参/入参，签名算法长度
 * @return 调用成功返回OES_OK，一般不返回错误码
 */
OES_API int OES_CALL    OESV4_GetSignMethod(OES_HANDLE iSession,
                                        unsigned char* puchSignMethod,int* piSignMethodLen);

/**
 * @brief 获取当前时间 [Optional]
 * @param[in]      iSession				 入参，线程句柄
 * @param[out]     puchSignDateTime      出参，时间（字符时用UTF-8编码，如“20180101115959Z”）。当为NULL时，通过piSignDateTimeLen给出长度
 * @param[out/in]  piSignDateTimeLen     出参/入参，时间长度
 * @return 调用成功返回OES_OK，一般不返回错误码
 */
OES_API int OES_CALL    OESV4_GetDateTime(OES_HANDLE iSession,
                                        unsigned char* puchSignDateTime,int* piSignDateTimeLen);


/**
 * @brief 询问操作所需的额外参数的名称
 * @param[in]      iSession				 入参，线程句柄
 * @param[out]     puchParamName		 出参，所需参数的名称（UTF-8编码字符串，多个参数时用;分隔），当为NULL时，通过piParamNameLen给出长度
 * @param[out/in]  piParamNameLen		 出参/入参，所需参数的名称长度
 * @return 固定返回OES_OK
*/
OES_API int OES_CALL OESV4_GetParamNameList(OES_HANDLE iSession,
                                        unsigned char* puchParamName,int* piParamNameLen);

 /**
 * @brief 设置操作所需的额外参数，这些数据可能是指文档的属性、当前操作者的信息，也有可能是所在环境的信息，还有可能是文档的特定内容
 *        设置文档内容时，可用“行业标识.内容标识”来表示，例如"fp.NativeSign"
 * @param[in]       iSession			 入参，线程句柄
 * @param[in]       puchParamName        入参，参数或者参数集名称（UTF-8编码字符串）.一般对于简单类型的参数用集合名称，例如doc、usr或者env
 * @param[in]       iParamNameLen        入参，参数或者参数集名称长度
 * @param[in]       puchParamValue       入参，参数或者参数集取值（二进制值，字符串时取值使用UTF-8编码）.例如:{"env.SoftwareType":"OFDReader";"env.IP":"127.0.0.1"}
 * @param[in]       iParamValueLen       入参，参数或者参数集取值长度
 * @return 调用成功返回OES_OK，一般不返回错误码
*/
OES_API  int OES_CALL OESV4_SetExtendParam(OES_HANDLE iSession,
                                        unsigned char* puchParamName,int iParamNameLen,
                                        unsigned char* puchParamValue,int iParamValueLen);

/**
 * @brief 设置文档元数据，这些数据可能是指文档密级信息，也可能是指紧急程度、文档标识等其他信息
 *        该接口可用于支持印章厂商实现自定义功能，但应注意，这些传入的数据不得用于破坏数据结构的规范性，建议仅将其用于自定义区
 * @param[in]       puchDocParam        入参，元数据（UTF-8编码字符串）.例如:{"doc.MJHBMQX":"秘密★1年";"doc.GWBS":"1.2.156.3005.1.12100000400002195R-000-2016-0A-00056-X"}
 * @param[in]       iDocParamLen        入参，元数据长度
 * @return 调用成功返回OES_OK，否则是错误码
*/
//OES_API  int OES_CALL OES_SetDocParam(unsigned char* puchDocParam,int iDocParamLen);


 /**
 * @brief 设置用户元数据，这些数据可能是指用户标识、名称、所属单位等信息
 *        该接口可用于支持印章厂商实现自定义功能，但应注意，这些传入的数据不得用于破坏数据结构的规范性，建议仅将其用于自定义区
 * @param[in]       puchUserParam       入参，元数据（UTF-8编码字符串）.例如:{"usr.UserID":"01172";"usr.UserName":"孙╳文";"usr.DeptName":"基础软件研究室";}
 * @param[in]       iUserParamLen       入参，元数据长度
 * @return 调用成功返回OES_OK，否则是错误码
*/
//OES_API  int OES_CALL OES_SetUserParam(unsigned char* puchUserParam,int iUserParamLen);


 /**
 * @brief 设置环境元数据，这些数据可能是指软件的型号、版本号，也有可能是所在环境的标识如IP地址、Mac地址等
 *        该接口可用于支持印章厂商实现自定义功能，但应注意，这些传入的数据不得用于破坏数据结构的规范性，建议仅将其用于自定义区
 * @param[in]       puchEnvParam        入参，元数据（UTF-8编码字符串）.例如:{"env.SoftwareType":"OFDReader";"env.Version":"3.0.18.1228";"env.IP":"127.0.0.1"}
 * @param[in]       iEnvParamLen        入参，元数据长度
 * @return 调用成功返回OES_OK，否则是错误码
*/
//OES_API  int OES_CALL OES_SetEnvParam(unsigned char* puchEnvParam,int iEnvParamLen);

/**
 * @brief 获取当前时间的时间戳形式 [Optional]
 * @param[in]       puchContent         入参，原文（二进制值）
 * @param[in]		iContentLen			入参，原文的长度
 * @param[out]      puchTimeStamp       出参，时间戳（二进制值，GB/T 20520-2006）。当为NULL时，通过piTimeStampLen给出长度
 * @param[out/in]   piTimeStampLen      出参/入参，时间戳长度
 * @return 调用成功返回OES_OK，否则是错误代码
 */
OES_API int OES_CALL    OESV4_GetTimeStamp(OES_HANDLE iSession,unsigned char* puchContent,int iContentLen,
                                        unsigned char* puchTimeStamp,int* piTimeStampLen);

/**
 * @brief 验证时间戳形式 [Optional]
 * @param[in]       puchContent         入参，原文（二进制值）
 * @param[in]		iContentLen			入参，原文的长度
 * @param[in]       puchTimeStamp       入参，时间戳（二进制值，GB/T 20520-2006）。当为NULL时，通过piTimeStampLen给出长度
 * @param[in]       piTimeStampLen      入参，时间戳长度
 * @return 调用成功返回OES_OK，否则是错误代码
 */
OES_API int OES_CALL    OESV4_VerifyTimeStamp(OES_HANDLE iSession,unsigned char* puchContent,int iContentLen,
                                        unsigned char* puchTimeStamp,int piTimeStampLen);



/**
 * @brief 代理解析时间戳 [Optional]
 * @param[in]		puchTimeStamp		入参，时间戳时间（二进制值）
 * @param[in]		iTimeStampLen		入参，时间戳时间长度
 * @param[out]		puchContent			出参，原文（二进制值），当为NULL时，通过piContentLen给出长度
 * @param[out/in]	piContentLen		出参/入参，原文的长度
 * @param[out]		puchDateTime		出参，时间（UTF-8编码字符串，如“20180101115959Z”）。当为NULL时，通过piDateTimeLen给出长度
 * @param[out/in]	piDateTimeLen		出参/入参，时间长度
 * @param[out]		puchCert			出参，签名证书（二进制值）。当为NULL时，通过piCertLen给出长度
 * @param[out/in]	piCertLen			出参/入参，签名证书长度
 * @return 调用成功返回OES_OK，否则是错误代码
 */
OES_API int OES_CALL    OESV4_GetTimeStampInfo(OES_HANDLE iSession,unsigned char* puchTimeStamp,int iTimeStampLen,
                                            unsigned char* puchContent,int* piContentLen,
                                            unsigned char* puchDateTime,int* piDateTimeLen,
                                            unsigned char* puchCert,int* piCertLen);



#ifdef __cplusplus
}
#endif

#endif
