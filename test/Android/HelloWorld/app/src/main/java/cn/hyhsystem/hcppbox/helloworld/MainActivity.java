package cn.hyhsystem.hcppbox.helloworld;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.widget.TextView;

import cn.hyhsystem.hcppbox.helloworld.databinding.ActivityMainBinding;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'helloworld' library on application startup.
    static {
        System.loadLibrary("helloworld");
    }

    private ActivityMainBinding binding;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        // Example of a call to a native method
        TextView tv = binding.sampleText;
        tv.setText(stringFromJNI());
    }

    /**
     * A native method that is implemented by the 'helloworld' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();
}