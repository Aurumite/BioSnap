package com.eec136b.biosnap.biosnapapp;

import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.os.Handler;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.widget.TextView;

import org.w3c.dom.Text;

import java.util.Random;

/**
 * Created by Davit on 2/27/2017.
 *
 */

public class SplashActivity extends AppCompatActivity{

    private final static String TAG = SplashActivity.class.getSimpleName();

    private static String[] healthTips;
    private TextView splashText;
    SharedPreferences sharedPref;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        Context context = getApplicationContext();
        sharedPref = context.getSharedPreferences("BioSnapPrefs", MODE_PRIVATE);
        first_time_check();

        setContentView(R.layout.activity_splash);

        healthTips = new String[]{"Don't Drink Sugar Calories",
                "Eat Nuts",
                "Avoid Processed Junk Food",
                "Get Enough Sleep",
                "Drink Some Water, Especially Before Meals",
                "Avoid Bright Lights Before Sleep",
                "Eat Vegetables and Fruits",
                "Make Sure to Eat Enough Protein",
                "Do Some Cardio! The Heart is an Important Muscle"};

        splashText = (TextView) (findViewById(R.id.splash_text_view));
        TextView welcomeTV = (TextView)findViewById(R.id.welcome_back_tv);

        Random r = new Random();
        int randomVal = r.nextInt(healthTips.length - 1);
        boolean first = sharedPref.getBoolean("loggedInBefore", false);


        splashText.setText(healthTips[randomVal]);
        String welcomeString = "Welcome back, " + sharedPref.getString("firstName", "User").toUpperCase() + "!";
        welcomeTV.setText(welcomeString);
        if (first){
            new Handler().postDelayed(new Runnable() {

            /*
             * Showing splash screen with a timer. This will be useful when you
             * want to show case your app logo / company
             */

                @Override
                public void run() {
                    // This method will be executed once the timer is over
                    // Start your app main activity
                    Intent i = new Intent(SplashActivity.this, MainActivity.class);
                    startActivity(i);

                    // close this activity
                    finish();
                }
            }, 1500);
        }
    }

    private boolean first_time_check(){
        boolean first = sharedPref.getBoolean("loggedInBefore", false );

        Log.d(TAG, "first value: " + first);
        if(!first){
            Intent i = new Intent(SplashActivity.this, NewUserActivity.class);
            startActivity(i);
            finish();
        }
        return false;
    }
}
