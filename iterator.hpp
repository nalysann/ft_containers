#pragma once

#include <cstddef>
#include <iterator>

#include "type_traits.hpp"

namespace ft {

    template <class Iter>
    struct iterator_traits {
        typedef typename Iter::difference_type difference_type;
        typedef typename Iter::value_type value_type;
        typedef typename Iter::pointer pointer;
        typedef typename Iter::reference reference;
        typedef typename Iter::iterator_category iterator_category;
    };

    template <class T>
    struct iterator_traits<T*> {
        typedef std::ptrdiff_t difference_type;
        typedef typename remove_cv<T>::type value_type;
        typedef T* pointer;
        typedef T& reference;
        typedef std::random_access_iterator_tag iterator_category;
    };

    template <class Iter>
    class reverse_iterator {
    protected:
        Iter current_;

    public:
        typedef Iter iterator_type;
        typedef typename iterator_traits<Iter>::difference_type difference_type;
        typedef typename iterator_traits<Iter>::reference reference;
        typedef typename iterator_traits<Iter>::pointer pointer;
        typedef typename iterator_traits<Iter>::iterator_category iterator_category;
        typedef typename iterator_traits<Iter>::value_type value_type;

        reverse_iterator() : current_() {}

        explicit reverse_iterator(Iter x) : current_(x) {}

        template <class U>
        reverse_iterator(const reverse_iterator<U>& u) : current_(u.base()) {}

        template <class U>
        reverse_iterator& operator=(const reverse_iterator<U>& u) {
            current_ = u.base();
            return *this;
        }

        Iter base() const {
            return current_;
        }

        reference operator*() const {
            Iter tmp = current_;
            return *--tmp;
        }

        pointer operator->() const {
            return &operator*();
        }

        reverse_iterator& operator++() {
            --current_;
            return *this;
        }

        reverse_iterator operator++(int) {
            reverse_iterator tmp(*this);
            --current_;
            return tmp;
        }

        reverse_iterator& operator--() {
            ++current_;
            return *this;
        }

        reverse_iterator operator--(int) {
            reverse_iterator tmp(*this);
            ++current_;
            return tmp;
        }

        reverse_iterator operator+(difference_type n) const {
            return reverse_iterator(current_ - n);
        }

        reverse_iterator& operator+=(difference_type n) {
            current_ -= n;
            return *this;
        }

        reverse_iterator operator-(difference_type n) const {
            return reverse_iterator(current_ + n);
        }

        reverse_iterator& operator-=(difference_type n) {
            current_ += n;
            return *this;
        }

        reference operator[](difference_type n) const {
            return *(*this + n);
        }
    };

    template <class Iter1, class Iter2>
    inline bool operator==(const reverse_iterator<Iter1>& x, const reverse_iterator<Iter2>& y) {
        return x.base() == y.base();
    }

    template <class Iter1, class Iter2>
    inline bool operator!=(const reverse_iterator<Iter1>& x, const reverse_iterator<Iter2>& y) {
        return x.base() != y.base();
    }

    template <class Iter1, class Iter2>
    inline bool operator<(const reverse_iterator<Iter1>& x, const reverse_iterator<Iter2>& y) {
        return x.base() > y.base();
    }

    template <class Iter1, class Iter2>
    inline bool operator>(const reverse_iterator<Iter1>& x, const reverse_iterator<Iter2>& y) {
        return x.base() < y.base();
    }

    template <class Iter1, class Iter2>
    inline bool operator<=(const reverse_iterator<Iter1>& x, const reverse_iterator<Iter2>& y) {
        return x.base() >= y.base();
    }

    template <class Iter1, class Iter2>
    inline bool operator>=(const reverse_iterator<Iter1>& x, const reverse_iterator<Iter2>& y) {
        return x.base() <= y.base();
    }

    template <class Iter1, class Iter2>
    inline typename reverse_iterator<Iter1>::difference_type
    operator-(const reverse_iterator<Iter1>& x, const reverse_iterator<Iter2>& y) {
        return y.base() - x.base();
    }

    template <class Iter>
    inline reverse_iterator<Iter>
    operator+(typename reverse_iterator<Iter>::difference_type n, const reverse_iterator<Iter>& x) {
        return reverse_iterator<Iter>(x.base() - n);
    }

    template <class Iter>
    class wrap_iter {
    public:
        typedef Iter iterator_type;
        typedef typename iterator_traits<iterator_type>::value_type value_type;
        typedef typename iterator_traits<iterator_type>::difference_type difference_type;
        typedef typename iterator_traits<iterator_type>::pointer pointer;
        typedef typename iterator_traits<iterator_type>::reference reference;
        typedef typename iterator_traits<iterator_type>::iterator_category iterator_category;

    private:
        iterator_type i_;

    public:
        wrap_iter() : i_() {}

        reference operator*() const {
            return *i_;
        }

        pointer operator->() const {
            return i_;
        }

        wrap_iter& operator++() {
            ++i_;
            return *this;
        }

        wrap_iter operator++(int) {
            wrap_iter tmp(*this);
            ++i_;
            return tmp;
        }

        wrap_iter& operator--() {
            --i_;
            return *this;
        }

        wrap_iter operator--(int) {
            wrap_iter tmp(*this);
            --i_;
            return tmp;
        }

        wrap_iter operator+(difference_type n) const {
            return i_ + n;
        }

        wrap_iter& operator+=(difference_type n) {
            i_ += n;
            return *this;
        }

        wrap_iter operator-(difference_type n) const {
            return i_ - n;
        }

        wrap_iter& operator-=(difference_type n) {
            i_ -= n;
            return *this;
        }

        reference operator[](difference_type n) const {
            return i_[n];
        }

        iterator_type base() const {
            return i_;
        }

    private:
        wrap_iter(iterator_type x) : i_(x) {}

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
    wrap_iter<Iter> operator+(typename wrap_iter<Iter>::difference_type n, wrap_iter<Iter> x) {
        x += n;
        return x;
    }

}
