#ifndef OES_DLL_GLOBAL_H
#define OES_DLL_GLOBAL_H

//#include <QtCore/qglobal.h>

#if defined(OES_DLL_LIBRARY)
#  define OES_DLLSHARED_EXPORT Q_DECL_EXPORT
#else
#  define OES_DLLSHARED_EXPORT Q_DECL_IMPORT
#endif

#define    _T(x)       L##x


#endif // OES_DLL_GLOBAL_H
