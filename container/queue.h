#pragma once

#include "deque.h"

namespace DS {
    template<typename T, typename Sequence = deque<T> >
    class queue
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
        reference back() {
            return seq.back();
        }
        void push(const value_type &x) { seq.push_back(x); }
        void pop() { seq.pop_front(); }
    };
}