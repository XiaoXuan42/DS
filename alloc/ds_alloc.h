#pragma once
// use malloc to practice coding set_new_handler()

#include <cstdlib>
#include <cstring>
#include <new>
#include <iostream>

namespace xxds {
    template<int inst>
    class malloc_alloc_template {
        static void * oom_malloc(const size_t);
        static void * oom_realloc(void *, const size_t);
        //oom: out of memory 溢出处理
        static std::new_handler oom_handler; // new_handler : typedef void *(new_handler)()

    public:
        static void * allocate(const size_t n) {
            void *result = malloc(n);
            if(result == nullptr) {
                return oom_malloc(n);
            }
            return result;
        }
        static void * reallocate(void *p, const size_t old_n, const size_t new_n) {
            void *result = malloc(new_n);
            if(result == nullptr) {
                return oom_realloc(p, new_n);
            }
            return result;
        }
        static void deallocate(void *p, const size_t n) {
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
    void * malloc_alloc_template<inst>::oom_malloc(const size_t n) {
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
    void *malloc_alloc_template<inst>::oom_realloc(void *p, const size_t new_n) {
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

    typedef malloc_alloc_template<0> malloc_alloc;

    template<int inst>
    class pool_alloc_template
    {
        const static int _align = 8; //_align should be a multiple of the size of a pointer, _align 应该是一个指针长度的整数倍,至少不要小于一个指针的长度
        const static int _max_bytes = 128;
        constexpr static int _max_list_size = _max_bytes / _align;

        static size_t round_up(const size_t n) {
            return n ? (n + _align - 1) & ~(_align - 1) : _align; // when n is zero, return it _align特殊情况:n为0的时候直接返回_align
        }
        static size_t freelist_index(const size_t n) {
            //找到比n大的并且能够被_align整除的最小的整数对应在free_list中的下标,find the index that can accommodate the size n
            if(n == 0)  return 0; // if n == 0, then n - 1 will lead to disasters 特殊情况:如果n是0,那么n-1就会变成一个极大的数
            else
                return (n + _align - 1) / _align - 1;
        }
        union obj {
            union obj *p_list;  //use union to conserve memory
            char client;
        }; //the real size of obj is the size of a pointer

        static char *start_free, *end_free;
        static size_t heapsize;
        static obj * freelist[_max_list_size];
        static void *refill(const size_t n);
        static char *chunk_alloc(const size_t n, int &nobjs);
    public:
        static void *allocate(const size_t n);
        static void deallocate(void *p, const size_t n);
        static void *reallocate(void *p, const size_t old_n, const size_t new_n);
    };
    template<int inst>
    char * pool_alloc_template<inst>::start_free = nullptr; 
    template<int inst>
    char * pool_alloc_template<inst>::end_free = nullptr;
    template<int inst>
    size_t pool_alloc_template<inst>::heapsize = 0;
    template<int inst>
    typename pool_alloc_template<inst>::obj * pool_alloc_template<inst>::freelist[pool_alloc_template<inst>::_max_list_size] = {nullptr};

    template<int inst>
    void *pool_alloc_template<inst>::allocate(const size_t n) {
        if(n > _max_bytes) {
            return malloc_alloc::allocate(n);
        }
        // if n == 0 then allocate 8 byte to it
        size_t index = freelist_index(n);
        obj * target = freelist[index];

        if(target == nullptr) {
            void *r = refill(n);
            return r;
        }
        freelist[index] = target->p_list;
        return target;
    }
    template<int inst>
    void *pool_alloc_template<inst>::reallocate(void *p, const size_t old_size, const size_t new_size) {
        if(old_size > _max_bytes) {
            // if old_size > _max_bytes means it was originally allocated by malloc_alloc
            // so we use malloc_alloc::reallocate to reallocate it
            return malloc_alloc::reallocate(p, old_size, new_size);
        }
        else {
            // p point to a node
            // be aware that it may affect more memory than old_size
            // allocate, copy and deallocate
            size_t to_copy = old_size < new_size ? old_size : new_size;
            void *result = allocate(new_size);
            memcpy(result, p, to_copy);
            deallocate(p, old_size);
            return result;
        }
    }
    template<int inst>
    void pool_alloc_template<inst>::deallocate(void *p, const size_t n) {
        //you should always use the same alloc's allocate and deallocate.
        if(n == 0 || p == nullptr) {
            return;
            //deallocate 0 byte makes no sense
        }
        if(n > _max_bytes) {
            malloc_alloc::deallocate(p, n);
        }
        else {
            obj *q = (obj *)p;
            size_t index = freelist_index(n);
            q->p_list = freelist[index];       
            freelist[index] = q;
        }
    }
    template<int inst>
    void *pool_alloc_template<inst>::refill(const size_t n) {
        int nobjs = 20;
        size_t modified_n = round_up(n);
        //modified_n is the minimum number which is a multiple of _align
        char *chunk = chunk_alloc(modified_n, nobjs);
        if(nobjs == 1) {
            return chunk;
        }
        else {
            obj *result = (obj*)chunk; //allocate the first node to be the result, the other add to the freelist
            obj *first_obj = (obj*)(chunk + modified_n);
            obj *current_obj = first_obj;
            for(int i = 2; i < nobjs; i++) {
                current_obj->p_list = (obj *)((char *)current_obj + modified_n); //modified_n must be greater than the size of a pointer
                //otherwise assignment of a obj's p_list will implicitly change other objs' value.
                //thus the _align must be greater than the size of a pointer
                current_obj = current_obj->p_list;
            }
            size_t index = freelist_index(modified_n);
            current_obj->p_list = freelist[index]; //link the end of the new list to the head of the old list
            freelist[index] = first_obj;
            return result;
        }
    }
    template<int inst>
    char *pool_alloc_template<inst>::chunk_alloc(const size_t n, int &nobjs) {
        //n should be a multiple of _align and greater than zero
        //make sure that the heapsize(in freelist and (start_free, end_free) and client) is a multiple of _align
        //the size of (start_free, end_free) is also a multiple of n
        char * result;
        size_t total_bytes = n * nobjs;
        size_t bytes_left = end_free - start_free;
        if(bytes_left > total_bytes) {
            //heap has enough space to allocate nobjs node
            result = start_free;
            start_free += total_bytes;
            return result;
        }
        else if(bytes_left >= n) {
            //heap has not enough space to allocate nobjs node but enough to allocate one
            nobjs = bytes_left / n;
            total_bytes = n * nobjs;
            result = start_free;
            start_free += total_bytes;
            return result;
        }
        else {
            //heap is too small to allocate any node
            size_t bytes_to_get = 2 * total_bytes + round_up(heapsize >> 4); //allocate more and more space
            if(bytes_left > 0) {
                //notice: if bytes_left > 0 then bytes_left should be a multiple of _align
                size_t index = freelist_index(bytes_left);
                ((obj*)start_free)->p_list = freelist[index];
                freelist[index] = (obj*)start_free;
            }

            start_free = (char *)malloc(bytes_to_get);
            if(start_free == nullptr) {
                // malloc failed
                // we can recyle the spare memory in freelist
                obj * free_node = nullptr;
                for(size_t i = n; i <= _max_bytes; i += _align) {
                    //only scan the node that is greater than n
                    //we don't utilize small nodes because of multi-process issues
                    size_t index = freelist_index(i);
                    if(freelist[index] != nullptr) {
                        free_node = freelist[index];
                        freelist[index] = freelist[index]->p_list;
                        start_free = (char *)free_node;
                        end_free = start_free + round_up(i);
                        return chunk_alloc(n, nobjs); // recursive call to adjust the nobjs
                    }
                }
                // no enough memory in freelist
                // take advantage of the malloc_alloc's oom mechanisim
                end_free = nullptr;
                start_free = (char *)malloc_alloc::allocate(bytes_to_get);
            }
            heapsize += bytes_to_get;
            end_free = start_free + bytes_to_get;
            return chunk_alloc(n, nobjs); // to adjust the nobjs
        }
    }

    typedef pool_alloc_template<0> pool_alloc;

    #ifdef DS_USE_MALLOC
        typedef malloc_alloc alloc;
    #else
        typedef pool_alloc alloc;
    #endif
    template<typename T, typename Alloc>
    class simple_alloc 
    {
    public:
        static T *allocate(size_t n) {
            return n == 0 ? 0 : (T*)Alloc::allocate(n * sizeof(T));
        }
        static T *allocate(void) {
            /*
                allocate memory enough to hold one element 
            */
            return (T*)Alloc::allocate(sizeof(T));
        }
        static void deallocate(T *p, size_t n) {
            if(n != 0) {
                return Alloc::deallocate(p, n * sizeof(T));
            }
        }
        static void deallocate(T *p) {
            Alloc::deallocate(p, sizeof(T));
        }
    };
}
