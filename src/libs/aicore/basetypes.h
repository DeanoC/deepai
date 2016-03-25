#pragma once

#include "core/core.h"
#include "aicore/aicore.h"

namespace AICore {


    enum DataCategory : uint8_t {
		Qualitive,	// categorical data
		Quantative, // numerical data
	};

    enum DataType : uint8_t {
		Nominal,
		Ordinal,
		Interval,
		Ratio,
	};

    enum DataTypeUsage : uint8_t {
        Input,
        Output,
        Generic
    };

    struct DataTypeInfo {
        DataTypeInfo(DataType _type) :
                type(_type),
                category((_type == DataType::Nominal || _type == DataType::Ordinal) ?
                         DataCategory::Qualitive : DataCategory::Quantative),
                usage(DataTypeUsage::Generic),
                index(0) { }

        using shared_ptr = std::shared_ptr<DataTypeInfo>;

        virtual ~DataTypeInfo() { };

        virtual void addData(const std::string &data) = 0;

        virtual void setIndex(const uint8_t index) = 0;

        virtual void setUsage(const DataTypeUsage usage) = 0;

        virtual void setRange(const double &low, const double &far) = 0;
        virtual void setRange(const std::string &low, const std::string &far) = 0;

        const DataType type;
        const DataCategory category;
        const DataTypeUsage usage;
        const uint8_t index;
    };

	template<typename T, typename ALU>
    struct DataTypeBase : public DataTypeInfo {

        virtual void setIndex(const uint8_t _index) override {
            auto &writeIndex = const_cast<uint8_t &>(index);
            writeIndex = _index;
        }

        void setUsage(const DataTypeUsage _usage) override {
            auto &writeUsage = const_cast<DataTypeUsage &>(usage);
            writeUsage = _usage;
        }

        void setRange(const std::string &low, const std::string &far) override {
            range = range_type(boost::lexical_cast<T>(low), boost::lexical_cast<T>(far));
        }

        void setRange(const double &low, const double &far) override {
            range = range_type(static_cast<T>(low), static_cast<T>(far));
        }

        explicit DataTypeBase(const DataType _type, ALU &_alu) :
                DataTypeInfo(_type), processor(_alu) { }

        typedef DataTypeInfo super;

        using super::type;
        using super::category;

        using container_type = typename ALU::template VectorOf<T>;

		typedef ALU alu_type;
        typedef T value_type;
        typedef std::pair<T, T> range_type;

		container_type data;
		alu_type& processor;
        range_type range;
	};

}
#include "aicore/nominaldata.h"
#include "aicore/ordinaldata.h"
#include "aicore/intervaldata.h"
#include "aicore/ratiodata.h"