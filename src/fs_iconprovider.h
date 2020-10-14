#ifndef _FS_ICONPROVIDER_H_
#define _FS_ICONPROVIDER_H_

#include "fxcore.h"
#include "../include/common.h"
#include "../include/fs_annot.h"

class FSAnnotIconProvider;

class CFSPDF_IconProviderManager;
class CFSPDF_DocIconProvider;

#ifndef _FSPDF_DEF_MACRO_ICONPROVIDERTYPE_
#define _FSPDF_DEF_MACRO_ICONPROVIDERTYPE_

#define	FSPDF_ICONPROVIDERTYPE_ANNOT		0x100
#define	FSPDF_ICONPROVIDERTYPE_SIGNATURE	0x200

#endif //_FSPDF_DEF_MACRO_ICONPROVIDERTYPE_

class CFSPDF_IconProviderManager: public CFX_Object
{
public:
	CFSPDF_IconProviderManager();
	~CFSPDF_IconProviderManager();

	FS_BOOL					SetAnnotIconProvider(FSAnnotIconProvider* newIconProvider);
	FSAnnotIconProvider*	GetAnnotIconProvider();


private:
	FSCRT_DEFINELOCK(m_lock);
	CFX_PrivateData			m_PrivateData;
};

#endif //_FS_ICONPROVIDER_H_