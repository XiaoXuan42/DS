#pragma once

namespace DS
{
    template<typename T, typename B = T>
    class greater
    {
    public:
        bool operator() (const T &first_arg, const B &second_arg) const {
            return first_arg > second_arg;
        }
    };
    template<typename T, typename B = T>
    class less
    {
    public:
        bool operator() (const T &first_arg, const B &second_arg) const {
            return first_arg < second_arg;
        }
    };
    template<typename T, typename B = T>
    class equal
    {
    public:
        bool operator() (const T &lhs, const B &rhs) const {
            return lhs == rhs;
        }
    };
}