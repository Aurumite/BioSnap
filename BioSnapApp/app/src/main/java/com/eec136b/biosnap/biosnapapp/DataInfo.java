package com.eec136b.biosnap.biosnapapp;

import android.os.Handler;
import android.provider.ContactsContract;

import com.jjoe64.graphview.GraphView;
import com.jjoe64.graphview.series.DataPoint;
import com.jjoe64.graphview.series.LineGraphSeries;

import java.util.Iterator;

/**
 * Created by Davit on 2/17/2017.
 */

public class DataInfo {

    protected String type;
    protected String measurement;
    protected String unit;
//    private int graphLastXValue = 0;
//    protected LineGraphSeries<DataPoint> series;

//    private final Handler mHandler = new Handler();
//    private Runnable mTimer;

    public DataInfo(String type){
        this.type = type;
        measurement = "0.00";
        unit = "unit";
//        series = new LineGraphSeries<>();

    }

    public DataInfo(String type, String unit){
        this.type = type;
        measurement = "0.00";
        this.unit = unit;
//        series = new LineGraphSeries<>();

    }

    public String getType(){
        return type;
    }

    public void setType(String type){
        this.type = type;
    }

    public String getMeasurement(){
        return measurement;
    }

    public void setMeasurement(String measurement){
        this.measurement = measurement;
    }

//    public void appendMyData(){
//        mTimer = new Runnable() {
//            @Override
//            public void run() {
//            series.appendData(new DataPoint(graphLastXValue++, Double.valueOf(measurement)),false,100);
//            if(graphLastXValue >= 100){
//                DataPoint[] values = new DataPoint[1];
//                values[0] = new DataPoint(0,0);
//
//                graphLastXValue = 1;
//                series.resetData(values);
//            }
//            mHandler.postDelayed(this, 300);
//            }
//        };
//        mHandler.postDelayed(mTimer, 1000);
//    }

    public String getUnit(){
        return unit;
    }

    public void setUnit(String unit){
        this.unit = unit;
    }

}
