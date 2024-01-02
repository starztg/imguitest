package com.epicgames.ue4;

import android.app.*;
import android.os.*;

public class MainActivity extends Activity 
{
     static {
        // When you change the lib name, change also on Android.mk file
        // Both must have same name
        System.loadLibrary("KarnishOFFICIAL");
     }
    
    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
    }
}
