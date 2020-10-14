#ifndef _FS_OFDFILEPACKAGE_H_
#define _FS_OFDFILEPACKAGE_H_

class CFS_OFDDocument;
class CFS_OFDFilePackage;

class COFD_DocProvider;
class IFX_FileWrite;

#define OFD_SAVE_FORMAT_OFD	0
#define OFD_SAVE_FORMAT_PDF	1

class COFD_VerifyPause : public IOFD_VerifyPause
{
public:
	COFD_VerifyPause(IOFD_Verifier* pVerifier);
	~COFD_VerifyPause();

	virtual FX_BOOL					NeedToPauseNow();

	int	GetErrorCount()	{ return m_array.GetSize(); }

	int GetErrorCode(int nIndex);

protected:
	IOFD_Verifier*	m_pVerifier;
	CFX_Int32Array	m_array;
};


typedef struct _OFD_XMLXSD_INFO_ : public CFX_Object{

	CFX_ByteString xmlPath;
	IFX_FileRead* xmlContent;
	CFX_ByteString xsdName;

}OFD_XMLXSD_INFO;


class COFD_VerifyOption : public IOFD_VerifyOption
{
public:
	COFD_VerifyOption();
	~COFD_VerifyOption(); 

	void       OnError(OFD_ERROR_INFO& info);
	void       OnErrorEx(OFD_ERROR_INFOEx& stErrorInfo);
	int		   CountError();
	int		   CountErrorEx();
	OFD_ERROR_INFO*		   GetErrrorInfo(FX_INT32 index);
	OFD_ERROR_INFOEx*	   GetErrrorInfoEx(FX_INT32 index);

	void       OnVerifyXML(const CFX_ByteString& xmlPath, const IFX_FileRead* xmlContent, const CFX_ByteString& xsdName);
	FX_BOOL     GetSecurityModule(int nIndex, IOFD_SecurityHandler*& pSecurityHandler, IOFD_CryptoHandler*& m_pCryptoHandler, 
										FX_LPBYTE& pPassword, FX_INT32& nPwdCount);
	int		   CountXmlXsdInfo();
	OFD_XMLXSD_INFO* 	   GetXmlXsdInfo(FX_INT32 index);

protected:
	CFX_PtrList* m_pErrorInfos;
	CFX_PtrList* m_pXmlXsdInfos;
	CFX_PtrList* m_pErrorInfoExs;
};

/*
 * TODO CFS_OFDFilePackage的设计分为Creator模式/Reader模式，处理多文档时需要考虑两种模式。
 * 
 * Creator模式: 创建一个空的文件，然后编辑
 * Reader模式: 加载一个OFD文件，可以读写
 * 
 */ 
class CFS_OFDFilePackage : public CFX_Object
{
public:
	CFS_OFDFilePackage();
	~CFS_OFDFilePackage();

	// Save to file.
	FX_BOOL Create(const CFX_WideString& wsFileName);
	// Save to stream.
	FX_BOOL Create(IFX_FileWrite* pFileWrite, FX_BOOL bTakeOver);

	// Load from file
	FX_BOOL	LoadFile(const CFX_WideString& wsFileName, FX_BOOL bClone = FALSE);
	// load from verify
	FX_BOOL	LoadFromVerify(const CFX_WideString& wsFileName);
	// Load from memory
	FX_BOOL	LoadBuffer(FX_LPCBYTE pbBuffer, unsigned int dwSize, FX_BOOL bTakeOver, FX_BOOL bClone);
	// Load from file read
	FX_BOOL	LoadFileRead(IFX_FileRead* pFileRead, FX_BOOL bTakeOver, FX_BOOL bClone = FALSE);

	FX_INT32 GetSaveFormat();
	void SetSaveFormat(FX_INT32 iFormat);
	void SetEmbedFont(FX_BOOL bFlag);
	void SetOptimizerFloatBit(FX_INT32 iNumber); //有效位数
	void SetOptimizerCompress(FX_BOOL bFlag); //LZMA压缩
	void SetOptimizerResource(FX_BOOL bFlag); //资源优化

	FX_BOOL Save();
	FX_BOOL SaveAs(const CFX_WideString& wsFileName);
	FX_BOOL SaveAs(IFX_FileWrite* pFileWrite);
	FX_BOOL SaveAsPDF(IFX_FileWrite* pFileWrite);

	FX_BOOL IsEncrypted(int index);
	int GetEmbedMethod(int index);
	FX_BOOL IsAdmin(int index, CFX_ByteString bsPassword);
	FX_INT32 GetEncryptType(int index);

	CFS_OFDDocument* AddDocument();
	FX_INT32 GetDocumentCount();
	CFS_OFDDocument* GetDocument(FX_INT32 nIndex, FX_LPCSTR password = NULL); 
	CFS_OFDDocument* GetDocument2(FX_INT32 nIndex, FX_LPCSTR password, FX_LPCWSTR certPath); 
	CFS_OFDDocument* InsertDocument(FX_INT32 nIndex);
	FX_BOOL RemoveDocument(FX_INT32 nIndex);

	void AddOtherDocument(CFS_OFDFilePackage* pPackageFrom, int index);

	FX_INT32 GetDocumentIndex(CFS_OFDDocument* pOFDDocument);

	void* Search(const CFX_WideString& wsKey);
	void GetText(OFD_WSTR* wstr);

	void Package_Verify();
	FX_INT32 CountErrorInfo();
	FX_INT32 CountErrorInfoEx();
	FX_INT32 CountXmlXsdInfo();

	OFD_ERROR_INFO*	Package_Verify_ErrorInfo(int index);
	OFD_ERROR_INFOEx*	Package_Verify_ErrorInfoEx(int index);
	OFD_XMLXSD_INFO* Package_Verify_XmlXsdInfo(int index);

	IFX_FileWrite* GetFileWrite();
	COFD_DocProvider* GetDocProvider();

	IOFD_Parser* GetParser();

	CFS_OFDDocument* ClearDocument(FX_INT32 nIndex);		// add 20200520, for hanxin; only for LoadFile modoe
protected:
	FX_BOOL _LoadByFileRead(IFX_FileRead* pOFDFile, FX_BOOL bClone = FALSE);
	COFD_CreatorOptions* _Optimizer_SaveOption(IOFD_Creator* pCreator);
	FX_BOOL _Save(const CFX_WideString& wsFileName, FX_BOOL bEmbedFont = FALSE);
	FX_BOOL	_SaveFileWrite(IFX_FileWrite* pFileWrite, FX_BOOL bEmbedFont = FALSE);
	void AddCopyright();

protected:
	CFX_WideString		m_wsFileName;

	IFX_FileWrite*		m_pFileWrite;
	FX_BOOL				m_bTakeOverFileWrite;

	FX_INT32			m_nSaveLabel;			// 0  file save or save as		1 file stream save

	CFX_PtrList*		m_pDocuments;
	CFX_PtrList*		m_pOFDDocuments;
	IOFD_FilePackage*	m_pOFDPackage;
	IOFD_Parser*		m_pOFDParser;			// Creator模式: m_pOFDParser != NULL
	IFX_FileRead*		m_pOFDFile;
	FX_BOOL				m_bTakeOverFileRead;

	FX_LPBYTE			m_pTakeOverBuffer;

	FX_INT32			m_saveFormat;
	FX_BOOL				m_embedFont;
	FX_INT32			m_optimizerFloatBit;
	FX_BOOL				m_optimizerCompress;
	FX_BOOL				m_optimizerResource;

	COFD_DocProvider*	m_docProvider;

	IOFD_Verifier*		m_pVerifier;

	COFD_VerifyOption*  m_pVerifyOption;
	COFD_VerifyPause*	m_pVerifyPause;

	FX_BOOL			m_bLoadSSL;
};

#endif //_FS_OFDFILEPACKAGE_H_
