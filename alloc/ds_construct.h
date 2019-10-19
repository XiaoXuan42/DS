#pragma once

#include <new>
#include "../iter/iter.h"
#include "../traits/type_traits.h"

namespace DS {

    template<typename T1, typename T2>
    inline void construct(T1 *p, const T2 &value) {
        new(p) T1(value);
    }

    template<typename T>
    inline void destroy(T *p) {
        p->~T();
    }

    template<typename ForwardIterator, typename T>
    inline void __destroy(ForwardIterator begin, ForwardIterator end, T* val) {
        typedef typename ds_type_traits<T>::has_trivial_destructor trivial_destructor;
        __destroy_aux(begin, end, trivial_destructor());
    }
    template<typename ForwardIterator>
    inline void __destroy_aux(ForwardIterator begin, ForwardIterator end, ds_true_type) {}
    template<typename ForwardIterator>
    inline void __destroy_aux(ForwardIterator begin, ForwardIterator end, ds_false_type) {
        for(; begin < end; ++begin) {
            destroy(&(*begin));
        }
    }
    template<typename ForwardIterator>
    inline void destroy(ForwardIterator begin, ForwardIterator end) {
        __destroy(begin, end, value_type(begin));
    }
    template<>
    inline void destroy(char *, char *) {}
    template<>
    inline void destroy(wchar_t *, wchar_t *) {}
}