#include "../include/fs_pdfpage.h"
#include "../include/common.h"

#include "fxcore.h"
#include "fs_file.h"
#include "fs_util.h"
#include "annotation/fs_annot_util.h"
#include "annotation/fs_annot_imp.h"
#include "inc/fs_pdfpageImp.h"
#include "inc/fs_archiveImp.h"

#if !defined(_SWIG_ANDROID_) && !defined(_SWIG_IOS_)
class CFS_Archive : public CFX_Object
{
public:
	CFS_Archive():
	  m_pIndirectObjects(NULL), 
		  m_pStream(NULL),
		  m_LoadingPos(0),
		  m_pSavingBuf(NULL)
	  {
		  m_ObjectMap.RemoveAll();
		  m_ObjsMap.RemoveAll();
		  m_ObjsNum.RemoveAll();
	  }

	  ~CFS_Archive() 
	  {
		  if(m_pIndirectObjects)
			  delete m_pIndirectObjects;
		  m_pIndirectObjects = NULL;

		  if(m_pSavingBuf) {
			  m_pSavingBuf->Clear();
			  delete m_pSavingBuf;
		  }
		  m_pSavingBuf = NULL;

		  m_pStream = NULL;
		  m_ObjectMap.RemoveAll();
		  m_ObjsMap.RemoveAll();
		  m_ObjsNum.RemoveAll();
	  }

	  CFS_Archive&		operator >> (FX_BYTE& i);
	  CFS_Archive&		operator >> (int& i);
	  CFS_Archive&		operator >> (FX_DWORD& i);
	  CFS_Archive&		operator >> (FX_FLOAT& i);
	  CFS_Archive&		operator >> (double& i);
	  CFS_Archive&		operator >> (CFX_ByteString& bstr);
	  CFS_Archive&		operator >> (CFX_WideString& wstr);
	  CFS_Archive&		operator >> (CPDF_Object*& pObj);
	  FX_BOOL				IsEOF();
	  FX_BOOL				Read(void* pBuf, FX_DWORD dwSize);

	  CFS_Archive&		operator << (FX_BYTE i);
	  CFS_Archive&		operator << (int i);
	  CFS_Archive&		operator << (FX_DWORD i);
	  CFS_Archive&		operator << (FX_FLOAT i);
	  CFS_Archive&		operator << (double i);
	  CFS_Archive&		operator << (FX_BSTR bstr);
	  CFS_Archive&		operator << (FX_LPCWSTR bstr);
	  CFS_Archive&		operator << (const CFX_WideString& wstr);
	  CFS_Archive&		operator << (const CPDF_Object* pObj);

	  void				Write(const void* pData, FX_STRSIZE dwSize);
	  FX_DWORD			GetLength() { return m_pSavingBuf ? m_pSavingBuf->GetSize() : 0; }
	  FX_LPCBYTE		GetBuffer() { return m_pSavingBuf ? m_pSavingBuf->GetBuffer() : NULL; }
	  void				SetStream(IFX_FileStream* pStream) {m_pStream = pStream;}
	  void				LoadBuffer(FX_LPCBYTE pBuffer, FX_DWORD bufLen)
	  {
		  Reset();
		  m_pSavingBuf->AppendBlock(pBuffer, bufLen);
	  }

	  FX_BOOL SerializeAnnotDict(CPDF_Page* pPage, CPDF_Dictionary* pAnnotDict);
	  FX_BOOL DeserializeAnnotDict(CPDF_Page* pPage, CPDF_Dictionary*& pAnnotDict);

	  void				Reset()
	  {
		  if(m_pSavingBuf)
			  m_pSavingBuf->Clear();
		  m_LoadingPos = 0;
		  m_pStream = NULL;
		  if(m_pIndirectObjects)
			  delete m_pIndirectObjects;
		  m_pIndirectObjects = FX_NEW CPDF_IndirectObjects(NULL);
		  m_ObjectMap.RemoveAll();
		  m_ObjsMap.RemoveAll();
		  m_ObjsNum.RemoveAll();
	  }

public:
	FS_BOOL	Initialize();

protected:
	/** Current loading position. */
	FX_DWORD				m_LoadingPos;
	/** Saving data. */
	CFX_BinaryBuf*			m_pSavingBuf;
	/** Stream data. */
	IFX_FileStream*			m_pStream;
	/** Indirect objects. */
	CPDF_IndirectObjects*	m_pIndirectObjects;
	/** map of indirect objects. */
	CFX_MapPtrToPtr			m_ObjectMap;
	/** map old object to new object. */
	CFX_MapPtrToPtr			m_ObjsMap;

	CFX_DWordArray			m_ObjsNum;
};
#endif

/////////////////////////////////////////////////
// Class FSArchive
FSArchive* FSArchive::create()
{
	FSArchiveImp* pArchive = FX_NEW FSArchiveImp();
	if (pArchive)
		pArchive->initialize();
	return pArchive;
}

/////////////////////////////////////////////////
// Class FSArchiveImp
FSArchiveImp::FSArchiveImp()
	:m_pSTArchive(NULL)
{
}

void FSArchiveImp::release()
{
	delete this;
}

FSArchiveImp::~FSArchiveImp()
{
	SAFE_DELETE(m_pSTArchive);
}

FS_BOOL FSArchiveImp::initialize()
{
	if (m_pSTArchive) return TRUE;
	m_pSTArchive = FX_NEW CFS_Archive();
	if(!m_pSTArchive)
		return FALSE;
	return m_pSTArchive->Initialize();
}

FSString FSArchiveImp::getData() const
{
	if (!m_pSTArchive) return FSString();

	FSString str((const char*)(m_pSTArchive->GetBuffer()), m_pSTArchive->GetLength(), FSString::e_encUnknow);
	return str;
}

FS_BOOL FSArchiveImp::loadData(const char* data)
{
	if (!m_pSTArchive || FSCRT_Util_IsStringEmpty(data)) return FALSE;
	m_pSTArchive->LoadBuffer((FX_LPCBYTE)data, FXSYS_strlen(data));
	return TRUE;
}

FS_BOOL FSArchiveImp::serializeAnnot(FSAnnot* annot)
{
	if (!m_pSTArchive || !annot) return FALSE;
	FSPDFPage* page = annot->getPage();
	if(!page)
		return FALSE;
	FSPDFDictionary* pAnnotDict = annot->getDict();
	if(pAnnotDict)
		return m_pSTArchive->SerializeAnnotDict(((FSPDFPageImp*)page)->getPage(), (CPDF_Dictionary*)pAnnotDict);
	else
		return FALSE;
}

FSAnnot* FSArchiveImp::deserializeAnnot(FSPDFPage* page)
{
	if (!m_pSTArchive || !page /*|| !((FSPDFPageImp*)page)->getAnnotArray()*/) 
		return NULL;
	FSPDFPageImp* pPageImp = (FSPDFPageImp*)page;
	CPDF_Dictionary* pAnnotDict = NULL;
	FSAnnot* pAnnot = NULL;
	if (m_pSTArchive->DeserializeAnnotDict(pPageImp->getPage(), pAnnotDict))
	{
		pAnnot = FSANNOT_UTIL_InsertAnnot(pPageImp->getAnnotCount(), pAnnotDict, page);
		pPageImp->getAnnotArray()->Add(pAnnot);
	}
	return pAnnot;
}

//////////////////////////////////////////////////////////////////////////
// Class CFS_Archive
FX_BOOL CFS_Archive::SerializeAnnotDict(CPDF_Page* pPage, CPDF_Dictionary* pAnnotDict)
{
	if (pAnnotDict == NULL) {
		(*this) << -1;		// flag for NULL object
		return FALSE;
	}

	FX_DWORD objnum = pAnnotDict->GetObjNum();
	*this << objnum;

	if (objnum) {
		// Check if the object is already serialized previously
		FX_LPVOID p;
		if (m_ObjectMap.Lookup((FX_LPVOID)(FX_UINTPTR)objnum, p))
			// No need to serialize again, quit
			return TRUE;
		else
			m_ObjectMap.SetAt((FX_LPVOID)(FX_UINTPTR)objnum, (FX_LPVOID)pAnnotDict);
	}

	// now object type
	FX_DWORD type = pAnnotDict->GetType();
	*this << type;

	FX_POSITION pos = pAnnotDict->GetStartPos();
	while (pos) {
		CFX_ByteString key;
		CPDF_Object* pElement = pAnnotDict->GetNextElement(pos, key);
		//Ignore "P","IRT", "Popup" item.
		//Modified by Yvonne 2014-3-7: ignore "Parent" item as well, since it has been ignored in deserializing.
		if(key.Equal(FX_BSTRC("P")) || key.Equal(FX_BSTRC("Popup"))
			|| key.Equal(("IRT")) || key.Equal("Parent"))
			continue;

		*this << key; 
		*this << pElement;
	}
	*this << CFX_ByteString();	// end tag

	return TRUE;
}

FX_BOOL CFS_Archive::DeserializeAnnotDict(CPDF_Page* pPage, CPDF_Dictionary*& pAnnotDict)
{
	m_ObjsNum.RemoveAll();
	(*this)>>(CPDF_Object*&)pAnnotDict;
	m_ObjsNum.RemoveAll();
	if(!pAnnotDict)
		return FALSE;

	CPDF_Document* pDoc = pPage->m_pDocument;
	if(pAnnotDict->GetObjNum())
	{
		size_t new_objnum;
		if (m_ObjsMap.Lookup((FX_LPVOID)(FX_UINTPTR)pAnnotDict->GetObjNum(), (FX_LPVOID&)new_objnum)) {
			// Yes we can use the mapped native object
			pAnnotDict =(CPDF_Dictionary*) pPage->m_pDocument->GetIndirectObject((FX_DWORD)new_objnum);
			return TRUE;
		}
	}
	pAnnotDict =(CPDF_Dictionary*) pDoc->ImportExternalObject(pAnnotDict, &m_ObjsMap);
	return TRUE;
}

FX_BOOL CFS_Archive::IsEOF()
{
	return m_LoadingPos >= GetLength();
}

CFS_Archive& CFS_Archive::operator >> (FX_BYTE& i)
{
	if (m_LoadingPos >= GetLength()) return *this;
	FX_LPCBYTE pLoadingBuf = GetBuffer();
	i = pLoadingBuf[m_LoadingPos++];
	return *this;
}

CFS_Archive& CFS_Archive::operator >> (int& i)
{
	Read(&i, sizeof(int));
	return *this;
}

CFS_Archive& CFS_Archive::operator >> (FX_DWORD& i)
{
	Read(&i, sizeof(FX_DWORD));
	return *this;
}

CFS_Archive& CFS_Archive::operator >> (FX_FLOAT& i)
{
	Read(&i, sizeof(FX_FLOAT));
	return *this;
}

CFS_Archive& CFS_Archive::operator >> (CFX_ByteString& str)
{
	if (m_LoadingPos + 4 > GetLength()) return *this;
	int len;
	operator >> (len);

	str.Empty();
	if (len <= 0 || m_LoadingPos + len > GetLength()) return *this;

	FX_LPSTR buffer = str.GetBuffer(len);
	FXSYS_memcpy(buffer, GetBuffer() + m_LoadingPos, len);
	str.ReleaseBuffer(len);

	m_LoadingPos += len;
	return *this;
}

CFS_Archive& CFS_Archive::operator >> (CFX_WideString& str)
{
	// first load the UTF-16LE encoded string
	CFX_ByteString encoded;
	operator >> (encoded);
	// now decode
	str = CFX_WideString::FromUTF16LE((const unsigned short*)(FX_LPCSTR)encoded, encoded.GetLength());
	return *this;
}

FX_BOOL CFS_Archive::Read(void* pBuf, FX_DWORD dwSize)
{
	if (m_LoadingPos + dwSize > GetLength()) return FALSE;
	FXSYS_memcpy(pBuf, GetBuffer() + m_LoadingPos, dwSize);
	m_LoadingPos += dwSize;
	return TRUE;
}

CFS_Archive& CFS_Archive::operator << (FX_BYTE i)
{
	if (m_pStream)
		m_pStream->WriteBlock(&i, 1);
	else 
		m_pSavingBuf->AppendByte(i);
	return *this;
}

CFS_Archive& CFS_Archive::operator << (int i)
{
	if (m_pStream)
		m_pStream->WriteBlock(&i, sizeof(int));
	else
		m_pSavingBuf->AppendBlock(&i, sizeof(int));
	return *this;
}

CFS_Archive& CFS_Archive::operator << (FX_DWORD i)
{
	if (m_pStream)
		m_pStream->WriteBlock(&i, sizeof(FX_DWORD));
	else
		m_pSavingBuf->AppendBlock(&i, sizeof(FX_DWORD));
	return *this;
}

CFS_Archive& CFS_Archive::operator << (FX_FLOAT f)
{
	if (m_pStream)
		m_pStream->WriteBlock(&f, sizeof(FX_FLOAT));
	else
		m_pSavingBuf->AppendBlock(&f, sizeof(FX_FLOAT));
	return *this;
}

CFS_Archive& CFS_Archive::operator << (FX_BSTR bstr)
{
	// First, the length
	int len = bstr.GetLength();
	if (m_pStream) {
		m_pStream->WriteBlock(&len, sizeof(int));
		m_pStream->WriteBlock(bstr, len);
	} else {
		m_pSavingBuf->AppendBlock(&len, sizeof(int));
		m_pSavingBuf->AppendBlock(bstr, len);
	}
	return *this;
}

CFS_Archive& CFS_Archive::operator << (FX_LPCWSTR wstr)
{
	// First, the length
	FX_STRSIZE len = (FX_STRSIZE)FXSYS_wcslen(wstr);
	if (m_pStream) {
		m_pStream->WriteBlock(&len, sizeof(int));
		m_pStream->WriteBlock(wstr, len);
	} else {
		m_pSavingBuf->AppendBlock(&len, sizeof(int));
		m_pSavingBuf->AppendBlock(wstr, len);
	}
	return *this;
}

CFS_Archive& CFS_Archive::operator << (const CFX_WideString& wstr)
{
	// translate to UTF-16LE encoding then write
	CFX_ByteString encoded = wstr.UTF16LE_Encode();
	return operator << (encoded);
}

void CFS_Archive::Write(const void* pData, FX_STRSIZE dwSize)
{
	if (m_pStream) 
		m_pStream->WriteBlock(pData, dwSize);
	else
		m_pSavingBuf->AppendBlock(pData, dwSize);
}

CFS_Archive& CFS_Archive::operator << (const CPDF_Object* pObj)
{
	if (pObj == NULL) {
		(*this) << -1;		// flag for NULL object
		return *this;
	}

	FX_DWORD objnum = pObj->GetObjNum();
	*this << objnum;

	if (objnum) {
		// Check if the object is already serialized previously
		FX_LPVOID p;
		if (m_ObjectMap.Lookup((FX_LPVOID)(FX_UINTPTR)objnum, p))
			// No need to serialize again, quit
			return *this;
		else
			m_ObjectMap.SetAt((FX_LPVOID)(FX_UINTPTR)objnum, (FX_LPVOID)pObj);
	}

	// now object type
	FX_DWORD type = pObj->GetType();
	*this << type;

	switch (type) {
	case PDFOBJ_ARRAY: {
		CPDF_Array* pArray = (CPDF_Array*)pObj;
		*this << pArray->GetCount();
		for (FX_DWORD i = 0; i < pArray->GetCount();i ++)
			*this << pArray->GetElementValue(i);
		break; }
	case PDFOBJ_DICTIONARY: {
		CPDF_Dictionary* pDict = (CPDF_Dictionary*)pObj;
		FX_POSITION pos = pDict->GetStartPos();
		while (pos) {
			CFX_ByteString key;
			CPDF_Object* pElement = pDict->GetNextElement(pos, key);
			if(key.Equal(FX_BSTRC("P")) || key.Equal(FX_BSTRC("Popup"))
				|| key.Equal(("IRT")) || key.Equal(FX_BSTRC("Parent")))
				continue;
			*this << key; 
			*this << pElement;
		}
		*this << CFX_ByteString();	// end tag
		break; }
	case PDFOBJ_STREAM: {
		CPDF_Stream* pStream = (CPDF_Stream*)pObj;
		CPDF_StreamAcc acc;
		if(!acc.LoadAllData(pStream, TRUE))
			break;
		*this << pStream->GetDict() << (FX_DWORD)acc.GetSize();;
		Write(acc.GetData(), acc.GetSize());
		break; }
	case PDFOBJ_REFERENCE:
		*this << pObj->GetDirect();
		break;
	case PDFOBJ_BOOLEAN:
		*this << pObj->GetInteger();
		break;
	case PDFOBJ_NUMBER:
		*this << ((CPDF_Number*)pObj)->IsInteger();
		if (((CPDF_Number*)pObj)->IsInteger()) 
			*this << ((CPDF_Number*)pObj)->GetInteger();
		else
			*this << ((CPDF_Number*)pObj)->GetNumber();
		break;
	case PDFOBJ_STRING:
	case PDFOBJ_NAME:
		*this << pObj->GetString();
		break;
	case PDFOBJ_NULL:
		break;
	}
	return *this;
}

CFS_Archive& CFS_Archive::operator >> (CPDF_Object*& pObj)
{
	// First object number or null flag
	FX_DWORD objnum;
	*this >> objnum;
	if (objnum == (FX_DWORD)-1) {
		// null object
		pObj = NULL;
		return *this;
	}

	if (objnum) {
		// Indirect object, check if the object is already loaded previously
		pObj = m_pIndirectObjects->GetIndirectObject(objnum);
		if (!pObj) {
			if (m_ObjsNum.Find(objnum) != -1) {
				return *this;
			}
		}
		if (pObj) return *this;	// No need to load again, return directly
		m_ObjsNum.Add(objnum);
	}

	// Now object type
	FX_DWORD type;
	*this >> type;

	switch (type) {
	case PDFOBJ_ARRAY: {
		int count;
		*this >> count;
		CPDF_Array* pArray = FX_NEW CPDF_Array;
		if(!pArray)
			break;
		for (int i = 0; i < count; i ++) {
			CPDF_Object* pElement;
			*this >> pElement;
			if (pElement)
				pArray->Add(pElement, m_pIndirectObjects);
		}
		pObj = pArray;
		break; }
	case PDFOBJ_DICTIONARY: {
		CPDF_Dictionary* pDict = FX_NEW CPDF_Dictionary;
		if(!pDict)
			break;
		while (1) {
			CFX_ByteString key; CPDF_Object* pElement;
			*this >> key;
			if (key.IsEmpty()) break;
			if(key.Equal(FX_BSTRC("Parent")))
				continue;
			*this >> pElement;
			pDict->SetAt(key, pElement, m_pIndirectObjects);
		}
		pObj = pDict;
		break; }
	case PDFOBJ_STREAM: {
		CPDF_Object* pDict;
		*this >> pDict;
		FX_DWORD size;
		*this >> size;
		FX_LPBYTE pData = FX_Alloc(FX_BYTE,size);
		if(!pData)
			break;
		Read(pData, size);
		pObj = FX_NEW CPDF_Stream(pData, size, (CPDF_Dictionary*)pDict);
		break; 
		}
	case PDFOBJ_REFERENCE: {
		CPDF_Object* pRefObj;

		FX_DWORD dwObjNum;
		*this >> dwObjNum;
		m_LoadingPos -= sizeof(FX_DWORD);

		*this >> pRefObj;
		// The reference object is no meaning in case the referred object is NULL, just ignore it.
		if (pRefObj == NULL) {
			if (m_ObjsNum.Find(dwObjNum) != -1) {
				pObj = FX_NEW CPDF_Reference(m_pIndirectObjects, dwObjNum);
				break;
			}
			pObj = NULL;
		}
		else pObj = FX_NEW CPDF_Reference(m_pIndirectObjects, pRefObj->GetObjNum());
		break; }
	case PDFOBJ_BOOLEAN: {
		FX_DWORD value;
		*this >> value;
		pObj = FX_NEW CPDF_Boolean(value);
		break; }
	case PDFOBJ_NUMBER: {
		int bInteger;
		*this >> bInteger;
		if (bInteger) {
			int value;
			*this >> value;
			pObj = FX_NEW CPDF_Number(value);
		} else {
			FX_FLOAT value;
			*this >> value;
			pObj = FX_NEW CPDF_Number(value);
		}
		break; }
	case PDFOBJ_STRING: {
		CFX_ByteString str;
		*this >> str;
		pObj = FX_NEW CPDF_String(str);
		break; }
	case PDFOBJ_NULL:
		pObj = FX_NEW CPDF_Null;
		break;
	case PDFOBJ_NAME: {
		CFX_ByteString str;
		*this >> str;
		pObj = FX_NEW CPDF_Name(str);
		break; }
	}

	if (objnum && pObj) {
		// Add to indirect object mapping
		m_pIndirectObjects->InsertIndirectObject(objnum, pObj);
	}

	return *this;
}

FS_BOOL	CFS_Archive::Initialize()
{
	if(!m_pSavingBuf)
		m_pSavingBuf = FX_NEW CFX_BinaryBuf;
	if(!m_pSavingBuf)
		return FALSE;

	m_pIndirectObjects = FX_NEW CPDF_IndirectObjects(NULL);
	if(!m_pIndirectObjects)
		return FALSE;

	m_ObjectMap.RemoveAll();
	m_ObjsMap.RemoveAll();
	m_ObjsNum.RemoveAll();

	return TRUE;
}

