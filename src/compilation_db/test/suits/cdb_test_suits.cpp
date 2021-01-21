#include "compilation_db/test/fixtures/cdb_test_fixture.hpp"

#include "compilation_db/api/cdb_command_object.hpp"
#include "compilation_db/api/cdb_database.hpp"

#include <boost/test/unit_test.hpp>
#include <string_view>

/*------------------------------------------------------------------------------

TEST PLAN:

1. One compilation object
2. Sever compilation objects

------------------------------------------------------------------------------*/

namespace compilation_db::test {

//------------------------------------------------------------------------------

BOOST_FIXTURE_TEST_SUITE(CompilationDbTests, CompilationDbFixture)

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(t1_one_compilation_object)
{
	// Run
	const Database & db = parseJson(
	R"([
	{
		"directory": "/home/user/temp/project",
		"command": "/usr/bin/c++ -I/home/user/temp/project file.o file.cpp",
		"file": "file.cpp"
	}
	])"
	);

	// Check
	BOOST_REQUIRE_EQUAL( db.getCount(), 1 );
	{
		const CommandObject & object = db.getObject( 0 );
		BOOST_CHECK_EQUAL( object.getDirectory(), "/home/user/temp/project" );
		BOOST_CHECK_EQUAL( object.getFile(), "file.cpp" );
		BOOST_CHECK_EQUAL(
			object.getCommand(),
			"/usr/bin/c++ -I/home/user/temp/project file.o file.cpp"
		);
	}

}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(t2_several_compilation_objects)
{
	// Run
	const Database & db = parseJson(
	R"([
	{
		"directory": "/home/user/temp/project",
		"command": "/usr/bin/c++ -I/home/user/temp/project file1.o file1.cpp",
		"file": "file1.cpp"
	},
	{
		"directory": "/home/user/temp/project",
		"command": "/usr/bin/c++ -I/home/user/temp/project file2.o file2.cpp",
		"file": "file2.cpp"
	},
	{
		"directory": "/home/user/temp/project",
		"command": "/usr/bin/c++ -I/home/user/temp/project file3.o file3.cpp",
		"file": "file3.cpp"
	}
	])"
	);

	// Check
	BOOST_REQUIRE_EQUAL( db.getCount(), 3 );
	{
		const CommandObject & object = db.getObject( 0 );
		BOOST_CHECK_EQUAL( object.getDirectory(), "/home/user/temp/project" );
		BOOST_CHECK_EQUAL( object.getFile(), "file1.cpp" );
		BOOST_CHECK_EQUAL(
			object.getCommand(),
			"/usr/bin/c++ -I/home/user/temp/project file1.o file1.cpp"
		);
	}
	{
		const CommandObject & object = db.getObject( 1 );
		BOOST_CHECK_EQUAL( object.getDirectory(), "/home/user/temp/project" );
		BOOST_CHECK_EQUAL( object.getFile(), "file2.cpp" );
		BOOST_CHECK_EQUAL(
			object.getCommand(),
			"/usr/bin/c++ -I/home/user/temp/project file2.o file2.cpp"
		);
	}
	{
		const CommandObject & object = db.getObject( 2 );
		BOOST_CHECK_EQUAL( object.getDirectory(), "/home/user/temp/project" );
		BOOST_CHECK_EQUAL( object.getFile(), "file3.cpp" );
		BOOST_CHECK_EQUAL(
			object.getCommand(),
			"/usr/bin/c++ -I/home/user/temp/project file3.o file3.cpp"
		);
	}

}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE_END()

//------------------------------------------------------------------------------

}
