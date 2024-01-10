package com.uvknc.subthreadwithjni;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.TextView;
import android.widget.Toast;

import com.uvknc.subthreadwithjni.databinding.ActivityMainBinding;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'subthreadwithjni' library on application startup.
    static {
        System.loadLibrary("subthreadwithjni");
    }

    private ActivityMainBinding binding;
    private int                 count = 1;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());


        initEvent();


    }

    private void initEvent() {
        binding.btnStartTask.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Toast.makeText(MainActivity.this, " 任务开始执行", Toast.LENGTH_SHORT).show();
                runTask();
                Toast.makeText(MainActivity.this, " 主线程继续", Toast.LENGTH_SHORT).show();
            }
        });

        binding.btnUiAbility.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Toast.makeText(MainActivity.this, "UI UI " + count, Toast.LENGTH_SHORT).show();
                count++;
            }
        });

    }


    private static final String TAG = "MainActivity";

    public void onTaskComplete() {
        Log.d(TAG, "onTaskComplete: ");

        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Toast.makeText(MainActivity.this, "onTaskComplete", Toast.LENGTH_SHORT).show();
            }
        });


    }


    public native void runTask();
}