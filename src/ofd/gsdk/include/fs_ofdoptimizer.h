#ifndef _FS_OFDOPTIMIZER_H_
#define _FS_OFDOPTIMIZER_H_

class CFS_OptimizerOption : public IOFD_OptimizerOption
{
public:
	CFS_OptimizerOption();
	~CFS_OptimizerOption();

	void OnProgress(FX_INT32 now, FX_INT32 total);
	FX_BOOL GetSecurityModule(FX_INT32 nIndex, IOFD_SecurityHandler*& pSecurityHandler, IOFD_CryptoHandler*& m_pCryptoHandler, 
		FX_LPBYTE& pPassword, FX_INT32& nPwdCount);
};

class CFS_Optimizer : public CFX_Object
{
public:
	CFS_Optimizer(FX_INT32 nOptFlag = 0x8);
	~CFS_Optimizer();

	FX_BOOL Init(FX_LPCWSTR ofdFile);
	FX_BOOL Init(IFX_FileRead* pofdFile);
	FX_BOOL Start(IFX_FileWrite* pOutFile, FX_BOOL bTakeOver = TRUE);
	FX_BOOL Continue();
	FX_BOOL Optimizer(IFX_FileRead* pofdFile, IFX_FileWrite* pOutFile);
	IOFD_Optimizer* GetOpt();
	void SetSrcFile(IFX_FileRead* pInFile);
	void SetDstFile(IFX_FileWrite* pOutFile);
protected:
	IOFD_Optimizer* m_pOpt;
	IOFD_OptimizerOption* m_pOptOption;
	IFX_FileRead* m_pOFDFile;
	IFX_FileWrite* m_pOFDDstFile;
	FX_BOOL m_nOptFlag; //优化标志
	FX_BOOL m_bTakeOver;
};
#endif //_FS_OFDOPTIMIZER_H_