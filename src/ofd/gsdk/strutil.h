//#include <jni.h>
#include <stdlib.h>
#include <vector>

class autoFree{
    std::vector <void *> m_vp;
public:
    autoFree(){}
    autoFree(void *p)
    {
        m_vp.push_back(p);
    }
    void append(void *p)
    {
        m_vp.push_back(p);
    }
    ~autoFree()
    {
        for(std::vector <void *>::iterator it = m_vp.begin(); it != m_vp.end(); it++)
            ;//free(*it);
    }
};
wchar_t* toWchar(char* src,  const char *locale = "zh_CN.utf8");
//wchar_t* jstringToWchar(JNIEnv* env, jstring str);
//wchar_t* jstringToWchar2(JNIEnv* env, jstring str);
//std::wstring Java_To_WStr(JNIEnv *env, jstring string);
//char* jstringtochar( JNIEnv *env, jstring jstr );
//char* jstringToChar(JNIEnv *env, jstring jstr);
//jstring charTojstring(JNIEnv* env, const char* str, int slen = 0, int type = 0);
//jstring wcharTojstring(JNIEnv* env, const wchar_t* wstr);
char* wcharTochar(const wchar_t* wstr);
//jstring stringToJstring(JNIEnv* env,std::string str);
//jstring charTojstring2(JNIEnv* env, const char* pat);
