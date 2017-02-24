package com.eec136b.biosnap.biosnapapp;

/**
 * Created by Davit on 2/17/2017.
 */

import android.os.Handler;
import android.support.v7.widget.RecyclerView;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import com.jjoe64.graphview.GraphView;
import com.jjoe64.graphview.series.DataPoint;
import com.jjoe64.graphview.series.LineGraphSeries;

import java.util.List;
public class DataAdapter extends RecyclerView.Adapter<DataAdapter.DataViewHolder> {

    private final static String TAG = MainActivity.class.getSimpleName();

    private boolean firstRunTemp = true;

    private List<DataInfo> dataList;
    private int lastX = 1;

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

        if(di.unit.equalsIgnoreCase("celsius")) {
            if(firstRunTemp) {
                dataViewHolder.vGraph.addSeries(di.series);
                dataViewHolder.vGraph.getViewport().setXAxisBoundsManual(true);
                dataViewHolder.vGraph.getViewport().setMinX(0);
                dataViewHolder.vGraph.getViewport().setMaxX(1000);
                firstRunTemp = false;
            }
        }
    }

    @Override
    public DataViewHolder onCreateViewHolder(ViewGroup viewGroup, int i){
        View itemView = LayoutInflater.from(viewGroup.getContext()).inflate(R.layout.card_layout,viewGroup,false);
//        GraphView graph = (GraphView) itemView.findViewById(R.id.graph);
//
//        LineGraphSeries<DataPoint> series = new LineGraphSeries<>();
//        graph.addSeries(series);
//        graph.getViewport().setXAxisBoundsManual(true);
//        graph.getViewport().setMinX(0);
//        graph.getViewport().setMaxX(40);

        return new DataViewHolder(itemView);
    }

    @Override
    public  void onAttachedToRecyclerView(RecyclerView recyclerView){
        super.onAttachedToRecyclerView(recyclerView);
    }


    public static class DataViewHolder extends RecyclerView.ViewHolder {
        protected TextView vType;
        protected TextView vMeasurement;
        protected TextView vUnit;
        protected GraphView vGraph;

        public DataViewHolder(View view){
            super(view);

            vType = (TextView)(view.findViewById(R.id.txtType));
            vMeasurement = (TextView)(view.findViewById(R.id.txtMeasurement));
            vUnit = (TextView)(view.findViewById(R.id.txtUnit));
            vGraph = (GraphView)(view.findViewById(R.id.graph));



        }
    }
}
