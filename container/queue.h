#pragma once

#include "list.h"

template<typename T, typename Sequence = list<T> >
class list
{
public:
    using value_type = typename Sequence::value_type;
    using reference = typename Sequence::reference;
    using const_reference = typename Sequence::const_reference;
    using pointer = typename Sequence::pointer;
    using difference_type = typename Sequence::difference_type;
    using size_type = typename Sequence::size_type;

private:
    using self = list<T, Sequence>;    
    Sequence seq;
public:
    bool operator == (const self &rhs) const {
        return seq == rhs.seq;
    }
    bool operator < (const self &rhs) const {
        return seq < rhs.seq;
    }
    bool empty() {
        return seq.empty();
    }
    size_type size() {
        return seq.size();
    }
    reference front() {
        return seq.front();
    }
    const_reference front() const {
        return seq.front();
    }
    void push(const value_type &x) { seq.push_back(x); }
    void pop() { seq.pop_front(); }
};