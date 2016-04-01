//
// Created by Dean Calver on 28/03/2016.
//
#pragma once

#include "core/core.h"
#include "aicore/aicore.h"
#include "core/vectoralu.h"
#include <type_traits>

namespace AICore {

    template<typename REAL = float, typename ALU = Core::VectorALU<Core::VectorALUBackend::BASIC_CPP>>
    class ErrorCalc {
    public:
        typedef ALU alu_type;
        typedef REAL value_type;
        using vector_type = typename ALU::template VectorOf<value_type>;
        using func_type = typename std::add_pointer<value_type(const vector_type &, const vector_type &)>::type;

        static value_type SumOfSquare(const vector_type &perfect, const vector_type &actual) {
            /*T result = T(0);
            for (int i = 0; i < perfect.size(); ++i) {
                const T im = perfect - actual;
                result += im * im;
            }
            return result;*/
            vector_type tmp(perfect.size());
            ALU::get().sub(perfect, actual, tmp);
            ALU::get().mul(tmp, tmp, tmp);
            value_type result = ALU::get().horizSum(tmp);
            return result;
        }

        static value_type MeanSquare(const vector_type &perfect, const vector_type &actual) {
            /* T result = T(0);
            for (int i = 0; i < perfect.size(); ++i) {
                const T im = perfect - actual;
                result += im * im;
            }
            return result / perfect.size();
            */

            vector_type tmp(perfect.size());
            ALU::get().sub(perfect, actual, tmp);
            ALU::get().mul(tmp, tmp, tmp);
            value_type result = ALU::get().horizSum(tmp);

            return result / value_type(perfect.size());
        }

        static value_type RootMeanSquare(const vector_type &perfect, const vector_type &actual) {
            return sqrt(MeanSquare(perfect, actual));
        }


    };

}
