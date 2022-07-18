#pragma once

#include <iterator>

#include "util/iterator_traits.hpp"

namespace ft {

template <class Iter>
class reverse_iterator
    : public std::iterator<typename iterator_traits<Iter>::iterator_category,
                           typename iterator_traits<Iter>::value_type,
                           typename iterator_traits<Iter>::difference_type,
                           typename iterator_traits<Iter>::pointer,
                           typename iterator_traits<Iter>::reference> {
protected:
    Iter current;

public:
    typedef Iter iterator_type;
    typedef typename iterator_traits<Iter>::difference_type difference_type;
    typedef typename iterator_traits<Iter>::value_type value_type;
    typedef typename iterator_traits<Iter>::pointer pointer;
    typedef typename iterator_traits<Iter>::reference reference;
    typedef typename iterator_traits<Iter>::iterator_category iterator_category;

    reverse_iterator() : current() {}

    explicit reverse_iterator(Iter x) : current(x) {}

    template <class U>
    reverse_iterator(const reverse_iterator<U>& u) : current(u.base()) {}

    Iter base() const {
        return current;
    }

    reference operator*() const {
        Iter tmp = current;
        return *--tmp;
    }

    pointer operator->() const {
        return &(operator*());
    }

    reverse_iterator& operator++() {
        --current;
        return *this;
    }

    reverse_iterator operator++(int) {
        reverse_iterator tmp(*this);
        --current;
        return tmp;
    }

    reverse_iterator& operator--() {
        ++current;
        return *this;
    }

    reverse_iterator operator--(int) {
        reverse_iterator tmp(*this);
        ++current;
        return tmp;
    }

    reverse_iterator operator+(difference_type n) const {
        return reverse_iterator(current - n);
    }

    reverse_iterator& operator+=(difference_type n) {
        current -= n;
        return *this;
    }

    reverse_iterator operator-(difference_type n) const {
        return reverse_iterator(current + n);
    }

    reverse_iterator& operator-=(difference_type n) {
        current += n;
        return *this;
    }

    reference operator[](difference_type n) const {
        return *(*this + n);
    }
};

template <class Iter>
inline bool operator==(const reverse_iterator<Iter>& x, const reverse_iterator<Iter>& y) {
    return x.base() == y.base();
}

template <class Iter>
inline bool operator!=(const reverse_iterator<Iter>& x, const reverse_iterator<Iter>& y) {
    return x.base() != y.base();
}

template <class Iter>
inline bool operator<(const reverse_iterator<Iter>& x, const reverse_iterator<Iter>& y) {
    return x.base() > y.base();
}

template <class Iter>
inline bool operator>(const reverse_iterator<Iter>& x, const reverse_iterator<Iter>& y) {
    return x.base() < y.base();
}

template <class Iter>
inline bool operator<=(const reverse_iterator<Iter>& x, const reverse_iterator<Iter>& y) {
    return x.base() >= y.base();
}

template <class Iter>
inline bool operator>=(const reverse_iterator<Iter>& x, const reverse_iterator<Iter>& y) {
    return x.base() <= y.base();
}

template <class Iter>
inline typename reverse_iterator<Iter>::difference_type
operator-(const reverse_iterator<Iter>& x, const reverse_iterator<Iter>& y) {
    return y.base() - x.base();
}

template <class Iter>
inline reverse_iterator<Iter>
operator+(typename reverse_iterator<Iter>::difference_type n, const reverse_iterator<Iter>& x) {
    return reverse_iterator<Iter>(x.base() - n);
}

} // namespace ft
