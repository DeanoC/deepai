#pragma once

namespace AICore {

    template<typename V = float, typename ALU = Core::VectorALU<Core::VectorALUBackend::BASIC_CPP>>
    struct NominalData : public DataTypeBase<V, ALU> {

        typedef DataTypeBase <V, ALU> super;
        typedef NominalData<V, ALU> type_name;
		using typename super::container_type;
		using typename super::value_type;
		using super::data;
		using super::processor;
        using super::type;
        using super::category;

        typedef std::shared_ptr<type_name> shared_ptr;

        std::vector<std::string> categories;

        explicit NominalData(ALU &_alu) : super(DataType::Nominal, _alu) { }

        void addData(const std::string &_data) override {
            // must be a string category name incoming
            categories.push_back(_data);
        }

		bool operator==( const type_name& b ) const {
			return processor.compareEquals( data, b.data );
		}

		bool operator!=( const type_name& b ) const {
			return processor.compareNotEquals( data, b.data );
		}

		const value_type& operator[]( size_t index ) const {
			return data[index];
		}
		value_type& operator[]( size_t index ) {
			return data[index];
		}	
		friend std::ostream& operator<<( std::ostream& out, const type_name& a) {
            out << a.type;
		//	out << a.data;
			return out;
		}
	};
}
