#include <wchar.h>
#include <string.h>
#include <string>
#include "strutil.h"


//char* jstringToChar( JNIEnv *env, jstring jstr )
//{
//    char* rtn = NULL;
//    jclass clsstring = (env)->FindClass("java/lang/String");
//    jstring strencode = (env)->NewStringUTF("utf-8");
//    jmethodID mid = (env)->GetMethodID(clsstring, "getBytes", "(Ljava/lang/String;)[B");
//    jbyteArray barr= (jbyteArray)(env)->CallObjectMethod(jstr, mid, strencode);
//    jsize alen = (env)->GetArrayLength(barr);
//    jbyte* ba = (env)->GetByteArrayElements(barr, JNI_FALSE);
//    if (alen > 0)
//    {
//        rtn = (char*)malloc(alen + 1);
//        memcpy(rtn, ba, alen);
//        rtn[alen] = 0;
//    }
//    (env)->ReleaseByteArrayElements(barr, ba, 0);
//    return rtn;
//}

char* wcharTochar(const wchar_t *wstr)
{
    int length = wcslen(wstr);
    char* rtn = (char*) malloc(length * 2 + 1);
    int size = wcstombs(rtn, (const wchar_t*)wstr, length*2+1);
    if (size <= 0)
    {
        free(rtn);
        return NULL;
    }
    rtn[size] = 0;
    return rtn;
}

//jstring charTojstring(JNIEnv* env, const char* str, int slen, int type) {
//    jstring rtn = 0;
//    if(slen == 0)
//        slen = strlen(str);
//    wchar_t *pwcs = (wchar_t *)malloc( slen + 2);
//    if (type == 0)
//        rtn = (env)->NewStringUTF(str);
//    else {
//        int length = mbstowcs( pwcs, str, slen/2 + 1);
//        if (length > 0)
//            rtn = (env)->NewString((jchar*) pwcs, length);
//    }
//    free(pwcs);
//    return rtn;
//}


wchar_t* toWchar(char* src, const char *locale)
{

    if (src == NULL) {
         return 0;
    }
      //根据环境变量设置locale
       // const char * oldLocale = getlocale();
       //setlocale(LC_CTYPE, locale);
       //得到转化为需要的宽字符大小
      int w_size = mbstowcs(NULL, src, 0) + 1;
       //w_size=0说明mbstowcs返回值为-1。即在运行过程中遇到了非法字符(很有可能使locale没有设置正确)
       if (w_size == 0) {
             return NULL;
           }
       wchar_t  *dest = new wchar_t[w_size * 2 + 1];
       if (!dest) return 0;
       memset(dest, 0, sizeof(wchar_t) * (w_size * 2 + 1) );
       int ret = mbstowcs(dest, src, strlen(src)+1);
       if (ret <= 0)return 0;
       return dest;
}

//wchar_t* jstringToWchar(JNIEnv* env, jstring str)
//{
//  int len = env->GetStringLength(str);
//  if(len == 0)
//      return NULL;
//  wchar_t *w_buffer = new wchar_t[len];
//  memset(w_buffer,0,len+1);
// w_buffer[len]='\0';
// wchar_t * tmp = (wchar_t *)env->GetStringChars(str,0);
//  wcsncpy(w_buffer,tmp,len);
// env->ReleaseStringChars(str,(const unsigned short *)tmp);
//  return w_buffer;
//}
//
//std::wstring Java_To_WStr(JNIEnv *env, jstring string)
//{
//    std::wstring value;
//
//    const jchar *raw = env->GetStringChars(string, 0);
//    jsize len = env->GetStringLength(string);
//    value.assign(raw, raw + len);
//
//    env->ReleaseStringChars(string, raw);
//
//    return value;
//}

//wchar_t* jstringToWchar2(JNIEnv* env, jstring str)
//{
//    int len = env->GetStringLength(str);
//    if(len == 0)
//        return NULL;
//    wchar_t *w_buffer = new wchar_t[len];
//    memset(w_buffer,0,len+1);
//    w_buffer[len]='\0';
//    wchar_t * tmp = (wchar_t *)env->GetStringUTFChars(str,0);
//    wcsncpy(w_buffer,tmp,len);
//    env->ReleaseStringChars(str,(const unsigned short *)tmp);
//    return w_buffer;
//}
//
//jstring wcharTojstring(JNIEnv* env, const wchar_t* wstr) {
//    jstring rtn = 0;
//    int length = wcslen(wstr);
//    rtn = (env)->NewString((jchar*) wstr, length*2);
//    return rtn;
//}

// 将char *转为jstring
//jstring charTojstring2(JNIEnv* env, const char* pat)
//{
//    //定义java String类 strClass
//    jclass strClass = (env)->FindClass("Ljava/lang/String;");
//    //获取String(byte[],String)的构造器,用于将本地byte[]数组转换为一个新String
//    jmethodID ctorID = (env)->GetMethodID(strClass, "<init>", "([BLjava/lang/String;)V");
//    //建立byte数组
//    jbyteArray bytes = (env)->NewByteArray(strlen(pat));
//    //将char* 转换为byte数组
//    (env)->SetByteArrayRegion(bytes, 0, strlen(pat), (jbyte*) pat);
//    // 设置String, 保存语言类型,用于byte数组转换至String时的参数
//    jstring encoding = (env)->NewStringUTF("GB2312");
//    //将byte数组转换为java String,并输出
//    return (jstring) (env)->NewObject(strClass, ctorID, bytes, encoding);
//}
//
//// 先将string转为char *，再将char *转为jstring
//jstring stringToJstring(JNIEnv* env,std::string str)
//{
//    // string 转 char*
//    const char* chardata = str.c_str();
//    // char* 转 jstring
//    jstring jstr = charTojstring(env, chardata);
//    return jstr;
//}
