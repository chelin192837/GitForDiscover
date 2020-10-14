#include "../include/fs_annot.h"
#include "fxcore.h"
#include "fs_util.h"
#include "fs_iconprovider.h"


void FSCallback_AnnotIconProvider_Free(FX_LPVOID pData)
{
	if (!pData) return;
	FSAnnotIconProvider* pIconProvider = (FSAnnotIconProvider*)pData;
	pIconProvider->release();
}

////////////////////////////////////////
// Class FSIconProviderManager
CFSPDF_IconProviderManager::CFSPDF_IconProviderManager() 
{
}

CFSPDF_IconProviderManager::~CFSPDF_IconProviderManager()
{
	FSCRT_LOCKOBJECT(lock, &m_lock);
	m_PrivateData.ClearAll();
}

FS_BOOL CFSPDF_IconProviderManager::SetAnnotIconProvider(FSAnnotIconProvider* newIconProvider)
{
	FSCRT_LOCKOBJECT(lock, &m_lock);
	m_PrivateData.SetPrivateData((FX_LPVOID)FSPDF_ICONPROVIDERTYPE_ANNOT, newIconProvider, newIconProvider?FSCallback_AnnotIconProvider_Free:NULL);
	return TRUE;
}

FSAnnotIconProvider* CFSPDF_IconProviderManager::GetAnnotIconProvider()
{
	FSCRT_LOCKOBJECT(lock, &m_lock);

	return (FSAnnotIconProvider*)m_PrivateData.GetPrivateData((FX_LPVOID)FSPDF_ICONPROVIDERTYPE_ANNOT);
}

