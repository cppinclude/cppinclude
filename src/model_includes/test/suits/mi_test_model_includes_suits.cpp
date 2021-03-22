#include "model_includes/test/fixtures/mi_test_model_includes_fixture.hpp"

#include "model_includes/test/fixtures/wrappers/mi_test_file_wrapper.hpp"
#include "model_includes/test/fixtures/wrappers/mi_test_model_wrapper.hpp"

#include "test_tools/test_macros.hpp"

/*------------------------------------------------------------------------------

TEST PLAN:

1. File
	1.1 Includes count recursive
	1.2 IncludedBy count recursive

------------------------------------------------------------------------------*/

namespace model_includes::test {

//------------------------------------------------------------------------------
// clazy:excludeall=non-pod-global-static
TEST_GROUP_NAME( ModelIncludesTests, ModelIncludesFixture )

//------------------------------------------------------------------------------

TEST_CASE( t1_1_file_includes_count_recursive )
{
	// Init
	const std::string baseFile = "/test_project/base.hpp";
	const std::string classAFile = "/test_project/classA.hpp";
	const std::string classBFile = "/test_project/classB.hpp";

	const std::string mainFile = "/test_project/main.cpp";

	// Run
	auto model = createModel();
	TEST_REQUIRE( model.isAvailable() );

	model.addInclude( classAFile, baseFile );
	model.addInclude( classBFile, baseFile );

	model.addInclude( mainFile, classAFile );
	model.addInclude( mainFile, classBFile );

	// Check

	auto file = model.findFile( mainFile );
	TEST_REQUIRE( file.isAvailable() );

	TEST_CHECK( file.checkIncludesCountRecursive( 3 ) );
}

//------------------------------------------------------------------------------

TEST_CASE( t1_2_file_includes_by_count_recursive )
{
	// Init
	const std::string baseFile = "/test_project/base.hpp";

	const std::string classAFile = "/test_project/classA.hpp";
	const std::string runA1File = "/test_project/runA1.hpp";

	const std::string classBFile = "/test_project/classB.hpp";
	const std::string runB1File = "/test_project/runB1.hpp";
	const std::string runB2File = "/test_project/runB2.hpp";

	const std::string mainFile = "/test_project/main.cpp";

	// Run
	auto model = createModel();
	TEST_REQUIRE( model.isAvailable() );

	model.addInclude( classAFile, baseFile );
	model.addInclude( runA1File, classAFile );

	model.addInclude( classBFile, baseFile );
	model.addInclude( runB1File, classBFile );
	model.addInclude( runB2File, classBFile );

	model.addInclude( mainFile, classAFile );
	model.addInclude( mainFile, classBFile );

	// Check

	auto file = model.findFile( baseFile );
	TEST_REQUIRE( file.isAvailable() );

	TEST_CHECK( file.checkIncludedByCountRecursive( 6 ) );
}

//------------------------------------------------------------------------------

TEST_GROUP_END

//------------------------------------------------------------------------------

}
