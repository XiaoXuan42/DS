#pragma once

#include "../container/pair.h"
namespace xxds
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
        constexpr T operator()(const T &x, const T &y) const {
            return x + y;
        }
    };
    template<typename T>
    struct minus : public binary_function<T, T, T>
    {
        constexpr T operator()(const T &x, const T &y) const {
            return x - y;
        }
    };
    template<typename T>
    struct multiplies : public binary_function<T, T, T>
    {
        constexpr T operator()(const T &x, const T &y) const {
            return x * y;
        }
    };
    template<typename T>
    struct divides : public binary_function<T, T, T>
    {
        constexpr T operator()(const T &x, const T &y) const {
            return x / y;
        }
    };
    template<typename T>
    struct modules : public binary_function<T, T, T>
    {
        constexpr T operator()(const T &x, const T &y) const {
            return x % y;
        }
    };
    template<typename T>
    struct negate : public unary_function<T, T>
    {
        constexpr T operator()(const T &x) const {
            return -x;
        }
    };
    template<typename Seq, typename T>
    struct select1 : public unary_function<Seq, T>
    {
        constexpr T& operator()(Seq &x) const {
            return x[0];
        }
        constexpr const T& operator()(const Seq &x) const {
            return x[0];
        }
    };
    template<typename T1, typename T2>
    struct select1<pair<T1, T2>, T1> : public unary_function<pair<T1, T2>, T1>
    {
        constexpr T1&  operator()(pair<T1, T2> &x) const {
            return x.first();
        }
        constexpr const T1& operator()(const pair<T1, T2> &x) const {
            return x.first();
        }
    };
    template<typename Seq, typename T>
    struct select2 : public unary_function<Seq, T>
    {
        constexpr T & operator()(Seq &x) const {
            return x[1];
        }
        constexpr const T & operator()(const Seq &x) const {
            return x[1];
        }
    };
    template<typename T1, typename T2>
    struct select2<pair<T1, T2>, T2> : public unary_function<pair<T1, T2>, T2>
    {
        constexpr T2 & operator()(pair<T1, T2> &x) const {
            return x.second();
        }
        constexpr const T2 operator()(const pair<T1, T2> &x) const {
            return x.second();
        }
    };
    template<typename T>
    struct identity : public unary_function<T, T>
    {
        constexpr T& operator() (T &x) const {
            return x;
        }
        constexpr const T& operator() (const T &x) const {
            return x;
        }
    };

    template<typename T1, typename T2 = T1>
    struct greater : public binary_function<T1, T2, bool>
    {
        constexpr bool operator() (const T1 &x, const T2 &y) const {
            return !(x < y || x == y);
        }
    };
    template<typename T1, typename T2 = T1>
    struct greater_equal : public binary_function<T1, T2, bool>
    {
        constexpr bool operator() (const T1 &x, const T2 &y) const {
            return !(x < y);
        }
    };
    template<typename T1, typename T2 = T1>
    struct less : public binary_function<T1, T2, bool>
    {
        constexpr bool operator() (const T1 &x, const T2 &y) const {
            return x < y; 
        }
    };
    template<typename T1, typename T2 = T1>
    struct less_equal : public binary_function<T1, T2, bool>
    {
        constexpr bool operator() (const T1 &x, const T2 &y) const {
            return x < y || x == y;
        }
    };

    template<typename T1, typename T2=T1>
    struct equal_to : public binary_function<T1, T2, bool>
    {
        constexpr bool operator() (const T1 &x, const T2 &y) const {
            return x == y;
        }
    };

    template<typename T1, typename T2=T1>
    struct not_equal_to : public binary_function<T1, T2, bool>
    {
        constexpr bool operator() (const T1 &x, const T2 &y) const {
            return !(x == y);
        }
    };

    template<typename T>
    struct logical_and : public binary_function<T, T, bool>
    {
        constexpr bool operator() (const T &x, const T &y) const {
            return x && y;
        }
    };
    template<typename T>
    struct logical_or : public binary_function<T, T, bool>
    {
        constexpr bool operator() (const T &x, const T &y) const {
            return x || y;
        }
    };
    template<typename T>
    struct logical_not : public unary_function<T, bool>
    {
        constexpr bool operator()(const T &x) const {
            return !x;
        }
    };

    template<typename T>
    inline T identity_element(plus<T>) {
        return T(0);
    }
    template<typename T>
    inline T identity_element(multiplies<T>) {
        return T(1);
    }
}