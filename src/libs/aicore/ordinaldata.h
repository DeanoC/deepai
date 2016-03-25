#pragma once

namespace AICore {
    struct OrdinalData : public DataTypeBase<DataCategory::Qualitive> {
        typedef DataTypeBase super;
        typedef OrdinalData type_name;

		using typename super::container_type;
		using typename super::value_type;
		using super::data;
        using super::type;
        using super::category;

        typedef std::shared_ptr<type_name> shared_ptr;

        explicit OrdinalData() : super(DataType::Ordinal) { }

		const value_type& operator[]( size_t index ) const {
			return data[index];
		}
		value_type& operator[]( size_t index ) {
			return data[index];
		}

        friend std::ostream &operator<<(std::ostream &out, const type_name &a) {
            out << a.type;
			//	out << a.data;
			return out;
		}
	};	
	
}