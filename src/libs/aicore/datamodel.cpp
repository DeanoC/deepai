//
// Created by Dean Calver on 25/03/2016.
//

#include "core/core.h"
#include "aicore/aicore.h"
#include "rapidjson/document.h"
#include <boost/algorithm/string.hpp>
#include <iostream>
#include "aicore/datamodel.h"

namespace AICore {

    DataModel::DataModel(const std::string &dataJSON) {
        using namespace Core;
        using namespace rapidjson;

        auto &alu = VectorALU<VectorALUBackend::BASIC_CPP>::get();

        rapidjson::Document doc;
//		std::cout << "Starting Parse of " << dataJSON << "\n";

        doc.Parse(dataJSON.c_str());

        if (doc.HasParseError()) {
            // TODO think about error handling...
            std::cout << "DataModel Parse Error " << doc.GetParseError() << " at " << doc.GetErrorOffset() << "\n";
            throw;
        } else {
            // check header and version
            if (!doc.IsObject()) throw;
            if (!doc["datamodel"].IsInt()) throw;
            if (doc["datamodel"].GetInt() != JsonVersion) throw;
            if (!doc.HasMember("datalayout")) throw;
            const auto &datalayout = doc["datalayout"];
            if (!datalayout.IsArray()) throw;

            // reset data
            data.clear();

            for (Value::ConstValueIterator itr = datalayout.Begin(); itr != datalayout.End(); ++itr) {

                // datalayout always have N weights, other optional data specialize them
                const auto &layout = *itr;

                // require a represents field so we can know what/how to use if
                if (!layout.HasMember("represents")) throw;
                std::string rep = layout["represents"].GetString();
                boost::algorithm::to_lower(rep);

                DataType::shared_ptr dataPtr;

                if (rep == std::string("nominal")) {
                    std::shared_ptr<NominalData<>> topPtr = std::make_shared<NominalData<>>(alu);
                    dataPtr = std::static_pointer_cast<DataType>(topPtr);
                } else if (rep == std::string("interval")) {
                    std::shared_ptr<IntervalData<>> topPtr = std::make_shared<IntervalData<>>(alu);
                    dataPtr = std::static_pointer_cast<DataType>(topPtr);
                } else if (rep == std::string("ordinal")) {
                    std::shared_ptr<OrdinalData<>> topPtr = std::make_shared<OrdinalData<>>(alu);
                    dataPtr = std::static_pointer_cast<DataType>(topPtr);
                } else if (rep == std::string("ratio")) {
                    std::shared_ptr<RatioData<>> topPtr = std::make_shared<RatioData<>>(alu);
                    dataPtr = std::static_pointer_cast<DataType>(topPtr);
                }

                if (layout.HasMember("categories")) {
                    const auto &categories = layout["categories"];
                    if (!categories.IsArray()) throw;

                    for (Value::ConstValueIterator itr = categories.Begin();
                         itr != categories.End();
                         ++itr) {
                        dataPtr->addData(itr->GetString());
                    }
                } else {
                    // qualative data must have them for normalisation
                    if (dataPtr->category == DataCategory::Qualitive) throw;
                }

                if (!layout.HasMember("type")) {
                    const auto &jsonType = layout["type"];
                    std::string typeString = jsonType.GetString();
                    boost::algorithm::to_lower(typeString);
                    if (typeString == std::string("Generic")) {
//						fvType = FeatureVectorType::Generic;
                    } else if (typeString == std::string("Input")) {
//						fvType = FeatureVectorType::InputLayer;
                    } else if (typeString == std::string("Output")) {
//						fvType = FeatureVectorType::OutputLayer;
                    }
                }

                addDataType(dataPtr);
            }
        }
    }

}