#pragma once

#include <cstddef>

namespace ft {

template <class Ptr>
struct pointer_traits;

template <class T>
struct pointer_traits<T*> {
    typedef T* pointer;
    typedef T element_type;
    typedef std::ptrdiff_t difference_type;

    template <class U>
    struct rebind {
        typedef U* other;
    };
};

} // namespace ft
