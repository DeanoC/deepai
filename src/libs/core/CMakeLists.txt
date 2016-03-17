cmake_minimum_required(VERSION 3.5)
set( LIB_NAME core )


# configure file replace cmake macros (i.e @VERSION_MAJOR@) with there value
# from the first file to the second, this makes our build config header
configure_file ( 
	"buildconfig.h.in"
	"buildconfig.h"
)

file( GLOB cpp_src_files *.cpp *.cxx)
file( GLOB c_src_files *.c )
file( GLOB header_files *.h *.inl *.hpp *.hxx)

list( APPEND header_files ${header_files} "${CMAKE_CURRENT_BINARY_DIR}/buildconfig.h" )
list( APPEND src_files ${cpp_src_files} ${c_src_files} )

add_library( ${LIB_NAME} ${src_files} ${header_files} )
target_link_libraries( ${LIB_NAME} ${targ_link_libs} )

# make a nice hierachy
set_target_properties( ${LIB_NAME} PROPERTIES FOLDER "Libs")