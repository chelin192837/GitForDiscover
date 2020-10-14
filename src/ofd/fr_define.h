#ifndef _FR_DEFINE_H_
#define _FR_DEFINE_H_
#define AUTO_RELEASE_PTR(type, var, lamda) std::unique_ptr<##type, ptr_releaser> ##var(##lamda)
#endif