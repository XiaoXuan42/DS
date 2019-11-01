#pragma once

#include "../iter/iter.h"
#include "../functor/ds_compare.h"
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

            using __rb_tree_iterator<Value, Ref, Ptr> = self;

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
    template<typename Key, typename Value, typename KeyOfValue,
        typename Compare, typename Alloc = alloc>
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

        static const Compare cmp;
        static const KeyOfValue get_key;
        
        size_type node_count;
        link_type header;

        /* some funciton like alias */
        static link_type & left(link_type x) {
            return (link_type &)(x->left);
        }
        static link_type & right(link_type x) {
            return (link_type &)(x->right);
        }
        static link_type & parent(link_type x) {
            return (link_type &)(x->parent);
        }
        static reference value(link_type x) {
            return x->val;
        }
        static const Key &key(link_type x) {
            return get_key(value(x));
        }
        static color_type & color(link_type x) {
            return color_type(x->color);
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
            return (link_type &)x->val;
        }
        static const Key & key(base_ptr x) {
            return get_key(value(x));
        }
        static color_type & color(base_ptr x) {
            return color_type(x->color);
        }
        static link_type minimum(link_type x) {
            return (link_type)__rb_tree_node_base::minimum((base_ptr)x);
        }
        static link_type maximum(base_ptr x) {
            return (link_type)__rb_tree_node_base::maximum(x);
        }

        link_type & root() {
            return header->parent;
        }

        static link_type get_node() {
            return data_alloc::allocate();
        }
        static void del_node(link_type tp) {
            data_alloc::deallocate(tp);
        }
        void init() {
            header = get_node();
            node_count = 0;
        }
        iterator __insert(base_ptr x, base_ptr y, const value_type v);

    public:
        rb_tree() {
            init();
        }
        rb_tree(const rb_tree &rhs) {
        }
        size_type size() {
            return node_count;
        }
        bool empty() {
            return header->parent == nullptr;
        }
    };

    template<typename Key, typename Value, typename KeyOfValue,
        typename Compare, typename Alloc>
    const Compare rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::cmp = Compare();
    template<typename Key, typename Value, typename KeyOfValue,
        typename Compare, typename Alloc>
    const KeyOfValue rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::get_key = KeyOfValue();
};