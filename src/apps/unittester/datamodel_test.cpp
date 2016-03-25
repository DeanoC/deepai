#include "core/core.h"
#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/range/irange.hpp>

#include <iostream>
#include "aicore/aicore.h"
#include "rapidjson/document.h"
#include "aicore/basetypes.h"
#include "aicore/datamodel.h"

namespace utf = boost::unit_test;
namespace bdata = utf::data;

BOOST_AUTO_TEST_CASE(test_datamodel_json_basic) {
    static const std::string s = R"(
		{
			"datamodel": 0
		}
	)";

    rapidjson::Document doc;
    // does it parse okay
    BOOST_TEST(doc.Parse(s.c_str()).HasParseError() == false);
    // is first object an object
    BOOST_TEST(doc.IsObject());
    BOOST_TEST(doc.HasMember("datamodel"));
    BOOST_TEST(doc["datamodel"].IsInt());
    BOOST_TEST(doc["datamodel"].GetInt() == 0);

    static const std::string s1 = R"(
		[
			"datamodel", "foo"
		]
	)";
    rapidjson::Document doc1;
    BOOST_TEST(doc1.Parse(s1.c_str()).HasParseError() == false);
    BOOST_TEST(doc1.IsArray());
    BOOST_TEST(doc1[0].IsInt() == false);
    BOOST_TEST(doc1[0].GetString() == "datamodel");
    BOOST_TEST(doc1[1].GetString() == "foo");

}

static const std::string s = R"(
{
	"datamodel": 0,
	"datalayout": [
		{	"represents": "Ratio",
			"type": "Input",
			"index": 0,
			"range": [ 0, 10 ]
		},
		{	"represents": "Ratio",
			"type": "Input",
			"index": 1,
			"range": [ 0, 10 ]
		},
		{	"represents": "Ratio",
			"type": "Input",
			"index": 2,
			"range": [ 0, 10 ]
		},
		{	"represents": "Ratio",
			"type": "Input",
			"index": 3,
			"range": [ 0, 10 ]
		},
		{	"represents": "Nominal",
			"type": "Output",
			"index": 4,
			"categories": ["Iris-setosa", "Iris-versicolor","Iris-virginica"],
			"range": [ 0, 10 ]
		}
	]

}
)";


BOOST_AUTO_TEST_CASE(test_datamodel_json_parse) {

    rapidjson::Document doc;
    // does it parse okay
    BOOST_TEST(!doc.Parse(s.c_str()).HasParseError());
    // is first object an object
    BOOST_TEST(doc.IsObject());
    BOOST_TEST((doc.FindMember("datamodel") != doc.MemberEnd()));
    BOOST_TEST(doc["datamodel"].IsInt());
    BOOST_TEST(doc["datamodel"].GetInt() == 0);
    BOOST_TEST(doc["datalayout"].IsArray());
    BOOST_TEST(doc["datalayout"][0].IsObject());
    BOOST_TEST(doc["datalayout"][1].IsObject());
    BOOST_TEST(doc["datalayout"][0]["represents"].IsString());
    BOOST_TEST(doc["datalayout"][1]["type"].IsString());
    BOOST_TEST(doc["datalayout"][2]["index"].IsNumber());
    BOOST_TEST(doc["datalayout"][3]["range"].IsArray());

}

BOOST_AUTO_TEST_CASE(test_datamodel_json_datamodel) {
    BOOST_CHECK_NO_THROW(AICore::DataModel model(s));
}

