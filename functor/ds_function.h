#pragma once

namespace DS
{
    template<typename Arg, typename Result>
    struct unary_function
    {
        typedef Arg argument_type;
        typedef Result result_type;
    };
    template<typename Arg1, typename Arg2, typename Result>
    struct binary_function
    {
        typedef Arg1 first_argument_type;
        typedef Arg2 second_argument_type;
        typedef Result result_type;
    };
    template<typename T>
    struct plus: public binary_function<T, T, T>
    {
        constexpr T operator(const T &x, const T &y) const {
            return x + y;
        }
    };
    template<typename T>
    struct minus : public binary_function<T, T, T>
    {
        constexpr T operator(const T &x, const T &y) const {
            return x - y;
        }
    };
    template<typename T>
    struct multiplies : public binary_function<T, T, T>
    {
        constexpr T operator(const T &x, const T &y) const {
            return x * y;
        }
    };
    template<typename T>
    struct divides : public binary_function<T, T, T>
    {
        constexpr T operator(const T &x, const T &y) const {
            return x / y;
        }
    };
    template<typename T>
    struct modules : public binary_function<T, T, T>
    {
        constexpr T operator(const T &x, const T &y) const {
            return x % y;
        }
    };
    template<typename T>
    struct negate : public unary_function<T, T>
    {
        constexpr T operator(const T &x) const {
            return -x;
        }
    };
}