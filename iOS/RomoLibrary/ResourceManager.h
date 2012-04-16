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
//  ResourceManager.h
//  An C++ class to get a resource file from an iOS app bundle.
//  http://romotive.com
//
//  Created by James Knight.
//  Last Revision Date: 3/5/2012
//  Copyright (c) 2012 Romotive, Inc. All rights reserved.
//

#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include "RomoInclude.h"
#include <string>

class ResourceManager
{
private:
    std::string mResourcePath;
    static ResourceManager *mInstance;
    
    ResourceManager() {};
    
public:
    static ResourceManager *getInstance();
    
    ~ResourceManager() {};
    
    void setResourcePath(std::string resourcePath);
    std::string getResourcePath();
};

#endif
