#pragma once

#include "../iter/iter.h"
#include "../alloc/ds_alloc.h"
#include "../container/vector.h"
#include "../algorithm/ds_search.h"
#include "../container/pair.h"
#include "../algorithm/ds_algobase.h"
#include "../functor/ds_function.h"


namespace {
    typedef unsigned long long hash_ds_prime;
    const int __ds_prime_nums = 28;
    const hash_ds_prime __ds_prime_list[__ds_prime_nums] = {
        53ull, 97ull, 193ull, 389ull, 796ull, 1543ull, 3079ull, 6151ull, 12289ull,
        24593ull, 49157ull, 98317ull, 196613ull, 393241ull, 786433ull,
        1572896ull, 3145739ull, 6291469ull, 12582917ull, 25165843ull, 50331653ull,
        10063319ull, 201326611ull, 402653189ull, 805306457ull, 1610612741ull,
        3221225473ull, 4294967291ull
    };
    hash_ds_prime hash_lower_bound_prime(hash_ds_prime x) {
        const hash_ds_prime * start = __ds_prime_list;
        const hash_ds_prime * end = __ds_prime_list + __ds_prime_nums;
        const hash_ds_prime * target = DS::lower_bound(start, end, x);
        if(target == end) {
            return __ds_prime_list[__ds_prime_nums-1];
        }
        else {
            return *target;
        }
    }
}
namespace DS
{
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
        typedef const iterator const_iterator;
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
            return *this;
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
        typedef const iterator const_iterator;
        typedef Value value_type;
        typedef Value& reference;
        typedef const reference const_reference;
        typedef Value* pointer;
        typedef const pointer const_pointer;
        typedef size_t size_type;
        typedef Key key_type;
        typedef HashFn hasher;
        typedef EqualKey key_equal;
    private:
        typedef hashtable<Value, Key, HashFn, ExtractKey, EqualKey, Alloc> self;
        typedef __hashtable_node<Value> node_type;
        typedef node_type* link_node;
        typedef simple_alloc<node_type, Alloc> node_alloc;
        
        link_node allocate_node() {
            return node_alloc::allocate();
        }
        link_node allocate_node(const value_type &obj) {
            link_node n = node_alloc::allocate();
            construct(&(n->val), obj);
            return n;
        }
        void deallocate_node(link_node p) {
            destroy(&(p->val));
            node_alloc::deallocate(p);
        }

    private:
        hasher hash;
        ExtractKey get_key;
        key_equal equals;

        vector<link_node, Alloc> bucket;        
        size_type num_content;

        void rehash(size_type n);
        pair<link_node, bool> __insert_unique(const value_type &v);
        link_node __insert_equal(const value_type &v);

        void initialize_buckets(size_type n) {
            const size_type n_buckets = hash_lower_bound_prime(n);
            bucket.reserve(n_buckets);
            bucket.insert(bucket.end(), n_buckets, nullptr);
            num_content = 0;
        }
        void copy_from(const hashtable &other) {
            clear();
            size_type htb_size = other.buck_size();
            bucket.reserve(htb_size);
            bucket.insert(bucket.begin(), htb_size, nullptr);
            num_content = other.num_content;
            for(size_type index = 0; index < htb_size; index++) {
                link_node cur = other.bucket[index];
                link_node rear;
                if(cur != nullptr) {
                    link_node new_node = allocate_node(cur->val);
                    new_node->next = nullptr;
                    rear = new_node;
                    bucket[index] = new_node;
                    cur = cur->next;
                    while(cur != nullptr) {
                        new_node = allocate_node(cur->val);
                        new_node->next = nullptr;
                        rear->next = new_node;
                        rear = new_node;
                        cur = cur->next;
                    }
                }
            }
        }

    public:
        friend class __hashtable_iterator<Value, Key, HashFn, ExtractKey, EqualKey, Alloc>;

        hashtable(size_type n, const HashFn &fn, const EqualKey& eql): hash(fn), get_key(ExtractKey()), equals(eql), num_content(0) {
            initialize_buckets(n);
        }
        hashtable(size_type n, const HashFn &fn): hash(fn), get_key(ExtractKey()), equals(key_equal()), num_content(n) {
            initialize_buckets(n);
        }
        explicit hashtable(size_type n): hash(hasher()), get_key(ExtractKey()), equals(key_equal()), num_content(0) {
            initialize_buckets(n);
        }
        hashtable(): hash(hasher()), get_key(ExtractKey()), equals(key_equal()), num_content(0) {
            initialize_buckets(0);
        }
        hashtable(const hashtable &other) {
            copy_from(other);
        }
        hashtable(hashtable &&other) {
            bucket.swap(other.bucket);
            num_content = other.num_content;
        }
        ~hashtable() {
            clear();
        }
        hashtable & operator = (const hashtable &other) {
            copy_from(other);
            return *this;
        }
        hashtable & operator = (hashtable &&other) {
            bucket.swap(other.bucket);
            num_content = other.num_content;
        }
        
        size_type buck_size() const {
            return bucket.size();
        }

        size_type bk_num_val(const value_type &v, size_type n) const {
            return bk_num_key(get_key(v), n);
        }
        size_type bk_num_key(const key_type &k, size_type n) const {
            return hash(k) % n;
        }
        size_type bk_num_key(const key_type &k) const {
            return bk_num_key(k, bucket.size());
        }
        size_type bk_num_val(const value_type &v) const {
            return bk_num_key(get_key(v));
        }

        void clear() {
            num_content = 0;
            size_type index = 0;
            size_type htb_size = buck_size();
            while(index < htb_size) {
                link_node cur = bucket[index];
                while(cur != nullptr) {
                    bucket[index] = cur->next;
                    deallocate_node(cur);
                    cur = bucket[index];
                }
                index++;
            }
        }

        bool empty() const {
            return num_content == 0;
        }
        size_type size() const {
            return num_content;
        }

        const_iterator begin() const {
            size_type index = 0;
            size_type htb_size = buck_size();
            iterator result;
            result.htb = const_cast<self*>(this);
            result.p = nullptr;
            for(index = 0; index < htb_size; index++) {
                if(bucket[index] != nullptr) {
                    result.p = bucket[index];
                    break;
                }
            }
            return result;
        }
        const_iterator end() const {
            iterator result;
            result.htb = const_cast<self*>(this);
            result.p = nullptr;
            return result;
        }
        iterator begin() {
            return static_cast<const self*>(this)->begin();
        }
        iterator end() {
            return static_cast<const self*>(this)->end();
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
        void swap(hashtable &other) {
            swap(bucket, other.bucket);
            swap(num_content, other.num_content);
        }

        size_type count(const key_type &k) const {
            size_type index = bk_num_key(k);
            link_node cur = bucket[index];
            size_type result = 0;
            while(cur != nullptr) {
                if(equals(get_key(cur->val), k)) {
                    result++;
                }
                cur = cur->next;
            }
            return result;
        }
        const_iterator find(const key_type &k) const {
            size_type index = bk_num_key(k);
            link_node cur = bucket[index];
            iterator result;
            while(cur != nullptr) {
                if(equals(get_key(cur->val), k)) {
                    result.htb = const_cast<self*>(this);
                    result.p = cur;
                    return result;
                }
                cur = cur->next;
            }
            return end();
        }
        iterator find(const key_type &v) {
            return static_cast<const self*>(this)->find(v);
        }
        void erase(iterator it) {
            if(it.htb != this || it == end())
                return;
            size_type index = bk_num_key(get_key(*it));
            if(index > buck_size())
                return;
            link_node cur = bucket[index];
            if(cur == nullptr)
                return;
            if(it.p == cur) {
                bucket[index] = cur->next;
                deallocate_node(cur);
                num_content--;
            }
            else {
                link_node pre = cur;
                cur = cur->next;
                while(cur != nullptr) {
                    if(cur == it.p) {
                        pre->next = cur->next;
                        deallocate_node(cur);
                        num_content--;
                        return;
                    }
                    pre = cur;
                    cur = cur->next;
                }
            }
        }
        void erase(iterator l, iterator r) {
            if(l->htb != this || r->htb != this) return;

            size_type index = bk_num_val(*l);
            link_node cur, pre;
            if(bucket[index] == l->p) {
                while(bucket[index] != nullptr && bucket[index] != r->p) {
                    cur =  bucket[index];
                    bucket[index] = bucket[index]->next;
                    deallocate_node(cur);
                    num_content--;
                }
                if(r->p != nullptr && bucket[index] == r->p)
                    return;
            }
            else {
                pre = bucket[index];
                cur = bucket[index]->next;
                while(cur != nullptr) {
                    if(cur == l.p) {
                        break;
                    }
                    else {
                        pre = cur;
                        cur = cur->next;
                    }
                }
                while(cur != nullptr) {
                    if(cur == r.p) {
                        return;
                    }
                    pre->next = cur->next;
                    deallocate_node(cur);
                    cur = pre->next;
                    num_content--;
                }
            }
            size_type htb_size = buck_size();
            index++;
            for( ; index < htb_size; ++index) {
                while(bucket[index] != nullptr) {
                    if(bucket[index] == r.p) {
                        return;
                    }
                    else {
                        cur = bucket[index];
                        bucket[index] = bucket[index]->next;
                        deallocate_node(cur);
                        num_content--;
                    }
                }
            }
        }
        void erase(const key_type &v) {
            size_type index = bk_num_key(v);
            link_node cur, pre;
            if(index >= buck_size())
                return;
            while(bucket[index] != nullptr && equals(get_key(bucket[index]->val), v)) {
                cur = bucket[index];
                bucket[index] = bucket[index]->next;
                deallocate_node(cur);
                num_content--;
            }
            if(bucket[index] == nullptr)
                return;
            pre = bucket[index];
            cur = bucket[index]->next;
            while(equals(get_key(pre->val), v)) {
                pre = cur;
                cur = cur->next; 
            }
            while(cur != nullptr) {
                if(equals(get_key(cur->val), v)) {
                    pre->next = cur->next;
                    deallocate_node(cur);
                    cur = pre->next;
                    num_content--;
                }
                else {
                    pre = cur;
                    cur = cur->next;
                }
            }
        }
    };
    #define HASHTB_TYPE template<typename Value, typename Key, typename HashFn, typename ExtractKey, typename EqualKey, typename Alloc>    
    #define HASHTB_ATTR(attr_name) hashtable<Value, Key, HashFn, ExtractKey, EqualKey, Alloc>::attr_name

    HASHTB_TYPE
    void HASHTB_ATTR(rehash) (HASHTB_ATTR(size_type) n) {
        size_type new_sz = hash_lower_bound_prime(n);
        if(new_sz < buck_size())
            return;
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
        bucket.swap(new_buck);
    }

    HASHTB_TYPE
    pair<typename HASHTB_ATTR(link_node), bool> HASHTB_ATTR(__insert_unique) (const HASHTB_ATTR(value_type) &v) {
        size_type index = bk_num_val(v);
        link_node cur = bucket[index];
        while(cur != nullptr) {
            if(equals(get_key(cur->val), get_key(v))) {
                return pair<link_node, bool>(cur, false);
            }
            cur = cur->next;
        }

        num_content++;
        if(num_content > buck_size()) {
            rehash(num_content);
            index = bk_num_val(v); // index may be changed after rehash!
        }
        link_node new_node = allocate_node(v);
        new_node->next = bucket[index];
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
        link_node new_node = allocate_node(v);
        new_node->next = bucket[index];
        bucket[index] = new_node;
        return new_node;
    }

    #undef HASHTB_TYPE
    #undef HASHTB_ATTR
}