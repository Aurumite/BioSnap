package com.eec136b.biosnap.biosnapapp;

import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import java.util.Random;


/**
 * Created by Davit on 3/3/2017.
 *
 */

public class NewUserActivity extends AppCompatActivity {

    private EditText lastNameET;
    private EditText firstNameET;
    private SharedPreferences sharedPref;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_splash_new_user);

        lastNameET = (EditText) findViewById(R.id.last_name_et);
        firstNameET = (EditText) findViewById(R.id.first_name_et);
//        Button loginButton = (Button)findViewById(R.id.new_user_button);

        sharedPref = getApplicationContext().getSharedPreferences("BioSnapPrefs", MODE_PRIVATE);


    }

    public void loginNewUser(View view){
        String lastName = lastNameET.getText().toString();
        String firstName = firstNameET.getText().toString();
        SharedPreferences.Editor mEditor = sharedPref.edit();


        //Set a random PIN for retrieving data later
        Random r = new Random();
        int pin = r.nextInt(10000)+1000;

        mEditor.putString("firstName", firstName);
        mEditor.putString("lastName", lastName);
        mEditor.putInt("userPIN", pin);
        mEditor.putBoolean("loggedInBefore", true);
        mEditor.apply();



        Intent i = new Intent(NewUserActivity.this, MainActivity.class);
        startActivity(i);

    }

}
