package com.crowxr.app;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.widget.TextView;

import com.crowxr.app.databinding.ActivityMainBinding;

public class MainActivity extends AppCompatActivity {
    static {
        System.loadLibrary("CrowXR");
    }

    public native String stringFromJNI();

    private ActivityMainBinding binding;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        TextView tv = binding.sampleText;
        tv.setText(stringFromJNI());
    }
}
