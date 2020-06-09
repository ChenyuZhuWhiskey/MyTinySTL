/*Created by Chenyu ZHU on 2020/06/08 */

#ifndef MY_STL_STL_TREE_H
#define MY_STL_STL_TREE_H



namespace MySTL_Unfinished {

#include "stl_algobase.h"
#include "stl_alloc.h"
#include "stl_construct.h"
#include "stl_config.h"
#include "stl_function.h"

    typedef bool _Rb_tree_Color_type;
    const _Rb_tree_Color_type _S_rb_tree_red = false;
    const _Rb_tree_Color_type _S_rb_tree_black = true;

    struct _Rb_tree_node_base {
        typedef _Rb_tree_Color_type _Color_type;
        typedef  _Rb_tree_node_base* _Base_ptr;

        _Color_type _M_color;
        _Base_ptr _M_parent;
        _Base_ptr _M_left;
        _Base_ptr _M_right;

        static _Base_ptr _S_minimum(_Base_ptr __x) {
            while (__x->_M_left != nullptr) {
                __x = __x->_M_left;
            }
            return __x;
        }

        static _Base_ptr _S_minimum(_Base_ptr __y) {
            while (__y->_M_right != nullptr) {
                __y = __y->_M_right;
            }
            return __y;
        }
    };


}

#include "sgi_stl_tree.h"






#endif