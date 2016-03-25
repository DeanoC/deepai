#include "core/core.h"
#include "aicore/aicore.h"
#include <iostream>
#include <boost/range/irange.hpp>
#include <boost/filesystem.hpp>
#include "boost/program_options.hpp"

#include "aicore/basetypes.h"
#include "aicore/datamodel.h"

int main(int argc, char **argv)
{

    using namespace AICore;
    using namespace Core;
    using namespace std;

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
            "iris.cvs"));

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

    size_t fileSize;
    ifstream modelifs(iris_model.filename().c_str(), ios::in | ios::binary | ios::ate);
    fileSize = modelifs.tellg();
    modelifs.seekg(0, ios::beg);
    vector<char> bytes(fileSize);
    modelifs.read(&bytes[0], fileSize);

    DataModel::shared_ptr mode = std::make_shared<DataModel>(string(&bytes[0], fileSize));

/*	DataSet input;

	VectorOfFloats f;
	ArrayOfColours c;

	auto irange = boost::irange(0,5);
	f.resize( irange.size() );
	c.resize( irange.size() );
	for( auto i : irange )
	{
		f[i] = i;
		c[i] = Colour_fromInt(i);
	}
	input.setA( f );
	input.setB( c );

	ResultData< NominalData<ArrayOfColours>,Core::VectorProcessingBackend<Core::VectorALU::BASIC_CPP,ArrayOfColours>  > results( DefaultVectorALU<ArrayOfColours>() );	
	results.allocateSpaceFor( 10 );
	for(auto i = 0; i < input.size(); ++i ) {
		results.generateIdealResult(i);
	}
	*/


	return 0;	

}