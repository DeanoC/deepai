#pragma once

#include "aicore.h"
#include "basetypes.h"


namespace AICore {


    template<typename V = float, typename ALU = Core::VectorALU<Core::VectorALUBackend::BASIC_CPP> >
    struct IntervalData : public DataTypeBase<V, ALU> {
        typedef DataTypeBase <V, ALU> super;
		typedef IntervalData<V,ALU> type_name;

		using typename super::container_type;
		using typename super::value_type;
		using super::data;
		using super::processor;
        using super::type;
        using super::category;

		typedef std::shared_ptr<type_name> shared_ptr;

        explicit IntervalData(ALU &_alu) : super(DataType::Interval, _alu) { }

        void addData(const std::string &_data) override {
            // must be numeric data incoming
            data.push_back(boost::lexical_cast<V>(_data));
        }

        shared_ptr operator+(const type_name &b) const {
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
            out << a.type;
			//	out << a	.data;
				return out;
		}
	};

}