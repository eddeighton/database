//  Copyright (c) Deighton Systems Limited. 2022. All Rights Reserved.
//  Author: Edward Deighton
//  License: Please see license.txt in the project root folder.

//  Use and copying of this software and preparation of derivative works
//  based upon this software are permitted. Any copy of this software or
//  of any derivative work must include the above copyright notice, this
//  paragraph and the one after it.  Any distribution of this software or
//  derivative works must comply with all applicable laws.

//  This software is made available AS IS, and COPYRIGHT OWNERS DISCLAIMS
//  ALL WARRANTIES, EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION THE
//  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
//  PURPOSE, AND NOTWITHSTANDING ANY OTHER PROVISION CONTAINED HEREIN, ANY
//  LIABILITY FOR DAMAGES RESULTING FROM THE SOFTWARE OR ITS USE IS
//  EXPRESSLY DISCLAIMED, WHETHER ARISING IN CONTRACT, TORT (INCLUDING
//  NEGLIGENCE) OR STRICT LIABILITY, EVEN IF COPYRIGHT OWNERS ARE ADVISED
//  OF THE POSSIBILITY OF SUCH DAMAGES.

#include "database/environment.hxx"
#include "database/directories.hpp"
#include "database/component_info.hpp"
#include "database/sources.hpp"
#include "database/BasicStage.hxx"
#include "database/SecondStage.hxx"
#include "database/ThirdStage.hxx"
#include "database/FourthStage.hxx"
#include "database/manifest.hxx"
#include "database/environment.hxx"
#include "database/serialisation.hpp"

#include "../environment_test.hpp"

#include "common/file.hpp"
#include "common/string.hpp"

#include <gtest/gtest.h>

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

#include <fstream>
#include <memory>
#include <optional>

class BasicDBTest : public ::testing::Test
{
public:
    using Env = mega::io::TestEnvironment< mega::io::Environment >;
    boost::filesystem::path                  m_tempDir;
    std::unique_ptr< mega::io::Directories > m_pDirectories;
    std::unique_ptr< Env >                   m_pEnvironment;

    virtual void SetUp() override
    {
        m_tempDir = boost::filesystem::temp_directory_path() / "BasicDBTest" / common::uuid();
        boost::filesystem::create_directories( m_tempDir );

        m_pDirectories
            = std::make_unique< mega::io::Directories >( mega::io::Directories{ m_tempDir, m_tempDir, "", "" } );
        m_pEnvironment = std::make_unique< Env >( *m_pDirectories );

        std::vector< mega::io::ComponentInfo > componentInfos;
        const mega::io::Manifest               manifest( *m_pEnvironment, m_tempDir, componentInfos );
        const mega::io::manifestFilePath       projectManifestPath = m_pEnvironment->project_manifest();
        manifest.save_temp( *m_pEnvironment, projectManifestPath );
        m_pEnvironment->temp_to_real( projectManifestPath );
    }
    virtual void TearDown() override
    {
        m_pEnvironment.reset();
        namespace bfs = boost::filesystem;
        bfs::remove_all( boost::filesystem::temp_directory_path() / "BasicDBTest" );
    }
};

TEST_F( BasicDBTest, BasicTypes )
{
    {
        using namespace BasicStage;
        using namespace BasicStage::TestNamespace;

        Database database( *m_pEnvironment, m_pEnvironment->project_manifest() );

        TestObject* pTestObject
            = database.construct< TestObject >( TestObject::Args( "test", {}, std::nullopt, {}, std::nullopt ) );

        ASSERT_TRUE( pTestObject );
        ASSERT_EQ( pTestObject->get_string(), "test" );
        ASSERT_TRUE( !pTestObject->get_optional_string().has_value() );

        pTestObject->set_optional_reference( pTestObject );
        pTestObject->set_late_reference( pTestObject );
        pTestObject->push_back_late_array_of_references( pTestObject );
        pTestObject->push_back_late_array_of_references( pTestObject );
        pTestObject->push_back_late_array_of_references( pTestObject );

        database.save_FirstFile_to_temp();
        m_pEnvironment->temp_to_real( m_pEnvironment->BasicStage_FirstFile( m_pEnvironment->project_manifest() ) );
    }
    {
        using namespace SecondStage;
        Database database( *m_pEnvironment, m_pEnvironment->project_manifest() );

        TestNamespace::TestObject* pTestObject
            = database.one< TestNamespace::TestObject >( m_pEnvironment->project_manifest() );
        ASSERT_TRUE( pTestObject );
        ASSERT_EQ( pTestObject->get_string(), "test" );
        ASSERT_TRUE( !pTestObject->get_optional_string().has_value() );
        ASSERT_EQ( pTestObject->get_optional_reference().value(), pTestObject );
        ASSERT_EQ( pTestObject->get_late_reference(), pTestObject );
        ASSERT_EQ( pTestObject->get_late_array_of_references().size(), 3 );
        ASSERT_EQ( pTestObject->get_late_array_of_references()[ 0 ], pTestObject );
        ASSERT_EQ( pTestObject->get_late_array_of_references()[ 1 ], pTestObject );
        ASSERT_EQ( pTestObject->get_late_array_of_references()[ 2 ], pTestObject );
    }
}

TEST_F( BasicDBTest, MultiStageTypes )
{
    {
        using namespace BasicStage;
        using namespace BasicStage::TestNamespace;

        Database database( *m_pEnvironment, m_pEnvironment->project_manifest() );

        OtherObject* pTestObject = database.construct< OtherObject >( OtherObject::Args( 1 ) );
        ASSERT_EQ( pTestObject->get_this_stage_value(), 1 );

        database.save_FirstFile_to_temp();
        m_pEnvironment->temp_to_real( m_pEnvironment->BasicStage_FirstFile( m_pEnvironment->project_manifest() ) );
    }
    {
        using namespace SecondStage;
        using namespace SecondStage::TestNamespace;

        Database database( *m_pEnvironment, m_pEnvironment->project_manifest() );

        OtherObject* pTestObject = database.one< OtherObject >( m_pEnvironment->project_manifest() );
        ASSERT_EQ( pTestObject->get_this_stage_value(), 1 );

        pTestObject = database.construct< OtherObject >( OtherObject::Args( pTestObject, 2, 3 ) );
        ASSERT_EQ( pTestObject->get_next_stage_value(), 2 );
        ASSERT_EQ( pTestObject->get_another_stage_value(), 3 );
        pTestObject->set_next_stage_value( 5 );
        ASSERT_EQ( pTestObject->get_next_stage_value(), 5 );

        TestInherit* pInherit = database.construct< TestInherit >( TestInherit::Args( pTestObject, 123 ) );
        ASSERT_EQ( pInherit->get_second_stage_inherited_value(), 123 );

        database.save_SecondFile_to_temp();
        m_pEnvironment->temp_to_real( m_pEnvironment->SecondStage_SecondFile( m_pEnvironment->project_manifest() ) );
        database.save_ThirdFile_to_temp();
        m_pEnvironment->temp_to_real( m_pEnvironment->SecondStage_ThirdFile( m_pEnvironment->project_manifest() ) );
    }
    {
        using namespace ThirdStage;
        using namespace ThirdStage::TestNamespace;

        Database database( *m_pEnvironment, m_pEnvironment->project_manifest() );

        OtherObject* pTestObject = database.one< OtherObject >( m_pEnvironment->project_manifest() );
        ASSERT_EQ( pTestObject->get_this_stage_value(), 1 );
        ASSERT_EQ( pTestObject->get_next_stage_value(), 5 );
        ASSERT_EQ( pTestObject->get_another_stage_value(), 3 );

        pTestObject = database.construct< OtherObject >( OtherObject::Args( pTestObject, 4 ) );
        ASSERT_EQ( pTestObject->get_third_stage_value(), 4 );

        pTestObject->set_third_stage_value( 6 );
        ASSERT_EQ( pTestObject->get_third_stage_value(), 6 );

        TestInherit* pInherited = database.one< TestInherit >( m_pEnvironment->project_manifest() );
        ASSERT_EQ( pInherited->get_second_stage_inherited_value(), 123 );

        pInherited->set_third_stage_value( 321 );
        ASSERT_EQ( pTestObject->get_third_stage_value(), 321 );
        ASSERT_EQ( pInherited->get_third_stage_value(), 321 );

        pInherited = database.construct< TestInherit >( TestInherit::Args( pInherited, 234 ) );
        ASSERT_EQ( pInherited->get_third_stage_inherited_value(), 234 );

        pInherited->set_third_stage_inherited_value( 235 );
        ASSERT_EQ( pInherited->get_third_stage_inherited_value(), 235 );

        ASSERT_EQ( pInherited->get_this_stage_value(), 1 );

        database.save_FourthFile_to_temp();
        m_pEnvironment->temp_to_real( m_pEnvironment->ThirdStage_FourthFile( m_pEnvironment->project_manifest() ) );
    }
    {
        using namespace FourthStage;
        using namespace FourthStage::TestNamespace;

        Database database( *m_pEnvironment, m_pEnvironment->project_manifest() );

        TestInherit* pInherited = database.one< TestInherit >( m_pEnvironment->project_manifest() );
        ASSERT_EQ( pInherited->get_this_stage_value(), 1 );
        ASSERT_EQ( pInherited->get_next_stage_value(), 5 );
        ASSERT_EQ( pInherited->get_another_stage_value(), 3 );
        ASSERT_EQ( pInherited->get_third_stage_value(), 321 );
        ASSERT_EQ( pInherited->get_third_stage_inherited_value(), 235 );
        ASSERT_EQ( pInherited->get_second_stage_inherited_value(), 123 );

        OtherObject* pReconstructedOtherObject
            = database.construct< OtherObject >( OtherObject::Args( pInherited, 10 ) );
        ASSERT_EQ( pReconstructedOtherObject->get_fifth_stage_value(), 10 );

        ASSERT_EQ( pInherited->get_fifth_stage_value(), 10 );
    }
}

TEST_F( BasicDBTest, DatabaseErrors )
{
    {
        using namespace BasicStage;
        using namespace BasicStage::TestNamespace;

        Database database( *m_pEnvironment, m_pEnvironment->project_manifest() );

        OtherObject* pTestObject = database.construct< OtherObject >( OtherObject::Args( 1 ) );
        ASSERT_EQ( pTestObject->get_this_stage_value(), 1 );
        pTestObject->set_this_stage_value( 2 );
        ASSERT_EQ( pTestObject->get_this_stage_value(), 2 );
        database.save_FirstFile_to_temp();
        m_pEnvironment->temp_to_real( m_pEnvironment->BasicStage_FirstFile( m_pEnvironment->project_manifest() ) );
    }
    {
        using namespace SecondStage;
        using namespace SecondStage::TestNamespace;

        Database database( *m_pEnvironment, m_pEnvironment->project_manifest() );

        OtherObject* pTestObject = database.one< OtherObject >( m_pEnvironment->project_manifest() );
        ASSERT_EQ( pTestObject->get_this_stage_value(), 2 );

        ASSERT_THROW( pTestObject->get_next_stage_value(), std::runtime_error );
        ASSERT_THROW( pTestObject->set_next_stage_value( 1 ), std::runtime_error );

        pTestObject = database.construct< OtherObject >( OtherObject::Args( pTestObject, 2, 3 ) );
        ASSERT_THROW( database.construct< OtherObject >( OtherObject::Args( pTestObject, 2, 3 ) ), std::runtime_error );

        TestInherit* pInherit = database.construct< TestInherit >( TestInherit::Args( pTestObject, 123 ) );
        ASSERT_THROW( database.construct< TestInherit >( TestInherit::Args( pTestObject, 123 ) ), std::runtime_error );

        ASSERT_EQ( pInherit, pTestObject );

        database.save_SecondFile_to_temp();
        m_pEnvironment->temp_to_real( m_pEnvironment->SecondStage_SecondFile( m_pEnvironment->project_manifest() ) );
        database.save_ThirdFile_to_temp();
        m_pEnvironment->temp_to_real( m_pEnvironment->SecondStage_ThirdFile( m_pEnvironment->project_manifest() ) );
    }
    {
        using namespace ThirdStage;
        using namespace ThirdStage::TestNamespace;

        Database database( *m_pEnvironment, m_pEnvironment->project_manifest() );

        OtherObject* pTestObject = database.one< OtherObject >( m_pEnvironment->project_manifest() );
        TestInherit* pInherited  = database.one< TestInherit >( m_pEnvironment->project_manifest() );
        ASSERT_EQ( pTestObject, pInherited );
        TestInherit* pOther = db_cast< TestInherit >( pTestObject );
        ASSERT_TRUE( pOther );
        ASSERT_TRUE( pInherited );
        ASSERT_EQ( pOther->_get_object_info().getFileID(), pInherited->_get_object_info().getFileID() );
        ASSERT_EQ( pOther->_get_object_info().getType(), pInherited->_get_object_info().getType() );
        ASSERT_EQ( pOther->_get_object_info().getIndex(), pInherited->_get_object_info().getIndex() );
        ASSERT_EQ( pOther, pInherited );

        ASSERT_THROW( pInherited->get_third_stage_inherited_value(), std::runtime_error );
        pInherited = database.construct< TestInherit >( TestInherit::Args( pInherited, 234 ) );
        ASSERT_THROW( database.construct< TestInherit >( TestInherit::Args( pInherited, 234 ) ), std::runtime_error );

        ASSERT_THROW( pTestObject->get_third_stage_value(), std::runtime_error );
        pTestObject = database.construct< OtherObject >( OtherObject::Args( pTestObject, 4 ) );
        ASSERT_THROW( database.construct< OtherObject >( OtherObject::Args( pTestObject, 4 ) ), std::runtime_error );
        ASSERT_EQ( pTestObject, pInherited );

        database.save_FourthFile_to_temp();
        m_pEnvironment->temp_to_real( m_pEnvironment->ThirdStage_FourthFile( m_pEnvironment->project_manifest() ) );
    }
}