#pragma once

#include "../alloc/ds_alloc.h"

namespace DS
{
    template<typename ForwardIterator>
    void shift_on(ForwardIterator left, ForwardIterator right_bound) { //[left, right_bound) -> [left+1, right_bound]
        ForwardIterator pre = left;
        ForwardIterator next = left + 1;
        while(pre != right_bound) {
            construct(&(*next), *next);
            pre++;
            next++;
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