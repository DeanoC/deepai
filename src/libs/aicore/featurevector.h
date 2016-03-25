#pragma once

#include <cstdint>
#include <memory>
#include <cassert>
#include <array>
#include <core/vectoralu.h>

namespace AICore {

    struct FeatureVectorBase {
        using shared_ptr = std::shared_ptr<FeatureVectorBase>;

        FeatureVectorBase() { }

        virtual ~FeatureVectorBase() { };

        virtual std::shared_ptr <FeatureVectorBase> clone() const = 0;

    };

    template<typename REAL = float, unsigned int DIM = 1, typename ALU = Core::VectorALU<Core::VectorALUBackend::BASIC_CPP>>
    class FeatureVector : public FeatureVectorBase {
    public:
        using StateVectorType = typename ALU::template VectorOf<REAL>;

        using shared_ptr = std::shared_ptr<FeatureVector<REAL, DIM, ALU>>;

        FeatureVector(const unsigned int _count[DIM]) {
            for (auto &&item : states) {
                item.resize(_count);
            }
        }

        std::shared_ptr <FeatureVectorBase> clone() const override {
            shared_ptr other = std::make_shared<FeatureVector<REAL, DIM, ALU>>(states);
            return std::static_pointer_cast<FeatureVectorBase>(other);
        }

        virtual ~FeatureVector() { };


        const StateVectorType &getStates(const unsigned int dim) const {
            return states[dim];
        }

        bool operator==(const FeatureVector<REAL, DIM, ALU> &b) {
            return (states == b.states);
        }

        bool operator!=(const FeatureVector<REAL, DIM, ALU> &b) {
            return !operator==(b);
        }

    protected:
        std::array<StateVectorType, DIM> states;
    };
}