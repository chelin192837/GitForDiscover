#include "../../include/common.h"
#include "../../include/fs_annot.h"
#include "../fs_util.h"
#include "fs_annot_imp.h"
#include "../inc/fs_actionImp.h"
#include "../inc/fs_pdfdocImp.h"

FSPDFPopupImp::FSPDFPopupImp(CPDF_Dictionary* pAnnotDict, FSPDFPage* page)
{
	FSPDFAnnotImp::init(pAnnotDict, page); 
}
//////////////
// Methods inherited from FSAnnot
FS_BOOL			FSPDFPopupImp::move(FSRectF rect)
{
	// Popup doesn't have /AP, so no need to resetap after being moved.
	return FSPDFAnnotImp::moveImp((FSPopup*)this, rect, FALSE);
}

FS_BOOL FSPDFPopupImp::getOpenStatus() const
{
	return getBoolean(FSANNOT_BOOLEANKEY_OPEN);
}

void FSPDFPopupImp::setOpenStatus(FS_BOOL status)
{
	setBoolean(FSANNOT_BOOLEANKEY_OPEN, status);
}

