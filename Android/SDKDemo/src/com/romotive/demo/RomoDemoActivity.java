package com.romotive.demo;

import com.romotive.library.RomoCommandInterface;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;

/**
 * This Activity provides an example of how to use the RomoSDK in your own Android projects.
 * Simply drag and drop the com.romotive.library package and the libs folder into your project.
 * 
 * @author James Knight
 * @version 1.0, 4-15-2012
 * 
 * Copyright (c) 2012 Romotive, Inc. All rights reserved.
 */
public class RomoDemoActivity extends Activity 
{
	// Add an instance of RomoCommandInterface to your activity.
	RomoCommandInterface mCommandInterface;
	
    @Override
    /** 
     * Called when the activity is first created.
     * Initialize your RomoCommandInterface here.
     */
    public void onCreate(Bundle savedInstanceState) 
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        
        // Initialize your RomoCommandInterface:
        mCommandInterface = new RomoCommandInterface();
    }

    @Override
    /**
     * Called when the activity is destroyed.
     * Be sure to shutdown the RomoCommandInterface here!
     */
    public void onDestroy()
    {
    	super.onDestroy();
    	
    	// Shutdown the RomoCommandInterface:
    	mCommandInterface.shutdown();
    	mCommandInterface = null;
    }
   
    /**
     * Called when one of the directional buttons is pressed.
     * @param view	The Android view which received the onClick event.
     */
    public void buttonPressed(View view)
    {
    	// Switch on the view's ID:
    	switch (view.getId())
    	{
    	case R.id.l_f:
    		mCommandInterface.playMotorCommand(0xC0, 0xFF);
    		break;
    		
    	case R.id.f:
			mCommandInterface.playMotorCommand(0xFF, 0xFF);
    		break;
    		
    	case R.id.r_f:
    		mCommandInterface.playMotorCommand(0xFF, 0xC0);
    		break;

    	case R.id.l:
    		mCommandInterface.playMotorCommand(0x00, 0xFF);
    		break;
    		
    	case R.id.s:
    		mCommandInterface.playMotorCommand(0x80, 0x80);
    		break;
    		
    	case R.id.r:
    		mCommandInterface.playMotorCommand(0xFF, 0x00);
    		break;

    	case R.id.l_b:
    		mCommandInterface.playMotorCommand(0x40, 0x00);
    		break;
    		
    	case R.id.b:
    		mCommandInterface.playMotorCommand(0x00, 0x00);
    		break;
    		
    	case R.id.r_b:
    		mCommandInterface.playMotorCommand(0x00, 0x40);
    		break;
    	}
    }
}