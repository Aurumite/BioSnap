package com.eec136b.biosnap.biosnapapp;

import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.support.v7.app.AppCompatActivity;
import android.widget.TextView;

import org.w3c.dom.Text;

import java.util.Random;

/**
 * Created by Davit on 2/27/2017.
 */

public class SplashActivity extends AppCompatActivity{


    private static String[] healthTips;
    private static TextView splashText;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
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

        splashText = (TextView)(findViewById(R.id.splash_text_view));

        Random r = new Random();
        int randomVal = r.nextInt(healthTips.length - 1);

        splashText.setText(healthTips[randomVal]);
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
