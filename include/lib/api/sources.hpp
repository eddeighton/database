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

#ifndef SOURCES_15_APRIL_2022
#define SOURCES_15_APRIL_2022

#include "database/serialisation.hpp"

#include <boost/filesystem/path.hpp>

namespace mega::io
{
class Environment;
class Manifest;

class FilePath
{
protected:
    FilePath( const boost::filesystem::path& filePath )
        : m_filePath( filePath )
    {
    }

public:
    FilePath() = default;
    const boost::filesystem::path& path() const { return m_filePath; }

    bool operator==( const FilePath& cmp ) const { return m_filePath == cmp.m_filePath; }
    bool operator<( const FilePath& cmp ) const { return m_filePath < cmp.m_filePath; }

    template < class Archive >
    inline void serialize( Archive& archive, const unsigned int )
    {
        archive& boost::serialization::make_nvp( "file_path", m_filePath );
    }

protected:
    boost::filesystem::path m_filePath;
};

class SourceFilePath : public FilePath
{
    friend class BuildEnvironment;
    friend class Manifest;

protected:
    SourceFilePath( const boost::filesystem::path& filePath )
        : FilePath( filePath )
    {
    }

public:
    SourceFilePath() = default;
    bool operator==( const SourceFilePath& cmp ) const { return m_filePath == cmp.m_filePath; }
    bool operator<( const SourceFilePath& cmp ) const { return m_filePath < cmp.m_filePath; }

    template < class Archive >
    inline void serialize( Archive& archive, const unsigned int )
    {
        archive& boost::serialization::make_nvp( "source_file_path", m_filePath );
    }
};

class manifestFilePath : public SourceFilePath
{
    friend class Environment;
    friend class BuildEnvironment;
    friend class Manifest;
    manifestFilePath( const boost::filesystem::path& filePath )
        : SourceFilePath( filePath )
    {
    }

public:
    manifestFilePath() = default;
    bool operator==( const manifestFilePath& cmp ) const { return m_filePath == cmp.m_filePath; }
    bool operator<( const manifestFilePath& cmp ) const { return m_filePath < cmp.m_filePath; }

    template < class Archive >
    inline void serialize( Archive& archive, const unsigned int )
    {
        archive& boost::serialization::make_nvp( "manifest_file_path", m_filePath );
    }

    static const boost::filesystem::path extension() { return ".manifest"; }
};

class megaFilePath : public SourceFilePath
{
    friend class BuildEnvironment;
    friend class Manifest;
    megaFilePath( const boost::filesystem::path& filePath )
        : SourceFilePath( filePath )
    {
    }

public:
    megaFilePath() = default;

    bool operator==( const megaFilePath& cmp ) const { return m_filePath == cmp.m_filePath; }
    bool operator<( const megaFilePath& cmp ) const { return m_filePath < cmp.m_filePath; }

    template < class Archive >
    inline void serialize( Archive& archive, const unsigned int )
    {
        archive& boost::serialization::make_nvp( "mega_file_path", m_filePath );
    }
    static const boost::filesystem::path extension() { return ".mega"; }
};

class cppFilePath : public SourceFilePath
{
    friend class BuildEnvironment;
    friend class Manifest;

    cppFilePath( const boost::filesystem::path& filePath )
        : SourceFilePath( filePath )
    {
    }

public:
    cppFilePath() = default;

    bool operator==( const cppFilePath& cmp ) const { return m_filePath == cmp.m_filePath; }
    bool operator<( const cppFilePath& cmp ) const { return m_filePath < cmp.m_filePath; }

    template < class Archive >
    inline void serialize( Archive& archive, const unsigned int )
    {
        archive& boost::serialization::make_nvp( "cpp_file_path", m_filePath );
    }
    static const boost::filesystem::path extension() { return ".cpp"; }
};

class pyFilePath : public SourceFilePath
{
    friend class BuildEnvironment;
    friend class Manifest;

    pyFilePath( const boost::filesystem::path& filePath )
        : SourceFilePath( filePath )
    {
    }

public:
    pyFilePath() = default;

    bool operator==( const pyFilePath& cmp ) const { return m_filePath == cmp.m_filePath; }
    bool operator<( const pyFilePath& cmp ) const { return m_filePath < cmp.m_filePath; }

    template < class Archive >
    inline void serialize( Archive& archive, const unsigned int )
    {
        archive& boost::serialization::make_nvp( "py_file_path", m_filePath );
    }
    static const boost::filesystem::path extension() { return ".py"; }
};

class schFilePath : public SourceFilePath
{
    friend class BuildEnvironment;
    friend class Manifest;

    schFilePath( const boost::filesystem::path& filePath )
        : SourceFilePath( filePath )
    {
    }

public:
    schFilePath() = default;

    bool operator==( const schFilePath& cmp ) const { return m_filePath == cmp.m_filePath; }
    bool operator<( const schFilePath& cmp ) const { return m_filePath < cmp.m_filePath; }

    template < class Archive >
    inline void serialize( Archive& archive, const unsigned int )
    {
        archive& boost::serialization::make_nvp( "schematic_file_path", m_filePath );
    }
    static const boost::filesystem::path extension() { return ".sch"; }
};

class BuildFilePath : public FilePath
{
    friend class Environment;
    friend class Manifest;

protected:
    BuildFilePath( const boost::filesystem::path& filePath )
        : FilePath( filePath )
    {
    }

public:
    BuildFilePath() = default;
    bool operator==( const BuildFilePath& cmp ) const { return m_filePath == cmp.m_filePath; }
    bool operator<( const BuildFilePath& cmp ) const { return m_filePath < cmp.m_filePath; }

    template < class Archive >
    inline void serialize( Archive& archive, const unsigned int )
    {
        archive& boost::serialization::make_nvp( "build_file_path", m_filePath );
    }
};

class ComponentListingFilePath : public BuildFilePath
{
    friend class BuildEnvironment;
    friend class Manifest;

    ComponentListingFilePath( const boost::filesystem::path& filePath )
        : BuildFilePath( filePath )
    {
    }

public:
    ComponentListingFilePath() = default;

    bool operator==( const ComponentListingFilePath& cmp ) const { return m_filePath == cmp.m_filePath; }
    bool operator<( const ComponentListingFilePath& cmp ) const { return m_filePath < cmp.m_filePath; }

    template < class Archive >
    inline void serialize( Archive& archive, const unsigned int )
    {
        archive& boost::serialization::make_nvp( "component_listing_file_path", m_filePath );
    }
};

class CompilationFilePath : public BuildFilePath
{
    friend class Environment;
    friend class Manifest;

    CompilationFilePath( const boost::filesystem::path& filePath )
        : BuildFilePath( filePath )
    {
    }

public:
    CompilationFilePath() = default;

    bool operator==( const CompilationFilePath& cmp ) const { return m_filePath == cmp.m_filePath; }
    bool operator<( const CompilationFilePath& cmp ) const { return m_filePath < cmp.m_filePath; }

    template < class Archive >
    inline void serialize( Archive& archive, const unsigned int )
    {
        archive& boost::serialization::make_nvp( "compilation_file_path", m_filePath );
    }
    static const boost::filesystem::path extension() { return ".db"; }
};

class GeneratedHPPSourceFilePath : public BuildFilePath
{
    friend class BuildEnvironment;
    friend class Manifest;

    GeneratedHPPSourceFilePath( const boost::filesystem::path& filePath )
        : BuildFilePath( filePath )
    {
    }

public:
    GeneratedHPPSourceFilePath() = default;

    bool operator==( const GeneratedHPPSourceFilePath& cmp ) const { return m_filePath == cmp.m_filePath; }
    bool operator<( const GeneratedHPPSourceFilePath& cmp ) const { return m_filePath < cmp.m_filePath; }

    template < class Archive >
    inline void serialize( Archive& archive, const unsigned int )
    {
        archive& boost::serialization::make_nvp( "generated_hxx_file_path", m_filePath );
    }
    static const boost::filesystem::path extension() { return ".hpp"; }
};

class GeneratedCPPSourceFilePath : public BuildFilePath
{
    friend class BuildEnvironment;
    friend class Manifest;

    GeneratedCPPSourceFilePath( const boost::filesystem::path& filePath )
        : BuildFilePath( filePath )
    {
    }

public:
    GeneratedCPPSourceFilePath() = default;

    bool operator==( const GeneratedCPPSourceFilePath& cmp ) const { return m_filePath == cmp.m_filePath; }
    bool operator<( const GeneratedCPPSourceFilePath& cmp ) const { return m_filePath < cmp.m_filePath; }

    template < class Archive >
    inline void serialize( Archive& archive, const unsigned int )
    {
        archive& boost::serialization::make_nvp( "generated_cxx_file_path", m_filePath );
    }
    static const boost::filesystem::path extension() { return ".cpp"; }
};

class PrecompiledHeaderFile : public BuildFilePath
{
    friend class BuildEnvironment;
    friend class Manifest;

    PrecompiledHeaderFile( const boost::filesystem::path& filePath )
        : BuildFilePath( filePath )
    {
    }

public:
    PrecompiledHeaderFile() = default;

    bool operator==( const PrecompiledHeaderFile& cmp ) const { return m_filePath == cmp.m_filePath; }
    bool operator<( const PrecompiledHeaderFile& cmp ) const { return m_filePath < cmp.m_filePath; }

    template < class Archive >
    inline void serialize( Archive& archive, const unsigned int )
    {
        archive& boost::serialization::make_nvp( "precompiled_header_file", m_filePath );
    }
    static const boost::filesystem::path extension() { return ".pch"; }
};

class ObjectFilePath : public BuildFilePath
{
    friend class BuildEnvironment;
    friend class Manifest;

    ObjectFilePath( const boost::filesystem::path& filePath )
        : BuildFilePath( filePath )
    {
    }

public:
    ObjectFilePath() = default;

    bool operator==( const ObjectFilePath& cmp ) const { return m_filePath == cmp.m_filePath; }
    bool operator<( const ObjectFilePath& cmp ) const { return m_filePath < cmp.m_filePath; }

    template < class Archive >
    inline void serialize( Archive& archive, const unsigned int )
    {
        archive& boost::serialization::make_nvp( "cpp_object_file", m_filePath );
    }
    static const boost::filesystem::path extension() { return ".obj"; }
};

class ComponentFilePath : public BuildFilePath
{
    friend class BuildEnvironment;
    friend class Manifest;

    ComponentFilePath( const boost::filesystem::path& filePath )
        : BuildFilePath( filePath )
    {
    }

public:
    ComponentFilePath() = default;

    bool operator==( const ComponentFilePath& cmp ) const { return m_filePath == cmp.m_filePath; }
    bool operator<( const ComponentFilePath& cmp ) const { return m_filePath < cmp.m_filePath; }

    template < class Archive >
    inline void serialize( Archive& archive, const unsigned int )
    {
        archive& boost::serialization::make_nvp( "component_file", m_filePath );
    }
};

class MapFilePath : public BuildFilePath
{
    friend class BuildEnvironment;
    friend class Manifest;

    MapFilePath( const boost::filesystem::path& filePath )
        : BuildFilePath( filePath )
    {
    }

public:
    MapFilePath() = default;

    bool operator==( const MapFilePath& cmp ) const { return m_filePath == cmp.m_filePath; }
    bool operator<( const MapFilePath& cmp ) const { return m_filePath < cmp.m_filePath; }

    template < class Archive >
    inline void serialize( Archive& archive, const unsigned int )
    {
        archive& boost::serialization::make_nvp( "map_file", m_filePath );
    }
    static const boost::filesystem::path extension() { return ".map"; }
};

} // namespace mega::io

#endif // SOURCES_15_APRIL_2022
