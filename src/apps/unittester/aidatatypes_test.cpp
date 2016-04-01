#include "core/core.h"
#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/range/irange.hpp>

#include "aicore/aicore.h"
#include "aicore/basetypes.h"

namespace utf = boost::unit_test;
namespace bdata = utf::data;

using ALU = Core::VectorALU<Core::VectorALUBackend::BASIC_CPP>;

// check compiler handles the way we do nominal data groups
enum class Colours {
	Red = 0,
	Blue,
	Green,
	White,
	Black,
	Violet,

	COUNT,
};


std::ostream& operator<<( std::ostream& out, Colours type ) {
	switch( type ) {
		case Colours::Red: out << "Colours::Red "; break;
		case Colours::Blue: out << "Colours::Blue "; break;
		case Colours::Green: out << "Colours::Green "; break;
		case Colours::White: out << "Colours::White "; break;
		case Colours::Black: out << "Colours::Black "; break;
		case Colours::Violet: out << "Colours::Violet "; break;
		default: out << "Colours ERROR - UNDEFINED "; break;
	}
	return out;
}


template< typename T>
constexpr T Colour_lowest() { return T(0); }
template< typename T>
constexpr T Colour_highest() { return T(Colours::COUNT)-1; }
template< typename T>
constexpr Colours Colour_fromInt( T in ) {
	assert( in < T(Colours::COUNT) ); 
	return (Colours)in; 
}

typedef ALU::VectorOf <Colours> VectorOfColours;

BOOST_AUTO_TEST_CASE( test_Colours_enum )
{
	BOOST_REQUIRE_EQUAL( Colour_lowest<int>(), 0 );
	BOOST_REQUIRE_EQUAL( Colour_highest<int>(), 5 );
	BOOST_REQUIRE_EQUAL( Colour_fromInt(0), Colours::Red );
	BOOST_REQUIRE_EQUAL( Colour_fromInt(5), Colours::Violet );
}

BOOST_AUTO_TEST_CASE( test_ArrayOf )
{
	using namespace AICore;
	using namespace Core;

    ALU::VectorOfFloats f;
    VectorOfColours c;

	auto irange = boost::irange(0,5);
	BOOST_REQUIRE_EQUAL( irange.size(), 5 );
	f.resize( irange.size() );
	c.resize( irange.size() );
	for( auto i : irange )
	{
		f[i] = i;
		c[i] = Colour_fromInt(i);
		BOOST_REQUIRE_EQUAL( f[i], i );
		BOOST_REQUIRE_EQUAL( c[i], Colour_fromInt(i) );
		if( i != 0 ) {
			BOOST_REQUIRE_EQUAL( f[i-1], i-1 );
			BOOST_REQUIRE_EQUAL( c[i-1], Colour_fromInt(i-1) );
		}
	}
}

BOOST_AUTO_TEST_CASE( test_Nominal_datatype, * utf::tolerance(0.00001) )
{
	using namespace AICore;
	using namespace Core;

    ALU &alu = ALU::get();

    typedef AICore::NominalData Y;

    Y a;
    Y b;

    BOOST_REQUIRE_EQUAL(a.category, AICore::DataCategory::Qualitive);
    BOOST_REQUIRE_EQUAL(a.type, AICore::DataType::Nominal);
    BOOST_REQUIRE_NE(a.category, AICore::DataCategory::Quantative);

}

BOOST_AUTO_TEST_CASE( test_Ordinal_datatype, * utf::tolerance(0.00001) )
{
	using namespace AICore;
	using namespace Core;

    ALU &alu = ALU::get();

    typedef AICore::OrdinalData Y;

    Y a;
    Y b;

    BOOST_REQUIRE_EQUAL(a.category, AICore::DataCategory::Qualitive);
    BOOST_REQUIRE_EQUAL(a.type, AICore::DataType::Ordinal);
    BOOST_REQUIRE_NE(a.category, AICore::DataCategory::Quantative);
}

BOOST_AUTO_TEST_CASE( test_Interval_datatype, * utf::tolerance(0.00001) )
{
	using namespace AICore;
	using namespace Core;

    ALU &alu = ALU::get();

    typedef AICore::IntervalData Y;

    Y a;
    Y b;

    BOOST_REQUIRE_EQUAL(a.category, AICore::DataCategory::Quantative);
    BOOST_REQUIRE_EQUAL(a.type, AICore::DataType::Interval);
    BOOST_REQUIRE_NE(a.category, AICore::DataCategory::Qualitive);

}

BOOST_AUTO_TEST_CASE( test_Ratio_datatype, * utf::tolerance(0.00001) )
{
	using namespace AICore;
	using namespace Core;

    ALU &alu = ALU::get();

    typedef AICore::RatioData Y;

    Y a;
    Y b;

    BOOST_REQUIRE_EQUAL(a.category, AICore::DataCategory::Quantative);
    BOOST_REQUIRE_EQUAL(a.type, AICore::DataType::Ratio);
    BOOST_REQUIRE_NE(a.category, AICore::DataCategory::Qualitive);

}