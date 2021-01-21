#include "application/test/fixtures/app_test_report_settings_loader_fixture.hpp"

#include <boost/test/unit_test.hpp>

/*------------------------------------------------------------------------------

TEST PLAN:

1. Empty
2. Load settings
	2.1 max files
		2.1.1 from arguments
		2.1.2 from configuration file
		2.1.3 mix
	2.2 max details
		2.2.1 from arguments
		2.2.2 from configuration file
		2.2.3 mix
	3.3 show std files
		2.3.1 from arguments
		2.3.2 from configuration file
		2.3.3 mix
3. Load report kinds
	3.1 From arguments
	3.2 From configuration file

------------------------------------------------------------------------------*/

namespace application::test {

//------------------------------------------------------------------------------

BOOST_FIXTURE_TEST_SUITE(ReportSettingsLoaderTests, ReportSettingsLoaderFixture)

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(t1_empty_arguments_and_configuration_file)
{
	//Run
	parserArguments( "" );
	load();

	//Check
	BOOST_CHECK_EQUAL( getMaxFilesCount(),		getDefaultMaxFilesCount() );
	BOOST_CHECK_EQUAL( getMaxDetailsCount(),	getDefaultMaxDetailsCount() );
	BOOST_CHECK_EQUAL( getShowStdFiles(),		getDefaultShowStdFiles() );
	BOOST_CHECK_EQUAL( getReports(),			getDefaultReports() );

}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(t2_1_1_load_max_files_from_arguments)
{
	//Run
	parserArguments( "--report_limit=42" );
	load();

	//Check
	BOOST_CHECK_EQUAL( getMaxFilesCount(), 42 );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(t2_1_2_load_max_files_from_configuration_file)
{
	//Run
	createConfigurationFile( R"({ "report_limit" : 42 })" );
	load();

	//Check
	BOOST_CHECK_EQUAL( getMaxFilesCount(), 42 );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(t2_1_3_load_max_files_from_mix)
{
	//Run
	createConfigurationFile( R"({ "report_limit" : 1 })"  );
	parserArguments( "--report_limit=2" );
	load();

	//Check
	BOOST_CHECK_EQUAL( getMaxFilesCount(), 2);
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(t2_2_1_load_max_details_from_arguments)
{
	//Run
	parserArguments( "--report_details_limit=42" );
	load();

	//Check
	BOOST_CHECK_EQUAL( getMaxDetailsCount(), 42 );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(t2_2_2_load_max_details_from_configuration_file)
{
	//Run
	createConfigurationFile( R"({ "report_details_limit" : 42 })" );
	load();

	//Check
	BOOST_CHECK_EQUAL( getMaxDetailsCount(), 42 );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(t2_2_2_load_max_details_from_mix)
{
	//Run
	createConfigurationFile( R"({ "report_details_limit" : 1 })" );
	parserArguments( "--report_details_limit=2" );
	load();

	//Check
	BOOST_CHECK_EQUAL( getMaxDetailsCount(), 2 );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(t2_3_1_load_show_std_files_from_arguments)
{
	//Run
	parserArguments( "--show_std_files=true" );
	load();

	//Check
	BOOST_CHECK_EQUAL( getShowStdFiles(), true );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(t2_3_2_load_show_std_files_from_configuration_file)
{
	//Run
	createConfigurationFile( R"({ "show_std_files" : true })" );
	load();

	//Check
	BOOST_CHECK_EQUAL( getShowStdFiles(), true );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(t2_3_3_load_show_std_files_from_mix)
{
	//Run
	createConfigurationFile( R"({ "show_std_files" : true })" );
	parserArguments( "--show_std_files=false" );
	load();

	//Check
	BOOST_CHECK_EQUAL( getShowStdFiles(), false );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(t3_3_1_load_reports_from_arguments)
{
	//Run
	parserArguments( "--report=most_impact,unresolved" );
	load();

	//Check
	BOOST_CHECK_EQUAL( getReports(), "most_impact,unresolved" );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(t3_3_2_load_reports_from_configuration_file)
{
	//Run
	createConfigurationFile( R"({ "report" : [ "most_impact","unresolved" ] })" );
	load();

	//Check
	BOOST_CHECK_EQUAL( getReports(), "most_impact,unresolved" );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(t3_3_3_load_reports_from_mix)
{
	//Run
	createConfigurationFile( R"({ "report" : [ "most_impact" ] })" );
	parserArguments( "--report=unresolved" );
	load();

	//Check
	BOOST_CHECK_EQUAL( getReports(), "unresolved" );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE_END()

//------------------------------------------------------------------------------

}
