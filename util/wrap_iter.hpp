#pragma once

#include "util/iterator_traits.hpp"

namespace ft {

template <class Iter>
class wrap_iter {
private:
    Iter i;

public:
    typedef Iter iterator_type;
    typedef typename iterator_traits<Iter>::difference_type difference_type;
    typedef typename iterator_traits<Iter>::value_type value_type;
    typedef typename iterator_traits<Iter>::pointer pointer;
    typedef typename iterator_traits<Iter>::reference reference;
    typedef typename iterator_traits<Iter>::iterator_category iterator_category;

    wrap_iter() : i() {}

    explicit wrap_iter(Iter x) : i(x) {}

    template <class U>
    wrap_iter(const wrap_iter<U>& u) : i(u.base()) {}

    Iter base() const {
        return i;
    }

    reference operator*() const {
        return *i;
    }

    pointer operator->() const {
        return i;
    }

    wrap_iter& operator++() {
        ++i;
        return *this;
    }

    wrap_iter operator++(int) {
        wrap_iter tmp(*this);
        ++i;
        return tmp;
    }

    wrap_iter& operator--() {
        --i;
        return *this;
    }

    wrap_iter operator--(int) {
        wrap_iter tmp(*this);
        --i;
        return tmp;
    }

    wrap_iter operator+(difference_type n) const {
        return wrap_iter(i + n);
    }

    wrap_iter& operator+=(difference_type n) {
        i += n;
        return *this;
    }

    wrap_iter operator-(difference_type n) const {
        return wrap_iter(i - n);
    }

    wrap_iter& operator-=(difference_type n) {
        i -= n;
        return *this;
    }

    reference operator[](difference_type n) const {
        return i[n];
    }

    template <class U>
    friend class wrap_iter;

    template <class T, class Allocator>
    friend class vector;
};

template <class Iter>
bool operator==(const wrap_iter<Iter>& x, const wrap_iter<Iter>& y) {
    return x.base() == y.base();
}

template <class Iter1, class Iter2>
bool operator==(const wrap_iter<Iter1>& x, const wrap_iter<Iter2>& y) {
    return x.base() == y.base();
}

template <class Iter>
bool operator!=(const wrap_iter<Iter>& x, const wrap_iter<Iter>& y) {
    return !(x == y);
}

template <class Iter1, class Iter2>
bool operator!=(const wrap_iter<Iter1>& x, const wrap_iter<Iter2>& y) {
    return !(x == y);
}

template <class Iter>
bool operator<(const wrap_iter<Iter>& x, const wrap_iter<Iter>& y) {
    return x.base() < y.base();
}

template <class Iter1, class Iter2>
bool operator<(const wrap_iter<Iter1>& x, const wrap_iter<Iter2>& y) {
    return x.base() < y.base();
}

template <class Iter>
bool operator>(const wrap_iter<Iter>& x, const wrap_iter<Iter>& y) {
    return y < x;
}

template <class Iter1, class Iter2>
bool operator>(const wrap_iter<Iter1>& x, const wrap_iter<Iter2>& y) {
    return y < x;
}

template <class Iter>
bool operator<=(const wrap_iter<Iter>& x, const wrap_iter<Iter>& y) {
    return !(y < x);
}

template <class Iter1, class Iter2>
bool operator<=(const wrap_iter<Iter1>& x, const wrap_iter<Iter2>& y) {
    return !(y < x);
}

template <class Iter>
bool operator>=(const wrap_iter<Iter>& x, const wrap_iter<Iter>& y) {
    return !(x < y);
}

template <class Iter1, class Iter2>
bool operator>=(const wrap_iter<Iter1>& x, const wrap_iter<Iter2>& y) {
    return !(x < y);
}

template <class Iter1, class Iter2>
typename wrap_iter<Iter1>::difference_type
operator-(const wrap_iter<Iter1>& x, const wrap_iter<Iter2>& y) {
    return x.base() - y.base();
}

template <class Iter>
wrap_iter<Iter>
operator+(typename wrap_iter<Iter>::difference_type n, const wrap_iter<Iter>& x) {
    return wrap_iter<Iter>(x.base() + n);
}

} // namespace ft
