package com.eec136b.biosnap.biosnapapp;

import android.Manifest;
import android.app.Activity;
import android.app.AlertDialog;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothManager;
import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.ServiceConnection;
import android.content.SharedPreferences;
import android.content.pm.PackageManager;
import android.graphics.Color;
import android.net.Uri;
import android.os.Build;
import android.os.Environment;
import android.os.Handler;
import android.os.IBinder;
import android.support.annotation.NonNull;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.support.v7.widget.Toolbar;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.TextView;
import android.widget.Toast;

import com.jjoe64.graphview.GraphView;
import com.jjoe64.graphview.LegendRenderer;
import com.jjoe64.graphview.series.DataPoint;
import com.jjoe64.graphview.series.LineGraphSeries;

import org.w3c.dom.Text;

import java.io.File;
import java.io.FileOutputStream;
import java.util.ArrayList;
import java.util.List;

import static android.os.Environment.isExternalStorageEmulated;

public class MainActivity extends AppCompatActivity {

    private final static String TAG = MainActivity.class.getSimpleName();

    private final Handler mHandler = new Handler();
    private Runnable mTimer;
    private Button mDisconnectButton;
    private Button mScanButton;
    private DataAdapter mDataAdapter;
    private List<DataInfo> mDataInfo;
    private GraphView mGraphView;
    private LineGraphSeries<DataPoint> mTempSeries;
    private LineGraphSeries<DataPoint> mHeartRateSeries;
    private File dataLog;
    private FileOutputStream out;


    private int tempCount = 0;
    private int lastTempX = 0;
    private double mCurTemp, mCurHeartRate;
    private boolean isExternalStorageReadable;
    private boolean isExternalStorageWritable;
    private boolean dirSuccess;


    // Variables to manage BLE connection
    private static boolean mConnectState;
    private static boolean mServiceConnected;
    private static boolean mBleStarted;
    private static BleService mBleService;


    private static final int REQUEST_ENABLE_BLE = 1;
    private static final int ACCELEROMETER = 0;
    private static final int BATTERY = 1;
    private static final int HEARTRATE = 2;
    private static final int OXIMETRY = 3;
    private static final int TEMPERATURE = 4;

    // Required for Android 6.0 (Marshmallow)
    private static final int PERMISSION_REQUEST_COARSE_LOCATION = 1;

    /**
     * This manages the lifecycle of the BLE service.
     * When the service starts we get the service object and initialize the service.
     */

    private final ServiceConnection mServiceConnection = new ServiceConnection() {

        /**
         * This is called when the BleService is connected
         *
         * @param name the component name of the service that has been connected
         * @param service service being bound
         */
        @Override
        public void onServiceConnected(ComponentName name, IBinder service) {
            Log.i(TAG, "onServiceConnected");
            mBleService = ((BleService.LocalBinder) service).getService();
            mServiceConnected = true;
            Log.d(TAG, "mServiceConnected is Enabled");
            mBleService.initialize();
        }

        @Override
        public void onServiceDisconnected(ComponentName name) {
            Log.i(TAG,"onServiceDisconnected");
            mBleService = null;
        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Toolbar mToolbar;
        mToolbar = (Toolbar)findViewById(R.id.toolbar);
        setSupportActionBar(mToolbar);
//        getSupportActionBar().setDisplayShowHomeEnabled(false);
        getSupportActionBar().setDisplayShowTitleEnabled(true);


        //view initialization
        mScanButton = (Button)(findViewById(R.id.scan_button));
        mDisconnectButton = (Button)(findViewById(R.id.disconnect_button));
        TextView pinTV = (TextView)findViewById(R.id.PIN_tv);
        SharedPreferences sharedPref = getApplicationContext().getSharedPreferences("BioSnapPrefs", MODE_PRIVATE);
        pinTV.setText(Integer.toString(sharedPref.getInt("userPIN", -1)));

        //graphView initialization
        initGraph();

        //external storage read/write check;
        isExternalStorageReadable = isExternalStorageReadable();
        isExternalStorageWritable = isExternalStorageWritable();
        Log.d(TAG, "externalStorageReadable\t" + isExternalStorageReadable);
        Log.d(TAG, "externalStorageWritable\t" + isExternalStorageWritable);



        //setting up RecyclerView adapter
        RecyclerView recList = (RecyclerView) findViewById(R.id.cardList);
        recList.setHasFixedSize(true);
        LinearLayoutManager llm = new LinearLayoutManager(this);
        llm.setOrientation(LinearLayoutManager.VERTICAL);
        recList.setLayoutManager(llm);

        mDataAdapter = new DataAdapter(getSampleData());
        recList.setAdapter(mDataAdapter);


        Log.d(TAG,"mBleStarted value: " + mBleStarted);


        //This section required for Android 6.0 (Marshmallow)
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            // Android M Permission check 
            if (this.checkSelfPermission(Manifest.permission.ACCESS_COARSE_LOCATION) != PackageManager.PERMISSION_GRANTED) {
                final AlertDialog.Builder builder = new AlertDialog.Builder(this);
                builder.setTitle("This app needs location access ");
                builder.setMessage("Please grant location access so this app can detect devices.");
                builder.setPositiveButton(android.R.string.ok, null);
                builder.setOnDismissListener(new DialogInterface.OnDismissListener() {
                    public void onDismiss(DialogInterface dialog) {
                        requestPermissions(new String[]{Manifest.permission.ACCESS_COARSE_LOCATION}, PERMISSION_REQUEST_COARSE_LOCATION);
                    }
                });
                builder.show();
            }
        } //End of section for Android 6.0 (Marshmallow)

        //Requesting read and write permission for data logging purposes
        if(this.checkSelfPermission(Manifest.permission.WRITE_EXTERNAL_STORAGE) != PackageManager.PERMISSION_GRANTED){
            final AlertDialog.Builder builder = new AlertDialog.Builder(this);
            builder.setTitle("This app needs writing to external storage ");
            builder.setMessage("Please grant location access so this app can log vital data.");
            builder.setPositiveButton(android.R.string.ok, null);
            builder.setOnDismissListener(new DialogInterface.OnDismissListener() {
                public void onDismiss(DialogInterface dialog) {
                    requestPermissions(new String[]{Manifest.permission.WRITE_EXTERNAL_STORAGE},1);
                }
            });
            builder.show();
        }

        if(this.checkSelfPermission(Manifest.permission.READ_EXTERNAL_STORAGE) != PackageManager.PERMISSION_GRANTED){
            final AlertDialog.Builder builder = new AlertDialog.Builder(this);
            builder.setTitle("This app needs writing to external storage ");
            builder.setMessage("Please grant location access so this app can log vital data.");
            builder.setPositiveButton(android.R.string.ok, null);
            builder.setOnDismissListener(new DialogInterface.OnDismissListener() {
                public void onDismiss(DialogInterface dialog) {
                    requestPermissions(new String[]{Manifest.permission.READ_EXTERNAL_STORAGE},1);
                }
            });
            builder.show();
        }

    }


    //This method required for Android 6.0 (Marshmallow)
    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String permissions[], @NonNull int[] grantResults) {
        switch (requestCode) {
            case PERMISSION_REQUEST_COARSE_LOCATION: {
                if (grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                    Log.d("Permission for 6.0:", "Coarse location permission granted");
                } else {
                    final AlertDialog.Builder builder = new AlertDialog.Builder(this);
                    builder.setTitle("Functionality limited");
                    builder.setMessage("Since location access has not been granted, this app will not be able to discover beacons when in the background.");
                    builder.setPositiveButton(android.R.string.ok, null);
                    builder.setOnDismissListener(new DialogInterface.OnDismissListener() {
                        @Override
                        public void onDismiss(DialogInterface dialog) {
                        }
                    });
                    builder.show();
                }
            }
        }
    } //End of section for Android 6.0 (Marshmallow)


    @Override
    public boolean onCreateOptionsMenu(Menu menu){
        //Inflate the menu; this adds items to the actionbar if it is present
        getMenuInflater().inflate(R.menu.menu_items,menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item){
        //Handle actionbar item clicks here
        //Actionbar will automatically handle up/back clicks
        //As long as a parent activity is specified in the manifest file

        int id = item.getItemId();

        if(id == R.id.action_bluetooth){
            //if(!mBleStarted)
                startBluetooth();
            return true;
        }
        if(id == R.id.action_assessment){
            Intent i = new Intent(MainActivity.this, PatientActivity.class);
            startActivity(i);
        }

        return super.onOptionsItemSelected(item);

    }

    @Override
    protected void onResume() {
        super.onResume();
        // Register the broadcast receiver. This specified the messages the main activity looks for from the BleService
        final IntentFilter filter = new IntentFilter();
        filter.addAction(BleService.ACTION_BLESCAN_CALLBACK);
        filter.addAction(BleService.ACTION_CONNECTED);
        filter.addAction(BleService.ACTION_DISCONNECTED);
        filter.addAction(BleService.ACTION_SERVICES_DISCOVERED);
        filter.addAction(BleService.ACTION_DATA_RECEIVED);
        registerReceiver(mBleUpdateReceiver, filter);

        Log.d(TAG,"onResume() is called!");
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        // User chose not to enable Bluetooth.
        if (requestCode == REQUEST_ENABLE_BLE && resultCode == Activity.RESULT_CANCELED) {
            finish();
            return;
        }
        super.onActivityResult(requestCode, resultCode, data);
    }

    @Override
    protected void onPause() {
        mHandler.removeCallbacks(mTimer);
        unregisterReceiver(mBleUpdateReceiver);
        Log.d(TAG,"onPause() called!");
        super.onPause();

    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        mHandler.removeCallbacks(mTimer);

        // Close and unbind the service when the activity goes away
        if(mBleService != null) {
            mBleService.disconnect();
            mBleService.close();
            unbindService(mServiceConnection);
            mBleService = null;
            mServiceConnected = false;
        }

        Log.d(TAG,"onDestroy() called!");
    }

    public void startBluetooth(){
        mBleStarted = true;
        //Find the BLE service and adapter
        final BluetoothManager bluetoothManager =
                (BluetoothManager) getSystemService(Context.BLUETOOTH_SERVICE);
        BluetoothAdapter mBluetoothAdapter = bluetoothManager.getAdapter();

        // Ensures Bluetooth is enabled on the device.  If Bluetooth is not currently enabled,
        // fire an intent to display a dialog asking the user to grant permission to enable it.
        if (!mBluetoothAdapter.isEnabled()) {
            Intent enableBtIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(enableBtIntent, REQUEST_ENABLE_BLE);
        }

        // Start the BLE Service
        Log.d(TAG, "Starting BLE Service");
        Intent gattServiceIntent = new Intent(this, BleService.class);
        bindService(gattServiceIntent, mServiceConnection, BIND_AUTO_CREATE);

        // Disable the start button and turn on the search  button
        if(!mServiceConnected)
            mScanButton.setEnabled(true);
        Log.d(TAG, "Bluetooth is Enabled");
        Toast.makeText(MainActivity.this,"Bluetooth is Enabled",Toast.LENGTH_SHORT).show();
    }
    public void bleScan(View view){
        Log.d(TAG," " + mServiceConnected);
        if(mServiceConnected) {
            mBleService.scan();
        }
        else {
            Toast.makeText(MainActivity.this,"Service not connected, will not scan",Toast.LENGTH_SHORT).show();
        }
    }

    public void bleDisconnect(View view){
        mBleService.disconnect();
    }




    /**
     * Listener for BLE event broadcasts
     */
    private final BroadcastReceiver mBleUpdateReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            final String action = intent.getAction();
            switch (action) {
                case BleService.ACTION_BLESCAN_CALLBACK:
                    // Disable the search button and enable the connect button
                    mScanButton.setEnabled(false);
                    mBleService.connect();
                    Toast.makeText(MainActivity.this,"CALLBACK received", Toast.LENGTH_SHORT).show();
                    break;
                case BleService.ACTION_CONNECTED:
                    /* This if statement is needed because we sometimes get a GATT_CONNECTED */
                    /* action when sending notifications */
                    if (!mConnectState) {
                        // Disable the connect button, discover services
                        mDisconnectButton.setEnabled(true);
                        mConnectState = true;
                        mBleService.discoverServices();

                        Log.d(TAG, "Connected to Device");
                        Toast.makeText(MainActivity.this,"Connected to device.",Toast.LENGTH_LONG).show();
                    }
                    break;
                case BleService.ACTION_DISCONNECTED:
                    // Disable the disconnect, discover svc, discover char button, and enable the search button
                    mConnectState = false;
                    mScanButton.setEnabled(true);
                    mDisconnectButton.setEnabled(false);
                    try{
                        out.flush();
                        out.close();
                    }catch(Exception e){
                        e.printStackTrace();
                    }


                    Log.d(TAG, "Disconnected");
                    break;
                case BleService.ACTION_SERVICES_DISCOVERED:
                    // Disable the discover services button
                    Log.d(TAG, "Services Discovered");
                    mBleService.notify(true);
                    mDataInfo = mDataAdapter.getDataList();

                    //check if directory for data exists, if so open a file in it
                    if(isExternalStorageReadable && isExternalStorageWritable){
                        SharedPreferences sharedPref = getApplicationContext().getSharedPreferences("BioSnapPrefs", MODE_PRIVATE);
                        dirSuccess = false;
                        String root = Environment.getExternalStorageDirectory().toString();

                        File myDir = new File(root, "BioSnapData");

                        if(!myDir.exists())
                            dirSuccess = myDir.mkdirs();

                        Log.d(TAG,"Successful in making dir? \t" + dirSuccess);

                        if(dirSuccess) {
                            String firstName = sharedPref.getString("firstName", "first_name");
                            String lastName = sharedPref.getString("lastName", "last_name");
                            String fileName = lastName + "_" + firstName + "dataLog.txt";
                            dataLog = new File(myDir, fileName);
                            Log.d(TAG, "Does dataLog exist:\t" + dataLog.exists());
                        }
                    }

                    break;
                case BleService.ACTION_DATA_RECEIVED:
                    //
                    if(mBleService.isEmergButtonPressed())
                        dialEmergencyNumber("3239194191");




                    // This is called after a notify or a read completes
                    if(tempCount++ % 50 == 0) {
                        mCurTemp = Double.valueOf(mBleService.getTemperature());
                        mCurHeartRate = mBleService.getHeartRateD();
                        mDataInfo.get(ACCELEROMETER).setMeasurement(mBleService.getXYZ());
                        mDataInfo.get(BATTERY).setMeasurement(mBleService.getBatteryLevel());
                        mDataInfo.get(HEARTRATE).setMeasurement(mBleService.getHeartRate());
                        mDataInfo.get(OXIMETRY).setMeasurement(mBleService.getSP02());
                        mDataInfo.get(TEMPERATURE).setMeasurement(mBleService.getTemperature());

//                    mDataInfo.get(TEMPERATURE).appendGraph();
                        mDataAdapter.notifyItemRangeChanged(0, mDataAdapter.getItemCount());
//                        mDataAdapter.notifyItemChanged(TEMPERATURE);
                        mTimer = new Runnable() {
                            @Override
                            public void run() {

                                mTempSeries.appendData(new DataPoint(lastTempX, mCurTemp),false,100);
                                mHeartRateSeries.appendData(new DataPoint(lastTempX, mCurHeartRate), false, 100);

                                //test writing temperature file
                                if(dirSuccess) {
                                    try {
                                        out = new FileOutputStream(dataLog, true);
                                        out.write((mDataInfo.get(TEMPERATURE).getMeasurement() + "\n").getBytes());
                                        out.flush();
                                        out.close();
                                        Log.d(TAG, "writing data");

                                    } catch (Exception e) {
                                        Toast.makeText(MainActivity.this, "Cannot create FileOutputStream", Toast.LENGTH_SHORT).show();
                                        e.printStackTrace();
                                    }
                                }
                                if(lastTempX >= 100){
                                    DataPoint[] valuesTemp = new DataPoint[1];
                                    DataPoint[] valuesHR = new DataPoint[1];

                                    valuesTemp[0] = new DataPoint(0,mCurTemp);
                                    valuesHR[0] = new DataPoint(0, mCurHeartRate);

                                    mTempSeries.resetData(valuesTemp);
                                    mHeartRateSeries.resetData(valuesHR);
                                    lastTempX = 1;

                                }
                                lastTempX += 1d;
                            }
                        };
                        mHandler.postDelayed(mTimer, 1000);

                    }

//                    mDataAdapter.notifyDataSetChanged();

                default:
                    break;
            }
        }
    };

    private List<DataInfo> getSampleData(){
        List<DataInfo> sampleList = new ArrayList<>();

        sampleList.add(new DataInfo("Activity"));
        sampleList.add(new DataInfo("Battery Level", "mV"));
        sampleList.add(new DataInfo("Heart Rate", "bpm"));
        sampleList.add(new DataInfo("Oxygen Concentration", "%"));
        sampleList.add(new DataInfo("Temperature","Celsius"));

        return sampleList;
    }

    public void onCheckboxClicked(View view){
        // Is the view now checked?
        boolean checked = ((CheckBox) view).isChecked();

        //Check which Checkbox was clicked
        switch(view.getId()){
            case R.id.checkbox_heart_rate:
                if(checked) {
                    mGraphView.getSecondScale().addSeries(mHeartRateSeries);
                }else
                    mGraphView.getSecondScale().removeSeries(mHeartRateSeries);
                break;
            case R.id.checkbox_temp:
                if(checked)
//                    Log.d(TAG,"checked temp box");
                    mGraphView.addSeries(mTempSeries);
                else
                    mGraphView.removeSeries(mTempSeries);
                break;
        }


    }

    private void initGraph(){
        mGraphView = (GraphView)(findViewById(R.id.my_graph));

        mTempSeries = new LineGraphSeries<>();
        mTempSeries.setTitle("Body Temperature");
        mHeartRateSeries = new LineGraphSeries<>();
        mHeartRateSeries.setColor(Color.RED);
        mHeartRateSeries.setTitle("Heart Rate");


        mGraphView.getViewport().setXAxisBoundsManual(true);
        mGraphView.getViewport().setYAxisBoundsManual(true);
        mGraphView.getViewport().setMaxX(100);
        mGraphView.getViewport().setMinX(0);
        mGraphView.getViewport().setMaxY(40);
        mGraphView.getSecondScale().setMinY(0);
        mGraphView.getSecondScale().setMaxY(160);
        mGraphView.getLegendRenderer().setVisible(true);
        mGraphView.getGridLabelRenderer().setHorizontalAxisTitle("data points");
        mGraphView.getGridLabelRenderer().setVerticalAxisTitle("celsius");
        mGraphView.getSecondScale().setVerticalAxisTitle("beats per minute");
        mGraphView.getLegendRenderer().setFixedPosition(0,0);
    }


    /* Checks if external storage is available for read and write */
    private boolean isExternalStorageWritable() {
        String state = Environment.getExternalStorageState();
        return Environment.MEDIA_MOUNTED.equals(state);
    }

    /* Checks if external storage is available to at least read */
    private boolean isExternalStorageReadable() {
        String state = Environment.getExternalStorageState();
        return (Environment.MEDIA_MOUNTED.equals(state) ||
                Environment.MEDIA_MOUNTED_READ_ONLY.equals(state));
    }

    private void dialEmergencyNumber(String emergNumber){
        startActivity(new Intent(Intent.ACTION_DIAL, Uri.fromParts("tel", emergNumber, null)));
    }
}
