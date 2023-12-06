#pragma once

#include "rb_tree.h"
#include "../functor/ds_function.h"

namespace xxds
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
        using size_type = size_t;
        using iterator = __rb_tree_iterator<Value, Value&, Value*>;

    private:
        rb_tree<Value, Value, identity<Value>, less<Value>, alloc> rbt;
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
        iterator lower_bound(const value_type &v) {
            return rbt.lower_bound(v);
        }
        iterator upper_bound(const value_type &v) {
            return rbt.upper_bound(v);
        }
    };
}