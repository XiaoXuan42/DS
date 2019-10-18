#pragma once

#include "../iter/iter.h"

/*
this file includes several algorithms used for numeric computing
这个文件中有一些数值操作的算法
*/

namespace DS
{
    template<typename InputIterator, typename T> 
    T accumulate(InputIterator first, InputIterator last, T init) {
        for( ; first != last; ++first) {
            init = init + *first;
        }
        return init;
    }
    template<typename InputIterator, typename T, typename BinaryOperation>
    T accumulate(InputIterator first, InputIterator last, T init, BinaryOperation op) {
        //op: init is the first parameter
        for( ; first != last; ++first) {
            init = op(init, *first);
        }
        return init;
    }

    template<typename InputIterator, typename OutputIterator, typename T>
    OutputIterator __adjacent_difference(InputIterator first, InputIterator last, OutputIterator result, T *) {
        T value = *first;
        ++first;
        ++result;
        for( ; first != last; ++first) {
            T tmp = *first;
            *result = tmp - value;
            ++result;
            value = tmp;
        }
        return result;
    }
    template<typename InputIterator, typename OutputIterator>
    OutputIterator adjacent_difference(InputIterator first, InputIterator last, OutputIterator result) {
        if(first == last)   return result; 
        *result = *first;
        return __adjacent_difference(first, last, result, value_type(first));
        //这里没有使用书上的另一种写法，因为我觉得value_type可能会根据具体情况特化
    }
    template<typename InputIterator, typename OutputIterator, typename BinaryOperation>
    OutputIterator adjacent_difference(InputIterator first, InputIterator last, OutputIterator result, BinaryOperation op) {
        if(first == last)   return result; 
        *result = *first;
        return __adjacent_difference(first, last, result, value_type(first), op);
        //这里没有使用书上的另一种写法，因为我觉得value_type可能会根据具体情况特化
    }
    template<typename InputIterator, typename OutputIterator, typename T, typename BinaryOperation>
    OutputIterator __adjacent_difference(InputIterator first, InputIterator last, OutputIterator result, T *, BinaryOperation op) {
        T value = *first;
        ++first;
        ++result;
        for( ; first != last; ++first) {
            T tmp = *first;
            *result = op(tmp, value); //为了保持风格一致，tmp作为第一个形参，这里的意思实际上是value - tmp
            ++result;
            value = tmp;
        }
        return result;
    }

    template<typename InputIterator1, typename InputIterator2, typename T>
    T inner_product(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, T init) {
        InputIterator1 cur1 = first1;
        InputIterator2 cur2 = first2;
        while(cur1 != last1) {
            init += (*(cur1)) * (*(cur2));
            ++cur1;
            ++cur2;
        }
        return init;
    }
    template<typename InputIterator1, typename InputIterator2, typename BinaryOperation, typename T>
    T inner_product(InputIterator1 first1, InputIterator2 last1, InputIterator2 first2, T init, BinaryOperation op) {
        InputIterator1 cur1 = first1;
        InputIterator2 cur2 = first2;
        while(cur1 != last1) {
            init += op(*cur1, *cur2);
            ++cur1;
            ++cur2;
        }
        return init;
    }
    
    template<typename InputIterator, typename OutputIterator, typename T>
    OutputIterator __partial_sum(InputIterator first, InputIterator last, OutputIterator result, T*) {
        T value = *first;
        ++first;
        ++result; //第一个元素已经复制到result中
        while(first != last) {
            T tmp = *first;
            *result = *first + value;
            value = value + tmp;
            ++first;
            ++result;
        }
        return result;
    }
    template<typename InputIterator, typename OutputIterator>
    OutputIterator partial_sum(InputIterator first, InputIterator last, OutputIterator result) {
        if(first == last) {
            return result;
        }
        *result = *first;
        return __partial_sum(first, last, result, value_type(first));
    }
    template<typename InputIterator, typename OutputIterator, typename BinaryOperation, typename T>
    OutputIterator __partial_sum(InputIterator first, InputIterator last, OutputIterator result, BinaryOperation op, T*) {
        T value = *first;
        ++first;
        ++result;
        while(first != last) {
            T tmp = *first;
            *result = op(*first, value);
            value = op(value, tmp);
            ++first;
            ++result;
        }
        return result;
    }
    template<typename InputIterator, typename OutputIterator, typename BinaryOperation>
    OutputIterator partial_sum(InputIterator first, InputIterator last, OutputIterator result, BinaryOperation op) {
        if(first == last) {
            return result;
        }
        *result = *first;
        return __partial_sum(first, last, result, op, value_type(first));
    }
    //TODO: add: pow
}