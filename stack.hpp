#pragma once

#include "vector.hpp"

namespace ft {

    template <class T, class Container = vector<T> >
    class stack {
    public:
        typedef Container container_type;
        typedef typename container_type::value_type value_type;
        typedef typename container_type::reference reference;
        typedef typename container_type::const_reference const_reference;
        typedef typename container_type::size_type size_type;

    protected:
        container_type c;

    public:
        stack() : c() {}

        stack(const stack& s) : c(s.c) {}

        stack& operator=(const stack& s) { c = s.c; return *this; }

        explicit stack(const container_type& c) : c(c) {}

        bool empty() const { return c.empty(); }

        size_type size() const { return c.size(); }

        reference top() { return c.back(); }

        const_reference top() const { return c.back(); }

        void push(const value_type& v) { c.push_back(v); }

        void pop() { c.pop_back(); }

        template <class T1, class C1>
        friend bool operator==(const stack<T1, C1>& lhs, const stack<T1, C1>& rhs);

        template <class T1, class C1>
        friend bool operator<(const stack<T1, C1>& lhs, const stack<T1, C1>& rhs);
    };

    template <class T, class Container>
    inline bool operator==(const stack<T, Container>& lhs, const stack<T, Container>& rhs) {
        return lhs.c == rhs.c;
    }

    template <class T, class Container>
    inline bool operator!=(const stack<T, Container>& lhs, const stack<T, Container>& rhs) {
        return !(lhs == rhs);
    }

    template <class T, class Container>
    inline bool operator<(const stack<T, Container>& lhs, const stack<T, Container>& rhs) {
        return lhs.c < rhs.c;
    }

    template <class T, class Container>
    inline bool operator>(const stack<T, Container>& lhs, const stack<T, Container>& rhs) {
        return rhs < lhs;
    }

    template <class T, class Container>
    inline bool operator<=(const stack<T, Container>& lhs, const stack<T, Container>& rhs) {
        return !(rhs < lhs);
    }

    template <class T, class Container>
    inline bool operator>=(const stack<T, Container>& lhs, const stack<T, Container>& rhs) {
        return !(lhs < rhs);
    }

}
