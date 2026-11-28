package cn.hyhsystem.hcppbox.monochromesimplegui;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.RectF;
import android.util.AttributeSet;
import android.view.InputDevice;
import android.view.MotionEvent;
import android.view.View;

/**
 * 点阵屏画布。将从native层获取的帧缓冲区绘制到屏幕上。
 */
public class ScreenCanvasView extends View {

    private final Paint paint = new Paint(Paint.FILTER_BITMAP_FLAG);
    private Bitmap frameBitmap = null;
    private int[] framePixels = null;
    private int frameWidth = 0;
    private int frameHeight = 0;

    private float drawScale = 1.0f;
    private float drawLeft = 0.0f;
    private float drawTop = 0.0f;
    private int lastMouseX = 0;
    private int lastMouseY = 0;
    private boolean lastMouseValid = false;
    private final int[] mappedPoint = new int[2];

    public ScreenCanvasView(Context context) {
        super(context);
        init();
    }

    public ScreenCanvasView(Context context, AttributeSet attrs) {
        super(context, attrs);
        init();
    }

    public ScreenCanvasView(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        init();
    }

    private void init() {
        //点阵屏需要使用最近邻缩放
        paint.setFilterBitmap(false);
        paint.setAntiAlias(false);
        paint.setDither(false);
        setBackgroundColor(Color.BLACK);
    }

    private int[] mapPoint(float x, float y) {
        int w = frameWidth;
        int h = frameHeight;
        if (w <= 0 || h <= 0 || drawScale <= 0.0f) {
            mappedPoint[0] = 0;
            mappedPoint[1] = 0;
            return mappedPoint;
        }
        int px = Math.round((x - drawLeft) / drawScale);
        int py = Math.round((y - drawTop) / drawScale);
        if (px < 0) {
            px = 0;
        } else if (px >= w) {
            px = w - 1;
        }
        if (py < 0) {
            py = 0;
        } else if (py >= h) {
            py = h - 1;
        }
        mappedPoint[0] = px;
        mappedPoint[1] = py;
        return mappedPoint;
    }

    private static int toHguiButtonState(int state) {
        int result = 0;
        if ((state & MotionEvent.BUTTON_PRIMARY) != 0) {
            result |= 0x01;
        }
        if ((state & MotionEvent.BUTTON_TERTIARY) != 0) {
            result |= 0x02;
        }
        if ((state & MotionEvent.BUTTON_SECONDARY) != 0) {
            result |= 0x04;
        }
        if ((state & MotionEvent.BUTTON_BACK) != 0) {
            result |= 0x08;
        }
        if ((state & MotionEvent.BUTTON_FORWARD) != 0) {
            result |= 0x10;
        }
        return result;
    }

    private static int toHguiButton(int button) {
        switch (button) {
            case MotionEvent.BUTTON_PRIMARY:
                return Native.MOUSE_BUTTON_LEFT;
            case MotionEvent.BUTTON_TERTIARY:
                return Native.MOUSE_BUTTON_MIDDLE;
            case MotionEvent.BUTTON_SECONDARY:
                return Native.MOUSE_BUTTON_RIGHT;
            case MotionEvent.BUTTON_BACK:
                return Native.MOUSE_BUTTON_X1;
            case MotionEvent.BUTTON_FORWARD:
                return Native.MOUSE_BUTTON_X2;
            default:
                return 0;
        }
    }

    private void sendMouseMotion(MotionEvent event) {
        int[] point = mapPoint(event.getX(), event.getY());
        int x = point[0];
        int y = point[1];
        int xrel = 0;
        int yrel = 0;
        if (lastMouseValid) {
            xrel = x - lastMouseX;
            yrel = y - lastMouseY;
        }
        lastMouseX = x;
        lastMouseY = y;
        lastMouseValid = true;
        Native.mouseMotion(x, y, xrel, yrel, toHguiButtonState(event.getButtonState()));
    }

    private boolean handleMouseEvent(MotionEvent event) {
        switch (event.getActionMasked()) {
            case MotionEvent.ACTION_DOWN:
            case MotionEvent.ACTION_UP: {
                sendMouseMotion(event);
                int button = toHguiButton(event.getActionButton());
                if (button != 0) {
                    Native.mouseButton(button, event.getActionMasked() == MotionEvent.ACTION_DOWN, lastMouseX, lastMouseY);
                }
                return true;
            }
            case MotionEvent.ACTION_MOVE: {
                sendMouseMotion(event);
                return true;
            }
            case MotionEvent.ACTION_SCROLL: {
                float value = event.getAxisValue(MotionEvent.AXIS_VSCROLL);
                if (value != 0.0f) {
                    Native.mouseWheel(lastMouseX, lastMouseY, value > 0.0f ? 1 : -1);
                }
                return true;
            }
            default:
                return false;
        }
    }

    private void sendTouchEvent(MotionEvent event, int pointerIndex, int action) {
        int[] point = mapPoint(event.getX(pointerIndex), event.getY(pointerIndex));
        int pressure = (int) (event.getPressure(pointerIndex) * 65535.0f);
        if (pressure < 0) {
            pressure = 0;
        } else if (pressure > 65535) {
            pressure = 65535;
        }
        Native.touchEvent(action, event.getPointerId(pointerIndex), point[0], point[1], 0, 0, pressure);
    }

    private boolean handleTouchEvent(MotionEvent event) {
        switch (event.getActionMasked()) {
            case MotionEvent.ACTION_DOWN:
            case MotionEvent.ACTION_POINTER_DOWN:
                sendTouchEvent(event, event.getActionIndex(), Native.TOUCH_ACTION_DOWN);
                return true;
            case MotionEvent.ACTION_MOVE:
                for (int i = 0; i < event.getPointerCount(); i++) {
                    sendTouchEvent(event, i, Native.TOUCH_ACTION_MOTION);
                }
                return true;
            case MotionEvent.ACTION_UP:
            case MotionEvent.ACTION_POINTER_UP:
                sendTouchEvent(event, event.getActionIndex(), Native.TOUCH_ACTION_UP);
                return true;
            case MotionEvent.ACTION_CANCEL:
                for (int i = 0; i < event.getPointerCount(); i++) {
                    sendTouchEvent(event, i, Native.TOUCH_ACTION_UP);
                }
                return true;
            default:
                return false;
        }
    }

    @Override
    public boolean onTouchEvent(MotionEvent event) {
        if (event.isFromSource(InputDevice.SOURCE_MOUSE)) {
            return handleMouseEvent(event) || super.onTouchEvent(event);
        }
        return handleTouchEvent(event) || super.onTouchEvent(event);
    }

    @Override
    public boolean onGenericMotionEvent(MotionEvent event) {
        if (event.isFromSource(InputDevice.SOURCE_MOUSE)) {
            switch (event.getActionMasked()) {
                case MotionEvent.ACTION_HOVER_ENTER:
                case MotionEvent.ACTION_HOVER_MOVE:
                    sendMouseMotion(event);
                    return true;
                case MotionEvent.ACTION_SCROLL:
                    return handleMouseEvent(event);
                default:
                    break;
            }
        }
        return super.onGenericMotionEvent(event);
    }

    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);

        int vw = getWidth();
        int vh = getHeight();
        if (vw <= 0 || vh <= 0) {
            postInvalidateOnAnimation();
            return;
        }

        int w = Native.getWidth();
        int h = Native.getHeight();
        if (w <= 0 || h <= 0) {
            postInvalidateOnAnimation();
            return;
        }

        if (w != frameWidth || h != frameHeight || frameBitmap == null || framePixels == null) {
            frameWidth = w;
            frameHeight = h;
            framePixels = new int[w * h];
            frameBitmap = Bitmap.createBitmap(w, h, Bitmap.Config.ARGB_8888);
        }

        Native.copyFrame(framePixels);
        frameBitmap.setPixels(framePixels, 0, frameWidth, 0, 0, frameWidth, frameHeight);

        //按比例缩放并居中
        float scale = Math.min(((float) vw) / frameWidth, ((float) vh) / frameHeight);
        float dw = frameWidth * scale;
        float dh = frameHeight * scale;
        float left = (vw - dw) / 2.0f;
        float top = (vh - dh) / 2.0f;
        drawScale = scale;
        drawLeft = left;
        drawTop = top;
        RectF dst = new RectF(left, top, left + dw, top + dh);
        canvas.drawBitmap(frameBitmap, null, dst, paint);

        //下一帧继续绘制，以保证界面及时更新
        postInvalidateOnAnimation();
    }
}
