package com.study.pluto.jnitest;

import org.junit.Test;

import static org.junit.Assert.*;

/**
 * Example local unit test, which will execute on the development machine (host).
 *
 * @see <a href="http://d.android.com/tools/testing">Testing documentation</a>
 */
    public class ExampleUnitTest {
    @Test
    public void addition_isCorrect() throws Exception {
        assertEquals(4, 2 + 2);
        System.load("C:\\Users\\Administrator\\CMakeBuilds\\6ddf0070-34b6-c53f-96eb-8f3dabb4ecde\\build\\x64-Debug\\lsn7_jni\\myjni.dll");
        String s = test(2,"999",2.1f);
        System.out.println(s);
    }

    native String test(int i, String j , float k);
}