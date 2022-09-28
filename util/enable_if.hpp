#pragma once

namespace ft {

template <bool, class = void>
struct enable_if {};

template <class T>
struct enable_if<true, T> {
    typedef T type;
};

} // namespace ft
