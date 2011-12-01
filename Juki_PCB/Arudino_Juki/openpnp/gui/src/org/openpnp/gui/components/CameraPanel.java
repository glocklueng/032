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

package org.openpnp.gui.components;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Component;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.util.LinkedHashSet;
import java.util.Set;

import javax.swing.AbstractAction;
import javax.swing.JComboBox;
import javax.swing.JPanel;

import org.openpnp.gui.support.CameraItem;
import org.openpnp.spi.Camera;

/**
 * Shows a square grid of cameras or a blown up image from a single camera.
 * 
 * @author jason TODO add a way to identify a particular camera in grid view;
 *         maybe tooltip
 */
@SuppressWarnings("serial")
public class CameraPanel extends JPanel {
	private static int maximumFps = 15;

	private Set<Camera> cameras = new LinkedHashSet<Camera>();

	private JComboBox camerasCombo;
	private JPanel camerasPanel;

	public CameraPanel() {
		createUi();

		camerasCombo.setSelectedIndex(1);
	}

	public void addCamera(Camera camera) {
		cameras.add(camera);
		camerasCombo.addItem(new CameraItem(camera));
		camerasCombo.setSelectedIndex(camerasCombo.getSelectedIndex());
	}
	
	private void createUi() {
		camerasPanel = new JPanel();

		camerasCombo = new JComboBox();
		camerasCombo.addActionListener(new CameraSelectedAction());
		camerasCombo.addItem("Show None");
		camerasCombo.addItem("Show All");

		setLayout(new BorderLayout());

		add(camerasCombo, BorderLayout.NORTH);
		add(camerasPanel);
	}

	class CameraSelectedAction extends AbstractAction {
		@Override
		public void actionPerformed(ActionEvent ev) {
			if (camerasCombo.getSelectedItem().equals("Show None")) {
				clearCameras();
				camerasPanel.setLayout(new BorderLayout());
				JPanel panel = new JPanel();
				panel.setBackground(Color.black);
				camerasPanel.add(panel);
			}
			else if (camerasCombo.getSelectedItem().equals("Show All")) {
				clearCameras();
				int columns = (int) Math.ceil(Math.sqrt(cameras.size()));
				if (columns == 0) {
					columns = 1;
				}
				camerasPanel.setLayout(new GridLayout(0, columns, 1, 1));
				for (Camera camera : cameras) {
					CameraView cameraView = new CameraView(maximumFps
							/ cameras.size());
					cameraView.setCamera(camera);
					camerasPanel.add(cameraView);
				}
				for (int i = 0; i < (columns * columns) - cameras.size(); i++) {
					JPanel panel = new JPanel();
					panel.setBackground(Color.black);
					camerasPanel.add(panel);
				}
			}
			else {
				clearCameras();
				camerasPanel.setLayout(new BorderLayout());
				CameraView cameraView = new CameraView(maximumFps);
				cameraView.setCamera(((CameraItem) camerasCombo
						.getSelectedItem()).getCamera());
				camerasPanel.add(cameraView);
			}
			revalidate();
			repaint();
		}
		
		private void clearCameras() {
			for (Component comp : camerasPanel.getComponents()) {
				if (comp instanceof CameraView) {
					((CameraView) comp).setCamera(null);
				}
			}
			camerasPanel.removeAll();
		}
	}
}
