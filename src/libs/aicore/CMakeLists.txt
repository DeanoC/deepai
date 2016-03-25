cmake_minimum_required(VERSION 3.5)
set( LIB_NAME aicore )


file( GLOB cpp_src_files *.cpp *.cxx)
file( GLOB c_src_files *.c )
file( GLOB header_files *.h *.inl *.hpp *.hxx)

list( APPEND src_files ${cpp_src_files} ${c_src_files} )

add_library(${LIB_NAME} ${src_files} ${header_files} datamodel.cpp)
target_link_libraries( ${LIB_NAME} ${targ_link_libs} )

# make a nice hierachy
set_target_properties( ${LIB_NAME} PROPERTIES FOLDER "Libs")