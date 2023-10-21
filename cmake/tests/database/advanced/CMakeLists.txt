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

set( ADVANCED_DIR ${DB_TEST_DIR}/advanced )

MegaDatabase( 
    DBCOMPILER  database_compiler_driver
    LIB         ${DB_API_DIR}/lib 
    TEMPLATE    ${DB_SRC_DIR}/compiler/templates
    DATA        ${ADVANCED_DIR}/data/
    API         ${ADVANCED_DIR}
    SRC         ${ADVANCED_DIR}
    SCHEMAS     ${ADVANCED_DIR}/schema.dbs
    STAGES      FirstStage SecondStage ThirdStage FourthStage
)

set( DB_ADVANCED_UNIT_TESTS
	${ADVANCED_DIR}/advanced_tests.cpp 
    )

enable_testing()

Message( STATUS "Generated ${DATABASE_GENERATED_STAGE_FILES} " )

add_executable( db_advanced_test 

                ${DATABASE_COPIED_FILES} 
                ${DATABASE_GENERATED_FILES} 
                ${DATABASE_GENERATED_STAGE_FILES} 

                ${DB_ADVANCED_UNIT_TESTS} 
                ${ADVANCED_DIR}/main.cpp 
                )

add_dependencies( db_advanced_test database_compiler_driver )

add_definitions(-DEG_DATABASE_API_SHARED_MODULE)

IF( ${WIN32} )
target_compile_options(db_advanced_test PRIVATE /bigobj)

# for windows need to work around limit of 65K symbols - /Ob2 appears to achieve this
target_compile_options(db_advanced_test PRIVATE /Ob2)

# symbols are manually exported so ensure this is OFF
set_property(TARGET db_advanced_test PROPERTY WINDOWS_EXPORT_ALL_SYMBOLS OFF )

# set_source_files_properties( 
#     ${MEGA_DATABASE_SOURCES} ${MEGA_DATABASE_GENERATED_SOURCES} ${MEGA_DATABASE_STAGE_SOURCES}
#     PROPERTIES
#     COMPILE_FLAGS -O1 
# )
ELSE( ${WIN32} )

set_property(TARGET db_advanced_test PROPERTY C_VISIBILITY_PRESET hidden)
set_property(TARGET db_advanced_test PROPERTY VISIBILITY_INLINES_HIDDEN ON)

ENDIF( ${WIN32} )

# allow access to source folder for tests - NOTE order sensitive here
target_include_directories( db_advanced_test PUBLIC ${ADVANCED_DIR} )
target_include_directories( db_advanced_test PUBLIC ${DB_API_DIR} )

set_target_properties( db_advanced_test PROPERTIES FOLDER tests/database )

link_boost( db_advanced_test filesystem )
link_boost( db_advanced_test system )
link_boost( db_advanced_test date_time )
link_boost( db_advanced_test program_options )
link_boost( db_advanced_test iostreams )
link_boost( db_advanced_test atomic )
link_boost( db_advanced_test serialization )
link_json( db_advanced_test )
link_gtest( db_advanced_test )
link_common( db_advanced_test )

gtest_add_tests( TARGET db_advanced_test SOURCES ${DB_ADVANCED_UNIT_TESTS} )

install( TARGETS db_advanced_test DESTINATION bin )

add_custom_command( TARGET db_advanced_test POST_BUILD COMMAND ctest --output-on-failure -C $<CONFIG> )