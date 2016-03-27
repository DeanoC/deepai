//
// Created by Dean Calver on 25/03/2016.
//

#pragma once

namespace AICore {
    template<typename TYPE, typename ALU = std::nullptr_t, unsigned int DIMS = 0, size_t STRIDE = 0>
    class FeatureSpace;

    template<typename TYPE>
    class FeatureSpace<TYPE, std::nullptr_t, 0, 0> {
    public:
        const size_t dimensions;
        const size_t stride;

        using type = TYPE;
        using tuple = std::vector<TYPE>;

        FeatureSpace(const size_t _dims, const size_t _stride) :
                dimensions(_dims),
                stride(_stride) {
        }

        tuple gather(TYPE *data) const {
            tuple results;
            for (int i = 0; i < dimensions; ++i) {
                results[i] = data[i * stride];
            }
            return results;
        }

        void scatter(const tuple &what, type *data) const {
            for (int i = 0; i < dimensions; ++i) {
                data[i * stride] = what[i];
            }
        }

        void scatter(const type *what, type *data) const {
            for (int i = 0; i < dimensions; ++i) {
                data[i * stride] = what[i];
            }
        }

        type euclideanDistance(const type *data) const {
            TYPE result = 0;
            for (int j = 0; j < dimensions; ++j) {
                result += data[j * stride] * data[j * stride];
            }
            result = sqrt(result);
            return result;
        }
    };

    template<typename TYPE, typename ALU>
    class FeatureSpace<TYPE, ALU, 0, 0> {
    public:
        const size_t dimensions;
        const size_t stride;

        using type = TYPE;
        using tuple = typename ALU::template VectorOf<type>;

        FeatureSpace(const size_t _dims, const size_t _stride) :
                dimensions(_dims),
                stride(_stride) {
        }

        const ALU &alu() const {
            return ALU::get();
        }

        ALU &alu() {
            return ALU::get();
        }

        tuple gather(TYPE *data) const {
            tuple tup(dimensions);
            alu().gather(data, dimensions, stride, tup);
            return tup;
        }

        void scatter(const tuple &what, type *data) const {
            alu().scatter(what, dimensions, stride, data);
        }

        void scatter(const type *what, type *data) const {
            alu().scatter(what, dimensions, stride, data);
        }

        type manhattenDistance(const type *a, const type *b) const {
            tuple ga(dimensions);
            alu().gather(a, dimensions, stride, ga);
            return manhattenDistance(ga, b);
        }

        type manhattenDistance(const tuple &a, const type *b) const {
            tuple gb(dimensions);
            tuple out(dimensions);
            alu().gather(b, dimensions, stride, gb);
            alu().sub(a, gb, out);
            return alu().norm1(out);
        }

        type euclideanDistance(const type *a, const type *b) const {
            tuple ga(dimensions);
            alu().gather(a, dimensions, stride, ga);
            return euclideanDistance(ga, b);
        }

        type euclideanDistance(const tuple &a, const type *b) const {
            tuple gb(dimensions);
            tuple out(dimensions);
            alu().gather(b, dimensions, stride, gb);
            alu().sub(a, gb, out);
            return alu().norm2(out);
        }

        type chebyshevDistance(const type *a, const type *b) const {
            tuple ga(dimensions);
            tuple gb(dimensions);
            alu().gather(a, dimensions, stride, ga);
            alu().gather(b, dimensions, stride, gb);
            return chebyshevDistance(ga, gb);
        }

        type chebyshevDistance(const tuple &a, const type *b) const {
            tuple gb(dimensions);
            alu().gather(b, dimensions, stride, gb);
            return chebyshevDistance(a, gb);
        }

        type chebyshevDistance(const type *a, const tuple &b) const {
            tuple ga(dimensions);
            alu().gather(a, dimensions, stride, ga);
            return chebyshevDistance(ga, b);
        }

        type chebyshevDistance(const tuple &a, const tuple &b) const {
            tuple out(dimensions);
            alu().sub(a, b, out);
            return alu().normInfinite(out);
        }

    };
}


