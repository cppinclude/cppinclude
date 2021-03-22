#include "model_includes/test/fixtures/mi_test_model_includes_fixture.hpp"

#include "model_includes/api/enums/mi_file_type.hpp"

#include "test_tools/test_macros.hpp"

#include <std_fs>

/*------------------------------------------------------------------------------

TEST PLAN:

1. Project file
2. Std Library File
3. Project file with std library name

------------------------------------------------------------------------------*/

namespace model_includes::test {

//------------------------------------------------------------------------------
// clazy:excludeall=non-pod-global-static
TEST_GROUP_NAME( ResolverFileTypeTests, ModelIncludesFixture )

//------------------------------------------------------------------------------

TEST_CASE( t1_project_file )
{
	// Init
	setProjectDir( "/test_project/" );
	addFileToProject( "main.cpp" );

	// Run
	const FileType fileType = resolveFileType( "/test_project/main.cpp" );

	// Check
	TEST_CHECK( checkFileType( fileType, FileType::ProjectFile ) );
}

//------------------------------------------------------------------------------

TEST_CASE( t2_std_library_file )
{
	// Init
	setProjectDir( "/test_project/" );
	addFileToProject( "main.cpp" );

	// Run
	const FileType fileType = resolveFileType( "iostream" );

	// Check
	TEST_CHECK( checkFileType( fileType, FileType::StdLibraryFile ) );
}

//------------------------------------------------------------------------------

TEST_CASE( t3_project_file_with_std_library_name )
{
	// Init
	setProjectDir( "/test_project/" );
	addFileToProject( "limits.h" );

	// Run
	const FileType fileType = resolveFileType( "/test_project/limit.h" );

	// Check
	TEST_CHECK( checkFileType( fileType, FileType::ProjectFile ) );
}

//------------------------------------------------------------------------------

TEST_GROUP_END

//------------------------------------------------------------------------------

}
