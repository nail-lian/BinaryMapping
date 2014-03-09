#ifndef BINARY_MAPPING_SRC_DETAIL_BASE_POINTER_H_
#define BINARY_MAPPING_SRC_DETAIL_BASE_POINTER_H_

#include <cstdint>

namespace BinaryMapping {
namespace dtl {

template <typename Type>
struct BasePointer {
	static_assert(
		std::is_integral<Type>::value,
		"BasePointer accepts only integral types"
	);

	typedef typename std::add_const<
		typename std::add_pointer<Type>::type
	>::type direct_ptr;

	typedef typename std::add_pointer<
		direct_ptr
	>::type indirect_ptr;

	enum class Tag : std::uint8_t {
		Direct,
		Indirect
	};

	BasePointer(direct_ptr ptr):
		direct(ptr),
		tag(Tag::Direct) { }

	BasePointer(indirect_ptr ptr):
		indirect(ptr),
		tag(Tag::Indirect) { }

	inline indirect_ptr get() const {
		switch ( this->tag ) {
			case Tag::Direct:   return &this->direct;
			case Tag::Indirect: return this->indirect;
			default:            return nullptr;
		}
	}

	union {
		direct_ptr   direct;
		indirect_ptr indirect; 
	};

	const Tag tag;
};

}
}

#endif  // BINARY_MAPPING_SRC_DETAIL_BASE_POINTER_H_
