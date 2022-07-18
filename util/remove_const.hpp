#pragma once

namespace ft {

template <class T>
struct remove_const {
    typedef T type;
};

template <class T>
struct remove_const<const T> {
    typedef T type;
};

} // namespace ft
