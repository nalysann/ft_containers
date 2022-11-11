#pragma once

#include <functional>
#include <iterator>
#include <memory>
#include <utility>

#include "util/equal.hpp"
#include "util/lexicographical_compare.hpp"
#include "util/pair.hpp"
#include "util/reverse_iterator.hpp"
#include "util/tree.hpp"
#include "util/unique_ptr.hpp"

namespace {

template <class Key, class CP, class Compare>
class map_value_compare {
private:
    Compare comp;

public:
    map_value_compare() : comp() {}
    map_value_compare(Compare c) : comp(c) {}

    const Compare& key_comp() const { return comp; }

    bool operator()(const CP& x, const CP& y) const {
        return comp(x.get_value().first, y.get_value().first);
    }

    bool operator()(const CP& x, const Key& y) const {
        return comp(x.get_value().first, y);
    }

    bool operator()(const Key& x, const CP& y) const {
        return comp(x, y.get_value().first);
    }

    void swap(map_value_compare& y) {
        using std::swap;
        swap(comp, y.comp);
    }
};

template <class Key, class CP, class Compare>
void swap(map_value_compare<Key, CP, Compare>& x,
          map_value_compare<Key, CP, Compare>& y)
{
    x.swap(y);
}

template <class Allocator>
class map_node_destructor {
private:
    typedef Allocator allocator_type;

public:
    typedef typename allocator_type::pointer pointer;

private:
    allocator_type& alloc;

    map_node_destructor& operator=(const map_node_destructor&);

public:
    bool first_constructed;
    bool second_constructed;

    explicit map_node_destructor(allocator_type& a)
        : alloc(a)
        , first_constructed(false)
        , second_constructed(false)
    {}

    void operator()(pointer p) {
        if (second_constructed) {
            alloc.destroy(&(p->value.get_value().second));
        }
        if (first_constructed) {
            alloc.destroy(&(p->value.get_value().first));
        }
        if (p) {
            alloc.deallocate(p, 1);
        }
    }
};

template <class Key, class T, class Compare, class Allocator> class map;
template <class TreeIterator> class map_const_iterator;

template <class Key, class T>
struct map_value_type {
public:
    typedef Key key_type;
    typedef T mapped_type;
    typedef pair<const key_type, mapped_type> value_type;

private:
    value_type val;

public:
    value_type& get_value() { return val; }
    const value_type& get_value() const { return val; }

private:
   map_value_type();
   map_value_type(const map_value_type&);
   map_value_type& operator=(const map_value_type&);
   ~map_value_type();
};

template <class TreeIterator>
class map_iterator {
private:
    typedef typename TreeIterator::node_types node_types;

    TreeIterator iter;

public:
    typedef std::bidirectional_iterator_tag iterator_category;
    typedef typename node_types::map_value_type value_type;
    typedef typename TreeIterator::difference_type difference_type;
    typedef value_type& reference;
    typedef typename node_types::map_value_type_pointer pointer;

    map_iterator() {}
    map_iterator(TreeIterator it) : iter(it) {}

    reference operator*() const { return iter->get_value(); }
    pointer operator->() const { return &(iter->get_value()); }

    map_iterator& operator++() {
        ++iter; return *this;
    }

    map_iterator operator++(int) {
        map_iterator tmp(*this);
        ++iter;
        return tmp;
    }

    map_iterator& operator--() {
        --iter;
        return *this;
    }

    map_iterator operator--(int) {
        map_iterator tmp(*this);
        --iter;
        return tmp;
    }

    friend bool operator==(const map_iterator& x, const map_iterator& y) {
        return x.iter == y.iter;
    }

    friend bool operator!=(const map_iterator& x, const map_iterator& y) {
        return x.iter != y.iter;
    }

    template <class, class, class, class> friend class map;
    template <class> friend class map_const_iterator;
};

template <class TreeIterator>
class map_const_iterator {
private:
    typedef typename TreeIterator::node_types node_types;

    TreeIterator iter;

public:
    typedef std::bidirectional_iterator_tag iterator_category;
    typedef typename node_types::map_value_type value_type;
    typedef typename TreeIterator::difference_type difference_type;
    typedef const value_type& reference;
    typedef typename node_types::const_map_value_type_pointer pointer;

    map_const_iterator() {}
    map_const_iterator(TreeIterator it) : iter(it) {}
    map_const_iterator(map_iterator<typename TreeIterator::non_const_iterator> it) : iter(it.iter) {}

    reference operator*() const { return iter->get_value(); }
    pointer operator->() const { return &(iter->get_value()); }

    map_const_iterator& operator++() {
        ++iter;
        return *this;
    }

    map_const_iterator operator++(int) {
        map_const_iterator tmp(*this);
        ++iter;
        return tmp;
    }

    map_const_iterator& operator--() {
        --iter;
        return *this;
    }

    map_const_iterator operator--(int) {
        map_const_iterator tmp(*this);
        --iter;
        return tmp;
    }

    friend bool operator==(const map_const_iterator& x, const map_const_iterator& y) {
        return x.iter == y.iter;
    }

    friend bool operator!=(const map_const_iterator& x, const map_const_iterator& y) {
        return x.iter != y.iter;
    }

    template <class, class, class, class> friend class map;
    template <class, class, class> friend class tree_const_iterator;
};

} // anonymous namespace

namespace ft {

template <
    class Key,
    class T,
    class Compare = std::less<Key>,
    class Allocator = std::allocator<pair<const Key, T> >
>
class map {
public:
    typedef Key key_type;
    typedef T mapped_type;
    typedef pair<const Key, T> value_type;
    typedef Compare key_compare;
    typedef Allocator allocator_type;
    typedef typename allocator_type::reference reference;
    typedef typename allocator_type::const_reference const_reference;
    typedef typename allocator_type::pointer pointer;
    typedef typename allocator_type::const_pointer const_pointer;
    typedef typename allocator_type::size_type size_type;
    typedef typename allocator_type::difference_type difference_type;

    class value_compare : public std::binary_function<value_type, value_type, bool> {
    private:
        friend class map;

    protected:
        Compare comp;
        value_compare(Compare c) : comp(c) {}

    public:
        bool operator()(const value_type& x, const value_type& y) const {
            return comp(x.first, y.first);
        }
    };

private:
    typedef map_value_type<key_type, mapped_type> map_value_type;
    typedef map_value_compare<key_type, value_type, key_compare> map_value_compare;
    typedef typename allocator_type::rebind<map_value_type>::other map_allocator_type;
    typedef tree<map_value_type, map_value_compare, map_allocator_type> tree_type;
    typedef typename tree_type::node_traits node_traits;

    tree_type tree_;

public:
    typedef map_iterator<typename tree_type::iterator> iterator;
    typedef map_const_iterator<typename tree_type::const_iterator> const_iterator;
    typedef ft::reverse_iterator<iterator> reverse_iterator;
    typedef ft::reverse_iterator<const_iterator> const_reverse_iterator;

    template <class Key2, class Value2, class Comp2, class Alloc2>
    friend class map;

    explicit map(const Compare& comp = Compare(), const Allocator& alloc = Allocator())
        : tree_(map_value_compare(comp), typename tree_type::allocator_type(alloc))
    {}

    template <class InputIterator>
    map(InputIterator first, InputIterator last, const Compare& comp = Compare(), const Allocator& alloc = Allocator())
        : tree_(map_value_compare(comp), typename tree_type::allocator_type(alloc))
    {
        insert(first, last);
    }

    map(const map& m)
        : tree_(m.tree_)
    {
        insert(m.begin(), m.end());
    }

    map& operator=(const map& m) {
        tree_ = m.tree_;
        return *this;
    }

    allocator_type get_allocator() const { return allocator_type(tree_.get_allocator()); }

    iterator begin() { return tree_.begin(); }
    const_iterator begin() const { return tree_.begin(); }
    iterator end() { return tree_.end(); }
    const_iterator end() const { return tree_.end(); }

    reverse_iterator rbegin() { return reverse_iterator(end()); }
    const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
    reverse_iterator rend() { return reverse_iterator(begin()); }
    const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }

    bool empty() const { return tree_.size() == 0; }
    size_type size() const { return tree_.size(); }
    size_type max_size() const { return tree_.max_size(); }

    mapped_type& operator[](const key_type& key) {
        parent_pointer parent;
        node_base_pointer& child = tree_.find_equal(parent, key);
        node_pointer r = static_cast<node_pointer>(child);
        if (child == 0) {
            node_holder h = construct_node_with_key(key);
            tree_.insert_node_at(parent, child, static_cast<node_base_pointer>(h.get()));
            r = h.release();
        }
        return r->value.get_value().second;
    }

    pair<iterator, bool> insert(const value_type& v) {
        return tree_.insert_unique(v);
    }

    iterator insert(iterator hint, const value_type& v) {
        return tree_.insert_unique(hint.iter, v);
    }

    template <class InputIterator>
    void insert(InputIterator first, InputIterator last) {
        for (iterator e = end(); first != last; ++first) {
            insert(e.iter, *first);
        }
    }

    void erase(iterator position) {
        tree_.erase(position.iter);
    }

    size_type erase(const key_type& key) {
        return tree_.erase_unique(key);
    }

    void erase(iterator first, iterator last) {
        tree_.erase(first.iter, last.iter);
    }

    void clear() {
        tree_.clear();
    }

    void swap(map& m) {
        tree_.swap(m.tree_);
    }

    key_compare key_comp() const { return tree_.value_comp().key_comp(); }
    value_compare value_comp() const { return value_compare(tree_.value_comp().key_comp()); }

    iterator find(const key_type& key) { return tree_.find(key); }
    const_iterator find(const key_type& key) const { return tree_.find(key); }
    size_type count(const key_type& key) const { return tree_.count_unique(key); }

    iterator lower_bound(const key_type& key) { return tree_.lower_bound(key); }
    const_iterator lower_bound(const key_type& key) const { return tree_.lower_bound(key); }
    iterator upper_bound(const key_type& key) { return tree_.upper_bound(key); }
    const_iterator upper_bound(const key_type& key) const { return tree_.upper_bound(key); }

    pair<iterator, iterator> equal_range(const key_type& key) {
        return tree_.equal_range_unique(key);
    }

    pair<const_iterator, const_iterator> equal_range(const key_type& key) const {
        return tree_.equal_range_unique(key);
    }

private:
    typedef typename tree_type::node node;
    typedef typename tree_type::node_allocator node_allocator;
    typedef typename tree_type::node_pointer node_pointer;
    typedef typename tree_type::node_base_pointer node_base_pointer;
    typedef typename tree_type::parent_pointer parent_pointer;

    typedef map_node_destructor<node_allocator> D;
    typedef unique_ptr<node, D> node_holder;

    node_holder construct_node_with_key(const key_type& key) {
        node_allocator& na = tree_.node_alloc();
        node_holder h(na.allocate(1), D(na));
        na.construct(&(h->value.get_value().first), key);
        h.get_deleter().first_constructed = true;
        na.construct(&(h->value.get_value().second));
        h.get_deleter().second_constructed = true;
        return h;
    }
};

template <class Key, class T, class Compare, class Allocator>
bool operator==(const map<Key, T, Compare, Allocator>& x,
                const map<Key, T, Compare, Allocator>& y)
{
    return x.size() == y.size() && equal(x.begin(), x.end(), y.begin());
}

template <class Key, class T, class Compare, class Allocator>
bool operator!=(const map<Key, T, Compare, Allocator>& x,
                const map<Key, T, Compare, Allocator>& y)
{
    return !(x == y);
}

template <class Key, class T, class Compare, class Allocator>
bool operator<(const map<Key, T, Compare, Allocator>& x,
               const map<Key, T, Compare, Allocator>& y)
{
    return lexicographical_compare(x.begin(), x.end(), y.begin(), y.end());
}

template <class Key, class T, class Compare, class Allocator>
bool operator>(const map<Key, T, Compare, Allocator>& x,
               const map<Key, T, Compare, Allocator>& y)
{
    return y < x;
}

template <class Key, class T, class Compare, class Allocator>
bool operator<=(const map<Key, T, Compare, Allocator>& x,
                const map<Key, T, Compare, Allocator>& y)
{
    return !(y < x);
}

template <class Key, class T, class Compare, class Allocator>
bool operator>=(const map<Key, T, Compare, Allocator>& x,
                const map<Key, T, Compare, Allocator>& y)
{
    return !(x < y);
}

template <class Key, class T, class Compare, class Allocator>
void swap(const map<Key, T, Compare, Allocator>& x,
          const map<Key, T, Compare, Allocator>& y)
{
    x.swap(y);
}

} // namespace ft
