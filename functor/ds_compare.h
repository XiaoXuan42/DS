#pragma once

namespace DS
{
    template<typename T>
    class greater
    {
        bool operator() (const T &first_arg, const T &second_arg) {
            return first_arg > second_arg;
        }
    };
    template<typename T>
    class less
    {
        bool operator() (const T &first_arg, const T &second_arg) {
            return first_arg < second_arg;
        }
    };
}