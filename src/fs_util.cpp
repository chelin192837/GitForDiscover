#include "../include/common.h"
#include "../include/fs_annot.h"
#include "../include/fs_pdfobject.h"
#include "../include/fs_action.h"
#include "../include/fs_pdfdoc.h"

#include "fxcore.h"
#include "fs_util.h"
#include "fs_pause.h"
#include "inc/fs_pdfdocImp.h"
#include "inc/fs_actionImp.h"
#include "annotation/fs_annot_util.h"
#include "inc/fs_renderImp.h"

#include <time.h>
#include <math.h>

////////////////////////////////////////
// Class CFSCRT_Pause
FX_BOOL CFSCRT_Pause::NeedToPauseNow()
{
	if (m_pPause)
		return m_pPause->needPauseNow();

	return FALSE;
}

////////////////////////////////////////
// CFSRDK_Pause
FS_BOOL CFSRDK_Pause::needPauseNow()
{
	if (!m_pFxPause) return FALSE;
	return m_pFxPause->NeedToPauseNow();
}

////////////////////////////////////////
// Class CFSCRT_Lock
#ifdef _FPDFAPI_MT_
FX_BOOL CFSCRT_Lock::TryLock()
{
#ifdef FSCRT_MUTEX
	return FX_Mutex_TryLock(&m_hLock);
#else
	return FX_SpinLock_TryLock(&m_hSpinLock);
#endif
}

void CFSCRT_Lock::Lock()
{
#ifdef FSCRT_MUTEX
	FX_Mutex_Lock(&m_hLock);
#else
	FX_SpinLock_Lock(&m_hSpinLock);
#endif
}

void CFSCRT_Lock::Unlock()
{
#ifdef FSCRT_MUTEX
	FX_Mutex_Unlock(&m_hLock);
#else
	FX_SpinLock_Unlock(&m_hSpinLock);
#endif
}

CFSCRT_Lock::CFSCRT_Lock()
{
#ifdef FSCRT_MUTEX
	FX_Mutex_Initialize(&m_hLock);
#else
	FX_SpinLock_Initialize(&m_hSpinLock);
#endif
}

CFSCRT_Lock::~CFSCRT_Lock()
{
#ifdef FSCRT_MUTEX
	FX_Mutex_Destroy(&m_hLock);
#else
	FX_SpinLock_Destroy(&m_hSpinLock);
#endif
}
#endif //_FPDFAPI_MT_

#ifdef _FPDFAPI_MT_
CFSCRT_LockObject::CFSCRT_LockObject(CFSCRT_Lock *pObj) : m_pObj(pObj)
{
	if (m_pObj) FSCRT_LOCKPTR(m_pObj)
}

CFSCRT_LockObject::~CFSCRT_LockObject()
{
	if (m_pObj) FSCRT_UNLOCKPTR(m_pObj);
}
#endif

//*****************************************************************************
//* Detect whether a data buffer is UTF-8 encoded or not 
//*****************************************************************************
FX_BOOL FSCRT_Util_IsUTF8Data(FX_LPCBYTE pData, FX_DWORD *pLen, FX_DWORD *pPos)
{
	if (!pData || !pLen) return FALSE;

	FX_DWORD iLen = *pLen;
	if (iLen < 1) return TRUE;
	*pLen = 0;

	FX_DWORD dwCode;
	FX_DWORD i, j, k = 0, iPending = 0;
	FX_BYTE b;
	if (pPos != NULL)
	{
		*pPos = -1;
	}
	while (iLen > 0)
	{
		if (iLen > 3)
		{
			dwCode = *((FX_DWORD const *)pData);
			pData += 4;
			iLen -= 4;
			j = 4;
		}
		else
		{
			dwCode = 0;
			for (i = 0; i < iLen; i ++)
			{
				j = (FX_BYTE)*pData ++;
				j <<= i * 8;
				dwCode |= j;
			}
			j = iLen;
			iLen = 0;
		}

		i = 0;
		while (i ++ < j)
		{
			b = (FX_BYTE)dwCode;
			dwCode >>= 8;
			k ++;
			if (pPos != NULL)
			{
				++*pPos;
			}

			if (0 == iPending)
			{
				if (b < 0x80)
					continue;
				else if (b < 0xE0)
				{
					if ((b & 0xE0) != 0xC0) return FALSE;

					iPending = 1;
				}
				else if (b < 0xF0)
				{
					if ((b & 0xF0) != 0xE0) return FALSE;

					iPending = 2;
				}
				else if (b < 0xF8)
				{
					if ((b & 0xF8) != 0xF0) return FALSE;

					iPending = 3;
				}
				else if (b < 0xFC)
				{
					if ((b & 0xFC) != 0xF8) return FALSE;

					iPending = 4;
				}
				else //if (b < 0xFE)
				{
					if ((b & 0xFE) != 0xFC) return FALSE;

					iPending = 5;
				}
			}
			else
			{
				if ((b & 0xC0) != 0x80) return FALSE;

				iPending --;
			}
			if (0 == iPending)
			{
				*pLen += k;
				k = 0;
			}
		}
	}
	if (iPending > 0)
	{
		return FALSE;
	}
	if (pPos != NULL)
	{
		*pPos=-1;
	}

	return TRUE;
}

//*****************************************************************************
//* Decode UTF-8 string into Unicode string
//*****************************************************************************
template <class T>
FX_INT32 _FSCRT_UTF8_DecodeToUnicode(FX_LPCSTR pSrc, FX_DWORD pSrcLen, T *pDst, FX_DWORD *pDstLen)
{
	if (pDstLen == NULL) return -1;
	if (pSrcLen < 1)
	{
		*pDstLen = 0;
		return 1;
	}

	FX_LPCSTR pOriSrc = pSrc;
	FS_INT32 i = 0, iPending = 0;
	FS_INT32 utf8Length = 0;
	FX_BYTE b;
	FS_DWORD wUnicode = 0;
	while (i ++ < pSrcLen)
	{
		b = (FX_BYTE)*pOriSrc ++;
		if (iPending == 0)
		{
			if (b < 0x80)
				wUnicode = (FS_DWORD)b;
			else if (b < 0xE0)
				wUnicode = (b & 0x1f), iPending = 1;
			else if (b < 0xF0)
				wUnicode = (b & 0x0f), iPending = 2;
			else if (b < 0xF8)
				wUnicode = (b & 0x07), iPending = 3;
			else if (b < 0xFC)
				wUnicode = (b & 0x03), iPending = 4;
			else //if (b < 0xFE)
				wUnicode = (b & 0x01), iPending = 5;
		}
		else
		{
			b &= 0x3f;
			wUnicode <<= 6;
			wUnicode |= b;
			iPending --;
		}
		if (iPending == 0)
		{
			if(sizeof(T) == 4)
			{
				if(pDst && utf8Length < (FS_INT32)*pDstLen)
					*pDst ++ = wUnicode;
				utf8Length += 1;
			} else if (sizeof(T) == 2)
			{
				if (wUnicode < 0x10000)
				{
					if(pDst && utf8Length < (FS_INT32)*pDstLen)
						*pDst ++ = wUnicode;
					utf8Length += 1;
				} else 
				{
					wUnicode -= 0x10000;
					if (wUnicode > 0xfffff) continue;		// unsupported in UTF-16
					// break the 20-bit code into two words, each uses 10bits
					if(pDst && utf8Length + 2 <= (FS_INT32)*pDstLen)
					{
						*pDst ++ = 0xD800 | (wUnicode >> 10);
						*pDst ++ = 0xDC00 | (wUnicode & 0x3ff);
					}
					utf8Length += 1;
					utf8Length += 1;
				}
			}

		}
	}
	if(!pDst)*pDstLen = utf8Length;
	*pDstLen = utf8Length > *pDstLen ? *pDstLen : utf8Length;
	return 1;
}

FX_LPWSTR FSCRT_UTF8_DecodeToUnicode(const char* utf8Str)
{
	if (FSCRT_Util_IsStringEmpty(utf8Str))
		return NULL;
	FX_DWORD len = FXSYS_strlen(utf8Str);
	if (!FSCRT_Util_IsUTF8Data((FX_LPCBYTE)utf8Str, &len, NULL)) return NULL;

	FX_DWORD dLen = 0;
	FX_LPWSTR unicodeName = NULL;
	_FSCRT_UTF8_DecodeToUnicode((FX_LPSTR)utf8Str, FXSYS_strlen(utf8Str), (FX_LPWSTR)NULL, &dLen);
	unicodeName = (FX_LPWSTR)FX_Alloc(FX_WCHAR, (++dLen) * sizeof(FX_WCHAR));
	if (NULL == unicodeName)
		return NULL;

	FXSYS_memset32((FX_LPVOID)unicodeName, 0, dLen * sizeof(FX_WCHAR));
	_FSCRT_UTF8_DecodeToUnicode((FX_LPSTR)utf8Str, FXSYS_strlen(utf8Str), unicodeName, &dLen);

	return unicodeName;
}

CFX_ByteString FSCRT_UTF8_FromTextString(const CFX_ByteString bsText)
{
	CFX_WideString wsValue = PDF_DecodeText(bsText);

	return FX_UTF8Encode(wsValue);
}

CFX_ByteString FSCRT_UTF8_ToTextString(const CFX_ByteString bsUtf8)
{
	CFX_WideString wsData = CFX_WideString::FromUTF8(bsUtf8);
	
	return PDF_EncodeText((FX_LPCWSTR)wsData, wsData.GetLength());
}

CFX_WideString FSCRT_UTF8_ToWideString(const char* utf8String)
{
	if (!utf8String || FXSYS_strlen(utf8String)<1)
		return CFX_WideString();
	return FSCRT_UTF8_ToWideString(utf8String, FXSYS_strlen(utf8String));
}

CFX_WideString FSCRT_UTF8_ToWideString(const char utf8String[], FS_INT32 sLen)
{
	if (!utf8String || sLen<1) return CFX_WideString();
	CFX_WideString wsRet;
#if _FX_OS_ == _FX_MACOSX_ || _FX_OS_==_FX_IOS_
	// Transplanted from GSDK CFSCRT_LTPDFAnnot::ST_SetString and FSPDF_UTF8ToWideString(), for fix SDK-7161.
	CFStringRef str = CFStringCreateWithBytes(kCFAllocatorDefault, (UInt8*)utf8String, (CFIndex)sLen, kCFStringEncodingUTF8, false);

	CFMutableStringRef msString = CFStringCreateMutableCopy(kCFAllocatorDefault, 0, str);
	CFStringNormalize(msString, kCFStringNormalizationFormC);

	CFIndex strLen = CFStringGetLength(msString);
	UniChar *buffer = FX_Alloc(UniChar, strLen);
	if(!buffer) return wsRet;
	CFStringGetCharacters(msString, CFRangeMake(0, strLen), buffer);

	wsRet = CFX_WideString::FromUTF16LE((const unsigned short*)buffer, strLen);
	FX_Free(buffer);
    CFRelease(str);
    CFRelease(msString);
#else
	wsRet = CFX_WideString::FromUTF8(utf8String, sLen);
#endif
	return wsRet;
}

CFX_ByteString FSCRT_UTF8_FromWideString(CFX_WideString wsStr)
{
	if (wsStr.IsEmpty()) return CFX_ByteString();
	return FX_UTF8Encode(wsStr);
}

///////////////////////////////////////////////////////////////////////////////////////
IFX_FileStream* FSCRT_Util_CreateFileFromPath(const char* filePath, FX_DWORD dwModes)
{
	if (NULL == filePath || FXSYS_strlen(filePath) < 5)
		return NULL;
	IFX_FileStream* fileStream  = NULL;
	FX_LPWSTR unicodeName = FSCRT_UTF8_DecodeToUnicode(filePath);
	CFX_WideString wStr = unicodeName;
	if (0 == wStr.Right(4).CompareNoCase(L".pdf"))
		fileStream = FX_CreateFileStream(unicodeName, dwModes);
	FX_Free((FX_LPVOID)unicodeName);
	return fileStream;
}

//CFX_ByteString FSCRT_Util_ToPDFDateTimeString(FSDateTime dt)
//{
//	CFX_ByteString dtStr;
//	CFX_ByteString tempStr;
//
//	tempStr.Format("D:%04d%02d%02d%02d%02d%02d", dt.year, dt.month, dt.day, dt.hour, dt.minute, dt.second);
//	dtStr = CFX_ByteString(tempStr);
//
//	if (dt.UTHourOffset < 0)
//		dtStr += CFX_ByteString("-");
//	else
//		dtStr += CFX_ByteString("+");
//
//	tempStr = "";
//	tempStr.Format("%02d'%02d'", FXSYS_abs(dt.UTHourOffset), dt.UTMinuteOffset);
//
//	dtStr += CFX_ByteString(tempStr);
//	return dtStr;
//}
CFX_PointF FSPDF_Util_FSPointF2CFXPointF(const FSPointF& fsPoint)
{
	CFX_PointF fxPoint;
	fxPoint.Set(fsPoint.x, fsPoint.y);
	return fxPoint;
}

FSPointF FSPDF_Util_CFXPointF2FSPointF(const CFX_PointF& fxPoint)
{
	FSPointF fsPoint;
	fsPoint.set(fxPoint.x, fxPoint.y);
	return fsPoint;
}

FS_BOOL FSPDF_Util_CheckAnnotIsStandard(FSM_ANNOTTYPE bsType)
{
	if (bsType < FSAnnot::e_annotNote || bsType > FSAnnot::e_annot3D || FSAnnot::e_annotPSInk == bsType) 
		return FALSE;
	
	return TRUE;
}

FS_BOOL FSPDF_Util_CheckAnnotIsStandard(CFX_ByteString bsAnnotType)
{
	
	FSM_ANNOTTYPE mAnnotType = FSANNOT_Util_ConvertAnnotTypeFromStrToEnum(bsAnnotType);
	return FSPDF_Util_CheckAnnotIsStandard(mAnnotType);

}

FS_BOOL FSPDF_Util_CheckShouldRenderAnnot(CPDF_Annot* pAnnot)
{
	if (!pAnnot) return FALSE;

	CFX_ByteString bsType = pAnnot->GetSubType();
	FSM_ANNOTTYPE annotType = FSANNOT_Util_ConvertAnnotTypeFromStrToEnum(bsType);
	FS_BOOL bStandardAnnot = FSPDF_Util_CheckAnnotIsStandard(annotType);

	FX_DWORD annot_flags = pAnnot->GetFlags();
	if (annot_flags & ANNOTFLAG_HIDDEN) 
		return FALSE;

	//TODO: for print.
	//...

	if ((annot_flags & ANNOTFLAG_NOVIEW)) return FALSE; // no-view annot
	if (!bStandardAnnot && (annot_flags & ANNOTFLAG_INVISIBLE))
		return FALSE; // invisible annot is only effective to non-standard annotation. 

	return TRUE;
}

FS_BOOL FSCRT_Util_IsFloatRectEqual(CFX_FloatRect rect1, CFX_FloatRect rect2)
{
	if (FXSYS_fabs(rect1.left-rect2.left) < FLOAT_ZERO_CMP
		&& FXSYS_fabs(rect1.top-rect2.top) < FLOAT_ZERO_CMP
		&& FXSYS_fabs(rect1.right-rect2.right) < FLOAT_ZERO_CMP
		&& FXSYS_fabs(rect1.bottom-rect2.bottom) < FLOAT_ZERO_CMP)
		return TRUE;
	else
		return FALSE;
}

FS_BOOL FSCRT_Util_IsFloatPointEqual(FSPointF point1, FSPointF point2)
{
	if (FXSYS_fabs(point1.x-point2.x) < FLOAT_ZERO_CMP
		&& FXSYS_fabs(point1.y-point2.y) < FLOAT_ZERO_CMP
		)
		return TRUE;
	else
		return FALSE;
}


CPDF_Object* FSPDF_ST_CloneObject2Doc(CPDF_Object* pSrcObj , CPDF_IndirectObjects* pTargetDoc ,FX_BOOL bCloneAlways, CFX_MapPtrToPtr &MergeObjs, FX_DWORD& TargetDoclastObjNum)
{
	if (!pSrcObj || !pTargetDoc) return NULL;
	if(!TargetDoclastObjNum) TargetDoclastObjNum = pTargetDoc->GetLastObjNum();

	switch (pSrcObj->GetType())
	{
	case PDFOBJ_BOOLEAN:
	case PDFOBJ_NUMBER:
	case PDFOBJ_STRING:
	case PDFOBJ_NAME:
	case PDFOBJ_NULL:
		return pSrcObj->Clone(FALSE);
	case PDFOBJ_ARRAY:
		{
			CPDF_Array* pNewArray = CPDF_Array::Create();
			if (pNewArray)
			{
				FX_DWORD nCount = ((CPDF_Array*)pSrcObj)->GetCount();
				for (FX_DWORD n = 0; n < nCount; n ++)
				{
					CPDF_Object* pElement = ((CPDF_Array*)pSrcObj)->GetElement(n);
					CPDF_Object* pClone = FSPDF_ST_CloneObject2Doc(pElement, pTargetDoc, bCloneAlways, MergeObjs, TargetDoclastObjNum);
					if (pClone) pNewArray->Add(pClone);
				}
			}
			return pNewArray;
		}
	case PDFOBJ_DICTIONARY:
		{
			CPDF_Dictionary* pNewDict = CPDF_Dictionary::Create();
			if (pNewDict)
			{
				FX_POSITION pos = ((CPDF_Dictionary*)pSrcObj)->GetStartPos();
				while (pos)
				{
					CFX_ByteString key;
					CPDF_Object* pValue = ((CPDF_Dictionary*)pSrcObj)->GetNextElement(pos, key);
					CPDF_Object* pClone = FSPDF_ST_CloneObject2Doc(pValue, pTargetDoc, bCloneAlways, MergeObjs, TargetDoclastObjNum);
					if (pClone) pNewDict->SetAt(key, pClone);
				}
			}
			return pNewDict;
		}
	case PDFOBJ_STREAM:
		{
			CPDF_Dictionary* pNewDict = (CPDF_Dictionary*)FSPDF_ST_CloneObject2Doc(((CPDF_Stream*)pSrcObj)->GetDict(), pTargetDoc, bCloneAlways, MergeObjs, TargetDoclastObjNum);
			if (!pNewDict) return NULL;
			CPDF_StreamAcc acc;
			acc.LoadAllData((CPDF_Stream*)pSrcObj, TRUE);
			FX_DWORD size = acc.GetSize();
			if (size < 1) {
				pNewDict->Release();
				return NULL;
			}
			else
				return CPDF_Stream::Create(acc.DetachData(), size, pNewDict);
		}
	case PDFOBJ_REFERENCE:
		{
			FX_DWORD dwSrcNum = ((CPDF_Reference*)pSrcObj)->GetRefObjNum();
			FX_DWORD dwDstNum = 0;
			void *pdwDstNum = NULL;
			if (!bCloneAlways)
			{
				if(MergeObjs.Lookup((void*)dwSrcNum, (void*&)pdwDstNum) && pdwDstNum)
				{
					dwDstNum = (FX_DWORD)(FX_INTPTR)pdwDstNum;
				}
			}

			if (!dwDstNum)
			{
				CPDF_Object* pSrcDirectObj = pSrcObj->GetDirect();
				dwDstNum = ++TargetDoclastObjNum;

				if (!bCloneAlways)
					MergeObjs.SetAt((void*)dwSrcNum, (void*)dwDstNum);
				CPDF_Object* pDstDirectObj = FSPDF_ST_CloneObject2Doc(pSrcDirectObj, pTargetDoc, bCloneAlways, MergeObjs, TargetDoclastObjNum);

				if (pDstDirectObj && NULL == pTargetDoc->GetIndirectObject(dwDstNum))
				{
					pTargetDoc->InsertIndirectObject(dwDstNum,pDstDirectObj);
					pDstDirectObj->SetModified(TRUE);
				}
				else
				{
					if (!bCloneAlways)
					{
						MergeObjs.RemoveKey((void*)dwSrcNum);
					}
					return NULL;
				}
			}
			return CPDF_Reference::Create(pTargetDoc, dwDstNum);
		}
		break;
	}
	return NULL;
}

FS_BOOL FSCRT_Util_CheckDateTime(const FSDateTime fsDateTime)
{
	if(fsDateTime.month<1 || fsDateTime.month > 12 //unsigned, from 1 to 12
		||	fsDateTime.day<1 || fsDateTime.day > 31 // unsigned, from 1 to 31
		||	fsDateTime.hour > 23//unsigned, from 0 to 23
		||	fsDateTime.minute > 59//unsigned, from 0 to 59
		||  fsDateTime.milliseconds > 999 //unsigned short, from 0 to 999
		||	fsDateTime.second > 60//unsigned, from 0 to 60-> 60 for leap second
		||	fsDateTime.UTHourOffset < -12 || fsDateTime.UTHourOffset > 12//from -12 to 12
		||	fsDateTime.UTMinuteOffset > 59)//unsigned, from 0 to 59
		return FALSE;
	else 
		return TRUE;
}

FS_BOOL		FSCRT_Util_IsStringEmpty(const char* inputStr)
{
	if (!inputStr || FXSYS_strlen(inputStr)<1)
		return TRUE;
	return FALSE;
}

FS_INT32	FSCRT_Util_GetWatermarkAnnotType(CPDF_Dictionary* pAnnotDict)
{
	if (!pAnnotDict) return WMANNOTTYPE_NO;

	CFX_ByteStringC bsSubType = pAnnotDict->GetConstString(FX_BSTRC("Subtype"));
	if(bsSubType != FX_BSTRC("Watermark"))	return WMANNOTTYPE_NO;

	CPDF_Dictionary* pAPDict = pAnnotDict->GetDict(FX_BSTRC("AP"));
	if (NULL == pAPDict)	return WMANNOTTYPE_COMMON;

	CPDF_Object* pNObject = pAPDict->GetElement(FX_BSTRC("N"));
	if (pNObject->GetType() != PDFOBJ_REFERENCE)	return WMANNOTTYPE_COMMON;

	CPDF_Dictionary* pResourcesDict = (((CPDF_Reference*)pNObject)->GetDict())->GetDict(FX_BSTRC("Resources"));
	if (NULL == pResourcesDict)		return WMANNOTTYPE_COMMON;

	CPDF_Dictionary* pXObject = pResourcesDict->GetDict(FX_BSTRC("XObject"));
	if (NULL == pXObject)	return WMANNOTTYPE_COMMON;

	FX_POSITION pos = pXObject->GetStartPos();
	CFX_ByteString bsKey;
	FS_INT32 iWMAnnotType = WMANNOTTYPE_COMMON;
	while (pos)
	{
		CPDF_Dictionary* pSubXObjectDict = (CPDF_Dictionary*)pXObject->GetNextElement(pos, bsKey);
		if (NULL == pSubXObjectDict)	break;

		if ( PDFOBJ_REFERENCE != pSubXObjectDict->GetType())	continue;

		CPDF_Dictionary* pPieceInfoDict = ((CPDF_Reference*)pSubXObjectDict)->GetDict()->GetDict(FX_BSTRC("PieceInfo"));
		if (NULL == pPieceInfoDict)		continue;

		CPDF_Dictionary* pAppDataDict = pPieceInfoDict->GetDict(FX_BSTRC("ADBE_CompoundType"));
		if (NULL == pAppDataDict)		continue;

		CFX_ByteString bsPrivateData = pAppDataDict->GetString(FX_BSTRC("Private"));
		if (bsPrivateData.Equal(FX_BSTRC(FSDK_TRIALWATERMARK_GSDK)))
		{
			iWMAnnotType = WMANNOTTYPE_GSDKTRIAL;
			break;
		}
		else if (bsPrivateData.Equal(FX_BSTRC(FSDK_TRIALWATERMARK_RDK)))
		{
			iWMAnnotType = WMANNOTTYPE_RDKTRIAL;
			break;
		}
	}
	return iWMAnnotType;
}

FS_BOOL		FSCRT_Util_RDKTrialWatermarkExisted(CPDF_Page* pPage)
{
	if (!pPage) return FALSE;

	CPDF_AnnotList annotList(pPage);
	FS_INT32 iAnnotCount = annotList.Count();
	// RDK trial watermark is always added to the end of annot array.
	// So find from the end of annot array.
	for (FS_INT32 i=iAnnotCount-1; i>=0; i--)
	{
		CPDF_Annot* pAnnot = annotList.GetAt(i);
		if (WMANNOTTYPE_RDKTRIAL == FSCRT_Util_GetWatermarkAnnotType(pAnnot->m_pAnnotDict))
			return TRUE;
	}
	return FALSE;
}


// Unicode subset bitfield table
static const FSCRT_FONTUSB g_FSFontUSBTable[] = { 
	{0x0000  ,	0x007F  ,	0  ,	1252  },	// Basic Latin                                 
	{0x0080  ,	0x00FF  ,	1  ,	1252  },	// Latin-1 Supplement
	{0x0100  ,	0x017F  ,	2  ,	1250  },	// Latin Extended-A
	{0x0180  ,	0x024F  ,	3  ,	1250  },	// Latin Extended-B
	{0x0250  ,	0x02AF  ,	4  ,	0xFFFF},	// IPA Extensions
	{0x02B0  ,	0x02FF  ,	5  ,	0xFFFF},	// Spacing Modifier Letters
	{0x0300  ,	0x036F  ,	6  ,	0xFFFF},	// Combining Diacritical Marks
	{0x0370  ,	0x03FF  ,	7  ,	1253  },	// Greek and Coptic
	{0x0400  ,	0x04FF  ,	9  ,	1251  },	// Cyrillic
	{0x0500  ,	0x052F  ,	9  ,	0xFFFF},	// Cyrillic Supplement
	{0x0530  ,	0x058F  ,	10 ,	0xFFFF},	// Armenian
	{0x0590  ,	0x05FF  ,	11 ,	1255  },	// Hebrew
	{0x0600  ,	0x06FF  ,	13 ,	1256  },	// Arabic
	{0x0700  ,	0x074F  ,	71 ,	0xFFFF},	// Syriac
	{0x0750  ,	0x077F  ,	13 ,	0xFFFF},	// Arabic Supplement
	{0x0780  ,	0x07BF  ,	72 ,	0xFFFF},	// Thaana
	{0x07C0  ,	0x07FF  ,	14 ,	0xFFFF},	// NKo
	{0x0800  ,	0x08FF  ,	999,	0xFFFF},	// 
	{0x0900  ,	0x097F  ,	15 ,	0xFFFF},	// Devanagari
	{0x0980  ,	0x09FF  ,	16 ,	0xFFFF},	// Bengali
	{0x0A00  ,	0x0A7F  ,	17 ,	0xFFFF},	// Gurmukhi
	{0x0A80  ,	0x0AFF  ,	18 ,	0xFFFF},	// Gujarati
	{0x0B00  ,	0x0B7F  ,	19 ,	0xFFFF},	// Oriya
	{0x0B80  ,	0x0BFF  ,	20 ,	0xFFFF},	// Tamil
	{0x0C00  ,	0x0C7F  ,	21 ,	0xFFFF},	// Telugu
	{0x0C80  ,	0x0CFF  ,	22 ,	0xFFFF},	// Kannada
	{0x0D00  ,	0x0D7F  ,	23 ,	0xFFFF},	// Malayalam
	{0x0D80  ,	0x0DFF  ,	73 ,	0xFFFF},	// Sinhala
	{0x0E00  ,	0x0E7F  ,	24 ,	874   },	// Thai
	{0x0E80  ,	0x0EFF  ,	25 ,	0xFFFF},	// Lao
	{0x0F00  ,	0x0FFF  ,	70 ,	0xFFFF},	// Tibetan
	{0x1000  ,	0x109F  ,	74 ,	0xFFFF},	// Myanmar
	{0x10A0  ,	0x10FF  ,	26 ,	0xFFFF},	// Georgian
	{0x1100  ,	0x11FF  ,	28 ,	0xFFFF},	// Hangul Jamo
	{0x1200  ,	0x137F  ,	75 ,	0xFFFF},	// Ethiopic
	{0x1380  ,	0x139F  ,	75 ,	0xFFFF},	// Ethiopic Supplement
	{0x13A0  ,	0x13FF  ,	76 ,	0xFFFF},	// Cherokee
	{0x1400  ,	0x167F  ,	77 ,	0xFFFF},	// Unified Canadian Aboriginal Syllabics
	{0x1680  ,	0x169F  ,	78 ,	0xFFFF},	// Ogham
	{0x16A0  ,	0x16FF  ,	79 ,	0xFFFF},	// Runic
	{0x1700  ,	0x171F  ,	84 ,	0xFFFF},	// Tagalog
	{0x1720  ,	0x173F  ,	84 ,	0xFFFF},	// Hanunoo
	{0x1740  ,	0x175F  ,	84 ,	0xFFFF},	// Buhid
	{0x1760  ,	0x177F  ,	84 ,	0xFFFF},	// Tagbanwa
	{0x1780  ,	0x17FF  ,	80 ,	0xFFFF},	// Khmer
	{0x1800  ,	0x18AF  ,	81 ,	0xFFFF},	// Mongolian
	{0x18B0  ,	0x18FF  ,	999,	0xFFFF},	// 
	{0x1900  ,	0x194F  ,	93 ,	0xFFFF},	// Limbu
	{0x1950  ,	0x197F  ,	94 ,	0xFFFF},	// Tai Le
	{0x1980  ,	0x19DF  ,	95 ,	0xFFFF},	// New Tai Lue
	{0x19E0  ,	0x19FF  ,	80 ,	0xFFFF},	// Khmer Symbols
	{0x1A00  ,	0x1A1F  ,	96 ,	0xFFFF},	// Buginese
	{0x1A20  ,	0x1AFF  ,	999,	0xFFFF},	// 
	{0x1B00  ,	0x1B7F  ,	27 ,	0xFFFF},	// Balinese
	{0x1B80  ,	0x1BBF  ,	112,	0xFFFF},	// Sundanese
	{0x1BC0  ,	0x1BFF  ,	999,	0xFFFF},	// 
	{0x1C00  ,	0x1C4F  ,	113,	0xFFFF},	// Lepcha
	{0x1C50  ,	0x1C7F  ,	114,	0xFFFF},	// Ol Chiki
	{0x1C80  ,	0x1CFF  ,	999,	0xFFFF},	// 
	{0x1D00  ,	0x1D7F  ,	4  ,	0xFFFF},	// Phonetic Extensions
	{0x1D80  ,	0x1DBF  ,	4  ,	0xFFFF},	// Phonetic Extensions Supplement
	{0x1DC0  ,	0x1DFF  ,	6  ,	0xFFFF},	// Combining Diacritical Marks Supplement
	{0x1E00  ,	0x1EFF  ,	29 ,	0xFFFF},	// Latin Extended Additional
	{0x1F00  ,	0x1FFF  ,	30 ,	0xFFFF},	// Greek Extended
	{0x2000  ,	0x206F  ,	31 ,	0xFFFF},	// General Punctuation
	{0x2070  ,	0x209F  ,	32 ,	0xFFFF},	// Superscripts And Subscripts
	{0x20A0  ,	0x20CF  ,	33 ,	0xFFFF},	// Currency Symbols
	{0x20D0  ,	0x20FF  ,	34 ,	0xFFFF},	// Combining Diacritical Marks For Symbols
	{0x2100  ,	0x214F  ,	35 ,	0xFFFF},	// Letterlike Symbols
	{0x2150  ,	0x215F  ,	36 ,	0xFFFF},	// Number Forms
	{0x2160  ,	0x216B  ,	36 ,	936   },	// Number Forms
	{0x216C  ,	0x216F  ,	36 ,	0xFFFF},	// Number Forms
	{0x2170  ,	0x2179  ,	36 ,	936   },	// Number Forms
	{0x217A  ,	0x218F  ,	36 ,	0xFFFF},	// Number Forms
	{0x2190  ,	0x2199  ,	37 ,	949   },	// Arrows
	{0x219A  ,	0x21FF  ,	37 ,	0xFFFF},	// Arrows
	{0x2200  ,	0x22FF  ,	38 ,	0xFFFF},	// Mathematical Operators
	{0x2300  ,	0x23FF  ,	39 ,	0xFFFF},	// Miscellaneous Technical
	{0x2400  ,	0x243F  ,	40 ,	0xFFFF},	// Control Pictures
	{0x2440  ,	0x245F  ,	41 ,	0xFFFF},	// Optical Character Recognition
	{0x2460  ,	0x2473  ,	42 ,	932   },	// Enclosed Alphanumerics
	{0x2474  ,	0x249B  ,	42 ,	936   },	// Enclosed Alphanumerics
	{0x249C  ,	0x24E9  ,	42 ,	949   },	// Enclosed Alphanumerics
	{0x24EA  ,	0x24FF  ,	42 ,	0xFFFF},	// Enclosed Alphanumerics
	{0x2500  ,	0x2573  ,	43 ,	936   },	// Box Drawing
	{0x2574  ,	0x257F  ,	43 ,	0xFFFF},	// Box Drawing
	{0x2580  ,	0x2580  ,	44 ,	0xFFFF},	// Block Elements
	{0x2581  ,	0x258F  ,	44 ,	936   },	// Block Elements
	{0x2590  ,	0x259F  ,	44 ,	0xFFFF},	// Block Elements
	{0x25A0  ,	0x25FF  ,	45 ,	936   },	// Geometric Shapes
	{0x2600  ,	0x26FF  ,	46 ,	0xFFFF},	// Miscellaneous Symbols
	{0x2700  ,	0x27BF  ,	47 ,	0xFFFF},	// Dingbats
	{0x27C0  ,	0x27EF  ,	38 ,	0xFFFF},	// Miscellaneous Mathematical Symbols-A
	{0x27F0  ,	0x27FF  ,	37 ,	0xFFFF},	// Supplemental Arrows-A
	{0x2800  ,	0x28FF  ,	82 ,	0xFFFF},	// Braille Patterns
	{0x2900  ,	0x297F  ,	37 ,	0xFFFF},	// Supplemental Arrows-B
	{0x2980  ,	0x29FF  ,	38 ,	0xFFFF},	// Miscellaneous Mathematical Symbols-B
	{0x2A00  ,	0x2AFF  ,	38 ,	0xFFFF},	// Supplemental Mathematical Operators
	{0x2B00  ,	0x2BFF  ,	37 ,	0xFFFF},	// Miscellaneous Symbols and Arrows
	{0x2C00  ,	0x2C5F  ,	97 ,	0xFFFF},	// Glagolitic
	{0x2C60  ,	0x2C7F  ,	29 ,	0xFFFF},	// Latin Extended-C
	{0x2C80  ,	0x2CFF  ,	8  ,	0xFFFF},	// Coptic
	{0x2D00  ,	0x2D2F  ,	26 ,	0xFFFF},	// Georgian Supplement
	{0x2D30  ,	0x2D7F  ,	98 ,	0xFFFF},	// Tifinagh
	{0x2D80  ,	0x2DDF  ,	75 ,	0xFFFF},	// Ethiopic Extended
	{0x2DE0  ,	0x2DFF  ,	9  ,	0xFFFF},	// Cyrillic Extended-A
	{0x2E00  ,	0x2E7F  ,	31 ,	0xFFFF},	// Supplemental Punctuation
	{0x2E80  ,	0x2EFF  ,	59 ,	0xFFFF},	// CJK Radicals Supplement
	{0x2F00  ,	0x2FDF  ,	59 ,	0xFFFF},	// Kangxi Radicals
	{0x2FE0  ,	0x2FEF  ,	999,	0xFFFF},	// 
	{0x2FF0  ,	0x2FFF  ,	59 ,	0xFFFF},	// Ideographic Description Characters
	{0x3000  ,	0x303F  ,	48 ,	936   },	// CJK Symbols And Punctuation
	{0x3040  ,	0x309F  ,	49 ,	932   },	// Hiragana
	{0x30A0  ,	0x30FF  ,	50 ,	932   },	// Katakana
	{0x3100  ,	0x3129  ,	51 ,	936   },	// Bopomofo
	{0x312A  ,	0x312F  ,	51 ,	936   },	// Bopomofo
	{0x3130  ,	0x318F  ,	52 ,	949   },	// Hangul Compatibility Jamo
	{0x3190  ,	0x319F  ,	59 ,	0xFFFF},	// Kanbun
	{0x31A0  ,	0x31BF  ,	51 ,	936   },	// Bopomofo Extended
	{0x31C0  ,	0x31EF  ,	61 ,	0xFFFF},	// CJK Strokes
	{0x31F0  ,	0x31FF  ,	50 ,	0xFFFF},	// Katakana Phonetic Extensions
	{0x3200  ,	0x321C  ,	54 ,	949   },	// Enclosed CJK Letters And Months
	{0x321D  ,	0x325F  ,	54 ,	0xFFFF},	// Enclosed CJK Letters And Months
	{0x3260  ,	0x327F  ,	54 ,	949   },	// Enclosed CJK Letters And Months
	{0x3280  ,	0x32FF  ,	54 ,	0xFFFF},	// Enclosed CJK Letters And Months
	{0x3300  ,	0x3387  ,	55 ,	0xFFFF},	// CJK Compatibility
	{0x3388  ,	0x33D0  ,	55 ,	949   },	// CJK Compatibility
	{0x33D1  ,	0x33FF  ,	55 ,	0xFFFF},	// CJK Compatibility
	{0x3400  ,	0x4DBF  ,	59 ,	0xFFFF},	// CJK Unified Ideographs Extension A
	{0x4DC0  ,	0x4DFF  ,	99 ,	0xFFFF},	// Yijing Hexagram Symbols
	{0x4E00  ,	0x9FA5  ,	59 ,	936   },	// CJK Unified Ideographs
	{0x9FA6  ,	0x9FFF  ,	59 ,	0xFFFF},	// CJK Unified Ideographs
	{0xA000  ,	0xA48F  ,	83 ,	0xFFFF},	// Yi Syllables
	{0xA490  ,	0xA4CF  ,	83 ,	0xFFFF},	// Yi Radicals
	{0xA4D0  ,	0xA4FF  ,	999,	0xFFFF},	// 
	{0xA500  ,	0xA63F  ,	12 ,	0xFFFF},	// Vai
	{0xA640  ,	0xA69F  ,	9  ,	0xFFFF},	// Cyrillic Extended-B
	{0xA6A0  ,	0xA6FF  ,	999,	0xFFFF},	// 
	{0xA700  ,	0xA71F  ,	5  ,	0xFFFF},	// Modifier Tone Letters
	{0xA720  ,	0xA7FF  ,	29 ,	0xFFFF},	// Latin Extended-D
	{0xA800  ,	0xA82F  ,	100,	0xFFFF},	// Syloti Nagri
	{0xA830  ,	0xA8FF  ,	999,	0xFFFF},	// 
	{0xA840  ,	0xA87F  ,	53 ,	0xFFFF},	// Phags-pa
	{0xA880  ,	0xA8DF  ,	115,	0xFFFF},	// Saurashtra
	{0xA8E0  ,	0xA8FF  ,	999,	0xFFFF},	// 
	{0xA900  ,	0xA92F  ,	116,	0xFFFF},	// Kayah Li
	{0xA930  ,	0xA95F  ,	117,	0xFFFF},	// Rejang
	{0xA960  ,	0xA9FF  ,	999,	0xFFFF},	// 
	{0xAA00  ,	0xAA5F  ,	118,	0xFFFF},	// Cham
	{0xAA60  ,	0xABFF  ,	999,	0xFFFF},	// 
	{0xAC00  ,	0xD7AF  ,	56 ,	949   },	// Hangul Syllables
	{0xD7B0  ,	0xD7FF  ,	999,	0xFFFF},	// 
	{0xD800  ,	0xDB7F  ,	57 ,	0xFFFF},	// High Surrogates
	{0xDB80  ,	0xDBFF  ,	57 ,	0xFFFF},	// High Private Use Surrogates
	{0xDC00  ,	0xDFFF  ,	57 ,	0xFFFF},	// Low Surrogates
	{0xE000  ,	0xE814  ,	60 ,	0xFFFF},	// Private Use Area
	{0xE815  ,	0xE864  ,	60 ,	936   },	// Private Use Area
	{0xE865  ,	0xEFFF  ,	60 ,	0xFFFF},	// Private Use Area
	{0xF000  ,	0xF0FF  ,	60 ,	42    },	// Private Use Area
	{0xF100  ,	0xF8FF  ,	60 ,	0xFFFF},	// Private Use Area
	{0xF900  ,	0xFA0B  ,	61 ,	949   },	// CJK Compatibility Ideographs
	{0xFA0C  ,	0xFA0D  ,	61 ,	936   },	// CJK Compatibility Ideographs
	{0xFA0E  ,	0xFA2D  ,	61 ,	932   },	// CJK Compatibility Ideographs
	{0xFA2E  ,	0xFAFF  ,	61 ,	0xFFFF},	// CJK Compatibility Ideographs
	{0xFB00  ,	0xFB4F  ,	62 ,	0xFFFF},	// Alphabetic Presentation Forms
	{0xFB50  ,	0xFDFF  ,	63 ,	1256  },	// Arabic Presentation Forms-A
	{0xFE00  ,	0xFE0F  ,	91 ,	0xFFFF},	// Variation Selectors
	{0xFE10  ,	0xFE1F  ,	65 ,	0xFFFF},	// Vertical Forms
	{0xFE20  ,	0xFE2F  ,	64 ,	0xFFFF},	// Combining Half Marks
	{0xFE30  ,	0xFE4F  ,	65 ,	0xFFFF},	// CJK Compatibility Forms
	{0xFE50  ,	0xFE6F  ,	66 ,	0xFFFF},	// Small Form Variants
	{0xFE70  ,	0xFEFF  ,	67 ,	1256  },	// Arabic Presentation Forms-B
	{0xFF00  ,	0xFF5F  ,	68 ,	936   },	// Halfwidth And Fullwidth Forms
	{0xFF60  ,	0xFF9F  ,	68 ,	932   },	// Halfwidth And Fullwidth Forms
	{0xFFA0  ,	0xFFEF  ,	68 ,	0xFFFF},	// Halfwidth And Fullwidth Forms
};

static const FSCRT_CHARSETMAP g_FSCharset2CodePageTable[] =
{	
	{ 0,	1252  },	// ANSI
	{ 1,	0	  },	// Default
	{ 2,	42	  },	// Symbol
	{ 77,	10000 },	// Mac Roman
	{ 78,	10001 },	// Mac Shift Jis
	{ 79,	10003 },	// Mac Korean
	{ 80,	10008 },	// Mac Chinese Simplified (GBK)
	{ 81,	10002 },	// Mac Big5
	{ 83,	10005 },	// Mac Hebrew
	{ 84,	10004 },	// Mac Arabic
	{ 85,	10006 },	// Mac Greek
	{ 86,	10081 },	// Mac Turkish
	{ 87,	10021 },	// Mac Thai
	{ 88,	10029 },	// Mac Eastern European (Latin 2)
	{ 89,	10007 },	// Mac Cyrillic
	{ 128,	932	  },	// Shift JIS
	{ 129,	949	  },	// Korean
	{ 130,	1361  },	// Johab
	{ 134,	936	  },	// Chinese Simplified (GBK)
	{ 136,	950	  },	// Big5
	{ 161,	1253  },	// Greek
	{ 162,	1254  },	// Turkish
	{ 163,	1258  },	// Vietnamese
	{ 177,	1255  },	// Hebrew
	{ 178,	1256  },	// Arabic 
	{ 186,	1257  },	// Baltic
	{ 204,	1251  },	// Cyrillic
	{ 222,	874	  },	// Thai
	{ 238,	1250  },	// Eastern European
	{ 254,	437	  },	// PC 437
	{ 255,	850	  }, 	// OEM
};


/////////////////////////////////////////////////
//CFSCRT_UnicodeMapper
FS_INT32 FSCRT_UnicodeMapper::GetCodePageFromUnicode(FS_DWORD unicode)
{
	FX_INT32 nEnd = sizeof(g_FSFontUSBTable) / sizeof(FSCRT_FONTUSB) - 1;
	if (nEnd < 0) return -1;
	FX_INT32 nStart = 0, nMid;

	do 
	{
		nMid = (nStart + nEnd) / 2;
		const FSCRT_FONTUSB &usb = g_FSFontUSBTable[nMid];
		if (unicode < usb.dwStartUnicode)
			nEnd = nMid - 1;
		else if (unicode > usb.dwEndUnicode)
			nStart = nMid + 1;
		else
			return (usb.wCodePage == 0xFFFF) ? -1 : (FX_INT32)usb.wCodePage;
	} while (nStart <= nEnd);

	return -1;
}

FS_DWORD FSCRT_UnicodeMapper::GetCharsetFromUnicode(FS_DWORD word)
{
	//to avoid CJK Font to show ASCII
	if (word < 0x7F) return ANSI_CHARSET;

	int nACP = GetSysACP();
	switch (nACP)
	{
	case 932:
	case 936:
	case 950:
	case 949:
		if ((word >= 0x2E80 && word <= 0x2EFF) ||
			(word >= 0x3000 && word <= 0x303F) ||
			(word >= 0x3200 && word <= 0x32FF) ||
			(word >= 0x3300 && word <= 0x33FF) ||
			(word >= 0x3400 && word <= 0x4DB5) ||
			(word >= 0x4E00 && word <= 0x9FFF) ||
			(word >= 0xF900 && word <= 0xFAFF) ||
			(word >= 0xFE30 && word <= 0xFE4F) ||
			(word >= 0x20000 && word <= 0x2A6D6) ||
			(word >= 0x2F800 && word <= 0x2FA1F)||
			(word >= 0xFF00 && word <= 0xFF5E))
		{
			switch (nACP)
			{
			case 932:
				return SHIFTJIS_CHARSET;
			case 936:
				return GB2312_CHARSET;
			case 950:
				return CHINESEBIG5_CHARSET;
			case 949:
				return HANGUL_CHARSET;
			}
		}
		break;
	}

	//find new charset
	if (word == 0x20A9)
		return HANGUL_CHARSET;
	if ((word >= 0x4E00 && word <= 0x9FA5) || 
		(word >= 0xE7C7 && word <= 0xE7F3) ||
		(word >= 0x3000 && word <= 0x303F)
		|| (word >= 0x2000 && word <= 0x206F)
		)
	{
		return GB2312_CHARSET;
	}

	if (((word >= 0x3040) && (word <= 0x309F)) ||
		((word >= 0x30A0) && (word <= 0x30FF)) ||
		((word >= 0x31F0) && (word <= 0x31FF)) ||
		((word >= 0xFF5F) && (word <= 0xFFEF)) )
	{
		return SHIFTJIS_CHARSET;
	}

	if (((word >= 0xAC00) && (word <= 0xD7AF)) ||
		((word >= 0x1100) && (word <= 0x11FF)) ||
		((word >= 0x3130) && (word <= 0x318F)))
	{
		return HANGUL_CHARSET;
	}

	if (word >= 0x0E00 && word <= 0x0E7F)
		return THAI_CHARSET;

	if ((word >= 0x0370 && word <= 0x03FF) ||
		(word >= 0x1F00 && word <= 0x1FFF))
		return GREEK_CHARSET;

	if ((word >= 0x0600 && word <= 0x06FF) ||
		(word >= 0xFB50 && word <= 0xFEFC))
		return ARABIC_CHARSET;

	if (word >= 0x0590 && word <= 0x05FF)
		return HEBREW_CHARSET;

	if (word >= 0x0400 && word <= 0x04FF)
		return RUSSIAN_CHARSET;

	if (word == 0x11E || word == 0x11F || word == 0x130 || word == 0x131 || word == 0x15E || word == 0x15F)
		return TURKISH_CHARSET;

	if (word >= 0x0100 && word <= 0x024F)
		return EASTEUROPE_CHARSET;

	if (word >= 0x1E00 && word <= 0x1EFF)
		return VIETNAMESE_CHARSET;

	return DEFAULT_CHARSET;
}

FS_DWORD FSCRT_UnicodeMapper::GetCharsetFromCodePage(FX_INT32 codepage)
{
	return FXFM_GetCharsetFromCodePage(codepage);
}

FS_INT32 FSCRT_UnicodeMapper::GetCodePageFromCharset(FS_DWORD charset)
{
	if (charset == FSCommonDefines::e_fontCharsetDefault)
		return FXFM_CODEPAGE_Unknown;
	FX_INT32 iEnd = sizeof(g_FSCharset2CodePageTable) / sizeof(FSCRT_CHARSETMAP) - 1;
	if (iEnd >= 0)
	{
		FX_INT32 iStart = 0, iMid;
		do 
		{
			iMid = (iStart + iEnd) / 2;
			const FSCRT_CHARSETMAP & cp = g_FSCharset2CodePageTable[iMid];
			if (charset == cp.dwCharset)
				return cp.nCodePage;
			else if (charset < cp.dwCharset)
				iEnd = iMid - 1;
			else 
				iStart = iMid + 1;
		} while (iStart <= iEnd);
	}
	return FXFM_CODEPAGE_Unknown;
}

extern FS_INT32 getACP();
FS_INT32 FSCRT_UnicodeMapper::GetSysACP()
{
	return (FX_INT32)getACP();
}

//////////////////////////////////////////////////////////////////////////////////////////
// Class CFSPDF_DateTime

static FS_INT32 FSPDF_Util_GetTimeZoneInSeconds(FX_CHAR tzhour, FS_BYTE tzminute)
{
	return (FS_INT32)tzhour * 3600 + (FS_INT32)tzminute * (tzhour >= 0 ? 60 : -60);
}

static FS_BOOL FSPDF_Util_IsLeapYear(FS_INT16 year)
{
	return ((year % 400 == 0) || ((year % 4 == 0) && (year % 100 != 0)));
}

static FS_WORD FSPDF_Util_GetYearDays(FS_INT16 year)
{
	return (FSPDF_Util_IsLeapYear(year) == TRUE ? 366 : 365);
}

static FS_BYTE FSPDF_Util_GetMonthDays(FS_INT16 year, FS_BYTE month)
{
	FX_BYTE	mDays;
	switch (month)
	{
	case 1:
	case 3:
	case 5:
	case 7:
	case 8:
	case 10:
	case 12:
		mDays = 31;
		break;
	case 4:
	case 6:
	case 9:
	case 11:
		mDays = 30;
		break;

	case 2:
		if (FSPDF_Util_IsLeapYear(year) == TRUE)
			mDays = 29;
		else
			mDays = 28;
		break;

	default:
		mDays = 0;
		break;
	}

	return mDays;
}

static FS_BOOL ParserDateTimeChar(const CFX_ByteString& dtStr, FS_INT32 &index, FS_BOOL &bContinue, FS_UINT16 parseCount, FS_UINT16 &ret)
{
	int i = 0;
	int strLength = dtStr.GetLength();

	CFX_ByteString sub;
	sub = dtStr.Mid(index, parseCount);
	while (i < parseCount) {
		if (FSCRT_CHAR_NotDigit(sub[i])) {
			bContinue = FALSE;
			return FALSE;
		}
		i ++;
	}

	ret = FXSYS_atoi(sub);
	index += parseCount;

	if (strLength == index) bContinue = FALSE;

	return TRUE;
}

CFSPDF_DateTime::CFSPDF_DateTime()
{
	m_timezone = -28800;
	//ResetCurrentSysTime();
	Empty();
}

//CFSPDF_DateTime::CFSPDF_DateTime(const CFX_ByteString& dtStr)
//{
//	m_timezone = -28800;
//	//ResetCurrentSysTime();
//	Empty();
//	//FromPDFDateTimeString(dtStr);
//	ParserPDFDateTimeString(dtStr);
//}

CFSPDF_DateTime::CFSPDF_DateTime(const CFSPDF_DateTime& datetime)
{
	m_timezone = -28800;
	Empty();
	operator = (datetime);
}

CFSPDF_DateTime::CFSPDF_DateTime(const FSDateTime& firstPageNumber)
{
	m_timezone = -28800;
	Empty();
	operator = (firstPageNumber) ;
	m_fsDatetime.milliseconds = 0;
}

void CFSPDF_DateTime::Empty()
{
	FSCRT_LOCKOBJECT(lock, &m_lock);
	FXSYS_memset(&m_fsDatetime, 0, sizeof(FSDateTime));
}

void CFSPDF_DateTime::SetCurrentSysTime()
{
#if _FX_OS_ == _FX_LINUX_DESKTOP_ || _FX_OS_ == _FX_LINUX_EMBEDDED_ 
	FX_SYSTEMTIME sysTime;
	FX_Time_GetSystemTime(&sysTime);
	tzset();
	FS_INT32 tzHour = __timezone / 3600 * -1;
	FS_INT32 tzMin = (FXSYS_abs(__timezone) % 3600) / 60;
#elif  _FX_OS_ == _FX_ANDROID_
	FX_SYSTEMTIME sysTime;
	FX_Time_GetSystemTime(&sysTime);
	tzset();
	FS_INT32 tzHour = 0;
	FS_INT32 tzMin = 0;
#elif _FX_OS_ == _FX_MACOSX_ || _FX_OS_ == _FX_IOS_
    FX_SYSTEMTIME sysTime;
    FX_Time_GetSystemTime(&sysTime);
    tzset();
    FS_INT16 tzHour = timezone / 3600 * -1;
    FS_INT16 tzMin = (FXSYS_abs(timezone) % 3600) / 60;
#elif (_FX_OS_ == _FX_WIN32_DESKTOP_ || _FX_OS_ == _FX_WIN32_MOBILE_ || _FX_OS_ == _FX_WIN64_) && !(WINAPI_PARTITION_APP || WINAPI_PARTITION_PC_APP)
	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);
	_tzset();

	FS_INT32 tzHour = timezone / 3600 * -1;
	FS_INT32 tzMin = (FXSYS_abs(timezone) % 3600) / 60;
#elif (WINAPI_PARTITION_APP || WINAPI_PARTITION_PC_APP)
	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);

	TIME_ZONE_INFORMATION tzInfo;
	GetTimeZoneInformation(&tzInfo);
	//TIME_ZONE_INFORMATION::Bias returned by GetTimeZoneInformation is based on bias1 in "UTC = local time + bias1", 
	//While in PDF Date format, the time zone part is based on bias2 in "local time = UTC + bias2".
	//So TIME_ZONE_INFORMATION::Bias is alwasy the opposite number to what we want for PDF Date format.
	//Also, the unit of TIME_ZONE_INFORMATION::Bias is minute.
	FS_INT32 tzHour = (tzInfo.Bias / 60) * -1;
	FS_INT32 tzMin = (FXSYS_abs(tzInfo.Bias) % 60);		
#endif

	FSCRT_LOCKOBJECT(lock, &m_lock);
	m_fsDatetime.year = sysTime.wYear;
	m_fsDatetime.month = sysTime.wMonth;
	m_fsDatetime.day = sysTime.wDay;
	m_fsDatetime.hour = sysTime.wHour;
	m_fsDatetime.minute = sysTime.wMinute;
	m_fsDatetime.second = sysTime.wSecond;
	m_fsDatetime.milliseconds = sysTime.wMilliseconds;
	m_fsDatetime.UTHourOffset = tzHour;
	m_fsDatetime.UTMinuteOffset = tzMin;
}

CFSPDF_DateTime& CFSPDF_DateTime::operator = (const CFSPDF_DateTime& datetime)
{
	FSCRT_LOCKOBJECT(lock, &m_lock);
	FXSYS_memcpy(&m_fsDatetime, &datetime.m_fsDatetime, sizeof(FSDateTime));
	return *this;
}

CFSPDF_DateTime& CFSPDF_DateTime::operator = (const FSDateTime& firstPageNumber)
{
	FSCRT_LOCKOBJECT(lock, &m_lock);
	FXSYS_memcpy(&m_fsDatetime, &firstPageNumber, sizeof(FSDateTime));
	return *this;
}

FS_BOOL CFSPDF_DateTime::operator == (CFSPDF_DateTime& datetime)
{
	FSCRT_LOCKOBJECT(lock, &m_lock);
	return (FXSYS_memcmp(&m_fsDatetime, &datetime.m_fsDatetime, sizeof(FSDateTime)) == 0);
}

FS_BOOL CFSPDF_DateTime::operator != (CFSPDF_DateTime& datetime)
{
	FSCRT_LOCKOBJECT(lock, &m_lock);
	return (FXSYS_memcmp(&m_fsDatetime, &datetime.m_fsDatetime, sizeof(FSDateTime)) != 0);
}

FS_BOOL CFSPDF_DateTime::operator > (CFSPDF_DateTime& datetime)
{
	FSCRT_LOCKOBJECT(lock, &m_lock);
	CFSPDF_DateTime dt1 = ToGMT();
	CFSPDF_DateTime dt2 = datetime.ToGMT();
	int d1 = (((int)dt1.m_fsDatetime.year) << 16) | (((int)dt1.m_fsDatetime.month) << 8) | (int)dt1.m_fsDatetime.day;
	int d2 = (((int)dt1.m_fsDatetime.hour) << 16) | (((int)dt1.m_fsDatetime.minute) << 8) | (int)dt1.m_fsDatetime.second;
	int d3 = (((int)dt2.m_fsDatetime.year) << 16) | (((int)dt2.m_fsDatetime.month) << 8) | (int)dt2.m_fsDatetime.day;
	int d4 = (((int)dt2.m_fsDatetime.hour) << 16) | (((int)dt2.m_fsDatetime.minute) << 8) | (int)dt2.m_fsDatetime.second;

	if (d1 > d3) return TRUE;
	if (d1 == d3 && d2 > d4) return TRUE;
	return FALSE;
}

FS_BOOL CFSPDF_DateTime::operator >= (CFSPDF_DateTime& datetime)
{
	FSCRT_LOCKOBJECT(lock, &m_lock);
	CFSPDF_DateTime dt1 = ToGMT();
	CFSPDF_DateTime dt2 = datetime.ToGMT();
	int d1 = (((int)dt1.m_fsDatetime.year) << 16) | (((int)dt1.m_fsDatetime.month) << 8) | (int)dt1.m_fsDatetime.day;
	int d2 = (((int)dt1.m_fsDatetime.hour) << 16) | (((int)dt1.m_fsDatetime.minute) << 8) | (int)dt1.m_fsDatetime.second;
	int d3 = (((int)dt2.m_fsDatetime.year) << 16) | (((int)dt2.m_fsDatetime.month) << 8) | (int)dt2.m_fsDatetime.day;
	int d4 = (((int)dt2.m_fsDatetime.hour) << 16) | (((int)dt2.m_fsDatetime.minute) << 8) | (int)dt2.m_fsDatetime.second;
	if (d1 > d3) return TRUE;
	if (d1 == d3 && d2 >= d4) return TRUE;

	return FALSE;
}

FS_BOOL CFSPDF_DateTime::operator < (CFSPDF_DateTime& datetime)
{
	FSCRT_LOCKOBJECT(lock, &m_lock);
	CFSPDF_DateTime dt1 = ToGMT();
	CFSPDF_DateTime dt2 = datetime.ToGMT();
	int d1 = (((int)dt1.m_fsDatetime.year) << 16) | (((int)dt1.m_fsDatetime.month) << 8) | (int)dt1.m_fsDatetime.day;
	int d2 = (((int)dt1.m_fsDatetime.hour) << 16) | (((int)dt1.m_fsDatetime.minute) << 8) | (int)dt1.m_fsDatetime.second;
	int d3 = (((int)dt2.m_fsDatetime.year) << 16) | (((int)dt2.m_fsDatetime.month) << 8) | (int)dt2.m_fsDatetime.day;
	int d4 = (((int)dt2.m_fsDatetime.hour) << 16) | (((int)dt2.m_fsDatetime.minute) << 8) | (int)dt2.m_fsDatetime.second;

	if (d1 < d3) return TRUE;
	if (d1 == d3 && d2 < d4) return TRUE;
	return FALSE;
}

FS_BOOL CFSPDF_DateTime::operator <= (CFSPDF_DateTime& datetime)
{
	FSCRT_LOCKOBJECT(lock, &m_lock);
	CFSPDF_DateTime dt1 = ToGMT();
	CFSPDF_DateTime dt2 = datetime.ToGMT();
	int d1 = (((int)dt1.m_fsDatetime.year) << 16) | (((int)dt1.m_fsDatetime.month) << 8) | (int)dt1.m_fsDatetime.day;
	int d2 = (((int)dt1.m_fsDatetime.hour) << 16) | (((int)dt1.m_fsDatetime.minute) << 8) | (int)dt1.m_fsDatetime.second;
	int d3 = (((int)dt2.m_fsDatetime.year) << 16) | (((int)dt2.m_fsDatetime.month) << 8) | (int)dt2.m_fsDatetime.day;
	int d4 = (((int)dt2.m_fsDatetime.hour) << 16) | (((int)dt2.m_fsDatetime.minute) << 8) | (int)dt2.m_fsDatetime.second;

	if (d1 <= d3) return TRUE;
	if (d1 == d3 && d2 <= d4) return TRUE;
	return FALSE;
}

CFSPDF_DateTime::operator FSPDF_UNITIME()
{
	struct tm newtime;

	FSCRT_LOCKOBJECT(lock, &m_lock);
	newtime.tm_year = m_fsDatetime.year - 1900;
	newtime.tm_mon = m_fsDatetime.month - 1;
	newtime.tm_mday = m_fsDatetime.day;
	newtime.tm_hour = m_fsDatetime.hour;
	newtime.tm_min = m_fsDatetime.minute;
	newtime.tm_sec = m_fsDatetime.second;
	newtime.tm_yday = 0;
	newtime.tm_wday = 0;
	newtime.tm_isdst = -1;

	return mktime(&newtime);
}

FS_BOOL CFSPDF_DateTime::ParserPDFDateTimeString(const CFX_ByteString& dtStr)
{
	int strLength = dtStr.GetLength();
	if (strLength < 4) return FALSE;

	int index = 0;
	int i = 0;
	int j = 0;
	FS_BOOL bContinue = TRUE;

	bool prefix = (dtStr[0] == 'D' && dtStr[1] == ':');
	if (prefix) {
		index = 2;
		if (strLength < 6) return FALSE;
	}

	FSCRT_LOCKOBJECT(lock, &m_lock);
	if (!ParserYear(dtStr, index, bContinue)) return FALSE;
	if (!bContinue) return TRUE;

	if (FSCRT_CHAR_TimeZoom(dtStr[index])) return ParserTimeZone(dtStr, index);

	if (strLength < index + 2) return FALSE;
	if (!ParserMonth(dtStr, index, bContinue)) return FALSE;
	if (!bContinue) return TRUE;

	if (FSCRT_CHAR_TimeZoom(dtStr[index])) return ParserTimeZone(dtStr, index);

	if (strLength < index + 2) return FALSE;
	if (!ParserDay(dtStr, index, bContinue)) return FALSE;
	if (!bContinue) return TRUE;

	if (FSCRT_CHAR_TimeZoom(dtStr[index])) return ParserTimeZone(dtStr, index);

	if (strLength < index + 2) return FALSE;
	if (!ParserHour(dtStr, index, bContinue)) return FALSE;
	if (!bContinue) return TRUE;

	if (FSCRT_CHAR_TimeZoom(dtStr[index])) return ParserTimeZone(dtStr, index);

	if (strLength < index + 2) return FALSE;
	if (!ParserMinute(dtStr, index, bContinue)) return FALSE;
	if (!bContinue) return TRUE;

	if (FSCRT_CHAR_TimeZoom(dtStr[index])) return ParserTimeZone(dtStr, index);

	if (strLength < index + 2) return FALSE;
	if (!ParserSecond(dtStr, index, bContinue)) return FALSE;
	if (!bContinue) return TRUE;

	return ParserTimeZone(dtStr, index);
}

FX_BOOL CFSPDF_DateTime::ParserPDFXMPDateTimeString(const CFX_WideString& dtStr)
{
	CFX_ByteString bsStr;
	bsStr.ConvertFrom(dtStr);

	int strLength = dtStr.GetLength();
	if (strLength < 4) return FALSE;

	int index = 0;
	FS_BOOL bContinue = TRUE;

	if (!ParserYear(bsStr, index, bContinue)) return FALSE;	
	if (!bContinue) return TRUE;
	if (bsStr[index] == '-')
		index++;

	if (strLength < index + 2) return FALSE;
	if (!ParserMonth(bsStr, index, bContinue)) return FALSE;
	if (!bContinue) return TRUE;
	if (bsStr[index] == '-')
		index++;

	if (strLength < index + 2) return FALSE;
	if (!ParserDay(bsStr, index, bContinue)) return FALSE;
	if (!bContinue) return TRUE;
	if (bsStr[index] == 'T')
		index++;

	if (strLength < index + 2) return FALSE;
	if (!ParserHour(bsStr, index, bContinue)) return FALSE;
	if (!bContinue) return TRUE;
	if (bsStr[index] == ':')
		index++;

	if (strLength < index + 2) return FALSE;
	if (!ParserMinute(bsStr, index, bContinue)) return FALSE;
	if (!bContinue) return TRUE;

	//it may not have any value for second in bsStr.
	if (bsStr[index] == ':')
	{
		index++;
		if (strLength < index + 2) return FALSE;
		if (!ParserSecond(bsStr, index, bContinue)) return FALSE;
		if (!bContinue) return TRUE;
	}
	else
		m_fsDatetime.second = 0;

	return ParserXMPTimeZone(bsStr, index);
}

FS_BOOL CFSPDF_DateTime::ParserXMPTimeZone(const CFX_ByteString& dtStr, FS_INT32 &index)
{
	CFX_ByteString sub;
	sub = dtStr.Mid(index, dtStr.GetLength() - index);

	if (!FSCRT_CHAR_TimeZoom(sub[0])) return FALSE;
	if (sub[0] == 'Z') return TRUE;

	int subStrLength = sub.GetLength();
	if (subStrLength == 1) return TRUE;

	if (subStrLength != 6 && subStrLength != 5 && subStrLength != 3) return FALSE;

	index = 1;

	if (sub[index] == ':') {
		index ++;
		FS_BOOL bContinue = TRUE;
		if (!ParserDateTimeChar(sub, index, bContinue, 2, *(FS_UINT16*)&m_fsDatetime.UTMinuteOffset)) return FALSE;
		index ++;
	} 
	else 
	{
		FS_BOOL bContinue = TRUE;
		if (!ParserDateTimeChar(sub, index, bContinue, 2, *(FS_UINT16*)&m_fsDatetime.UTHourOffset)) return FALSE;
		if (sub[0] == '-') m_fsDatetime.UTHourOffset = -m_fsDatetime.UTHourOffset;
		if (!bContinue) return TRUE;

		if (sub[index] != ':') return FALSE;

		index++;
		if (!ParserDateTimeChar(sub, index, bContinue, 2, *(FS_UINT16*)&m_fsDatetime.UTMinuteOffset)) return FALSE;
	}

	return index == subStrLength;
}

FS_BOOL CFSPDF_DateTime::ParserYear(const CFX_ByteString& dtStr, FS_INT32 &index, FS_BOOL &bContinue)
{
	return ParserDateTimeChar(dtStr, index, bContinue, 4, m_fsDatetime.year);
}

FS_BOOL CFSPDF_DateTime::ParserMonth(const CFX_ByteString& dtStr, FS_INT32 &index, FS_BOOL &bContinue)
{
	return ParserDateTimeChar(dtStr, index, bContinue, 2, m_fsDatetime.month);
}

FS_BOOL CFSPDF_DateTime::ParserDay(const CFX_ByteString& dtStr, FS_INT32 &index, FS_BOOL &bContinue)
{
	return ParserDateTimeChar(dtStr, index, bContinue, 2, m_fsDatetime.day);
}

FS_BOOL CFSPDF_DateTime::ParserHour(const CFX_ByteString& dtStr, FS_INT32 &index, FS_BOOL &bContinue)
{
	return ParserDateTimeChar(dtStr, index, bContinue, 2, m_fsDatetime.hour);
}

FS_BOOL CFSPDF_DateTime::ParserMinute(const CFX_ByteString& dtStr, FS_INT32 &index, FS_BOOL &bContinue)
{
	return ParserDateTimeChar(dtStr, index, bContinue, 2, m_fsDatetime.minute);
}

FS_BOOL CFSPDF_DateTime::ParserSecond(const CFX_ByteString& dtStr, FS_INT32 &index, FS_BOOL &bContinue)
{
	return ParserDateTimeChar(dtStr, index, bContinue, 2, m_fsDatetime.second);
}

FS_BOOL CFSPDF_DateTime::ParserTimeZone(const CFX_ByteString& dtStr, FS_INT32 &index)
{
	FSCRT_LOCKOBJECT(lock, &m_lock);
	CFX_ByteString sub;
	sub = dtStr.Mid(index, dtStr.GetLength() - index);

	if (!FSCRT_CHAR_TimeZoom(sub[0])) return FALSE;
	if (sub[0] == 'Z') return TRUE;

	int subStrLength = sub.GetLength();
	if (subStrLength == 1) return TRUE;

	if (subStrLength != 7 && subStrLength != 5 && subStrLength != 3) return FALSE;

	index = 1;

	if (sub[index] == '\'') {
		index ++;
		FS_BOOL bContinue = TRUE;
		if (!ParserDateTimeChar(sub, index, bContinue, 2, *(FS_UINT16*)&m_fsDatetime.UTMinuteOffset)) return FALSE;
		if (sub[index] != '\'') return FALSE;
		index ++;

	} else {
		FS_BOOL bContinue = TRUE;
		if (!ParserDateTimeChar(sub, index, bContinue, 2, *(FS_UINT16*)&m_fsDatetime.UTHourOffset)) return FALSE;
		if (sub[0] == '-') m_fsDatetime.UTHourOffset = -m_fsDatetime.UTHourOffset;
		if (!bContinue) return TRUE;

		if (sub[index] != '\'') return FALSE;
		index ++;

		if (!ParserDateTimeChar(sub, index, bContinue, 2, *(FS_UINT16*)&m_fsDatetime.UTMinuteOffset)) return FALSE;

		if (sub[index] != '\'') return FALSE;
		index ++;
	}

	return index == subStrLength;
}

CFX_ByteString CFSPDF_DateTime::ToCommonDateTimeString()
{
	FSCRT_LOCKOBJECT(lock, &m_lock);
	CFX_ByteString strDt;
	strDt.Format("%04d-%02d-%02d %02d:%02d:%02d ", m_fsDatetime.year, m_fsDatetime.month, m_fsDatetime.day, m_fsDatetime.hour, m_fsDatetime.minute, m_fsDatetime.second);
	if (m_fsDatetime.UTHourOffset < 0)
		strDt += '-';
	else
		strDt += '+';
	CFX_ByteString strTimeZone;
	strTimeZone.Format("%02d:%02d", FXSYS_abs(m_fsDatetime.UTHourOffset), m_fsDatetime.UTMinuteOffset);
	strDt += strTimeZone;
	return strDt + strTimeZone;
}

CFX_ByteString CFSPDF_DateTime::ToPDFDateTimeString()
{
	FSCRT_LOCKOBJECT(lock, &m_lock);
	CFX_ByteString strDt;
	strDt.Format("D:%04d%02d%02d%02d%02d%02d", m_fsDatetime.year, m_fsDatetime.month, m_fsDatetime.day, m_fsDatetime.hour, m_fsDatetime.minute, m_fsDatetime.second);

	if (m_fsDatetime.UTHourOffset < 0)
		strDt += '-';
	else if(m_fsDatetime.UTHourOffset == 0)
		strDt += 'Z';
	else
		strDt += '+';

	CFX_ByteString strTimeZone;
	strTimeZone.Format("%02d'%02d'", FXSYS_abs(m_fsDatetime.UTHourOffset), m_fsDatetime.UTMinuteOffset);
	strDt += strTimeZone;

	return strDt;
}

CFX_WideString CFSPDF_DateTime::ToXMPDateTimeString()
{
	CFX_WideString dtStr;
	CFX_ByteString tempStr;

	tempStr.Format("%04d-%02d-%02dT%02d:%02d:%02d", m_fsDatetime.year, m_fsDatetime.month, m_fsDatetime.day, m_fsDatetime.hour, m_fsDatetime.minute, m_fsDatetime.second);
	dtStr.ConvertFrom(tempStr);

	if (m_fsDatetime.UTHourOffset < 0)
		dtStr += FX_WSTRC(L"-");
	else
		dtStr += FX_WSTRC(L"+");

	tempStr = FX_BSTRC("");
	tempStr.Format("%02d:%02d", FXSYS_abs(m_fsDatetime.UTHourOffset), m_fsDatetime.UTMinuteOffset);

	CFX_WideString dtStrTmp;
	dtStrTmp.ConvertFrom(tempStr);
	dtStr += dtStrTmp;

	return dtStr;
}

FSDateTime CFSPDF_DateTime::ToSystemTime()
{
	FSCRT_LOCKOBJECT(lock, &m_lock);
	CFSPDF_DateTime dateTime = *this;
	time_t t = (time_t)dateTime;
	struct tm _Tm;
#if (WINAPI_PARTITION_APP || WINAPI_PARTITION_PC_APP) || (defined(_WIN32) || defined(_WIN64))&&_FX_COMPILER_!= _FX_VC6_
	localtime_s(&_Tm, &t);
	struct tm* pTime = &_Tm;
#else
	struct tm* pTime = localtime(&t); //This is unsafe and it suggests to use localtime_s for instead. 2013-04-22 by Yvonne
#endif
	FSDateTime retFSDt;
	retFSDt.set(0, 0, 0, 0, 0, 0, 0, 0, 0);
	retFSDt.year = (FX_WORD)pTime->tm_year + 1900;
	retFSDt.month = (FX_WORD)pTime->tm_mon + 1;
	retFSDt.day = (FX_WORD)pTime->tm_mday;
	retFSDt.hour = (FX_WORD)pTime->tm_hour;
	retFSDt.minute = (FX_WORD)pTime->tm_min;
	retFSDt.second = (FX_WORD)pTime->tm_sec;
	//retFSDt.milliseconds = 0;
	return retFSDt;
}

CFSPDF_DateTime CFSPDF_DateTime::ToGMT()
{
	FSCRT_LOCKOBJECT(lock, &m_lock);
	CFSPDF_DateTime dateTime = *this;
	dateTime.AddSeconds(-FSPDF_Util_GetTimeZoneInSeconds((FX_CHAR)dateTime.m_fsDatetime.UTHourOffset, dateTime.m_fsDatetime.UTMinuteOffset));
	dateTime.m_fsDatetime.UTHourOffset = 0;
	dateTime.m_fsDatetime.UTMinuteOffset = 0;
	return dateTime;
}

FS_BOOL CFSPDF_DateTime::AddDays(short days)
{
	if (days == 0) return FALSE;

	FSCRT_LOCKOBJECT(lock, &m_lock);
	FX_SHORT	y = m_fsDatetime.year, yy;
	FX_BYTE		m = m_fsDatetime.month;
	FX_BYTE		d = m_fsDatetime.day;
	int			mdays, ydays, ldays;

	ldays = days;
	if (ldays > 0)
	{
		yy = y;
		if (((FX_WORD)m * 100 + d) > 300) yy ++;
		ydays = FSPDF_Util_GetYearDays(yy);
		while (ldays >= ydays)
		{
			y ++;
			ldays -= ydays;
			yy ++;
			mdays = FSPDF_Util_GetMonthDays(y, m);
			if (d > mdays)
			{
				m ++;
				d -= mdays;
			}
			ydays = FSPDF_Util_GetYearDays(yy);
		}
		mdays = FSPDF_Util_GetMonthDays(y, m) - d + 1;
		while (ldays >= mdays)
		{
			ldays -= mdays;
			m ++;
			d = 1;
			mdays = FSPDF_Util_GetMonthDays(y, m);
		}
		d += ldays;
	}
	else
	{
		ldays *= -1;
		yy = y;
		if (((FX_WORD)m * 100 + d) < 300) yy --;
		ydays = FSPDF_Util_GetYearDays(yy);
		while (ldays >= ydays)
		{
			y --;
			ldays -= ydays;
			yy --;
			mdays = FSPDF_Util_GetMonthDays(y, m);
			if (d > mdays)
			{
				m ++;
				d -= mdays;
			}
			ydays = FSPDF_Util_GetYearDays(yy);
		}
		while (ldays >= d)
		{
			ldays -= d;
			m --;
			mdays = FSPDF_Util_GetMonthDays(y, m);
			d = mdays;
		}
		d -= ldays;
	}

	m_fsDatetime.year = y;
	m_fsDatetime.month = m;
	m_fsDatetime.day = d;

	return TRUE;
}

FS_BOOL CFSPDF_DateTime::AddSeconds(int seconds)
{
	if (seconds == 0) return FALSE;

	int	n;
	int	days;

	FSCRT_LOCKOBJECT(lock, &m_lock);
	n = m_fsDatetime.hour * 3600 + m_fsDatetime.minute * 60 + m_fsDatetime.second + seconds;
	if (n < 0)
	{
		days = (n - 86399) / 86400;
		n -= days * 86400;
	}
	else
	{
		days = n / 86400;
		n %= 86400;
	}
	m_fsDatetime.hour = (FX_BYTE)(n / 3600);
	m_fsDatetime.hour %= 24;
	n %= 3600;
	m_fsDatetime.minute = (FX_BYTE)(n / 60);
	m_fsDatetime.second = (FX_BYTE)(n % 60);
	if (days != 0) AddDays(days);

	return TRUE;
}

FS_DWORD FSCRT_PointType_RDK2Fxcore(FS_PATHPOINTTYPE pointType)
{
	switch (pointType)
	{
	case FSCommonDefines::e_pointTypeMoveTo:
		return FXPT_MOVETO;
	case FSCommonDefines::e_pointTypeLineTo:
		return FXPT_LINETO;
	case FSCommonDefines::e_pointTypeLineToCloseFigure:
		return (FXPT_LINETO | FXPT_CLOSEFIGURE);
	case FSCommonDefines::e_pointTypeBezierTo:
		return FXPT_BEZIERTO;
	case FSCommonDefines::e_pointTypeBezierToCloseFigure:
		return (FXPT_BEZIERTO | FXPT_CLOSEFIGURE);
	default: // error/unsupported
		return 0;
	}
}
FS_PATHPOINTTYPE FSCRT_PointType_Fxcore2RDK(FS_DWORD pointFlag)
{
	switch(pointFlag)
	{
	case FXPT_MOVETO:
		return FSCommonDefines::e_pointTypeMoveTo;
	case FXPT_LINETO:
		return FSCommonDefines::e_pointTypeLineTo;
	case (FXPT_LINETO|FXPT_CLOSEFIGURE):
		return FSCommonDefines::e_pointTypeLineToCloseFigure;
	case FXPT_BEZIERTO:
		return FSCommonDefines::e_pointTypeBezierTo;
	case (FXPT_BEZIERTO|FXPT_CLOSEFIGURE):
		return FSCommonDefines::e_pointTypeBezierToCloseFigure;
	default:
		return (FS_PATHPOINTTYPE)0;
	}
}

FSBitmap*	FSCRT_Util_ConvertDIBSource2FSBitmap(CFX_DIBSource* pDIBSrc)
{
	if (!pDIBSrc) return NULL;
	FXDIB_Format format = pDIBSrc->GetFormat();
	FSBitmapImp* pFSBitmapImp = NULL;
	if (FSCommonDefines::e_dibArgb != format && FSCommonDefines::e_dibRgb != format && FSCommonDefines::e_dibRgb32 != format)
	{
		pFSBitmapImp = FSBitmapImp::create(pDIBSrc->GetWidth(), pDIBSrc->GetHeight(), FSCommonDefines::e_dibRgb32, 0, 0);
		if (!pFSBitmapImp)
		{
			//delete pDibSrc;
			return NULL;
		}
		CFX_DIBitmap* pBitmap = pDIBSrc->CloneConvert((FXDIB_Format)FSCommonDefines::e_dibRgb32);
		pFSBitmapImp->setBitmap(pBitmap);
	}
	else
	{
		pFSBitmapImp = FSBitmapImp::create(pDIBSrc->GetWidth(), pDIBSrc->GetHeight(), (FS_DIBFORMAT)format, 0, 0);
		if (!pFSBitmapImp)
		{
			//delete pDIBSrc;
			return NULL;
		}
		CFX_DIBitmap* pBitmap = pFSBitmapImp->getBitmap();
		if (!pBitmap->TransferBitmap(0, 0, pDIBSrc->GetWidth(), pDIBSrc->GetHeight(), pDIBSrc, 0, 0))
		{
			//delete pDIBSrc;
			pFSBitmapImp->release();
			return NULL;
		}
	}
	//delete pDIBSrc;
	return pFSBitmapImp;
}

