#pragma once

#include "remove_const.hpp"
#include "remove_volatile.hpp"

namespace ft {

template <class T>
struct remove_cv {
    typedef typename remove_volatile<typename remove_const<T>::type>::type type;
};

} // namespace ft
