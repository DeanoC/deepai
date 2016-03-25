#pragma once

namespace Core {

    enum class VectorALUBackend : uint8_t {
		BASIC_CPP
	};


    template<VectorALUBackend backend>
    struct VectorALU {
        // these are never used, consider this the 'interface' which each ALU backend should support at a minimum

        template<typename type> using VectorOf = std::vector<type>;
        typedef VectorOf<float> VectorOfFloats;
        typedef VectorOf<double> VectorOfDoubles;
        template<typename type> using ResultPair = std::pair<type, type>;


        template<typename type>
        void add(const type &a, const type &b, type &out) { };

        template<typename type>
        void sub(const type &a, const type &b, type &out) { };

        template<typename type>
        void mul(const type &a, const type &b, type &out) { };

        template<typename type>
        void div(const type &a, const type &b, type &out) { };

        template<typename type>
        ResultPair<type> minMaxOf(const type &input) { };

        template<typename type>
        bool compareEquals(const type &a, const type &b) { };

        template<typename type>
        bool compareNotEquals(const type &a, const type &b) { };

        template<typename type>
        bool compareAllGreater(const type &a, const type &b) { };

        template<typename type>
        bool compareAllLess(const type &a, const type &b) { };


        template<typename type>
        void normaliseData0to1(const type &in, type &out) { };

        template<typename type>
        void normaliseDataNeg1toPos1(const type &in, type &out) { };

//		static bool init();
//		static VectorALU<backend> get();
    protected:
        VectorALU<backend>();
    };
}