cmake_minimum_required(VERSION 3.5)	


set( CMAKE_CXX_STANDARD 14 )
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project( DeepAI )

# check 64 bit
if( CMAKE_SIZEOF_VOID_P MATCHES 4 )
	set( IS_HOST_64_BIT 0 )
else()
	set( IS_HOST_64_BIT 1 )
endif()

find_package( Boost REQUIRED COMPONENTS thread chrono filesystem program_options system )

# if we have boost setup the include and libs
if( Boost_FOUND )
	include_directories( ${Boost_INCLUDE_DIRS} )
	link_directories( ${Boost_LIBRARY_DIRS} )
endif()

set( Core_LIBRARIES core ${Boost_LIBRARIES} )
add_definitions(${Boost_LIB_DIAGNOSTIC_DEFINITIONS})

# tell it to recurse into the source dir
add_subdirectory( src )