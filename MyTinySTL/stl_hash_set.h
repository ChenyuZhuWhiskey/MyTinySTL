/* created by Chenyu Zhu  on 2020/06/12*/

#ifndef MY_STL_STL_HASH_SET_H
#define MY_STL_STL_HASH_SET_H

#include "stl_config.h"
#include "stl_pair.h"
#include "stl_hashtable.h"
#include "stl_function.h"

__STL_BEGIN_NAMESPACE

template <class _Value, class _HashFn, class _EqKey, class _Alloc = alloc>
class hash_set;

template <class _Value, class _HashFn, class _EqKey, class _Alloc>
inline bool operator==(const hash_set<_Value, _HashFn,_EqKey, _Alloc>& __x,
    const hash_set<_Value, _HashFn,_EqKey, _Alloc>& __y);


template <class _Value, class _HashFn, class _EqKey, class _Alloc>
class hash_set {

private:
    typedef hashtable<_Value,_Value, _HashFn,_Identity<_Value>, _EqKey, _Alloc> _Ht;
    _Ht _M_ht;
public:
    typedef typename _Ht::key_type key_type;
    typedef typename _Ht::value_type value_type;
    typedef typename _Ht::hasher hasher;
    typedef typename _Ht::key_euqal key_equal;

    typedef typename _Ht::size_type size_type;
    typedef typename _Ht::difference_type difference_type;
    typedef typename _Ht::const_pointer pointer;
    typedef typename _Ht::const_pointer const_pointer;
    typedef typename _Ht::const_reference reference;
    typedef typename _Ht::const_reference const_reference;
    
    typedef typename _Ht::const_iterator iterator;
    typedef typename _Ht::const_iterator const_iterator;

    typedef typename _Ht::allocator_type allocator_type;

    hasher hash_funct() const {return _M_ht.hash_funct();}
    key_equal key_eq() const {return _M_ht.key_eq();}
    allocator_type get_allocator() const {return allocator_type();}

public:

    hash_set() :_M_ht(100,hasher(),key_equal(), allocator_type()) {}
    explicit hash_set(size_type __n)
    :_M_ht(__n, hasher(), key_equal(), allocator_type()){}

    hash_set(size_type __n, const hasher& __hf)
    : _M_ht(__n, __hf, key_equal,allocator_type()){}

    hash_set(size_type __n, const hasher& __hf, const key_equal& __eql, const allocator_type& __a = allocator_type())
    : _M_ht(__n, __hf, __eql, __a){}

    hash_set(const value_type* __f, const value_type* __l)
    :_M_ht(100, hasher(), key_equal(), allocator_type()){
        _M_ht.insert_unique(__f,__l);
    }

    hash_set(const value_type* __f, const value_type* __l, size_type __n)
    :_M_ht(__n, hasher(), key_equal(), allocator_type()){
        _M_ht.insert_unique(__f,__l);
    }

    hash_set(const value_type* __f, const value_type* __l, size_type __n, const hasher& __hf)
    :_M_ht(__n, __hf, key_equal(), allocator_type())
    {_M_ht.insert_unique(__f,__l);}

    hash_set(const value_type* __f, const value_type* __l, size_type __n, const hasher& __hf, const key_equal& __eql)
    :_M_ht(__n, __hf, __eql, allocator_type())
    {_M_ht.insert_unique(__f,__l);}

    hash_set(const value_type* __f, const value_type* __l, size_type __n, const hasher& __hf, const key_equal& __eql, const allocator_type& __a = allocator_type())
    :_M_ht(__n, __hf, __eql, __a)
    {_M_ht.insert_unique(__f,__l);}    

    hash_set(const_iterator __f, const_iterator __l)
    :_M_ht(100, hasher(), key_equal(), allocator_type()){
        _M_ht.insert_unique(__f,__l);
    }

    hash_set(const_iterator __f, const_iterator __l, size_type __n)
    :_M_ht(__n, hasher(), key_equal(), allocator_type()){
        _M_ht.insert_unique(__f,__l);
    }

    hash_set(const_iterator __f, const_iterator __l, size_type __n, const hasher& __hf)
    :_M_ht(__n, __hf, key_equal(), allocator_type()){
        _M_ht.insert_unique(__f,__l);
    }

    hash_set(const_iterator __f, const_iterator __l, size_type __n, const hasher& __hf, const key_equal __eql)
    :_M_ht(__n, __hf, __eql, allocator_type()){
        _M_ht.insert_unique(__f,__l);
    }

    hash_set(const_iterator __f, const_iterator __l, size_type __n, const hasher& __hf, const key_equal __eql, const allocator_type& __a = allocator_type())
    :_M_ht(__n, __hf, __eql, __a){
        _M_ht.insert_unique(__f,__l);
    }


    hash_set(const hash_set& __x)
    : _M_ht(__x._M_ht){}

    hash_set<_Value,_HashFn,_EqKey,_Alloc>& 
    operator=(const hash_set& __x){
        _M_ht = __x._M_ht;
        return *this;
    }

public:
    size_type size() const { return _M_ht.size(); }
    size_type max_size() const { return _M_ht.max_size(); }
    bool empty() const { return _M_ht.empty(); }
    void swap(hash_set& __hs) { _M_ht.swap(__hs._M_ht); }   

    friend bool operator==(const hash_set&, const hash_set&);

    iterator begin() const {return _M_ht.begin();}
    iterator end() const { return _M_ht.end(); }

public:
    pair<iterator,bool> insert(const value_type& __x){
        return _M_ht.insert_unique(__x);
    }

    void insert(const_iterator __first, const_iterator __last){
        _M_ht.insert_unique(__first, __last);
    }

    void insert(const value_type* __first, const value_type* __last){
        _M_ht.insert_unique(__first, __last);
    }

    pair<iterator,bool> insert_noresize(const value_type& __obj)
    { return _M_ht.insert_unique_noresize(__obj); }  

    iterator find(const key_type& __x) {return _M_ht.find(__x);}


    size_type count (const key_type& __key) const {return _M_ht.count(__key);}

    pair<iterator,iterator> equal_range(const key_type& __key){
        return _M_ht.equal_range(__key);
    }


    void erase(const key_type& __key) {_M_ht.earse(__key);}
    size_type erase(const key_type& __x) {return _M_ht.erase(__x);}
    void erase(iterator __first, iterator __last){
        _M_ht.erase(__first, __last);
    }
    void clear() {_M_ht.clear();}

    void resize(size_type __hint) {_M_ht.resize(__hint);}

    size_type bucket_count() const {return _M_ht.bucket_count();}
    size_type max_bucket_count() const {return _M_ht.max_bucket_count();}
    size_type elems_in_bucket(size_type __n) const {
        return _M_ht.elems_in_bucket(__n);
    }



};

template <class _Value, class _HashFcn, class _EqlKey, class _Alloc>
inline bool 
operator==(const hash_set<_Value,_HashFcn,_EqlKey,_Alloc>& __hm1,
           const hash_set<_Value,_HashFcn,_EqlKey,_Alloc>& __hm2)
{
  return __hm1._M_ht == __hm2._M_ht;
}



//hash_multiset

template <class _Value, class _HashFn, class _EqKey, class _Alloc = alloc>
class hash_multiset;

template <class _Value, class _HashFn, class _EqKey, class _Alloc>
inline bool operator==(const hash_multiset<_Value, _HashFn,_EqKey, _Alloc>& __x,
    const hash_multiset<_Value, _HashFn,_EqKey, _Alloc>& __y);


template <class _Value, class _HashFn, class _EqKey, class _Alloc>
class hash_multiset {

private:
    typedef hashtable<_Value,_Value, _HashFn,_Identity<_Value>, _EqKey, _Alloc> _Ht;
    _Ht _M_ht;
public:
    typedef typename _Ht::key_type key_type;
    typedef typename _Ht::value_type value_type;
    typedef typename _Ht::hasher hasher;
    typedef typename _Ht::key_euqal key_equal;

    typedef typename _Ht::size_type size_type;
    typedef typename _Ht::difference_type difference_type;
    typedef typename _Ht::const_pointer pointer;
    typedef typename _Ht::const_pointer const_pointer;
    typedef typename _Ht::const_reference reference;
    typedef typename _Ht::const_reference const_reference;
    
    typedef typename _Ht::const_iterator iterator;
    typedef typename _Ht::const_iterator const_iterator;

    typedef typename _Ht::allocator_type allocator_type;

    hasher hash_funct() const {return _M_ht.hash_funct();}
    key_equal key_eq() const {return _M_ht.key_eq();}
    allocator_type get_allocator() const {return allocator_type();}

public:

    hash_multiset() :_M_ht(100,hasher(),key_equal(), allocator_type()) {}
    explicit hash_multiset(size_type __n)
    :_M_ht(__n, hasher(), key_equal(), allocator_type()){}

    hash_multiset(size_type __n, const hasher& __hf)
    : _M_ht(__n, __hf, key_equal,allocator_type()){}

    hash_multiset(size_type __n, const hasher& __hf, const key_equal& __eql, const allocator_type& __a = allocator_type())
    : _M_ht(__n, __hf, __eql, __a){}

    hash_multiset(const value_type* __f, const value_type* __l)
    :_M_ht(100, hasher(), key_equal(), allocator_type()){
        _M_ht.insert_equal(__f,__l);
    }

    hash_multiset(const value_type* __f, const value_type* __l, size_type __n)
    :_M_ht(__n, hasher(), key_equal(), allocator_type()){
        _M_ht.insert_equal(__f,__l);
    }

    hash_multiset(const value_type* __f, const value_type* __l, size_type __n, const hasher& __hf)
    :_M_ht(__n, __hf, key_equal(), allocator_type())
    {_M_ht.insert_equal(__f,__l);}

    hash_multiset(const value_type* __f, const value_type* __l, size_type __n, const hasher& __hf, const key_equal& __eql)
    :_M_ht(__n, __hf, __eql, allocator_type())
    {_M_ht.insert_equal(__f,__l);}

    hash_multiset(const value_type* __f, const value_type* __l, size_type __n, const hasher& __hf, const key_equal& __eql, const allocator_type& __a = allocator_type())
    :_M_ht(__n, __hf, __eql, __a)
    {_M_ht.insert_equal(__f,__l);}    

    hash_multiset(const_iterator __f, const_iterator __l)
    :_M_ht(100, hasher(), key_equal(), allocator_type()){
        _M_ht.insert_equal(__f,__l);
    }

    hash_multiset(const_iterator __f, const_iterator __l, size_type __n)
    :_M_ht(__n, hasher(), key_equal(), allocator_type()){
        _M_ht.insert_equal(__f,__l);
    }

    hash_multiset(const_iterator __f, const_iterator __l, size_type __n, const hasher& __hf)
    :_M_ht(__n, __hf, key_equal(), allocator_type()){
        _M_ht.insert_equal(__f,__l);
    }

    hash_multiset(const_iterator __f, const_iterator __l, size_type __n, const hasher& __hf, const key_equal __eql)
    :_M_ht(__n, __hf, __eql, allocator_type()){
        _M_ht.insert_equal(__f,__l);
    }

    hash_multiset(const_iterator __f, const_iterator __l, size_type __n, const hasher& __hf, const key_equal __eql, const allocator_type& __a = allocator_type())
    :_M_ht(__n, __hf, __eql, __a){
        _M_ht.insert_equal(__f,__l);
    }

    hash_multiset(const hash_multiset& __x)
    : _M_ht(__x._M_ht){}

    hash_multiset<_Value,_HashFn,_EqKey,_Alloc>& 
    operator=(const hash_multiset& __x){
        _M_ht = __x._M_ht;
        return *this;
    }


public:
    size_type size() const { return _M_ht.size(); }
    size_type max_size() const { return _M_ht.max_size(); }
    bool empty() const { return _M_ht.empty(); }
    void swap(hash_multiset& __hs) { _M_ht.swap(__hs._M_ht); }   

    friend bool operator==(const hash_multiset&, const hash_multiset&);

    iterator begin() const {return _M_ht.begin();}
    iterator end() const { return _M_ht.end(); }

public:
    pair<iterator,bool> insert(const value_type& __x){
        return _M_ht.insert_equal(__x);
    }

    void insert(const_iterator __first, const_iterator __last){
        _M_ht.insert_equal(__first, __last);
    }

    void insert(const value_type* __first, const value_type* __last){
        _M_ht.insert_equal(__first, __last);
    }

    pair<iterator,bool> insert_noresize(const value_type& __obj)
    { return _M_ht.insert_equal_noresize(__obj); }  

    iterator find(const key_type& __x) {return _M_ht.find(__x);}


    size_type count (const key_type& __key) const {return _M_ht.count(__key);}

    pair<iterator,iterator> equal_range(const key_type& __key){
        return _M_ht.equal_range(__key);
    }


    void erase(const key_type& __key) {_M_ht.earse(__key);}
    size_type erase(const key_type& __x) {return _M_ht.erase(__x);}
    void erase(iterator __first, iterator __last){
        _M_ht.erase(__first, __last);
    }
    void clear() {_M_ht.clear();}

    void resize(size_type __hint) {_M_ht.resize(__hint);}

    size_type bucket_count() const {return _M_ht.bucket_count();}
    size_type max_bucket_count() const {return _M_ht.max_bucket_count();}
    size_type elems_in_bucket(size_type __n) const {
        return _M_ht.elems_in_bucket(__n);
    }



};

template <class _Value, class _HashFcn, class _EqlKey, class _Alloc>
inline bool 
operator==(const hash_multiset<_Value,_HashFcn,_EqlKey,_Alloc>& __hm1,
           const hash_multiset<_Value,_HashFcn,_EqlKey,_Alloc>& __hm2)
{
  return __hm1._M_ht == __hm2._M_ht;
}


__STL_END_NAMESPACE



#endif //MY_STL_STL_HASH_SET_H