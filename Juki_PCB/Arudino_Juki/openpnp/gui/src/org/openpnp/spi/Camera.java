/*
 	Copyright (C) 2011 Jason von Nieda <jason@vonnieda.org>
 	
 	This file is part of OpenPnP.
 	
	OpenPnP is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenPnP is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with OpenPnP.  If not, see <http://www.gnu.org/licenses/>.
 	
 	For more information about OpenPnP visit http://openpnp.org
 */

package org.openpnp.spi;

import java.awt.image.BufferedImage;

import org.openpnp.CameraListener;
import org.openpnp.Location;

/**
 * Represents a Camera attached to the system and allows a caller to retrieve images from it.
 * This is a simple abstract that is expected to grow as the system's image processing becomes
 * more full featured. A caller can call capture() one time to get the dimensions and details
 * about the returned image. The Camera is expected to return all future images using
 * the same dimensions and type. 
 */
public interface Camera {
	public enum Looking {
		Down,
		Up
	}
	
	/**
	 * Get the name of the Camera as defined by it's configuration.
	 * @return
	 */
	public String getName();
	
	/**
	 * Get the Location of the Camera in relation to either the Head it's attached to
	 * or the Machine itself.
	 * @return
	 */
	public Location getLocation();
	
	/**
	 * Get the direction the Camera is looking. 
	 * @return
	 */
	public Looking getLooking();
	
	/**
	 * The number of X and Y units per pixel this camera shows when in perfect focus.
	 * Location isn't a great datatype for this, but it gets the job done.
	 * @return
	 */
	public Location getUnitsPerPixel();
	
	/**
	 * Get the Head the Camera is attached to. If the Camera is stationary this should return
	 * null.
	 * @return
	 */
	public Head getHead();
	
	/**
	 * Immediately captures an image from the camera and returns it in it's native format.
	 * @return
	 */
	public BufferedImage capture();
	
	/**
	 * Registers a listener to receive continuous images from the camera at a rate less than
	 * or equal to maximumFps images per second.
	 * @param listener
	 * @param maximumFps
	 */
	public void startContinuousCapture(CameraListener listener, int maximumFps);
	
	/**
	 * Requests that the continuous capture be stopped for the previously registered listener. If the
	 * Camera has other listeners they should still receive updates.
	 * @param listener
	 */
	public void stopContinuousCapture(CameraListener listener);
}
