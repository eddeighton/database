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

// TestRunner.cpp : Defines the entry point for the console application.
//

#include "../edsUnitTestWrapper.hpp"

#include <gtest/gtest.h>

#include <boost/program_options.hpp>
#include <boost/stacktrace.hpp>

#include <iostream>
#include <memory>
#include <optional>

#include "common/backtrace.hpp"

void on_terminate()
{
    std::cout << "terminate called..." << std::endl;
    std::cout << boost::stacktrace::stacktrace();
    std::abort();
}

int inner_main(int argc, const char* argv[])
{
    std::set_terminate( on_terminate );

    common::disableDebugErrorPrompts();

    std::string strFilter, strXSL;
    int iRepeats = 1;
    bool bWait = false, bDebug = false, bReport = false, bCOut = false, bBreak = false;

    namespace bpo = boost::program_options;
    boost::program_options::options_description desc("Allowed options");
    desc.add_options()
        ("help", "produce help message")
        ("filter",      bpo::value< std::string >( &strFilter ), "filter string to select subset of tests")
        ("gtest_filter",bpo::value< std::string >( &strFilter ), "filter string to select subset of tests")
        ("xsl",         bpo::value< std::string >( &strXSL ),    "xsl file to generate html report")
        ("repeats",     bpo::value< int >( &iRepeats ),          "number of times to repeat tests")
        ("report",      bpo::value< bool >( &bReport )->implicit_value( true ),          "generate xml report")
        ("wait",        bpo::value< bool >( &bWait )->implicit_value( true ),            "wait once complete to keep terminal open")
        ("debug",       bpo::value< bool >( &bDebug )->implicit_value( true ),           "enable debugging")
        ("break",       bpo::value< bool >( &bBreak )->implicit_value( true ),           "break at startup to enable attaching debugger" )
        ("cout",        bpo::value< bool >( &bCOut )->implicit_value( true ),            "display standard output")
    ;

    boost::program_options::variables_map vm;
    boost::program_options::store( boost::program_options::parse_command_line( argc, argv, desc), vm);
    boost::program_options::notify( vm );

    if ( vm.count("help") ) {
        std::cout << desc << "\n";
        return 1;
    }

    if( bBreak )
    {
        char c;
        std::cin >> c;
    }

    int szResult = 0U;

    iRepeats = std::max< int >( 1, iRepeats );

    std::unique_ptr< EDUTS::UnitTestResultWrapper > results;
    try
    {
        EDUTS::UnitTestWrapper test( EDUTS::UnitTestOptions( bDebug, bReport, iRepeats, strFilter.c_str(), strXSL.c_str() ) );
        szResult = test.run();
        results = test.getResult();
    }
    catch( std::runtime_error& e )
    {
        std::cout << "Encountered exception: " << e.what() << std::endl;
        szResult = 1;
    }
    catch( ... )
    {
        std::cout << "Encountered unknown exception" << std::endl;
        szResult = 1;
    }

    //wait for UnitTestWrapper to restore standard output before we print
    if( bCOut && results.get() )
    {
        std::cout << "Settings" << "\n";
        while( const char* pszReportLine = results->getSettings() )
            std::cout << pszReportLine << "\n";
        std::cout << "\nReport" << "\n";
        while( const char* pszReportLine = results->getReport() )
            std::cout << pszReportLine;
        std::cout << "\nStandard Output" << "\n";
        while( const char* pszReportLine = results->getStandardOut() )
            std::cout << pszReportLine << "\n";
        std::cout << "\nStandard Error" << "\n";
        while( const char* pszReportLine = results->getStandardErr() )
            std::cout << pszReportLine << "\n";
    }

    if( bWait )
    {
        char cWait = ' ';
        std::cin >> cWait;
    }

	return szResult;
}

int main(int argc, const char* argv[])
{
    try
    {
        return inner_main( argc, argv );
    }
    catch( std::exception& ex )
    {
        std::cerr << "Exception calling main: " << ex.what() << std::endl;
        return 1;
    }
    catch( ... )
    {
        std::cerr << "Unknown exception calling main" << std::endl;
        return 1;
    }
}
