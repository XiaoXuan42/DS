#pragma once

#include "../alloc/ds_alloc.h"
#include "../alloc/ds_construct.h"
#include "../alloc/ds_uninitialized.h"
#include "../algorithm/ds_algorithm.h"

namespace xxds {
    template<typename T, typename Alloc=alloc>
    class vector
    {
    public:
        using value_type = T;
        using pointer = value_type*;
        using iterator = value_type*;
        using reference = value_type&;
        using size_type = size_t;
        using difference_type = ptrdiff_t;

        using const_value_type = const T;
        using const_reference = const T&;
        using const_pointer = const T*;
        using const_iterator = const T*;
        using const_size_type = const size_t;
        using const_difference_type = const ptrdiff_t;

        using data_alloc = simple_alloc<T, Alloc>;
        //because iterator is actually a pointer, so we simply use the simple_alloc as data_alloc
    private:
        iterator start;
        iterator finish;
        iterator end_of_storage;
        
        void deallocate() {
            if(start) {
                data_alloc::deallocate(start, end_of_storage - start);
            }
        }
        void insert_aux(iterator position, const_reference x) {
            /*
               insert x at the given position
               if the allocated memory isn't enough, this function will allocate more memory
               NOTE: please make sure that your copy constructor and assignment is equivalence
            */
            if(finish == end_of_storage) {
                const size_type old_sz = capacity();
                const size_type new_sz = old_sz ? old_sz * 2 : 1; //new_sz means how many elements, not how many bytes

                iterator new_start = data_alloc::allocate(new_sz);
                iterator new_finish = new_start;
                try {
                    new_finish = uninitialized_copy(start, position, new_start);
                    construct(new_finish, x);
                    ++new_finish;
                    new_finish = uninitialized_copy(position, finish, new_finish);
                }
                catch(...) {
                    destroy(new_start, new_finish);
                    data_alloc::deallocate(new_start, new_sz);
                    throw;
                }

                destroy(start, finish);
                deallocate();
                start = new_start;
                finish = new_finish;
                end_of_storage = start + new_sz;
            }
            else {
                construct(finish, *(finish - 1)); // create a element (you shouldn't use copy_backward() because this space may not be initialized)
                copy_backward(position, finish - 1, finish); // since the seq overlaps, it's not correct to use copy
                ++finish;
                *position = x;
            }
        }
        void fill_initialize(size_type n, const_reference value) {
            start = allocate_and_fill(n, value);
            finish = start + n;
            end_of_storage = finish;
        }
        iterator allocate_and_fill(size_type n, const_reference value) {
            iterator pos = data_alloc::allocate(n);
            uninitialized_fill_n(pos, n, value);
            return pos;
        }

    public:
        vector(): start(nullptr), finish(nullptr), end_of_storage(nullptr) {}
        vector(size_type n, const_reference value): start(nullptr), finish(nullptr), end_of_storage(nullptr) {
            fill_initialize(n, value);
        }
        vector(int n, const_reference value): start(nullptr), finish(nullptr), end_of_storage(nullptr) {
            fill_initialize((size_type)n, value);
        }
        explicit vector(size_type n): start(nullptr), finish(nullptr), end_of_storage(nullptr) {
            fill_initialize(n, T());
            finish = start;
        }

        ~vector() {
            destroy(start, finish);
            deallocate();
            start = finish = end_of_storage = nullptr;
        }
        vector(const vector &rhs) {
            start = data_alloc::allocate(rhs.capacity());
            end_of_storage = start + rhs.capacity();
            finish = uninitialized_copy(rhs.begin(), rhs.end(), begin());
        }
        vector & operator = (const vector &rhs) {
            if(this == &rhs)    return *this;
            destroy(start, finish);    
            if(rhs.size() >= capacity()) {
                deallocate();
                const size_type old_sz = capacity();
                const size_type new_sz = old_sz + max(old_sz, rhs.capacity()) + 1;
                start = data_alloc::allocate(new_sz);
                end_of_storage = start + new_sz;
                finish = uninitialized_copy(rhs.begin(), rhs.end(), begin());
            }
            else {
                finish = uninitialized_copy(rhs.begin(), rhs.end(), start);
            }
            return *this;
        }

        iterator begin() const { return start; }
        iterator end() const { return finish; }
        size_type size() const { return (size_type)(finish - start); }
        size_type capacity() const {
            return (size_type)(end_of_storage - start);
        }
        bool empty() const {
            return begin() == end();
        }

        void push_back(const T &target) {
            if(finish == end_of_storage) { // 申请的存储空间不足 
                insert_aux(end(), target); //借用insert_aux中的处理存储空间不足的机制
            }
            else {
                construct(finish, target);
                ++finish;
            }
        }
        void pop_back() {
            if(start != finish) {
                --finish;
                destroy(finish);
            }
        }
        reference front() {
            return *begin();
        }
        reference back() {
            return *(end() - 1);
        }
        iterator erase(iterator first, iterator last) {
            iterator i = copy(last, finish, first);
            destroy(i, finish);
            finish = i;
            return first;
        }
        iterator erase(iterator position) {
            if(position + 1 != end()) {
                copy(position + 1, finish, position);
            }
            --finish;
            destroy(finish);
            return position;
        }
        void insert(iterator position, size_type n, const_reference x) {
            //allocate n elements that's equal to x
            if(n != 0) {
                if(size_type(end_of_storage - finish) >= n) {
                    const size_type elem_remain = finish - position;
                    iterator old_finish = finish;
                    if(elem_remain > n) {
                        uninitialized_copy(finish - n, finish, finish);
                        finish = finish + n;
                        copy_backward(position, position + n, old_finish);
                        fill(position, position + n, x);
                    }
                    else {
                        uninitialized_copy(position, finish, finish + n - elem_remain);
                        finish = finish + n;
                        fill(position, old_finish + n - elem_remain, x);
                    }
                }
                else {
                    const size_type old_size = capacity();
                    const size_type len = old_size + max(old_size, n);

                    iterator new_start = data_alloc::allocate(len);
                    iterator new_finish = new_start;
                    try {
                        new_finish = uninitialized_copy(start, position, new_start);
                        new_finish = uninitialized_fill_n(new_finish, n, x);
                        new_finish = uninitialized_copy(position, finish, new_finish);
                    }
                    catch(...) {
                        destroy(new_start, new_finish);
                        data_alloc::deallocate(new_start, n);
                        throw;
                    }
                    destroy(start, finish);
                    deallocate();
                    start = new_start;
                    finish = new_finish;
                    end_of_storage = new_start + len;
                }
            }
        }
        void resize(size_type new_size, const T &x) {
            if(new_size < size()) {
                erase(begin() + new_size, end());
            }
            else {
                insert(end(), new_size - size(), x);
            }
        }
        void resize(size_type new_size) {
            resize(new_size, T());
        }
        void clear() {
            erase(begin(), end());
        }
        reference operator [] (size_type n) {
            return *(begin() + n);
        }
        const_reference operator [] (size_type n) const {
            return *(begin() + n);
        }
        void reserve(const size_type n) {
            if(n <= capacity()) return;
            else {
                iterator new_start = data_alloc::allocate(n);
                iterator new_finish = uninitialized_copy(start, finish, new_start);
                destroy(start, finish);
                deallocate();
                start = new_start;
                finish = new_finish;
                end_of_storage = start + n;
            }
        }
        void swap(vector &rhs) {
            xxds::swap(start, rhs.start);            
            xxds::swap(finish, rhs.finish);
            xxds::swap(end_of_storage, rhs.end_of_storage);
        }
    };
}