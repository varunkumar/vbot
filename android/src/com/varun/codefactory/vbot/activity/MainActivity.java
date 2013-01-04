package com.varun.codefactory.vbot.activity;

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
import android.os.ParcelFileDescriptor;
import android.text.method.LinkMovementMethod;
import android.util.Log;
import android.view.View;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.TextView;

import com.android.future.usb.UsbAccessory;
import com.android.future.usb.UsbManager;
import com.varun.codefactory.vbot.R;

public class MainActivity extends Activity {
	private static final String TAG = "VBot";
	private static final String TAG_USB = TAG + " - USB";
	
	/** 
	 * Constants used for Permissions
	 */
    private static final String ACTION_USB_PERMISSION = "com.varun.codefactory.vbot.action.USB_PERMISSION";
    
    /**
     * Constants used for sending commands to the bot
     */
	protected static final byte COMMAND_FORWARD = 1;
	protected static final byte COMMAND_BACKWARD = 2;
	protected static final byte COMMAND_LEFT = 3;
	protected static final byte COMMAND_RIGHT = 4;
	protected static final byte COMMAND_STRETCH = 5;
	protected static final byte COMMAND_DANCE = 6;
    
    /**
	 *  Members related to USB Accessory API
	 */
    private UsbManager mUsbManager;
	private PendingIntent mPermissionIntent;
	private boolean mPermissionRequestPending;
	private UsbAccessory mAccessory;
	private ParcelFileDescriptor mFileDescriptor;
	private FileInputStream mInputStream;
	private FileOutputStream mOutputStream;
	
	/**
	 * Members related to the bot
	 */
	private boolean botConnected = false;
	
	public boolean isBotConnected() {
		return botConnected;
	}
	
	/**
	 * BroadcastReceiver for listening to USB events. 
	 */
	private final BroadcastReceiver mUsbReceiver = new BroadcastReceiver() {
		@Override
		public void onReceive(Context context, Intent intent) {
			String action = intent.getAction();
			if (ACTION_USB_PERMISSION.equals(action)) {
				synchronized (this) {
					UsbAccessory accessory = UsbManager.getAccessory(intent);
					if (intent.getBooleanExtra(
							UsbManager.EXTRA_PERMISSION_GRANTED, false)) {
						openAccessory(accessory);
					} else {
						Log.d(TAG_USB, "Permission denied for accessory "
								+ accessory);
					}
					mPermissionRequestPending = false;
				}
			} else if (UsbManager.ACTION_USB_ACCESSORY_DETACHED.equals(action)) {
				UsbAccessory accessory = UsbManager.getAccessory(intent);
				if (accessory != null && accessory.equals(mAccessory)) {
					closeAccessory();
				}
			}
		}
	};
	
	/** 
	 * Called when the activity is first created. 
	 */
    @SuppressWarnings("deprecation")
	@Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        
        // Initializing USB manager and registering the broadcast receiver 
        mUsbManager = UsbManager.getInstance(this);
		mPermissionIntent = PendingIntent.getBroadcast(this, 0, new Intent(
				ACTION_USB_PERMISSION), 0);
		IntentFilter filter = new IntentFilter(ACTION_USB_PERMISSION);
		filter.addAction(UsbManager.ACTION_USB_ACCESSORY_DETACHED);
		registerReceiver(mUsbReceiver, filter);

		if (getLastNonConfigurationInstance() != null) {
			mAccessory = (UsbAccessory) getLastNonConfigurationInstance();
			openAccessory(mAccessory);
		}

		// Loading the UI layout
        setContentView(R.layout.main);
        
        // Making the link clickable in the label
        TextView txtDetails = (TextView) findViewById(R.id.txtDetails);
        txtDetails.setMovementMethod(LinkMovementMethod.getInstance());
        
        updateBotStatusInUI();
		
		// Preventing the screen from locking when the activity is active
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
        
        final Button btnForward = (Button) findViewById(R.id.btnForward);
        btnForward.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
            	byte command = COMMAND_FORWARD;
            	sendCommand(command);
            }
        });
        
        final Button btnBackward = (Button) findViewById(R.id.btnBackward);
        btnBackward.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
            	byte command = COMMAND_BACKWARD;
            	sendCommand(command);
            }
        });
        
        final Button btnLeft = (Button) findViewById(R.id.btnLeft);
        btnLeft.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
            	byte command = COMMAND_LEFT;
            	sendCommand(command);
            }
        });
        
        final Button btnRight = (Button) findViewById(R.id.btnRight);
        btnRight.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
            	byte command = COMMAND_RIGHT;
            	sendCommand(command);
            }
        });
        
        final Button btnStretch = (Button) findViewById(R.id.btnStretch);
        btnStretch.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
            	byte command = COMMAND_STRETCH;
            	sendCommand(command);
            }
        });
        
        final Button btnDance = (Button) findViewById(R.id.btnDance);
        btnDance.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
            	// Play song
            	
            	byte command = COMMAND_DANCE;
            	sendCommand(command);
            }
        });
    }
    
    public static Intent createIntent(Context context) {
        Intent i = new Intent(context, MainActivity.class);
        i.addFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
        return i;
    }
    
    @SuppressWarnings("deprecation")
	@Override
	public Object onRetainNonConfigurationInstance() {
		if (mAccessory != null) {
			return mAccessory;
		} else {
			return super.onRetainNonConfigurationInstance();
		}
	}
    
    /**
     * Called when the activity gets back the focus
     */
    @Override
	public void onResume() {
		super.onResume();

		Log.d(TAG, "onResume()");
		if (mInputStream != null && mOutputStream != null) {
			return;
		}

		Log.d(TAG_USB, "Getting accessory list.. ");
		UsbAccessory[] accessories = mUsbManager.getAccessoryList();
		UsbAccessory accessory = (accessories == null ? null : accessories[0]);
		if (accessory != null) {
			Log.d(TAG_USB, "Accessory found");
			if (mUsbManager.hasPermission(accessory)) {
				Log.d(TAG_USB, "Has access");
				openAccessory(accessory);
			} else {
				synchronized (mUsbReceiver) {
					if (!mPermissionRequestPending) {
						mUsbManager.requestPermission(accessory,
								mPermissionIntent);
						mPermissionRequestPending = true;
					}
				}
			}
		} else {
			Log.d(TAG_USB, "Accessory is null");
		}
    }
    
    @Override
	public void onPause() {
		super.onPause();
		Log.d(TAG, "onPause()");
		closeAccessory();
	}

	@Override
	public void onDestroy() {
		unregisterReceiver(mUsbReceiver);
		Log.d(TAG, "onDestroy()");
		super.onDestroy();
	}
    
	/**
	 * Opens the accessory which we have just acquired the permission for reading / writing.
	 * @param accessory
	 */
    private void openAccessory(UsbAccessory accessory) {
		mFileDescriptor = mUsbManager.openAccessory(accessory);
		if (mFileDescriptor != null) {
			mAccessory = accessory;
			FileDescriptor fd = mFileDescriptor.getFileDescriptor();
			mInputStream = new FileInputStream(fd);
			mOutputStream = new FileOutputStream(fd);
			Log.d(TAG_USB, "Accessory opened");
			
			botConnected = true;
			updateBotStatusInUI();
		} else {
			Log.d(TAG_USB, "Accessory open fail");
		}
	}

    /**
     * Closes the accessory when the activity loses its focus
     */
	private void closeAccessory() {
		try {
			if (mFileDescriptor != null) {
				mFileDescriptor.close();
				
				botConnected = false;
				updateBotStatusInUI();
			}
		} catch (IOException e) {
		} finally {
			mInputStream = null;
			mOutputStream = null;
			mFileDescriptor = null;
			mAccessory = null;
		}
	}
	
	private void updateBotStatusInUI() {
		TextView txtStatus = (TextView) findViewById(R.id.txtStatus);
		
		if (isBotConnected()) {
			txtStatus.setText(R.string.txtStatusOnline);
			txtStatus.setCompoundDrawablesWithIntrinsicBounds(null, null, getResources().getDrawable(R.drawable.online), null);
		} else {
			txtStatus.setText(R.string.txtStatusOffline);
			txtStatus.setCompoundDrawablesWithIntrinsicBounds(null, null, getResources().getDrawable(R.drawable.offline), null);
		}
	}

	/**
	 * Sends command to the bot 
	 * @param command
	 */
	public void sendCommand(byte command) {
		byte[] buffer = new byte[1];
		
		buffer[0] = command;
		if (mOutputStream != null) {
			try {
				mOutputStream.write(buffer);
			} catch (IOException ex) {
				Log.e(TAG_USB, "Error sending command to the device");
			}
		}
	}
}