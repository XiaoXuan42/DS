#pragma once

#include "../alloc/ds_alloc.h"
#include "../alloc/ds_uninitialized.h"

namespace DS {
    template<typename T, typename Alloc=alloc>
    class vector
    {
    public:
        typedef T value_type;
        typedef value_type* pointer;
        typedef value_type* iterator;
        typedef value_type& reference;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;

        typedef simple_alloc<T, Alloc> data_alloc;
    private:
        iterator start;
        iterator finish;
        iterator end_of_storage;
        
        void deallocate() {
            if(start) {
                data_alloc::deallocate(start, end_of_storage - start)
            }
        }
        void insert_aux(iterator position, const reference x) {
            if(finish == end_of_storage) {
            }
            else {
            }
        }
        void fill_initialize(size_type n, const reference value) {
            start = allocate_and_fill(n, value);
            finish = start + n;
            end_of_storage = finish;
        }
        iterator allocate_and_fill(size_type n, const reference value) {
            iterator pos = data_alloc::allocate(n);
            unintialized_fill_n(pos, n, value);
            return pos;
        }

    public:
        vector(): start(nullptr), finish(nullptr), end_of_storage(nullptr) {}
        vector(size_type n, const reference value) {
            fill_initialize(n, value);
        }
        vector(int n, const reference value) {
            fill_initialize((size_type)n, value);
        }
        explicit vector(size_type n) {
            fill_initialize(n, T());
        }

        ~vector() {
            destroy(start, finish);
            deallocate();
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
            }
        }
        void pop_back() {
            if(start != finish) {
                --finish;
                destroy(finish);
            }
        }
    };
}