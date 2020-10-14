#ifndef _OFD_RENDERER_APPLE_R_H_
#define _OFD_RENDERER_APPLE_R_H_

#include "../ofd_base_r.h"

#ifdef __cplusplus
extern "C" {
#endif

OFD_RENDERER		OFD_Renderer_CreateOnCGContext(CGContextRef context, int deviceClass);

#ifdef __cplusplus
};
#endif

#endif // _OFD_RENDERER_APPLE_R_H_
