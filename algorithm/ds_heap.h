#pragma once

/*
this file is mainly used for priority_queue,
but you can also utilize these function to
build a heap
default heap: max-heap
*/
#include "../traits/type_traits.h"
#include "../iter/iter.h"
#include "../functor/ds_function.h"
#include "ds_algobase.h"

namespace xxds
{
    namespace
    {
        template<typename RandomIterator, typename Difference>
        RandomIterator shift_up(RandomIterator first, Difference cur) {
            /*
                shift the current value up the heap 
                return the final position.
                please make sure that the value is at (first + cur)
            */
           Difference parent;
           while(cur > 0) {
               parent = (cur - 1) / 2;
               if(*(first + parent) < *(first + cur)) {
                   swap(*(first + parent), *(first + cur));
                   cur = parent;
               }
               else {
                   break;
               }
           }
           return first + cur;
        }
        template<typename RandomIterator, typename Difference>
        RandomIterator shift_down(RandomIterator first, Difference cur, Difference final) {
            if(cur < 0) return first + final; //final means invalid
            Difference l_child, r_child, next_child;
            while(cur < final) {
                l_child = 2 * cur + 1, r_child = 2 * cur + 2;
                if(l_child >= final) {
                    break;
                }
                else if(r_child >= final) {
                    next_child = l_child;
                }
                else {
                    if(*(first + l_child) < *(first + r_child)) {
                        next_child = r_child;
                    }
                    else {
                        next_child = l_child;
                    }
                } // select the possible child which has greater value
                if(*(first + cur) < *(first + next_child)) {
                    swap(*(first + cur), *(first + next_child));
                    cur = next_child;
                }
                else {
                    break;
                }
            }
            return first + cur;
        }
    }
    
    //version : functor
    namespace
    {
        template<typename RandomIterator, typename Difference, typename Cmp>
        RandomIterator shift_up(RandomIterator first, Difference cur, Cmp cmp) {
            /*
                shift the current value up the heap 
                return the final position.
                please make sure that the value is at (first + cur)
            */
           Difference parent;
           while(cur > 0) {
               parent = (cur - 1) / 2;
               if(!cmp(*(first + parent), *(first + cur))) {
                   swap(*(first + parent), *(first + cur));
                   cur = parent;
               }
               else {
                   break;
               }
           }
           return first + cur;
        }
        template<typename RandomIterator, typename Difference, typename Cmp>
        RandomIterator shift_down(RandomIterator first, Difference cur, Difference final, Cmp cmp) {
            if(cur < 0) return first + final; //final means invalid
            Difference l_child, r_child, next_child;
            while(cur < final) {
                l_child = 2 * cur + 1, r_child = 2 * cur + 2;
                if(l_child >= final) {
                    break;
                }
                else if(r_child >= final) {
                    next_child = l_child;
                }
                else {
                    if(!cmp(*(first + l_child), *(first + r_child))) {
                        next_child = r_child;
                    }
                    else {
                        next_child = l_child;
                    }
                } // select the possible child which has greater value
                if(!cmp(*(first + cur), *(first + next_child))) {
                    swap(*(first + cur), *(first + next_child));
                    cur = next_child;
                }
                else {
                    break;
                }
            }
            return first + cur;
        }
    }
    namespace
    {
        template<typename RandomIterator, typename Difference>
        inline void __make_heap(RandomIterator first, RandomIterator last, Difference *) {
            RandomIterator cur = first + 1;
            while(cur != last) {
                shift_up(first, static_cast<Difference>(cur - first));
                ++cur;
            }
        }
        template<typename RandomIterator, typename Difference>
        inline RandomIterator __push_heap(RandomIterator first, RandomIterator last, Difference *) {
            return shift_up(first, static_cast<Difference>((last - first)-1));
        }    
        template<typename RandomIterator, typename Difference>
        inline void __pop_heap(RandomIterator first, RandomIterator last, Difference *) {
            swap(*first, *(last - 1));
            shift_down(first, static_cast<Difference>(0), static_cast<Difference>((last - first) - 1));
        }
    }
    template<typename RandomIterator>
    inline void make_heap(RandomIterator first, RandomIterator last) {
        return __make_heap(first, last, difference_type(first));
    }
    template<typename RandomIterator>
    inline RandomIterator push_heap(RandomIterator first, RandomIterator last) {
        // the value to push should already at the "end" of the data structure, namely the last-1
        return __push_heap(first, last, difference_type(first));
    }
    template<typename RandomIterator>
    inline void pop_heap(RandomIterator first, RandomIterator last) {
        // this function will not change the "end" of the data structure, you should mannully change that
        // this function will move the top element to the end of the data_structure
        __pop_heap(first, last, difference_type(first));
        // to make sure the shift_down's Difference is iterator's difference type
        // rather than default int
    }
    template<typename RandomIterator>
    inline void sort_heap(RandomIterator first, RandomIterator last) {
        // sort the heap to a sequence, from minimum to maximum 
        RandomIterator cur_last = last;
        while(cur_last != first) {
            pop_heap(first, cur_last);
            --cur_last;
        }
    }

    //version： with functor compare

    namespace {
        template<typename RandomIterator, typename Difference, typename Cmp>
        inline void __make_heap(RandomIterator first, RandomIterator last, Difference *, Cmp cmp) {
            RandomIterator cur = first + 1;
            while(cur != last) {
                shift_up(first, static_cast<Difference>(cur - first), cmp);
                ++cur;
            }
        }
        template<typename RandomIterator, typename Difference, typename Cmp>
        inline RandomIterator __push_heap(RandomIterator first, RandomIterator last, Difference *, Cmp cmp) {
            return shift_up(first, static_cast<Difference>((last - first)-1), cmp);
        }    
        template<typename RandomIterator, typename Difference, typename Cmp>
        inline void __pop_heap(RandomIterator first, RandomIterator last, Difference *, Cmp cmp) {
            swap(*first, *(last - 1));
            shift_down(first, static_cast<Difference>(0), static_cast<Difference>((last - first) - 1), cmp);
        }
    }
    template<typename RandomIterator, typename Cmp>
    inline void make_heap(RandomIterator first, RandomIterator last, Cmp cmp) {
        return __make_heap(first, last, difference_type(first), cmp);
    }
    template<typename RandomIterator, typename Cmp>
    inline RandomIterator push_heap(RandomIterator first, RandomIterator last, Cmp cmp) {
        // the value to push should already at the "end" of the data structure, namely the last-1
        return __push_heap(first, last, difference_type(first), cmp);
    }
    template<typename RandomIterator, typename Cmp>
    inline void pop_heap(RandomIterator first, RandomIterator last, Cmp cmp) {
        // this function will not change the "end" of the data structure, you should mannully change that
        // this function will move the top element to the end of the data_structure
        __pop_heap(first, last, difference_type(first), cmp);
        // to make sure the shift_down's Difference is iterator's difference type
        // rather than default int
    }
    template<typename RandomIterator, typename Cmp>
    inline void sort_heap(RandomIterator first, RandomIterator last, Cmp cmp) {
        // sort the heap to a sequence, from minimum to maximum 
        RandomIterator cur_last = last;
        while(cur_last != first) {
            pop_heap(first, cur_last, cmp);
            --cur_last;
        }
    }
}