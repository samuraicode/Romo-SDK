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
//  NativeInterface.h
//  C++ header file to abstract interfacing between ObjC, Java and C++.
//  http://romotive.com
//
//  Created by James Knight.
//  Last Revision Date: 1/20/2012
//  Copyright (c) 2012 Romotive, Inc. All rights reserved.
//

#ifndef NativeNetwokTest_NativeInterface_h
#define NativeNetwokTest_NativeInterface_h

@class NativeManager;
class AudioManager;

class NativeInterface
{
private:
    NativeManager *mNativeManager;

protected:
    AudioManager *mAudioManager;
    
public:
    inline void setManager(NativeManager *nativeManager)
    {
        mNativeManager = nativeManager;
    }
    
    inline void setPlatformInterface(NativeManager *nativeManager)
    {
        mNativeManager = nativeManager;
    }
    
    void initialize();
    void shutdown();
    
    void setResourcePath(std::string resourcePath);
       
    void playSoundFile(std::string filename);
    void playMotorCommand(uint8_t leftValue, uint8_t rightValue);
    void playAuxCommand(uint8_t auxValue);
   
};

#endif
