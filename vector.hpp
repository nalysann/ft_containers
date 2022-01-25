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
        typedef impl::wrap_iter<pointer> iterator;
        typedef impl::wrap_iter<const_pointer> const_iterator;
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

//        // todo
//        iterator insert (iterator position, const value_type& val)
//        {
//            size_type pos_len = &(*position) - _start;
//            if (size_type(_end_capacity - _end) >= this->size() + 1)
//            {
//                for (size_type i = 0; i < pos_len; i++)
//                    _alloc.construct(_end - i, *(_end - i - 1));
//                _end++;
//                _alloc.construct(&(*position), val);
//            }
//            else
//            {
//                pointer new_start = pointer();
//                pointer new_end = pointer();
//                pointer new_end_capacity = pointer();
//
//                int next_capacity = (this->size() * 2 > 0) ? this->size() * 2 : 1;
//                new_start = _alloc.allocate( next_capacity );
//                new_end = new_start + this->size() + 1;
//                new_end_capacity = new_start + next_capacity;
//
//                for (size_type i = 0; i < pos_len; i++)
//                    _alloc.construct(new_start + i, *(_start + i));
//                _alloc.construct(new_start + pos_len, val);
//                for (size_type j = 0; j < this->size() - pos_len; j++)
//                    _alloc.construct(new_end - j - 1, *(_end - j - 1));
//
//                for (size_type l = 0; l < this->size(); l++)
//                    _alloc.destroy(_start + l);
//                if (_start)
//                    _alloc.deallocate(_start, this->capacity());
//
//                _start = new_start;
//                _end = new_end;
//                _end_capacity = new_end_capacity;
//            }
//            return (iterator(_start + pos_len));
//        }
//
//        // todo
//        void insert (iterator position, size_type n, const value_type& val)
//        {
//            if (n == 0)
//                return ;
//            if (n > this->max_size())
//                throw (std::length_error("vector::insert (fill)"));
//            size_type pos_len = &(*position) - _start;
//            if (size_type(_end_capacity - _end) >= n)
//            {
//                for (size_type i = 0; i < this->size() - pos_len; i++)
//                    _alloc.construct(_end - i + (n - 1), *(_end - i - 1));
//                _end += n;
//                while (n)
//                {
//                    _alloc.construct(&(*position) + (n - 1), val);
//                    n--;
//                }
//            }
//            else
//            {
//                pointer new_start = pointer();
//                pointer new_end = pointer();
//                pointer new_end_capacity = pointer();
//
//                int next_capacity = (this->capacity() > 0) ? (int)(this->size() * 2) : 1;
//                new_start = _alloc.allocate(next_capacity);
//                new_end_capacity = new_start + next_capacity;
//
//                if (size_type(new_end_capacity - new_start) < this->size() + n)
//                {
//                    if (new_start)
//                        _alloc.deallocate(new_start, new_start - new_end_capacity);
//                    next_capacity = this->size() + n;
//                    new_start = _alloc.allocate(next_capacity);
//                    new_end = new_start + this->size() + n;
//                    new_end_capacity = new_start + next_capacity;
//                }
//
//                new_end = new_start + this->size() + n;
//
//                for (int i = 0; i < (&(*position) - _start); i++)
//                    _alloc.construct(new_start + i, *(_start + i));
//                for (size_type k = 0; k < n; k++)
//                    _alloc.construct(new_start + pos_len + k, val);
//                for (size_type j = 0; j < (this->size() - pos_len); j++)
//                    _alloc.construct(new_end - j - 1, *(_end - j - 1));
//
//                for (size_type u = 0; u < this->size(); u++)
//                    _alloc.destroy(_start + u);
//                _alloc.deallocate(_start, this->capacity());
//
//                _start = new_start;
//                _end = new_end;
//                _end_capacity = new_end_capacity;
//            }
//        }
//
//        // todo
//        template <class InputIterator>
//        void insert (iterator position, InputIterator first, InputIterator last,
//                     typename ft::enable_if<!ft::is_integral<InputIterator>::value, InputIterator>::type* = u_nullptr)
//        {
//            bool is_valid;
//            if (!(is_valid = ft::is_ft_iterator_tagged<typename ft::iterator_traits<InputIterator>::iterator_category >::value))
//                throw (ft::InvalidIteratorException<typename ft::is_ft_iterator_tagged<typename ft::iterator_traits<InputIterator>::iterator_category >::type>());
//
//            size_type dist = ft::distance(first, last);
//            if (size_type(_end_capacity - _end) >= dist)
//            {
//                for(size_type i = 0; i < this->size() - (&(*position) - _start); i++)
//                    _alloc.construct(_end - i + (dist - 1), *(_end - i - 1));
//                _end += dist;
//                for (; &(*first) != &(*last); first++, position++)
//                    _alloc.construct(&(*position), *first);
//            }
//            else
//            {
//                pointer new_start = pointer();
//                pointer new_end = pointer();
//                pointer new_end_capacity = pointer();
//
//                new_start = _alloc.allocate( this->size() * 2 );
//                new_end = new_start + this->size() + dist;
//                new_end_capacity = new_start + ( this->size() * 2 );
//
//                if (size_type(new_end_capacity - new_start) < this->size() + dist)
//                {
//                    if (new_start)
//                        _alloc.deallocate(new_start, new_end_capacity - new_start);
//                    new_start = _alloc.allocate (this->size() + dist);
//                    new_end = new_start + this->size() + dist;
//                    new_end_capacity = new_end;
//                }
//
//                for (int i = 0; i < &(*position) - _start; i++)
//                    _alloc.construct(new_start + i, *(_start + i));
//                for (int j = 0; &(*first) != &(*last); first++, j++)
//                    _alloc.construct(new_start + (&(*position) - _start) + j, *first);
//                for (size_type k = 0; k < this->size() - (&(*position) - _start); k++)
//                    _alloc.construct(new_start + (&(*position) - _start) + dist + k, *(_start + (&(*position) - _start) + k));
//
//                for (size_type l = 0; l < this->size(); l++)
//                    _alloc.destroy(_start + l);
//                _alloc.deallocate(_start, this->capacity());
//
//                _start = new_start;
//                _end = new_end;
//                _end_capacity = new_end_capacity;
//            }
//        }

        iterator erase(iterator position) {
            difference_type d = position - begin();
            for (pointer to = begin_ + d, from = to + 1; from != end_; ++to, ++from) {
                alloc_.destroy(to);
                alloc_.construct(to, *from);
            }
            destruct_at_end(end_ - 1);
            return begin_ + d;
        }

        iterator erase(iterator first, iterator last) {
            difference_type d = first - begin();
            if (first != last) {
                pointer to = begin_ + d;
                for (pointer from = to + (last - first); from != end_; ++to, ++from) {
                    alloc_.destroy(to);
                    alloc_.construct(to, *from);
                }
                destruct_at_end(to);
            }
            return begin_ + d;
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
        void construct_at_end(InputIterator first, InputIterator last) {
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
