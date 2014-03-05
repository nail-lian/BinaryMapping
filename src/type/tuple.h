#ifndef BINARY_MAPPING_SRC_TYPE_TUPLE_H_
#define BINARY_MAPPING_SRC_TYPE_TUPLE_H_

#include <tuple>

#include "detail/utility.h" 
#include "detail/base_pointer.h"
#include "detail/relative_pointer.h" 

#include "detail/tuple/weigher.h" 
#include "detail/tuple/setter.h" 

#include "detail/tuple/mapper.h" 
#include "detail/tuple/modifier/relative_tuple.h"
#include "detail/tuple/modifier/value_tuple.h"

#include "endianess/serializer.h" 
#include "endianess/in_place_sorter.h" 
#include "endianess/out_of_place_sorter.h" 
#include "endianess/undefined.h" 

namespace BinaryMapping {

template <
	typename Endianess,
	typename... Types
>
class Tuple {
	typedef std::tuple<dtl::RelativePointer<uint8_t, Types>...> relative_tuple;

	template <size_t Index>
	using type_at = typename std::tuple_element<
		Index,
		relative_tuple
	>::type::element_type;

	public:
		typedef std::tuple<Types...> value_type;

		static const size_t size = dtl::Weigher::size<relative_tuple>();

		static inline value_type constructValue(const uint8_t*const data) {
			return dtl::Mapper::construct<
				dtl::ValueTuple<Endianess>,
				value_type
			>(
				const_cast<uint8_t*const*>(&data)
			);
		}

		Tuple(uint8_t*const data):
			base_ptr_(data),
			tuple_(dtl::Mapper::construct<
				dtl::RelativeTuple,
				relative_tuple
			>(
				this->base_ptr_.get()
			)) { }

		Tuple(uint8_t*const* data):
			base_ptr_(data),
			tuple_(dtl::Mapper::construct<
				dtl::RelativeTuple,
				relative_tuple
			>(
				this->base_ptr_.get()
			)) { }

		inline operator value_type() const {
			return Tuple::constructValue(*(this->base_ptr_.get()));
		}

		template <size_t Index> 
		inline type_at<Index> get() const {
			return OutOfPlaceSorter<Endianess>::sort(
				*std::get<Index>(this->tuple_)
			);
		}

		template <size_t Index> 
		inline typename std::add_pointer<type_at<Index>>::type ptr() {
			return std::get<Index>(this->tuple_).get();
		}

		inline void operator=(dtl::const_lvalue_reference<value_type> values) {
			dtl::Setter<Endianess>::template populate<
				relative_tuple,
				value_type
			>(values, this->tuple_);
		}

		template <size_t Index>
		inline void set(dtl::const_lvalue_reference<type_at<Index>> value) {
			InPlaceSorter<Endianess>::template mix<
				type_at<Index>
			>(
				std::get<Index>(this->tuple_).get(),
				value
			);
		}

		template <typename CustomOrder>
		inline void serialize() {
			static_assert(
				std::is_same<Endianess, UndefinedEndian>::value,
				"Endianess must be UndefinedEndian to use serialize<*>()"
			);

			Serializer<CustomOrder>::serialize(this->tuple_);
		}

		template <typename CustomOrder>
		inline void deserialize() {
			static_assert(
				std::is_same<Endianess, UndefinedEndian>::value,
				"Endianess must be UndefinedEndian to use deserialize<*>()"
			);

			Serializer<CustomOrder>::deserialize(this->tuple_);
		}

	protected:
		const dtl::BasePointer<uint8_t> base_ptr_;
		const relative_tuple tuple_;

};

template <typename... Types>
using PlainTuple = Tuple<UndefinedEndian, Types...>;

}

#endif  // BINARY_MAPPING_SRC_TYPE_TUPLE_H_