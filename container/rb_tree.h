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
};