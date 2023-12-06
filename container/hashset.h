#pragma once

#include "../functor/ds_hash_func.h"
#include "../alloc/ds_alloc.h"
#include "../container/hashtable.h"

namespace xxds
{
    template<typename Value, typename HashFn = hash<Value>, typename EqualKey = equal_to<Value>, typename Alloc = alloc>
    class hashset
    {
    private:
        typedef hashtable<Value, Value, HashFn, identity<Value>, EqualKey, Alloc> ht;

        ht htb;
    public:
        typedef typename ht::iterator iterator;
        typedef typename ht::size_type size_type;
        typedef typename ht::value_type value_type;
        typedef typename ht::const_iterator const_iterator;
        typedef typename ht::const_pointer pointer;
        typedef typename ht::const_pointer const_pointer;
        typedef typename ht::key_equal key_equal;
        typedef typename ht::hasher hasher;

        hashset(): htb(100, hasher(), key_equal()) {}
        explicit hashset(size_type n): htb(n, hasher(), key_equal()) {}
        hashset(size_type n, const hasher& hf): htb(n, hf, key_equal()) {}
        hashset(size_type n, const hasher& hf, const key_equal& eql): htb(n, hf, eql) {}

        hashset(const hashset &) = default;
        hashset(hashset &&) = default;
        ~hashset() = default;
        hashset & operator = (const hashset &) = default;
        hashset & operator = (hashset &&) = default;

    public:
        size_type size() const {
            return htb.size();
        }
        bool empty() const {
            return htb.empty();
        }
        iterator begin() const {
            return htb.begin();
        }
        iterator end() const {
            return htb.end();
        }
        void clear() {
            htb.clear();
        }
        iterator insert(const value_type &v) {
            return htb.insert_unique(v).first();    
        }
        iterator find(const value_type &v) const {
            return htb.find(v);
        }
        size_type count(const value_type &v) const {
            return htb.count(v);
        }
        void erase(iterator it) {
            htb.erase(it);
        }
        void erase(iterator l, iterator r) {
            htb.erase(l, r);
        }
        void erase(const value_type &v) {
            htb.erase(v);
        }
    };
}