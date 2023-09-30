#pragma once

#include "type_list_hdr.hh"
#include "type_list_impl.hh"

namespace kuba
{

template<typename... Ts>
struct type_list
{
    using self_type = type_list<Ts...>;

    // checks
    static constexpr auto size = type_list_size_v<self_type>;

    static constexpr auto is_empty = type_list_is_empty_v<self_type>;

    template<template<typename> typename Predicate>
    static constexpr auto count_if = type_list_count_if_v<self_type, Predicate>;

    template<typename... Us>
    static constexpr auto count = type_list_count_v<self_type, Us...>;

    template<typename T, template<typename, typename> typename IsSame = std::is_same>
    static constexpr auto contains = type_list_contains_v<self_type, T, IsSame>;

    template<template<typename, typename> typename IsSame = std::is_same>
    static constexpr auto is_unique = type_list_is_unique_v<self_type, IsSame>;

    template<template<typename> typename Predicate>
    static constexpr bool all_of = type_list_all_of_v<self_type, Predicate>;

    template<template<typename> typename Predicate>
    static constexpr auto any_of = type_list_any_of_v<self_type, Predicate>;

    template<template<typename> typename Predicate>
    static constexpr auto none_of = type_list_none_of_v<self_type, Predicate>;

    template<std::size_t K, template<typename, typename> typename Predicate>
      requires(K == 2) // sorry, current limitation
    static constexpr bool all_of_combinations = type_list_all_of_combinations_v<self_type, K, Predicate>;

    // access
#if 0
    using first = type_list_first_t<self_type>;

    using last = type_list_last_t<self_type>;
#endif

    template<std::size_t Index>
        requires( Index < type_list_size_v<self_type> )
    using at = type_list_at_t<self_type, Index>;

    template<std::size_t... Indicies>
        requires( (Indicies < type_list_size_v<self_type>) && ... )
    using sublist = type_list_sublist_t<self_type, Indicies...>;

    /*
    template<template<typename> typename Predicate>
    using sublist_if = 

    template<template<typename> typename Predicate>
    static constexpr auto first_of = ...;

    template<template<typename> typename Predicate>
    static constexpr auto last_of = ...;

    template<template<typename> typename Predicate>
    static constexpr auto lowest = ...;

    template<template<typename> typename Predicate>
    static constexpr auto highest = ...;

    template<template<typename> typename Predicate>
    static constexpr auto is_sorted = ...;

    template<template<typename> typename Predicate>
    using sorted = 

    template<typename T>
    requires (
    )
    static constexpr auto index_of = type_list_index_of<self_type, T>;

    */

    template<std::size_t>
        requires ( type_list_is_unique_v<self_type> )
    static constexpr auto index = -1; // TODO

    // modify
    template<any_type_list... TypeLists>
    using concat = type_list_concat_t<self_type, TypeLists...>;

    template<typename... Us>
    using append = type_list_append_t<self_type, Us...>;

    template<std::size_t Index, typename... Us>
    using insert = type_list_insert_t<self_type, Index, Us...>;

    template<template<typename>typename Predicate>
    using erase_if = type_list_erase_if_t<self_type, Predicate>;

    template<std::size_t Index>
        requires ( Index < type_list_size_v<self_type> )
    using erase_at = type_list_erase_at_t<self_type, Index>;

    template<typename... Us>
    using erase = type_list_erase_t<self_type, Us...>;

    template<std::size_t Index>
        requires ( Index <= type_list_size_v<self_type> )
    using split = type_list_split_t<self_type, Index>;


    template<template<typename> typename Transformer>
    using transform = type_list_transform_t<self_type, Transformer>;

    template<template<typename> typename ConvertType>
    using convert = type_list_convert_t<self_type, ConvertType>;

    template<template<typename> typename... ExtendTypes>
    using extend = type_list_extend_t<self_type, ExtendTypes...>;

    template<typename OldType, typename NewType>
    using replace = type_list_replace_t<self_type, OldType, NewType>;


    // algo
    
    template<template<typename> typename Predicate>
    using find_all = type_list_find_all_t<self_type, Predicate>;

    template<template<typename, typename> typename IsSame = std::is_same>
    using unique = type_list_unique_t<self_type, IsSame>;

    using reverse = type_list_reverse_t<self_type>;

    template<std::size_t K>
      requires(K == 2) // sorry, current limitation
    using combinations = type_list_combinations_t<self_type, K>;

    template<any_type_list TypeList>
    using tie = type_list_tie<self_type, TypeList>;

    // functional
    template<template<typename...> typename T>
    using apply = type_list_apply_t<self_type, T>;
};



////////////
template<typename TypeList, template<typename>typename Predicate>
concept any_type_list_of = is_type_list_v<TypeList> && type_list_all_of_v<TypeList, Predicate>;

}
