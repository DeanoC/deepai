#include "core/core.h"
#include "core/random.h"
#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/range/irange.hpp>

#include "aicore/aicore.h"
#include "aicore/basetypes.h"
#include "aicore/datamodel.h"
#include "aialgorithms/greedytrainer.h"

namespace utf = boost::unit_test;
namespace bdata = utf::data;

BOOST_AUTO_TEST_CASE(greedy_trainer_test, *utf::tolerance(1e-4)) {
    using namespace Core;
    using namespace AICore;
    using namespace AIAlgorithm;

    const FeatureVector<float>::Vector a{10, 5, 2};
    const FeatureVector<float>::Vector b{9, 6, -3};

    auto train = std::make_shared<GreedyTrainer<float>::FVector>(3, 1, &a[0]);
    auto test = std::make_shared<GreedyTrainer<float>::FVector>(3, 1, &b[0]);
    GreedyTrainer<float>::range_type range(0, 10);

    GreedyTrainer<float> gt(train, range);

    BOOST_REQUIRE_EQUAL(gt.calculateScore(test), 3);

    // set the seed so result doesn't change...
    Random::seed(42);

    int count = 0;
    while (gt.iterate(train) == false) {
        count++;
        if (count >= 10000)
            break;
    };

    BOOST_REQUIRE_EQUAL(count, 10000);
    BOOST_REQUIRE_EQUAL(gt.getBestScore(), 0.22524215f);

}