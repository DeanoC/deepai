#pragma once

#include "aicore/aicore.h"
#include "core/vectoralu.h"
#include "core/vectoralu_basicc.h"
#include "aicore/basetypes.h"
#include "rapidjson/document.h"

namespace AICore {

    class DataModel {
    public:
        using DataType = DataTypeInfo;
        typedef std::vector<typename DataType::shared_ptr> FeatureContainer;

        typedef std::shared_ptr<DataModel> shared_ptr;

        static const int JsonVersion = 0;

        DataModel(const std::string &inputString);

        void AddFeature(typename DataType::shared_ptr &da) {
            features.push_back(da);
        }

        size_t dimensionOfFeatures() const { return features.size(); }

        typename DataType::shared_ptr getFeature(size_t i) { return features[i]; }

        FeatureContainer &getFeatures() { return features; }

    protected:
        FeatureContainer features;

        void extractDataTypeProperties(const rapidjson::GenericValue<rapidjson::UTF8<>> &layout,
                                       DataTypeInfo::shared_ptr &dataPtr) const;
    };


}

