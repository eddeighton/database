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

#ifndef FILE_HEADER_12_MAY_2022
#define FILE_HEADER_12_MAY_2022


#include <cstddef>

namespace mega::io
{

class FileHeader
{
public:
    FileHeader()
        : m_versionHashCode( 0U )
        , m_manifestHashCode( 0U )
    {
    }

    FileHeader( std::size_t versionHashCode, std::size_t manifestHashCode )
        : m_versionHashCode( versionHashCode )
        , m_manifestHashCode( manifestHashCode )
    {
    }

    std::size_t getVersion() const { return m_versionHashCode; }
    std::size_t getManifestHashCode() const { return m_manifestHashCode; }

    template < class Archive >
    inline void serialize( Archive& archive, const unsigned int )
    {
        archive& m_versionHashCode;
        archive& m_manifestHashCode;
    }

private:
    std::size_t m_versionHashCode;
    std::size_t m_manifestHashCode;
};

} // namespace mega::io

#endif // FILE_HEADER_12_MAY_2022