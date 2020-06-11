/* Created by Chenyu ZHU on 20200610 */

#ifndef MY_STL_STL_TEMPBUF_H
#define MY_STL_STL_TEMPBUF_H

#include "stl_config.h"
#include "stl_pair.h"
#include "type_traits.h"

__STL_BEGIN_NAMESPACE

template<class _Tp>
pair<_Tp*, ptrdiff_t>
__get_temporary_buffer(ptrdiff_t __len, _Tp*){
    if (__len > ptrdiff_t(INT_MAX/sizeof(_Tp))){
        __len = INT_MAX / sizeof(_Tp);
    }

    while (__len > 0){
        _Tp* __tmp = (_Tp*) malloc((size_t)__len * sizeof(_Tp));
        if(__tmp != nullptr){
            return pair<_Tp*, ptrdiff_t>(__tmp, __len);
        }
        __len /= 2;
    }

    return pair<_Tp*,ptrdiff_t>((_Tp*)nullptr, 0);
}


template<class _Tp>
inline pair<_Tp*, ptrdiff_t> get_temporary_buffer(ptrdiff_t __len, _Tp*){
    return __get_temporary_buffer(__len, (_Tp*)nullptr);
}

template<class _Tp>
void return_template_buffer(_Tp* __p){
    free(__p);
}

template<class _ForwardIter, class _Tp>
class _Temporary_buffer{
private:
    ptrdiff_t _M_original_len;
    ptrdiff_t _M_len;
    _Tp* _M_buffer;

    void _M_allocate_buffer(){
        _M_original_len = _M_len;
        _M_buffer = nullptr;

        if(_M_len > (ptrdiff_t)(INT_MAX/sizeof(_Tp))){
            _M_len = INT_MAX / sizeof(_Tp);
        }
        while (_M_len > 0){
            _M_buffer = (_Tp*) malloc (_M_len * sizeof (_Tp));
            if(_M_buffer){
                break;
            }
            _M_len /= 2;
        }
    }

    void _M_initialize_buffer(const _Tp&, __true_type){}
    void _M_initialize_buffer(const _Tp& __val, __false_type){
        uninitialized_fill(_M_buffer, _M_len, __val);
    }

public:
    ptrdiff_t size() const {return _M_len; }
    ptrdiff_t requested_size() const {return _M_original_len; }

    _Tp* begin() {return _M_buffer; }
    _Tp* end() {return _M_buffer + _M_len;}

    _Temporary_buffer(_ForwardIter __first, _ForwardIter __last){
        typedef typename __type_traits<_Tp>::has_trivial_default_constructor _Trivial;
        try{
            _M_len = 0;
            distance(__first, __last, _M_len);
            _M_allocate_buffer();
            if(_M_len > 0){
                _M_initialize_buffer(*__first, _Trivial());
            }
        }catch(...){
            free(_M_buffer);
            _M_buffer = 0;
            _M_len = 0;
            throw;
        }

    }

    ~_Temporary_buffer(){
        destroy(_M_buffer, _M_buffer + _M_len);
        free(_M_buffer);
    }

private:
    _Temporary_buffer(const _Temporary_buffer&){}
    void operator=(const _Temporary_buffer&) {}

};

template<class _ForwardIter, class _Tp>
struct temporary_buffer: public _Temporary_buffer<_ForwardIter, _Tp>{
    temporary_buffer(_ForwardIter __first, _ForwardIter __last)
    : _Temporary_buffer<_ForwardIter, _Tp>(__first, __last) {}

    ~temporary_buffer() {}
};

__STL_END_NAMESPACE


#endif