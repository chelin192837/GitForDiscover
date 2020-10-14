#ifndef _FSCRT_ST_PDF_INTERFORM_H_
#define _FSCRT_ST_PDF_INTERFORM_H_

class CFSCRT_STPDFActionHandler;

#define CPDFSDK_FormControlMap	CFX_MapPtrTemplate<CPDF_FormControl*, FSFormControl*>

class CFSCRT_STPDFInterForm : public CPDF_FormNotify
{
public:
	CFSCRT_STPDFInterForm(FSForm* form);
	~CFSCRT_STPDFInterForm();

	FS_RESULT		InitPDFInterForm(CPDF_Document* pPDFDoc);

	//CPDF_FormNotify
	virtual int		BeforeValueChange(const CPDF_FormField* pField, CFX_WideString& csValue);
	virtual int		AfterValueChange(const CPDF_FormField* pField);
	virtual int		BeforeSelectionChange(const CPDF_FormField* pField, CFX_WideString& csValue);
	virtual int		AfterSelectionChange(const CPDF_FormField* pField);
	virtual int		AfterCheckedStatusChange(const CPDF_FormField* pField, const CFX_ByteArray& statusArray);
	virtual int		BeforeFormReset(const CPDF_InterForm* pForm);
	virtual int		AfterFormReset(const CPDF_InterForm* pForm);
	virtual int		BeforeFormImportData(const CPDF_InterForm* pForm);
	virtual int		AfterFormImportData(const CPDF_InterForm* pForm);
	
	void			EnableCalculate(FX_BOOL bEnabled);
	FX_BOOL			IsCalculateEnabled();

	CPDF_InterForm*			m_pInterForm;

	void			SetActionHandler(CFSCRT_STPDFActionHandler* pActionHandler) {m_pActionHandler = pActionHandler;}
	CFSCRT_STPDFActionHandler*	GetActionHandler() {return m_pActionHandler;}
public:
	FX_BOOL			DoAction_Hide(const CPDF_Action& action) {return FALSE;}
	FX_BOOL			DoAction_SubmitForm(const CPDF_Action& action);
	FX_BOOL			DoAction_ResetForm(const CPDF_Action& action);
	FX_BOOL			DoAction_ImportData(const CPDF_Action& action){return FALSE;}

	FX_BOOL			SubmitFields(const CFX_WideString& csDestination, const CFX_PtrArray& fields, FX_BOOL bIncludeOrExclude, FX_BOOL bUrlEncoded);
	FX_BOOL			SubmitForm(const CFX_WideString& sDestination, FX_BOOL bUrlEncoded);
	FX_BOOL			ExportFieldsToFDFTextBuf(const CFX_PtrArray& fields,FX_BOOL bIncludeOrExclude, CFX_ByteTextBuf& textBuf);
	FX_BOOL			FDFToURLEncodedData(FX_LPBYTE& pBuf, FX_STRSIZE& nBufSize);
public:
	void			ResetFieldAppearance(CPDF_FormField* pFormField,FX_LPCWSTR sValue);
	void			UpdateField(CPDF_FormField* pFormField);
	FSFormControl*	GetLTFormControl(CPDF_FormControl* pControl) const;
	FS_RESULT		FixPageFields(CPDF_Dictionary* pPageDict = NULL);

protected:
	void			GetFieldFromObjects(const CFX_PtrArray& objects, CFX_PtrArray& fields);
	int				GetPageIndexByAnnotDict(CPDF_Document* pDocument, CPDF_Dictionary* pAnnotDict) const;
	CPDFSDK_FormControlMap	m_Map;
public:
	void				OnKeyStrokeCommit(CPDF_FormField* pFormField, FSPDF_FieldActionParam& param, FX_BOOL& bRC);
	void				OnValidate(CPDF_FormField* pFormField, FSPDF_FieldActionParam& param, FX_BOOL& bRC);
	void				OnCalculate(CPDF_FormField* pFormField = NULL);
	CFX_WideString		OnFormat(CPDF_FormField* pFormField, int nCommitKey, FX_BOOL& bFormated, FX_BOOL* bFormatAction = NULL);
protected:
	FSForm*			m_pLTPDFForm;
	CFSCRT_STPDFActionHandler* m_pActionHandler;
	FX_DWORD				m_nCalculate;
	FX_BOOL					m_bCalculateEnabled;
};

#endif //_FSCRT_ST_PDF_INTERFORM_H_


