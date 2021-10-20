#pragma once

#include "iterator.hpp"
#include "functional.hpp"

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
    inline bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1,
                                        InputIterator2 first2, InputIterator2 last2)
    {
        return lexicographical_compare(first1, last1, first2, last2,
                                       less<typename iterator_traits<InputIterator1>::value_type,
                                            typename iterator_traits<InputIterator2>::value_type>());
    }

}
