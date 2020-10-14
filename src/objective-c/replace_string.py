
import os

def replace_fsannot():
    arr = ["iosrdkswig_proxy.h","iosrdkswig_proxy.mm"]
    for path in arr:
        if not os.path.exists(path):
            exit(-1)
        
        f = open(path,'r+')
        lines = f.readlines()
        f.seek(0)
        f.truncate()
        for s in lines:
            f.write(s.replace(' FSAnnot_',' '))
        f.close()

def replace_declearation():
    f = open("iosrdkswig_proxy.h",'r+')
    content = f.read()
    f.seek(0)
    pos = content.find("@interface SWIGTYPE_p_void : NSObject")
    if pos != -1:
        declearation = '''#import <CoreGraphics/CoreGraphics.h>
@class FSAnnotIconProvider;
@class FSPDFDoc;
@class FSPDFPage;
@class FSPDFTextSelect;
@class FSBorderInfo;
@class FSPDFDictionary;
@class FSPDFObject;
@class FSPDFStream;
@class FSPDFArray;
@class FSNote;
@class FSBitmap;

'''
        content = content[:pos] + declearation + content[pos:]
        f.write(content)
    f.close()

#def replace_fspdfdoc():
#    f = open("iosrdkswig_proxy.h",'r+')
#    content = f.read()
#    f.seek(0)
#    pos = content.find("-(enum FS_ERRORCODE)load: (NSString *)password passlen: (int)passlen")
#    if pos != -1:
#        string = "-(id)initWithPath:(NSString *)path;\n"
#        content = content[:pos] + string + content[pos:]
#        f.write(content)
#    f.close()
#
#    f = open("iosrdkswig_proxy.mm",'r+')
#    content = f.read()
#    f.seek(0)
#    pos = content.find("-(enum FS_ERRORCODE)load: (NSString *)password passlen: (int)passlen")
#    if pos != -1:
#        string = '''-(id)initWithPath:(NSString *)path
#{
#
#    if((self = [super init])) {
#        void* cptr = _wrap_new_FSPDFDocWithPath(path);
#        swigCPtr = cptr;
#        swigCMemOwn = YES;
#    }
#    return self;
#
#}
#
#'''
#        content = content[:pos] + string + content[pos:]
#        f.write(content)
#    f.close()
#
#    f = open("iosrdkswig_wrap.h",'r+')
#    content = f.read()
#    f.seek(0)
#    pos = content.find("int _wrap_FSPDFDoc_load(void* imarg1, NSString * imarg2, int imarg3)")
#    if pos != -1:
#        string = "void* _wrap_new_FSPDFDocWithPath(NSString *imarg1);\n"
#        content = content[:pos] + string + content[pos:]
#        f.write(content)
#    f.close()
#
#    f = open("iosrdkswig_wrap.mm",'r+')
#    content = f.read()
#    f.seek(0)
#    pos = content.find("int _wrap_FSPDFDoc_load(void* imarg1, NSString * imarg2, int imarg3)")
#    if pos != -1:
#        string = '''void *_wrap_new_FSPDFDocWithPath(NSString *imarg1)
#{
#    void* imresult = 0 ;
#    FSPDFDoc *result = 0 ;
#    char *arg1 = "";
#    
#    if (imarg1) {
#        arg1 = (char *)[imarg1 UTF8String];
#    }
#    result = (FSPDFDoc *)new FSPDFDoc(arg1);
#    imresult = (void*)result;
#    return imresult;
#}
#
#'''
#        content = content[:pos] + string + content[pos:]
#        f.write(content)
#    f.close()


def main():
    replace_fsannot()
    replace_declearation()
#    replace_fspdfdoc()

if __name__=='__main__':
    main()