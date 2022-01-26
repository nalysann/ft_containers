#pragma once

#include <functional>
#include <memory>

#include "tree.hpp"
#include "type_traits.hpp"
#include "utility.hpp"

namespace ft {

    template <class Key, class T, class Compare = std::less<Key>, class Allocator = std::allocator<pair<const Key, T> > >
    class map {
    public:
        typedef Key key_type;
        typedef T mapped_type;
        typedef pair<const Key, T> value_type;
        typedef Compare key_compare;
        typedef Allocator allocator_type;
        typedef typename allocator_type::size_type size_type;
        typedef typename allocator_type::difference_type difference_type;
        typedef typename allocator_type::reference reference;
        typedef typename allocator_type::const_reference const_reference;
        typedef typename allocator_type::pointer pointer;
        typedef typename allocator_type::const_pointer const_pointer;

        class value_compare : public std::binary_function<value_type, value_type, bool> {
            friend class map;
        protected:
            key_compare comp_;
            value_compare(key_compare c) : comp_(c) {}

        public:
            bool operator()(const value_type& x, const value_type& y) const {
                return comp_(x.first, y.first);
            }
        };

    private:
        typedef impl::tree<value_type, value_compare, allocator_type> tree_type;
        tree_type tree_;

    public:
        typedef typename tree_type::iterator iterator;
        typedef typename tree_type::const_iterator const_iterator;
        typedef typename tree_type::reverse_iterator reverse_iterator;
        typedef typename tree_type::const_reverse_iterator const_reverse_iterator;

    public:
        explicit map(const key_compare& comp = key_compare(),
                     const allocator_type& alloc = allocator_type())
            : tree_(comp, alloc)
        {}

        template <class InputIterator>
        map(InputIterator first, InputIterator last,
            const key_compare& comp = key_compare(),
            const allocator_type& alloc = allocator_type())
            : tree_(first, last, comp, alloc)
        {}

        map(const map& x) : tree_(x.tree_) {}

        map& operator=(const map& x) {
            tree_ = x.tree_;
            return *this;
        }

        iterator begin() { return tree_.begin(); }
        const_iterator begin() const { return tree_.begin(); }
        iterator end() { return tree_.end(); }
        const_iterator end() const { return tree_.end(); }

        reverse_iterator rbegin() { return tree_.rbegin(); }
        const_reverse_iterator rbegin() const { return tree_.rbegin(); }
        reverse_iterator rend() { return tree_.rend(); }
        const_reverse_iterator rend() const { return tree_.rend(); }

        bool empty() const { return tree_.empty(); }
        size_type size() const { return tree_.size(); }
        size_type max_size() const { return tree_.max_size(); }

        mapped_type& operator[](const key_type& key) {
            return insert(make_pair(key, mapped_type())).first->second;
        }

        pair<iterator, bool> insert(const value_type& val) {
            return tree_.insert(val);
        }

        iterator insert(iterator hint, const value_type& val) {
            return tree_.insert(hint, val);
        }

        template <class InputIterator>
        typename enable_if<!is_integral<InputIterator>::value, void>::type
        insert(InputIterator first, InputIterator last) {
            tree_.insert(first, last);
        }

        void erase(iterator position) {
            tree_.erase(position);
        }

        size_type erase(const key_type& key) {
            return tree_.erase(key);
        }

        void erase(iterator first, iterator last) {
            tree_.erase(first, last);
        }

        void swap(map& x) {
            tree_.swap(x._tree);
        }

        void clear() {
            tree_.clear();
        }

        key_compare key_comp() const { return tree_.value_comp().comp_; }
        value_compare value_comp() const { return tree_.value_comp(); }

        iterator find(const key_type& key) { return tree_.find(key); }
        const_iterator find(const key_type& key) const { return tree_.find(key); }

        size_type count(const key_type& key) const { return tree_.count(key); }

        iterator lower_bound(const key_type& key) { return tree_.lower_bound(key); }
        const_iterator lower_bound(const key_type& key) const { return tree_.lower_bound(key); }

        iterator upper_bound(const key_type& key) { return tree_.upper_bound(key); }
        const_iterator upper_bound(const key_type& key) const { return tree_.upper_bound(key); }

        pair<iterator, iterator> equal_range(const key_type& key) {
            return tree_.equal_range(key);
        }

        pair<const_iterator, const_iterator> equal_range(const key_type& key) const {
            return tree_.equal_range(key);
        }

        allocator_type get_allocator() const { return tree_.get_allocator(); }

        template <class K1, class T1, class C1, class A1>
        friend bool operator==(const map<K1, T1, C1, A1>& x, const map<K1, T1, C1, A1>& y);

        template <class K1, class T1, class C1, class A1>
        friend bool operator<(const map<K1, T1, C1, A1>& x, const map<K1, T1, C1, A1>& y);
    };

    template <class Key, class T, class Compare, class Allocator>
    inline bool operator==(const map<Key, T, Compare, Allocator>& x, const map<Key, T, Compare, Allocator>& y) {
        return x._tree == y._tree;
    }

    template <class Key, class T, class Compare, class Allocator>
    inline bool operator!=(const map<Key, T, Compare, Allocator>& x, const map<Key, T, Compare, Allocator>& y) {
        return !(x == y);
    }

    template <class Key, class T, class Compare, class Allocator>
    inline bool operator<(const map<Key, T, Compare, Allocator>& x, const map<Key, T, Compare, Allocator>& y) {
        return x.tree_ < y.tree_;
    }

    template <class Key, class T, class Compare, class Allocator>
    inline bool operator>(const map<Key, T, Compare, Allocator>& x, const map<Key, T, Compare, Allocator>& y) {
        return y < x;
    }

    template <class Key, class T, class Compare, class Allocator>
    inline bool operator<=(const map<Key, T, Compare, Allocator>& x, const map<Key, T, Compare, Allocator>& y) {
        return !(y < x);
    }

    template <class Key, class T, class Compare, class Allocator>
    inline bool operator>=(const map<Key, T, Compare, Allocator>& x, const map<Key, T, Compare, Allocator>& y) {
        return !(x < y);
    }

    template <class Key, class T, class Compare, class Allocator>
    inline void swap(const map<Key, T, Compare, Allocator>& x, const map<Key, T, Compare, Allocator>& y) {
        x.swap(y);
    }

}
