package com.eec136b.biosnap.biosnapapp;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.support.v7.widget.Toolbar;

import java.util.ArrayList;
import java.util.List;

public class MainActivity extends AppCompatActivity {

    private Toolbar mToolbar;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        mToolbar = (Toolbar)findViewById(R.id.toolbar);
        setSupportActionBar(mToolbar);
        getSupportActionBar().setDisplayShowHomeEnabled(false);
        //getSupportActionBar().setDisplayShowTitleEnabled(true);




        RecyclerView recList = (RecyclerView) findViewById(R.id.cardList);
        recList.setHasFixedSize(true);
        LinearLayoutManager llm = new LinearLayoutManager(this);
        llm.setOrientation(LinearLayoutManager.VERTICAL);
        recList.setLayoutManager(llm);

        DataAdapter mDataAdapter = new DataAdapter(getSampleData());
        recList.setAdapter(mDataAdapter);
    }

    private List<DataInfo> getSampleData(){
        List<DataInfo> sampleList = new ArrayList<DataInfo>();

        sampleList.add(new DataInfo("Heart Rate"));
        sampleList.add(new DataInfo("Oxygen Concentration"));
        sampleList.add(new DataInfo("Temperature"));
        sampleList.add(new DataInfo("Activity"));

        return sampleList;
    }
}
