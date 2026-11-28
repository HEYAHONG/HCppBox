package cn.hyhsystem.hcppbox.monochromesimplegui;

/**
 * 与native层交互的接口。
 */
public final class Native {

    static {
        System.loadLibrary("monochromescreen_simplegui");
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
}
