#pragma once

#include <cassert>
#include "core/core.h"
#include "core/vectoralu.h"

namespace Core {

    template<>
    struct VectorALU<VectorALUBackend::BASIC_CPP> {

        template<typename type> using ResultPair = std::pair<type, type>;
        template<typename type> using VectorOf = std::vector<type>;

        typedef VectorOf<float> VectorOfFloats;
        typedef VectorOf<double> VectorOfDoubles;

        static bool init() { return true; }

        static VectorALU &get() {
            static VectorALU alu;
            return alu;
        }


        template<typename type>
        static void Op(VectorOf<type> const &a, VectorOf<type> const &b, VectorOf<type> &o,
                       type (*lamdba)(const type, const type)) {
			const auto siz = (a.size() < b.size()) ? a.size() : b.size();

			o.resize( siz );

			for( auto i = 0;i < siz; ++i ){
				o[i] = lamdba( a[i], b[i] );
			}
		}

        template<typename type>
        static void add(const VectorOf<type> &a, const VectorOf<type> &b, VectorOf<type> &o)
		{
            Op<type>(a, b, o, [](const type av, const type bv) -> type { return av + bv; });
		}

        template<typename type>
        static void sub(const VectorOf<type> &a, const VectorOf<type> &b, VectorOf<type> &o)
		{
            Op<type>(a, b, o, [](const type av, const type bv) -> type { return av - bv; });
		}

        template<typename type>
        static void mul(const VectorOf<type> &a, const VectorOf<type> &b, VectorOf<type> &o)
		{
            Op<type>(a, b, o, [](const type av, const type bv) -> type { return av * bv; });
		}

        template<typename type>
        static void div(const VectorOf<type> &a, const VectorOf<type> &b, VectorOf<type> &o)
		{
            Op<type>(a, b, o, [](const type av, const type bv) -> type { return av / bv; });
		}

        template<typename type>
        bool compareEquals(const VectorOf<type> &a, const VectorOf<type> &b)
		{
			auto bit = b.begin();
			for( auto ait : a ) {
				if( ait != *bit ) {
					return false;
				}
				++bit;
			}

			return true;
		}

        template<typename type>
        bool compareNotEquals(const VectorOf<type> &a, const VectorOf<type> &b)
		{
			return !compareEquals(a,b);
		}

        template<typename type>
        bool compareAllGreater(const VectorOf<type> &a, const VectorOf<type> &b)
		{
			auto bit = b.begin();
			for( auto ait : a ) {
				if( ait <= *bit ) {
					return false;
				}
				++bit;
			}

			return true;
		}

        template<typename type>
        bool compareAllLess(const VectorOf<type> &a, const VectorOf<type> &b)
		{
			auto bit = b.begin();
			for( auto ait : a ) {
				if( ait >= *bit ) {
					return false;
				}
				++bit;
			}

			return true;
		}


        template<typename type>
        ResultPair<type> minMaxOf(const VectorOf<type> &in) {
			// default to min and max of the type held in the container
            type mini = std::numeric_limits<type>::max();
            type maxi = std::numeric_limits<type>::min();
			
			for( auto it : in ) {
				mini = std::min( it, mini );
				maxi = std::max( it, maxi );
			}

            return ResultPair<type>(mini, maxi);
		}

    protected:
        VectorALU<VectorALUBackend::BASIC_CPP>() { };
	};	
}