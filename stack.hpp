#pragma once

#include "vector.hpp"

namespace ft {

    template <class T, class Container = ft::vector<T> >
    class stack {
    public:
        typedef Container container_type;
        typedef typename container_type::value_type value_type;
        typedef typename container_type::reference reference;
        typedef typename container_type::const_reference const_reference;
        typedef typename container_type::size_type size_type;

    protected:
        container_type c_;

    public:
        explicit stack(const container_type& c = container_type()) : c_(c) {}

        stack(const stack& x) : c_(x.c_) {}

        stack& operator=(const stack& x) { c_ = x.c_; return *this; }

        bool empty() const { return c_.empty(); }

        size_type size() const { return c_.size(); }

        reference top() { return c_.back(); }

        const_reference top() const { return c_.back(); }

        void push(const value_type& v) { c_.push_back(v); }

        void pop() { c_.pop_back(); }

        template <class T1, class C1>
        friend bool operator==(const stack<T1, C1>& x, const stack<T1, C1>& y);

        template <class T1, class C1>
        friend bool operator<(const stack<T1, C1>& x, const stack<T1, C1>& y);
    };

    template <class T, class Container>
    inline bool operator==(const stack<T, Container>& x, const stack<T, Container>& y) {
        return x.c == y.c;
    }

    template <class T, class Container>
    inline bool operator!=(const stack<T, Container>& x, const stack<T, Container>& y) {
        return !(x == y);
    }

    template <class T, class Container>
    inline bool operator<(const stack<T, Container>& x, const stack<T, Container>& y) {
        return x.c < y.c;
    }

    template <class T, class Container>
    inline bool operator>(const stack<T, Container>& x, const stack<T, Container>& y) {
        return y < x;
    }

    template <class T, class Container>
    inline bool operator<=(const stack<T, Container>& x, const stack<T, Container>& y) {
        return !(y < x);
    }

    template <class T, class Container>
    inline bool operator>=(const stack<T, Container>& x, const stack<T, Container>& y) {
        return !(x < y);
    }

}
