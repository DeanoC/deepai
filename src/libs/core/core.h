#pragma once

#if !defined( __STDC_LIMIT_MACROS )
#	define __STDC_LIMIT_MACROS
#endif
#if !defined( __STDC_CONSTANT_MACROS )
#	define __STDC_CONSTANT_MACROS
#endif
// for vc10/11
#define _VARIADIC_MAX 10

#include "core/buildconfig.h"	

#include <cstdint>
#include <vector>
#include <array>
#include <limits>

#include "core/vectoralu.h"
#include "core/vectoralu_basicc.h"

namespace Core {
}