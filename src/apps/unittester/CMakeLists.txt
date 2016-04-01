cmake_minimum_required(VERSION 3.5)
set( APP_NAME unittester )

file( GLOB cpp_src_files *.cpp *.cxx)
file( GLOB c_src_files *.c )
file( GLOB header_files *.h *.inl *.hpp *.hxx )
list( APPEND src_files ${cpp_src_files} ${c_src_files} )

list(APPEND targ_link_libs ${Core_LIBRARIES} aicore aialgorithms)
	
add_executable(  ${APP_NAME} ${src_files} ${header_files} )
target_link_libraries(  ${APP_NAME} ${targ_link_libs} )
set_target_properties(  ${APP_NAME} PROPERTIES FOLDER "Apps")
