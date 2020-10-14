#ifndef _FR_OFDSIGNATUREIMP_H_
#define _FR_OFDSIGNATUREIMP_H_
#include "../../include/common.h"
#include "../../include/fs_signature.h"

#define OFD_SG_RESULT_MODIFY		1

class FR_OFDSignatureHandler;

//class FR_OFDSignatureImp
class FR_OFDSignatureImp : public FSSignature
{
public:
	//FSAnnot
	FSPDFPage*			getPage() const;
	FS_BOOL				isMarkup() const;
	FSM_ANNOTTYPE		getType() const;
	FS_INT32			getIndex() const;
	FSString			getContent() const;
	void				setContent(const char* content);
	FSDateTime			getModifiedDateTime() const;
	void				setModifiedDateTime(FSDateTime dataTime);
	FS_DWORD			getFlags() const;
	void				setFlags(FS_DWORD flags);
	FSString			getUniqueID() const;
	void				setUniqueID(const char* uniqueID);
	FSRectF				getRect() const;
	FS_BOOL				move(FSRectF rect);
	FSBorderInfo		getBorderInfo() const;
	void				setBorderInfo(FSBorderInfo border);
	FS_ARGB				getBorderColor() const;
	void				setBorderColor(FS_ARGB color);
	FS_BOOL				resetAppearanceStream();
	FSRectI				getDeviceRect(FS_BOOL bTransformIcon, const FSMatrix* matrix);
	FSPDFDictionary*	getDict() const;
	FS_BOOL				removeProperty(FSM_ANNOTPROPERTY property);

	//FSFormControl
	FSFormField*		getField();

	//FSSignature
	FS_BOOL				isSigned();
	FS_PROGRESSSTATE	startSign(const char* savePath, const char* certPath, const char password[], FS_INT32 passwordLen, FS_DIGEST_ALGORITHM digestAlgorithm, FSPause* pause, void* clientData);
	FS_PROGRESSSTATE	continueSign();
	FS_PROGRESSSTATE	startVerify(FSPause* pause, void* clientData);
	FS_PROGRESSSTATE	continueVerify();
	FS_DWORD			getState();
	FS_BOOL				clearSignedData();
	FSPDFDoc*			getDocument();
	FS_DWORD			getAppearanceFlags();
	void				setAppearanceFlags(FS_DWORD apFlags);
	FSDateTime			getSigningTime();
	void				setSigningTime(FSDateTime signTime);
	FSString			getKeyValue(FS_SIGNATUREKEYNAME key);
	void				setKeyValue(FS_SIGNATUREKEYNAME key, const char* value);
	FSBitmap*			getBitmap();
	void				setBitmap(FSBitmap* bitmap);
	void				setImagePath(const char* path);
	FSPDFDictionary*	getSignatureDict();
	void				setAppearanceContent(const char* appearanceContent);
    
    void                setSignatureHandler(FSSignatureHandler* handler);

public:
	//Internal use
	FR_OFDSignatureImp();
	~FR_OFDSignatureImp();
	// add new 
	void				init(FR_OFDPageImp* page,/* IOFD_Page* mPage,*/FSRectF rect);
	// load exist
	void				init(COFD_Signatures* signs, int index);

	//Seal function
	FS_BOOL				GenerateSeal(time_t tmNow);
	CFX_WideString		Time2UTCString(time_t t);
	CFX_WideString		GenerateGUID();
	std::string			GetFileString(IFX_FileRead* pFileRead);
#if (_FX_OS_==_FX_WIN32_DESKTOP_)
	FX_BOOL				IsPrivateKeyCert(PCCERT_CONTEXT pCertContext);
	FX_BOOL				IsPrivateHasUsage(PCCERT_CONTEXT pCertContext);
#endif
	CFX_ByteString		CR_Base64_Encode(const unsigned char* Data, int DataByte);
	CFX_ByteString		CR_Base64_Decode(const char* Data, int DataByte, int& OutByte);

	FX_INT32			CreateCertSign(IFX_FileWrite* pOutputStream);

private:
	FR_OFDPageImp*		m_pPage;
	CFX_RectF			m_Rect;
	map<int, string>	m_Value;
	FSDateTime			m_SignDate;
	string				m_CertPath;
	string				m_CertPassword;
	std::string			m_CretData;
	FSBitmap*			m_Bitmap;	
#if (_FX_OS_==_FX_WIN32_DESKTOP_)
	PCCERT_CONTEXT		m_pCertData;
#else
	void*		m_pCertData;
#endif
	IOFD_FileStream*	m_pSealStream;
	std::string			m_ImagePath;

	int					m_State;
    int                 m_OESRet;

protected:
	COFD_Signature* m_pSignature;
    FR_OFDSignatureHandler*     m_pSignHandler;
};

#endif
