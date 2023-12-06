#pragma once

#include "vector.h"

namespace xxds {
    template<typename T, typename Sequence = vector<T> >
    class stack
    {
    public:
        using value_type = typename Sequence::value_type;
        using size_type = typename Sequence::size_type;
        using difference_type = typename Sequence::difference_type;
        using pointer = typename Sequence::pointer;
        using reference = typename Sequence::reference;
        using const_reference = typename Sequence::const_reference;
        
    private:
        Sequence seq;
        using self = stack<T, Sequence>;

    public:
        bool empty() const { return seq.empty(); }
        size_type size() const { return seq.size(); }
        const_reference top() const { return seq.back(); }
        reference top() { return seq.back(); }
        void push(const value_type &x) {
            seq.push_back(x);
        }
        void pop() {
            seq.pop_back();
        }
        
        bool operator == (const self &x) const {
            return seq == x.seq;
        }
        bool operator < (const self &x) const {
            return seq < x.seq;
        }
    };
}
