#include "model_includes/test/fixtures/mi_test_model_includes_fixture.hpp"

#include "model_includes/api/enums/mi_file_type.hpp"

#include <boost/test/unit_test.hpp>

#include <filesystem>

/*------------------------------------------------------------------------------

TEST PLAN:

1. Project file
2. Std Library File

------------------------------------------------------------------------------*/

namespace model_includes::test {

//------------------------------------------------------------------------------

BOOST_FIXTURE_TEST_SUITE(ResolverFileTypeTests, ModelIncludesFixture)

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(project_file_1)
{
	// Init
	setProjectDir( "/test_project/" );
	addFileToProject( "main.cpp" );

	// Run
	const FileType fileType = resolveFileType( "/test_project/main.cpp" );

	// Check
	BOOST_CHECK( checkFileType( fileType, FileType::ProjectFile ) );

}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(std_library_file_2)
{
	// Init
	setProjectDir( "/test_project/" );
	addFileToProject( "main.cpp" );

	// Run
	const FileType fileType = resolveFileType( "iostream" );

	// Check
	BOOST_CHECK( checkFileType( fileType, FileType::StdLibraryFile ) );

}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE_END()

//------------------------------------------------------------------------------

}
