#pragma once

namespace AICore {
	template < typename E, typename ALU > struct NominalData;

	template<typename E, typename ALU >
	struct DataTypeBase<struct NominalData<E,ALU>,ALU> :  protected DataTypeBaseImpl<E,ALU>{
		typedef DataTypeBaseImpl<E,ALU> super;
		using typename super::container_type;
		using typename super::value_type;
		using super::data;
		using super::processor;

		explicit DataTypeBase(ALU& _alu) : super(_alu) {}

		enum { DataCategory = DataCategory::Qualitive };
		enum { DataType = DataType::Nominal };
	};

	template < typename E, typename ALU = Core::VectorProcessingBackend<Core::VectorALU::BASIC_CPP,E>>
	struct NominalData : protected DataTypeBase<NominalData<E,ALU>,ALU>{

		typedef DataTypeBase<NominalData<E,ALU>,ALU>  super;
		typedef NominalData<E,ALU> type_name;
		using typename super::container_type;
		using typename super::value_type;
		using super::DataCategory;
		using super::DataType;
		using super::data;
		using super::processor;
		
		typedef std::shared_ptr<type_name> shared_ptr;

		explicit NominalData( ALU& _alu ) : super(_alu) {}

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
			out << a.DataType;
		//	out << a.data;
			return out;
		}
	};
}
