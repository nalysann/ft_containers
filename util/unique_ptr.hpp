#pragma once

#include <utility>

namespace ft::impl {

template <class T>
struct default_delete {
    default_delete() {}

    void operator()(T* ptr) const {
        delete ptr;
    }
};

template <class T, class D = default_delete<T> >
class unique_ptr {
public:
    typedef T* pointer;
    typedef T element_type;
    typedef D deleter_type;

private:
    pointer ptr;
    deleter_type d;

public:
    unique_ptr() : ptr(), d() {}
    explicit unique_ptr(pointer p) : ptr(p), d() {}
    unique_ptr(pointer p, deleter_type d) : ptr(p), d() {}
    ~unique_ptr() { reset(); }

    element_type& operator*() const {
        return *get();
    }

    pointer operator->() const {
        return get();
    }

    pointer get() const {
        return ptr;
    }

    deleter_type& get_deleter() {
        return d;
    }

    const deleter_type& get_deleter() const {
        return d;
    }

    explicit operator bool() const {
        return get() != 0;
    }

    pointer release() {
        pointer p = get();
        ptr = 0;
        return p;
    }

    void reset(pointer p = pointer()) {
        if (p != get()) {
            get_deleter()(get());
            ptr = p;
        }
    }

    void swap(unique_ptr& u) {
        using std::swap;
        swap(ptr, u.ptr);
        swap(d, u.d);
    }

private:
    unique_ptr(const unique_ptr&);
    unique_ptr& operator=(const unique_ptr&);
};

template <class T, class D>
void swap(unique_ptr<T, D>& x, unique_ptr<T, D>& y){
    x.swap(y);
}

template <class T, class TD, class U, class UD>
bool operator==(const unique_ptr<T, TD>& x, const unique_ptr<U, UD>& y) {
    return x.get() == y.get();
}

template <class T, class TD, class U, class UD>
bool operator!=(const unique_ptr<T, TD>& x, const unique_ptr<U, UD>& y) {
    return !(x.get() == y.get());
}

template <class T, class TD, class U, class UD>
bool operator<(const unique_ptr<T, TD>& x, const unique_ptr<U, UD>& y) {
    return x.get() < y.get();
}

template <class T, class TD, class U, class UD>
bool operator<=(const unique_ptr<T, TD>& x, const unique_ptr<U, UD>& y) {
    return !(y.get() < x.get());
}

template <class T, class TD, class U, class UD>
bool operator>(const unique_ptr<T, TD>& x, const unique_ptr<U, UD>& y) {
    return y.get() < x.get();
}

template <class T, class TD, class U, class UD>
bool operator>=(const unique_ptr<T, TD>& x, const unique_ptr<U, UD>& y) {
    return !(x.get() < y.get());
}

} // namespace ft::impl
