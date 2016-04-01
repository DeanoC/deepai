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

        FeatureVectorBase(const size_t _dims, const size_t _count) :
                count(_count), dimensions(_dims) { }
        virtual ~FeatureVectorBase() { };

        virtual void reserve(const size_t size) = 0;

        virtual void push_back(const double value) = 0;

        virtual void setAt(const size_t index, const double value) = 0;

        const size_t count;
        const size_t dimensions;

    };

    template<typename REAL = float, typename ALU = Core::VectorALU<Core::VectorALUBackend::BASIC_CPP>>
    class FeatureVector : public FeatureVectorBase {
    public:
        using Vector = typename ALU::template VectorOf<REAL>;
        using shared_ptr = std::shared_ptr<FeatureVector<REAL, ALU>>;

        explicit FeatureVector(const size_t _dims, const size_t _count) :
                FeatureVectorBase(_dims, _count) {
            vector.resize(_count * _dims);
        }

        explicit FeatureVector(const size_t _dims, const size_t _count, const REAL *_data) :
                FeatureVectorBase(_dims, _count) {
            vector.resize(_count * _dims);

            for (int j = 0; j < _count * _dims; ++j) {
                vector[j] = _data[j];
            }
        }

        virtual ~FeatureVector() override { };

        const Vector &get() const {
            return vector;
        }

        Vector &get() {
            return vector;
        }

        const REAL *data(size_t index = 0) const {
            return get().data() + index;
        }

        REAL *data(size_t index = 0) {
            return get().data() + index;
        }

        void reserve(const size_t size) override {
            vector.reserve(size);
        }

        void push_back(const double value) override {
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