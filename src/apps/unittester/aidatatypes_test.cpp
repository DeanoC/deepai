#include "core/core.h"
#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/data/monomorphic.hpp>
#include <boost/range/irange.hpp>

#include "aicore/aicore.h"
#include "aicore/basetypes.h"

namespace utf = boost::unit_test;
namespace bdata = utf::data;
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

typedef std::vector<Colours> ArrayOfColours;

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
	VectorOfFloats f;
	ArrayOfColours c;

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
	
   	typedef AICore::NominalData<VectorOfFloats> Y;

	BOOST_REQUIRE_EQUAL( Y::DataCategory, AICore::DataCategory::Qualitive );
	BOOST_REQUIRE_EQUAL( Y::DataType, AICore::DataType::Nominal );
	BOOST_REQUIRE_NE( Y::DataCategory, AICore::DataCategory::Quantative );

	Y a(Core::DefaultVectorALU<Core::VectorOfFloats>());
	Y b(Core::DefaultVectorALU<Core::VectorOfFloats>());
	BOOST_REQUIRE( a == a );
	// with no data set, y and z are equal with null data
	BOOST_REQUIRE( a == b );

	Y::shared_ptr y = std::make_shared<Y>(Core::DefaultVectorALU<Core::VectorOfFloats>());
	Y::shared_ptr z = std::make_shared<Y>(Core::DefaultVectorALU<Core::VectorOfFloats>());
	Y::shared_ptr err = std::make_shared<Y>(Core::DefaultVectorALU<Core::VectorOfFloats>());
	auto irange = boost::irange(0,5);
	y->data.resize( irange.size() );
	z->data.resize( irange.size() );
	err->data.resize( irange.size() );
	for( auto i : irange )
	{
		(*y)[i] = i;
		(*z)[i] = i;
		(*err)[i] = 42;
	}

	BOOST_TEST( *y == *z );
	BOOST_TEST( *y != *err );
	BOOST_TEST( *z != *err );
	err->data.resize( 0 );
	BOOST_TEST( *z != *err );

	Y::shared_ptr aP = std::make_shared<Y>(Core::DefaultVectorALU<Core::VectorOfFloats>());
	Y::shared_ptr bP = std::make_shared<Y>(Core::DefaultVectorALU<Core::VectorOfFloats>());
	BOOST_TEST( operator!=( aP, bP ) );
	BOOST_TEST( operator!=( aP, z ) );
}

BOOST_AUTO_TEST_CASE( test_Ordinal_datatype, * utf::tolerance(0.00001) )
{
	using namespace AICore;
	using namespace Core;

	typedef AICore::OrdinalData<VectorOfFloats> Y;

	BOOST_REQUIRE_EQUAL( Y::DataCategory, AICore::DataCategory::Qualitive );
	BOOST_REQUIRE_EQUAL( Y::DataType, AICore::DataType::Ordinal );
	BOOST_REQUIRE_NE( Y::DataCategory, AICore::DataCategory::Quantative );

	Y a(Core::DefaultVectorALU<Core::VectorOfFloats>());
	Y b(Core::DefaultVectorALU<Core::VectorOfFloats>());
	BOOST_REQUIRE( a == a );
	// with no data set, y and z are equal with null data
	BOOST_REQUIRE( a == b );

	Y::shared_ptr y = std::make_shared<Y>(Core::DefaultVectorALU<Core::VectorOfFloats>());
	Y::shared_ptr z = std::make_shared<Y>(Core::DefaultVectorALU<Core::VectorOfFloats>());
	Y::shared_ptr err = std::make_shared<Y>(Core::DefaultVectorALU<Core::VectorOfFloats>());
	auto irange = boost::irange(0,5);
	y->data.resize( irange.size() );
	z->data.resize( irange.size() );
	err->data.resize( irange.size() );
	for( auto i : irange )
	{
		(*y)[i] = i;
		(*z)[i] = i;
		(*err)[i] = 42;
	}
	BOOST_TEST( *y == *z );
	BOOST_TEST( *y != *err );
	BOOST_TEST( *z != *err );
	err->data.resize( 0 );
	BOOST_TEST( *z != *err );
	BOOST_TEST( *err > *y );
	BOOST_TEST( *y < *err );

	Y::shared_ptr aP = std::make_shared<Y>(Core::DefaultVectorALU<Core::VectorOfFloats>());
	Y::shared_ptr bP = std::make_shared<Y>(Core::DefaultVectorALU<Core::VectorOfFloats>());
	BOOST_TEST( operator!=( aP, bP ) );
	BOOST_TEST( operator!=( aP, z ) );
}

BOOST_AUTO_TEST_CASE( test_Interval_datatype, * utf::tolerance(0.00001) )
{
	using namespace AICore;
	using namespace Core;
	
   	typedef AICore::IntervalData<VectorOfFloats> Y;

	BOOST_REQUIRE_EQUAL( Y::DataCategory, AICore::DataCategory::Quantative );
	BOOST_REQUIRE_EQUAL( Y::DataType, AICore::DataType::Interval );
	BOOST_REQUIRE_NE( Y::DataCategory, AICore::DataCategory::Qualitive );

	Y a(Core::DefaultVectorALU<Core::VectorOfFloats>());
	Y b(Core::DefaultVectorALU<Core::VectorOfFloats>());
	BOOST_REQUIRE( a == a );
	// with no data set, y and z are equal with null data
	BOOST_REQUIRE( a == b );

	Y::shared_ptr y = std::make_shared<Y>(Core::DefaultVectorALU<Core::VectorOfFloats>());
	Y::shared_ptr z = std::make_shared<Y>(Core::DefaultVectorALU<Core::VectorOfFloats>());
	Y::shared_ptr err = std::make_shared<Y>(Core::DefaultVectorALU<Core::VectorOfFloats>());
	auto irange = boost::irange(0,5);
	y->data.resize( irange.size() );
	z->data.resize( irange.size() );
	err->data.resize( irange.size() );
	for( auto i : irange )
	{
		(*y)[i] = i;
		(*z)[i] = i;
		(*err)[i] = 42;
	}

	BOOST_TEST( *y == *z );
	BOOST_TEST( *y != *err );
	BOOST_TEST( *z != *err );
	err->data.resize( 0 );
	BOOST_TEST( *z != *err );
	BOOST_TEST( *err > *y );
	BOOST_TEST( *y < *err );


	auto ar0 = *y + *z;
	for( auto i : irange )
	{	
		BOOST_TEST( (*ar0)[i] == i+i );
	}	

	auto ar1 = *y - *z;	
	for( auto i : irange )
	{
		BOOST_TEST( (*ar1)[i] == 0.0 );
	}	

	Y::shared_ptr aP = std::make_shared<Y>(Core::DefaultVectorALU<Core::VectorOfFloats>());
	Y::shared_ptr bP = std::make_shared<Y>(Core::DefaultVectorALU<Core::VectorOfFloats>());
	BOOST_TEST( operator!=( aP, bP ) );
	BOOST_TEST( operator!=( aP, z ) );
}

BOOST_AUTO_TEST_CASE( test_Ratio_datatype, * utf::tolerance(0.00001) )
{
	using namespace AICore;
	using namespace Core;

	typedef AICore::RatioData<VectorOfFloats> Y;

	BOOST_REQUIRE_EQUAL( Y::DataCategory, AICore::DataCategory::Quantative );
	BOOST_REQUIRE_EQUAL( Y::DataType, AICore::DataType::Ratio );
	BOOST_REQUIRE_NE( Y::DataCategory, AICore::DataCategory::Qualitive );

	Y a(Core::DefaultVectorALU<Core::VectorOfFloats>());
	Y b(Core::DefaultVectorALU<Core::VectorOfFloats>());
	BOOST_REQUIRE( a == a );
	// with no data set, y and z are equal with null data
	BOOST_REQUIRE( a == b );

	Y::shared_ptr y = std::make_shared<Y>(Core::DefaultVectorALU<Core::VectorOfFloats>());
	Y::shared_ptr z = std::make_shared<Y>(Core::DefaultVectorALU<Core::VectorOfFloats>());
	Y::shared_ptr err = std::make_shared<Y>(Core::DefaultVectorALU<Core::VectorOfFloats>());
	auto irange = boost::irange(0,5);
	y->data.resize( irange.size() );
	z->data.resize( irange.size() );
	err->data.resize( irange.size() );
	for( auto i : irange )
	{
		(*y)[i] = i;
		(*z)[i] = i;
		(*err)[i] = 42;
	}
	BOOST_TEST( *y == *z );
	BOOST_TEST( *y != *err );
	BOOST_TEST( *z != *err );
	err->data.resize( 0 );
	BOOST_TEST( *z != *err );
	BOOST_TEST( *err > *y );
	BOOST_TEST( *y < *err );

	auto ar0 = *y + *z;
	for( auto i : irange )
	{	
		BOOST_TEST( (*ar0)[i] == i+i );
	}	

	auto ar1 = *y - *z;	
	for( auto i : irange )
	{
		BOOST_TEST( (*ar1)[i] == 0.0 );
	}	
	auto ar2 = *y * *z;
	for( auto i : irange )
	{
		BOOST_TEST( (*ar2)[i] == i*i );
	}	
	auto ar3 = *y / *z;
	for( auto i : irange )
	{
		if( i > 0 ) {
			BOOST_TEST( (*ar3)[i] == 1.0 );
		}
	}	

	Y::shared_ptr aP = std::make_shared<Y>(Core::DefaultVectorALU<Core::VectorOfFloats>());
	Y::shared_ptr bP = std::make_shared<Y>(Core::DefaultVectorALU<Core::VectorOfFloats>());
	BOOST_TEST( operator!=( aP, bP ) );
	BOOST_TEST( operator!=( aP, z ) );
}