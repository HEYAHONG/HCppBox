package cn.hyhsystem.hcppbox.monochromescreen;

/**
 * 与native层交互的接口。
 */
public final class Native {

    static {
        System.loadLibrary("monochromescreen");
    }

    /**
     * hgui按键值，与hgui_gui_event_key_value_t保持一致。
     */
    public static final int KEY_TAB = 9;
    public static final int KEY_ESCAPE = 27;
    public static final int KEY_RETURN = 13;
    public static final int KEY_UP = 273;
    public static final int KEY_DOWN = 274;
    public static final int KEY_RIGHT = 275;
    public static final int KEY_LEFT = 276;
    public static final int KEY_F1 = 282;
    public static final int KEY_F2 = 283;
    public static final int KEY_F3 = 284;

    public static final int TOUCH_ACTION_DOWN = 1;
    public static final int TOUCH_ACTION_UP = 2;
    public static final int TOUCH_ACTION_MOTION = 3;

    public static final int MOUSE_BUTTON_LEFT = 1;
    public static final int MOUSE_BUTTON_MIDDLE = 2;
    public static final int MOUSE_BUTTON_RIGHT = 3;
    public static final int MOUSE_BUTTON_X1 = 6;
    public static final int MOUSE_BUTTON_X2 = 7;

    private Native() {
    }

    public static native void init();

    public static native void deinit();

    public static native int getWidth();

    public static native int getHeight();

    public static native long getFrameId();

    /**
     * 拷贝当前帧缓冲区（ARGB8888）。
     *
     * @param pixels 目标数组，长度不小于getWidth()*getHeight()
     * @return 实际拷贝的像素数
     */
    public static native int copyFrame(int[] pixels);

    /**
     * 发送hgui按键事件。
     *
     * @param keyValue hgui按键值
     * @param pressed  true为按下，false为弹起
     */
    public static native void keyEvent(int keyValue, boolean pressed);

    public static native void touchEvent(int action, int id, int x, int y, int dx, int dy, int pressure);

    public static native void mouseMotion(int x, int y, int xrel, int yrel, int buttonState);

    public static native void mouseButton(int button, boolean pressed, int x, int y);

    public static native void mouseWheel(int x, int y, int delta);

    public static native void joystickAxis(int which, int axis, int value);

    public static native void joystickHat(int which, int hat, int value);

    public static native void joystickButton(int which, int button, boolean pressed);
}
