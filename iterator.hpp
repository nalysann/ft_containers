#pragma once

#include <cstddef>
#include <iterator>

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
        typedef T value_type;
        typedef T* pointer;
        typedef T& reference;
        typedef std::random_access_iterator_tag iterator_category;
    };

    template <class T>
    struct iterator_traits<const T*> {
        typedef std::ptrdiff_t difference_type;
        typedef T value_type;
        typedef const T* pointer;
        typedef const T& reference;
        typedef std::random_access_iterator_tag iterator_category;
    };

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

    namespace impl {

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
        inline bool operator==(const wrap_iter<Iter>& x, const wrap_iter<Iter>& y) {
            return x.base() == y.base();
        }

        template <class Iter1, class Iter2>
        inline bool operator==(const wrap_iter<Iter1>& x, const wrap_iter<Iter2>& y) {
            return x.base() == y.base();
        }

        template <class Iter>
        inline bool operator!=(const wrap_iter<Iter>& x, const wrap_iter<Iter>& y) {
            return !(x == y);
        }

        template <class Iter1, class Iter2>
        inline bool operator!=(const wrap_iter<Iter1>& x, const wrap_iter<Iter2>& y) {
            return !(x == y);
        }

        template <class Iter>
        inline bool operator<(const wrap_iter<Iter>& x, const wrap_iter<Iter>& y) {
            return x.base() < y.base();
        }

        template <class Iter1, class Iter2>
        inline bool operator<(const wrap_iter<Iter1>& x, const wrap_iter<Iter2>& y) {
            return x.base() < y.base();
        }

        template <class Iter>
        inline bool operator>(const wrap_iter<Iter>& x, const wrap_iter<Iter>& y) {
            return y < x;
        }

        template <class Iter1, class Iter2>
        inline bool operator>(const wrap_iter<Iter1>& x, const wrap_iter<Iter2>& y) {
            return y < x;
        }

        template <class Iter>
        inline bool operator<=(const wrap_iter<Iter>& x, const wrap_iter<Iter>& y) {
            return !(y < x);
        }

        template <class Iter1, class Iter2>
        inline bool operator<=(const wrap_iter<Iter1>& x, const wrap_iter<Iter2>& y) {
            return !(y < x);
        }

        template <class Iter>
        inline bool operator>=(const wrap_iter<Iter>& x, const wrap_iter<Iter>& y) {
            return !(x < y);
        }

        template <class Iter1, class Iter2>
        inline bool operator>=(const wrap_iter<Iter1>& x, const wrap_iter<Iter2>& y) {
            return !(x < y);
        }

        template <class Iter1, class Iter2>
        inline typename wrap_iter<Iter1>::difference_type
        operator-(const wrap_iter<Iter1>& x, const wrap_iter<Iter2>& y) {
            return x.base() - y.base();
        }

        template <class Iter>
        inline wrap_iter<Iter>
        operator+(typename wrap_iter<Iter>::difference_type n, const wrap_iter<Iter>& x) {
            return wrap_iter<Iter>(x.base() + n);
        }

    }

}
