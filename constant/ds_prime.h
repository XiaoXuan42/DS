#pragma once

#include "../algorithm/ds_algorithm.h"
#include "../standard/ds_numeric_def.h"

namespace DS
{
    const int __ds_prime_nums = 28;
    const ds_prime_type __ds_prime_list[__ds_prime_nums] = {
        53, 97, 193, 389, 796, 1543, 3079, 6151, 12289,
        24593, 49157, 98317, 196613, 393241, 786433,
        1572896, 3145739, 6291469, 12582917, 25165843, 50331653,
        10063319, 201326611, 402653189, 805306457, 1610612741,
        3221225473ull, 4294967291ull
    };
    ds_prime_type lower_bound_prime(ds_prime_type x) {
        const ds_prime_type * start = __ds_prime_list;
        const ds_prime_type * end = __ds_prime_list + __ds_prime_nums;
        const ds_prime_type * target = lower_bound(start, end, x);
        if(target == end) {
            return __ds_prime_list[__ds_prime_nums-1];
        }
        else {
            return *target;
        }
    }
}