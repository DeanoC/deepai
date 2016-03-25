#pragma once

#include "core/vectoralu.h"
#include "core/vectoralu_basicc.h"

namespace AICore {

    struct RatioData : public DataTypeBase<DataCategory::Quantative> {
        typedef DataTypeBase super;
        typedef RatioData type_name;
		using typename super::container_type;
		using typename super::value_type;
		using super::data;

		typedef std::shared_ptr<type_name> shared_ptr;

        explicit RatioData() : super(DataType::Ratio) { }

		const value_type& operator[]( size_t index ) const {
			return data[index];
		}
		value_type& operator[]( size_t index ) {
			return data[index];
		}

        friend std::ostream &operator<<(std::ostream &out, const type_name &a)
		{
			out << a.type;
		//	out << a.data;
			return out;
		}
	};
}