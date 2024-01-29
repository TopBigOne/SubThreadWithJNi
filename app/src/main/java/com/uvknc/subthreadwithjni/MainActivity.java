package com.uvknc.subthreadwithjni;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.RelativeLayout;
import android.widget.TextView;
import android.widget.Toast;

import com.uvknc.subthreadwithjni.databinding.ActivityMainBinding;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'subthreadwithjni' library on application startup.
    static {
        System.loadLibrary("subthreadwithjni");
    }

    private ActivityMainBinding binding;

    private              int    count = 1;
    private static final String TAG   = "NativeTask_M";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        // 创建一个新的按钮实例
        Button button = new Button(this);
        button.setText("Button");

       // 设置按钮的布局参数
        RelativeLayout.LayoutParams layoutParams = new RelativeLayout.LayoutParams(
                RelativeLayout.LayoutParams.WRAP_CONTENT,
                RelativeLayout.LayoutParams.WRAP_CONTENT
        );
        layoutParams.addRule(RelativeLayout.ALIGN_PARENT_START);
        layoutParams.addRule(RelativeLayout.ALIGN_PARENT_TOP);
        layoutParams.leftMargin = 60;
        layoutParams.topMargin = 30;
        button.setLayoutParams(layoutParams);

        binding.getRoot().addView(button);





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


    public void onTaskComplete() {
        Log.d(TAG, "onTaskComplete: ");

        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Toast.makeText(MainActivity.this, "onTaskComplete", Toast.LENGTH_SHORT).show();
            }
        });

    }


    public void enterSecond() {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Log.d(TAG, "enterSecond : " + Thread.currentThread().getName());
                enterB();
            }
        });
    }

    public native void runTask();


    public native void enterB();
}