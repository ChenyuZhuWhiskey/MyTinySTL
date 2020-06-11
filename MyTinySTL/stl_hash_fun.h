/* Created By Chenyu ZHU on 2020/06/10 */

#ifndef MY_STL_STL_HASH_FUN_H
#define MY_STL_STL_HASH_FUN_H

#include <cstddef>
#include "stl_config.h"

__STL_BEGIN_NAMESPACE

inline size_t __stl_hash_string(const char* __s){
    unsigned long __h = 0;
    for(; *__s; ++__s){
        __h = 5*__h + *__s;
    }
    return size_t(__h);
    
}


__STL_END_NAMESPACE

#endif