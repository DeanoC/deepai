#pragma once

#include "aicore/aicore.h"
#include "core/vectoralu.h"
#include "core/vectoralu_basicc.h"
#include "aicore/basetypes.h"

namespace AICore {

    class DataModel {
    public:

        using DataType = DataTypeInfo;

        typedef std::shared_ptr<DataModel> shared_ptr;

        static const int JsonVersion = 0;

        DataModel(const std::string &inputString);

        void addDataType(typename DataType::shared_ptr &da) {
            data.push_back(da);
        }

    protected:
        std::vector<typename DataType::shared_ptr> data;
    };


}

