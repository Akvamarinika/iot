package com.akvamarin.beacon;

import android.annotation.SuppressLint;
import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.TextView;

import java.util.List;


public class CustomBeaconAdapter extends BaseAdapter {
    List<BeaconModel> beaconModelList;
    Context context;

    public CustomBeaconAdapter(Context context, List<BeaconModel> list) {
        this.context = context;
        this.beaconModelList = list;
    }

    @Override
    public int getCount() {
        return beaconModelList.size();
    }

    @Override
    public Object getItem(int position) {
        return beaconModelList.get(position);
    }

    @Override
    public long getItemId(int position) {
        return position;
    }

    @Override
    public View getView(int position, View convertView, ViewGroup parent) {
        LayoutInflater inflater = LayoutInflater.from(context);
        @SuppressLint({"ViewHolder", "InflateParams"}) View view = inflater.inflate(R.layout.item_beacon, null);

        TextView tvUUID = view.findViewById(R.id.tv_UUID);
        TextView tvMacAddress = view.findViewById(R.id.tv_MAC);
        TextView tvMajor = view.findViewById(R.id.tv_major);
        TextView tvMinor = view.findViewById(R.id.tv_minor);
        TextView tvTxPower = view.findViewById(R.id.tv_tx_power);
        TextView tvRSSI = view.findViewById(R.id.tv_RSSI);
        TextView tvAvgRSSI = view.findViewById(R.id.tv_avg_RSSI);
        TextView tvDistance = view.findViewById(R.id.tv_distance);


        //tvUUID.setText(String.format("UUID: %s ", beaconModelList.get(position).getNamespaceUUID()));
        tvMacAddress.setText(String.format("MAC: %s ", beaconModelList.get(position).getBleMACAddress()));
        tvMajor.setText(String.format("Major: %s    ", beaconModelList.get(position).getMajor()));
        tvMinor.setText(String.format("Minor: %s    ", beaconModelList.get(position).getMinor()));
        tvTxPower.setText(String.format("TX power: %s ", String.valueOf(beaconModelList.get(position).getTxPower())));
        tvDistance.setText(String.format("Distance: %s     ", beaconModelList.get(position).getDistance())); //String.valueOf(beaconModelList.get(position).getDistance())
        tvRSSI.setText(String.format("RSSI: %s     ", String.valueOf(beaconModelList.get(position).getRSSI())));
        tvAvgRSSI.setText(String.format("AVG RSSI: %s ", String.valueOf(beaconModelList.get(position).getAvgRSSI())));

        return view;
    }
}
