#pragma once

#include <cstring>
#include "../traits/type_traits.h"
#include "../iter/iter.h"

namespace xxds
{
    template<typename InputIterator1, typename InputIterator2>
    inline bool equal(InputIterator1 first1, InputIterator1 second1, InputIterator2 first2) {
        for( ; first1 != second1; ++first1, ++first2) {
            if(*first1 != *first2) {
                return false;
            }
        }
        return true;
    }
    template<typename InputIterator1, typename InputIterator2, typename BinaryOperation>
    inline bool equal(InputIterator1 first1, InputIterator1 second1, InputIterator2 first2, BinaryOperation op) {
        for( ; first1 != second1; ++first1, ++first2) {
            if(!op(*first1, *first2)) {
                return false;
            }
        }
        return true;
    }

    template<typename OutputIterator, typename T>
    OutputIterator fill(OutputIterator first, OutputIterator last, const T &val) {
        for( ; first != last; ++first) {
            *first = val;
        }
        return first;
    }
    template<typename OutputIterator, typename Size, typename T>
    OutputIterator fill_n(OutputIterator first, Size n, const T &val) {
        for( ; n > 0; --n, ++first) {
            *first = val;
        }
        return first;
    }

    template<typename ForwardIterator1, typename ForwardIterator2, typename T>
    inline void __iter_swap(ForwardIterator1 a, ForwardIterator2 b, T*) {
        T tmp = *a;
        *a = *b;
        *b = tmp; 
    }
    template<typename ForwardIterator1, typename ForwardIterator2>
    inline void iter_swap(ForwardIterator1 a, ForwardIterator2 b) {
        __iter_swap(a, b, value_type(a));
    }

    template<typename InputIterator1, typename InputIterator2>
    bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2) {
        //默认使用<和==来判定大小
        while(first1 != last1 && first2 != last2) {
            if(*first1 < *first2) {
                return true;
            }
            else if(*first1 == *first2) {
                ++first1;
                ++first2;
            }
            else {
                return false;
            }
        }
        if(first1 == last1) return true;
        else return false;
    }
    template<typename InputIterator1, typename InputIterator2, typename Compare>
    bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, Compare cmp) {
        while(first1 != last1 && first2 != last2) {
            if(cmp(*first1, *first2)) {
                return true;
            }
            else if(*first1 == *first2) {
                ++first1;
                ++first2;
                continue;
            }
            else {
                return false;
            }
        }
        if(first1 == last1) return true;
        else return false;
    }

    template<typename T>
    inline const T& max(const T& a, const T& b) {
        return a < b ? b : a;
    }
    template<typename T, typename Compare>
    inline const T& max(const T& a, const T& b, Compare cmp) {
        return cmp(a, b) ? b : a;
    }
    template<typename T>
    inline const T& min(const T& a, const T& b) {
        return a < b ? a : b;
    }
    template<typename T, typename Compare>
    inline const T& min(const T& a, const T& b, Compare cmp) {
        return cmp(a, b) ? a : b;
    }
    template<typename T>
    inline void swap(T &a, T& b) {
        T tmp = a;
        a = b;
        b = tmp;
    }

    /* copy */
    template<typename RandomAccessIterator, typename OutputIterator, typename Distance>
    OutputIterator __copy_d(RandomAccessIterator first, RandomAccessIterator last, OutputIterator result, Distance *) {
        for(Distance n = last - first; n > 0; --n, ++result, ++first) {
            *result = *first;
        }
        return result;
    }
    template<typename T>
    T* __copy_t(const T* first, const T* last, T* result, ds_false_type) {
        return __copy_d(first, last, result, difference_type(first));
    }
    template<typename T>
    T* __copy_t(const T* first, const T* last, T* result, ds_true_type) {
        memmove(result, first, sizeof(T) * (last - first));
        return result + (last - first);
    }
    template<typename InputIterator, typename OutputIterator>
    OutputIterator __copy(InputIterator first, InputIterator last, OutputIterator result, input_iterator_tag) {
        for( ; first != last; ++first, ++result) {
            *result = *first;
        }
        return result;
    }
    template<typename RandomAccessIterator, typename OutputIterator>
    OutputIterator __copy(RandomAccessIterator first, RandomAccessIterator last, OutputIterator result, random_iterator_tag) {
        return __copy_d(first, last, result, difference_type(first));
    }
    template<typename InputIterator, typename OutputIterator>
    struct __copy_dispatch
    {
        OutputIterator operator() (InputIterator first, InputIterator last, OutputIterator result) {
            return __copy(first, last, result, iterator_category(first));
        }
    };
    template<typename T>
    struct __copy_dispatch<T*, T*>
    {
        T * operator()(T *first, T *last, T *result) {
            typedef typename ds_type_traits<T>::has_trivial_assignment_operator t;
            return __copy_t(first, last, result, t());
        }
    };
    template<typename T>
    struct __copy_dispatch<const T*, T*>
    {
        T *operator()(const T* first, const T* last, T *result) {
            typedef typename ds_type_traits<T>::has_trivial_assignment_operator t;
            return __copy_t(first, last, result, t());
        }
    };
    template<typename InputIterator, typename OutputIterator>
    inline OutputIterator copy(InputIterator first, InputIterator last, OutputIterator result) {
        return __copy_dispatch<InputIterator, OutputIterator>()(first, last, result);
    }
    template<>
    inline char *copy(const char * first, const char * last, char *result) {
        memmove(result, first, sizeof(char) * (last-first));
        return result + (last - first);
    }
    template<>
    inline wchar_t *copy(const wchar_t * first, const wchar_t * last, wchar_t *result) {
        memmove(result, first, sizeof(wchar_t) * (last - first));
        return result + (last - first);
    }
    /* end_copy */

    /* copy_backward */
    template<typename RandomAccessIterator, typename BidirectionalIterator2, typename Distance>
    BidirectionalIterator2 __copy_backward_d(RandomAccessIterator first, RandomAccessIterator last, BidirectionalIterator2 result, Distance *) {
        Distance n = last - first;
        while(n > 0) {
            --result;
            --last;
            *result = *last;            
            --n;
        }
        return result;
    }
    template<typename T>
    T* __copy_backward_t(const T* first, const T* last, T* result, ds_false_type) {
        return __copy_backward_d(first, last, result, difference_type(first));
    }
    template<typename T>
    T* __copy_backward_t(const T* first, const T* last, T* result, ds_true_type) {
        memmove(result - (last - first), first, sizeof(T) * (last - first));
        return result - (last - first);
    }
    template<typename BidirectionalIterator1, typename BidirectionalIterator2>
    BidirectionalIterator2 __copy_backward(BidirectionalIterator1 first, BidirectionalIterator1 last, BidirectionalIterator2 result, input_iterator_tag) {
        while(last != first) {
            --last;
            --result;
            *result = *last;
        }
        return result;
    }
    template<typename RandomAccessIterator, typename BidirectionalIterator2>
    BidirectionalIterator2 __copy_backward(RandomAccessIterator first, RandomAccessIterator last, BidirectionalIterator2 result, random_iterator_tag) {
        return __copy_backward_d(first, last, result, difference_type(first));
    }
    template<typename BidirectionalIterator1, typename BidirectionalIterator2>
    struct __copy_backward_dispatch
    {
        BidirectionalIterator2 operator() (BidirectionalIterator1 first, BidirectionalIterator1 last, BidirectionalIterator2 result) {
            return __copy_backward(first, last, result, iterator_category(first));
        }
    };
    template<typename T>
    struct __copy_backward_dispatch<T*, T*>
    {
        T * operator()(T *first, T *last, T *result) {
            typedef typename ds_type_traits<T>::has_trivial_assignment_operator t;
            return __copy_backward_t(first, last, result, t());
        }
    };
    template<typename T>
    struct __copy_backward_dispatch<const T*, T*>
    {
        T *operator()(const T* first, const T* last, T *result) {
            typedef typename ds_type_traits<T>::has_trivial_assignment_operator t;
            return __copy_backward_t(first, last, result, t());
        }
    };
    template<typename BidirectionalIterator1, typename BidirectionalIterator2>
    inline BidirectionalIterator2 copy_backward(BidirectionalIterator1 first, BidirectionalIterator1 last, BidirectionalIterator2 result) {
        return __copy_backward_dispatch<BidirectionalIterator1, BidirectionalIterator2>()(first, last, result);
    }
    template<>
    inline char *copy_backward(const char * first, const char * last, char *result) {
        memmove(result - (last - first), first, sizeof(char) * (last-first));
        return result - (last - first);
    }
    template<>
    inline wchar_t *copy_backward(const wchar_t * first, const wchar_t * last, wchar_t *result) {
        memmove(result - (last - first), first, sizeof(wchar_t) * (last - first));
        return result - (last - first);
    }
    /* end copy_backward_back */
}