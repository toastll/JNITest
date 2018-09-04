package com.study.pluto.jnitest;

import android.util.Log;

public class Student {

    int age = 12345;
    static String name ="name";

    public Student(int age) {
        this.age = age;
    }
    public Student() {

    }

    public void show(){
        Log.d("MY_JNI_TAG","Student的Age值= "+age+"  name="+name);
    }
}
