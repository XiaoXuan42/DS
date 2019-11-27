#pragma once

namespace DS
{
    template<typename ForwardIterator1, typename ForwardIterator2, typename InputIterator>
    void set_union(ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2, ForwardIterator2 last2, InputIterator dst) {
        ForwardIterator2 p2 = first2;
        for(ForwardIterator1 it1 = first1; it1 != last1; ++it1) {
            *dst = *it1;
            ++dst;
        }
        while(p2 != last2) {
            bool has = false;
            for(ForwardIterator1 it1 = first1; it1 != last1; ++it1) {
                if(*it1 == *p2) {
                    has = true;
                    break;
                }
            }
            if(!has) {
                *dst = *p2;
                ++dst;
            }
            ++p2;
        }
    }
    template<typename ForwardIterator1, typename ForwardIterator2, typename InputIterator>
    void set_intersection(ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2, ForwardIterator2 last2, InputIterator dst) {
        ForwardIterator1 p1 = first1;
        ForwardIterator2 p2 = first2;
        while(p1 != last1) {
            bool has = false;
            for(ForwardIterator2 it2 = first2; it2 != last2; ++it2) {
                if(*it2 == *p1) {
                    has = true;
                    break;
                }
            }
            if(has) {
                *dst = *p1;
                ++dst;
            }
            ++p1;
        }
    }
    template<typename ForwardIterator1, typename ForwardIterator2, typename InputIterator>
    void set_difference(ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2, ForwardIterator2 last2, InputIterator dst) {
        ForwardIterator1 p1 = first1;
        ForwardIterator2 p2 = first2;
        while(p1 != last1) {
            bool has = false;
            for(ForwardIterator2 it2 = first2; it2 != last2; ++it2) {
                if(*it2 == *p1) {
                    has = true;
                    break;
                }
            }
            if(!has) {
                *dst = *p1;
                ++dst;
            }
            ++p1;
        }
        while(p2 != last2) {
            bool has = false;
            for(ForwardIterator1 it1 = first1; it1 != last1; ++it1) {
                if(*it1 == *p2) {
                    has = true;
                    break;
                }
            }
            if(!has) {
                *dst = *p2;
            }
            ++p2;
        }
    }
}