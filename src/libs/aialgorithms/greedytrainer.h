//
// Created by Dean Calver on 28/03/2016.
//

#pragma once

#include <aicore/errorcalc.h>
#include <boost/generator_iterator.hpp>
#include <core/vectoralu_basicc.h>
#include "core/core.h"
#include "aicore/aicore.h"
#include "aicore/featurevector.h"
#include "aicore/featurespace.h"
#include "aicore/errorcalc.h"


namespace AIAlgorithm {

    template<typename REAL,
            typename ALU = Core::VectorALU<Core::VectorALUBackend::BASIC_CPP> >
    class GreedyTrainer {
    public:
        typedef REAL value_type;
        typedef ALU alu_type;
        using range_type = typename ALU::template Range<value_type>;

        typedef AICore::FeatureVector<value_type, alu_type> FVector;
        typedef AICore::FeatureSpace<value_type, alu_type> FSpace;
        typedef boost::uniform_real<> distribution_type;
        typedef boost::variate_generator<Core::Random::generator_type &, distribution_type> gen_type;

        GreedyTrainer(const typename FVector::shared_ptr _trainingVector,
                      const range_type &_randomRange,
                      const REAL _maxErr = REAL(1e-4)) :
                trainingVector(std::static_pointer_cast<FVector>(_trainingVector)),
                randGen(Core::Random::generator, distribution_type(_randomRange.first, _randomRange.second)),
                randIter(&randGen),
                maxErr(_maxErr) {
        }

        bool iterate(AICore::FeatureVectorBase::shared_ptr _featureVector) {
            typename FVector::shared_ptr fv(std::static_pointer_cast<FVector>(_featureVector));

            // generate new random data
            typename FVector::shared_ptr newVec = std::make_shared<FVector>(trainingVector->dimensions, fv->count);
            for (auto &&item :newVec->get()) {
                item = *randIter++;
            }
            auto newScore = calculateScore(newVec);
            if (newScore < bestScore) {
                _featureVector = newVec;
                bestScore = newScore;
            }
            return (bestScore < maxErr) ? true : false;
        }

        // generally use iterate, but here if you want the score without the iterations
        value_type calculateScore(const typename FVector::shared_ptr _featureVector) {
            typename FVector::shared_ptr fv(std::static_pointer_cast<FVector>(_featureVector));
            assert(fv->dimensions == trainingVector->dimensions);
            assert(fv->count == trainingVector->count);
            return AICore::ErrorCalc<REAL, ALU>::RootMeanSquare(trainingVector->get(), fv->get());
        }

        value_type getBestScore() const { return bestScore; }

    protected:
        const typename FVector::shared_ptr trainingVector;
        const REAL maxErr;

        gen_type randGen;
        boost::generator_iterator<gen_type> randIter;

        value_type bestScore = std::numeric_limits<value_type>::infinity();

    };
}