#pragma once

#include "../functor/ds_simple_map.h"
#include "../functor/ds_compare.h"
#include "../alloc/ds_memory.h"
#include "../container/rb_tree.h"

namespace DS
{
    template<typename Key, typename Value, typename KeyOfValue, typename Comp, typename Alloc = alloc>
    class map
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
        using iterator = typename rb_tree<Key, Value, KeyOfValue, Comp, Alloc>::iterator;
    private:
        rb_tree<Key, Value, KeyOfValue, Comp, Alloc> rbt;
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
    };
}