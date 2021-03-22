#include "cmake_project/test/fixtures/cprj_test_includes_parser_fixture.hpp"

#include "test_tools/test_macros.hpp"

#include <string>

/*------------------------------------------------------------------------------

TEST PLAN
1. Without include parameter
2. Parameter in name
3. One parameter
4. Several parameters
5. Parameter in end of string
6. File name with space
7. Include with space
8. Double spaces

------------------------------------------------------------------------------*/

namespace cmake_project::test {

//------------------------------------------------------------------------------
// clazy:excludeall=non-pod-global-static
TEST_GROUP_NAME( IncldesParserTests, IncldesParserFixture )

//------------------------------------------------------------------------------

TEST_CASE( t1_without_include_paramter )
{
	// Run
	auto actualResult = parseString( "/usr/bin/c++ -o file2.cpp.o -c file2.cpp" );
	Strings exceptedResult{};

	// Check
	TEST_CHECK_EQUAL_COLLECTIONS(
		actualResult.begin(),
		actualResult.end(),
		exceptedResult.begin(),
		exceptedResult.end()
	);

}

//------------------------------------------------------------------------------

TEST_CASE( t2_paramter_in_name )
{
	// Run
	auto actualResult = parseString( "/bin/c++ -o f-I.cpp.o -c f-I.cpp" );
	Strings exceptedResult{};

	// Check
	TEST_CHECK_EQUAL_COLLECTIONS(
		actualResult.begin(),
		actualResult.end(),
		exceptedResult.begin(),
		exceptedResult.end()
	);

}

//------------------------------------------------------------------------------

TEST_CASE( t3_one_paramter )
{
	// Run
	auto actualResult = parseString( "/bin/c++ -o f.cpp.o -I./include -c f.cpp" );
	Strings exceptedResult = toPathStrings( {"./include"} );

	// Check
	TEST_CHECK_EQUAL_COLLECTIONS(
		actualResult.begin(),
		actualResult.end(),
		exceptedResult.begin(),
		exceptedResult.end()
	);
}

//------------------------------------------------------------------------------

TEST_CASE( t4_several_parameters )
{
	// Run
	auto actualResult = parseString( "/bin/c++ -o f.cpp.o -I./lib1 -I./lib2 -c f.cpp" );
	Strings exceptedResult = toPathStrings( { "./lib1", "./lib2" } );

	// Check
	TEST_CHECK_EQUAL_COLLECTIONS(
		actualResult.begin(),
		actualResult.end(),
		exceptedResult.begin(),
		exceptedResult.end()
	);
}

//------------------------------------------------------------------------------

TEST_CASE( t5_paramter_in_end_of_string )
{
	// Run
	auto actualResult = parseString( "/bin/c++ -o f.cpp.o -c f.cpp -I./include" );
	Strings exceptedResult = toPathStrings( {"./include"} );

	// Check
	TEST_CHECK_EQUAL_COLLECTIONS(
		actualResult.begin(),
		actualResult.end(),
		exceptedResult.begin(),
		exceptedResult.end()
	);
}

//------------------------------------------------------------------------------

TEST_CASE( t6_file_with_space )
{
	// Run
	auto actualResult = parseString( R"(/bin/c++ -o f.cpp.o -c \"space in file\".cpp)" );
	Strings exceptedResult;

	// Check
	TEST_CHECK_EQUAL_COLLECTIONS(
		actualResult.begin(),
		actualResult.end(),
		exceptedResult.begin(),
		exceptedResult.end()
	);
}

//------------------------------------------------------------------------------

TEST_CASE( t7_include_with_space )
{
	// Run
	auto actualResult = parseString(
		R"(/bin/c++ -o f.cpp.o -I\"./include with space\" -c f.cpp)" );
	Strings exceptedResult = toPathStrings( { "./include with space" } );

	// Check
	TEST_CHECK_EQUAL_COLLECTIONS(
		actualResult.begin(),
		actualResult.end(),
		exceptedResult.begin(),
		exceptedResult.end()
	);
}

//------------------------------------------------------------------------------

TEST_CASE( t8_double_spaces )
{
	// Run
	auto actualResult =
		parseString( "/bin/c++  -I./include -o f.cpp.o -c f.cpp" );
	Strings exceptedResult = toPathStrings( { "./include" } );

	// Check
	TEST_CHECK_EQUAL_COLLECTIONS(
		actualResult.begin(),
		actualResult.end(),
		exceptedResult.begin(),
		exceptedResult.end()
	);
}

//------------------------------------------------------------------------------

TEST_GROUP_END

//------------------------------------------------------------------------------

}
