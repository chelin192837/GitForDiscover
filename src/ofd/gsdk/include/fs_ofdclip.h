#ifndef _FS_OFDCLIP_H_
#define _FS_OFDCLIP_H_

//
class CFS_OFDContentObject;
class CFS_OFDPathObject;

//
class CFS_OFDClipRegion;
class CFS_OFDClip;
class CFS_OFDClipArea;

//
class CFS_OFDClipRegion : public CFX_Object
{
public:
	CFS_OFDClipRegion();
	~CFS_OFDClipRegion();

	FX_BOOL Create(CFS_OFDContentObject* pContentObj);

	CFS_OFDClip* AddClip();

protected:
	CFS_OFDContentObject* m_pContentObj;
	COFD_WriteClipRegion* m_pWriteClipRegion;

	friend class CFS_OFDClip;
	friend class CFS_OFDClipArea;
};

//
class CFS_OFDClip : public CFX_Object
{
public:
	CFS_OFDClip();
	~CFS_OFDClip();

	FX_BOOL Create(CFS_OFDClipRegion* pClipRegion, COFD_Clip* pReadClip = NULL);

	CFS_OFDClipArea* AddClipArea();

protected:
	CFS_OFDClipRegion* m_pClipRegion;
	COFD_WriteClip* m_pWriteClip;

	friend class CFS_OFDClipArea;
	friend class CFS_OFDClipRegion;
};

//
class CFS_OFDClipArea : public CFX_Object
{
public:
	CFS_OFDClipArea();
	~CFS_OFDClipArea();

	FX_BOOL Create(CFS_OFDClip* pClip, COFD_ClipArea* pReadClipArea = NULL);

	CFS_OFDPathObject* AddPathObject();

protected:
	IOFD_WriteDocument* GetWriteDocument();

	CFS_OFDClip* m_pClip;
	COFD_WriteClipArea* m_pWriteClipArea;

	friend class CFS_OFDClip;
	friend class CFS_OFDClipRegion;
};

#endif //_FS_OFDCLIP_H_
