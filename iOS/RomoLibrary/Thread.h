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
//  Thread.h
//  A C++ class to create a new thread.
//  http://romotive.com
//
//  Created by James Knight.
//  Last Revision Date: 1/20/2012
//  Copyright (c) 2012 Romotive, Inc. All rights reserved.
//

#ifndef THREAD_H
#define	THREAD_H

#include "RomoInclude.h"

class Thread 
{
public:
    enum ThreadState
    {
        THREAD_NEW      = 0,
        THREAD_RUNNING  = 1,
        THREAD_PAUSED   = 2,
        THREAD_STOPPED  = 3,
        THREAD_FINISHED = 4
    };
    
protected:    
    ThreadState mState;
    pthread_t mHandle;
    
    Thread();
    virtual ~Thread();
    
public:    
    void start();
    void shutdown();
    void finish();
    
    virtual void run() = 0;
    
    bool getStatus();
};

#endif	/* THREAD_H */

