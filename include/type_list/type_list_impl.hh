#pragma once

#include "type_list_hdr.hh"
#include <utility>

namespace kuba
{

namespace impl
{
    template<std::size_t Value>
    using index_constant = std::integral_constant<std::size_t, Value>;
}

namespace helper
{
    template<typename... T>
    struct is_same_as
    {
        template<typename U>
        struct type
            : std::bool_constant< (false || ... || std::is_same_v<T, U>) > { };
    };
}

/*** type_list_size ***/

namespace impl
{
    template<any_type_list TypeList>
    struct type_list_size;

    template<typename... Ts>
    struct type_list_size< type_list<Ts... > >
        : index_constant<sizeof...(Ts)>
    {
    };
}

template<any_type_list TypeList>
using type_list_size = impl::type_list_size<TypeList>;

template<any_type_list TypeList>
using type_list_size_t = typename type_list_size<TypeList>::type;

template<any_type_list TypeList>
inline constexpr auto type_list_size_v = type_list_size<TypeList>::value;


template<typename TypeList, std::size_t Size>
concept oversize_list_type = type_list_size_v<TypeList> >= Size;



/*** type_list_is_empty ***/

namespace impl
{
    template<any_type_list TypeList>
    struct type_list_is_empty
        : std::bool_constant< type_list_size_v<TypeList> == 0 >
    {
    };
}

template<any_type_list TypeList>
using type_list_is_empty = impl::type_list_is_empty<TypeList>;

template<any_type_list TypeList>
inline constexpr auto type_list_is_empty_v = type_list_is_empty<TypeList>::value;


template<typename TypeList>
concept nonempty_type_list = !type_list_is_empty_v<TypeList>;



/*** type_list_count_if ***/

namespace impl
{
    template<any_type_list TypeList, template<typename> typename Predicate>
    struct type_list_count_if;

    template<template<typename> typename Predicate>
    struct type_list_count_if<type_list<>, Predicate>
      : index_constant<0>
    {
    };

    template<typename T, typename... Ts, template<typename> typename Predicate>
    struct type_list_count_if< type_list<T, Ts...>, Predicate>
      : std::conditional_t</*if*/Predicate<T>::value,
                           /*then*/ index_constant<1 + type_list_count_if< type_list<Ts...>, Predicate>::value>,
                           /*else*/ type_list_count_if< type_list<Ts...>, Predicate>
                          >
    {
    };
}

template<any_type_list TypeList, template<typename> typename Predicate>
using type_list_count_if = impl::type_list_count_if<TypeList, Predicate>;

template<any_type_list TypeList, template<typename> typename Predicate>
inline constexpr auto type_list_count_if_v = type_list_count_if<TypeList, Predicate>::value;



/*** type_list_count ***/

namespace impl
{
    template<any_type_list TypeList, typename... Ts>
    using type_list_count = type_list_count_if<TypeList, helper::is_same_as<Ts...>::template type>;
}

template<any_type_list TypeList, typename... Ts>
using type_list_count = impl::type_list_count<TypeList, Ts...>;

template<any_type_list TypeList, typename... Ts>
inline constexpr auto type_list_count_v = type_list_count<TypeList, Ts...>::value;



/*** type_list_contains ***/

namespace impl
{
    template<any_type_list Typelist, typename T, template<typename, typename> typename IsSame>
    struct type_list_contains;

    template<typename T, template<typename, typename> typename IsSame>
    struct type_list_contains<type_list<>, T, IsSame> : std::false_type { };

    template<typename U, typename... Us, typename T, template<typename, typename> typename IsSame>
    struct type_list_contains<type_list<U, Us...>, T, IsSame>
      : std::conditional_t</*if*/ IsSame<U, T>::value,
                           /*then*/ std::true_type,
                           /*else*/ type_list_contains<type_list<Us...>, T, IsSame>
                          >
    {
    };
}

template<
    any_type_list TypeList,
    typename T,
    template<typename, typename> typename IsSame = std::is_same
>
using type_list_contains = impl::type_list_contains<TypeList, T, IsSame>;

template<
    any_type_list TypeList,
    typename T,
    template<typename, typename> typename IsSame = std::is_same
>
inline constexpr auto type_list_contains_v = type_list_contains<TypeList, T, IsSame>::value;



/*** type_list_first ***/

namespace impl
{
    template<any_type_list TypeList>
    struct type_list_first;

    template<typename T, typename... Ts>
    struct type_list_first< type_list<T, Ts...> >
    {
        using type = T;
    };
}

template<nonempty_type_list TypeList>
using type_list_first = typename impl::type_list_first<TypeList>;

template<nonempty_type_list TypeList>
using type_list_first_t = typename type_list_first<TypeList>::type;


/*** type_list_last ***/

namespace impl
{
    template<any_type_list TypeList>
    struct type_list_last;

    template<typename T, typename U, typename... Us>
    struct type_list_last< type_list<T, U, Us...> >
        : type_list_last< type_list<U, Us...> >
    {
    };

    template<typename T>
    struct type_list_last< type_list<T> >
    {
        using type = T;
    };
}

template<nonempty_type_list TypeList>
using type_list_last = impl::type_list_last<TypeList>;

template<nonempty_type_list TypeList>
using type_list_last_t = typename type_list_last<TypeList>::type;



/*** type_list_at ***/

namespace impl
{
    template<any_type_list TypeList, std::size_t Index>
    struct type_list_at;

    template<typename T, typename... Ts>
    struct type_list_at<type_list<T, Ts...>, /*Index*/0>
    {
        using type = T;
    };

    template<typename T, typename... Ts, std::size_t Index>
    struct type_list_at<type_list<T, Ts...>, Index>
        : type_list_at<type_list<Ts...>, Index-1 >
    {
    };
}

template<any_type_list TypeList, std::size_t Index>
    requires( Index < type_list_size_v<TypeList> )
using type_list_at = impl::type_list_at<TypeList, Index>;

template<any_type_list TypeList, std::size_t Index>
    requires( Index < type_list_size_v<TypeList> )
using type_list_at_t = typename type_list_at<TypeList, Index>::type;



/*** type_list_concat ***/

namespace impl
{
    template<any_type_list TypeList, any_type_list... TypeLists>
    struct type_list_concat;

    template<typename... Ts>
    struct type_list_concat< type_list<Ts...> >
    {
        using type = type_list<Ts...>;
    };

    template<typename... Ts, typename... Us>
    struct type_list_concat< type_list<Ts...>, type_list<Us...> >
    {
        using type = type_list<Ts..., Us...>;
    };

    template<typename TypeList1, typename TypeList2, typename... TypeLists>
    struct type_list_concat<TypeList1, TypeList2, TypeLists...>
    {
        using type = typename type_list_concat< typename type_list_concat<TypeList1, TypeList2>::type,
                                               TypeLists... >::type;
    };
}


template<any_type_list... TypeLists>
using type_list_concat = impl::type_list_concat<TypeLists...>;

template<any_type_list... TypeLists>
using type_list_concat_t = typename type_list_concat<TypeLists...>::type;


/*** type_list_combinations ***/
namespace impl
{
    template<any_type_list TypeList, any_type_list... Results>
    struct type_list_combinations;

    template<any_type_list... Results>
    struct type_list_combinations< type_list<>, Results... >
    {
        using type = type_list<Results...>;
    };

    template<typename T, typename... Ts, any_type_list... Results>
    struct type_list_combinations< type_list<T, Ts...>, Results... >
      : type_list_combinations< type_list<Ts...>, Results..., type_list<T, Ts>... >
    {
    };
}

template<any_type_list TypeList, std::size_t K>
  requires(K == 2) // sorry, current limitation
using type_list_combinations = impl::type_list_combinations<TypeList>;

template<any_type_list TypeList, std::size_t K>
  requires(K == 2) // sorry, current limitation
using type_list_combinations_t = typename type_list_combinations<TypeList, K>::type;


/*** type_list_all_of ***/
namespace impl
{
    template<any_type_list TypeList, template<typename> typename Predicate>
    struct type_list_all_of;

    template<template<typename> typename Predicate>
    struct type_list_all_of< type_list<>, Predicate >
      : std::true_type
    {
    };

    template<typename T, typename... Ts, template<typename> typename Predicate>
    struct type_list_all_of< type_list<T, Ts...>, Predicate >
      : std::conditional_t</*if*/Predicate<T>::value,
                           /*then*/type_list_all_of< type_list<Ts...>, Predicate >,
                           /*else*/std::false_type
                          >
    {
    };
}

template<any_type_list TypeList, template<typename> typename Predicate>
using type_list_all_of = impl::type_list_all_of<TypeList, Predicate>;

template<any_type_list TypeList, template<typename> typename Predicate>
using type_list_all_of_t = typename type_list_all_of<TypeList, Predicate>::type;

template<any_type_list TypeList, template<typename> typename Predicate>
static constexpr bool type_list_all_of_v = type_list_all_of<TypeList, Predicate>::value;



/*** type_list_any_of ***/
namespace impl
{
    template<any_type_list TypeList, template<typename> typename Predicate>
    struct type_list_any_of;

    template<template<typename> typename Predicate>
    struct type_list_any_of< type_list<>, Predicate >
      : std::false_type
    {
    };

    template<typename T, typename... Ts, template<typename> typename Predicate>
    struct type_list_any_of< type_list<T, Ts...>, Predicate >
      : std::conditional_t</*if*/Predicate<T>::value,
                           /*else*/std::true_type,
                           /*then*/type_list_any_of< type_list<Ts...>, Predicate >
                          >
    {
    };
}

template<any_type_list TypeList, template<typename> typename Predicate>
using type_list_any_of = impl::type_list_any_of<TypeList, Predicate>;

template<any_type_list TypeList, template<typename> typename Predicate>
using type_list_any_of_t = typename type_list_any_of<TypeList, Predicate>::type;

template<any_type_list TypeList, template<typename> typename Predicate>
static constexpr bool type_list_any_of_v = type_list_any_of<TypeList, Predicate>::value;



/*** type_list_none_of ***/
namespace impl
{
    template<any_type_list TypeList, template<typename> typename Predicate>
    struct type_list_none_of;

    template<template<typename> typename Predicate>
    struct type_list_none_of< type_list<>, Predicate >
      : std::true_type
    {
    };

    template<typename T, typename... Ts, template<typename> typename Predicate>
    struct type_list_none_of< type_list<T, Ts...>, Predicate >
      : std::conditional_t</*if*/Predicate<T>::value,
                           /*else*/std::false_type,
                           /*then*/type_list_none_of< type_list<Ts...>, Predicate >
                          >
    {
    };
}

template<any_type_list TypeList, template<typename> typename Predicate>
using type_list_none_of = impl::type_list_none_of<TypeList, Predicate>;

template<any_type_list TypeList, template<typename> typename Predicate>
using type_list_none_of_t = typename type_list_none_of<TypeList, Predicate>::type;

template<any_type_list TypeList, template<typename> typename Predicate>
static constexpr bool type_list_none_of_v = type_list_none_of<TypeList, Predicate>::value;



/*** type_list_sublist ***/

namespace impl
{
    template<any_type_list TypeList, std::size_t... Indicies>
    struct type_list_sublist;

    template<any_type_list TypeList>
    struct type_list_sublist<TypeList>
    {
        using type = type_list<>;
    };

    template<any_type_list TypeList, std::size_t Index, std::size_t... Indicies>
    struct type_list_sublist<TypeList, Index, Indicies...>
    {
      private:
        using lhs = type_list< type_list_at_t<TypeList, Index> >;
        using rhs = typename type_list_sublist<TypeList, Indicies...>::type;

      public:
        using type = type_list_concat_t<lhs, rhs>;
    };
}

template<any_type_list TypeList, std::size_t... Indicies>
    requires( (Indicies < type_list_size_v<TypeList>) && ... )
using type_list_sublist = impl::type_list_sublist<TypeList, Indicies...>;

template<any_type_list TypeList, std::size_t... Indicies>
using type_list_sublist_t = typename type_list_sublist<TypeList, Indicies...>::type;




/*** type_list_apply ***/
namespace impl
{
    template<any_type_list TypeList, template<typename...> typename T>
    struct type_list_apply;

    template<typename... Us, template<typename...> typename T>
    struct type_list_apply<type_list<Us...>, T>
    {
        using type = T<Us...>;
    };
}


template<any_type_list TypeList, template<typename...> typename T>
using type_list_apply = impl::type_list_apply<TypeList, T>;

template<any_type_list TypeList, template<typename...> typename T>
using type_list_apply_t = typename type_list_apply<TypeList, T>::type;


/*** type_list_is_unique ***/
namespace impl
{
    // FIXME: with GCC 12.2.0 if any_type_list is used, it fails to compile.
    // Works fine with Clang 15.0.7
    // More investigation needed.
    template<template<typename,typename>typename Adapter>
    struct adapt_type_list
    {
        template<typename /*any_type_list*/ TypeList>
        using type = type_list_apply_t<TypeList, Adapter>;
    };


    template<any_type_list TypeList, template<typename, typename> typename Compare,
             template<typename/*any_type_list*/> typename Adapter = adapt_type_list<Compare>::template type>
    struct type_list_is_unique
      : std::bool_constant< type_list_none_of_v<
                                type_list_combinations_t<TypeList, 2>,
                                Adapter
                            >
                          >
    {
    };

#if 0
    template<any_type_list TypeList, template<typename, typename> typename IsSame>
    struct type_list_is_unique;

    template<template<typename, typename> typename IsSame>
    struct type_list_is_unique< type_list<>, IsSame >
      : std::true_type
    {
    };

    template<typename T, typename... Ts, template<typename, typename> typename IsSame>
    struct type_list_is_unique< type_list<T, Ts...>, IsSame >
      : std::conditional_t< /*if*/type_list_contains_v<type_list<Ts...>, T, IsSame>,
                            /*then*/std::false_type,
                            /*else*/type_list_is_unique< type_list<Ts...>, IsSame >
                          >
    {
    };
#endif
}

template<any_type_list TypeList, template<typename,typename>typename IsSame = std::is_same>
using type_list_is_unique = impl::type_list_is_unique<TypeList, IsSame>;

template<any_type_list TypeList, template<typename,typename>typename IsSame = std::is_same>
inline constexpr auto type_list_is_unique_v = type_list_is_unique<TypeList, IsSame>::value;


template<typename TypeList, template<typename,typename>typename IsSame = std::is_same>
concept any_unique_type_list = type_list_is_unique_v<TypeList, IsSame>;



/*** type_list_unique ***/
// keeps the first copy
namespace impl
{
    template<any_type_list TypeList, template<typename, typename> typename IsSame, typename... Good>
    struct type_list_unique;

    template<template<typename, typename> typename IsSame, typename... Good>
    struct type_list_unique<type_list<>, IsSame, Good...>
    {
        using type = type_list<Good...>;
    };

    template<typename T, typename... Ts, template<typename, typename> typename IsSame, typename... Good>
    struct type_list_unique< type_list<T, Ts...>, IsSame, Good... >
      : std::conditional_t</*if*/type_list_contains_v<type_list<Good...>, T, IsSame>,
                           /*then*/type_list_unique< type_list<Ts...>, IsSame, Good...>,
                           /*else*/type_list_unique< type_list<Ts...>, IsSame, Good..., T>
                          >
    {
    };
}

template<any_type_list TypeList, template<typename, typename> typename IsSame = std::is_same>
using type_list_unique = impl::type_list_unique<TypeList, IsSame>;

template<any_type_list TypeList, template<typename, typename> typename IsSame = std::is_same>
using type_list_unique_t = typename type_list_unique<TypeList, IsSame>::type;




/*** type_list_erase_at ***/
namespace impl
{
    template<any_type_list TypeList, std::size_t Index, typename... Front>
    struct type_list_erase_at;

    template<typename T, typename... Ts, typename... Front>
    struct type_list_erase_at< type_list<T, Ts...>, /*Index*/0, Front... >
    {
        using type = type_list<Front..., Ts...>;
    };

    template<typename T, typename... Ts, std::size_t Index, typename... Front>
    struct type_list_erase_at< type_list<T, Ts...>, Index, Front... >
      : type_list_erase_at< type_list<Ts...>, Index-1, Front..., T >
    {
    };
}

template<any_type_list TypeList, std::size_t Index>
    requires( Index < (type_list_size_v<TypeList>) )
using type_list_erase_at = impl::type_list_erase_at<TypeList, Index>;

template<any_type_list TypeList, std::size_t Index>
    requires( Index < (type_list_size_v<TypeList>) )
using type_list_erase_at_t = typename type_list_erase_at<TypeList, Index>::type;



/*** type_list_erase_if ***/
namespace impl
{
    template<any_type_list TypeList, template<typename>typename Predicate, typename... Good>
    struct type_list_erase_if;

    template<template<typename>typename Predicate, typename... Good>
    struct type_list_erase_if< type_list<>, Predicate, Good... >
    {
        using type = type_list<Good...>;
    };

    template<typename T, typename... Ts, template<typename>typename Predicate, typename... Good>
    struct type_list_erase_if< type_list<T, Ts...>, Predicate, Good... >
      : std::conditional_t</*if*/Predicate<T>::value,
                           /*then*/type_list_erase_if< type_list<Ts...>, Predicate, Good...>,
                           /*else*/type_list_erase_if< type_list<Ts...>, Predicate, Good..., T>
                          >
    {
    };
}

template<any_type_list TypeList, template<typename>typename Predicate>
using type_list_erase_if = impl::type_list_erase_if<TypeList, Predicate>;

template<any_type_list TypeList, template<typename>typename Predicate>
using type_list_erase_if_t = typename type_list_erase_if<TypeList, Predicate>::type;


/*** type_list_erase ***/
// TODO: double chekc this code with GCC, clang seems to have some bugs here
namespace impl
{
    template<any_type_list TypeList, typename... Ts>
    using type_list_erase = type_list_erase_if<TypeList, helper::is_same_as<Ts...>::template type>;
}

template<any_type_list TypeList, typename... Ts>
using type_list_erase = impl::type_list_erase<TypeList, Ts...>;

template<any_type_list TypeList, typename... Ts>
using type_list_erase_t = typename type_list_erase<TypeList, Ts...>::type;



/*** type_list_split ***/
namespace impl
{
    template<any_type_list TypeList, std::size_t Index, typename... Front>
    struct type_list_split;

    template<typename... Front>
    struct type_list_split< type_list<>, /*Index*/0, Front...>
    {
        using type = type_list<
            type_list<Front...>,
            type_list<>
        >;
    };

    template<typename T, typename... Ts, typename... Front>
    struct type_list_split< type_list<T, Ts...>, /*Index*/0, Front...>
    {
        using type = type_list<
            type_list<Front...>,
            type_list<T, Ts...>
        >;
    };

    template<typename T, typename... Ts, std::size_t Index, typename... Front>
    struct type_list_split< type_list<T, Ts...>, Index, Front...>
      : type_list_split< type_list<Ts...>, Index-1, Front..., T>
    {
    };

}

// type_list = <int, float, std::string>
// split<0> = left_type=<>                        , right_type=<int, float, std::string>
// split<1> = left_type=<int>                     , right_type=<float, std::string>
// split<2> = left_type=<int, float>              , right_type=<std::string>
// split<3> = left_type=<int, float, std::string> , right_type=<>
// split<4> -> invalid
template<any_type_list TypeList, std::size_t Index>
    requires( Index <= (type_list_size_v<TypeList>) )
using type_list_split = impl::type_list_split<TypeList, Index>;

template<any_type_list TypeList, std::size_t Index>
    requires( Index <= (type_list_size_v<TypeList>) )
using type_list_split_t = typename type_list_split<TypeList, Index>::type;






/*** type_list_append ***/
namespace impl
{
    template<any_type_list, typename... Us>
    struct type_list_append;

    template<typename... Ts, typename... Us>
    struct type_list_append< type_list<Ts...>, Us... >
    {
        using type = type_list<Ts..., Us...>;
    };
}

template<any_type_list TypeList, typename... Us>
using type_list_append = impl::type_list_append<TypeList, Us...>;

template<any_type_list TypeList, typename... Us>
using type_list_append_t = typename type_list_append<TypeList, Us...>::type;



/*** type_list_insert ***/
namespace impl
{
    template<any_type_list TypeList, std::size_t Index, typename... Us>
    struct type_list_insert
    {
      private:
        using split_type = type_list_split_t<TypeList, Index>;

        using left_type = type_list_at_t<split_type, 0>;
        using middle_type = type_list<Us...>;
        using right_type = type_list_at_t<split_type, 1>;

      public:
        using type = type_list_concat_t<left_type, middle_type, right_type>;

    };
}

template<any_type_list TypeList, std::size_t Index, typename... Us>
using type_list_insert = impl::type_list_insert<TypeList, Index, Us...>;

template<any_type_list TypeList, std::size_t Index, typename... Us>
using type_list_insert_t = typename type_list_insert<TypeList, Index, Us...>::type;



/*** type_list_reverse ***/
namespace impl
{
    template<any_type_list TypeList, typename... Reversed>
    struct type_list_reverse;

    template<typename... Reversed>
    struct type_list_reverse< type_list<>, Reversed... >
    {
        using type = type_list<Reversed...>;
    };

    template<typename T, typename... Ts, typename... Reversed>
    struct type_list_reverse< type_list<T, Ts...>, Reversed... >
      : type_list_reverse< type_list<Ts...>, T, Reversed... >
    {
    };
}

template<any_type_list TypeList>
using type_list_reverse = impl::type_list_reverse<TypeList>;

template<any_type_list TypeList>
using type_list_reverse_t = typename type_list_reverse<TypeList>::type;



/*** type_list_transform ***/
namespace impl
{
    template<any_type_list TypeList, template<typename> typename Transformer, typename... Results>
    struct type_list_transform;

    template<template<typename> typename Transformer, typename... Results>
    struct type_list_transform< type_list<>, Transformer, Results... >
    {
        using type = type_list<Results...>;
    };

    template<typename T, typename... Ts, template<typename> typename Transformer, typename... Results>
    struct type_list_transform< type_list<T, Ts...>, Transformer, Results... >
      : type_list_transform< type_list<Ts...>, Transformer, Results...,
                             Transformer<T>
                           >
    {
    };
}

template<any_type_list TypeList, template<typename> typename Transformer>
using type_list_transform = impl::type_list_transform<TypeList, Transformer>;

template<any_type_list TypeList, template<typename> typename Transformer>
using type_list_transform_t = typename type_list_transform<TypeList, Transformer>::type;


/*** type_list_convert ***/
namespace impl
{
    template<template<typename> typename ConvertType>
    struct convert
    {
        template<typename T>
        using type = ConvertType<T>;
    };

    template<any_type_list TypeList, template<typename> typename ConvertType,
             template<typename> typename Transformer = convert<ConvertType>::template type >
    struct type_list_convert : type_list_transform<TypeList, Transformer>
    {
    };
}

template<any_type_list TypeList, template<typename> typename ConvertType>
using type_list_convert = impl::type_list_convert<TypeList, ConvertType>;

template<any_type_list TypeList, template<typename> typename ConvertType>
using type_list_convert_t = typename type_list_convert<TypeList, ConvertType>::type;


/*** type_list_extend */
namespace impl
{
    // template<typename>
    template<template<typename> typename... ExtendTypes>
    struct extend
    {
        template<typename T>
        using type = type_list<
            T, /* original type */
            ExtendTypes<T>...
        >;
    };

    template<any_type_list TypeList, template<typename> typename... ExtendTypes>
    struct type_list_extend
        : type_list_transform<TypeList, extend<ExtendTypes...>::template type  >
    {
    };
}

template<any_type_list TypeList, template<typename> typename... ExtendTypes>
using type_list_extend = impl::type_list_extend<TypeList, ExtendTypes...>;

template<any_type_list TypeList, template<typename> typename... ExtendTypes>
using type_list_extend_t = typename type_list_extend<TypeList, ExtendTypes...>::type;

/*** extract_type_list */

namespace impl
{
    template<typename>
    struct extract_type_list;

    template<template<typename...> typename T, typename... Ts>
    struct extract_type_list< T<Ts...> >
    {
        using type = type_list<Ts...>;
    };
}

/*** type_list_find_first ***/

namespace impl
{
    template<std::size_t I, any_type_list TypeList, template<typename> typename Predicate>
    struct type_list_find_first;

    template<std::size_t I, template<typename> typename Predicate>
    struct type_list_find_first< I, type_list<>, Predicate>
      : index_constant<I>
    {
    };

    template<std::size_t I, typename T, typename... Ts, template<typename> typename Predicate>
    struct type_list_find_first< I, type_list<T, Ts...>, Predicate>
      : std::conditional_t</*if*/Predicate<T>::value,
                           /*then*/index_constant<I>,
                           /*else*/type_list_find_first<I + 1u, type_list<Ts...>, Predicate>
                          >
    {
    };
}

template<any_type_list TypeList, template<typename> typename Predicate>
  requires (type_list_any_of_v<TypeList, Predicate>)
using type_list_find_first = impl::type_list_find_first<0, TypeList, Predicate>;

template<any_type_list TypeList, template<typename> typename Predicate>
  requires (type_list_any_of_v<TypeList, Predicate>)
using type_list_find_first_t = typename type_list_find_first<TypeList, Predicate>::type;

template<any_type_list TypeList, template<typename> typename Predicate>
  requires (type_list_any_of_v<TypeList, Predicate>)
static constexpr auto type_list_find_first_v = type_list_find_first<TypeList, Predicate>::value;


/*** type_list_find_all ***/

namespace impl
{
    template<std::size_t I, any_type_list TypeList, template<typename> typename Predicate>
    struct type_list_find_all;

    template<std::size_t I, template<typename> typename Predicate>
    struct type_list_find_all< I, type_list<>, Predicate>
    {
        using type = type_list<>;
    };

    template<std::size_t I, typename T, typename... Ts, template<typename> typename Predicate>
    struct type_list_find_all< I, type_list<T, Ts...>, Predicate>
    {
      private:
        using lhs = std::conditional_t</*if*/Predicate<T>::value,
                                       /*then*/type_list< index_constant<I> >,
                                       /*else*/type_list< >
                                      >;

        using rhs = typename type_list_find_all<I+1, type_list<Ts...>, Predicate>::type;

      public:
        using type = type_list_concat_t<lhs, rhs>;
    };
}

template<any_type_list TypeList, template<typename> typename Predicate>
using type_list_find_all = impl::type_list_find_all<0u, TypeList, Predicate>;

template<any_type_list TypeList, template<typename> typename Predicate>
using type_list_find_all_t = typename type_list_find_all<TypeList, Predicate>::type;

// type_name_find_if_else
namespace impl
{
    template<std::size_t I, any_type_list TypeList, template<typename> typename Predicate>
    struct type_list_find_if_else;

    template<std::size_t I, template<typename> typename Predicate>
    struct type_list_find_if_else< I, type_list<>, Predicate>
    {
        using type = type_list<
            type_list<>,
            type_list<>
        >;
    };

    template<std::size_t I, typename T, typename... Ts, template<typename> typename Predicate>
    struct type_list_find_if_else< I, type_list<T, Ts...>, Predicate>
    {
      private:
        using lhs = std::conditional_t</*if*/Predicate<T>::value,
                                       /*then*/ type_list< type_list<index_constant<I>>, type_list<> >,
                                       /*else*/ type_list< type_list<>, type_list<index_constant<I>> >
                                      >;

        using rhs = typename type_list_find_if_else<I+1, type_list<Ts...>, Predicate>::type;

      public:
        using type = type_list<
            type_list_concat_t< type_list_at_t<lhs, 0>, type_list_at_t<rhs, 0> >,
            type_list_concat_t< type_list_at_t<lhs, 1>, type_list_at_t<rhs, 1> >
        >;
    };
}

template<any_type_list TypeList, template<typename> typename Predicate>
using type_list_find_if_else = impl::type_list_find_if_else<0u, TypeList, Predicate>;

template<any_type_list TypeList, template<typename> typename Predicate>
using type_list_find_if_else_t = typename type_list_find_if_else<TypeList, Predicate>::type;


/// type_list_tie

namespace impl
{
    template <typename TypeList1, typename TypeList2>
    struct type_list_tie;

    template <>
    struct type_list_tie<type_list<>, type_list<>>
    {
        using type = type_list<>;
    };

    template<typename T, typename... Ts, typename U, typename... Us>
    struct type_list_tie< type_list<T, Ts...>, type_list<U, Us...> >
    {
      private:
        using lhs = type_list< type_list<T, U> >;
        using rhs = typename type_list_tie< type_list<Ts...>, type_list<Us...> >::type;

      public:
        using type = type_list_concat_t<lhs, rhs>;
    };

}

// TODO, make this variadic
// TODO, check for equal size
template<any_type_list List1, any_type_list List2>
using type_list_tie = typename impl::type_list_tie<List1, List2>;

template<any_type_list List1, any_type_list List2>
using type_list_tie_t = typename type_list_tie<List1, List2>::type;


namespace impl
{
    template<typename T>
    struct make_type_list_from_integer_sequence;

    template<typename T, T... Values>
    struct make_type_list_from_integer_sequence< std::integer_sequence<T, Values...> >
    {
        using type = type_list<
            std::integer_sequence<T, Values>...
        >;
    };
}

template<typename IntegerSequence>
using make_type_list_from_integer_sequence = impl::make_type_list_from_integer_sequence<IntegerSequence>;

template<typename IntegerSequence>
using make_type_list_from_integer_sequence_t = typename make_type_list_from_integer_sequence<IntegerSequence>::type;

template<any_type_list>
using to_integer_sequence = void;


// type_list_all_of_combinations

namespace impl
{

    template<any_type_list List, template<typename, typename> typename Predicate>
    struct type_list_all_of_combinations;

    template<template<typename, typename> typename Predicate>
    struct type_list_all_of_combinations< type_list<>, Predicate >
    {
        static constexpr bool value = true;
    };

    template<typename T, typename... Ts, template<typename, typename> typename Predicate>
    struct type_list_all_of_combinations< type_list<T, Ts...>, Predicate >
    {
        static constexpr bool value = (Predicate<T, Ts>::value && ... && true)
            && type_list_all_of_combinations<type_list<Ts...>, Predicate>::value;
    };
}

template<any_type_list List, std::size_t K, template<typename, typename> typename Predicate>
  requires(K == 2) // sorry, current limitation
using type_list_all_of_combinations = impl::type_list_all_of_combinations<List, Predicate>;

template<any_type_list List, std::size_t K, template<typename, typename> typename Predicate>
  requires(K == 2) // sorry, current limitation
constexpr bool type_list_all_of_combinations_v = type_list_all_of_combinations<List, K, Predicate>::value;


// type_list_replace

namespace impl
{
    template<typename OldType, typename NewType>
    struct ReplaceTransormer
    {
        template<typename T>
        using type = std::conditional_t< std::is_same_v<T, OldType>, NewType, T>;
    };

    template<any_type_list List, typename OldType, typename NewType>
    using type_list_replace = type_list_transform<List, ReplaceTransormer<OldType, NewType>::template type >;
};

template<any_type_list List, typename OldType, typename NewType>
using type_list_replace = impl::type_list_replace<List, OldType, NewType>;

template<any_type_list List, typename OldType, typename NewType>
using type_list_replace_t = typename type_list_replace<List, OldType, NewType>::type;



} // end namespace kuba
