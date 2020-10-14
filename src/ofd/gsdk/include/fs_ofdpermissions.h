#ifndef _FS_OFDPERMISSIONS_H_
#define _FS_OFDPERMISSIONS_H_

class CFS_OFDPermissions : public CFX_Object
{
public:
	CFS_OFDPermissions();
	~CFS_OFDPermissions();

	FX_BOOL Create(COFD_Permissions* pPermissions);
	FX_BOOL Create(COFD_WritePermissions* pWPermissions);

	COFD_Permissions* GetPermissions();
	COFD_WritePermissions* GetWritePermissions();

	FX_BOOL	IsEdit();
	FX_BOOL	IsAnnot();
	FX_BOOL	IsExport();
	FX_BOOL	IsSignature();
	FX_BOOL	IsWatermark();
	FX_BOOL	IsPrintScreen();
	FX_BOOL	IsPrintable();
	FX_INT32 GetPrintCopies();
	CFX_WideString GetStartDate();
	CFX_WideString GetEndDate();

	FX_BOOL	SetEdit(FX_BOOL bEdit);
	FX_BOOL	SetAnnot(FX_BOOL bAnnot);
	FX_BOOL	SetExport(FX_BOOL bExport);
	FX_BOOL	SetSignature(FX_BOOL bSignature);
	FX_BOOL	SetWatermark(FX_BOOL bWatermark);
	FX_BOOL	SetPrintScreen(FX_BOOL bPrintScreen);
	FX_BOOL	SetPrintable(FX_BOOL bPrintable);
	FX_BOOL	SetPrintCopies(FX_INT32 nCopies);
	FX_BOOL	SetStartDate(FX_WSTR wsStateDate);
	FX_BOOL	SetEndDate(FX_WSTR wsEndDate);

protected:
	COFD_Permissions* m_pPermissions;
	COFD_WritePermissions* m_pWPermissions;
};

#endif //_FS_OFDPERMISSIONS_H_
