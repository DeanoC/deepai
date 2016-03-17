#pragma once

namespace Core {

	enum class VectorALU : uint8_t {
		BASIC_CPP
	};

	// TODO typedef std::vector<half> VectorOfHalfs;
	typedef std::vector<float> VectorOfFloats;
	typedef std::vector<double> VectorOfDoubles;

//	#define ArrayOfHalfs(Count) std::array<half,(Count)>
	#define ArrayOfFloats(Count) std::array<float,(Count)>
	#define ArrayOfDoubles(Count) std::array<double,(Count)>

	template< VectorALU ALU,typename type >
	struct VectorProcessingBackend {

		// these are never used, consider this the 'interface' which each ALU backend should support at a minimum
		typedef std::pair< typename type::value_type, typename type::value_type> ResultPair;		

		void add( const type& a, const type& b, type& out );
		void sub( const type& a, const type& b, type& out );
		void mul( const type& a, const type& b, type& out );
		void div( const type& a, const type& b, type& out );

		ResultPair minMaxOf( const type& input );

		bool compareEquals( const type& a, const type& b );
		bool compareNotEquals( const type& a, const type& b );
		bool compareAllGreater( const type& a, const type& b );
		bool compareAllLess( const type& a, const type& b );


		void normaliseData0to1( const type& in, type& out );
		void normaliseDataNeg1toPos1( const type& in, type& out );
	};

	template< typename T>
	inline VectorProcessingBackend<VectorALU::BASIC_CPP,T>& DefaultVectorALU() {
		static VectorProcessingBackend<VectorALU::BASIC_CPP,T> alu;
		return alu;
	}
}