
/* 
 * IMTYPE TYPEMAPS: Represents the ObjectiveC++ type corresponding to each C++ type. 
 */
/* 
 * IN TYPEMAPS: Used for "in" arguments type conversions: ObjectiveC++ to C++.
 */
/*
 * OUT TYPEMAPS: Used for return-type type conversions: C++ to ObjectiveC++
 */
/* 
 * OBJCTYPE TYPEMAPS: Represents the ObjectiveC(proxy) type corresponding to each ObjectiveC++ type in the intermediate(wrapper) layer. 
 */
/* 
 * OBJCIN TYPEMAPS: Used for "in" arguments type conversions from ObjectiveC(proxy) layer to ObjectiveC++(wrapper) layer.
 */
/*
 * OBJCOUT TYPEMAPS: Used for return-type type conversions: ObjectiveC++(wrapper) to ObjectiveC(proxy)
 */

//convert (void* buffer) to NSData*
%typemap(objctype) (void* buffer) "NSData *"
%typemap(objcin) (void* buffer) "$objcinput"
%typemap(imtype) (void* buffer) "NSData *"
%typemap(in) (void* buffer){
    $1 = 0;
    if ($input) {
        $1 = [$input bytes];
    }
}

//convert (unsigned char*) to NSData*
//last objc type
%typemap(objctype) (unsigned char* getBuffer) "NSData *"
%typemap(imtype) (unsigned char* getBuffer) "NSData *"
%typemap(out) (unsigned char* getBuffer) {
    if ($1) {
        $result = [NSData dataWithBytes: $1 length:strlen($1)];
    }
}
