#pragma once

#include <cstddef>

#include "type_traits.hpp"

namespace ft {

    struct input_iterator_tag {};
    struct output_iterator_tag {};
    struct forward_iterator_tag : public input_iterator_tag {};
    struct bidirectional_iterator_tag : public forward_iterator_tag {};
    struct random_access_iterator_tag : public bidirectional_iterator_tag {};

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
        typedef ptrdiff_t difference_type;
        typedef typename remove_cv<T>::type value_type;
        typedef T* pointer;
        typedef T& reference;
        typedef random_access_iterator_tag iterator_category;
    };

    template <class Iterator>
    class reverse_iterator
    {
    public:

        /* Iterator's type. */
        typedef Iterator    iterator_type;

        /* Preserves Iterator's category. */
        typedef typename ft::iterator_traits<Iterator>::iterator_category iterator_category;

        /* Preserves Iterator's value type. */
        typedef typename ft::iterator_traits<Iterator>::value_type      value_type;

        /* Preserves Iterator's difference type. */
        typedef typename ft::iterator_traits<Iterator>::difference_type     difference_type;

        /* Preserves Iterator's pointer. */
        typedef typename ft::iterator_traits<Iterator>::pointer     pointer;

        /* Preserves Iterator's reference. */
        typedef typename ft::iterator_traits<Iterator>::reference   reference;

        /*
        ** @brief Default.
        ** Construct a reverse iterator object.
        ** This will points to no object.
        */
        reverse_iterator()
                :
                _elem()
        {}

        /*
        ** @brief Initialization.
        ** Construct a reverse iterator object from
        ** an original iteretor "it".
        ** The behavior of the constructed object
        ** replicate the orignal, but he iterates
        ** in the reverse order.
        **
        ** @param it The iterator to replicate.
        */
        explicit reverse_iterator (iterator_type it)
                :
                _elem(it)
        {}

        /*
        ** @brief Copy.
        ** Contruct a reverse iterator from other reverse iterator.
        ** Save the sense of iterateration as "rev_it".
        **
        ** @param rev_it original reverse iterator.
        */
        template <class Iter>
        reverse_iterator (const reverse_iterator<Iter>& rev_it)
                :
                _elem(rev_it.base())
        {}

        /* Added to follow subject obligation */
        virtual ~reverse_iterator() {}

        /*
        ** @brief Return a copy of the base iterator.
        ** Same type as the usert to construct the reverse_iterator.
        ** But pointing to the element next of this.
        ** (A reverse iterator always pointing to an element at
        ** an offset of -1).
        **
        ** @return A copy of the base iterator.
        */
        iterator_type base() const
        { return (_elem); }

        /*
        ** @brief Return a reference to the element pointed
        ** by the iterator.
        **
        ** @return The reference.
        */
        reference operator*() const
        {
            iterator_type tmp = _elem;
            return (*(--tmp));
        }

        /*
        ** @brief Return a reverse iterator pointing to
        ** the element at n position away from the pointed
        ** element of the iterator.
        ** This function need the base iterator to be a
        ** "random-access iterator".
        **
        ** @param "n" Number of elements to offset.
        ** @return An iterator pointing to the element at "n"
        ** position away.
        */
        reverse_iterator operator+ (difference_type n) const { return (reverse_iterator(_elem - n)); }

        /*
        ** @brief Advances the reverse_iterator by one position.
        ** Pre-increment.
        **
        ** @return return "(*this)" incremented.
        */
        reverse_iterator& operator++()
        {
            --_elem;
            return (*this);
        }

        /*
        ** @brief Advances the reverse_iterator by one position.
        ** Post-increment.
        **
        ** @return the value "(*this)" value had before the
        ** call.
        */
        reverse_iterator operator++(int)
        {
            reverse_iterator tmp = *this;
            ++(*this);
            return (tmp);
        }

        /*
        ** @brief Advances the "reverse_iterator" by n element positions.
        ** This function need the base iterator to be a
        ** "random-access iterator".
        **
        ** @param n the number of element.
        ** @return the reverse iterator itself (*this).
        */
        reverse_iterator& operator+= (difference_type n)
        {
            _elem -= n;
            return (*this);
        }

        /*
        ** @brief Return a reverse iterator pointing to the element
        ** located n positions before the element the iterator
        ** currently points to.
        ** This function need the base iterator to be a
        ** "random-access iterator".
        **
        ** @param n the number of element.
        ** @return An iterator pointing to the element
        ** n position before the currently pointed one.
        */
        reverse_iterator operator- (difference_type n) const { return (reverse_iterator(_elem + n)); }

        /*
        ** @brief Decreases the reverse iterator by one position.
        **
        ** @return "(*this)".
        */
        reverse_iterator& operator--()
        {
            ++_elem;
            return (*this);
        }

        /*
        ** @brief Decreases the reverse iterator by one position.
        **
        ** @retun "(*this)" value before the call.
        */
        reverse_iterator operator--(int)
        {
            reverse_iterator tmp = *this;
            --(*this);
            return (tmp);
        }

        /*
        ** @brief Decreases the reverse iterator by "n" element
        ** postion.
        ** This function need the base iterator to be a
        ** "random-access iterator".
        **
        ** @param n Number of elements to offset.
        ** @return "(*this)".
        */
        reverse_iterator& operator-= (difference_type n)
        {
            _elem += n;
            return (*this);
        }

        /*
        ** @brief Give a pointer to the element
        ** pointed.
        **
        ** @return A pointer to the element pointed.
        */
        pointer operator->() const { return &(operator*()); }

        /*
        ** @brief Accesse the element at "n" positions
        ** away from the element currently pointed.
        ** Cause undefined behavior if the element
        ** does not exist.
        **
        ** @param n The number of positions.
        ** @return A reference at "n".
        */
        reference operator[] (difference_type n) const { return (this->base()[-n - 1]); }

    private:
        iterator_type     _elem;
    };

    /*
    ** @brief Equal comparison between two reverse iterator.
    **
    ** @param lhs Base of comparison.
    ** @param rhs To compare with "lsh".
    ** @return True if the condition is hold, otherwise false.
    */
    template <class Iterator>
    bool operator== (const reverse_iterator<Iterator>& lhs,
                     const reverse_iterator<Iterator>& rhs) { return (lhs.base() == rhs.base()); }

    /* For reverser_iterator == const_reverse_iterator */
    template <class Iterator_L, class Iterator_R>
    bool operator== (const reverse_iterator<Iterator_L>& lhs,
                     const reverse_iterator<Iterator_R>& rhs) { return (lhs.base() == rhs.base()); }

    /*
    ** @brief Different comparison between two reverse iterator.
    **
    ** @param lhs Base of comparison.
    ** @param rhs To compare with "lsh".
    ** @return True if the condition is hold, otherwise false.
    */
    template <class Iterator>
    bool operator!= (const reverse_iterator<Iterator>& lhs,
                     const reverse_iterator<Iterator>& rhs) { return (lhs.base() != rhs.base()); }

    /* For reverser_iterator != const_reverse_iterator */
    template <class Iterator_L, class Iterator_R>
    bool operator!= (const reverse_iterator<Iterator_L>& lhs,
                     const reverse_iterator<Iterator_R>& rhs) { return (lhs.base() != rhs.base()); }

    /*
    ** @brief Inferior comparison between two reverse iterator.
    **
    ** @param lhs Base of comparison.
    ** @param rhs To compare with "lsh".
    ** @return True if the condition is hold, otherwise false.
    */
    template <class Iterator>
    bool operator<  (const reverse_iterator<Iterator>& lhs,
                     const reverse_iterator<Iterator>& rhs) { return (lhs.base() > rhs.base()); }

    /* For reverser_iterator < const_reverse_iterator */
    template <class Iterator_L, class Iterator_R>
    bool operator< (const reverse_iterator<Iterator_L>& lhs,
                    const reverse_iterator<Iterator_R>& rhs) { return (lhs.base() > rhs.base()); }

    /*
    ** @brief Inferior or equal comparison between two reverse iterator.
    **
    ** @param lhs Base of comparison.
    ** @param rhs To compare with "lsh".
    ** @return True if the condition is hold, otherwise false.
    */
    template <class Iterator>
    bool operator<= (const reverse_iterator<Iterator>& lhs,
                     const reverse_iterator<Iterator>& rhs) { return (lhs.base() >= rhs.base()); }

    /* For reverser_iterator <= const_reverse_iterator */
    template <class Iterator_L, class Iterator_R>
    bool operator<= (const reverse_iterator<Iterator_L>& lhs,
                     const reverse_iterator<Iterator_R>& rhs) { return (lhs.base() >= rhs.base()); }

    /*
    ** @brief Superior comparison between two reverse iterator.
    **
    ** @param lhs Base of comparison.
    ** @param rhs To compare with "lsh".
    ** @return True if the condition is hold, otherwise false.
    */
    template <class Iterator>
    bool operator> (const reverse_iterator<Iterator>& lhs,
                    const reverse_iterator<Iterator>& rhs) { return (lhs.base() < rhs.bash()); }

    /* For reverser_iterator > const_reverse_iterator */
    template <class Iterator_L, class Iterator_R>
    bool operator> (const reverse_iterator<Iterator_L>& lhs,
                    const reverse_iterator<Iterator_R>& rhs) { return (lhs.base() < rhs.base()); }
    /*
    ** @brief Superior or equal comparison between two reverse iterator.
    **
    ** @param lhs Base of comparison.
    ** @param rhs To compare with "lsh".
    ** @return True if the condition is hold, otherwise false.
    */
    template <class Iterator>
    bool operator>= (const reverse_iterator<Iterator>& lhs,
                     const reverse_iterator<Iterator>& rhs) { return (lhs.base() <= rhs.base()); }

    /* For reverser_iterator >= const_reverse_iterator */
    template <class Iterator_L, class Iterator_R>
    bool operator>= (const reverse_iterator<Iterator_L>& lhs,
                     const reverse_iterator<Iterator_R>& rhs) { return (lhs.base() <= rhs.base()); }

    /*
    ** @brief Give a reverse iterator pointing to
    ** "rev_it" plus "n".
    **
    ** @param n The number of location away the element pointed
    ** by rev_it.
    ** @param rev_it The reverse iterator.
    ** @return A reverse iterator pointing to n element
    ** after rev_it pointed element.
    */
    template <class Iterator>
    reverse_iterator<Iterator> operator+ (
            typename reverse_iterator<Iterator>::difference_type n,
            const reverse_iterator<Iterator>& rev_it) { return (rev_it + n); }

    /*
    ** @brief The distance between lhs and rhs.
    **
    ** @param lhs Base of comparison.
    ** @param rhs To compare with "lsh".
    ** @return The number of elements between lsh and rhs.
    */
    template <class Iterator>
    typename reverse_iterator<Iterator>::difference_type operator- (
            const reverse_iterator<Iterator>& lhs,
            const reverse_iterator<Iterator>& rhs) { return (lhs.base() - rhs.base()); }

    /* For reverser_iterator - const_reverse_iterator */
    template <class Iterator_L, class Iterator_R>
    bool operator- (const reverse_iterator<Iterator_L>& lhs,
                    const reverse_iterator<Iterator_R>& rhs) { return (lhs.base() - rhs.base()); }

    namespace impl {

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
            iterator_type i;

        public:
            wrap_iter() {}

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
                ++*this;
                return tmp;
            }

            wrap_iter& operator--() {
                --i;
                return *this;
            }

            wrap_iter operator--(int) {
                wrap_iter tmp(*this);
                --*this;
                return tmp;
            }

            wrap_iter operator+(difference_type n) const {
                wrap_iter tmp(*this);
                tmp += n;
                return tmp;
            }

            wrap_iter& operator+=(difference_type n) {
                i += n;
                return *this;
            }

            wrap_iter operator-(difference_type n) const {
                return *this + (-n);
            }

            wrap_iter& operator-=(difference_type n) {
                *this += -n;
                return *this;
            }

            reference operator[](difference_type n) const {
                return i[n];
            }

            iterator_type base() const {
                return i;
            }

        private:
            wrap_iter(iterator_type x) : i(x) {}

            template <class Up>
            friend class wrap_iter;

            template <class T, class Allocator>
            friend class vector;
        };

        template <class Iter>
        bool operator==(const wrap_iter<Iter>& lhs, const wrap_iter<Iter>& rhs) {
            return lhs.base() == rhs.base();
        }

        template <class Iter1, class Iter2>
        bool operator==(const wrap_iter<Iter1>& lhs, const wrap_iter<Iter2>& rhs) {
            return lhs.base() == rhs.base();
        }

        template <class Iter>
        bool operator<(const wrap_iter<Iter>& lhs, const wrap_iter<Iter>& rhs) {
            return lhs.base() < rhs.base();
        }

        template <class Iter1, class Iter2>
        bool operator<(const wrap_iter<Iter1>& lhs, const wrap_iter<Iter2>& rhs) {
            return lhs.base() < rhs.base();
        }

        template <class Iter>
        bool operator!=(const wrap_iter<Iter>& lhs, const wrap_iter<Iter>& rhs) {
            return !(lhs == rhs);
        }

        template <class Iter1, class Iter2>
        bool operator!=(const wrap_iter<Iter1>& lhs, const wrap_iter<Iter2>& rhs) {
            return !(lhs == rhs);
        }

        template <class Iter>
        bool operator>(const wrap_iter<Iter>& lhs, const wrap_iter<Iter>& rhs) {
            return rhs < lhs;
        }

        template <class Iter1, class Iter2>
        bool operator>(const wrap_iter<Iter1>& lhs, const wrap_iter<Iter2>& rhs) {
            return rhs < lhs;
        }

        template <class Iter>
        bool operator>=(const wrap_iter<Iter>& lhs, const wrap_iter<Iter>& rhs) {
            return !(lhs < rhs);
        }

        template <class Iter1, class Iter2>
        bool operator>=(const wrap_iter<Iter1>& lhs, const wrap_iter<Iter2>& rhs) {
            return !(lhs < rhs);
        }

        template <class Iter>
        bool operator<=(const wrap_iter<Iter>& lhs, const wrap_iter<Iter>& rhs) {
            return !(rhs < lhs);
        }

        template <class Iter1, class Iter2>
        bool operator<=(const wrap_iter<Iter1>& lhs, const wrap_iter<Iter2>& rhs) {
            return !(rhs < lhs);
        }

        template <class Iter1, class Iter2>
        typename wrap_iter<Iter1>::difference_type
        operator-(const wrap_iter<Iter1>& lhs, const wrap_iter<Iter2>& rhs) {
            return lhs.base() - rhs.base();
        }

        template <class Iter>
        wrap_iter<Iter> operator+(typename wrap_iter<Iter>::difference_type n, wrap_iter<Iter> x) {
            x += n;
            return x;
        }

    }

}
