#include "parser/test/fixtures/pr_test_file_wrapper.hpp"
#include "parser/test/fixtures/pr_test_fixture.hpp"

#include "parser/api/pr_include_file.hpp"

#include "test_tools/test_macros.hpp"

/*------------------------------------------------------------------------------

TEST PLAN:

1.  Simple system include
2.  Simple user include
3.  System and user includes
4.  Include without space
5.  Include with several spaces
6.  Include after comment
7.  Include in multiline comment
8.  In string
9.  Include in multiline comment with sever lines
10. Include after multiline comment with sever lines
11. Include in multiline string
12. First line \n
13. String with quotation marks
14. Raw string
15. Raw string with separator
16. Brackets in comments
17. File name in comments

------------------------------------------------------------------------------*/

namespace parser::test {

//------------------------------------------------------------------------------
// clazy:excludeall=non-pod-global-static
TEST_GROUP_NAME( ParserTests, ParserFixture )

//------------------------------------------------------------------------------

TEST_CASE( t1_simple_system_include )
{
	// Init
	file() << "#include <iostream>";

	// Run
	parse();

	// Check
	const auto & files = getResults();
	BOOST_REQUIRE_EQUAL( files.size(), 1 );

	const auto & file = files.at( 0 );
	BOOST_CHECK_EQUAL( file.getName(), "iostream" );
	BOOST_CHECK_EQUAL( file.isSystem(), true );

	const auto & location = file.getLocation();
	BOOST_CHECK_EQUAL( location.getLineNumber(), 1 );
	BOOST_CHECK_EQUAL( location.getBegin(), 11 );
	BOOST_CHECK_EQUAL( location.getEnd(), 19 );
}

//------------------------------------------------------------------------------

TEST_CASE( t2_simple_user_include )
{
	// Init
	file() << "#include \"user.hpp\"";

	// Run
	parse();

	// Check
	const auto & files = getResults();
	BOOST_REQUIRE_EQUAL( files.size(), 1 );

	const auto & file = files.at( 0 );
	BOOST_CHECK_EQUAL( file.getName(), "user.hpp" );
	BOOST_CHECK_EQUAL( file.isSystem(), false );
}

//------------------------------------------------------------------------------

TEST_CASE( t3_simple_user_and_system_include )
{
	// Init
	file()
		<< "#include \"user.hpp\""
		<< "#include <iostream>"
	;

	// Run
	parse();

	// Check
	const auto & files = getResults();
	BOOST_REQUIRE_EQUAL( files.size(), 2 );

	const auto & fileUser = files.at( 0 );
	BOOST_CHECK_EQUAL( fileUser.getName(), "user.hpp" );
	BOOST_CHECK_EQUAL( fileUser.isSystem(), false );

	const auto & fileSystem = files.at( 1 );
	BOOST_CHECK_EQUAL( fileSystem.getName(), "iostream" );
	BOOST_CHECK_EQUAL( fileSystem.isSystem(), true );
}

//------------------------------------------------------------------------------

TEST_CASE( t4_include_without_space )
{
	// Init
	file() << "#include<iostream>";

	// Run
	parse();

	// Check
	const auto & files = getResults();
	BOOST_REQUIRE_EQUAL( files.size(), 1 );

	const auto & fileSystem = files.at( 0 );
	BOOST_CHECK_EQUAL( fileSystem.getName(), "iostream" );
	BOOST_CHECK_EQUAL( fileSystem.isSystem(), true );
}

//------------------------------------------------------------------------------

TEST_CASE( t5_include_with_several_spaces )
{
	// Init
	file()<<
		"#include                     <iostream>"
	;

	// Run
	parse();

	// Check
	const auto & files = getResults();
	BOOST_REQUIRE_EQUAL( files.size(), 1 );

	const auto & file = files.at( 0 );
	BOOST_CHECK_EQUAL( file.getName(), "iostream" );
	BOOST_CHECK_EQUAL( file.isSystem(), true );
}

//------------------------------------------------------------------------------

TEST_CASE( t6_include_after_comment )
{
	// Init
	file() << " //#include <iostream>";

	// Run
	parse();

	// Check
	const auto & files = getResults();
	BOOST_REQUIRE_EQUAL( files.size(), 0 );
}

//------------------------------------------------------------------------------

TEST_CASE( t7_include_in_multiline_coment )
{
	// Init
	file() << "/* include <memory> */";

	// Run
	parse();

	// Check
	const auto & files = getResults();
	BOOST_REQUIRE_EQUAL( files.size(), 0 );
}

//------------------------------------------------------------------------------

TEST_CASE( t8_include_in_string )
{
	// Init
	file() << "\" include <memory> \"";

	// Run
	parse();

	// Check
	const auto & files = getResults();
	BOOST_REQUIRE_EQUAL( files.size(), 0 );
}

//------------------------------------------------------------------------------

TEST_CASE( t9_include_in_multiline_coment_with_several_lines )
{
	// Init
	file() <<
		R"(/* text
		line #include "...".
		*/
		)"
	;

	// Run
	parse();

	// Check
	const auto & files = getResults();
	BOOST_REQUIRE_EQUAL( files.size(), 0 );
}

//------------------------------------------------------------------------------

TEST_CASE( t10_include_after_multiline_coment_with_several_lines )
{
	// Init
	file()<<
		"/* text \n"
		"*/\n"
		"#include <iostream>"
	;

	// Run
	parse();

	// Check
	const auto & files = getResults();
	BOOST_REQUIRE_EQUAL( files.size(), 1 );

	const auto & file = files.at( 0 );
	BOOST_CHECK_EQUAL( file.getName(), "iostream" );
	BOOST_CHECK_EQUAL( file.isSystem(), true );
}

//------------------------------------------------------------------------------

TEST_CASE( t11_include_include_in_multiline_string )
{
	// Init
	file()<<
		"\" text\\\n"
		" line #include <iostream>\\\n"
		"\""
	;

	// Run
	parse();

	// Check
	const auto & files = getResults();
	BOOST_REQUIRE_EQUAL( files.size(), 0 );
}

//------------------------------------------------------------------------------

TEST_CASE( t12_first_line_new_line )
{
	// Init
	file() <<
		R"(
		#include <boost/test/unit_test.hpp>
		)"
	;

	// Run
	parse();

	// Check
	const auto & files = getResults();
	BOOST_REQUIRE_EQUAL( files.size(), 1 );

	const auto & file = files.at( 0 );
	BOOST_CHECK_EQUAL( file.getName(), "boost/test/unit_test.hpp" );
	BOOST_CHECK_EQUAL( file.isSystem(), true );

	const auto & location = file.getLocation();
	BOOST_CHECK_EQUAL( location.getLineNumber(), 2 );
	BOOST_CHECK_EQUAL( location.getBegin(), 13 );
	BOOST_CHECK_EQUAL( location.getEnd(), 37 );
}

//------------------------------------------------------------------------------

TEST_CASE( t13_string_with_quotation_marks )
{
	// Init
	file() << R"(\" \"#include <iostream> ")";

	// Run
	parse();

	// Check
	const auto & files = getResults();
	BOOST_REQUIRE_EQUAL( files.size(), 0 );
}

//------------------------------------------------------------------------------

TEST_CASE( t14_raw_string )
{
	// Init
	file()<<
		" R\"( \n"
		"#include <lib1>"
		")\" \n"
		"#include <lib2>"
	;

	// Run
	parse();

	// Check
	const auto & files = getResults();
	BOOST_REQUIRE_EQUAL( files.size(), 1 );
}

//------------------------------------------------------------------------------

TEST_CASE( t15_raw_string_with_seperator )
{
	// Init
	file()<<
		" R\"( \n"
		"some text ) #include <lib1>"
		")\" \n"
		"#include <lib2>"
	;

	// Run
	parse();

	// Check
	const auto & files = getResults();
	BOOST_REQUIRE_EQUAL( files.size(), 1 );
}

//------------------------------------------------------------------------------

TEST_CASE( t16_brackets_in_comments )
{
	// Init
	file() <<
		R"(
		#include "file.hpp" //  <->
		)"
	;

	// Run
	parse();

	// Check
	const auto & files = getResults();
	BOOST_REQUIRE_EQUAL( files.size(), 1 );

	const auto & file = files.at( 0 );
	BOOST_CHECK_EQUAL( file.getName(), "file.hpp" );
	BOOST_CHECK_EQUAL( file.isSystem(), false );

	const auto & location = file.getLocation();
	BOOST_CHECK_EQUAL( location.getLineNumber(), 2 );
	BOOST_CHECK_EQUAL( location.getBegin(), 13 );
	BOOST_CHECK_EQUAL( location.getEnd(), 21 );
}

//------------------------------------------------------------------------------

TEST_CASE( t17_file_name_in_comments )
{
	// Init
	file() <<
		R"(
		#include /*<text1>*/ "filename1"
		#include /*"text2"*/ <filename2>
		)"
	;

	// Run
	parse();

	// Check
	const auto & files = getResults();
	BOOST_REQUIRE_EQUAL( files.size(), 2 );

	{
		const auto & file1 = files.at( 0 );
		BOOST_CHECK_EQUAL( file1.getName(), "filename1" );
		BOOST_CHECK_EQUAL( file1.isSystem(), false );

		const auto & location1 = file1.getLocation();
		BOOST_CHECK_EQUAL( location1.getLineNumber(), 2 );
		BOOST_CHECK_EQUAL( location1.getBegin(), 25 );
		BOOST_CHECK_EQUAL( location1.getEnd(), 34 );
	}
	{
		const auto & file2 = files.at( 1 );
		BOOST_CHECK_EQUAL( file2.getName(), "filename2" );
		BOOST_CHECK_EQUAL( file2.isSystem(), true );

		const auto & location2 = file2.getLocation();
		BOOST_CHECK_EQUAL( location2.getLineNumber(), 3 );
		BOOST_CHECK_EQUAL( location2.getBegin(), 25 );
		BOOST_CHECK_EQUAL( location2.getEnd(), 34 );
	}
}

//------------------------------------------------------------------------------

TEST_GROUP_END

//------------------------------------------------------------------------------

}
