#pragma once

#include <cstdio>
#include <cmath>
#include "../iter/iter.h"
#include "../alloc/ds_alloc.h"
#include "../alloc/ds_uninitialized.h"

namespace DS
{
    namespace
    {
        const size_t __deque_default_size = 512;
        inline size_t __deque_segment_size(size_t buff_sz) {
            return buff_sz == 0 ? __deque_default_size : buff_sz;
        }
        template<typename T, typename Ref, typename Ptr, size_t Buf_sz>
        struct __deque_iterator
        {
            using value_type = T;
            using reference = Ref;
            using pointer = Ptr;
            using difference_type = ptrdiff_t;
            using iterator_category = random_iterator_tag;

            using self = __deque_iterator<T, Ref, Ptr, Buf_sz>;

            using map_pointer = T**;

            static difference_type buffer_sz() const {
                return static_cast<difference_type>(__deque_segment_size(Buf_sz));
            }
            T *cur, last, first;
            map_pointer node;

            void set_node(map_pointer new_node) {
                node = new_node;
                first = *new_node;
                last = first + buffer_sz();
            }
            
            bool operator == (const self &rhs) const {
                return cur == rhs.cur;
            }
            bool operator != (const self &rhs) const {
                return !(*this == rhs);
            }
            self & operator ++ () {
                ++cur;
                if(cur == last) {
                    set_node(node+1);
                    cur = first;
                }
                return *this;
            }
            self & operator -- () {
                if(cur == first) {
                    set_node(node-1);
                    cur = last - 1;
                }
                else {
                    --cur;
                }
                return *this;
            }
            self operator ++(int) { 
                self tmp = *this;
                ++(*this);
                return tmp;
            }
            self operator --(int) {
                self tmp = *this;
                --(*this);
                return tmp;
            }
            self & operator += (difference_type diff) {
                difference_type offset = (cur - first) + diff;
                if(offset >= 0 && offset < buffer_sz()) {
                    cur += diff;
                }
                else {
                    difference_type node_diff = offset > 0 ? offset / buffer_sz() : (offset + 1) / buffer_sz()) - 1;
                    set_node(node_diff);
                    cur = first + (offset - node_diff * buffer_sz());
                }
                return *this;
            }
            self & operator -= (difference_type diff) {
                return this->operator+=(-diff);
            }
            self operator + (difference_type diff) const {
                self tmp = *this;
                return tmp += diff;
            }
            self operator - (difference_type diff) const {
                self tmp = *this;
                return tmp -= diff;
            } // use op= instead of stand-alone op
            difference_type operator - (const self &rhs) const {
                return (node - rhs.node) * buffer_sz() + (cur - first) - (rhs.cur - rhs.first);
            }

            T operator *() const {
                return *cur;
            }
            pointer oeprator -> () const {
                return &(operator*());
            }

            reference operator [] (difference_type n) const {
                return *(*this + n);
            }
            bool operator < (const self &rhs) const {
                return (node == rhs.node) ? cur < rhs.cur : node < x.node;
            }
        };
    }
    template<typename T, typename Alloc = alloc, size_t Buf_sz=0>
    class deque
    {
    public:
        using value_type = T;
        using reference = T&;
        using pointer = T*;
        using size_type = size_t;

        using iterator = __deque_iterator<T, T&, T*, Buf_sz>;
        using const_iterator = __deque_iterator<const T, const T&, const T*, Buf_sz>;

        using map_pointer = T**;

    private:
        static const size_t mini_mapsize = 8;
        map_pointer map;
        iterator start, finish;
        size_type map_size;

        using data_alloc = simple_alloc<value_type, alloc>;
        using map_alloc = simple_alloc<pointer, alloc>;

        pointer allocate_node() {
            /* allocate a node's memeory */
            return data_alloc::allocate(__deque_segment_size(Buf_sz));
        }

        void create_map_nodes(size_type n) {
            /* create map structure based on the size n */
            const size_t per_seg_size = __deque_segment_size(Buf_sz);
            const size_type nodes_num = (size_type)(ceil((double)n / double(per_seg_size)));
            const size_type map_sz = max(mini_mapsize, nodes_num + 2);
            map = map_alloc::allocate(nodes_num);
            map_pointer nstart = map + (map_sz - nodes_num) / 2;
            map_pointer nfinish = nstart + nodes_num - 1; // map_nodes between nstart and nfinish are allocated
            for(map_pointer cur_node = nstart; cur_node <= nfinish; ++cur_node) {
                *cur_node = allocate_node();
            }
            start.set_node(nstart);
            finish.set_node(nfinish);

            start.cur = start.first;
            finish.cur = finish.first + ((nodes_num + per_seg_size - 1) % per_seg_size) + 1;
        }
        void fill_initialize(size_type n, const value_type &val) {
            /* initialize the map structure and set them to val */
            create_map_nodes(n);
            map_pointer cur = start.node;
            const size_t per_seg_size = __deque_segment_size(Buf_sz);
            while(cur != finish.node) {
                uninitialized_fill_n(*cur, per_seg_size, val);
                ++cur;
            }
            uninitialized_fill(finish.first, finish.cur, val);
        }

    public:
        deque(size_type n, const value_type &val): map(nullptr), start(nullptr), finish(nullptr), map_size(0) {
            fill_initialize(n, val);
        }
        iterator begin() { return start; }
        iterator end() { return finish; }
        
        reference front() { return *start; }
        reference back() {
            iterator tmp = finish;
            --tmp;
            return *tmp;
        }
        size_type size() const {}
        bool empty() {
            return start == finish;
        }
    };
}