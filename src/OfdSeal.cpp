//
//  OfdSeal.cpp
//  iosrdk_platform
//
//  Created by mac on 2020/9/27.
//  Copyright © 2020 FoxitInc. All rights reserved.
//

#include "OfdSeal.hpp"
#include "fs_ofdsignaturefun.h"
#include "oes.h"
#include "fs_selimage.hpp"

#include "ofd/include/ofd_base_r.h"

#include <string>

class init_fini{
    
public:
    init_fini(OFD_LPCSTR lpszPin = "", OFD_LPCSTR lpszSeal = ""){
        
        
        
//        if(lpszPin == NULL || lpszSeal == NULL)
//            return;
//        OFD_LPCSTR wsOESFile = "libfxoes.so";
//        //OFD_LPCSTR wsOESFile = "liboes.so";
//        if(strlen(lpszPin)) {
//            if (init_fini::lpszPin != NULL)
//                delete[] init_fini::lpszPin;
//            init_fini::lpszPin = new char[strlen(lpszPin)];
//            strcpy(init_fini::lpszPin, lpszPin);
//        }
//        if(strlen(lpszSeal)) {
//            if (init_fini::lpszSeal != NULL)
//                delete[] init_fini::lpszSeal;
//            init_fini::lpszSeal = new char[strlen(lpszSeal)];
//            strcpy(init_fini::lpszSeal, lpszSeal);
//        }
//        //LOGD("lpszPin:%ds", init_fini::lpszPin);
//        OFD_SEAL_Init( wsOESFile, init_fini::lpszPin);
//        int ierr = 0;
//        m_sealInfo = (FS_SEAL_INFO*)OFD_SEAL_Load( init_fini::lpszSeal, 1, 1, init_fini::lpszPin, &ierr);
//        if(m_sealInfo)
//        {
//            //LOGD("width:%d,height:%d", m_sealInfo->width, m_sealInfo->height);
//            //LOGD("digestmethod:%s",m_sealInfo->digestMethod.GetBuffer(20));
//        }
//        else
//            ;//LOGD("OFD_SEAL_Load:%d", ierr);
        
        
        
    }
    FS_SEAL_INFO* m_sealInfo;
    static OFD_LPSTR lpszPin;
    static OFD_LPSTR lpszSeal;
    ~init_fini(){
        OFD_SEAL_Destory();
        //LOGD("OFD_SEAL_Destory");
    }
};

void* getSealImage(std::string password , std::string sealID){
    init_fini seal_env("","");
    FX_LPBYTE pPictureData;
    FX_LPBYTE pPictureType;
    FX_INT32 pictureDataLen;
    FX_INT32 pictureTypeLen;
    FS_GetSealImage_raw(seal_env.m_sealInfo, pPictureData, pPictureType, pictureDataLen, pictureTypeLen);
    FSSealImage * ret = new FSSealImage;
    ret->height =seal_env.m_sealInfo->height;
    ret->width = seal_env.m_sealInfo->width;
//    ret->bitmap = pPictureData;
    return ret;
}
