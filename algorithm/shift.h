#pragma once

#include "../alloc/ds_alloc.h"
#include "../iter/iter.h"

namespace DS
{
    template<typename Iterator>
    void shift_on(Iterator left, Iterator right_bound) { //[left, right_bound) -> [left+1, right_bound]
        typename iterator_traits<Iterator>::Category category;
        return shift_on(left, right, category());
    }
    template<typename ForwardIterator>
    void shift_on(ForwardIterator left, ForwardIterator right_bound, forward_iterator_tag ft) {
        typename iterator_traits<ForwardIterator>::value_type tmp, old_tmp = *left;
        ForwardIterator pre = left, cur = left + 1;
        while(pre != right_bound) {
            tmp = *cur;
            construct(&(*cur), old_tmp);
            old_tmp = tmp;
            pre++;
            cur++;
        }
    }
    template<typename RandomIterator>
    void shift_on(RandomIterator left, RandomIterator right_bound, random_iterator_tag rt) {
        RandomIterator pre = right_bound, cur = right_bound - 1;
        while(pre != left) {
            construct(&(*pre), *cur);
            pre--;
            cur--;
        }
    }
    template<typename ForwardIterator>
    void shift_back(ForwardIterator left_bound, ForwardIterator right) {//(left_bound, right] -> [left_bound, right-1]
        ForwardIterator cur = left_bound;
        ForwardIterator next = left_bound + 1;
        while(cur != next) {
            constrcut(&(*cur), *next);
            cur++;
            next++;
        }
    }
};