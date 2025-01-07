package cn.hyhsystem.hcppbox.helloworld;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.widget.TextView;

import java.util.Timer;
import java.util.TimerTask;

import cn.hyhsystem.hcppbox.helloworld.databinding.ActivityMainBinding;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'helloworld' library on application startup.
    static {
        System.loadLibrary("helloworld");
    }

    private ActivityMainBinding binding;

    //毫秒定时器
    private Timer MsTimer=null;
    private TimerTask MsTimerTask=null;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        MsTimer=new Timer();
        MsTimerTask=new TimerTask() {
            @Override
            public void run() {
                MsTick();
            }
        };
        MsTimer.schedule(MsTimerTask,1,1);

        // Example of a call to a native method
        TextView tv = binding.sampleText;
        tv.setText(stringFromJNI());
    }

    /**
     * A native method that is implemented by the 'helloworld' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();

    /*
     * 初始化     *
     */
    public native void Init();

    public MainActivity()
    {
        //初始化
        Init();
    }

    /*
     * 毫秒定时器,由定时器调用，一般用于更新UI
     */
    public native void MsTick();

}