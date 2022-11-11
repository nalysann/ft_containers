#pragma once

#include <iterator>

#include "pointer_traits.hpp"
#include "unique_ptr.hpp"

namespace ft {
namespace impl {

template <class NodePtr>
bool tree_is_left_child(NodePtr x) {
    return x == x->parent->left;
}

template <class NodePtr>
NodePtr tree_min(NodePtr x) {
    while (x->left != 0) {
        x = x->left;
    }
    return x;
}

template <class NodePtr>
NodePtr tree_max(NodePtr x) {
    while (x->right != 0) {
        x = x->right;
    }
    return x;
}

template <class NodePtr>
NodePtr tree_next(NodePtr x) {
    if (x->right != 0) {
        return tree_min(x->right);
    }
    while (!tree_is_left_child(x)) {
        x = x->parent_unsafe();
    }
    return x->parent_unsafe();
}

template <class EndNodePtr, class NodePtr>
EndNodePtr tree_next_iter(NodePtr x) {
    if (x->right != 0) {
        return static_cast<EndNodePtr>(tree_min(x->right));
    }
    while (!tree_is_left_child(x)) {
        x = x->parent_unsafe();
    }
    return static_cast<EndNodePtr>(x->parent);
}

template <class NodePtr, class EndNodePtr>
NodePtr tree_prev_iter(EndNodePtr x) {
    if (x->left != 0) {
        return tree_max(x->left);
    }
    NodePtr xx = static_cast<NodePtr>(x);
    while (tree_is_left_child(xx)) {
        xx = xx->parent_unsafe();
    }
    return xx->parent_unsafe();
}

template <class NodePtr>
NodePtr tree_leaf(NodePtr x) {
    while (true) {
        if (x->left != 0) {
            x = x->left;
            continue;
        }
        if (x->right != 0) {
            x = x->right;
            continue;
        }
        break;
    }
    return x;
}

template <class NodePtr>
void tree_left_rotate(NodePtr x) {
    NodePtr y = x->right;
    x->right = y->left;
    if (x->right != 0) {
        x->right->set_parent(x);
    }
    y->parent = x->parent;
    if (tree_is_left_child(x)) {
        x->parent->left = y;
    } else {
        x->parent_unsafe()->right = y;
    }
    y->left = x;
    x->set_parent(y);
}

template <class NodePtr>
void tree_right_rotate(NodePtr x) {
    NodePtr y = x->left;
    x->left = y->right;
    if (x->left != 0) {
        x->left->set_parent(x);
    }
    y->parent = x->parent;
    if (tree_is_left_child(x)) {
        x->parent->left = y;
    } else {
        x->parent_unsafe()->right = y;
    }
    y->right = x;
    x->set_parent(y);
}

template <class NodePtr>
void tree_balance_after_insert(NodePtr root, NodePtr x) {
    x->is_black = x == root;
    while (x != root && !x->parent_unsafe()->is_black) {
        if (tree_is_left_child(x->parent_unsafe())) {
            NodePtr y = x->parent_unsafe()->parent_unsafe()->right;
            if (y != 0 && !y->is_black) {
                x = x->parent_unsafe();
                x->is_black = true;
                x = x->parent_unsafe();
                x->is_black = x == root;
                y->is_black = true;
            } else {
                if (!tree_is_left_child(x)) {
                    x = x->parent_unsafe();
                    tree_left_rotate(x);
                }
                x = x->parent_unsafe();
                x->is_black = true;
                x = x->parent_unsafe();
                x->is_black = false;
                tree_right_rotate(x);
                break;
            }
        } else {
            NodePtr y = x->parent_unsafe()->parent->left;
            if (y != 0 && !y->is_black) {
                x = x->parent_unsafe();
                x->is_black = true;
                x = x->parent_unsafe();
                x->is_black = x == root;
                y->is_black = true;
            } else {
                if (tree_is_left_child(x)) {
                    x = x->parent_unsafe();
                    tree_right_rotate(x);
                }
                x = x->parent_unsafe();
                x->is_black = true;
                x = x->parent_unsafe();
                x->is_black = false;
                tree_left_rotate(x);
                break;
            }
        }
    }
}

template <class NodePtr>
void tree_remove(NodePtr root, NodePtr z) {
    NodePtr y = (z->left == 0 || z->right == 0) ? z : tree_next(z);
    NodePtr x = y->left != 0 ? y->left : y->right;
    NodePtr w = 0;
    if (x != 0) {
        x->parent = y->parent;
    }
    if (tree_is_left_child(y)) {
        y->parent->left = x;
        if (y != root) {
            w = y->parent_unsafe()->right;
        } else {
            root = x;
        }
    } else {
        y->parent_unsafe()->right = x;
        w = y->parent->left;
    }
    bool removed_black = y->is_black;
    if (y != z) {
        y->parent = z->parent;
        if (tree_is_left_child(z)) {
            y->parent->left = y;
        } else {
            y->parent_unsafe()->right = y;
        }
        y->left = z->left;
        y->left->set_parent(y);
        y->right = z->right;
        if (y->right != 0) {
            y->right->set_parent(y);
        }
        y->is_black = z->is_black;
        if (root == z) {
            root = y;
        }
    }
    if (removed_black && root != 0) {
        if (x != 0) {
            x->is_black = true;
        } else {
            while (true) {
                if (!tree_is_left_child(w)) {
                    if (!w->is_black) {
                        w->is_black = true;
                        w->parent_unsafe()->is_black = false;
                        tree_left_rotate(w->parent_unsafe());
                        if (root == w->left) {
                            root = w;
                        }
                        w = w->left->right;
                    }
                    if ((w->left  == 0 || w->left->is_black) && (w->right == 0 || w->right->is_black)) {
                        w->is_black = false;
                        x = w->parent_unsafe();
                        if (x == root || !x->is_black) {
                            x->is_black = true;
                            break;
                        }
                        w = tree_is_left_child(x) ? x->parent_unsafe()->right : x->parent->left;
                    } else {
                        if (w->right == 0 || w->right->is_black) {
                            w->left->is_black = true;
                            w->is_black = false;
                            tree_right_rotate(w);
                            w = w->parent_unsafe();
                        }
                        w->is_black = w->parent_unsafe()->is_black;
                        w->parent_unsafe()->is_black = true;
                        w->right->is_black = true;
                        tree_left_rotate(w->parent_unsafe());
                        break;
                    }
                } else {
                    if (!w->is_black) {
                        w->is_black = true;
                        w->parent_unsafe()->is_black = false;
                        tree_right_rotate(w->parent_unsafe());
                        if (root == w->right) {
                            root = w;
                        }
                        w = w->right->left;
                    }
                    if ((w->left  == 0 || w->left->is_black) && (w->right == 0 || w->right->is_black)) {
                        w->is_black = false;
                        x = w->parent_unsafe();
                        if (!x->is_black || x == root) {
                            x->is_black = true;
                            break;
                        }
                        w = tree_is_left_child(x) ? x->parent_unsafe()->right : x->parent->left;
                    } else {
                        if (w->left == 0 || w->left->is_black) {
                            w->right->is_black = true;
                            w->is_black = false;
                            tree_left_rotate(w);
                            w = w->parent_unsafe();
                        }
                        w->is_black = w->parent_unsafe()->is_black;
                        w->parent_unsafe()->is_black = true;
                        w->left->is_black = true;
                        tree_right_rotate(w->parent_unsafe());
                        break;
                    }
                }
            }
        }
    }
}

template <class Pointer> class tree_end_node;
template <class VoidPtr> class tree_node_base;
template <class T, class VoidPtr> class tree_node;

template <class VoidPtr>
struct tree_node_base_types {
  typedef VoidPtr void_pointer;
  typedef tree_node_base<void_pointer> node_base_type;
  typedef node_base_type* node_base_pointer;
  typedef tree_end_node<node_base_pointer> end_node_type;
  typedef end_node_type* end_node_pointer;
  typedef end_node_pointer parent_pointer;
};

template <class NodePtr, class NodeType = typename pointer_traits<NodePtr>::element_type>
struct tree_node_types;

template <class NodePtr, class T, class VoidPtr>
struct tree_node_types<NodePtr, tree_node<T, VoidPtr> >
    : public tree_node_base_types<VoidPtr>
{
    typedef tree_node_base_types<VoidPtr> base;

    typedef typename pointer_traits<NodePtr>::element_type node_type;
    typedef NodePtr node_pointer;

    typedef T node_value_type;
    typedef node_value_type* node_value_type_pointer;
    typedef const node_value_type* const_node_value_type_pointer;

    typedef typename base::end_node_pointer iter_pointer;
};

template <class T, class VoidPtr>
struct make_tree_node_types {
    typedef tree_node<T, VoidPtr>* NodePtr;
    typedef tree_node_types<NodePtr> type;
};

template <class Pointer>
class tree_end_node {
public:
    typedef Pointer pointer;
    pointer left;

    tree_end_node() : left() {}
};

template <class VoidPtr>
class tree_node_base : public tree_node_base_types<VoidPtr>::end_node_type {
    typedef tree_node_base_types<VoidPtr> node_base_types;

public:
    typedef typename node_base_types::node_base_pointer pointer;
    typedef typename node_base_types::parent_pointer parent_pointer;

    pointer right;
    parent_pointer parent;
    bool is_black;

    pointer parent_unsafe() const { return static_cast<pointer>(parent); }
    void set_parent(pointer p) { parent = static_cast<parent_pointer>(p); }

private:
    ~tree_node_base();
    tree_node_base(const tree_node_base&);
    tree_node_base& operator=(const tree_node_base&);
};

template <class T, class VoidPtr>
class tree_node : public tree_node_base<VoidPtr> {
public:
    typedef T node_value_type;

    node_value_type value;

private:
    ~tree_node();
    tree_node(const tree_node&);
    tree_node& operator=(const tree_node&);
};

template <class Allocator>
class tree_node_destructor {
    typedef Allocator allocator_type;

public:
    typedef typename allocator_type::pointer pointer;

private:
    allocator_type& alloc;

    tree_node_destructor& operator=(const tree_node_destructor&);

public:
    bool value_constructed;

    tree_node_destructor(const tree_node_destructor& other)
        : alloc(other.alloc)
        , value_constructed(other.value_constructed)
    {}

    explicit tree_node_destructor(allocator_type& alloc, bool val = false)
        : alloc(alloc)
        , value_constructed(val)
    {}

    void operator()(pointer p) {
        if (value_constructed) {
            alloc.destroy(&p->value);
        }
        if (p) {
            alloc.deallocate(p, 1);
        }
    }
};

template <class T, class NodePtr, class DiffType>
class tree_iterator {
    typedef tree_node_types<NodePtr> node_types;
    typedef NodePtr node_pointer;
    typedef typename node_types::node_base_pointer node_base_pointer;
    typedef typename node_types::end_node_pointer end_node_pointer;
    typedef typename node_types::iter_pointer iter_pointer;
    typedef pointer_traits<node_pointer> pointer_traits;

    iter_pointer ptr;

public:
    typedef std::bidirectional_iterator_tag iterator_category;
    typedef T value_type;
    typedef DiffType difference_type;
    typedef value_type& reference;
    typedef typename node_types::node_value_type_pointer pointer;

    tree_iterator() {}

    reference operator*() const { return get_np()->value; }
    pointer operator->() const { return &(operator*()); }

    tree_iterator& operator++() {
        ptr = static_cast<iter_pointer>(tree_next_iter<end_node_pointer>(static_cast<node_base_pointer>(ptr)));
        return *this;
    }

    tree_iterator operator++(int) {
        tree_iterator tmp(*this);
        ++(*this);
        return tmp;
    }

    tree_iterator& operator--() {
        ptr = static_cast<iter_pointer>(tree_prev_iter<node_base_pointer>(static_cast<end_node_pointer>(ptr)));
        return *this;
    }

    tree_iterator operator--(int) {
        tree_iterator tmp(*this);
        --(*this);
        return tmp;
    }

    friend bool operator==(const tree_iterator& x, const tree_iterator& y) {
        return x.ptr == y.ptr;
    }

    friend bool operator!=(const tree_iterator& x, const tree_iterator& y) {
        return !(x == y);
    }

private:
    explicit tree_iterator(node_pointer p) : ptr(p) {}
    explicit tree_iterator(end_node_pointer p) : ptr(p) {}

    node_pointer get_np() const {
        return static_cast<node_pointer>(ptr);
    }

    template <class, class, class> friend class tree_const_iterator;
    template <class, class, class> friend class tree;
    template <class, class, class, class> friend class map;
    template <class, class, class> friend class set;
};

template <class T, class NodePtr, class DiffType>
class tree_const_iterator {
    typedef tree_node_types<NodePtr> node_types;
    typedef typename node_types::node_pointer node_pointer;
    typedef typename node_types::node_base_pointer node_base_pointer;
    typedef typename node_types::end_node_pointer end_node_pointer;
    typedef typename node_types::iter_pointer iter_pointer;
    typedef pointer_traits<node_pointer> pointer_traits;

    iter_pointer ptr;

public:
    typedef std::bidirectional_iterator_tag iterator_category;
    typedef T value_type;
    typedef DiffType difference_type;
    typedef const value_type& reference;
    typedef typename node_types::const_node_value_type_pointer pointer;

    tree_const_iterator() {}

private:
    typedef tree_iterator<value_type, node_pointer, difference_type> non_const_iterator;

public:
    tree_const_iterator(non_const_iterator p) : ptr(p.ptr) {}

    reference operator*() const { return get_np()->value; }
    pointer operator->() const { &(operator*()); }

    tree_const_iterator& operator++() {
        ptr = static_cast<iter_pointer>(tree_next_iter<end_node_pointer>(static_cast<node_base_pointer>(ptr)));
        return *this;
    }

    tree_const_iterator operator++(int) {
        tree_const_iterator tmp(*this);
        ++(*this);
        return tmp;
    }

    tree_const_iterator& operator--() {
        ptr = static_cast<iter_pointer>(tree_prev_iter<node_base_pointer>(static_cast<end_node_pointer>(ptr)));
        return *this;
    }

    tree_const_iterator operator--(int) {
        tree_const_iterator tmp(*this);
        --(*this);
        return tmp;
    }

    friend bool operator==(const tree_const_iterator& x, const tree_const_iterator& y) {
        return x.ptr == y.ptr;
    }

    friend bool operator!=(const tree_const_iterator& x, const tree_const_iterator& y) {
        return !(x == y);
    }

private:
    explicit tree_const_iterator(node_pointer p) : ptr(p) {}
    explicit tree_const_iterator(end_node_pointer p) : ptr(p) {}

    node_pointer get_np() const {
        return static_cast<node_pointer>(ptr);
    }

    template <class, class, class> friend class tree;
    template <class, class, class, class> friend class map;
    template <class, class, class> friend class set;
};

template <class _Tp, class _Compare, class _Allocator>
class __tree
{
public:
    typedef _Tp                                      value_type;
    typedef _Compare                                 value_compare;
    typedef _Allocator                               allocator_type;

private:
    typedef allocator_traits<allocator_type>         __alloc_traits;
    typedef typename __make_tree_node_types<value_type,
        typename __alloc_traits::void_pointer>::type
                                                    _NodeTypes;
    typedef typename _NodeTypes::key_type           key_type;
public:
    typedef typename _NodeTypes::__node_value_type      __node_value_type;
    typedef typename _NodeTypes::__container_value_type __container_value_type;

    typedef typename __alloc_traits::pointer         pointer;
    typedef typename __alloc_traits::const_pointer   const_pointer;
    typedef typename __alloc_traits::size_type       size_type;
    typedef typename __alloc_traits::difference_type difference_type;

public:
    typedef typename _NodeTypes::__void_pointer        __void_pointer;

    typedef typename _NodeTypes::__node_type           __node;
    typedef typename _NodeTypes::__node_pointer        __node_pointer;

    typedef typename _NodeTypes::__node_base_type      __node_base;
    typedef typename _NodeTypes::__node_base_pointer   __node_base_pointer;

    typedef typename _NodeTypes::__end_node_type       __end_node_t;
    typedef typename _NodeTypes::__end_node_pointer    __end_node_ptr;

    typedef typename _NodeTypes::__parent_pointer      __parent_pointer;
    typedef typename _NodeTypes::__iter_pointer        __iter_pointer;

    typedef typename __rebind_alloc_helper<__alloc_traits, __node>::type __node_allocator;
    typedef allocator_traits<__node_allocator>         __node_traits;

private:
    // check for sane allocator pointer rebinding semantics. Rebinding the
    // allocator for a new pointer type should be exactly the same as rebinding
    // the pointer using 'pointer_traits'.
    static_assert((is_same<__node_pointer, typename __node_traits::pointer>::value),
                  "Allocator does not rebind pointers in a sane manner.");
    typedef typename __rebind_alloc_helper<__node_traits, __node_base>::type
        __node_base_allocator;
    typedef allocator_traits<__node_base_allocator> __node_base_traits;
    static_assert((is_same<__node_base_pointer, typename __node_base_traits::pointer>::value),
                 "Allocator does not rebind pointers in a sane manner.");

private:
    __iter_pointer                                     __begin_node_;
    __compressed_pair<__end_node_t, __node_allocator>  __pair1_;
    __compressed_pair<size_type, value_compare>        __pair3_;

public:
    _LIBCPP_INLINE_VISIBILITY
    __iter_pointer __end_node() _NOEXCEPT
    {
        return static_cast<__iter_pointer>(
                pointer_traits<__end_node_ptr>::pointer_to(__pair1_.first())
        );
    }
    _LIBCPP_INLINE_VISIBILITY
    __iter_pointer __end_node() const _NOEXCEPT
    {
        return static_cast<__iter_pointer>(
            pointer_traits<__end_node_ptr>::pointer_to(
                const_cast<__end_node_t&>(__pair1_.first())
            )
        );
    }
    _LIBCPP_INLINE_VISIBILITY
          __node_allocator& __node_alloc() _NOEXCEPT {return __pair1_.second();}
private:
    _LIBCPP_INLINE_VISIBILITY
    const __node_allocator& __node_alloc() const _NOEXCEPT
        {return __pair1_.second();}
    _LIBCPP_INLINE_VISIBILITY
          __iter_pointer& __begin_node() _NOEXCEPT {return __begin_node_;}
    _LIBCPP_INLINE_VISIBILITY
    const __iter_pointer& __begin_node() const _NOEXCEPT {return __begin_node_;}
public:
    _LIBCPP_INLINE_VISIBILITY
    allocator_type __alloc() const _NOEXCEPT
        {return allocator_type(__node_alloc());}
private:
    _LIBCPP_INLINE_VISIBILITY
          size_type& size() _NOEXCEPT {return __pair3_.first();}
public:
    _LIBCPP_INLINE_VISIBILITY
    const size_type& size() const _NOEXCEPT {return __pair3_.first();}
    _LIBCPP_INLINE_VISIBILITY
          value_compare& value_comp() _NOEXCEPT {return __pair3_.second();}
    _LIBCPP_INLINE_VISIBILITY
    const value_compare& value_comp() const _NOEXCEPT
        {return __pair3_.second();}
public:

    _LIBCPP_INLINE_VISIBILITY
    __node_pointer __root() const _NOEXCEPT
        {return static_cast<__node_pointer>(__end_node()->__left_);}

    __node_base_pointer* __root_ptr() const _NOEXCEPT {
        return _VSTD::addressof(__end_node()->__left_);
    }

    typedef __tree_iterator<value_type, __node_pointer, difference_type>             iterator;
    typedef __tree_const_iterator<value_type, __node_pointer, difference_type> const_iterator;

    explicit __tree(const value_compare& __comp)
        _NOEXCEPT_(
            is_nothrow_default_constructible<__node_allocator>::value &&
            is_nothrow_copy_constructible<value_compare>::value);
    explicit __tree(const allocator_type& __a);
    __tree(const value_compare& __comp, const allocator_type& __a);
    __tree(const __tree& __t);
    __tree& operator=(const __tree& __t);
    template <class _ForwardIterator>
        void __assign_unique(_ForwardIterator __first, _ForwardIterator __last);
    template <class _InputIterator>
        void __assign_multi(_InputIterator __first, _InputIterator __last);
    __tree(__tree&& __t)
        _NOEXCEPT_(
            is_nothrow_move_constructible<__node_allocator>::value &&
            is_nothrow_move_constructible<value_compare>::value);
    __tree(__tree&& __t, const allocator_type& __a);
    __tree& operator=(__tree&& __t)
        _NOEXCEPT_(
            __node_traits::propagate_on_container_move_assignment::value &&
            is_nothrow_move_assignable<value_compare>::value &&
            is_nothrow_move_assignable<__node_allocator>::value);
    ~__tree();

    _LIBCPP_INLINE_VISIBILITY
          iterator begin()  _NOEXCEPT {return       iterator(__begin_node());}
    _LIBCPP_INLINE_VISIBILITY
    const_iterator begin() const _NOEXCEPT {return const_iterator(__begin_node());}
    _LIBCPP_INLINE_VISIBILITY
          iterator end() _NOEXCEPT {return       iterator(__end_node());}
    _LIBCPP_INLINE_VISIBILITY
    const_iterator end() const _NOEXCEPT {return const_iterator(__end_node());}

    _LIBCPP_INLINE_VISIBILITY
    size_type max_size() const _NOEXCEPT
        {return _VSTD::min<size_type>(
                __node_traits::max_size(__node_alloc()),
                numeric_limits<difference_type >::max());}

    void clear() _NOEXCEPT;

    void swap(__tree& __t)
#if _LIBCPP_STD_VER <= 11
        _NOEXCEPT_(
            __is_nothrow_swappable<value_compare>::value
            && (!__node_traits::propagate_on_container_swap::value ||
                 __is_nothrow_swappable<__node_allocator>::value)
            );
#else
        _NOEXCEPT_(__is_nothrow_swappable<value_compare>::value);
#endif

    template <class _Key, class ..._Args>
    pair<iterator, bool>
    __emplace_unique_key_args(_Key const&, _Args&&... __args);
    template <class _Key, class ..._Args>
    pair<iterator, bool>
    __emplace_hint_unique_key_args(const_iterator, _Key const&, _Args&&...);

    template <class... _Args>
    pair<iterator, bool> __emplace_unique_impl(_Args&&... __args);

    template <class... _Args>
    iterator __emplace_hint_unique_impl(const_iterator __p, _Args&&... __args);

    template <class... _Args>
    iterator __emplace_multi(_Args&&... __args);

    template <class... _Args>
    iterator __emplace_hint_multi(const_iterator __p, _Args&&... __args);

    template <class _Pp>
    _LIBCPP_INLINE_VISIBILITY
    pair<iterator, bool> __emplace_unique(_Pp&& __x) {
        return __emplace_unique_extract_key(_VSTD::forward<_Pp>(__x),
                                            __can_extract_key<_Pp, key_type>());
    }

    template <class _First, class _Second>
    _LIBCPP_INLINE_VISIBILITY
    __enable_if_t<__can_extract_map_key<_First, key_type, __container_value_type>::value, pair<iterator, bool> >
    __emplace_unique(_First&& __f, _Second&& __s) {
        return __emplace_unique_key_args(__f, _VSTD::forward<_First>(__f),
                                              _VSTD::forward<_Second>(__s));
    }

    template <class... _Args>
    _LIBCPP_INLINE_VISIBILITY
    pair<iterator, bool> __emplace_unique(_Args&&... __args) {
        return __emplace_unique_impl(_VSTD::forward<_Args>(__args)...);
    }

    template <class _Pp>
    _LIBCPP_INLINE_VISIBILITY
    pair<iterator, bool>
    __emplace_unique_extract_key(_Pp&& __x, __extract_key_fail_tag) {
      return __emplace_unique_impl(_VSTD::forward<_Pp>(__x));
    }

    template <class _Pp>
    _LIBCPP_INLINE_VISIBILITY
    pair<iterator, bool>
    __emplace_unique_extract_key(_Pp&& __x, __extract_key_self_tag) {
      return __emplace_unique_key_args(__x, _VSTD::forward<_Pp>(__x));
    }

    template <class _Pp>
    _LIBCPP_INLINE_VISIBILITY
    pair<iterator, bool>
    __emplace_unique_extract_key(_Pp&& __x, __extract_key_first_tag) {
      return __emplace_unique_key_args(__x.first, _VSTD::forward<_Pp>(__x));
    }

    template <class _Pp>
    _LIBCPP_INLINE_VISIBILITY
    iterator __emplace_hint_unique(const_iterator __p, _Pp&& __x) {
        return __emplace_hint_unique_extract_key(__p, _VSTD::forward<_Pp>(__x),
                                            __can_extract_key<_Pp, key_type>());
    }

    template <class _First, class _Second>
    _LIBCPP_INLINE_VISIBILITY
    __enable_if_t<__can_extract_map_key<_First, key_type, __container_value_type>::value, iterator>
    __emplace_hint_unique(const_iterator __p, _First&& __f, _Second&& __s) {
        return __emplace_hint_unique_key_args(__p, __f,
                                              _VSTD::forward<_First>(__f),
                                              _VSTD::forward<_Second>(__s)).first;
    }

    template <class... _Args>
    _LIBCPP_INLINE_VISIBILITY
    iterator __emplace_hint_unique(const_iterator __p, _Args&&... __args) {
        return __emplace_hint_unique_impl(__p, _VSTD::forward<_Args>(__args)...);
    }

    template <class _Pp>
    _LIBCPP_INLINE_VISIBILITY
    iterator
    __emplace_hint_unique_extract_key(const_iterator __p, _Pp&& __x, __extract_key_fail_tag) {
      return __emplace_hint_unique_impl(__p, _VSTD::forward<_Pp>(__x));
    }

    template <class _Pp>
    _LIBCPP_INLINE_VISIBILITY
    iterator
    __emplace_hint_unique_extract_key(const_iterator __p, _Pp&& __x, __extract_key_self_tag) {
      return __emplace_hint_unique_key_args(__p, __x, _VSTD::forward<_Pp>(__x)).first;
    }

    template <class _Pp>
    _LIBCPP_INLINE_VISIBILITY
    iterator
    __emplace_hint_unique_extract_key(const_iterator __p, _Pp&& __x, __extract_key_first_tag) {
      return __emplace_hint_unique_key_args(__p, __x.first, _VSTD::forward<_Pp>(__x)).first;
    }

    _LIBCPP_INLINE_VISIBILITY
    pair<iterator, bool> __insert_unique(const __container_value_type& __v) {
        return __emplace_unique_key_args(_NodeTypes::__get_key(__v), __v);
    }

    _LIBCPP_INLINE_VISIBILITY
    iterator __insert_unique(const_iterator __p, const __container_value_type& __v) {
        return __emplace_hint_unique_key_args(__p, _NodeTypes::__get_key(__v), __v).first;
    }

    _LIBCPP_INLINE_VISIBILITY
    pair<iterator, bool> __insert_unique(__container_value_type&& __v) {
        return __emplace_unique_key_args(_NodeTypes::__get_key(__v), _VSTD::move(__v));
    }

    _LIBCPP_INLINE_VISIBILITY
    iterator __insert_unique(const_iterator __p, __container_value_type&& __v) {
        return __emplace_hint_unique_key_args(__p, _NodeTypes::__get_key(__v), _VSTD::move(__v)).first;
    }

    template <class _Vp,
              class = __enable_if_t<!is_same<typename __unconstref<_Vp>::type, __container_value_type>::value> >
    _LIBCPP_INLINE_VISIBILITY
    pair<iterator, bool> __insert_unique(_Vp&& __v) {
        return __emplace_unique(_VSTD::forward<_Vp>(__v));
    }

    template <class _Vp,
              class = __enable_if_t<!is_same<typename __unconstref<_Vp>::type, __container_value_type>::value> >
    _LIBCPP_INLINE_VISIBILITY
    iterator __insert_unique(const_iterator __p, _Vp&& __v) {
        return __emplace_hint_unique(__p, _VSTD::forward<_Vp>(__v));
    }

    _LIBCPP_INLINE_VISIBILITY
    iterator __insert_multi(__container_value_type&& __v) {
        return __emplace_multi(_VSTD::move(__v));
    }

    _LIBCPP_INLINE_VISIBILITY
    iterator __insert_multi(const_iterator __p, __container_value_type&& __v) {
        return __emplace_hint_multi(__p, _VSTD::move(__v));
    }

    template <class _Vp>
    _LIBCPP_INLINE_VISIBILITY
    iterator __insert_multi(_Vp&& __v) {
        return __emplace_multi(_VSTD::forward<_Vp>(__v));
    }

    template <class _Vp>
    _LIBCPP_INLINE_VISIBILITY
    iterator __insert_multi(const_iterator __p, _Vp&& __v) {
        return __emplace_hint_multi(__p, _VSTD::forward<_Vp>(__v));
    }

    _LIBCPP_INLINE_VISIBILITY
    pair<iterator, bool> __node_assign_unique(const __container_value_type& __v, __node_pointer __dest);

    _LIBCPP_INLINE_VISIBILITY
    iterator __node_insert_multi(__node_pointer __nd);
    _LIBCPP_INLINE_VISIBILITY
    iterator __node_insert_multi(const_iterator __p, __node_pointer __nd);


    _LIBCPP_INLINE_VISIBILITY iterator
    __remove_node_pointer(__node_pointer) _NOEXCEPT;

#if _LIBCPP_STD_VER > 14
    template <class _NodeHandle, class _InsertReturnType>
    _LIBCPP_INLINE_VISIBILITY
    _InsertReturnType __node_handle_insert_unique(_NodeHandle&&);
    template <class _NodeHandle>
    _LIBCPP_INLINE_VISIBILITY
    iterator __node_handle_insert_unique(const_iterator, _NodeHandle&&);
    template <class _Tree>
    _LIBCPP_INLINE_VISIBILITY
    void __node_handle_merge_unique(_Tree& __source);

    template <class _NodeHandle>
    _LIBCPP_INLINE_VISIBILITY
    iterator __node_handle_insert_multi(_NodeHandle&&);
    template <class _NodeHandle>
    _LIBCPP_INLINE_VISIBILITY
    iterator __node_handle_insert_multi(const_iterator, _NodeHandle&&);
    template <class _Tree>
    _LIBCPP_INLINE_VISIBILITY
    void __node_handle_merge_multi(_Tree& __source);


    template <class _NodeHandle>
    _LIBCPP_INLINE_VISIBILITY
    _NodeHandle __node_handle_extract(key_type const&);
    template <class _NodeHandle>
    _LIBCPP_INLINE_VISIBILITY
    _NodeHandle __node_handle_extract(const_iterator);
#endif

    iterator erase(const_iterator __p);
    iterator erase(const_iterator __f, const_iterator __l);
    template <class _Key>
        size_type __erase_unique(const _Key& __k);
    template <class _Key>
        size_type __erase_multi(const _Key& __k);

    void __insert_node_at(__parent_pointer     __parent,
                          __node_base_pointer& __child,
                          __node_base_pointer __new_node) _NOEXCEPT;

    template <class _Key>
        iterator find(const _Key& __v);
    template <class _Key>
        const_iterator find(const _Key& __v) const;

    template <class _Key>
        size_type __count_unique(const _Key& __k) const;
    template <class _Key>
        size_type __count_multi(const _Key& __k) const;

    template <class _Key>
        _LIBCPP_INLINE_VISIBILITY
        iterator lower_bound(const _Key& __v)
            {return __lower_bound(__v, __root(), __end_node());}
    template <class _Key>
        iterator __lower_bound(const _Key& __v,
                               __node_pointer __root,
                               __iter_pointer __result);
    template <class _Key>
        _LIBCPP_INLINE_VISIBILITY
        const_iterator lower_bound(const _Key& __v) const
            {return __lower_bound(__v, __root(), __end_node());}
    template <class _Key>
        const_iterator __lower_bound(const _Key& __v,
                                     __node_pointer __root,
                                     __iter_pointer __result) const;
    template <class _Key>
        _LIBCPP_INLINE_VISIBILITY
        iterator upper_bound(const _Key& __v)
            {return __upper_bound(__v, __root(), __end_node());}
    template <class _Key>
        iterator __upper_bound(const _Key& __v,
                               __node_pointer __root,
                               __iter_pointer __result);
    template <class _Key>
        _LIBCPP_INLINE_VISIBILITY
        const_iterator upper_bound(const _Key& __v) const
            {return __upper_bound(__v, __root(), __end_node());}
    template <class _Key>
        const_iterator __upper_bound(const _Key& __v,
                                     __node_pointer __root,
                                     __iter_pointer __result) const;
    template <class _Key>
        pair<iterator, iterator>
        __equal_range_unique(const _Key& __k);
    template <class _Key>
        pair<const_iterator, const_iterator>
        __equal_range_unique(const _Key& __k) const;

    template <class _Key>
        pair<iterator, iterator>
        __equal_range_multi(const _Key& __k);
    template <class _Key>
        pair<const_iterator, const_iterator>
        __equal_range_multi(const _Key& __k) const;

    typedef __tree_node_destructor<__node_allocator> _Dp;
    typedef unique_ptr<__node, _Dp> __node_holder;

    __node_holder remove(const_iterator __p) _NOEXCEPT;
private:
    __node_base_pointer&
        __find_leaf_low(__parent_pointer& __parent, const key_type& __v);
    __node_base_pointer&
        __find_leaf_high(__parent_pointer& __parent, const key_type& __v);
    __node_base_pointer&
        __find_leaf(const_iterator __hint,
                    __parent_pointer& __parent, const key_type& __v);
    // FIXME: Make this function const qualified. Unfortunately doing so
    // breaks existing code which uses non-const callable comparators.
    template <class _Key>
    __node_base_pointer&
        __find_equal(__parent_pointer& __parent, const _Key& __v);
    template <class _Key>
    _LIBCPP_INLINE_VISIBILITY __node_base_pointer&
    __find_equal(__parent_pointer& __parent, const _Key& __v) const {
      return const_cast<__tree*>(this)->__find_equal(__parent, __v);
    }
    template <class _Key>
    __node_base_pointer&
        __find_equal(const_iterator __hint, __parent_pointer& __parent,
                     __node_base_pointer& __dummy,
                     const _Key& __v);

    template <class ..._Args>
    __node_holder __construct_node(_Args&& ...__args);

    void destroy(__node_pointer __nd) _NOEXCEPT;

    _LIBCPP_INLINE_VISIBILITY
    void __copy_assign_alloc(const __tree& __t)
        {__copy_assign_alloc(__t, integral_constant<bool,
             __node_traits::propagate_on_container_copy_assignment::value>());}

    _LIBCPP_INLINE_VISIBILITY
    void __copy_assign_alloc(const __tree& __t, true_type)
        {
        if (__node_alloc() != __t.__node_alloc())
            clear();
        __node_alloc() = __t.__node_alloc();
        }
    _LIBCPP_INLINE_VISIBILITY
    void __copy_assign_alloc(const __tree&, false_type) {}

    void __move_assign(__tree& __t, false_type);
    void __move_assign(__tree& __t, true_type)
        _NOEXCEPT_(is_nothrow_move_assignable<value_compare>::value &&
                   is_nothrow_move_assignable<__node_allocator>::value);

    _LIBCPP_INLINE_VISIBILITY
    void __move_assign_alloc(__tree& __t)
        _NOEXCEPT_(
            !__node_traits::propagate_on_container_move_assignment::value ||
            is_nothrow_move_assignable<__node_allocator>::value)
        {__move_assign_alloc(__t, integral_constant<bool,
             __node_traits::propagate_on_container_move_assignment::value>());}

    _LIBCPP_INLINE_VISIBILITY
    void __move_assign_alloc(__tree& __t, true_type)
        _NOEXCEPT_(is_nothrow_move_assignable<__node_allocator>::value)
        {__node_alloc() = _VSTD::move(__t.__node_alloc());}
    _LIBCPP_INLINE_VISIBILITY
    void __move_assign_alloc(__tree&, false_type) _NOEXCEPT {}

    struct _DetachedTreeCache {
      _LIBCPP_INLINE_VISIBILITY
      explicit _DetachedTreeCache(__tree *__t) _NOEXCEPT : __t_(__t),
        __cache_root_(__detach_from_tree(__t)) {
          __advance();
        }

      _LIBCPP_INLINE_VISIBILITY
      __node_pointer __get() const _NOEXCEPT {
        return __cache_elem_;
      }

      _LIBCPP_INLINE_VISIBILITY
      void __advance() _NOEXCEPT {
        __cache_elem_ = __cache_root_;
        if (__cache_root_) {
          __cache_root_ = __detach_next(__cache_root_);
        }
      }

      _LIBCPP_INLINE_VISIBILITY
      ~_DetachedTreeCache() {
        __t_->destroy(__cache_elem_);
        if (__cache_root_) {
          while (__cache_root_->__parent_ != nullptr)
            __cache_root_ = static_cast<__node_pointer>(__cache_root_->__parent_);
          __t_->destroy(__cache_root_);
        }
      }

       _DetachedTreeCache(_DetachedTreeCache const&) = delete;
       _DetachedTreeCache& operator=(_DetachedTreeCache const&) = delete;

    private:
      _LIBCPP_INLINE_VISIBILITY
      static __node_pointer __detach_from_tree(__tree *__t) _NOEXCEPT;
      _LIBCPP_INLINE_VISIBILITY
      static __node_pointer __detach_next(__node_pointer) _NOEXCEPT;

      __tree *__t_;
      __node_pointer __cache_root_;
      __node_pointer __cache_elem_;
    };


    template <class, class, class, class> friend class _LIBCPP_TEMPLATE_VIS map;
    template <class, class, class, class> friend class _LIBCPP_TEMPLATE_VIS multimap;
};

// template <class Tr>
// class Tree_nod : public Tr {
// protected:
//     typedef typename Tr::allocator_type allocator_type;
//     typedef typename Tr::key_compare key_compare;
//     typedef typename Tr::value_type value_type;
//     typedef typename allocator_type::template rebind<void>::other::pointer Genptr;

//     struct Node;
//     friend struct Node;
//     struct Node {
//         Genptr Left, Parent, Right;
//         value_type Value;
//         char Color, Isnil;
//     };

//     Tree_nod(const key_compare& Parg, allocator_type Al)
//         : Tr(Parg), Alnod(Al)
//     {}

//     typename allocator_type::temlpate rebind<Node>::other Alnod;
// };

// template <class Tr>
// class Tree_ptr : public Tree_nod<Tr> {
// protected:
//     typedef typename Tree_node<Tr>::Node Node;
//     typedef typename Tr::allocator_type allocator_type;
//     typedef typename Tr::key_compare key_compare;
//     typedef typename allocator_type::template rebind<Node>::other::pointer Nodeptr;

//     Tree_ptr(const key_compare& Parg, allocator_type Al)
//         : Tree_nod<Tr>(Parg, Al), Alptr(Al)
//     {}

//     typename allocator_type::template rebind<Nodeptr>::other Alptr;
// };

// template <class Tr>
// class Tree_val : public Tree_ptr<Tr> {
// protected:
//     typedef typename Tr::allocator_type allocator_type;
//     typedef typename Tr::key_compare key_compare;

//     Tree_val(const key_compare& Parg, allocator_type Al)
//         : Tree_ptr<Tr>(Parg, Al), Alval(Al)
//     {}

//     allocator_type Alval;
// };

// template <class Tr>
// class Tree : public Tree_val<Tr> {
// public:
//     typedef Tree<Tr> Myt;
//     typedef Tree_val<Tr> Mybase;
//     typedef typename Tr::key_type key_type;
//     typedef typename Tr::key_compare key_compare;
//     typedef typename Tr::value_compare value_compare;
//     typedef typename Tr::value_type value_type;
//     typedef typename Tr::allocator_type allocator_type;

// protected:
//     typedef typename Tree_nod<Tr>::Genptr Genptr;
//     typedef typename Tree_nod<Tr>::Node Node;

//     enum Redbl {Red, Black};

//     typedef typename allocator_type::template rebind<Node>::other::pointer Nodeptr;
//     typedef typename allocator_type::template rebind<Nodeptr>::other::reference Nodepref;
//     typedef typename allocator_type::template rebind<key_type>::other::const_reference Keyref;
//     typedef typename allocator_type::template rebind<char>::other::reference Charref;
//     typedef typename allocator_type::template rebind<value_type>::other::reference Vref;

//     static Charref Color(Nodeptr P) { return static_cast<Charref>(P->Color); }
//     static Charref Isnil(Nodeptr P) { return static_cast<Charref>(P->Isnil); }
//     static Keyref Key(Nodeptr P) { return Kfn()(Value(P)); }
//     static Nodepref Left(Nodeptr P) { return static_cast<Nodepref>(P->Left); }
//     static Nodepref Right(Nodeptr P) { return static_cast<Nodepref>(P->Right); }
//     static Nodepref Parent(Nodeptr P) { return static_cast<Nodepref>(P->Parent); }
//     static Vref Value(Nodeptr P) { return static_cast<Vref>(P->Value); }

// public:
//     typedef typename allocator_type::size_type size_type;
//     typedef typename allocator_type::difference_type Dift;
//     typedef Dift difference_type;
//     typedef typename allocator_type::template rebind<value_type>::other::pointer Tptr;
//     typedef typename allocator_type::template rebind<value_type>::other::const_pointer Ctptr;
//     typedef typename allocator_type::template rebind<value_type>::other::reference Reft;
//     typedef Tptr pointer;
//     typedef Ctptr const_pointer;
//     typedef Reft reference;
//     typedef typename allocator_type::template rebind<value_type>::other::const_reference const_reference;

//     class iterator;
//     friend class iterator;
//     class iterator
//         : public std::iterator<std::bidirectional_iterator_tag, value_type, Dift, Tptr, Reft> {
//     public:
//         typedef std::iterator<std::bidirectional_iterator_tag, value_type, Dift, Tptr, Reft> Mybase;
//         typedef typename Mybase::iterator_category iterator_category;
//         typedef typename Mybase::value_type value_type;
//         typedef typename Mybase::difference_type difference_type;
//         typedef typename Mybase::pointer pointer;
//         typedef typename Mybase::reference reference;

//     protected:
//         Nodeptr Ptr;

//     public:
//         iterator() : Ptr(NULL) {}
//         iterator(Nodeptr P) : Ptr(P) {}
//         reference operator*() const { return Value(Ptr); }
//         Tptr operator->() const { &**this; }
//         iterator& operator++() { Inc(); return *this; }
//         iterator operator++(int) { iterator Tmp(*this); ++*this; return Tmp; }
//         iterator& operator--() { Dec(); return *this; }
//         iterator operator--(int) { iterator Tmp(*this); --*this; return Tmp; }

//         bool operator==(const iterator& x) const { return Ptr == x.Ptr; }
//         bool operator!=(const iterator& x) const { return !(*this == x); }

//         void Dec() {
//             if (Isnil(Ptr)) {
//                 Ptr = Right(Ptr);
//             } else if (!Isnil(Left(Ptr))) {
//                 Ptr = Max(Left(Ptr));
//             } else {
//                 Nodeptr P;
//                 while (!Isnil(P = Parent(Ptr)) && Ptr == Left(P)) {
//                     Ptr = P;
//                 }
//                 if (!Isnil(P)) {
//                     Ptr = P;
//                 }
//             }
//         }

//         void Inc() {
//             if (Isnil(Ptr)) {
//                 // end node
//             } else if (!Isnil(Right(Ptr))) {
//                 Ptr = Min(Right(Ptr));
//             } else {
//                 Nodeptr P;
//                 while (!Isnil(P = Parent(Ptr)) && Ptr == Right(P)) {
//                     Ptr = P;
//                 }
//                 Ptr = P;
//             }
//         }

//         Nodeptr Mynode() const { return Ptr; }
//     };


//     class const_iterator;
//     friend class const_iterator;
//     class const_iterator
//         : public std::iterator<std::bidirectional_iterator_tag, value_type, Dift, Ctptr, const_reference> {
//     public:
//         typedef std::iterator<std::bidirectional_iterator_tag, value_type, Dift, Ctptr, const_reference> Mybase;
//         typedef typename Mybase::iterator_category iterator_category;
//         typedef typename Mybase::value_type value_type;
//         typedef typename Mybase::difference_type difference_type;
//         typedef typename Mybase::pointer pointer;
//         typedef typename Mybase::reference reference;

//     protected:
//         Nodeptr Ptr;

//     public:
//         const_iterator() : Ptr(NULL) {}
//         const_iterator(Nodeptr P) : Ptr(P) {}
//         const_iterator(const typename Tree<Tr>::iterator& x) : Ptr(x.Mynode()) {}
//         const_reference operator*() const { return Value(Ptr); }
//         Ctptr operator->() const { &**this; }
//         const_iterator& operator++() { Inc(); return *this; }
//         const_iterator operator++(int) { const_iterator Tmp(*this); ++*this; return Tmp; }
//         const_iterator& operator--() { Dec(); return *this; }
//         const_iterator operator--(int) { const_iterator Tmp(*this); --*this; return Tmp; }

//         bool operator==(const const_iterator& x) const { return Ptr == x.Ptr; }
//         bool operator!=(const const_iterator& x) const { return !(*this == x); }

//         void Dec() {
//             if (Isnil(Ptr)) {
//                 Ptr = Right(Ptr);
//             } else if (!Isnil(Left(Ptr))) {
//                 Ptr = Max(Left(Ptr));
//             } else {
//                 Nodeptr P;
//                 while (!Isnil(P = Parent(Ptr)) && Ptr == Left(P)) {
//                     Ptr = P;
//                 }
//                 if (!Isnil(P)) {
//                     Ptr = P;
//                 }
//             }
//         }

//         void Inc() {
//             if (Isnil(Ptr)) {
//                 // end node
//             } else if (!Isnil(Right(Ptr))) {
//                 Ptr = Min(Right(Ptr));
//             } else {
//                 Nodeptr P;
//                 while (!Isnil(P = Parent(Ptr)) && Ptr == Right(P)) {
//                     Ptr = P;
//                 }
//                 Ptr = P;
//             }
//         }

//         Nodeptr Mynode() const { return Ptr; }
//     };

//     typedef ft::reverse_iterator<iterator> reverse_iterator;
//     typedef ft::reverse_iterator<const_iterator> const_reverse_iterator;
//     typedef pair<iterator, bool> Pairib;
//     typedef pair<iterator, iterator> Pairii;
//     typedef pair<const_iterator, const_iterator> Paircc;


// };











// template <class Value, class Compare, class Allocator>
// class tree {
// public:
//     typedef Value value_type;
//     typedef Compare value_compare;
//     typedef Allocator allocator_type;
//     // typedef typename allocator_type::reference reference;
//     // typedef typename allocator_type::const_reference const_reference;
//     // typedef typename allocator_type::pointer pointer;
//     // typedef typename allocator_type::const_pointer const_pointer;

//     class Node;

//     typedef Node<value_type> node_type;
//     typedef typename allocator_type::template rebind<node_type>::other node_allocator;
//     typedef typename node_allocator::pointer node_pointer;

//     typedef std::ptrdiff_t difference_type;
//     typedef std::size_t size_type;
//     typedef TreeIter<Value> iterator;
//     typedef TreeIter<const Value> const_iterator;
//     typedef	ft::reverse_iterator<iterator> reverse_iterator;
//     typedef	ft::reverse_iterator<const_iterator> const_reverse_iterator;

// };

} // namespace impl
} // namespace ft
