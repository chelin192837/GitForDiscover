#include "../../../inc/fs_precompilers.h"
#if defined(_FSDK_ALL_) || defined(_FSDK_PDFFORM_) || defined(_FSDK_PDFFORM_R_) || defined(_FSDK_PDFFORM_W_)

#include "../include/fxedit/fxet_stub.h"
#include "../include/fxedit/fx_edit.h"
#include "../include/fxedit/fxet_edit.h"
#include "../include/fxedit/fxet_list.h"

/* ---------------------- IFX_Edit ---------------------- */

IFX_Edit* IFX_Edit::NewEdit()
{
	if (IPDF_VariableText * pVT = IPDF_VariableText::NewVariableText())
	{
		return new CFX_Edit(pVT);
	}

	return NULL;
}

void IFX_Edit::DelEdit(IFX_Edit* pEdit)
{
	ASSERT(pEdit != NULL);

	IPDF_VariableText::DelVariableText(pEdit->GetVariableText());

	delete (CFX_Edit*)pEdit;
}


/* ---------------------- IFX_List ---------------------- */

IFX_List* IFX_List::NewList()
{
	return new CFX_ListCtrl();
}

void IFX_List::DelList(IFX_List* pList)
{
	ASSERT(pList != NULL);

	delete (CFX_ListCtrl*)pList;
}

#endif //defined(_FSDK_ALL_) || defined(_FSDK_PDFFORM_) || defined(_FSDK_PDFFORM_R_) || defined(_FSDK_PDFFORM_W_)