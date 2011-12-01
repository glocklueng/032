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

package org.openpnp;

import java.io.File;
import java.io.FileInputStream;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.Collection;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.xpath.XPath;
import javax.xml.xpath.XPathConstants;
import javax.xml.xpath.XPathFactory;

import org.openpnp.Part.FeederLocation;
import org.openpnp.machine.reference.ReferenceCamera;
import org.openpnp.machine.reference.ReferenceDriver;
import org.openpnp.machine.reference.ReferenceFeeder;
import org.openpnp.machine.reference.ReferenceHead;
import org.openpnp.machine.reference.ReferenceMachine;
import org.openpnp.machine.reference.camera.LtiCivilCamera;
import org.openpnp.machine.reference.camera.VfwCamera;
import org.openpnp.machine.reference.driver.GrblDriver;
import org.openpnp.machine.reference.feeder.ReferenceTapeFeeder;
import org.openpnp.machine.reference.feeder.ReferenceTrayFeeder;
import org.openpnp.spi.Machine;
import org.w3c.dom.Document;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.xml.sax.InputSource;

import com.thoughtworks.xstream.XStream;
import com.thoughtworks.xstream.annotations.XStreamAlias;
import com.thoughtworks.xstream.converters.reflection.PureJavaReflectionProvider;
import com.thoughtworks.xstream.io.xml.DomDriver;

public class Configuration {
	private Map<String, PackageDef> packages = new HashMap<String, PackageDef>();
	private Map<String, Part> parts = new HashMap<String, Part>();
	private Machine machine;
	
	public Configuration(String configurationDirectory) throws Exception {
		loadMachine(new File(new File(configurationDirectory), "openpnp.xml"));
		loadPackages(new File(new File(configurationDirectory), "packages.xml"));
		loadParts(new File(new File(configurationDirectory), "parts.xml"));
		
		// TODO: move this into the GUI so we can respond to errors with dialogs
		machine.start();
	}
	
	public PackageDef getPackage(String ref) {
		return packages.get(ref);
	}
	
	public Part getPart(String ref) {
		return parts.get(ref);
	}
	
	public Collection<Part> getParts() {
		return parts.values();
	}
	
	public Machine getMachine() {
		return machine;
	}
	
	private void loadPackages(File file) throws Exception {
        Document document = loadDocument(new FileInputStream(file));
        
		XPath xpath = XPathFactory.newInstance().newXPath();

		NodeList nodes = (NodeList) xpath.evaluate("/Packages/Package", document, XPathConstants.NODESET);

		for (int i = 0; i < nodes.getLength(); i++) {
			Node n = nodes.item(i);
			PackageDef pkg = new PackageDef();

			pkg.setReference(getAttribute(n, "reference"));
			pkg.setName(getAttribute(n, "name"));
			pkg.setOutline(new Outline());
			pkg.getOutline().parse((Node) xpath.evaluate("Outline", n, XPathConstants.NODE));
			
			packages.put(pkg.getReference(), pkg);
		}
	}
	
	private void loadParts(File file) throws Exception {
        Document document = loadDocument(new FileInputStream(file));
        
		XPath xpath = XPathFactory.newInstance().newXPath();

		NodeList nodes = (NodeList) xpath.evaluate("/Parts/Part", document, XPathConstants.NODESET);

		for (int i = 0; i < nodes.getLength(); i++) {
			Node n = nodes.item(i);
			Part part = new Part();
			
			part.setReference(getAttribute(n, "reference"));
			part.setName(getAttribute(n, "name"));
			part.setHeight(Double.valueOf(n.getAttributes().getNamedItem("height").getNodeValue()));
			part.setHeightUnits(LengthUnit.valueOf(n.getAttributes().getNamedItem("height-units").getNodeValue()));
			part.setPackage(getPackage(n.getAttributes().getNamedItem("package").getNodeValue()));
				
			parts.put(part.getReference(), part);
			
			NodeList feederLocationNodes = (NodeList) xpath.evaluate("FeederLocations/FeederLocation", n, XPathConstants.NODESET);
			part.setFeederLocations(loadFeederLocations(feederLocationNodes));
		}
	}
	
	private void loadMachine(File file) throws Exception {
		XStream xstream = new XStream(new PureJavaReflectionProvider(), new DomDriver());
		xstream.autodetectAnnotations(true);
		xstream.setMode(XStream.SINGLE_NODE_XPATH_RELATIVE_REFERENCES);
		xstream.processAnnotations(ReferenceMachine.class);
		xstream.processAnnotations(ReferenceCamera.class);
		xstream.processAnnotations(ReferenceFeeder.class);
		xstream.processAnnotations(ReferenceHead.class);
		xstream.processAnnotations(ReferenceDriver.class);
		xstream.processAnnotations(GrblDriver.class);
		xstream.processAnnotations(LtiCivilCamera.class);
		xstream.processAnnotations(VfwCamera.class);
		xstream.processAnnotations(ReferenceTrayFeeder.class);
		xstream.processAnnotations(ReferenceTapeFeeder.class);
		xstream.processAnnotations(MachineConfigurationHolder.class);
		MachineConfigurationHolder holder = (MachineConfigurationHolder) xstream.fromXML(file);
		machine = holder.machine;
		
		System.out.println(xstream.toXML(holder));
	}
	
	private List<FeederLocation> loadFeederLocations(NodeList nodes) throws Exception {
		XPath xpath = XPathFactory.newInstance().newXPath();
		
		List<FeederLocation> feederLocations = new ArrayList<FeederLocation>();
		for (int i = 0; i < nodes.getLength(); i++) {
			Node n = nodes.item(i);
			FeederLocation feederLocation = new FeederLocation();
			
			feederLocation.setFeeder(machine.getFeeder(Configuration.getAttribute(n, "feeder")));
			
			Location location = new Location();
			location.parse((Node) xpath.evaluate("Location", n, XPathConstants.NODE));
			feederLocation.setLocation(location);
			
			feederLocations.add(feederLocation);
		}
		return feederLocations; 
	}
	
	public static Document loadDocument(InputStream in) throws Exception {
        return DocumentBuilderFactory.newInstance().newDocumentBuilder().parse( 
        		new InputSource(in));
	}
	
	public static String getAttribute(Node n, String attribute) {
		return getAttribute(n, attribute, null);
	}
	
	public static String getAttribute(Node n, String attribute, String def) {
		Node node = n.getAttributes().getNamedItem(attribute);
		if (node == null) {
			return def;
		}
		return node.getNodeValue();
	}
	
	public static double getDoubleAttribute(Node n, String attribute) {
		return getDoubleAttribute(n, attribute, 0);
	}
	
	public static double getDoubleAttribute(Node n, String attribute, double def) {
		String s = getAttribute(n, attribute);
		if (s == null) {
			return def;
		}
		return Double.valueOf(s);
	}
	
	public static boolean getBooleanAttribute(Node n, String attribute) {
		return getBooleanAttribute(n, attribute, false);
	}
	
	public static boolean getBooleanAttribute(Node n, String attribute, boolean def) {
		String s = getAttribute(n, attribute);
		if (s == null) {
			return def;
		}
		return Boolean.valueOf(s);
	}
	
	public static LengthUnit getLengthUnitAttribute(Node n, String attribute) {
		return LengthUnit.valueOf(n.getAttributes().getNamedItem(attribute).getNodeValue());
	}
	
	@XStreamAlias(value="OpenPnP")
	public static class MachineConfigurationHolder {
		@XStreamAlias(value="Machine")
		private Machine machine;
	}
}
