#pragma once

#include "../iter/iter.h"
#include "../traits/type_traits.h"

template<typename ForwardIterator>
ForwardIterator copy(ForwardIterator first, ForwardIterator last, ForwardIterator target) {
    ForwardIterator cur = first, p_destination = target;
    while(cur != last) {
        typename iterator_traits<ForwardIterator>::value_type tmp = *cur; 
        construct(&(*target), cur); //FIXME: haven't call destructor
        cur++;
        p_destination++;
    }
    return p_destination;
}//TODO: re-code this function later