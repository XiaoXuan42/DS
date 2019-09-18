#pragma once

#include "link.h"

namespace DS {
    template<typename T>
    class Queue
    {
        Link<T> sublink;

    public:
        T front() const {
            return sublink.get(0);
        }
        void push(const T &data) {
            sublink.push(data);
        }
        bool pop() {
            return sublink.remove(0);
        }
        bool empty() const{
            return sublink.empty();
        }
        int size() const {
            return sublink.size();
        }
    };

}