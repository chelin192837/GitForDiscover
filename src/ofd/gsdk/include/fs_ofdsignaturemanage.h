#ifndef _FS_OFDSIGNATUREMANAGE_H_
#define _FS_OFDSIGNATUREMANAGE_H_

class CFS_OFDSignatureManage;
class CFS_OESInterface;
class CFS_OESInterface_V0;
class CFS_OESInterface_V1;
class CFS_OESInterface_V2;
class CFS_OESInterface_V3;

enum OESVERRSION
{
	OES_VERSION_0 = 0,	//original version
	OES_VERSION_1,		//XinAn v3 version
	OES_VERSION_2,		//DianKeYuan version
    OES_VERSION_3
};

class CFS_OFDSignatureManage : public CFX_Object 
{
public:
	CFS_OFDSignatureManage();
	~CFS_OFDSignatureManage();

	FX_INT32 LoadSealModule(CFX_WideString& wsOESFile, CFX_ByteString bsPin);
	void FreeSealModule();
	CFS_OESInterface* GetOESInterface();
	CFS_OESInterface* GetOESInterface(OFD_SIGNVERSION eOESVersion);

protected:
	void* g_sealModule;
	CFS_OESInterface* m_pOESInterface;
	CFS_OESInterface_V0* m_pOESInterfaceV0;
	CFS_OESInterface_V1* m_pOESInterfaceV1;
	CFS_OESInterface_V2* m_pOESInterfaceV2;
    CFS_OESInterface_V3* m_pOESInterfaceV3;
private:
	OESVERRSION CheckOESVerion();
};

#endif //_FS_OFDSIGNATUREMANAGE_H_
