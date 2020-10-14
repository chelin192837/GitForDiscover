#ifndef _FS_SIGNATURE_IMP_H_
#define _FS_SIGNATURE_IMP_H_

#include "../../include/fs_signature.h"
#include "../../include/fs_annot.h"
#include "../fxcore.h"
#include "../form/fs_pdfformImp.h"

#if defined(ENABLE_SIGNATURE)
class FSFormField;
class FSBitmap;
class FSFormControlImp;

struct FSSignatureClientData
{
	void*				pClientData;
	FSSignature*		pFSSig;
	FS_DIGEST_ALGORITHM m_algorithm;
};

template<typename IFormControl> class tFSFormControlImp : public IFormControl, public FSFormControlImp
{
public:
	virtual ~tFSFormControlImp(){}
	virtual FS_BOOL				move(FSRectF rect) { return FSFormControlImp::move(rect); } 
	virtual FS_BOOL				resetAppearanceStream() { return FSFormControlImp::resetAppearanceStream(); } 
	virtual FSPDFPage*			getPage() const { return FSPDFAnnotImp::getPage();}
	virtual FS_BOOL				isMarkup() const { return FSFormControlImp::isMarkup();}
	virtual FSM_ANNOTTYPE		getType() const { return FSFormControlImp::getType();}
	virtual FS_INT32			getIndex() const{ return FSPDFAnnotImp::getIndex(); }
	virtual FSString			getContent() const { return FSPDFAnnotImp::getContent(); }
	virtual void				setContent(const char* content) { FSPDFAnnotImp::setContent(content); }
	virtual FSDateTime			getModifiedDateTime() const { return FSPDFAnnotImp::getModifiedDateTime(); }
	virtual void				setModifiedDateTime(FSDateTime dataTime) { FSPDFAnnotImp::setModifiedDateTime(dataTime); }
	virtual FS_DWORD			getFlags() const { return FSPDFAnnotImp::getFlags(); }
	virtual void				setFlags(FS_DWORD flags) { FSPDFAnnotImp::setFlags(flags); }
	virtual FSString			getUniqueID() const { return FSPDFAnnotImp::getUniqueID(); }
	virtual void				setUniqueID(const char* uniqueID) { FSPDFAnnotImp::setUniqueID(uniqueID); }
	virtual FSRectF				getRect() const { return FSPDFAnnotImp::getRect(); }
	virtual FSBorderInfo		getBorderInfo() const { return FSPDFAnnotImp::getBorderInfo(); }
	virtual void				setBorderInfo(FSBorderInfo border) { FSPDFAnnotImp::setBorderInfo(border); }
	virtual FS_ARGB				getBorderColor() const { return FSPDFAnnotImp::getBorderColor(); }
	virtual void				setBorderColor(FS_ARGB color) { FSPDFAnnotImp::setBorderColor(color); }
	virtual FSRectI				getDeviceRect(FS_BOOL bTransformIcon, const FSMatrix* matrix) { return FSPDFAnnotImp::getDeviceRect(bTransformIcon, matrix); }
	virtual FSPDFDictionary*	getDict() const { return FSPDFAnnotImp::getDict(); }
	virtual FS_BOOL				removeProperty(FSM_ANNOTPROPERTY property) { return FSPDFAnnotImp::removeProperty(property); }
	virtual FSFormField*		getField(){return FSFormControlImp::getField();}
};

class FSSignatureImp : public tFSFormControlImp<FSSignature>
{
public:
	virtual FS_BOOL move(FSRectF rect);
	virtual FS_BOOL resetAppearanceStream();
	virtual FSPDFDictionary* getDict() const;

	// FSSignature
	virtual ~FSSignatureImp();
	virtual FS_BOOL	isSigned();
	virtual FS_PROGRESSSTATE startSign(const char* savePath, const char* certPath, const char password[], FS_INT32 passwordLen,
		FS_DIGEST_ALGORITHM digestAlgorithm, FSPause* pause, void* clientData);
	virtual FS_PROGRESSSTATE continueSign();
	virtual FS_PROGRESSSTATE startVerify(FSPause* pause, void* clientData);
	virtual FS_PROGRESSSTATE continueVerify();
	virtual FS_DWORD getState();
	virtual FS_BOOL	clearSignedData();
	virtual	FSPDFDoc*	getDocument();
	virtual FS_DWORD getAppearanceFlags();
	virtual void setAppearanceFlags(FS_DWORD apFlags);
	virtual FSString getKeyValue(FS_SIGNATUREKEYNAME key);
	virtual void setKeyValue(FS_SIGNATUREKEYNAME key, const char* value);
	virtual FSDateTime getSigningTime();
	virtual void setSigningTime(FSDateTime signTime);
	virtual FSBitmap* getBitmap();
	virtual void setBitmap(FSBitmap* bitmap);
	void	setSignerCertPath(const char* certPath, const char password[], FS_INT32 passwordLen);
	void	getSignerCertPath(FSString& certPath, FSString& certPassword);
	virtual FSPDFDictionary*	getSignatureDict();
	virtual void setAppearanceContent(const char* appearanceContent);
    
    void setSignatureHandler(FSSignatureHandler* handler) {}

	//Internal use
	FSSignatureImp(CPDF_Signature* pSignature, FSPDFPage* pPage, FS_BOOL bNewAdd, FS_BOOL bCreateVDict);
	CPDF_Signature*			getPDFSignature() { return m_pSignature; }
	FSFormControlImp* getFormControlImp() { return this; }
private:
	CPDF_Form*		generateSignAP();

	FS_BOOL getDescription(CFX_WideString& wcDes);
	FS_BOOL getSubDescription(FX_BSTR key, CFX_WideString& wcDes);
	FS_BOOL loadImageStream(CPDF_Stream*& pImageStream);
	CPDF_Rect getRotatedRect();
	CPDF_Matrix getRotatedMatrix();
	FS_BOOL addImageResource(CPDF_Document* pPDFDoc, CPDF_Stream*& pImageStream);

private:
	CPDF_Signature*	m_pSignature;
	FS_BOOL			m_bNewAdd;
	FS_BOOL			m_bCreateSigVDict;

	FS_BOOL				m_bNewImage;
	CPDF_Image*			m_pImage;
	CFX_ByteString		m_bsCustomApContent;
	CPDF_Form*			m_pAPForm;

	FSCRT_DEFINELOCK(m_lock);
	IFX_FileStream*		m_pSignFile;
	CPDF_SignatureSign*	m_pSigSign;
	FSPause*			m_pSignPause;
	FSSignatureClientData	m_signClientData;

	CPDF_SignatureVerify*	m_pSigVerify;
	FSPause*				m_pVerifyPause;
	FSSignatureClientData	m_verifyClientData;
};

class CFSRDK_FileRead : public FSFileRead, public CFX_Object
{
public:
	CFSRDK_FileRead(IFX_FileRead* pFxFileRead);
	virtual ~CFSRDK_FileRead();

	virtual FS_BOOL		readBlock(void* buffer, FS_UINT64 offset, FS_UINT64 size);
	virtual FS_UINT64	getSize();
	virtual void		release();
private:
	IFX_FileRead* m_pFxFileRead;
};

class CFSPDF_SignatureHandler: public CPDF_SignatureHandler
{
public:
	CFSPDF_SignatureHandler();
	CFSPDF_SignatureHandler(FSSignatureHandler* pSigHandler);
	~CFSPDF_SignatureHandler();

	virtual FX_BOOL				StartCalcDigest(IFX_FileRead* pFile, const CFX_DWordArray& byteArray, CPDF_Signature* pSig, FX_LPVOID pClientData = NULL);
	virtual FX_INT32			ContinueCalcDigest(IFX_Pause* pPause = NULL, FX_LPVOID pClientData = NULL) ;
	virtual CFX_ByteString		GetDigest(FX_LPVOID pClientData = NULL) const;
	virtual FX_BOOL				Sign(const CFX_ByteString& bsDigest, CFX_ByteString& bsSignedData, FX_LPVOID pClientData = NULL);
	virtual FX_BOOL				Verify(const CFX_ByteString& bsDigest, const CFX_ByteString& bsSignedData, FX_DWORD& dwVerResult, FX_LPVOID pClientData = NULL);
	// GetCertificateInfo() is inherited from CPDF_SignatureHandler but no need to release it to outside yet.
	// So here just return a valid value to fxcore.
	virtual FX_INT32			GetCertificateInfo(CPDF_Signature* pSig, FX_LPVOID pClientData = NULL) const { return CPDF_SIGNATURE_CERT_VALID;}
private:
	FSSignatureHandler* m_pFSSigHandler;
	CFSRDK_FileRead*	m_pFSFileRead;
};

#endif
#endif //_FS_SIGNATURE_IMP_H_
