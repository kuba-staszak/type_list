#pragma once

#include <type_traits>

namespace kuba
{

template<typename... Ts>
struct type_list;

template<typename T>
struct is_type_list : std::false_type{};

template<typename... Ts>
struct is_type_list< type_list<Ts...> > : std::true_type { };

template<typename T>
inline constexpr auto is_type_list_v = is_type_list<T>::value;

template<typename T>
concept any_type_list = is_type_list_v<T>;

}
