#pragma once

#include "util/integral_constant.hpp"
#include "util/remove_cv.hpp"

namespace ft::impl {

template <class>
struct is_integral_helper : public false_type {};

template <>
struct is_integral_helper<bool> : public true_type {};

template <>
struct is_integral_helper<char> : public true_type {};

template <>
struct is_integral_helper<signed char> : public true_type {};

template <>
struct is_integral_helper<unsigned char> : public true_type {};

template <>
struct is_integral_helper<wchar_t> : public true_type {};

template <>
struct is_integral_helper<short> : public true_type {};

template <>
struct is_integral_helper<unsigned short> : public true_type {};

template <>
struct is_integral_helper<int> : public true_type {};

template <>
struct is_integral_helper<unsigned int> : public true_type {};

template <>
struct is_integral_helper<long> : public true_type {};

template <>
struct is_integral_helper<unsigned long> : public true_type {};

template <>
struct is_integral_helper<long long> : public true_type {};

template <>
struct is_integral_helper<unsigned long long> : public true_type {};

template <>
struct is_integral_helper<__int128_t> : public true_type {};

template <>
struct is_integral_helper<__uint128_t> : public true_type {};

} // namespace ft::impl

namespace ft {

template <class T>
struct is_integral : public impl::is_integral_helper<typename remove_cv<T>::type>::type {};

} // namespace ft
