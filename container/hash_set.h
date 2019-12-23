#pragma once

#include "../functor/ds_hash_func.h"
#include "../alloc/ds_alloc.h"
#include "../container/hashtable.h"

namespace DS
{
    template<typename Value, typename HashFn = hash<Value>, typename EqualKey = equal_to<Value>, typename Alloc = alloc>
    class hash_set
    {
    private:
        typedef hashtable<Value, Value, HashFn, identity<Value>, EqualKey, Alloc> ht;

        ht htb;
    public:
        typedef typename ht::iterator iterator;
        typedef typename ht::size_type size_type;
        typedef typename ht::value_type value_type;

        hash_set(): ht(100) {}
        explicit hash_set(size_type n): ht(n) {}

        hash_set(const hash_set &) = default;
        hash_set(hash_set &&) = default;
        ~hash_set() = default;
        hash_set & operator = (const hash_set &) = default;
        hash_set & operator = (hash_set &&) = default;

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
            return ht.insert_unique(v).first;    
        }
        iterator find(const value_type &v) const {
            return ht.find(v);
        }
        size_type count() const {
            return ht.count();
        }
        void erase(iterator it) {
            ht.erase(it);
        }
        void erase(iterator l, iterator r) {
            ht.erase(l, r);
        }
        void erase(const value_type &v) {
            ht.erase(v);
        }
    };
}