#pragma once

#include <memory>

#include "algorithm.hpp"
#include "iterator.hpp"
#include "nullptr.hpp"

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
        pointer begin_;
        pointer end_;
        pointer end_cap_;
        allocator_type alloc_;

    public:
        // TODO
        explicit vector(const allocator_type& alloc = allocator_type())
            : begin_(nullptr_v)
            , end_(nullptr_v)
            , end_cap_(nullptr_v)
            , alloc_(alloc)
        {}

        // TODO
        explicit vector(size_type n, const value_type& val = value_type(),
                        const allocator_type& alloc = allocator_type())
            : begin_(alloc_.allocate(n))
            , end_(begin_)
            , end_cap_(begin_ + n)
            , alloc_(alloc)
        {
            for (; n--; ) {
                alloc_.construct(end_++, val);
            }
        }

        // very TODO
        template <class InputIterator>
        vector(InputIterator first, InputIterator last,
               const allocator_type& alloc = allocator_type(),
               typename ft::enable_if<!ft::is_integral<InputIterator>::value, InputIterator>::type* = nullptr_v)
            : alloc_(alloc)
        {
            bool is_valid;
            if (!(is_valid = ft::is_ft_iterator_tagged<typename ft::iterator_traits<InputIterator>::iterator_category >::value))
                throw (ft::InvalidIteratorException<typename ft::is_ft_iterator_tagged<typename ft::iterator_traits<InputIterator>::iterator_category >::type>());

            difference_type n = distance(first, last);
            _start = _alloc.allocate( n );
            _end_capacity = _start + n;
            _end = _start;
            for (; n--; ) {
                _alloc.construct(_end, *first++);
                _end++;
            }
        }

        // TODO
        vector (const vector& v)
            : begin_(nullptr_v)
            , end_(nullptr_v)
            , end_cap_(nullptr_v)
            , alloc_(v.alloc_)
        {
            insert(begin(), v.begin(), v.end());
        }

        // TODO
        ~vector() {
            clear();
            alloc_.deallocate(begin_, capacity());
        }

        // TODO
        vector &operator=(const vector& x)
        {
            if (x == *this)
                return (*this);
            this->clear();
            this->insert(this->end(), x.begin(), x.end());
            return (*this);
        }

        iterator begin() { return begin_; }
        const_iterator begin() const { return begin_; }
        iterator end() { return end_; }
        const_iterator end() const { return end_; }

        reverse_iterator rbegin() { return reverse_iterator(end()); }
        const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
        reverse_iterator rend() { return reverse_iterator(begin()); }
        const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }

        size_type size() const {
            return static_cast<size_type>(end_ - begin_);
        }

        size_type capacity() const {
            return static_cast<size_type>(end_cap_ - begin_);
        }

        bool empty() const {
            return begin_ == end_;
        }

        // TODO
        size_type max_size() const {
            return allocator_type().max_size();
        }

        reference front() { return *begin_; }
        const_reference front() const { return *begin_; }
        reference back() { return *(end_ - 1); }
        const_reference back() const { return *(end_ - 1); }

        value_type* data() { return begin_; }
        const value_type* data() const { return begin_; }

        /*
        ** @brief Resizes the container so that it contain "n"
        ** element. If "n" is smaller than the actual size
        ** the container is reduced to "n". If it is greater,
        ** val is inserting at the end and according capacity
        ** to allocation calcul system.
        **
        ** @param n the new size of the container.
        ** @param val the element to set.
        */
        void        resize (size_type n, value_type val = value_type())
        {
            if (n > this->max_size())
                throw (std::length_error("vector::resize"));
            else if (n < this->size())
            {
                while (this->size() > n)
                {
                    --_end;
                    _alloc.destroy(_end);
                }
            }
            else
                this->insert(this->end(), n - this->size(), val);
        }

        /*
        ** @brief Request that the vector capacity be at least
        ** enougth to contain "n" element.
        ** If n is greater that the actual capacity a reallocation
        ** can be happen, otherwise nothing happen.
        **
        ** @param n the capacity asked.
        */
        void        reserve (size_type n)
        {
            if (n > this->max_size())
                throw (std::length_error("vector::reserve"));
            else if (n > this->capacity())
            {
                pointer prev_start = _start;
                pointer prev_end = _end;
                size_type prev_size = this->size();
                size_type prev_capacity = this->capacity();

                _start = _alloc.allocate( n );
                _end_capacity = _start + n;
                _end = _start;
                while (prev_start != prev_end)
                {
                    _alloc.construct(_end, *prev_start);
                    _end++;
                    prev_start++;
                }
                _alloc.deallocate(prev_start - prev_size, prev_capacity);
            }
        }

        // Element access:

        /*
        ** @brief Returns a reference to the element at
        ** position n in the vector container.
        ** If "n" is out of range that's causes undefined behavior.
        **
        ** @param n Position of the element in the container.
        ** @return The specified element at "n" position.
        */
        reference operator[] (size_type n) { return (*(_start + n)); }

        /*
        ** @brief Returns a const reference to the element at
        ** position n in the vector container.
        ** If "n" is out of range that's causes undefined behavior.
        **
        ** @param n Position of the element in the container.
        ** @return The specified element at "n" position.
        */
        const_reference operator[] (size_type n) const { return (*(_start + n)); }

        /*
        ** @brief Returns a reference to the element at
        ** position n in the vector container.
        ** The main difference between this function and the
        ** operator "[]" is that the function throw an
        ** std::out_of_range exception if "n" is out of the range of
        ** the container.
        **
        ** @param n Position of the element in the container.
        ** @return The specified element at "n" position.
        */
        reference at (size_type n)
        {
            checkRange(n);
            return ((*this)[n]);
        }

        /*
        ** @brief Returns a const reference to the element at
        ** position n in the vector container.
        ** The main difference between this function and the
        ** operator "[]" is that the function throw an
        ** std::out_of_range exception if "n" is out of the range of
        ** the container.
        **
        ** @param n Position of the element in the container.
        ** @return The specified element at "n" position.
        */
        const_reference at (size_type n) const
        {
            checkRange(n);
            return ((*this)[n]);
        }

        // Modifiers:

        /*
        ** Range (1)
        ** @brief Assigns new contents to the vector, replacing its current
        ** contents, add modifying its size accordingly.
        ** New elements are contruct from each of the elements in tht
        ** range, between first and last, in the same order.
        **
        ** @param first the first element in the range.
        ** @param last the last element in the range.
        */
        template <class InputIterator>
        void assign (InputIterator first, InputIterator last,
                     typename ft::enable_if<!ft::is_integral<InputIterator>::value, InputIterator>::type* = u_nullptr)
        {
            bool is_valid;
            if (!(is_valid = ft::is_ft_iterator_tagged<typename ft::iterator_traits<InputIterator>::iterator_category>::value))
                throw (ft::InvalidIteratorException<typename ft::is_ft_iterator_tagged<typename ft::iterator_traits<InputIterator>::iterator_category >::type>());
            this->clear();
            size_type dist = ft::distance(first, last);
            if (size_type(_end_capacity - _start) >= dist)
            {
                for(; &(*first) != &(*last); first++, _end++)
                    _alloc.construct(_end, *first);
            }
            else
            {
                pointer new_start = pointer();
                pointer new_end = pointer();
                pointer new_end_capacity = pointer();

                new_start = _alloc.allocate(dist);
                new_end = new_start;
                new_end_capacity = new_start + dist;

                for(; &(*first) != &(*last); first++, new_end++)
                    _alloc.construct(new_end, *first);

                _alloc.deallocate(_start, this->capacity());

                _start = new_start;
                _end = new_end;
                _end_capacity = new_end_capacity;
            }
        }

        /*
        ** @brief Fill assign.
        ** Clear the container, fill it with val "n" times.
        ** According size to "n".
        **
        ** @param "n" the number of element.
        ** @param "val" the value of element.
        */
        void assign (size_type n, const value_type& val)
        {
            this->clear();
            if (n == 0)
                return ;
            if (size_type(_end_capacity - _start) >= n)
            {
                while (n)
                {
                    _alloc.construct(_end++ , val);
                    n--;
                }
            }
            else
            {
                _alloc.deallocate(_start, this->capacity());
                _start = _alloc.allocate( n );
                _end = _start;
                _end_capacity = _start + n;
                while (n)
                {
                    _alloc.construct(_end++, val);
                    n--;
                }
            }
        }

        /*
        ** @brief Add new element at the end of the vector.
        ** The content of "val" is copied (moved) to the new element.
        **
        ** @param val The value to be copied.
        */
        void push_back (const value_type& val)
        {
            if (_end == _end_capacity)
            {
                int next_capacity = (this->size() > 0) ? (int)(this->size() * 2) : 1;
                this->reserve(next_capacity);
            }
            _alloc.construct(_end, val);
            _end++;
        }

        /*
        ** @brief Delete the last element of the container.
        ** Reduce the size of the vector of one.
        ** If the container is empty, cause undefined behavior.
        */
        void pop_back()
        {
            _alloc.destroy(&this->back());
            _end--;
        }

        /*
        ** @brief Insert an element a the position. Can ecrease de size
        ** of the container. This action force the container to
        ** realocate all the elements that were after "postion"
        ** to their new positions.
        **
        ** @param position The position where insert.
        ** @param val The element to insert.
        ** @return An iterator to the new element in the container.
        */
        iterator insert (iterator position, const value_type& val)
        {
            size_type pos_len = &(*position) - _start;
            if (size_type(_end_capacity - _end) >= this->size() + 1)
            {
                for (size_type i = 0; i < pos_len; i++)
                    _alloc.construct(_end - i, *(_end - i - 1));
                _end++;
                _alloc.construct(&(*position), val);
            }
            else
            {
                pointer new_start = pointer();
                pointer new_end = pointer();
                pointer new_end_capacity = pointer();

                int next_capacity = (this->size() * 2 > 0) ? this->size() * 2 : 1;
                new_start = _alloc.allocate( next_capacity );
                new_end = new_start + this->size() + 1;
                new_end_capacity = new_start + next_capacity;

                for (size_type i = 0; i < pos_len; i++)
                    _alloc.construct(new_start + i, *(_start + i));
                _alloc.construct(new_start + pos_len, val);
                for (size_type j = 0; j < this->size() - pos_len; j++)
                    _alloc.construct(new_end - j - 1, *(_end - j - 1));

                for (size_type l = 0; l < this->size(); l++)
                    _alloc.destroy(_start + l);
                if (_start)
                    _alloc.deallocate(_start, this->capacity());

                _start = new_start;
                _end = new_end;
                _end_capacity = new_end_capacity;
            }
            return (iterator(_start + pos_len));
        }

        /*
        ** @brief Insert an element a "n" amount of time
        ** before the specified position. Can ecrease de capacity
        ** of the container. This action force the container to
        ** realocate all the elements that were after "position"
        ** to their new positions.
        **
        ** @param position The position where insert.
        ** @param n Amout of element to insert.
        ** @param val The element to insert.
        */
        void insert (iterator position, size_type n, const value_type& val)
        {
            if (n == 0)
                return ;
            if (n > this->max_size())
                throw (std::length_error("vector::insert (fill)"));
            size_type pos_len = &(*position) - _start;
            if (size_type(_end_capacity - _end) >= n)
            {
                for (size_type i = 0; i < this->size() - pos_len; i++)
                    _alloc.construct(_end - i + (n - 1), *(_end - i - 1));
                _end += n;
                while (n)
                {
                    _alloc.construct(&(*position) + (n - 1), val);
                    n--;
                }
            }
            else
            {
                pointer new_start = pointer();
                pointer new_end = pointer();
                pointer new_end_capacity = pointer();

                int next_capacity = (this->capacity() > 0) ? (int)(this->size() * 2) : 1;
                new_start = _alloc.allocate(next_capacity);
                new_end_capacity = new_start + next_capacity;

                if (size_type(new_end_capacity - new_start) < this->size() + n)
                {
                    if (new_start)
                        _alloc.deallocate(new_start, new_start - new_end_capacity);
                    next_capacity = this->size() + n;
                    new_start = _alloc.allocate(next_capacity);
                    new_end = new_start + this->size() + n;
                    new_end_capacity = new_start + next_capacity;
                }

                new_end = new_start + this->size() + n;

                for (int i = 0; i < (&(*position) - _start); i++)
                    _alloc.construct(new_start + i, *(_start + i));
                for (size_type k = 0; k < n; k++)
                    _alloc.construct(new_start + pos_len + k, val);
                for (size_type j = 0; j < (this->size() - pos_len); j++)
                    _alloc.construct(new_end - j - 1, *(_end - j - 1));

                for (size_type u = 0; u < this->size(); u++)
                    _alloc.destroy(_start + u);
                _alloc.deallocate(_start, this->capacity());

                _start = new_start;
                _end = new_end;
                _end_capacity = new_end_capacity;
            }
        }

        /*
        ** @brief Insert element in range from ["first" to
        ** "last") at "position". Can increase the capacity of
        ** the container. Throw if the iterator given is not valid.
        ** Reallocate all elements after the dist between first and last.
        **
        ** @param position the position where insert.
        ** @param first the first element in the range.
        ** @param last the last element in the range.
        */
        template <class InputIterator>
        void insert (iterator position, InputIterator first, InputIterator last,
                     typename ft::enable_if<!ft::is_integral<InputIterator>::value, InputIterator>::type* = u_nullptr)
        {
            bool is_valid;
            if (!(is_valid = ft::is_ft_iterator_tagged<typename ft::iterator_traits<InputIterator>::iterator_category >::value))
                throw (ft::InvalidIteratorException<typename ft::is_ft_iterator_tagged<typename ft::iterator_traits<InputIterator>::iterator_category >::type>());

            size_type dist = ft::distance(first, last);
            if (size_type(_end_capacity - _end) >= dist)
            {
                for(size_type i = 0; i < this->size() - (&(*position) - _start); i++)
                    _alloc.construct(_end - i + (dist - 1), *(_end - i - 1));
                _end += dist;
                for (; &(*first) != &(*last); first++, position++)
                    _alloc.construct(&(*position), *first);
            }
            else
            {
                pointer new_start = pointer();
                pointer new_end = pointer();
                pointer new_end_capacity = pointer();

                new_start = _alloc.allocate( this->size() * 2 );
                new_end = new_start + this->size() + dist;
                new_end_capacity = new_start + ( this->size() * 2 );

                if (size_type(new_end_capacity - new_start) < this->size() + dist)
                {
                    if (new_start)
                        _alloc.deallocate(new_start, new_end_capacity - new_start);
                    new_start = _alloc.allocate (this->size() + dist);
                    new_end = new_start + this->size() + dist;
                    new_end_capacity = new_end;
                }

                for (int i = 0; i < &(*position) - _start; i++)
                    _alloc.construct(new_start + i, *(_start + i));
                for (int j = 0; &(*first) != &(*last); first++, j++)
                    _alloc.construct(new_start + (&(*position) - _start) + j, *first);
                for (size_type k = 0; k < this->size() - (&(*position) - _start); k++)
                    _alloc.construct(new_start + (&(*position) - _start) + dist + k, *(_start + (&(*position) - _start) + k));

                for (size_type l = 0; l < this->size(); l++)
                    _alloc.destroy(_start + l);
                _alloc.deallocate(_start, this->capacity());

                _start = new_start;
                _end = new_end;
                _end_capacity = new_end_capacity;
            }
        }

        /*
        ** @brief Remove element from the vector at "position".
        ** Reduce the size of 1;
        **
        ** @param position the iterator pointing to the
        ** element to remove.
        ** @return a pointer to the element a "&(*position) + 1";
        */
        iterator erase (iterator position)
        {
            pointer p_pos = &(*position);
            _alloc.destroy(&(*position));
            if (&(*position) + 1 == _end)
                _alloc.destroy(&(*position));
            else
            {
                for (int i = 0; i < _end - &(*position) - 1; i++)
                {
                    _alloc.construct(&(*position) + i, *(&(*position) + i + 1));
                    _alloc.destroy(&(*position) + i + 1);
                }
            }
            _end -= 1;
            return (iterator(p_pos));
        }

        /*
        ** @brief Remove element from the vector a range of element.
        ** Reduce the size by the number of element removed.
        **
        ** @param first the first element in the range.
        ** @param last the last element in the range.
        ** @return An iterator that point to the first element
        ** after "last".
        */
        iterator erase (iterator first, iterator last)
        {
            pointer p_first = &(*first);
            for (; &(*first) != &(*last); first++)
                _alloc.destroy(&(*first));
            for (int i = 0; i < _end - &(*last); i++)
            {
                _alloc.construct(p_first + i, *(&(*last) + i));
                _alloc.destroy(&(*last) + i);
            }
            _end -= (&(*last) - p_first);
            return (iterator(p_first));
        }

        /*
        ** @brief Exchanges the content with "x" content.
        ** "x" is of same type. Elements of "x" are elements
        ** of this container and elements of this are of "x".
        ** All iterators, references, pointer on the swaped
        ** objects stay valid.
        **
        ** @param x the vector to swap.
        */
        void swap (vector& x)
        {
            if (x == *this)
                return;

            pointer save_start = x._start;
            pointer save_end = x._end;
            pointer save_end_capacity = x._end_capacity;
            allocator_type save_alloc = x._alloc;

            x._start = this->_start;
            x._end = this->_end;
            x._end_capacity = this->_end_capacity;
            x._alloc = this->_alloc;

            this->_start = save_start;
            this->_end = save_end;
            this->_end_capacity = save_end_capacity;
            this->_alloc = save_alloc;
        }

        /*
        ** @brief Removes (destroy) all elements from the
        ** container. Final size is 0.
        */
        void clear()
        {
            size_type save_size = this->size();
            for (size_type i = 0; i < save_size; i++)
            {
                _end--;
                _alloc.destroy(_end);
            }
        }

    private:
        allocator_type  _alloc;
        pointer         _start;
        pointer         _end;
        pointer         _end_capacity;

        /*
        ** @brief Check if "n" is in the range of the container.
        ** If "n" is out of range that's throw an std::out_of_range
        ** exception.
        **
        ** @param n The position of the element to check.
        */
        void checkRange(const size_type& n) const
        {
            if (n >= this->size())
                throw (std::out_of_range("vector::checkRange: n (which is "
                                         + ft::to_string(n) + ") >= this->size() (which is "
                                         + ft::to_string(this->size()) + ")"));
        }
    };

    template <class T, class Allocator>
    inline bool operator==(const vector<T, Allocator>& lhs, const vector<T, Allocator>& rhs) {
        return lhs.size() == rhs.size() && equal(lhs.begin(), lhs.end(), rhs.begin());
    }

    template <class T, class Allocator>
    inline bool operator!=(const vector<T, Allocator>& lhs, const vector<T, Allocator>& rhs) {
        return !(lhs == rhs);
    }

    template <class T, class Allocator>
    inline bool operator<(const vector<T, Allocator>& lhs, const vector<T, Allocator>& rhs) {
        return lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
    }

    template <class T, class Allocator>
    inline bool operator>(const vector<T, Allocator>& lhs, const vector<T, Allocator>& rhs) {
        return rhs < lhs;
    }

    template <class T, class Allocator>
    inline bool operator<=(const vector<T, Allocator>& lhs, const vector<T, Allocator>& rhs) {
        return !(rhs < lhs);
    }

    template <class T, class Allocator>
    inline bool operator>=(const vector<T, Allocator>& lhs, const vector<T, Allocator>& rhs) {
        return !(lhs < rhs);
    }

    template <class T, class Allocator>
    inline void swap(vector<T, Allocator>& lhs, vector<T, Allocator>& rhs) {
        lhs.swap(rhs);
    }

}
