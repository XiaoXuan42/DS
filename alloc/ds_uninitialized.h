#pragma once

#include "../traits/type_traits.h"

namespace DS {
    template<typename InputIterator, typename ForwardIterator>
    inline ForwardIterator unintialized_copy(InputIterator first, InputIterator last, ForwardIterator result) {
        return __unintialized_copy(first, last, result, value_type(result));
    }
    template<typename InputIterator, typename ForwardIterator, typename T>
    inline ForwardIterator __unintialized_copy(InputIterator first ,InputIterator last, ForwardIterator result, T * val) {
        typedef typename ds_type_traits<T>::is_POD_type is_POD;
        return __unintialized_copy_aux(first, last, result, is_POD());
    }
    template<typename InputIterator, typename ForwardIterator>
    inline ForwardIterator __unintialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, ds_false_type ft) {
        ForwardIterator cur = result;
        for( ; first != last; ++first, ++cur) {
            construct(&(*cur), *first);
        }
        return cur;
    }
    template<typename InputIterator, typename ForwardIterator>
    inline ForwardIterator __unintialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, ds_true_type tt) {
        return __unintialized_copy_aux(first, last, result, ds_false_type ft); // TODO: use copy() to replace this
    }
    template<>
    inline char * unintialized_copy<const char *, char *>(const char *first, const char *last, char *result) {
        memmove((void*)result, (void*)first, last - first);
        return result + (last - first);
    }
    template<>
    inline wchar_t * unintialized_copy<const wchar_t *, wchar_t *>(const wchar_t * first, const wchar_t *last, wchar_t *result) {
        memmove((void*)result, first, last - first);
        return result + (last - first);
    }

    template<typename ForwardIterator, typename Size, typename T>
    inline ForwardIterator unintialized_fill_n(ForwardIterator first, Size n, const T &x) {
        return __unintialized_fill(first, n, value_type(x)); 
    }
    template<typename ForwardIterator, typename Size, typename T>
    inline ForwardIterator __unintialized_fill_n(ForwardIterator first, Size n, const T &x, T *val) {
        typedef typename ds_type_traits<T>::is_POD_type is_POD;
        return __unintialized_fill_n_aux(first, n, val, is_POD());
    }
    template<typename ForwardIterator, typename Size, typename T>
    inline ForwardIterator __unintialized_fill_n_aux(ForwardIterator first, Size n, const T &x, ds_false_type ft) {
        ForwardIterator cur = first;
        for( ; n > 0; --n, ++cur) {
            construct(&(*cur), x);
        } 
        return cur;
    } 
    template<typename ForwardIterator, typename Size, typename T>
    inline ForwardIterator __unintialized_fill_n_aux(ForwardIterator first, Size n, const T &x, ds_true_type tt) {
        return __unintialized_fill_n_aux(first, n, x, ds_false_type()); //TODO: replace this with fill_n()
    } 
    template<typename ForwardIterator, typename T>
    inline ForwardIterator unintialized_fill(ForwardIterator first, ForwardIterator last, const T &x) {
        return __uninitialized_fill(first, last, x, value_type(x));
    }
    template<typename ForwardIterator, typename T>
    inline ForwardIterator __uninitialized_fill(ForwardIterator first, ForwardIterator last, const T &x, T * val) {
        typedef typename ds_type_traits<T>::is_POD_type is_POD;
        return __uninitialized_fill_aux(first, last, x, is_POD);
    }
    template<typename ForwardIterator, typename T>
    inline ForwardIterator __uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T &x, ds_false_type ft) {
        ForwardIterator cur = first;
        for(; cur != last; ++cur) {
            construct(&(*cur), x);
        }
        return cur;
    }
    template<typename InputIterator, typename ForwardIterator, typename T>
    inline ForwardIterator __uninitialized_fill_aux(InputIterator first, InputIterator last, const T &x, ds_true_type tt) {
        __uninitialized_fill_aux(first, last, x, ds_false_type()); //TODO: replace this with fill()
    }

}