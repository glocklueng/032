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

package org.openpnp.machine.reference.feeder;

import org.openpnp.Location;
import org.openpnp.Part;
import org.openpnp.machine.reference.ReferenceFeeder;
import org.openpnp.machine.reference.ReferenceHead;
import org.openpnp.spi.Head;

import com.thoughtworks.xstream.annotations.XStreamAlias;
import com.thoughtworks.xstream.annotations.XStreamAsAttribute;
import com.thoughtworks.xstream.annotations.XStreamOmitField;

/**
 * Implemention of Feeder that indexes based on an offset. This allows a tray of parts to be picked from without moving any tape.
 * Can handle trays of arbitrary X and Y count.
<pre>
{@code
<Configuration trayCountX="10" trayCountY="2">
	<Offsets units="Millimeters" x="10" y="10" z="0" rotation="0"/>
</Configuration>
}
</pre>
 */
public class ReferenceTrayFeeder extends ReferenceFeeder {
	@XStreamAsAttribute
	@XStreamAlias(value="tray-count-x")
	private int trayCountX;
	@XStreamAsAttribute
	@XStreamAlias(value="tray-count-y")
	private int trayCountY;
	@XStreamAlias(value="Offsets")
	private Location offsets;  
	
	@XStreamOmitField
	private int pickCount;
	
	@Override
	public boolean canFeedForHead(Part part, Head head) {
		return (pickCount < (trayCountX * trayCountY));
	}

	public Location feed(Head head_, Part part, Location pickLocation) throws Exception {
		ReferenceHead head = (ReferenceHead) head_;
		
		int partX = (pickCount / trayCountX);
		int partY = (pickCount - (partX * trayCountX));
		
		Location l = new Location();
		l.setX(pickLocation.getX() + (partX * offsets.getX()));
		l.setY(pickLocation.getY() + (partY * offsets.getY()));
		l.setZ(pickLocation.getZ());
		l.setRotation(pickLocation.getRotation());
		l.setUnits(pickLocation.getUnits());
		
		System.out.println(String.format("Feeding part # %d, x %d, y %d, xPos %f, yPos %f", pickCount, partX, partY, l.getX(), l.getY()));
		
		pickCount++;
		
		return l; 
	}
}
