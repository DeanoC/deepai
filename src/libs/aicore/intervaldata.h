#pragma once

#include "aicore.h"
#include "basetypes.h"


namespace AICore {


    struct IntervalData : public DataTypeBase<DataCategory::Quantative> {
        typedef DataTypeBase super;
        typedef IntervalData type_name;

		using typename super::container_type;
		using typename super::value_type;
		using super::data;
        using super::type;
        using super::category;

		typedef std::shared_ptr<type_name> shared_ptr;

        explicit IntervalData() : super(DataType::Interval) { }

		const value_type& operator[]( size_t index ) const {
			return data[index];
		}

        value_type &operator[](size_t index) {
			return data[index];
		}	

		friend std::ostream& operator<<( std::ostream& out, const type_name& a)
		{
            out << a.type;
			//	out << a	.data;
				return out;
		}
	};

}