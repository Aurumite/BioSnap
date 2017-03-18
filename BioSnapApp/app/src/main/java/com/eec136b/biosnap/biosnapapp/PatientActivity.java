package com.eec136b.biosnap.biosnapapp;

import android.os.Bundle;
import android.os.Environment;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

import com.jjoe64.graphview.GraphView;
import com.jjoe64.graphview.series.DataPoint;
import com.jjoe64.graphview.series.LineGraphSeries;

import java.io.File;

/**
 * Created by Davit on 3/2/2017.
 */

public class PatientActivity extends AppCompatActivity {


    private Toolbar mToolbar;
    private Button mGetDataButton;
    private GraphView mPatientGraph;
    private EditText firstName;
    private EditText lastName;
    private File mDataFile;
    private LineGraphSeries<DataPoint> mSeries;

    @Override
    protected void onCreate(Bundle savedInstanceState){
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_patient_data);


        mToolbar = (Toolbar)(findViewById(R.id.toolbar_patient));
        mGetDataButton = (Button)(findViewById(R.id.get_patient_data_button));
        mPatientGraph = (GraphView)(findViewById(R.id.patient_data_graph));
        firstName = (EditText)(findViewById(R.id.first_name_data_et));
        lastName = (EditText)(findViewById(R.id.last_name_data_et));

        mSeries = new LineGraphSeries<>();

        setSupportActionBar(mToolbar);
        getSupportActionBar().setDisplayShowTitleEnabled(true);
        getSupportActionBar().setDisplayHomeAsUpEnabled(true);


    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu){
        //Inflate the menu; this adds items to the actionbar if it is present
        getMenuInflater().inflate(R.menu.menu_assessment,menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch (item.getItemId()){
            case android.R.id.home:
                onBackPressed();
                return true;
        }
        return super.onOptionsItemSelected(item);
    }

    public void getPatientData(View view){
        String root = Environment.getExternalStorageDirectory().toString();
        File myDir = new File(root + "/BioSnapData/");
        myDir.mkdir();

        //search for the right file
        File[] dirFiles = myDir.listFiles();
        for(File file : dirFiles){
            if(file.getName().equalsIgnoreCase(lastName.getText().toString() + "_" + firstName.getText().toString() + "dataLog")){
                mDataFile = file;
                setUserData();
                return;
            }
        }

        //if we got here, no file found, notify user
        Toast.makeText(this, "No data found for that user", Toast.LENGTH_SHORT).show();
    }

    //set the data for the graph series
    private void setUserData(){
        //just in case
        if(mDataFile == null)
            return;


    }



}
