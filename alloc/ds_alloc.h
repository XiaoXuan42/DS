#pragma once
// use malloc to practice coding set_new_handler()

#include <cstdlib>
#include <new>
#include <iostream>

namespace DS {
    template<int inst>
    class malloc_alloc_template {
        static void * oom_malloc(size_t);
        static void * oom_realloc(void *, size_t);
        //oom: out of memory
        static std::new_handler oom_handler; // new_handler : typedef void *(new_handler)()

    public:
        static void * allocate(size_t n) {
            void *result = malloc(n);
            if(result == nullptr) {
                return oom_malloc(n);
            }
            return result;
        }
        static void * reallocate(void *p, size_t old_n, size_t new_n) {
            void *result = malloc(new_n);
            if(result == nullptr) {
                return oom_realloc(p, new_n);
            }
            return result;
        }
        static void deallocate(void *p, size_t n) {
            free(p);
        }

        static std::new_handler set_malloc_handler(std::new_handler nd);
    };

    template<int inst>
    std::new_handler malloc_alloc_template<inst>::oom_handler = nullptr;

    template<int inst>
    std::new_handler malloc_alloc_template<inst>::set_malloc_handler(std::new_handler nd) {
        std::new_handler old_handler = oom_handler;
        oom_handler = nd;
        return old_handler;
    }
    template<int inst>
    void * malloc_alloc_template<inst>::oom_malloc(size_t n) {
        void *result = nullptr;
        std::new_handler cur_handler = nullptr;
        while(true) {
            cur_handler = oom_handler;
            if(cur_handler == nullptr) {
                throw std::bad_alloc();
            }
            (*cur_handler)();
            result = malloc(n);
            if(result != nullptr) {
                return result;
            }
        }
    }
    template<int inst>
    void *malloc_alloc_template<inst>::oom_realloc(void *p, size_t new_n) {
        void *result = nullptr;
        std::new_handler cur_handler = nullptr;
        while(true) {
            cur_handler = oom_handler;
            if(cur_handler == nullptr) {
                throw std::bad_alloc();
            }
            (*cur_handler)();
            result = realloc(p, new_n);
            if(result != nullptr) {
                return result;
            }
        }
    }

    typedef malloc_alloc_template<0> alloc;
}