#pragma once

#include "core/core.h"
#include "aicore/aicore.h"

namespace AICore {
	

	enum DataCategory{
		Qualitive,	// categorical data
		Quantative, // numerical data
	};

	enum DataType{
		Nominal,
		Ordinal,
		Interval,
		Ratio,
	};

    struct DataTypeInfo {
        DataTypeInfo(DataType _type) :
                type(_type),
                category((_type == DataType::Nominal || _type == DataType::Ordinal) ?
                         DataCategory::Qualitive : DataCategory::Quantative) { }

        using shared_ptr = std::shared_ptr<DataTypeInfo>;

        virtual ~DataTypeInfo() { };

        virtual void addData(const std::string &data) = 0;

        virtual void setRange(const std::string &low, const std::string &far) = 0;

        const DataType type;
        const DataCategory category;
    };

	template<typename T, typename ALU>
    struct DataTypeBase : public DataTypeInfo {

        explicit DataTypeBase(const DataType _type, ALU &_alu) :
                DataTypeInfo(_type), processor(_alu) { }

        typedef DataTypeInfo super;

        void setRange(const std::string &low, const std::string &far) override {
            range = range_type(boost::lexical_cast<T>(low), boost::lexical_cast<T>(far));
        }

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