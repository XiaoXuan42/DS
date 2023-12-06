#pragma once

#define BUILT_IN_TYPE_TRAITS(name)\
template<> struct ds_type_traits< name > {\
    typedef ds_true_type has_trivial_default_constructor;\
    typedef ds_true_type has_trivial_copy_constructor;\
    typedef ds_true_type has_trivial_assignment_operator;\
    typedef ds_true_type has_trivial_destructor;\
    typedef ds_true_type is_POD_type;\
};

namespace xxds
{
    struct ds_true_type {};
    struct ds_false_type {};

    template<typename T>
    struct ds_type_traits {
        typedef ds_false_type has_trivial_default_constructor;
        typedef ds_false_type has_trivial_copy_constructor;
        typedef ds_false_type has_trivial_assignment_operator;
        typedef ds_false_type has_trivial_destructor;
        typedef ds_false_type is_POD_type;
        /* POD is plain old data according to [1] */
    };
    BUILT_IN_TYPE_TRAITS(char);
    BUILT_IN_TYPE_TRAITS(unsigned char);
    BUILT_IN_TYPE_TRAITS(short);
    BUILT_IN_TYPE_TRAITS(unsigned short);
    BUILT_IN_TYPE_TRAITS(int);
    BUILT_IN_TYPE_TRAITS(unsigned int);
    BUILT_IN_TYPE_TRAITS(long);
    BUILT_IN_TYPE_TRAITS(unsigned long);
    BUILT_IN_TYPE_TRAITS(long long);
    BUILT_IN_TYPE_TRAITS(unsigned long long);
    BUILT_IN_TYPE_TRAITS(float);
    BUILT_IN_TYPE_TRAITS(double);
    BUILT_IN_TYPE_TRAITS(long double);

    template<typename T>
    struct ds_type_traits<T*> {
        typedef ds_true_type has_trivial_default_constructor;
        typedef ds_true_type has_trivial_copy_constructor;
        typedef ds_true_type has_trivial_assignment_operator;
        typedef ds_true_type has_trivial_destructor;
        typedef ds_true_type is_POD_type;
    };
} // namespace DS

#undef BUILT_IN_TYPE_TRAITS