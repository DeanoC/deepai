#pragma once

#include "core/core.h"

namespace Core {
	template< typename type>
	struct VectorProcessingBackend<VectorALU::BASIC_CPP, type> {
		typedef typename type::value_type value_type;
		
		typedef std::pair< value_type, value_type> ResultPair;

		static void Op( type const& a, type const& b, type& o, value_type (*lamdba)(const value_type, const value_type)) {
			const auto siz = (a.size() < b.size()) ? a.size() : b.size();

			o.resize( siz );

			for( auto i = 0;i < siz; ++i ){
				o[i] = lamdba( a[i], b[i] );
			}
		}
		
		void add( const type& a, const type& b, type& o ) 
		{
			Op( a, b, o, []( const value_type av, const value_type bv ) -> value_type { return av + bv; } );
		}
		void sub( const type& a, const type& b, type& o ) 
		{
			Op( a, b, o, []( const value_type av, const value_type bv ) -> value_type { return av - bv; } );
		}
		void mul( const type& a, const type& b, type& o ) 
		{
			Op( a, b, o, []( const value_type av, const value_type bv ) -> value_type { return av * bv; } );
		}
		void div( const type& a, const type& b, type& o ) 
		{
			Op( a, b, o, []( const value_type av, const value_type bv ) -> value_type { return av / bv; } );
		}

		bool compareEquals( const type& a, const type& b ) 
		{
			auto bit = b.begin();
			for( auto ait : a ) {
				if( ait != *bit ) {
					return false;
				}
				++bit;
			}

			return true;
		}
		bool compareNotEquals( const type& a, const type& b )
		{
			return !compareEquals(a,b);
		}
		bool compareAllGreater( const type& a, const type& b )
		{
			auto bit = b.begin();
			for( auto ait : a ) {
				if( ait <= *bit ) {
					return false;
				}
				++bit;
			}

			return true;
		}
		bool compareAllLess( const type& a, const type& b )
		{
			auto bit = b.begin();
			for( auto ait : a ) {
				if( ait >= *bit ) {
					return false;
				}
				++bit;
			}

			return true;
		}


		ResultPair minMaxOf( const type& in ) {
			// default to min and max of the type held in the container
			typename type::value_type mini = std::numeric_limits<typename type::value_type>::max();
			typename type::value_type maxi = std::numeric_limits<typename type::value_type>::min();
			
			for( auto it : in ) {
				mini = std::min( it, mini );
				maxi = std::max( it, maxi );
			}

			return ResultPair(mini, maxi);
		}
		void normaliseData0to1( const type& in, type& out ) {
			out.resize( in.size() );
			ResultPair minAndMax = MinMaxOf( in );
			typename type::value_type width = minAndMax.second - minAndMax.first;
			assert( width > type::value_type(0) );

			auto& ot = out.begin();
			for( auto it : in ) {
				*ot = *it / width;
				++ot;
			}
		}
	};	
}