/*
 * Copyright (C) 2011 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.google.android.DemoKit;

import java.io.FileDescriptor;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;

import android.app.Activity;
import android.app.PendingIntent;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.os.ParcelFileDescriptor;
import android.util.Log;
import android.widget.SeekBar;


public class DemoKitActivity extends Activity implements Runnable {
	private static final String TAG = "DemoKit";

	private static final String ACTION_USB_PERMISSION = "com.google.android.DemoKit.action.USB_PERMISSION";

	private PendingIntent mPermissionIntent;
	private boolean mPermissionRequestPending;

	ParcelFileDescriptor mFileDescriptor;
	FileInputStream mInputStream;
	FileOutputStream mOutputStream;

	private static final int MESSAGE_SWITCH = 1;
	private static final int MESSAGE_TEMPERATURE = 2;
	private static final int MESSAGE_LIGHT = 3;
	private static final int MESSAGE_JOY = 4;

	public static final byte LED_SERVO_COMMAND = 2;
	public static final byte RELAY_COMMAND = 3;

	protected class SwitchMsg {
		private byte sw;
		private byte state;

		public SwitchMsg(byte sw, byte state) {
			this.sw = sw;
			this.state = state;
		}

		public byte getSw() {
			return sw;
		}

		public byte getState() {
			return state;
		}
	}

	protected class TemperatureMsg {
		private int temperature;

		public TemperatureMsg(int temperature) {
			this.temperature = temperature;
		}

		public int getTemperature() {
			return temperature;
		}
	}

	protected class LightMsg {
		private int light;

		public LightMsg(int light) {
			this.light = light;
		}

		public int getLight() {
			return light;
		}
	}

	protected class JoyMsg {
		private int x;
		private int y;

		public JoyMsg(int x, int y) {
			this.x = x;
			this.y = y;
		}

		public int getX() {
			return x;
		}

		public int getY() {
			return y;
		}
	}


	/** Called when the activity is first created. */
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		setContentView(R.layout.main);

		enableControls(false);
	}

	@Override
	public Object onRetainNonConfigurationInstance() {
	
		return super.onRetainNonConfigurationInstance();
	
	}

	@Override
	public void onResume() {
		super.onResume();

		Intent intent = getIntent();
		if (mInputStream != null && mOutputStream != null) {
			return;
		}
	}

	@Override
	public void onPause() {
		super.onPause();
		closeAccessory();
	}

	@Override
	public void onDestroy() {
		
		super.onDestroy();
	}


	private void closeAccessory() {
		enableControls(false);

		try {
			if (mFileDescriptor != null) {
				mFileDescriptor.close();
			}
		} catch (IOException e) {
		} finally {
			mFileDescriptor = null;
			
		}
	}

	protected void enableControls(boolean enable) {
	}

	private int composeInt(byte hi, byte lo) {
		int val = (int) hi & 0xff;
		val *= 256;
		val += (int) lo & 0xff;
		return val;
	}

	public void run() {
		int ret = 0;
		byte[] buffer = new byte[16384];
		int i;

		while (ret >= 0) {
			try {
				ret = mInputStream.read(buffer);
			} catch (IOException e) {
				break;
			}

			i = 0;
			while (i < ret) {
				int len = ret - i;

				switch (buffer[i]) {
				case 0x1:
					if (len >= 3) {
						Message m = Message.obtain(mHandler, MESSAGE_SWITCH);
						m.obj = new SwitchMsg(buffer[i + 1], buffer[i + 2]);
						mHandler.sendMessage(m);
					}
					i += 3;
					break;

				case 0x4:
					if (len >= 3) {
						Message m = Message.obtain(mHandler,
								MESSAGE_TEMPERATURE);
						m.obj = new TemperatureMsg(composeInt(buffer[i + 1],
								buffer[i + 2]));
						mHandler.sendMessage(m);
					}
					i += 3;
					break;

				case 0x5:
					if (len >= 3) {
						Message m = Message.obtain(mHandler, MESSAGE_LIGHT);
						m.obj = new LightMsg(composeInt(buffer[i + 1],
								buffer[i + 2]));
						mHandler.sendMessage(m);
					}
					i += 3;
					break;

				case 0x6:
					if (len >= 3) {
						Message m = Message.obtain(mHandler, MESSAGE_JOY);
						m.obj = new JoyMsg(buffer[i + 1], buffer[i + 2]);
						mHandler.sendMessage(m);
					}
					i += 3;
					break;

				default:
					Log.d(TAG, "unknown msg: " + buffer[i]);
					i = len;
					break;
				}
			}

		}
	}

	Handler mHandler = new Handler() {
		@Override
		public void handleMessage(Message msg) {
			switch (msg.what) {
			case MESSAGE_SWITCH:
				SwitchMsg o = (SwitchMsg) msg.obj;
				handleSwitchMessage(o);
				break;

			case MESSAGE_TEMPERATURE:
				TemperatureMsg t = (TemperatureMsg) msg.obj;
				handleTemperatureMessage(t);
				break;

			case MESSAGE_LIGHT:
				LightMsg l = (LightMsg) msg.obj;
				handleLightMessage(l);
				break;

			case MESSAGE_JOY:
				JoyMsg j = (JoyMsg) msg.obj;
				handleJoyMessage(j);
				break;

			}
		}
	};

	public void sendCommand(byte command, byte target, int value) {
		byte[] buffer = new byte[3];
		if (value > 255)
			value = 255;

		buffer[0] = command;
		buffer[1] = target;
		buffer[2] = (byte) value;
		if (mOutputStream != null && buffer[1] != -1) {
			try {
				mOutputStream.write(buffer);
			} catch (IOException e) {
				Log.e(TAG, "write failed", e);
			}
		}
	}

	protected void handleJoyMessage(JoyMsg j) {
	}

	protected void handleLightMessage(LightMsg l) {
	}

	protected void handleTemperatureMessage(TemperatureMsg t) {
	}

	protected void handleSwitchMessage(SwitchMsg o) {
	}

	public void onStartTrackingTouch(SeekBar seekBar) {
	}

	public void onStopTrackingTouch(SeekBar seekBar) {
	}
}
