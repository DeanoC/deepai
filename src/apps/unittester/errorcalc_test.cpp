#include "core/core.h"
#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/range/irange.hpp>

#include <iostream>
#include "aicore/aicore.h"
#include "aicore/basetypes.h"
#include "aicore/errorcalc.h"

namespace utf = boost::unit_test;
namespace bdata = utf::data;

BOOST_AUTO_TEST_CASE(error_calc_test, *utf::tolerance(1e-5)) {
    using namespace Core;
    using namespace AICore;

    ErrorCalc<>::vector_type perfect{10, 5, 2};
    ErrorCalc<>::vector_type test{9, 6, -3};

    BOOST_REQUIRE_EQUAL(ErrorCalc<>::SumOfSquare(perfect, test), 27);

    BOOST_REQUIRE_EQUAL(ErrorCalc<>::MeanSquare(perfect, test), 9);

    BOOST_REQUIRE_EQUAL(ErrorCalc<>::RootMeanSquare(perfect, test), 3);

}