#pragma once

#include "./pair.h"
#include "../iter/iter.h"
#include "../functor/ds_compare.h"
#include "../functor/ds_simple_map.h"
#include "../alloc/ds_memory.h"

namespace DS
{
    namespace {
        typedef bool __rb_tree_color;
        const __rb_tree_color __rb_tree_black = false;
        const __rb_tree_color __rb_tree_red = true;

        struct __rb_tree_node_base
        {
            typedef __rb_tree_node_base* base_ptr;
            typedef __rb_tree_color color_type;
            base_ptr lch, rch;
            base_ptr parent;
            color_type color;
            __rb_tree_node_base(): lch(nullptr), rch(nullptr), parent(nullptr), color(__rb_tree_black) {}
            static base_ptr minimum(base_ptr x) {
                while(x->lch != nullptr) {
                    x = x->lch;
                }
                return x;
            }
            static base_ptr maximum(base_ptr y) {
                while(y->rch != nullptr) {
                    y = y->rch;
                }
                return y;
            }
        };
        template<typename Value>
        struct __rb_tree_node : public __rb_tree_node_base
        {
            typedef __rb_tree_node<Value> * link_type;
            Value val;
        };
        struct __rb_tree_iterator_base
        {
            typedef typename __rb_tree_node_base::base_ptr base_ptr;
            typedef bidirectional_iterator_tag iterator_category;
            base_ptr node;

            __rb_tree_iterator_base(): node(nullptr) {}
            __rb_tree_iterator_base(base_ptr node): node(node) {}
            
            void increment() {
                if(node->rch != nullptr) {
                    node = node->rch;
                    while(node->lch != nullptr) {
                        node = node->lch;
                    }
                }
                else {
                    base_ptr p = node->parent;
                    while(node == p->rch) {
                        node = p;
                        p = p->parent;
                    }
                    if(node->rch != p) {
                        node = p;
                    } // if the root is the maximum element, then increment it will make the iterator point to the header
                }
            }
            void decrement() {
                
                //if the node point to the header, then set it point to the maximum element
                if(node->color == __rb_tree_red && node->parent->parent == node) {
                    node = node->rch;
                    return;
                }
            
                if(node->lch != nullptr) {
                    node = node->lch;
                    while(node->rch != nullptr) {
                        node = node->rch;
                    }
                }
                else {
                    base_ptr p = node->parent;
                    while(node == p->lch) {
                        node = p;
                        p = p->parent;
                    }
                    node = p;
                }
            }
        };

        template<typename Value, typename Ref, typename Ptr>
        struct __rb_tree_iterator : public __rb_tree_iterator_base
        {
            using value_type = Value;
            using pointer = Ptr;
            using reference = Ref;
            using difference_type = ptrdiff_t;
            using link_type = __rb_tree_node<Value>*;

            using self = __rb_tree_iterator<Value, Ref, Ptr>;

            __rb_tree_iterator() {}
            __rb_tree_iterator(link_type lk): __rb_tree_iterator_base(lk) {
            }

            bool operator == (const __rb_tree_iterator &rhs) const {
                return node == rhs.node;
            }
            bool operator != (const __rb_tree_iterator &rhs) const {
                return !operator==(rhs);
            }
            reference operator * () const {
                return link_type(node)->val;
            }
            pointer operator ->() const {
                // this operator is special for smart pointer
                // it actually return the raw pointer to the value_type
                return &(operator *());
            }
            self & operator ++() {
                increment();
                return *this;
            }
            self & operator --() {
                decrement();
                return *this;
            }
            self operator ++(int) {
                self tmp = *this;
                operator++();
                return tmp;
            }
            self operator --(int) {
                self tmp = *this;
                operator--();
                return tmp;
            }
        };
    }

    //the keyofvalue stand for a map from value to its key
    template<typename Key, typename Value = Key, typename KeyOfValue = identical<Value, Key>,
        typename Compare = less<Key>, typename Alloc = alloc>
    class rb_tree
    {
    public:
        using value_type = Value;
        using reference = value_type&;
        using const_pointer = const value_type *;
        using const_reference = const value_type &;
        using difference_type = ptrdiff_t;
        using iterator = __rb_tree_iterator<Value, Value&, Value*>;
        using key_type = Key;
        using size_type = size_t;

    private:
        using base_ptr = typename __rb_tree_node_base::base_ptr;
        using link_type = typename __rb_tree_node<value_type>::link_type;
        using data_alloc = simple_alloc<__rb_tree_node<value_type>, Alloc>;
        using color_type = __rb_tree_color;
        using self = rb_tree<Key, Value, KeyOfValue, Compare, Alloc>;

        // make them static because they are just functors.
        static const Compare cmp;
        static const KeyOfValue get_key;
        
        size_type node_count;
        link_type header;

        /* some funciton like alias */
        static link_type & left(link_type x) {
            return (link_type &)(x->lch);
        }
        static link_type & right(link_type x) {
            return (link_type &)(x->rch);
        }
        static link_type & parent(link_type x) {
            return (link_type &)(x->parent);
        }
        static reference value(link_type x) {
            return x->val;
        }
        static const Key key(link_type x) {
            return get_key(value(x));
        }
        static color_type & color(link_type x) {
            return (color_type&)(x->color);
        }
        static link_type & left(base_ptr x) {
            return (link_type &)(x->lch);
        }
        static link_type & right(base_ptr x) {
            return (link_type &)(x->rch);
        }
        static link_type & parent(base_ptr x) {
            return (link_type &)(x->parent);
        }
        static reference value(base_ptr x) {
            return (link_type(x))->val;
        }
        static const Key key(base_ptr x) {
            return get_key(value(x));
        }
        static color_type & color(base_ptr x) {
            return (color_type &)(x->color);
        }
        static link_type minimum(link_type x) {
            return (link_type)__rb_tree_node_base::minimum((base_ptr)x);
        }
        static link_type maximum(base_ptr x) {
            return (link_type)__rb_tree_node_base::maximum(x);
        }

        link_type & leftmost() const {
            return left(header);
        }
        link_type & rightmost() const {
            return right(header);
        }
        link_type & root() const {
            return parent(header);
        }

        static link_type get_node() {
            // notice: data_alloc::allocate() will not autonomously call the _rb_tree_node's constructor
            return data_alloc::allocate();
        }
        static void del_node(link_type tp) {
            data_alloc::deallocate(tp);
        }
        static link_type create_node(const value_type &x) {
            link_type tmp = get_node();
            construct(&tmp->val, x);
            return tmp;
        }
        void init() {
            header = get_node();
            leftmost() = header;
            rightmost() = header;
            color(header) = __rb_tree_red;
            node_count = 0;

            root() = nullptr;
        }
        link_type __insert(link_type x, link_type y, const value_type &v); // the parent is y, current is x
        link_type __copy(link_type x, link_type p);
        void __insert_rebalance_tree(base_ptr x, base_ptr y);
        void __erase(link_type x);
        void __clear(link_type x) {
            if(x == nullptr || x == header)    return;
            __clear(left(x));
            __clear(right(x));
            del_node(x);
        }
        inline void __rb_tree_rotateright(base_ptr x);
        inline void __rb_tree_rotateleft(base_ptr x);

    public:
        rb_tree(): node_count(0) {
            init();
        }
        rb_tree(const rb_tree &rhs) {
            //TODO: finish this
        }
        ~rb_tree() {
            clear();
            delete header;
            node_count = 0;
            header = nullptr;
        }
        void clear() {
            __clear(root());
            leftmost() = header;
            rightmost() = header;
            color(header) = __rb_tree_red;
            node_count = 0;
            root() = nullptr;
        }
        size_type size() {
            return node_count;
        }
        bool empty() {
            return header->parent == nullptr;
        }
        iterator begin() {
            return iterator(leftmost());
        }
        iterator end() {
            return iterator(header);
        }
        pair<iterator, bool> insert_unique(const value_type &v);
        iterator insert_equal(const value_type &v);
    };

    // for simplicity I declare some alias
    #define Type_def_header template<typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
    #define Rb_tree_t rb_tree<Key, Value, KeyOfValue, Compare, Alloc>
    #define Rb_attr(attribute_name) Rb_tree_t:: attribute_name
    #define Rb_type(type_name) typename Rb_tree_t:: type_name

    Type_def_header
    const Compare Rb_attr(cmp) = Compare();

    Type_def_header
    const KeyOfValue Rb_attr(get_key) = KeyOfValue();

    Type_def_header
    inline void Rb_attr(__rb_tree_rotateleft) (Rb_type(base_ptr) x) {
        // x should never be the header
        link_type newroot = right(x);
        link_type pa_x = parent(x);
        link_type _x = link_type(x);
        right(x) = left(newroot);
        if(left(newroot) != nullptr)
            parent(left(newroot)) = _x;
        left(newroot) = _x;

        if(x == root()) {
            parent(newroot) = header;
            parent(x) = newroot;
            root() = newroot;
        }
        else if(x == left(pa_x)) {
            parent(newroot) = pa_x;
            parent(x) = newroot;
            left(pa_x) = newroot;
        }
        else {
            parent(newroot) = pa_x;
            parent(x) = newroot;
            right(pa_x) = newroot;
        }
    }
    Type_def_header
    inline void Rb_attr(__rb_tree_rotateright) (Rb_type(base_ptr) x) {
        link_type newroot = left(x);
        link_type pa_x = parent(x);
        link_type _x = link_type(x);
        left(x) = right(newroot);
        if(right(newroot) != nullptr)
            parent(right(newroot)) = _x;
        right(newroot) = _x;
        
        if(x == root()) {
            parent(newroot) = header;
            parent(x) = newroot;
            root() = newroot;
        }
        else if(x == left(pa_x)) {
            parent(newroot) = pa_x;
            parent(x) = newroot;
            left(pa_x) = newroot;
        }
        else {
            parent(newroot) = pa_x;
            parent(x) = newroot;
            right(pa_x) = newroot;
        }
    }
    Type_def_header
    void Rb_attr(__insert_rebalance_tree) (Rb_type(base_ptr) x, Rb_type(base_ptr) y) {
        base_ptr grandpa;
        base_ptr uncle;
        while(x != header && color(y) == __rb_tree_red) {
            // the x is the root
            if(y == header) {
                color(x) = __rb_tree_black;
                break;
            }
            // because y is a red node, it must have a parent
            // because y is a red node, it's not the root
            grandpa = parent(y);
            if(y == left(grandpa)) {
                uncle = right(grandpa);
            }
            else {
                uncle = left(grandpa);
            }
            if(uncle != nullptr && color(uncle) == __rb_tree_red) {
                color(uncle) = __rb_tree_black;
                color(y) = __rb_tree_black;
                color(grandpa) = __rb_tree_red;
                x = grandpa;
                y = parent(x);
            }
            else {
                color(x) = __rb_tree_black;
                color(grandpa) = __rb_tree_red;
                if(x == left(y)) {
                    __rb_tree_rotateright(y);
                    // the position of x and y has already been changed
                    if(x == left(grandpa)) {
                        __rb_tree_rotateright(grandpa);
                    }
                    else {
                        __rb_tree_rotateleft(grandpa);
                    }
                }
                else {
                    __rb_tree_rotateleft(y);
                    if(x == right(grandpa)) {
                        __rb_tree_rotateleft(grandpa);
                    }
                    else {
                        __rb_tree_rotateright(grandpa);
                    }
                }
                // after these rotation, all the propertities are kept
                break;
            }
        } 
    }

    Type_def_header
    Rb_type(link_type) Rb_attr(__insert) (Rb_type(link_type) x, Rb_type(link_type) y, const Rb_type(value_type) &v) {
        link_type z;
        z = create_node(v);
        color(z) = __rb_tree_red;
        if(y == header || cmp(get_key(v), key(y))) {
            // if y == header, this will set the leftmost autonomously
            left(y) = z;
            if(y == header) {
                rightmost() = z;
                root() = z;
            }
            else if(y == leftmost()) {
                leftmost() = z;
            }
        }
        else {
            right(y) = z;
            if(y == rightmost()) {
                rightmost() = z;
            }
        }
        parent(z) = y;
        left(z) = nullptr;
        right(z) = nullptr;
        __insert_rebalance_tree(z, y);
        return z;
    }

    Type_def_header
    Rb_type(iterator) Rb_attr(insert_equal) (const Rb_type(value_type) &v) {
        link_type y = header;
        link_type x = root();
        while(x != nullptr) {
            y = x;
            x = cmp(get_key(v), key(x)) ? left(x) : right(x);
        }
        return iterator(__insert(x, y, v));
    }
    Type_def_header
    pair<Rb_type(iterator), bool> Rb_attr(insert_unique) (const Rb_type(value_type) &v) {
        link_type y = header;
        link_type x = root();
        bool last_cmp = false;
        while(x != nullptr) {
            y = x;
            last_cmp = cmp(get_key(v), key(x));
            x = last_cmp ? left(x) : right(x);
        }
        iterator j = iterator(y);
        //determine whether there is already v in the tree
        if(last_cmp) {
            if(j == begin()) {
                return pair<iterator, bool>(iterator(__insert(x, y, v)), true);
            }
            else {
                // the grandparent of j may be equal to the v
                j--;
            }
        }
        if(cmp(key(j.node), get_key(v))) {
            return pair<iterator, bool>(iterator(__insert(x, y, v)), true);
        }
        return pair<iterator, bool>(j, false);
    }

    #undef Rb_alias
    #undef Rb_tree_t
    #undef Type_def_header
};