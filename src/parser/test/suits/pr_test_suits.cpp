#include "parser/test/fixtures/pr_test_fixture.hpp"
#include "parser/test/fixtures/pr_test_file_wrapper.hpp"

#include "parser/api/pr_include_file.hpp"

#include <boost/test/unit_test.hpp>

/*------------------------------------------------------------------------------

TEST PLAN:

1 Correct cases
	1. Simple system include
	2. Simple user include
	3. System and user includes
	4. Include without space
	5. Include with several spaces
	6. Include after comment
	7. Include in multi line comment
	8. In string
2. Incorrect cases
	1  include without " or <
	2  Include is not closed

------------------------------------------------------------------------------*/

namespace parser::test {

//------------------------------------------------------------------------------

BOOST_FIXTURE_TEST_SUITE(ParserTests, ParserFixture)

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(simple_system_include_1_1)
{
	// Init
	file()<<
		"#include <iostream>"
	;

	// Run
	parse();

	// Check
	const auto & files = getResults();
	BOOST_REQUIRE_EQUAL( files.size(), 1 );

	const auto & file = files.at( 0 );
	BOOST_CHECK_EQUAL( file.getName(), "iostream");
	BOOST_CHECK_EQUAL( file.isSystem(), true );

	const auto & location = file.getLocation();
	BOOST_CHECK_EQUAL( location.getLineNumber(), 1 );
	BOOST_CHECK_EQUAL( location.getBegin(), 11 );
	BOOST_CHECK_EQUAL( location.getEnd(), 19 );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(simple_user_include_1_2)
{
	// Init
	file()<<
		"#include \"user.hpp\""
	;

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

BOOST_AUTO_TEST_CASE(simple_user_and_system_include_1_3)
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

BOOST_AUTO_TEST_CASE(include_without_space_1_4)
{
	// Init
	file()<<
		"#include<iostream>"
	;

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

BOOST_AUTO_TEST_CASE(include_with_several_spaces_1_5)
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

BOOST_AUTO_TEST_CASE(include_after_comment_1_6)
{
	// Init
	file()<<
		" //#include <iostream>"
	;

	// Run
	parse();

	// Check
	const auto & files = getResults();
	BOOST_REQUIRE_EQUAL( files.size(), 0 );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(include_in_multiline_coment_1_7)
{
	// Init
	file()<<
		"/* include <memory> */"
	;

	// Run
	parse();

	// Check
	const auto & files = getResults();
	BOOST_REQUIRE_EQUAL( files.size(), 0 );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(include_in_string_1_8)
{
	// Init
	file()<<
		"\" include <memory> \""
	;

	// Run
	parse();

	// Check
	const auto & files = getResults();
	BOOST_REQUIRE_EQUAL( files.size(), 0 );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE_END()

//------------------------------------------------------------------------------

}
