package cn.hyhsystem.hcppbox.monochromescreen;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.RectF;
import android.util.AttributeSet;
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
        RectF dst = new RectF(left, top, left + dw, top + dh);
        canvas.drawBitmap(frameBitmap, null, dst, paint);

        //下一帧继续绘制，以保证界面及时更新
        postInvalidateOnAnimation();
    }
}
