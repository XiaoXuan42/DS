#pragma once

#include "../iter/iter.h"

namespace xxds
{
    template<typename ForwardIterator, typename T>
    ForwardIterator lower_bound(ForwardIterator start, ForwardIterator end, T element) {
        ForwardIterator it;
        typename iterator_traits<ForwardIterator>::difference_type cnt, step;
        cnt = distance(start, end);
        while(cnt > 0) {
            step = cnt / 2;
            it = start;
            advance(it, step);
            if(*it < element) {
                start = ++it;
                cnt -= step + 1;
            } 
            else {
                cnt = step;   
            }
        } 
        return start;
    }
    template<typename ForwardIterator, typename T, typename Comp>
    ForwardIterator lower_bound(ForwardIterator start, ForwardIterator end, const T &element) {
        ForwardIterator it;
        typename iterator_traits<ForwardIterator>::difference_type cnt, step;
        cnt = distance(start, end);
        while(cnt > 0) {
            step = cnt / 2;
            it = start;
            advance(it, step);
            if(Comp(*it, element)) {
                start = ++it;
                cnt -= step + 1;
            }
            else {
                cnt = step;
            }
        }
        return start;
    }
    template<typename ForwardIterator, typename T>
    ForwardIterator upper_bound(ForwardIterator start, ForwardIterator end, const T &element) {
        ForwardIterator it;
        typename iterator_traits<ForwardIterator>::difference_type cnt, step;
        cnt = distance(start, end);
        while(cnt > 0) {
            step = cnt / 2;
            it = start;
            advance(it, step);
            if(!(element < *it)) {
                start = ++it;
                cnt -= step + 1;
            }
            else {
                cnt = step;
            }
        }
        return start;
    }
    template<typename ForwardIterator, typename T, typename Comp>
    ForwardIterator upper_bound(ForwardIterator start, ForwardIterator end, const T &element) {
        ForwardIterator it;
        typename iterator_traits<ForwardIterator>::difference_type cnt, step;
        cnt = distance(start, end);
        while(cnt > 0) {
            step = cnt / 2;
            it = start;
            advance(it, step);
            if(!(Comp(element, *it))) {
                start = ++it;
                cnt -= step + 1;
            }
            else {
                cnt = step;
            }
        }
        return start;
    }
}