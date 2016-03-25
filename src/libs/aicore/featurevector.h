#pragma once

#include <cstdint>
#include <memory>
#include <cassert>
#include <array>
#include <core/vectoralu.h>
#include "basetypes.h"

namespace AICore {

    struct FeatureVectorBase {
        using shared_ptr = std::shared_ptr<FeatureVectorBase>;

        FeatureVectorBase() { }
        virtual ~FeatureVectorBase() { };

        virtual size_t size() = 0;

        virtual void reserve(const size_t size) = 0;

        virtual void push_back(const double value) = 0;

        virtual void setAt(const size_t index, const double value) = 0;

    };

    template<typename REAL = float, typename ALU = Core::VectorALU<Core::VectorALUBackend::BASIC_CPP>>
    class FeatureVector : public FeatureVectorBase {
    public:
        using Vector = typename ALU::template VectorOf<REAL>;
        using shared_ptr = std::shared_ptr<FeatureVector<REAL, ALU>>;

        FeatureVector(const unsigned int _count, const unsigned int _dimensions) {
            vector.resize(_count * _dimensions);
        }

        virtual ~FeatureVector() override { };

        const Vector &get() const {
            return vector;
        }

        virtual size_t size() override {
            return vector.size();
        }

        virtual void reserve(const size_t size) override {
            vector.reserve(size);
        }

        virtual void push_back(const double value) override {
            vector.push_back(static_cast<REAL>(value));
        }

        virtual void setAt(const size_t index, const double value) override {
            vector.at(index) = static_cast<REAL>(value);
        }


        bool operator==(const FeatureVector<REAL, ALU> &b) {
            return (vector == b.vector);
        }

        bool operator!=(const FeatureVector<REAL, ALU> &b) {
            return !operator==(b);
        }

    protected:
        Vector vector;
    };
}