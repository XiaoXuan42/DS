#pragma once
#include <cstddef>
#include <cstdlib>

namespace xxds {
    struct input_iterator_tag {};
    struct output_iterator_tag {};
    struct forward_iterator_tag : public input_iterator_tag {};
    struct bidirectional_iterator_tag : public forward_iterator_tag {};
    struct random_iterator_tag : public bidirectional_iterator_tag {};
 
    template<typename Category, typename T, typename Distance = ptrdiff_t,
                    typename Reference = T&, typename Pointer = T*>
    struct iterator {
        typedef Category    iterator_category;
        typedef T   value_type;
        typedef Distance    difference_type;
        typedef Pointer pointer;
        typedef Reference  reference;
    };
    template <typename Iterator>
    struct iterator_traits {
        typedef typename Iterator::iterator_category iterator_category;
        typedef typename Iterator::value_type   value_type;
        typedef typename Iterator::difference_type  difference_type;
        typedef typename Iterator::pointer  pointer;
        typedef typename Iterator::reference    reference;
    };
    template <typename T>
    struct iterator_traits<T*> {
        typedef random_iterator_tag iterator_category;
        typedef T value_type;
        typedef ptrdiff_t difference_type;
        typedef T* pointer;
        typedef T& reference;
    };
    template <typename T>
    struct iterator_traits<const T*> {
        typedef random_iterator_tag iterator_category;
        typedef T value_type;
        typedef ptrdiff_t difference_type;
        typedef const T* pointer;
        typedef const T& reference;
    };

    template<typename Iterator>
    inline typename iterator_traits<Iterator>::iterator_category iterator_category(const Iterator &) {
        typedef typename iterator_traits<Iterator>::iterator_category category;
        return category();
    }
    template<typename Iterator>
    inline typename iterator_traits<Iterator>::value_type * value_type(const Iterator &) {
        return static_cast<typename iterator_traits<Iterator>::value_type *>(nullptr);
    }
    template<typename Iterator>
    inline typename iterator_traits<Iterator>::difference_type * difference_type(const Iterator &) {
        return static_cast<typename iterator_traits<Iterator>::difference_type *>(nullptr);
    }
    template<typename Iterator>
    inline typename iterator_traits<Iterator>::difference_type __distance(Iterator first, Iterator last, input_iterator_tag) {
        typename iterator_traits<Iterator>::difference_type n = 0;
        while(first != last) {
            ++first;
            ++n;
        }
        return n;
    }
    template<typename Iterator>
    inline typename iterator_traits<Iterator>::difference_type __distance(Iterator first, Iterator last, random_iterator_tag) {
        return last - first;
    }
    template<typename Iterator>
    inline typename iterator_traits<Iterator>::difference_type distance(Iterator first, Iterator last) {
        return __distance(first ,last, iterator_category(first));
    }

    template<typename Iterator, typename Distance>
    inline void __advance(Iterator &i, Distance n, input_iterator_tag) {
        while(n--) {
            ++i;
        }
    }
    template<typename Iterator, typename Distance>
    inline void __advance(Iterator &i, Distance n, random_iterator_tag) {
        i = i + n;
    }
    template<typename Iterator, typename Distance>
    inline void advance(Iterator &i, Distance n) {
        return __advance(i, n, iterator_category(i));
    }
}