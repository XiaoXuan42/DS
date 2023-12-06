#pragma once

#include "../alloc/ds_memory.h"
#include "rb_tree.h"
#include "pair.h"
#include "../functor/ds_function.h"

namespace xxds
{
    template<typename Key, typename Value, typename Comp = less<Key>, typename Alloc = alloc>
    class map
    {
    public:
        using value_type = pair<Key, Value>;
        using key_type = Key;
        using reference = value_type&;
        using const_pointer = const value_type *;
        using const_reference = const value_type &;
        using difference_type = ptrdiff_t;
        using size_type = size_t;
        using iterator = typename rb_tree<key_type, value_type, select1<value_type, Key>, Comp, Alloc>::iterator;
    private:
        rb_tree<key_type, value_type, select1<value_type, Key>, Comp, Alloc> rbt;
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
        iterator lower_bound(const key_type &k) const {
            return rbt.lower_bound(k);
        }
        iterator upper_bound(const key_type &k) const {
            return rbt.upper_bound(k);
        }
        iterator find(const key_type &k) const {
            return rbt.find(k);
        }
        void remove(const key_type &k) const {
            rbt.remove(k);
        }
        pair<iterator, bool> insert(const value_type &x) {
            return rbt.insert_unique(x);
        }
        Value & operator [] (const key_type &k) {
            return (*(insert(value_type(k, Value())).first())).second();
        }
    };
}