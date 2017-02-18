package com.eec136b.biosnap.biosnapapp;

/**
 * Created by Davit on 2/17/2017.
 */

import android.support.v7.widget.RecyclerView;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;
import java.util.List;
public class DataAdapter extends RecyclerView.Adapter<DataAdapter.DataViewHolder> {

    private List<DataInfo> dataList;

    public DataAdapter(List<DataInfo> dataList){

        this.dataList = dataList;

    }

    @Override
    public int getItemCount(){
        return dataList.size();
    }

    public List<DataInfo> getDataList(){ return dataList; }

    @Override
    public void onBindViewHolder(DataViewHolder dataViewHolder, int i){
        DataInfo di = dataList.get(i);
        dataViewHolder.vType.setText(di.type);
        dataViewHolder.vMeasurement.setText(di.measurement);
        dataViewHolder.vUnit.setText(di.unit);
    }

    @Override
    public DataViewHolder onCreateViewHolder(ViewGroup viewGroup, int i){
        View itemView = LayoutInflater.from(viewGroup.getContext()).inflate(R.layout.card_layout,viewGroup,false);

        return new DataViewHolder(itemView);
    }


    public static class DataViewHolder extends RecyclerView.ViewHolder {
        protected TextView vType;
        protected TextView vMeasurement;
        protected TextView vUnit;

        public DataViewHolder(View view){
            super(view);

            vType = (TextView)(view.findViewById(R.id.txtType));
            vMeasurement = (TextView)(view.findViewById(R.id.txtMeasurement));
            vUnit = (TextView)(view.findViewById(R.id.txtUnit));

        }
    }
}
