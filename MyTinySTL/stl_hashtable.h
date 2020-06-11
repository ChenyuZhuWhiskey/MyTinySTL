/* Created by Chenyu ZHU on 2020/06/09 */

#ifndef MY_STL_STL_HASHTABLE_H
#define MY_STL_STL_HASHTABLE_H

#include "stl_algobase.h"
#include "stl_alloc.h"
#include "stl_construct.h"
#include "stl_config.h"
#include "stl_algo.h"
#include "stl_uninitialized.h"
#include "stl_function.h"
#include "stl_vector.h"
#include "stl_hash_fun.h"
#include "stl_tempbuf.h"

__STL_BEGIN_NAMESPACE

template<class _Val>
struct _Hashtable_node{
    _Hashtable_node* _M_next;
    _Val _M_val;
};

template<class _Val, class _Key, class _HashFcn, class _ExtractKey, class _Equalkey, class _Alloc = alloc>
class hashtable;

template<class _Val, class _Key, class _HashFcn, class _ExtractKey, class _Equalkey, class _Alloc = alloc>
struct _Hashtable_iterator;

template<class _Val, class _Key, class _HashFcn, class _ExtractKey, class _Equalkey, class _Alloc = alloc>
class _Hashtable_const_iterator;

template<class _Val, class _Key, class _HashFcn, class _ExtractKey, class _Equalkey, class _Alloc = alloc>
struct _Hashtable_iterator{
    typedef hashtable<_Val, _Key, _HashFcn, _ExtractKey, _Equalkey, _Alloc> _Hashtable;
    typedef _Hashtable_iterator<_Val, _Key, _HashFcn, _ExtractKey,_Equalkey, _Alloc> iterator;
    typedef _Hashtable_const_iterator<_Val, _Key, _HashFcn, _ExtractKey,_Equalkey, _Alloc> const_iterator;
    typedef _Hashtable_node<_Val> _Node;

    typedef forward_iterator_tag iterator_category;
    typedef _Val value_type;
    typedef ptrdiff_t difference_type;
    typedef size_t size_type;
    typedef _Val& reference;
    typedef _Val* pointer;

    _Node* _M_cur;
    _Hashtable* _M_ht;

    _Hashtable_iterator(_Node* __n, _Hashtable* __tab)
    : _M_cur(__n), _M_ht(__tab) {}

    _Hashtable_iterator() {}
    reference operator*() const { return _M_cur -> _M_val ; }
    pointer operator->() const { return &(operator*());}
    iterator& operator++();
    iterator operator++(int);

    bool operator==(const iterator& __it) const {
        return _M_cur = __it._M_cur;
    }

    bool operator!=(const iterator& __it) const {
        return _M_cur != __it._M_cur;
    }
};

template<class _Val, class _Key, class _HashFcn, class _ExtractKey,class _Equalkey, class _Alloc = alloc>
struct _Hashtable_const_iterator{
    typedef hashtable<_Val, _Key, _HashFcn, _ExtractKey,_Equalkey, _Alloc> _Hashtable;
    typedef _Hashtable_iterator<_Val, _Key, _HashFcn, _ExtractKey,_Equalkey, _Alloc> iterator;
    typedef _Hashtable_const_iterator<_Val, _Key, _HashFcn, _ExtractKey,_Equalkey, _Alloc> const_iterator;
    typedef _Hashtable_node<_Val> _Node;

    typedef forward_iterator_tag iterator_category;
    typedef _Val value_type;
    typedef ptrdiff_t difference_type;
    typedef size_t size_type;
    typedef const _Val& reference;
    typedef const _Val* pointer;   

    const _Node* _M_cur;
    const _Hashtable* _M_ht;

    _Hashtable_const_iterator(const _Node* __n, const _Hashtable* __tab)
    : _M_cur(__n), _M_ht(__tab) {}

    _Hashtable_const_iterator() {}
    reference operator*() const { return _M_cur -> _M_val ; }
    pointer operator->() const { return &(operator*());}

    const_iterator& operator++();
    const_iterator operator++(int);


    bool operator==(const iterator& __it) const {
        return _M_cur = __it._M_cur;
    }

    bool operator!=(const iterator& __it) const {
        return _M_cur != __it._M_cur;
    }

};

//assume long is at least 32 bits.
enum {__stl_num_primes = 28};

static const unsigned long __stl_prime_list[__stl_num_primes] = {
    53ul,         97ul,         193ul,       389ul,       769ul,
    1543ul,       3079ul,       6151ul,      12289ul,     24593ul,
    49157ul,      98317ul,      196613ul,    393241ul,    786433ul,
    1572869ul,    3145739ul,    6291469ul,   12582917ul,  25165843ul,
    50331653ul,   100663319ul,  201326611ul, 402653189ul, 805306457ul, 
    1610612741ul, 3221225473ul, 4294967291ul 
};


inline unsigned long __stl_next_prime(unsigned long __n){
    const unsigned long* __first = __stl_prime_list;
    const unsigned long* __last = __stl_prime_list + (int) __stl_num_primes;
    const unsigned long* __pos = lower_bound(__first, __last, __n);
    return __pos == __last ? *(__last - 1) : *__pos;
}

// template<class _Val, class _Key, class _HF, class _Ex, class _Eq, class _All>
// class hashtable;

template<class _Val, class _Key, class _HashFcn, class _ExtractKey, class _Equalkey, class _Alloc = alloc>
bool operator==(const hashtable<_Val, _Key, _HashFcn, _ExtractKey,_Equalkey, _Alloc>& __ht1,
                const hashtable<_Val, _Key, _HashFcn, _ExtractKey,_Equalkey, _Alloc>& __ht2 );

template<class _Val, class _Key, class _HashFcn, class _ExtractKey, class _Equalkey, class _Alloc = alloc>
class hashtable{
public:
    typedef _Key key_type;
    typedef _Val value_type;
    typedef _HashFcn hasher;
    typedef _Equalkey key_equal;

    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef value_type& reference;
    typedef const value_type& const_reference;

    hasher hash_funct() const {return _M_hash;}
    key_equal key_eq() const {return _M_equals;}

private:
    typedef _Hashtable_node<_Val> _Node;

public:
    typedef _Alloc allocator_type;
    allocator_type get_allocator() const {return allocator_type(); }
private:
    typedef simple_alloc<_Node, _Alloc> _M_node_allocator_type;
    _Node* _M_get_node() {return _M_node_allocator_type::allocate(1); }
    void _M_put_node(_Node* __p) {_M_node_allocator_type::deallocate(__p); }
    #define __HASH_ALLOC_INIT(__a)

private:
    hasher _M_hash;
    key_equal _M_equals;
    _ExtractKey _M_get_key;
    vector<_Node*, _Alloc> _M_buckets;
    size_type _M_num_elements;

public:
    typedef _Hashtable_iterator<_Val, _Key, _HashFcn, _ExtractKey,_Equalkey, _Alloc> iterator;
    typedef _Hashtable_const_iterator<_Val, _Key, _HashFcn, _ExtractKey,_Equalkey, _Alloc> const_iterator;

    friend struct _Hashtable_iterator<_Val, _Key, _HashFcn, _ExtractKey,_Equalkey, _Alloc>;
    friend struct _Hashtable_const_iterator<_Val, _Key, _HashFcn, _ExtractKey,_Equalkey, _Alloc>;
    
public:
    hashtable(size_type __n,
              const _HashFcn& __hf,
              const _Equalkey& __eql,
              const _ExtractKey& __ext,
              const allocator_type& __a = allocator_type())
    : __HASH_ALLOC_INIT(__a)
    _M_hash(__hf),
    _M_equals(__eql),
    _M_get_key(__ext),
    _M_buckets(__a),
    _M_num_elements(0)
    {
        _M_initialize_buckets(__n);
    }

    hashtable(size_type __n,
              const _HashFcn& __hf,
              const _Equalkey& __eql,
              const allocator_type& __a = allocator_type())
    :__HASH_ALLOC_INIT(__a)
    _M_hash(__hf),
    _M_equals(__eql),
    _M_get_key(_ExtractKey()),
    _M_buckets(__a),
    _M_num_elements(0)
    {
        _M_initialize_buckets(__n);
    }

    hashtable(const hashtable& __ht)
    : __HASH_ALLOC_INIT(__ht.get_allocator())
    _M_hash(__ht._M_hash),
    _M_equals(__ht._M_equals),
    _M_get_key(__ht._M_get_key),
    _M_buckets(__ht.get_allocator()),
    _M_num_elements(0)
    {
        _M_copy_from(__ht);
    }

    #undef __HASH_ALLOC_INIT

    hashtable& operator=(const hashtable& __ht){
        if(&__ht != this){
            clear();
            _M_hash = __ht._M_hash;
            _M_equals = __ht._M_equals;
            _M_get_key = __ht._M_get_key;
            _M_copy_from(__ht);
        }
        return *this;
    }

    ~hashtable() {clear();}

    size_type size() const {return _M_num_elements;}
    size_type max_size() const {return size_type(-1);}
    bool empty() const {return size() == 0;}

    void swap(hashtable& __ht){
        std::swap(_M_hash,__ht._M_hash);
        std::swap(_M_equals,__ht._M_equals);
        std::swap(_M_get_key,__ht._M_get_key);
        _M_buckets.swap(__ht._M_buckets);
        std::swap(_M_num_elements, __ht._M_num_elements);
    }

    iterator begin(){
        for(size_type __n = 0; __n < _M_buckets.size(); ++__n){
            if(_M_buckets[__n]){
                return iterator(_M_buckets[__n], this);
            }
        }
        return end();
    }

    iterator end() {return iterator(nullptr,this);}

    const_iterator begin() const {
        for(size_type __n = 0; __n < _M_buckets.size(); ++__n){
            if(_M_buckets[__n]){
                return const_iterator(_M_buckets[__n], this);
            }            
        }
        return end();
    }

    const_iterator end() const {return const_iterator(nullptr,this);}


    friend bool operator==(const hashtable&, const hashtable&);

public:
    size_type bucket_count() const {return _M_buckets.size();}
    size_type max_bucket_count() const {
        return __stl_prime_list[(int)__stl_num_primes - 1];
    }

    size_type elems_in_bucket(size_type __bucket) const{
        size_type __result = 0;
        for(_Node* __cur = _M_buckets[__bucket]; __cur; __cur = __cur ->_M_next){
            __result += 1;
        }
        return __result;
    }

    pair<iterator,bool> insert_unique(const value_type& __obj){
        resize(_M_num_elements + 1);
        return insert_unique_noresize(__obj);
    }

    iterator insert_equal(const value_type& __obj){
        resize(_M_num_elements + 1);
        return insert_equal_noresize(__obj);
    }

    pair<iterator, bool> insert_unique_noresize(const value_type& __obj);
    iterator insert_equal_noresize(const value_type& __obj);


    void insert_unique(const value_type* __f, const value_type* __l){
        size_type __n = __l - __f;
        resize(_M_num_elements + __n);
        for(; __n > 0; --__n, ++__f){
            insert_unique_noresize(*__f);
        }
    }

    void insert_equal(const_iterator __f, const_iterator __l){
        size_type __n = 0;
        distance(__f, __l, __n);
        resize(_M_num_elements + __n);
        for(;__n>0;--__n,++__f){
            insert_equal_noresize(*__f);
        }
        
    }

    reference find_or_insert(const value_type& __obj);

    iterator find(const key_type& __key){
        size_type __n = _M_bkt_num_key(__key);
        _Node* __first;
        for(__first = _M_buckets[__n];
            __first && !_M_equals(_M_get_key(__first -> _M_val), __key);
            __first = __first -> _M_next)
        {}
        return iterator(__first, this);
    }

    const_iterator find(const key_type& __key) const{
        size_type __n = _M_bkt_num_key(__key);
        _Node* __first;
        for(__first = _M_buckets[__n];
            __first && !_M_equals(_M_get_key(__first -> _M_val), __key);
            __first = __first -> _M_next)
        {}
        return const_iterator(__first, this);
    }

    size_type count(const key_type& __key) const{
        const size_type __n = _M_bkt_num_key(__key);
        size_type __result = 0;

        for(const _Node* __cur = _M_buckets[__n]; __cur; __cur = __cur->_M_next){
            if(_M_equals(_M_get_key(__cur->_M_val),__key)){
                ++__result;
            }
            return __result;
        }
    }

    pair<iterator, iterator>
    equal_range(const key_type& __key);

    pair<const_iterator, const_iterator>
    equal_range(const key_type& __key) const;

    size_type erase(const key_type& __key);
    void erase(const iterator& __it);
    void erase(iterator __first, iterator __last);
    void erase(const const_iterator& __it);
    void erase(const_iterator __first, const_iterator __last);

    void resize(size_type __num_elements_hint);
    void clear();

private:
    size_type _M_next_size(size_type __n) const{
        return __stl_next_prime(__n);
    }

    void _M_initialize_buckets(size_type __n){
        const size_type __n_buckets = _M_next_size(__n);
        _M_buckets.reserve(__n_buckets);
        _M_buckets.insert(_M_buckets.end(), __n_buckets, (_Node*)nullptr);
        _M_num_elements = 0;
    }

    size_type _M_bkt_num_key(const key_type& __key) const{
        return _M_bkt_num_key(__key, _M_buckets.size());
    }

    size_type _M_bkt_num(const value_type& __obj) const{
        return _M_bkt_num_key(_M_get_key(__obj));
    }

    size_type _M_bkt_num_key(const key_type& __key, size_t __n) const{
        return _M_hash(__key) % __n;
    }

    size_type _M_bkt_num(const value_type& __obj, size_t __n) const{
        return _M_bkt_num_key(_M_get_key(__obj), __n);
    }

    _Node* _M_new_node(const value_type& __obj){
        _Node* __n = _M_get_node();
        __n ->_M_next = nullptr;
        try{
            construct(&__n->_M_val, __obj);
            return __n;
        }catch(...){
            _M_put_node(__n);
            throw;
        }
    }

    void _M_delete_node(_Node* __n){
        destroy(&__n->_M_val);
        _M_put_node(__n);
    }

    void _M_erase_bucket(const size_type __n, _Node* __first, _Node* __last);
    void _M_erase_bucket(const size_type __n, _Node* __last);

    void _M_copy_from(const hashtable& __ht);
};

template<class _Val, class _Key, class _HF, class _ExK, class _Eqk, class _All>
_Hashtable_iterator<_Val, _Key, _HF, _ExK, _Eqk, _All>&
_Hashtable_iterator<_Val, _Key, _HF, _ExK, _Eqk, _All>::operator++(){
    const _Node* __old = _M_cur;
    _M_cur = _M_cur -> _M_next;
    if(!_M_cur){
        size_type __bucket = _M_ht -> _M_bkt_num(__old -> _M_val);
        while(!_M_cur && ++__bucket < _M_ht -> _M_buckets.size()){
            _M_cur = _M_ht -> _M_buckets[__bucket];
        }
    }
    return *this;
}


template<class _Val, class _Key, class _HF, class _ExK, class _Eqk, class _All>
inline _Hashtable_iterator<_Val, _Key, _HF, _ExK, _Eqk, _All>
_Hashtable_iterator<_Val, _Key, _HF, _ExK, _Eqk, _All>::operator++(int){
iterator __tmp = *this;
  ++*this;
  return __tmp;
}

template<class _Val, class _Key, class _HF, class _ExK, class _Eqk, class _All>
_Hashtable_const_iterator<_Val, _Key, _HF, _ExK, _Eqk, _All>&
_Hashtable_const_iterator<_Val, _Key, _HF, _ExK, _Eqk, _All>::operator++(){
    const _Node* __old = _M_cur;
    _M_cur = _M_cur -> _M_next;
    if(!_M_cur){
        size_type __bucket = _M_ht -> _M_bkt_num(__old -> _M_val);
        while(!_M_cur && ++__bucket < _M_ht -> _M_buckets.size()){
            _M_cur = _M_ht -> _M_buckets[__bucket];
        }
    }
    return *this;
}

template<class _Val, class _Key, class _HF, class _ExK, class _Eqk, class _All>
inline _Hashtable_const_iterator<_Val, _Key, _HF, _ExK, _Eqk, _All>
_Hashtable_const_iterator<_Val, _Key, _HF, _ExK, _Eqk, _All>::operator++(int){
    const_iterator __tmp = *this;
    ++*this;
    return __tmp;
}

template <class _Val, class _Key, class _HF, class _ExK, class _EqK, 
          class _All>
inline forward_iterator_tag
iterator_category(const _Hashtable_iterator<_Val,_Key,_HF,_ExK,_EqK,_All>&)
{
    return forward_iterator_tag();
}

template <class _Val, class _Key, class _HF, class _ExK, class _EqK, 
          class _All>
inline _Val* 
value_type(const _Hashtable_iterator<_Val,_Key,_HF,_ExK,_EqK,_All>&)
{
    return (_Val*) nullptr;
}

template <class _Val, class _Key, class _HF, class _ExK, class _EqK, 
          class _All>
inline typename hashtable<_Val,_Key,_HF,_ExK,_EqK,_All>::difference_type*
distance_type(const _Hashtable_iterator<_Val,_Key,_HF,_ExK,_EqK,_All>&)
{
    return (typename hashtable<_Val,_Key,_HF,_ExK,_EqK,_All>::difference_type*) nullptr;
}

template <class _Val, class _Key, class _HF, class _ExK, class _EqK, 
          class _All>
inline forward_iterator_tag
iterator_category(const _Hashtable_const_iterator<_Val,_Key,_HF,
                                                  _ExK,_EqK,_All>&)
{
    return forward_iterator_tag();
}

template <class _Val, class _Key, class _HF, class _ExK, class _EqK, 
          class _All>
inline _Val* 
value_type(const _Hashtable_const_iterator<_Val,_Key,_HF,_ExK,_EqK,_All>&)
{
    return (_Val*) nullptr;
}

template <class _Val, class _Key, class _HF, class _ExK, class _EqK, 
          class _All>
inline typename hashtable<_Val,_Key,_HF,_ExK,_EqK,_All>::difference_type*
distance_type(const _Hashtable_const_iterator<_Val,_Key,_HF,_ExK,_EqK,_All>&)
{
    return (typename hashtable<_Val,_Key,_HF,_ExK,_EqK,_All>::difference_type*) nullptr;
}

template <class _Val, class _Key, class _HF, class _ExK, class _EqK, 
          class _All>
bool operator==(const hashtable<_Val,_Key,_HF,_ExK,_EqK,_All>& __ht1,
                const hashtable<_Val,_Key,_HF,_ExK,_EqK,_All>& __ht2){
    typedef typename hashtable<_Val,_Key,_HF,_ExK,_EqK,_All>::_Node _Node;
    if(__ht1._M_buckets.size() != __ht2._M_buckets.size()){
        return false;
    }
    for(int __n = 0; __n < __ht1._M_buckets.size(); ++__n){
        _Node* __cur1 = __ht1._M_buckets[__n];
        _Node* __cur2 = __ht2._M_buckets[__n];
        for(; __cur1 && __cur2 && __cur1 -> _M_val == __cur2 -> _M_val;
            __cur1 = __cur1 -> _M_next, __cur2 = __cur2 -> _M_next)
            {}
        if(__cur1 || __cur2){
            return false;
        }

    }
    return true;
}

template <class _Val, class _Key, class _HF, class _Ex, class _Eq, class _All>
pair<typename hashtable<_Val,_Key,_HF,_Ex,_Eq,_All>::iterator, bool> 
hashtable<_Val,_Key,_HF,_Ex,_Eq,_All>
::insert_unique_noresize(const value_type& __obj){
    const size_type __n = _M_bkt_num(__obj);
    _Node* __first = _M_buckets[__n];

    for(_Node* __cur = __first; __cur; __cur = __cur->_M_next){
        if(_M_equals(_M_get_key(__cur->_M_val), _M_get_key(__obj))){
            return pair<iterator,bool> (iterator(__cur, this), false);
        }
    }
    _Node* __tmp = _M_new_node(__obj);
    __tmp -> _M_next = __first;
    _M_buckets[__n] = __tmp;
    ++_M_num_elements;
    return pair<iterator, bool>(iterator(__tmp, this), true);
}

template <class _Val, class _Key, class _HF, class _Ex, class _Eq, class _All>
typename hashtable<_Val,_Key,_HF,_Ex,_Eq,_All>::iterator 
hashtable<_Val,_Key,_HF,_Ex,_Eq,_All>
::insert_equal_noresize(const value_type& __obj){
    const size_type  __n = _M_bkt_num(__obj);
    _Node* __first = _M_buckets[__n];

    for(_Node* __cur = __first; __cur; __cur = __cur -> _M_next){
        if(_M_equals(_M_get_key(__cur->_M_val), _M_get_key(__obj))){
            _Node* __tmp = _M_new_node(__obj);
            __tmp->_M_next = __cur->_M_next;
            __cur->_M_next = __cur ->_M_next;
            ++_M_num_elements;
            return iterator(__tmp, this);
        }
    }
    _Node* __tmp = _M_new_node(__obj);
    __tmp->_M_next = __first;
    _M_buckets[__n] = __tmp;
    ++_M_num_elements;
    return iterator(__tmp, this);

}

template <class _Val, class _Key, class _HF, class _Ex, class _Eq, class _All>
typename hashtable<_Val,_Key,_HF,_Ex,_Eq,_All>::reference 
hashtable<_Val,_Key,_HF,_Ex,_Eq,_All>::find_or_insert(const value_type& __obj){
    resize(_M_num_elements + 1);

    size_type __n = _M_bkt_num(__obj);
    _Node* __first = _M_buckets[__n];

    for(_Node* __cur = __first; __cur; __cur = __cur -> _M_next){
        if(_M_equals(_M_get_key(__cur->_M_val), _M_get_key(__obj))){
            return __cur -> _M_val;
        }
    }

    _Node* __tmp = _M_new_node(__obj);
    __tmp->_M_next = __first;
    _M_buckets[__n] = __tmp;
    ++_M_num_elements;
    return __tmp->_M_val;
}

template <class _Val, class _Key, class _HF, class _Ex, class _Eq, class _All>
pair<typename hashtable<_Val,_Key,_HF,_Ex,_Eq,_All>::iterator,
     typename hashtable<_Val,_Key,_HF,_Ex,_Eq,_All>::iterator> 
hashtable<_Val,_Key,_HF,_Ex,_Eq,_All>::equal_range(const key_type& __key){
    typedef pair<iterator,iterator> _Pii;
    const size_type __n = _M_bkt_num_key(__key);
    for(_Node* __first = _M_buckets[__n]; __first; __first = __first -> _M_next){
        if(_M_equals(_M_get_key(__first->_M_val),__key)){
            for(_Node* __cur = __first ->_M_next; __cur; __cur = __cur->_M_next){
                if(!_M_equals(_M_get_key(__cur->_M_val),__key)){
                    return _Pii(iterator(__first, this), iterator(__cur,this));
                }
            }
            for(size_type __m = __n+1; __m < _M_buckets.size(); ++__m){
                if(_M_buckets[__m]){
                    return _Pii(iterator(__first, this), iterator(_M_buckets[__m], this));
                }
            }
            return _Pii(iterator(__first, this), end());
        }
    }
    return _Pii(end(),end());

}

template <class _Val, class _Key, class _HF, class _Ex, class _Eq, class _All>
pair<typename hashtable<_Val,_Key,_HF,_Ex,_Eq,_All>::const_iterator, 
     typename hashtable<_Val,_Key,_HF,_Ex,_Eq,_All>::const_iterator> 
hashtable<_Val,_Key,_HF,_Ex,_Eq,_All>
  ::equal_range(const key_type& __key) const{
    typedef pair<const_iterator,const_iterator> _Pii;
    const size_type __n = _M_bkt_num_key(__key);
    for(_Node* __first = _M_buckets[__n]; __first; __first = __first -> _M_next){
        if(_M_equals(_M_get_key(__first->_M_val),__key)){
            for(_Node* __cur = __first ->_M_next; __cur; __cur = __cur->_M_next){
                if(!_M_equals(_M_get_key(__cur->_M_val),__key)){
                    return _Pii(iterator(__first, this), iterator(__cur,this));
                }
            }
            for(size_type __m = __n+1; __m < _M_buckets.size(); ++__m){
                if(_M_buckets[__m]){
                    return _Pii(iterator(__first, this), iterator(_M_buckets[__m], this));
                }
            }
            return _Pii(iterator(__first, this), end());
        }
    }
    return _Pii(end(),end());
}

template <class _Val, class _Key, class _HF, class _Ex, class _Eq, class _All>
typename hashtable<_Val,_Key,_HF,_Ex,_Eq,_All>::size_type 
hashtable<_Val,_Key,_HF,_Ex,_Eq,_All>::erase(const key_type& __key){
    const size_type __n = _M_bkt_num_key(__key);
    _Node* __first = _M_buckets[__n];
    size_type __earsed = 0;

    if(__first){
        _Node* __cur = __first;
        _Node* __next = __cur->_M_next;
        while(__next){
            if(_M_equals(_M_get_key(__next->_M_val), __key)){
                __cur -> _M_next = __next->_M_next;
                _M_delete_node(__next);
                __next = __cur->_M_next;
                ++__earsed;
                --_M_num_elements;
            }else{
                __cur = __next;
                __next = __cur ->_M_next;
            }
        }
        if(_M_equals(_M_get_key(__first->_M_val), __key)){
            _M_buckets[__n] = __first ->_M_next;
            _M_delete_node(__first);
            ++__earsed;
            --_M_num_elements;
        }
    }
    return __earsed;

}

template <class _Val, class _Key, class _HF, class _Ex, class _Eq, class _All>
void hashtable<_Val,_Key,_HF,_Ex,_Eq,_All>::erase(const iterator& __it){
    _Node* __p = __it._M_cur;
    if(__p){
        const size_type __n = _M_bkt_num(__p->_M_val);
        _Node* __cur = _M_buckets[__n];
        if(__cur == __p){
            _M_buckets[__n] = __cur -> _M_next;
            _M_delete_node(__cur);
            --_M_num_elements;
        }else{
            _Node* __next = __cur -> _M_next;
            while(__next){
                if(__next == __p){
                    __cur->_M_next = __next ->_M_next;
                    _M_delete_node(__next);
                    --_M_num_elements;
                    break;
                }else{
                    __cur = __cur->_M_next;
                    __next = __cur->_M_next;
                }
            }
        }
    }
}

template <class _Val, class _Key, class _HF, class _Ex, class _Eq, class _All>
void hashtable<_Val,_Key,_HF,_Ex,_Eq,_All>
::erase(iterator __first, iterator __last){
    size_type __f_bucket = __first._M_cur ?
        _M_bkt_num(__first._M_cur->_M_val) : _M_buckets.size();
    size_type __l_bucket = __last._M_cur?
        _M_bkt_num(__last._M_cur->_M_val) : _M_buckets.size();
    
    if(__first._M_cur == __last._M_cur){
        return;
    }else if(__f_bucket == __l_bucket){
        _M_erase_bucket(__f_bucket, __first._M_cur, __last._M_cur);
    }else{
        _M_erase_bucket(__f_bucket, __first._M_cur, (_Node*)nullptr);
        for(size_type __n = __f_bucket + 1; __n < __l_bucket; ++__n){
            _M_erase_bucket(__n, (_Node*)nullptr);
        }
        if(__l_bucket != _M_buckets.size()){
            _M_erase_bucket(__l_bucket, __last._M_cur);
        }
    }
}


template <class _Val, class _Key, class _HF, class _Ex, class _Eq, class _All>
inline void
hashtable<_Val,_Key,_HF,_Ex,_Eq,_All>
::erase(const_iterator __first,const_iterator __last){
    erase(iterator(const_cast<_Node*>(__first._M_cur),const_cast<hashtable*>(__first._M_ht)),
          iterator(const_cast<_Node*>(__last._M_cur),const_cast<hashtable*>(__last._M_ht)));
}


template <class _Val, class _Key, class _HF, class _Ex, class _Eq, class _All>
inline void
hashtable<_Val,_Key,_HF,_Ex,_Eq,_All>::erase(const const_iterator& __it){
    erase(iterator(const_cast<_Node*>(__it._M_cur), const_cast<hashtable*>(__it._M_ht)));
}


template <class _Val, class _Key, class _HF, class _Ex, class _Eq, class _All>
void hashtable<_Val,_Key,_HF,_Ex,_Eq,_All>
::resize(size_type __num_elements_hint){
    const size_type __old_n = _M_buckets.size();
    if(__num_elements_hint > __old_n){
        const size_type __n = _M_next_size(__num_elements_hint);
        if(__n > __old_n){
            vector<_Node*, _All> __tmp(__n, (_Node*)nullptr, _M_buckets.get_allocator());
            try{
                for(size_type __bucket = 0; __bucket < __old_n; ++ __bucket){
                    _Node* __first = _M_buckets[__bucket];
                    while(__first){
                        size_type __new_bucket = _M_bkt_num(__first -> _M_val, __n);
                        _M_buckets[__bucket] = __first ->_M_next;
                        __first ->_M_next = __tmp[__new_bucket];
                        __tmp[__new_bucket] = __first;
                        __first = _M_buckets[__bucket];
                    }
                }
                _M_buckets.swap(__tmp);
            }catch(...){
                for(size_type __bucket = 0; __bucket < __tmp.size(); ++__bucket){
                    while(__tmp[__bucket]){
                        _Node* __next = __tmp[__bucket]->_M_next;
                        _M_delete_node(__tmp[__bucket]);
                        __tmp[__bucket] = __next;
                    }
                }
                throw;
            }
        }
    }
}

template <class _Val, class _Key, class _HF, class _Ex, class _Eq, class _All>
void hashtable<_Val,_Key,_HF,_Ex,_Eq,_All>
::_M_erase_bucket(const size_type __n, _Node* __first, _Node* __last){
    _Node* __cur = _M_buckets[__n];
    if(__cur == __first){
        _M_erase_bucket(__n, __last);
    }else{
        _Node* __next;
        for(__next = __cur->_M_next; __next != __first; __cur = __next, __next = __cur ->_M_next)
        {}
        while(__next != __last){
            __cur -> _M_next = __next ->_M_next;
            _M_delete_node(__next);
            __next = __cur->_M_next;
            --_M_num_elements;
        }
    }

}

template <class _Val, class _Key, class _HF, class _Ex, class _Eq, class _All>
void hashtable<_Val,_Key,_HF,_Ex,_Eq,_All>
::_M_erase_bucket(const size_type __n, _Node* __last){
    _Node* __cur = _M_buckets[__n];
    while(__cur != __last){
        _Node* __next = __cur ->_M_next;
        _M_delete_node(__cur);
        __cur = __next;
        _M_buckets[__n] = __cur;
        --_M_num_elements;
    }

}

template <class _Val, class _Key, class _HF, class _Ex, class _Eq, class _All>
void hashtable<_Val,_Key,_HF,_Ex,_Eq,_All>::clear()
{
    for(size_type __i = 0; __i < _M_buckets.size(); ++__i){
        _Node* __cur = _M_buckets[__i];
        while (__cur != nullptr) {
            _Node* __next = __cur ->_M_next;
            _M_delete_node(__cur);
            __cur = __next;
        }
        _M_buckets[__i] = nullptr;
    }
    _M_num_elements = 0;
}

template <class _Val, class _Key, class _HF, class _Ex, class _Eq, class _All>
void hashtable<_Val,_Key,_HF,_Ex,_Eq,_All>
  ::_M_copy_from(const hashtable& __ht)
{
    _M_buckets.clear();
    _M_buckets.reserve(__ht._M_buckets.size());
    _M_buckets.insert(_M_buckets.end(), __ht._M_buckets.size(), (_Node*) nullptr);
    try{
        for(size_type __i = 0; __i < __ht._M_buckets.size(); ++__i){
            const _Node* __cur = __ht._M_buckets[__i];
            if(__cur){
                _Node* __copy = _M_new_node(__cur -> _M_val);
                _M_buckets[__i] = __copy;
                for(_Node* __next = __cur ->_M_next;
                    __next; __cur = __next, __next = __cur->_M_next){
                    __copy->_M_next = _M_new_node(__next->_M_val);
                    __copy = __copy->_M_next;
                } 
            }
        }
        _M_num_elements = __ht._M_num_elements;
    }catch(...){
        clear();
        throw;
    }
}

__STL_END_NAMESPACE


#endif