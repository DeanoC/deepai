#pragma once

#include "core/vectoralu.h"
#include "core/vectoralu_basicc.h"

namespace AICore {
	template< typename V, typename ALU> struct RatioData;
	
	#define RATIO_SHARED_FUNC(T,ALU) 							\
			typedef DataTypeBaseImpl<T,ALU> super;				\
			using typename super::container_type;				\
			using typename super::value_type;					\
			using typename super::alu_type;						\
			using super::data;									\
			using super::processor;								\
			enum { DataCategory = DataCategory::Quantative }; 	\
			enum { DataType = DataType::Ratio }; 																								

	// general case where data type must provide normalisation step
	template< typename T, typename ALU >
	struct DataTypeBase<struct RatioData<T,ALU>,ALU> :  protected DataTypeBaseImpl<T,ALU> {
		RATIO_SHARED_FUNC(T,ALU);

		explicit DataTypeBase(ALU& _alu) : super(_alu) {}

	};

	// specialize the common case of arrays of floats
	template<typename ALU>
	struct DataTypeBase<struct RatioData<Core::VectorOfFloats,ALU>,ALU> :  protected DataTypeBaseImpl<Core::VectorOfFloats,ALU> {
		RATIO_SHARED_FUNC(Core::VectorOfFloats,ALU);

		explicit DataTypeBase(ALU& _alu) : super(_alu) {}

	};


	#undef RATIO_SHARED_FUNC

	template< typename V, typename ALU = Core::VectorProcessingBackend<Core::VectorALU::BASIC_CPP,V> >
	struct RatioData : protected DataTypeBase<RatioData<V,ALU>,ALU> {
		typedef DataTypeBase<RatioData<V,ALU>,ALU>  super;			
		typedef RatioData<V,ALU> type_name;

		using typename super::container_type;
		using typename super::value_type;
		using super::DataCategory;
		using super::DataType;
		using super::data;
		using super::processor;

		typedef std::shared_ptr<type_name> shared_ptr;


		explicit RatioData( ALU& _alu ) : super(_alu) {}

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
		shared_ptr operator*( const type_name& b) const {
			auto o = std::make_shared<type_name>(super::processor);
			processor.mul( data, b.data, o->data );
			return o;
		}
		shared_ptr operator/( const type_name& b) const {
			auto o = std::make_shared<type_name>(super::processor);
			processor.div( data, b.data, o->data );
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

		friend std::ostream& operator<<( std::ostream& out, const RatioData<V,ALU>& a)	
		{
			out << a.DataType;
		//	out << a.data;
			return out;
		}
	};
}