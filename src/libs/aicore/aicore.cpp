#include "core/core.h"
#include <ostream>
#include "aicore/basetypes.h"
#include "aicore/aicore.h"

namespace AICore {

	std::ostream& operator<<( std::ostream& out, AICore::DataCategory type ) {
		switch( type ) {
			case AICore::DataCategory::Qualitive: out << "DataCategory::Qualitive "; break;
			case AICore::DataCategory::Quantative: out << "DataCategory::Quantative "; break;
			default: out << "DataCategory ERROR - UNDEFINED "; break;
		}
		return out;
	}
	std::ostream& operator<<( std::ostream& out, AICore::DataType type ) {
		switch( type ) {
			case AICore::DataType::Nominal: out << "DataType::Nominal "; break;
			case AICore::DataType::Ordinal: out << "DataType::Ordinal "; break;
			case AICore::DataType::Interval: out << "DataType::Interval "; break;
			case AICore::DataType::Ratio: out << "DataType::Ratio "; break;
			default: out << "DataType ERROR - UNDEFINED "; break;
		}
		return out;
	}



}