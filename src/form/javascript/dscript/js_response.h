#ifndef _JS_RESPONSE_H_
#define _JS_RESPONSE_H_

#include "../res/resource.h"

class CJS_Response : public CDialog
{
// Construction
public:
	CJS_Response(CReader_PageView* pPageView);   // standard constructor
	virtual ~CJS_Response();

// Dialog Data
	//{{AFX_DATA(CJS_Response)
	enum { IDD = IDD_RESPONSE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


public:
	void				SetTitle(CFX_WideString swTitle);
	void				SetQuestion(CFX_WideString swQuestion);
	void				SetDefault(CFX_WideString swDefault);
	void				SetLabel(CFX_WideString swLabel);
	void				SetIsVisible(bool bPassword);
	CFX_WideString		GetResponse();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CJS_Response)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
private:
	CFX_WideString		m_swTitle;
	CFX_WideString		m_swQuestion;
	CFX_WideString		m_swDefault;
	CFX_WideString		m_swLabel;
	CFX_WideString		m_swResponse;
	bool				m_bIsVisible;
	CReader_PageView*	m_pPageView;
	CEdit*				m_pResponseEdit;
	FX_HGLOBAL			m_hGlobal;
	HBRUSH				m_hBgBrush;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CJS_Response)
		// NOTE: the ClassWizard will add member functions here
	virtual BOOL OnInitDialog();
	afx_msg void OnResOk();
	afx_msg void OnResCancel();
	afx_msg HBRUSH OnCtlColor( CDC* pDC, CWnd* pWnd, UINT nCtlColor );
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif //_JS_RESPONSE_H_