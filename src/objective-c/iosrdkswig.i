/* File : example.i */
%module iosrdkswig
%include <objc.swg>
%include <objcproxycode.swg>
%include <objcenums.swg>
%include <objcruntime.swg>
%include <objckw.swg>
%include <std_common.i>
%include <std_deque.i>
%include <std_except.i>
%include <std_map.i>
%include <std_pair.i>
%include <std_string.i>
%include <std_vector.i>
%include <typemaps.i>


//convert NSData* to (void* buffer)
%typemap(objctype) (void* buffer) "NSData *"
%typemap(objcin) (void* buffer) "$objcinput"
%typemap(imtype) (void* buffer) "NSData *"
%typemap(in) (void* buffer){
    $1 = 0;
    if ($input) {
        $1 = (void*)[$input bytes];
    }
}

//convert (unsigned char* getBuffer) to NSData*
%typemap(objctype) (unsigned char* getBuffer) "NSData *"
%typemap(objcout) (unsigned char* getBuffer) %{ return $imcall; %}
%typemap(imtype) (unsigned char* getBuffer) "NSData *"
%typemap(out) (unsigned char* getBuffer) {
    if ($1) {
        $result = [NSData dataWithBytes: $1 length:strlen((const char*)$1)];
    }
}

//convert NSString* to FSString
%typemap(objctype) (FSString*) "NSString *"
%typemap(objcin) (FSString*) "$objcinput"
%typemap(imtype) (FSString*) "NSString *"
%typemap(in) (FSString*){
    $1 = 0;
    if ($input) {
        const char* temp = [$input UTF8String];
        $1 = new FSString(temp);
    }
}

//convert FSString to NSString*
%typemap(objctype) (FSString) "NSString *"
%typemap(objcout) (FSString) %{ return $imcall; %}
%typemap(imtype) (FSString) "NSString *"
%typemap(out) (FSString) {
    if (0 < ($1).getBufferLen()) {
        $result = [NSString stringWithUTF8String: (const char*)(($1).getBuffer())];
    }
}

//convert FSString* to NSString*
%typemap(objctype) (FSString*) "NSString *"
%typemap(objcout) (FSString*) %{ return $imcall; %}
%typemap(imtype) (FSString*) "NSString *"
%typemap(out) (FSString*) {
    if ($1 && 0 < $1->getBufferLen()) {
        $result = [NSString stringWithUTF8String: (const char*)($1->getBuffer())];
    }
}

//convert NSMutableArray* to (int dashPatterns[16])
%typemap(objctype) (int dashPatterns[16]) "NSMutableArray *"
%typemap(objcin) (int dashPatterns[16]) "$objcinput"
%typemap(imtype) (int dashPatterns[16]) "NSMutableArray *"
%typemap(in) (int dashPatterns[16]){
    $1 = 0;
    if ($input) {
        int temp[16] = {};
        memset(temp, -1, 16*sizeof(int));
        int count = [$input count];
        for (int i = 0; i < count; i++)
        {
            temp[i] = [[$input objectAtIndex:i] intValue];
        }
        $1 = temp;
    }
}

//convert (int dashPatterns[16]) to NSMutableArray*
%typemap(objctype) (int dashPatterns[16]) "NSMutableArray *"
%typemap(objcout) (int dashPatterns[16]) %{ return $imcall; %}
%typemap(imtype) (int dashPatterns[16]) "NSMutableArray *"
%typemap(out) (int dashPatterns[16]) {
    if ($1) {
        $result = [NSMutableArray array];
        for (int i = 0; i < 16; i++)
        {
            if (-1 == $1[i])
                break;
            NSNumber *num = [NSNumber numberWithInt:$1[i]];
            [$result addObject:num];
            
        }
    }
}

//convert CGContextRef to CGContextRef
%typemap(objctype) (CGContextRef) "CGContextRef"
%typemap(objcin) (CGContextRef) "$objcinput"
%typemap(imtype) (CGContextRef) "CGContextRef"
%typemap(in) (CGContextRef){
    $1 = 0;
    if ($input) {
        $1 = $input;
    }
}


//%apply (void* buffer) {NSData *}
//%typemap(imtype) void * "NSData *"
//%typemap(objctype) void * "NSData *"
//%apply CGContextRef {void* context};
//%typemap(imtype) (const void* buffer, int len) "NSData*"
//%typemap(objctype) (const void* buffer, int len) "NSData*"
//%typemap(in) (const char password[], int passlen){
//    //$1 = (const char *) [$input cStringUsingEncoding:NSUTF8StringEncoding];
//    //$2 = (int) [$input length];
//    $1 = 0;
//    $2 = 0;
//    if ($input) { // TODO: Find a better way to do this!
//        int length = [$input length];
//        int bufferSize = sizeof(char) * (length + 1);
//        char *buffer = (char*)malloc(bufferSize);
//        memset(buffer, 0, bufferSize);
//        int i = 0;
//        for (; i < length; ++i)
//            buffer[i] = [$input characterAtIndex: i];
//        $1 = ($1_ltype)buffer;
//        $2 = length;
//    }
//}



%{
#include "../../include/common.h"
#include "../../include/fs_action.h"
#include "../../include/fs_annot.h"
#include "../../include/fs_bookmark.h"
#include "../../include/fs_readingbookmark.h"
#include "../../include/fs_pdfdoc.h"
#include "../../include/fs_pdfobject.h"
#include "../../include/fs_pdfpage.h"
#include "../../include/fs_render.h"
#include "../../include/fs_search.h"
#include "../../include/fs_pdfform.h"
#include "../../include/fs_formfiller.h"
#include "../../include/fs_signature.h"
%}

%insert(runtime) %{
#import <CoreGraphics/CoreGraphics.h>
%}

/* Let's just grab the original header file here */
%include "../../include/common.h"
%include "../../include/fs_action.h"
%include "../../include/fs_annot.h"
%include "../../include/fs_bookmark.h"
%include "../../include/fs_readingbookmark.h"
%include "../../include/fs_pdfdoc.h"
%include "../../include/fs_pdfobject.h"
%include "../../include/fs_pdfpage.h"
%include "../../include/fs_render.h"
%include "../../include/fs_search.h"
%include "../../include/fs_pdfform.h"
%include "../../include/fs_formfiller.h"
%include "../../include/fs_signature.h"

