#include "application/test/fixtures/app_test_parser_fixture.hpp"

#include <boost/test/unit_test.hpp>

#include <string>
#include <optional>
#include <std_fs>

/*------------------------------------------------------------------------------

TEST PLAN:

1. Argument with value
2. Argument only with default value
3. Argument with value and default value
4. Unrecognized argument
5. Help
6. Value with "
7. Value with '
8. Several arguments
9. Check exist argument
10. Array
	10.1 simple
	10.2 with default value
	10.3 values with ' and ""
11. Int
12. Bool
13. Path
14. Paths

------------------------------------------------------------------------------*/

namespace application::test {

//------------------------------------------------------------------------------

BOOST_FIXTURE_TEST_SUITE(ParserArgumentsTets, ParserArgumentsFixture)

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(t1_argument_with_value)
{
	// Init
	const std::string argumentName = "dir";
	const std::string argumentDescription = "";
	const std::string argumentValue = "test_dir";
	const std::string argumentDefaultValue = "";

	const std::string argument = "--" + argumentName + "=" + argumentValue;
	addArgument( argumentName, argumentDescription, argumentDefaultValue );

	// Run
	parseArguments( { argument } );

	// Check
	auto valueOpt = getArgumentStringValue( argumentName );

	BOOST_REQUIRE( valueOpt.has_value() );
	BOOST_CHECK_EQUAL( argumentValue, *valueOpt );

}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(t2_argument_with_only_default_value)
{
	// Init
	const std::string argumentName = "dir";
	const std::string argumentDescription = "";
	const std::string argumentDefaultValue = "test_dir";

	addArgument( argumentName, argumentDescription, argumentDefaultValue );

	// Run
	parseArguments( { "" } );

	// Check
	auto valueOpt = getArgumentStringValue( argumentName );
	BOOST_REQUIRE( !valueOpt.has_value() );

}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(t3_argument_with_value_and_default_value)
{
	// Init
	const std::string argumentName = "dir";
	const std::string argumentDescription = "";
	const std::string argumentDefaultValue = "test_dir";
	const std::string argumentValue = "new_dir";

	const std::string argument = "--" + argumentName + "=" + argumentValue;
	addArgument( argumentName, argumentDescription, argumentDefaultValue );

	// Run
	parseArguments( { argument } );

	// Check
	auto valueOpt = getArgumentStringValue( argumentName );

	BOOST_REQUIRE( valueOpt.has_value() );
	BOOST_CHECK_EQUAL( argumentValue, *valueOpt );

}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(t4_unrecognized_argument)
{
	// Init
	const std::string argumentName = "dir";
	const std::string argumentDescription = "";
	const std::string argumentValue = "new_dir";

	const std::string argument = "--" + argumentName + "=" + argumentValue;

	// Run
	BOOST_CHECK_THROW( parseArguments( { argument } ), std::logic_error );

}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(t5_help)
{
	// Init
	const std::string argumentName = "dir";
	const std::string argumentDescription = "Test dir";
	const std::string argumentDefaultValue = "test_dir";
	const std::string argumentValue = "new_dir";

	const std::string argument = "--" + argumentName + "=" + argumentValue;
	addArgument( argumentName, argumentDescription, argumentDefaultValue );

	// Run

	BOOST_CHECK_EQUAL( getHelpString(),
		"\n"
		"Usage:\n"
		"   [OPTION...]\n"
		"\n"
		"      --dir=arg  Test dir (default: test_dir)\n"
	);

}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(t6_value_with_quotation_mark)
{
	// Init
	const std::string argumentName = "dir";
	const std::string argumentDescription = "";
	const std::string argumentValue = "new_dir";
	const std::string argumentDefaultValue = "";

	const std::string argument =
		"--" + argumentName + "=\"" + argumentValue + "\"";

	addArgument( argumentName, argumentDescription, argumentDefaultValue );

	// Run
	parseArguments( { argument } );

	// Check

	// Check
	auto valueOpt = getArgumentStringValue( argumentName );

	BOOST_REQUIRE( valueOpt.has_value() );
	BOOST_CHECK_EQUAL( argumentValue, *valueOpt );

}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(t7_value_with_apostrophe)
{
	// Init
	const std::string argumentName = "dir";
	const std::string argumentDescription = "";
	const std::string argumentValue = "new_dir";
	const std::string argumentDefaultValue = "";

	const std::string argument =
		"--" + argumentName + "='" + argumentValue + "'";

	addArgument( argumentName, argumentDescription, argumentDefaultValue );


	// Run
	parseArguments( { argument } );

	// Check
	auto valueOpt = getArgumentStringValue( argumentName );

	BOOST_REQUIRE( valueOpt.has_value() );
	BOOST_CHECK_EQUAL( argumentValue, *valueOpt );

}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(t8_several_arguments)
{
	// Init
	const std::string argumentNewName = "new";
	const std::string argumentNewDescription = "";
	const std::string argumentNewValue = "new_dir";
	const std::string argumentNewDefaultValue = "";
	const std::string argumentNew =
		"--" + argumentNewName + "=" + argumentNewValue;

	const std::string argumentOldName = "old";
	const std::string argumentOldDescription = "";
	const std::string argumentOldValue = "old_dir";
	const std::string argumentOldDefaultValue = "";
	const std::string argumentOld =
		"--" + argumentOldName + "=" + argumentOldValue;

	addArgument( argumentNewName, argumentNewDescription, argumentNewDefaultValue );
	addArgument( argumentOldName, argumentOldDescription, argumentOldDefaultValue );

	// Run
	parseArguments( { argumentNew, argumentOld } );

	// Check
	auto valueNewOpt = getArgumentStringValue( argumentNewName );

	BOOST_REQUIRE( valueNewOpt.has_value() );
	BOOST_CHECK_EQUAL( argumentNewValue, *valueNewOpt );

	auto valueOldOpt = getArgumentStringValue( argumentOldName );

	BOOST_REQUIRE( valueOldOpt.has_value() );
	BOOST_CHECK_EQUAL( argumentOldValue, *valueOldOpt );

}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(t9_check_exist_argument)
{
	// Init
	const std::string argumentName = "help";
	const std::string argumentDescription = "";
	const std::string argument = "--" + argumentName;

	addArgument( argumentName, argumentDescription );

	// Run
	parseArguments( { argument } );

	// Check
	BOOST_CHECK( isExistArgument( argumentName ) );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(t10_1_array_simple)
{
	// Init
	const std::string argumentName = "array";
	const std::string argumentDescription = "";
	const std::string argumentValue = "a,b,c";
	const Strings argumentDefaultValues{};

	addArgument( argumentName, argumentDescription, argumentDefaultValues );

	const std::string argument = "--" + argumentName + "=" + argumentValue;

	// Run
	parseArguments( { argument } );

	// Check

	auto valueOpt = getArgumentStringsValue( argumentName );

	BOOST_REQUIRE( valueOpt.has_value() );
	BOOST_CHECK_EQUAL( argumentValue, *valueOpt );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(t10_2_array_with_default_value)
{
	// Init
	const std::string argumentName = "array";
	const std::string argumentDescription = "";
	const Strings argumentDefaultValues{ "a", "b", "c" };

	addArgument( argumentName, argumentDescription, argumentDefaultValues );

	const std::string argument = "";

	// Run
	parseArguments( { argument } );

	// Check

	auto valueOpt = getArgumentStringValue( argumentName );

	BOOST_REQUIRE( !valueOpt.has_value() );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(t10_3_array_with_quotes)
{
	// Init
	const std::string argumentName = "array";
	const std::string argumentDescription = "";
	const std::string argumentValue = "'a',\"b\",c";
	const Strings argumentDefaultValues{};

	addArgument( argumentName, argumentDescription, argumentDefaultValues );

	const std::string argument = "--" + argumentName + "=" + argumentValue;

	// Run
	parseArguments( { argument } );

	// Check

	auto valueOpt = getArgumentStringsValue( argumentName );

	BOOST_REQUIRE( valueOpt.has_value() );
	BOOST_CHECK_EQUAL( "a,b,c", *valueOpt );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(t11_int)
{
	// Init
	const std::string argumentName = "number";
	const std::string argumentDescription = "";
	const int argumentValue = 42;
	const int argumentDefaultValue = 1;

	const std::string argument =
		"--" + argumentName + "=" + std::to_string( argumentValue );

	addArgument( argumentName, argumentDescription, argumentDefaultValue );

	// Run
	parseArguments( { argument } );

	// Check
	auto valueOpt = getArgumentIntValue( argumentName );

	BOOST_REQUIRE( valueOpt.has_value() );
	BOOST_CHECK_EQUAL( argumentValue, *valueOpt );

}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(t12_bool)
{
	// Init
	const std::string argumentName = "skip";
	const std::string argumentDescription = "";
	const bool argumentValue = true;
	const bool argumentDefaultValue = false;

	const std::string argument =
		"--" + argumentName + "=" + ( argumentValue ? "true" : "false" );

	addArgument( argumentName, argumentDescription, argumentDefaultValue );

	// Run
	parseArguments( { argument } );

	// Check
	auto valueOpt = getArgumentBoolValue( argumentName );

	BOOST_REQUIRE( valueOpt.has_value() );
	BOOST_CHECK_EQUAL( argumentValue, *valueOpt );

}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(t13_path)
{
	// Init
	const std::string argumentName = "number";
	const std::string argumentDescription = "";
	const Path argumentValue{ "/dir" };
	const std::string argumentValueStr{ argumentValue.string() };
	const Path argumentDefaultValue{ "/usr" };

	const std::string argument = "--" + argumentName + "=" + argumentValueStr;

	addArgument( argumentName, argumentDescription, argumentDefaultValue );

	// Run
	parseArguments( { argument } );

	// Check
	auto valueOpt = getArgumentPathValue( argumentName );

	BOOST_REQUIRE( valueOpt.has_value() );
	BOOST_CHECK_EQUAL( argumentValueStr, *valueOpt );

}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(t14_paths)
{
	// Init
	const std::string argumentName = "number";
	const std::string argumentDescription = "";
	const Paths argumentValues{ "/dir1" , "/dir2" , "/dir3" };
	const std::string argumentValueStr{ toString( argumentValues ) };
	const Paths argumentDefaultValues{ "/usr1" , "/usr2", "/usr3" };

	const std::string argument = "--" + argumentName + "=" + argumentValueStr;

	addArgument( argumentName, argumentDescription, argumentDefaultValues );

	// Run
	parseArguments( { argument } );

	// Check
	auto valueOpt = getArgumentPathsValue( argumentName );

	BOOST_REQUIRE( valueOpt.has_value() );
	BOOST_CHECK_EQUAL( argumentValueStr, *valueOpt );

}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE_END()

//------------------------------------------------------------------------------

}


