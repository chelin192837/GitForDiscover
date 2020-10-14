#ifndef _FS_PDFFORM_IMP_H_
#define _FS_PDFFORM_IMP_H_

#include "../inc/fpdf_define.h"

class FSPDFDoc;
class CPDF_Page;
template<class TYPE>
class CFX_ArrayTemplate;
class CFSCRT_Lock;
class CFSCRT_STPDFInterForm;
class FSFormFillerImp;


class FSFormImp : public FSForm, public CPDF_FormNotify
{
public:
	FSFormImp();
	virtual ~FSFormImp();

	virtual FS_INT32 getFieldCount(const char* filter) const;
	virtual FSFormField* getField(const char* filter, FS_INT32 index);
	virtual FSFormControl* getControlAtPos(FSPDFPage* page, FSPointF pos, FS_FLOAT tolerance);
	virtual FSFormControl* getControlAtDevicePos(FSPDFPage* page, const FSMatrix* matrix, FSPointF pos, FS_FLOAT tolerance);
    virtual FSFormFiller* getFormFiller() const;
	virtual FS_BOOL reset();
	virtual FS_BOOL exportToXML(const char* path);
	virtual FS_BOOL importFromXML(const char* path);

	FS_BOOL FixPageFields(CPDF_Dictionary* pPageDict = NULL);
	FS_RESULT ST_LoadFromPDF(FSPDFDoc* pLTPDFDoc);
	void EnableCalculate(FX_BOOL bEnable);
	FX_BOOL IsCalculateEnabled();
	CFSCRT_STPDFInterForm*  ST_GetSTInterForm();
	FSFormFillerImp*    GetFormFiller();
	void SetFormFiller(FSFormFillerImp* pFormFiller);
	FSPDFDoc*	GetPDFDoc();
	void SetPDFDoc(FSPDFDoc* pDoc);
	CFX_MapPtrToPtr* GetMapFields();
	void SetMapFields(CFX_MapPtrToPtr* pMapFields);
	CFX_ByteString MakeXMLTagName(const CFX_ByteString& sAliasName, const CFX_ByteString& sTrueName);

private:
	CFX_MapPtrToPtr* m_pMapFields;
	CFSCRT_STPDFInterForm* m_pSTInterForm;
	FSFormFillerImp* m_pFormFiller;
	FSPDFDoc* m_pPDFDoc;

	FX_BOOL				m_bCalculate;
};

class FSFormFieldImp : public FSFormField, public CFX_Object
{
public:
	FSFormFieldImp(FSFormImp* pFormImp, CPDF_FormField* pFormField);
	virtual ~FSFormFieldImp();

	virtual FSM_FORMFIELDTYPE getType() const;
	virtual FSM_FORMFIELDFLAGS getFlags() const;
	virtual FSString getName() const;
	virtual FSString getDefaultValue() const;
	virtual FSString getValue() const;
	virtual FS_INT32 getControlCount(FSPDFPage* page) const;
	virtual FSFormControl* getControl(FSPDFPage* page, FS_INT32 index);

private:
	FSFormImp*		m_pFormImp;
	CPDF_FormField* m_pFormField;
};

class FSFormControlImp : public tFSPDFAnnotImp<FSFormControl>
{
public:
	FSFormControlImp(CPDF_Dictionary* pAnnotDict, FSPDFPage* page);
	FSFormControlImp();
	void init(CPDF_Dictionary* pAnnotDict, FSPDFPage* page);
	virtual ~FSFormControlImp();
	virtual FSFormField* getField();
	virtual FS_BOOL isMarkup() const { return FALSE; }
	virtual FSM_ANNOTTYPE getType() const { return FSAnnot::e_annotWidget; }
	virtual FS_BOOL move(FSRectF rect);
	virtual FS_BOOL resetAppearanceStream();

	void loadFormControl(FSForm* form);
	CPDF_FormControl* GetCPDFFormControl();
	FSPDFPage* GetPageView();
	FSPDFPage* GetPage();
	void GetPageIndex(FX_INT32* nPageIndex);
	CPDF_Page* GetPDFPage();

public:
	FX_BOOL	OnAAction(CPDF_AAction::AActionType type, FSPDF_FieldActionParam& data, FSPDFPage* pPageView);
	virtual CPDF_Action	GetAAction(CPDF_AAction::AActionType eAAT);
	FX_INT32 GetAppearanceAge() const {return m_nAppAge;}
	FX_INT32 GetValueAge() const{return m_nValueAge;}
	FS_RESULT	ST_GetFlags(FS_INT32* flags);
	FS_RESULT	ST_SetFlags(FS_INT32 flags);
	FS_RESULT	ST_GetRect(CPDF_Rect& rect);
	FS_RESULT	ST_SetRect(CPDF_Rect& rect);
	FS_BOOL		ST_IsVisible();

	FS_RESULT	IsChecked(FS_BOOL* checked, FS_BOOL isDefault);
	FS_RESULT	SetChecked(FS_BOOL checked, FS_BOOL isDefault);
	FS_RESULT	GetMKRotation(int* rotation);

	void ResetAppearance(CFX_WideString value);
	void ResetAppearance(FX_LPCWSTR value);
	FX_BOOL	drawAppearance(CFX_RenderDevice* pDevice, const CFX_Matrix* pUser2Device, FX_BSTR bsMode, CPDF_RenderOptions* pOptions);
	FX_BOOL	drawInContext(const CPDF_Page* pPage, const CPDF_RenderContext* pContext, const CFX_AffineMatrix* pUser2Device, CPDF_Annot::AppearanceMode mode);
	void clearCachedAppearance();
	CPDF_Form*	getAPForm(FX_BSTR mode);

private:
	CPDF_FormControl* m_pFormControl;
	FSFormFieldImp* m_pFormFieldImp;

	int m_nAppAge;
	int	m_nValueAge;
};

#endif //_FS_PDFFORM_IMP_H_
