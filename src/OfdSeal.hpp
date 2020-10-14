//
//  OfdSeal.hpp
//  iosrdk_platform
//
//  Created by mac on 2020/9/27.
//  Copyright © 2020 FoxitInc. All rights reserved.
//

#ifndef OfdSeal_hpp
#define OfdSeal_hpp

#include <stdio.h>
#include <string>
//void* FSSealImage::getSealImage(string password , string sealID);

void* getSealImage(std::string password , std::string sealID);

#endif /* OfdSeal_hpp */
