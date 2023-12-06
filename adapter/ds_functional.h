#pragma once

#include "../functor/ds_function.h"

namespace xxds
{
    template<typename Predicate>
    class unary_negate : public unary_function<typename Predicate::argument_type, bool>
    {
    protected:
        Predicate pred;
    public:
        explicit unary_negate(const Predicate& x) : pred(x) {}
        bool operator()(const typename Predicate::argument_type &x) const {
            return !pred(x);
        }
    };
    template<typename Predicate>
    inline unary_negate<Predicate> not1(const Predicate &pred) {
        return unary_negate<Predicate>(pred);
    }
    template<typename Predicate>
    class binary_negate : public binary_function<Predicate::first_argument_type, Predicate::second_argument_type, bool>
    {
    protected:
        Predicate pred;
    public:
        explicit binary_negate(const Predicate &x): pred(x) {}
        bool operator() (const typename Predicate::first_argument_type &x, const typename Predicate::second_argument_type &y) const {
            return !pred(x, y);
        }
    };
    template<typename Predicate>
    inline binary_negate<Predicate> not2(const Predicate &pred) {
        return binary_negate<Predicate>(pred);
    }
    template<typename Func>
    class binder1st : public unary_function<typename Func::second_argument_type, typename Func::result_type>
    {
    protected:
        Func func;
        typename Func::first_argument_type first_arg;
    public:
        explicit bind1st(const Func &f, const typename Func::first_argument_type &fst_arg): func(f), first_arg(fst_arg) {}
        typename Func::result_type operator() (const typename Func::second_argument_type snd_arg) const {
            return func(first_arg, snd_arg);
        }
    };
    template<typename Func>
    inline binder1st<Func> bind1st(Func f, const typename Func::first_argument_type fst_arg) {
        return binder1st<Func>(f, fst_arg);
    }
    template<typename Func>
    class binder2nd : public unary_function<typename Func::first_argument_type, typename Func::result_type>
    {
    protected:
        Func func;
        typename Func::second_argument_type second_arg;
    public:
        explicit bind2nd(const Func &f, const typename Func::second_argument_type &snd_arg): func(f), second_arg(snd_arg) {}
        typename Func::result_type operator() (const typename Func::second_argument_type fst_arg) const {
            return func(fst_arg, second_arg);
        }
    };
    template<typename Func>
    inline binder2nd<Func> bind2nd(Func f, const typename Func::second_argument_type snd_arg) {
        return binder2nd<Func>(f, snd_arg);
    }
    template<typename Arg, typename Result>
    class pointer_to_unary_function : public unary_function<Arg, Result>
    {
    protected:
        Result (*ptr)(Arg);
    public:
        explicit pointer_to_unary_function(Result (*x)(Arg)): ptr(x) {}
        Result operator()(Arg x) const {
            return ptr(x);
        }
    };
    template<typename Arg, typename Result>
    inline pointer_to_unary_function<Arg, Result> ptr_fun(Result (*x)(Arg)) {
        return pointer_to_unary_function<Arg, Result>(x);
    }
    template<typename Arg1, typename Arg2, typename Result>
    class pointer_to_binary_function : public binary_function<Arg1, Arg2, Result>
    {
    protected:
        Result (*ptr)(Arg1, Arg2);
    public:
        explicit pointer_to_binary_function(Result (*x)(Arg1, Arg2)) : ptr(x) {}
        Result operator()(Arg1 x, Arg2 y) const {
            return ptr(x, y);
        }
    };
    template<typename Arg1, typename Arg2, typename Result>
    inline pointer_to_binary_function<Arg1, Arg2, Result> ptr_fun(Result (*x)(Arg1, Arg2)) {
        return pointer_to_binary_function<Arg1, Arg2, Result>(x);
    }
    template<typename S, class T>
    class mem_fun_t : public unary_function<T*, S>
    {
    public:
        explicit mem_fun_t(S (T::*pf)()): f(pf) {}
        S operator()(T *p) const { return (p->*f)(); }
    private:
        S (T::*f)();
    };
    template<typename S, class T>
    class const_mem_fun_t : public unary_function<const T*, S>
    {
    public:
        explicit const_mem_fun_t(S (T::*pf)() const): f(pf) {}
        S operator()(const T *p) const { return (p->*f)(); }
    private:
        S (T::*f)() const;
    };
    template<typename S, class T>
    class mem_fun_ref_t : public unary_function<T, S>
    {
    public:
        explicit mem_fun_ref_t(S (T::*pf)()): f(pf) {}
        S operator()(T &r) const { return (r.*f)(); }
    private:
        S (T::*f)();
    };
    template<typename S, class T>
    class const_mem_fun_ref_t : public unary_function<const T, S>
    {
    public:
        explicit const_mem_fun_ref_t(S (T::*pf)() const): f(pf) {}
        S operator()(const T &r) const { return (r.*f)(); }
    private:
        S (T::*f)() const;
    };
    template<typename S, class T, typename A>
    class mem_fun1_t : public binary_function<T*, A, S>
    {
    public:
        explicit mem_fun1_t(S (T::*pf)(A)): f(pf) {}
        S operator()(T *p, A arg) const { return (p->*f)(arg); }
    private:
        S (T::*f)(A);
    };
    template<typename S, class T, typename A>
    class const_mem_fun1_t : public binary_function<const T*, A, S>
    {
    public:
        explicit const_mem_fun1_t(S (const T::*pf)(A) const): f(pf) {}
        S operator()(const T *p, A arg) const { return (p->*f)(arg); }
    private:
        S (T::*f)(A) const;
    }; 
    template<typename S, class T, typename A>
    class mem_fun1_ref_t : public binary_function<T, A, S>
    {
    public:
        explicit mem_fun1_ref_t(S (T::*pf)(A)): f(pf) {}
        S operator()(T &r, A arg) const { return (r.*f)(arg); }
    private:
        S (T::*f)(A);
    }; 
    template<typename S, class T, typename A>
    class const_mem_fun1_ref_t : public binary_function<const T, A, S>
    {
    public:
        explicit const_mem_fun1_t(S (T::*pf)(A) const): f(pf) {}
        S operator()(const T &r, A arg) const { return (r.*f)(arg); }
    private:
        S (T::*f)(A) const;
    };

    template<typename S, class T>
    inline mem_fun_t<S, T> mem_fun(S (T::*f)()) {
        return mem_fun_t<S, T>(f);
    }
    template<typename S, class T>
    inline const_mem_fun_t<S, T> mem_fun(S (T::*f)() const) {
        return const_mem_fun_t<S, T>(f);
    }
    template<typename S, class T>
    inline mem_fun_ref_t<S, T> mem_fun_ref(S (T::*f)()) {
        return mem_fun_ref_t<S, T>(f);
    }
    template<typename S, class T>
    inline const_mem_fun_ref_t<S, T> mem_fun_ref(S (T::*f)() const) {
        return const_mem_fun_ref_t<S, T>(f);
    }
    template<typename S, class T>
    inline mem_fun_t<S, T> mem_fun1(S (T::*f)()) {
        return mem_fun_t<S, T>(f);
    }
    template<typename S, class T>
    inline const_mem_fun_t<S, T> mem_fun1(S (T::*f)() const) {
        return const_mem_fun_t<S, T>(f);
    }
    template<typename S, class T>
    inline mem_fun_ref_t<S, T> mem_fun1_ref(S (T::*f)()) {
        return mem_fun_ref_t<S, T>(f);
    }
    template<typename S, class T>
    inline const_mem_fun_ref_t<S, T> mem_fun1_ref(S (T::*f)() const) {
        return const_mem_fun_ref_t<S, T>(f);
    }
};