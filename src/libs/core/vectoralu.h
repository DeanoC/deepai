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
        void add(const VectorOf<type> &a, const VectorOf<type> &b, VectorOf<type> &out) { };

        template<typename type>
        void sub(const VectorOf<type> &a, const VectorOf<type> &b, VectorOf<type> &out) { };

        template<typename type>
        void mul(const VectorOf<type> &a, const VectorOf<type> &b, VectorOf<type> &out) { };

        template<typename type>
        void div(const VectorOf<type> &a, const VectorOf<type> &b, VectorOf<type> &out) { };

        template<typename type>
        void horizSum(const VectorOf<type> &a, VectorOf<type> &out) { };

        template<typename type>
        type horizSum(const VectorOf<type> &a) { };

        template<typename type>
        void abs(const VectorOf<type> &a, VectorOf<type> &out) { };

        template<typename type>
        void set(const type value, VectorOf<type> &out) { }

        template<typename type>
        ResultPair<type> minMaxOf(const VectorOf<type> &input) { };

        template<typename type>
        bool compareEquals(const VectorOf<type> &a, const VectorOf<type> &b) { };

        template<typename type>
        bool compareNotEquals(const VectorOf<type> &a, const VectorOf<type> &b) { };

        template<typename type>
        bool compareAllGreater(const VectorOf<type> &a, const VectorOf<type> &b) { };

        template<typename type>
        bool compareAllLess(const VectorOf<type> &a, const VectorOf<type> &b) { };

        template<typename type>
        void gather(const type *data, const unsigned int num, const size_t stride, VectorOf<type> &out) { }

        template<typename type>
        void scatter(const VectorOf<type> &in, const unsigned int num, const size_t stride, type *data) { }

//		static bool init();
//		static VectorALU<backend> get();
    protected:
        VectorALU<backend>();
    };
}