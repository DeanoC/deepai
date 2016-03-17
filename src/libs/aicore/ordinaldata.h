#pragma once

namespace AICore {
	template < typename E, typename ALU> struct OrdinalData;

	template< typename E, typename ALU >
	struct DataTypeBase<struct OrdinalData<E,ALU>,ALU>  :  protected DataTypeBaseImpl<E,ALU> {
		typedef DataTypeBaseImpl<E,ALU> super;
		using typename super::container_type;
		using typename super::value_type;
		using super::data;
		using super::processor;

		explicit DataTypeBase(ALU& _alu) : super(_alu) {}

		enum { DataCategory = DataCategory::Qualitive };
		enum { DataType = DataType::Ordinal };	
	};

	template < typename E, typename ALU = Core::VectorProcessingBackend<Core::VectorALU::BASIC_CPP,E>>
	struct OrdinalData :  protected DataTypeBase<OrdinalData<E,ALU>,ALU>{
		typedef DataTypeBase<OrdinalData<E,ALU>,ALU>  super;
		typedef OrdinalData<E,ALU> type_name;

		using typename super::container_type;
		using typename super::value_type;
		using super::DataCategory;
		using super::DataType;
		using super::data;
		using super::processor;

		explicit OrdinalData( ALU& _alu ) : super(_alu) {}

		typedef std::shared_ptr<type_name> shared_ptr;

		bool operator==( const type_name& b ) const {
			return processor.compareEquals( data, b.data );
		}

		bool operator!=( const type_name& b ) const {
			return processor.compareNotEquals( data, b.data );
		}

		bool operator<( const type_name& b ) const {
			return processor.compareAllLess( data, b.data );
		}
		bool operator>( const type_name& b ) const {
			return processor.compareAllGreater( data, b.data );
		}	
		const value_type& operator[]( size_t index ) const {
			return data[index];
		}
		value_type& operator[]( size_t index ) {
			return data[index];
		}	
		friend std::ostream& operator<<( std::ostream& out, const type_name& a)
 		{
 		//	out << a.type;
			//	out << a.data;
			return out;
		}
	};	
	
}