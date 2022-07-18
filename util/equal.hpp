#pragma once

#include "util/comp.hpp"
#include "util/iterator_traits.hpp"

namespace ft {

template <class InputIterator1, class InputIterator2, class BinaryPredicate>
inline bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, BinaryPredicate pred)
{
    for (; first1 != last1; ++first1, ++first2) {
        if (!pred(*first1, *first2)) {
            return false;
        }
    }
    return true;
}

template <class InputIterator1, class InputIterator2>
inline bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2)
{
    typedef typename iterator_traits<InputIterator1>::value_type v1;
    typedef typename iterator_traits<InputIterator2>::value_type v2;
    return equal(first1, last1, first2, impl::equal_to<v1, v2>());
}

} // namespace ft
