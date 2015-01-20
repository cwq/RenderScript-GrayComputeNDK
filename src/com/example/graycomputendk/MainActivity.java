package com.example.graycomputendk;

import android.app.Activity;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.ImageView;

public class MainActivity extends Activity {

    private Bitmap inBitmap;
    private Bitmap outBitmap;
    
    private ImageView inImageView;
    private ImageView outImageView;
    
    private String cacheDir;
    
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        // TODO Auto-generated method stub
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        
        cacheDir = this.getCacheDir().getAbsolutePath();
        Log.e("cacheDir", cacheDir);
        
        inBitmap = loadBitmap(R.drawable.pano);
        outBitmap = Bitmap.createBitmap(inBitmap.getWidth(), inBitmap.getHeight(), inBitmap.getConfig());
        
        inImageView = (ImageView) findViewById(R.id.in);
        outImageView = (ImageView) findViewById(R.id.out);
        inImageView.setImageBitmap(inBitmap);
        outImageView.setImageBitmap(outBitmap);
        
        Button grayButton = (Button) findViewById(R.id.button1);
        grayButton.setOnClickListener(new OnClickListener() {
            
            @Override
            public void onClick(View v) {
                // TODO Auto-generated method stub
                JNIHelper.nativeMono(inBitmap.getWidth(), inBitmap.getHeight(), inBitmap, outBitmap, cacheDir);
                outImageView.setImageBitmap(outBitmap);
                outImageView.invalidate();
            }
        });
    }
    
    private Bitmap loadBitmap(int resId) {
        final BitmapFactory.Options options = new BitmapFactory.Options();
        options.inPreferredConfig = Bitmap.Config.ARGB_8888;
        return BitmapFactory.decodeResource(getResources(), resId, options);
    }
}
