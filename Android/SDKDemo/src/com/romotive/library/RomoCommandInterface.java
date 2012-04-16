package com.romotive.library;

/**
 * This class loads the libromo.so file and acts as an interface to the native layer.
 * 
 * @author James Knight
 * @version 1.0, 4-15-2012
 * 
 * Copyright (c) 2012 Romotive, Inc. All rights reserved.
 */
public class RomoCommandInterface
{
	// Load the Native Romo library:
	static {
		System.loadLibrary("romo");
	}

	/**
	 * No initialization is needed for the Interface, so this constructor is empty.
	 */
	public RomoCommandInterface()
	{
		// No initialization is needed.
	}
	
	/**
	 * Call this method when you're finished with the Romo Audio library.
	 */
	public void shutdown()
	{
		nativeShutdown();
	}
	
	/**
	 * This method handles the shutdown of the library on the Native side.
	 */
    private native void nativeShutdown();
    
    /**
     * Passes the motor command values down to the native layer for buffering and playing.
     * 0 (0x00) is full reverse, 255 (0xFF) is full forward, and 128 (0x80) is stop.
     * @param leftValue		The power to set the left motor to. 
     * @param rightValue	The value to set the right motor to.
     */
	public native void playMotorCommand(int leftValue, int rightValue);
	
	/**
	 * Passes the auxiliary port command values down to the native layer for buffering and playing.
	 * @param auxValue	
	 */
	public native void playAuxCommand(int auxValue);
}
