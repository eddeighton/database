##  Copyright (c) Deighton Systems Limited. 2022. All Rights Reserved.
##  Author: Edward Deighton
##  License: Please see license.txt in the project root folder.

##  Use and copying of this software and preparation of derivative works
##  based upon this software are permitted. Any copy of this software or
##  of any derivative work must include the above copyright notice, this
##  paragraph and the one after it.  Any distribution of this software or
##  derivative works must comply with all applicable laws.

##  This software is made available AS IS, and COPYRIGHT OWNERS DISCLAIMS
##  ALL WARRANTIES, EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION THE
##  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
##  PURPOSE, AND NOTWITHSTANDING ANY OTHER PROVISION CONTAINED HEREIN, ANY
##  LIABILITY FOR DAMAGES RESULTING FROM THE SOFTWARE OR ITS USE IS
##  EXPRESSLY DISCLAIMED, WHETHER ARISING IN CONTRACT, TORT (INCLUDING
##  NEGLIGENCE) OR STRICT LIABILITY, EVEN IF COPYRIGHT OWNERS ARE ADVISED
##  OF THE POSSIBILITY OF SUCH DAMAGES.



cmake_minimum_required(VERSION 3.2)

#get boost
include( ${WORKSPACE_ROOT_PATH}/thirdparty/boost/boost_include.cmake )

#get gtest
include( ${WORKSPACE_ROOT_PATH}/thirdparty/gtest/gtest_include.cmake )

#get common
include( ${WORKSPACE_ROOT_PATH}/src/common/common_include.cmake )

include_directories( ${DB_API_DIR} )
include_directories( ${DB_SRC_DIR} )

set( DB_UNIT_TESTS_DIR ${DB_TEST_DIR}/unit_tests )

set( DB_UNIT_TESTS
	${DB_UNIT_TESTS_DIR}/schema_grammar_tests.cpp
	${DB_UNIT_TESTS_DIR}/main_test_runner.cpp
	)

set( MEGA_UNIT_TESTS_DATA
)

enable_testing()

add_executable( db_tests ${DB_UNIT_TESTS} ${DB_UNIT_TESTS_DIR}/main_test_runner.cpp )

IF( ${WIN32} )
target_compile_options( db_tests PUBLIC "/await:strict" )
target_compile_options( db_tests PRIVATE "/bigobj" )
ELSE( ${WIN32} )
target_compile_options( db_tests PUBLIC "-fcoroutines" )
ENDIF( ${WIN32} )

target_compile_definitions( db_tests PUBLIC -DBOOST_PHOENIX_STL_TUPLE_H_ )

# allow access to source folder for tests
target_include_directories(db_tests PUBLIC ${MEGA_SRC_DIR})

set_target_properties( db_tests PROPERTIES FOLDER tests/unit )

link_boost( db_tests filesystem )
link_boost( db_tests system )
link_boost( db_tests date_time )
link_boost( db_tests program_options )
link_boost( db_tests iostreams )
link_boost( db_tests atomic )
link_boost( db_tests serialization )
link_gtest( db_tests )

target_link_libraries( db_tests database_compiler)

gtest_add_tests( TARGET db_tests SOURCES ${DB_UNIT_TESTS} )

install( TARGETS db_tests DESTINATION bin )
install( FILES ${MEGA_UNIT_TESTS_DATA} DESTINATION bin/test_data )	

add_custom_command( TARGET db_tests POST_BUILD COMMAND db_tests WORKING_DIRECTORY ${DB_UNIT_TESTS_DIR} )