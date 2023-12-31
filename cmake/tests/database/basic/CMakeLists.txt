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

cmake_minimum_required( VERSION 3.1...3.16 )

#get boost
include( ${WORKSPACE_ROOT_PATH}/thirdparty/boost/boost_include.cmake )

#get gtest
include( ${WORKSPACE_ROOT_PATH}/thirdparty/gtest/gtest_include.cmake )

#get json
include( ${WORKSPACE_ROOT_PATH}/thirdparty/nlohmann/json_include.cmake )

#get common
include( ${WORKSPACE_ROOT_PATH}/src/common/common_include.cmake )

#get the database macro
include( ${WORKSPACE_ROOT_PATH}/src/database/db_include.cmake )

set( BASIC_DIR ${DB_TEST_DIR}/basic )

MegaDatabase( 
    DBNAME      database_basic_tests
    DBCOMPILER  database_compiler_driver
    LIB         ${DB_API_DIR}/lib 
    TEMPLATE    ${DB_SRC_DIR}/compiler/templates
    DATA        ${BASIC_DIR}/data/
    API         ${BASIC_DIR}
    SRC         ${BASIC_DIR}
    SCHEMAS     ${BASIC_DIR}/schema.dbs
    STAGES      BasicStage SecondStage ThirdStage FourthStage
)

set( DB_BASIC_UNIT_TESTS
	${BASIC_DIR}/basic_tests.cpp 
    )

enable_testing()

Message( STATUS "Generated ${DATABASE_GENERATED_STAGE_FILES} " )

add_executable( database_basic_tests 

                ${DATABASE_COPIED_FILES} 
                ${DATABASE_GENERATED_FILES} 
                ${DATABASE_GENERATED_STAGE_FILES} 

                ${DB_BASIC_UNIT_TESTS} 
                ${BASIC_DIR}/main.cpp 
                )

add_dependencies( database_basic_tests database_compiler_driver )

add_definitions(-DEG_DATABASE_API_SHARED_MODULE)

# allow access to source folder for tests - NOTE order sensitive here
target_include_directories( database_basic_tests PUBLIC ${BASIC_DIR} )
target_include_directories( database_basic_tests PUBLIC ${DB_API_DIR} )

set_target_properties( database_basic_tests PROPERTIES FOLDER tests/database )

link_boost( database_basic_tests filesystem )
link_boost( database_basic_tests system )
link_boost( database_basic_tests date_time )
link_boost( database_basic_tests program_options )
link_boost( database_basic_tests iostreams )
link_boost( database_basic_tests atomic )
link_boost( database_basic_tests serialization )
link_json( database_basic_tests )
link_gtest( database_basic_tests )
link_common( database_basic_tests )

gtest_add_tests( TARGET database_basic_tests SOURCES ${DB_BASIC_UNIT_TESTS}  )

install( TARGETS database_basic_tests DESTINATION bin )

add_custom_command( TARGET database_basic_tests POST_BUILD COMMAND ctest --output-on-failure -C $<CONFIG> )