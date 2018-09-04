#include <jni.h>
#include <string>
//引入头文件，使得C++可以将信息输出到Android的logcat中
#include <android/log.h>
#include <pthread.h>


//   __VA_ARGS__  代表可变参数 ...
#define LOG(jstring, ...)  __android_log_print(ANDROID_LOG_DEBUG,jstring,__VA_ARGS__);
#define LOGE(...)  __android_log_print(ANDROID_LOG_DEBUG,"MY_JNI_TAG",__VA_ARGS__);



extern "C"
JNIEXPORT jstring
JNICALL
Java_com_study_pluto_jnitest_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    //android/log.h 中的原始用法，
    //第一个参数是log的等级（比如debug，error等），第二个参数是TAG，设置筛选标志，第三个是要输出的字符串，可以使用占位符
    __android_log_print(ANDROID_LOG_DEBUG, "JNI_TAG", "我被输出出来了", 5);
    __android_log_print(ANDROID_LOG_DEBUG, "JNI_TAG", "我被第%d次输出出来了", 5);
    LOG("MY_TAG", "自己定义的输出的log日志,有占位符%d", 5);
    LOG("MY_TAG_HAVE", "自己定义的输出的log日志，没有占位符");
    return env->NewStringUTF(hello.c_str());
}


extern "C"
JNIEXPORT void JNICALL
Java_com_study_pluto_jnitest_MainActivity_tt(JNIEnv *env, jobject instance, jint i, jstring s_) {
    const char *s = env->GetStringUTFChars(s_, 0);


    env->ReleaseStringUTFChars(s_, s);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_study_pluto_jnitest_MainActivity_getString_1s(JNIEnv *env, jobject instance,
                                                       jobjectArray str) {
    //1.获取数组长度
    jsize length = env->GetArrayLength(str);
    for (int i = 0; i < length; ++i) {
        jstring pJobject = (jstring) env->GetObjectArrayElement(str, i);
        const char *string = env->GetStringUTFChars(pJobject, 0);
        LOGE("字符串数组中的元素：%s", string);
        //用完就释放，get=====release
        env->ReleaseStringUTFChars(pJobject, string);
    }
    // jobject pJobject = env->GetObjectArrayElement(str,);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_study_pluto_jnitest_MainActivity_getBean(JNIEnv *env, jobject instance, jobject bean) {

    //反射调用java方法
    //1. 获取java对应的class对象
    jclass mBean = env->GetObjectClass(bean);
    //2.找到要调用的方法
    //获取方法id
    jmethodID getI = env->GetMethodID(mBean, "getI", "()I");
    jmethodID setI = env->GetMethodID(mBean, "setI", "(I)V");
    env->CallVoidMethod(bean, setI, 5);

    //通过bean与方法id来完成调用
    jint callIntMethod = env->CallIntMethod(bean, getI);
    LOGE("C++调用JAVA的getI方法，i=%d", callIntMethod);

    //===============================================================================
//    //获取构造方法
//    jmethodID constructMethod = env->GetMethodID(mBean, "<init>", "(I)V");
//    //创建对象
//    jobject newBean = env->NewObject(mBean, constructMethod);
//
//
    //static方法
    jmethodID printInfo = env->GetStaticMethodID(mBean, "printInfo", "(Ljava/lang/String;)V");
    //new一个java中的字符串，记得释放
    jstring str = env->NewStringUTF("通过C++调用了java中的static方法");
    //不要忘了写入参数
    env->CallStaticVoidMethod(mBean, printInfo, str);
    //释放局部的引用=======================================================================
    env->DeleteLocalRef(str);



    //在jni创建java对象
    jclass student = env->FindClass("com/study/pluto/jnitest/Student");
    //反射创建对象
    //1.获得类的构造方法
    jmethodID construct = env->GetMethodID(student, "<init>", "(I)V");
    //2.调用构造方法，创建对象
    jint i = 88;
    jobject studentBean = env->NewObject(student, construct, i);

    jmethodID show = env->GetMethodID(student, "show", "()V");
    env->CallVoidMethod(studentBean, show);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_study_pluto_jnitest_MainActivity_getInt(JNIEnv *env, jobject instance,
                                                 jintArray intArrs_) {
    //=========================整型数组
    //获取数组长度
    int32_t length = env->GetArrayLength(intArrs_);
    LOGE("数组长度：%d", length);
    //获取数组的首元素指针
    jint *pInt = env->GetIntArrayElements(intArrs_, 0);
    for (int i = 0; i < length; ++i) {
        //得到了数组首地址，可以通过下标的方式操作该数组中的元素
        LOGE("intArrs_数组中的元素有：%d", pInt[i]);
    }
    //使用完毕，确保该方法内之后不再使用该数组，就可以释放了，此时是局部引用
    //参数3：mode
    //0: 刷新java数组，并释放C++数组
    //1: JNI_COMMIT  只刷新java数组
    //2: JNI_ABORT  只释放
    env->ReleaseIntArrayElements(intArrs_, pInt, 0);

}

extern "C"
JNIEXPORT void JNICALL
Java_com_study_pluto_jnitest_MainActivity_invokeMethod(JNIEnv *env, jobject instance) {


    jclass clazz = env->FindClass("com/study/pluto/jnitest/Bean");
    //==================================================================这里写的限定名中，包名中的.要使用/来代替
    //参数一定要写详细，记得确认方法名，签名是否正确
    jmethodID staticMethod = env->GetStaticMethodID(clazz, "staticMethod",
                                                    "(Ljava/lang/String;IZ)V");
    jstring str = env->NewStringUTF("我是从C++调用的静态方法");
    env->CallStaticVoidMethod(clazz, staticMethod, str, 5, 1);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_study_pluto_jnitest_MainActivity_testReflect(JNIEnv *env, jobject instance) {
    //反射获取field属性
    jclass mClass = env->FindClass("com/study/pluto/jnitest/Student");
    jfieldID age = env->GetFieldID(mClass, "age", "I");
    jmethodID construct = env->GetMethodID(mClass, "<init>", "()V");
    jobject mObject = env->NewObject(mClass, construct);
    env->SetIntField(mObject, age, 88);


    jstring nameStr = env->NewStringUTF("李大钊同志");
    //反射一个引用类型的字段field时，千万千万不要忘了在最后加上  ；
    jfieldID name = env->GetStaticFieldID(mClass, "name", "Ljava/lang/String;");
    env->SetStaticObjectField(mClass, name, nameStr);

    jmethodID show = env->GetMethodID(mClass, "show", "()V");
    env->CallVoidMethod(mObject, show);

    //释放资源：
    env->DeleteLocalRef(mObject);
    env->DeleteLocalRef(mClass);
    env->DeleteLocalRef(nameStr);
//    env->GetFieldID()
}

jstring str;
jclass localStudent;

extern "C"
JNIEXPORT jstring JNICALL     //JNIEXPORT   [方法的返回值类型]  JNICALL
Java_com_study_pluto_jnitest_MainActivity_showLocalReference(JNIEnv *env, jobject instance) {

//    if (localStudent == NULL) {
//        localStudent = env->FindClass("com/study/pluto/jnitest/Student");
//    }

    //d第一次调用的时候是没有问题的 关键就是经过第一次的调用之后，str指向了一个地址，env->NewStringUTF("C++中的调用invoke");代表的地址
    //但是当我们再一次调用该方法的时候，由于env->NewStringUTF()方法得到的是一个局部引用，其中的值，在该方法第一次结束之后就被释放掉了
    //所以在第二次执行的时候，str依然有值，但是其引用的“C++中的调用invoke”已经被释放掉了
    //释放一个局部引用有两个方式：
    //  1.本地方法执行完毕后，VM自动释放      2.通过deleteLocalRef()方法手动释放局部引用
    //既然GC会自动释放局部引用，为什么还要手动释放呢？
    // 这是因为局部引用会阻止它所引用的对象被回收，当GC想要回收该对象时是无法回收成功的，因为它还被一个局部引用引用着
    //所以需要DeleteLocalReference()来释放
    //================================================错误 todo
//    if (str == NULL) {
//        str = env->NewStringUTF("C++中的调用invoke");
//    }

    static jstring globalRef;
    if (globalRef == NULL) {
        jstring str1;
        str1 = env->NewStringUTF("C++全局引用测试"+123);
//        globalRef = str1;//错误操作【不可以将一个局部引用直接赋值给一个全局引用】
//        globalRef = static_cast<jstring>(env->NewGlobalRef(str1));
        globalRef = (jstring) env->NewGlobalRef(str1);
        //上面的两种转换方式都是可以的

        //此时，有了一个全局引用使用局部引用str1，而且以后也不会使用str1了，所以此处就可以释放str1了
//        env->DeleteLocalRef(str1);

    }
    return globalRef;
}

extern "C"
JNIEXPORT jclass JNICALL
Java_com_study_pluto_jnitest_MainActivity_testWeakGlobalReference(JNIEnv *env, jobject instance) {
    static jclass weakClass;
    //判定一个Object 是不是NULL的方法就是通过isSameObject()方法将之与NULL进行对比，若相等，就返回true
    jboolean isEquals = env->IsSameObject(weakClass, NULL);
    if (weakClass == NULL || isEquals){
        //创建一个临时对象
        jclass tempClass= env->GetObjectClass(instance);
        //将mClass升级为一个弱引用
        weakClass = (jclass) env->NewWeakGlobalRef(tempClass);
        //释放不使用的临时引用对象
        env->DeleteWeakGlobalRef(tempClass);
    }
    return weakClass;
}

//================================================================动态注册  MainActivity============================

//配置一个静态变量 用来存放需要动态注册的方法所在类名，方便填写
const char *classNameMainActivity = "com/study/pluto/jnitest/MainActivity";


void dynamicNative(JNIEnv *env, jobject instance){
    LOGE("dynamicNative动态注册");
}

//
//void dynamicNativeReturn(JavaVM* vm, jobject instance,jstring i){
////    int length;
////
//    JNIEnv* env ;
//    jint r = vm->GetEnv((void**) &env, JNI_VERSION_1_6);
////    if (r != JNI_OK){
////        throw "获取env对象失败~~";
////    }
////    length = env->GetStringUTFLength(i);
//////    return env->NewStringUTF("从java中获取到的字符串长度是"+length);
//    LOGE("=================================================");
////    return env->NewStringUTF("==========================");
//}

jstring dynamicNativeReturn(JNIEnv* env,jobject instance,jstring mystring){

    LOGE("=======================dynamicNativeReturn==============================");

    return env->NewStringUTF(classNameMainActivity);

}
//需要动态注册的方法数组
static const JNINativeMethod mMethods[]={
        {"dynamicNative","()V",(void*)dynamicNative},
        {"dynamicNativeReturn","(Ljava/lang/String;)Ljava/lang/String;",(jstring*)dynamicNativeReturn}
};

JavaVM *_vm=0;
jobject  _instance=0;
/**
 * declared in jni.h
 * @param vm
 * @param reserved
 * @return
 */
jint JNI_OnLoad(JavaVM* vm, void* reserved){
    _vm = vm;
    // 2  4  6 都可以
    LOGE("JNI_OnLoad(JavaVM* vm, void* reserved)被调用了");
    JNIEnv *env = NULL;
    //获得JNIEnv  ，getEnv的返回值是一个jint值，通过这个值可以进行判断
    //#define JNI_OK          (0)         /* no error */
    //#define JNI_ERR         (-1)        /* generic error */
    int r = vm->GetEnv((void**)&env,JNI_VERSION_1_6);
    if(r!=JNI_OK){//获取失败
        return -1;
    }
    //获取到需要注册的方法所在类
    jclass mainActivityCls = env->FindClass(classNameMainActivity);
    //动态注册  如果返回值小于0  注册失败
    r = env->RegisterNatives(mainActivityCls,mMethods, sizeof(mMethods)/ sizeof(JNINativeMethod));
    if(r!=JNI_OK){
        return -1;
    }
    return JNI_VERSION_1_6;
}

void *task(void *args){
//    jobject mobject = static_cast<jobject >(args);
    JNIEnv *env;
    jint isSuccess = _vm->AttachCurrentThread(&env, 0);//将native线程附加到到JVM虚拟机  ，并获取与线程相关的env

//    jint isSuccess = _vm->GetEnv((void **) &env, JNI_VERSION_1_6);
    if(isSuccess!=JNI_OK){
        LOGE("获取env失败了");
        return 0;
    }
//    if(isSuccess!=JNI_OK){
//        LOGE("env添加到主线程失败了");
//        return 0;
//    }
    jclass pJclass = env->GetObjectClass(_instance);
    jmethodID updateUi = env->GetMethodID(pJclass, "updateUi", "()V");
    env->CallVoidMethod(_instance,updateUi);

    _vm->DetachCurrentThread();
    return 0;
//    JNIEnv* env = _vm->GetEnv()
}

extern "C"
JNIEXPORT void JNICALL
Java_com_study_pluto_jnitest_MainActivity_testThread(JNIEnv *env,jobject instance){
    pthread_t  myThread;
    _instance=env->NewGlobalRef(instance);
    pthread_create(&myThread,0,task,0);

}

