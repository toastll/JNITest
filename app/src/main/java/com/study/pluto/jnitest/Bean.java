package com.study.pluto.jnitest;

import android.util.Log;

/**
 * Created by Administrator on 2018/8/29.
 */

public class Bean {
    int i=0;
    private static int m;
    private static void staticMethod(String a,int b,boolean c){
        Log.d("MY_JNI_TAG","staticMethod a=" +a +" b="+b+" c="+c);
    }

    public static void printInfo(String msg){
        Log.d("MY_JNI_TAG",msg);
//        Log.e("MY_JNI_TAG",msg);
    }

    Bean(int  i){
        this.i = i;
    }
    //private与public之类的在native中一样调用，没啥区别
    private int getI() {
        Log.d("MY_JNI_TAG","java中的geti(),i=%d"+i);
        return i;
    }

    public void setI(int i) {
        Log.d("MY_JNI_TAG","java中的geti()");
        this.i = i;
    }
}
