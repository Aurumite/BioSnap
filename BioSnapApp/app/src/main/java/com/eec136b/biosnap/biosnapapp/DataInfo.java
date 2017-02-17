package com.eec136b.biosnap.biosnapapp;

/**
 * Created by Davit on 2/17/2017.
 */

public class DataInfo {

    protected String type;
    protected String measurement;
    protected String unit;

    public DataInfo(String type){
        this.type = type;
        measurement = "Data";
        unit = "unit";
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

    public String getUnit(){
        return unit;
    }

    public void setUnit(String unit){
        this.unit = unit;
    }

}
