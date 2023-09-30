#pragma once

#include <string>
#include <optional>

#include <type_list/type_list.hh>
#include <type_list/type_list_utilities.hh>

namespace tn
{

namespace testing
{

// template<typename, typename = void>
// struct is_valid_type : std::false_type { };
//
// template<typename T>
// struct is_valid_type<T, std::void_t<T>> : std::true_type {};
//

// template<typename T>
// inline auto is_valid_type_v = is_valid_type<T>::value;

namespace test_concat
{
    using List1 = type_list<int, float>;
    using List2 = type_list<>;
    using List3 = type_list<std::string>;

    using Result = type_list_concat_t<List1, List2, List3>;

    using Expected = type_list<int, float, std::string>;

    static_assert(std::is_same_v<Result, Expected>);
}

namespace test_combinations
{
    using List0 = type_list<>;
    using Result0 = type_list_combinations_t<List0, 2>;
    using Expected0 = type_list<
    >;

    using List1 = type_list<int>;
    using Result1 = type_list_combinations_t<List1, 2>;
    using Expected1 = type_list<
    >;

    using List2 = type_list<int, float>;
    using Result2 = type_list_combinations_t<List2, 2>;
    using Expected2 = type_list<
        type_list<int, float>
    >;
    static_assert(std::is_same_v<Result2, Expected2>);

    using List3 = type_list<int, float, std::string>;
    using Result3 = type_list_combinations_t<List3, 2>;
    using Expected3 = type_list<
        type_list<int, float>,
        type_list<int, std::string>,
        type_list<float, std::string>
    >;
    static_assert(std::is_same_v<Result3, Expected3>);

    using List4 = type_list<int, float, std::string, char>;
    using Result4 = type_list_combinations_t<List4, 2>;
    using Expected4 = type_list<
        type_list<int, float>,
        type_list<int, std::string>,
        type_list<int, char>,

        type_list<float, std::string>,
        type_list<float, char>,

        type_list<std::string, char>
    >;
    static_assert(std::is_same_v<Result4, Expected4>);


}

namespace test_sublist
{
    inline namespace test0
    {
        using List = type_list<int, float, std::string>;
        using Result = type_list_sublist_t<List, 0, 2>;
        using Expected = type_list<int, std::string>;
        static_assert(std::is_same_v<Result, Expected>);
    }

//     inline namespace test0
//     {
//         using List = type_list<int, float, std::string>;
//         static_assert( !is_valid_type_v< decltype([]{
//             return std::type_identity<type_list_sublist_t<List, 0, 3>>()}) >);
//     }
}

namespace test_is_unique
{
    template<any_unique_type_list List>
    using UniqueList = List;

    using List1 = type_list<int, float, std::string>;
    static_assert(type_list_is_unique_v<List1>);

    using List2 = type_list<int, float, int>;
    static_assert(!type_list_is_unique_v<List2>);

    using UList1 = UniqueList<List1>;

    // Won't compile
    // using UList2 = UniqueList<List2>;

}

namespace test_unique
{
    using List = type_list<std::string, int, std::string, int, std::string, float, float>;

    using Result = type_list_unique_t<List>;
    using Expected = type_list<std::string, int, float>;
    static_assert(std::is_same_v<Result, Expected>);

}

namespace test_erase_at
{
    using List = type_list<int, float, std::string>;

    using Expected0 = type_list<float, std::string>;
    using Result0 = type_list_erase_at_t<List, 0>;
    static_assert(std::is_same_v<Result0, Expected0>);

    using Expected1 = type_list<int, std::string>;
    using Result1 = type_list_erase_at_t<List, 1>;
    static_assert(std::is_same_v<Result1, Expected1>);

    using Expected2 = type_list<int, float>;
    using Result2 = type_list_erase_at_t<List, 2>;
    static_assert(std::is_same_v<Result2, Expected2>);
}

namespace test_erase_if
{
    using List = type_list<int, float, std::string>;

    using Expected = type_list<std::string>;
    using Result = type_list_erase_if_t<List, std::is_fundamental>;

    static_assert(std::is_same_v<Result, Expected>);
}

namespace test_erase
{
    using List = type_list<int, float, std::string>;

    using Result0 = impl::type_list_erase<List>::type;
    using Expected0 = type_list<int, float, std::string>;
    static_assert(std::is_same_v<Result0, Expected0>);

    using Result1 = impl::type_list_erase<List, int, float>::type;
    using Expected1 = type_list<std::string>;
    static_assert(std::is_same_v<Result1, Expected1>);
}

namespace test_split
{
    using List = type_list<int, float, std::string>;

    using Result0 = type_list_split_t<List, 0>;
    using Result0Left = type_list_at_t<Result0, 0>;
    using Result0Right = type_list_at_t<Result0, 1>;
    using Expected0Left = type_list<>;
    using Expected0Right = type_list<int, float, std::string>;
    static_assert(std::is_same_v<Result0Left, Expected0Left>);
    static_assert(std::is_same_v<Result0Right, Expected0Right>);

    using Result1 = type_list_split_t<List, 1>;
    using Result1Left = type_list_at_t<Result1, 0>;
    using Result1Right = type_list_at_t<Result1, 1>;
    using Expected1Left = type_list<int>;
    using Expected1Right = type_list<float, std::string>;
    static_assert(std::is_same_v<Result1Left, Expected1Left>);
    static_assert(std::is_same_v<Result1Right, Expected1Right>);

    using Result2 = type_list_split_t<List, 2>;
    using Result2Left = type_list_at_t<Result2, 0>;
    using Result2Right = type_list_at_t<Result2, 1>;
    using Expected2Left = type_list<int, float>;
    using Expected2Right = type_list<std::string>;
    static_assert(std::is_same_v<Result2Left, Expected2Left>);
    static_assert(std::is_same_v<Result2Right, Expected2Right>);

    using Result3 = type_list_split_t<List, 3>;
    using Result3Left = type_list_at_t<Result3, 0>;
    using Result3Right = type_list_at_t<Result3, 1>;
    using Expected3Left = type_list<int, float, std::string>;
    using Expected3Right = type_list<>;
    static_assert(std::is_same_v<Result3Left, Expected3Left>);
    static_assert(std::is_same_v<Result3Right, Expected3Right>);
}

namespace test_insert
{
    using List0 = type_list<>;

    using List1 = type_list_insert_t<List0, 0, float>;
    using Expected1 = type_list<float>;
    static_assert(std::is_same_v<List1, Expected1>);

    using List2 = type_list_insert_t<List1, 0, int, std::string>;
    using Expected2 = type_list<int, std::string, float>;
    static_assert(std::is_same_v<List2, Expected2>);

    using List3 = type_list_insert_t<List2, 1, double>;
    using Expected3 = type_list<int, double, std::string, float>;
    static_assert(std::is_same_v<List3, Expected3>);

    using List4 = type_list_insert_t<List3, type_list_size_v<List3>, char>;
    using Expected4 = type_list<int, double, std::string, float, char>;
    static_assert(std::is_same_v<List4, Expected4>);
}

namespace test_reverse
{
    using List0 = type_list<>;
    using Result0 = type_list_reverse_t<List0>;
    using Expected0 = type_list<>;
    static_assert(std::is_same_v<Result0, Expected0>);

    using List1 = type_list<int, float, std::string>;
    using Result1 = type_list_reverse_t<List1>;
    using Expected1 = type_list<std::string, float, int>;
    static_assert(std::is_same_v<Result1, Expected1>);
}

namespace test_apply
{
    using List = type_list<int, float, std::string>;
    using Result = List::apply<std::tuple>;
    using Expected = std::tuple<int, float, std::string>;

    static_assert(std::is_same_v<Result, Expected>);
}

namespace test_contains
{
    using List = type_list<int, float, std::string>;

    static_assert(type_list_contains_v<List, float>);
    static_assert(!type_list_contains_v<List, double>);
}

namespace test_count
{
    using List = type_list<int, float, std::string, int>;

    static_assert(type_list_count_v<List, int> == 2);
    static_assert(type_list_count_v<List, std::string> == 1);
    static_assert(type_list_count_v<List, double> == 0);
}

namespace test_all_of
{
    using List1 = type_list<int, char, long>;
    constexpr auto Result1 = type_list_all_of_v<List1, std::is_integral>;
    static_assert(Result1);

    using List2 = type_list<int, float, char, long>;
    using Result2 = type_list_all_of_t<List2, std::is_integral>;
    static_assert(std::is_same_v<Result2, std::false_type>);

    using List3 = type_list<>;
    constexpr auto Result3 = type_list_all_of_v<List3, std::is_integral>;
    static_assert(Result3);
}

namespace test_any_of
{
    using List1 = type_list<int, char, long>;
    constexpr auto Result1 = type_list_any_of_v<List1, std::is_floating_point>;
    static_assert(!Result1);

    using List2 = type_list<int, float, char, long>;
    using Result2 = type_list_any_of_t<List2, std::is_floating_point>;
    static_assert(std::is_same_v<Result2, std::true_type>);
}

namespace test_none_of
{
    using List1 = type_list<int, char, long>;
    constexpr auto Result1 = type_list_none_of_v<List1, std::is_floating_point>;
    static_assert(Result1);

    using List2 = type_list<int, float, char, long>;
    using Result2 = type_list_none_of_t<List2, std::is_floating_point>;
    static_assert(std::is_same_v<Result2, std::false_type>);

    using List3 = type_list<>;
    constexpr auto Result3 = type_list_none_of_v<List3, std::is_floating_point>;
    static_assert(Result3);
}

namespace test_convert
{
    using List = type_list<int, float, std::string>;
    using Result = type_list_convert_t<List, std::optional>;
    using Expected = type_list<
        std::optional<int>,
        std::optional<float>,
        std::optional<std::string>
    >;

    static_assert(std::is_same_v<Result, Expected>);
}

namespace test_extend
{
    template<typename T>
    struct type_value : std::integral_constant<int, -1> { };

    template<> struct type_value<int> : std::integral_constant<int, 5>{};
    template<> struct type_value<float> : std::integral_constant<int, 3>{};
    template<> struct type_value<std::string> : std::integral_constant<int, 1>{};

    template<typename T>
    using type_value_t = typename type_value<T>::type;

    using List1 = type_list<int, float, double, std::string>;

    template<typename T> // single parameter
    using void_t = std::void_t<T>;

    using Result1 = type_list_extend_t<List1, type_value_t, void_t >;
    using Expected1 = type_list<
        type_list<int, std::integral_constant<int, 5>, void>,
        type_list<float, std::integral_constant<int, 3>, void>,
        type_list<double, std::integral_constant<int, -1>, void>,
        type_list<std::string, std::integral_constant<int, 1>, void>
    >;
    static_assert(std::is_same_v<Result1, Expected1>);

    // extending with nothing, will just sink types into type_list
    using Result2 = type_list_extend_t<List1>; // no extensions
    using Expected2 = type_list<
        type_list<int>,
        type_list<float>,
        type_list<double>,
        type_list<std::string>
    >;
    static_assert(std::is_same_v<Result2, Expected2>);
}

namespace test_transform
{
    template<typename T>
    using VoidTransformer = void;

    using List1 = type_list<int, float, double>;
    using Result1 = type_list_transform_t<List1, VoidTransformer>;
    using Expected1 = type_list<void, void, void>;
    static_assert(std::is_same_v<Result1, Expected1>);

}

namespace test_extract
{
    using Tuple1 = std::tuple<int, float>;
    using Result1 = extract_type_list_t<Tuple1>;
    using Expected1 = type_list<int, float>;

    static_assert(std::is_same_v<Result1, Expected1>);
}

namespace test_append
{
    template<typename... Ts>
    struct my_list
    {
        using type = type_list<Ts...>;
    };

    using MyList = my_list<int, float>;

    using Result = type_list_append_t<MyList::type, double>;
    using Expected = type_list<int, float, double>;

    static_assert(std::is_same_v<Result, Expected>);
}

namespace test_iota
{
    using Result1 = iota_type_list_t<int, 3>;

    using Expected1 = type_list<
        std::integral_constant<int, 0>,
        std::integral_constant<int, 1>,
        std::integral_constant<int, 2>
    >;

    static_assert(std::is_same_v<Result1, Expected1>);
}


namespace test_tie
{
    using List1 = type_list<int, float, std::string>;
    using List2 = type_list<char, double, short>;

    using Result = type_list_tie_t<List1, List2>;

    using Expected = type_list<
        type_list<int, char>,
        type_list<float, double>,
        type_list<std::string, short>
    >;

    static_assert(std::is_same_v<Result, Expected>);
}

namespace test_from_integer_sequence
{
    using Sequence = std::integer_sequence<std::size_t, 5, 3, 4>;

    using Result = make_type_list_from_integer_sequence_t<Sequence>;

    using Expected = type_list<
        std::integral_constant<std::size_t, 5>,
        std::integral_constant<std::size_t, 3>,
        std::integral_constant<std::size_t, 4>
    >;
}

namespace test_find_first
{
    using List = type_list<int, float>;

    constexpr inline std::size_t Expected = 1;

    constexpr inline auto Result = type_list_find_first_v<List, std::is_floating_point>;

    static_assert(Result == Expected);

}

namespace test_find_all
{
    using List1 = type_list<int, float, std::string, double>;

    using Expected1 = type_list<
        std::integral_constant<std::size_t, 1>,
        std::integral_constant<std::size_t, 3>
    >;

    using Result1 = type_list_find_all_t<List1, std::is_floating_point>;

    static_assert(std::is_same_v<Result1, Expected1>);
}

namespace test_find_if_else
{
    using List1 = type_list<int, float, std::string, double>;

    using ExpectedTrue1 = type_list<
        std::integral_constant<std::size_t, 1>,
        std::integral_constant<std::size_t, 3>
    >;

    using ExpectedFalse1 = type_list<
        std::integral_constant<std::size_t, 0>,
        std::integral_constant<std::size_t, 2>
    >;

    using Expected1 = type_list<ExpectedTrue1, ExpectedFalse1>;

    using Result1 = type_list_find_if_else_t<List1, std::is_floating_point>;

    static_assert(std::is_same_v<Result1, Expected1>);
}

namespace test_all_of_combinations
{
    template<template<typename, std::size_t, template<typename, typename> typename> typename Func>
    struct tester {
        using List1 = type_list<int, int, int, void>;
        static constexpr auto Expected1 = false;
        static constexpr auto Result1 = Func<List1, 2, std::is_same>::value;
        static_assert(Result1 == Expected1);

        using List2 = type_list<int, int, int, int>;
        static constexpr auto Expected2 = true;
        static constexpr auto Result2 = Func<List2, 2, std::is_same>::value;
        static_assert(Result2 == Expected2);

        using List3 = type_list<int>;
        static constexpr auto Expected3 = true;
        static constexpr auto Result3 = Func<List3, 2, std::is_same>::value;
        static_assert(Result3 == Expected3);

        using List4 = type_list<>;
        static constexpr auto Expected4 = true;
        static constexpr auto Result4 = Func<List4, 2, std::is_same>::value;
        static_assert(Result4 == Expected4);
    };

    template<any_type_list List, std::size_t K, template<typename, typename> typename Predicate>
    struct type_list_combinations_all_of
    : std::bool_constant<
        List::template combinations<K>
            ::template all_of<applied_type_list<Predicate>::template type>
    >
    {};

    template struct tester<type_list_all_of_combinations>;
    template struct tester<type_list_combinations_all_of>;
};

namespace test_replace
{
    using List = type_list<int, float, void>;
    using Expected = type_list<int, float, short>;
    using Result = type_list_replace_t<List, void, short>;
    static_assert(std::is_same_v<Expected, Result>);
}


} // end namespace testing

}
