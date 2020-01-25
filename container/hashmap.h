#pragma once

#include "../functor/ds_hash_func.h"
#include "../alloc/ds_alloc.h"
#include "../container/hashtable.h"
#include "../functor/ds_function.h"
#include "../container/pair.h"

namespace DS
{
    template<typename Key, typename Value, typename HashFn = hash<Key>, typename EqualKey = equal_to<Key>, typename Alloc = alloc>
    class hashmap
    {
    public:
        typedef pair<Key, Value> value_type;
        typedef Key key_type;
    private:
        typedef hashtable<value_type, key_type, HashFn, select1<value_type, key_type>, EqualKey, Alloc> ht;
        ht htb;

    public:
        typedef typename ht::iterator iterator;
        typedef typename ht::size_type size_type;
        typedef typename ht::const_iterator const_iterator;
        typedef typename ht::const_pointer pointer;
        typedef typename ht::const_pointer const_pointer;
        typedef typename ht::key_equal key_equal;
        typedef typename ht::hasher hasher;

        hashmap(): htb(100, hasher(), key_equal()) {}
        explicit hashmap(size_type n): htb(n, hasher(), key_equal()) {}
        hashmap(size_type n, const hasher& hf): htb(n, hf, key_equal()) {}
        hashmap(size_type n, const hasher& hf, const key_equal& eql): htb(n, hf, eql) {}
        hashmap(const hashmap &) = default;
        hashmap(hashmap &&) = default;
        ~hashmap() = default;
        hashmap & operator = (const hashmap &) = default;
        hashmap & operator = (hashmap &&) = default;
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
        pair<iterator, bool> insert(const value_type &v) {
            return htb.insert_unique(v);
        }
        iterator find(const key_type &v) const {
            return htb.find(v);
        }
        size_type count(const key_type &k) const {
            return htb.count(k);
        }
        void erase(iterator it) {
            htb.erase(it);
        }
        void erase(iterator l, iterator r) {
            htb.erase(l, r);
        }
        void erase(const key_type &v) {
            htb.erase(v);
        }
        Value & operator [] (const key_type &k) {
            return (*(insert(value_type(k, Value())).first())).second();
        }
    };
}