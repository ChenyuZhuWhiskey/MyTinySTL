/* Created by Chenyu ZHU on 20200612 */

#ifndef MY_STL_STL_MULTISET_H
#define MY_STL_STL_MULTISET_H

#include "stl_config.h"
#include "stl_pair.h"
#include "stl_tree.h"
#include "stl_function.h"

__STL_BEGIN_NAMESPACE

template<class _Key, class _Compare = less<_Key>, class _Alloc = alloc>
class multiset;

template<class _Key, class _Compare, class _Alloc>
inline bool operator==(const multiset<_Key, _Compare, _Alloc>& __x,
    const multiset<_Key, _Compare, _Alloc>& __y);

template<class _Key, class _Compare, class _Alloc>
inline bool operator<(const multiset<_Key, _Compare, _Alloc>& __x,
    const multiset<_Key, _Compare, _Alloc>& __y);

template<class _Key, class _Compare, class _Alloc>
class multiset {
public:
    typedef _Key key_type;
    typedef _Key value_type;
    typedef _Compare key_compare;
    typedef _Compare value_compare;


private:
    typedef _Rb_tree<key_type, value_type, _Select1st<value_type>, key_compare, _Alloc> _Rep_type;
    _Rep_type _M_t;
public:
    typedef typename _Rep_type::const_pointer pointer;
    typedef typename _Rep_type::const_pointer const_pointer;
    typedef typename _Rep_type::const_reference reference;
    typedef typename _Rep_type::const_reference const_reference;
    typedef typename _Rep_type::const_iterator iterator;
    typedef typename _Rep_type::const_iterator const_iterator;
    typedef typename _Rep_type::const_reverse_iterator reverse_iterator;
    typedef typename _Rep_type::const_reverse_iterator const_reverse_iterator;
    typedef typename _Rep_type::size_type size_type;
    typedef typename _Rep_type::difference_type difference_type;
    typedef typename _Rep_type::allocator_type allocator_type;

    multiset() :_M_t(_Compare(), allocator_type()) {}
    explicit multiset(const _Compare& __comp, const allocator_type& __a = allocator_type())
        : _M_t(__comp, __a) {}

    multiset(const value_type* __first, const value_type* __last)
        : _M_t(_Compare(), allocator_type()) {
        _M_t.insert_equal(__first, __last);

    }

    multiset(const value_type* __first, const value_type __last, const _Compare& __comp, const allocator_type& __a = allocator_type())
        : _M_t(__comp, __a)
    {
        _M_t.insert_equal(__first, __last);
    }

    multiset(const_iterator __first, const_iterator __last, const _Compare& __comp,
        const allocator_type& __a = allocator_type())
        : _M_t(__comp, __a) { _M_t.insert_equal(__first, __last); }

    multiset(const set<_Key,_Compare,_Alloc>& __x): _M_t(__x._M_t) {}

    multiset<_Key, _Compare, _Alloc>&
    operator=(const set<_Key,_Compare,_Alloc>& __x){
        _M_t = __x._M_t;
        return *this;
    }

    key_compare key_comp() const {return _M_t.key_comp();}
    value_compare value_comp() const {return value_compare(_M_t.key_comp());}
    allocator_type get_allocator() const {return _M_t.get_allocator(); }

    iterator begin() const {return _M_t.begin(); }
    iterator end() const {return _M_t.end(); }
    reverse_iterator rbegin() const {return end();}
    reverse_iterator rend() const {return begin();}
    bool empty() const {return _M_t.empty();}
    size_type size() const {return _M_t.size();}
    size_type max_size() const {return _M_t.max_size();}

    void swap(multiset<_Key,_Compare,_Alloc>& __x) {return _M_t.swap(__x._M_t);}

    pair<iterator,bool> insert(const value_type& __x){
        return _M_t.insert_equal(__x);
    }
    iterator insert(iterator __pos, const value_type& __x){
        return _M_t.insert_equal(__pos, __x);
    }

    void insert(const value_type* __first, const value_type* __last){
        _M_t.insert_equal(__first, __last);
    }

    void erase(iterator __pos) {_M_t.earse(__pos);}
    size_type erase(const key_type& __x) {return _M_t.erase(__x);}
    void erase(iterator __first, iterator __last){
        _M_t.erase(__first, __last);
    }
    void clear() {_M_t.clear();}

    // set operations
    iterator find(const key_type& __x) const {return _M_t.find(__x);}
    size_type count(const key_type& __x) const{
        return _M_t.count(__x);
    }

    iterator lower_bound(const key_type& __x) const {return _M_t.lower_bound(__x);}
    iterator upper_bound(const key_type& __x) const {return _M_t.upper_bound(__x);}

    pair<iterator,iterator>equal_range(const key_type& __x) const{
        return _M_t.equal_range(__x);
    }

    friend bool operator==(const multiset&, const multiset&);
    friend bool operator<(const multiset&, const multiset&);


};

template<class _Key, class _Compare, class _Alloc>
inline bool operator==(const multiset<_Key, _Compare, _Alloc>& __x, const multiset<_Key, _Compare, _Alloc>& __y){
    return __x._M_t == __y._M_t;
}


template<class _Key, class _Compare, class _Alloc>
inline bool operator<(const multiset<_Key, _Compare, _Alloc>& __x, const multiset<_Key, _Compare, _Alloc>& __y){
    return __x._M_t < __y._M_t;
}

__STL_END_NAMESPACE

#endif