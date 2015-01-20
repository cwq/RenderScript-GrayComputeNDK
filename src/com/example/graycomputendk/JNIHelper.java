package com.example.graycomputendk;

import android.graphics.Bitmap;

public class JNIHelper {

    static {
        System.loadLibrary("stlport_shared");
        System.loadLibrary("RSSupport");
        System.loadLibrary("graycompute");
    }
    
    public static native void nativeMono(int w, int h, Bitmap in, Bitmap out, String name);
}
