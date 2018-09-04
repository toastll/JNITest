package com.study.pluto.jnitest;

import android.os.Looper;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;
import android.widget.Toast;

public class MainActivity extends AppCompatActivity {

    static {
        //调用System.load()函数时，内部就会去查找so库中的JNI_OnLoad(JavaVM* vm,void* reserved)函数，
        //如果存在该函数则调用。
        //告诉VM此native组件使用的版本，为了对应java版本，Android只支持
        // JNI_VERSION_1_2 JNI_VERSION_1_4 JNI_VERSION_1_6
        //JDK1.8中有JNI_VERSION_1_8
        System.loadLibrary("native-lib");

//        System.load();
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Example of a call to a native method
        TextView tv = (TextView) findViewById(R.id.sample_text);
        tv.setText(stringFromJNI());
        Log.d("MY_JNI_TAG",stringFromJNI());

        //字符串数组
        String s[] = {"你", "好"};
        getString_s(s);
        //整型数组
        int i[] = {1, 2, 3, 4, 5, 6};
        getInt(i);

        Bean bean = new Bean(0);
        getBean(bean);

        invokeMethod();
        testReflect();
        Log.d("MY_JNI_TAG",showLocalReference());
        Log.d("MY_JNI_TAG",showLocalReference());

        Class c = testWeakGlobalReference();
        Log.d("MY_JNI_TAG",c.getName().toString());

        dynamicNative();
//        dynamicNativeReturn("ssss");
//        dynamicNativeReturn("8");
        Log.d("MY_JNI_TAG",dynamicNativeReturn("888888"));;
        testThread();
    }


    //动态注册
    native void dynamicNative();
//    native void dynamicNativeReturn(String i);
    native String dynamicNativeReturn(String i );
    public void updateUi(){
        if (Looper.myLooper() == Looper.getMainLooper()){//如果是主线程
            Toast.makeText(this,"更新UI了啊",Toast.LENGTH_SHORT).show();
        }else {//如果不是主线程
            runOnUiThread(new Runnable() {//runOnUiThread
                @Override
                public void run() {
                    Toast.makeText(MainActivity.this,"更新UI了啊",Toast.LENGTH_SHORT).show();
                }
            });

        }
    }

    native void testThread();
    native Class testWeakGlobalReference();
    native String showLocalReference();

    public native String stringFromJNI();

    native void testReflect();

    native void tt(int i, String s);

    native void getInt(int[] intArrs);

    native void invokeMethod();


    //获取数组
    //java方法名中的  _  在C++中的表示方式为  _1
    //当方法名中含有_（如  _s ）时，C++中的命名为  _1s_
    native void getString_s(String[] str);

    //获取对象
    native void getBean(Bean bean);
}
