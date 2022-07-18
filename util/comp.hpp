#pragma once

namespace ft::impl {

template <class T1, class T2 = T1>
struct equal_to {
    bool operator()(const T1& x, const T1& y) const { return x == y; }
    bool operator()(const T1& x, const T2& y) const { return x == y; }
    bool operator()(const T2& x, const T1& y) const { return x == y; }
    bool operator()(const T2& x, const T2& y) const { return x == y; }
};

template <class T1>
struct equal_to<T1, T1> {
    bool operator()(const T1& x, const T1& y) const { return x == y; }
};

template <class T1>
struct equal_to<const T1, T1> {
    bool operator()(const T1& x, const T1& y) const { return x == y; }
};

template <class T1>
struct equal_to<T1, const T1> {
    bool operator()(const T1& x, const T1& y) const { return x == y; }
};

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

} // namespace ft::impl
