
rm -f *_proxy.h *_proxy.mm *_wrap.h *_wrap.mm
if [ “$1” == “all” ];then
rm -rf *.o
rm -rf libswig.a
fi;

swig -objc -c++ -D_SWIG_IOS_ -D__APPLE__ -D__x86_64__ iosrdkswig.i
python replace_string.py


if [ “$1” == “all” ];then
/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang++ -arch i386 -arch x86_64 -pipe -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/iPhoneSimulator.platform/Developer/SDKs/iPhoneSimulator.sdk -D_FPDFAPI_MINI_ -D_FXCORE_FEATURE_ALL_ -D_FX_CHINESE_SYSTEM_ -D_FPDFEMB_GB_EXT_ -D_FPDFEMB_JAPAN_EXT_ -DDARWIN_NO_CARBON -D_JPX_DECODER_ -D_PNG_DECODER_ -D_GIF_DECODER_ -D_TIFF_DECODER_ -D_BMP_DECODER_ -D_JPEG_ENCODER_ -D_FX_FULL_ -D_FX_JBIG2_ENCODER_ -D_FX_BMP_ENCODER_ -D_FX_GIF_ENCODER_ -D_FX_JPX_ENCODER_ -D_FX_TIFF_ENCODER_ -D_FPDFAPI_ASYNC_PARSING_ -D_FXCORE_LIMITED_MEMORY_ -D_FSDK_OOM_ -D_FXGE_IMAGERENDER_SHORTCUT_ -D_FXMATH_IMAGE_CCITTRD_ -D_FPDFAPI_MT_   -D_FSDK_LICENSE_BUILD_ -DPNG_ARM_NEON_OPT=0 -fno-builtin -fexceptions -fstrict-aliasing -D_FSDK_GA_OOM_TEST_  -c -g fxcore_swig_wrap.mm fxcore_swig_proxy.mm 

ar -r libswig.a *.o
libtool -static -o libswig.a libswig.a libfxlib_ios_emu.a
#ranlib libswig.a
fi;