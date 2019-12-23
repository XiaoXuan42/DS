#pragma once

#include "../iter/iter.h"
#include "../alloc/ds_alloc.h"
#include "../container/vector.h"
#include "../algorithm/ds_search.h"
#include "../container/pair.h"
#include "../algorithm/ds_algobase.h"
#include "../functor/ds_function.h"

namespace DS
{
    namespace {
        typedef unsigned long long hash_ds_prime;
        static const int __ds_prime_nums = 28;
        static const hash_ds_prime __ds_prime_list[__ds_prime_nums] = {
            53, 97, 193, 389, 796, 1543, 3079, 6151, 12289,
            24593, 49157, 98317, 196613, 393241, 786433,
            1572896, 3145739, 6291469, 12582917, 25165843, 50331653,
            10063319, 201326611, 402653189, 805306457, 1610612741,
            3221225473ull, 4294967291ull
        };
        hash_ds_prime hash_lower_bound_prime(hash_ds_prime x) {
            const hash_ds_prime * start = __ds_prime_list;
            const hash_ds_prime * end = __ds_prime_list + __ds_prime_nums;
            const hash_ds_prime * target = lower_bound(start, end, x);
            if(target == end) {
                return __ds_prime_list[__ds_prime_nums-1];
            }
            else {
                return *target;
            }
        }
    }

    template<typename Value, typename Key, typename HashFn, typename ExtractKey, typename EqualKey, typename Alloc=alloc>
    class hashtable;

    template<typename Value>
    struct __hashtable_node
    {
        Value val;
        __hashtable_node *next;
    };

    template<typename Value, typename Key, typename HashFn,
                typename ExtractKey, typename EqualKey, typename Alloc>
    struct __hashtable_iterator
    {
        typedef forward_iterator_tag iterator_category;
        typedef Value value_type;
        typedef Value* pointer;
        typedef Value& reference;
        typedef ptrdiff_t difference_type;

        typedef hashtable<Value, Key, HashFn, ExtractKey, EqualKey, Alloc> htable;
        typedef __hashtable_node<Value> node;
        typedef __hashtable_iterator<Value, Key, HashFn, ExtractKey, EqualKey, Alloc> iterator;
        typedef typename hashtable<Value, Key, HashFn, ExtractKey, EqualKey, Alloc>::size_type size_type;

        node *p;
        htable *htb;

        iterator& operator++ () {
            node * pnext = p->next;
            if(pnext == nullptr) {
                size_type index = htb->bk_num(pnext->val) + 1;                
                size_type htb_size = htb->buck_size();
                while(index < htb_size && htb.bucket[index] == nullptr) {
                    ++index;
                }
                pnext = *htb.bucket[index];
            }
            p = pnext;
        }
        iterator operator++ (int) {
            iterator tmp = *this;
            ++(*this);
            return tmp;
        }
        reference operator* () {
            return p->val;
        }
        pointer operator-> () {
            return &(p->val);
        }
        bool operator== (const iterator &rhs) {
            return p == rhs.p && htb == rhs.htb;
        }
        bool operator != (const iterator &rhs) {
            return !(*this == rhs);
        }
    };
    template<typename Value, typename Key, typename HashFn, typename ExtractKey, typename EqualKey, typename Alloc>
    class hashtable
    {
    public:
        typedef __hashtable_iterator<Value, Key, HashFn, ExtractKey, EqualKey, Alloc> iterator;
        typedef Value value_type;
        typedef Value& reference;
        typedef Value* pointer;
        typedef size_t size_type;
        typedef Key key_type;
    private:
        typedef hashtable<Value, Key, HashFn, ExtractKey, EqualKey, Alloc> self;
    private:
        typedef __hashtable_node<Value> node_type;
        typedef node_type* link_node;
        typedef simple_alloc<node_type, Alloc> node_alloc;
        
        link_node allocate_node() {
            return node_alloc::allocate();
        }
        void deallocate_node(link_node p) {
            node_alloc::deallocate(p);
        }

    private:
        HashFn hash;
        ExtractKey get_key;
        EqualKey equal;

        vector<link_node, Alloc> bucket;        
        size_type num_content;


        void rehash(size_type n);
        pair<link_node, bool> __insert_unique(const value_type &v);
        link_node __insert_equal(const value_type &v);

    public:
        friend class __hashtable_iterator<Value, Key, HashFn, ExtractKey, EqualKey, Alloc>;

        hashtable(): bucket(), num_content(0) {}
        hashtable(const hashtable &other) = default;
        hashtable(hashtable &&other) = default;
        ~hashtable() = default;
        hashtable & operator = (const hashtable &other) = default;
        hashtable & operator = (hashtable &&other) = default;
        
        size_type buck_size() {
            return bucket.size();
        }

        size_type bk_num_val(const value_type &v, size_type n) {
            return bk_num_key(get_key(v), n);
        }
        size_type bk_num_key(const key_type &k, size_type n) {
            return hash(k) % n;
        }
        size_type bk_num_key(const key_type &k) {
            return bk_num_key(k, bucket.size());
        }
        size_type bk_num_val(const value_type &v) {
            return bk_num_key(get_key(v));
        }

        iterator begin() {
            size_type index = 0;
            size_type htb_size = buck_size();
            iterator result;
            result.htb = this;
            result.p = nullptr;
            for(index = 0; index < htb_size; index++) {
                if(bucket[index] != nullptr) {
                    result.p = bucket[index];
                    break;
                }
            }
            return result;
        }
        iterator end() {
            iterator result;
            result.htb = this;
            result.p = nullptr;
            return result;
        }  
        pair<iterator, bool> insert_unique(const value_type &v) {
            pair<link_node, bool> result = __insert_unique(v);
            pair<iterator, bool> ans;
            ans.first().p = result.first();
            ans.first().htb = this;
            ans.second() = result.second();
            return ans;
        }
        iterator insert_equal(const value_type &v) {
            link_node result = __insert_equal(v);
            iterator ans;
            ans.htb = this;
            ans.p = result;
            return ans;
        }
    };
    #define HASHTB_TYPE template<typename Value, typename Key, typename HashFn, typename ExtractKey, typename EqualKey, typename Alloc>    
    #define HASHTB_ATTR(attr_name) hashtable<Value, Key, HashFn, ExtractKey, EqualKey, Alloc>::attr_name

    HASHTB_TYPE
    void HASHTB_ATTR(rehash) (HASHTB_ATTR(size_type) n) {
        size_type new_sz = hash_lower_bound_prime(n);
        vector<link_node, Alloc> new_buck(new_sz, nullptr);
        size_type index = 0, hbt_size = buck_size();
        size_type new_index;
        for(index = 0; index < hbt_size; index++) {
            link_node cur = bucket[index];
            link_node cur_next;
            while(cur != nullptr) {
                cur_next = cur->next;
                new_index = bk_num_val(cur->val, new_sz);
                cur->next = new_buck[new_index];
                new_buck[new_index] = cur;
                cur = cur_next;
            }
        }
        swap(new_buck, bucket);
    }

    HASHTB_TYPE
    pair<typename HASHTB_ATTR(link_node), bool> HASHTB_ATTR(__insert_unique) (const HASHTB_ATTR(value_type) &v) {
        num_content++;
        if(num_content > buck_size()) {
            rehash(num_content);
        }
        size_type index = bk_num_val(v);
        link_node cur = bucket[index];
        while(cur != nullptr) {
            if(cur->val == v) {
                return pair<link_node, bool>(cur, false);
            }
        }
        link_node new_node = allocate_node();
        new_node->next = bucket[index];
        new_node->val = v;
        bucket[index] = new_node;
        return pair<link_node, bool>(new_node, true);
    }

    HASHTB_TYPE
    typename HASHTB_ATTR(link_node) HASHTB_ATTR(__insert_equal) (const HASHTB_ATTR(value_type) &v) {
        num_content++;
        if(num_content > buck_size()) {
            rehash(num_content);
        }
        size_type index = bk_num_val(v);
        link_node new_node = allocate_node();
        new_node->next = bucket[index];
        new_node->val = v;
        bucket[index] = new_node;
        return new_node;
    }

    #undef HASHTB_TYPE
    #undef HASHTB_ATTR
}