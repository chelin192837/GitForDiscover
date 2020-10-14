#ifndef _FS_OFDAISINO_FOXIT_H_
#define _FS_OFDAISINO_FOXIT_H_

struct HashFunc {
    void (*HashInit)(void* ctx);
    void (*HashUpdate)(void* ctx, const unsigned char* data, int dataLen);
    void (*HashFinal)(void* ctx, unsigned char* out, int* outLen); //返回byte和长度，最大长度64
    const char* (*GetHashOID)(void* ctx);
    const char* (*GetSignOID)(void* ctx);
};

/**
 * @brief 创建签章handler。
 *
 * @param[in]	srcFile			源文件。
 * @param[in]	signedFile		签过章的文件。
 * @return 成功返回handler，失败返回NULL。
 *				
 */
void* CreateDocSignHandler(char* srcFile, char* signedFile);// UTF8 Encode.

/**
 * @brief 释放。
 *
 * @param[in]	signHandler		签章handler。
 * @return 无。
 *				
 */
void ReleaseSignHandler(void* signHandler);

//添加印章获取文档hash，返回index
/**
 * @brief 添加印章获取文档。
 *
 * @param[in]	handler			签章handler。
 * @param[out]	index			章序号。
 * @param[in]	hashFunc		回调函数（获取hash之后的数据）。
 * @param[in]	ctx				自定义参数。
 * @param[in]	sealData		印章数据。
 * @param[in]	sealDataLen		印章数据长度。
 * @param[in]	page			页面序号。
 * @param[in]	left			签章位置left。
 * @param[in]	top				签章位置top。
 * @param[in]	weight			签章宽度。
 * @param[in]	height			签章高度。
 * @param[in]	providerName	提供者名称。
 * @param[in]	company			公司名称。
 * @param[in]	version			签名版本号。
 * @param[in]	signTime		签名时间。
 * @return 成功返回0，失败返回-1。
 *				
 */
int GetDocDigest(void* handler, int *index, HashFunc *hashFunc, void *ctx, const unsigned char* sealData, int sealDataLen, int page, float left, float top, float weight, float height,
                 const char* providerName, const char* company, const char* version, const char* signTime);

//根据index添加签章
/**
 * @brief 根据index添加签章。
 *
 * @param[in]	handler			签章handler。
 * @param[in]	index			章序号。
 * @param[in]	signData		签名数据。
 * @param[in]	signDataLen		签名数据长度。
 * @return 成功返0，失败返回-1。
 *				
 */
int AddDocSign(void* handler, int index, const unsigned char* signData, int signDataLen);

/**
 * @brief 创建验章handler。
 *
 * @param[in]	signedFile		签过章的文件。
 * @return 成功返回验章handler，失败返回NULL。
 *				
 */
void* CreateDocVerifyHandler(char* signedFile);

/**
 * @brief 释放。
 *
 * @param[in]	verifyHandler	验章handler。
 * @return 无。
 *				
 */
void ReleaseDocVerifyHandler(void* verifyHandler);

/**
 * @brief 获取文档的签章数量。
 *
 * @param[in]	verifyHandler	验章handler。
 * @return 成功返回签章数量，失败返回-1。
 *				
 */
int GetDocSignCount(void* verifyHandler);

/**
 * @brief 获取文档签章的hash。
 *
 * @param[in]	verifyHandler	验章handler。
 * @param[in]	index			章序号。
 * @param[in]	hashFunc		回调函数。（调用者获取并处理数据之后返回）
 * @param[in]	ctx				自定义参数。
 * @return 成功返回动作对象个数，失败返回-1。
 *				
 */
int GetDocSignDigest(void* verifyHandler, int index, HashFunc *hashFunc, void *ctx);

/**
 * @brief 获取签章值。（两次调用，第一次获取长度。)
 *
 * @param[in]	verifyHandler	验章handler。
 * @param[in]	index			章序号。
 * @param[out]	signData		签名数据。
 * @param[out]	signDataLen		签名数据长度。
 * @return 成功返回动作对象个数，失败返回-1。
 *				
 */
int GetDocSignValue(void* verifyHandler, int index, char* signData, int *signDataLen);

/**
 * @brief 获取签章位置。
 *
 * @param[in]	verifyHandler	验章handler。
 * @param[in]	index			章序号。
 * @param[out]	page			页面索引。
 * @param[out]	left			签章位置left。
 * @param[out]	top				签章位置top。
 * @param[out]	weight			签章宽度。
 * @param[out]	height			签章高度。
 * @return 成功返回动作对象个数，失败返回-1。
 *				
 */
int GetDocSignPos(void* verifyHandler, int index, int *page, float *left, float *top, float *weight, float *height);

/**
 * @brief 获取签章hash的OID和签名的OID。（两次调用，第一次获取长度。)
 *
 * @param[in]	verifyHandler	验章handler。
 * @param[in]	index			章序号。
 * @param[out]	hashOID			签章hash的OID。
 * @param[out]	hashOIDLen		签章hash的OID长度。
 * @param[out]	signOID			签名OID。
 * @param[out]	signOIDLen		签名OID长度。
 * @return 成功返回0，失败返回-1。
 *				
 */
int GetDocSignOID(void* verifyHandler, int index, char* hashOID, int* hashOIDLen, char* signOID, int* signOIDLen);

/**
 * @brief 获取签章的属性。（两次调用，第一次获取长度。)
 *
 * @param[in]	verifyHandler	验章handler。
 * @param[in]	index			章序号。
 * @param[out]	providerName	提供者名称。
 * @param[out]	providerNameLen	提供者名称长度。
 * @param[out]	company			公司名称。
 * @param[out]	companyLen		公司名称长度。
 * @param[out]	version			版本号。
 * @param[out]	versionLen		版本号长度。
 * @param[out]	signTime		签章时间。
 * @param[out]	signTimeLen		签章时间长度。

 * @return 成功返回0，失败返回-1。
 *				
 */
int GetDocSignProperty(void* verifyHandler, int index, char* providerName, int* providerNameLen, char* company, int* companyLen, char* version, int* versionLen, char* signTime, int* signTimeLen);


#endif
