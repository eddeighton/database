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

#ifndef LOADER_18_04_2019
#define LOADER_18_04_2019


#include "database/serialisation.hpp"
#include "database/object_info.hpp"
#include "database/object.hpp"
#include "database/file_system.hpp"

#include "database/sources.hpp"

#include "common/assert_verify.hpp"

#include <boost/filesystem/path.hpp>

#include <istream>
#include <memory>
#include <optional>
#include <set>
#include <map>
#include <vector>

namespace data
{
class ObjectPartLoader;
}
namespace mega::io
{
class Manifest;

class Loader
{
public:
    Loader( const FileSystem& fileSystem, const CompilationFilePath& filePath, ::data::ObjectPartLoader& loader );

    template < typename T >
    void load( T& value )
    {
        m_archive& value;
    }

    void postLoad( const Manifest& runtimeManifest );

private:
    std::set< mega::io::ObjectInfo* > m_objectInfos;
    std::unique_ptr< std::istream >   m_pFileStream;
    boost::archive::MegaIArchive      m_archive;
};

} // namespace mega::io

#endif // LOADER_18_04_2019
