#pragma once

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <limits>
#include <memory>
#include <stdexcept>
#include <utility>

#include "algorithm.hpp"
#include "iterator.hpp"
#include "type_traits.hpp"

namespace ft {

    template <class T, class Allocator = std::allocator<T> >
    class vector {
    public:
        typedef T value_type;
        typedef Allocator allocator_type;
        typedef typename allocator_type::size_type size_type;
        typedef typename allocator_type::difference_type difference_type;
        typedef typename allocator_type::reference reference;
        typedef typename allocator_type::const_reference const_reference;
        typedef typename allocator_type::pointer pointer;
        typedef typename allocator_type::const_pointer const_pointer;
        typedef wrap_iter<pointer> iterator;
        typedef wrap_iter<const_pointer> const_iterator;
        typedef ft::reverse_iterator<iterator> reverse_iterator;
        typedef ft::reverse_iterator<const_iterator> const_reverse_iterator;

    private:
        allocator_type alloc_;
        pointer begin_;
        pointer end_;
        pointer end_cap_;

    public:
        explicit vector(const allocator_type& alloc = allocator_type())
            : alloc_(alloc)
            , begin_(NULL)
            , end_(NULL)
            , end_cap_(NULL)
        {}

        explicit vector(size_type n, const value_type& val = value_type(),
                        const allocator_type& alloc = allocator_type())
            : alloc_(alloc)
            , begin_(NULL)
            , end_(NULL)
            , end_cap_(NULL)
        {
            if (n > 0) {
                vallocate(n);
                construct_at_end(n, val);
            }
        }

        template <class InputIterator>
        vector(InputIterator first, InputIterator last,
               const allocator_type& alloc = allocator_type(),
               typename enable_if<!is_integral<InputIterator>::value>::type* = NULL)
            : alloc_(alloc)
            , begin_(NULL)
            , end_(NULL)
            , end_cap_(NULL)
        {
            for (; first != last; ++first) {
                push_back(*first);
            }
        }

        vector(const vector& x)
            : alloc_(x.alloc_)
            , begin_(NULL)
            , end_(NULL)
            , end_cap_(NULL)
        {
            size_type n = x.size();
            if (n > 0) {
                vallocate(n);
                construct_at_end(x.begin_, x.end_);
            }
        }

        vector& operator=(const vector& x) {
            if (this != &x) {
                vdeallocate();
                begin_ = end_ = end_cap_ = NULL;
                assign(x.begin_, x.end_);
            }
            return *this;
        }

        ~vector() {
            vdeallocate();
        }

        iterator begin() { return begin_; }
        const_iterator begin() const { return begin_; }
        iterator end() { return end_; }
        const_iterator end() const { return end_; }

        reverse_iterator rbegin() { return reverse_iterator(end()); }
        const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
        reverse_iterator rend() { return reverse_iterator(begin()); }
        const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }

        size_type size() const { return static_cast<size_type>(end_ - begin_); }
        size_type capacity() const { return static_cast<size_type>(end_cap_ - begin_); }
        size_type max_size() const { return std::min<size_type>(alloc_.max_size(), std::numeric_limits<difference_type>::max()); }
        bool empty() const { return begin_ == end_; }

        void reserve(size_type n) {
            if (n <= capacity()) {
                return;
            }
            pointer new_begin = alloc_.allocate(n);
            pointer new_end = new_begin + size();
            pointer new_end_cap = new_begin + n;
            try {
                std::uninitialized_copy(begin_, end_, new_begin);
            } catch (...) {
                alloc_.deallocate(new_begin, n);
                throw;
            }
            vdeallocate();
            begin_ = new_begin;
            end_ = new_end;
            end_cap_ = new_end_cap;
        }

        void resize(size_type n, value_type val = value_type()) {
            size_type sz = size();
            if (sz < n) {
                if (n > capacity()) {
                    reserve(n);
                }
                construct_at_end(n - sz, val);
            } else if (sz > n) {
                destruct_at_end(begin_ + n);
            }
        }

        reference operator[](size_type n) { return begin_[n]; }
        const_reference operator[](size_type n) const { return begin_[n]; }

        reference at(size_type n) {
            if (n >= size()) {
                throw std::out_of_range("vector");
            }
            return begin_[n];
        }

        const_reference at(size_type n) const {
            if (n >= size()) {
                throw std::out_of_range("vector");
            }
            return begin_[n];
        }

        reference front() { return *begin_; }
        const_reference front() const { return *begin_; }
        reference back() { return *(end_ - 1); }
        const_reference back() const { return *(end_ - 1); }

        template <class InputIterator>
        typename enable_if<!is_integral<InputIterator>::value, void>::type
        assign(InputIterator first, InputIterator last) {
            clear();
            for (; first != last; ++first)
                push_back(*first);
        }

        void assign(size_type n, const value_type& val) {
            if (n <= capacity()) {
                size_type sz = size();
                std::fill_n(begin_, std::min(n, sz), val);
                if (n > sz) {
                    construct_at_end(n - sz, val);
                } else {
                    destruct_at_end(begin_ + n);
                }
            } else {
                vdeallocate();
                vallocate(n);
                construct_at_end(n, val);
            }
        }

        void push_back(const value_type& val) {
            if (end_ == end_cap_) {
                reserve(capacity() ? capacity() * 2 : 1);
            }
            construct_at_end(1, val);
        }

        void pop_back() {
            destruct_at_end(end_ - 1);
        }

        iterator insert(iterator position, const value_type& val) {
            difference_type d = position - begin();
            pointer p = begin_ + d;
            if (end_ != end_cap_) {
                if (p == end_) {
                    construct_at_end(1, val);
                } else {
                    std::copy_backward(p, end_, end_ + 1);
                    *p = val;
                    ++end_;
                }
            } else {
                size_type n = capacity() ? capacity() * 2 : 1;
                pointer new_begin = alloc_.allocate(n);
                pointer new_end = new_begin + size() + 1;
                pointer new_end_cap = new_begin + n;
                try {
                    std::uninitialized_copy(begin_, p, new_begin);
                    alloc_.construct(new_begin + d, val);
                    std::uninitialized_copy(p, end_, new_begin + d + 1);
                } catch (...) {
                    alloc_.destroy(new_begin + d);
                    alloc_.deallocate(new_begin, n);
                    throw;
                }
                vdeallocate();
                begin_ = new_begin;
                end_ = new_end;
                end_cap_ = new_end_cap;
            }
            return p;
        }

        void insert(iterator position, size_type n, const value_type& val) {
            if (n == 0) {
                return;
            }
            difference_type d = position - begin();
            pointer p = begin_ + d;
            if (n <= static_cast<size_type>(end_cap_ - end_)) {
                pointer old_end = end_;
                if (n > static_cast<size_type>(end_ - p)) {
                    size_type c = n - (end_ - p);
                    construct_at_end(c, val);
                    n -= c;
                }
                if (n > 0) {
                    std::copy_backward(p, old_end, old_end + n);
                    std::fill_n(p, n, val);
                }
            } else {
                size_type new_cap = std::max(capacity() * 2, size() + n);
                pointer new_begin = alloc_.allocate(new_cap);
                pointer new_end = new_begin + size() + n;
                pointer new_end_cap = new_begin + new_cap;
                try {
                    std::uninitialized_copy(begin_, p, new_begin);
                    for (size_type i = 0; i < n; ++i) {
                        alloc_.construct(new_begin + d + i, val);
                    }
                    std::uninitialized_copy(p, end_, new_begin + d + n);
                } catch (...) {
                    for (size_type i = 0; i < n; ++i) {
                        alloc_.destroy(new_begin + d + i);
                    }
                    alloc_.deallocate(new_begin, n);
                    throw;
                }
                vdeallocate();
                begin_ = new_begin;
                end_ = new_end;
                end_cap_ = new_end_cap;
            }
        }

        template <class InputIterator>
        typename enable_if<!is_integral<InputIterator>::value, void>::type
        insert(iterator position, InputIterator first, InputIterator last) {
            if (first == last) {
                return;
            }
            difference_type d = position - begin();
            pointer p = begin_ + d;
            vector<T> tmp(first, last);
            size_type n = tmp.size();
            if (n <= static_cast<size_type>(end_cap_ - end_)) {
                std::copy_backward(p, end_, end_ + n);
                std::copy(tmp.begin_, tmp.end_, p);
                end_ += n;
            } else {
                size_type new_cap = std::max(capacity() * 2, size() + n);
                pointer new_begin = alloc_.allocate(new_cap);
                pointer new_end = new_begin + size() + n;
                pointer new_end_cap = new_begin + new_cap;
                try {
                    std::uninitialized_copy(begin_, p, new_begin);
                    std::uninitialized_copy(tmp.begin_, tmp.end_, new_begin + d);
                    std::uninitialized_copy(p, end_, new_begin + d + n);
                } catch (...) {
                    alloc_.deallocate(new_begin, n);
                    throw;
                }
                vdeallocate();
                begin_ = new_begin;
                end_ = new_end;
                end_cap_ = new_end_cap;
            }
        }

        iterator erase(iterator position) {
            pointer p = begin_ + (position - begin());
            destruct_at_end(std::copy(p + 1, end_, p));
            return p;
        }

        iterator erase(iterator first, iterator last) {
            pointer p = begin_ + (first - begin());
            if (first != last) {
                destruct_at_end(std::copy(p + (last - first), end_, p));
            }
            return p;
        }

        void swap(vector& x) {
            using std::swap;
            swap(alloc_, x.alloc_);
            std::swap(begin_, x.begin_);
            std::swap(end_, x.end_);
            std::swap(end_cap_, x.end_cap_);
        }

        void clear() {
            destruct_at_end(begin_);
        }

        allocator_type get_allocator() const {
            return alloc_;
        }

    private:
        void vallocate(size_type n) {
            begin_ = end_ = alloc_.allocate(n);
            end_cap_ = begin_ + n;
        }

        void vdeallocate() {
            if (begin_ != NULL) {
                clear();
                alloc_.deallocate(begin_, capacity());
            }
        }

        void construct_at_end(size_type n, const value_type& val) {
            while (n--) {
                alloc_.construct(end_++, val);
            }
        }

        template <class InputIterator>
        typename enable_if<!is_integral<InputIterator>::value, void>::type
        construct_at_end(InputIterator first, InputIterator last) {
            while (first != last) {
                alloc_.construct(end_++, *first++);
            }
        }

        void destruct_at_end(pointer new_end) {
            while (end_ != new_end) {
                alloc_.destroy(--end_);
            }
        }
    };

    template <class T, class Allocator>
    inline bool operator==(const vector<T, Allocator>& x, const vector<T, Allocator>& y) {
        return x.size() == y.size() && equal(x.begin(), x.end(), y.begin());
    }

    template <class T, class Allocator>
    inline bool operator!=(const vector<T, Allocator>& x, const vector<T, Allocator>& y) {
        return !(x == y);
    }

    template <class T, class Allocator>
    inline bool operator<(const vector<T, Allocator>& x, const vector<T, Allocator>& y) {
        return lexicographical_compare(x.begin(), x.end(), y.begin(), y.end());
    }

    template <class T, class Allocator>
    inline bool operator>(const vector<T, Allocator>& x, const vector<T, Allocator>& y) {
        return y < x;
    }

    template <class T, class Allocator>
    inline bool operator<=(const vector<T, Allocator>& x, const vector<T, Allocator>& y) {
        return !(y < x);
    }

    template <class T, class Allocator>
    inline bool operator>=(const vector<T, Allocator>& x, const vector<T, Allocator>& y) {
        return !(x < y);
    }

    template <class T, class Allocator>
    inline void swap(vector<T, Allocator>& x, vector<T, Allocator>& y) {
        x.swap(y);
    }

}
