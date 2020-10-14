#ifndef _FS_OFDATTACHMENT_H_
#define _FS_OFDATTACHMENT_H_

//
class CFS_OFDDocument;

//
class CFS_OFDAttachment;
class CFS_OFDAttachments;


class CFS_OFDAttachment : public CFX_Object
{
public:
	CFS_OFDAttachment(CFS_OFDAttachments* pAttachments);
	~CFS_OFDAttachment();

	FX_BOOL Create(CFS_OFDAttachments* pAttachments, COFD_Attachment* pAttachment = NULL);

	COFD_Attachment* GetAttachment();
	COFD_WriteAttachment* GetWriteAttachment();

	CFX_WideString GetName();
	CFX_WideString GetFormat();
	CFX_WideString GetCreationDate();
	CFX_WideString GetModDate();
	FX_BOOL GetVisible();
	CFX_WideString GetUsage();
	void ExportAttachFile(CFX_WideString& wsFile);
	FX_BOOL ExportAttachData(FX_LPBYTE bsData, FX_INT32* size);

	void SetName(CFX_WideString& wsName);
	void SetFormat(CFX_WideString& wsFormat);
	void SetCreationDate(CFX_WideString& wsCreationDate);
	void SetModDate(CFX_WideString& wsModDate);
	void SetVisible(FX_BOOL bVisible);
	void SetUsage(CFX_WideString& wsUsage);
	void SetAttachFile(CFX_WideString& wsFile, CFX_WideString wsFileName = L"");
	void SetAttachFile(FX_LPBYTE lpbData, FX_INT32 iLen, OFD_BOOL bTakeover, CFX_WideString wsFileName = L"");

protected:
	COFD_WriteAttachment* m_pWriteAttachment;
	CFS_OFDAttachments* m_pAttachments;
};


class CFS_OFDAttachments : public CFX_Object
{
public:
	CFS_OFDAttachments();
	~CFS_OFDAttachments();

	FX_BOOL Create(CFS_OFDDocument* pDocument, IOFD_Attachments* pAttachments = NULL);

	FX_INT32 Count();
	CFS_OFDAttachment* Get(FX_INT32 index);
	CFS_OFDAttachment* Add();
	FX_BOOL Delete(int index);

protected:
	CFS_OFDAttachment* FindAttachment_Storage(COFD_Attachment* pAttachment);

	CFS_OFDDocument* m_pDocument;
	IOFD_WriteAttachments* m_pWriteAttachments;

	CFX_PtrList* m_attchments;

	friend class CFS_OFDAttachment;
};

#endif //_FS_OFDATTACHMENT_H_
