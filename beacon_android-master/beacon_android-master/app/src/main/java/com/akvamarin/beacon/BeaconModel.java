package com.akvamarin.beacon;

import android.os.Build;

import androidx.annotation.RequiresApi;

import java.util.Objects;

public class BeaconModel {

    private String bleMACAddress;
    private String namespaceUUID;
    private String major;
    private String minor;
    private double distance;
    private int RSSI;
    private double avgRSSI;
    private int txPower;

    public String getBleMACAddress() {
        return bleMACAddress;
    }

    public void setBleMACAddress(String bleMACAddress) {
        this.bleMACAddress = bleMACAddress;
    }

    public String getNamespaceUUID() {
        return namespaceUUID;
    }

    public void setNamespaceUUID(String namespaceUUID) {
        this.namespaceUUID = namespaceUUID;
    }

    public String getMajor() {
        return major;
    }

    public void setMajor(String major) {
        this.major = major;
    }

    public String getMinor() {
        return minor;
    }

    public void setMinor(String minor) {
        this.minor = minor;
    }

    public double getDistance() {
        return distance;
    }

    public void setDistance(double distance) {
        this.distance = distance;
    }

    public int getRSSI() {
        return RSSI;
    }

    public void setRSSI(int RSSI) {
        this.RSSI = RSSI;
    }

    public int getTxPower() {
        return txPower;
    }

    public void setTxPower(int txPower) {
        this.txPower = txPower;
    }

    public double getAvgRSSI() {
        return avgRSSI;
    }

    public void setAvgRSSI(double avgRSSI) {
        this.avgRSSI = avgRSSI;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        BeaconModel that = (BeaconModel) o;
        return bleMACAddress.equals(that.bleMACAddress) && major.equals(that.major) && minor.equals(that.minor);
    }

    @RequiresApi(api = Build.VERSION_CODES.KITKAT)
    @Override
    public int hashCode() {
        return Objects.hash(bleMACAddress, major, minor);
    }

    @Override
    public String toString() {
        return "BeaconModel{" +
                "bleMACAddress='" + bleMACAddress + '\'' +
                ", major='" + major + '\'' +
                ", minor='" + minor + '\'' +
                ", distance=" + distance +
                ", RSSI=" + RSSI +
                ", avgRSSI=" + avgRSSI  +
                '}';
    }
}





//    public func calculateAccuracy(txPower : Double, rssi : Double) -> Double {
//        if (rssi == 0) {
//        return -1.0; // if we cannot determine accuracy, return -1.
//        }
//
//        let ratio :Double = rssi*1.0/txPower;
//        if (ratio < 1.0) {
//        return pow(ratio,10.0);
//        }
//        else {
//        let accuracy :Double =  (0.89976)*pow(ratio,7.7095) + 0.111;
//        return accuracy;
//        }
//        }



//    Формула в библиотеке Android Beacon предназначена для Nexus 5, но она имеет открытый исходный код,
//    поэтому вы можете настроить код для любой модели устройства по своему желанию.
//    Чтобы составить новую формулу для другого устройства,
//    вам необходимо выполнить несколько измерений среднего значения RSSI за 60 секунд на различных расстояниях (0,5 м, 1 м, 2 м, 3 м,... 10 м, 12 м... 20 м),
//    а затем выполнить расчет наилучшего соответствия.

//https://medium.com/beingcoders/convert-rssi-value-of-the-ble-bluetooth-low-energy-beacons-to-meters-63259f307283

//https://stackoverflow.com/questions/20983734/tools-to-determine-exact-location-when-using-ibeacons

//    Значение RSSI будет зависеть от ряда факторов помимо мощности TX, среди них:
//        Многолучевые отражения: частота 2,4 ГГц будет отражаться от стен, поэтому,
//        если отражение помогает дальнему маяку и ослабляет ближний маяк, то дальний маяк может получить более сильный RSSI.
//        Расположение антенны: правильное положение телефона может иметь большое значение.