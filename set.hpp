#pragma once

#include <functional>
#include <memory>

#include "util/reverse_iterator.hpp"
#include "util/tree.hpp"

namespace ft {

template <
    class Key,
    class Compare = std::less<Key>,
    class Allocator = std::allocator<Key>
>
class set {
public:
    typedef Key key_type;
    typedef Key value_type;
    typedef Compare key_compare;
    typedef Compare value_compare;
    typedef Allocator allocator_type;
    typedef typename allocator_type::reference reference;
    typedef typename allocator_type::const_reference const_reference;
    typedef typename allocator_type::pointer pointer;
    typedef typename allocator_type::const_pointer const_pointer;
    typedef typename allocator_type::size_type size_type;
    typedef typename allocator_type::difference_type difference_type;

private:
    typedef tree<value_type, value_compare, allocator_type> tree_type;

    tree_type tree_;

public:
    typedef typename tree_type::const_iterator iterator;
    typedef typename tree_type::const_iterator const_iterator;
    typedef ft::reverse_iterator<iterator> reverse_iterator;
    typedef ft::reverse_iterator<const_iterator> const_reverse_iterator;

    template <class Key2, class Compare2, class Alloc2>
    friend class set;

    explicit set(const Compare& comp = Compare(), const Allocator& alloc = Allocator())
        : tree_(comp, alloc)
    {}

    template <class InputIterator>
    set(InputIterator first, InputIterator last, const Compare& comp = Compare(), const Allocator& alloc = Allocator())
        : tree_(comp, alloc)
    {
        insert(first, last);
    }

    set(const set& s)
        : tree_(s.tree_)
    {
        insert(s.begin(), s.end());
    }

    set& operator=(const set& s) {
        tree_ = s.tree_;
        return *this;
    }

    allocator_type get_allocator() const { return tree_.get_allocator(); }

    iterator begin() { return tree_.begin(); }
    const_iterator begin() const { return tree_.begin(); }
    iterator end() { return tree_.end(); }
    const_iterator end() const { return tree_.end(); }

    reverse_iterator rbegin() { return reverse_iterator(end()); }
    const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
    reverse_iterator rend() { return reverse_iterator(begin()); }
    const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }

    bool empty() const { return tree_.empty(); }
    size_type size() const { return tree_.size(); }
    size_type max_size() const { return tree_.max_size(); }

    pair<iterator, bool> insert(const value_type& v) {
        return tree_.insert(v);
    }

    iterator insert(iterator hint, const value_type& v) {
        return tree_.insert(hint, v);
    }

    template <class InputIterator>
    void insert(InputIterator first, InputIterator last) {
        for (iterator e = end(); first != last; ++first) {
            tree_.insert(e, *first);
        }
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

    void swap(set& s) {
        tree_.swap(s.tree_);
    }

    void clear() {
        tree_.clear();
    }

    key_compare key_comp() const { return tree_.value_comp(); }
    value_compare value_comp() const { return tree_.value_comp(); }

    iterator find(const key_type& key) const { return tree_.find(key); }
    size_type count(const key_type& key) const { return tree_.count(key); }

    iterator lower_bound(const key_type& key) const { return tree_.lower_bound(key); }
    iterator upper_bound(const key_type& key) const { return tree_.upper_bound(key); }

    pair<iterator, iterator> equal_range(const key_type& key) const {
        return tree_.equal_range(key);
    }
};

template <class Key, class Compare, class Allocator>
inline bool operator==(const set<Key, Compare, Allocator>& x,
                        const set<Key, Compare, Allocator>& y)
{
    return x.size() == y.size() && equal(x.begin(), x.end(), y.begin());
}

template <class Key, class Compare, class Allocator>
inline bool operator!=(const set<Key, Compare, Allocator>& x,
                        const set<Key, Compare, Allocator>& y)
{
    return !(x == y);
}

template <class Key, class Compare, class Allocator>
inline bool operator<(const set<Key, Compare, Allocator>& x,
                        const set<Key, Compare, Allocator>& y)
{
    return lexicographical_compare(x.begin(), x.end(), y.begin(), y.end());
}

template <class Key, class Compare, class Allocator>
inline bool operator>(const set<Key, Compare, Allocator>& x,
                        const set<Key, Compare, Allocator>& y)
{
    return y < x;
}

template <class Key, class Compare, class Allocator>
inline bool operator<=(const set<Key, Compare, Allocator>& x,
                        const set<Key, Compare, Allocator>& y)
{
    return !(y < x);
}

template <class Key, class Compare, class Allocator>
inline bool operator>=(const set<Key, Compare, Allocator>& x,
                        const set<Key, Compare, Allocator>& y)
{
    return !(x < y);
}

template <class Key, class Compare, class Allocator>
inline void swap(const set<Key, Compare, Allocator>& x,
                    const set<Key, Compare, Allocator>& y)
{
    x.swap(y);
}

}
