#ifndef BINARY_MAPPING_SRC_CONTAINER_H_
#define BINARY_MAPPING_SRC_CONTAINER_H_

#include <stdexcept>

#include "tuple/tuple.h"
#include "endianess/undefined.h"
#include "detail/io/buffer.h"

#include "iterator.h"

namespace BinaryMapping {

template <typename Type>
class Container {
	public:
		typedef Type element_type;
		typedef typename Type::value_type element_value_type;
		typedef Iterator<Type> iterator_type;
		typedef Iterator<const Type> const_iterator_type;

		Container(size_t size):
			buffer_(size * element_type::size),
			tuple_count_(buffer_.size<element_type::size>()) { }

		Container(uint8_t*const ptr, size_t size):
			buffer_(ptr, size),
			tuple_count_(buffer_.size<element_type::size>()) { }

		inline size_t size() const {
			return this->tuple_count_;
		}

		inline element_value_type operator[](size_t index) const {
			return static_cast<element_value_type>(element_type(
				this->buffer_[element_type::size * index]
			));
		}

		inline element_value_type at(size_t index) const {
			return static_cast<element_value_type>(element_type(
				this->buffer_.template at<element_type::size>(index)
			));
		}

		inline element_value_type front() const {
			return static_cast<element_value_type>(
				element_type(this->buffer_.front())
			);
		}

		inline element_value_type back() const {
			return static_cast<element_value_type>(element_type(
				this->buffer_.template at<element_type::size>(
					this->tuple_count_ - 1
				)
			));
		}

		inline const_iterator_type begin() const {
			return const_iterator_type(
				this->buffer_.template begin<element_type::size>(),
				this->buffer_.template begin<element_type::size>(),
				this->buffer_.template end<element_type::size>()
			);
		}

		inline const_iterator_type end() const {
			return const_iterator_type(
				this->buffer_.template end<element_type::size>(),
				this->buffer_.template begin<element_type::size>(),
				this->buffer_.template end<element_type::size>()
			);
		}

		inline std::pair<uint8_t*const, const size_t> data() const {
			return std::make_pair(this->buffer_.front(), this->buffer_.size());
		}

		inline element_type operator[](size_t index) {
			return element_type(
				this->buffer_[element_type::size * index]
			);
		}

		inline element_type at(size_t index) {
			return element_type(
				this->buffer_.template at<element_type::size>(index)
			);
		}

		inline element_type front() {
			return element_type(this->buffer_.front());
		}

		inline element_type back() {
			return element_type(
				this->buffer_.template at<element_type::size>(
					this->tuple_count_ - 1
				)
			);
		}

		inline iterator_type begin() {
			return iterator_type(
				this->buffer_.template begin<element_type::size>(),
				this->buffer_.template begin<element_type::size>(),
				this->buffer_.template end<element_type::size>()
			);
		}

		inline iterator_type end() {
			return iterator_type(
				this->buffer_.template end<element_type::size>(),
				this->buffer_.template begin<element_type::size>(),
				this->buffer_.template end<element_type::size>()
			);
		}

	private:
		dtl::Buffer buffer_;
		const size_t tuple_count_;

};

}

#endif  // BINARY_MAPPING_SRC_CONTAINER_H_
