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
        Generic,

        COUNT
    };

    enum DataTypeNormalisation : uint8_t {
        scaling0to1,
        scalingNeg1ToPos1,
        oneOfN,
        equilateral
    };

    struct DataTypeInfo {
        DataTypeInfo(DataType _type) :
                type(_type),
                category((_type == DataType::Nominal || _type == DataType::Ordinal) ?
                         DataCategory::Qualitive : DataCategory::Quantative),
                usage(DataTypeUsage::Generic),
                index(0),
                normalisationMethod(DataTypeNormalisation::scaling0to1) { }

        using shared_ptr = std::shared_ptr<DataTypeInfo>;

        virtual ~DataTypeInfo() { };

        virtual void addData(const std::string &data) = 0;

        // only qualative should implement this
        virtual void addCategory(const std::string &data) = 0;

        virtual size_t numCategories() = 0;

        virtual size_t size() const = 0;

        void setIndex(const uint8_t _index) {
            auto &writeIndex = const_cast<uint8_t &>(index);
            writeIndex = _index;
        }

        void setUsage(const DataTypeUsage _usage) {
            auto &writeUsage = const_cast<DataTypeUsage &>(usage);
            writeUsage = _usage;
        }

        void setNormalisationMethod(const DataTypeNormalisation _method) {
            auto &write = const_cast<DataTypeNormalisation &>(normalisationMethod);
            write = _method;
        }

        void setRange(const std::string &low, const std::string &far) {
            range = range_type(boost::lexical_cast<double>(low), boost::lexical_cast<double>(far));
        }

        void setRange(const double &low, const double &far) {
            range = range_type(low, far);
        }

        const DataType type;
        const DataCategory category;
        const DataTypeUsage usage;
        const uint8_t index;
        const DataTypeNormalisation normalisationMethod;

        typedef std::pair<double, double> range_type;
        range_type range;
    };

    template<DataCategory CAT>
    struct DataTypeBase;

    template<>
    struct DataTypeBase<DataCategory::Qualitive> : public DataTypeInfo {
        typedef std::string value_type;

        size_t size() const override {
            return data.size();
        }

        explicit DataTypeBase(const DataType _type) :
                DataTypeInfo(_type) { }

        typedef DataTypeInfo super;

        using super::type;
        using super::category;

        using container_type = std::vector<value_type>;

        void addData(const std::string &_data) override {
            // must be a string category name incoming
            data.push_back(_data);
        }

        void addCategory(const std::string &_cate) override {
            categories.push_back(_cate);
        }

        virtual size_t numCategories() override {
            return categories.size();
        }

        container_type data;
        container_type categories;
    };

    template<>
    struct DataTypeBase<DataCategory::Quantative> : public DataTypeInfo {
        typedef double value_type;

        size_t size() const override {
            return data.size();
        }

        explicit DataTypeBase(const DataType _type) :
                DataTypeInfo(_type) { }

        typedef DataTypeInfo super;

        using super::type;
        using super::category;

        using container_type = std::vector<value_type>;

        void addData(const std::string &_data) override {
            // must be numeric data incoming
            data.push_back(boost::lexical_cast<value_type>(_data));
        }

        void addData(const value_type _data) {
            data.push_back(_data);
        }

        void addCategory(const std::string &_cate) override {
            assert(false);
        }

        virtual size_t numCategories() override {
            assert(false);
            return 0;
        }


        container_type data;
	};

}
#include "aicore/nominaldata.h"
#include "aicore/ordinaldata.h"
#include "aicore/intervaldata.h"
#include "aicore/ratiodata.h"