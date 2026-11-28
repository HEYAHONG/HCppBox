package cn.hyhsystem.hcppbox.monochromesimplegui;

import android.os.Bundle;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.View;

import androidx.appcompat.app.AppCompatActivity;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        Native.init();

        setContentView(R.layout.activity_main);

        //第一排：F1、F2、F3
        bindButton(R.id.button_f1, Native.KEY_F1);
        bindButton(R.id.button_f2, Native.KEY_F2);
        bindButton(R.id.button_f3, Native.KEY_F3);
        //第二排：确定、取消
        bindButton(R.id.button_ok, Native.KEY_RETURN);
        bindButton(R.id.button_cancel, Native.KEY_ESCAPE);
        bindButton(R.id.button_tab, Native.KEY_TAB);
        //方向键（十字排列）
        bindButton(R.id.button_up, Native.KEY_UP);
        bindButton(R.id.button_down, Native.KEY_DOWN);
        bindButton(R.id.button_left, Native.KEY_LEFT);
        bindButton(R.id.button_right, Native.KEY_RIGHT);
    }

    private void bindButton(int id, final int keyValue) {
        View view = findViewById(id);
        if (view == null) {
            return;
        }
        view.setOnTouchListener((v, event) -> {
            switch (event.getActionMasked()) {
                case MotionEvent.ACTION_DOWN:
                    v.setPressed(true);
                    Native.keyEvent(keyValue, true);
                    return true;
                case MotionEvent.ACTION_UP:
                    Native.keyEvent(keyValue, false);
                    v.setPressed(false);
                    v.performClick();
                    return true;
                case MotionEvent.ACTION_CANCEL:
                    Native.keyEvent(keyValue, false);
                    v.setPressed(false);
                    return true;
                default:
                    return false;
            }
        });
    }

    private static int keyCodeToHguiKey(int keyCode) {
        switch (keyCode) {
            case KeyEvent.KEYCODE_TAB:
                return Native.KEY_TAB;
            case KeyEvent.KEYCODE_F1:
                return Native.KEY_F1;
            case KeyEvent.KEYCODE_F2:
                return Native.KEY_F2;
            case KeyEvent.KEYCODE_F3:
                return Native.KEY_F3;
            case KeyEvent.KEYCODE_DPAD_UP:
                return Native.KEY_UP;
            case KeyEvent.KEYCODE_DPAD_DOWN:
                return Native.KEY_DOWN;
            case KeyEvent.KEYCODE_DPAD_LEFT:
                return Native.KEY_LEFT;
            case KeyEvent.KEYCODE_DPAD_RIGHT:
                return Native.KEY_RIGHT;
            case KeyEvent.KEYCODE_ENTER:
            case KeyEvent.KEYCODE_NUMPAD_ENTER:
            case KeyEvent.KEYCODE_DPAD_CENTER:
                return Native.KEY_RETURN;
            case KeyEvent.KEYCODE_ESCAPE:
            case KeyEvent.KEYCODE_BACK:
                return Native.KEY_ESCAPE;
            default:
                return -1;
        }
    }

    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        int hguiKey = keyCodeToHguiKey(keyCode);
        if (hguiKey >= 0) {
            if (event.getRepeatCount() == 0) {
                Native.keyEvent(hguiKey, true);
            }
            return true;
        }
        return super.onKeyDown(keyCode, event);
    }

    @Override
    public boolean onKeyUp(int keyCode, KeyEvent event) {
        int hguiKey = keyCodeToHguiKey(keyCode);
        if (hguiKey >= 0) {
            Native.keyEvent(hguiKey, false);
            return true;
        }
        return super.onKeyUp(keyCode, event);
    }

    @Override
    protected void onDestroy() {
        Native.deinit();
        super.onDestroy();
    }
}
