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

#get json
include( ${WORKSPACE_ROOT_PATH}/thirdparty/nlohmann/json_include.cmake )

#get inja
include( ${WORKSPACE_ROOT_PATH}/thirdparty/inja/inja_include.cmake )

#get common
include( ${WORKSPACE_ROOT_PATH}/src/common/common_include.cmake )

include_directories( ${DB_API_DIR} )
include_directories( ${DB_SRC_DIR} )

set( MEGA_DATABASE_TEMPLATES
    ${DB_SRC_DIR}/compiler/templates/data.cxx.jinja
    ${DB_SRC_DIR}/compiler/templates/data.hxx.jinja
    ${DB_SRC_DIR}/compiler/templates/environment.cxx.jinja
    ${DB_SRC_DIR}/compiler/templates/environment.hxx.jinja
    ${DB_SRC_DIR}/compiler/templates/file_info.cxx.jinja
    ${DB_SRC_DIR}/compiler/templates/file_info.hxx.jinja
    ${DB_SRC_DIR}/compiler/templates/manifest.cxx.jinja
    ${DB_SRC_DIR}/compiler/templates/manifest.hxx.jinja
    ${DB_SRC_DIR}/compiler/templates/view.cxx.jinja
    ${DB_SRC_DIR}/compiler/templates/view.hxx.jinja
)

set( MEGA_DATABASE_COMPILER
    ${DB_SRC_DIR}/compiler/native_types.hpp
    ${DB_SRC_DIR}/compiler/generator.hpp
    ${DB_SRC_DIR}/compiler/generator.cpp
    ${DB_SRC_DIR}/compiler/grammar.hpp
    ${DB_SRC_DIR}/compiler/grammar.cpp
    ${DB_SRC_DIR}/compiler/json_converter.hpp
    ${DB_SRC_DIR}/compiler/json_converter.cpp
    ${DB_SRC_DIR}/compiler/model.hpp
    ${DB_SRC_DIR}/compiler/model.cpp
	)
	
set( MEGA_DATABASE_LIB_HEADERS
    ${DB_API_DIR}/lib/api/api.hpp
    ${DB_API_DIR}/lib/api/archive.hpp
    ${DB_API_DIR}/lib/api/component_info.hpp
    ${DB_API_DIR}/lib/api/component_type.hpp
    ${DB_API_DIR}/lib/api/data_pointer.hpp
    ${DB_API_DIR}/lib/api/data_variant.hpp
    ${DB_API_DIR}/lib/api/directories.hpp
    ${DB_API_DIR}/lib/api/exception.hpp
    ${DB_API_DIR}/lib/api/file_header.hpp
    ${DB_API_DIR}/lib/api/file_store.hpp
    ${DB_API_DIR}/lib/api/file_system.hpp
    ${DB_API_DIR}/lib/api/file.hpp
    ${DB_API_DIR}/lib/api/generics.hpp
    ${DB_API_DIR}/lib/api/loader.hpp
    ${DB_API_DIR}/lib/api/manifest_data.hpp
    ${DB_API_DIR}/lib/api/object_info.hpp
    ${DB_API_DIR}/lib/api/object_loader.hpp
    ${DB_API_DIR}/lib/api/object.hpp
    ${DB_API_DIR}/lib/api/serialisation.hpp
    ${DB_API_DIR}/lib/api/sources.hpp
    ${DB_API_DIR}/lib/api/storer.hpp
)

set( MEGA_DATABASE_LIB_SRC
    ${DB_API_DIR}/lib/src/archive.cpp
    ${DB_API_DIR}/lib/src/component_type.cpp
    ${DB_API_DIR}/lib/src/file.cpp
    ${DB_API_DIR}/lib/src/loader.cpp
    ${DB_API_DIR}/lib/src/object.cpp
    ${DB_API_DIR}/lib/src/storer.cpp
)

add_library( database_lib INTERFACE ${MEGA_DATABASE_LIB_HEADERS} ${MEGA_DATABASE_LIB_SRC} )

add_library( database_compiler ${MEGA_DATABASE_COMPILER} )

add_dependencies( database_compiler database_lib )

set_target_properties( database_compiler PROPERTIES FOLDER database )

IF( ${WIN32} )

set_property(TARGET database_compiler PROPERTY WINDOWS_EXPORT_ALL_SYMBOLS ON )

ELSE( ${WIN32} )

ENDIF( ${WIN32} )

link_boost( database_compiler filesystem )
link_boost( database_compiler iostreams )
link_boost( database_compiler serialization )
link_boost( database_compiler system )
link_boost( database_compiler atomic )

link_json( database_compiler )
link_inja( database_compiler )

link_common( database_compiler ) 

install( FILES ${MEGA_DATABASE_TEMPLATES} DESTINATION templates )
install( FILES ${MEGA_DATABASE_LIB_HEADERS} DESTINATION lib/api )
install( FILES ${MEGA_DATABASE_LIB_SRC} DESTINATION lib/src )
install( TARGETS database_compiler DESTINATION bin)
