#pragma once

#include "iterator_traits.hpp"

namespace {

template <class T1, class T2 = T1>
struct less {
    bool operator()(const T1& x, const T1& y) const { return x < y; }
    bool operator()(const T1& x, const T2& y) const { return x < y; }
    bool operator()(const T2& x, const T1& y) const { return x < y; }
    bool operator()(const T2& x, const T2& y) const { return x < y; }
};

template <class T1>
struct less<T1, T1> {
    bool operator()(const T1& x, const T1& y) const { return x < y; }
};

template <class T1>
struct less<const T1, T1> {
    bool operator()(const T1& x, const T1& y) const { return x < y; }
};

template <class T1>
struct less<T1, const T1> {
    bool operator()(const T1& x, const T1& y) const { return x < y; }
};

} // anonymous namespace

namespace ft {

template <class InputIterator1, class InputIterator2, class Compare>
bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1,
                             InputIterator2 first2, InputIterator2 last2,
                             Compare comp)
{
    for (; first2 != last2; ++first1, ++first2) {
        if (first1 == last1 || comp(*first1, *first2)) {
            return true;
        }
        if (comp(*first2, *first1)) {
            return false;
        }
    }
    return false;
}

template <class InputIterator1, class InputIterator2>
bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1,
                             InputIterator2 first2, InputIterator2 last2)
{
    typedef typename iterator_traits<InputIterator1>::value_type v1;
    typedef typename iterator_traits<InputIterator2>::value_type v2;
    return lexicographical_compare(first1, last1, first2, last2, less<v1, v2>());
}

} // namespace ft
