#pragma once

namespace ft {

template <class T>
struct remove_volatile {
    typedef T type;
};

template <class T>
struct remove_volatile<volatile T> {
    typedef T type;
};

} // namespace ft
