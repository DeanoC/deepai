#pragma once

namespace AICore {

    enum FeatureVectorType : uint8_t {
        Generic = 0,
        InputLayer,
        OutputLayer,
        HiddenLayer
    };

    struct FeatureVectorBase {
        using shared_ptr = std::shared_ptr<FeatureVectorBase>;

        FeatureVectorBase(FeatureVectorType _type) : type(_type) { }

        const FeatureVectorType type;

        virtual ~FeatureVectorBase() { };

        virtual std::shared_ptr <FeatureVectorBase> clone() const = 0;

    };

    template<typename REAL = float, typename ALU = Core::VectorALU <Core::VectorALUBackend::BASIC_CPP>>
    class FeatureVector : public FeatureVectorBase {
    public:
        using StateVectorType = typename ALU::template VectorOf<REAL>;
        using WeightVectorType = typename ALU::template VectorOf<REAL>;

        using shared_ptr = std::shared_ptr<FeatureVector<REAL, ALU>>;

        FeatureVector(const unsigned int _count) :
                FeatureVectorBase(FeatureVectorType::Generic),
                weights(_count),
                states(_count) { }

        FeatureVector(const WeightVectorType &_wht) :
                FeatureVectorBase(FeatureVectorType::Generic),
                weights(_wht),
                states(_wht.size()) { }

        FeatureVector(const WeightVectorType &_wht, const StateVectorType &_state) :
                FeatureVectorBase(FeatureVectorType::Generic),
                weights(_wht),
                states(_state) { }

        std::shared_ptr <FeatureVectorBase> clone() const override {
            shared_ptr other = std::make_shared<FeatureVector<REAL, ALU>>(weights, states);
            assert(other->Type() == FeatureVectorType::Generic);
            return std::static_pointer_cast<FeatureVectorBase>(other);
        }

        virtual ~FeatureVector() { };

        FeatureVectorType Type() const { return type; }

        void setWeights(const WeightVectorType &in) {
            weights = in;
        }

        const WeightVectorType &getWeights() const { return weights; }

        const StateVectorType &getStates() const { return states; }

        bool operator==(const FeatureVector<REAL, ALU> &b) {
            if (Type() != b.Type()) return false;
            if (getWeights() != b.getWeights()) return false;
            if (getStates() != b.getStates()) return false;

            return true;
        }

        bool operator!=(const FeatureVector<REAL, ALU> &b) {
            return !operator==(b);
        }


    protected:
        // sub classes use this to set the type before filling the data
        FeatureVector(const FeatureVectorType _type) : FeatureVectorBase(_type) { }

        WeightVectorType weights;
        StateVectorType states;
    };
//	template< typename REAL float>
//	class WeightedFeatureVector {
//
//	};
}