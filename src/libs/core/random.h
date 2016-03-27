//
// Created by Dean Calver on 25/03/2016.
//

#pragma once

#include "core/core.h"
#include <ctime>
#include <boost/random.hpp>

namespace Core {

    // thin wrapper around boost random, just a singleton
    // with a easy place to change and seed
    struct Random {
        typedef boost::random::mt19937 generator_type;

        static void seed(unsigned int seed = static_cast<unsigned int>(std::time(0))) {
            generator.seed(seed);
        }

        static generator_type generator;
    };
}