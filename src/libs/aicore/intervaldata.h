#pragma once

namespace AICore {

	template< typename V, typename ALU> struct IntervalData;

	template<typename T, typename ALU >
	struct DataTypeBase<struct IntervalData<T,ALU>,ALU> : protected DataTypeBaseImpl<T,ALU> {
		typedef DataTypeBaseImpl<T,ALU> super;
		using typename super::container_type;
		using typename super::value_type;
		using super::data;
		using super::processor;

		enum { DataCategory = DataCategory::Quantative };
		enum { DataType = DataType::Interval };
	
		explicit DataTypeBase(ALU& _alu) : super(_alu) {}
		
	};


	template< typename V, typename ALU = Core::VectorProcessingBackend<Core::VectorALU::BASIC_CPP,V> >
	struct IntervalData :  protected DataTypeBase<IntervalData<V,ALU>,ALU>{
		typedef DataTypeBase<IntervalData<V,ALU>,ALU>  super;
		typedef IntervalData<V,ALU> type_name;

		using typename super::container_type;
		using typename super::value_type;
		using super::DataCategory;
		using super::DataType;
		using super::data;
		using super::processor;

		typedef std::shared_ptr<type_name> shared_ptr;

		explicit IntervalData( ALU& _alu ) : super(_alu) {}

		shared_ptr operator+( const type_name& b) const {
			auto o = std::make_shared<type_name>(super::processor);
			processor.add( data, b.data, o->data );
			return o;
		}
		shared_ptr operator-( const type_name& b) const {
			auto o = std::make_shared<type_name>(super::processor);
			processor.sub( data, b.data, o->data );
			return o;
		}
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
				out << a.DataType;
			//	out << a	.data;
				return out;
		}
	};

}