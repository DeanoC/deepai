#pragma once

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

	template<typename T, typename ALU>
	struct DataTypeBaseImpl {
		DataTypeBaseImpl( ALU& _alu ) : processor(_alu) {}

		typedef T container_type;
		typedef ALU alu_type;
		typedef typename container_type::value_type value_type;		

		container_type data;
		alu_type& processor;
	};

	template< typename T, typename ALU> struct DataTypeBase;
}
#include "aicore/nominaldata.h"
#include "aicore/ordinaldata.h"
#include "aicore/intervaldata.h"
#include "aicore/ratiodata.h"