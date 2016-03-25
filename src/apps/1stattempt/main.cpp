#include "core/core.h"
#include "aicore/aicore.h"
#include <iostream>
#include <boost/range/irange.hpp>
#include <boost/filesystem.hpp>
#include <aicore/featurevector.h>
#include "boost/program_options.hpp"

#include "aicore/datamodel.h"

#include "readcvs.h"

int main(int argc, char **argv)
{

    using namespace AICore;
    using namespace Core;

    namespace fs = boost::filesystem;
    namespace po = boost::program_options;

    // Declare the supported options.
    po::options_description desc("Allowed options");
    desc.add_options()
            ("help", "produce help message");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << desc << "\n";
        return 1;
	}

    const fs::path iris_model(boost::filesystem::current_path().append(
            "iris_datamodel.json"));
    const fs::path iris_data(boost::filesystem::current_path().append(
            "iris.csv"));

    try {

        if (!fs::exists(iris_model)) {
            std::cerr << "Data descriptor (" << iris_model << ") not found\n";
            return 1;
        }

        if (!fs::exists(iris_data)) {
            std::cerr << "Data (" << iris_data << ") not found\n";
            return 1;
        }
    } catch (fs::filesystem_error &err) {
        std::cerr << err.what();
        return 1;
    }

    const auto fileProps = std::ios::in | std::ios::binary | std::ios::ate;

    size_t fileSize;
    std::ifstream modelifs(iris_model.filename().c_str(), fileProps);
    fileSize = (size_t) modelifs.tellg();
    modelifs.seekg(0, std::ios::beg);
    std::vector<char> bytes(fileSize);
    modelifs.read(&bytes[0], fileSize);

    // datamodel describes the data, and the initial data but not the working data
    DataModel::shared_ptr model = std::make_shared<DataModel>(std::string(&bytes[0], fileSize));

    // now we need to create a working data set ready to pump data through it
    // each DataType in the model counts as a feature set dimension
    // each DataType will require a normaliser to encode it and vice versa
    // afterwards.

    std::ifstream dataifs(iris_data.filename().c_str(), fileProps);
    if (dataifs.bad()) {
        std::cerr << "Error opening " << iris_data << "\n";
        return 1;
    }

    // Stream from start of file
    dataifs.seekg(0, std::ios::beg);

    const auto csv = Util::readCSV(dataifs);
    // check number of features matching number of columns in CVS
    assert(csv.size() > 0);
    assert(csv[0].size() == model->dimensionOfFeatures());

    auto &features = model->getFeatures();

    for (auto row = csv.begin(); row != csv.end(); ++row) {

        // skip invalid rows;
        if (row->size() != model->dimensionOfFeatures()) continue;

        int i = 0;
        for (auto &&val : *row) {
            features[i]->addData(val);
            ++i;
        }
    }

    AICore::FeatureVector<float> fv(model->sizeOfFeatures(), model->dimensionOfFeatures());
    model->fill(fv);


	return 0;	

}