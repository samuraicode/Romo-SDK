//      
//       o      
//       | 
//  //=======\\
//  || o   o ||
//  ||       ||=>
//  ||  ---  ||
//  \\=======//
//
//  ROMOTIVE, INC
//  ResourceManager.cpp
//  A C++ class to get a resource file from an iOS app bundle.
//  http://romotive.com
//
//  Created by James Knight.
//  Last Revision Date: 3/5/2012
//  Copyright (c) 2012 Romotive, Inc. All rights reserved.
//

#include "ResourceManager.h"

ResourceManager *ResourceManager::mInstance = NULL;

ResourceManager *ResourceManager::getInstance()
{
    if (!mInstance)
        mInstance = new ResourceManager();
    
    return mInstance;
}

std::string ResourceManager::getResourcePath()
{
    return mResourcePath;
}

void ResourceManager::setResourcePath(std::string resourcePath)
{
    mResourcePath = resourcePath + "/";
}