#pragma once

#include <cstdio>
#include <cmath>
#include "../iter/iter.h"
#include "../alloc/ds_alloc.h"
#include "../alloc/ds_uninitialized.h"
#include "../alloc/ds_construct.h"
#include "../algorithm/ds_algobase.h"

namespace DS
{
    namespace
    {
        const size_t __deque_default_size_byte = 512;
        inline size_t __deque_segment_size(size_t buff_sz, size_t sz) {
            return buff_sz ? buff_sz : (buff_sz < __deque_default_size_byte ?
                                            size_t(__deque_default_size_byte / sz) : size_t(1));
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

            static difference_type buffer_sz() {
                return static_cast<difference_type>(__deque_segment_size(Buf_sz, sizeof(T)));
            }
            T *cur, *last, *first;
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
                    difference_type node_diff = offset > 0 ? offset / buffer_sz() : (offset + 1) / buffer_sz() - 1;
                    set_node(node + node_diff);
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

            reference operator *() const {
                return *cur;
            }
            pointer operator -> () const {
                return &(operator*());
            }

            reference operator [] (difference_type n) const {
                return *(*this + n);
            }
            bool operator < (const self &rhs) const {
                return (node == rhs.node) ? cur < rhs.cur : node < rhs.node;
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
        using difference_type = ptrdiff_t;

        using iterator = __deque_iterator<T, T&, T*, Buf_sz>;
        using const_iterator = __deque_iterator<const T, const T&, const T*, Buf_sz>;

        using map_pointer = T**;

    private:
        static inline const size_t mini_mapsize = 8;
        map_pointer map;
        iterator start, finish;
        size_type map_size;

        using data_alloc = simple_alloc<value_type, alloc>;
        using map_alloc = simple_alloc<pointer, alloc>;

        static size_type buffer_sz() {
            return static_cast<size_type>(__deque_segment_size(Buf_sz, sizeof(T)));
        }

        pointer allocate_node() {
            /* allocate a node's memeory */
            return data_alloc::allocate(buffer_sz());
        }
        void deallocate_node(pointer to_delete) {
            /*
                the parameter is a pointer to the first element of
                the node to be deleted.
            */
            data_alloc::deallocate(to_delete, buffer_sz());
        }
        void deallocate_all_node() {
            for(map_pointer cur = start.node; cur <= finish.node; ++cur) {
                deallocate_node(*cur);
            }
        }
        void deallocate_map() {
            map_alloc::deallocate(map, map_size);
        }

        void create_map_nodes(size_type n) {
            /* create map structure based on the size n */
            const size_type nodes_num = n / buffer_sz() + 1; // note that we will allocate one more node when __deque_segment_size | n
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
            /* 
                the cur will always point to an address that's in [first, last)
                if the __deque_segment_size | n, then we will allocate one more node
                and then set finish.cur to the additional node's first place
            */
            finish.cur = finish.first + n % buffer_sz();
            //update the map_size
            map_size = map_sz;
        }
        void fill_initialize(size_type n, const value_type &val) {
            /* initialize the map structure and set them to val */
            create_map_nodes(n);
            map_pointer cur = start.node;
            const size_t per_seg_size = buffer_sz();
            while(cur != finish.node) {
                uninitialized_fill_n(*cur, per_seg_size, val);
                ++cur;
            }
            uninitialized_fill(finish.first, finish.cur, val);
        }

        void reallocate_map(size_type nodes_to_add, bool is_front) {
            /*
                this function will only change the map
            */
            size_type old_nodes_num = finish.node - start.node + 1;
            size_type new_nodes_num = old_nodes_num + nodes_to_add;

            map_pointer new_nstart;
            if(map_size > 2 * new_nodes_num) {
                new_nstart = map + (map_size - new_nodes_num) / 2 + (is_front ? nodes_to_add : 0);
                if(new_nstart < start.node) {
                    copy(start.node, finish.node, new_nstart);
                }
                else {
                    copy_backward(start.node, finish.node, new_nstart + old_nodes_num);
                }
            }
            else {
                size_type new_map_size = map_size + max(map_size, nodes_to_add) + 2; // add 2: in case of incomplete segment
                map_pointer new_map = map_alloc::allocate(new_map_size);
                new_nstart = new_map + (new_map_size - new_nodes_num) / 2 + (is_front ? nodes_to_add : 0);
                copy(start.node, finish.node, new_nstart);

                deallocate_map();
                map = new_map;
                map_size = new_map_size;
            }
            start.set_node(new_nstart);
            finish.set_node(new_nstart + old_nodes_num - 1);
        }
        void reverse_map_front(size_type nodes_to_add = 1) {
            if(static_cast<size_type>(start.node - map) < nodes_to_add) {
                reallocate_map(nodes_to_add, true);
            }
        }
        void reverse_map_back(size_type nodes_to_add = 1) {
            if(map_size - (finish.node - map + 1) < nodes_to_add) {
                reallocate_map(nodes_to_add, false);
            }
        }

        iterator insert_aux(iterator pos, const value_type &x) {
            difference_type index = pos - start;
            if(index < size() / 2) {
                push_front(front());
                iterator front1 = start;
                ++front1;
                iterator front2 = front1;
                ++front2;
                pos = start + index;
                iterator pos1 = pos;
                ++pos1;
                copy(front2, pos1, front1);
            }
            else {
                push_back(back());
                iterator back1 = finish;
                --back1;
                iterator back2 = back1;
                --back2;
                pos = start + index;
                copy_backward(pos, back2, back1);
            }
            *pos = x;
            return pos;
        }
        void pop_front_aux() {
            destroy(start.cur);
            deallocate_node(start.first);
            start.set_node(start.node + 1);
            start.cur = start.first;
        }
        void pop_back_aux() {
            deallocate_node(finish.first);    
            finish.set_node(finish.node - 1);
            finish.cur = finish.last - 1;
            destroy(finish.cur);
        }

    public:
        deque(): map(nullptr), map_size(0) {
            create_map_nodes(0); //we can use this safely although it will allocate some memory :)
        }
        deque(size_type n, const value_type &val): map(nullptr), map_size(0) {
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
        size_type size() const {
            return static_cast<size_type>(finish - start);
        }
        bool empty() {
            return start == finish;
        }

        void push_back(const value_type &x) {
            if(finish.cur != finish.last - 1) {
                construct(finish.cur, x);
                ++finish.cur;
            }
            else {
                push_back_aux(x);
            }
        }
        void push_back_aux(const value_type &x) {
            /*
                this function will only be called when the memory left
                on the right of current node can only holds one element
                and we now need to push_back a new element
            */
           reverse_map_back();
           *(finish.node + 1) = allocate_node();
           construct(finish.cur, x);
           finish.set_node(finish.node + 1);
           finish.cur = finish.first;
        }
        void push_front(const value_type &x) {
            if(start.cur == start.first) {
                push_front_aux(x);
            }
            else {
                --start.cur;
                construct(start.cur, x);
            }
        }
        void push_front_aux(const value_type &x) {
            /*
                this function will only be called there is no memory
                left on the left of current node
            */
            reverse_map_front();
            *(start.node - 1) = allocate_node();
            start.set_node(start.node - 1);
            start.cur = start.last - 1;
            construct(start.cur, x);
        }
        void pop_front() {
            if(start.cur != start.last - 1) {
                destroy(start.cur);
                ++start.cur;
            }
            else {
                pop_front_aux();
            }        
        }
        void pop_back() {
            if(finish.cur != finish.first) {
                --finish.cur;
                destroy(finish.cur);
            }
            else {
                pop_back_aux();
            }
        }
        void clear() {
            const size_t per_seg_sz = buffer_sz();
            for(map_pointer cur = start.node + 1; cur < finish.node; ++cur) {
                destroy(*cur, (*cur) + buffer_sz());
                data_alloc::deallocate(*cur, per_seg_sz);
            }
            if(start.node != finish.node) {
                destroy(start.first, start.last);
                destroy(finish.first, finish.last);
                data_alloc::deallocate(finish.first, per_seg_sz); // only deallocate the finish's node
            }
            else {
                destroy(start.first, start.last);
            }
            finish = start;
        }
        iterator erase(iterator pos) {
            iterator next = pos;
            ++next;
            difference_type index = pos - start;
            if(index < (size() >> 1)) {
                // if the elements before the current position
                // is less than the elements after the current position
                // then move the left part right
                copy_backward(start, pos, next);
                pop_front();
            }
            else {
                copy(next, finish, pos);
                pop_back();
            }
            return start + index;
        }
        iterator erase(iterator first, iterator last) {
            const size_t per_seg_sz = buffer_sz();
            if(start == first && last == finish) {
                clear();
                return finish;
            }
            else {
                const difference_type l_left = first - start, r_left = finish - last;
                if(l_left < r_left) {
                    iterator new_start = copy_backward(start, first, last);
                    destroy(start, new_start);
                    for(map_pointer cur = start.node; cur < new_start.node; ++cur) {
                        data_alloc::deallocate(*cur, per_seg_sz);
                    }
                    start = new_start;
                }
                else {
                    iterator new_finish = copy(last, finish, first);
                    destroy(new_finish, finish);
                    for(map_pointer cur = finish.node - 1; cur > new_finish.node; --cur) {
                        deallocate(*cur);
                    }
                    finish = new_finish;
                }
                return start + l_left;
            }
        }
        iterator insert(iterator position, const value_type &x) {
            if(position.cur == start.cur) {
                push_front(x);
                return start;
            }
            else if(position.cur == finish.cur) {
                push_back(x);
                iterator tmp = finish;
                tmp--;
                return tmp;
            }
            else {
                return insert_aux(position, x);
            }
        }
        reference operator [] (int idx) {
            return start[idx];
        }
    };
}