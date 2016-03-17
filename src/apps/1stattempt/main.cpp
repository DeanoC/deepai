#include "core/core.h"
#include "aicore/aicore.h"
#include <array>
#include <vector>
#include <cassert>
#include <boost/range/irange.hpp>

#include "aicore/basetypes.h"


enum class Colours {
	Red = 0,
	Blue,
	Green,
	White,
	Black,
	Violet,

	COUNT,
};
template< typename T>
constexpr T Colour_lowest() { return T(0); }
template< typename T>
constexpr T Colour_highest() { return T(Colours::COUNT)-1; }
template< typename T>
constexpr Colours Colour_fromInt( T in ) { return (Colours)in; }

// result == 0 AKA false if both templates are the same
template< int A, int B>
struct TemplateSubtract {
	enum { result = A - B };
};

typedef std::vector<Colours> ArrayOfColours;

template< typename T, typename ALU = Core::VectorProcessingBackend<Core::VectorALU::BASIC_CPP,T> , typename InternalResults = Core::VectorOfFloats, bool b = TemplateSubtract<T::DataCategory, AICore::DataCategory::Qualitive>::result >
struct ResultData;


template< typename T, typename ALU, typename InternalResults >
struct ResultData<T, ALU, InternalResults, false>
{
	explicit ResultData(ALU& _alu) : results(_alu) {}

	typedef typename InternalResults::value_type NumericType;

	static const unsigned int NumChannelsPerElement = (unsigned int) T::value_type::COUNT;

	void allocateSpaceFor( unsigned int elementCount ) {
		results.data.resize( elementCount );
		normalised.resize( NumChannelsPerElement * elementCount );
	}

	void generateIdealResult( unsigned int index ) {
		using namespace AICore;
		if( T::DataType == Nominal ) {
			// one of N normalization for nominal data
			const auto chan = index % NumChannelsPerElement;
			for(int i=0;i < NumChannelsPerElement;++i) {
				normalised[ (index*NumChannelsPerElement)+i] = -1;
			}
			normalised[ (index * NumChannelsPerElement) + chan ] = 1;
		} else {
			// range normalization for Ordinal
			NumericType percentage = static_cast<NumericType>(index) / Colour_highest<NumericType>();
			NumericType width = Colour_highest<NumericType>() - Colour_lowest<NumericType>();

			normalised[ index ] = percentage * width + Colour_lowest<NumericType>();
		}
	}
	T results;
	InternalResults normalised;
};

template< typename T, typename ALU, typename InternalResults >
struct ResultData<T, ALU, InternalResults, true>
{
	explicit ResultData(ALU& _alu) : results(_alu) {}

	void allocateSpaceFor( unsigned int elementCount ) {
		results.resize( elementCount );
	}
	void generateIdealResults( unsigned int index ) {
	}

	T results;
};



class DataSet {
public:

	DataSet() : a( Core::DefaultVectorALU<Core::VectorOfFloats>() ), b( Core::DefaultVectorALU<ArrayOfColours>() ) {}
	size_t size() const { 
		assert( validate() );
		return a.data.size(); 
	}

	bool validate() const {
		if( a.data.size() != b.data.size() ) return false;

		return true;
	}

	void setA( const Core::VectorOfFloats& in ) {
		a.data = in;
	}

	void setB( const ArrayOfColours& in ) {
		b.data = in;
	}

protected:
	AICore::RatioData<Core::VectorOfFloats> 	a;
	AICore::NominalData<ArrayOfColours> b;
};

int main()
{
	using namespace AICore;
	using namespace Core;
	DataSet input;

	VectorOfFloats f;
	ArrayOfColours c;

	auto irange = boost::irange(0,5);
	f.resize( irange.size() );
	c.resize( irange.size() );
	for( auto i : irange )
	{
		f[i] = i;
		c[i] = Colour_fromInt(i);
	}
	input.setA( f );
	input.setB( c );

	ResultData< NominalData<ArrayOfColours>,Core::VectorProcessingBackend<Core::VectorALU::BASIC_CPP,ArrayOfColours>  > results( DefaultVectorALU<ArrayOfColours>() );	
	results.allocateSpaceFor( 10 );
	for(auto i = 0; i < input.size(); ++i ) {
		results.generateIdealResult(i);
	}


	return 0;	

}