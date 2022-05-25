package com.akvamarin.beacon;

import android.Manifest;
import android.annotation.SuppressLint;
import android.bluetooth.BluetoothAdapter;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.graphics.drawable.ColorDrawable;
import android.net.Uri;
import android.os.Bundle;
import android.os.Handler;
import android.os.RemoteException;
import android.provider.Settings;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;

import org.altbeacon.beacon.Beacon;
import org.altbeacon.beacon.BeaconConsumer;
import org.altbeacon.beacon.BeaconManager;
import org.altbeacon.beacon.BeaconParser;
import org.altbeacon.beacon.RangeNotifier;
import org.altbeacon.beacon.Region;

import java.util.ArrayList;
import java.util.Collection;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class MainActivity extends AppCompatActivity implements BeaconConsumer {
    protected static final String TAG = "MainActivity";

    private BluetoothAdapter mBluetoothAdapter;
    private BeaconManager beaconManager;

    private List<BeaconModel> mBeaconsList;
    private Map<String, BeaconModel> mBeaconsStartMap  = new HashMap<>();
   // private List<BeaconModel> mBeaconsStartLst = new ArrayList<>();
    private Map<String, BeaconModel> beaconDynamicModelMap;
    private CustomBeaconAdapter mAdapter;
    private TextView tvResult;
    private Button buttonSaveGeo;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        tvResult = findViewById(R.id.tvResult);
        buttonSaveGeo = findViewById(R.id.buttonSaveGeo);

        initVariable();
        initUI();
        checkBluetoothStatus();

        buttonSaveGeo.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if (!beaconDynamicModelMap.isEmpty()){
                    mBeaconsStartMap.putAll(beaconDynamicModelMap);
                    Log.d(TAG, "savePositionBeacons: SAVE PLACE!");

                    ColorDrawable buttonColor = (ColorDrawable) buttonSaveGeo.getBackground();
                    int colorID = buttonColor.getColor();
                    Log.d(TAG, "ID COLOR: " + colorID); // -10611094
                    if (colorID == -10611094){
                        buttonSaveGeo.setBackgroundResource(R.color.colorPrimary);
                    } else {
                        buttonSaveGeo.setBackgroundResource(R.color.colorPrimaryDark);
                    }

                } else {
                    Log.d(TAG, "savePositionBeacons: NOT SAVE PLACE!");
                }
            }
        });


    }

    private void initVariable() {
        mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        if (mBluetoothAdapter == null) {
            Toast.makeText(this, "Устройство не поддерживает Bluetooth", Toast.LENGTH_SHORT).show();
            finish();
        }

        mBeaconsList = new ArrayList<>();
        beaconDynamicModelMap = new HashMap<>();
        mAdapter = new CustomBeaconAdapter(this, mBeaconsList);
    }

    private void initUI() {
        ListView beaconList = (ListView) findViewById(R.id.beacon_list);
        beaconList.setAdapter(mAdapter);
    }

    private void checkBluetoothStatus() {
        new Handler().postDelayed(new Runnable() {
            @SuppressLint("MissingPermission")
            @Override
            public void run() {
                if (!mBluetoothAdapter.isEnabled()) {
                    Toast.makeText(MainActivity.this, "Включение Bluetooth...", Toast.LENGTH_SHORT).show();
                    mBluetoothAdapter.enable();
                }
                checkLocationPermission();
            }
        }, 1000);
    }

    private void checkLocationPermission() {
        String permission = Manifest.permission.ACCESS_FINE_LOCATION;
        int rc = ActivityCompat.checkSelfPermission(this, permission);
        if (rc == PackageManager.PERMISSION_GRANTED) {
            onPermissionGranted();
        } else {
            ActivityCompat.requestPermissions(this, new String[]{permission}, 1000);
        }
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        int numOfRequest = grantResults.length;
        boolean isGranted = numOfRequest == 1 && PackageManager.PERMISSION_GRANTED == grantResults[numOfRequest - 1];
        String permission;
        if (requestCode == 1000) {
            permission = Manifest.permission.ACCESS_FINE_LOCATION;
            if (isGranted) {
                onPermissionGranted();
            } else {
                if (ActivityCompat.shouldShowRequestPermissionRationale(this, permission)) {
                    checkLocationPermission();
                } else {
                    showOpenPermissionSetting();
                }
            }
        }
    }

    void showOpenPermissionSetting() {
        new AlertDialog.Builder(this)
                .setTitle(R.string.app_name)
                .setMessage("Требуется разрешение на местоположение")
                .setCancelable(false)
                .setPositiveButton("OK", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        Intent intent = new Intent(Settings.ACTION_APPLICATION_DETAILS_SETTINGS, Uri.parse("package:" + getPackageName()));
                        startActivityForResult(intent, 1001);
                    }
                })
                .show();
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if (requestCode == 1001) {
            checkLocationPermission();
        }
    }


    private void onPermissionGranted() {
        beaconManager = BeaconManager.getInstanceForApplication(this);
        // Detect the main identifier (UID) frame:
        beaconManager.getBeaconParsers().clear();
        beaconManager.getBeaconParsers().add(new BeaconParser("ibeacon").
                setBeaconLayout("m:2-3=0215,i:4-19,i:20-21,i:22-23,p:24-24"));  // BeaconParser.EDDYSTONE_UID_LAYOUT
        beaconManager.bind(this);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        Log.e(TAG, "onDestroy");
        beaconManager.unbind(this);
    }


    @Override
    public void onBeaconServiceConnect() {
        beaconManager.removeAllRangeNotifiers();
        beaconManager.addRangeNotifier(new RangeNotifier() {
            @Override
            public void didRangeBeaconsInRegion(Collection<Beacon> beacons, Region region) {
                if (beacons.size() > 0) {
                    Log.e(TAG, "Scan iBeacon: " + beacons.size()); // count beacons
                    for (Beacon beacon : beacons) {
                        String MAC = beacon.getBluetoothAddress();
                        int RSSI = beacon.getRssi();

                        BeaconModel beaconModel = new BeaconModel();
                        beaconModel.setBleMACAddress(MAC);
                        beaconModel.setNamespaceUUID(beacon.getId1().toString());
                        beaconModel.setMajor(beacon.getId2().toString());
                        beaconModel.setMinor(beacon.getId3().toString());
                        double distance = (double) Math.round(beacon.getDistance() * 1000) / 1000;
                        beaconModel.setDistance(distance); //// double roundOff = (double) Math.round(a * 100) / 100;
                        beaconModel.setRSSI(RSSI);
                        double avgRSSI = (double) Math.round(beacon.getRunningAverageRssi() * 1000) / 1000;
                        beaconModel.setAvgRSSI(avgRSSI);
                        beaconModel.setTxPower(beacon.getTxPower());

                        beaconDynamicModelMap.put(beaconModel.getBleMACAddress(), beaconModel);



                    }

                    runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            mBeaconsList.clear();
                            mBeaconsList.addAll(beaconDynamicModelMap.values());
                            mAdapter.notifyDataSetChanged();

                            if (!mBeaconsList.isEmpty()){
                                equalsDistances();
                            }
                            
                        }
                    });
                }
            }
        });

        try {
            beaconManager.startRangingBeaconsInRegion(new Region("my region", null, null, null));
        } catch (RemoteException e) {
            e.printStackTrace();
        }
    }

    @SuppressLint("ResourceAsColor")
    public void equalsDistances() {
       final double CONST_DIST =  0.5;
       int countDynamicBeacons = 0;

       // if (mBeaconsStartMap.size() == 10){
            for (String macAddress : beaconDynamicModelMap.keySet()){
                if (mBeaconsStartMap.containsKey(macAddress) ){
                    double startBeaconDist = mBeaconsStartMap.get(macAddress).getDistance();
                    double dynamicBeaconDist = beaconDynamicModelMap.get(macAddress).getDistance();

                    if ((startBeaconDist - CONST_DIST) <= dynamicBeaconDist &&
                            (startBeaconDist + CONST_DIST) >= dynamicBeaconDist) {
                        countDynamicBeacons++;
                        Log.d(TAG, "equalsDistances COUNT: " + countDynamicBeacons);
                    }

//                    double raznica = mBeaconsStartMap.get(macAddress).getDistance() - beaconDynamicModelMap.get(macAddress).getDistance();
//                    //Math.abs(c-1.0) <= 0.000001
//                    if (Math.abs(raznica) <= 0.1 && countDynamicBeacons <= 4){
//                        countDynamicBeacons++;//beaconModelMap.get(macAddress)
//                        Log.d(TAG, "equalsDistances COUNT: " + countDynamicBeacons);
//                    }
                }
            }


        int countStartBeacons = mBeaconsStartMap.size();
        if (countStartBeacons != 0 && countDynamicBeacons == countStartBeacons){
            setGreenStatusPlace();
        } else if (countDynamicBeacons >= countStartBeacons / 2){
            setGreenStatusPlace();
        } else {
            setRedStatusPlace();
        }
        //}
    }




    private void setGreenStatusPlace() {
        tvResult.setBackgroundResource(R.color.colorGreen);
        tvResult.setText("Super! Вы нашли свой стол.");
    }

    private void setRedStatusPlace() {
        tvResult.setBackgroundResource(R.color.colorAccent);
        tvResult.setText("Простите, но это не Ваш стол!");
    }
}
