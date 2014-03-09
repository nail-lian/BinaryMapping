#ifndef BINARY_MAPPING_SRC_DETAIL_UTILITY_H_
#define BINARY_MAPPING_SRC_DETAIL_UTILITY_H_

#include <type_traits>

namespace BinaryMapping {
namespace dtl {

template <typename Type>
using const_lvalue_reference = typename std::add_lvalue_reference<
	typename std::add_const<Type>::type
>::type;

template <typename Type>
using pointer_to_const = typename std::add_pointer<
	typename std::add_const<Type>::type
>::type;

template <bool Condition>
using enable_if = typename std::enable_if<Condition, std::size_t>::type;

template <
	typename Type,
	typename TypeA,
	typename TypeB
>
using either = std::integral_constant<
	bool,
	std::is_same<Type, TypeA>::value || std::is_same<Type, TypeB>::value
>;

template <
	typename Type,
	typename TypeA,
	typename TypeB
>
using enable_if_either = enable_if<either<Type, TypeA, TypeB>::value>;

template <
	typename Type,
	typename Check
>
using check_if_class = typename std::conditional<
	std::is_class<Type>::value,
	Check,
	std::false_type
>::type;

template <typename Type>
struct has_data_member : std::integral_constant<
	bool,
	std::is_pod<decltype(Type:: bytes)>::value
> { };

template <typename Type>
using is_custom_serializable = std::integral_constant<
	bool,
	check_if_class<
		Type,
		has_data_member<Type>
	>::value
>;

template <
	typename Type,
	enable_if<is_custom_serializable<Type>::value> = 0
>
constexpr std::size_t size_of() {
	return std::tuple_size<decltype(Type:: bytes)>::value;
}

template <
	typename Type,
	enable_if<std::is_pod<Type>::value> = 0
>
constexpr std::size_t size_of() {
	return sizeof(Type);
}

template <std::size_t Size>
struct bits_to_bytes : std::integral_constant<
	std::size_t,
	Size / 8
> {
	static_assert(
		Size % 8 == 0,
		"Bit count has to be byte aligned"
	);
};

}
}

#endif  // BINARY_MAPPING_SRC_DETAIL_UTILITY_H_
