#include "../include/fs_pdfobject.h"
#include "../include/fs_pdfdoc.h"

#include "fxcore.h"
#include "fs_util.h"
#include "inc/fs_pdfdocImp.h"
//#include "inc/fs_pdfobject.h"

////////////////////////////////////
// Class FSPDFObject
FS_BOOL _FSPDF_Object_IsType(FSPDFObject* obj, FS_PDFOBJECTTYPE type)
{
	return ((CPDF_Object*)obj)->GetType() == type;
}

FSPDFObject*	FSPDFObject::createFromBoolean(FS_BOOL boolean)
{
	return (FSPDFObject*)(CPDF_Boolean::Create(boolean));
}

FSPDFObject*	FSPDFObject::createFromFloat(FS_FLOAT f)
{
	return (FSPDFObject*)(CPDF_Number::Create(f));
}

FSPDFObject*	FSPDFObject::createFromInteger(FS_INT32 integer)
{
	return (FSPDFObject*)(CPDF_Number::Create(integer));
}

// the same as _FSPDF_Object_CreateUnicodeString in GSDK
FSPDFObject*	FSPDFObject::createFromString(const char* string)
{
	if (FSCRT_Util_IsStringEmpty(string))
		return NULL;

	FX_DWORD len = FXSYS_strlen(string);
	if (!FSCRT_Util_IsUTF8Data(( FX_LPCBYTE)string, &len, NULL))
		return NULL;
	CFX_WideString wsString = CFX_WideString::FromUTF8(string);
	if (wsString.IsEmpty()) 
		return NULL;

	return (FSPDFObject*)CPDF_String::Create(wsString);
}

FSPDFObject*	FSPDFObject::createFromName(const char* name)
{
	if (FSCRT_Util_IsStringEmpty(name))
		return NULL;

	FX_DWORD dwOrgNamelen = FXSYS_strlen(name);
	FX_DWORD dwTempLen = dwOrgNamelen;
	if (!FSCRT_Util_IsUTF8Data(( FX_LPCBYTE)name, &dwTempLen, NULL))
		return NULL;
	CFX_ByteString bsStr = PDF_NameEncode(CFX_ByteString(name, dwOrgNamelen));
	return (FSPDFObject*)CPDF_Name::Create(bsStr);
}

FSPDFObject*	FSPDFObject::createFromDateTime(FSDateTime dt)
{
	if (FALSE == FSCRT_Util_CheckDateTime(dt)) return NULL;
	CFSPDF_DateTime dateTime(dt);
	CFX_ByteString dtStr = dateTime.ToPDFDateTimeString();
	if(dtStr.GetLength() == 0)
		return NULL;
	return (FSPDFObject*)CPDF_String::Create(dtStr);
}

FSPDFObject*	FSPDFObject::createReference(FSPDFDoc* pDoc, FS_DWORD objnum)
{
	if(!pDoc || objnum<1
		|| NULL == pDoc->getIndirectObject(objnum))
		return NULL;

	return (FSPDFObject*)CPDF_Reference::Create(((FSPDFDocImp*)pDoc)->getPDFDocument(), objnum);
}

//////////////////////////////////
// Class FSPDFObject

void FSPDFObject::release()
{
	((CPDF_Object*)this)->Release();
}

FSPDFObject* FSPDFObject::cloneObject() const
{
	return (FSPDFObject*) ((CPDF_Object*)this)->Clone();
}

FS_PDFOBJECTTYPE FSPDFObject::getType() const
{
	return (FS_PDFOBJECTTYPE)((CPDF_Object*)this)->GetType();
}

FS_DWORD FSPDFObject::getObjNum() const
{
	return ((CPDF_Object*)this)->GetObjNum();
}

FS_INT32 FSPDFObject::getInteger() const
{
	if (((CPDF_Object*)this)->GetType() != e_objNumber)
		return -1;

	return ((CPDF_Object*)this)->GetInteger();
}

FS_FLOAT FSPDFObject::getFloat() const
{
	if (((CPDF_Object*)this)->GetType() != e_objNumber)
		return -1;

	return ((CPDF_Object*)this)->GetNumber();
}

FS_BOOL	FSPDFObject::getBoolean() const
{
	if (((CPDF_Object*)this)->GetType() != e_objBoolean)
		return FALSE;

	FX_INT32 iValue = ((CPDF_Object*)this)->GetInteger();
	if (iValue)
		return TRUE;
	else
		return FALSE;
}

FSMatrix FSPDFObject::getMatrix() const
{
	FSMatrix matrix;
	matrix.set(0, 0, 0, 0, 0, 0);
	if (((CPDF_Object*)this)->GetType() != e_objArray || ((CPDF_Array*)((CPDF_Object*)this))->GetCount() != 6)
		return matrix;

	for (FS_INT32 i = 0; i < 6; i++)
	{
		CPDF_Object* pMatrixEle = ((CPDF_Array*)((CPDF_Object*)this))->GetElementValue(i);
		if (pMatrixEle->GetType() != e_objNumber)
			return matrix;

		switch(i)
		{
		case 0:
			matrix.a = pMatrixEle->GetNumber();
			break;
		case 1:
			matrix.b = pMatrixEle->GetNumber();
			break;
		case 2:
			matrix.c = pMatrixEle->GetNumber();
			break;
		case 3:
			matrix.d = pMatrixEle->GetNumber();
			break;
		case 4:
			matrix.e = pMatrixEle->GetNumber();
			break;
		case 5:
			matrix.f = pMatrixEle->GetNumber();
			break;
		default:
			break;
		}
	}
	return matrix;
}

FSRectF	FSPDFObject::getRect() const
{
	FSRectF rectF;
	rectF.set(0, 0, 0, 0);
	if (((CPDF_Object*)this)->GetType() != e_objArray || ((CPDF_Array*)((CPDF_Object*)this))->GetCount() != 4)
		return rectF;

	for (FS_INT32 i = 0; i < 4; i++)
	{
		CPDF_Object* pRectEle = ((CPDF_Array*)((CPDF_Object*)this))->GetElementValue(i);
		if (pRectEle->GetType() != e_objNumber)
			return rectF;

		switch(i)
		{
		case 0:
			rectF.left = pRectEle->GetNumber();
			break;
		case 1:
			rectF.bottom = pRectEle->GetNumber();
			break;
		case 2:
			rectF.right = pRectEle->GetNumber();
			break;
		case 3:
			rectF.top = pRectEle->GetNumber();
			break;
		default:
			break;
		}
	}
	return rectF;
}

FSPDFObject*	FSPDFObject::getDirectObject() const
{
	if (((CPDF_Object*)this)->GetType() != e_objReference)
		return NULL;

	return (FSPDFObject*)((CPDF_Object*)this)->GetDirect();
}

FSDateTime FSPDFObject::getDateTime() const
{
	FSDateTime fsDateTime;
	fsDateTime.set(0, 0, 0, 0, 0, 0, 0, 0, 0);
	if (((CPDF_Object*)this)->GetType() != e_objString)
		return fsDateTime;

	CFX_ByteString bsDateTime = ((CPDF_Object*)this)->GetString();
	if (bsDateTime.IsEmpty())
		return fsDateTime;

	CFSPDF_DateTime dtDateTime;
	if (dtDateTime.ParserPDFDateTimeString(bsDateTime))
		fsDateTime.set(dtDateTime.m_fsDatetime.year, dtDateTime.m_fsDatetime.month, dtDateTime.m_fsDatetime.day, dtDateTime.m_fsDatetime.hour, dtDateTime.m_fsDatetime.minute, dtDateTime.m_fsDatetime.second, 0, dtDateTime.m_fsDatetime.UTHourOffset, dtDateTime.m_fsDatetime.UTMinuteOffset);
	return fsDateTime;
}

FSString FSPDFObject::getString() const
{
	CFX_WideString wsStr = ((CPDF_Object*)this)->GetUnicodeText();
	FSString str = FSString::createFromWideString(wsStr);
	str.toUTF8();
	return str;
}

///////////////////////////////////////////////////////////
// Class FSPDFStream
FSPDFStream*	FSPDFStream::create(FSPDFDictionary* dict)
{
	if (!dict)
	{
		dict = (FSPDFDictionary*)CPDF_Dictionary::Create();
		if (!dict) return NULL;
	}
	else
	{
		if (!_FSPDF_Object_IsType((FSPDFObject*)dict, e_objDictionary))
			return NULL;
	}

	return (FSPDFStream*)CPDF_Stream::Create(NULL, 0, (CPDF_Dictionary*)dict);
}

FSPDFDictionary* FSPDFStream::getDictionary()
{
	if (((CPDF_Object*)this)->GetType() != PDFOBJ_STREAM)
		return NULL;

	return (FSPDFDictionary*)(((CPDF_Stream*)this)->GetDict());
}

FS_DWORD FSPDFStream::getDataSize(FS_BOOL rawData)
{
	if (((CPDF_Object*)this)->GetType() != PDFOBJ_STREAM)
		return 0;

	CPDF_StreamAcc streamAcc;
	streamAcc.LoadAllData((const CPDF_Stream*)this, rawData);

	return streamAcc.GetSize();
}

FS_BOOL FSPDFStream::getData(FS_BOOL rawData, void* buffer, FS_INT32 bufLen)
{
	if (((CPDF_Object*)this)->GetType() != PDFOBJ_STREAM || !buffer || bufLen < 1)
		return FALSE;

	CPDF_StreamAcc streamAcc;
	if (!streamAcc.LoadAllData((const CPDF_Stream*)this, rawData))
		return FALSE;

	FX_DWORD streamSize = streamAcc.GetSize();
	if (buffer && bufLen < streamSize)
		return FALSE;

	FX_LPBYTE streamData = (FX_LPBYTE)streamAcc.DetachData();
	if (NULL == streamData)
		return FALSE;
	FXSYS_memcpy(buffer, streamData, streamSize);
	FX_Free(streamData);

	return TRUE;
}

void FSPDFStream::setData(const void* buffer, FS_DWORD length)
{
	// todo
	if (((CPDF_Object*)this)->GetType() != PDFOBJ_STREAM || !buffer || length < 1)
		return;

	((CPDF_Stream*)this)->InitStream((FX_BYTE*)buffer, length, NULL);
}
 
//////////////////////////////////////////////////
// Class FSPDFArray
FSPDFArray*	FSPDFArray::create()
{
	return (FSPDFArray*)CPDF_Array::Create();
}

FSPDFArray*	FSPDFArray::createFromMatrix(FSMatrix matrix)
{
	CPDF_Array* pNewArray = CPDF_Array::Create();
	if (NULL == pNewArray) return NULL;
	pNewArray->AddNumber(matrix.a);
	pNewArray->AddNumber(matrix.b);
	pNewArray->AddNumber(matrix.c);
	pNewArray->AddNumber(matrix.d);
	pNewArray->AddNumber(matrix.e);
	pNewArray->AddNumber(matrix.f);

	return (FSPDFArray*)pNewArray;
}

FSPDFArray*	FSPDFArray::createFromRect(FSRectF rect)
{
	CPDF_Array* pNewArray = CPDF_Array::Create();
	if (NULL == pNewArray) return NULL;
	pNewArray->AddNumber(rect.left);
	pNewArray->AddNumber(rect.bottom);
	pNewArray->AddNumber(rect.right);
	pNewArray->AddNumber(rect.top);
	return (FSPDFArray*)pNewArray;
}

FS_INT32 FSPDFArray::getElementCount() const
{
	if (((CPDF_Object*)this)->GetType() != PDFOBJ_ARRAY) 
		return -1;

	return ((CPDF_Array*)this)->GetCount();
}

FSPDFObject* FSPDFArray::getElement(FS_INT32 index) const
{
	if (((CPDF_Object*)this)->GetType() != PDFOBJ_ARRAY || index < 0 || index >= getElementCount()) 
		return NULL;

	return (FSPDFObject* )((CPDF_Array*)this)->GetElement(index);
}

void FSPDFArray::addElement(FSPDFObject* element)
{
	if (((CPDF_Object*)this)->GetType() != PDFOBJ_ARRAY || !element ) 
		return;
	((CPDF_Array*)this)->Add((CPDF_Object*)element);
}
 
void FSPDFArray::insertAt(FS_INT32 index, FSPDFObject* element)
{
	if (((CPDF_Object*)this)->GetType() != PDFOBJ_ARRAY || !element)
		return;

	FS_INT32 iCount = getElementCount();
	if (index < 0 || iCount == 0) index = 0;
	if (index > iCount) index = iCount;

	((CPDF_Array*)this)->InsertAt(index, (CPDF_Object*)element);
}
 
void FSPDFArray::setAt(FS_INT32 index, FSPDFObject* element)
{
	if (((CPDF_Object*)this)->GetType() != PDFOBJ_ARRAY || index < 0 || index >= getElementCount()|| !element)
		return;

	((CPDF_Array*)this)->SetAt(index, (CPDF_Object*)element);
}

void FSPDFArray::removeAt(FS_INT32 index)
{
	if (((CPDF_Object*)this)->GetType() != PDFOBJ_ARRAY || index < 0 || index >= getElementCount())
		return;

	((CPDF_Array*)this)->RemoveAt(index);
}

////////////////////////////////////////////////////////////
// Class FSPDFDictionary
FSPDFDictionary* FSPDFDictionary::create()
{
	return (FSPDFDictionary*)CPDF_Dictionary::Create();
}

FS_BOOL	FSPDFDictionary::hasKey(const char* key) const
{
	if (((CPDF_Object*)this)->GetType() != PDFOBJ_DICTIONARY || FSCRT_Util_IsStringEmpty(key)) 
		 return FALSE;

	CFX_ByteString bsKey = PDF_NameEncode(CFX_ByteStringC(key, FXSYS_strlen(key)));
	if (bsKey.IsEmpty())
		return FALSE;
	return ((CPDF_Dictionary*)this)->KeyExist(bsKey);
}

FSPDFObject* FSPDFDictionary::getElement(const char* key) const
{
	if (((CPDF_Object*)this)->GetType() != PDFOBJ_DICTIONARY || FSCRT_Util_IsStringEmpty(key)) 
		return NULL;

	CFX_ByteString bsKey = PDF_NameEncode(CFX_ByteStringC(key, FXSYS_strlen(key)));
	if (bsKey.IsEmpty()) return NULL;
	return (FSPDFObject*)(((CPDF_Dictionary*)this)->GetElement(bsKey));
}

FSString FSPDFDictionary::getKey(FS_POSITION pos) const
{
	if (((CPDF_Object*)this)->GetType() != PDFOBJ_DICTIONARY || !pos) 
		return FSString();
	
	CFX_ByteString bsKey;
	((CPDF_Dictionary*)this)->GetNextElement(pos, bsKey);
	if (bsKey.IsEmpty())
		return FSString();

	CFX_ByteString bsDecodeKey = PDF_NameDecode(bsKey);
	if (bsDecodeKey.IsEmpty())
		return FSString();

	FSString strKey = FSString::createFromByteString(bsDecodeKey);
	strKey.toUTF8();
	return strKey;
}

FSPDFObject* FSPDFDictionary::getValue(FS_POSITION pos) const
{
	if (((CPDF_Object*)this)->GetType() != PDFOBJ_DICTIONARY || !pos) 
		return NULL;

	CFX_ByteString bsKey;
	return (FSPDFObject*)(((CPDF_Dictionary*)this)->GetNextElement(pos, bsKey));
}

FS_POSITION FSPDFDictionary::moveNext(FS_POSITION pos)
{
	if (((CPDF_Object*)this)->GetType() != PDFOBJ_DICTIONARY) 
		return NULL;

	if (!pos)
		return (FS_POSITION)(((CPDF_Dictionary*)this)->GetStartPos());

	CFX_ByteString bsKey;
	((CPDF_Dictionary*)this)->GetNextElement(pos, bsKey);
	return pos;
}

void FSPDFDictionary::setAt(const char* key, FSPDFObject* object)
{
	if (((CPDF_Object*)this)->GetType() != PDFOBJ_DICTIONARY || !object || FSCRT_Util_IsStringEmpty(key))
		return;

	CFX_ByteString bsKey = PDF_NameEncode(CFX_ByteStringC(key, FXSYS_strlen(key)));
	if (bsKey.IsEmpty())
		return;

	((CPDF_Dictionary*)this)->SetAt(bsKey, (CPDF_Object*)object);
}

void FSPDFDictionary::removeAt(const char* key)
{
	if (((CPDF_Object*)this)->GetType() != PDFOBJ_DICTIONARY || FSCRT_Util_IsStringEmpty(key))
		return;

	CFX_ByteString bsKey = PDF_NameEncode(CFX_ByteStringC(key, FXSYS_strlen(key)));
	if (bsKey.IsEmpty())
		return;

	((CPDF_Dictionary*)this)->RemoveAt(bsKey);
}

