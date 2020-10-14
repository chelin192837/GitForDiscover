#ifndef _OFD_QT_H_
#define _OFD_QT_H_

#include "../include/ofd_base_r.h"

// External classes
class IFX_FileRead;
class IFX_FileWrite;
class IFX_MemoryStream;

// Internal interface

// QTSDK
OFD_PACKAGE	OFD_Package_CreateFileWrite(IFX_FileWrite* pFileWrite, FX_BOOL bTakeover);
IFX_FileWrite* OFD_Package_GetFileWrite(OFD_PACKAGE hPackage);


#endif //_OFD_QT_H_
