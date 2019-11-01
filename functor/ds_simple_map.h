#pragma once

namespace DS {
    template<typename A, typename B = A>
    class identical
    {
    public:
        B operator() (const A &rhs) const {
            return B(rhs);
        }
    };
}