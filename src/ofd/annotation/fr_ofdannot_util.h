#ifndef _FR_OFD_ANNOTUTIL_H_
#define _FR_OFD_ANNOTUTIL_H_

#include "../fr_include.h"
#include"../../fs_util.h"
#include "../../../include/fs_annot.h"


OFD_ANNOTTYPE FR_OFDANNOT_Util_ConvertAnnotTypeFromEnumToOFDType(FSM_ANNOTTYPE annotType);

COFD_WriteAnnot* FR_OFDANNOT_UTIL_InsertAnnot(FSM_ANNOTTYPE annotType, const FS_INT32 annotIndex, const FSRectF annotRect, IOFD_Page* pPageDict, IOFD_Document* pDoc, FS_BOOL bRDKTrialWM = FALSE);
FSAnnot* FSANNOT_UTIL_InsertAnnot(int annotIndex, COFD_WriteAnnot* pAnnotDict, FSPDFPage* pPage);

void	FR_OFDANNOT_UTIL_SetDefaultProperties(FSAnnot* pAnnot);

FS_BOOL FRANNOT_Util_IsSignatureField(COFD_Annot* pAnnotDict);

FX_BOOL	FRANNOT_Util_CheckAnnotIsAtPoint(FSAnnot* pAnnot, FS_FLOAT x, FS_FLOAT y, FS_FLOAT tolerance, const FSMatrix* pSrcMatrix = NULL);
FX_BOOL FRANNOT_Util_IsPointInRect(FSAnnot* pAnnot, FSM_ANNOTTYPE annotType, FS_FLOAT x, FS_FLOAT y, FS_FLOAT tolerance, FS_INT32 nPageRotate, const FSMatrix* pSrcMatrix = NULL);

#endif