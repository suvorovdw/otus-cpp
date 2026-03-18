#pragma once

#include <cstdint>
#include <iterator>
#include <ostream>
#include <iostream>
#include <string>
#include <tuple>
#include <type_traits>

namespace my
{
namespace meta
{
template <class...>
using void_t = void;

template <class T, class = void>
struct is_range : std::false_type
{
};

template <class T>
struct is_range<T, void_t<decltype(std::begin(std::declval<const T&>())),
                          decltype(std::end(std::declval<const T&>()))>> : std::true_type
{
};

template <class T>
inline constexpr bool is_range_v = is_range<T>::value;

template <class T>
struct is_string : std::is_same<std::decay_t<T>, std::string>
{
};

template <class T>
inline constexpr bool is_string_v = is_string<T>::value;

template <class... Ts>
struct all_same;

template <>
struct all_same<> : std::true_type
{
};

template <class T>
struct all_same<T> : std::true_type
{
};

template <class T, class U, class... Rest>
struct all_same<T, U, Rest...>
    : std::conditional_t<std::is_same_v<T, U>, all_same<U, Rest...>, std::false_type>
{
};

template <class... Ts>
inline constexpr bool all_same_v = all_same<Ts...>::value;
} // namespace meta

template <class Integer, std::enable_if_t<std::is_integral_v<Integer>, int> = 0>
void print_ip(Integer ip, std::ostream& os)
{
    using U = std::make_unsigned_t<Integer>;
    const U u = static_cast<U>(ip);

    for (std::size_t i = sizeof(U); i-- > 0;)
    {
        const auto byte = static_cast<unsigned>((u >> (i * 8)) & static_cast<U>(0xFF));
        os << byte;
        if (i != 0) os << '.';
    }
}

template <class Integer, std::enable_if_t<std::is_integral_v<Integer>, int> = 0>
void print_ip(Integer ip)
{
    print_ip(ip, std::cout);
}

template <class String, std::enable_if_t<meta::is_string_v<String>, int> = 0>
void print_ip(const String& ip, std::ostream& os)
{
    os << ip;
}

template <class String, std::enable_if_t<meta::is_string_v<String>, int> = 0>
void print_ip(const String& ip)
{
    print_ip(ip, std::cout);
}

template <class Container,
          std::enable_if_t<meta::is_range_v<Container> && !meta::is_string_v<Container>, int> = 0>
void print_ip(const Container& ip, std::ostream& os)
{
    auto it = std::begin(ip);
    const auto end = std::end(ip);
    if (it == end) return;

    os << *it;
    for (++it; it != end; ++it)
        os << '.' << *it;
}

template <class Container,
          std::enable_if_t<meta::is_range_v<Container> && !meta::is_string_v<Container>, int> = 0>
void print_ip(const Container& ip)
{
    print_ip(ip, std::cout);
}

namespace detail
{
template <class Tuple, std::size_t... I>
void print_tuple_impl(const Tuple& t, std::ostream& os, std::index_sequence<I...>)
{
    const auto print_one = [&](auto idx_const)
    {
        constexpr std::size_t idx = decltype(idx_const)::value;
        if constexpr (idx == 0)
            os << std::get<0>(t);
        else
            os << '.' << std::get<idx>(t);
    };

    (print_one(std::integral_constant<std::size_t, I>{}), ...);
}
} // namespace detail

template <class... Args, std::enable_if_t<meta::all_same_v<Args...>, int> = 0>
void print_ip(const std::tuple<Args...>& ip, std::ostream& os)
{
    if constexpr (sizeof...(Args) == 0)
        return;
    else
        detail::print_tuple_impl(ip, os, std::index_sequence_for<Args...>{});
}

template <class... Args, std::enable_if_t<meta::all_same_v<Args...>, int> = 0>
void print_ip(const std::tuple<Args...>& ip)
{
    print_ip(ip, std::cout);
}

} // namespace my

