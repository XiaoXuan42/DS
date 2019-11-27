#pragma once

#include "rb_tree.h"
#include "../functor/ds_simple_map.h"

namespace DS
{
    template<typename Value, typename Alloc = alloc>
    class set
    {
    public:
        using value_type = Value;
        using reference = value_type&;
        using const_reference = const value_type &;
        using const_pointer = const value_type *;
        using difference_type = ptrdiff_t;
        using iterator = __rb_tree_iterator<Value, Value&, Value*>;
        using size_type = size_t;

    private:
        rb_tree<Value, Value, identical<Value, Value>, less<Value>, alloc> rbt;
    public:
        iterator begin() const {
            return rbt.begin();
        }
        iterator end() const {
            return rbt.end();
        }
        size_type size() const {
            return rbt.size();
        }
        void clear() {
            rbt.clear();
        }
        bool empty() const {
            return rbt.empty();
        }
        iterator find(const value_type &v) const {
            return rbt.find(v);
        }
        iterator insert(const value_type &v) {
            return rbt.insert_unique(v).first();
        }
        void remove(const value_type &v) {
            rbt.remove(v);
        } 
    };
}