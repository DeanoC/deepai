//
// Created by Dean Calver on 25/03/2016.
//

#include "core/core.h"
#include "aicore/aicore.h"
#include "rapidjson/document.h"
#include <boost/algorithm/string.hpp>
#include <iostream>
#include <set>
#include <unordered_map>
#include <map>
#include "aicore/featurevector.h"
#include "aicore/datamodel.h"


namespace AICore {

    DataModel::DataModel(const std::string &dataJSON) {
        using namespace Core;
        using namespace rapidjson;

        // by default no feature is ignored
        std::fill(ignoreArray.begin(), ignoreArray.end(), 0);

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

            // reset features
            features.clear();

            for (Value::ConstValueIterator itr = datalayout.Begin(); itr != datalayout.End(); ++itr) {

                const auto &layout = *itr;

                // require a represents field so we can know what/how to use if
                if (!layout.HasMember("represents")) throw;
                std::string rep = layout["represents"].GetString();
                boost::algorithm::to_lower(rep);

                DataType::shared_ptr dataPtr;

                if (rep == std::string("nominal")) {
                    auto topPtr = std::make_shared<NominalData>();
                    dataPtr = std::static_pointer_cast<DataType>(topPtr);
                    topPtr->setNormalisationMethod(DataTypeNormalisation::oneOfN);
                    extractDataTypeProperties(layout, dataPtr);
                } else if (rep == std::string("interval")) {
                    auto topPtr = std::make_shared<IntervalData>();
                    dataPtr = std::static_pointer_cast<DataType>(topPtr);
                    topPtr->setNormalisationMethod(DataTypeNormalisation::oneOfN);
                    extractDataTypeProperties(layout, dataPtr);
                } else if (rep == std::string("ordinal")) {
                    auto topPtr = std::make_shared<OrdinalData>();
                    dataPtr = std::static_pointer_cast<DataType>(topPtr);
                    topPtr->setNormalisationMethod(DataTypeNormalisation::scalingNeg1ToPos1);
                    extractDataTypeProperties(layout, dataPtr);
                } else if (rep == std::string("ratio")) {
                    auto topPtr = std::make_shared<RatioData>();
                    dataPtr = std::static_pointer_cast<DataType>(topPtr);
                    topPtr->setNormalisationMethod(DataTypeNormalisation::scalingNeg1ToPos1);
                    extractDataTypeProperties(layout, dataPtr);
                }

                AddFeature(dataPtr);
            }
        }
    }

    void DataModel::extractDataTypeProperties(const rapidjson::GenericValue<rapidjson::UTF8<>> &layout,
                                              DataTypeInfo::shared_ptr &dataPtr) const {
        using namespace Core;
        using namespace rapidjson;

        double low = 1.0;
        double high = 1.0;
        if (layout.HasMember("range")) {
            const auto &range = layout["range"];
            if (!range.IsArray()) throw;
            if (range.Size() != 2) throw;

            low = range[0].GetDouble();
            high = range[1].GetDouble();

        }
        dataPtr->setRange(low, high);

        if (layout.HasMember("type")) {
            const auto &jsonType = layout["type"];
            std::string typeString = jsonType.GetString();
            boost::algorithm::to_lower(typeString);

            DataTypeUsage usage = DataTypeUsage::Generic;

            if (typeString == std::string("generic")) {
                usage = DataTypeUsage::Generic;
            } else if (typeString == std::string("input")) {
                usage = DataTypeUsage::Input;
            } else if (typeString == std::string("output")) {
                usage = DataTypeUsage::Output;
            } else {
                throw;
            }
            dataPtr->setUsage(usage);
        }

        if (layout.HasMember("categories")) {
            const auto &categories = layout["categories"];
            if (!categories.IsArray()) throw;

            for (Value::ConstValueIterator itr = categories.Begin();
                 itr != categories.End();
                 ++itr) {
                dataPtr->addCategory(itr->GetString());
            }
        } else {
            // qualitative features must have them for normalisation
            if (dataPtr->category == DataCategory::Qualitive) throw;
        }
    }

    size_t DataModel::dimensionOfFeatures() const {
        size_t numDims = 0;
        for (auto &&fv : features) {
            if (ignoreArray[(int) fv->usage] > 0) continue;

            if (fv->normalisationMethod == DataTypeNormalisation::oneOfN) {
                numDims += fv->numCategories();
            } else if (fv->normalisationMethod == DataTypeNormalisation::equilateral) {
                numDims += fv->numCategories() - 1;
            } else {
                numDims += 1;
            }
        }

        return numDims;
    }


    void DataModel::fill(FeatureVectorBase::shared_ptr base) {
        using namespace Core;
        using namespace AICore;

        // base is already sized to size * dim
        size_t currentIndex = 0;
        for (auto &&fv : features) {
            if (ignoreArray[(int) fv->usage] > 0) continue;

            switch (fv->category) {
                case DataCategory::Qualitive: {
                    auto q = std::static_pointer_cast<const DataTypeBase<DataCategory::Qualitive>>(fv);

                    switch (fv->normalisationMethod) {
                        case DataTypeNormalisation::oneOfN: {
                            // whole vector per enum value
                            for (int i = 0; i < fv->numCategories(); ++i) {
                                for (auto &&item : q->data) {
                                    if (item == q->categories[i]) {
                                        base->setAt(currentIndex++, 1.0);
                                    } else {
                                        base->setAt(currentIndex++, -1.0);
                                    }
                                }

                            }
                        }
                            break;
                        default: // TODO other normalisation
                            assert(false);
                    }
                }
                    break;
                case DataCategory::Quantative: {
                    auto q = std::static_pointer_cast<const DataTypeBase<DataCategory::Quantative>>(fv);

                    const double rangeLow = fv->range.first;
                    const double rangeWidth = fv->range.second - fv->range.first;
                    switch (fv->normalisationMethod) {
                        case DataTypeNormalisation::scaling0to1: {
                            for (auto &&item : q->data) {
                                base->setAt(currentIndex++, (item - rangeLow) / rangeWidth);
                            }
                        }
                            break;
                        case DataTypeNormalisation::scalingNeg1ToPos1: {
                            for (auto &&item : q->data) {
                                base->setAt(currentIndex++, ((item - rangeLow) / rangeWidth) * 2.0 - 1.0);
                            }
                        }
                            break;
                        default:
                            assert(false);
                    }
                }
                    break;
            }
        }
    }
}