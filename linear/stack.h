#pragma once

#include "link.h"

namespace DS
{
    template<typename T>
    class Stack
    {
        Link<T> sublink;
    public:
        int size() const {
            return sublink.size();
        }
        bool empty() const {
            return sublink.empty();
        }
        void push(const T &data) {
            sublink.push(data);
        }
        bool pop() {
            return sublink.pop();
        }
        T top() const {
            return sublink.getTail();
        }
    };
}