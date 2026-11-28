package cn.hyhsystem.hcppbox.monochromescreen;

import android.os.Bundle;
import android.util.SparseArray;
import android.view.InputDevice;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.View;

import androidx.appcompat.app.AppCompatActivity;

public class MainActivity extends AppCompatActivity {

    private static final int[] JOYSTICK_AXIS_IDS = {
            MotionEvent.AXIS_X,
            MotionEvent.AXIS_Y,
            MotionEvent.AXIS_Z,
            MotionEvent.AXIS_RZ,
            MotionEvent.AXIS_RX,
            MotionEvent.AXIS_RY
    };

    private final SparseArray<float[]> joystickAxisState = new SparseArray<>();
    private final SparseArray<Integer> joystickHatState = new SparseArray<>();

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

    private static int keyCodeToJoystickButton(int keyCode) {
        switch (keyCode) {
            case KeyEvent.KEYCODE_BUTTON_A:
                return 0;
            case KeyEvent.KEYCODE_BUTTON_B:
                return 1;
            case KeyEvent.KEYCODE_BUTTON_X:
                return 2;
            case KeyEvent.KEYCODE_BUTTON_Y:
                return 3;
            case KeyEvent.KEYCODE_BUTTON_L1:
                return 4;
            case KeyEvent.KEYCODE_BUTTON_R1:
                return 5;
            case KeyEvent.KEYCODE_BUTTON_L2:
                return 6;
            case KeyEvent.KEYCODE_BUTTON_R2:
                return 7;
            case KeyEvent.KEYCODE_BUTTON_THUMBL:
                return 8;
            case KeyEvent.KEYCODE_BUTTON_THUMBR:
                return 9;
            case KeyEvent.KEYCODE_BUTTON_START:
                return 10;
            case KeyEvent.KEYCODE_BUTTON_SELECT:
                return 11;
            case KeyEvent.KEYCODE_BUTTON_MODE:
                return 12;
            default:
                return -1;
        }
    }

    private boolean handleJoystickMotion(MotionEvent event) {
        int deviceId = event.getDeviceId() & 0xFF;
        float[] axisState = joystickAxisState.get(deviceId);
        if (axisState == null) {
            axisState = new float[JOYSTICK_AXIS_IDS.length];
            joystickAxisState.put(deviceId, axisState);
        }
        for (int i = 0; i < JOYSTICK_AXIS_IDS.length; i++) {
            float value = event.getAxisValue(JOYSTICK_AXIS_IDS[i]);
            if (Math.abs(value - axisState[i]) < 0.01f) {
                continue;
            }
            axisState[i] = value;
            int intValue = (int) (value * 32767.0f);
            if (intValue < -32768) {
                intValue = -32768;
            } else if (intValue > 32767) {
                intValue = 32767;
            }
            Native.joystickAxis(deviceId, i, intValue);
        }

        float hatX = event.getAxisValue(MotionEvent.AXIS_HAT_X);
        float hatY = event.getAxisValue(MotionEvent.AXIS_HAT_Y);
        int hat = 0;
        if (hatY < -0.5f) {
            hat |= 0x01;
        } else if (hatY > 0.5f) {
            hat |= 0x04;
        }
        if (hatX < -0.5f) {
            hat |= 0x08;
        } else if (hatX > 0.5f) {
            hat |= 0x02;
        }
        Integer oldHat = joystickHatState.get(deviceId);
        if (oldHat == null || oldHat != hat) {
            joystickHatState.put(deviceId, hat);
            Native.joystickHat(deviceId, 0, hat);
        }
        return true;
    }

    @Override
    public boolean onGenericMotionEvent(MotionEvent event) {
        if (event.isFromSource(InputDevice.SOURCE_JOYSTICK) || event.isFromSource(InputDevice.SOURCE_GAMEPAD)) {
            return handleJoystickMotion(event);
        }
        return super.onGenericMotionEvent(event);
    }

    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        if (event.isFromSource(InputDevice.SOURCE_GAMEPAD) || event.isFromSource(InputDevice.SOURCE_JOYSTICK) || KeyEvent.isGamepadButton(keyCode)) {
            int joystickButton = keyCodeToJoystickButton(keyCode);
            if (joystickButton >= 0) {
                if (event.getRepeatCount() == 0) {
                    Native.joystickButton(event.getDeviceId() & 0xFF, joystickButton, true);
                }
                return true;
            }
        }
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
        if (event.isFromSource(InputDevice.SOURCE_GAMEPAD) || event.isFromSource(InputDevice.SOURCE_JOYSTICK) || KeyEvent.isGamepadButton(keyCode)) {
            int joystickButton = keyCodeToJoystickButton(keyCode);
            if (joystickButton >= 0) {
                Native.joystickButton(event.getDeviceId() & 0xFF, joystickButton, false);
                return true;
            }
        }
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
