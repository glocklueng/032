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

package org.openpnp.machine.reference;

import org.openpnp.spi.Feeder;

import com.thoughtworks.xstream.annotations.XStreamAsAttribute;

/**
 * A common base class for Feeders that the reference machine supports.
 * Provides support for additional configuration. 
 */
public abstract class ReferenceFeeder implements Feeder {
	@XStreamAsAttribute
	protected String id;
	
	public void start(ReferenceMachine machine) throws Exception {
		
	}

	@Override
	public String getId() {
		return id;
	}

	public void setReference(String id) {
		this.id = id;
	}
}
