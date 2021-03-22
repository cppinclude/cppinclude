#include "test_tools/boost_macroses.hpp"

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define BOOST_TEST_MODULE project_test
#include <boost/test/unit_test.hpp>

int main( int argc, char * argv[] )
{
	return boost::unit_test::unit_test_main( &init_unit_test, argc, argv );
}
