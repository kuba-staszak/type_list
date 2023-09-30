#pragma once

#include "type_list.hh"

namespace kuba
{

namespace impl
{
template<typename T, T Begin, T End>
struct iota_type_list
{
    using type = typename type_list<
        std::integral_constant<T, Begin>
    >::template concat<
        typename iota_type_list<T, Begin + 1, End>::type
    >;
};

template<typename T, T End>
struct iota_type_list<T, End, End>
{
    using type = type_list<>;
};

}


template<typename T, T End>
using iota_type_list = impl::iota_type_list<T, 0, End>;

template<typename T, T End>
using iota_type_list_t = typename iota_type_list<T, End>::type;








template< template<typename...> class T >
struct applied_type_list
{
    template<any_type_list List>
    using type = typename List::template apply<T>;
};


// NOTE: the name of the trait doesn't start with "type_list", as this is not an operation
//       on the typelist itself. It is more like a constructor :)
// extract_type_list

template<typename T>
using extract_type_list = impl::extract_type_list<T>;

template<typename T>
using extract_type_list_t = typename extract_type_list<T>::type;



}
