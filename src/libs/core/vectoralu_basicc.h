#pragma once

#include <cassert>
#include <cmath>
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
        void Op(VectorOf<type> const &a, VectorOf<type> const &b, VectorOf<type> &o,
                type (*lamdba)(const type, const type)) const {
			const auto siz = (a.size() < b.size()) ? a.size() : b.size();

			o.resize( siz );

			for( auto i = 0;i < siz; ++i ){
				o[i] = lamdba( a[i], b[i] );
			}
		}

        template<typename type>
        void add(const VectorOf<type> &a, const VectorOf<type> &b, VectorOf<type> &o) const
		{
            Op<type>(a, b, o, [](const type av, const type bv) -> type { return av + bv; });
		}

        template<typename type>
        void sub(const VectorOf<type> &a, const VectorOf<type> &b, VectorOf<type> &o) const
		{
            Op<type>(a, b, o, [](const type av, const type bv) -> type { return av - bv; });
		}

        template<typename type>
        void mul(const VectorOf<type> &a, const VectorOf<type> &b, VectorOf<type> &o) const
		{
            Op<type>(a, b, o, [](const type av, const type bv) -> type { return av * bv; });
		}

        template<typename type>
        void div(const VectorOf<type> &a, const VectorOf<type> &b, VectorOf<type> &o) const
		{
            Op<type>(a, b, o, [](const type av, const type bv) -> type { return av / bv; });
		}

        template<typename type>
        void horizSum(const VectorOf<type> &a, type &out) const {
            out = static_cast<type>(0);
            for (int i = 0; i < a.size(); ++i) {
                out = out + a[i];
            }
        };

        template<typename type>
        type horizSum(const VectorOf<type> &a) const {
            type out;
            horizSum(a, out);
            return out;
        };

        template<typename type>
        void abs(const VectorOf<type> &a, VectorOf<type> &out) const {
            for (int i = 0; i < a.size(); ++i) {
                out[i] = std::abs(a[i]);
            }
        };

        template<typename type>
        void set(const type value, VectorOf<type> &out) {
            for (int i = 0; i < out.size(); ++i) {
                out[i] = value;
            }
        }


        template<typename type>
        type norm1(const VectorOf<type> &a) const {
            VectorOf<type> res;
            abs(a, res);
            return horizSum(res);
        }

        template<typename type>
        type norm2(const VectorOf<type> &a) const {
            VectorOf<type> res;
            mul(a, a, res);
            return std::sqrt(horizSum(res));
        }

        template<typename type>
        type norm3(const VectorOf<type> &a) const {
            VectorOf<type> res, res2;
            mul(a, a, res);
            mul(res, res, res2);
            return std::cbrt(horizSum(res2));
        }

        template<typename type>
        type normInfinite(const VectorOf<type> &a) const {
            VectorOf<type> res;
            abs(a, res);
            auto mm = minMaxOf(res);
            return mm.second;
        }

        template<typename type>
        bool compareEquals(const VectorOf<type> &a, const VectorOf<type> &b) const
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
        bool compareNotEquals(const VectorOf<type> &a, const VectorOf<type> &b) const
		{
			return !compareEquals(a,b);
		}

        template<typename type>
        bool compareAllGreater(const VectorOf<type> &a, const VectorOf<type> &b) const
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
        ResultPair<type> minMaxOf(const VectorOf<type> &in) const {
			// default to min and max of the type held in the container
            type mini = std::numeric_limits<type>::max();
            type maxi = std::numeric_limits<type>::min();
			
			for( auto it : in ) {
				mini = std::min( it, mini );
				maxi = std::max( it, maxi );
			}

            return ResultPair<type>(mini, maxi);
		}

        template<typename type>
        void gather(const type *data, const unsigned int num, const size_t stride, VectorOf<type> &out) const {
            for (int i = 0; i < num; ++i) {
                out[i] = data[i * stride];
            }
        }

        template<typename type>
        void scatter(const VectorOf<type> &in, const unsigned int num, const size_t stride, type *data) const {
            for (int i = 0; i < num; ++i) {
                data[i * stride] = in[i];
            }
        }

        template<typename type>
        void scatter(const type *&in, const unsigned int num, const size_t stride, type *data) const {
            for (int i = 0; i < num; ++i) {
                data[i * stride] = in[i];
            }
        }
    protected:
        VectorALU<VectorALUBackend::BASIC_CPP>() { };
	};	
}